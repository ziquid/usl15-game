<?php

/**
 * @file stlouis_staff_list.tpl.php
 * Stlouis staff list
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 */

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$ai_output = 'staff-prices';

// Fix expenses in case they are out of whack.
game_recalc_income($game_user);
$data = game_fetch_visible_staff($game_user);
$next = game_fetch_next_staff($game_user);

// ------ VIEW ------
$fetch_header($game_user);
game_show_aides_menu($game_user);

if ($game_user->level < 15) {
  echo <<< EOF
    <ul>
      <li>Hire {$game_text['staff']} to help you win elections and stay elected</li>
    </ul>
EOF;
}

echo '<div id="all-staff">';

foreach ($data as $item) {
  game_show_staff($game_user, $item,$ai_output);
}

game_show_ai_output($phone_id, $ai_output);

// Show next one.
if (!empty($next)) {
  game_show_staff($game_user, $next, $ai_output, ['soon' => TRUE]);
}

echo '</div>';
db_set_active('default');
