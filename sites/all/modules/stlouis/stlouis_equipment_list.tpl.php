<?php

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$ai_output = 'equipment-prices';

// Fix expenses in case they are out of whack.
game_recalc_income($game_user);
$data = game_fetch_visible_equip($game_user);
$next = game_fetch_next_equip($game_user);

// ------ VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);

if ($game_user->level < 15) {
  echo <<< EOF
<ul>
  <li>Purchase $equipment_lower to help you and your aides</li>
</ul>
EOF;
}

echo '<div id="all-equip">';

foreach ($data as $item) {
//firep($item, 'equipment item');
  game_show_equip($game_user, $item, $ai_output);
}

game_show_ai_output($phone_id, $ai_output);

// show next one
if (!empty($next)) {
  game_show_equip($game_user, $item, $ai_output, ['soon' => TRUE]);
}

echo '</div>';
db_set_active('default');
