<?php

/**
 * @file
 * Game welcome page.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
 */

global $game, $phone_id;

// We won't have gone through fetch_user() yet, so set these here.
$game = check_plain(arg(0));
$phone_id = zg_get_phoneid();
$arg2 = check_plain(arg(2));
$ip_address = ip_address();

db_set_active('game_' . $game);

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

$d = zg_get_default(
  [
    'initial_hood',
    'initial_user_value',
    'new_user_comm_member_msg',
    'welcome_page_1_speech',
    'welcome_page_2_speech',
  ]
) + zg_get_html(
  [
    'tagline',
    'welcome_page_1',
    'welcome_page_2',
  ]
);

// Show welcome message.
echo <<< EOF
<div class="title">
  <img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<div class="tagline">
  {$d['tagline']}
</div>
EOF;

switch ($_REQUEST['page']) {

  case 2:

    $sql = 'insert into users set phone_id = "%s", username = "(new player)",
      experience = 0, level = 1, fkey_neighborhoods_id = %d, fkey_values_id = 0,
      `values` = "%s", money = 500, energy = 200, energy_max = 200';
    db_query($sql, $phone_id, $d['initial_hood'], $d['initial_user_value']);

    $sql = 'insert into user_creations set datetime = "%s", phone_id = "%s",
      remote_ip = "%s";';
    db_query($sql, date('Y-m-d H:i:s'), $phone_id, $ip_address);

    $game_user = zg_fetch_user();

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

    zg_send_user_message($game_user->id, $data, 0,
      $d['new_user_comm_member_msg'], 'user');

    print $d['welcome_page_2'];
    zg_button('quest_groups', 'continue', '?show_expanded=0');
    zg_speech($game_user, $d['welcome_page_2_speech'], TRUE);
    break;

  default:
    print $d['welcome_page_1'];
    zg_button('welcome', 'continue', '?page=2');
    zg_speech($game_user, $d['welcome_page_1_speech'], TRUE);
    break;
}

db_set_active('default');
