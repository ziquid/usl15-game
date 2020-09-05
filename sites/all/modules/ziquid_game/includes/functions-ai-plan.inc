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
function zg_ai_need_bots_num_type($num, $type) {
  return [
    '#type' => 'goals',
    ['count_bots_type', $type],
    ['doneif', '>=', $num],
    ['create_or_repurpose_bot_type', $type],
  ];
}

function zg_ai_plan_create_or_repurpose_bot_type($type) {
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
function zg_ai_plan_convert_to_type($type) {
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
function zg_ai_count_bots_type($type) {
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
function zg_ai_doneif($operator, $val2) {
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

function zg_ai_protest() {
  return [
    '#type' => 'goals',
    ['move_to_hood', 'downtown'],
    ['debate'],
    ['build'],
  ];
}

/**
 * Find bots that need refreshing and refresh them.
 *
 * @return array
 *   A list of goals.
 */
function zg_ai_refresh() {
  return [
    '#type' => 'goals',
    ['find stale bots', 3],
    'refresh bots',
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
function zg_ai_find_stale_bots($num) {
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

function zg_ai_refresh_bots() {
  $bots = zg_ai_get_last_value();
//  zg_ai_out('will refresh ' . zg_print_r($bots));
  return [
    '#type' => 'goals',
    ['foreach', $bots, 'web request', 'home'],
  ];
}

/**
 * Expand a list and goals into individual goals.
 *
 * @param $data
 *   The list to expand.
 * @param $goal
 *   The goal to perform for each member of the list.
 *
 * @return string[]
 *   Goals to perform.
 */
function zg_ai_foreach($data, $goal) {
  $goals = [
    '#type' => 'goals',
  ];
  $args = func_get_args();

  foreach ($data as $item) {
    $goals[] = [$goal, $item] + $args;
  }
  return $goals;
}
