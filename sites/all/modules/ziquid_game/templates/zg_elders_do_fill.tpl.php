<?php

/**
 * @file
 * Fill your stats by using Luck.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * .
 */

global $game, $phone_id;

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();

/*  if ($game == 'stlouis') {

  $link = $destination ? $destination : "/$game/user/$phone_id";

  $fetch_header($game_user);

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

  db_set_active('default');

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

      $game_user = $fetch_user();
      $fetch_header($game_user);

      echo '<div class="subtitle">Amusez-vous bien !</div>';
      echo '<div class="subtitle">
        <a href="/' . $game . '/home/' . $phone_id . '">
          <img src="/sites/default/files/images/' . $game . '_continue.png"/>
        </a>
      </div>';

      db_set_active('default');
      return;

    }
*/
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
      db_set_active('default');
      return;
    }

    if ($game_user->actions < $game_user->actions_max) {
      $amount_filled = $game_user->actions_max;
      $sql = 'update users set actions = %d, luck = luck - 1
        where id = %d;';
      db_query($sql, $amount_filled, $game_user->id);
      $luck_remaining--;
    }

    // FIXME replace with zg_luck().
    db_query($sql_log, $game_user->id, $fill_type, $amount_filled, $luck_remaining);

    break;

  case 'energy':

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
      db_set_active('default');
      return;
    }

    if ($game_user->energy < $game_user->energy_max) {
      $text = 'refilling energy';
      list($amount_filled, $comment) = zg_luck_energy_offer($game_user);
      if (strlen($comment)) {
        $text .= ' (' . $comment . ')';
      }
      $sql = 'update users set energy = LEAST(energy + %d, energy_max * 3)
        where id = %d;';
      db_query($sql, $amount_filled, $game_user->id);
      zg_luck($game_user, -1, $game_user->energy, $amount_filled,
        min($game_user->energy + $amount_filled, $game_user->energy_max * 3),
        $text, $fill_type, '');
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
      db_set_active('default');
      return;
    }

    $amount_filled = zg_luck_money_offer($game_user);
    $sql = 'update users set money = money + %d, luck = luck - 1
      where id = %d;';
    db_query($sql, $amount_filled, $game_user->id);
    // FIXME replace with zg_luck().
    $luck_remaining--;
    db_query($sql_log, $game_user->id, $fill_type, $amount_filled, $luck_remaining);
    break;

}

db_set_active('default');
drupal_goto($game . '/user/' . $arg2);
