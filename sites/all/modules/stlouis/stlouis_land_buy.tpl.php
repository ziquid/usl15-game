<?php

global $game, $phone_id;

$fetch_user = '_' . arg(0) . '_fetch_user';
$fetch_header = '_' . arg(0) . '_header';

$game_user = $fetch_user();
include drupal_get_path('module', $game) . '/game_defs.inc';
$arg2 = check_plain(arg(2));

if (empty($game_user->username)) {
  db_set_active('default');
  drupal_goto($game . '/choose_name/' . $arg2);
}

if ($quantity === 'use-quantity') {
  $quantity = check_plain($_GET['quantity']);
}

$data = [];
$sql = 'SELECT land.*, land_ownership.quantity,
  competencies.name as competency, comp1.name as competency_name_1
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
    competencies.id
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

  WHERE land.id = %d;';
$result = db_query($sql, $game_user->id, $land_id);
$game_land = db_fetch_object($result);
$orig_quantity = $count = $quantity;
$land_price = 0;
firep($game_land);

while ($count--) {
  $land_price += $game_land->price + (($game_land->quantity + $count) *
    $game_land->price_increase);
}

$options = [];
$options['land-buy-succeeded'] = 'buy-success';
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
  $ai_output = 'land-failed not-required-value';
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

  // Job?  delete other job(s).
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

    $game_land->quantity = '';
  }

  // Investment?  Add competency.
  if ($game_land->type == 'investment') {
    competency_gain($game_user, (int) $game_land->fkey_enhanced_competencies_id);
  }
  land_gain($game_user, $land_id, $quantity, $land_price);
}
else {
  $quantity = 0;
}

// Show the stuff.
$fetch_header($game_user);
game_show_aides_menu($game_user);

$game_land->quantity = $game_land->quantity + (int) $quantity;
game_show_land($game_user, $game_land, $options);

echo <<< EOF
<div class="title">
Available $land_plural
</div>
EOF;

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"$ai_output\"/>\n-->";
}

$data = [];
$sql = 'SELECT land.*, land_ownership.quantity,
  competencies.name as competency, comp1.name as competency_name_1
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
    competencies.id
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

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
    AND active = 1
  )

  OR land_ownership.quantity > 0

  ORDER BY fkey_enhanced_competencies_id, required_level ASC';
$result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $game_user->level);

while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

echo '<div id="all-land">';

foreach ($data as $item) {
firep($item, 'Item: ' . $item->name);
  game_show_land($game_user, $item);
}

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"$ai_output\"/>\n-->";
}

// Show next one.
$sql = 'SELECT land.*, land_ownership.quantity,
  competencies.name as competency, comp1.name as competency_name_1
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
    competencies.id
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

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
  AND active = 1
  ORDER BY required_level ASC LIMIT 1';
$result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $game_user->level);
$item = db_fetch_object($result);

if (!empty($item)) {
  firep($item, 'Soon Item: ' . $item->name);
  game_show_land($game_user, $item, ['soon' => TRUE]);
}

echo '</div>';

db_set_active('default');
