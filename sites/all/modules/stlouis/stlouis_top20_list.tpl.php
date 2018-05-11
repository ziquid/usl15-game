<?php

/**
 * @file stlouis_top20_list.tpl.php
 * Show the top 20 players.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 */

global $game, $phone_id;

$fetch_user = '_' . arg(0) . '_fetch_user';
$fetch_header = '_' . arg(0) . '_header';

$game_user = $fetch_user();
$fetch_header($game_user);
include drupal_get_path('module', $game) . '/game_defs.inc';
$arg2 = check_plain(arg(2));
$show_where = check_plain($_GET['where']);
$show_what = check_plain($_GET['what']);

if (empty($game_user->username)) {
  drupal_goto($game . '/choose_name/' . $arg2);
}

if ($debate == 'Box') {
  $title = 'Top Boxers';
}
//elseif ($event_type == EVENT_GATHER_AMETHYST
//|| $event_type == EVENT_AMETHYST_DONE) {
//  $title = 'Top 20 Gatherers';
//}
else {
  $title = 'Top 20 Players';
}

game_show_elections_menu($game_user);

$data = [];

//if ($event_type == EVENT_GATHER_AMETHYST  || $event_type == EVENT_AMETHYST_DONE) {
//
//  $sql = 'SELECT username, experience, initiative, endurance,
//    elocution, debates_won, debates_lost, skill_points, luck,
//    debates_last_time, users.fkey_values_id, level, phone_id,
//    `values`.clan_title, `values`.clan_icon,
//    `values`.name, users.id, users.fkey_neighborhoods_id,
//    elected_positions.name as ep_name,
//    elected_officials.approval_rating,
//    clan_members.is_clan_leader,
//    clans.name as clan_name, clans.acronym as clan_acronym,
//    neighborhoods.name as neighborhood,
//    users.meta_int,
//    "Raw Amethyst" as weight
//
//    FROM `users`
//
//    LEFT JOIN `values` ON users.fkey_values_id = `values`.id
//
//    LEFT OUTER JOIN elected_officials
//    ON elected_officials.fkey_users_id = users.id
//
//    LEFT OUTER JOIN elected_positions
//    ON elected_positions.id = elected_officials.fkey_elected_positions_id
//
//    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id
//
//    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
//
//    LEFT OUTER JOIN neighborhoods
//      ON users.fkey_neighborhoods_id = neighborhoods.id
//
//    where users.meta_int > 0
//
//    ORDER by users.meta_int DESC, users.experience ASC
//    LIMIT 100;';
//
//  $result = db_query($sql);
//  while ($item = db_fetch_object($result)) {
//    $data[] = $item;
//  }
//
//}
/*else*/ if ($debate == 'Box') {

  $already_listed = [];

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Heavyweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Cruiserweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 125
    AND users.id not in %s
    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Middleweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 110
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Welterweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 95
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Lightweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 80
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Featherweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 65
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Bantamweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 50
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Flyweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 35
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood,
    users.meta_int,
    "Minimumweight" as weight

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
      ON users.fkey_neighborhoods_id = neighborhoods.id

    WHERE users.level <= 20
    AND users.id not in %s

    ORDER by users.meta_int DESC
    LIMIT 3;';

  $result = db_query($sql, '(' . implode(',', $already_listed) . ')');
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
    $already_listed[] = $item->id;
  }

}
else { // normal

  // Base SQL statement.
  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    `values`.party_title, `values`.party_icon,
    `values`.name, users.id, users.fkey_neighborhoods_id,
    elected_positions.name as ep_name,
    -- elected_positions.id as ep_level,
    elected_officials.approval_rating,
    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    neighborhoods.name as neighborhood

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT OUTER JOIN neighborhoods
    ON users.fkey_neighborhoods_id = neighborhoods.id';

  // Order by.
  switch ($show_what) {
    case '':
    case 'experience':
      $show_what = 'experience';
      $subtitle = $experience;
      $sql .= '
        ORDER BY users.experience DESC
      ';
      $sql2_what = 'experience';
      $sql2_limit = $game_user->experience;
      break;

    case 'debates':
      $subtitle = $debate . 's won';
      $sql .= '
        ORDER BY users.debates_won DESC
      ';
      $sql2_what = 'debates_won';
      $sql2_limit = $game_user->debates_won;
      break;

  }

  $result = db_query($sql . ' LIMIT 20;');
  $count = 0;
  while ($item = db_fetch_object($result)) {
    $data[++$count] = $item;
  }

  // Player rank.
  $sql2 = 'select count(*) as count from users
    WHERE %s > %d;';
  $result = db_query($sql2, $sql2_what, $sql2_limit);
  $game_rank = db_fetch_object($result);
  $game_rank = $game_rank->count + 1;

  $count = max($game_rank - 5, 0);
  $result = db_query($sql . ' LIMIT %d, 9;', $count);
  while ($item = db_fetch_object($result)) {
    $data[++$count] = $item;
  }

}

$where = [
  'game' => t('Game-Wide (Free)'),
  'clan' => $clan . ' (2 Actions)',
  'party' => $party . ' (3 Actions)',
  'hood' => $hood . ' (5 Actions)',
];

$what = [
  'experience' => $experience . ' (Free)',
  'debates' => $debate . 's won (1 Action)',
  'initiative' => $initiative . ' (2 Actions)',
  'endurance' => $endurance . ' (2 Actions)',
  'elocution' => $elocution . ' (2 Actions)',
  'income' => $land . ' (3 Actions)',
];

echo <<< EOF
<form action="/$game/top20/$arg2">
  <div class="title">$title 
    <select name="where">
EOF;

foreach ($where as $where_name => $where_title) {
  $where_selected = ($where_name == $show_where) ? 'selected' : '';
  $disabled = ($where_name == 'game' || $game_user->meta == 'admin') ? '' : 'disabled';
  echo "<option value=\"$where_name\" $where_selected $disabled>$where_title</option>";
}

echo <<< EOF
    </select>
  </div>
  <div class="subtitle">Based on 
    <select name="what">
EOF;

foreach ($what as $what_name => $what_title) {
  $what_selected = ($what_name == $show_what) ? 'selected' : '';
  $disabled = ($what_name == 'experience' || $game_user->meta == 'admin') ? '' : 'disabled';
  echo "<option value=\"$what_name\" $what_selected $disabled>$what_title</option>";
}

echo <<< EOF
    </select>
    <input class="land-buy-button" type="submit" Value="Go"/>
  </div>
</form>
<div class="subsubtitle">(Your rank: $game_rank)</div>
<div class="elections-header">
<div class="election-details">
<div class="clan-title">$party</div>
<div class="opponent-name">Name</div>
<div class="opponent-influence">Stats</div>
</div>
</div>
<div class="elections">
EOF;

$last_weight = '';
$last_rank = 0;

foreach ($data as $rank => $item) {
firep($item, 'rank ' . $rank);

//  if (empty($item->ep_name)) $item->ep_name = 'Subjugate';

  $username = $item->username;
  $action_class = '';
  $official_link = $item->ep_name;
  if ($debate == 'Box') {
    $official_link = $item->weight;
  }
  $clan_class = 'election-details';

  if ($item->phone_id == $phone_id) {
    $clan_class .= ' me';
  }

  if ($item->can_broadcast_to_party) {
    $official_link .= '<div class="can-broadcast-to-party">*</div>';
  }

  $official_link .= '<br/><a href="/' . $game . '/user/' .
    $arg2 . '/id:' . $item->id . '"><em>' . $username . '</em></a>';

  $icon = $game . '_clan_' . $item->party_icon . '.png';
  $party_title = $item->party_title;
  $exp = number_format($item->experience);
  $clan_acronym = '';

  if (!empty($item->clan_acronym)) {
    $clan_acronym = "($item->clan_acronym)";
  }

  if ($item->is_clan_leader) {
    $clan_acronym .= '*';
  }

  if ($debate == 'Box') {
    $exp = $item->meta_int;
    $experience = 'Boxing Points';
  }

//  if ($event_type == EVENT_GATHER_AMETHYST  || $event_type == EVENT_AMETHYST_DONE) {
//    $exp = $item->meta_int;
//    $experience = 'Raw Amethyst';
//  }

  // What to show.
  switch ($show_what) {
    case '':
    case 'experience':
      $data_point = "$exp $experience<br>(Level $item->level)";
      break;

    case 'debates':
      $data_point = number_format($item->debates_won) . " ${debate}s&nbsp;won";
      break;

  }

  if (($item->weight != $last_weight) && $last_weight != '') {
    echo '</div><div class="elections">';
  }

//  if (($event_type == EVENT_GATHER_AMETHYST
//  || $event_type == EVENT_AMETHYST_DONE)
//  && $count == 20)
//    echo '</div><div class="title">The Rest</div><div class="elections">';

  if ($rank != $last_rank + 1) {
    echo <<< EOF
<div class="$clan_class"><br>
<div class="subsubtitle">. . .</div>
</div>
EOF;
  }

  echo <<< EOF
<div class="$clan_class">
<div class="clan-icon"><img width="24"
  src="/sites/default/files/images/$icon"/></div>
<div class="clan-title">$party_title</div>
<div class="rank">#$rank</div>
<div class="opponent-name">$official_link $clan_acronym</div>
<div class="opponent-influence">$data_point</div>
</div>
EOF;

  $last_weight = $item->weight;
  $last_rank = $rank;
}

echo '</div>';

db_set_active('default');
