<?php

/**
 * @file
 * Includes specific to the Mother's Day event.
 */

/**
 * Init the event.
 */
function stlouis_event_mothers_day_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_mothers_day_homepage_event_notice(\stdClass $game_user, &$event_text) {
  global $game;
  $arg2 = check_plain(arg(2));

  $event_text .= '
<div class="event">
  <div class="event-title">
    ~ EXTENDED through May 18 ~
  </div>
  <div class="event-tagline">
    <a href="/' . $game . '/quest_groups/' . $arg2 . '#group-1017">Mother\'s Day In Quarantine</a>
  </div>
  <div class="event-text">
    How can you make this Mother\'s Day memorable?
  </div>
  <div class="event-title">
    ~ Second round Bonus now available ~
  </div>
</div>';
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_mothers_day_flag_show_quest_group_1017(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}
