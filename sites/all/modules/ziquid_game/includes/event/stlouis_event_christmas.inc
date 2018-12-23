<?php

/**
 * @file
 * Includes specific to the Christmas event.
 */

/**
 * Init the event.
 */
function stlouis_event_christmas_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_christmas_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-christmas';
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1012(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1013(\stdClass $game_user, \stdClass $qg) {

  // Show group 2 only if player has received 2nd-round bonus for group 1.
  if (zg_fetch_equip_by_id($game_user, 67)->quantity) {
    return TRUE;
  }
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1014(\stdClass $game_user, \stdClass $qg) {
//  return TRUE;
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_christmas_flag_show_quest_group_1015(\stdClass $game_user, \stdClass $qg) {
//  return TRUE;
}
