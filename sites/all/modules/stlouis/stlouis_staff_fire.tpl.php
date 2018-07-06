<?php

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$title = t('Hire @staff and @agents', [
  '@staff' => $game_text['staff'],
  '@agents' => $game_text['agents'],
]);

if ($quantity === 'use-quantity') {
  $quantity = (int) check_plain($_GET['quantity']);
}

$game_staff = game_fetch_staff_by_id($game_user, $staff_id);
$orig_quantity = $quantity;
$staff_price = ceil($game_staff->price * $quantity * 0.6);
/* allow for 80% sale price
$staff_price = ceil($game_staff->price +
  ($game_staff->price_increase * ($game_staff->quantity - $quantity))
   * $quantity * 0.8);
*/

$options = [];
$options['staff-sell-succeeded'] = 'sell-success';
$options['orig-quantity'] = $orig_quantity;
$ai_output = 'staff-succeeded';

// Check to see if staff prerequisites are met.
// Hit a quantity limit?
if ($quantity > $game_staff->quantity) {
  $options['staff-sell-succeeded'] = 'failed not-enough-staff';
  $ai_output = 'staff-failed not-enough-staff';
  game_karma($game_user,
    "trying to sell $quantity of $game_staff->name but only has $game_staff->quantity",
    $quantity * -10);
}

// Can't sell?
if ($game_staff->can_sell != 1) {
  $options['staff-sell-succeeded'] = 'failed cant-sell';
  $ai_output = 'staff-failed cant-sell';
  game_karma($game_user, "trying to sell unsalable $game_staff->name", -100);
}


// Success!
if ($options['staff-sell-succeeded'] == 'sell-success') {
  game_staff_lose($game_user, $staff_id, $quantity, $staff_price);
}
else {
  $quantity = 0;
}

$game_staff->quantity -= $quantity;
$data = game_fetch_visible_staff($game_user);
$next = game_fetch_next_staff($game_user);

// ------ VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);

if ($game_user->level < 15) {
  echo <<< EOF
    <ul>
      <li>Hire {$game_text['staff']} to help you win elections and stay elected</li>
    </ul>
EOF;
}

game_show_staff($game_user, $game_staff, $ai_output, $options);

echo <<< EOF
<div class="title">
  $title
</div>
EOF;

echo '<div id="all-staff">';

foreach ($data as $item) {
  game_show_staff($game_user, $item,$ai_output);
}

game_show_ai_output($phone_id, $ai_output);

// Show next one.
if (!empty($next)) {
  game_show_staff($game_user, $next, $ai_output, ['soon' => TRUE]);
}

echo '</div>';
db_set_active('default');
