<?php

/**
 * @file
 * Perform a character reset.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done
 * Ready for MVC separation: done
 * Controller moved to callback include: done
 * View only in theme template: N/A
 * All db queries in controller: yes
 * Minimal function calls in view: yes
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

/**
 * Callback for performing a player reset.
 *
 * @param string $phone_id
 *   Arg2.
 */
function zg_elders_do_reset_callback($phone_id) {

  if (empty($phone_id)) {
    drupal_access_denied();
    return;
  }

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $pl = zg_fetch_player();

  $reset_me = key_exists('reset_me', $_GET) ?
    strtoupper(check_plain(trim($_GET['reset_me']))) : '';

  // To prevent errant resets.
  if ($reset_me == 'RESET ME') {

    $d = zg_get_default(
      [
        'initial_hood',
        'initial_user_value',
      ]
    );

    $sql = 'delete from elected_officials where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'UPDATE bank_accounts SET active = 0 where fkey_users_id = %d';
    db_query($sql, $pl->id);

    $sql = 'delete from user_competencies where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from equipment_ownership where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from land_ownership where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from staff_ownership where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from quest_completion where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from quest_group_completion where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from user_messages where fkey_users_to_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from challenge_messages where fkey_users_to_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from goals_achieved where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'select * from clan_members where fkey_users_id = %d;';
    $item = db_query($sql, $pl->id)->fetch_object();

    // Clan leader? Delete entire clan.
    if ($item->is_clan_leader) {
      $sql = 'delete from clan_messages where fkey_neighborhoods_id = %d;';
      db_query($sql, $pl->fkey_clans_id);
      $sql = 'delete from clan_members where fkey_users_id = %d;';
      db_query($sql, $item->id);
      $sql = 'delete from clans where id = %d;';
      db_query($sql, $item->id);
    }
    else {
      $sql = 'delete from clan_members where fkey_users_id = %d;';
      db_query($sql, $pl->id);
    }

    $sql = 'delete from quest_completion where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $sql = 'delete from {user_attributes} where fkey_users_id = %d;';
    db_query($sql, $pl->id);

    $luck_in_sql = max($pl->luck, 10);

    $sql = "UPDATE users
      SET username = '(new player)',
      password = NULL,
      referral_code = NULL,
      referred_by = NULL,
      experience = 0,
      level = 1,
      -- last_access
      fkey_neighborhoods_id = %d,
      fkey_values_id = 0,
      `values` = '%s',
      money = 500,
      energy = 200,
      energy_max = 200,
      energy_next_gain = CURRENT_TIMESTAMP,
      income = 0,
      expenses = 0,
      income_next_gain = CURRENT_TIMESTAMP,
      actions = 3,
      actions_max = 3,
      actions_next_gain = CURRENT_TIMESTAMP,
      initiative = 1,
      endurance = 1,
      elocution = 1,
      debates_won = 0,
      debates_lost = 0,
      debates_last_time = CURRENT_TIMESTAMP,
      favors_asked_noncompleted = 0,
      favors_asked_completed = 0,
      favors_noncompleted = 0,
      favors_completed = 0,
      last_bonus_date = CURRENT_TIMESTAMP,
      skill_points = 0,
      luck = %d,
      -- karma
      seen_neighborhood_quests = 0,
      fkey_last_played_quest_groups_id = 0
      -- authKey
      -- meta
      -- meta_int
      WHERE id = %d;";
    db_query($sql, $d['initial_hood'], $d['initial_user_value'],
      $luck_in_sql, $pl->id);
    db_set_active();
    drupal_goto($game . '/welcome/' . $arg2, 'type=reset');

  }
  else {
    db_set_active();
    drupal_goto($game . '/elders_ask_reset/' . $arg2, 'msg=error');
  }
}
