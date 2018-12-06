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
