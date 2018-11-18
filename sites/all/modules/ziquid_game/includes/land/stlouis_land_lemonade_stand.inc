<?php

/**
 * @file
 * Lemonade Stand includes.
 *
 * Includes specific to the Lemonade Stand.
 */

/**
 * Implements hook_init().
 */
function stlouis_land_lemonade_stand_init() {
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_land_lemonade_stand_flag_show_quest_group_1105(\stdClass $game_user, \stdClass $qg) {

  // If the player has a Lemonade Stand, show the Lemonade Stand quests.
  return TRUE;
}
