<?php

/**
 * @file
 * Game Elders page.
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
$game_user = zg_fetch_user();
zg_fetch_header($game_user);

if (substr($phone_id, 0, 3) == 'ai-') {

  // Useful for freshening stats.
  echo "<!--\n<ai \"elders\"/>\n-->";
  db_set_active('default');
  return;
}

$offer = game_luck_money_offer($game_user);

echo <<< EOF
<div class="title">Visit the {$game_text['elders']}</div>
<div class="subtitle">You have $game_user->luck&nbsp;$luck</div>
<div class="elders-menu slide-in from-right">
EOF;

if ($game_user->level >= 6) {

  // Only allow users to change parties if they can join one.

  // AT LEAST 10 LUCK!
  if ($game_user->luck > 9) {

    echo <<< EOF
<div class="menu-option slide-in-content"><a href="/$game/choose_name/$arg2">Change your
character's name (10&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/choose_party/$arg2/0">Join a
different $party_lower (5&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_ask_reset_skills/$arg2">Reset
your skill points (3&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  elseif ($game_user->luck > 4) {

    // AT LEAST 5 LUCK!
    echo <<< EOF
<div class="menu-option not-yet slide-in-content">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option slide-in-content"><a href="/$game/choose_party/$arg2/0">Join a
different $party_lower (5&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_ask_reset_skills/$arg2">Reset
your skill points (3&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  elseif ($game_user->luck > 2) {

    // AT LEAST THREE LUCK!
    echo <<< EOF
<div class="menu-option not-yet slide-in-content">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Join a different $party_lower (5&nbsp;$luck)</div>
<div class="menu-option slide-in-content"><a href="/$game/elders_ask_reset_skills/$arg2">Reset
your skill points (3&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  elseif ($game_user->luck > 0) {

    // AT LEAST ONE LUCK!
    echo <<< EOF
<div class="menu-option not-yet slide-in-content">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Join a different $party_lower (5&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Reset your skill points (3&nbsp;$luck)</div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  else {

    // NO LUCK!
    echo <<< EOF
<div class="menu-option not-yet slide-in-content">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Join a different $party_lower (5&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Reset your skill points (3&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Refill your Action (1&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Refill your Energy (1&nbsp;$luck)</div>
<div class="menu-option not-yet slide-in-content">Receive $offer $game_user->values (1&nbsp;$luck)</div>
EOF;

  }

}

echo <<< EOF
<div class="menu-option slide-in-content"><a href="/$game/elders_set_password/$arg2">Set a
password for your account (Free)</a></div>
<div class="menu-option slide-in-content"><a href="/$game/elders_ask_reset/$arg2">Reset
your character (Free)</a></div>
<!--<div class="menu-option slide-in-content"><a href="/$game/elders_preferences/$arg2">Game
Preferences</a></div>-->
<div class="menu-option slide-in-content"><a href="/$game/elders_ask_purchase/$arg2">Purchase
more $luck</a></div>
EOF;

if (zg_get_value($game_user, 'enabled_alpha')) {
  $enable = 'Disable';
}
else {
  $enable = 'Enable';
}
echo <<< EOF
<div class="menu-option slide-in-content">
  <a href="/$game/elders_enable_alpha/$arg2">
    $enable pre-release (Alpha) features (Free)
  </a>
</div>
EOF;

if (zg_get_value($game_user, 'NothingButFur', FALSE)) {
  echo <<< EOF
<div class="menu-option slide-in-content">
  <a href="/wonderland/bounce/$arg2">
    Go Down the Rabbit Hole (Free)
  </a>
</div>
EOF;
}

if (zg_get_value($game_user, 'RockThisTown', FALSE)) {
  echo <<< EOF
<div class="menu-option slide-in-content">
  <a href="/detroit/bounce/$arg2">
    Move to Detroit (Free)
  </a>
</div>
EOF;
}

db_set_active('default');
