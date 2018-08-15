<?php

/**
 * @file event_poll_jun_01_2018.inc
 *
 * A link to an external poll.
 */

/**
 * Init the event.
 */
function game_event_poll_jun_01_2018_init() {
}

/**
 * HOOKS.
 */
function game_event_poll_jun_01_2018_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    USL:CE Poll
  </div>
  <div class="event-tagline">
    ~ How best to announce new features? ~<br>
  </div>
  <div class="event-text">
    Provide a valid e-mail address and referral code and receive <strong>5 Luck</strong>
  </div>
  <div class="event-text">
    <a href="external://goo.gl/forms/wrmSZQno8sOzYLPE3">Take the poll here</a>
  </div>
</div>';
}
