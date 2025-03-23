<?php

/**
 * @file
 * Includes specific to the Stats-only Hood event.
 */

/**
 * Init the event.
 */
function stlouis_event_stats_only_hood_init() {
}

/* HOOKS */

function stlouis_event_stats_only_hood_homepage_event_notice($stlouis_user, &$event_text) {
  $hood = stlouis_event_stats_only_hood_get_todays_hood();
  $event_text .= '
<div class="event">
  <div class="event-title">
    From now to July 4th
  </div>
  <div class="event-tagline">
    ~ Stats-only elections ~
  </div>
  <div class="event-text">
    Each day, a selected hood has stats-only elections<br>
    (No clan or party loyalty)
  </div>
  <div class="event-text">
    Today\'s hood: ' . $hood->name . '
  </div>

</div>';
}

function stlouis_event_stats_only_hood_get_todays_hood() {
  $hood = new stdClass();
  $today = date('Y-m-d', REQUEST_TIME - 3600);
  switch($today) {

    case '2018-06-30':
      $hood->id = 36;
      $hood->name = 'Downtown West';
      break;

    case '2018-07-01':
      $hood->id = 37;
      $hood->name = 'Midtown';
      break;

    case '2018-07-02':
      $hood->id = 41;
      $hood->name = 'Cheltenham';
      break;

    case '2018-07-03':
      $hood->id = 40;
      $hood->name = 'Kings Oak';
      break;

    case '2018-07-04':
      $hood->id = 0;
      $hood->name = 'All Hoods!';
      break;
  }

  return $hood;
}

function stlouis_event_stats_only_hood_elections_challenge_get_hood($stlouis_user, &$hood_info) {
  $today = date('Y-m-d', REQUEST_TIME - 3600);
  $hood = stlouis_event_stats_only_hood_get_todays_hood();
  firep($hood_info, 'hood info before');

  if ($hood_info->id == $hood->id || $today == '2018-07-04') {
    $hood_info->stats_only_hood = TRUE;
  }

  firep($hood_info, 'hood info after');
}
