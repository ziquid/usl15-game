<?php

/**
 * @file Event_pre_memorial_day.inc.
 *
 * Pre-Memorial Day event.
 */

/**
 * Init the event.
 */
function game_event_pre_memorial_day_init() {
}

/**
 * HOOKS.
 */
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
  <div class="event-text">
    Starting now, talking to disabled veterans only requires 100 Energy<br>
    Friday: Every 100 Disabled American Veterans<br>
    counts as an extra vote
  </div>
</div>';
}

/**
 *
 */
function game_event_pre_memorial_day_quest_item($game_user, $quest_item) {

  // Talk to disabled vets mission, 100 energy.
  if ($quest_item->id == 24) {
    $quest_item->required_energy = 100;
  }
}
