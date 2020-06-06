<?php

/**
 * @file
 * Show a user's profile.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of zg_defs include: no
 * .
 */

/* ------ CONTROLLER ------ */

global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();

if (empty($game_user->username) || $game_user->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

zg_slack('pages', "\"User Profile\" for Player \"$game_user->username\".");

$q = $_GET['q'];
$message_error = '';

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

if (isset($_GET['comp_show_level'])) {
  // Using an action which gives curious comp; do nothing.
}
elseif ($phone_id_to_check == $phone_id) {
  zg_competency_gain($game_user, 'introspective');
}
else {
  zg_competency_gain($game_user, 'people person');
}

$item = zg_fetch_user_by_id($phone_id_to_check);
$location = $item->location;
$points = $item->points + 0;
$extra_comp = 0;

// Same party?  Bonus to comp.
if ($game_user->fkey_values_id &&
  ($game_user->fkey_values_id == $item->fkey_values_id)) {
  $extra_comp++;
  firep('extra comp because same party!');
}

// Same clan?  Another bonus to comp.
if ($game_user->fkey_clans_id &&
  ($game_user->fkey_clans_id == $item->fkey_clans_id)) {
  $extra_comp++;
  firep('extra comp because same clan!');
}

if ($phone_id_to_check == $phone_id || $game_user->meta == 'admin') {
  $comp_show_level = 6;
  firep('checking yourself!');
}
else if ($_GET['comp_show_level'] == 'yes') {
  $comp_show_level = 5;
  firep('investigating a clannie!');
}
else if ($_GET['comp_show_level'] == 'curious') {
  $comp_show_level = zg_competency_level($game_user, 'curious')->level;
  firep($comp_show_level, 'comps based on curiosity');
  if ($comp_show_level == 5) {
    $message_error .= '<div class="message-error">
      You know that curiosity killed the cat, right?
    </div>';
  }
  $comp_show_level = min($comp_show_level + $extra_comp, 5);
}
else {
  $comp_show_level = zg_competency_level($game_user, 'people person')->level;
  firep($comp_show_level, 'comps based on people person');
  $comp_show_level = min($comp_show_level + $extra_comp, 4);
}
firep($comp_show_level, 'final comp_show_level value (' . $extra_comp .
  ' were extra)');

//$want_jol = ($_GET['want_jol'] == 'yes') ? '/want_jol' : '';
//if (arg(4) == 'want_jol') $want_jol = '/want_jol';

$message_orig = check_plain($_GET['message']);
$message = _stlouis_filter_profanity($message_orig);
//firep($message);

if (strlen($message) && strlen($message) < 3) {
  $message_error .= '<div class="message-error">Your message must be at least 3
    characters long.</div>';
  $message = '';
  zg_competency_gain($game_user, 'silent cal');
}

if (substr($message, 0, 3) == 'XXX') {
  $message_error .= '<div class="message-error">Your message contains words that
    are not allowed.&nbsp; Please rephrase.&nbsp; ' . $message . '</div>';
  $message = '';
  zg_competency_gain($game_user, 'uncouth');
}

$sql = 'select count(id) as ranking from event_points
  where points > %d;';
$result = db_query($sql, $points);
$ranking = db_fetch_object($result);
$rank = $ranking->ranking + 1;

$event_status = 'Event starts soon';
if ($item->meta == 'frozen') {
  $event_status = 'FROZEN';
}

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
$party_title = preg_replace('/^The /', '', $item->party_title);

// Save the message, if any.
$private = check_plain($_GET['private']) == '1' ? 1 : 0;

if (!empty($message)) {
  zg_send_user_message($game_user->id, $item->id, $private, $message);
  $message_orig = '';
  zg_competency_gain($game_user, 'talkative');
}

// FIXME: Halloween Jack-o-lantern posting.
if (FALSE && ($want_jol == '/want_jol') && !empty($message)) {

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
      <div class="subtitle">You can only get one Jack-O\'-Lantern<br>
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
  $clan_link .= ' (leader)';
}

if (($game_user->fkey_clans_id) &&
  ($game_user->fkey_clans_id == $item->fkey_clans_id)) {

  $clan_link = '<a href="/' . $game . '/clan_list/' . $arg2 .
    '/' . $game_user->fkey_clans_id . '">' . $clan_link . '</a>';
}

$details_start = <<< EOF
<div class="title">
$item->ep_name <span class="username">$item->username</span> $clan_acronym
</div>
<div class="user-profile">
EOF;

$details_politics = <<< EOF
<div class="heading">$politics:</div>
<div class="clan-icon"><img width="24"
  src="/sites/default/files/images/$icon"/></div>
<div class="value">$party_title</div><br>
<div class="heading">Clan:</div>
$clan_icon_html
<div class="value">$clan_link</div><br>
EOF;

$details_referral_code = <<< EOF
<div class="heading">Referral Code:</div>
<div class="value">$item->referral_code</div><br>
EOF;

$exp = number_format($item->experience);
$details_level = <<< EOF
<div class="heading">Level:</div>
<div class="clan-icon">$item->level</div><br>
<div class="heading">$experience:</div>
<div class="value">$exp</div><br>
EOF;

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

if ($comp_show_level > 1) {
  $extra_initiative = '(' . number_format($staff_bonus->initiative
      + $equipment_bonus->initiative) . ')';
  $extra_endurance = '(' . number_format($staff_bonus->endurance
      + $equipment_bonus->endurance) . ')';
  $extra_elocution = '(' . number_format($staff_bonus->elocution
      + $equipment_bonus->elocution) . ')';
}
else {
  $extra_initiative = $extra_endurance = $extra_elocution = '';
}
$extra_votes = (int) $staff_bonus->extra_votes;
$extra_defending_votes = (int) $staff_bonus->extra_defending_votes;
$money = number_format($item->money);
$iph = number_format($item->income - $item->expenses);

zg_alter('extra_votes', $item, $extra_votes,
  $extra_defending_votes);

$details_money = <<< EOF
<div class="heading">$item->values:</div>
<div class="value">$money ($iph IPH)</div><br>
EOF;

if ($comp_show_level >= 5) {
  $energy = $item->energy;
  $actions = $item->actions;
}
else {
  $energy = $actions = '???';
}

$details_energy_action = <<< EOF
<div class="heading">{$game_text['energy']}:</div>
<div class="value">$energy ($item->energy_max)</div><br>
<div class="heading">{$game_text['actions']}:</div>
<div class="value">$actions ($item->actions_max)</div><br>
EOF;

$details_iee_stats = <<< EOF
<div class="heading">$initiative:</div>
<div class="value">$item->initiative $extra_initiative</div><br>
<div class="heading">$endurance:</div>
<div class="value">$item->endurance $extra_endurance</div><br>
<div class="heading">$elocution:</div>
<div class="value">$item->elocution $extra_elocution</div><br>
EOF;

$details_vote_stats = <<< EOF
<div class="heading">Extra Votes:</div>
<div class="value">$extra_votes</div><br>
<div class="heading">Extra Def. Votes:</div>
<div class="value">$extra_defending_votes</div><br>
EOF;

// Debates.
if ($item->debates_won >= $item->level * 100) {
  $super_debater = '<strong>(** Super **)</strong>';
}
else {
  $super_debater = '';
}

$details_debates = <<< EOF
<div class="heading">{$debate}s won:</div>
<div class="value">$item->debates_won $super_debater</div>
EOF;

//  $debate_wait_time = 1200;
//  if ($debate == 'Box') $debate_wait_time = 900;

if (($phone_id_to_check != $phone_id) &&
  (abs($item->level - $game_user->level) <= 15) &&
  (($item->fkey_clans_id != $game_user->fkey_clans_id) ||
    empty($item->fkey_clans_id) || empty($game_user->fkey_clans_id))) {

  $debate_since = REQUEST_TIME - strtotime($item->debates_last_time);
  if ((($debate_since > $debate_wait_time) ||
    ($item->meta == 'zombie' && $debate_since > $zombie_debate_wait))) {

    // Debateable and enough time has passed.
    $details_debates .= <<< EOF
<div class="news relative">
<div class="message-reply-wrapper">
  <div class="message-reply">
    <a href="/$game/debates_challenge/$arg2/$item->id">$debate</a>
  </div>
</div>
</div>
EOF;
  }
  else {

    // Debateable but not enough time has passed.
    if ($item->meta == 'zombie') {
      $time_left = $zombie_debate_wait - $debate_since;
    }
    else {
      $time_left = $debate_wait_time - $debate_since;
    }

    $time_min = floor($time_left / 60);
    $time_sec = sprintf('%02d', $time_left % 60);

    $details_debates .= <<< EOF
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
else {

  // Not debateable at all.
  $details_debates .= '<br>';
}

$details_debates .= <<< EOF
<div class="heading">{$debate}s lost:</div>
<div class="value">$item->debates_lost</div><br>
EOF;

if ($debate == 'Box') {

  if ($item->level <= 20) {
    $boxing_weight = 'Minimumweight';
  }
  elseif ($item->level <= 35) {
    $boxing_weight = 'Flyweight';
  }
  elseif ($item->level <= 50) {
    $boxing_weight = 'Bantamweight';
  }
  elseif ($item->level <= 65) {
    $boxing_weight = 'Featherweight';
  }
  elseif ($item->level <= 80) {
    $boxing_weight = 'Lightweight';
  }
  elseif ($item->level <= 95) {
    $boxing_weight = 'Welterweight';
  }
  elseif ($item->level <= 110) {
    $boxing_weight = 'Middleweight';
  }
  elseif ($item->level <= 125) {
    $boxing_weight = 'Cruiserweight';
  }
  else {
    $boxing_weight = 'Heavyweight';
  }

  $details_debates .= <<< EOF
<div class="heading">{$debate_tab} Points:</div>
<div class="value">$item->meta_int</div><br>
<div class="heading">{$debate_tab} Weight:</div>
<div class="value">$boxing_weight</div><br>
EOF;
}

// FIXME: Valentine's day massacre.
if (FALSE && $comp_show_level) {

  echo <<< EOF
<span class="event-status">
<div class="heading">Event Points:</div>
<div class="value">$points (Rank: $rank)</div><br>
<!--<div class="heading">Current status:</div>
<div class="value">$event_status</div><br>-->
</span>
EOF;
}

$details_residence = <<< EOF
<div class="heading">$residence:</div>
<div class="value">$location</div><br>
EOF;


// Elected? Give approval rating!
if (!empty($item->ep_name)) {
  $details_approval = <<< EOF
<div class="heading">Approval Rating:</div>
<div class="value">$item->approval_rating%</div><br>
EOF;
}
else {
  $details_approval = '';
}

if ($item->skill_points == 0) {
  $skill_button = '<div class="action not-yet">Can\'t increase skills</div>';
}
else {
  $skill_button = '<div class="action"><a href="/' . $game . '/increase_skills/' .
    $arg2 . '/none">Increase skills</a></div>';
}

$details_luck_expenses_skills = <<< EOF
<div class="heading">Luck:</div>
<div class="value">$item->luck</div><br>
<div class="heading">Expenses:</div>
<div class="value">$item->expenses</div><br>
<div class="heading">Skill Points:</div>
<div class="value">$item->skill_points</div>$skill_button<br>
EOF;

if (strlen($item->meta)) {
  $details_meta = <<< EOF
<div class="heading">Meta:</div>
<div class="value">$item->meta</div><br>
EOF;
}
else {
  $details_meta = '';
}

$last_access = zg_format_date($item->last_access);
$startDate = zg_format_date($item->startdate);
$details_last_access = <<< EOF
<div class="heading">Start Date:</div>
<div class="value">$startDate</div><br>
<div class="heading">Last Access:</div>
<div class="value">$last_access</div><br>
EOF;

$details_end = '</div>';

// Messages.
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

if (!empty($block)) {
  $block_this_user = '<div class="block-user"><a href="/' . $game .
    '/block_user_toggle/' . $arg2 . '/' . $arg3 .
    '">Unblock this user</a></div>';
}

if ($phone_id == $phone_id_to_check) {
  $block_this_user = '';
}

if ($game_user->meta == 'admin') {
  $private_message = '<div class="private-message-checkbox">
    <input type="checkbox" name="private" id="private" value="1"/>
    <label for="private">Send as private message</label>
    </div>';
}
else {
  $private_message = '';
}

// It's ok to send to this user.
if (empty($is_blocked)) {
  $send_a_message = <<< EOF
<div class="message-title">Send a message</div>
<div class="send-message">
<form method=get action="/$game/user/$arg2/$arg3$want_jol">
<textarea class="message-textarea" name="message" rows="2">$message_orig</textarea>
<br>
$private_message
$block_this_user
<div class="send-message-send-wrapper">
  <input class="send-message-send" type="submit" value="Send"/>
</div>
</form>
</div>
EOF;
}
else {

  // You can't send to them but you can still block them.
  $send_a_message = '<div class="send-message">' . $block_this_user . '</div>';
}

$message_start = <<< EOF
<div class="news">
<div class="messages-title">
  Messages
</div>
EOF;
$messages = '';
$data = zg_get_new_user_messages($game_user, $item->id);
zg_format_messages($game_user, $item->id, $data);

foreach ($data as $item) {
  $messages .= <<< EOF
  <div class="news-item $item->type" id="{$item->display->msg_id}">
    <div class="dateline">
      {$item->display->timestamp} {$item->display->username} {$item->display->private_text}
    </div>
    <div class="message-body {$item->display->private_css}">
      {$item->display->delete}
      <p>{$item->display->message}</p>{$item->display->reply}
    </div>
  </div>
EOF;
}

$message_end = '</div>';

/* ------ VIEW ------ */

zg_fetch_header($game_user);
db_set_active();
zg_show_profile_menu($game_user);
print $message_error;

zg_show_by_level($game_user, $details_start, $comp_show_level, 0);
zg_show_by_level($game_user, $details_politics, $comp_show_level, 0);
zg_show_by_level($game_user, $details_referral_code, $comp_show_level, 6);
zg_show_by_level($game_user, $details_level, $comp_show_level, 0);
zg_show_by_level($game_user, $details_money, $comp_show_level, 5);
zg_show_by_level($game_user, $details_energy_action, $comp_show_level, 4);
zg_show_by_level($game_user, $details_iee_stats, $comp_show_level, 1);
zg_show_by_level($game_user, $details_vote_stats, $comp_show_level, 3);
zg_show_by_level($game_user, $details_debates, $comp_show_level, 2);
zg_show_by_level($game_user, $details_residence, $comp_show_level, 2);
zg_show_by_level($game_user, $details_approval, $comp_show_level, 2);
zg_show_by_level($game_user, $details_luck_expenses_skills, $comp_show_level,
  6);
zg_show_by_level($game_user, $details_meta, $comp_show_level, 4);
zg_show_by_level($game_user, $details_last_access, $comp_show_level, 5);
zg_show_by_level($game_user, $details_end, $comp_show_level, 0);
zg_show_by_level($game_user, $send_a_message, $comp_show_level, 0);
zg_show_by_level($game_user, $message_start, $comp_show_level, 0);
zg_show_by_level($game_user, $messages, $comp_show_level, 0);
zg_show_by_level($game_user, $message_end, $comp_show_level, 0);
