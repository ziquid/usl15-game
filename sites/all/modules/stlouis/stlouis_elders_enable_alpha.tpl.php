<?php

/**
 * Template for enabling/disabling prerelease/alpha access.
 * @file stlouis_elders_enable_alpha.tpl.php
 *
 * Synced with CG: N/A
 * Synced with 2114: N/A
 * Ready for phpcbf: yes
 */

// ------ CONTROLLER ------
global $game, $phone_id;

include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();

// User chose to toggle!
if ($arg3 == 'yes') {
  game_set_value($game_user, 'enabled_alpha', !game_get_value($game_user, 'enabled_alpha'));
  db_set_active();
  drupal_goto("/$game/elders/$arg2");
}

if (game_get_value($game_user, 'enabled_alpha')) {
  $enable = 'Disable';
}
else {
  $enable = 'Enable';
}
$enable_lower = drupal_strtolower($enable);

// ------ VIEW ------
$fetch_header($game_user);
db_set_active('default');
?>

<div class="title">
  <?php print $enable; ?> Alpha access?
</div>
<div class="subtitle">
  Do you really want to <?php print $enable_lower; ?> pre-release (alpha) access?
</div>
<p>
  Enabling pre-release features may give you early access to new features but
  <em>they may not be stable!</em>  In fact, <em>they may not work at all!</em>
</p>
<p>
  If you are unsure, disabling pre-release features is the safer choice.
</p>

<div class="elders-menu big">
  <div class="menu-option">
    <a href="/<?php print $game; ?>/elders_enable_alpha/<?php print $arg2; ?>/yes">
      Yes, I want to <?php print $enable_lower; ?> pre-release (Alpha) features
    </a>
  </div>
  <div class="menu-option">
    <a href="/<?php print $game; ?>/elders/<?php print $arg2; ?>">
      No, I prefer to keep things the way they are
    </a>
  </div>
</div>

<div class="message-title">If you enable pre-release features, you can further customize your game experience by entering a code here:</div>
<div class="send-message">
  <form method="get" action="">
    <input type="text" class="message-textarea" name="code"/>
    <input class="send-message-send" type="submit" value="send"/>
  </form>
</div>

