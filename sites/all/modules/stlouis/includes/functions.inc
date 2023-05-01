<?php

/**
 * Misc functions for Ziquid games.
 *
 * @file
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

require_once drupal_get_path('module', 'stlouis') . '/includes/functions-staff.inc';
//require_once drupal_get_path('module', 'stlouis') . '/includes/functions-no.inc';
require_once drupal_get_path('module', 'stlouis') . '/includes/functions-equip.inc';
require_once drupal_get_path('module', 'stlouis') . '/includes/functions-land.inc';

function game_button($link = 'home', $type = 'continue', $extra_link = '') {

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $link = $game . '/' . $link . '/' . $arg2 . $extra_link;

  echo <<< EOF
<div class="button-wrapper">
  <a class="button-$type button" href="/$link">
    $type
  </a>
</div>
EOF;
}

/**
 * Recalc the income and expenses for a user.
 *
 * @param \stdClass $game_user
 *   The game user object.
 */
function game_recalc_income(\stdClass $game_user) {
  return zg_recalc_income($game_user);
}

function game_show_aides_menu(\stdClass $game_user) {

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $title = '';

  if (substr(arg(1), 0, 4) == 'land') {
    $land_active = 'active';
  }
  if (arg(1) == 'land') {
    $title = t('Available @land', ['@land' => $land_plural]);
  }

  if (substr(arg(1), 0, 9) == 'equipment') {
    $equip_active = 'active';
  }
  if (arg(1) == 'equipment') {
    $title = t('Purchase @equip', ['@equip' => $equipment]);
  }

  if (substr(arg(1), 0, 5) == 'staff') {
    $staff_active = 'active';
  }
  if (arg(1) == 'staff') {
    $title = t('Hire @staff and @agents', [
      '@staff' => $game_text['staff'],
      '@agents' => $game_text['agents'],
    ]);
  }

  echo <<< EOF
<div class="news">
  <a href="/$game/land/$arg2" class="button $land_active">$land_plural</a>
  <a href="/$game/equipment/$arg2" class="button $equip_active">$equipment</a>
  <a href="#" class="button not-yet $clan_eq_active">{$game_text['clan_eq_short']}</a>
  <a href="/$game/staff/$arg2" class="button $staff_active">{$game_text['staff']}/{$game_text['agents']}</a>
  <!--<a href="/$game/agents/$arg2" class="button">Agents</a>-->
</div>
EOF;

  if ($land_active) {
    echo <<< EOF
<div class="news secondary">
  <div class="news-buttons">
    <button id="land-all" class="active">All</button>
    <button id="land-jobs">{$game_text['job_plural']}</button>
    <button id="land-investments">{$game_text['investment_plural']}</button>
  </div>
</div>
EOF;

    drupal_add_js(drupal_get_path('module', 'stlouis')
      . '/game_show_aides_menu_land.js', 'module', 'footer');
  }

  if ($equip_active && $game_user->level >= 25) {
    echo <<< EOF
<div class="news secondary">
  <div class="news-buttons">
    <button id="equip-all" class="active">All</button>
    <button id="equip-ini">{$game_text['initiative_short']}</button>
    <button id="equip-end">{$game_text['endurance_short']}</button>
    <button id="equip-elo">{$game_text['elocution_short']}</button>
    <button id="equip-move">Moving</button>
    <button id="equip-other">Other</button>
  </div>
</div>

<div class="news secondary">
  <div class="news-buttons">
    <button id="equip-buy">Buyable</button>
    <button id="equip-sal">Salable</button>
    <button id="equip-upk">Upkeep</button>
    <button id="equip-eng">{$game_text['energy']}</button>
    <button id="equip-act">{$game_text['actions_short']}</button>
  </div>
</div>
EOF;

    drupal_add_js(drupal_get_path('module', 'stlouis')
      . '/game_show_aides_menu_equip.js', 'module', 'footer');
  }

  if ($staff_active && $game_user->level >= 25) {
    echo <<< EOF
<div class="news secondary">
  <div class="news-buttons">
    <button id="staff-staff">Staff</button>
    <button id="staff-agent">Agents</button>
    <button id="staff-all" class="active">All</button>
    <button id="staff-ini">{$game_text['initiative_short']}</button>
    <button id="staff-end">{$game_text['endurance_short']}</button>
    <button id="staff-elo">{$game_text['elocution_short']}</button>
  </div>
</div>

<div class="news secondary">
  <div class="news-buttons">
    <button id="staff-buy">Hireable</button>
    <button id="staff-sal">Fireable</button>
    <button id="staff-upk">Upkeep</button>
    <button id="staff-eng">{$game_text['energy']}</button>
    <button id="staff-act">{$game_text['actions_short']}</button>
  </div>
</div>
EOF;

    drupal_add_js(drupal_get_path('module', 'stlouis')
      . '/game_show_aides_menu_staff.js', 'module', 'footer');
  }

//  _show_goal($game_user);

 if (!empty($title)) {
   echo <<< EOF
<div class="title">
  $title
</div>
EOF;
  }
}

/**
 * Find the icon path for a given type.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $obj
 *   The quest, etc. object.
 * @param string $type
 *   The type of object.
 */
function game_get_icon_path(\stdClass $game_user, \stdClass $obj, $type) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  if (strpos($obj->meta, 'use_job_picture') !== FALSE) {
    $sql = 'SELECT land.id FROM `land_ownership`
      LEFT JOIN land ON fkey_land_id = land.id
      WHERE fkey_users_id = %d
      AND land.type = "job";';
    $result = db_query($sql, $game_user->id);
    $job = db_fetch_object($result);
    $obj->icon_path = '/sites/default/files/images/land/' . "$game-$job->id.png?3";
  }
  else {
    $icon_path = base_path() . file_directory_path() . '/images/' . $type . '/' . $game . '-' . $obj->id;

    if (file_exists($_SERVER['DOCUMENT_ROOT'] . $icon_path . '.jpg')) {
      $obj->icon_path = $icon_path . '.jpg';
    }
    else {
      $obj->icon_path = $icon_path . '.png';
    }
  }
}

function game_competency_gain(\stdClass $game_user, $id) {
  return zg_competency_gain($game_user, $id);
}

/**
 * Debit (use) an amount of action from (by) the user.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $quantity
 *   The quantity of action to debit.
 *
 * @return bool
 *   Whether the action(s) could be debited from the game user.
 */
function game_action_use(\stdClass $game_user, $quantity = 1) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  if ($quantity < 1) {
    return TRUE;
  }
  if ($game_user->actions < $quantity) {
    return FALSE;
  }

  $sql = 'update users set actions = actions - %d
    where id = %d;';
  db_query($sql, $quantity, $game_user->id);

  // Start the actions clock, if needed.
  if ($game_user->actions == $game_user->actions_max) {
    $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
      $game_user->id);
  }

  // Reprocess user object.
  $game_user = zg_fetch_user_by_id($game_user->id);
  return TRUE;
}

/**
 * Update a user's karma.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string $text
 *   Explanatory text.
 * @param int $quantity
 *   How much karma to add (subtract).
 *
 * @return bool
 *   Whether the update was successful or not.
 */
function game_karma(\stdClass $game_user, $text, $quantity) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $sql = 'update users set karma = karma + %d where id = %d;';
  db_query($sql, $quantity, $game_user->id);

  $sql = 'insert into karma (fkey_users_id, text, amount) values
    (%d, "%s", %d);';
  db_query($sql, $game_user->id,
    "$game_user->username gained/lost $quantity karma " .
    "(was: $game_user->karma) for: $text", $quantity);

  // Reprocess user object.
  $game_user = zg_fetch_user_by_id($game_user->id);
  return TRUE;
}

function game_competency_level(\stdClass $game_user, $id) {
  return zg_competency_level($game_user, $id);
}

function game_competency_level_name($id, $req = FALSE) {

  if ($req) {
    $competencies = [
      'No Comp\'cy',
      'Minimal Comp\'cy',
      'Marginal Comp\'cy',
      'Normal Comp\'cy',
      'More Comp\'cy',
      'Masterful Comp\'cy',
    ];
  }
  else {
    $competencies = [
      'Not Comp.',
      'Minimally Comp.',
      'Marginally Comp.',
      'Normally Comp.',
      'More Comp.',
      'Masterfully Comp.',
    ];
  }

  return $competencies[$id];
}

function game_competency_name($comp) {
  $comps = &drupal_static(__FUNCTION__, []);

  if (!count($comps)) {
    $sql = 'select name from competencies;';
    $result = db_query($sql);
    while ($item = db_fetch_object($result)) {
      $comps[$item->name] = $item->name;
    }
  }

  game_alter('comp_names_alter', NULL, $comps);
  return $comps[$comp];
}

/**
 * Send a user a message.
 *
 * @param int $from
 *   From user id.
 * @param int|int[] $to
 *   To user id, or array of uids.  Note: assumes presence of $to[0].
 * @param int|bool $private
 *   1/TRUE if this is a private message, 0/FALSE otherwise.
 * @param string $message
 *   The message.
 * @param string $subtype
 *   Subtype of message.  See db for allowed subtypes.
 *
 * @return bool
 *   Whether the user message succeeded or not (for a single uid),
 *   or whether ALL messages succeeded or not (for an array of uids).
 */
function game_send_user_message($from, $to, $private = 0, $message, $subtype = 'user') {
  $message = nl2br($message);
  if (is_array($to) && is_numeric($to[0])) {
    $result = TRUE;
    foreach ($to as $item) {
      $result &= game_send_user_message($from, $item, $private, $message, $subtype);
    }
    return $result;
  }
  else {
    $sql = 'insert into user_messages (fkey_users_from_id, fkey_users_to_id,
      private, message, subtype) values (%d, %d, %d, "%s", "%s");';
    return db_query($sql, $from, $to, (int) $private, $message, $subtype);
  }
}

/**
 * Send a system message.
 *
 * @param int $from
 *   From user id.
 * @param string $message
 *   The message.
 * @param string $subtype
 *   Subtype of message.  See db for allowed subtypes.
 *
 * @return bool
 *   Whether the message succeeded or not.
 */
function game_send_system_message($from, $message, $subtype = 'system') {
  $sql = 'insert into system_messages (subtype, fkey_users_from_id,
    fkey_users_to_id, message) values ("%s", %d, 0, "%s");';
  return db_query($sql, $subtype, $from, $message);
}

function game_zombie_name() {

  $first = [
    'Angry',
    'Beastly',
    'Creepy',
    'Dreadful',
    'Eerie',
    'Ferocious',
    'Gruesome',
    'Heinous',
    'Infected',
    'Jaundiced',
    'Killer',
  ]; // https://s-media-cache-ak0.pinimg.com/736x/d5/87/a4/d587a4917ff097a6ed2697e5b84dc4c7.jpg

  $last = [
    'Quilter',
    'Rogue',
    'Suck-Up',
    'Twitcher',
    'Undead',
    'Voter',
    'Walker',
    'Xtra Creep',
    'Yard Bump',
    'Zero',
  ];

  return /* 'PoliZombie ' . */ $first[mt_rand(0, count($first) - 1)] . ' '
    . $last[mt_rand(0, count($last) - 1)];

}

/**
 * Can I perform an action yet?
 *
 * @param \stdClass $game_user
 *   The user object.
 * @param string $key
 *   The key to check.
 *
 * @return \stdClass
 *   An object with whether the action can be performed; if FALSE, also the
 *   hours, minutes, and seconds until action can be performed.
 */
function game_can_do_yet($game_user, $key) {
  $can_do = new stdClass();
  $can_do->time_ready = (int) zg_get_value($game_user, $key, REQUEST_TIME);
  $can_do->time_remaining = $can_do->time_ready - REQUEST_TIME;

  if ($can_do->time_remaining > 0) {
    $can_do->allowed = FALSE;
    $can_do->hours_remaining = floor($can_do->time_remaining / 3600);
    $minutes_remaining_in_sec = $can_do->time_remaining % 3600;
    $can_do->minutes_remaining = floor($minutes_remaining_in_sec / 60);
    $can_do->seconds_remaining = floor($minutes_remaining_in_sec % 60);
  }
  else {
    $can_do->allowed = TRUE;
  }

  return $can_do;
}

/**
 * Return the amount of money the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return array
 *   The offer.
 */
function game_luck_money_offer($game_user) {
  return zg_luck_money_offer($game_user);
}

/**
 * Alter the game based on e.g. an event.
 *
 * All parameters after $hook are passed straight to the event's hook
 * function.  Traditionally $game_user is the first parameter passed.
 *
 * @param string $hook
 *   Which hook to call.
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return bool
 *   TRUE if any hooks exist and returned TRUE.  FALSE otherwise.
 */
function game_alter($hook, $game_user, &$garg2 = NULL, &$garg3 = NULL, &$garg4 = NULL) {
  global $game;

  $retval = FALSE;
  $game_include_list = game_includes_get();

  // Find and call hook(s).
  foreach ($game_include_list as $type_include_file) {
    $include_file = substr($type_include_file, strpos($type_include_file, '/') + 1);
    $function_name = 'game_' . $include_file . '_' . $hook;
    if (function_exists($function_name)) {
//      dpm('calling ' . $function_name);
      $retval |= $function_name($game_user, $garg2, $garg3, $garg4);
    }
    else {
//      dpm('function ' . $function_name . ' does not exist!');
    }
  }

  return $retval;
}

/**
 * Add a file to the list of includes.  Run its init hook, if it exists.
 *
 * Will ignore multiple submissions of the same file and any missing files.
 *
 * @param string $include_file
 *   The file to include.
 * @param string $type
 *   The type file inclusion.
 */
function game_includes_add($include_file, $type) {
  global $game;
//dpm ($include_file, 'calling game_includes_add for ' . $type . ' file');
  $game_include_list = &drupal_static('game_include_list', []);
  $game_include_missing = &drupal_static('game_include_missing', []);
  $module_path = drupal_get_path('module', $game);
  $file_to_include = $module_path . '/includes/' . $type . '/' . $include_file . '.inc';

  // Already in include list; return.
  if (in_array($type . '/' . $include_file, $game_include_list)) {
//    dpm($game_include_list, $type . '/' . $include_file .
// ' is already in game include list, not adding again');
    return;
  }

  // Already in missing list; return.
  if (in_array($type . '/' . $include_file, $game_include_missing)) {
//    dpm($game_include_missing, $type . '/' . $include_file .
// ' is already in game include missing list, not adding again');
    return;
  }

  if (file_exists($file_to_include)) {
    $game_include_list[] = $type . '/' . $include_file;
    include_once $file_to_include;
//    dpm('including file ' . $file_to_include);
    $function_name = 'game_' . $include_file . '_init';
    if (function_exists($function_name)) {
//      dpm('calling ' . $function_name);
      $function_name();
    }
  }
  else {
    // File does not exist; add to missing list.
    $game_include_missing[] = $type . '/' . $include_file;
  }
}

/**
 * Return the list of includes.
 *
 * @return string[]
 *   The list of includes.
 */
function game_includes_get() {
  $game_include_list = &drupal_static('game_include_list', []);
  return $game_include_list;
}

/**
 * Show the ai output, if necessary.
 *
 * @param $phone_id
 *   The user's phone ID.
 * @param $ai_output
 *   The output to show.
 */
function game_show_ai_output($phone_id, $ai_output) {
  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"$ai_output\"/>\n-->";
  }
}
