<?php

/**
 * @file
 * Choose the player's political party.
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
$game_user = zg_fetch_user();

$d = zg_get_default(
  [
    'new_user_comm_member_msg',
    'welcome_page_1_speech',
    'welcome_page_2_speech',
  ]
) + zg_get_html(
  [
    'tagline',
    'welcome_page_1',
    'welcome_page_2',
  ]
);

// If they have chosen a party.
if ($party_id != 0) {

  // No change?  Just show stats.
  if ($party_id == $game_user->fkey_values_id) {
    db_set_active();
    drupal_goto($game . '/user/' . $arg2);
  }

  // Changing parties? Dock experience, bring level down to match.
  $new_experience = max(floor($game_user->experience * 0.75), 75);

  $sql = 'SELECT max(level) as new_level from levels where experience <= %d;';
  $result = db_query($sql, $new_experience);
  $item = db_fetch_object($result);
  $new_level = $item->new_level;

  $sql = 'SELECT count(quests.id) as bonus FROM `quest_group_completion`
    left outer join quests
    on quest_group_completion.fkey_quest_groups_id = quests.group
    WHERE fkey_users_id = %d and quests.active = 1;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  $new_skill_points = ($new_level * 4) + $item->bonus - 20;

  $sql = 'select * from `values` where id = %d;';
  $result = db_query($sql, $party_id);
  $item = db_fetch_object($result);

  // Update user entry.
  $sql = 'update users set fkey_neighborhoods_id = %d, fkey_values_id = %d,
    `values` = "%s", level = %d, experience = %d, energy_max = 200,
    skill_points = %d, initiative = 1, endurance = 1, actions = 3,
    actions_max = 3, elocution = 1
    where id = %d;';
  $result = db_query($sql, $item->fkey_neighborhoods_id, $party_id,
    $item->name, $new_level, $new_experience, $new_skill_points,
    $game_user->id);

  // Remove Luck if changing parties.
  if ($game_user->fkey_values_id != 0) {
    zg_luck($game_user, -5, $game_user->fkey_values_id, 0, $party_id,
      $game_user->username . ' changed parties to ' . $item->party_title . ' from ' . $game_user->party_title,
      'change_party', $item->party_title);
  }

  // Also delete any offices held.
  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);

  // And any clan memberships (disband if player was the leader).
  $sql = 'select * from clan_members where fkey_users_id = %d;';
  $item = db_query($sql, $game_user->id)->fetch_object();

  if ($item->is_clan_leader) {
    $sql = 'delete from clan_messages where fkey_neighborhoods_id = %d;';
    db_query($sql, $game_user->fkey_clans_id);
    $sql = 'delete from clan_members where fkey_clans_id = %d;';
    db_query($sql, $item->fkey_clans_id);
    $sql = 'delete from clans where id = %d;';
    db_query($sql, $item->fkey_clans_id);
  }
  else {
    $sql = 'delete from clan_members where fkey_users_id = %d;';
    db_query($sql, $game_user->id);
  }

  // Add 24-hour waiting period on major actions.
  zg_set_timer($game_user, 'next_major_action', 86400);

  db_set_active();

  // First time choosing? Go to debates.
  if ($game_user->fkey_values_id == 0) {
    drupal_goto($game . '/debates/' . $arg2);
  }

  // Otherwise show your character profile.
  drupal_goto($game . '/user/' . $arg2);
}

// Otherwise they have not chosen a party or are rechoosing one.
echo <<< EOF
<div class="title">
  <img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<div class="tagline">
  {$d['tagline']}
</div>
EOF;

if ($game_user->level <= 6) {

  $referral_code_button = zg_render_button('enter_referral_code', 'I have a referral code');

  // New party.
  echo <<< EOF
<div class="welcome">
<div class="wise_old_man_large">
</div>
<p>You are met by the city elder again.&nbsp; &quot;Well done,&quot; he
  says.&nbsp; &quot;I am impressed by what you have learned.</p>
<p class="second">&quot;In order to continue your journey, you will need a
  mentor.&nbsp; Your mentor will provide guidance and answer any questions
  that you may have.&nbsp; He or she should have provided you with a
  referral code.</p>
<p class="second">&quot;Alternatively, you can continue on your own without a
  code.&nbsp; Which do you prefer?&quot;</p>
</div>
$referral_code_button
<div class="choose-party">
<div class="subtitle">If you don't have a referral code, you may<br/>
instead choose a $party_small_lower:</div>
<br/>
EOF;
}
else {
  echo <<< EOF
<div class="welcome">
<div class="wise_old_man_small">
</div>
<p>&quot;So you wish to join a different $party_small_lower.&nbsp; You will
  not rank as highly in that $party_small_lower as you do in your current
  one, but that is your choice.</p>
<p class="second">&quot;Which one do you prefer?&quot;</p>
</div>
<div class="choose-party">
EOF;
}

$sql = 'SELECT * FROM  `values`
   where `values`.`user_selectable` = 1
   order by rand()';
$result = db_query($sql);
$data = [];

while ($item = db_fetch_object($result)) {
  $data[] = $item;
}
db_set_active();
firep($data, 'values');

foreach ($data as $item) {
  $value = strtolower($item->name);
  $icon = $game . '_party_' . $item->party_icon . '.png';

  echo <<< EOF
<div>
  <div class="choose-party-icon">
    <img width="24" src="/sites/default/files/images/$icon">
  </div>
  <span class="choose-party-name">
    <a href="/$game/choose_party/$arg2/$item->id"
      style="color: #$item->color;">
      $item->party_title
    </a>
  </span>
  value $value
</div>
<div class="choose-party-slogan">
  $item->slogan
</div>
EOF;
}

echo '</div>';
