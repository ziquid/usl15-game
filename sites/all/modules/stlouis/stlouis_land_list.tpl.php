<?php

/**
 * @file stlouis_land _list.tpl.php
 * Stlouis land list
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$ai_output = 'land-prices';

game_recalc_income($game_user);
$data = game_fetch_visible_land($game_user);
$next = game_fetch_next_land($game_user);

// ------ VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);
echo '<div id="all-land">';

foreach ($data as $item) {
//firep($item, 'Item: ' . $item->name);
  game_show_land($game_user, $item);

  $land_price = $item->price + ($item->quantity * $item->price_increase);
  $ai_output .= " $item->id=$land_price";
}

game_show_ai_output($phone_id, $ai_output);

if (!empty($next)) {
//  firep($next, 'Soon Item: ' . $next->name);
  game_show_land($game_user, $next, ['soon' => TRUE]);
}

echo '</div>';
db_set_active('default');
