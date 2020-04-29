<?php

/**
 * @file
 * Template for bounce.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: yes
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: N/A
 * .
 */

$game = check_plain(arg(0));
$arg2 = check_plain(arg(2));
$version = 'v0.9.6, Mar 25 2020';

db_set_active('game_' . $game);
$d = zg_get_html([
  'tagline',
]);
$button = zg_render_button();
$game_user = zg_fetch_user_by_id(zg_get_phoneid());
$game_user_str = zg_render_user($game_user, 'header');
$welcome_msg = strlen($game_user->username) ? t('Welcome back to') :
  t('Welcome to');
db_set_active();

if ($arg2 == 'facebook') {

//  $phone_id = zg_get_fbid();
  // echo $phone_id;
  echo <<< EOF
<form method=post action="/$game/home/$arg2">
  <input type="Submit" value="Continue to the game">
</form>
EOF;

<<<<<<< Updated upstream
  return;
}

// Landscape?  Show user profile.
if (zg_is_landscape() && strlen($game_user->username)) {
  echo <<< EOF
<!-- player -->
<div id="player">
  $game_user_str
</div>
EOF;
}

?>

<br>
<br>
<br>
<div class="bounce-welcome">
  <?php print $welcome_msg; ?>
</div>
<div class="title">
  <img src="/sites/default/files/images/<?php print $game; ?>_title.png">
</div>
<div class="tagline">
  <?php print $d['tagline']; ?>
</div>
<br>
<?php print $button; ?>
<br>
<br>
<br>
<br>
<br>

<a class="version" href="/<?php print $game; ?>/changelog/<?php print $arg2; ?>">
  <?php print $version; ?>
</a>
=======
  db_set_active();
  return;
}

db_set_active();
drupal_goto($game . '/home/' . $arg2);
>>>>>>> Stashed changes
