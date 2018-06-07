<?php

/**
 * @file functions-staff.inc
 * Misc staff functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

/**
* Show a staff or agent.
*
* @param \stdClass $game_user
*   The game user object.
* @param \stdClass $item
*   The staff item.
* @param string $ai_output
*   AI output.
* @param array $options
*   Options that modify the display behavior.
*/
function game_show_staff($game_user, $item, &$ai_output, $options = []) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  static $party_title;
  $classes = [];

  if (empty($party_title)) {
    $sql = 'select party_title from `values` where id = %d;';
    $result = db_query($sql, $game_user->fkey_values_id);
    $data = db_fetch_object($result);
    $party_title = preg_replace('/^The /', '', $data->party_title);
  }

  $soon = ($options['soon']) ? 'soon' : '';
  $header = ($options['staff-succeeded'] == 'success') ?
  '<div class="land-succeeded">Success</div>' : '';

  switch ($options['staff-buy-succeeded']) {

    case 'failed no-money':
      $offer = game_luck_money_offer($game_user);
      $header = '<div class="title">' .

        // FIXME remove special characters from text here.
        t('∆ @value <span>Insufficient</span> ∆',
        array('@value' => $game_user->values)) . '
      </div>
      <div class="try-an-election-wrapper">
        <div class="try-an-election">
          <a href="/' . $game . '/elders_do_fill/' . $arg2 .
              '/money?destination=/' . $game . '/staff/' . $arg2 . '">' .
            t('Receive @offer @values (1&nbsp;@luck',
            array(
            '@offer' => $offer,
            '@values' => $game_user->values,
            '@luck' => $luck,
            )) . ')
          </a>
        </div>
      </div>';

      $failed = 'failed';
      break;

    case 'failed not-required-level':
      $header = '<div class="title">' .
        t('∆ Player Level <span>Insufficient</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed hit-quantity-limit':
      $header = '<div class="title">' .
        t('Limit reached!') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-enough-income':
      $header = '<div class="title">' .
        t('Not enough hourly income!') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-required-hood':
      $header = '<div class="title">' .
        t('∆ Location <span>Incorrect</span> ∆') .
        '</div>';
      $failed = 'failed';
      competency_gain($game_user, 'lost');
      break;

    case 'failed not-required-party':
      $header = '<div class="title">' .
        t('∆ @party <span>Incorrect</span> ∆',
        array('@party' => $party)) .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-active':
    case 'failed is-loot':
      $header = '<div class="title">' .
        t('∆ Item <span>Unavailable</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed cant-sell':
      $header = '<div class="title">' .
        t('∆ Item <span>Unsalable</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;
  }

  $description = str_replace('%party', "<em>$party_title</em>",
  $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) {
    $quantity = '<em>' . t('None') . '</em>';
  }

  $staff_price = $item->price + ($item->quantity *
  $item->price_increase);

  if (!$item->is_loot) {
    $ai_output .= " $item->id=$staff_price";
  }

  if ((($staff_price % 1000) == 0) && $staff_price > 0) {
    $staff_price = ($staff_price / 1000) . 'K';
  }

  if ($item->quantity_limit > 0) {
    $quantity_limit = '<em>(Limited to ' . $item->quantity_limit . ')</em>';
  }
  else {
    $quantity_limit = '';
  }

  $can_buy = $can_sell = TRUE;
  $cant_buy_reason = $cant_sell_reason = '';

  if ($item->fkey_neighborhoods_id != 0 &&
    $item->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Wrong ' . $game_text['hood_short'] . ')';
  }

  if ($item->fkey_values_id != 0 &&
    $item->fkey_values_id != $game_user->fkey_values_id) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Wrong ' . $party . ')';
  }

  if ($item->required_level > $game_user->level) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Not Enough ' . $experience . ')';
  }

  if (!$item->active) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Inactive)';
  }

  if ($item->is_loot != 0) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Loot Only)';
  }

  if ($item->can_sell != 1) {
    $can_sell = FALSE;
    $cant_sell_reason = '(Ever)';
  }

  if ($item->quantity < 1) {
    $can_sell = FALSE;
  }

  $inactive = ($item->active) ? '' : t('(Inactive)');

  $types = [
    'a' => 'Agent',
    's' => 'Staff',
  ];

  $item->type = $types[$item->staff_or_agent];

  if ($can_buy) {
    $icon_button = <<< EOF
      <a href="/$game/staff_hire/$arg2/$item->id/1">
        <img src="/sites/default/files/images/staff/$game-$item->id.png?2" width="96"
             border="0">
      </a>
EOF;
    $name_link = <<< EOF
      <a href="/$game/staff_hire/$arg2/$item->id/1">
        <span class="type">
          $inactive $item->type:
        </span>
        $item->name
      </a>
EOF;
    $buy_button = <<< EOF
      <div class="land-buy-button">
        <a href="/$game/staff_hire/$arg2/$item->id/1">
          Hire
        </a>
      </div>
EOF;
  }
  else {
  // Can't buy.
    $icon_button = <<< EOF
      <img src="/sites/default/files/images/staff/$game-$item->id.png?2" width="96"
           border="0">
EOF;
    $name_link = <<< EOF
      <span class="type">
        $inactive $item->type:
      </span>
      $item->name
EOF;
    $buy_button = <<< EOF
      <div class="land-buy-button not-yet">
        Can't Hire $cant_buy_reason
      </div>
EOF;
  }

  if ($can_sell) {
    $sell_button = <<< EOF
      <div class="land-sell-button">
        <a href="/$game/staff_fire/$arg2/$item->id/1">
          Fire
        </a>
      </div>
EOF;
  }
  else {
    $sell_button = <<< EOF
      <div class="land-sell-button not-yet">
        Can't Fire $cant_sell_reason
      </div>
EOF;
  }

  // grab each action for the staff
  $acts_arr = [];
  $sql = 'select * from actions where fkey_staff_id = %d;';
  $result = db_query($sql, $item->id);
  while ($action = db_fetch_object($result)) {
    $acts_arr[] = $action;
  }

  if ($item->initiative_bonus != 0) {
    $classes[] = 'staff-ini';
  }

  if ($item->endurance_bonus != 0) {
    $classes[] = 'staff-end';
  }

  if ($item->elocution_bonus != 0) {
    $classes[] = 'staff-elo';
  }

  if ($item->speed_increase != 0) {
    $classes[] = 'staff-move';
  }

  if ($can_buy) {
    $classes[] = 'staff-buy';
  }

  if ($can_sell) {
    $classes[] = 'staff-sal';
  }

  if ($item->upkeep != 0) {
    $classes[] = 'staff-upk';
  }

  if ($item->energy_bonus != 0 || $item->energy_increase != 0) {
    $classes[] = 'staff-eng';
  }

  if (count($acts_arr) > 0) {
    $classes[] = 'staff-act';
  }

  $classes[] = 'staff-' . drupal_strtolower($item->type);

  $class_str = implode(' ', $classes);

  if ($soon) {
echo <<< EOF
<div class="land $soon $failed $class_str">
  <div class="land-icon">
    <img src="/sites/default/files/images/staff/$game-soon.png?1" width=96
         border="0">
  </div>
  <div class="land-details">
    <div class="land-name">
      <span class="type">$inactive</span>
      $item->type: $item->name
    </div>
    <div class="land-description">$description</div>
EOF;
  }
  else {
    echo <<< EOF
    <div class="land $failed $class_str">
      $header
      <div class="land-icon">
        $icon_button
      </div>
      <div class="land-details">
        <div class="land-name">
          $name_link
        </div>
        <div class="land-description">$description</div>
EOF;
  }

  echo <<< EOF
    <div class="land-owned">Owned: $quantity $quantity_limit</div>
EOF;
  $reqs = <<< EOF
    <div class="land-cost">Cost: $staff_price $game_user->values</div>
EOF;

  if ($game_user->level < $item->required_level) {
    $reqs .= <<< EOF
      <div class="land-required_level">Player: Level $item->required_level</div>
EOF;
  }

  if (!empty($reqs)) {
    echo <<< EOF
      <!--<div class="land-requisites">// Requisites //</div>-->
      $reqs
EOF;
  }

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
  else if ($item->initiative_bonus < 0) {
    echo <<< EOF
      <div class="land-payout negative">
        $initiative: $item->initiative_bonus
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
  else if ($item->endurance_bonus < 0) {
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

    if (!$item->is_loot) {
      $ai_output .= "/+elo:$item->elocution_bonus";
    }
  }
  else if ($item->elocution_bonus < 0) {
    echo <<< EOF
      <div class="land-payout negative">
        $elocution: $item->elocution_bonus
      </div>
EOF;

    if (!$item->is_loot) {
      $ai_output .= "/+elo:$item->elocution_bonus";
    }
  }

  if ($item->speed_increase > 0) {
    echo <<< EOF
      <div class="land-payout">
        Speed Increase: $item->speed_increase fewer Action
        needed to move to a new $hood_lower
      </div>
EOF;

    if ((!$item->is_loot) && ($item->quantity == 0)) {
      $ai_output .= "/+spd:$item->speed_increase";
    }
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

  foreach ($acts_arr as $action) {
  //    firep($action, 'action associated with staff');

    $cost = "Cost: $action->cost Action";
    if ($action->values_cost > 0)
      $cost .= ", $action->values_cost $game_user->values";

    $name = str_replace('%value', $game_user->values, $action->name);

    echo '<div class="land-action">Action: ' . $name . '</div>';
    echo '<div class="land-description">' . $action->description . '</div>';
    echo '<div class="land-action-cost">' . $cost . '</div>';

    if ($action->influence_change < 0) {
      $inf_change = -$action->influence_change;
      echo <<< EOF
        <div class="land-payout negative">Effect: Target's
          {$experience_lower} is reduced by $inf_change</div>
EOF;
    }

    if (($action->rating_change < 0.10) && ($action->rating_change != 0.0)) {
      $rat_change = abs($action->rating_change);

      if ($action->rating_change < 0.0) {
        echo <<< EOF
          <div class="land-payout negative">Effect: $target approval rating is
            reduced by $rat_change%</div>
EOF;
      }
      else {
        echo <<< EOF
          <div class="land-payout">Effect: $target approval rating is
            increased by $rat_change%</div>
EOF;
      }
    }

    if ($action->rating_change >= 0.10) {
      $rat_change = $action->rating_change;
      echo <<< EOF
        <div class="land-payout">Effect: Your approval rating is
          increased by $rat_change%</div>
EOF;
    }

    if ($action->neighborhood_rating_change < 0.0) {
      $rat_change = -$action->neighborhood_rating_change;
      echo <<< EOF
        <div class="land-payout negative">Effect: $hood $beauty_lower
          rating is reduced by $rat_change</div>
EOF;
    }

    if ($action->neighborhood_rating_change > 0.0) {
      $rat_change = $action->neighborhood_rating_change;
      echo <<< EOF
        <div class="land-payout">Effect: $hood $beauty_lower rating is
          increased by $rat_change</div>
EOF;
    }

    if ($action->values_change < 0) {
      $val_change = -$action->values_change;
      echo <<< EOF
        <div class="land-payout negative">Effect: Target's $game_user->values is
          reduced by $val_change</div>
EOF;
    }
  }

  echo '</div>';

  // BUY buttons.
  if ($soon) {
    echo <<< EOF
      <div class="land-button-wrapper">
        <div class="land-buy-button not-yet">
          Can't Hire
        </div>
        <div class="land-sell-button not-yet">
          Can't Fire
        </div>
      </div>
EOF;
  }
  else {

    // Attempt to buy.
    if ($options['staff-buy-succeeded']) {
      echo <<< EOF
        <div class="land-button-wrapper">
          <form action="/$game/staff_hire/$arg2/$item->id/use-quantity">
            <div class="quantity">
              <select name="quantity">
EOF;

      foreach ([1, 5, 10, 25, 50, 100] as $option) {
        if ($option == $options['orig-quantity']) {
          echo '<option selected="selected" value="' . $option . '">' .
            $option . '</option>';
        }
        else {
          echo '<option value="' . $option . '">' . $option . '</option>';
        }
      }
      echo <<< EOF
        </select>
      </div>
      <input class="land-buy-button" type="submit" Value="Hire"/>
    </form>
  </div>
EOF;
    }

    // Normal show.
    else {
      echo <<< EOF
      <div class="land-button-wrapper">
        $buy_button
        $sell_button
      </div>
EOF;
    }

  }

  echo '</div>';
}

/**
 * Fetch the list of staff to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass[]
 *   The array of staff objects.
 */
function game_fetch_visible_staff($game_user) {
  if ($game_user->meta == 'admin') {
    $sql_to_add = 'WHERE staff.id > 0';
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
  
    OR staff_ownership.quantity > 0 ';
  }

  $data = [];
  $sql = 'SELECT staff.*, staff_ownership.quantity
  FROM staff

  LEFT OUTER JOIN staff_ownership ON staff_ownership.fkey_staff_id = staff.id
  AND staff_ownership.fkey_users_id = %d

  ' . $sql_to_add . '
  ORDER BY required_level ASC';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  return $data;
}

/**
 * Fetch the next staff item to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @return \stdClass
 *   The next staff item to show, if any.
 */
function game_fetch_next_staff($game_user) {
  if ($game_user->meta == 'admin') {
    $sql_to_add = 'AND staff.id > 0';
  }
  else {
    $sql_to_add = 'AND active = 1
      AND is_loot = 0';
  }
  $sql = 'SELECT staff.*, staff_ownership.quantity
  FROM staff

  LEFT OUTER JOIN staff_ownership ON staff_ownership.fkey_staff_id = staff.id
  AND staff_ownership.fkey_users_id = %d

  WHERE ((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND
  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

  AND required_level > %d
  ' . $sql_to_add . '
  ORDER BY required_level ASC LIMIT 1';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);
  return db_fetch_object($result);
}
