<?php

/**
 * @file stlouis_userprofile.tpl.php
 * Show a user's profile.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

global $game, $phone_id;
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);
$q = $_GET['q'];

if (empty($game_user->username) || $game_user->username == '(new player)') {
  drupal_goto($game . '/choose_name/' . $arg2);
}

_show_profile_menu($game_user);

$phone_id_to_check = $phone_id;
if ($arg3 != '') {
  $phone_id_to_check = $arg3;
}

if (substr($arg3, 0, 3) == 'id:') {
  $sql = 'select phone_id from users where id = %d;';
  $result = db_query($sql, (int) substr($arg3, 3));
  $item = db_fetch_object($result);
  $phone_id_to_check = $item->phone_id;
}

if ($phone_id_to_check == $phone_id) {
  game_competency_gain($game_user, 'introspective');
}
else {
  game_competency_gain($game_user, 'people person');
}

if (($phone_id_to_check == $phone_id) ||
  ($_GET['show_all'] == 'yes') || $game_user->meta == 'admin') {
  $show_all = TRUE;
}
else {
  $show_all = FALSE;
}

$want_jol = ($_GET['want_jol'] == 'yes') ? '/want_jol' : '';
if (arg(4) == 'want_jol') $want_jol = '/want_jol';

$message_orig = check_plain($_GET['message']);
$message = _stlouis_filter_profanity($message_orig);
//firep($message);

if (strlen($message) > 0 and strlen($message) < 3) {
  echo '<div class="message-error">Your message must be at least 3
    characters long.</div>';
  $message = '';
  game_competency_gain($game_user, 'silent cal');
}

if (substr($message, 0, 3) == 'XXX') {
  echo '<div class="message-error">Your message contains words that are not
    allowed.&nbsp; Please rephrase.&nbsp; ' . $message . '</div>';
  $message = '';
  game_competency_gain($game_user, 'uncouth');
}

$sql = 'SELECT username, experience, initiative, endurance,
  elocution, debates_won, debates_lost, skill_points, luck,
  debates_last_time, users.fkey_values_id, level, referral_code,
  users.meta, users.income, users.expenses,
  user_creations.datetime as startdate, users.meta_int,
  `values`.party_title, `values`.party_icon,
  `values`.name, users.id, users.fkey_neighborhoods_id,
  elected_positions.name as ep_name,
  elected_officials.approval_rating,
  clan_members.is_clan_leader,
  clans.name as clan_name, clans.acronym as clan_acronym,
  clans.id as fkey_clans_id,
  event_points.points

  FROM `users`

  LEFT JOIN `values` ON users.fkey_values_id = `values`.id

  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id

  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id

  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

  LEFT JOIN user_creations on user_creations.phone_id = users.phone_id

  LEFT JOIN event_points on event_points.fkey_users_id = users.id

  WHERE users.phone_id = "%s"

  ORDER by user_creations.datetime ASC
  LIMIT 1';

$result = db_query($sql, $phone_id_to_check);
$item = db_fetch_object($result);
firep($item, 'user profile to view');

$points = $item->points + 0;

$sql = 'select count(id) as ranking from event_points
  where points > %d;';
$result = db_query($sql, $points);
$ranking = db_fetch_object($result);
$rank = $ranking->ranking + 1;

$event_status = 'Event starts soon';
if ($item->meta == 'frozen') $event_status = 'FROZEN';

// labor day -- all are UWP -- jwc
//  $item->fkey_values_id = 7;
//  $item->party_icon = 'workers';
//  $item->party_title = 'United Workers Party';

$icon_path = file_directory_path() . '/images/' . $game . '_clan_' .
  strtolower($item->clan_acronym) . '.png';
//firep($icon_path, 'icon path');

if (file_exists($_SERVER['DOCUMENT_ROOT'] . base_path() . $icon_path)) {

  $clan_icon_html = '<div class="clan-icon"><img width="24"
    src="/sites/default/files/images/' .
    $game . '_clan_' . strtolower($item->clan_acronym) . '.png"/></div>';

}

$icon = $game . '_clan_' . $item->party_icon . '.png';

$clan_title = preg_replace('/^The /', '', $item->clan_title);

$sql = "select name from neighborhoods where id = '%d';";
$result = db_query($sql, $item->fkey_neighborhoods_id);
$data = db_fetch_object($result);
$location = $data->name;

// save the message, if any

$private = check_plain($_GET['private']) == '1' ? 1 : 0;

if (!empty($message)) {

  $sql = 'insert into user_messages (fkey_users_from_id,
    fkey_users_to_id, private, message) values (%d, %d, %d, "%s");';
  $result = db_query($sql, $game_user->id, $item->id, $private, $message);
  $message_orig = '';
  game_competency_gain($game_user, 'talkative');
}

// Halloween Jack-o-lantern posting.
if (($want_jol == '/want_jol') && !empty($message)) {

  $get_jol = TRUE;

  // No costume!
  if ($game_user->username == $game_user->real_username) {

    echo '<div class="title">Huh?</div>
      <div class="subtitle">You can\'t get a Jack-O\'-Lantern
      without a costume!</div>';
    $get_jol = FALSE;

  }

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = 26 and fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $data = db_fetch_object($result);

  // No ticket!
  if ($data->quantity < 1) {

    echo '<div class="title">Huh?</div>
      <div class="subtitle">You can\'t party without a ticket!</div>';
    $get_jol = FALSE;

  }

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = 27 and fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $data = db_fetch_object($result);
  // No JoLs yet!
  if ($data->quantity < 1) {

    echo '<div class="title">Sorry</div>
      <div class="subtitle">You must check out all the people first</div>';
    $get_jol = FALSE;

  }

  $sql = 'select * from jols
    where fkey_users_from_id = %d and fkey_users_to_id = %d;';
  $result = db_query($sql, $game_user->id, $item->id);
  $data = db_fetch_object($result);
  // Already gotten a JoLs for this user!
  if (!empty($data)) {

    echo '<div class="title">Remember</div>
      <div class="subtitle">You can only get one Jack-O\'-Lantern<br/>
        from each person</div>';
    $get_jol = FALSE;

  }

  // They get one!
  if ($get_jol) {
/*
    $sql = 'insert into jols (fkey_users_from_id, fkey_users_to_id)
      values (%d, %d);';
    $result = db_query($sql, $game_user->id, $item->id);

    $sql = 'update equipment_ownership set quantity = quantity + 1
      where fkey_equipment_id = 27 and fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);
*/

    echo '<div class="title">Sorry!</div>
      <div class="subtitle">We are out of Jack-O\'-Lanterns!</div>';

  }

}

if (!empty($item->clan_acronym)) {
  $clan_acronym = "($item->clan_acronym)";
  $clan_link = $item->clan_name;
}
else {
  $clan_link = t('None');
}

if ($item->is_clan_leader) {
  $clan_acronym .= '*';
  $clan_link .= " (leader)";
}

if (($game_user->fkey_clans_id) &&
  ($game_user->fkey_clans_id == $item->fkey_clans_id)) {

    $clan_link = '<a href="/' . $game . '/clan_list/' . $arg2 .
      '/' . $game_user->fkey_clans_id . '">' . $clan_link . '</a>';

  }

echo <<< EOF
<div class="title">
$item->ep_name <span class="username">$item->username</span> $clan_acronym
</div>
<div class="user-profile">
<div class="heading">$politics:</div>
<div class="clan-icon"><img width="24"
  src="/sites/default/files/images/$icon"/></div>
<div class="value">$clan_title</div><br/>
<div class="heading">Clan:</div>
$clan_icon_html
<div class="value">$clan_link</div><br/>
EOF;

 // Show more stats if it's you.
if ($phone_id_to_check == $phone_id) {

  echo <<< EOF
<div class="heading">Referral Code:</div>
<div class="value">$item->referral_code</div><br/>
EOF;

}

$exp = number_format($item->experience);

echo <<< EOF
<div class="heading">Level:</div>
<div class="clan-icon">$item->level</div><br/>
<div class="heading">$experience:</div>
<div class="value">$exp</div><br/>
EOF;

// Show more stats if it's show all.
if ($show_all) {

  $sql = 'SELECT
    SUM( staff.extra_votes * staff_ownership.quantity ) AS extra_votes,
    SUM( staff.extra_defending_votes * staff_ownership.quantity )
      AS extra_defending_votes,
    SUM( staff.initiative_bonus * staff_ownership.quantity ) AS initiative,
    SUM( staff.endurance_bonus * staff_ownership.quantity ) AS endurance,
    SUM( staff.elocution_bonus * staff_ownership.quantity ) AS elocution
    FROM staff
    LEFT JOIN staff_ownership ON staff_ownership.fkey_staff_id = staff.id
    AND staff_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $item->id);
  $staff_bonus = db_fetch_object($result);

  $sql = 'SELECT
    SUM( equipment.initiative_bonus * equipment_ownership.quantity ) AS initiative,
    SUM( equipment.endurance_bonus * equipment_ownership.quantity ) AS endurance,
    SUM( equipment.elocution_bonus * equipment_ownership.quantity ) AS elocution
    FROM equipment
    LEFT JOIN equipment_ownership
    ON equipment_ownership.fkey_equipment_id = equipment.id
    AND equipment_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $item->id);
  $equipment_bonus = db_fetch_object($result);

  $extra_initiative = number_format($staff_bonus->initiative
  + $equipment_bonus->initiative);
  $extra_endurance = number_format($staff_bonus->endurance
  + $equipment_bonus->endurance);
  $extra_elocution = number_format($staff_bonus->elocution
  + $equipment_bonus->elocution);
  $extra_votes = (int) $staff_bonus->extra_votes;
  $extra_defending_votes = (int) $staff_bonus->extra_defending_votes;

  game_alter('extra_votes', $item, $extra_votes, $extra_defending_votes);

  echo <<< EOF
<div class="heading">$initiative:</div>
<div class="value">$item->initiative ($extra_initiative)</div><br/>
<div class="heading">$endurance:</div>
<div class="value">$item->endurance ($extra_endurance)</div><br/>
<div class="heading">$elocution:</div>
<div class="value">$item->elocution ($extra_elocution)</div><br/>
<div class="heading">Extra Votes:</div>
<div class="value">$extra_votes</div><br/>
<div class="heading">Extra Def. Votes:</div>
<div class="value">$extra_defending_votes</div><br/>
EOF;

}

if ($item->debates_won >= $item->level * 100) {
  $super_debater = '<strong>(** Super **)</strong>';
}
else {
  $super_debater = '';
}

echo <<< EOF
<div class="heading">{$debate}s won:</div>
<div class="value">$item->debates_won $super_debater</div>
EOF;

//  $debate_wait_time = 1200;
//  if ($debate == 'Box') $debate_wait_time = 900;

if (($phone_id_to_check != $phone_id) &&
  (abs($item->level - $game_user->level) <= 15) &&
  (($item->fkey_clans_id != $game_user->fkey_clans_id) ||
    empty($item->fkey_clans_id) || empty($game_user->fkey_clans_id))) {

  if ((((time() - strtotime($item->debates_last_time)) > $debate_wait_time) ||
    (($item->meta == 'zombie') &&
    ((time() - strtotime($item->debates_last_time)) > $zombie_debate_wait)))) {
// debateable and enough time has passed
    echo <<< EOF
<div class="news relative">
<div class="message-reply-wrapper">
  <div class="message-reply">
    <a href="/$game/debates_challenge/$arg2/$item->id">$debate</a>
  </div>
</div>
</div>
EOF;

  }
  else { // debateable but not enough time has passed

    if ($item->meta == 'zombie') {
      $time_left = $zombie_debate_wait -
        (time() - strtotime($item->debates_last_time));
    }
	else {
      $time_left = $debate_wait_time -
        (time() - strtotime($item->debates_last_time));
    }

    $time_min = floor($time_left / 60);
    $time_sec = sprintf('%02d', $time_left % 60);

    echo <<< EOF
<div class="news relative">
<div class="message-reply-wrapper">
  <div class="message-reply not-yet">
    $debate in $time_min:$time_sec
  </div>
</div>
</div>
EOF;

  }

}
else { // not debateable at all
  echo '<br>';
}

echo <<< EOF
<div class="heading">{$debate}s lost:</div>
<div class="value">$item->debates_lost</div><br/>
EOF;

if ($debate == 'Box') {

  if ($item->level <= 20) {
    $boxing_weight = 'Minimumweight';
  }
  else if ($item->level <= 35) {
    $boxing_weight = 'Flyweight';
  }
  else if ($item->level <= 50) {
    $boxing_weight = 'Bantamweight';
  }
  else if ($item->level <= 65) {
    $boxing_weight = 'Featherweight';
  }
  else if ($item->level <= 80) {
    $boxing_weight = 'Lightweight';
  }
  else if ($item->level <= 95) {
    $boxing_weight = 'Welterweight';
  }
  else if ($item->level <= 110) {
    $boxing_weight = 'Middleweight';
  }
  else if ($item->level <= 125) {
    $boxing_weight = 'Cruiserweight';
  }
  else {
    $boxing_weight = 'Heavyweight';
  }

  echo <<< EOF
<div class="heading">{$debate_tab} Points:</div>
<div class="value">$item->meta_int</div><br/>
<div class="heading">{$debate_tab} Weight:</div>
<div class="value">$boxing_weight</div><br/>
EOF;

}

// Valentine's day massacre.
if (FALSE && $show_all) {

  echo <<< EOF
<span class="event-status">
<div class="heading">Event Points:</div>
<div class="value">$points (Rank: $rank)</div><br/>
<!--<div class="heading">Current status:</div>
<div class="value">$event_status</div><br/>-->
</span>
EOF;

}

echo <<< EOF
<div class="heading">$residence:</div>
<div class="value">$location</div><br/>
EOF;

// Elected?  give approval rating!
if (!empty($item->ep_name)) {

  echo <<< EOF
<div class="heading">Approval Rating:</div>
<div class="value">$item->approval_rating%</div><br/>
EOF;

}

// Show more stats if it's you.
if ($phone_id_to_check == $phone_id) {

  if ($item->skill_points == 0) {
    $skill_button = '<div class="action not-yet">Can\'t increase skills</div>';
  }
  else {
    $skill_button = '<div class="action"><a href="/' . $game . '/increase_skills/' .
      $arg2 . '/none">Increase skills</a></div>';
  }

  echo <<< EOF
<div class="heading">Luck:</div>
<div class="value">$item->luck</div><br>
<div class="heading">Expenses:</div>
<div class="value">$item->expenses</div><br>
<div class="heading">Skill Points:</div>
<div class="value">$item->skill_points</div>$skill_button<br/>
EOF;

}

$block_this_user = '<div class="block-user"><a href="/' . $game .
  '/block_user_toggle/' . $arg2 . '/' . $arg3 .
  '">Block this user</a></div>';

$sql = 'select * from message_blocks where fkey_blocked_users_id = %d
  and fkey_blocking_users_id = %d;';
$result = db_query($sql, $item->id, $game_user->id);
$block = db_fetch_object($result);

 $sql = 'select * from message_blocks where fkey_blocked_users_id = %d
  and fkey_blocking_users_id = %d;';
$result = db_query($sql, $game_user->id, $item->id);
$is_blocked = db_fetch_object($result);

if (!empty($block))
  $block_this_user = '<div class="block-user"><a href="/' . $game .
  '/block_user_toggle/' . $arg2 . '/' . $arg3 .
  '">Unblock this user</a></div>';

if ($phone_id == $phone_id_to_check) $block_this_user = '';

if (($phone_id == 'abc123') || ($game_user->username == 'New iPad test')) {
  $private_message = '<div class="private-message-checkbox">
    <input type="checkbox" name="private" id="private" value="1"/>
    <label for="private">Send as private message</label>
    </div>';
}
else {
  $private_message = '';
}

echo <<< EOF
</div>
EOF;

  // It's ok to send to this user.
 if (empty($is_blocked)) {
   echo <<< EOF
<div class="message-title">Send a message</div>
<div class="send-message">
<form method=get action="/$game/user/$arg2/$arg3$want_jol">
  <textarea class="message-textarea" name="message" rows="2">$message_orig</textarea>
  <br/>
  $private_message
  $block_this_user
  <div class="send-message-send-wrapper">
    <input class="send-message-send" type="submit" value="Send"/>
  </div>
</form>
</div>
EOF;
 }
 else { // you can't send to them but you can still block them

   echo '<div class="send-message">' . $block_this_user . '</div>';

 }

echo <<< EOF
<div class="news">
<div class="messages-title">
  Messages
</div>
EOF;

if ($phone_id != $phone_id_to_check) {
// not looking at yourself?  don't show private messages
  $no_private = 'and (private = 0 OR user_messages.fkey_users_from_id = ' .
  $game_user->id . ')'; // FIXME -- don't add user id inline, use %d instead
}

$sql = 'select user_messages.*, users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym

  from user_messages

  left join users on user_messages.fkey_users_from_id = users.id

  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id

  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id

  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    user_messages.fkey_users_from_id

  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

  where fkey_users_to_id = %d ' . $no_private . '

  order by id DESC
  LIMIT 50;';

$result = db_query($sql, $item->id);
$msg_shown = FALSE;

$data = array();
while ($item = db_fetch_object($result)) $data[] = $item;

foreach ($data as $item) {
firep($item->id);
  $display_time = _stlouis_format_date(strtotime($item->timestamp));
  $clan_acronym = '';

if (!empty($item->clan_acronym))
  $clan_acronym = "($item->clan_acronym)";

if ($item->is_clan_leader)
  $clan_acronym .= '*';

if ($item->private) {
  $private_css = 'private';
  $private_text = '(private)';
}
else {
  $private_css = $private_text = '';
}

$private_css .= ' user';

  echo <<< EOF
<div class="dateline">
$display_time from $item->ep_name $item->username $clan_acronym $private_text
</div>
<div class="message-body $private_css">
EOF;

  // Allow user to delete own messages.
  if ($phone_id_to_check == $phone_id || $item->fkey_users_from_id == $game_user->id) {
    echo <<< EOF
      <div class="message-delete"><a href="/$game/msg_delete/$arg2/$item->id?destination=/$q"><img
        src="/sites/default/files/images/delete.png" width="16" height="16"/></a></div>
EOF;
  }

  echo '<p>' . $item->message . '</p>';
EOF;

  if ($item->username != 'USLCE Game') {
    echo <<< EOF
<div class="message-reply-wrapper"><div class="message-reply">
  <a href="/$game/user/$arg2/id:$item->fkey_users_from_id">View / Respond</a>
</div></div>
EOF;
  }

  echo <<< EOF
</div>
EOF;
  $msg_shown = TRUE;

}

db_set_active('default');
