<?php

global $game, $phone_id;

$fetch_user = '_' . arg(0) . '_fetch_user';
$fetch_header = '_' . arg(0) . '_header';

$game_user = $fetch_user();
include drupal_get_path('module', $game) . '/game_defs.inc';
$arg2 = check_plain(arg(2));
$ai_output = 'equipment-prices';

// Fix expenses in case they are out of whack.
game_recalc_income($game_user);
$fetch_header($game_user);
show_aides_menu($game_user);

if ($game_user->level < 15) {
  echo <<< EOF
<ul>
  <li>Purchase $equipment_lower to help you and your aides</li>
</ul>
EOF;
}

// For testing - exclude all exclusions (!) if I am an admin.
if ($game_user->meta == 'admin') {
  $land_active = ' AND (active = 1 OR active = 0) ';
}
else {
  $land_active = ' AND active = 1 ';
}

$data = [];
$sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership
    ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE (((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND

  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

  AND required_level <= %d
  ' . $land_active . '
  AND is_loot = 0) OR equipment_ownership.quantity > 0
  ORDER BY required_level ASC';
$result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $game_user->level);

while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

foreach ($data as $item) {
firep($item, 'equipment item');

  $quantity = $item->quantity;
  if (empty($quantity)) {
    $quantity = '<em>None</em>';
  }

  $equipment_price = $item->price + ($item->quantity *
    $item->price_increase);

  if (!$item->is_loot) {
    $ai_output .= " $item->id=$equipment_price";
  }

  if (($equipment_price % 1000) == 0) {
    $equipment_price = ($equipment_price / 1000) . 'K';
  }

  if ($item->quantity_limit > 0) {
    $quantity_limit = '<em>(Limited to ' . $item->quantity_limit . ')</em>';
  }
  else {
    $quantity_limit = '';
  }

  echo <<< EOF
<div class="land">
<div class="land-icon"><a href="/$game/equipment_buy/$arg2/$item->id/1"><img
  src="/sites/default/files/images/equipment/$game-$item->id.png" border="0"
  width="96"></a></div>
<div class="land-details">
  <div class="land-name"><a
    href="/$game/equipment_buy/$arg2/$item->id/1">$item->name</a></div>
  <div class="land-description">$item->description</div>
  <div class="land-owned">Owned: $quantity $quantity_limit</div>
  <div class="land-cost">Cost: $equipment_price $game_user->values</div>
EOF;

  if ($item->energy_bonus > 0) {

    echo <<< EOF
  <div class="land-payout">Energy: +$item->energy_bonus immediate energy bonus
    </div>
EOF;

  }

  if ($item->energy_increase > 0) {

    echo <<< EOF
  <div class="land-payout">Energy: +$item->energy_increase every 5 minutes
    </div>
EOF;

  }

  if ($item->initiative_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">$initiative: +$item->initiative_bonus
    </div>
EOF;
  }
  else if ($item->initiative_bonus < 0) {
    echo <<< EOF
  <div class="land-payout negative">$initiative: $item->initiative_bonus
    </div>
EOF;
  }

  if ($item->endurance_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">$endurance: +$item->endurance_bonus
    </div>
EOF;
  }
  else if ($item->endurance_bonus < 0) {
    echo <<< EOF
  <div class="land-payout negative">$endurance: $item->endurance_bonus
    </div>
EOF;
  }

  if ($item->elocution_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">$elocution: +$item->elocution_bonus
    </div>
EOF;

  if (!$item->is_loot) $ai_output .= "/+elo:$item->elocution_bonus";
  }
  else if ($item->elocution_bonus < 0) {
    echo <<< EOF
  <div class="land-payout negative">$elocution: $item->elocution_bonus
    </div>
EOF;

    if (!$item->is_loot) $ai_output .= "/+elo:$item->elocution_bonus";
  }

  if ($item->speed_increase > 0) {

    echo <<< EOF
  <div class="land-payout">Speed Increase: $item->speed_increase fewer Action
    needed to move to a new $hood_lower
    </div>
EOF;

  if ((!$item->is_loot) && ($item->quantity == 0))
    $ai_output .= "/+spd:$item->speed_increase";

  }

  if ($item->upkeep > 0) {

    echo <<< EOF
  <div class="land-payout negative">Upkeep: $item->upkeep every 60 minutes</div>
EOF;

  }

  if ($item->chance_of_loss > 0) {

    $lifetime = floor(100 / $item->chance_of_loss);
     $use = ($lifetime == 1) ? 'use' : 'uses';
    echo <<< EOF
  <div class="land-payout negative">Expected Lifetime: $lifetime $use</div>
EOF;

  }

// grab each action for the equipment
  $data2 = array();
  $sql = 'select * from actions where fkey_equipment_id = %d;';
  $result = db_query($sql, $item->id);

  while ($action = db_fetch_object($result)) $data2[] = $action;

  foreach ($data2 as $action) {
firep($action);

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
  <div class="land-payout negative">Effect: Neighborhood $beauty_lower
    rating is reduced by $rat_change</div>
EOF;

  }

  if ($action->neighborhood_rating_change > 0.0) {

    $rat_change = $action->neighborhood_rating_change;

    echo <<< EOF
  <div class="land-payout">Effect: Neighborhood $beauty_lower rating is
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

  if (!$item->is_loot) {

    echo <<< EOF
<div class="land-button-wrapper"><div class="land-buy-button"><a
  href="/$game/equipment_buy/$arg2/$item->id/1">Buy</a></div>
EOF;

  }
  else {

    echo <<< EOF
<div class="land-button-wrapper"><div class="land-buy-button not-yet">Can't
  Buy</div>
EOF;

  }

  if ($item->can_sell) {

    echo <<< EOF
<div class="land-sell-button"><a
  href="/$game/equipment_sell/$arg2/$item->id/1">Sell</a></div></div>
EOF;

  }
  else {

    echo <<< EOF
<div class="land-sell-button not-yet"><!--<a
  href="/$game/equipment_sell/$arg2/$item->id/1">-->Can't Sell<!--</a>--></div></div>
EOF;

  }

  echo '</div>';

}

if (substr($phone_id, 0, 3) == 'ai-')
  echo "<!--\n<ai \"$ai_output\"/>\n-->";

// show next one
$sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE ((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND

  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

  AND required_level > %d' . $land_active .
  'AND is_loot = 0
  ORDER BY required_level ASC LIMIT 1';
$result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $game_user->level);

$item = db_fetch_object($result);
firep($item);

if (!empty($item)) {
  $description = str_replace('%clan', "<em>$clan_title</em>",
    $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) $quantity = '<em>None</em>';

  $equipment_price = $item->price + ($item->quantity *
    $item->price_increase);

  echo <<< EOF
<div class="land-soon">
<div class="land-details">
  <div class="land-name">$item->name</div>
  <div class="land-description">$description</div>
  <div class="land-required_level">Requires level $item->required_level</div>
  <div class="land-cost">Cost: $equipment_price $game_user->values</div>
EOF;

  if ($item->energy_bonus > 0) {

    echo <<< EOF
  <div class="land-payout">Energy: +$item->energy_bonus immediate energy bonus
    </div>
EOF;

  }

  if ($item->energy_increase > 0) {

    echo <<< EOF
  <div class="land-payout">Energy: +$item->energy_increase every 5 minutes
    </div>
EOF;

  }

  if ($item->elocution_bonus > 0) {

    echo <<< EOF
  <div class="land-payout">$elocution: +$item->elocution_bonus
    </div>
EOF;

  }

  if ($item->speed_increase > 0) {

    echo <<< EOF
  <div class="land-payout">Speed Increase: $item->speed_increase fewer Action
    needed to move to a new $hood_lower
    </div>
EOF;

  }

  echo <<< EOF
</div>
</div>
EOF;

}

db_set_active('default');
