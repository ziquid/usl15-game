<?php

/**
 * @file stlouis_party_msg.tpl.php
 * Stlouis party message page
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

global $game, $phone_id;

include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);

if (empty($game_user->username)) {
  drupal_goto($game . '/choose_name/' . $arg2);
}

$target = check_plain($_GET['target']);
//firep($target);

$message_orig = check_plain($_GET['message']);
$message = _stlouis_filter_profanity($message_orig);

if (strlen($message) < 3) {
  $fetch_header($game_user);
  echo '<div class="message-error">Your message must be at least 3
    characters long.</div>';
  echo '<div class="election-continue"><a href="/' . $game . '/home/' .
    $arg2 . '?message=' . $message_orig . '">' .
    t('Try again') . '</a></div>';

  db_set_active('default');
  return;
}

if (substr($message, 0, 3) == 'XXX') {
  $fetch_header($game_user);
  echo '<div class="message-error">Your message contains words that are not
    allowed.&nbsp; Please rephrase.&nbsp; ' . $message . '</div>';
  echo '<div class="election-continue"><a href="/' . $game . '/home/' .
    $arg2 . '?message=' . $message_orig . '">' .
    t('Try again') . '</a></div>';

  db_set_active('default');
  return;
}

if (!empty($message)) {

  switch ($target) {

    case 'neighborhood':
      $sql = 'insert into neighborhood_messages (fkey_users_from_id,
        fkey_neighborhoods_id, message) values (%d, %d, "%s");';
      db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
        $message);
      break;

    case 'clan':
      $sql = 'insert into clan_messages (fkey_users_from_id,
        fkey_neighborhoods_id, message) values (%d, %d, "%s");';
      db_query($sql, $game_user->id, $game_user->fkey_clans_id,
        $message);
      break;

    case 'values':
      $sql = 'insert into values_messages (fkey_users_from_id,
        fkey_values_id, fkey_neighborhoods_id, message)
        values (%d, %d, %d, "%s");';
      db_query($sql, $game_user->id, $game_user->fkey_values_id,
        $game_user->fkey_neighborhoods_id, $message);
      break;

  }

  db_set_active('default');
  drupal_goto($game . '/home/' . $arg2);
}

db_set_active('default');
