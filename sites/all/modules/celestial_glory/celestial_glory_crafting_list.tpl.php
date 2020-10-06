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

  $fetch_header($game_user);

  if (empty($game_user->username))
    drupal_goto($game . '/choose_name/' . $arg2);

  if (($game_user->meta == 'frozen') && ($phone_id != 'abc123')) {

    echo <<< EOF
<div class="title">Frozen!</div>
<div class="subtitle">You have been tagged and cannot perform any actions</div>
<div class="subtitle">Call on a teammate to unfreeze you!</div>
EOF;

  db_set_active('default');
  return;

  }

  if (arg(1) == 'crafting') {
    $crafting_active = 'active';
    $actions_type = 'Crafting';
  } else {
    $normal_active = 'active';
    $actions_type = 'Normal';
  }

  echo <<< EOF
<div class="news">
  <a href="/$game/actions/$arg2" class="button $normal_active">Normal</a>
  <a href="/$game/crafting/$arg2" class="button $crafting_active">Crafting</a>
</div>
EOF;

  if ($game_user->level < 200) {

    echo <<< EOF
<ul>
  <li>Use crafting to create new items from existing items</li>
  <li>Crafting always uses one action</li>
</ul>
EOF;

  }

  echo <<< EOF
<div class="title">
  $actions_type Actions
</div>
<div class="subtitle">
  Select three items
 </div>
<form class="item-picklist" action="/$game/crafting_do/$arg2">
EOF;

  // Get list of equipment useful to crafting
  $useful_equipment = array();
  $sql = 'SELECT * from crafting;';
  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $useful_equipment[$item->fkey_equipment_id_1] = TRUE;
    $useful_equipment[$item->fkey_equipment_id_2] = TRUE;
    $useful_equipment[$item->fkey_equipment_id_3] = TRUE;
  }
//  firep($useful_equipment, 'useful equipment');

  // Get list of equipment owned, filtered by what is craftable.
  $data = array();
  $sql = 'SELECT equipment.*, equipment_ownership.quantity
    FROM equipment

    LEFT OUTER JOIN equipment_ownership
      ON equipment_ownership.fkey_equipment_id = equipment.id
      AND equipment_ownership.fkey_users_id = %d

    WHERE
      equipment_ownership.quantity > 0
    AND
      equipment.id IN (%s)
    OR

      "%s" = "abc123"

    ORDER BY equipment.name ASC';
  $result = db_query($sql, $game_user->id, implode(',', array_keys($useful_equipment)), $arg2);
  while ($item = db_fetch_object($result)) {
    $item->quantity = 0 + $item->quantity;
    $data[] = $item;
  }

  // Show three picklists.
  foreach (array(1, 2, 3) as $picklist) {

    echo <<< EOF
<select name="item_$picklist">
  <option value="0">Select item $picklist</option>
EOF;

    foreach ($data as $item) {
      echo <<< EOF
      <option value="$item->id">$item->name ($item->quantity)</option>
EOF;
    }

    echo '</select>';

  }

  echo <<< EOF
  <div class="try-an-election-wrapper">
    <input class="try-an-election" type="submit" Value="Craft"/>
   </div>
</form>
EOF;

  db_set_active('default');
