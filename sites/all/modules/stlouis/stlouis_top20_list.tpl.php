<?php

/**
 * @file
 * Show the top 20 players.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done
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
$game_user = $fetch_user();
$show_where = check_plain($_GET['where']);
$show_what = check_plain($_GET['what']);
zg_alter('top20_list_show_what', $game_user, $show_what);

if (empty($game_user->username) || $game_user->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

if ($debate == 'Box') {
  $title = 'Top Boxers';
}
/* elseif ($event_type == EVENT_GATHER_AMETHYST
 || $event_type == EVENT_AMETHYST_DONE) {
  $title = 'Top 20 Gatherers';
 } */
else {
  $title = 'Top 20 Players';
}

$data = [];

// if ($event_type == EVENT_GATHER_AMETHYST  || $event_type == EVENT_AMETHYST_DONE) {
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
// }
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
else {

  // Normal.
  // Base SQL statement.
  $sql = 'SELECT username, experience, initiative, endurance,
    elocution, debates_won, debates_lost, skill_points, luck,
    debates_last_time, users.fkey_values_id, level, phone_id,
    income, expenses, users.money, users.meta_int,
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

    case 'debate_ratio':
      $subtitle = $debate . 'win/lose ratio';
      $sql .= '
        ORDER BY (users.debates_won * 100 / users.debates_lost) DESC
      ';
      $sql2_what = '(users.debates_won * 100 / users.debates_lost)';
      $sql2_limit = ($game_user->debates_won * 100 / $game_user->debates_lost);
      break;

    case 'initiative':
      break;

    case 'endurance':
      break;

    case 'elocution':
      break;

    case 'income':
      $subtitle = $land;
      $sql .= '
        ORDER BY (users.income - users.expenses) DESC
      ';
      $sql2_what = '(users.income - users.expenses)';
      $sql2_limit = $game_user->income - $game_user->expenses;
      break;

    case 'cash':
      $subtitle = 'Cash on hand';
      $sql .= '
        ORDER BY users.money DESC
      ';
      $sql2_what = 'money';
      $sql2_limit = $game_user->money;
      break;

    case 'max_energy':
    case 'max_actions':
    case 'init_aides':
    case 'end_aides':
    case 'eloc_aides':
      break;

    case 'stolen_money':
      $subtitle = 'Stolen money received';
      $sql .= '
        ORDER BY users.meta_int DESC
      ';
      $sql2_what = 'meta_int';
      $sql2_limit = $game_user->meta_int;
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

  if ($show_what != 'income' && $show_what != 'cash') {
    $count = max($game_rank - 5, 0);
    $result = db_query($sql . ' LIMIT %d, 9;', $count);
    while ($item = db_fetch_object($result)) {
      $data[++$count] = $item;
    }
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
  'debate_ratio' => $debate . 's win/lose ratio (1 Action)',
  'initiative' => $initiative . ' (2 Actions)',
  'endurance' => $endurance . ' (2 Actions)',
  'elocution' => $elocution . ' (2 Actions)',
  'income' => $land . ' (3 Actions)',
  'cash' => 'Cash on Hand (3 Actions)',
  'max_energy' => 'Max ' . $game_text['energy'] . ' (3 Actions)',
  'max_actions' => 'Max Actions (3 Actions)',
  'init_aides' => 'Init Aides (5 Actions)',
  'end_aides' => 'End Aides (5 Actions)',
  'eloc_aides' => 'Eloc Aides (5 Actions)',
];

zg_alter('top20_list_what', $game_user, $what);

$total_cost = 0;
$where_title = $where[$show_where];
$what_title = $what[$show_what];
$where_matches = $what_matches = [];

preg_match('/\(([0-9]) Action/', $where_title, $where_matches);
preg_match('/\(([0-9]) Action/', $what_title, $what_matches);

if (array_key_exists(1, $where_matches)) {
  $total_cost += (int) $where_matches[1];
}
if (array_key_exists(1, $what_matches)) {
  $total_cost += (int) $what_matches[1];
}

$actions_paid = game_action_use($game_user, $total_cost);
if (!$actions_paid) {
  $data = [];
}

/* ------ VIEW ------ */
zg_fetch_header($game_user);
zg_show_elections_menu($game_user);

if (!$actions_paid) {
  print '<div class="land-failed">' . t('Out of Action!') .
    '</div><div class="try-an-election-wrapper"><div
      class="try-an-election"><a href="/' . $game . '/elders_do_fill/' .
    $arg2 . '/action?destination=/' . $game . '/top20/' . $arg2 .
    '">Refill your Action (1&nbsp;Luck)</a></div></div>';
}

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
  $disabled = ($what_name == 'experience' || $what_name == 'debates' ||
    $what_name == 'debate_ratio' || $what_name == 'income' || $what_name == 'cash' ||
    $game_user->meta == 'admin') ? '' : 'disabled';
  echo "<option value=\"$what_name\" $what_selected $disabled>$what_title</option>";
}

echo <<< EOF
    </select>
    <input class="land-buy-button" type="submit" Value="Go"/>
  </div>
</form>
<div class="subsubtitle">(Total cost: $total_cost Action(s))</div>
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

/*  if (empty($item->ep_name)) $item->ep_name = 'Subjugate'; */

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

/*
  if ($event_type == EVENT_GATHER_AMETHYST  || $event_type == EVENT_AMETHYST_DONE) {
    $exp = $item->meta_int;
    $experience = 'Raw Amethyst';
  } */

  // What to show.
  switch ($show_what) {
    case '':
    case 'experience':
      $data_point = "$exp $experience<br>(Level $item->level)";
      break;

    case 'debates':
      $data_point = number_format($item->debates_won) . " ${debate}s&nbsp;won";
      break;

    case 'debate_ratio':
      $data_point = sprintf('%2.02f%%', $item->debates_won * 100 / $item->debates_lost) .
        " ${debate} win/lose ratio";
      break;

    case 'initiative':
      break;

    case 'endurance':
      break;

    case 'elocution':
      break;

    case 'income':
      $data_point = $item->income - $item->expenses;

      if (!isset($data_point_standard)) {
        $data_point_standard = $data_point;
      }

      $percent = $data_point / $data_point_standard * 100;

      if ($game_user->meta == 'admin') {
        $data_point = number_format($data_point) . ' (' . sprintf('%02.02f', $percent) . '%)';
      }
      else {
        $data_point = sprintf('%02.02f', $percent) . '%';
      }
      break;

    case 'cash':
      $data_point = $item->money;

      if (!isset($data_point_standard)) {
        $data_point_standard = $data_point;
      }

      $percent = $data_point / $data_point_standard * 100;

      if ($game_user->meta == 'admin') {
        $data_point = number_format($data_point) . ' (' . sprintf('%02.02f', $percent) . '%)';
      }
      else {
        $data_point = sprintf('%02.02f', $percent) . '%';
      }
      break;

    case 'max_energy':
    case 'max_actions':
    case 'init_aides':
    case 'end_aides':
    case 'eloc_aides':
      break;

    case 'stolen_money':
      $data_point = '$tL ' . number_format($item->meta_int) . " received";
      break;
  }

  if (($item->weight != $last_weight) && $last_weight != '') {
    echo '</div><div class="elections">';
  }

/*  if (($event_type == EVENT_GATHER_AMETHYST
  || $event_type == EVENT_AMETHYST_DONE)
  && $count == 20)
    echo '</div><div class="title">The Rest</div><div class="elections">'; */

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

db_set_active();
