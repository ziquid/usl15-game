<?php

/**
 * @file
 * Stlouis welcome page.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done
 * Ready for MVC separation: no
 */

global $game, $phone_id;

// We won't have gone through fetch_user() yet, so set these here.
$game = check_plain(arg(0));
$get_phoneid = '_' . $game . '_get_phoneid';
$phone_id = $get_phoneid();
$arg2 = check_plain(arg(2));
$ip_address = ip_address();

db_set_active('game_' . $game);

$default_neighborhood = 81;
$default_value = 'Greenbacks';

// Check to make sure not too many from the same IP address.
$sql = 'select count(`value`) as count from user_attributes
  where `key` = "last_IP" and `value` = "%s";';
$result = db_query($sql, $ip_address);
$item = db_fetch_object($result);
if ($item->count > 5) {
  $sql = 'select * from ip_whitelist where ip_address = "%s";';
  $result = db_query($sql, $ip_address);
  $ips = db_fetch_object($result);
  if (empty($ips)) {
    db_set_active('default');
    drupal_goto($game . '/error/' . $arg2 . '/E-2242');
  }
}

// Show welcome message.
echo <<< EOF
<div class="title">
  <img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<div class="tagline">
  &bull; Become the Mayor &bull;
</div>
EOF;

switch ($_REQUEST['page']) {

  case 2:
    echo <<< EOF
<div class="welcome">
<div class="city-icon">
</div>
<div class="subtitle">
  How to play
</div>
<ul>
  <li>Finish missions to earn skills and influence</li>
  <li>Cooperate and compete with other players to achieve your goals</li>
  <li>Purchase investments and equipment to earn money and win votes</li>
  <li>Become a city elder, political party leader, and then mayor</li>
</ul>
</div>
EOF;

    $sql = 'insert into users set phone_id = "%s", username = "(new player)",
      experience = 0, level = 1, fkey_neighborhoods_id = %d, fkey_values_id = 0,
      `values` = "%s", money = 500, energy = 200, energy_max = 200';
    $result = db_query($sql, $phone_id, $default_neighborhood,
      $default_value);

    $sql = 'insert into user_creations set datetime = "%s", phone_id = "%s",
      remote_ip = "%s";';
    $result = db_query($sql, date('Y-m-d H:i:s'), $phone_id, $ip_address);

    $fetch_user = '_' . $game . '_fetch_user';
    $game_user = $fetch_user();

    // Notify all party welcome comm members.
    $sql = 'SELECT users.id FROM `users`
      left join elected_officials eo on users.id = eo.fkey_users_id
      left join elected_positions ep on eo.fkey_elected_positions_id = ep.id
      WHERE ep.gets_new_user_notifications = 1;';
    $result = db_query($sql);
    $data = [];
    while ($item = db_fetch_object($result)) {
      $data[] = $item->id;
    }

    $msg = 'I am a new user who has just joined the game.  Please welcome me.';
    game_send_user_message($game_user->id, $data, 0, $msg, 'user');
/*    if (game_get_value($game_user, 'enabled_alpha')) { */
      $links = 'quest_groups';
/*    }
    else {
      $links = 'quests';
    }*/
    game_button($links, 'continue', '?show_expanded=0');
    game_speech($game_user, 'Get ready to become Mayor!', TRUE);
    db_set_active('default');
    break;

  default:
    echo <<< EOF
<div class="welcome">
<div class="wise_old_man_large point">
</div>
<p>A wizened old man comes up to you.&nbsp; You recognize him as one of the
  elders of the city.</p>
<p class="second">&quot;I've been watching you for some time,
  and I like what I see.&nbsp; I think you have the potential for
  greatness.&nbsp; Maybe you could even lead this city.&quot;</p>
<p class="second">Could you?</p>
</div>
EOF;

    game_button('welcome', 'continue', '?page=2');
    game_speech($game_user, 'Hello there', TRUE);
    db_set_active('default');
    break;
}
