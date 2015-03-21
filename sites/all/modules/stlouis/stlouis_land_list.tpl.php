<?php

  global $game, $phone_id;

  $fetch_user = '_' . arg(0) . '_fetch_user';
  $fetch_header = '_' . arg(0) . '_header';

  $game_user = $fetch_user();
  include_once(drupal_get_path('module', $game) . '/game_defs.inc');
  $arg2 = check_plain(arg(2));
  $ai_output = 'land-prices';

// sometimes the income gets hacked - fix here

  $sql = 'update users set income =
    (SELECT sum(land.payout * land_ownership.quantity)
    as income from land 
    left join land_ownership
    on land_ownership.fkey_land_id = land.id and
    land_ownership.fkey_users_id = %d)
    where id = %d;';  
  $result = db_query($sql, $game_user->id, $game_user->id);

  $game_user = $fetch_user();
  $fetch_header($game_user);

  $sql = 'select clan_title from `values` where id = %d;';
  $result = db_query($sql, $game_user->fkey_values_id);
  $data = db_fetch_object($result);
  $clan_title = preg_replace('/^The /', '', $data->clan_title);

  echo <<< EOF
<div class="news">
  <a href="/$game/land/$arg2" class="button active">$land_plural</a>
  <a href="/$game/equipment/$arg2" class="button">$equipment</a>
EOF;

  if ($game != 'celestial_glory') {

    echo <<< EOF
  <a href="/$game/staff/$arg2" class="button">Staff</a>
  <a href="/$game/agents/$arg2" class="button">Agents</a>
EOF;

  }

  echo <<< EOF
</div>  
EOF;

  if ($game_user->level < 15) {
    
    echo <<< EOF
<ul>
  <li>Purchase $land_plural_lower to earn hourly income</li>
</ul>
EOF;

  }
  
  echo <<< EOF
<div class="title">
Purchase $land_plural
</div>
EOF;

  $sql_to_add = '    WHERE ((
      fkey_neighborhoods_id = 0
      OR fkey_neighborhoods_id = %d
    ) 
    
    AND
    
    (
      fkey_values_id = 0
      OR fkey_values_id = %d
    ))
  
    AND required_level <= %d
    AND active =1';

  if ($game_user->phone_id == 'abc123')
    $sql_to_add = '';
    
  $data = array();
  $sql = 'SELECT land.*, land_ownership.quantity
    FROM land
    
    LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
    AND land_ownership.fkey_users_id = %d
    ' . $sql_to_add . '
    ORDER BY required_level ASC';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  while ($item = db_fetch_object($result)) $data[] = $item;
  
  foreach ($data as $item) {
firep($item);
    
    $description = str_replace('%clan', "<em>$clan_title</em>",
      $item->description);
      
    $quantity = $item->quantity;
    if (empty($quantity)) $quantity = '<em>None</em>';

    $land_price = $item->price + ($item->quantity *
      $item->price_increase);
      
    $ai_output .= " $item->id=$land_price";

    if (($land_price % 1000) == 0)
      $land_price = ($land_price / 1000) . 'K';

    $payout = $item->payout;

    if (($payout % 1000) == 0)
      $payout = ($payout / 1000) . 'K';

    echo <<< EOF
<div class="land">
  <div class="land-icon"><a href="/$game/land_buy/$arg2/$item->id/1"><img
    src="/sites/default/files/images/land/$game-$item->id.png" width="96"
    border="0"></a></div>
  <div class="land-details">
    <div class="land-name"><a
      href="/$game/land_buy/$arg2/$item->id/1">$item->name</a></div>
    <div class="land-description">$description</div>
    <div class="land-owned">Owned: $quantity</div>
    <div class="land-cost">Cost: $land_price $game_user->values</div>
    <div class="land-payout">Income: +$payout $game_user->values
      every 60 minutes</div>
  </div>
  <div class="land-button-wrapper"><div class="land-buy-button"><a
    href="/$game/land_buy/$arg2/$item->id/1">Buy</a></div>
  <div class="land-sell-button"><a
    href="/$game/land_sell/$arg2/$item->id/1">Sell</a></div></div>
</div>
EOF;

  }

  if (substr($phone_id, 0, 3) == 'ai-')
    echo "<!--\n<ai \"$ai_output\"/>\n-->";
  
// show next one
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
    AND active =1
    ORDER BY required_level ASC LIMIT 1';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  $item = db_fetch_object($result);
firep($item);
    
  if (!empty($item)) {
    $description = str_replace('%clan', "<em>$clan_title</em>",
      $item->description);
      
    $quantity = $item->quantity;
    if (empty($quantity)) $quantity = '<em>None</em>';
  
    $land_price = $item->price + ($item->quantity *
      $item->price_increase);
      
    echo <<< EOF
<div class="land-soon">
  <div class="land-details">
    <div class="land-name">$item->name</div>
    <div class="land-description">$description</div>
    <div class="land-required_level">Requires level $item->required_level</div>
    <div class="land-cost">Cost: $land_price $game_user->values</div>
    <div class="land-payout">Income: +$item->payout $game_user->values
      every 60 minutes</div>
  </div>
</div>
EOF;

  }
  
  db_set_active('default');
