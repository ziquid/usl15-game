<?php

/**
 * @file
 * Includes specific to Hood 42 (Clayton/Tamm).
 */

/**
 * Init the event.
 */
function stlouis_hood_42_init() {
//   firep('Clayton/Tamm!');
}

/* HOOKS */

/**
 * Implements hook_move_to_succeeded().
 */
function stlouis_hood_42_move_to_succeeded(\stdClass $game_user, $old_hood_id, $new_hood_id) {
  static $songs = [
    'Another Brick in the Wall teaser',
  ];

  if (($new_hood_id == 42) && (mt_rand(0, 1) == 1)) {
    zg_song($game_user, $songs[mt_rand(0, count($songs) - 1)], FALSE);
  }
}
