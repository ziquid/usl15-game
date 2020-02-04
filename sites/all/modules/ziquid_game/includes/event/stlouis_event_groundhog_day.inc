<?php

/**
 * @file
 * Includes specific to the GroundHog Day event.
 */

/**
 * Init the event.
 */
function stlouis_event_groundhog_day_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_groundhog_day_homepage_event_notice(\stdClass $game_user, &$event_text) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch ($today) {

    case '2020-02-01':
    case '2020-02-02':
    case '2020-02-03':
    case '2020-02-04':
    case '2020-02-05':
    case '2020-02-06':
    case '2020-02-07':
      $event_text .= '
<div class="event">
  <div class="event-title">
    ~ This Weekend ~
  </div>
  <div class="event-tagline">
    <a href="/' . $game . '/quest_groups/' . $arg2 . '#group-1016">Groundhog Day Missions</a>
  </div>
  <div class="event-text">
    Missions End Friday at 11:59 PM
  </div>
</div>';
      break;
  }
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_groundhog_day_flag_show_quest_group_1016(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}
