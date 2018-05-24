<?php

/**
 * @file event_memorial_day_1.inc
 *
 * Memorial Day event, Day 1.
 */

/**
 * Init the event.
 */
function game_event_memorial_day_1_init() {
}

// HOOKS

function game_event_memorial_day_1_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Memorial Day Weekend Friday
  </div>
  <div class="event-tagline">
    ~ Every 100 Disabled American Veterans ~<br>
    ~ Counts as an extra vote ~
  </div>
  <div class="event-text">
    For seated players, an extra defending vote<br>
    Otherwise, an extra challenging vote
  </div>
  <div class="event-text">
    Talking to disabled veterans is still only 100 Energy
  </div>
</div>';
}

function game_event_memorial_day_1_quest_item($game_user, $quest_item) {

  // Talk to disabled vets mission, 100 energy.
  if ($quest_item->id == 24) {
    $quest_item->required_energy = 100;
  }
}

function game_event_memorial_day_1_extra_votes($game_user, &$extra_votes, &$extra_defending_votes) {

  // Every 100 DAVs is an extra vote.
  $divisor = 100;
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
  firep($message, 'game_event_memorial_day_1_extra_votes.');
}
