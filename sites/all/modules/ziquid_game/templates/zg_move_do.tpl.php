<?php

/**
 * @file
 * Template for doing movement.
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
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();
$q = $_GET['q'];

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
$current_neighborhood_id = $game_user->fkey_neighborhoods_id;
if ($neighborhood_id == $current_neighborhood_id &&
  $game_user->meta != 'admin') {
  zg_fetch_header($game_user);
  echo <<< EOF
<div class="title">You are already in $game_user->location</div>
<div class="election-continue"><a href="/$game/move/$arg2/0">Try again</a></div>
EOF;

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"move-failed already-there\"/>\n-->";
  }

  db_set_active();
  return;
}

if ($neighborhood_id > 0) {
  list($cur_hood, $new_hood, $actions_to_move, $verb, $eq) =
    zg_get_actions_to_move($game_user, $current_neighborhood_id, $neighborhood_id);

  // April fools 2013.
//    $actions_to_move = 1;

  if ($game_user->actions < $actions_to_move) {

    zg_fetch_header($game_user);

    echo '<div class="land-failed">' . t('Out of Action!') . '</div>';
    zg_button('elders_do_fill', t('Refill your Action (1&nbsp;Luck)'),
      '/action?destination=/' . $q, 'big-68');
    zg_button('move', t('Choose a different @neighborhood',
      ['@neighborhood' => $hood_lower]), '/0', 'big-68');

    if (substr($phone_id, 0, 3) == 'ai-') {
      echo "<!--\n<ai \"move-failed no-action\"/>\n-->";
    }

    db_set_active();
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

  // Check new hood clan.
  $sql = 'SELECT clans.acronym FROM `users`
    inner join elected_officials on elected_officials.fkey_users_id = users.id
    inner join clan_members cm on users.id = cm.fkey_users_id
    inner join clans on cm.fkey_clans_id = clans.id
    WHERE fkey_neighborhoods_id = %d
    and elected_officials.fkey_elected_positions_id = 1;';
  $clan = db_query($sql, $neighborhood_id)->fetch_object();
  if (!empty($clan->acronym)) {
    $clan_msg = t('<div class="subsubtitle">This @hood is controlled by the %clan clan.</div>',
      ['@hood' => $game_text['hood_lower'], '%clan' => $clan->acronym]);
  }
  else {
    $clan_msg = '';
  }

  $game_user = zg_fetch_user();
  zg_alter('move_to_succeeded', $game_user, $current_neighborhood_id,
    $neighborhood_id);
  zg_fetch_header($game_user);

  echo '<div class="land-succeeded">' . t('Success!') . '</div>';

  echo <<< EOF
<div class="subtitle">You have arrived in <span class="nowrap highlight">$game_user->location</span></div>
<div class="subsubtitle">$resigned_text</div>
$clan_msg
EOF;

  if (!empty($new_hood->welcome_msg)) {
    echo <<< EOF
<p class="second">You see a billboard when you enter the {$game_text['hood_lower']}.&nbsp; It states:</p>
<div class="subsubtitle">$new_hood->welcome_msg</div>
EOF;
  }

  echo $unfrozen_msg;

  if ($equip_lost) {

    // FIXME: check equipment_failure_reasons.
    echo '<div class="subtitle">' . t('Your @stuff has worn out',
      array('@stuff' => strtolower($eq->name))) . '</div>';
  }

  $link = 'quest_groups';
  $lqg = zg_fetch_latest_quest_group($game_user);

  zg_button($link, "Continue to ${quest}s", "#group-{$lqg}", 'big-68');
  zg_button('actions', 'Continue to Actions', '', 'big-68');
  zg_button('home', 'Go to the home page', '', 'big-68');

  if (zg_get_value($game_user, 'WanderLust', FALSE)) {
    zg_button('move', "Move Again", "/0", 'big-68');
  }

  // Cinco De Mayo in Benton Park West.
  if ($event_type == EVENT_CINCO_DE_MAYO && $neighborhood_id == 30) {
    zg_button($link, "Go to <i>Los Tacos</i>", "#group-1100", 'big-68');
  }

  // FIXME: add hood_id to the query.
  $hood_equip = zg_fetch_visible_equip($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_equip as $item) {
    if ($item->fkey_neighborhoods_id == $neighborhood_id) {
      if (!$title_shown) {
        echo <<< EOF
<div class="title">
  Useful Equipment in <span class="highlight nowrap">$game_user->location</span>
</div>
EOF;
        $title_shown = TRUE;
      }
      zg_show_equip($game_user, $item, $ai_output);
    }
  }

  // FIXME: add hood_id to the query.
  $hood_staff = zg_fetch_visible_staff($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_staff as $item) {
    if ($item->fkey_neighborhoods_id == $neighborhood_id) {
      if (!$title_shown) {
        echo <<< EOF
<div class="title">
  Useful Staff and Aides in <span class="highlight nowrap">$game_user->location</span>
</div>
EOF;
        $title_shown = TRUE;
      }
      zg_show_staff($game_user, $item, $ai_output);
    }
  }

  $hood_qgs = zg_fetch_highlighted_quest_groups($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_qgs as $item) {
    if (!$title_shown) {
      echo <<< EOF
<div class="title">
  Useful Missions in <span class="highlight nowrap">$game_user->location</span>
</div>
EOF;
      $title_shown = TRUE;
    }
    zg_show_quest_group($game_user, $item, $ai_output);
  }

}

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"move-succeeded\"/>\n-->";
}

db_set_active();
