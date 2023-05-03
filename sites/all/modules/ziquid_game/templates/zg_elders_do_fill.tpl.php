<?php

/**
 * @file
 * Fill your stats by using Luck.
 *
 * Synced with CG: yes
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
$game_user = zg_fetch_player();

/*  if ($game == 'stlouis') {

  $link = $destination ? $destination : "/$game/user/$phone_id";

  zg_fetch_header($game_user);

  echo <<< EOF
<div class="title">
Luck-free 4th
</div>
<div class="subtitle">
Sorry!&nbsp; No $luck today!
</div>
<div class="subtitle">
<a href="$link">
  <img src="/sites/default/files/images/{$game}_continue.png"/>
</a>
</div>
EOF;

  db_set_active();

  return;

}*/

$quest_lower = strtolower($quest);
$experience_lower = strtolower($experience);
$amount_filled = 0;
$luck_remaining = $game_user->luck;
$sql_log = 'insert into luck_use (fkey_users_id, use_type, amount_filled, luck_remaining)
  values (%d, "%s", %d, %d);';

switch ($fill_type) {

  case 'action':

/*    	if (($game == 'stlouis') &&
      ($game_user->actions < $game_user->actions_max)) {

      $sql = 'update users set actions = actions_max where id = %d;';
      $result = db_query($sql, $game_user->id);

      $game_user = zg_fetch_user();
      zg_fetch_header($game_user);

      echo '<div class="subtitle">Amusez-vous bien !</div>';
      echo '<div class="subtitle">
        <a href="/' . $game . '/home/' . $phone_id . '">
          <img src="/sites/default/files/images/' . $game . '_continue.png"/>
        </a>
      </div>';

      db_set_active();
      return;

    }
*/
    if ($game_user->luck < 1) {
      $text = "Player {$game_user->username} attempted to refill $fill_type (currently: $game_user->actions) but only had $game_user->luck $luck.";
      zg_luck($game_user, 0, $game_user->actions, 0,
        $game_user->actions, $text, $fill_type, 'attempted fill');

      zg_fetch_header($game_user);
      echo '<div class="speech-bubble-wrapper background-color">
        <div class="wise_old_man sad"></div>
        <div class="speech-bubble">
          <p class="bonus-text">Out of ' . $luck . '!</p>
          <p>Your ' . $luck . ' has run out.</p>
          <p>Perhaps you would like to purchase some more?</p>
        </div>
      </div>';
      zg_button('elders_ask_purchase', t('Purchase more @luck', ['@luck' => $luck]));
      db_set_active();
      return;
    }

    list($amount_now, $comment) = zg_luck_action_offer($game_user);
    $amount_filled = $amount_now - $game_user->actions;
    if ($game_user->actions < $amount_now) {
      $text = "Player {$game_user->username} refilled action (from $game_user->actions to $amount_now, gain of $amount_filled) using 1 $luck ($game_user->luck $luck before, now 1 less).";
      if (strlen($comment)) {
        $text .= ' (' . $comment . ')';
      }
      $sql = 'update users set actions = %d where id = %d;';
      db_query($sql, $amount_now, $game_user->id);
      zg_luck($game_user, -1, $game_user->actions, $amount_filled,
        $amount_now, $text, $fill_type, 'fill');
    }
    else {
      $text = "Player {$game_user->username} attempted to refill $fill_type (from $game_user->actions to $amount_now, gain of $amount_filled) using 1 $luck ($game_user->luck $luck currently) but was refused as $fill_type is already full.";
      if (strlen($comment)) {
        $text .= ' (' . $comment . ')';
      }
      zg_luck($game_user, 0, $game_user->actions, $amount_filled,
        $amount_now, $text, $fill_type, 'fill');

    }
    break;

  case 'energy':

    if ($game_user->luck < 1) {
      $text = "Player {$game_user->username} attempted to refill $fill_type (currently: $game_user->energy) but only had $game_user->luck $luck.";
      zg_luck($game_user, 0, $game_user->energy, 0,
        $game_user->energy, $text, $fill_type, 'attempted fill');

      zg_fetch_header($game_user);
      echo '<div class="speech-bubble-wrapper background-color">
        <div class="wise_old_man sad"></div>
        <div class="speech-bubble">
          <p class="bonus-text">Out of ' . $luck . '!</p>
          <p>Your ' . $luck . ' has run out.</p>
          <p>Perhaps you would like to purchase some more?</p>
        </div>
      </div>';
      zg_button('elders_ask_purchase', t('Purchase more @luck', ['@luck' => $luck]));
      db_set_active();
      return;
    }

    if ($game_user->energy < $game_user->energy_max) {
      list($amount_filled, $comment) = zg_luck_energy_offer($game_user);
      $amount_now = min($game_user->energy + $amount_filled, $game_user->energy_max * 3);
      $text = "Player {$game_user->username} refilled energy (from $game_user->energy to $amount_now, gain of $amount_filled) using 1 $luck ($game_user->luck $luck before, now 1 less).";
      if (strlen($comment)) {
        $text .= ' (' . $comment . ')';
      }
      $sql = 'update users set energy = LEAST(energy + %d, energy_max * 3)
        where id = %d;';
      db_query($sql, $amount_filled, $game_user->id);
      zg_luck($game_user, -1, $game_user->energy, $amount_filled,
        $amount_now, $text, $fill_type, 'fill');
    }

    break;

  case 'money':

    if ($game_user->luck < 1) {
      zg_fetch_header($game_user);

      echo '<div class="land-failed">' . t('Out of @s!', ['@s' => $luck])
        . '</div>';
      echo '<div class="try-an-election-wrapper"><div
        class="try-an-election"><a href="/' . $game .
        '/elders_ask_purchase/' . $phone_id .
        '">Purchase more ' . $luck . '</div></div>';
      // FIXME replace with zg_luck().
      db_query($sql_log, $game_user->id, $fill_type, $amount_filled, $luck_remaining);
      db_set_active();
      return;
    }

    list($amount_filled, $comment) = zg_luck_money_offer($game_user);
    $amount_now = $amount_filled;
    $text = "Player {$game_user->username} bought $amount_filled money (now $amount_now) using 1 $luck ($game_user->luck $luck before, now 1 less).";
    if (strlen($comment)) {
      $text .= ' (' . $comment . ')';
    }
    $sql = 'update users set money = money + %d where id = %d;';
    db_query($sql, $amount_filled, $game_user->id);
    zg_luck($game_user, -1, $game_user->money, $amount_filled,
      $amount_now, $text, $fill_type, 'fill');
    break;

}

db_set_active();
drupal_goto($game . '/user/' . $arg2);
