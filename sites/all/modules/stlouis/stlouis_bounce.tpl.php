<?php

/**
 * @file
 */

  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  $get_id = '_' . $game . '_get_fbid';

  if ($arg2 == 'facebook') {

    $phone_id = $get_id();
// Echo $phone_id;.
    echo <<< EOF
<form method=post action="/$game/home/$arg2">
<input type="Submit" value="Continue to the game"/>
</form>
EOF;

    db_set_active('default');
    return;

  }

  db_set_active('default');
  drupal_goto($game . '/home/' . $phone_id);
