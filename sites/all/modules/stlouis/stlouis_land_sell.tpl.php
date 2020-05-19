<?php

/**
 * @file stlouis_land_sell.tpl.php
 * Stlouis land sell
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

if (empty($game_user->username) || $game_user->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

if ($quantity === 'use-quantity') {
  $quantity = (int) check_plain($_GET['quantity']);
}

$game_land = game_fetch_land_by_id($game_user, $land_id);
$orig_quantity = $quantity;
$land_price = ceil($game_land->price * $quantity * 0.6);
/* allow for 80% sale price
$land_price = ceil($game_land->price +
  ($game_land->price_increase * ($game_land->quantity - $quantity))
   * $quantity * 0.8);
*/

$options = [];
$options['land-sell-succeeded'] = 'sell-success';
$options['orig-quantity'] = $orig_quantity;
$ai_output = 'land-succeeded';

// Check to see if land prerequisites are met.
// Hit a quantity limit?
if ($quantity > $game_land->quantity) {
  $options['land-sell-succeeded'] = 'failed not-enough-land';
  $ai_output = 'land-failed not-enough-land';
  game_karma($game_user,
    "trying to sell $quantity of $game_land->name but only has $game_land->quantity",
    $quantity * -10);
}

// Can't sell?
if ($game_land->can_sell != 1) {
  $options['land-sell-succeeded'] = 'failed cant-sell';
  $ai_output = 'land-failed cant-sell';
  game_karma($game_user, "trying to sell unsalable $game_land->name", -100);
}

// Job?
if ($game_land->type == 'job') {
  $options['land-sell-succeeded'] = 'failed cant-sell-job';
  $ai_output = 'land-failed cant-sell-job';
  game_karma($game_user, "trying to sell job $game_land->name", -100);
}

// Success!
if ($options['land-sell-succeeded'] == 'sell-success') {
  game_land_lose($game_user, $land_id, $quantity, $land_price);
}
else {
  $quantity = 0;
}

$game_land->quantity -= $quantity;
$data = game_fetch_visible_land($game_user);
$next = game_fetch_next_land($game_user);

// ------- VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);
game_show_land($game_user, $game_land, $options);

echo <<< EOF
<div class="title">
Available $land_plural
</div>
EOF;

game_show_ai_output($phone_id, $ai_output);

echo '<div id="all-land">';

foreach ($data as $item) {
  game_show_land($game_user, $item);
}

if (!empty($next)) {
  game_show_land($game_user, $next, ['soon' => TRUE]);
}

echo '</div>';
db_set_active();
