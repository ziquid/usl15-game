<?php

/**
 * @file event_memorial_day_2.inc
 *
 * Memorial Day event, Day 2 (Saturday).
 */

/**
 * Init the event.
 */
function game_event_memorial_day_2_init() {
}

/**
 * HOOKS.
 */
function game_event_memorial_day_2_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Memorial Day Weekend Saturday
  </div>
  <div class="event-tagline">
    ~ Every 75 Disabled American Veterans ~<br>
    ~ Counts as an extra vote ~
  </div>
  <div class="event-text">
    For seated players, an extra defending vote<br>
    Otherwise, an extra challenging vote
  </div>
  <div class="event-text">
    Talking to disabled veterans is now only 75 Energy
  </div>
</div>';
}

/**
 *
 */
function game_event_memorial_day_2_quest_item($game_user, $quest_item) {

  // Talk to disabled vets mission, 75 energy.
  if ($quest_item->id == 24) {
    $quest_item->required_energy = 75;
  }
}

/**
 *
 */
function game_event_memorial_day_2_extra_votes($game_user, &$extra_votes, &$extra_defending_votes) {

  // Every 75 DAVs is an extra vote.
  $divisor = 75;
  $sql = 'SELECT quantity FROM staff_ownership
    WHERE fkey_users_id = %d AND fkey_staff_id = 16;';
  $result = db_query($sql, $game_user->id);
  $vets = db_fetch_object($result);
  $vet_bonus = floor($vets->quantity / $divisor);

  // Add it to extra votes or extra defending votes, depending on whether seated or not.
  if ($game_user->ep_name) {
    $extra_defending_votes += $vet_bonus;
    $defending = 'defending';
  }
  else {
    $extra_votes += $vet_bonus;
    $defending = '';
  }

  $message = 'Your ' . $vets->quantity . ' Disabled American Veterans give you an extra '
    . $vet_bonus . ' ' . $defending . ' votes.';
  firep($message, 'memorial_day extra_votes.');
}
