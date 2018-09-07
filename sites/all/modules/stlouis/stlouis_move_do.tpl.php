<?php

/**
 * @file stlouis_move_do.tpl.php
 * Template for doing movement.
 */

global $game, $phone_id;
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();

// Random hood -- April fools 2013.
/*
if (mt_rand(0, 1) > 0) {

  $sql = 'select id from neighborhoods where xcoor > 0 and ycoor > 0
    order by rand() limit 1;';
  $result = db_query($sql);
  $item = db_fetch_object($result);
  $neighborhood_id = $item->id;

}
*/
if ($neighborhood_id == $game_user->fkey_neighborhoods_id &&
  $game_user->meta != 'admin') {
  $fetch_header($game_user);
  echo <<< EOF
<div class="title">You are already in $game_user->location</div>
<div class="election-continue"><a href="/$game/move/$arg2/0">Try again</a></div>
EOF;

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"move-failed already-there\"/>\n-->";

  db_set_active('default');
  return;
}

if ($neighborhood_id > 0) {

  $sql = 'select * from neighborhoods where id = %d;';
  $result = db_query($sql, $game_user->fkey_neighborhoods_id);
  $cur_hood = db_fetch_object($result);
firep($cur_hood, 'current hood');

  $sql = 'select * from neighborhoods where id = %d;';
  $result = db_query($sql, $neighborhood_id);
  $new_hood = db_fetch_object($result);
firep($new_hood, 'new hood');

  $distance = floor(sqrt(pow($cur_hood->xcoor - $new_hood->xcoor, 2) +
    pow($cur_hood->ycoor - $new_hood->ycoor, 2)));

  $actions_to_move = floor($distance / 8);

  $sql = 'SELECT equipment.speed_increase as speed_increase,
    action_verb, chance_of_loss, equipment.id, name, upkeep from equipment

    left join equipment_ownership
      on equipment_ownership.fkey_equipment_id = equipment.id
      and equipment_ownership.fkey_users_id = %d

    where equipment_ownership.quantity > 0
    order by equipment.speed_increase DESC limit 1;';

  $result = db_query($sql, $game_user->id);
  $eq = db_fetch_object($result);

  if ($eq->speed_increase > 0) {
    $actions_to_move -= $eq->speed_increase;
  }

  $actions_to_move = max($actions_to_move, 6);

  if ($game_user->meta == 'admin') {
    $actions_to_move = 0;
  }

  game_alter('actions_to_move', $game_user, $actions_to_move);

  // April fools 2013.
//    $actions_to_move = 1;

  if ($game_user->actions < $actions_to_move) {

    $fetch_header($game_user);

    echo '<div class="land-failed">' . t('Out of Action!') .
      '</div>';
    echo '<div class="try-an-election-wrapper"><div
      class="try-an-election"><a
      href="/' . $game . '/elders_do_fill/' . $arg2 .
      '/action?destination=/' . $game . '/move/' . $arg2 . '/' .
      $neighborhood_id . '">' . t('Refill your Action (1&nbsp;Luck)') .
      '</a></div></div>';
    echo '<div class="try-an-election-wrapper"><div
      class="try-an-election"><a href="/' . $game . '/move/' .
      $arg2 . '/0">' . t('Choose a different @neighborhood',
      array('@neighborhood' => $hood_lower)) .
      '</a></div></div>';

    if (substr($phone_id, 0, 3) == 'ai-')
      echo "<!--\n<ai \"move-failed no-action\"/>\n-->";

    db_set_active('default');
    return;
  }

  $resigned_text = '';

  // You lose your old type 1 position, if any (type 1 = neighborhood).
  // Moving to a new district loses the type 3 (house) position.
  $sql = 'SELECT elected_positions.type
    FROM elected_officials
    LEFT JOIN elected_positions
    ON elected_officials.fkey_elected_positions_id = elected_positions.id
    WHERE elected_officials.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  if (($item->type == 1) ||
    ($item->type == 3 && ($cur_hood->district != $new_hood->district))) {

    $sql = 'delete from elected_officials where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);
    $resigned_text = 'and resigned your current position';
  }

  // Update neighborhood and actions.
  $sql = 'update users set fkey_neighborhoods_id = %d,
    actions = actions - %d where id = %d;';
  $result = db_query($sql, $neighborhood_id, $actions_to_move,
    $game_user->id);

  // Start the actions clock if needed.
  if ($game_user->actions == $game_user->actions_max) {
     $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
       $game_user->id);
  }

  $unfrozen_msg = '';

  // Frozen? 10% chance to mark as unfrozen.
  if (($game_user->meta == 'frozen') && (mt_rand(1, 10) == 1)) {
    $sql = 'update users set meta = "" where id = %d;';
    $result = db_query($sql, $game_user->id);
    $game_user->meta = '';
    $unfrozen_msg =
      '<div class="subtitle">Your movement has unfrozen you!</div>';
  }

  // Chance of loss.
  // Give them a little extra chance.
  if ($eq->chance_of_loss >= mt_rand(1,110)) {
    $equip_lost = TRUE;
    firep($eq->name . ' wore out!');
    $sql = 'update equipment_ownership set quantity = quantity - 1
      where fkey_equipment_id = %d and fkey_users_id = %d;';
    $result = db_query($sql, $eq->id, $game_user->id);

    // Player expenses need resetting?
    // Subtract upkeep from your expenses.
    if ($eq->upkeep > 0) {
      $sql = 'update users set expenses = expenses - %d where id = %d;';
      $result = db_query($sql, $eq->upkeep, $game_user->id);
    }
  }
  else {
    $equip_lost = FALSE;
    firep($eq->name . ' did NOT wear out');
  }

  $game_user = $fetch_user();
  $fetch_header($game_user);

  echo '<div class="land-succeeded">' . t('Success!') .
    '</div>';

  echo <<< EOF
<div class="subtitle">You have arrived in <span class="nowrap highlight">$game_user->location</span></div>
<div class="subsubtitle">$resigned_text</div>
EOF;

  if (!empty($new_hood->welcome_msg)) {
    echo <<< EOF
<p class="second">You see a billboard when you enter the $hood_lower.&nbsp; It states:</p>
<p class="second">$new_hood->welcome_msg</p>
EOF;
  }

  echo $unfrozen_msg;

  if ($equip_lost) {

    // FIXME: check equipment_failure_reasons.
    echo '<div class="subtitle">' . t('Your @stuff has worn out',
      array('@stuff' => strtolower($eq->name))) . '</div>';
  }

  echo <<< EOF
<div class="try-an-election-wrapper">
<div class="try-an-election">
  <a href="/$game/quests/$arg2">
    Continue to ${quest}s
  </a>
</div>
</div>
<div class="try-an-election-wrapper">
<div class="try-an-election">
  <a href="/$game/actions/$arg2">
    Continue to Actions
  </a>
</div>
</div>
<div class="try-an-election-wrapper">
<div class="try-an-election">
  <a href="/$game/home/$arg2">
    Go to the home page
  </a>
</div>
</div>
EOF;

  // Cinco De Mayo in Benton Park West.
  if ($event_type == EVENT_CINCO_DE_MAYO && $neighborhood_id == 30) {
    echo <<< EOF
  <div class="try-an-election-wrapper">
<div class="try-an-election">
  <a href="/$game/quests/$arg2/1100">
    Go to <i>Los Tacos</i>
  </a>
</div>
</div>
EOF;
  }

  // FIXME: add hood_id to the query.
  $hood_equip = game_fetch_visible_equip($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_equip as $item) {
    if ($item->fkey_neighborhoods_id == $neighborhood_id) {
      if (!$title_shown) {
        echo <<< EOF
<div class="title">
  Useful Equipment in <span class="nowrap">$game_user->location</span>
</div>
EOF;
        $title_shown = TRUE;
      }
      game_show_equip($game_user, $item, $ai_output);
    }
  }

  // FIXME: add hood_id to the query.
  $hood_staff = game_fetch_visible_staff($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_staff as $item) {
    if ($item->fkey_neighborhoods_id == $neighborhood_id) {
      if (!$title_shown) {
        echo <<< EOF
<div class="title">
  Useful Staff and Aides in <span class="nowrap">$game_user->location</span>
</div>
EOF;
        $title_shown = TRUE;
      }
      game_show_staff($game_user, $item, $ai_output);
    }
  }

  $hood_qgs = game_fetch_visible_quest_groups($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_qgs as $item) {
    if (!$title_shown) {
      echo <<< EOF
<div class="title">
  Useful Missions in <span class="nowrap">$game_user->location</span>
</div>
EOF;
      $title_shown = TRUE;
      game_show_quest_group($game_user, $item, $ai_output);
    }
  }

}

if (substr($phone_id, 0, 3) == 'ai-')
  echo "<!--\n<ai \"move-succeeded\"/>\n-->";

 db_set_active('default');
