<?php

/**
 * @file
 * Includes specific to the Christmas event.
 */

/**
 * Init the event.
 */
function stlouis_event_christmas_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_christmas_homepage_event_notice(\stdClass $game_user, &$event_text) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch ($today) {

    case '2018-12-30':
    case '2018-12-31':
      $event_text .= '
<div class="event">
  <div class="event-title">
    Christmas Quests
  </div>
  <div class="event-tagline">
    End at midnight Dec 31st
  </div>
  <div class="event-text">
    ~ Finish them while you can! ~
  </div>
</div>';
      break;
  }
}

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_christmas_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-christmas';
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1012(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1013(\stdClass $game_user, \stdClass $qg) {

  // Show group 2 only if player has received 2nd-round bonus for group 1.
  if (zg_fetch_equip_by_id($game_user, 67)->quantity) {
    return TRUE;
  }
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1014(\stdClass $game_user, \stdClass $qg) {

  // Show group 3 only if player has received 2nd-round bonus for group 2.
  if (zg_fetch_equip_by_id($game_user, 71)->quantity) {
    return TRUE;
  }
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1015(\stdClass $game_user, \stdClass $qg) {

  // Show group 4 only if player has received 2nd-round bonus for group 3.
  if (zg_fetch_staff_by_id($game_user, 28)->quantity) {
    return TRUE;
  }
}
