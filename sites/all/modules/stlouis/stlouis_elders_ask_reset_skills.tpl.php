<?php

/**
 * @file stlouis_elders_ask_reset_skills.tpl.php
 * Template for confirmation of resettings skill points.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 */

  global $game, $phone_id;
  
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $game_user = $fetch_user();
  $fetch_header($game_user);

  echo <<< EOF
<div class="title">
Do you really want to reset your skill points?
</div>
<div class="subtitle">
All the $initiative, Endurance, $elocution, and Action your character has
  collected will be converted back into skill points
</div>
<div class="elders-menu big">
<div class="menu-option"><a href="/$game/elders_do_reset_skills/$phone_id">Yes,
  I want to reset my skill points</a></div>
</div>
EOF;
    
  db_set_active('default');  
