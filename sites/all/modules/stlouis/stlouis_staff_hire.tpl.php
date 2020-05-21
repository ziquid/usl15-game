<?php

/**
 * @file stlouis_staff_hire.tpl.php
 * Stlouis staff hire
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();
$title = t('Hire @staff and @agents', [
  '@staff' => $game_text['staff'],
  '@agents' => $game_text['agents'],
]);

if ($quantity === 'use-quantity') {
  $quantity = (int) check_plain($_GET['quantity']);
}

$game_staff = game_fetch_staff_by_id($game_user, $staff_id);
$orig_quantity = $count = $quantity;
$staff_price = 0;

while ($count--) {
  $staff_price += $game_staff->price +
    (($game_staff->quantity + $count) *  $game_staff->price_increase);
}

$options = [];
$options['staff-buy-succeeded'] = 'buy-success';
$options['orig-quantity'] = $orig_quantity;
$ai_output = 'staff-succeeded';

// Check to see if staff prerequisites are met.
// Enough money?
if ($game_user->money < $staff_price) {
  $options['staff-buy-succeeded'] = 'failed no-money';
  $ai_output = 'staff-failed no-money';
}

// Not high enough level.
if ($game_user->level < $game_staff->required_level) {
  $options['staff-buy-succeeded'] = 'failed not-required-level';
  $ai_output = 'staff-failed not-required-level';
  game_karma($game_user,
    "trying to hire $game_staff->name at level $game_user->level", -100);
}

// Hit a quantity limit?
if ((($game_staff->quantity + $quantity) > $game_staff->quantity_limit) &&
  ($game_staff->quantity_limit >= 1)) {
  $options['staff-buy-succeeded'] = 'failed hit-quantity-limit';
  $ai_output = 'staff-failed hit-quantity-limit';
  game_karma($game_user,
    "trying to hire more $game_staff->name than allowed", -100);
}

// Too little income to cover upkeep?
if ($game_user->income < $game_user->expenses +
  ($game_staff->upkeep * $quantity)) {
  $options['staff-buy-succeeded'] = 'failed not-enough-income';
  $ai_output = 'staff-failed not-enough-income';
  game_karma($game_user,
    "not enough income to cover $game_staff->name\'s upkeep", -100);
}

// Not in right hood.
if ($game_staff->fkey_neighborhoods_id != 0 &&
  $game_staff->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id) {
  $options['staff-buy-succeeded'] = 'failed not-required-hood';
  $ai_output = 'staff-failed not-required-hood';
  game_karma($game_user,
    "trying to hire $game_staff->name in wrong hood", -50);
}

// Not required party.
if ($game_staff->fkey_values_id != 0 &&
  $game_staff->fkey_values_id != $game_user->fkey_values_id) {
  $options['staff-buy-succeeded'] = 'failed not-required-party';
  $ai_output = 'staff-failed not-required-party';
  game_karma($game_user,
    "trying to hire $game_staff->name in wrong party", -50);
}

// Not active.
if ($game_staff->active != 1) {
  $options['staff-buy-succeeded'] = 'failed not-active';
  $ai_output = 'staff-failed not-active';
  game_karma($game_user,
    "trying to hire $game_staff->name which is not active", -500);
}

// Is loot.
if ($game_staff->is_loot != 0) {
  $options['staff-buy-succeeded'] = 'failed is-loot';
  $ai_output = 'staff-failed is-loot';
  game_karma($game_user,
    "trying to hire $game_staff->name which is loot", -25);
}

// Success!
if ($options['staff-buy-succeeded'] == 'buy-success') {
  game_staff_gain($game_user, $staff_id, $quantity, $staff_price);
}
else {
  $quantity = 0;
}

$game_staff->quantity += $quantity;
$data = game_fetch_visible_staff($game_user);
$next = game_fetch_next_staff($game_user);

// ------ VIEW ------
zg_fetch_header($game_user);
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
db_set_active();
