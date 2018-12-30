<?php

/**
 * @file
 * Socialite includes.
 *
 * Includes specific to the Socialite.
 */

/**
 * Implements hook_init().
 */
function stlouis_staff_socialite_init() {
}

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_staff_socialite_homepage_event_notice(\stdClass $game_user, &$event_text) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch ($today) {

    case '2018-12-06':
    case '2018-12-07':
    case '2018-12-08':
    case '2018-12-09':
      $event_text .= '
<div class="event">
  <div class="event-tagline">
    Your Socialite texts you:
  </div>
</div>' . zg_render_button('quest_groups', 'good news msg me back pls', '#group-1101');
      break;
  }
}

/**
 * Implements hook_homepage_messages().
 */
function stlouis_staff_socialite_homepage_messages(\stdClass $game_user, &$messages) {
  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch ($today) {

    case '2018-12-30':
      $message = (object) [
        'timestamp' => '2018-12-30 01:11:11',
        'message' => 'Hey &mdash; the Mayor is hosting his own New Year\'s Eve party tomorrow downtown.&nbsp; Might be good to go there; I\'ll see if I can get you in.&nbsp; More details later.',
        'username' => 'The Socialite',
        'phone_id' => '',
        'ep_name' => '',
        'is_clan_leader' => '0',
        'clan_acronym' => '',
        'private' => '1',
        'type' => 'user',
        'subtype' => '',
      ];
      array_unshift($messages, $message);
      break;
  }
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_staff_socialite_flag_show_quest_group_1101(\stdClass $game_user, \stdClass $qg) {

  // If the player has a Socialite, show Socialite quests.
  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch ($today) {

    case '2018-12-06':
    case '2018-12-07':
    case '2018-12-08':
    case '2018-12-09':
      return TRUE;
  }

  return FALSE;
}
