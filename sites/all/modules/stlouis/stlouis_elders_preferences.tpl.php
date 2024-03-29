<?php

/**
 * @file
 * Stlouis preferences page.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
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

$ask_luck_refill = trim(check_plain($_GET['ask_luck_refill']));
if ($ask_luck_refill <= 0) {
  $ask_luck_refill = 0;
}

$currentPreferences = zg_get_value($game_user, 'ask_before_refilling_luck',FALSE);
if ($currentPreferences) {
  $checkedYes = 'checked="checked"';
}
else {
  $checkedNo = 'checked="checked"';
}

echo <<< EOF
<div class="title">Game Preferences</div>
<div class="subtitle">Ask for confirmation when refilling with $luck?</div>  
<div class="menu-option">
  <div class="ask-name">
    <form method=get action="/$game/elders_preferences/$arg2">      
      <input type="radio" name="ask_luck_refill" value="1" $checkedYes />&nbsp;
        Yes
      <input type="radio" name="ask_luck_refill" value="0" $checkedNo />&nbsp;No
      <input type="submit" value="Submit"/>
    </form>
  </div>  
</div>
EOF;

if (($ask_luck_refill) >= 0) {
  zg_set_value($game_user, 'ask_before_refilling_luck', (bool) $ask_luck_refill);
}

db_set_active();
