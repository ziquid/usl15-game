<?php

/**
 * @file
 * Includes specific to the SupperBowl event.
 */

/**
 * Init the event.
 */
function stlouis_event_supperbowl_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_supperbowl_homepage_event_notice(\stdClass $game_user, &$event_text) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $event_text .= '
<div class="event">
  <div class="event-title">
    ~ Today Only ~
  </div>
  <div class="event-text">
    While the rest of the world watches another event, we here in St. Louis play
  </div>
  <div class="event-title">
    THE SUPPER BOWL
  </div>
  <div class="event-text">
    Fight to become "Alderman" of Forest Park
    <br>
    ~ Alder and clan at 11:59PM tonight win Luck! ~
    <br>
    <a href="/' . $game . '/move_do/' . $arg2 . '/81">Move to Forest Park</a>
    <br>
    &laquo; All moves today are only 6 Action &raquo;
  </div>
</div>';
}

/**
 * Implements hook_actions_to_move().
 */
function stlouis_event_supperbowl_actions_to_move($game_user, &$actions_to_move) {
  $actions_to_move = min($actions_to_move, 6);
}

/**
 * Implements hook_speed_increase().
 */
function stlouis_event_supperbowl_speed_increase($game_user, &$speed_increase, &$verb) {
  $verb = 'Use Supperbowl transit';
}

/**
 * Implements hook_elections_list_get_hood().
 */
function stlouis_event_supperbowl_elections_list_get_hood($game_user, &$data) {
  if ($game_user->fkey_neighborhoods_id == 81) {
    $data->has_elections = TRUE;
  }
}

/**
 * Implements hook_elections_challenge_get_hood().
 */
function stlouis_event_supperbowl_elections_challenge_get_hood($game_user, &$data) {
  if ($game_user->fkey_neighborhoods_id == 81) {
    $data->has_elections = TRUE;
  }
}
