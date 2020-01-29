<?php

/**
 * @file
 * Jan 29, 2020 event.
 */

/**
 * Init the event.
 */
function stlouis_event_jan_29_2020_init() {
}

/* HOOKS */

/**
 * Implements hook_homepage_event_notice().
 */
function stlouis_event_jan_29_2020_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Wednesday, January 29th, 2020
  </div>
  <div class="event-tagline">
    ~ Today ~
  </div>
  <div class="event-text">
    Receive an extra 20% ' . $game_user->values . ' for each Luck
  </div>
</div>' . zg_luck_money_render_button($game_user);
}

/**
 * Implements hook_luck_money_offer().
 */
function stlouis_event_jan_29_2020_luck_money_offer($game_user, &$offer, &$comment) {
  $offer = ceil($offer * 1.2);
}

/**
 * Implements hook_cron().
 */
function stlouis_event_jan_29_2020_cron($game_user) {

  // People are getting sick!
  $sql = 'update `users` set meta_int = meta_int + 1 where meta = "" AND level > 10 order by RAND() limit 5;';
  db_query($sql);
}
