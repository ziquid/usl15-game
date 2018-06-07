<?php

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();

if ($quantity === 'use-quantity') {
  $quantity = (int) check_plain($_GET['quantity']);
}

$game_equipment = game_fetch_equip_by_id($game_user, $equipment_id);
$orig_quantity = $quantity;
$equipment_price = ceil($game_equipment->price * $quantity * 0.6);
/* allow for 80% sale price
$equipment_price = ceil($game_equipment->price +
  ($game_equipment->price_increase * ($game_equipment->quantity - $quantity))
   * $quantity * 0.8);
*/

$options = [];
$options['equipment-sell-succeeded'] = 'sell-success';
$options['orig-quantity'] = $orig_quantity;
$ai_output = 'equipment-succeeded';

// Check to see if equipment prerequisites are met.

// Hit a quantity limit?
if ($quantity > $game_equipment->quantity) {
  $options['equipment-sell-succeeded'] = 'failed not-enough-equipment';
  $ai_output = 'equipment-failed not-enough-equipment';
  game_karma($game_user,
    "trying to sell $quantity of $game_equipment->name but only has $game_equipment->quantity",
    $quantity * -10);
}

// Can't sell?
if ($game_equipment->can_sell != 1) {
  $options['equipment-sell-succeeded'] = 'failed cant-sell';
  $ai_output = 'equipment-failed cant-sell';
  game_karma($game_user, "trying to sell unsalable $game_equipment->name", -100);
}

// Success!
if ($options['equipment-sell-succeeded'] == 'sell-success') {
  game_equipment_lose($game_user, $equipment_id, $quantity, $equipment_price);

  if ($game_equipment->is_loot > 0) {
    competency_gain($game_user, 'thrifty');
  }
}
else {
  $quantity = 0;
}

$game_equipment->quantity -= $quantity;
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

// show next one
if (!empty($next)) {
  game_show_equip($game_user, $next, $ai_output, ['soon' => TRUE]);
}

echo '</div>';
db_set_active('default');
