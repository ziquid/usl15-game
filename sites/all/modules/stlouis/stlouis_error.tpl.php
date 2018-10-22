<?php

/**
 * @file
 * The game's error screen.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 */

global $game, $phone_id;

include drupal_get_path('module', arg(0)) . '/game_defs.inc';
$game_user = $fetch_user();
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
