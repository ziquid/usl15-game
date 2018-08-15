<?php

/**
 * @file event_fast_competencies.inc
 *
 * Includes specific to the Fast Competencies event.
 */

/**
 * Init the event.
 */
function game_event_fast_competencies_init() {
}

/**
 * HOOKS.
 */
function game_event_fast_competencies_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Fast Competencies Today
  </div>
  <div class="event-tagline">
    ~ Competencies increase after only 30 seconds ~
  </div>
  <div class="event-text">
    Today Only
  </div>
</div>';
}
