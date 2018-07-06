<?php

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();

if ($quantity === 'use-quantity') {
  $quantity = (int) check_plain($_GET['quantity']);
}

$game_equipment = game_fetch_equip_by_id($game_user, $equipment_id);
$orig_quantity = $count = $quantity;
$equipment_price = 0;

while ($count--) {
  $equipment_price += $game_equipment->price +
    (($game_equipment->quantity + $count) *  $game_equipment->price_increase);
}

$options = [];
$options['equipment-buy-succeeded'] = 'buy-success';
$options['orig-quantity'] = $orig_quantity;
$ai_output = 'equipment-succeeded';

// Check to see if equipment prerequisites are met.
// Enough money?
if ($game_user->money < $equipment_price) {
  $options['equipment-buy-succeeded'] = 'failed no-money';
  $ai_output = 'equipment-failed no-money';
}

// Not high enough level.
if ($game_user->level < $game_equipment->required_level) {
  $options['equipment-buy-succeeded'] = 'failed not-required-level';
  $ai_output = 'equipment-failed not-required-level';
  game_karma($game_user,
    "trying to purchase $game_equipment->name at level $game_user->level", -100);
}

// Hit a quantity limit?
if ((($game_equipment->quantity + $quantity) > $game_equipment->quantity_limit) &&
  ($game_equipment->quantity_limit >= 1)) {
  $options['equipment-buy-succeeded'] = 'failed hit-quantity-limit';
  $ai_output = 'equipment-failed hit-quantity-limit';
  game_karma($game_user,
    "trying to purchase more $game_equipment->name than allowed", -100);
}

// Too little income to cover upkeep?
if ($game_user->income < $game_user->expenses +
   ($game_equipment->upkeep * $quantity)) {
  $options['equipment-buy-succeeded'] = 'failed not-enough-income';
  $ai_output = 'equipment-failed not-enough-income';
  game_karma($game_user,
    "not enough income to cover $game_equipment->name\'s upkeep", -100);
}

// Not in right hood.
if ($game_equipment->fkey_neighborhoods_id != 0 &&
  $game_equipment->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id) {
  $options['equipment-buy-succeeded'] = 'failed not-required-hood';
  $ai_output = 'equipment-failed not-required-hood';
  game_karma($game_user,
    "trying to purchase $game_equipment->name in wrong hood", -50);
}

// Not required party.
if ($game_equipment->fkey_values_id != 0 &&
  $game_equipment->fkey_values_id != $game_user->fkey_values_id) {
  $options['equipment-buy-succeeded'] = 'failed not-required-party';
  $ai_output = 'equipment-failed not-required-party';
  game_karma($game_user,
    "trying to purchase $game_equipment->name in wrong party", -50);
}

// Not active.
if ($game_equipment->active != 1) {
  $options['equipment-buy-succeeded'] = 'failed not-active';
  $ai_output = 'equipment-failed not-active';
  game_karma($game_user,
    "trying to purchase $game_equipment->name which is not active", -500);
}

// Is loot.
if ($game_equipment->is_loot != 0) {
  $options['equipment-buy-succeeded'] = 'failed is-loot';
  $ai_output = 'equipment-failed is-loot';
  game_karma($game_user,
    "trying to purchase $game_equipment->name which is loot", -25);
}

// Success!
if ($options['equipment-buy-succeeded'] == 'buy-success') {
  game_equipment_gain($game_user, $equipment_id, $quantity, $equipment_price);
}
else {
  $quantity = 0;
}

$game_equipment->quantity += $quantity;
$data = game_fetch_visible_equip($game_user);
$next = game_fetch_next_equip($game_user);

// ------ VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);

if ($game_user->level < 15) {
  echo <<< EOF
<ul>
  <li>Purchase $equipment_lower to help you and your aides</li>
</ul>
EOF;
}

game_show_equip($game_user, $game_equipment, $ai_output, $options);

echo <<< EOF
<div class="title">
  Purchase $equipment
</div>
EOF;

echo '<div id="all-equip">';

foreach ($data as $item) {
  //firep($item, 'equipment item');
  game_show_equip($game_user, $item, $ai_output);
}

game_show_ai_output($phone_id, $ai_output);

// Show next one.
if (!empty($next)) {
  game_show_equip($game_user, $next, $ai_output, ['soon' => TRUE]);
}

echo '</div>';
db_set_active('default');
