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

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_st_pattys_homepage_event_notice(\stdClass $game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    ~ St Patty\'s Day ~
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
  $game_user->username = 'Blarney';
}

/**
 * Implements hook_get_all_messages().
 */
function stlouis_event_st_pattys_get_all_messages(\stdClass $game_user, &$messages) {
  foreach ($messages as &$msg) {
    $new_user_name = zg_irish_name();
    firep($msg, 'msg object');
    $msg->message = str_replace($msg->username, $new_user_name, $msg->message);
    $msg->username = $new_user_name;
  }
}

/**
 * Implements hook_get_new_user_messages().
 */
function stlouis_event_st_pattys_get_new_user_messages(\stdClass $game_user, &$messages) {
  return stlouis_event_st_pattys_get_all_messages($game_user, $messages);
}
