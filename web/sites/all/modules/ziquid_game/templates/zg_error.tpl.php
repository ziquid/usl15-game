<?php

/**
 * @file
 * The game's error screen.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 * Ready for phpcbf: done
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

// Don't go through fetch_user(); set these here.
$game = check_plain(arg(0));
db_set_active('game_' . $game);
$phone_id = zg_get_phoneid();
$arg2 = check_plain(arg(2));
$game_user = new stdClass();

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$d = zg_get_html(['tagline']);

zg_slack($game_user, 'error', $error_code,
  'Error ' . $error_code . ' for phone ID ' . $phone_id);
zg_song($game_user, 'In the air tonight teaser', TRUE);
db_set_active();

/* ------ VIEW ------ */
?>

<div class="title">
  <img src="/sites/default/files/images/<?php print $game; ?>_title.png">
</div>
<div class="tagline">
  <?php print $d['tagline']; ?>
</div>

<br>

<div class="speech-bubble-wrapper background-color">
  <div class="wise_old_man embarrassed">
  </div>
  <div class="speech-bubble">
    <p>D'oh!</p>
    <p>Error <strong><?php print $error_code; ?></strong></p>
    <p>happened for user</p>
    <p><strong><?php print $phone_id; ?></strong></p>
    <p>If you message us at <strong>zipport@ziquid.com</strong>,
      we will do what we can to help you.
    </p>
  </div>
</div>
