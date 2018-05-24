<?php

/**
 * @file event_memorial_day_1.inc
 *
 * Memorial Day event, Day 1.
 */

/**
 * Init the event.
 */
function game_event_memorial_day_1_init() {
}

// HOOKS

function game_event_memorial_day_1_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Memorial Day Weekend, Day 1
  </div>
  <div class="event-tagline">
    ~ Every 100 Disabled American Veterans ~<br>
    ~ Counts as an extra vote ~
  </div>
  <div class="event-text">
    For seated players, an extra defending vote<br>
    Otherwise, an extra challenging vote
  </div>
</div>';
}
