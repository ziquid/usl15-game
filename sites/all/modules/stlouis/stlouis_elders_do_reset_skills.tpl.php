<?php

/**
 * @file stlouis_elders_do_reset_skills.tpl.php
 * Template for resetting skills.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $game_user = $fetch_user();

  $sql = 'SELECT count(quests.id) as bonus FROM `quest_group_completion`
    left outer join quests
    on quest_group_completion.fkey_quest_groups_id = quests.group
    WHERE fkey_users_id = %d and quests.active = 1;';
  $result = db_query($sql, $game_user->id);

  // Limited to 1 in db.
  $item = db_fetch_object($result);
  $skill_points = ($game_user->level * 4) + $item->bonus - 20;

  if ($game_user->skill_points == $skill_points) {
    db_set_active('default');
    drupal_goto($game . '/user/' . $arg2);
  }

  // Update his/her user entry.
  $sql = 'update users set energy_max = 200,
    skill_points = %d, initiative = 1, endurance = 1, actions = 3,
    actions_max = 3, elocution = 1, luck = luck - 3
    where id = %d;';
  $result = db_query($sql, $skill_points, $game_user->id);

  db_set_active('default');
  drupal_goto($game . '/user/' . $arg2);
