<?php

/**
 * @file stlouis_choose_clan.tpl.php
 * Stlouis choose clan
 *
 * Synced with CG: no
 * Synced with 2114: no
 */

global $game, $phone_id;
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();

// If they have chosen a clan.
if ($clan_id != 0) {

  // No change?  Just show stats.
  if ($clan_id == $game_user->fkey_values_id)
    drupal_goto($game . '/user/' . $arg2);

  // Changing clans? Dock experience, bring level down to match.
  $new_experience = floor($game_user->experience * 0.75);
  if ($new_experience < 75) $new_experience = 75;

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
  $result = db_query($sql, $clan_id);
  $item = db_fetch_object($result);

  // Update his/her user entry.
  $sql = 'update users set fkey_neighborhoods_id = %d, fkey_values_id = %d,
    `values` = "%s", level = %d, experience = %d, energy_max = 200,
    skill_points = %d, initiative = 1, endurance = 1, actions = 3,
    actions_max = 3, elocution = 1
    where id = %d;';
  $result = db_query($sql, $item->fkey_neighborhoods_id, $clan_id,
    $item->name, $new_level, $new_experience, $new_skill_points,
    $game_user->id);

  // Remove Luck if changing clans.
  if ($game_user->fkey_values_id != 0) {

    $sql = 'update users set luck = luck - 5 where id = %d;';
    $result = db_query($sql, $game_user->id);

  }

  // Also delete any offices s/he held.
  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);

  // And any clan memberships s/he had (disband the clan if s/he was the leader).
  $sql = 'select * from clan_members where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  // Clan leader? Delete entire clan.
  if ($item->is_clan_leader) {

    $sql = 'delete from clan_messages where fkey_neighborhoods_id = %d;';
    $result = db_query($sql, $game_user->fkey_clans_id);
    $sql = 'delete from clan_members where fkey_clans_id = %d;';
    $result = db_query($sql, $item->fkey_clans_id);
    $sql = 'delete from clans where id = %d;';
    $result = db_query($sql, $item->fkey_clans_id);

  }
  else {

    $sql = 'delete from clan_members where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

  }

  // Add 24-hour waiting period on major actions.
  $set_value = '_' . arg(0) . '_set_value';
  $set_value($game_user->id, 'next_major_action', REQUEST_TIME + 86400);

  // First time choosing? Go to debates.
  if ($game_user->fkey_values_id == 0)
    drupal_goto($game . '/debates/' . $arg2);

  // Otherwise show your character profile.
  drupal_goto($game . '/user/' . $arg2);

}

// Otherwise they have not chosen a clan or are rechoosing one.

// New clan.
if ($game_user->level <= 6) {

$elder = 'You are met by the city elder again';

if ($game == 'celestial_glory')
  $elder = 'You see Lehi in a vision again';

  echo <<< EOF
<p>&nbsp;</p>
<div class="welcome">
<div class="wise_old_man_large">
</div>
<p>$elder.&nbsp; &quot;Well done,&quot; he
  says.&nbsp; &quot;I am impressed by what you have learned.</p>
<p class="second">&quot;In order to continue your journey, you will need a
  mentor.&nbsp; Your mentor will provide guidance and answer any questions
  that you may have.&nbsp; He or she should have provided you with a
  referral code.</p>
<p class="second">&quot;Alternatively, you can continue on your own without a
  code.&nbsp; Which do you prefer?&quot;</p>
</div>
<div class="try-an-election-wrapper">
<div class="try-an-election">
  <a href="/$game/enter_referral_code/$arg2">I have a referral code</a>
</div>
</div>
<div class="choose-clan">
<div class="subtitle">If you don't have a referral code, you may<br/>
instead choose a $party_small_lower:</div>
<br/>
EOF;

}
else {

  echo <<< EOF
<p>&nbsp;</p>
<div class="welcome">
<div class="wise_old_man_small">
</div>
<p>&quot;So you wish to join a different $party_small_lower.&nbsp; You will
  not rank as highly in that $party_small_lower as you do in your current
  one, but that is your choice.</p>
<p class="second">&quot;Which one do you prefer?&quot;</p>
</div>
<div class="choose-clan">
EOF;

}

$sql = 'SELECT * FROM  `values`
   where `values`.`user_selectable` = 1
   order by rand()';
$result = db_query($sql);
$data = array();

while ($item = db_fetch_object($result)) $data[] = $item;
firep($data, 'values');
foreach ($data as $item) {
  $value = strtolower($item->name);
  $icon = $game . '_clan_' . $item->party_icon . '.png';

  echo <<< EOF
<div>
<div class="choose-clan-icon"><img width="24"
  src="/sites/default/files/images/$icon"/></div>
<span class="choose-clan-name"><a
href="/$game/choose_clan/$arg2/$item->id"
style="color: #$item->color;">$item->party_title</a></span>
value $value</div>
<div class="choose-clan-slogan">$item->slogan</div>
EOF;

}

echo '</div>';

db_set_active('default');

