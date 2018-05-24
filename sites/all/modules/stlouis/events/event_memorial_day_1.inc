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
    Memorial Day Weekend Friday
  </div>
  <div class="event-tagline">
    ~ Every 100 Disabled American Veterans ~<br>
    ~ Counts as an extra vote ~
  </div>
  <div class="event-text">
    For seated players, an extra defending vote<br>
    Otherwise, an extra challenging vote
  </div>
  <div class="event-text">
    Talking to disabled veterans is still only 100 Energy
  </div>
</div>';
}

function game_event_memorial_day_1_quest_item($game_user, $quest_item) {

  // Talk to disabled vets mission, 100 energy.
  if ($quest_item->id == 24) {
    $quest_item->required_energy = 100;
  }
}
