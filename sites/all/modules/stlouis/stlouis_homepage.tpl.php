<?php

/**
 * @file stlouis_homepage.tpl.php
 * The game's main screen.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

$version = 'v0.6.0, Jul 02 2018';

global $game, $phone_id;

include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$message = check_plain($_GET['message']);

game_competency_gain($game_user, 'where the heart is');

if (FALSE && $game_user->level < 6) {

  echo <<< EOF
<div class="title">
  <img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<p>&nbsp;</p>
<div class="welcome">
<div class="wise_old_man_small">
</div>
<p>&quot;You're not influential enough yet for the home page.&nbsp;
Come back at level 6.&quot;</p>
<p class="second">&nbsp;</p>
<p class="second">&nbsp;</p>
<p class="second">&nbsp;</p>
</div>
<div class="subtitle"><a
href="/$game/quests/$arg2"><img
  src="/sites/default/files/images/{$game}_continue.png"/></a></div>
EOF;

if (substr($phone_id, 0, 3) == 'ai-')
  echo "<!--\n<ai \"home not-yet\"/>\n-->";

db_set_active('default');
return;

}

if (substr($phone_id, 0, 3) == 'ai-')
  echo "<!--\n<ai \"home\"/>\n-->";

$today = date('Y-m-d');

if ($game_user->last_bonus_date != $today || $game_user->meta == 'admin') {

  $sql = 'select residents from neighborhoods where id = %d;';
  $result = db_query($sql, $game_user->fkey_neighborhoods_id);
  $item = db_fetch_object($result);

  $money = ($game_user->level * $item->residents) + $game_user->income -
    $game_user->expenses;
  $extra_bonus = '';
/*
  if ($game == 'stlouis') {

    $sql = 'select quantity from staff_ownership
      where fkey_staff_id = 18 and fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);
    $item = db_fetch_object($result);

    if ($item->quantity >= 1) {
      $money *= 3;
      $extra_text .= '<div class="level-up-text">
        ~ Your private banker tripled your bonus ~
      </div>';
    }

  }

  if ($game == 'celestial_glory') {

    if ($game_user->fkey_values_id == 5) {
      $money *= 1.01;
      $extra_text .= '<div class="level-up-text">
        ~ As a Merchant, you gained an extra 1% ~
      </div>';
    }

  }
*/
  $money = ceil($money);

firep("adding $money money because last_bonus_date = $last_bonus_date");

  $sql = 'update users set money = money + %d, last_bonus_date = "%s"
    where id = %d;';
  $result = db_query($sql, $money, $today, $game_user->id);
  $game_user = $fetch_user();

  $extra_bonus = '<div class="level-up">
      <div class="wise_old_man happy"></div>
      <div class="level-up-header">Daily Bonus!</div>
      <div class="level-up-text">You have received a bonus of ' .
        number_format($money) . ' ' . $game_user->values . '!</div>' .
        $extra_text .
      '<div class="level-up-text">For the next three minutes, competencies can be enhanced every 15 seconds</div>
      <div class="level-up-text">Come back tomorrow for another bonus</div>
    </div>';

  // Fast comps for the next three minutes.
  game_set_timer($game_user, 'fast_comps_15', 180);

  // Add competency for work.
  $sql = 'SELECT land.fkey_enhanced_competencies_id FROM `land`
  left join land_ownership on land_ownership.fkey_land_id = land.id
  left join users on users.id = land_ownership.fkey_users_id
  WHERE users.id = %d and land.type = "job"';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  if (isset($item->fkey_enhanced_competencies_id)) {
    game_competency_gain($game_user, (int) $item->fkey_enhanced_competencies_id);
  }

}

// Get values of current hood's alder.
$sql = 'SELECT `values` FROM `users`
  inner join elected_officials on elected_officials.fkey_users_id = users.id
  WHERE fkey_neighborhoods_id = %d
  and elected_officials.fkey_elected_positions_id = 1;';
$result = db_query($sql, $game_user->fkey_neighborhoods_id);
$alder = db_fetch_object($result);
firep($alder, 'values of current hood alder');
$alder_values = drupal_strtolower($alder->values);

$fetch_header($game_user);

if (empty($game_user->referral_code)) {

  $good_code = FALSE;
  $count = 0;

  while (!$good_code && $count++ < 10) {

    $referral_code = '0000' .
      base_convert(mt_rand(0, pow(36, 5) - 1) . '', 10, 36);
    $referral_code = strtoupper(substr($referral_code,
      strlen($referral_code) - 5, 5));
firep($referral_code);

    $sql = 'select referral_code from users where referral_code = "%s";';
    $result = db_query($sql, $referral_code);
    $item = db_fetch_object($result);

    // Code not already in use - use it!
    if (empty($item->referral_code)) {

      $good_code = TRUE;
      $sql = 'update users set referral_code = "%s" where id = %d;';
      $result = db_query($sql, $referral_code, $game_user->id);
      $game_user->referral_code = $referral_code;

    }

  }

}

if (substr(arg(2), 0, 4) == 'nkc ') {
  $coefficient = 1.875;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 8') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 7') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 6') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 5') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.4') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.3') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.2') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.1') !== FALSE) {
  $coefficient = 1;
}
else if ((stripos($_SERVER['HTTP_USER_AGENT'], 'BNTV') !== FALSE) &&
  (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4') !== FALSE)) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=800') !== FALSE) {
  $coefficient = 2.5;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=600') !== FALSE) {
  $coefficient = 1.875;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=533') !== FALSE) {
  $coefficient = 1.66;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=480') !== FALSE) {
  $coefficient = 1.5;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=411') !== FALSE) {
  $coefficient = 1.25;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=400') !== FALSE) {
  $coefficient = 1.25;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=384') !== FALSE) {
  $coefficient = 1.2;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=360') !== FALSE) {
  $coefficient = 1.125;
}
else {
  $coefficient = 1;
}

/*
if (($today == '2012-12-26') || $game_user->username == 'abc123')
$extra_menu = '-boxing';
*/

$event_text = '';

switch($event_type) {

  case EVENT_DONE:

    $event_text = '<div class="event">
      The event is over!&nbsp; We hope you had fun.
      </div><div class="event-tagline small">
        <a href="/' . $game . '/top_event_points/' . $arg2 .
          '">Leaderboard</a>
      </div>';
    break;

  case EVENT_DEBATE:

    $event_text = '<div class="event">
        While we are waiting on ToxiCorp to be ready,
        let\'s have a debate mini event.&nbsp; Debate for prizes today!
      </div><div class="event-tagline small">
        <a href="/' . $game . '/top_event_points/' . $arg2 .
          '">Leaderboard</a>
      </div>';
    break;

  case EVENT_PRE_MAY:

    $event_text = '<div class="event">
<div class="event-title">
        May\'s Quest
      </div>
      <div class="event-tagline">
        ~ Find the perfect gift for Mother\'s Day ~
      </div>
      <div class="event-text">
        Starts May 1
      </div>
      <!--<div class="event-tagline small">
        <a href="/' . $game . '/top_event_points/' . $arg2 .
      '">Leaderboard</a>
      </div>-->
      </div>';
    break;

  case EVENT_CINCO_DE_MAYO:

    $event_text = '<div class="event">
      <div class="event-tagline">
        <!--Are you going to the Cinco De Mayo party in Benton Park West?-->
        Didn\'t finish the Cinco De Mayo event in Benton Park West?
      </div>
      <div class="event-text">
        <!--I hear it\'s going to be fun!-->
        Try it again this weekend! (Fri, Sat, Sun)
      </div>
      </div>';
    break;

  case EVENT_DOUBLE_LUCK_MONEY:
    $event_text = '<div class="event">
      <div class="event-tagline">
        DOUBLE YOUR MONEY EVENT
      </div>
      <div class="event-text">
        May 14 and 15 only, get 2x the normal amount of money for 1 ' . $luck . '!
      </div>
      </div>
      <div class="try-an-election-wrapper">
        <div class="try-an-election">
          <a href="/' . $game . '/elders_do_fill/' . $arg2 .
            '/money?destination=/' . $game . '/home/' . $arg2 . '">' .
            t('Receive @offer @values NOW (1&nbsp;@luck',
              array(
                '@offer' => game_luck_money_offer($game_user),
                '@values' => $game_user->values,
                '@luck' => $luck,
              )) . ')
          </a>
        </div>
      </div>';
    break;
}

// Monthly quests.
switch ($month_mission) {

  case MISSION_MAY:
    $event_text .= '<div class="event">
<div class="event-title">
        May\'s Quest
      </div>
      <div class="event-tagline">
        ~ Find the perfect gift for your wife ~
      </div>
      <div class="event-tagline">
        to celebrate Mother\'s Day
      </div>
      <div class="event-text">
        Ends May 13
      </div>
      <div class="event-text">
        <a href="/' . $game . '/quests/' . $arg2 .
          '/1005">Start Here</a>
      </div>
      <!--<div class="event-tagline small">
        <a href="/' . $game . '/top_event_points/' . $arg2 .
    '">Leaderboard</a>
      </div>-->
      </div>';
    break;

  case MISSION_JUN:
    $event_text .= '<div class="event">
<div class="event-title">
        June\'s Quest
      </div>
      <div class="event-tagline">
        ~ Find the perfect tie for your husband ~
      </div>
      <div class="event-tagline">
        to celebrate Father\'s Day
      </div>
      <div class="event-text">
        Ends June 24
      </div>
      <div class="event-text">
        <a href="/' . $game . '/quests/' . $arg2 .
      '/1006">Start Here</a>
      </div>
      <!--<div class="event-tagline small">
        <a href="/' . $game . '/top_event_points/' . $arg2 .
      '">Leaderboard</a>
      </div>-->
      </div>';
    break;
}

// dead presidents event
/*
if ($game == 'stlouis') $event_text = '<!--<a href="/' . $game .
'/top_event_points/' . $arg2 . '">-->
<div class="event">
  <img src="/sites/default/files/images/toxicorp_takeover.png" border=0
  width="160">
</div>
<div class="event-text">
    New&nbsp;Event <!--Starts&nbsp;Feb&nbsp;28th-->DELAYED
</div>
<div class="event-tagline small">
  Turning St. Louis into an industrial wasteland
</div>
<div class="event-tagline">
  &mdash; one &mdash; hood &mdash; at &mdash; a &mdash; time &mdash;
</div>
</div>
<!--</a>-->';
*/

// Add event text, if any.
game_alter('homepage_event_notice', $game_user,$event_text);

echo <<< EOF
$extra_bonus
<div class="title">
<img src="/sites/default/files/images/{$game}_title.png"/>
<a class="version" href="/$game/changelog/$arg2">
  $version
</a>
</div>
<div class="new-main-menu">
<img src="/sites/default/files/images/{$game}_home_menu{$extra_menu}.png"
usemap="#new_main_menu"/>
<a class="elections-menu" href="/$game/elections/$arg2">
  $election_tab
</a>

<map name="new_main_menu">
EOF;

 $coords = _stlouis_scale_coords($coefficient, 107, 34, 210, 63);

 echo <<< EOF
  <area shape="rect" coords="$coords" alt="Missions" href="/$game/quests/$arg2" />
EOF;

 $coords = _stlouis_scale_coords($coefficient, 42, 72, 122, 92);

echo <<< EOF
  <area shape="rect" coords="$coords" alt="Debates" href="/$game/debates/$arg2" />
EOF;

$coords = _stlouis_scale_coords($coefficient, 197, 72, 257, 92);
$coords2 = _stlouis_scale_coords($coefficient, 187, 93, 267, 115);

echo <<< EOF
  <area shape="rect" coords="$coords" alt="Aides" href="/$game/land/$arg2" />
  <area shape="rect" coords="$coords2" alt="Actions" href="/$game/actions/$arg2" />
EOF;


// Move
  $coords = _stlouis_scale_coords($coefficient, 131, 127, 183, 147);

  echo <<< EOF
  <area shape="rect" coords="$coords" alt="Move" href="/$game/move/$arg2/0" />
EOF;


// Elders, Profile
$coords = _stlouis_scale_coords($coefficient, 126, 155, 192, 180);
$coords2 = _stlouis_scale_coords($coefficient, 45, 192, 100, 210);

echo <<< EOF
  <area shape="rect" coords="$coords" alt="Elders" href="/$game/elders/$arg2" />
  <area shape="rect" coords="$coords2" alt="Profile" href="/$game/user/$arg2" />
EOF;

$coords = _stlouis_scale_coords($coefficient, 113, 192, 151, 210);

if ($game_user->fkey_clans_id > 0) {

  echo <<< EOF
  <area shape="rect" coords="$coords" alt="Clan"
    href="/$game/clan_list/$arg2/{$game_user->fkey_clans_id}" />
EOF;

}
else {

  echo <<< EOF
  <area shape="rect" coords="$coords" alt="Clan"
    href="/$game/clan_list_available/$arg2" />
EOF;

}

  $coords = _stlouis_scale_coords($coefficient, 162, 192, 200, 210);

  echo <<< EOF
  <area shape="rect" coords="$coords" alt="Help" href="/$game/help/$arg2" />
EOF;

  $coords = _stlouis_scale_coords($coefficient, 214, 192, 265, 210);

  echo <<< EOF
  <area shape="rect" coords="$coords" alt="Forum"
    href="external://discord.gg/cFyt7w9" />
</map>
</div>
<div class="location">
<span class="location-$alder_values">$game_user->location</span>
</div>
$event_text
<div class="news">
<div class="title">
  News
</div>
<div class="news-buttons">
  <button id="news-all" class="active">All</button>
  <button id="news-user">Personal</button>
  <button id="news-challenge">{$election_tab}</button>
  <button id="news-clan">$party_small</button>
  <button id="news-mayor">${game_text['mayor']}</button>
</div>
<div id="all-text">
EOF;

// No reason to spend cycles on msgs.
if (substr($phone_id, 0, 3) == 'ai-') {
  db_set_active('default');
  return;
}

// are we a type 2 elected official?
$sql = 'SELECT type FROM elected_officials
  left join elected_positions on elected_positions.id = fkey_elected_positions_id
  WHERE fkey_users_id = %d;';
$result = db_query($sql, $game_user->id);
$item = db_fetch_object($result);

$elected_official_type = $item->type;

// If a party official.
if ($elected_official_type == 2) {
  // Grab the list of all clans in the user's party.
  // We need to do this separately to keep the db from locking.
  // Wish mysql had a select with nolock feature - JWC.
  $data = [];
  $sql = 'SELECT fkey_clans_id FROM clan_members
    left join users on fkey_users_id = users.id
    WHERE fkey_values_id = %d
    and is_clan_leader = 1;';
  $result = db_query($sql, $game_user->fkey_values_id);
  while ($item = db_fetch_object($result)) {
    $data[] = $item->fkey_clans_id;
  }

  $clan_sql = 'where clan_messages.fkey_neighborhoods_id in (%s)';
  $clan_id_to_use = implode(',', $data);
//firep($clan_id_to_use);
  $limit = 50;
  $all_limit = 150;
}
else {
  $clan_sql = 'where clan_messages.fkey_neighborhoods_id = %d';
  $clan_id_to_use = $game_user->fkey_clans_id;
  $limit = 20;
  $all_limit = 100;
}

$sql = '
  (
  select user_messages.timestamp, user_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  user_messages.private,
  "user" as type,
  subtype
  from user_messages
  left join users on user_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  user_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where fkey_users_to_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select challenge_messages.timestamp, challenge_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private,
  "challenge" as type,
  "" as subtype
  from challenge_messages
  left join users on challenge_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  challenge_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where fkey_users_to_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select neighborhood_messages.timestamp, neighborhood_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private,
  "hood" as type,
  "" as subtype
  from neighborhood_messages
  left join users on neighborhood_messages.fkey_users_from_id =
    users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  neighborhood_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where neighborhood_messages.fkey_neighborhoods_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select clan_messages.timestamp, clan_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private,
  "clan" as type,
  "" as subtype
  from clan_messages
  left join users on clan_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    clan_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  ' . $clan_sql . '
  order by timestamp DESC limit %d
  )

  union

  (
  select values_messages.timestamp, values_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private,
  "values" as type,
  "" as subtype
  from values_messages
  left join users on values_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    values_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where values_messages.fkey_values_id = %d
--    AND values_messages.fkey_neighborhoods_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select system_messages.timestamp, system_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
--  NULL AS username, NULL as phone_id,
--  NULL AS ep_name,
--  0 AS is_clan_leader,
--  NULL AS clan_acronym,
  0 AS private,
  "mayor" as type,
  subtype
  from system_messages
  left join users on system_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    system_messages.fkey_users_from_id
    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  order by timestamp DESC limit %d
  )

  order by timestamp DESC limit %d;';
firep($sql, 'sql for homepage');

// don't show if load avg too high

//  $load_avg = sys_getloadavg(); // FIXME: get load avg of db server
$data = [];

if (TRUE/*$load_avg[0] <= 2.0*/) {
// expensive query - goes to slave
//   db_set_active('game_' . $game . '_slave1');
  $result = db_query($sql, $game_user->id, $limit,
    $game_user->id, 10, // challenge limit of 10
    $game_user->fkey_neighborhoods_id, $limit,
    $clan_id_to_use, $limit,
    $game_user->fkey_values_id, $game_user->fkey_neighborhoods_id, $limit,
    $limit, $all_limit);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  db_set_active('game_' . $game); // reset to master
}

$msg_shown = FALSE;

echo <<< EOF
<div class="news-item clan clan-msg">
<div class="message-title">Send a message to your clan</div>
<div class="send-message">
  <form method=get action="/$game/party_msg/$arg2">
    <textarea class="message-textarea" name="message"
    rows="2">$message</textarea>
  <br/>
  <div class="send-message-target">
    <select name="target">
EOF;

if ($game_user->fkey_clans_id) {
  echo '<option value="clan">Clan</option>';
}
if ($game_user->can_broadcast_to_party || $game_user->meta == 'admin') {
  echo '<option value="neighborhood">' . $hood . '</option>';
}

echo <<< EOF
        <option value="values">$party</option>
      </select>
    </div>
    <div class="send-message-send-wrapper">
      <input class="send-message-send" type="submit" value="Send"/>
    </div>
  </form>
</div>
</div>
EOF;

foreach ($data as $item) {
// firep($item);

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

  $private_css .= ' ' . $item->type . ' ' . $item->type . '-' . $item->subtype;

  if (empty($item->username)) {
    $username = '';
    $reply = '';
  }
  else {
    $username = 'from ' . $item->ep_name . ' ' . $item->username . ' ' .
      $clan_acronym;
    if ($item->username != 'USLCE Game') {
      $reply = '<div class="message-reply-wrapper"><div class="message-reply">
        <a href="/' . $game . '/user/' . $arg2 . '/' . $item->phone_id .
        '">View / Respond</a></div></div>';
    }
    else {
      $reply = '';
    }
  }

  echo <<< EOF
<div class="news-item $item->type">
<div class="dateline">
  $display_time $username $private_text
</div>
<div class="message-body $private_css">
  <p>$item->message</p>$reply
</div>
</div>
EOF;
  $msg_shown = TRUE;

}

echo <<< EOF
</div>
</div>
<script type="text/javascript">
var isoNews = $('#all-text').isotope({
itemSelector: '.news-item',
layoutMode: 'fitRows'
});

$("#news-all").bind("click", function() {
isoNews.isotope({ filter: "*:not(.clan-msg)" });
$(".news-buttons button").removeClass("active");
$("#news-all").addClass("active");
});

$('#news-user').bind('click', function() {
isoNews.isotope({ filter: ".user" });
$(".news-buttons button").removeClass("active");
$("#news-user").addClass("active");
});

$('#news-challenge').bind('click', function() {
isoNews.isotope({ filter: ".challenge" });
$(".news-buttons button").removeClass("active");
$("#news-challenge").addClass("active");
});

$('#news-clan').bind('click', function() {
isoNews.isotope({ filter: ".hood, .clan, .values" });
$(".news-buttons button").removeClass("active");
$("#news-clan").addClass("active");
});

$('#news-mayor').bind('click', function() {
isoNews.isotope({ filter: ".mayor" });
$(".news-buttons button").removeClass("active");
$("#news-mayor").addClass("active");
});
</script>
<!--  <div id="personal-text">-->
EOF;

db_set_active('default');
