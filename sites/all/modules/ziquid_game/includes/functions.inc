<?php

/**
 * @file
 * Misc functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * .
 */

require_once drupal_get_path('module', 'zg') . '/includes/functions-staff.inc';
//require_once drupal_get_path('module', 'zg') . '/includes/functions-no.inc';
require_once drupal_get_path('module', 'zg') . '/includes/functions-equip.inc';
require_once drupal_get_path('module', 'zg') . '/includes/functions-quests.inc';
require_once drupal_get_path('module', 'zg') . '/includes/functions-land.inc';

/**
 * Show a button.
 *
 * @param string $link
 *   The link for the button.
 * @param string $type
 *   The type of the button.  Used as the text of the button.
 * @param string $extra_link
 *   An extra link, if any, to add to the link.
 * @param string $extra_classes
 *   Extra classes, if any, to add to the button wrapper.
 */
function zg_button($link = 'home', $type = 'continue', $extra_link = '', $extra_classes = '') {
  print zg_render_button($link, $type, $extra_link, $extra_classes);
}

/**
 * Render a button.
 *
 * @param string $link
 *   The link for the button.
 * @param string $type
 *   The type of the button.  Used as the text of the button.
 * @param string $extra_link
 *   An extra link, if any, to add to the link.
 * @param string $extra_classes
 *   Extra classes, if any, to add to the button wrapper.
 *
 * @return string
 *   The rendered button in HTML.
 */
function zg_render_button($link = 'home', $type = 'continue', $extra_link = '', $extra_classes = '') {

  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  $class = drupal_html_class($type);
  if (strlen($link)) {
    if ($link[0] == '/') {
      $link .= '/' . $arg2 . $extra_link;
    }
    else {
      $link = $game . '/' . $link . '/' . $arg2 . $extra_link;
    }
    $button = <<< EOF
<div class="button-wrapper-$class button-wrapper $extra_classes">
  <a class="button-$class button" href="/$link">
    $type
  </a>
</div>
EOF;
  }
  else {
    $button = <<< EOF
<div class="button-wrapper-$class button-wrapper $extra_classes">
  <span class="button-$class button not-yet">
    $type
  </span>
</div>
EOF;
  }
  return $button;
}

/**
 * Tell the client to speak some text.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string $text
 *   The text to speak.
 * @param bool $force
 *   Force the code to instruct the client to speak even if user has not opted
 *   in.
 */
function zg_speech(\stdClass $game_user, $text, $force = FALSE) {
//  global $game;
//  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  if ($force || zg_get_value($game_user, 'GiveMeMyTongue', FALSE)) {
    drupal_add_js([
      'zg' => [
        'speech' => $text,
      ],
    ], 'setting');
  }
}

/**
 * Tell the client to download and play a sound.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string $sound
 *   The sound file's URL.
 * @param bool $force
 *   Force the code to instruct the client to play the sound even if user has
 *   not opted in.
 */
function zg_sound(\stdClass $game_user, $sound, $force = FALSE) {
  if ($force || !zg_get_value($game_user, 'SoundOff', FALSE)) {
    drupal_add_js([
      'zg' => [
        'sound' => $sound,
      ],
    ], 'setting');
  }
}

/**
 * Tell the client to play a song.
 *
 * This function tracks the number of times a particular sound has been played
 * recently and plays shorter versions if the longer versions have been played
 * too much recently.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string $song
 *   The song name.
 * @param bool $force
 *   Force the game client to play the song even if user has not opted in.
 */
function zg_song(\stdClass $game_user, $song, $force = FALSE) {
  $sound_file = 'http://' . $_SERVER['HTTP_HOST'] . '/' . file_directory_path() . '/sounds/' . $song . ' micro.mp3';
  firep($sound_file, 'sound file');
  zg_sound($game_user, $sound_file, $force);
}

/**
 * Recalc the income and expenses for a user.
 *
 * @param \stdClass $game_user
 *   The game user object.
 */
function zg_recalc_income(\stdClass $game_user) {
  $sql = 'update users set income =
    (SELECT sum(land.payout * land_ownership.quantity)
    as income from land
    left join land_ownership
    on land_ownership.fkey_land_id = land.id and
    land_ownership.fkey_users_id = %d)
    where id = %d;';
  db_query($sql, $game_user->id, $game_user->id);

  $sql = 'SELECT sum(equipment.upkeep * equipment_ownership.quantity)
    as expenses from equipment
    left join equipment_ownership
    on equipment_ownership.fkey_equipment_id = equipment.id and
    equipment_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $eq = db_fetch_object($result);

  $sql = 'SELECT sum(staff.upkeep * staff_ownership.quantity)
    as expenses from staff
    left join staff_ownership
    on staff_ownership.fkey_staff_id = staff.id and
    staff_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $st = db_fetch_object($result);

  $sql = 'update users set expenses = %d where id = %d;';
  db_query($sql, $eq->expenses + $st->expenses, $game_user->id);

  $sql = 'select income, expenses from users
    where id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);
  $game_user->income = $item->income;
  $game_user->expenses = $item->expenses;

  zg_alter('recalc_income', $game_user);
}

function zg_show_aides_menu(\stdClass $game_user) {

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
  <a href="/$game/staff/$arg2" class="button $staff_active">{$game_text['staff_tab']}</a>
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

    drupal_add_js(drupal_get_path('module', 'zg')
      . '/js/zg_show_aides_menu_land.js', 'module', 'footer');
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

    drupal_add_js(drupal_get_path('module', 'zg')
      . '/js/zg_show_aides_menu_equip.js', 'module', 'footer');
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

    drupal_add_js(drupal_get_path('module', 'zg')
      . '/js/zg_show_aides_menu_staff.js', 'module', 'footer');
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

function zg_show_elections_menu(\stdClass $game_user) {

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $arg2 = check_plain(arg(2));

  if (substr(arg(1), 0, 7) == 'debates') {
    $debates_active = 'active';
  }
  if (substr(arg(1), 0, 9) == 'elections') {
    $elections_active = 'active';
  }
  if (substr(arg(1), 0, 3) == 'top') {
    $top_active = 'active';
  }
  if (substr(arg(1), 0, 5) == 'top20') {
    $top20_active = 'active';
  }
  if (substr(arg(1), 0, 12) == 'top_alderman') {
    $topalders_active = 'active';
  }

  echo <<< EOF
<div class="news">
<a href="/$game/debates/$arg2" class="button $debates_active">{$debate_tab}</a>
<a href="/$game/elections/$arg2" class="button $elections_active">{$election_tab}</a>
<a href="/$game/top20/$arg2" class="button $top20_active">$top20</a>
<a href="/$game/top_aldermen/$arg2" class="button $topalders_active">Top $alders_short</a>
</div>
EOF;

//  _show_goal($game_user);
}

function zg_show_profile_menu(\stdClass $game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $arg2 = check_plain(arg(2));
  $arg3 = check_plain(arg(3));

  if (arg(1) == 'user') {
    $user_active = 'active';
  }
  if (arg(1) == 'user_favors') {
    $favor_active = 'active';
  }
  if (arg(1) == 'favor_perform') {
    $favor_active = 'active';
    $arg3 = '';
  }
  if (arg(1) == 'user_comp') {
    $comp_active = 'active';
  }

  echo <<< EOF
<div class="news">
  <a href="/$game/user/$arg2/$arg3" class="button $user_active">Profile</a>
  <!--<a href="/$game/user_favors/$arg2/$arg3"
    class="button $favor_active">{$favor}s</a>-->
  <a href="/$game/user_comp/$arg2/$arg3"
    class="button $comp_active">$competencies</a>
</div>
EOF;

//  _show_goal($game_user);
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
function zg_get_icon_path(\stdClass $game_user, \stdClass $obj, $type) {
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
    elseif (file_exists($_SERVER['DOCUMENT_ROOT'] . $icon_path . '.png')) {
      $obj->icon_path = $icon_path . '.png';
    }
    else {
      $obj->icon_path = base_path() . file_directory_path() . '/images/' . $type . '/' . $game . '-w.png';
    }
  }
}

function zg_competency_gain(\stdClass $game_user, $id, $level = 1) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  // If id is string, find numerical id.
  if (gettype($id) == 'string') {

    $sql = 'select * from competencies where name = "%s";';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);

    if (!empty($item)) {
      $id = (int) $item->id;
    }
    else {
      // Not found! Create it!
      $sql = 'insert into competencies (name, level) values ("%s", %d);';
      db_query($sql, $id, $level);

      $sql = 'select * from competencies where name = "%s";';
      $result = db_query($sql, $id);
      $item = db_fetch_object($result);
      $id = (int) $item->id;
    }
  }
  else {
    $sql = 'select * from competencies where id = %d;';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);
  }

  $sql = 'select use_count, timestamp from user_competencies
    where fkey_competencies_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $data = db_fetch_object($result);

  if (zg_timed_bonus_in_effect($game_user, 'fast_comps_30')->allowed) {
    $competency_gain_wait_time = min($competency_gain_wait_time, 30);
  }
  if (zg_timed_bonus_in_effect($game_user, 'fast_comps_15')->allowed) {
    $competency_gain_wait_time = min($competency_gain_wait_time, 15);
  }

  if ((REQUEST_TIME - strtotime($data->timestamp)) < $competency_gain_wait_time) {
firep('Last gain of competency &quot;' . $item->name . '&quot; too recent');
    return FALSE;
  }

  // Already there.
  if (!empty($data)) {

    $comp_before = zg_competency_level($game_user, $id);
    $sql = 'update user_competencies set use_count = use_count + 1
      where fkey_competencies_id = %d and fkey_users_id = %d;';
    db_query($sql, $id, $game_user->id);
    $comp_after = zg_competency_level($game_user, $id);

    // Leveled up!
    if ($comp_after->level > $comp_before->level) {
      zg_send_user_message(1, $game_user->id, 1,
        'You have increased your competency &quot;<span class="initial-caps">'
      . $item->name . '</span>&quot;!&nbsp; You are now '
      . $comp_after->level_name_2 . ' and have gained '
      . $comp_after->level . ' luck.', 'comp');

      $sql = 'update users set luck = luck + %d where id = %d;';
      db_query($sql, $comp_after->level, $game_user->id);
    }

  }

  // New competency.  Tell user about it if above level 6.
  else {
    $sql = 'insert into user_competencies
      (fkey_users_id, fkey_competencies_id, use_count) values
      (%d, %d, 1);';
    db_query($sql, $game_user->id, $id);
    if ($game_user->level > 6) {
      zg_send_user_message(1, $game_user->id, 1,
      'You have started learning a new competency, &quot;<span class="initial-caps">'
      . $item->name . '</span>&quot;!', 'comp');
    }
  }

  return TRUE;
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
function zg_action_use(\stdClass $game_user, $quantity = 1) {
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
  $game_user = $fetch_user();
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
function zg_karma(\stdClass &$game_user, $text, $quantity) {
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
  $game_user = zg_fetch_user();
  return TRUE;
}

/**
 * Update a user's luck.  Log to the appropriate database.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $quantity
 *   The amount of luck to credit (debit).
 * @param int $amount_before
 *   The amount of $type that player had before this action.
 * @param int $amount_filled
 *   The amount of $type that player received for the luck, if any.
 * @param int $amount_now
 *   The amount of $type that player has now, if applicable.
 * @param string $text
 *   Explanatory text.
 * @param string $type
 *   Type of update: purchase, use, system_gift.
 * @param string $subtype
 *   Subtype of $type.
 *
 * @return bool
 *   Whether the update succeeded or not.
 */
function zg_luck(\stdClass &$game_user, $quantity, $amount_before, $amount_filled, $amount_now, $text, $type, $subtype) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $sql = 'update users set luck = luck + %d where id = %d;';
  db_query($sql, $quantity, $game_user->id);

  switch ($type) {

    case 'purchase':
      // FIXME: amount_now really should be amount_before.
      $sql = 'insert into purchases (fkey_users_id, amount_now, amount_purchased, purchase)
        values (%d, %d, %d, "%s");';
      db_query($sql, $game_user->id, $game_user->luck, $quantity, $text);
      mail('joseph@ziquid.com', $game . ' Luck purchase', $text);
      break;

    case 'action':
    case 'change_party':
    case 'energy':
      $sql = 'insert into luck_use (fkey_users_id, use_type, amount_before, 
        amount_filled, amount_now, luck_remaining, comment)
        values (%d, "%s", %d, %d, %d, %d, "%s");';
      db_query($sql, $game_user->id, $type, $amount_before, $amount_filled,
        $amount_now, $game_user->luck + $quantity, $text);
      break;
  }

  zg_slack('luck', $text);

  // Reprocess user object.
  $game_user = zg_fetch_user();
  return TRUE;
}

/**
 * Return the user's competency level [0..5] for a given competency.
 *
 * @param \stdClass $game_user
 *   The game_user object.
 * @param mixed $id
 *   The competency, either as an integer or a string.
 *
 * @return bool|object|\stdClass|null
 *   The competency object.
 */
function zg_competency_level(\stdClass $game_user, $id) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  // Find id from string.
  if (gettype($id) == 'string') {

    $sql = 'select * from competencies where name = "%s";';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);

    if (!empty($item)) {
      $id = $item->id;
    }
    else {
      echo 'competency_level for id ' . $id . ' doesn\'t exist!';
      slack_send_message('competency_level for id ' . $id . 'doesn\'t exist!',
        $slack_channel);
      slack_send_message(print_r($game_user, TRUE), $slack_channel);
      die;
    }

  }
  else {
    $sql = 'select * from competencies where id = %d;';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);
  }

  $sql = 'select use_count from user_competencies
    where fkey_competencies_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $comp = db_fetch_object($result);

  $level = 0;

  while ($level <= 5) {

    $min = zg_competency_min_count($level, $item->level);
    $max = zg_competency_min_count($level + 1, $item->level);

    if ($comp->use_count >= $min && $comp->use_count < $max) {
      $comp->level = $level;
      $comp->min = $min;
      $comp->max = $max;
      $comp->next = $max;

      // Cannot attain next level.
      if ($level == 5) {
        $comp->next = $comp->use_count;
      }

      break;
    }

    $level++;
  }

  $comp->level_name = zg_competency_level_name($comp->level, TRUE);
  $comp->level_name_2 = zg_competency_level_name($comp->level);
  return $comp;
}

/**
 * Show text if certain comp. level has been obtained.
 *
 * @param \stdClass $game_user
 *   The zg_user object.
 * @param string $text
 *   The text to show.
 * @param int $level_obtained
 *   The comp. level obtained.
 * @param int $level_needed
 *   The comp. level needed.
 */
function zg_show_by_level(\stdClass $game_user, $text, $level_obtained, $level_needed = 0) {
  if ($level_obtained >= $level_needed) {
    if ($game_user->meta == 'admin') {
      $level_debug = ' (' . $level_obtained . ':' . $level_needed . ')';
      if (substr($text, -4) === '<br>') {
        $text = str_replace('<br>', $level_debug . '<br>', $text);
      }
      else {
        $text .= $level_debug;
      }
    }
    print $text;
  }
}

function zg_competency_min_count($level, $comp_level) {

  // Return the minimum use count for a given level [0..5] and competency level.
  // [1..3].
  $min_count = [
    1 => [
      0,
      20,
      60,
      150,
      300,
      500,
      PHP_INT_MAX,
    ],
    2 => [
      0,
      10,
      30,
      75,
      150,
      250,
      PHP_INT_MAX,
    ],
    3 => [
      0,
      2,
      6,
      15,
      30,
      50,
      PHP_INT_MAX,
    ],
  ];

  return $min_count[$comp_level][$level];
}

function zg_competency_level_name($id, $req = FALSE) {

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

function zg_competency_name($comp) {
  $comps = &drupal_static(__FUNCTION__, []);

  if (!count($comps)) {
    $sql = 'select name from competencies;';
    $result = db_query($sql);
    while ($item = db_fetch_object($result)) {
      $comps[$item->name] = $item->name;
    }
  }

  zg_alter('comp_names_alter', NULL, $comps);
  return $comps[$comp];
}

/**
 * Retrieve the user object.
 *
 * @param mixed $id
 *   The user ID (int) or the phone ID (string).
 *
 * @return bool|object|null
 *   The user object, if any.
 */
function zg_fetch_user_by_id($id) {

  // Simple per-page cache.
  static $user_cache;

  if (!empty($user_cache[$id])) {
    firep("query for user $id resulted in cached object");
    // Jwc 05Jul2013 -- don't return cached results.
    // Until we can reliably invalidate the cache.
//    return $user_cache[$id];
  }

  // Find id from phone_id.
  if (gettype($id) == 'string') {
    $where = 'WHERE users.phone_id = "%s"';
  }
  else {
    $where = 'WHERE users.id = %d';
  }

  $sql = 'SELECT users.*,
    user_creations.datetime as startdate,
    `values`.party_title, `values`.party_icon,
    elected_positions.name as ep_name,
    elected_positions.id as ep_level,
    elected_officials.approval_rating,
    elected_positions.energy_bonus as ep_energy_bonus,

    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    clans.id as fkey_clans_id,
    event_points.points,

    neighborhoods.name as location

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT JOIN user_creations on user_creations.phone_id = users.phone_id

    LEFT JOIN event_points on event_points.fkey_users_id = users.id

    LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id

    ' . $where . ';';

  $result = db_query($sql, $id);
  $item = db_fetch_object($result);

//   if (empty($item->ep_name)) $item->ep_name = 'Subjugate';
  if (empty($item->ep_level)) {
    $item->ep_level = 0;
  }
  $user_cache[$id] = $item;

firep($item, 'zg_fetch_user_by_id for id ' . $id);
  return $item;
}

/**
 * Get the user ID from the phone ID.
 *
 * @param string $phone_id
 *   The phone ID to check.
 *
 * @return object|null
 *   The user ID, as an 'id' property of an object, or null if no results.
 */
function zg_get_userid_from_phoneid($phone_id) {
  global $game;

  db_set_active('game_' . $game);
  $sql = 'select id from users
    where phone_id = "%s";';
  $result = db_query($sql, $phone_id);
  if ($result) {
    return db_fetch_object($result);
  }
  return NULL;
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
function zg_send_user_message($from, $to, $private, $message, $subtype = 'user') {
  $message = nl2br($message);
  if (is_array($to) && is_numeric($to[0])) {
    $result = TRUE;
    foreach ($to as $item) {
      $result &= zg_send_user_message($from, $item, $private, $message, $subtype);
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
function zg_send_system_message($from, $message, $subtype = 'system') {
  $sql = 'insert into system_messages (subtype, fkey_users_from_id,
    fkey_users_to_id, message) values ("%s", %d, 0, "%s");';
  return db_query($sql, $subtype, $from, $message);
}

/**
 * Generate a zombie  name.
 *
 * @see https://s-media-cache-ak0.pinimg.com/736x/d5/87/a4/d587a4917ff097a6ed2697e5b84dc4c7.jpg
 *
 * @return string
 *   The zombie name.
 */
function zg_zombie_name() {

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
  ];

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

  return 'PoliZombie ' . $first[mt_rand(0, count($first) - 1)] . ' '
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
function zg_can_do_yet(\stdClass $game_user, $key) {
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
 * Timed bonus still in effect?
 *
 * Logically the opposite of zg_can_do_yet().
 *
 * @param \stdClass $game_user
 *   The user object.
 * @param string $bonus_key
 *   The key to check.
 *
 * @return \stdClass
 *   An object with whether a bonus is in effect and the hours, minutes, and
 *   seconds remaining, or FALSE.
 */
function zg_timed_bonus_in_effect(\stdClass $game_user, $bonus_key) {
  $in_effect = new stdClass();
  $in_effect->time_ready = (int) zg_get_value($game_user, $bonus_key, REQUEST_TIME);
  $in_effect->time_remaining = $in_effect->time_ready - REQUEST_TIME;

  if ($in_effect->time_remaining > 0) {
    $in_effect->allowed = TRUE;
    $in_effect->hours_remaining = floor($in_effect->time_remaining / 3600);
    $minutes_remaining_in_sec = $in_effect->time_remaining % 3600;
    $in_effect->minutes_remaining = floor($minutes_remaining_in_sec / 60);
    $in_effect->seconds_remaining = floor($minutes_remaining_in_sec % 60);
  }
  else {
    $in_effect->allowed = FALSE;
  }

  return $in_effect;
}

/**
 * Set timer for user (either for bonus or disallowed action).
 *
 * @param \stdClass $game_user
 *   The user object.
 * @param string $timer_key
 *   The key to set.
 * @param int $time_secs
 *   The number of seconds to set for the timer.
 */
function zg_set_timer(\stdClass $game_user, $timer_key, $time_secs) {
  zg_set_value($game_user, $timer_key, REQUEST_TIME + $time_secs);
}

/**
 * Return the value for the key in the db if found, or $default if not found.
 *
 * @param \stdClass $game_user
 *   The zg_user object.
 * @param string $key
 *   The key to use for the query.
 * @param mixed $default
 *   The result to return if KVP is not found.
 *
 * @return mixed
 *   The value, if found, or $default, if not found.
 */
function zg_get_value(\stdClass $game_user, $key, $default = NULL) {

  $sql = 'SELECT `value` FROM {user_attributes}
    WHERE `fkey_users_id` = %d AND `key` = "%s";';
  $item = db_query($sql, (int) $game_user->id, $key)->fetch_object();

  // Serialized data?  Undo it.
  if (substr($item->value, 0, 11) == 'serialized:') {
    return unserialize(substr($item->value, 11));
  }

  return !empty($item->value) ? $item->value : $default;
}

/**
 * Set a value for a certain key for the game user.
 *
 * Updates the value if the KVP already exists.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string $key
 *   The key to use for the value in the db.
 * @param string $value
 *   The value to set in the db.
 *
 * @return bool
 *   TRUE if the set succeeded; FALSE otherwise.
 */
function zg_set_value(\stdClass $game_user, $key, $value = 'set') {

  // Array?  Serialize the data before storing.
  if (is_array($value)) {
    $value = 'serialized:' . serialize($value);
  }
  $sql = 'INSERT INTO {user_attributes}
    (`fkey_users_id`, `key`, `value`) VALUES (%d, "%s", "%s")
    ON DUPLICATE KEY UPDATE `value` = "%s", last_update = CURRENT_TIMESTAMP;';
  return db_query($sql, $game_user->id, $key, $value, $value);
}

/**
 * Remove a KVP for a user.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string $key
 *   The key to remove.
 *
 * @return bool
 *   Whether the removal succeeded or not.
 */
function zg_remove_value(\stdClass $game_user, $key) {
  $sql = 'delete from {user_attributes}
    where `fkey_users_id` = %d and `key` = "%s";';
  return db_query($sql, $game_user->id, $key);
}

/**
 * Return the normal amount of money the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return int
 *   The offer.
 */
function zg_normal_luck_money_offer(\stdClass $game_user) {
  $offer = ($game_user->income - $game_user->expenses) * 5;
  $offer = min($offer, $game_user->level * 1000);
  $offer = max($offer, $game_user->level * 100);
  return $offer;
}

/**
 * Render a button allowing purchase of money for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return string
 *   The button, as a string of HTML.
 */
function zg_luck_money_render_button(\stdClass $game_user) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $normal_offer = zg_normal_luck_money_offer($game_user);
  $offer = zg_luck_money_offer($game_user);

  if ($offer == $normal_offer) {
    $type = "Receive $offer $game_user->values (1&nbsp;$luck)";
  }
  else {
    $type = "Receive <span class='highlight'>$offer</span> $game_user->values (1&nbsp;$luck)";
  }
  return zg_render_button('elders_do_fill',
    $type,
    '/money',
    'big-80 slide-in-content');
}

/**
 * Return the amount of money the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return int
 *   The offer.
 */
function zg_luck_money_offer(\stdClass $game_user) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $offer = zg_normal_luck_money_offer($game_user);

  if ($event_type == EVENT_DOUBLE_LUCK_MONEY) {
    $offer *= 2;
  }

  $comment = '';
  zg_alter('luck_money_offer', $game_user, $offer, $comment);

  return $offer;
}

/**
 * Return the amount of energy the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return array
 *   The offer as an integer and explanation text, if any.
 */
function zg_luck_energy_offer(\stdClass $game_user) {
  $offer = $game_user->energy_max;
  $comment = '';
  zg_alter('luck_energy_offer', $game_user, $offer, $comment);
  return [$offer, $comment];
}

/**
 * Alter the game based on e.g. an event.
 *
 * All parameters after $hook are passed straight to the event's hook
 * function.  Traditionally $game_user is the first parameter passed.
 *
 * @param string $hook
 *   Which hook to call.
 * @param \stdClass|null $game_user
 *   The game's user object.
 *
 * @return bool
 *   TRUE if any hooks exist and returned TRUE.  FALSE otherwise.
 */
function zg_alter($hook, $game_user, &$garg2 = NULL, &$garg3 = NULL, &$garg4 = NULL) {
  global $game;

  $retval = FALSE;
  $game_include_list = zg_includes_get();

  // Find and call hook(s).
  foreach ($game_include_list as $type_include_file) {
    $include_file = substr($type_include_file, strpos($type_include_file, '/') + 1);
    $function_name = $game . '_' . $include_file . '_' . $hook;
    if (function_exists($function_name)) {
      $retval |= $function_name($game_user, $garg2, $garg3, $garg4);
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
 *   The type of file to be included.
 */
function zg_includes_add($include_file, $type) {
  global $game;
  $game_include_list = &drupal_static('zg_include_list', []);
  $game_include_missing = &drupal_static('zg_include_missing', []);
  $module_path = drupal_get_path('module', 'zg');
  $file_to_include = $module_path . '/includes/' . $type . '/' . $game . '_' . $include_file . '.inc';

  // Already in include list; return.
  if (in_array($type . '/' . $include_file, $game_include_list)) {
    return;
  }

  // Already in missing list; return.
  if (in_array($type . '/' . $include_file, $game_include_missing)) {
    return;
  }

  if (file_exists($file_to_include)) {
    $game_include_list[] = $type . '/' . $include_file;
    include_once $file_to_include;
    $function_name = $game . '_' . $include_file . '_init';
    if (function_exists($function_name)) {
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
function zg_includes_get() {
  $game_include_list = &drupal_static('zg_include_list', []);
  return $game_include_list;
}

/**
 * Show the ai output, if necessary.
 *
 * @param string $phone_id
 *   The user's phone ID.
 * @param string $ai_output
 *   The output to show.
 */
function zg_show_ai_output($phone_id, $ai_output) {
  if (substr($phone_id, 0, 3) == 'ai-') {
    echo "<!--\n<ai \"$ai_output\"/>\n-->";
  }
}

/**
 * Check to see if the user has a particular trait.
 *
 * Admin users automatically have all traits.
 *
 * @param \stdClass $game_user
 *   The zg_user object.
 * @param string $trait
 *   The trait for which to check.
 *
 * @return bool
 *   Whether the user has that trait or not.
 */
function zg_user_has_trait(\stdClass $game_user, $trait) {
  if ($game_user->meta == 'admin' ||
    (array_key_exists($trait, $game_user->traits) &&
    $game_user->traits[$trait])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * How many actions necessary to move to a new hood?
 *
 * Also returns hood objects for from and to hood and verb used.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $from_id
 *   The id of the hood moving from.
 * @param int $to_id
 *   The id of the hood moving to.
 *
 * @return array
 *   Returns the from hood object, the to hood object, the number of actions to
 *   move, and the verb used to display the move.
 */
function zg_get_actions_to_move(\stdClass $game_user, $from_id, $to_id) {
  $sql = 'select * from neighborhoods where id = %d;';
  $result = db_query($sql, $from_id);
  $from_hood = db_fetch_object($result);
  $result = db_query($sql, $to_id);
  $to_hood = db_fetch_object($result);

  $distance = floor(sqrt(pow($from_hood->xcoor - $to_hood->xcoor, 2) +
    pow($from_hood->ycoor - $to_hood->ycoor, 2)));
  $actions_to_move = floor($distance / 8);
  $verb = t('Walk');

  // Find player's fastest equipment.
  $sql = 'SELECT equipment.speed_increase as speed_increase,
    action_verb from equipment

    left join equipment_ownership
      on equipment_ownership.fkey_equipment_id = equipment.id
      and equipment_ownership.fkey_users_id = %d

    where equipment_ownership.quantity > 0

    order by equipment.speed_increase DESC limit 1;';
  $result = db_query($sql, $game_user->id);
  $eq = db_fetch_object($result);

  if (is_object($eq) && $eq->speed_increase > 0) {
    $verb = $eq->action_verb;
    zg_alter('speed_increase', $game_user, $eq->speed_increase, $verb);
    $actions_to_move -= $eq->speed_increase;
  }

  $actions_to_move = max($actions_to_move, 6);
  zg_alter('actions_to_move', $game_user, $actions_to_move);
  return [$from_hood, $to_hood, $actions_to_move, $verb];
}

/**
 * Get the list of supported game machine names.
 *
 * @return string[]
 *   The list of game machine names.
 */
function zg_get_game_list() {
  return [
//    'detroit',
    'stlouis',
//    'wonderland',
  ];
}

function zg_get_fbid() {

  include drupal_get_path('module', 'zg') . '/fb/facebook.php';

  // Facebook configuration.
  if (arg(0) == 'stlouis') {
    $app_id = '234564386596716';
    $secret = '0b631d36aaba590fe31b2f1b9392aad7';
    $canvas_page = 'https://apps.facebook.com/uprisingstlouis/';
  }
  else if (arg(0) == 'celestial_glory') {
    $app_id = '315942001775829';
    $secret = '648a1945d57bdad144677f5cfd045f62';
    $canvas_page = 'https://apps.facebook.com/celestial_glory/';
  }
  else {
    echo 'uhoh!  which game am i?'; exit;
  }

  // First, try normal facebook getUser().  If that works, awesome.
  $facebook = new Facebook([
    'appId'  => $app_id,
    'secret' => $secret,
    'cookie' => TRUE,
  ]);

  //  echo '<pre>'; print_r($_SERVER); echo '</pre>';
  $signed_request = $_REQUEST['signed_request'];
  //  echo '<pre>'; print_r($signed_request); echo '</pre>';

  // Get User ID.
  $user = $facebook->getUser();
  if ($user != '0') {
    return 'fb=' . $user;
  }

  // GetUser() didn't work.  Try oAuth.  Maybe user needs to log in or
  // authorize the game?
  $auth_url = 'http://www.facebook.com/dialog/oauth?client_id='
    . $app_id . '&redirect_uri=' . urlencode($canvas_page);

  list($encoded_sig, $payload) = explode('.', $signed_request, 2);
  $data = json_decode(base64_decode(strtr($payload, '-_', '+/')), TRUE);

  if (empty($data["user_id"])) {
    echo '<a target="_top" href="' . $auth_url . '">Login to Facebook</a>';
    exit;
    //    echo("<script> top.location.href='" . $auth_url . "'</script>");
  }
  else {
    //    echo ("Welcome User: " . $data["user_id"]);
    return 'fb=' . $data['user_id'];
  }
}

/**
 * Get a bit of HTML from the html well.
 *
 * If the key is an array, assume it is an array of keys.  Return the values
 * of those keys.
 *
 * @param string|string[] $key
 *   The key(s) of the KVP pair(s).
 *
 * @return string|string[]|bool|bool[]
 *   The resultant HTML, or FALSE if not found, or array of the same.
 */
function zg_get_html($key) {
  global $game;

  $arg2 = check_plain(arg(2));

  if (is_array($key)) {
    $values = [];
    foreach ($key as $k) {
      $values[$k] = zg_get_html($k);
    }
    return $values;
  }

  $sql = 'select html_value from html_well where html_key = "%s";';
  $result = db_query($sql, $key);
  $item = db_fetch_object($result);
  if (is_object($item)) {
    return str_replace(
      ['%arg2', '%game'],
      [$arg2, $game],
      $item->html_value
    );
  }

  // Wonderland or Detroit and value doesn't exist?  Create dummy value.
  if ($game == 'wonderland' || $game == 'detroit') {
    $sql = 'insert into html_well
      (html_key, html_value, comment)
      values ("%s", "%s", "FIXME -- automatically-generated value");';
    db_query($sql, $key, "$game $key");
    return "$game $key";
  }

  return FALSE;
}

/**
 * Get a default value for the current game.
 *
 * If the key is an array, assume it is an array of keys.  Return the values
 * of those keys.
 *
 * @param string|string[] $key
 *   The key(s) of the KVP pair(s).
 *
 * @return mixed
 *   The resultant value, or FALSE if not found, or array of the same.
 */
function zg_get_default($key) {
  global $game;

  if (is_array($key)) {
    $values = [];
    foreach ($key as $k) {
      $values[$k] = zg_get_default($k);
    }
    return $values;
  }

  $game_defaults = &drupal_static(__FUNCTION__, []);
  if (array_key_exists($key, $game_defaults)) {
    return $game_defaults[$key];
  }

  $sql = 'select game_value, value_type from game_defaults where game_key = "%s";';
  $result = db_query($sql, $key);
  $item = db_fetch_object($result);

  if (is_object($item)) {
    switch ($item->value_type) {
      case 'int':
        $game_defaults[$key] = (int) $item->game_value;
        break;

      case 'text':
        $game_defaults[$key] = $item->game_value;
        break;

      case 'serialized':
        $game_defaults[$key] = unserialize($item->game_value);
        break;

      case 'float':
        $game_defaults[$key] = (float) $item->game_value;
        break;

      case 'bool':
        if ((drupal_strtolower($item->game_value) == 'false') ||
          (drupal_strtolower($item->game_value) == 'f')) {
          $game_defaults[$key] = FALSE;
          break;
        }
        else {
          $game_defaults[$key] = (bool) $item->game_value;
        }
    }
    return $game_defaults[$key];
  }

  // Wonderland or Detroit and value doesn't exist?  Copy value from St Louis.
  if ($game == 'wonderland' || $game == 'detroit') {
    db_set_active('game_stlouis');
    $sql = 'select game_value, value_type from game_defaults where game_key = "%s";';
    $result = db_query($sql, $key);
    $item = db_fetch_object($result);
    db_set_active('game_' . $game);

    if (is_object($item)) {
      $sql = 'insert into game_defaults
        (game_key, game_value, value_type, comment)
        values ("%s", "%s", "%s", "FIXME -- automatic import from St. Louis");';
      db_query($sql, $key, $item->game_value, $item->value_type);

      switch ($item->value_type) {
        case 'int':
          $game_defaults[$key] = (int) $item->game_value;
          break;

        case 'text':
          $game_defaults[$key] = $item->game_value;
          break;

        case 'serialized':
          $game_defaults[$key] = unserialize($item->game_value);
          break;

        case 'float':
          $game_defaults[$key] = (float) $item->game_value;
          break;

        case 'bool':
          if ((drupal_strtolower($item->game_value) == 'false') ||
            (drupal_strtolower($item->game_value) == 'f')) {
            $game_defaults[$key] = FALSE;
            break;
          }
          else {
            $game_defaults[$key] = (bool) $item->game_value;
          }
      }
      return $game_defaults[$key];
    }
  }

  return FALSE;
}

// Backported from Drupal 7.
if (!function_exists('drupal_clean_css_identifier')) {

  function drupal_clean_css_identifier($identifier, $filter = [
    ' ' => '-',
    '_' => '-',
    '/' => '-',
    '[' => '-',
    ']' => '',
  ]) {

    // Use the advanced drupal_static() pattern, since this is called veryoften.
    static $drupal_static_fast;
    if (!isset($drupal_static_fast)) {
      $drupal_static_fast['allow_css_double_underscores'] =&
        drupal_static(__FUNCTION__ . ':allow_css_double_underscores');
    }
    $allow_css_double_underscores =& $drupal_static_fast['allow_css_double_underscores'];
    if (!isset($allow_css_double_underscores)) {
      $allow_css_double_underscores = variable_get('allow_css_double_underscores', FALSE);
    }

    // Preserve BEM-style double-underscores depending on custom setting.
    if ($allow_css_double_underscores) {
      $filter['__'] = '__';
    }

    // By default, we filter using Drupal's coding standards.
    $identifier = strtr($identifier, $filter);

    // Valid characters in a CSS identifier are:
    // - the hyphen (U+002D)
    // - a-z (U+0030 - U+0039)
    // - A-Z (U+0041 - U+005A)
    // - the underscore (U+005F)
    // - 0-9 (U+0061 - U+007A)
    // - ISO 10646 characters U+00A1 and higher
    // We strip out any character not in the above list.
    $identifier = preg_replace(
      '/[^\\x{002D}\\x{0030}-\\x{0039}\\x{0041}-\\x{005A}\\x{005F}' .
      '\\x{0061}-\\x{007A}\\x{00A1}-\\x{FFFF}]/u',
      '', $identifier);
    return $identifier;
  }

}

// Backported from Drupal 7.
if (!function_exists('drupal_html_class')) {

  function drupal_html_class($class) {

    // The output of this function will never change, so this uses a normal
    // static instead of drupal_static().
    static $classes = [];
    if (!isset($classes[$class])) {
      $classes[$class] = drupal_clean_css_identifier(drupal_strtolower($class));
    }
    return $classes[$class];
  }

}

/**
 * Send message to Slack.
 *
 * @param string $type
 *   The type of the message.
 * @param string $message
 *   The message to post into a channel.
 *
 * @return bool|object
 */
function zg_slack($type, $message) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $slack_message = $type . ': ' . $message;

  switch (mb_strtolower($type)) {

    case 'bot':
    case 'bots':
      $slack_channel .= '-bots';
      break;
  }

  return slack_send_message($slack_message, $slack_channel);
}

/**
 * Fetch all messages for a user.
 *
 * @param object $game_user
 *   The game_user object.
 *
 * @return array
 *   The messages.
 */
function zg_get_all_messages($game_user) {

  global $game;

  // Are we a type 2 elected official?
  $sql = 'SELECT type FROM elected_officials
  left join elected_positions on elected_positions.id = fkey_elected_positions_id
  WHERE fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);
  $elected_official_type = $item->type;

  // If a party official.
  if ($elected_official_type == 2) {

    // Grab the list of all clans in the user's party.
    // We need to do this separately to keep the db from locking.
    // Wish mysql had a select with nolock feature - JWC.
    $data = [];
    $sql = 'SELECT fkey_clans_id FROM clan_members
    left join users on fkey_users_id = users.id
    WHERE fkey_values_id = %d
    and is_clan_leader = 1;';
    $result = db_query($sql, $game_user->fkey_values_id);
    while ($item = db_fetch_object($result)) {
      $data[] = $item->fkey_clans_id;
    }

    $clan_sql = 'where clan_messages.fkey_neighborhoods_id in (%s)';
    $clan_id_to_use = implode(',', $data);
    //firep($clan_id_to_use);
    $limit = 50;
    $all_limit = 150;
  }
  else {
    $clan_sql = 'where clan_messages.fkey_neighborhoods_id = %d';
    $clan_id_to_use = $game_user->fkey_clans_id;
    $limit = 20;
    $all_limit = 100;
  }

  $sql = '
  (
  select user_messages.timestamp, user_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  user_messages.private, user_messages.id as msg_id,
  "user" as type,
  subtype
  from user_messages
  left join users on user_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  user_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where fkey_users_to_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select challenge_messages.timestamp, challenge_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, challenge_messages.id as msg_id,
  "challenge" as type,
  "" as subtype
  from challenge_messages
  left join users on challenge_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  challenge_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where fkey_users_to_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select neighborhood_messages.timestamp, neighborhood_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, neighborhood_messages.id as msg_id,
  "hood" as type,
  "" as subtype
  from neighborhood_messages
  left join users on neighborhood_messages.fkey_users_from_id =
    users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  neighborhood_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where neighborhood_messages.fkey_neighborhoods_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select clan_messages.timestamp, clan_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, clan_messages.id as msg_id,
  "clan" as type,
  "" as subtype
  from clan_messages
  left join users on clan_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    clan_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  ' . $clan_sql . '
  order by timestamp DESC limit %d
  )

  union

  (
  select values_messages.timestamp, values_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, values_messages.id as msg_id,
  "values" as type,
  "" as subtype
  from values_messages
  left join users on values_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    values_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where values_messages.fkey_values_id = %d
--    AND values_messages.fkey_neighborhoods_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select system_messages.timestamp, system_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
--  NULL AS username, NULL as phone_id,
--  NULL AS ep_name,
--  0 AS is_clan_leader,
--  NULL AS clan_acronym,
  0 AS private, system_messages.id as msg_id,
  "mayor" as type,
  subtype
  from system_messages
  left join users on system_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    system_messages.fkey_users_from_id
    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  order by timestamp DESC limit %d
  )

  order by timestamp DESC limit %d;';
  //firep($sql, 'sql for homepage');

  // Don't show if load avg too high.
  // FIXME: get load avg of db server.
  //  $load_avg = sys_getloadavg();
  $data = [];

  if (TRUE/*$load_avg[0] <= 2.0*/) {
    // Expensive query - goes to slave.
    //   db_set_active('game_' . $game . '_slave1');
    $result = db_query($sql, $game_user->id, $limit,
      $game_user->id, 10, // challenge limit of 10
      $game_user->fkey_neighborhoods_id, $limit,
      $clan_id_to_use, $limit,
      $game_user->fkey_values_id, $game_user->fkey_neighborhoods_id, $limit,
      $limit, $all_limit);
    while ($item = db_fetch_object($result)) {
      $data[] = $item;
    }

    // Reset to master.
    db_set_active('game_' . $game);
  }
  return $data;
}

/**
 * Fetch all new user messages for a user.
 *
 * Retrieves all messages above a given message ID.
 *
 * @param object $game_user
 *   The game_user object.
 * @param int $msg_id
 *   The message ID of messages already received.
 *
 * @return array
 *   The messages.
 */
function zg_get_new_user_messages($game_user, $msg_id = 0) {

  $sql = '
  select user_messages.timestamp, user_messages.message,
  users.username, users.phone_id,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  user_messages.private, user_messages.id as msg_id,
  "user" as type,
  subtype
  from user_messages
  left join users on user_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  user_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  where fkey_users_to_id = %d AND user_messages.id > %d
  order by timestamp DESC;';
  $data = [];

  $result = db_query($sql, $game_user->id, $msg_id);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  return $data;
}

/**
 * Fetch the new user messages count for a user.
 *
 * Retrieves the number of messages above a given message ID.
 *
 * @param object $game_user
 *   The game_user object.
 * @param int $msg_id
 *   The message ID of messages already received.
 *
 * @return int
 *   The message count.
 */
function zg_get_new_user_message_count($game_user, $msg_id = 0) {

  $sql = '
  select count(user_messages.id) as count
  from user_messages
  where fkey_users_to_id = %d AND user_messages.id > %d;';

  $result = db_query($sql, $game_user->id, $msg_id)->fetch_object();
  return (int) $result->count;
}

/**
 * Format items in a message for display.
 *
 * @param object $game_user
 *   The game user object.
 * @param object $message
 *   The message object.
 */
function zg_format_message($game_user, &$message) {

  $message->display = new \stdClass();
  $message->display->timestamp = zg_format_date(strtotime($message->timestamp));
  $message->display->msg_id = 'news-item-' . $message->type . '-' . $message->msg_id;

  $message->display->clan_acronym = '';
  if (!empty($message->clan_acronym)) {
    $message->display->clan_acronym = "($message->clan_acronym)";
  }

  if ($message->is_clan_leader) {
    $message->display->clan_acronym .= '*';
  }

  if ($message->private) {
    $message->display->private_css = 'private';
    $message->display->private_text = '(private)';
  }
  else {
    $message->display->private_css = $message->display->private_text = '';
  }

  $message->display->private_css .= ' ' . $message->type . ' ' . $message->type . '-' . $message->subtype;

  if (empty($message->username)) {
    $message->display->username = '';
    $message->display->reply = '';
  }
  else {
    $message->display->username = 'from ' . $message->ep_name . ' ' . $message->username . ' ' .
      $message->display->clan_acronym;
    if (!in_array($message->username, ['USLCE Game', 'The Socialite'])) {
      $message->display->reply = zg_render_button('user', 'View / Respond', '/' .
        $message->phone_id);
    }
    else {
      $message->display->reply = '';
    }
  }
}
