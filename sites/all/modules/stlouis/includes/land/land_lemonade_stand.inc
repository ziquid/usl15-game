<?php

/**
 * @file
 * Lemonade Stand includes.
 *
 * Includes specific to the Lemonade Stand.
 */

/**
 * Init the include.
 */
function game_land_lemonade_stand_init() {
//  firep('init lemonade stand!');
}

// HOOKS

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function game_land_lemonade_stand_flag_show_quest_group_1105(\stdClass $game_user, \stdClass $qg) {

  // If the player has a Lemonade Stand, show the Lemonade Stand quests.
  return TRUE;
}
