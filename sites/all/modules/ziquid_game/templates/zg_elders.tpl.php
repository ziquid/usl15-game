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

/* ------ CONTROLLER ------ */

global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();
zg_fetch_header($game_user);

<<<<<<< Updated upstream
=======
if (empty($game_user->username)) {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

>>>>>>> Stashed changes
if (substr($phone_id, 0, 3) == 'ai-') {

  // Useful for freshening stats.
  echo "<!--\n<ai \"elders\"/>\n-->";
  db_set_active();
  return;
}

if ($game_user->luck > 99) {
  $happy = 'happy';
  $luck_text = '<p>You have <strong>' . $game_user->luck . '&nbsp;' . $luck . '</strong>!</p>';
  $help_text = '<p>How can I help you?</p>';
}
elseif ($game_user->luck > 0) {
  $happy = 'small';
  $luck_text = '<p>You have <strong>' . $game_user->luck . '</strong>&nbsp;' . $luck . '.</p>';
  $help_text = '<p>How can I help you?</p>';
}
else {
  $happy = 'sad';
  $luck_text = '<p>You are <strong class="yellow">Out of ' . $luck . '</strong>.</p>';
  $help_text = '<p>Would you like more ' . $luck . '?</p>';
}
$offer = number_format(game_luck_money_offer($game_user));
$elder_welcome = '<div class="speech-bubble-wrapper background-color">
  <div class="wise_old_man ' . $happy . '">
  </div>
  <div class="speech-bubble">
    <p>Hello again, <em>' . $game_user->username . '</em>!</p>
    ' . $luck_text . '
    ' . $help_text . '
  </div>
</div>';

echo <<< EOF
<div class="title">Visit the {$game_text['elders']}</div>
$elder_welcome
<div class="elders-menu slide-in from-right">
EOF;

$menus = [];

// Only allow users to change parties if they can join one.
if ($game_user->level >= 6) {

  if ($game_user->luck > 9) {
    $menus[] = zg_render_button('choose_name',
      "Change your character's name (10&nbsp;Luck)",
      '',
      'big-80 slide-in-content');
  }
  else {
    $menus[] = zg_render_button('',
      "Change your character's name (10&nbsp;Luck)",
      '',
      'big-80 slide-in-content');
  }

  if ($game_user->luck > 4) {
    $menus[] = zg_render_button('choose_party',
      "Join a different $party_lower (5&nbsp;$luck)",
      '/0',
      'big-80 slide-in-content');
  }
  else {
    $menus[] = zg_render_button('',
      "Join a different $party_lower (5&nbsp;$luck)",
      '',
      'big-80 slide-in-content');
  }

  if ($game_user->luck > 2) {
    $menus[] = zg_render_button('elders_ask_reset_skills',
      "Reset your skill points (3&nbsp;$luck)",
      '',
      'big-80 slide-in-content');
  }
  else {
    $menus[] = zg_render_button('',
      "Reset your skill points (3&nbsp;$luck)",
      '',
      'big-80 slide-in-content');
  }

  if ($game_user->luck > 0) {
    $menus[] = zg_render_button('elders_do_fill',
      "Refill your Action (1&nbsp;$luck)",
      '/action',
      'big-80 slide-in-content');
    $menus[] = zg_render_button('elders_do_fill',
      "Refill your Energy (1&nbsp;$luck)",
      '/energy',
      'big-80 slide-in-content');
    $menus[] = zg_luck_money_render_button($game_user);
  }
  else {
    $menus[] = zg_render_button('',
      "Refill your Action (1&nbsp;$luck)",
      '',
      'big-80 slide-in-content');
    $menus[] = zg_render_button('',
      "Refill your Energy (1&nbsp;$luck)",
      '',
      'big-80 slide-in-content');
    $menus[] = zg_render_button('',
      "Receive $offer $game_user->values (1&nbsp;$luck)",
      '',
      'big-80 slide-in-content');
  }

}

$menus[] = zg_render_button('elders_set_password',
  "Set a password for your account (Free)",
  '',
  'big-80 slide-in-content');
$menus[] = zg_render_button('elders_ask_reset',
  "Reset your character (Free)",
  '',
  'big-80 slide-in-content');
$menus[] = zg_render_button('elders_ask_purchase',
  "Purchase more $luck",
  '',
  'big-80 slide-in-content highlight');

$enabled_alpha = (zg_get_value($game_user, 'enabled_alpha')) ?
  'Disable' : 'Enable';
$menus[] = zg_render_button('elders_enable_alpha',
  "$enabled_alpha pre-release (Alpha) features (Free)",
  '',
  'big-80 slide-in-content');

if (zg_get_value($game_user, 'NothingButFur', FALSE)) {
  $menus[] = zg_render_button('/wonderland/bounce',
    "Go Down the Rabbit Hole (Free)",
    '',
    'big-80 slide-in-content halflight');
}

if (zg_get_value($game_user, 'RockThisTown', FALSE)) {
  $menus[] = zg_render_button('/detroit/bounce',
    "Move to Detroit (Free)",
    '',
    'big-80 slide-in-content halflight');
}

db_set_active();

/* ------ VIEW ------ */

foreach ($menus as $menu) {
  print $menu;
}
