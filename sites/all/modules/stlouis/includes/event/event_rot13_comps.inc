<?php

/**
 * @file event_rot13_comps.inc
 *
 * Includes specific to the Rot13 Competencies event.
 */

/**
 * Init the event.
 */
function game_event_fast_competencies_init() {
}

/**
 * HOOKS.
 */
function game_event_rot13_comps_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-text">
    You feel unsure about your competencies today
  </div>
</div>';
}

/**
 *
 */
function game_event_rot13_comps_comp_names_alter($game_user, &$comps) {
  foreach ($comps as $name => &$comp) {
    $comp = str_rot13($name);
  }
}
