<?php

/**
 * @file stlouis_land_buy.tpl.php
 * Stloiuis land buy
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
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();

if (empty($game_user->username)) {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

if ($quantity === 'use-quantity') {
  $quantity = (int) check_plain($_GET['quantity']);
}

$game_land = game_fetch_land_by_id($game_user, $land_id);
$orig_quantity = $count = $quantity;
$land_price = 0;

while ($count--) {
  $land_price += $game_land->price + (($game_land->quantity + $count) *
    $game_land->price_increase);
}

$options = [];
$options['land-buy-succeeded'] = 'buy-success';
$options['orig-quantity'] = $orig_quantity;
$ai_output = 'land-succeeded';

// Check to see if land prerequisites are met.
// Not enough money.
if ($game_user->money < $land_price) {
  $options['land-buy-succeeded'] = 'failed no-money';
  $ai_output = 'land-failed no-money';
}

// Not high enough level.
if ($game_user->level < $game_land->required_level) {
  $options['land-buy-succeeded'] = 'failed not-required-level';
  $ai_output = 'land-failed not-required-level';
  game_karma($game_user,
    "trying to purchase $game_land->name at level $game_user->level", -100);
}

// Not required competency.
if ($game_land->fkey_required_competencies_id > 0) {
  $check = game_competency_level($game_user,
    (int) $game_land->fkey_required_competencies_id);
// firep($check);
  if ($check->level < $game_land->required_competencies_level) {
    $options['land-buy-succeeded'] = 'failed not-required-competency';
    $ai_output = 'land-failed not-required-competency';
  }
}

// Not in right hood.
if ($game_land->fkey_neighborhoods_id != 0 &&
  $game_land->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id) {
  $options['land-buy-succeeded'] = 'failed not-required-hood';
  $ai_output = 'land-failed not-required-hood';
  game_karma($game_user,
    "trying to purchase $game_land->name in wrong hood", -50);
}

// Not required party.
if ($game_land->fkey_values_id != 0 &&
  $game_land->fkey_values_id != $game_user->fkey_values_id) {
  $options['land-buy-succeeded'] = 'failed not-required-party';
  $ai_output = 'land-failed not-required-party';
  game_karma($game_user,
    "trying to purchase $game_land->name in wrong party", -50);
}

// Not active.
if ($game_land->active != 1) {
  $options['land-buy-succeeded'] = 'failed not-active';
  $ai_output = 'land-failed not-active';
  game_karma($game_user,
    "trying to purchase $game_land->name which is not active", -500);
}

// Is loot.
if ($game_land->is_loot != 0) {
  $options['land-buy-succeeded'] = 'failed is-loot';
  $ai_output = 'land-failed is-loot';
  game_karma($game_user,
    "trying to purchase $game_land->name which is loot", -25);
}

// Success!
if ($options['land-buy-succeeded'] == 'buy-success') {

  // Job? Delete other job(s).
  if ($game_land->type == 'job') {

    $sql = 'DELETE FROM `land_ownership` WHERE id IN (
      SELECT id FROM (
        SELECT lo.id
        FROM land_ownership AS lo
        LEFT JOIN land ON lo.fkey_land_id = land.id
        WHERE fkey_users_id = %d
        AND land.type = "job"
      ) x
    );';
    db_query($sql, $game_user->id);

    $game_land->quantity = 0;
  }

  // Investment?  Add competency.
  if ($game_land->type == 'investment') {
    game_competency_gain($game_user, (int) $game_land->fkey_enhanced_competencies_id);
  }
  game_land_gain($game_user, $land_id, $quantity, $land_price);
}
else {
  $quantity = 0;
}

$game_land->quantity = $game_land->quantity + $quantity;
$data = game_fetch_visible_land($game_user);
$next = game_fetch_next_land($game_user);

// ------- VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);
game_show_land($game_user, $game_land, $options);
?>
<div class="title">
  Available <?php print $land_plural; ?>
</div>
<?php
  game_show_ai_output($phone_id, $ai_output);
?>
<div id="all-land">
  <?php
    foreach ($data as $item):
      game_show_land($game_user, $item);
    endforeach;
    if (!empty($next)):
      game_show_land($game_user, $next, ['soon' => TRUE]);
    endif;
     db_set_active();
  ?>
</div>
