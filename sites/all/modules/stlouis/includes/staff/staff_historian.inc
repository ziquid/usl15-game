<?php

/**
 * @file staff_historian.inc
 *
 * Includes specific to the Historian staff member.
 */

/**
 * Init the staff member.
 */
function game_staff_historian_init() {
  // dpm('Historian reporting for duty!');.
}

/**
 * HOOKS.
 */
function game_staff_historian_homepage_event_notice($game_user, &$event_text) {
  return;
  $event_text .= '
<div class="event">
  <div class="event-title">
    You have a historian!
  </div>
</div>';
}

/**
 *
 */
function game_staff_historian_game_fetch_user_post($game_user) {
  $game_user->traits['show_highlighted_quest_groups_on_map'] = TRUE;
}
