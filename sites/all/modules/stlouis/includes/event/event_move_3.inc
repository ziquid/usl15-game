<?php

/**
 * @file
 * Includes specific to the Move 3 event.
 */

/**
 * Init the event.
 */
function game_event_move_3_init() {
}

/* HOOKS */

/**
 * Implements hook_actions_to_move().
 */
function game_event_move_3_actions_to_move($game_user, &$actions_to_move) {
  $actions_to_move = min($actions_to_move, 3);
}

/**
 * Implements hook_homepage_event_notice().
 */
function game_event_move_3_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Due to traffic congestion, the Mayor\'s Office has authorized taxi subsidies
  </div>
  <div class="event-tagline">
    ~ Move to any hood for only 3 Actions ~
  </div>
  <div class="event-text">
    Today Only
  </div>
</div>';
}

/**
 * Implements hook_speed_increase().
 */
function game_event_move_3_speed_increase($game_user, &$speed_increase, &$verb) {
  $verb = 'Ride in a taxi';
}
