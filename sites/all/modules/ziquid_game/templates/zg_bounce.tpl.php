<?php

/**
 * @file
 * Template for bounce.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: N/A
 * .
 */

include drupal_get_path('module', 'zg') . '/includes/game_defs.inc';
$version = $game_settings['version'] . ' ' . $game_settings['last_update'];

db_set_active('game_' . $game);
$d = zg_get_html([
  'tagline',
]);
$pl = zg_fetch_player_by_id(zg_get_phoneid());

// Start welcome wizard if user not in db.
if (empty($pl->id)) {
  db_set_active();
  drupal_goto($game . '/welcome/' . $arg2);
}

$game_user_str = zg_render_player($pl, 'header');
$welcome_msg = strlen($pl->username) ? t('Welcome back to') :
  t('Welcome to');

zg_slack($pl, 'pages', 'bounce',
  "\"Bounce\" for Player \"$pl->username\".");

if ($arg2 == 'facebook') {

  //  $phone_id = zg_get_fbid();
  // echo $phone_id;
  echo <<< EOF
<form method=post action="/$game/home/$arg2">
  <input type="Submit" value="Continue to the game">
</form>
EOF;

  return;
}

/*
// Landscape?  Show user profile.
if (zg_is_landscape() && strlen($pl->username)) {
  echo <<< EOF
<!-- player -->
<div id="player">
  $game_user_str
</div>
EOF;
  zg_song($pl, 'Jump welcome');
}
*/

db_set_active();
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
<p class="center small">For support, please contact <strong>zipport@ziquid.com</strong>.</p>
<br>

<a class="version" href="/<?php print $game; ?>/changelog/<?php print $arg2; ?>">
  <?php print $version; ?>
</a>
