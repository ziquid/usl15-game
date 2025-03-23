<?php

/**
 * @file
 * Dog Walking includes.
 *
 * Includes specific to Dog Walking.
 */

/**
 * Implements hook_init().
 */
function stlouis_land_dog_walking_init() {
}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_land_dog_walking_flag_show_quest_group_1106(\stdClass $game_user, \stdClass $qg) {

  // If the player has gained Dog Walking, show the Dog Walking quests.
  return TRUE;
}
