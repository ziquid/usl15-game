<?php

/**
 * @file
 * Includes specific to Hood 36 (Downtown West).
 */

/**
 * Init the event.
 */
function stlouis_hood_36_init() {
  firep('Downtown West!');
}

/* HOOKS */

/**
 * Implements hook_move_to_succeeded().
 */
function stlouis_hood_36_move_to_succeeded(\stdClass $game_user, $old_hood_id, $new_hood_id) {
  static $songs = [
    'Basket Case',
    'Boulevard of Broken Dreams',
    'Holiday',
    'When September Ends',
  ];

  if ($new_hood_id == 36) {
    zg_song($game_user, $songs[mt_rand(0, count($songs) - 1)], FALSE);
  }
}
