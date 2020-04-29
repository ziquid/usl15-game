<?php

/**
 * @file
 * User asks for reset.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
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

/* ------ CONTROLLER ------ */

global $game;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();
zg_fetch_header($game_user);
db_set_active();

/* ------ VIEW ------ */

if ($game_user->level >= 50) {
  echo <<< EOF
<div class="title">
  Please e-mail us
</div>
<div class="subtitle">
  E-mail us at <strong>zipport@ziquid.com</strong> to have your character reset
</div>
EOF;
  return;
}

if (check_plain($_GET['msg']) == 'error') {
  echo <<< EOF
<div class="subsubtitle error">Please enter &quot;RESET ME&quot;.</div>
EOF;
}

echo <<< EOF
<div class="title">
  Do you really want to reset your character?
</div>
<div class="subtitle">
  You will lose all $experience, stats, and $game_user->values your character
  has collected
</div>
<div class="subsubtitle">
  If you really want to reset, enter the words &quot;RESET&nbsp;ME&quot; here:
</div>
<div class="ask-name">
  <form method=get action="/$game/elders_do_reset/$arg2">
    <input type="text" name="reset_me" size="8" maxlength="8"/>
    <input type="submit" value="Submit"/>
  </form>
</div>
EOF;
