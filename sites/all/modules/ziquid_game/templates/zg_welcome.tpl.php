<?php

/**
 * @file
 * Game welcome page.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done
 * Ready for MVC separation: done
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: N/A
 * .
 */

/* ------ CONTROLLER ------ */

global $game, $phone_id;

// We won't have gone through fetch_user() yet, so set these here.
$game = check_plain(arg(0));
$phone_id = zg_get_phoneid();
$arg2 = check_plain(arg(2));
$ip_address = ip_address();
if (array_key_exists('page', $_GET)) {
  $page = (int) $_GET['page'];
}
else {
  $page = 1;
}
$button_link = 'welcome';
$button_extra_link = '?page=' . ($page + 1);

db_set_active('game_' . $game);

// Check to make sure not too many from the same IP address, unless AI bot.
if ((substr($arg2, 0, 3) != 'ai-') && $ip_address != '127.0.0.1') {
  $sql = 'select count(`value`) as count from user_attributes
  where `key` = "last_IP" and `value` = "%s";';
  $result = db_query($sql, $ip_address);
  $item = db_fetch_object($result);
  if ($item->count > 5) {
    $sql = 'select * from ip_whitelist where ip_address = "%s";';
    $result = db_query($sql, $ip_address);
    $ips = db_fetch_object($result);
    if (empty($ips)) {
      db_set_active();
      drupal_goto($game . '/error/' . $arg2 . '/E-2242');
    }
  }
}

$d = zg_get_default(
  [
    'initial_hood',
    'initial_user_value',
    'new_user_comm_member_msg',
    'welcome_page_' . $page . '_speech',
    'number_of_welcome_pages',
  ]
) + zg_get_html(
  [
    'tagline',
    'welcome_page_' . $page,
  ]
);

// Last page or AI bot?
if ($page == $d['number_of_welcome_pages'] ||
  substr($phone_id, 0, 3) == 'ai-') {

  // If account doesn't already exist, create it.
  $pl_uid = zg_get_userid_from_phoneid($phone_id);
  if (!$pl_uid) {
    $sql = 'insert into users set phone_id = "%s", username = "(new player)",
      experience = 0, level = 1, fkey_neighborhoods_id = %d, fkey_values_id = 0,
      `values` = "%s", money = 500, energy = 200, energy_max = 200';
    db_query($sql, $phone_id, $d['initial_hood'], $d['initial_user_value']);
    $sql = 'insert into user_creations set datetime = "%s", phone_id = "%s",
      remote_ip = "%s";';
    db_query($sql, date('Y-m-d H:i:s'), $phone_id, $ip_address);
  }
  $pl = zg_fetch_player();

  // Notify all party welcome comm members, if any.
  $sql = 'SELECT users.id FROM `users`
      left join elected_officials eo on users.id = eo.fkey_users_id
      left join elected_positions ep on eo.fkey_elected_positions_id = ep.id
      WHERE ep.gets_new_user_notifications = 1;';
  $result = db_query($sql);
  $data = [];
  while ($item = db_fetch_object($result)) {
    $data[] = $item->id;
  }

  if (count($data)) {
    zg_send_user_message($pl->id, $data, 0,
      $d['new_user_comm_member_msg'], 'user');
  }

  // Alter link to go to quests.
  $button_link = 'quest_groups';
  $button_extra_link = '?show_expanded=0';
}
else {
  // Dummy object for zg_speech().
  $pl = new \stdClass();
}

/* ------ VIEW ------ */
?>
<div class="title">
  <img src="/sites/default/files/images/<?php print $game; ?>_title.png">
</div>
<div class="tagline">
  <?php print $d['tagline']; ?>
</div>

<?php print $d['welcome_page_' . $page]; ?>
<?php zg_button($button_link, 'continue', $button_extra_link); ?>
<?php zg_speech($pl, $d['welcome_page_' . $page . '_speech'], TRUE); ?>
<?php
   db_set_active();
