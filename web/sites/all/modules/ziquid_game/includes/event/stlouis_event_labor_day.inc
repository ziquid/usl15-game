<?php

/**
 * @file
 * Includes specific to the Labor Day event.
 */

/**
 * Init the event.
 */
function stlouis_event_labor_day_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_labor_day_homepage_event_notice(\stdClass $game_user, &$event_text) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch ($today) {

    case '2020-09-07':
    case '2020-09-08':
    case '2020-09-09':
    case '2020-09-10':
      $event_text .= '
<div class="event">
  <div class="event-title">
    ~ This Week ~
  </div>
  <div class="event-tagline">
    <a href="/' . $game . '/quest_groups/' . $arg2 . '#group-1009">Labor Day Missions</a>
  </div>
  <!--<div class="event-text">
    Missions End Sunday at 11:59 PM
  </div>-->
</div>';
      break;
  }
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_labor_day_flag_show_quest_group_1009(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}
