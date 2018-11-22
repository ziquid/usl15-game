<?php

/**
 * @file
 * Includes specific to Thanksgiving event.
 */

/**
 * Init the event.
 */
function stlouis_event_thanksgiving_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_thanksgiving_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-thanksgiving';
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_thanksgiving_flag_show_quest_group_1011(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}
