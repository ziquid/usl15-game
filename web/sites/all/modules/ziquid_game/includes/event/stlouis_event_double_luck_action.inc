<?php

/**
 * @file
 * Includes specific to Double Luck Action event.
 */

/**
 * Init the event.
 */
function stlouis_event_double_luck_action_init() {
//  firep('double luck action!');
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_double_luck_action_homepage_event_notice(\stdClass $game_user, &$event_text) {
  $button = zg_render_button('elders_do_fill', 'Refill Now', '/action');
  $event_text .= '<div class="speech-bubble-wrapper background-color">
  <div class="wise_old_man happy">
  </div>
  <div class="speech-bubble event">
<p class="event-title">
    Double Your Action Event
  </p>
  <div class="event-tagline">
    Receive 2x your max Action for 1 Luck
  </div>
  <div class="event-text">
    ~ Jun 1 - 3 only ~ 
  </div>
  ' . $button . '
  </div>
</div>';
}

/**
 * Implements hook_luck_action_offer().
 */
function stlouis_event_double_luck_action_luck_action_offer(\stdClass $game_user, &$offer, &$comment) {
  $offer *= 2;
  $comment = 'Action offer doubled due to Double Luck Action event.';
}
