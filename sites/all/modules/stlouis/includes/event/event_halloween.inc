<?php

/**
 * @file
 * Includes specific to Halloween event.
 */

/**
 * Init the event.
 */
function game_event_halloween_init() {
}

// HOOKS

/**
 * Implements hook_homepage_menu.
 */
function game_event_halloween_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-halloween';
}

