<?php

/**
 * @file
 * The game's error screen.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

// Don't go through fetch_user(); set these here.
$game = check_plain(arg(0));
$phone_id = zg_get_phoneid();
$arg2 = check_plain(arg(2));

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
db_set_active('default');

echo <<< EOF
  <div class="title">
    <img src="/sites/default/files/images/{$game}_title.png"/>
  </div>
  <p>&nbsp;</p>
  <div class="welcome">
   <div class="wise_old_man_large">

  </div>
  <p>And it came to pass that</p>
  <p class="second">Error <strong>{$error_code}</strong></p>
  <p class="second">happened for user</p>
  <p class="second"><strong>{$phone_id}</strong>.</p>
  <p class="second">Please report this to
    <strong>zipport@ziquid.com</strong>.
  </p>
  </div>
EOF;

slack_send_message('Error ' . $error_code . ' for phone ID '
. $phone_id, $slack_channel);
