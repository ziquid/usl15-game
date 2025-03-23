<?php

/**
 * @file
 * Includes specific to Double Luck Energy event.
 */

/**
 * Init the event.
 */
function stlouis_event_double_luck_energy_init() {
//  firep('double luck energy!');
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_double_luck_energy_homepage_event_notice(\stdClass $game_user, &$event_text) {
  $event_text .= '
<div class="event">
<div class="event-title">
    Double Your Energy
  </div>
  <div class="event-tagline">
    Receive 2x your max Energy for 1 Luck
  </div>
  <div class="event-text">
    ~ Dec 13 - 17 only ~ 
  </div>

</div>';
}

/**
 * Implements hook_luck_energy_offer().
 */
function stlouis_event_double_luck_energy_luck_energy_offer(\stdClass $game_user, &$offer, &$comment) {
  $offer *= 2;
  $comment = 'Energy offer doubled due to Double Luck Energy event.';
}
