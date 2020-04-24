<?php

/**
 * @file
 * Template for bounce.
 *
 * Synced with CG: N/A
 * Synced with 2114: N/A
 * Ready for phpcbf: yes
 * Ready for MVC separation: done
 * Controller moved to callback include: N/A
 * View only in theme template: yes
 * All db queries in controller: yes
 * Minimal function calls in view: yes
 * Removal of globals: no
 * Removal of game_defs include: N/A
 * .
 */

// We won't have gone through fetch_user() yet, so set these here.
$game = 'stlouis';
$phone_id = zg_get_phoneid();
$arg2 = check_plain(arg(2));
db_set_active('game_' . $game);

$sql = 'select * from users where phone_id = "%s";';
$result = db_query($sql, $phone_id);
$game_user = db_fetch_object($result);
firep($game_user, 'game_user object');

if ($game_user->meta == 'admin') {
  $button = zg_render_button() . "<p class='center'>(if you dare)</p>";
}
else {
  $button = '';
}

?>
<br>
<br>
<h1 class="title center">???</h1>
<br>
<div class="center">Maybe you should rotate back?</div>
<br>
<br>
<?php print $button;
