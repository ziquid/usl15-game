<?php

/**
 * @file
 * Includes specific to the Independence Day event.
 */

/**
 * Init the event.
 */
function stlouis_event_independence_day_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_independence_day_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-fireworks';
}
