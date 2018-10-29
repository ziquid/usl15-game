<?php

/**
* @file
* Template for bounce.
*
* Synced with CG: yes
* Synced with 2114: yes
* Ready for phpcbf: no
* Ready for MVC separation: no
*/

$game = check_plain(arg(0));
$arg2 = check_plain(arg(2));

if ($arg2 == 'facebook') {

  $get_id = '_' . $game . '_get_fbid';
  $phone_id = $get_id();
// echo $phone_id;
  echo <<< EOF
<form method=post action="/$game/home/$arg2">
  <input type="Submit" value="Continue to the game">
</form>
EOF;

  db_set_active('default');
  return;
}

db_set_active('default');
drupal_goto($game . '/home/' . $phone_id);
