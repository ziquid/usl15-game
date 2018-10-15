<?php

/**
 * @file stlouis_quests_list.tpl.php
 * List of quests.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

global $game, $phone_id;

include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);

// Do AI moves from this page!!!
if (mt_rand(0, 5) == 1 || $game_user->meta == 'toxiboss' || $game_user->meta == 'admin') {
  include drupal_get_path('module', $game) . '/' . $game . '_ai.inc';
  game_move_ai();
}

if (is_numeric($arg3)) {
  $group_to_show = $arg3;
}

if (is_numeric($group_to_show)) {
  $sql_quest_neighborhood = 'where `group` = ' . $group_to_show;
}
else if ($game_user->level < 6) {

  // Show beginning quests.
  $group_to_show = '0';
  $sql_quest_neighborhood = 'where `group` = 0';
}
else if ($event_type == EVENT_CINCO_DE_MAYO && $game_user->fkey_neighborhoods_id == 30) {
  // Cinco De Mayo Quests.
  $group_to_show = '1100';
  $sql_quest_neighborhood = 'where `group` = 1100';
}
else {
  // Show the group for which the player last successfully completed a quest.
  $group_to_show = $game_user->fkey_last_played_quest_groups_id;
  $sql_quest_neighborhood = 'where `group` = ' . $group_to_show;
}

$location = $game_user->location;

$sql = 'select party_title from `values` where id = %d;';
$result = db_query($sql, $game_user->fkey_values_id);
$data = db_fetch_object($result);
$party_title = preg_replace('/^The /', '', $data->party_title);

// Show more welcome text for new user.
if ($game_user->experience == 0) {

  echo <<< EOF
<div class="welcome">
<div class="wise_old_man_small">
</div>
<p>The city elder continues.</p>
<p class="second">&quot;If you wish to lead this city,
  you will need to learn more about it.&nbsp; I have some things for you to
  do now.&nbsp; I will come back later when you are ready for more.</p>
  <p class="second">&quot;To perform a mission, touch its picture or title.&quot;</p>
<ul>
  <li>Each mission completed gives you more $game_user->values and
    $experience</li>
  <li>Wait and rest for a few minutes if you run out of Energy</li>
</ul>
</div>
EOF;
}

if ($game_user->fkey_values_id == 0 && $game_user->level >= 6 &&
  $game_user->level <= 25) {
  db_set_active('default');
  drupal_goto($game . '/choose_clan/' . $arg2 . '/0');
}

// Don't let them do quests at levels 6-25 without being in a party.
if (!$game_user->seen_neighborhood_quests && $game_user->level >= 6) {

  // Intro neighborhood quests at level 6.
  echo <<< EOF
<div class="welcome">
<div class="wise_old_man_small">
</div>
<!--<p>&quot;A wise choice &mdash; that party will serve you well.</p>-->
<p>&quot;Some of your $game_text['quest'] now depend on the part of the $city_lower in
  which you are located.&nbsp; You are now in the <strong>$location</strong>
  $hood_lower.&nbsp;
  You will find more $game_text['quest'] as you move to different parts of the
  $city_lower.&quot;</p>
<br/>
</div>
EOF;

  $sql = 'update users set seen_neighborhood_quests = 1 where id = %d;';
  $result = db_query($sql, $game_user->id);

}

// Keep location from user.
if ($game_user->level < 6) {
  $location = '';
}

if ($game_user->level < 6 and $game_user->experience > 0) {

  echo <<< EOF
<ul>
<li>Each $game_text['quest'] gives you more $game_user->values and $experience</li>
<li>Wait and rest for a few minutes if you run out of Energy</li>
</ul>
EOF;

}

$sql = 'select name from quest_groups where id = %s;';
$result = db_query($sql, $group_to_show);
$qg = db_fetch_object($result);
//firep($qg, 'quest group');

$location = str_replace('%location', $location, $qg->name);

if ($game_user->level < 6) {
  $location = '';
}

$sql = 'select name from quest_groups where id = %s;';
$result = db_query($sql, $group_to_show - 1);
$qgo = db_fetch_object($result);

if (!empty($qgo->name) && ($group_to_show <= 1000)) {

  $older_group = $group_to_show - 1;
  $older_missions_html = <<< EOF
<a href="/$game/quests/$arg2/$older_group">&lt;&lt;</a>
EOF;

}

$sql = 'select min(required_level) as min from quests
  where `group` = %d;';
$result = db_query($sql, $group_to_show + 1);
$item = db_fetch_object($result);
firep($item);

if (!empty($item->min) && ($item->min <= $game_user->level + 1) &&
  ($group_to_show <= 1000)) {

  $newer_group = $group_to_show + 1;
  $newer_missions_html = <<< EOF
<a href="/$game/quests/$arg2/$newer_group">&gt;&gt;</a>
EOF;

}

$quests = "$game_text['quest']";

echo <<< EOF
<div class="title">
$older_missions_html $location $quests $newer_missions_html
</div>
EOF;

// Admin?  Show all quests.
$active_quests = ($game_user->meta == 'admin') ? '' : 'and quests.active = 1';

list($quest_group, $percentage_target, $percentage_divisor, $next_group_html) =
  game_fetch_quest_group_stats($game_user, $active_quests, $group_to_show);

echo <<< EOF
<div class="quest-group-completion">
<strong>$quest_group->completed</strong> of $quest_group->total {$quest}s
complete $next_group_html
</div>
EOF;

// Show each quest.
$data = [];
$sql = 'select quests.*, quest_completion.percent_complete as completed_percent,
  neighborhoods.name as hood, comp1.name as competency_name_1
  from quests
  LEFT OUTER JOIN neighborhoods
  ON quests.fkey_neighborhoods_id = neighborhoods.id
  LEFT OUTER JOIN quest_completion
  ON quest_completion.fkey_quests_id = quests.id
  AND quest_completion.fkey_users_id = %d
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id
  ' . $sql_quest_neighborhood .
  ' and required_level <= %d ' . $active_quests .
  ' order by required_level ASC;';
//firep($sql);
$result = db_query($sql, $game_user->id, $game_user->level);

while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

foreach ($data as $item) {
  list($item->rgb, $item->width) = game_get_quest_completion($item->completed_percent,
    $percentage_target, $percentage_divisor);
  game_alter('quest_item', $game_user, $item);
  //    if ($event_type == EVENT_QUESTS_100)
  //      $item->required_energy = min($item->required_energy, 100);

  game_show_quest_slide($game_user, $item);
}

  // Don't show extra quests at first.
//  if ($game_user->level > 1) {

  $data = [];
  $sql = 'select * from quests ' . $sql_quest_neighborhood .
    ' and required_level = %d ' . $active_quests .
    ' order by required_level ASC;';
  $result = db_query($sql, $game_user->level + 1);

  while ($item = db_fetch_object($result)) $data[] = $item;

  foreach ($data as $item) {

//    if ($event_type == EVENT_QUESTS_100)
//      $item->required_energy = min($item->required_energy, 100);

    $description = str_replace('%party', "<em>$party_title</em>",
      $item->description);

    $active = ($item->active) ? '' : ' (inactive)';
    game_alter('quest_item', $game_user, $item);

    echo <<< EOF
<div class="quests-soon">
<div class="quest-name">$item->name $active</div>
<div class="quest-description">$description</div>
<div class="quest-required_level">Requires level $item->required_level</div>
<div class="quest-experience">+$item->experience $experience</div>
<div class="quest-money">+$item->min_money to $item->max_money
  $game_user->values</div>
<div class="quest-required_energy">Requires $item->required_energy energy</div>
</div>
EOF;

  }

//  }

db_set_active('default');
