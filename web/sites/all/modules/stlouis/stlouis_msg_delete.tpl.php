<?php

/**
 * @file stlouis_msg_delete.tpl.php
 * Delete a message sent to you.
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

// Check permissions.
$sql = 'select fkey_users_from_id, fkey_users_to_id
  from user_messages
  where id = %d;';
$result = db_query($sql, $msg_id);
$msg = db_fetch_object($result);

// Not origin or recipient of msg?
if ($msg->fkey_users_to_id != $game_user->id && $msg->fkey_users_from_id != $game_user->id) {

  // FIXME jwc 10Apr2014 -- deduct karma.
  db_set_active();
  drupal_goto($game . '/home/' . $arg2);
}

zg_competency_gain($game_user, 'pruner');

$sql = 'delete from user_messages where id = %d;';
db_query($sql, $msg_id);

db_set_active();
drupal_goto($game . '/user/' . $arg2);
