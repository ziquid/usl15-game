<?php

/**
 * @file event_memorial_day_3.inc
 *
 * Memorial Day event, Day 3 (Sunday).
 */

/**
 * Init the event.
 */
function game_event_memorial_day_3_init() {
}

// HOOKS

function game_event_memorial_day_3_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Memorial Day Weekend Sunday
  </div>
  <div class="event-tagline">
    ~ Every 60 Disabled American Veterans ~<br>
    ~ Counts as an extra vote ~<br>
    and<br>
    ~ Reduces hood movement by 1 Action ~<br>
    (min 4 Actions)
  </div>
  <div class="event-text">
    Talking to disabled veterans is now only 60 Energy
  </div>
</div>';
}

function game_event_memorial_day_3_quest_item($game_user, $quest_item) {

  // Talk to disabled vets mission, 60 energy.
  if ($quest_item->id == 24) {
    $quest_item->required_energy = 60;
  }
}

function game_event_memorial_day_3_extra_votes($game_user, &$extra_votes, &$extra_defending_votes) {

  // Every 60 DAVs is an extra vote.
  $divisor = 60;
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

function game_event_memorial_day_3_actions_to_move($game_user, &$actions_to_move) {

  // Every 60 DAVs reduces actions to move, min 4.
  $divisor = 60;
  $sql = 'SELECT quantity FROM staff_ownership
    WHERE fkey_users_id = %d AND fkey_staff_id = 16;';
  $result = db_query($sql, $game_user->id);
  $vets = db_fetch_object($result);
  $vet_bonus = floor($vets->quantity / $divisor);

  $actions_to_move = max($actions_to_move - $vet_bonus, 4);
}
