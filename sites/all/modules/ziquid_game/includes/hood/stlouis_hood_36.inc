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
  if ($new_hood_id == 36) {
    zg_song($game_user, 'Holiday', FALSE);
  }
}
