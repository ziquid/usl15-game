<?php

/**
 * @file
 * Includes specific to St Patty's Day event.
 */

/**
 * Init the event.
 */
function stlouis_event_st_pattys_init() {
}

/**
 * Return the Irish name of the user.
 *
 * @param object $game_user
 *   The game user object.
 *
 * @return string
 *   The Irish name.
 */
function stlouis_event_st_pattys_get_irish_name(&$game_user) {
  firep(['id' => $game_user->id, 'username' => $game_user->username, 'meta' => $game_user->meta],
    'game user object looking for an irish name');

  static $irish_names = [];

  if (array_key_exists($game_user->id, $irish_names)) {
    firep($irish_names[$game_user->id], 'irish name cached in static array');
    return $irish_names[$game_user->id];
  }

  if (substr($game_user->meta, 0, 6) == 'irish:') {
    firep($game_user->meta, 'irish name cached in user object');
    $irish_names[$game_user->id] = substr($game_user->meta, 6);
    return substr($game_user->meta, 6);
  }

  $irish_names[$game_user->id] = $new_irish_name = zg_irish_name();
  firep($new_irish_name, 'new irish name needed');

  if (!strlen($game_user->meta)) {
    $game_user->meta = 'irish:' . $new_irish_name;
    $sql = 'update users set meta = "%s" where id = %d;';
    db_query($sql, 'irish:' . $new_irish_name, $game_user->id);
  }

  return $new_irish_name;
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_st_pattys_homepage_event_notice(\stdClass $game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    &#127808; Happy St Patty\'s Day &#127808;
  </div>
</div>';
}

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_st_pattys_homepage_menu(\stdClass $game_user, &$extra_menu) {
//  $extra_menu = '-st_pattys';
}

/**
 * Implements hook_fetch_user().
 */
function stlouis_event_st_pattys_fetch_user(\stdClass $game_user) {
  $game_user->username = stlouis_event_st_pattys_get_irish_name($game_user);
}

/**
 * Implements hook_get_all_messages().
 */
function stlouis_event_st_pattys_get_all_messages(\stdClass $game_user, &$messages) {
  foreach ($messages as &$msg) {
    $msg_user = zg_fetch_user_by_id((int) $msg->fkey_users_from_id);
    $new_user_name = stlouis_event_st_pattys_get_irish_name($msg_user);
    //    firep($msg_user, 'msg user object');
    $msg->message = str_replace($msg->username, $new_user_name, $msg->message);
    $msg->username = $new_user_name;
    firep($msg, 'msg object');
  }
}

/**
 * Implements hook_get_new_user_messages().
 */
function stlouis_event_st_pattys_get_new_user_messages(\stdClass $game_user, &$messages) {
  return stlouis_event_st_pattys_get_all_messages($game_user, $messages);
}

/**
 * Implements hook_debates_list().
 */
function stlouis_event_st_pattys_debates_list(\stdClass $game_user, &$player) {
  $player->username = stlouis_event_st_pattys_get_irish_name($player);
}

/**
 * Implements hook_debates_challenge().
 */
function stlouis_event_st_pattys_debates_challenge(\stdClass $game_user, &$player) {
  $player->username = stlouis_event_st_pattys_get_irish_name($player);
}
