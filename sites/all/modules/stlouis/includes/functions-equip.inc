<?php

/**
 * @file
 * Misc Equipment functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

/**
 * Show a piece of required equipment.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $equipment_id
 *   The equipment id.
 * @param int $req_quantity
 *   The quantity needed.
 */
function game_required_equipment(\stdClass $game_user, $equipment_id, $req_quantity = 1) {

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $eo = game_fetch_equip_by_id($game_user, $equipment_id);

  if ($eo->quantity >= $req_quantity) {
    $not_yet = $a_start = $a_end = '';
  }
  else {
    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/equipment_buy/' .
      $arg2 . '/' . $equipment_id . '/' .
      ($req_quantity - $eo->quantity) . '">';
    $a_end = '</a>';
  }

  echo <<< EOF
    <div class="quest-required_stuff">Requires
      <div class="quest-required_equipment">
        $a_start
          <img class="$not_yet" src="$eo->icon_path" width="48">
        $a_end
      </div>&nbsp;x$req_quantity
    </div>
EOF;
}

/**
 * Fetch the list of equipment to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass[]
 *   The array of equipment objects.
 */
function game_fetch_visible_equip(\stdClass $game_user) {
  if ($game_user->meta == 'admin') {
    $sql_to_add = 'WHERE equipment.id > 0';
  }
  else {
    $sql_to_add = 'WHERE (((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
    )
  
    AND
  
    (
      fkey_values_id = 0
      OR fkey_values_id = %d
    ))
  
      AND required_level <= %d
      AND active = 1
      AND is_loot = 0
    )
  
    OR equipment_ownership.quantity > 0 ';
  }

  $data = [];
  $sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  ' . $sql_to_add . '
  ORDER BY required_level ASC';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  while ($eq = db_fetch_object($result)) {
    game_get_icon_path($game_user, $eq, 'equipment');
    $data[] = $eq;
  }
  return $data;
}

/**
 * Fetch the next equipment item to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass
 *   The next equipment item to show, if any.
 */
function game_fetch_next_equip(\stdClass $game_user) {
  return zg_fetch_next_equip($game_user);
}

/**
 * Fetch the specified equipment item.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $equip_id
 *   The equip id.
 *
 * @return \stdClass
 *   The equipment item, if any.
 */
function game_fetch_equip_by_id(\stdClass $game_user, $equip_id) {
  $sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE equipment.id = %d;';
  $result = db_query($sql, $game_user->id, $equip_id);
  $eq = db_fetch_object($result);
  game_get_icon_path($game_user, $eq, 'equipment');
  return $eq;
}

/**
 * Show a piece of equipment.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $item
 *   The equipment item.
 * @param string $ai_output
 *   AI output.
 * @param array $options
 *   Options that modify the display behavior.
 *
 * @return string
 *   The rendered HTML.
 */
function game_show_equip(\stdClass $game_user, \stdClass $item, &$ai_output, $options = []) {
  $html = game_render_equip($game_user, $item, $ai_output, $options);
  print $html;
  return $html;
}

/**
 * Render (without showing) a piece of equipment.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $item
 *   The equipment item.
 * @param string $ai_output
 *   AI output.
 * @param array $options
 *   Options that modify the display behavior.
 *
 * @return string
 *   The rendered HTML.
 */
function game_render_equip(\stdClass $game_user, \stdClass $item, &$ai_output, array $options = []) {
  return zg_render_equip($game_user, $item, $ai_output, $options);
}

/**
 * Gain (buy) piece(s) of equipment for a price.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $id
 *   The equipment id.
 * @param int $quantity
 *   The quantity to gain.
 * @param int $eq_price
 *   The total price.
 *
 * @return array
 *   Whether the gain succeeded or not.
 */
function game_equipment_gain(\stdClass &$game_user, $id, $quantity = 1, $eq_price = 0) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  // BUG!  How do these get here?
  if ($id == 0) {
    mail('joseph@ziquid.com', $game_user->name . ' trying to get equipment 0!',
      $game_user->phone_id);
    return [FALSE, 'bad-id', 0];
  }

  // FIXME: refactor to use game_fetch_equip_by_id().
  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  // Check for enough income to cover upkeep.
  if ($game_user->income <
    $game_user->expenses + ($eq->upkeep * $quantity)) {
    return [FALSE, 'not-enough-income', ($eq->upkeep * $quantity)];
  }

  // Eo entry already there.
  if (!empty($eo)) {

    if (($eq->quantity_limit > 0) &&
      (($eo->quantity + $quantity) > $eq->quantity_limit)) {
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];
    }

    $sql = 'update equipment_ownership set quantity = quantity + %d
      where fkey_equipment_id = %d and fkey_users_id = %d;';
    db_query($sql, $quantity, $id, $game_user->id);
  }
  else {

    if (($eq->quantity_limit > 0) && ($quantity > $eq->quantity_limit)) {
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];
    }

    $sql = 'insert into equipment_ownership
      (fkey_users_id, fkey_equipment_id, quantity) values
      (%d, %d, %d);';
    db_query($sql, $game_user->id, $id, $quantity);
  }

  // Pay for the equipment, if needed.
  if ($eq_price != 0) {
    $sql = 'update users set money = money - %d where id = %d;';
    db_query($sql, $eq_price, $game_user->id);
  }

  // Give energy bonus, if needed.
  if ($eq->energy_bonus > 0) {

    // Start the energy clock, if needed.
    if ($game_user->energy == $game_user->energy_max) {
      $sql = 'update users set energy_next_gain = "%s" where id = %d;';
      db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 300),
        $game_user->id);
    }

    $sql = 'update users set energy = energy + %d where id = %d;';
    db_query($sql, $eq->energy_bonus * $quantity, $game_user->id);
  }

  // Reprocess user object.
  game_recalc_income($game_user);
  $game_user = zg_fetch_player_by_id($game_user->id);
  return [TRUE, 'success', 0];
}

/**
 * Lose (or Sell) one or more pieces of equipment.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $id
 *   The equipment ID.
 * @param int $quantity
 *   Number of pieces to lose/sell
 * @param int $eq_price
 *   Amount of compensation to receive for sale.
 *
 * @return bool
 *   Whether the sale succeeded or not.
 */
function game_equipment_lose(&$game_user, $id, $quantity, $eq_price = 0) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  // FIXME: refactor to use game_fetch_equip_by_id().
  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  // Nothing there?
  if (empty($eo)) {
    return FALSE;
  }

  // Not enough?
  if ($eo->quantity < $quantity) {
    return FALSE;
  }

  $sql = 'update equipment_ownership set quantity = quantity - %d
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  db_query($sql, $quantity, $id, $game_user->id);

  // Receive money for the equipment, if needed.
  // FIXME: don't process if money is 0.
  $sql = 'update users set money = money + %d where id = %d;';
  db_query($sql, $eq_price, $game_user->id);

  // Reprocess user object.
  game_recalc_income($game_user);
  $game_user = zg_fetch_player_by_id($game_user->id);
  return TRUE;
}

/**
 * Use one or more pieces equipment.
 *
 * Same as Losing/Selling except no compensation is made, and the quantity used
 * is tracked in the database.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $id
 *   The equipment ID.
 * @param int $quantity
 *   Number of pieces to lose/sell.
 *
 * @return bool
 *   Whether the sale succeeded or not.
 */
function game_equipment_use(&$game_user, $id, $quantity) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  if (game_equipment_lose($game_user, $id, $quantity, 0)) {
    $sql = 'update equipment_ownership set quantity_used = quantity_used + %d
    where fkey_equipment_id = %d and fkey_users_id = %d;';
    db_query($sql, $quantity, $id, $game_user->id);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function game_show_equipment(\stdClass $game_user, \stdClass $item, $options = []) {

  global $game, $phone_id;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $arg2 = check_plain(arg(2));
  static $party_title;

  $description = str_replace('%clan', "<em>$party_title</em>",
    $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) $quantity = '<em>None</em>';

  $equipment_price = $item->price + ($item->quantity *
    $item->price_increase);

  if (!$item->is_loot) $ai_output .= " $item->id=$equipment_price";

  if (($equipment_price % 1000) == 0)
    $equipment_price = ($equipment_price / 1000) . 'K';

  if ($equipment_price === '0K') {
    $equipment_price = 'Free';
  }
  else {
    $equipment_price .= ' ' . $game_user->values;
  }

  if ($item->quantity_limit > 0) {
    $quantity_limit = '<em>(Limited to ' . $item->quantity_limit . ')</em>';
  }
  else {
    $quantity_limit = '';
  }

  if (strpos($_SERVER['REQUEST_URI'], 'equipment') !== FALSE) {
    $link = 'equipment';
  }
  else {
    $link = 'weapons';
  }

  $soon = ($options['soon']) ? 'soon' : '';
  $clan = ($options['clan']) ? 'clan' : '';

  if ($soon) {
    $icon = "$game-soon.png";
    $a_start = $a_end = '';
  }
  else {
    $icon = "$game-$item->id.png";
    $a_start = "<a href=\"/$game/{$link}_buy/$arg2/$item->id/1\">";
    $a_end = '</a>';
  }

  // Cannot buy.
  if ($clan | $item->is_loot) {
    $a_start = $a_end = '';
  }

  echo <<< EOF
<div class="land">
  <div class="land-icon">
    $a_start
      <img src="/sites/default/files/images/equipment/$icon"
        border="0" width="96"/>
    $a_end
  </div>
  <div class="land-details">
    <div class="land-name">
      $a_start
        $item->name
      $a_end
    </div>
    <div class="land-description">
      $description
    </div>
EOF;

  if (empty($soon)) echo <<< EOF
    <div class="land-owned">
      Owned: $quantity $quantity_limit
    </div>
EOF;

  echo <<< EOF
    <div class="land-requisites">
      // Requisites //
    </div>
EOF;

  if ($item->required_level > $game_user->level) echo <<< EOF
    <div class="land-required_level">
      Player: Level $item->required_level
    </div>
EOF;

  echo <<< EOF
    <div class="land-cost">
      Cost: $equipment_price
    </div>
EOF;

  if ($item->energy_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      Energy: +$item->energy_bonus immediate energy bonus
    </div>
EOF;

  }

  if ($item->energy_increase > 0) {

    echo <<< EOF
    <div class="land-payout">
      Energy: +$item->energy_increase every 5 minutes
    </div>
EOF;

  }

  if ($item->initiative_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $initiative: +$item->initiative_bonus
    </div>
EOF;

  }

  if ($item->endurance_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $endurance: +$item->endurance_bonus
    </div>
EOF;

  }

  if ($item->endurance_bonus < 0) {

    echo <<< EOF
    <div class="land-payout negative">
      $endurance: $item->endurance_bonus
    </div>
EOF;

  }

  if ($item->elocution_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $elocution: +$item->elocution_bonus
    </div>
EOF;

  if (!$item->is_loot) $ai_output .= "/+elo:$item->elocution_bonus";

  }

  if ($item->speed_increase > 0) {

    echo <<< EOF
    <div class="land-payout">
      Speed Increase: $item->speed_increase fewer Action
        needed to move to a new $hood_lower
    </div>
EOF;

  if ((!$item->is_loot) && ($item->quantity == 0))
    $ai_output .= "/+spd:$item->speed_increase";

  }

  if ($item->upkeep > 0) {

    echo <<< EOF
    <div class="land-payout negative">
      Upkeep: $item->upkeep every 60 minutes
    </div>
EOF;

  }

  if ($item->chance_of_loss > 0) {

    $lifetime = floor(100 / $item->chance_of_loss);
    $use = ($lifetime == 1) ? 'use' : 'uses';
    echo <<< EOF
    <div class="land-payout negative">
      Expected Lifetime: $lifetime $use
    </div>
EOF;

  }

  // Grab each action for the equipment.
  $data2 = [];
  $sql = 'select * from actions where fkey_equipment_id = %d;';
  $result = db_query($sql, $item->id);

  while ($action = db_fetch_object($result)) $data2[] = $action;

  foreach ($data2 as $action) {
firep($action);

    $cost = "Cost: $action->cost Action";
    if ($action->values_cost > 0)
      $cost .= ", $action->values_cost $game_user->values";

    $name = str_replace('%value', $game_user->values, $action->name);

    echo <<< EOF
    <div class="land-action">
      Action: $name
    </div>
    <div class="land-description">
      $action->description
    </div>
    <div class="land-action-cost">
      $cost
    </div>
EOF;

    if ($action->influence_change < 0) {

      $inf_change = -$action->influence_change;

      echo <<< EOF
    <div class="land-payout negative">
       Effect: Target's {$experience_lower} is reduced by $inf_change
    </div>
EOF;

    }

    if (($action->rating_change < 0.10) && ($action->rating_change != 0.0)) {

      $rat_change = abs($action->rating_change);

      if ($action->rating_change < 0.0) {

        echo <<< EOF
    <div class="land-payout negative">
      Effect: $target approval rating is reduced by $rat_change%
    </div>
EOF;

      }
      else {

        echo <<< EOF
    <div class="land-payout">
      Effect: $target approval rating is increased by $rat_change%
    </div>
EOF;

      }

    }

    if ($action->rating_change >= 0.10) {

      $rat_change = $action->rating_change;

      echo <<< EOF
    <div class="land-payout">
      Effect: Your approval rating is increased by $rat_change%
    </div>
EOF;

    }

    if ($action->neighborhood_rating_change < 0.0) {

      $rat_change = -$action->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout negative">
      Effect: Neighborhood $beauty_lower rating is reduced by $rat_change
    </div>
EOF;

    }

    if ($action->neighborhood_rating_change > 0.0) {

      $rat_change = $action->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout">
      Effect: Neighborhood $beauty_lower rating is increased by $rat_change
    </div>
EOF;

    }

    if ($action->values_change < 0) {

      $val_change = -$action->values_change;

      echo <<< EOF
    <div class="land-payout negative">
      Effect: Target's $game_user->values is reduced by $val_change
    </div>
EOF;

    }

  }

  echo '</div>';

  if (empty($soon) && empty($clan)) {

    if (!$item->is_loot) {

      echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button">
      <a href="/$game/{$link}_buy/$arg2/$item->id/1">
        Buy
      </a>
    </div>
EOF;

    }
    else {

      echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button not-yet">
      Can't Buy
    </div>
EOF;

    }

    if ($item->can_sell) {

      echo <<< EOF
    <div class="land-sell-button">
      <a href="/$game/{$link}_sell/$arg2/$item->id/1">
        Sell
      </a>
    </div>
  </div>
EOF;

    }
    else {

      echo <<< EOF
    <div class="land-sell-button not-yet">
      Can't Sell
    </div>
  </div>
EOF;

    }
  }

  echo '</div>';
}
