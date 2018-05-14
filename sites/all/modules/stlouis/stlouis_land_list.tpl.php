<?php

global $game, $phone_id;

$fetch_user = '_' . arg(0) . '_fetch_user';
$fetch_header = '_' . arg(0) . '_header';

$game_user = $fetch_user();
include drupal_get_path('module', $game) . '/game_defs.inc';
$arg2 = check_plain(arg(2));
$ai_output = 'land-prices';

game_recalc_income($game_user);
$fetch_header($game_user);
show_aides_menu($game_user);

$sql_to_add = 'WHERE (((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND

  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

    AND required_level <= %d
    AND active = 1
  )

  OR land_ownership.quantity > 0 ';

if ($game_user->meta == 'admin') {
  $sql_to_add = '';
}

$data = [];
$sql = 'SELECT land.*, land_ownership.quantity,
  competencies.name as competency, comp1.name as competency_name_1
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
    competencies.id
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

  ' . $sql_to_add . '
  ORDER BY required_level ASC';
$result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $game_user->level);

while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

foreach ($data as $item) {
firep($item, 'Item: ' . $item->name);
  game_show_land($game_user, $item);

  $land_price = $item->price + ($item->quantity * $item->price_increase);
  $ai_output .= " $item->id=$land_price";
}

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"$ai_output\"/>\n-->";
}

// Show next one.
$sql = 'SELECT land.*, land_ownership.quantity
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  WHERE ((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND
  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

  AND required_level > %d
  AND active = 1
  ORDER BY required_level ASC LIMIT 1';
$result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
  $game_user->fkey_values_id, $game_user->level);
$item = db_fetch_object($result);

if (!empty($item)) {
  firep($item, 'Soon Item: ' . $item->name);
  game_show_land($game_user, $item, ['soon' => TRUE]);
}

db_set_active('default');
