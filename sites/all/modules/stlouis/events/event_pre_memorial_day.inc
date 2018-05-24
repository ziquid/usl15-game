<?php

/**
 * @file event_pre_memorial_day.inc
 *
 * Pre-Memorial Day event.
 */

/**
 * Init the event.
 */
function game_event_pre_memorial_day_init() {
}

// HOOKS

function game_event_pre_memorial_day_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Public Service Announcement
  </div>
  <div class="event-tagline">
    ~ The Mayor\'s Office reminds you ~<br>
    ~ Remember your Veterans this Memorial Day weekend ~
  </div>
</div>';
}
