<?php

/**
 * @file
 * The game's SECRET HIDDEN PAGE!!!
 *
 * Synced with CG: N/A
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: yes
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_player();
zg_fetch_header($game_user);
db_set_active();

?>
<div class="title">
  You did it!
</div>

<div class="subtitle">
  You found the secret page!
</div>

<div class="subsubtitle">
  Congratulations!
</div>

<div class="subsubtitle">
  And that's it.
</div>

<?php print $button;
