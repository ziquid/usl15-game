<?php

/**
 * @file
 * List of land (income).
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: yes
 * Ready for MVC separation: done
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

/* ------ CONTROLLER ------ */
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();
$ai_output = 'land-prices';

game_recalc_income($game_user);
$data = zg_fetch_visible_land($game_user);
$next = zg_fetch_next_land($game_user);

/* ------ VIEW ------ */
zg_fetch_header($game_user);
zg_show_aides_menu($game_user);
echo '<div id="all-land">';

foreach ($data as $item) {
//firep($item, 'Item: ' . $item->name);
  zg_show_land($game_user, $item);

  $land_price = $item->price + ($item->quantity * $item->price_increase);
  $ai_output .= " $item->id=$land_price";
}

zg_show_ai_output($phone_id, $ai_output);

if (!empty($next)) {
//  firep($next, 'Soon Item: ' . $next->name);
  zg_show_land($game_user, $next, ['soon' => TRUE]);
}

db_set_active('default');
?>
</div>
