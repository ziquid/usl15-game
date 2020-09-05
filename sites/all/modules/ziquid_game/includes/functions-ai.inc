<?php

/**
 * @file
 * AI moves.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * .
 */

include_once __DIR__ . '/functions-ai-plan.inc';

define('CONSERVE_ACTIONS', 1);
define('CONSERVE_MONEY', 2);

/**
 * Top-level bot entry point.
 */
function zg_move_ai() {

  // Turn off AI moves for now.
//  return;

  global $game, $ai_output, $bt_skip;

  // $game = check_plain(arg(0));.
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $game_user = zg_fetch_user();

  // PRE failed?  return.
  if (!zg_pre_move_ai($game, $game_user, $game_name_full, $ai_output)) {
    return $ai_output;
  };

//  $ai_goals = [
//    'list all ais', // [0 =>
//    ['list all ais'], // [0 =>
//    'ai1' => 'list all ais', // [ai1 =>
//    'ai2' => ['list all ais'], // [ai2 =>
//    ['ai3' => 'list all ais'], // ai3 =>

//    'list all ais', 1, 2, 3, // [2-5 =>
//    ['list all ais', 1, 2, 3], // [6 =>
//    'ai4' => 'list all ais', 1, 2, 3, // [ai4 =>
//    'ai5' => ['list all ais', 1, 2, 3], // [ai5 =>
//    ['ai6' => 'list all ais', 1, 2, 3], // ai6 =>
//    ['have num players', 1],
//    ['have num clan leaders', 1],
//    'build',
//  ];

  $ai_goals = [
    ['need_bots_num_type', 9, 'protester'],
    'protest',
    'build',
  ];

  zg_ai_out(zg_print_r($ai_goals));
  zg_ai_do_goals($ai_goals);

  //  zg_ai_do('have_num_interns', 21) && zg_ai_do('build');
  //  zg_ai_do('have_num_members', 18) && zg_ai_do('build');
  //  zg_ai_do('have_num_treasurers', 2) && zg_ai_do('build');
  //  zg_ai_do('have_num_alders', 1) && zg_ai_do('build');.

  zg_post_move_ai($game, $game_user, $ai_output);
  return $ai_output;
}

/**
 * Stuff to do before moving AIs.
 *
 * @param string $game
 *   The name of the game.
 * @param object $game_user
 *   The game user object.
 * @param string $game_name_full
 *   The full name of the game.
 * @param string $ai_output
 *   The AI log output.
 *
 * @return bool
 *   Whether this function succeeded or not.
 */
function zg_pre_move_ai($game, $game_user, $game_name_full, &$ai_output) {

  global $game, $ai_output, $bt_skip;

  $ai_pre_goals = [
    ['refresh', 3],
  ];

  // No need to do AI moves on a Windows Platform.
  if (!function_exists('sys_getloadavg')) {
    return FALSE;
  }

  // Nor on local.
  if ($_SERVER['HTTP_HOST'] == 'usl15.dd:8083') {
    //    return FALSE;
  }

  // No AI moves if load is too high.
  $load = sys_getloadavg();

  if ($load[0] > 7) {
    zg_slack(NULL, 'bots', 'warning',
      'not performing AI moves for ' .
      $game_name_full . " since load average is $load[0]");
    return FALSE;
  }

  db_set_active();
  lock_init();

  if (!lock_acquire('ai_move', 1.0)) {
    zg_slack($game_user, 'bots', 'warning',
      'not performing AI moves for ' .
      $game_name_full . " since lock could not be acquired");
    db_set_active('game_' . $game);
    return FALSE;
  }

  db_set_active('game_' . $game);
  $ai_output = '';
  $bt_skip = count(debug_backtrace());
  zg_ai_out('start: load avg ' . $load[0]);

  zg_ai_out(zg_print_r($ai_pre_goals));
  zg_ai_do_goals($ai_pre_goals);
  return TRUE;
}

/**
 * Stuff to do after moving AIs.
 *
 * @param $game
 * @param $game_user
 * @param $ai_output
 */
function zg_post_move_ai($game, $game_user, $ai_output) {
  if ($game_user->meta == 'toxiboss' || $game_user->meta == 'admin') {
    firep($ai_output, 'ai output');
  }

  // From devel?  send to dpm.
  if (arg(0) == 'devel') {
    dpm($ai_output, 'ai output');
  }

  // From cron?  Send to Slack.
  if (arg(0) == 'node') {
    zg_slack($game_user, 'bots', 'output', $ai_output);
  }

  db_set_active();
  lock_release('ai_move');
  db_set_active('game_' . $game);
}

/**
 * Log output.
 *
 * @param string $output
 *   The string to log.
 */
function zg_ai_out($output) {

  // Add output to $ai_output.
  global $ai_output, $bt_skip;

  $bt = debug_backtrace();
//  firep($bt, 'backtrace');
  $args = '(' . implode(', ', $bt[1]['args']) . ')';
  $line = '(L:' . $bt[0]['line'] . ')';
  $spaces = str_repeat('| ', count($bt) - $bt_skip);

  if ((count($bt) - $bt_skip) <= 2) {
    $ai_output .= "\n";
  }

  if ($bt[1]['function'] == 'zg_ai_do') {
    $ai_output .= $spaces . "$output\n";
  }
  else {
    $ai_output .= $spaces . "$line: $output\n";
  }

  // FIXME: this doesn't actually do anything.
  // Failsafe hack.
  // Too long -- kill script.
  if (strlen($ai_output) > 100000) {
    // mail('joseph@ziquid.com', 'ai output too long!', $ai_output);.
//    db_set_active();
    return;
  }

}

/**
 * Turn a set of goals into a set of detailed plans.
 *
 * @param string[] $goals
 *   What to do.
 *
 * @return string[]
 *   A detailed set of plans.
 */
function zg_ai_plan($goals) {

  $plans = [];
  foreach ($goals as $var => $goal) {
//    zg_ai_out('$var plus $goal is: ' . $var . ' => ' . print_r($goal, TRUE));

    // Accidentally did $key => ['a', 'b', ...]?  Turn it into $key => [$key => 'a', 'b', ...].
    if (is_array($goal) && !is_numeric($var)) {
      $first_value = reset($goal);
      $first_key = key($goal);
      if (is_numeric($first_key)) {
        array_shift($goal);
        $goal = [$var => $first_value] + $goal;
      }
    }

    // Used shorthand string syntax?  Convert into an array, preserving the key.
    if (!is_array($goal)) {
      $goal = [$var => $goal];
//      zg_ai_out('$var plus $goal is: ' . $var . ' => ' . print_r($goal, TRUE));
    }

    // Plan how to accomplish the goal.
    $plan_function = 'zg_ai_plan_' . str_replace(' ', '_', reset($goal));
    $args = array_slice($goal, 1);

    if (function_exists($plan_function)) {
      zg_ai_out('calling ' . $plan_function . '(' . implode(', ', $args) . ')');
      $temp_plan = call_user_func_array($plan_function, $args);

      // Used shorthand string syntax?  Convert into an array, preserving the key.
      if (!is_array($temp_plan)) {
        $temp_plan = [$var => $temp_plan];
      }

      // Have a variable (named key for first goal item)?  Ensure last plan uses
      // that key as its variable, if it doesn't have its own var already.
      if (!is_numeric($var)) {
        $last_value = end($temp_plan);
        $last_key = key($temp_plan);
        if (is_numeric($last_key)) {
          $temp_plan = array_slice($temp_plan, 0, -1) + [$var => $last_value];
        }
      }

      // Copy plan into $plans array.
      $plans[$var] = [
        '#original' => $goal,
      ] + zg_ai_plan($temp_plan);
//      foreach ($temp_plan as $key => $temp_plan_item) {
//        if (!is_array($temp_plan_item)) {
//          $plans[$var][$key] = [$key => $temp_plan_item];
//        }
//        else {
//          $plans[$var][$key] = $temp_plan_item;
//        }
//      }
    }
    else {
      zg_ai_out('warning: could not find function ' . $plan_function . '.');
      $plans[] = $goal;
    }
  }

  return $plans;
}

/**
 * Perform a set of goals.
 *
 * @param array[] $goals
 *   What to do.
 *
 * @return bool
 *   Whether the plans were successfully executed or not.
 */
function zg_ai_do_goals($goals) {

  $success = TRUE;
  foreach ($goals as $var => $goal) {
    if ($var === '#type') {
      if ($goal !== 'goals') {
        zg_ai_out('returning FALSE because #type != goals');
        return FALSE;
      }
      continue;
    }

    if (!is_array($goal)) {
      $goal = [$goal];
    }
    zg_ai_out('calling zg_ai_do with params of ' . zg_print_r($goal));
    $return = call_user_func_array('zg_ai_do', $goal);
    if (is_bool($return) && !$return) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Do something and log it.
 *
 * @param string $goal
 *   What to do.
 *
 * @return mixed|bool
 *   The return value from calling the function to do the goal or FALSE if
 *   that goal function has not yet been implemented.
 */
function zg_ai_do($goal) {

  global $last_value;
  // Work toward a specific goal, if not already accomplished.
  $goal_function = 'zg_ai_' . str_replace(' ', '_', $goal);
  $args = func_get_args();
  $args = array_slice($args, 1);

  if (!function_exists($goal_function)) {
    zg_ai_out("*FIXME: MISSING* $goal_function(" . implode(', ', $args) . ')');
    return FALSE;
  }

  zg_ai_out('calling ' . $goal_function . '(' . implode(', ', $args) . ')');
  $return = call_user_func_array($goal_function, $args);
  zg_ai_out('returned: ' . zg_print_r($return));
  $last_value = NULL;
  if (is_array($return) && array_key_exists('#type', $return) &&
    $return['#type'] == 'goals') {
    zg_ai_do_goals($return);
  }
  $last_value = $return;
  return $return;
}

/**
 * Generic function for building a random AI player.
 *
 * @return bool|mixed
 */
function zg_ai_build() {
  zg_ai_out('figuring out what is the best building course of action');

  // Find most energy.
  $sql = 'select phone_id, energy, actions from users
    where meta like "ai_%" and energy >= 20 and actions >= 1
    order by ((actions * 20) + energy) desc
    limit 1;';
  $result = db_query($sql);
  $item = db_fetch_object($result);

  if (empty($item)) {
    zg_ai_out("No bots with enough energy or actions!");
    return FALSE;
  }

  $ai_id = $item->phone_id;

  zg_ai_web_request($ai_id, 'home');
  $ai_user = zg_ai_fetch_user($ai_id);
  zg_ai_out("Building player #$ai_id with {$item->energy} energy, {$item->actions} action");

  if ($ai_user->skill_points > 0) {
    zg_ai_out('We have skill points to allocate!');
    return zg_ai_do('allocate skill points', $ai_id);
  }

  return zg_ai_do('do quest', $ai_id, 4) |
    zg_ai_do('do quest', $ai_id, 3) |
    zg_ai_do('do quest', $ai_id, 2) |
    zg_ai_do('do quest', $ai_id, 1) |
    zg_ai_do('debate players', $ai_id, 3);
}

/**
 * Perform a quest as a specific AI player.
 *
 * @param string $ai_id
 *   The phone ID of the AI player.
 * @param int $quest
 *   The quest ID.
 *
 * @return bool
 *   Whether the quest succeeded or failed.
 */
function zg_ai_do_quest($ai_id, $quest) {
  $ai_response = zg_ai_web_request($ai_id, 'quests_do', $quest);

  if ($ai_response == 'quest-succeeded') {
    return TRUE;
  }

  zg_ai_out('FIXME: Quest failed!!!  What to do?');
  $response_array = explode(' ', $ai_response);

  foreach ($response_array as $response) {

    if ($response == 'quest-failed') {
      continue;
    }
    elseif (strpos($response, 'need-staff-') === 0) {

      $staff_to_get = substr($response, 11);
      zg_ai_out("Trying to get Staff #$staff_to_get");

      // See if there is a quest that will loot that staff.
      $sql = 'select id, name from quests
        where fkey_loot_staff_id = %d
        order by rand() limit 1;';
      $result = db_query($sql, $staff_to_get);
      $item = db_fetch_object($result);

      if (!empty($item->id)) {

        zg_ai_out("Quest #$item->id, $item->name has Staff #$staff_to_get; " .
          'doing it!');
        $worked = zg_ai_do('do quest', $ai_id, $item->id);
        // yes, call recursively.
        if ($worked) {
          return FALSE;
        }
      }

      // next, check to see if we can purchase item.
      zg_ai_out("Ok, seeing if we can purchase Staff #$staff_to_get");
      zg_ai_do('purchase staff', $ai_id, $staff_to_get);
      return FALSE;
    }
    elseif (strpos($response, 'need-equipment-') === 0) {

      $eq_to_get = substr($response, 15);
      zg_ai_out("Trying to get Equipment #$eq_to_get");

      // See if there is a quest that will loot that eq.
      $sql = 'select id, name from quests
        where fkey_loot_equipment_id = %d
        order by rand() limit 1;';
      $result = db_query($sql, $eq_to_get);
      $item = db_fetch_object($result);

      if (!empty($item->id)) {

        zg_ai_out("Quest #$item->id, $item->name has Equipment #$eq_to_get; " .
          'doing it!');
        $worked = zg_ai_do('do quest', $ai_id, $item->id);

        // Yes, call recursively.
        if ($worked) {
          return FALSE;
        }

        // This quest may have succeeded, but goal failed.
      }

      // Next, check to see if we can purchase item.
      zg_ai_out("Ok, seeing if we can purchase Equipment #$eq_to_get");
      zg_ai_do('purchase equipment', $ai_id, $eq_to_get);
      return FALSE;
    }
  }

  zg_ai_out('FIXME: Not doing anything -- giving up!!!');
  return FALSE;
}

/**
 * Call a specific web request.
 *
 * @param string $ai_id
 *   The Phone ID of the AI player.
 * @param string $screen
 *   The name of the screen.
 * @param string $arg1
 *   Any additional arguments to the request.
 *
 * @return string
 *   The resulting string.
 */
function zg_ai_web_request($ai_id, $screen, $arg1 = '') {
  global $game, $base_url;

  $uri = $base_url . "/$game/$screen/$ai_id";
  if (strlen($arg1)) {
    $uri .= "/$arg1";
  }

//  if (($screen != 'home') &&
//    ($screen != 'debates_challenge') &&
//    ($screen != 'land')) {
    $ai_out = "web-req: $screen/$ai_id/$arg1";
//  }

  $response = file_get_contents($uri);
  $ai_response_start = strpos($response, "\n<ai ") + 5;

  if ($ai_response_start > 5) {
    $ai_response_end = strpos($response, "/>\n", $ai_response_start);
    $ai_response = trim(substr($response, $ai_response_start,
      $ai_response_end - $ai_response_start));
  }
  else {
    $ai_response = $response;
  }

  if (($screen != 'home')) {
//    ($screen != 'land-o')) {
    zg_ai_out($ai_out . ", response: $ai_response");
  }

  return trim(str_replace('"', '', $ai_response));
}

/**
 *
 */
function _ai_have_num_interns($num_interns) {

  // Goal: $num interns across the game.
  $sql = 'select count(users.id) as count from users
    left join elected_officials
    on elected_officials.fkey_users_id = users.id
    where users.meta like "ai_%"
    and fkey_elected_positions_id >= 7
    and fkey_elected_positions_id <= 11;';
  $result = db_query($sql);
  $item = db_fetch_object($result);

  zg_ai_out("Want $num_interns interns, have $item->count");

  if ($item->count >= $num_interns) {
    zg_ai_out('woohoo!  we have enough interns!  returning TRUE');
    return TRUE;
  }

  // Have an extra account.
  $extra_accts = 1;

  // Can't have $num interns if we don't have $num breachers. Do that first.
  if (!zg_ai_do('have num type level min max players',
    $num_interns + $extra_accts,
    'breacher', 6, 30)) {
    return FALSE;
  }

  // Can't have $num interns if we don't have $num holders. Do that next.
  if (!zg_ai_do('have num type level min max players',
    $num_interns + $extra_accts,
    'holder', 6, 30)) {
    return FALSE;
  }

  // Need some scouts to look for free seats, too.
  if (!zg_ai_do('have num type level min max players',
    ceil(($num_interns + $extra_accts) / 4),
    'scout', 12, 130)) {
    return FALSE;
  }

  $ai_user = zg_ai_find_free_player('breacher', 6, 30, 1);

  if (!empty($ai_user)) {

    zg_ai_out("Found a free breacher, $ai_user->phone_id " .
      "(level $ai_user->level)");

    zg_ai_do('hood seat challenge', $ai_user->phone_id, 7, 11);
    return FALSE;

  }

  zg_ai_out('What?  We couldn\'t find a single free breacher?');

  zg_ai_out("*FIXME*: we have $num_interns intern-capable players now, " .
    'but none of them have free actions.  What do I do?');

  return FALSE;

}

/**
 *
 */
function zg_ai_have_num_clan_leaders($num_leaders) {

  // Goal: $num clan leaders across the game.
  $sql = 'SELECT COUNT( users.id ) AS count
    FROM users
    LEFT JOIN clan_members ON clan_members.fkey_users_id = users.id
    WHERE users.phone_id LIKE "ai-%"
    AND clan_members.is_clan_leader =1;';
  $result = db_query($sql);
  $item = db_fetch_object($result);

  zg_ai_out("Want $num_leaders clan leaders, have $item->count");

  if ($item->count >= $num_leaders) {
    zg_ai_out('woohoo!  we have enough clan leaders!  returning TRUE');
    return TRUE;
  }

  // Find a free player. Make it a holder, just cause they don't do much.
  $ai_user = zg_ai_do('find free player', 'holder', 20, 150, 5);

  if (!empty($ai_user)) {

    zg_ai_out("Found a free player, $ai_user->phone_id " .
      "(level $ai_user->level)");

    zg_ai_out('Checking for necessary equipment ' . "(money: $ai_user->money)");

    // equip_id => level.
    $stuff_to_get = [
      6 => 15,
      10 => 30,
      13 => 45,
      28 => 60,
    ];

    $sql = 'select quantity from equipment_ownership
      where fkey_equipment_id = %d and fkey_users_id = %d;';

    foreach ($stuff_to_get as $equip_id => $level) {

      if ($ai_user->level >= $level) {

        zg_ai_out("At level $level; maybe I can buy equip #$equip_id?");

        $result = db_query($sql, $equip_id, $ai_user->id);
        $item = db_fetch_object($result);

        if ($item->quantity >= 1) {
          zg_ai_out('Already have one!');
          continue;
        }

        zg_ai_out('I don\'t have one... trying to get it!');
        zg_ai_web_request($ai_user->phone_id, 'equipment_buy', "$equip_id/1");

      }

    }

    // FIXME: increment clan # if it doesn't exist.
    $digit = '1'; mt_rand(0, 9);
    $name = 'ToxiCorp%20' . $digit;
    $acronym = 'TC' . $digit;

    zg_ai_web_request($ai_user->phone_id, 'actions_do',
      '1?name=' . $name . '&acronym=' . $acronym);

    return FALSE;
  }

  zg_ai_out('What?  We couldn\'t find a single free holder?');

  zg_ai_out("*FIXME* " . __LINE__);
  return FALSE;

}

/**
 *
 */
function _ai_have_num_members($num_members) {

  // Goal: $num members across the game.
  $sql = 'select count(users.id) as count from users
    left join elected_officials
    on elected_officials.fkey_users_id = users.id
    where users.meta like "ai_%"
    and fkey_elected_positions_id >= 4
    and fkey_elected_positions_id <= 6;';
  $result = db_query($sql);
  $item = db_fetch_object($result);

  zg_ai_out("Want $num_members members, have $item->count");

  if ($item->count >= $num_members) {
    zg_ai_out('woohoo!  we have enough members!  returning TRUE');
    return TRUE;
  }

  // Have an extra account.
  $extra_accts = 1;

  // Can't have $num interns if we don't have $num breachers. Do that first.
  if (!zg_ai_do('have num type level min max players',
    $num_members + $extra_accts,
    'breacher', 20, 45)) {
    return FALSE;
  }

  // Can't have $num interns if we don't have $num holders. Do that next.
  if (!zg_ai_do('have num type level min max players',
    $num_members + $extra_accts,
    'holder', 20, 45)) {
    return FALSE;
  }

  // Need some scouts to look for free seats, too.
  if (!zg_ai_do('have num type level min max players',
    ceil(($num_members + $extra_accts) / 4),
    'scout', 12, 130)) {
    return FALSE;
  }

  $ai_user = zg_ai_find_free_player('breacher', 20, 45, 3);

  if (!empty($ai_user)) {

    zg_ai_out("Found a free breacher, $ai_user->phone_id " .
      "(level $ai_user->level)");

    zg_ai_do('hood seat challenge', $ai_user->phone_id, 4, 6);
    return FALSE;

  }

  zg_ai_out('What?  We couldn\'t find a single free breacher?');

  zg_ai_out("*FIXME*: we have $num_members member-capable players now, " .
    'but none of them have free actions.  What do I do?');

  return FALSE;

}

/**
 *
 */
function _ai_have_num_treasurers($num) {

  // Goal: $num treasurers across the game.
  $sql = 'select count(users.id) as count from users
    left join elected_officials
    on elected_officials.fkey_users_id = users.id
    where users.meta like "ai_%"
    and fkey_elected_positions_id = 3;';
  $result = db_query($sql);
  $item = db_fetch_object($result);

  zg_ai_out("Want $num treasurers, have $item->count");

  if ($item->count >= $num) {
    zg_ai_out('woohoo!  we have enough!  returning TRUE');
    return TRUE;
  }

  // Have an extra account.
  $extra_accts = 1;

  // Can't have $num treasurers if we don't have $num breachers. Do that first.
  if (!zg_ai_do('have num type level min max players',
    $num + $extra_accts,
    'breacher', 35, 55)) {
    return FALSE;
  }

  // Can't have $num treasurers if we don't have $num holders. Do that next.
  if (!zg_ai_do('have num type level min max players',
    $num + $extra_accts,
    'holder', 35, 55)) {
    return FALSE;
  }

  // Need some scouts to look for free seats, too.
  if (!zg_ai_do('have num type level min max players',
    ceil(($num + $extra_accts) / 4),
    'scout', 12, 130)) {
    return FALSE;
  }

  $ai_user = zg_ai_find_free_player('breacher', 35, 55, 5);

  if (!empty($ai_user)) {

    zg_ai_out("Found a free breacher, $ai_user->phone_id " .
      "(level $ai_user->level)");

    zg_ai_do('hood seat challenge', $ai_user->phone_id, 3, 3);
    return FALSE;

  }

  zg_ai_out('What?  We couldn\'t find a single free breacher?');

  zg_ai_out("*FIXME*: we have $num_members treasurer-capable players now, " .
    'but none of them have free actions.  What do I do?');

  return FALSE;

}

/**
 *
 */
function _ai_have_num_alders($num_alders) {

  // Goal: $num alders across the game.
  $sql = 'select count(users.id) as count from users
    left join elected_officials
    on elected_officials.fkey_users_id = users.id
    where users.meta like "ai_%"
    and fkey_elected_positions_id = 1;';
  $result = db_query($sql);
  $item = db_fetch_object($result);

  zg_ai_out("Want $num_alders alders, have $item->count");

  if ($item->count >= $num_alders) {
    // zg_ai_out('woohoo!  we have enough alders!  returning TRUE');.
    return TRUE;
  }

  // Can't have $num alders if we don't have $num breachers.  Do that first.
  if (!zg_ai_do('have num type level min max players', $num_alders,
    'breacher', 50, 70)) {
    return FALSE;
  }

  // Can't have $num alders if we don't have $num holders. Do that next.
  if (!zg_ai_do('have num type level min max players', $num_alders,
    'holder', 50, 70)) {
    return FALSE;
  }

  // Need some scouts to look for free seats, too.
  if (!zg_ai_do('have num type level min max players', floor($num_alders / 4),
    'scout', 12, 130)) {
    return FALSE;
  }

  $ai_user = zg_ai_find_free_player('breacher', 50, 70, 10);

  if (!empty($ai_user)) {

    zg_ai_out("Found a free breacher, $ai_user->phone_id " .
      "(level $ai_user->level)");

  }
  else {

    zg_ai_out('*FIXME*  Egads!  No free breachers found!');

  }

  zg_ai_out("*FIXME*: we have $num_alders alder-capable players now.
    What do I do?");

  return TRUE;

}

/**
 *
 */
function zg_ai_find_free_player($type, $level_min, $level_max, $min_actions = 0) {

  zg_ai_out("checking for free $type between levels $level_min and " .
    "$level_max, inclusive, with $min_actions free action(s)");

  $sql = 'select phone_id from users
    left join elected_officials
    on elected_officials.fkey_users_id = users.id
    where users.meta = "%s"
    and level >= %d
    and level <= %d
    and fkey_elected_positions_id is NULL
    and actions >= %d
    order by (energy + (actions * 40)) DESC
    limit 10;';
  $result = db_query($sql, "ai_$type", $level_min, $level_max, $min_actions);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  if (!empty($data)) {
    return zg_ai_fetch_user($data[mt_rand(0, count($data) - 1)]->phone_id);
  }

  zg_ai_out("No free $type ... looking for one with extra skill points");

  $sql = 'select phone_id from users
    left join elected_officials
    on elected_officials.fkey_users_id = users.id
    where users.meta = "%s"
    and level >= %d
    and level <= %d
    and fkey_elected_positions_id is NULL
    and skill_points > 1
    order by skill_points DESC
    limit 1;';
  $result = db_query($sql, "ai_$type", $level_min, $level_max);
  $item = db_fetch_object($result);

  if (!empty($item)) {

    _ai_allocate_skill_for_action($item->phone_id);
    $ai_user = zg_ai_fetch_user($item->phone_id);

    if ($ai_user->actions >= $min_actions) {
      return $ai_user;
    }

  }

  // If not: do we have a player of $type that is a lower level?
  // If so, we can just boost.
  zg_ai_out("Still no free $type ... boosting one");
  $data = [];

  $sql = 'select phone_id, level, energy, actions from users
    where meta like "%s"
    and level < %d
    order by (energy + (actions * 40)) desc
    limit 10;';
  $result = db_query($sql, "ai_$type", $level_min);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  // Found one!
  if (!empty($data)) {

    $item = $data[mt_rand(0, 9)];

    // Out of actions and energy?  Maybe we need another one...
    if (($item->energy <= 0) && ($item->actions <= 0)) {

      zg_ai_out("Maybe we need another $type!");
      zg_ai_out("*FIXME* stub " . __LINE__);

    }

    zg_ai_out("found level $item->level $type player ($item->phone_id), " .
      'commencing boosting');

    zg_ai_do('boost experience', $item->phone_id);

  }

  // If not: do we have an extra minion that isn't higher than $level_max?
  // If so, tell that player to specialize.
  // Wasted effort if we just want a minion.
  if ($type != 'minion') {

    $sql = 'select phone_id, level from users
      where meta like "ai_minion"
      and level <= %d
      order by experience desc
      limit 1;';
    $result = db_query($sql, $level_max);
    $item = db_fetch_object($result);

    // Found one!
    if ($item->phone_id) {
      zg_ai_out("found level $item->level minion ($item->phone_id), " .
        "telling to specialize as $type");

      $sql = 'update users set meta = "%s"
        where phone_id = "%s";';
      $result = db_query($sql, "ai_$type", $item->phone_id);

      return FALSE;

    }

  }

  return FALSE;
}

/**
 * Create AI players until there are enough.
 *
 * @param int $num_players
 *   The number of players to have.
 *
 * @return bool
 *   Whether there were already enough players or not.
 */
function zg_ai_have_num_players($num_players) {

  // Goal: $num players across the game.
  $sql = 'select count(id) as count from users
    where meta like "ai_%";';
  $result = db_query($sql);
  $item = db_fetch_object($result);
  firep($item, 'ai item');
  $count = (int) $item->count;

  zg_ai_out("want $num_players players, have $count");

  if ($count >= $num_players) {
    // zg_ai_out('woohoo!  we have enough players!  returning TRUE');.
    return TRUE;
  }

  zg_ai_do('create new player');
  zg_ai_do('create new player');
  zg_ai_do('create new player');
  return FALSE;
}

/**
 * Create a new AI player.
 */
function game_ai_create_new_player() {
  global $game, $base_url, $ai_output;

  while (TRUE) {
    $num = mt_rand(0, 99999);
    $id_to_check = 'ai-' . $num;
    zg_ai_out("checking for existing player $id_to_check");

    $sql = 'select id from users
      where phone_id = "%s";';
    $result = db_query($sql, $id_to_check);
    $item = db_fetch_object($result);

    if (empty($item)) {
      break;
    }
  }

  $uri = $base_url . "/$game/home/$id_to_check";
  zg_ai_out("phone_id $id_to_check not in use; URI is $uri");
  $response = zg_ai_web_request($id_to_check, 'home');
  zg_ai_out($response);

  zg_ai_out('updating record to make this a ToxiCorp Employee');
  $sql = 'update users set username = "%s", fkey_neighborhoods_id = 75,
    fkey_values_id = 9, `values` = "Goo", meta = "ai_minion"
    where phone_id = "%s";';
  db_query($sql, "TC Emp $num", $id_to_check);
  $ai_bot = zg_fetch_user_by_id($id_to_check);
  // mail('joseph@ziquid.com', 'ai trying to create a new player AGAIN', $ai_output);.
  zg_slack($ai_bot, 'bots', 'new bot creation', $ai_output);
}

/**
 *
 */
function zg_ai_have_num_type_level_min_max_players($num_players, $type,
  $level_min, $level_max) {

  // Goal: $num players across the game.
  $sql = 'select count(id) as count from users
    where meta like "%s"
    and level >= %d
    and level <= %d;';
  $result = db_query($sql, "ai_$type", $level_min, $level_max);
  $item = db_fetch_object($result);

  zg_ai_out("want $num_players {$type}s, have $item->count");

  if ($item->count >= $num_players) {
    // zg_ai_out('woohoo!  we have enough players!  returning TRUE');.
    return TRUE;
  }

  // If not: do we have a player of $type that is a lower level?
  // If so, we can just boost.
  $sql = 'select phone_id, level from users
    where meta like "%s"
    and level < %d
    order by (energy + (actions * 40)) desc
    limit 1;';
  $result = db_query($sql, "ai_$type", $level_min);
  $item = db_fetch_object($result);

  // Found one!
  if ($item->phone_id) {
    zg_ai_out("found level $item->level $type player ($item->phone_id), " .
      'commencing boosting');

    zg_ai_do('boost experience', $item->phone_id);
    return FALSE;

  }

  // If not: do we have an extra minion that isn't higher than $level_max?
  // If so, tell that player to specialize.
  // Wasted effort if we just want a minion.
  if ($type != 'minion') {

    $sql = 'select phone_id, level from users
      where meta like "ai_minion"
      and level <= %d
      order by experience desc
      limit 1;';
    $result = db_query($sql, $level_max);
    $item = db_fetch_object($result);

    // Found one!
    if ($item->phone_id) {
      zg_ai_out("found level $item->level minion ($item->phone_id), " .
        "telling to specialize as $type");

      $sql = 'update users set meta = "%s"
        where phone_id = "%s";';
      $result = db_query($sql, "ai_$type", $item->phone_id);

      return FALSE;

    }

  }

  // No $type player found of appropriate level, none lower that we can boost,
  // no minions that can be told to specialize.  Creating new minion.
  zg_ai_out('efforts to find existing player to progress toward ' .
    "level $level_min to $level_max $type player have failed.  " .
    'Creating new minion.');

  zg_ai_do('create new player');
  return FALSE;

}

/**
 *
 */
function _ai_allocate_skill_for_elocution($ai_id) {

  // NB: don't *EVER* use zg_ai_fetch_user() here.
  // This could cause an infinite loop.
  $sql = 'select * from users where phone_id = "%s";';
  $result = db_query($sql, $ai_id);
  $ai_user = db_fetch_object($result);

  zg_ai_out("bumping Elocution up (now: $ai_user->elocution)");
  $ai_response = zg_ai_web_request($ai_id, 'increase_skills', 'elocution');

  if ($ai_response == 'increase-skill-succeeded') {
    zg_ai_out('Woohoo!  Elocution increase succeeded!');
    return TRUE;
  }

  zg_ai_out('Uhoh!  Elocution increase failed');
  return FALSE;

}

/**
 *
 */
function _ai_allocate_skill_for_action($ai_id) {

  // NB: don't *EVER* use zg_ai_fetch_user() here.
  // This could cause an infinite loop.
  $sql = 'select * from users where phone_id = "%s";';
  $result = db_query($sql, $ai_id);
  $ai_user = db_fetch_object($result);

  zg_ai_out("bumping Actions up (now: $ai_user->actions_max)");
  $ai_response = zg_ai_web_request($ai_id, 'increase_skills', 'actions');

  if ($ai_response == 'increase-skill-succeeded') {
    zg_ai_out('Woohoo!  Actions increase succeeded!');
    return TRUE;
  }

  zg_ai_out('Uhoh!  Actions increase failed');
  return FALSE;

}

/**
 *
 */
function _ai_allocate_skill_for_energy($ai_id) {

  // NB: don't *EVER* use zg_ai_fetch_user() here.
  // This could cause an infinite loop.
  $sql = 'select * from users where phone_id = "%s";';
  $result = db_query($sql, $ai_id);
  $ai_user = db_fetch_object($result);

  zg_ai_out("bumping Energy up (now: $ai_user->energy_max)");
  $ai_response = zg_ai_web_request($ai_id, 'increase_skills', 'energy_max');

  if ($ai_response == 'increase-skill-succeeded') {
    zg_ai_out('Woohoo!  Energy increase succeeded!');
    return TRUE;
  }

  zg_ai_out('Uhoh!  Energy increase failed');
  return FALSE;

}

/**
 *
 */
function _ai_allocate_skill_for_initiative($ai_id) {

  // NB: don't *EVER* use zg_ai_fetch_user() here.
  // This could cause an infinite loop.
  $sql = 'select * from users where phone_id = "%s";';
  $result = db_query($sql, $ai_id);
  $ai_user = db_fetch_object($result);

  zg_ai_out("bumping Initiative up (now: $ai_user->initiative)");
  $ai_response = zg_ai_web_request($ai_id, 'increase_skills', 'initiative');

  if ($ai_response == 'increase-skill-succeeded') {
    zg_ai_out('Woohoo!  Initiative increase succeeded!');
    return TRUE;
  }

  zg_ai_out('Uhoh!  Initiative increase failed');
  return FALSE;

}

/**
 *
 */
function _ai_allocate_skill_for_endurance($ai_id) {

  // NB: don't *EVER* use zg_ai_fetch_user() here.
  // This could cause an infinite loop.
  $sql = 'select * from users where phone_id = "%s";';
  $result = db_query($sql, $ai_id);
  $ai_user = db_fetch_object($result);

  zg_ai_out("bumping Endurance up (now: $ai_user->endurance)");
  $ai_response = zg_ai_web_request($ai_id, 'increase_skills', 'endurance');

  if ($ai_response == 'increase-skill-succeeded') {
    zg_ai_out('Woohoo!  Endurance increase succeeded!');
    return TRUE;
  }

  zg_ai_out('Uhoh!  Endurance increase failed');
  return FALSE;

}

/**
 * Allocate skill points.
 *
 * @param string $ai_id
 *   The bot player ID.
 */
function zg_ai_allocate_skill_points($ai_id) {

  // Allocate skill points.
  // NB: don't *EVER* use zg_ai_fetch_user() here.
  // This could cause an infinite loop.
  $sql = 'select * from users where phone_id = "%s";';
  $result = db_query($sql, $ai_id);
  $item = db_fetch_object($result);

  zg_ai_out("$ai_id is an $item->meta; allocating skill points based on that");

  if ($item->elocution < 8) {
    zg_ai_out("elocution is less than 8, allocating for elocution!");
    return _ai_allocate_skill_for_elocution($ai_id);
  }

  if ($item->energy_max < 300) {
    zg_ai_out("energy is less than 300, allocating for energy!");
    return _ai_allocate_skill_for_energy($ai_id);
  }

  if (($item->actions_max < 8) && ($item->skill_points >= 2)) {
    zg_ai_out("actions is less than 8, allocating for action!");
    return _ai_allocate_skill_for_action($ai_id);
  }

  if ($item->elocution < 16) {
    zg_ai_out("elocution is less than 16, allocating for elocution!");
    return _ai_allocate_skill_for_elocution($ai_id);
  }

  if (($item->actions_max < 12) && ($item->skill_points >= 2)) {
    zg_ai_out("actions is less than 12, allocating for action!");
    return _ai_allocate_skill_for_action($ai_id);
  }

  // Up to level 25: even split between actions, energy, specialty, and eloc'n.
  if ($item->level <= 25) {

    switch (mt_rand(0, 3)) {

      case 0:

        // Actions.
        zg_ai_out("random is 0, allocating for action!");
        return _ai_allocate_skill_for_action($ai_id);

      case 1:

        // Energy.
        zg_ai_out("random is 1, allocating for energy!");
        return _ai_allocate_skill_for_energy($ai_id);

      case 2:

        // Specialty.
        switch ($item->meta) {

          case 'ai_breacher':
            zg_ai_out("random is 2, specialty is breacher, allocating for initiative!");
            return _ai_allocate_skill_for_initiative($ai_id);

          case 'ai_holder':
            zg_ai_out("random is 2, specialty is holder, allocating for endurance!");
            return _ai_allocate_skill_for_endurance($ai_id);

          case 'ai_scout':
            zg_ai_out("random is 2, specialty is scout, allocating for action!");
            return _ai_allocate_skill_for_action($ai_id);

          case 'ai_minion':
          default:
            zg_ai_out("random is 2, no specialty, allocating for action!");
            return _ai_allocate_skill_for_action($ai_id);
        }

      case 3:

        // Elocution.
        zg_ai_out("random is 3, allocating for elocution!");
        return _ai_allocate_skill_for_elocution($ai_id);
    }
  }

  // Above level 25: pure specialty.
  switch ($item->meta) {

    case 'ai_breacher':
      zg_ai_out("specialty is breacher, allocating for initiative!");
      return _ai_allocate_skill_for_initiative($ai_id);

    case 'ai_holder':
      zg_ai_out("specialty is holder, allocating for endurance!");
      return _ai_allocate_skill_for_endurance($ai_id);

    case 'ai_minion':
      zg_ai_out("no specialty, allocating for action!");
      return _ai_allocate_skill_for_action($ai_id);

    case 'ai_scout':
      zg_ai_out("specialty is scout, allocating for action!");
      return _ai_allocate_skill_for_action($ai_id);
  }
}

/**
 * Boost bot's experience.
 */
function _ai_boost_experience($ai_id, $boost_flags = 0) {

  global $game;

  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $ai_user = zg_ai_fetch_user($ai_id);
  zg_ai_out("Trying to boost experience of $ai_id");

  $multiplier = 0;
  ($ai_user->level >= 10) && $multiplier = 100;
  ($ai_user->level >= 20) && $multiplier = 200;
  ($ai_user->level >= 25) && $multiplier = 400;
  ($ai_user->level >= 30) && $multiplier = 700;
  ($ai_user->level >= 35) && $multiplier = 1000;

  $net_income = $ai_user->income - $ai_user->expenses;
  $income_wanted = $ai_user->level * $multiplier;

  if ($net_income < $income_wanted) {

    zg_ai_out("Sorry, but I need more income first.  Have $net_income iph, " .
      "want $income_wanted.");

    zg_ai_do('buy land', $ai_id);
    return FALSE;

  }

  $boost_conserve_actions = ($boost_flags & CONSERVE_ACTIONS);
  $boost_conserve_money = ($boost_flags & CONSERVE_MONEY);

  if ($boost_conserve_actions) {
    zg_ai_out('... but will not use any actions');
  }

  if ($boost_conserve_money) {
    zg_ai_out('... but will not use any money');
  }

  $sql = 'select * from elected_officials
    where fkey_users_id = %d;';
  $result = db_query($sql, $ai_id);
  $seat = db_fetch_object($result);

  if (!$boost_conserve_actions) {

    zg_ai_out("Seeing if I can exploit my seat... " .
      "(actions: $ai_user->actions/$ai_user->actions_max)");

    if (!empty($seat)) {

      zg_ai_out('Woohoo!  I\'m an elected official!');

      // Need to have 5 actions to exploit.
      if ($ai_user->actions >= 5) {

        zg_ai_out('Woohoo!  I\'m an elected official with 5 action -- ' .
          'I\'m going to exploit my seat!');

        zg_ai_web_request($ai_id, 'actions_do', 58);
        zg_ai_do('open presents', $ai_id);
      }

    }
    else {
      zg_ai_out('I\'m not an elected official 8-(');
    }

  }

  if (empty($seat)) {

    // Only do missions if i'm not in a seat.
    zg_ai_out("Trying boosting by doing a quest (energy: $ai_user->energy)");

    // Find the highest level quest we can do.
    $sql = 'select id, name from quests
      where active = 1
      and (fkey_neighborhoods_id = 0 or fkey_neighborhoods_id = %d)
      and required_level <= %d
      and required_energy <= %d
      order by required_energy desc
      limit 1;';
    $result = db_query($sql, $ai_user->fkey_neighborhoods_id,
      $ai_user->level, $ai_user->energy);
    $item = db_fetch_object($result);

    if (!empty($item)) {

      // zg_ai_out("Highest quest I can do is #$item->id, $item->name");.
      if (zg_ai_do('do quest', $ai_id, $item->id)) {
        return TRUE;
      }

    }
    else {
      zg_ai_out('No quests found!');
    }

    if (($boost_conserve_actions) && ($event_type != EVENT_DEBATE)) {
      zg_ai_out('Not going to debate (conserving actions)');
    }
    elseif (($ai_user->meta == 'ai_scout') &&
      ($event_type != EVENT_DEBATE)) {
      zg_ai_out('Not going to debate (I am a scout)');
    }
    else {

      // If I won debates, call it good.
      zg_ai_out('Trying boosting by debating');
      if (zg_ai_do('debate players', $ai_id)) {
        return TRUE;
      }
    }
  }
  else {

    // In a seat.
    zg_ai_out('Not going to mission or debate (I am in a seat)');
  }

  if (!$boost_conserve_money) {

    zg_ai_out('Maybe I can buy something?  Checking for necessary equipment ' .
      "(money: $ai_user->money)");

    // equip_id => level.
    $stuff_to_get = [
      5 => 16,
      8 => 12,
      13 => 23,
    ];

    $sql = 'select quantity from equipment_ownership
      where fkey_equipment_id = %d and fkey_users_id = %d;';

    $attempt_to_get_one = FALSE;

    foreach ($stuff_to_get as $equip_id => $level) {

      if ($ai_user->level >= $level) {

        zg_ai_out("At level $level; maybe I can buy equip #$equip_id?");

        $result = db_query($sql, $equip_id, $ai_user->id);
        $item = db_fetch_object($result);

        if ($item->quantity >= 1) {
          zg_ai_out('Already have one!');
          continue;
        }

        zg_ai_out('I don\'t have one... trying to get it!');
        $attempt_to_get_one = TRUE;
        zg_ai_web_request($ai_id, 'equipment_buy', "$equip_id/1");

      }

    }

    if ($attempt_to_get_one) {
      return FALSE;
    }

    // Don't buy land if we need(ed) these.
    zg_ai_out('Checking for necessary staff/agents...');

    // staff_id => level.
    $staff_to_get = [
      9 => 25,
    ];

    $sql = 'select quantity from staff_ownership
      where fkey_staff_id = %d and fkey_users_id = %d;';
    $attempt_to_get_one = FALSE;

    foreach ($staff_to_get as $staff_id => $level) {

      if ($ai_user->level >= $level) {

        zg_ai_out("At level $level; maybe I can buy staff #$staff_id?");

        $result = db_query($sql, $staff_id, $ai_user->id);
        $item = db_fetch_object($result);

        if ($item->quantity >= 1) {
          zg_ai_out('Already have one!');
          continue;
        }

        zg_ai_out('I don\'t have one... trying to get it!');
        $attempt_to_get_one = TRUE;
        zg_ai_web_request($ai_id, 'staff_hire', "$staff_id/1");

      }

    }

    if ($attempt_to_get_one) {
      return FALSE;
    }

    // Don't buy land if we need(ed) these.
    zg_ai_out("Ok, trying to get a business...");
    zg_ai_do('buy land', $ai_id);
  }
  else {
    zg_ai_out('Not buying any aides (conserving money)');
  }

  return FALSE;
}

/**
 *
 */
function _ai_hood_seat_challenge($ai_id, $seat_min, $seat_max) {

  // Challenge for a hood seat in a specified range.
  $ai_user = zg_ai_fetch_user($ai_id);

  zg_ai_out('Trying to get more seats...');
  zg_ai_out("looking for empty seats in hood #$ai_user->fkey_neighborhoods_id "
    . "(actions: $ai_user->actions/$ai_user->actions_max)");

  if ($ai_user->actions == 0) {

    zg_ai_out("No actions!!!");
    zg_ai_do('boost experience', $ai_id);
    return FALSE;

  }

  $sql = 'SELECT id, name
    FROM elected_positions
    WHERE id NOT IN (

      SELECT elected_officials.fkey_elected_positions_id
        FROM users
        RIGHT JOIN elected_officials
        ON users.id = elected_officials.fkey_users_id
        WHERE users.fkey_neighborhoods_id = %d
      )

    AND id >= %d
    AND id <= %d
    AND energy_bonus <= %d
    order by energy_bonus DESC
    limit 1;';
  $result = db_query($sql, $ai_user->fkey_neighborhoods_id,
    $seat_min, $seat_max, $ai_user->actions);
  $item = db_fetch_object($result);

  if (!empty($item)) {

    zg_ai_out("Found empty $item->name seat (#$item->id)!");

    $ai_response = zg_ai_web_request($ai_id, 'elections_challenge', $item->id);

    if ($ai_response == 'election-won') {
      zg_ai_out('Woohoo!  Player challenged and won!');
      return TRUE;
    }

    if ($ai_response == 'election-lost') {
      zg_ai_out('Egads!  Player challenged an empty seat and lost?!?!?');
    }

    if ($ai_response == 'election-failed no-action') {
      zg_ai_out('Egads!  Out of Action!  But I checked before I challenged!');
    }

    if ($ai_response == 'election-failed no-challenge-yourself') {
      zg_ai_out('*FIXME*  How can I challenge myself?');
    }

    if ($ai_response == 'election-failed none-here') {
      zg_ai_out("No elections here (hood $ai_user->fkey_neighborhoods_id)" .
        '...  I must move!');
      zg_ai_do('move somewhere', $ai_id);

      return FALSE;
    }

  }
  else {
    zg_ai_out('Found no empty seats... 8-(((');
  }

  zg_ai_out('Looking for occupied seats to challenge... *FIXME* -- stub @ '
    . __LINE__);

  // zg_ai_out('Checking for empty seats in other hoods...');.
  $sql = 'select DISTINCT fkey_neighborhoods_id as hood_id,
    neighborhoods.has_elections from users
    left join neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
    where meta like "ai_%"
    and has_elections = 1;';
  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $hoods[] = $item;
  }

  $sql = 'SELECT id, name
    FROM elected_positions
    WHERE id NOT IN (

      SELECT elected_officials.fkey_elected_positions_id
        FROM users
        RIGHT JOIN elected_officials
        ON users.id = elected_officials.fkey_users_id
        WHERE users.fkey_neighborhoods_id = %d
      )

    AND id >= %d
    AND id <= %d
    order by id ASC
    limit 1;';

  foreach ($hoods as $hood) {

    zg_ai_out("do we have any free seats in hood #$hood->hood_id?");

    $result = db_query($sql, $hood->hood_id, $seat_min, $seat_max);
    $item = db_fetch_object($result);

    if (!empty($item)) {

      zg_ai_out("Hood #$hood->hood_id has free seats, getting ready to move...");
      zg_ai_do('move', $ai_id, $hood->hood_id);

      return FALSE;
    }

  }

  zg_ai_out('Sending a scout somewhere to find free seats...');
  $ai_user_scout = zg_ai_find_free_player('scout', 12, 130, 6);

  if (!empty($ai_user_scout)) {

    zg_ai_out("Found a free scout, $ai_user_scout->phone_id " .
      "(actions $ai_user_scout->actions/$ai_user_scout->actions_max)");

    zg_ai_do('move somewhere', $ai_user_scout->phone_id);

    return FALSE;
  }

  zg_ai_out('What?  We couldn\'t find a single free scout?  ' .
    'Guess I will venture out myself...');

  zg_ai_do('move somewhere', $ai_user->phone_id);
  return FALSE;

}

/**
 *
 */
function _ai_move_somewhere($ai_id) {

  // Take me... Far away... Anywhere...
  // As long as it's FUN FUN FUN FUN FUN FUN FUN I wanna go!
  $ai_user = zg_ai_fetch_user($ai_id);

  // Not enough actions to move!
  if ($ai_user->actions < 6) {

    zg_ai_out('The controller wants me to move but I don\'t have enough ' .
      "Actions ($ai_user->actions/$ai_user->actions_max)!  What shall I do?");

    if (($ai_user->actions_max < 6) && ($ai_user->skill_points > 0)) {
      zg_ai_out('I\'m going to allocate some of my skill points');
      zg_ai_do('allocate skill points', $ai_id);
    }

    zg_ai_out('Perhaps if I get more experience, ' .
      'I will have enough to move soon');
    zg_ai_do('boost experience', $ai_id);

    return FALSE;
  }

  // Scout or already home? Send out and about.
  if (($ai_user->meta == 'ai_scout') ||
    ($ai_user->fkey_neighborhoods_id == 15)) {

    $sql = 'select * from neighborhoods
      where has_elections = 1
      and id <> 15
      and xcoor > 0
      and ycoor > 0
      order by rand()
      limit 1;';
    $result = db_query($sql);
    $item = db_fetch_object($result);

    zg_ai_out("Sending scout $ai_id to hood #$item->id, $item->name");
    return zg_ai_do('move', $ai_id, $item->id);

  }

  if ($ai_user->fkey_neighborhoods_id != 15) {

    // Not home -- move there!
    zg_ai_out('Why not go home?');
    return zg_ai_do('move', $ai_id, 15);

  }

  return FALSE;
}

/**
 *
 */
function _ai_move($ai_id, $hood) {

  // Move to hood $hood.
  $ai_user = zg_ai_fetch_user($ai_id);

  // Are we already here?
  if ($ai_user->fkey_neighborhoods_id == $hood) {
    return TRUE;
  }

  // Not enough actions to move!
  if ($ai_user->actions < 6) {

    zg_ai_out("What?  I don't have enough actions ($ai_user->actions/$ai_user->actions_max) " .
      "to move to hood $hood...");

    if ($ai_user->actions_max < 6) {
      zg_ai_do('boost experience', $ai_id);
    }
    else {
      zg_ai_do('boost experience', $ai_id, CONSERVE_ACTIONS);
    }

    return FALSE;

  }

  $boost_flags = CONSERVE_ACTIONS;

  $sql = 'select xcoor, ycoor, name
    from neighborhoods
    where id = %d;';
  $result = db_query($sql, $ai_user->fkey_neighborhoods_id);
  $here = db_fetch_object($result);

  $sql = 'select xcoor, ycoor, name
    from neighborhoods
    where id = %d;';
  $result = db_query($sql, $hood);
  $there = db_fetch_object($result);

  $distance_now = _ai_distance($here->xcoor, $here->ycoor,
    $there->xcoor, $there->ycoor);

  zg_ai_out("I am in hood #$ai_user->fkey_neighborhoods_id, $here->name, " .
    "going to hood #$hood, $there->name.");

  $sql = 'select * from neighborhoods
    where xcoor > 0 and ycoor > 0;';
  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  $winning_hood = 0;
  $winning_distance = 9999;

  foreach ($data as $item) {

    $distance_to_dest = _ai_distance($item->xcoor, $item->ycoor,
      $there->xcoor, $there->ycoor);
    $distance_from_me = _ai_distance($item->xcoor, $item->ycoor,
      $here->xcoor, $here->ycoor);

    if ($distance_to_dest < $distance_now) {

      // zg_ai_out("Hood #$item->id, $item->name, is closer to $there->name " .
      //          "than I am now in $here->name " .
      //          "($distance_to_dest vs. $distance_now).  Its distance from me is " .
      //          "$distance_from_me.");.
      if ($distance_from_me < $winning_distance) {
        // zg_ai_out("New candidate for closest hood!  $distance_from_me < " .
        //            $winning_distance);.
        $winning_distance = $distance_from_me;
        $winning_hood = $item->id;
        $winning_name = $item->name;
      }

    }

  }

  if ($winning_hood > 0) {

    zg_ai_out(
      "hood #$winning_hood, $winning_name, is the next step to $there->name...");

    $ai_response = zg_ai_web_request($ai_id, 'move_do', $winning_hood);

    // Succeeded?  Try again!
    if ($ai_response == 'move-succeeded') {
      $ai_user = zg_ai_fetch_user($ai_id);

      // Are already here?
      if ($ai_user->fkey_neighborhoods_id == $hood) {
        return TRUE;
      }

      _ai_move($ai_id, $hood);
      return TRUE;
    }

    if ($ai_response == 'move-failed no-action') {

      zg_ai_out('Not enough action!  Let\'s see if I can get another aide...');

      if (!zg_ai_do('increase speed stats', $ai_id)) {
        $boost_flags |= CONSERVE_MONEY;
      }

      // Don't spend money if we are saving up for another speed boost.
    }

  }
  else {

    zg_ai_out('What?  *No* hoods can get me there?');

  }

  zg_ai_out('Move failed!  Settling for boosting of my experience...');
  zg_ai_do('boost experience', $ai_id, $boost_flags);
  return FALSE;

}

/**
 *
 */
function _ai_distance($xcoor1, $ycoor1, $xcoor2, $ycoor2) {

  // Find the distance between the two points.
  $deltax = $xcoor1 - $xcoor2;
  $deltay = $ycoor1 - $ycoor2;

  return ceil(sqrt(pow($deltax, 2) + pow($deltay, 2)));
}

/**
 * Get more elocution aides for bot.
 *
 * @param string $ai_id
 *   The player ID of the AI bot.
 *
 * @return bool
 *   Whether any elocution aides could be successfully obtained.
 */
function zg_ai_increase_elocution_stats($ai_id) {
  $ai_user = zg_ai_fetch_user($ai_id);

  $ai_response = zg_ai_web_request($ai_id, 'equipment');
  $response = array_slice(explode(' ', $ai_response), 1);
  $can_buy = 0;

  foreach ($response as $res) {

    // No elo aides?
    if (strpos($res, '/+elo:') === FALSE) {
      continue;
    }

    // Only look at elo aides.
    // Remove /+elo: fluff.
    $res = substr($res, 0, strpos($res, '/'));
    if ($ai_user->money >= (substr($res, strpos($res, '=') + 1) + 0)) {
      $can_buy = substr($res, 0, strpos($res, '='));
    }
  }

  if ($can_buy > 0) {
    // zg_ai_out("I have $ai_user->money money, " .
    //        "so I can buy aide #$can_buy...");.
    $ai_response = zg_ai_web_request($ai_id, 'equipment_buy', "$can_buy/1");
    return TRUE;
  }
  // zg_ai_out("I have $ai_user->money money, " .
  //        "so I can't buy any elocution aides!");.
  return FALSE;
}

/**
 *
 */
function _ai_increase_speed_stats($ai_id) {
  // Get more speed aides for player.
  $ai_user = zg_ai_fetch_user($ai_id);

  $ai_response = zg_ai_web_request($ai_id, 'equipment');
  $response = array_slice(explode(' ', $ai_response), 1);
  $can_buy = 0;

  foreach ($response as $res) {

    if (strpos($res, '/+spd:') === FALSE) {
      continue;
    }

    // Only look at elo aides.
    // Remove /+elo: fluff.
    $res = substr($res, 0, strpos($res, '/'));

    if ($ai_user->money >= (substr($res, strpos($res, '=') + 1) + 0)) {
      $can_buy = substr($res, 0, strpos($res, '='));
    }

  }

  if ($can_buy > 0) {

    zg_ai_out("I have $ai_user->money money, " .
      "so I can buy aide #$can_buy...");
    $ai_response = zg_ai_web_request($ai_id, 'equipment_buy', "$can_buy/1");

  }
  else {

    // zg_ai_out("I have $ai_user->money money, " .
    //        "so I can't buy any speed aides!");.
  }

  return FALSE;
}

/**
 * Buy land.
 *
 * @param string $ai_id
 *   AI phone ID.
 *
 * @return bool
 *   Whether the buy succeeded or not.
 */
function zg_ai_buy_land($ai_id) {

  $ai_user = zg_ai_fetch_user($ai_id);

  $ai_response = zg_ai_web_request($ai_id, 'land');
  $response = array_slice(explode(' ', $ai_response), 1);
  $can_buy = [];
  $did_buy = FALSE;
  zg_ai_out("I have $ai_user->money money...");

  foreach ($response as $res) {
    if ($ai_user->money >= (substr($res, strpos($res, '=') + 1) + 0)) {
      $can_buy[] = substr($res, 0, strpos($res, '='));
    }
  }

  foreach (array_reverse($can_buy) as $item) {
//    zg_ai_out("I have $ai_user->money money, so I can buy item #$item...");

    $ai_response = zg_ai_web_request($ai_id, 'land_buy', "$item/1");
    if ($ai_response == 'land-succeeded') {
      $did_buy = TRUE;
      zg_recalc_income($ai_user);
    }
  }

  return $did_buy;
}

/**
 * Debate players a specific number of times.
 *
 * @param string $ai_id
 *   The player ID of the AI bot.
 * @param int $num_debates
 *   Max number of debates to attempt.
 *
 * @return bool
 *   Whether any debates were successful or not.
 */
function zg_ai_debate_players($ai_id, $num_debates = 99) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $ai_user = zg_ai_fetch_user($ai_id);
  $num_attempts = min($num_debates, $ai_user->actions);

  // $debate_wait_time = 1200;
  // $zombie_debate_wait = 300;
  $sql = 'SELECT users.id, users.username, users.meta
    FROM users
    LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = users.id
    LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
    WHERE users.id <> %d
    AND (clans.id <> %d OR clans.id IS NULL OR users.meta = "zombie")
    AND username <> ""
    AND (debates_last_time < "%s" OR
     (users.meta = "zombie" AND debates_last_time < "%s"))
    AND users.meta NOT like "ai_%"
    AND users.level > %d
    AND users.level < %d
    ORDER BY abs(users.experience - %d) ASC
    LIMIT %d;';

  $result = db_query($sql, $ai_user->id, $ai_user->fkey_clans_id,
    date('Y-m-d H:i:s', REQUEST_TIME - $debate_time),
    date('Y-m-d H:i:s', REQUEST_TIME - $zombie_debate_wait),
    $ai_user->level - 15,
    $ai_user->level + 15, $ai_user->experience, $num_attempts);
  $data = [];
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  $successful_debate = FALSE;
  zg_ai_out("Found " . count($data) .
    " players to debate ($ai_user->actions actions)");

  foreach ($data as $item) {
    zg_ai_out("Can debate $item->username");
    $ai_response = zg_ai_web_request($ai_id, 'debates_challenge', $item->id);

    if ($ai_response == 'debate-won') {
      // zg_ai_out('Woohoo!  Player debated and won!');.
      $successful_debate = TRUE;
    }

    if ($ai_response == 'debate-lost') {
      // zg_ai_out('Egads!  Player debated and lost');.
      zg_ai_do('increase elocution stats', $ai_id);
    }

    if ($ai_response == 'debate-no-action') {
      // zg_ai_out('Egads!  Out of Action!');.
      break;
    }

    if ($ai_response == 'debate-must-wait') {
      // zg_ai_out("Must wait to debate $item->username");.
      continue;
    }
  }

  return ($successful_debate);
}

/**
 * Fetch AI user account.
 *
 * @param string $ai_id
 *   The phone ID of the AI player.
 *
 * @return bool|null|object|\stdClass
 *   The ai player object.
 */
function zg_ai_fetch_user($ai_id) {
  static $count = 4;

  $sql = 'select users.*, clan_members.fkey_clans_id
    from users
    LEFT OUTER JOIN clan_members
    ON clan_members.fkey_users_id = users.id
    where phone_id = "%s";';

  do {
    $result = db_query($sql, $ai_id);
    $ai_user = db_fetch_object($result);

    if ($ai_user->skill_points > 0) {
      zg_ai_out('bot has ' . $ai_user->skill_points . ' skill points (count ' .
      $count . '), allocating!');
      zg_ai_do('allocate skill points', $ai_id);
    }

  } while ($ai_user->skill_points && --$count);

  $count = 4;
  return $ai_user;
}

/**
 * Join a clan.
 *
 * @param string $ai_id
 *   AI player phone ID.
 * @param string $clan
 *   Clan acronym to join.
 *
 * @return string
 *   Output from the web request.
 */
function zg_ai_join_clan($ai_id, $clan = 'TC1') {
  // FIXME: game_find_action_by_id('join a clan');.
  $action_id = 2;
  return zg_ai_web_request($ai_id, 'actions_do', $action_id . '?acronym=' . $clan);
}

function zg_ai_plan_housekeeping() {
  return [
    'freshen ais',
    ['join clan', 'TC1'],
    ['richest num buy land', 2],
    ['poorest num buy land', 3],
  ];
}

/**
 * List all AI players.
 *
 * Freshen stats of those that haven't been accessed recently.
 */
function zg_ai_list_all_ais() {
  $sql = 'select phone_id from users
    where meta like "ai_%"
    and last_access < "%s"
    order by last_access ASC
    limit 3;';
  $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME - 300));
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  zg_ai_out(count($data) . ' ai players need refreshing.');

  if (count($data) > 0) {
    foreach ($data as $item) {
      zg_ai_web_request($item->phone_id, 'home');
    }
  }

  // All AIs not in a clan should join TC1.
  $sql = 'select users.phone_id
    from users
    LEFT OUTER JOIN clan_members
    ON clan_members.fkey_users_id = users.id
    where users.meta like "ai_%"
      and last_access < "%s"
      and actions > 0
      and money >= 100
    and clan_members.fkey_users_id IS NULL
    limit 6;';
  $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME - 900));
  $data = [];
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  foreach ($data as $item) {
    zg_ai_do('join clan', $item->phone_id);
  }

  // Tell the richest player(s) to buy more land.
  $data = [];
  $sql = 'select phone_id from users
    where meta like "ai_%"
      and last_access < "%s"
    order by money DESC
    limit 2;';
  $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME - 900));
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  foreach ($data as $item) {
    zg_ai_do('buy land', $item->phone_id);
  }

  // Tell the player(s) with the least income to buy more land.
  $data = [];
  $sql = 'select phone_id from users
    where meta like "ai_%"
      and last_access < "%s"
    order by income ASC
    limit 3;';
  $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME - 900));
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  foreach ($data as $item) {
    zg_ai_do('buy land', $item->phone_id);
  }

  // Now show stats.
  // Or not.
//  return;

  zg_ai_out(zg_ai_show_brief_stats());
}

/**
 * Show brief stats for all AI bots.
 *
 * @return string
 *   The stats, as a string.
 */
function zg_ai_show_brief_stats() {
  $data = [];
  $stats = '';

  $sql = 'select users.id from users
    where meta like "ai_%"
    order by experience desc;';
  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $data[] = (int) $item->id;
  }

  $bots = zg_fetch_users_by_id($data);
  foreach ($bots as $bot) {
    $clan_leader = $bot->is_clan_leader ? '*' : ' ';
    zg_fetch_user_extra_attributes($bot);
    $stats .= sprintf('%-8s (level %03d %-8s)  inf:%9s  engy:%4d/%4d  acts:%3d/%3d  ' .
      '$$$:%9s  last acc:%15s', $bot->phone_id, $bot->level,
      substr($bot->meta, 3),  number_format($bot->experience),
      $bot->energy, $bot->energy_max, $bot->actions, $bot->actions_max,
        number_format($bot->money),
      zg_format_date($bot->last_access)) . PHP_EOL;
    $stats .= sprintf('loc:%-20s clan:%3s%1s ini:%-12s end:%-12s elo:%-12s  ' .
        'luck:%4d',
      $bot->location, $bot->clan_acronym, $clan_leader,
      "{$bot->total_initiative}({$bot->initiative}x{$bot->extra_initiative})",
      "{$bot->total_endurance}({$bot->endurance}x{$bot->extra_endurance})",
      "{$bot->total_elocution}({$bot->elocution}x{$bot->extra_elocution})",
      $bot->luck) . PHP_EOL;
    $stats .= PHP_EOL;
  }

  return $stats;
}

/**
 * Generate an AI name.
 *
 * @return string
 *   The AI name.
 */
function zg_ai_name() {

  $first = [
    'Michael',
    'Christopher',
    'Matthew',
    'Joshua',
    'Daniel',
    'David',
    'James',
    'Robert',
    'John',
    'Joseph',
    'Andrew',
    'Ryan',
    'Brandon',
    'Jason',
    'Justin',
    'William',
    'Jonathan',
    'Brian',
    'Nicholas',
    'Anthony',
    'Eric',
    'Adam',
    'Kevin',
    'Steven',
    'Thomas',
    'Jessica',
    'Ashley',
    'Jennifer',
    'Amanda',
    'Sarah',
    'Stephanie',
    'Nicole',
    'Heather',
    'Elizabeth',
    'Megan',
    'Melissa',
    'Christina',
    'Rachel',
    'Laura',
    'Lauren',
    'Amber',
    'Brittany',
    'Danielle',
    'Kimberly',
    'Amy',
    'Crystal',
    'Michelle',
    'Tiffany',
    'Emily',
    'Rebecca',
  ];

  $last = [
    'Smith',
    'Johnson',
    'Williams',
    'Brown',
    'Jones',
    'Miller',
    'Davis',
    'Wilson',
    'Anderson',
    'Thomas',
    'Taylor',
    'Moore',
    'Jackson',
    'Thompson',
    'White',
    'Harris',
    'Clark',
    'Lewis',
    'Robinson',
    'Walker',
    'Young',
    'Allen',
    'King',
    'Wright',
    'Scott',
    'Hill',
    'Green',
    'Adams',
    'Baker',
    'Hall',
    'Campbell',
    'Mitchell',
    'Carter',
    'Turner',
    'Parker',
    'Edwards',
    'Collins',
    'Stewart',
    'Morris',
    'Morales',
    'Cook',
    'Rogers',
    'Cooper',
    'Peterson',
    'Bailey',
    'Reed',
    'Howard',
    'Ward',
    'Richardson',
    'Watson',
  ];

  return $first[mt_rand(0, count($first) - 1)] . ' '
    . $last[mt_rand(0, count($last) - 1)];
}

/**
 * Return the last value saved.
 *
 * @return mixed
 *   The last value.
 */
function zg_ai_get_last_value() {
  global $last_value;
  return $last_value;
}
