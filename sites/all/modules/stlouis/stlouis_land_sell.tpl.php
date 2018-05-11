<?php

  global $game, $phone_id;

  $fetch_user = '_' . arg(0) . '_fetch_user';
  $fetch_header = '_' . arg(0) . '_header';

  $game_user = $fetch_user();
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));

  if (empty($game_user->username)) {
    db_set_active('default');
    drupal_goto($game . '/choose_name/' . $arg2);
  }

  if ($quantity === 'use-quantity') {
    $quantity = check_plain($_GET['quantity']);
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

    WHERE land.id = %d;';
  $result = db_query($sql, $game_user->id, $land_id);
  $game_land = db_fetch_object($result);
  $orig_quantity = $quantity;
  $land_price = ceil($game_land->price * $quantity * 0.6);

/* allow for 80% sale price
  $land_price = ceil($game_land->price +
    ($game_land->price_increase * ($game_land->quantity - $quantity))
     * $quantity * 0.8);
*/

  $options = [];
  $options['land-sell-succeeded'] = 'sell-success';
  $ai_output = 'land-succeeded';

// check to see if land prerequisites are met

// hit a quantity limit?
  if ($quantity > $game_land->quantity) {

    $options['land-sell-succeeded'] = 'failed not-enough-land';
    $ai_output = 'land-failed not-enough-land';
    game_karma($game_user,
      "trying to sell $quantity of $game_land->name but only has $game_land->quantity",
      $quantity * -10);

  }

// can't sell?
  if ($game_land->can_sell != 1) {

    $options['land-sell-succeeded'] = 'failed cant-sell';
    $ai_output = 'land-failed cant-sell';
    game_karma($game_user, "trying to sell unsalable $game_land->name", -100);

  }

// job?
  if ($game_land->type == 'job') {

    $options['land-sell-succeeded'] = 'failed cant-sell-job';
    $ai_output = 'land-failed cant-sell-job';
    game_karma($game_user, "trying to sell job $game_land->name", -100);

  }

  // Success!
  if ($options['land-sell-succeeded'] == 'sell-success') {

      // Investment?  Add competency.
//    if ($game_land->type == 'investment') {
//      competency_lose($game_user, 'investor', $quantity);
//    }
    land_lose($game_user, $land_id, $quantity, $land_price);
  }
  else {
    $quantity = 0;
  }

  // Show the stuff.
  $fetch_header($game_user);
  show_aides_menu($game_user);

  $game_land->quantity = $game_land->quantity - (int) $quantity;
  game_show_land($game_user, $game_land, $options);

  echo <<< EOF
<div class="title">
  Available $land_plural
</div>
EOF;

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"$ai_output\"/>\n-->";

  $data = [];
  $sql = 'SELECT land.*, land_ownership.quantity,
    competencies.name as competency, comp1.name as competency_name_1
    FROM land

    LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
    AND land_ownership.fkey_users_id = %d

    LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
      competencies.id
    left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

    WHERE (((
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

    OR land_ownership.quantity > 0

    ORDER BY required_level ASC';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  while ($item = db_fetch_object($result)) $data[] = $item;

  foreach ($data as $item) {

    game_show_land($game_user, $item);

  }

  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"$ai_output\"/>\n-->";
  }

  // Show next one.
  $sql = 'SELECT land.*, land_ownership.quantity,
    competencies.name as competency, comp1.name as competency_name_1
    FROM land

    LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
    AND land_ownership.fkey_users_id = %d

    LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
      competencies.id
    left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

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
    game_show_land($game_user, $item, ['soon' => TRUE]);
  }

  db_set_active('default');
