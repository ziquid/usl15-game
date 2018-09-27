<?php

/**
 * @file stlouis_elders_do_reset.tpl.php
 * Stlouis elders do reset
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

  global $game, $phone_id;

  include drupal_get_path('module', $game) . '/game_defs.inc';
  $game_user = $fetch_user();

  $reset_me = check_plain(trim($_GET['reset_me']));

  // To prevent errant resets.
  if (strtoupper($reset_me) == 'RESET ME') {

    $sql = 'delete from elected_officials where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'UPDATE bank_accounts SET active = 0 where fkey_users_id = %d';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from equipment_ownership where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from land_ownership where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from staff_ownership where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from quest_completion where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from quest_group_completion where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from user_messages where fkey_users_to_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'delete from challenge_messages where fkey_users_to_id = %d;';
    $result = db_query($sql, $game_user->id);

    $sql = 'select * from clan_members where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);
    $item = db_fetch_object($result);

    // Clan leader? Delete entire clan.
    if ($item->is_clan_leader) {

      $sql = 'delete from clan_messages where fkey_neighborhoods_id = %d;';
      $result = db_query($sql, $game_user->fkey_clans_id);
      $sql = 'delete from clan_members where fkey_users_id = %d;';
      $result = db_query($sql, $item->id);
      $sql = 'delete from clans where id = %d;';
      $result = db_query($sql, $item->id);

    }
    else {

      $sql = 'delete from clan_members where fkey_users_id = %d;';
      $result = db_query($sql, $game_user->id);

    }

    $sql = 'delete from quest_completion where fkey_users_id = %d;';
    $result = db_query($sql, $game_user->id);

    $default_neighborhood = 81;
    $default_value = 'Greenbacks';

    if ($game_user->luck > 10) {
      $luck_in_sql = $game_user->luck;
    }
    else {
      $luck_in_sql = 10;
    }

    $sql = "UPDATE users
      SET username = '',
      password = '',
      referral_code = '',
      referred_by = '',
      experience = 0,
      level = 1,
      fkey_neighborhoods_id = %d,
      fkey_values_id = 0,
      `values` = '%s',
      money = 500,
      energy = 200,
      energy_max = 200,
      energy_next_gain = CURRENT_TIMESTAMP,
      income = 0,
      expenses = 0,
      income_next_gain = '0000-00-00 00:00:00',
      actions = 3,
      actions_max = 3,
      actions_next_gain = '0000-00-00 00:00:00',
      initiative = 1,
      endurance = 1,
      elocution = 1,
      debates_won = 0,
      debates_lost = 0,
      debates_last_time = '0000-00-00 00:00:00',
      last_bonus_date = '0000-00-00',
      skill_points = 0,
      luck = %d,
      seen_neighborhood_quests = 0,
      fkey_last_played_quest_groups_id = 0
      WHERE id = %d;";
    $result = db_query($sql, $default_neighborhood, $default_value,
      $luck_in_sql, $game_user->id);
    db_set_active('default');
    drupal_goto($game . '/welcome/' . $arg2);

  }
  else {
    db_set_active('default');
    drupal_goto($game . '/elders_ask_reset/' . $arg2,'msg=error');
    //drupal_goto($game . '/user/' . $arg2);
  }
