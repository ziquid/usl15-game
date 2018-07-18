<?php

/**
 * @file stlouis_land_work.tpl.php
 * Template for working a job.
 *
 * Synced with CG: no
 * Synced with 2114: no
 */

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

  WHERE land.id = %d
  AND land.type = "job"
  AND land.active = 1;';
$result = db_query($sql, $game_user->id, $land_id);
$game_land = db_fetch_object($result);
firep($game_land, 'job found: ');

$options = [];
$options['land-work-succeeded'] = 'work-success';
$ai_output = 'land-succeeded';

// Check to see if land prerequisites are met.
// No job found.
if (empty($game_land)) {
  $options['land-work-succeeded'] = 'failed no-such-job';
  $ai_output = 'land-failed no-such-job';
  game_karma($game_user,
    "trying to work a non-existent job (id: $land_id)", -100);
}

// Too soon to work again.
$can_work_again = game_can_do_yet($game_user, 'can_work_again');
firep($can_work_again, 'can work again');
if (!$can_work_again->allowed) {
  $options['land-work-succeeded'] = 'failed too-soon';
  $ai_output = 'land-failed too-soon';
  game_karma($game_user,
    "trying to work too soon (time remaining: $can_work_again->time_remaining)", -10);
}

// Success!
if ($options['land-work-succeeded'] == 'work-success') {

  // Add competency.
  game_competency_gain($game_user, (int) $game_land->fkey_enhanced_competencies_id);

  // Set timer.  Can work again in 4 hours.
  $set_value = '_' . $game . '_set_value';
  $set_value($game_user->id, 'can_work_again', time() + 60*60*4);

  // Gain the wage.
  $sql = 'update users set money = money + %d where id = %d;';
  $result = db_query($sql, $game_land->payout, $game_user->id);
  $game_user->money += $game_land->payout;

  // Set output.
  $title = t('Success!  You feel tired!');
}
else {
  $title = t('Uhoh!');
}

// Show the stuff.
$fetch_header($game_user);
game_show_aides_menu($game_user);

echo <<< EOF
<div class="title">
$title
</div>
$outcome_reason
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
