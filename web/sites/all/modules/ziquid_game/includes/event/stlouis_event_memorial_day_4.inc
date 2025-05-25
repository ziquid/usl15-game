<?php

/**
 * @file event_memorial_day_4.inc
 *
 * Memorial Day event, Day 4 (Monday).
 */

/**
 * Init the event.
 */
function stlouis_event_memorial_day_4_init() {
}

// HOOKS

function stlouis_event_memorial_day_4_homepage_event_notice(\stdClass $game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Memorial Day Weekend Monday
  </div>
  <div class="event-tagline">
    ~ Every 50 Disabled American Veterans ~<br>
    ~ Counts as an extra vote ~<br>
    and<br>
    ~ Reduces hood movement by 1 Action ~<br>
    (min 2 Actions)
  </div>
  <div class="event-text">
    Talking to disabled veterans is now only 50 Energy<br>
    and<br>
    !! All PNG costs are halved !!
  </div>
</div>';
}

function stlouis_event_memorial_day_4_quest_item(\stdClass $game_user, \stdClass $quest_item) {

  // Talk to disabled vets mission, 50 energy.
  if ($quest_item->id == 24) {
    $quest_item->required_energy = 50;
  }
}

function stlouis_event_memorial_day_4_extra_votes(\stdClass $game_user, &$extra_votes, &$extra_defending_votes) {

  // Every 50 DAVs is an extra vote.
  $divisor = 50;
  $sql = 'SELECT quantity FROM staff_ownership
    WHERE fkey_users_id = %d AND fkey_staff_id = 16;';
  $result = db_query($sql, $game_user->id);
  $vets = db_fetch_object($result);
  $vet_bonus = floor($vets->quantity / $divisor);

  // Add it to extra votes or extra defending votes,
  // depending on whether seated or not.
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

function stlouis_event_memorial_day_4_actions_to_move(\stdClass $game_user, &$actions_to_move) {

  // Every 50 DAVs reduces actions to move, min 2.
  $divisor = 50;
  $sql = 'SELECT quantity FROM staff_ownership
    WHERE fkey_users_id = %d AND fkey_staff_id = 16;';
  $result = db_query($sql, $game_user->id);
  $vets = db_fetch_object($result);
  $vet_bonus = floor($vets->quantity / $divisor);

  $actions_to_move = max($actions_to_move - $vet_bonus, 2);
}

function stlouis_event_memorial_day_4_png_cost(\stdClass $game_user, &$money_required, &$actions_required) {

  // Halve the money and actions required.
  $money_required = floor($money_required / 2);
  $actions_required = floor($actions_required / 2);
}
