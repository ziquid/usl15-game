<?php

/**
 * @file event_flag_day.inc
 *
 * Includes specific to the Flag Day event.
 */

/**
 * Init the event.
 */
function game_event_flag_day_init() {
}

// HOOKS

function game_event_flag_day_homepage_event_notice($game_user, &$event_text) {
  $event_text .= '
<div class="event">
  <div class="event-title">
    Flag Day Ceremonies
  </div>
  <div class="event-tagline">
    ~ Ceremonies today in Jeff VanderLou ~
  </div>
  <div class="event-text">
    All elected officials in Jeff VanderLou receive a commemorative flag (+14 init, no upkeep) every 15 minutes
  </div>
</div>';
}

function game_event_flag_day_cron() {
  $sql = 'SELECT users.* FROM `users`
    inner join elected_officials on elected_officials.fkey_users_id = users.id 
    WHERE fkey_neighborhoods_id = 59';
  $result = db_query($sql);
  $data = [];
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
echo count($data) . ' officials will get flags.' . PHP_EOL;
  $msg = 'Because of your attendance at today\'s flag ceremony, you have been
    awarded a ceremonial flag.';
  foreach ($data as $item) {
    game_equipment_gain($item, 44, 1);
    game_send_user_message(1, $item->id, 1, $msg, 'comp');
  }
}
