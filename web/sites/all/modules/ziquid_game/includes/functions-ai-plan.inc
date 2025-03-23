<?php

/**
 * @file
 * AI plans.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * .
 */

/**
 * Plan this goal.
 *
 * @param int $num
 *   How many bots?
 * @param string $type
 *   Of what type?
 *
 * @return array[]
 *   A plan array.
 */
function old_zg_ai_need_bots_num_type($num, $type) {
  return [
    '#type' => 'goals',
    ['count_bots_type', $type],
    ['doneif', '>=', $num],
    ['create_or_repurpose_bot_type', $type],
  ];
}

function old_zg_ai_plan_create_or_repurpose_bot_type($type) {
  return [
    ['count_bots_type', 'minion'],
    ['if', '>', 0],
    ['if', '>', 0, ['convert_to_type', $type]],
    ['iftrue', 'convert_to_type', $type],
//    ['create_or_repurpose_bot_type', $type],
  ];
}

/**
 * @param $type
 *
 * @return string[]
 */
function old_zg_ai_plan_convert_to_type($type) {
  return [
    '#type' => 'goals',
    'dunno',
  ];
}

/**
 * Count the number of bots of the specified type.
 *
 * @param string $type
 *   The type for which to check.
 *
 * @return int
 *   The count.
 */
function old_zg_ai_count_bots_type($type) {
  $sql = 'select count(*) as count from users
    where meta like "ai_%s";';
  $result = db_query($sql, $type)->fetch_object();
  return $result->count;
}

/**
 * Consider the goal set "done" if test is true.
 *
 * @param string $operator
 *   Operator for testing.
 * @param mixed $val2
 *   Second value for testing.
 *
 * @return false|string[]|void
 */
function old_zg_ai_doneif($operator, $val2) {
  $val1 = zg_ai_get_last_value();
  zg_ai_out("$val1 $operator $val2");

  switch ($operator) {
    case '>=':
      $d = ($val1 >= $val2);
      break;

    case '>':
      $d = ($val1 > $val2);
      break;

    default:
      zg_ai_out('operator ' . $operator . 'is not supported!');
      return FALSE;
  }
  if ($d) {
    return [
      '#type' => 'done',
    ];
  }
  return NULL;
}

function old_zg_ai_protest() {
  return [
    '#type' => 'goals',
    ['move_to_hood', 'downtown'],
    ['debate'],
    ['build'],
  ];
}

/**
 * Find stale bots.
 *
 * @param int $num
 *   The maximum number of stale bots to find.
 *
 * @return string[]
 *   Stale bots, if any.
 */
function old_zg_ai_find_stale_bots($num) {
  $data = [];
  $sql = 'select phone_id from users
    where meta like "ai_%"
    and last_access < "%s"
    order by last_access ASC
    limit %d;';
  $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME - 300), $num);
  while ($item = db_fetch_object($result)) {
    $data[] = $item->phone_id;
  }
  return $data;
}

/**
 * Refresh a bot's stats.
 *
 * @param $bot
 *  The bot object.
 *
 * @return array
 *   The result array.
 */
function zg_ai_refresh($bot) {
  return zg_ai_goals_type([
    ['web request', 'home'],
  ]);
}

/**
 * Expand a list and a goal into multiple goals.
 *
 * @param object $bot
 *   The bot object.
 * @param string $goal
 *   The goal to perform for each member of the list.
 *
 * @return array
 *   The result array.
 */
function zg_ai_foreach($bot, string $goal) {
  $goals = [];
  $args = func_get_args();
  $data = zg_ai_get_last_value();
//  zg_ai_out($data, 'FOREACH data from last value');

  // No data in the list?  This expands to nothing.
  if (!count($data)) {
    return zg_ai_data_type([
      'No data in list; not performing foreach',
    ]);
  }

  $first_item = reset($data);
  if (!count($first_item)) {
    return zg_ai_data_type([
      'No data in list; not performing foreach',
    ]);
  }

  foreach ($data as $item) {
    $goals[] = [$goal, $item] + $args;
  }
  return zg_ai_goals_type($goals);
}
