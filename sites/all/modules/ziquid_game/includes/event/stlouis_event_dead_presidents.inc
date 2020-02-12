<?php

/**
 * @file
 * Includes specific to the Dead Presidents event.
 */

/**
 * Init the event.
 */
function stlouis_event_dead_presidents_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_dead_presidents_homepage_event_notice(\stdClass $game_user, &$event_text) {

  $event_text .= '
<div class="event">
  <div class="event-title">
    ~ February 17, 2020 ~
  </div>
  <div class="event-tagline">
    $$$ DEAD PRESIDENTS $$$
  </div>
  <div class="event-text">
    No Mission.&nbsp; No Job.&nbsp; No Income.
  <br>
    Only one thing to do:
  </div>
  <div class="event-text">
    Steal <span class="highlight">A LOT</span> of money.
  </div>
</div>';
}

/**
 * Implements hook_actions_to_move().
 */
function stlouis_event_dead_presidents_actions_to_move($game_user, &$actions_to_move) {
  $today = date('Y-m-d', REQUEST_TIME - 3600);
  if ($today == '2020-02-17') {
    $actions_to_move = min($actions_to_move, 6);
  }
}
