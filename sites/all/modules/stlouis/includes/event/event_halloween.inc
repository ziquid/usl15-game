<?php

/**
 * @file
 * Includes specific to Halloween event.
 */

/**
 * Init the event.
 */
function game_event_halloween_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_menu().
 */
function game_event_halloween_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-halloween';
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function game_event_halloween_flag_show_quest_group_1010(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}
