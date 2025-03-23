<?php

/**
 * @file
 * AI goals.
 */

/**
 * Figure out which bot needs to do its goals and previous results.
 *
 * @return array
 *   An array with the bot, the goals, and previous results.
 */
function zg_ai_get_bot_goals_results() {
  $bots = zg_ai_get_all_bots_stalest_first();

  foreach ($bots as $bot) {
//    zg_ai_out($bot, 'bot to check');
    $goals = zg_ai_fetch_bot_goals($bot);
    if (is_array($goals) && count($goals)) {
      $results = zg_ai_fetch_bot_results($bot);
      $bot = zg_fetch_player_by_id($bot->phone_id);
      return [$bot, $goals, $results];
    }
  }

  // No bots with goals?  Give stalest bot a goal.
  $bot = reset($bots);
  $bot = zg_fetch_player_by_id($bot->phone_id);
  return [$bot, zg_ai_get_all_default_goals($bot), []];
}

/**
 * Save a bot's goals and results.
 *
 * @param object $bot
 *   The bot object.
 * @param array[] $goals
 *   The goals to save.
 * @param array[] $results
 *   The results to save.
 */
function zg_ai_set_bot_goals_results($bot, array $goals, array $results) {
  zg_ai_save_bot_goals($bot, $goals);
  if (!zg_ai_save_bot_results($bot, $results)) {
    zg_ai_out('Egads!  FAILURE when trying to save bot results!!!');
  }
}

/**
 * Get bot goals, if any.
 *
 * @param object|string $bot
 *   The bot to check.
 * @param mixed $default
 *   The goals to return if the bot has no goals.
 *
 * @return array
 *   The goals, if any.
 */
function zg_ai_fetch_bot_goals($bot, $default = []) {
  return zg_get_value($bot, 'ai_goals', $default);
}

/**
 * Save bot goals.
 *
 * @param object $bot
 *   The bot object.
 * @param array $goals
 *   The goals to save.
 *
 * @return bool
 *   Whether the save succeeded or not.
 */
function zg_ai_save_bot_goals($bot, array $goals) {
  return zg_set_value($bot, 'ai_goals', $goals);
}

/**
 * Get bot results, if any.
 *
 * @param object|string $bot
 *   The bot to check.
 *
 * @return array
 *   The goals, if any.
 */
function zg_ai_fetch_bot_results($bot) {
  return zg_get_value($bot, 'ai_results', []);
}

/**
 * Save bot results.
 *
 * @param object $bot
 *   The bot object.
 * @param array $results
 *   The results to save.
 *
 * @return bool
 *   Whether the save succeeded or not.
 */
function zg_ai_save_bot_results($bot, array $results) {
  return zg_set_value($bot, 'ai_results', $results);
}

/**
 * Get all bots from stalest to freshest.
 *
 * @return object[]
 *   All bots.
 */
function zg_ai_get_all_bots_stalest_first() {
  $data = [];
  $sql = 'select id, phone_id from users
    where meta like "ai_%"
    order by last_access ASC;';
  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  return $data;
}

/**
 * Get all default goals.
 *
 * @param object $bot
 *   The bot object.
 *
 * @return array
 *   The goals.
 */
function zg_ai_get_all_default_goals($bot) {
  $goals = array_merge(zg_ai_get_implied_goals($bot), zg_ai_get_default_goals($bot));
  if (!zg_server_is_busy()) {
    $goals = array_merge($goals, zg_ai_get_suggested_goals($bot));
  }
  return $goals;
}

/**
 * Get implied goals.
 *
 * @param object $bot
 *   The bot object.
 *
 * @return array
 *   The goals.
 */
function zg_ai_get_implied_goals($bot) {
  return zg_ai_get_TYPE_goals($bot, 'implied', [['refresh']]);
}

/**
 * Get default goals.
 *
 * @param object $bot
 *   The bot object.
 *
 * @return array[]
 *   The goals.
 */
function zg_ai_get_default_goals($bot) {
  return zg_ai_get_TYPE_goals($bot, 'default', [['build']]);
}

/**
 * Get suggested goals.
 *
 * @param object $bot
 *   The bot object.
 *
 * @return array[]
 *   The goals.
 */
function zg_ai_get_suggested_goals($bot) {
  return zg_ai_get_TYPE_goals($bot, 'suggested', [['dunno']]);
}

/**
 * Get goals of a specified type, or defa.
 *
 * @param object $bot
 *   The bot object.
 * @param string $type
 *   The type of goals to get.
 * @param array $default
 *   The default goals if none are found.
 *
 * @return array[]
 *   The goals.
 */
function zg_ai_get_TYPE_goals($bot, $type, $default) {
  $meta = $bot->meta;
  if (substr($meta, 0, 3) == 'ai_') {
    $meta = substr($meta, 3);
  }

  $goals = zg_get_default('ai_' . $meta . '_' . $type . '_goals');

  if ($goals === FALSE) {
    zg_ai_out('Specialize this AI "' . $meta . '" by adding a game_default for key ' .
      '"ai_' . $meta . '_' . $type . '_goals"', 'Suggestion');
    $goals = zg_get_default('ai_' . $type . '_goals');
  }

  if ($goals === FALSE) {
    zg_ai_out('Set AI "' . $type . '" behavior by adding a game_default for key ' .
      '"ai_' . $type . '_goals".', 'Suggestion');
    $goals = $default;
    zg_ai_out($goals, 'Generic ' . $type . ' goals');
  }

  return $goals;
}

/**
 * Mark a result array as of type 'goals'.
 *
 * @param array[]
 *   The result array.
 *
 * @return array[]
 *   The array of goals.
 */
function zg_ai_goals_type() {
  $array = [
    '#type' => 'goals',
  ];
  $array = array_merge($array, func_get_args());
//  zg_ai_out($array,'zg_ai_goals_type array');
  return $array;
}

/**
 * Mark a result array as of type 'data'.
 *
 * @param array $data
 *   The data array.
 *
 * @return array[]
 *   The result array, marked as data.
 */
function zg_ai_data_type($data) {
  $array = [
    '#type' => 'data',
    '#data' => $data,
  ];
  return $array;
}

/**
 * Is the result array of type 'goals'?
 *
 * @param array[] $arr
 *   The result array.
 *
 * @return bool
 *   Whether the result array is of type 'goals' or not.
 */
function zg_ai_is_goals_type(array $arr) {
  return (is_array($arr) && array_key_exists('return value', $arr) &&
    zg_ai_is_type_type($arr['return value'], 'goals'));
}

/**
 * Is the result array of type 'data'?
 *
 * @param array[] $arr
 *   The result array.
 *
 * @return bool
 *   Whether the result array is of type 'data' or not.
 */
function zg_ai_is_data_type(array $arr) {
  return (is_array($arr) && array_key_exists('return value', $arr) &&
    zg_ai_is_type_type($arr['return value'], 'data'));
}

/**
 * Is the result array of type $type?
 *
 * @param array[] $arr
 *   The result array.
 * @param string $type
 *   The type to check.
 *
 * @return bool
 *   Whether the result array is of type $type or not.
 */
function zg_ai_is_type_type(array $arr, string $type) {
  return (is_array($arr) && array_key_exists('#type', $arr) && $arr['#type'] === $type);
}

/**
 * Expand a single goal into a set of new goals.
 *
 * @param array $new
 *   The new goals.
 * @param array $old
 *   The old goal, of which the first is the single goal to expand.
 *
 * @return array
 *   The new goals.
 */
function zg_ai_expand_goals(array $new, array $old) {
  $new = $new['return value'];
  unset($new['#type']);
  return array_merge(reset($new), array_slice($old, 1));
}

/**
 * The first goal was successful.  Remove it from the goal list.
 *
 * @param array $goals
 *   The goal list.
 *
 * @return array
 *   The goal list with the first item removed.
 */
function zg_ai_remove_successful_goal(array $goals) {
  return array_slice($goals, 1);
}

/**
 * The first goal failed.  Remove it from the goal list.
 *
 * @param array $goals
 *   The goal list.
 *
 * @return array
 *   The goal list with the first item removed.
 */
function zg_ai_remove_failed_goal(array $goals) {
  return array_slice($goals, 1);
}

/**
 * Get the data from a result array of type 'data'.
 *
 * @param array[] $result
 *   The result array to get data from.
 *
 * @return array
 *   The data, if there is any, or an empty array.
 */
function zg_ai_get_data(array $result) {
//  zg_ai_out($result, 'result passed to ' . __FUNCTION__);
  if (zg_ai_is_data_type($result)) {
    $return_value = $result['return value'];
    return $return_value['#data'];
  }
  return [];
}
