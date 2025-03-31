<?php

/**
 * @file stlouis_elections_challenge.tpl.php
 * Stlouis elections challenge
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_player();

if (empty($game_user->username) || $game_user->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

$sql = 'select id, name, has_elections, residents, district, is_limited,
  false as stats_only_hood
  from neighborhoods where id = %d;';
$result = db_query($sql, $game_user->fkey_neighborhoods_id);
$item = db_fetch_object($result);
zg_alter('elections_challenge_get_hood', $game_user, $item);
$location = $item->name;
$residents = $item->residents;
$district = $item->district;
$is_limited = $item->is_limited;
$stats_only_hood = (bool) $item->stats_only_hood;

if ($item->has_elections == 0) {

  echo <<< EOF
<div class="title">
  No Elections here!
</div>
<div class="subtitle">
  You're on vacation!&nbsp;
  Why worry about elections here?
</div>
EOF;

  zg_button();

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"election-failed none-here\"/>\n-->";
  }

  zg_karma($game_user, "No elections in current hood", -25);
  db_set_active();
  return;
}

$sql = 'SELECT elected_positions.id AS ep_id, elected_positions.energy_bonus,
  elected_positions.name AS ep_name, elected_positions.type,
  blah.*, `values`.party_icon,
  `values`.party_title, clan_members.fkey_clans_id

  FROM elected_positions

  LEFT OUTER JOIN (

-- type 1: neighborhood positions

  SELECT elected_officials.fkey_elected_positions_id,
      elected_officials.approval_rating,
      elected_officials.approval_15,
      elected_officials.approval_30,
      elected_officials.approval_45, users.*
    FROM elected_officials
    LEFT JOIN users ON elected_officials.fkey_users_id = users.id
    LEFT JOIN elected_positions
      ON elected_positions.id = elected_officials.fkey_elected_positions_id
    WHERE users.fkey_neighborhoods_id = %d
    AND elected_positions.type = 1

    UNION

-- type 2: party positions

    SELECT elected_officials.fkey_elected_positions_id,
      elected_officials.approval_rating,
      elected_officials.approval_15,
      elected_officials.approval_30,
      elected_officials.approval_45, users.*
    FROM elected_officials
    LEFT JOIN users ON elected_officials.fkey_users_id = users.id
    LEFT JOIN elected_positions
      ON elected_positions.id = elected_officials.fkey_elected_positions_id
    WHERE users.fkey_values_id = %d
    AND elected_positions.type = 2

    UNION

-- type 3: house positions

    SELECT elected_officials.fkey_elected_positions_id,
      elected_officials.approval_rating,
      elected_officials.approval_15,
      elected_officials.approval_30,
      elected_officials.approval_45, users.*
    FROM elected_officials
    LEFT JOIN users ON elected_officials.fkey_users_id = users.id
    LEFT JOIN elected_positions
      ON elected_positions.id = elected_officials.fkey_elected_positions_id
    WHERE users.fkey_neighborhoods_id IN
      (SELECT id from neighborhoods where district = %d)
    AND elected_positions.type = 3
  ) AS blah ON blah.fkey_elected_positions_id = elected_positions.id

  LEFT JOIN `values` ON blah.fkey_values_id = `values`.id

  LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = blah.id

  WHERE elected_positions.id = %d;';

$result = db_query($sql, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $district, $position_id);
$item = db_fetch_object($result);
firep($item, 'opponent object');

  // Labor day -- all are UWP - jwc.
//  $game_user->fkey_values_id = 7;

$username = $item->username;

if (empty($item->id)) {
  $title = "Run for the office of $item->ep_name";
}
else {
  $title = "Challenge $item->ep_name $username";
}

if ($item->id == $game_user->id) {

  echo <<< EOF
<div class="title">$title</div>
EOF;

  echo '<div class="subtitle">' . t('You cannot challenge yourself.') .
    '</div>';
  echo '<div class="subtitle">
<a href="/' . $game . '/elections/' . $arg2 . '">
  <img src="/sites/default/files/images/' . $game . '_continue.png"/>
</a>
</div>';

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"election-failed no-challenge-yourself\"/>\n-->";

  db_set_active();
  return;
}

// Not enough action left.
if ($game_user->actions < $item->energy_bonus) {

  zg_fetch_header($game_user);

  echo '<div class="title">' . $title . '</div>';
  echo '<div class="subtitle">' . t('Not enough Action!') .
    '</div>';
  zg_button('elders_do_fill',
    "Refill your Action (1&nbsp;$luck)",
    '/action?destination=/' . $game . '/elections/' . $arg2,
    'big-80 slide-in-content');

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"election-failed no-action\"/>\n-->";

  db_set_active();
  return;
}

// If you are running without an opponent, you automatically win.
if (empty($item->id)) {

  // Party office.
  if ($item->type == 2) {

    // Make it so player can't perform a major action for a day.
    zg_set_timer($game_user, 'next_major_action', 86400);
  }

  // You can only hold one position.
  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);

  $sql = 'insert into elected_officials set fkey_users_id = %d,
    fkey_elected_positions_id = %d;';
  $result = db_query($sql, $game_user->id, $position_id);

  $sql = 'insert into challenge_history
    (type, fkey_from_users_id, fkey_to_users_id, fkey_neighborhoods_id,
    fkey_elected_positions_id, won, desc_short, desc_long) values
    ("election", %d, NULL, %d, %d, 1, "' . $game_user->username .
    ' ran unopposed and automatically won.", "' . $game_user->username .
    ' ran unopposed and automatically won.")';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $position_id);

  $sql = 'update users set actions = actions - %d  where id = %d;';
  $result = db_query($sql, $item->energy_bonus, $game_user->id);

  // Start the actions clock, if needed.
  if ($game_user->actions == $game_user->actions_max) {
     $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
       $game_user->id);
  }

  $game_user = zg_fetch_player();
  zg_fetch_header($game_user);

  echo <<< EOF
<div class="title">$title</div>
EOF;

  echo '<div class="election-succeeded">' . t('Success!') . '</div>';
  echo '<div class="subtitle">' .
    t('You ran unopposed and automatically win!') . '</div>';
  zg_button('elections');

  $message = "$game_user->username ran unopposed for the seat $item->ep_name" .
    " in $location.";
  zg_slack($game_user, 'challenge', $item->ep_name, $message);

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"election-won\"/>\n-->";
  }

  $sql = 'insert into challenge_history
  (type, fkey_from_users_id, fkey_to_users_id, fkey_neighborhoods_id,
  fkey_elected_positions_id, won, desc_short, desc_long) values
  ("election", %d, NULL, %d, %d, %d, "%s", "%s");';
  $result = db_query($sql, $game_user->id,
    $game_user->fkey_neighborhoods_id, $position_id, 1,
    "$game_user->username challenged for the seat $item->ep_name in $location unopposed.",
    "$game_user->username challenged for the seat $item->ep_name in $location unopposed.");

  db_set_active();
  return;
}

// Too powerful to challenge.
if ($game_user->level > ($item->level + 15)) {
  echo '<div class="election-failed">' . t('Sorry!') . '</div>';
  echo t('<div class="subtitle">Your @influence is too high to challenge @name</div>',
    ['@influence' => $game_text['experience'], '@name' => $item->username]);
  zg_button('elections');
  db_set_active();
  return;
}

// Game on!  Election happens.
$election_polls = [];

$sql = 'SELECT sum(staff.initiative_bonus * staff_ownership.quantity)
  as initiative from staff
  left join staff_ownership on staff_ownership.fkey_staff_id = staff.id and
  staff_ownership.fkey_users_id = %d;';
$result = db_query($sql, $game_user->id);
$st_initiative_bonus = db_fetch_object($result);

$sql = 'SELECT sum(equipment.initiative_bonus * equipment_ownership.quantity)
  as initiative from equipment
  left join equipment_ownership
  on equipment_ownership.fkey_equipment_id = equipment.id and
  equipment_ownership.fkey_users_id = %d;';
$result = db_query($sql, $game_user->id);
$eq_initiative_bonus = db_fetch_object($result);

$in_bonus = $st_initiative_bonus->initiative +
  $eq_initiative_bonus->initiative + 100;
firep("Initiative bonus = " . $in_bonus);

$sql = 'SELECT sum(staff.extra_votes * staff_ownership.quantity)
  as extra_votes from staff
  left join staff_ownership on staff_ownership.fkey_staff_id = staff.id and
  staff_ownership.fkey_users_id = %d;';
$result = db_query($sql, $game_user->id);
$st_extra_votes = db_fetch_object($result);

$sql = 'SELECT sum(equipment.extra_votes * equipment_ownership.quantity)
  as extra_votes from equipment
  left join equipment_ownership
  on equipment_ownership.fkey_equipment_id = equipment.id and
  equipment_ownership.fkey_users_id = %d;';
$result = db_query($sql, $game_user->id);
$eq_extra_votes = db_fetch_object($result);

$extra_votes = $st_extra_votes->extra_votes + $eq_extra_votes->extra_votes;
game_alter('extra_votes', $game_user, $extra_votes, $extra_defending_votes);

// INCUMBENT's endurance.
$sql = 'SELECT sum(staff.endurance_bonus * staff_ownership.quantity)
  as endurance from staff
  left join staff_ownership on staff_ownership.fkey_staff_id = staff.id and
  staff_ownership.fkey_users_id = %d;';
$result = db_query($sql, $item->id);
$st_endurance_bonus = db_fetch_object($result);

$sql = 'SELECT sum(equipment.endurance_bonus * equipment_ownership.quantity)
  as endurance from equipment
  left join equipment_ownership
  on equipment_ownership.fkey_equipment_id = equipment.id and
  equipment_ownership.fkey_users_id = %d;';
$result = db_query($sql, $item->id);
$eq_endurance_bonus = db_fetch_object($result);

$en_bonus = $st_endurance_bonus->endurance +
  $eq_endurance_bonus->endurance + 100;
firep("Endurance bonus = $en_bonus");

$sql = 'SELECT sum(staff.extra_defending_votes * staff_ownership.quantity)
  as votes from staff
  left join staff_ownership on staff_ownership.fkey_staff_id = staff.id and
  staff_ownership.fkey_users_id = %d;';
$result = db_query($sql, $item->id);
$st_extra_defending_votes = db_fetch_object($result);

$sql = 'SELECT sum(equipment.extra_defending_votes * equipment_ownership.quantity)
  as votes from equipment
  left join equipment_ownership
  on equipment_ownership.fkey_equipment_id = equipment.id and
  equipment_ownership.fkey_users_id = %d;';
$result = db_query($sql, $item->id);
$eq_extra_defending_votes = db_fetch_object($result);

$extra_defending_votes = $st_extra_defending_votes->votes +
  $eq_extra_defending_votes->votes;
game_alter('extra_votes', $item, $extra_votes, $extra_defending_votes);

$my_influence = ceil($game_user->experience / 5) + ($game_user->initiative *
  $in_bonus);
firep("your total influence: $my_influence");

$opp_approval = max(($item->approval_rating + $item->approval_15 +
  $item->approval_30 + $item->approval_45) / 4, 10);

// Minimum of 10% average approval rating.
$opp_influence = ceil((ceil($item->experience / 5) + ($item->endurance *
  $en_bonus)) * $opp_approval * 0.017);

// 60% is a "normal" approval rating - multiplying by .017 = 1.02, close enough.
firep("opp total influence: $opp_influence");

$total_influence = $my_influence + $opp_influence + 100; // bias for incumbent
$votes = $extra_defending_votes - $extra_votes;
firep("Your $extra_votes voters vote for you");
firep("His/her $extra_defending_votes voters vote for him/her");

// Limited (ie, training) hood? Don't allow challengers with more than 100k.
// Influence for Alderman seat.
// Also don't allow defenders with more than 100k endurance.
if (($is_limited) && ($item->ep_id == 1)) {

  if ($opp_influence > 100000) {

    // Opponent has too much influence!
    $extra_votes += 10000;
  }
  elseif ($my_influence > 100000) {

    // Challenger has too much influence.
    echo '<div class="election-failed">' . t('Sorry!') . '</div>';
    echo '<div class="subtitle">' .
      t('Your influence is too high to challenge @user.', ['@user' => $item->username]) .
      '</div>';
    zg_button('elections');

    db_set_active();
    return;
  }

}

// Get voters.
$data = [];

// Neighborhood.
if ($item->type == 1) {

  $sql = 'SELECT users.*, clan_members.fkey_clans_id,
    ua_ip.`value` AS last_IP, ua_sdk.`value` AS sdk

    FROM users

    LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN user_attributes AS ua_ip
    ON users.id = ua_ip.fkey_users_id AND ua_ip.`key` =  "last_IP"

    LEFT OUTER JOIN user_attributes AS ua_sdk
    ON users.id = ua_sdk.fkey_users_id AND ua_sdk.`key` =  "sdk"

    WHERE (
      actions_next_gain >  "%s"
      OR energy_next_gain >  "%s"
    )

    AND fkey_neighborhoods_id = %d
    AND (SUBSTR( phone_id, 0, 4 ) <>  "sdk ")
    AND ua_sdk.`value` IS NULL
    AND username <>  ""
    AND meta <> "admin";';
  $result = db_query($sql, date('Y-m-d', REQUEST_TIME - 1728000),
    date('Y-m-d', REQUEST_TIME - 1728000),
    $game_user->fkey_neighborhoods_id);

}
elseif ($item->type == 2) {

  // Party.
  $sql = 'SELECT users.*, clan_members.fkey_clans_id,
    ua_ip.`value` AS last_IP, ua_sdk.`value` AS sdk

    FROM users

    LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN user_attributes AS ua_ip
    ON users.id = ua_ip.fkey_users_id AND ua_ip.`key` =  "last_IP"

    LEFT OUTER JOIN user_attributes AS ua_sdk
    ON users.id = ua_sdk.fkey_users_id AND ua_sdk.`key` =  "sdk"

    WHERE (
      actions_next_gain >  "%s"
      OR energy_next_gain >  "%s"
    )

    AND fkey_values_id = %d
    AND (SUBSTR( phone_id, 0, 4 ) <>  "sdk ")
    AND ua_sdk.`value` IS NULL
    AND username <> ""
    AND meta <> "admin";';
  $result = db_query($sql, date('Y-m-d', REQUEST_TIME - 1728000),
    date('Y-m-d', REQUEST_TIME - 1728000), $game_user->fkey_values_id);

}
elseif ($item->type == 3) {

  // District.
  $sql = 'SELECT users.*, clan_members.fkey_clans_id,
    ua_ip.`value` AS last_IP, ua_sdk.`value` AS sdk

    FROM users

    LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN user_attributes AS ua_ip
    ON users.id = ua_ip.fkey_users_id AND ua_ip.`key` =  "last_IP"

    LEFT OUTER JOIN user_attributes AS ua_sdk
    ON users.id = ua_sdk.fkey_users_id AND ua_sdk.`key` =  "sdk"

    WHERE (
      actions_next_gain >  "%s"
      OR energy_next_gain >  "%s"
    )

    AND fkey_neighborhoods_id IN
      (SELECT id from neighborhoods where district = %d)
    AND (SUBSTR( phone_id, 0, 4 ) <>  "sdk ")
    AND ua_sdk.`value` IS NULL
    AND username <>  ""
    AND meta <> "admin";';
  $result = db_query($sql, date('Y-m-d', REQUEST_TIME - 1728000),
    date('Y-m-d', REQUEST_TIME - 1728000), $district);

}

$votes_you_same_clan = $votes_you_same_party = $votes_you_influence =
  $votes_opp_same_clan = $votes_opp_same_party = $votes_opp_influence = 0;

while ($voter = db_fetch_object($result)) {
  $data[] = $voter;
}

foreach ($data as $voter) {
//firep($voter);
firep('voter IP is ' . $voter->last_IP);
$ip_key = $game_user->fkey_neighborhoods_id . '_' . $voter->last_IP;
$ip_array[$ip_key]++;

 // Only allow first five players from same IP address to vote.
 if (($ip_array[$ip_key] > 6) && (substr($voter->meta, 0, 3) != 'ai_')) {

    // Move to FP, zero actions.
    if ($game == 'stlouis') {
      $sql = 'update users set fkey_neighborhoods_id = 81, actions = 0,
        actions_next_gain = "%s" where id = %d;';
        game_user($game_user, "Number of players allowed exceeds the limit", -100);
    }
    else {

      // CG -- move to zagros.
      $sql = 'update users set fkey_neighborhoods_id = 11, actions = 0,
        actions_next_gain = "%s" where id = %d;';
      game_user($game_user, "Number of players allowed exceeds the limit", -100);
    }

    db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180), $voter->id);

    $sql = 'delete from elected_officials where fkey_users_id = %d;';
    db_query($sql, $voter->id);
/*
    mail('joseph@cheek.com', 'moving ' . $voter->username .
      ' to Forest Park', 'as s/he was voter number ' .
      $ip_array[$ip_key] . ' at IP address ' .
      $voter->last_IP . ' in ' . $location . '.');
*/

    // Vote doesn't count!
    continue;
  }

  if ($voter->id == $game_user->id) {

    // You vote for yourself.
    $votes--;
firep('you vote for yourself');
  }
  elseif ($voter->id == $item->id) {

    // S/he votes for her/himself.
    $votes++;
firep($item->username . ' votes for her/himself');
  }
  else {

    // Other voters.
/* 4th of July - check wall postings
    $sql = 'select * from user_messages
      where fkey_users_from_id = %d and fkey_users_to_id = %d
      and timestamp > "%s"
      order by timestamp DESC
      limit 1;';
    $result = db_query($sql, $voter->id, $game_user->id,
      date('Y-m-d H:i:s', REQUEST_TIME - 259200));
    $challenger_wall = db_fetch_object($result);
if (!empty($challenger_wall)) {
firep('voter posted to challenger:');
firep($challenger_wall);
}
    $sql = 'select * from user_messages
      where fkey_users_from_id = %d and fkey_users_to_id = %d
      and timestamp > "%s"
      order by timestamp DESC
      limit 1;';
    $result = db_query($sql, $voter->id, $item->id,
      date('Y-m-d H:i:s', REQUEST_TIME - 259200));
    $incumbent_wall = db_fetch_object($result);
if (!empty($incumbent_wall)) {
firep('voter posted to incumbent:');
firep($incumbent_wall);
}

    // Recent wall post to challenger.
    if (empty($incumbent_wall) &&
      !empty($challenger_wall) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for you.
      $votes--;
firep($voter->username .
' votes for you because s/he posted to your wall recently');
      $election_polls[] =
        'I voted for you because I posted to your wall recently.';
      $votes_you_same_clan++;
      continue;

    }

    // Recent wall post to incumbent.
    if (!empty($incumbent_wall) &&
      empty($challenger_wall) &&
      ((strtotime($incumbent_wall->timestamp) + 259200) > REQUEST_TIME) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for opponent.
      $votes++;
firep($voter->username .
' votes for your opponent because s/he posted to his/her wall recently');
      $election_polls[] =
        'I voted for your opponent because I posted to his/her wall recently.';
      $votes_opp_same_clan++;
      continue;

    }
*/

    // Labor day -- all players are UWP.
// $voter->fkey_clans_id = 7;

    // Same clan, used actions in last 3 days (challenger).
    if (!$stats_only_hood &&
      ($voter->fkey_clans_id > 0) &&
      ($voter->fkey_clans_id == $game_user->fkey_clans_id) &&
      ($voter->fkey_clans_id != $item->fkey_clans_id) &&
      (((strtotime($voter->actions_next_gain) + 259200) > REQUEST_TIME) ||
      ((strtotime($voter->energy_next_gain) + 259200) > REQUEST_TIME)) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for you.
      $votes--;
firep($voter->username . ' votes for you because you are in the same clan');
      $election_polls[] = 'I voted for you because we are in the same clan.';
      $votes_you_same_clan++;
      continue;
    }

    // Same clan, used actions in last 3 days (incumbent).
    if (!$stats_only_hood &&
      ($voter->fkey_clans_id > 0) &&
      ($voter->fkey_clans_id == $item->fkey_clans_id) &&
      ($voter->fkey_clans_id != $game_user->fkey_clans_id) &&
      (((strtotime($voter->actions_next_gain) + 259200) > REQUEST_TIME) ||
      ((strtotime($voter->energy_next_gain) + 259200) > REQUEST_TIME)) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for opponent.
      $votes++;
firep($voter->username . ' (' . $voter->fkey_neighborhoods_id . ') votes for ' . $item->username .
' because they are in the same clan');
      $election_polls[] = 'I voted for your opponent because we are in the same clan.';
      $votes_opp_same_clan++;
      continue;
    }

/* 4th of July - check wall postings
    $sql = 'select * from user_messages
      where fkey_users_to_id = %d and fkey_users_from_id = %d
      and timestamp > "%s"
      order by timestamp DESC
      limit 1;';
    $result = db_query($sql, $voter->id, $game_user->id,
      date('Y-m-d H:i:s', REQUEST_TIME - 604800));
    $challenger_wall = db_fetch_object($result);
if (!empty($challenger_wall)) {
firep('challenger posted to voter:');
firep($challenger_wall);
}

    $sql = 'select * from user_messages
      where fkey_users_to_id = %d and fkey_users_from_id = %d
      and timestamp > "%s"
      order by timestamp DESC
      limit 1;';
    $result = db_query($sql, $voter->id, $item->id,
      date('Y-m-d H:i:s', REQUEST_TIME - 604800));
    $incumbent_wall = db_fetch_object($result);
if (!empty($incumbent_wall)) {
firep('incumbent posted to voter:');
firep($incumbent_wall);
}


    // Recent wall post from challenger.
    if (empty($incumbent_wall) &&
      !empty($challenger_wall) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for you.
      $votes--;
firep($voter->username .
' votes for you because you posted to his/her wall');
      $election_polls[] =
        'I voted for you because you posted to my wall.';
      $votes_you_same_party++;
      continue;

    }

    // Recent wall post from incumbent.
    if (!empty($incumbent_wall) &&
      empty($challenger_wall) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for opponent.
      $votes++;
firep($voter->username . ' votes for ' . $item->username .
' because s/he posted to his/her wall');
      $election_polls[] =
        'I voted for your opponent because s/he posted to my wall.';
      $votes_opp_same_party++;
      continue;

    }
*/

    // Same party, used actions in last 7 days (challenger).
    if (!$stats_only_hood &&
      ($voter->fkey_values_id > 0) &&
      ($voter->fkey_values_id == $game_user->fkey_values_id) &&
      ($voter->fkey_values_id != $item->fkey_values_id) &&
      (((strtotime($voter->actions_next_gain) + 604800) > REQUEST_TIME) ||
      ((strtotime($voter->energy_next_gain) + 604800) > REQUEST_TIME)) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for you.
      $votes--;
firep($voter->username . ' votes for you because you are in the same party');
      $election_polls[] = 'I voted for you because we are in the same political party.';
      $votes_you_same_party++;
      continue;
    }

    // Same party, used actions in last 7 days (incumbent).
    if (!$stats_only_hood &&
      ($voter->fkey_values_id > 0) &&
      ($voter->fkey_values_id == $item->fkey_values_id) &&
      ($voter->fkey_values_id != $game_user->fkey_values_id) &&
      (((strtotime($voter->actions_next_gain) + 604800) > REQUEST_TIME) ||
      ((strtotime($voter->energy_next_gain) + 604800) > REQUEST_TIME)) &&
      (mt_rand(-50,50) <= $voter->level)) {

      // Vote for opponent.
      $votes++;
firep($voter->username . ' votes for ' . $item->username .
' because they are in the same party');
      $election_polls[] = 'I voted for your opponent because we are in the same political party.';
      $votes_opp_same_party++;
      continue;
    }

    $vote_rand = mt_rand(0, $total_influence);

    // Vote for me!
    if ($vote_rand < $my_influence) {

      // Voter votes for you.
      $votes--;
firep($voter->username . ' level ' . $voter->level . ' votes for you');
      $election_polls[] = 'I voted for you because of your ' . $experience . '.';
      $votes_you_influence++;

    }
    else {

      // Voter votes for incumbent.
      $votes++;
firep($voter->username . ' level ' . $voter->level . ' votes for ' . $item->username);
      $election_polls[] = 'I voted for your opponent because of his or her ' . $experience . '.';
      $votes_opp_influence++;

    }

  }

}

// Resident voters for type 1 (hood) elections.
$count = ($item->type == 1) ? $residents : 0;
while ($count--) {

  $vote_rand = mt_rand(0, $total_influence);

  // Vote for me!
  if ($vote_rand < $my_influence) {

    // Voter votes for you.
    $votes--;
firep('resident votes for you');
    $election_polls[] = 'I voted for you because of your ' . $experience . '.';
    $votes_you_influence++;

  }
  else {

    // Votes for incumbent.
    $votes++;
firep('resident votes for incumbent');
    $election_polls[] = 'I voted for your opponent because of his or her ' . $experience . '.';
    $votes_opp_influence++;

  }

}
firep('total votes are ' . $votes);
firep('voter IP array:');
firep($ip_array);

// Influence changed.
$experience_change = mt_rand(10 + ($game_user->level * 2),
  15 + ($game_user->level * 3));

$sql = 'select count(id) as count from challenge_history
  where fkey_from_users_id = %d and fkey_to_users_id = %d
  and timestamp > "%s";';
$result = db_query($sql, $game_user->id, $item->id, date('Y-m-d H:i:s', REQUEST_TIME - 3600));
$challenge_history = db_fetch_object($result);

// Sorry!  No experience!
if ($challenge_history->count > 5) {
  $experience_change = 0;
}

// You won!  Woohoo!
if ($votes < 0) {

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"election-won\"/>\n-->";

  $message = t('%office: %user1 has successfully challenged %user2 in %place.',
    [
      '%user1' => $game_user->username,
      '%user2' => $item->username,
      '%office' => $item->ep_name,
      '%place' => $game_user->location,
    ]
  );
  zg_slack($game_user, 'challenge', $item->ep_name, $message);

  // Party office.
  if ($item->type == 2) {

    // Make it so player can't perform a major action for a day.
    zg_set_timer($game_user, 'next_major_action', 86400);
  }

  if ($item->ep_id == 1) {

    // You beat the Alder - all type 1 officials in that hood lose their seats.
    $data = [];
    $sql = 'SELECT users.id FROM elected_officials
      left join users on elected_officials.fkey_users_id = users.id
      left join elected_positions on elected_officials.fkey_elected_positions_id = elected_positions.id
      where fkey_neighborhoods_id = %d and elected_positions.type = 1;';
    $result = db_query($sql, $game_user->fkey_neighborhoods_id);
    while ($official = db_fetch_object($result)) {
      $data[] = $official;
    }

    $message = t('%user1 has successfully challenged %user2 for the office of %office.&nbsp; You lose your seat.',
      [
        '%user1' => $game_user->username,
        '%user2' => $item->username,
        '%office' => $item->ep_name,
      ]
    );
    $official_ids = [];

    $sql = 'insert into challenge_messages (fkey_users_from_id,
        fkey_users_to_id, message) values (%d, %d, "%s");';
    foreach ($data as $official) {
      $result = db_query($sql, $game_user->id, $official->id, $message);
      $official_ids[] = $official->id;
    }

    $sql = 'delete from elected_officials
      where fkey_users_id in (%s);';
    $result = db_query($sql, implode(',', $official_ids));

    // Set a flag.
    $all_officials_in = $location;

  }

  $sql = 'delete from elected_officials where fkey_users_id = %d or
    fkey_users_id = %d;';

  // Incumbent lost.
  $result = db_query($sql, $game_user->id, $item->id);

  // And you lost your old position, if any.
  $sql = 'update users set actions = 0 where id = %d;';
  $result = db_query($sql, $item->id); // incumbent loses all actions

  // Start the incumbent actions clock if needed.
  if ($item->actions == $item->actions_max) {

     $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
       $item->id);

  }

  $sql = 'insert into elected_officials set fkey_users_id = %d,
    fkey_elected_positions_id = %d;';
  $result = db_query($sql, $game_user->id, $position_id);

  $sql = 'insert into challenge_messages
    (fkey_users_from_id, fkey_users_to_id, message)
    values (%d, %d, "%s");';
  $message = t('%user has successfully challenged you for your office!&nbsp; You are no longer %office and lost @exp influence.',
    [
      '%user' => $game_user->username,
      '%office' => $item->ep_name,
      '@exp' => $experience_change
    ]
  );
  $result = db_query($sql, $game_user->id, $item->id, $message);

  $sql = 'update users set experience = experience + %d,
    actions = actions - %d where id = %d;';
  $result = db_query($sql, $experience_change, $item->energy_bonus, $game_user->id);
  $sql = 'update users set experience = greatest(experience - %d, 0) where id = %d;';
  $result = db_query($sql, $experience_change, $item->id);

  // Start the actions clock if needed.
  if ($game_user->actions == $game_user->actions_max) {

    $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
      $game_user->id);

  }

  $game_user = zg_fetch_player();
  zg_fetch_header($game_user);

  echo <<< EOF
<div class="title">$title</div>
EOF;

  echo '<div class="election-succeeded">' . t('Success!') . '</div>';
  echo "<div class=\"subtitle\">You beat $item->username by " .
    (0 - $votes) . " vote(s)!</div>";

  if ($all_officials_in)
    echo '<div class="subtitle">' .
      t('All @hood officials in @place lose their seats',
        ['@place' => $all_officials_in, '@hood' => $game_text['hood']]) . '</div>';

}
else {

  $message = t('%office: %user1 has UNSUCCESSFULLY challenged %user2 in %place.',
    [
      '%user1' => $game_user->username,
      '%user2' => $item->username,
      '%office' => $item->ep_name,
      '%place' => $game_user->location,
    ]
  );
  zg_slack($game_user, 'challenge', $item->ep_name, $message);

  // You lost.
  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"election-lost\"/>\n-->";

  $sql = 'insert into challenge_messages
    (fkey_users_from_id, fkey_users_to_id, message)
    values (%d, %d, "%s");';
  $message = t('You have successfully defended yourself against a challenge ' .
    'from %user.  You remain %office and gain @exp influence.',
    ['%user' => $game_user->username, '%office' => $item->ep_name,
      '@exp' => $experience_change]);
  $result = db_query($sql, $game_user->id, $item->id, $message,
    $experience_change);

  $sql = 'update users set experience = greatest(experience - %d, 0),
    actions = actions - %d where id = %d;';
  $result = db_query($sql, $experience_change, $item->energy_bonus,
    $game_user->id);
  $sql = 'update users set experience = experience + %d where id = %d;';
  $result = db_query($sql, $experience_change, $item->id);

  // Start the actions clock if needed.
  if ($game_user->actions == $game_user->actions_max) {

     $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
       $game_user->id);

  }

  $game_user = zg_fetch_player();
  zg_fetch_header($game_user);

  echo <<< EOF
<div class="title">$title</div>
EOF;

  $experience_change = min($experience_change, $game_user->experience);

  // Don't tell s/he that s/he has lost more experience than s/he has.
  echo '<div class="election-failed">' . t('Defeated') . '</div>';
  echo "<div class=\"subtitle\">You lost to $item->username by $votes" .
    " vote(s)</div><div class=\"action-effect\">" .
    t('You lost @exp @influence', ['@exp' => $experience_change,
      '@influence' => $experience_lower]) .
    '</div>';

}

zg_button('elections');

$message = "$game_user->username [$my_influence] challenged $item->username " .
"[$opp_influence] for the seat $item->ep_name in $location and " .
(($votes < 0) ? 'won' : 'lost') . " by " . abs($votes) . " votes.

{$game_user->username}'s initiative = $st_initiative_bonus->initiative staff initiative + $eq_initiative_bonus->initiative equipment initiative + 100 = $in_bonus
total influence: ceil($game_user->experience influence / 5) [" .
ceil($game_user->experience / 5) .
"] + ($game_user->initiative initiative * $in_bonus initiative bonus) [" .
ceil($game_user->initiative * $in_bonus) . "] = $my_influence

Clan votes: $votes_you_same_clan
Party votes: $votes_you_same_party
Influence votes: $votes_you_influence
Extra votes: $extra_votes


{$item->username}'s endurance = $st_endurance_bonus->endurance staff endurance + $eq_endurance_bonus->endurance equipment endurance + 100 = $en_bonus
total influence: (ceil($item->experience influence / 5) [" .
ceil($item->experience / 5) .
"] + ($item->endurance endurance * $en_bonus endurance_bonus) [" .
($item->endurance * $en_bonus) .
"]) * max(($item->approval_rating + $item->approval_15 + " .
"$item->approval_30 + $item->approval_45) / 4, 10) [" .
max(($item->approval_rating + $item->approval_15 +
$item->approval_30 + $item->approval_45) / 4, 10) .
"] approval rating * 0.017) [" .
$opp_approval * 0.017 .
"] = $opp_influence

Clan votes: $votes_opp_same_clan
Party votes: $votes_opp_same_party
Influence votes: $votes_opp_influence
Extra defending votes: $extra_defending_votes


$residents residents";

// Mail me hood tosses.
//  if (($item->ep_id == 1) && ($votes < 0))
//    mail('joseph@cheek.com', "election results" /* for $game_user->username " .
//      "[$my_influence] vs. $item->username [$opp_influence] in $location" */,
//      $message);

// And house challenges.
//  if ($item->ep_id >= 28)
//    mail('joseph@cheek.com', "house seat results (district $district seat " .
//    "$item->ep_id)", $message);

$sql = 'insert into challenge_history
  (type, fkey_from_users_id, fkey_to_users_id, fkey_neighborhoods_id,
  fkey_elected_positions_id, won, desc_short, desc_long) values
  ("election", %d, %d, %d, %d, %d, "%s", "%s");';
$result = db_query($sql, $game_user->id, $item->id,
  $game_user->fkey_neighborhoods_id, $position_id, (($votes < 0) ? 1 : 0),
  "$game_user->username challenged $item->username " .
  "for the seat $item->ep_name in $location and " .
  (($votes < 0) ? 'won' : 'lost') . " by " . abs($votes) . " votes.",
  $message);

echo "<div class=\"subtitle\">Election poll results:</div>";
$total_polls = 2;

for ($c = 0 ; $c < $total_polls ; $c++) {

  $c1 = mt_rand(0, count($election_polls));

  echo "<div class=\"action-effect\">&quot;{$election_polls[$c1]}&quot;</div>";

  // So we don't get dups.
  unset($election_polls[$c1]);


}

db_set_active();
