<?php

/**
 * @file
 * Includes specific to Hood 79.
 */

/**
 * Init the event.
 */
function stlouis_hood_79_init() {
  firep('hood 79!');
}

/* HOOKS */

/**
 * Implements hook_move_to_succeeded().
 */
function stlouis_hood_79_move_to_succeeded(\stdClass $game_user, $old_hood_id, $new_hood_id) {
  if ($new_hood_id == 79) {
    zg_song($game_user, 'Mr roboto intro', FALSE);
  }
}
