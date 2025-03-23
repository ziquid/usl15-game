<?php

global $game, $phone_id;

$fetch_user = '_' . arg(0) . '_fetch_user';
$fetch_header = '_' . arg(0) . '_header';

$game_user = $fetch_user();
include(drupal_get_path('module', $game) . '/game_defs.inc');
include(drupal_get_path('module', $game) . '/' . $game .
  '_actions.inc');

$arg2 = check_plain(arg(2));

if ($game_user->level < 6) {

  echo <<< EOF
<div class="title">
<img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<p>&nbsp;</p>
<div class="welcome">
  <div class="wise_old_man_small">
  </div>
  <p>&quot;You're not yet influential enough for this page.&nbsp;
  Come back at level 6.&quot;</p>
  <p class="second">&nbsp;</p>
  <p class="second">&nbsp;</p>
  <p class="second">&nbsp;</p>
</div>
<div class="subtitle"><a
  href="/$game/quests/$arg2"><img
    src="/sites/default/files/images/{$game}_continue.png"/></a></div>
EOF;

  db_set_active('default');
  return;

}

if (empty($game_user->username))
  drupal_goto($game . '/choose_name/' . $arg2);

if (($game_user->meta == 'frozen') && ($phone_id != 'abc123')) {
  $fetch_header($game_user);
  echo <<< EOF
<div class="title">Frozen!</div>
<div class="subtitle">You have been tagged and cannot perform any actions</div>
<div class="subtitle">Call on a teammate to unfreeze you!</div>
EOF;

db_set_active('default');
return;
}

// Get list of equipment owned.
$data = array();
$sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership
    ON equipment_ownership.fkey_equipment_id = equipment.id
    AND equipment_ownership.fkey_users_id = %d

  WHERE

    equipment_ownership.quantity > 0

  OR

    "%s" = "abc123"

  ORDER BY equipment.name ASC';
$result = db_query($sql, $game_user->id, $arg2);
while ($item = db_fetch_object($result)) {
  $item->quantity = 0 + $item->quantity;
  $data[$item->id] = $item;
}

// Check to see if prerequisites are met.
$action_succeeded = TRUE;

// Out of action?
if ($game_user->actions < 1) {
  competency_gain($game_user, 'actionless');
  $title = 'Crafting Failed';
  $outcome_reason = '<div class="land-failed">' . t('Out of Action!') .
    '</div>
    <div class="try-an-election-wrapper"><div
      class="try-an-election"><a href="/' . $game . '/elders_do_fill/' .
    $arg2 . '/action?destination=/' . $game . '/crafting/' . $arg2 .
    '">Refill your Action (1&nbsp;Luck)</a></div></div>';
  $action_succeeded = FALSE;
}

if ($action_succeeded) {
  $items = array(
    check_plain($_GET['item_1']),
    check_plain($_GET['item_2']),
    check_plain($_GET['item_3']),
  );
  sort($items);
//  firep($items);

// Forgot to select three items?
  foreach ($items as $item) {
    if ($item == 0) {
      competency_gain($game_user, 'hole in your pocket');
      $title = 'Crafting Failed';
      $outcome_reason = <<< EOF
<div class="subtitle">
  You must select three items!
</div>
<div class="try-an-election-wrapper">
  <a class="try-an-election" href="/$game/crafting/$arg2">
    Try Again
  </a>
</div>
EOF;
      $action_succeeded = FALSE;
      break;
    }
  }
}

if ($action_succeeded) {
// Quantity limit?
  $quantities = array();
  foreach ($items as $item) {
    $quantities[$item]++;
  }
//  firep($quantities, 'quantities');
  foreach ($quantities as $eid => $quantity) {
    firep($data[$eid], 'equipment for eid ' . $eid);
    if ($quantity > $data[$eid]->quantity) {
      competency_gain($game_user, 'hole in your pocket');
      if ($phone_id == 'abc123') {
        continue;
      }
      $title = 'Crafting Failed';
      $name = $data[$eid]->name;
      $outcome_reason = <<< EOF
<div class="subtitle">
  You need more $name!
</div>
<div class="try-an-election-wrapper">
  <a class="try-an-election" href="/$game/crafting/$arg2">
    Try Again
  </a>
</div>
EOF;
      $action_succeeded = FALSE;
      break;
    }
  }
}

if ($action_succeeded) {
// Check if there is a formula for these three items
  $sql = 'SELECT crafting.*, equipment.name, equipment_ownership.quantity FROM crafting
  LEFT JOIN equipment ON crafting.fkey_equipment_id_result = equipment.id
  LEFT JOIN equipment_ownership ON equipment.id = equipment_ownership.fkey_equipment_id
    AND
    equipment_ownership.fkey_users_id = %d
  WHERE
  fkey_equipment_id_1 = %d
  AND 
  fkey_equipment_id_2 = %d
  AND 
  fkey_equipment_id_3 = %d
  LIMIT 1';
  $result = db_query($sql, $game_user->id, $items[0], $items[1], $items[2]);
  $formula = db_fetch_object($result);
  firep($formula, 'formula');

  if (empty($formula)) {
    competency_gain($game_user, 'inventor');
    $title = 'Crafting Failed';
    $outcome_reason = <<< EOF
<div class="subtitle">
  These items don't seem to work together
</div>
<div class="try-an-election-wrapper">
  <a class="try-an-election" href="/$game/crafting/$arg2">
    Try Again
  </a>
</div>
EOF;
    $action_succeeded = FALSE;
  }
}

if ($action_succeeded) {
// Success!  Remove items used in crafting.
  $sql = 'UPDATE equipment_ownership SET quantity = greatest(quantity - %d, 0)
    WHERE fkey_equipment_id = %d AND fkey_users_id = %d;';
  foreach ($quantities as $eid => $quantity) {
    db_query($sql, $quantity, $eid, $game_user->id);
  }

// Add items used in crafting.
  if ($formula->quantity == '') { // no record exists - insert one
    $sql = 'INSERT INTO equipment_ownership (fkey_equipment_id, fkey_users_id,
    quantity)
    VALUES (%d, %d, 1);';
    db_query($sql, $formula->fkey_equipment_id_result, $game_user->id);
  }
  else { // existing record - update it
    $sql = 'UPDATE equipment_ownership SET quantity = quantity + 1 WHERE
    fkey_equipment_id = %d AND fkey_users_id = %d;';
    db_query($sql, $formula->fkey_equipment_id_result, $game_user->id);
  }

// Debit action, update income and expenses.
  $sql = 'UPDATE users SET income =
    (SELECT sum(land.payout * land_ownership.quantity)
    AS income FROM land
    LEFT JOIN land_ownership
    ON land_ownership.fkey_land_id = land.id AND
    land_ownership.fkey_users_id = %d),
    expenses =
    (SELECT sum(equipment.upkeep * equipment_ownership.quantity)
    AS expenses FROM equipment
    LEFT JOIN equipment_ownership
    ON equipment_ownership.fkey_equipment_id = equipment.id AND
    equipment_ownership.fkey_users_id = %d),
    actions = actions - 1
    WHERE id = %d;';
  db_query($sql, $game_user->id, $game_user->id, $game_user->id);

// Start the actions clock if needed.
  if ($game_user->actions == $game_user->actions_max) {
    $sql = 'UPDATE users SET actions_next_gain = "%s" WHERE id = %d;';
    db_query($sql, date('Y-m-d H:i:s', time() + 180),
      $game_user->id);
  }

// Competency and text.
  competency_gain($game_user, 'craftsman');
  $title = 'Success!';
  $outcome_reason = <<< EOF
<div class="subtitle">
  You have created a new $formula->name!
</div>
<div class="subtitle">
  <img 
    src="/sites/default/files/images/equipment/$game-$formula->fkey_equipment_id_result.png"
  border="0" width="96">
</div>
<div class="try-an-election-wrapper">
  <a class="try-an-election" href="/$game/crafting_do/$arg2?item_1=$items[0]&item_2=$items[1]&item_3=$items[2]">
    Craft This Again
   </a>
 </div>
<div class="try-an-election-wrapper">
  <a class="try-an-election" href="/$game/crafting/$arg2">
    Craft Something Else
  </a>
</div>
EOF;
}

// Tell the user.
$game_user = $fetch_user();
$fetch_header($game_user);

if (arg(1) == 'crafting' || arg(1) == 'crafting_do') {
  $crafting_active = 'active';
  $actions_type = 'Crafting';
} else {
  $normal_active = 'active';
  $actions_type = 'Normal';
}
$title = t($title);
$title_class = $action_succeeded ? '' : 'failed';

echo <<< EOF
<div class="news">
  <a href="/$game/actions/$arg2" class="button $normal_active">Normal</a>
  <a href="/$game/crafting/$arg2" class="button $crafting_active">Crafting</a>
</div>
<div class="title $title_class">
  $title
</div>
$outcome_reason
EOF;

if (substr($phone_id, 0, 3) == 'ai-')
  echo "<!--\n<ai \"$ai_output " .
    filter_xss($outcome_reason, array()) .
    " \"/>\n-->";

db_set_active('default');

