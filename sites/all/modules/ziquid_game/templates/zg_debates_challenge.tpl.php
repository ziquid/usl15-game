<?php

/**
 * @file
 * Debate results.
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
 * Removal of game_defs include: no
 * .
 */


global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$pl = zg_fetch_player();

if (empty($pl->username) || $pl->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

$q = $_GET['q'];

$sql = 'SELECT users.*,  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym

  from users

  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id

  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id

  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

  WHERE users.id = %d';

$result = db_query($sql, $opp_id);
$opp = db_fetch_object($result);
zg_alter('debates_challenge', $pl, $opp);
firep($opp, 'opponent');

$username = $opp->username;
$title = "$debate $opp->ep_name $username";

if ($opp->id == $pl->id) {

  zg_fetch_header($pl);

  echo "<div class=\"title\">$title</div>";
  echo '<div class="subtitle">' . t('You cannot @debate yourself.',
    ['@debate' => $debate_lower]) . '</div>';
  zg_button('debates');

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"debate-lost\"/>\n-->";
  }

  db_set_active();
  return;
}

if ($pl->actions == 0) {

  zg_fetch_header($pl);

  echo "<div class=\"title\">$title</div>";
  echo '<div class="subtitle">' . t('Out of Action!') .
    '</div>';
  zg_button('elders_do_fill', t('Refill your Action (1&nbsp;Luck)'),
    '/action?destination=/' . $q, 'big-68');

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"debate-no-action\"/>\n-->";
  }
  zg_slack($pl, 'ran-out-of', 'action',
    'Player "' . $pl->username .
    '" does not have enough Action (has ' . $pl->actions . ', needs ' .
    '1) to debate "' . $username . '".');

  db_set_active();
  return;
}

if (($opp->meta != 'zombie' && $opp->meta != 'debatebot' &&
  (REQUEST_TIME - strtotime($opp->debates_last_time)) <= $debate_wait_time) ||
  ($opp->meta == 'zombie' &&
  (REQUEST_TIME - strtotime($opp->debates_last_time)) <= $zombie_debate_wait)) {

  // Not long enough.
  zg_fetch_header($pl);

  echo "<div class=\"title\">$title</div>";
  echo '<div class="subtitle">' .
    t('You must wait longer to @debate this player',
    ['@debate' => $debate_lower]) . '</div>';
  zg_button('debates');

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"debate-must-wait\"/>\n-->";
  }

  db_set_active();
  return;
}

/*  if ($pl->experience > ($opp->experience * 2)) {

  // You cannot challenge someone if you have more than twice their influence.
  zg_fetch_header($pl);

  echo "<div class=\"title\">$title</div>";
  echo '<div class="election-failed">' . t('Sorry!') . '</div>';
  echo '<div class="subtitle">' .
    t('Your @experience is too high to challenge ',
      array('@experience' => $experience)) . $opp->username .
      '.</div>';
  echo '<div class="election-continue"><a href="/' . $game . '/debates/' .
    $arg2 . '">' . t('Continue') . '</a></div>';

  db_set_active();
  return;

}*/

// Otherwise, we need to apply a formula to get votes.

// Ex formulas.
$sql = 'SELECT sum(equipment.elocution_bonus * equipment_ownership.quantity)
  as elocution from equipment

  left join equipment_ownership
  on equipment_ownership.fkey_equipment_id = equipment.id and
  equipment_ownership.fkey_users_id = %d;';

$result = db_query($sql, $pl->id);
$elocution_bonus = db_fetch_object($result);

$sql = 'SELECT sum(staff.elocution_bonus * staff_ownership.quantity)
  as elocution from staff

  left join staff_ownership
  on staff_ownership.fkey_staff_id = staff.id and
  staff_ownership.fkey_users_id = %d;';

$result = db_query($sql, $pl->id);
$elocution_bonus_st = db_fetch_object($result);
firep('staff elocution bonus is ' . $elocution_bonus_st->elocution);

$my_el_bonus = $elocution_bonus->elocution + $elocution_bonus_st->elocution +
  50;

// Opponent's elocution.
$sql = 'SELECT sum(equipment.elocution_bonus * equipment_ownership.quantity)
  as elocution from equipment

  left join equipment_ownership
  on equipment_ownership.fkey_equipment_id = equipment.id and
  equipment_ownership.fkey_users_id = %d;';

$result = db_query($sql, $opp->id);
$elocution_bonus = db_fetch_object($result);

$sql = 'SELECT sum(staff.elocution_bonus * staff_ownership.quantity)
  as elocution from staff

  left join staff_ownership
  on staff_ownership.fkey_staff_id = staff.id and
  staff_ownership.fkey_users_id = %d;';

$result = db_query($sql, $opp->id);
$elocution_bonus_st = db_fetch_object($result);

$opp_el_bonus = $elocution_bonus->elocution + $elocution_bonus_st->elocution +
  50;

$my_influence = floor(sqrt(max(0, $pl->experience)) + ($pl->elocution *
  $my_el_bonus));
$opp_influence = floor(sqrt(max(0, $opp->experience)) + ($opp->elocution *
  $opp_el_bonus));
$message = "Your total influence: $my_influence: sqrt($pl->experience) +
($pl->elocution * $my_el_bonus).
<br>
Opponent's total influence: $opp_influence: sqrt($opp->experience) + ($opp->elocution *
  $opp_el_bonus).";

$money_change = mt_rand(5 + $pl->level, 10 + ($pl->level * 2));

// Don't change more than net income / 10 for each user.
$money_change = min($money_change,
  floor(($pl->income - $pl->expenses) / 10));
$money_change = min($money_change,
  floor(($opp->income - $opp->expenses) / 10));

// Don't let money get negative or more than double.
if ($money_change > $pl->money) {
  $money_change = $pl->money;
}
if ($money_change > $opp->money) {
  $money_change = $opp->money;
}
if ($money_change < 0) {
  $money_change = 0;
}

$won = $my_influence > $opp_influence;

// Log debate details.
$sql = 'insert into challenge_history
  (type, fkey_from_users_id, fkey_to_users_id, fkey_neighborhoods_id,
  fkey_elected_positions_id, won, desc_short, desc_long) values
  ("debate", %d, %d, %d, %d, %d, "%s", "%s");';
db_query($sql, $pl->id, $opp->id, 0, 0, ($won ? 1 : 0),
  "$pl->username debated $opp->username and " . ($won ? 'won' : 'lost') .
  " by a margin of " . abs($my_influence - $opp_influence) . '.',
  $message);

// Did you win?
if ($won) {

  zg_competency_gain($pl, 'challenger');

  // The experience you gain is based on their level.
  $experience_gained = mt_rand(floor($opp->level / 3),
    ceil($opp->level * 2 / 3));

  $sql = 'insert into challenge_messages
    (fkey_users_from_id, fkey_users_to_id, message)
    values (%d, %d, "%s");';
  $message = t('%user has successfully @debated you!  ' .
    'You lost @money @value.',
    ['%user' => $pl->username, '@money' => $money_change,
      '@value' => $opp->values, '@debated' => "{$debate_lower}d"]);
  $result = db_query($sql, $pl->id, $opp->id, $message);

  $sql = 'update users set money = money + %d, experience = experience + %d,
    actions = actions - 1, debates_won = debates_won + 1 where id = %d;';
  $result = db_query($sql, $money_change, $experience_gained, $pl->id);
  $sql = 'update users set money = money - %d, debates_lost = debates_lost + 1,
    debates_last_time = "%s" where id = %d;';
  $result = db_query($sql, $money_change, date('Y-m-d H:i:s', REQUEST_TIME), $opp->id);

  // Boxing day? Add boxing stats.
  if ($debate == 'Box') {

    // You get one boxing point for initiating the challenge.
    $boxing_points = $money_change + 1;

    $sql = 'update users set meta_int = meta_int + %d where id = %d;';
    $result = db_query($sql, $boxing_points, $pl->id);
    $sql = 'update users set meta_int = meta_int - %d + 1 where id = %d;';
    $result = db_query($sql, $money_change, $opp->id);
    $gain_extra = ' and ' . $boxing_points . ' Boxing Points<br/>';
  }
  else {
    $gain_extra = '';
  }

  // Start the actions clock if needed.
  if ($pl->actions == $pl->actions_max) {
     $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
       $pl->id);
  }

  $pl = zg_fetch_player();

  if ($event_type == EVENT_DEBATE) {
    $bump = '_' . $game . '_bump_event_tags_con';
    $reset = '_' . $game . '_reset_event_tags_con';
    $row = $bump($pl->id);
    $reset($opp->id);
    firep($row);
  }

  zg_fetch_header($pl);

  echo '<div class="election-succeeded">' . t('Success!') . '</div>';
  echo "<div class=\"subtitle\">You beat
    <a href=\"/$game/user/$arg2/id:$opp->id\">$opp->username</a></div>
    <div class=\"action-effect\">You gained
    $money_change $pl->values $gain_extra
    and $experience_gained $experience!</div>";

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"debate-won\"/>\n-->";

  if ($event_type == EVENT_DEBATE) {
    echo '<div class="subsubtitle">You have ' . $row->tags_con .
    ' consecutive debate win(s) and ' . $row->points . ' point(s)</div>';
  }

  $points_to_add = 0;

  // Zombies!
  if ($opp->meta == 'zombie') {

    zg_competency_gain($pl, 'zombie whisperer');
    if ($pl->debates_won >= ($pl->level * 100)
    || $pl->meta == 'admin') {

      // Beaten by super debater... evolve.
      $sql = 'select fkey_clans_id from clan_members
        where fkey_users_id = %d;';
      $result = db_query($sql, $pl->id);
      $clan_player = db_fetch_object($result);
      $sql = 'select fkey_clans_id from clan_members
        where fkey_users_id = %d;';
      $result = db_query($sql, $opp->id);
      $clan_zombie = db_fetch_object($result);

      if ($opp->fkey_values_id != $pl->fkey_values_id) {

        // First -- join party.
        $sql = 'update users set fkey_values_id = %d, `values` = "%s"
          where id = %d;';
        $result = db_query($sql, $pl->fkey_values_id,
          $pl->values, $opp->id);
        $sql = 'delete from clan_members where fkey_users_id = %d;';
        $result = db_query($sql, $opp->id);
        $sql = 'select party_title from `values`
          where id = %d;';
        $result = db_query($sql, $pl->fkey_values_id);
        $party = db_fetch_object($result);
        echo '<div class="subtitle">Because you are a super debater,<br/>' .
          $opp->username . ' now owes its allegiance to ' .
          $party->party_title . '.</div>';

        $points_to_add = 10;

//        slack_send_message("Zombie $opp->id ($opp->username)"
//        . " was beaten by super debater $pl->username"
//        . " and has switched to $party->clan_title!", $slack_channel);

      }
      else if
        (($clan_player->fkey_clans_id != $clan_zombie->fkey_clans_id) &&
        ($clan_player->fkey_clans_id > 0)) {

        // Second -- join clan.
        $sql = 'delete from clan_members where fkey_users_id = %d;';
        $result = db_query($sql, $opp->id);

        $sql = 'insert into clan_members
          (fkey_clans_id, fkey_users_id) values (%d, %d);';
        $result = db_query($sql, $clan_player->fkey_clans_id, $opp->id);

        $sql = 'select name from clans where id = %d;';
        $result = db_query($sql, $clan_player->fkey_clans_id);
        $clan_name = db_fetch_object($result);

        echo '<div class="subtitle">Because you are a super debater,<br/>' .
          $opp->username . ' has now joined your clan.</div>';

        $points_to_add = 15;

//        slack_send_message("Zombie $opp->id ($opp->username)"
//        . " was beaten by super debater $pl->username"
//        . " and has switched to $clan_name->name!", $slack_channel);

      // Already party and clan -- move them!
      } else if
        ((($clan_player->fkey_clans_id == $clan_zombie->fkey_clans_id) &&
        ($clan_player->fkey_clans_id > 0) &&
        ($opp->fkey_values_id == $pl->fkey_values_id))

        ) {
        // Move them!
        $hoods = [];
        $sql = 'select id, name from neighborhoods
          where has_elections = 1
          order by name ASC;';
        $result = db_query($sql);
        while ($hood = db_fetch_object($result)) {
          $hoods[] = $hood;
        }

        echo <<< EOF
<div class="subtitle">Because you are a super debater,<br/>
$opp->username will move to where you want it.</div>
<div class="title">To where should it move?</div>
EOF;

        foreach ($hoods as $hood) {
          echo <<< EOF
<div class="subsubtitle">
<a href="/$game/zombie_move/$phone_id/$opp->id/$hood->id">
$hood->name</a></div>
EOF;
        }

        $points_to_add = 20;
      }

    // Not beaten by a super debater.
    }
    else if ($opp->debates_lost > 4) {

        // Lost 5 debates.
        $sql = 'delete from users where id = %d;';
        $result = db_query($sql, $opp->id);
        $sql = 'delete from user_messages where fkey_users_from_id = %d
          or fkey_users_to_id = %d;';
        $result = db_query($sql, $opp->id, $opp->id);
        echo '<div class="subtitle">' . $opp->username .
          ' has retreated in shame.</div>';

        $points_to_add = 10;

//        slack_send_message("Conquered zombie $opp->id ($opp->username)"
//        . " has five debate losses and has left!", $slack_channel);

    }
    else {

      // Not 5 losses yet.
      $points_to_add = ($opp->debates_lost + 1) * 2;

//      slack_send_message("Unconquered zombie $opp->id ($opp->username)"
//      . " has less than five debate losses", $slack_channel);

    }

  }

  // Create entry for debater.
//  $sql = 'select * from event_points where fkey_users_id = %d;';
//  $result = db_query($sql, $pl->id);
//  $row = db_fetch_object($result);
//
//  if (empty($row)) {
//    $sql = 'insert into event_points set fkey_users_id = %d;';
//      $result = db_query($sql, $pl->id);
//  }

//  $points_to_add += (int) date('n') + min($row->tags_con, 10);

//  $sql = 'update event_points set points = points + %d
//      where fkey_users_id = %d;';
//    $result = db_query($sql, $points_to_add, $pl->id);

//  echo <<< EOF
//<!--<div class="subsubtitle">You gained $points_to_add point(s)</div>-->
//EOF;

  // Debatebots
  if ($opp->meta == 'debatebot') {

    zg_competency_gain($pl, 'beat a bot');

    $sql = 'select id from neighborhoods where has_elections = 1
      and id <> %d
      order by rand() limit 1;';
    $result = db_query($sql, $opp->fkey_neighborhoods_id);
    $hood = db_fetch_object($result);

    $sql = 'update users set fkey_neighborhoods_id = %d
      where id = %d;';
    $result = db_query($sql, $hood->id, $opp->id);

    echo '<div class="subtitle">' . $opp->username
    . ' has retreated to another region.</div>';

//    slack_send_message("$opp->username has lost a debate and moved to"
//      . " a new region.", $slack_channel);

    // Give debatebot another token.
    $sql = 'select * from equipment_ownership where fkey_users_id = %d
      and fkey_equipment_id = %d;';
    $result = db_query($sql, $opp->id, $opp->meta_int);
    $row = db_fetch_object($result);

    if (empty($row)) {
      $sql = 'insert into equipment_ownership set fkey_users_id = %d,
        fkey_equipment_id = %d, quantity = 1;';
    }
    else {
      $sql = 'update equipment_ownership set quantity = quantity + 1
        where fkey_users_id = %d and fkey_equipment_id = %d;';
    }
    db_query($sql, $opp->id, $opp->meta_int);

    // Give user a debatebot token, possibly.
    if (mt_rand(0, 2) == 0) {
      $sql = 'SELECT * FROM equipment_ownership WHERE fkey_users_id = %d
      AND fkey_equipment_id = %d;';
      $result = db_query($sql, $pl->id, $opp->meta_int);
      $row = db_fetch_object($result);

      if (empty($row)) {
        $sql = 'INSERT INTO equipment_ownership SET fkey_users_id = %d,
        fkey_equipment_id = %d, quantity = 1;';
      }
      else {
        $sql = 'UPDATE equipment_ownership SET quantity = quantity + 1
        WHERE fkey_users_id = %d AND fkey_equipment_id = %d;';
      }
      db_query($sql, $pl->id, $opp->meta_int);

      echo '<div class="subtitle">In haste to leave, ' . $opp->username
        . ' dropped a token.&nbsp; You pick it up.</div>';

//      slack_send_message("$opp->username left behind a token", $slack_channel);
    }

  }
/*
// flag day -- did they get a flag?
  $sql = 'select * from equipment_ownership where fkey_users_id = %d
    and fkey_equipment_id = 23;';
  $result = db_query($sql, $opp->id);
  $flag = db_fetch_object($result);

  // They had a flag -- you get it!
  if ($flag->quantity > 0) {

    echo '<div class="election-succeeded">You found a flag!</div>';
    echo '<div class="subtitle"><img
      src="/sites/default/files/images/equipment/stlouis-23.png"></div>';
    echo '<div class="subtitle">It will give you 1 Luck every 5 minutes</div>';
    $sql = 'update equipment_ownership set fkey_users_id = %d
      where id = %d;';
firep("update equipment_ownership set fkey_users_id = $pl->id
      where id = $flag->id");
    $result = db_query($sql, $pl->id, $flag->id);

  $sql = 'insert into challenge_messages
    (fkey_users_from_id, fkey_users_to_id, message)
    values (%d, %d, "%s");';
  $message = t('%user took your flag!',
    array('%user' => $pl->username));
  $result = db_query($sql, $pl->id, $opp->id, $message);

    mail('joseph@cheek.com', 'flag transfer',
      "$opp->username's flag was captured by $pl->username!");

  }
*/ // flag day
}
else {

  // You lost.
  zg_competency_gain($opp, 'defender');

  $experience_gained = mt_rand(floor($pl->level / 3),
    ceil($pl->level * 2 / 3));

  // The experience they gain is based on your level.
  $sql = 'insert into challenge_messages
    (fkey_users_from_id, fkey_users_to_id, message)
    values (%d, %d, "%s");';
  $message = t('You have successfully defended yourself against a @debate ' .
    'from %user.  You gained @money @value and @exp @experience.',
    ['%user' => $pl->username, '@money' => $money_change,
      '@value' => $opp->values, '@experience' => $experience,
      '@exp' => $experience_gained, '@debate' => $debate_lower]);
  $result = db_query($sql, $pl->id, $opp->id, $message);

  $sql = 'update users set money = money - %d, actions = actions - 1,
    debates_lost = debates_lost + 1 where id = %d;';
  $result = db_query($sql, $money_change, $pl->id);
  $sql = 'update users set money = money + %d,
    experience = experience + %d, debates_won = debates_won + 1,
    debates_last_time = "%s" where id = %d;';
  $result = db_query($sql, $money_change, $experience_gained,
    date('Y-m-d H:i:s', REQUEST_TIME), $opp->id);

  // Boxing day?  Add boxing stats.
  if ($debate == 'Box') {

    // You lose one less boxing point for initiating the challenge.
    $boxing_points = $money_change - 1;

    $sql = 'update users set meta_int = meta_int - %d where id = %d;';
    $result = db_query($sql, $boxing_points, $pl->id);
    $sql = 'update users set meta_int = meta_int + %d where id = %d;';
    $result = db_query($sql, $money_change, $opp->id);
    $gain_extra = ' and ' . $boxing_points . ' Boxing Points<br/>';
  }
  else {
    $gain_extra = '';
  }

  // Start the actions clock if needed.
  if ($pl->actions == $pl->actions_max) {
     $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
       $pl->id);
  }

  $pl = zg_fetch_player();

  if (in_array(EVENT_DEBATE, $event_type)) {
    $bump = '_' . $game . '_bump_event_tags_con';
    $reset = '_' . $game . '_reset_event_tags_con';
    $row = $reset($pl->id);
    $bump($opp->id);
    firep($row);
  }

  zg_fetch_header($pl);

  if ($opp->meta == 'zombie') {

    $sql = 'updates users set experience = experience + 500
      where id = %d;';
    $result = db_query($sql, $opp->id);
    echo '<div class="subtitle">' . $opp->username .
      ' has gained 500 ' . $experience . '.</div>';

//    slack_send_message("Zombie $opp->id won the debate!");

  }

  echo '<div class="election-failed">' . t('Defeated') . '</div>';
  echo "<div class=\"subtitle\">You lost to
  <a href=\"/$game/user/$arg2/id:$opp->id\">$opp->username</a></div>
    <div class=\"action-effect\">" .
    t('You lost @money @value' . $gain_extra, ['@money' => $money_change,
      '@value' => $pl->values]) .
    '</div>';

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"debate-lost\"/>\n-->";
  }

  if (in_array(EVENT_DEBATE, $event_type)) {
    echo '<div class="subsubtitle">You have ' . $row->tags_con .
    ' consecutive debate win(s) and ' . $row->points . ' point(s)</div>';
  }

/*

  // Flag day -- did they get a flag?
  $sql = 'select * from equipment_ownership where fkey_users_id = %d
    and fkey_equipment_id = 23;';
  $result = db_query($sql, $opp->id);
  $flag = db_fetch_object($result);

  // They had a flag, but you don't get it.
  if ($flag->quantity > 0) {

    echo '<div class="subtitle">He or she had a flag,
      but you don\'t get it</div>';

    slack_send_message("{$opp->username}'s flag was NOT captured by {$pl->username}!");

  }
*/
}

// YOU USED.
zg_button('debates');
echo "<div class=\"subtitle\">You used</div><div class=\"debate-used-wrapper\">";

$data = [];
$sql = 'SELECT equipment.id, equipment.elocution_bonus,
  "equipment" as type, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership
  ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE equipment.elocution_bonus > 0
  AND equipment_ownership.quantity > 0

  union

  SELECT staff.id, staff.elocution_bonus, "staff" as type,
  staff_ownership.quantity
  FROM staff

  LEFT OUTER JOIN staff_ownership
  ON staff_ownership.fkey_staff_id = staff.id
  AND staff_ownership.fkey_users_id = %d

  WHERE staff.elocution_bonus > 0
  AND staff_ownership.quantity > 0

  ORDER BY elocution_bonus DESC;';
$result = db_query($sql, $pl->id, $pl->id);

while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

if (empty($data)) {
  echo '<div class="debate-used">' . t('Nothing') . '</div>';
}

foreach ($data as $item) {
firep($item);

  echo <<< EOF
<div class="debate-used">
<img src="/sites/default/files/images/$item->type/$game-$item->id.png"
border="0"><span class="debate-num"> x $item->quantity</span></div>
EOF;

}

// OPPONENT USED.
echo "</div><div class=\"subtitle\">$username used</div>
  <div class=\"debate-used-wrapper\">";

$data = [];
$sql = 'SELECT equipment.id, equipment.elocution_bonus,
  "equipment" as type, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership
  ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE equipment.elocution_bonus > 0
  AND equipment_ownership.quantity > 0

  union

  SELECT staff.id, staff.elocution_bonus, "staff" as type,
  staff_ownership.quantity
  FROM staff

  LEFT OUTER JOIN staff_ownership
  ON staff_ownership.fkey_staff_id = staff.id
  AND staff_ownership.fkey_users_id = %d

  WHERE staff.elocution_bonus > 0
  AND staff_ownership.quantity > 0

  ORDER BY elocution_bonus DESC;';
$result = db_query($sql, $item_id, $item_id);
while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

if (empty($data)) {
  echo '<div class="debate-used">' . t('Nothing') . '</div>';
}

foreach ($data as $item) {
  firep($item, 'debate opp used');

  echo <<< EOF
<div class="debate-used">
<img src="/sites/default/files/images/$item->type/$game-$item->id.png"
border="0"><span class="debate-num"> x $item->quantity</span></div>
EOF;

}

zg_button('debates');
echo '<div>&nbsp;</div>';
db_set_active();
