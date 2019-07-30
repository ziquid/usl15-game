<?php

/**
 * @file stlouis_block_user_toggle.tpl.php
 * Toggle whether to block a user or not.
 *
 * Synced with CG: no
 * Synced with 2114: yes
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

include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);

if (empty($game_user->username)) {
  db_set_active('default');
  drupal_goto($game . '/choose_name/' . $arg2);
}

 // User id.
if (substr($arg3, 0, 3) == 'id:') {
  $sql = 'select username from users where id = %d;';
  $result = db_query($sql, (int) substr($arg3, 3));
  $item = db_fetch_object($result);
  $target_id = (int) substr($arg3, 3);
}
else { // phone_id
  $sql = 'select id, username from users where phone_id = "%s";';
  $result = db_query($sql, $phone_id_to_block);
  $item = db_fetch_object($result);
  $target_id = $item->id;
}

$sql = 'select * from message_blocks where fkey_blocked_users_id = %d
  and fkey_blocking_users_id = %d;';
$result = db_query($sql, $target_id, $game_user->id);
$block = db_fetch_object($result);

// Block doesn't exist - create one.
if (empty($block)) {
  $sql = 'delete from user_messages where fkey_users_from_id = %d
    and fkey_users_to_id = %d;';
  db_query($sql, $target_id, $game_user->id);

  $sql = 'insert into message_blocks
    (fkey_blocked_users_id, fkey_blocking_users_id) values (%d, %d);';
  db_query($sql, $target_id, $game_user->id);

  echo <<< EOF
<div class="title">Block player $item->username</div>
<div class="subtitle"><a href="/$game/user/$arg2/$arg3">$item->username</a>
can no longer send you messages</div>
EOF;
}
else {

  // Delete block.
  $sql = 'delete from message_blocks
    where fkey_blocked_users_id = %d
    and fkey_blocking_users_id = %d;';
  db_query($sql, $target_id, $game_user->id);

  echo <<< EOF
<div class="title">Unblock player $item->username</div>
<div class="subtitle"><a href="/$game/user/$arg2/$arg3">$item->username</a>
can again send you messages</div>
EOF;
}

game_button();
db_set_active('default');
