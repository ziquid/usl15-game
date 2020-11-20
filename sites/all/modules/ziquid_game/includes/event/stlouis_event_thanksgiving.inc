<?php

/**
 * @file
 * Includes specific to Thanksgiving event.
 */

/**
 * Init the event.
 */
function stlouis_event_thanksgiving_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_thanksgiving_homepage_event_notice(\stdClass $game_user, &$event_text) {
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));

  $event_text .= '
<div class="event">
  <div class="event-title">
    ~ This Thanksgiving Week ~
  </div>
  <div class="event-text">
    Show your gratitude and help promote positivity on social media
  </div>
  <div class="event-title">
    #givethanks
  </div>
  <div class="event-text">
    Create posts on social media using the hashtag #givethanks 
  </div>
  <div class="event-text">
    Share your positivity with the world
  </div>
</div>';
}

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_thanksgiving_homepage_menu(\stdClass $game_user, &$extra_menu) {
  $extra_menu = '-thanksgiving';
}

/**
 * Implements hook_homepage_menu().
 */
function stlouis_event_thanksgiving_homepage_menu_links(\stdClass $game_user, &$extra_menu_links) {
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  $extra_menu_links = <<< EOF
<li>
  <a class="extra-menu-link portrait-only" href="/$game/extra_menu_link/$arg2">
    &nbsp;
  </a>
</li>
EOF;

}

/**
 * Implements hook_flag_show_quest_group_ID().
 */
function stlouis_event_thanksgiving_flag_show_quest_group_1011(\stdClass $game_user, \stdClass $qg) {
  return TRUE;
}
