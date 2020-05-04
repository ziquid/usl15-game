<?php

/**
 * @file
 * Game increase skills page.
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
$game_user = zg_fetch_user();
$ai_output = 'increase-skill-failed';

switch ($skill) {

  case 'initiative':
  case 'endurance':
  case 'elocution':

    if ($game_user->skill_points >= 1) {
      $sql = 'update users set %s = %s + 1, skill_points = skill_points - 1
        where id = %d;';
      $result = db_query($sql, $skill, $skill, $game_user->id);
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'elocution_10':

    if ($game_user->skill_points >= 10) {
      $sql = 'update users set elocution = elocution + 10,
  skill_points = skill_points - 10
        where id = %d;';
      $result = db_query($sql, $game_user->id);
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'endurance_10':

    if ($game_user->skill_points >= 10) {
      $sql = 'update users set endurance = endurance + 10,
  skill_points = skill_points - 10
        where id = %d;';
      $result = db_query($sql, $game_user->id);
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'initiative_10':

    if ($game_user->skill_points >= 10) {
      $sql = 'update users set initiative = initiative + 10,
        skill_points = skill_points - 10
        where id = %d;';
      $result = db_query($sql, $game_user->id);
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'energy_max':

    if ($game_user->skill_points >= 1) {
      $sql = 'update users set energy = energy + 10,
        energy_max = energy_max + 10, skill_points = skill_points - 1
        where id = %d;';
      $result = db_query($sql, $game_user->id);
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'energy_100':

    if ($game_user->skill_points >= 10) {
      $sql = 'update users set energy = energy + 100,
        energy_max = energy_max + 100, skill_points = skill_points - 10
        where id = %d;';
      $result = db_query($sql, $game_user->id);
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'actions_5':

    if ($game_user->skill_points >= 10) {
      $sql = 'update users set actions = actions + 5,
        actions_max = actions_max + 5, skill_points = skill_points - 10
        where id = %d;';
      $result = db_query($sql, $game_user->id);

      // Start the actions clock if needed.
      if ($game_user->actions == $game_user->actions_max) {
        $sql = 'update users set actions_next_gain = "%s" where id = %d;';
        $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
          $game_user->id);
      }
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'actions':

    if ($game_user->skill_points >= 2) {
      $sql = 'update users set actions = actions + 1,
        actions_max = actions_max + 1, skill_points = skill_points - 2
        where id = %d;';
      db_query($sql, $game_user->id);

      // Start the actions clock if needed.
      if ($game_user->actions == $game_user->actions_max) {
        $sql = 'update users set actions_next_gain = "%s" where id = %d;';
        db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
          $game_user->id);
      }
      $game_user = zg_fetch_user();
      $ai_output = 'increase-skill-succeeded';
    }
    break;

  case 'none':

    $ai_output = 'increase-skill-shown';
    break;
}

zg_fetch_header($game_user);

// _show_goal($game_user);

echo <<< EOF
<div class="goals current">
<div class="title">
  Skill Points Remaining: $game_user->skill_points
</div>
<ul>
  <li>
    Use skill points to increase your character's abilities
  </li>
  <li>
    All abilities cost 1 point to increase; Actions cost 2
  </li>
  <li>
    Once a skill point has been used, it cannot be undone
  </li>
</ul>
</div>
<div class="user-profile">
EOF;

$energy_button = zg_render_button('increase_skills', 'Increase',
  '/energy_max', 'skill-point');
$energy_100_button = zg_render_button('increase_skills', 'Inc +100',
  '/energy_100', 'skill-point');
$ini_button = zg_render_button('increase_skills', 'Increase',
  '/initiative', 'skill-point');
$ini_10_button = zg_render_button('increase_skills', 'Inc +10',
  '/initiative_10', 'skill-point');
$end_button = zg_render_button('increase_skills', 'Increase',
  '/endurance', 'skill-point');
$end_10_button = zg_render_button('increase_skills', 'Inc +10',
  '/endurance_10', 'skill-point');
$elo_button = zg_render_button('increase_skills', 'Increase',
  '/elocution', 'skill-point');
$elo_10_button = zg_render_button('increase_skills', 'Inc +10',
  '/elocution_10', 'skill-point');
$act_button = zg_render_button('increase_skills', 'Increase',
  '/actions', 'skill-point');
$act_5_button = zg_render_button('increase_skills', 'Inc +5',
  '/actions_5', 'skill-point');
$cant_button = zg_render_button('increase_skills', 'Can\'t Increase',
  '/none', 'skill-point not-yet');

if ($game_user->skill_points == 0) {

  echo <<< EOF
<div class="heading">Energy:</div>
<div class="value">$game_user->energy_max $cant_button</div><br>

<div class="heading">{$game_text['initiative']}:</div>
<div class="value">$game_user->initiative $cant_button</div><br>

<div class="heading">{$game_text['endurance']}:</div>
<div class="value">$game_user->endurance $cant_button</div><br>

<div class="heading">$elocution:</div>
<div class="value">$game_user->elocution $cant_button</div><br>

  <div class="heading">Actions:</div>
<div class="value">$game_user->actions_max $cant_button</div><br>
</div>
EOF;

}
elseif ($game_user->skill_points == 1) {

  echo <<< EOF
<div class="heading">Energy:</div>
<div class="value">$game_user->energy_max $energy_button</div><br>

<div class="heading">{$game_text['initiative']}:</div>
<div class="value">$game_user->initiative $ini_button</div><br>

<div class="heading">{$game_text['endurance']}:</div>
<div class="value">$game_user->endurance $end_button</div><br>

<div class="heading">$elocution:</div>
<div class="value">$game_user->elocution $elo_button</div><br>

  <div class="heading">Actions:</div>
<div class="value">$game_user->actions_max $cant_button</div><br>
</div>
EOF;

}
elseif ($game_user->skill_points >= 10) {

      echo <<< EOF
<div class="heading">Energy:</div>
<div class="value">$game_user->energy_max $energy_button $energy_100_button</div><br>

<div class="heading">{$game_text['initiative']}:</div>
<div class="value">$game_user->initiative $ini_button $ini_10_button</div><br>

<div class="heading">{$game_text['endurance']}:</div>
<div class="value">$game_user->endurance $end_button $end_10_button</div><br>

<div class="heading">$elocution:</div>
<div class="value">$game_user->elocution $elo_button $elo_10_button</div><br>

<div class="heading">Actions:</div>
<div class="value">$game_user->actions_max $act_button $act_5_button</div><br>
</div>
EOF;

}
elseif ($game_user->skill_points > 1) {

      echo <<< EOF
<div class="heading">Energy:</div>
<div class="value">$game_user->energy_max $energy_button</div><br>

<div class="heading">{$game_text['initiative']}:</div>
<div class="value">$game_user->initiative $ini_button</div><br>

<div class="heading">{$game_text['endurance']}:</div>
<div class="value">$game_user->endurance $end_button</div><br>

<div class="heading">$elocution:</div>
<div class="value">$game_user->elocution $elo_button</div><br>

<div class="heading">Actions:</div>
<div class="value">$game_user->actions_max $act_button</div><br>
</div>
EOF;

}

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"$ai_output\"/>\n-->";
}

db_set_active();
