<?php

/**
 * @file
 * Template for confirmation of resetting skill points.
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
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_player();
zg_fetch_header($game_user);

echo <<< EOF
<div class="title">
  Do you really want to reset your skill points?
</div>
<div class="subtitle">
  All the $initiative, $endurance, $elocution, and $action your character has
  collected will be converted back into skill points
</div>
<div class="elders-menu big">
  <div class="menu-option">
    <a href="/$game/elders_do_reset_skills/$arg2">
      Yes, I want to reset my skill points
    </a>
  </div>
</div>
EOF;

db_set_active();
