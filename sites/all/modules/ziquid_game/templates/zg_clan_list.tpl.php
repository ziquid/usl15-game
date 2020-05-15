<?php

/**
 * @file
 * Clan list page.
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

global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();

if (empty($game_user->username) || $game_user->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

zg_fetch_header($game_user);

// Do AI moves from this page.
if (mt_rand(0, 5) == 1 || $game_user->meta == 'toxiboss' || $game_user->meta == 'admin') {
  include drupal_get_path('module', $game) . '/' . $game . '_ai.inc';
  //  zg_move_ai();
}

echo <<< EOF
<div class="news">
	<a href="/$game/clan_list/$arg2/$clan_id" class="button active">Clan List</a>
	<a href="/$game/clan_msg/$arg2/$clan_id" class="button">Clan Messages</a>
	<a href="/$game/clan_announcements/$arg2/$clan_id"
	  class="button">Announcements</a>
</div>
<div class="title">Clan List</div>
EOF;

$data = [];
$sql = 'SELECT username, experience, initiative, endurance, 
  elocution, debates_won, debates_lost, skill_points, luck,
  debates_last_time, users.fkey_values_id, level, phone_id,
  `values`.party_title, `values`.party_icon,
  `values`.name, users.id, users.fkey_neighborhoods_id,
  elected_positions.name as ep_name,
  elected_officials.approval_rating,
  clan_members.is_clan_leader,
  clans.name as clan_name, clans.acronym as clan_acronym,
  clans.rules as clan_rules,
  neighborhoods.name as location
  
  FROM `users`
  
  LEFT JOIN `values` ON users.fkey_values_id = `values`.id
  
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id
  
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
  
  WHERE clan_members.fkey_clans_id = %d
  ORDER by users.experience DESC;';

$result = db_query($sql, $clan_id);
while ($item = db_fetch_object($result)) {
  $data[] = $item;
}
db_set_active();

$num_members = count($data);

echo <<< EOF
<div>{$data[0]->clan_name} ({$data[0]->clan_acronym}) ($num_members members) -
  {$data[0]->clan_rules}</div>
<div class="elections-header">
  <div class="election-details">
    <div class="clan-title">Location</div>
    <div class="opponent-name">Name</div>
    <div class="opponent-influence">Stats</div>
  </div>
</div>
<div class="elections">
EOF;

foreach ($data as $item) {
  firep($item, 'clan member');

  $username = $item->username;
  $action_class = '';
  $official_link = $item->ep_name;
  $clan_class = 'election-details';

  if ($item->can_broadcast_to_party) {
    $official_link .= '<div class="can-broadcast-to-party">*</div>';
  }

  $official_link .= '<br/><a href="/' . $game . '/user/' .
    $arg2 . '/' . $item->phone_id . '"><em>' . $username . '</em></a>';

  $experience = $item->experience;
  $clan_acronym = '';

  if (!empty($item->clan_acronym)) {
    $clan_acronym = "($item->clan_acronym)";
  }

  if ($item->is_clan_leader) {
    $clan_acronym .= '*';
  }

  echo <<< EOF
<div class="$clan_class">
  <div class="clan-title">$item->location</div>
  <div class="opponent-name">$official_link $clan_acronym</div>
	<div class="opponent-influence">$experience Influence<br/>
		Level $item->level</div>
</div>
EOF;

}

?>
</div>
