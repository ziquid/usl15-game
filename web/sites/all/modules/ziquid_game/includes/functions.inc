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

$zg_path = drupal_get_path('module', 'zg');
require_once $zg_path . '/includes/functions-ai.inc';
require_once $zg_path . '/includes/functions-equip.inc';
require_once $zg_path . '/includes/functions-land.inc';
require_once $zg_path . '/includes/functions-quests.inc';
require_once $zg_path . '/includes/functions-staff.inc';

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
function zg_render_button($link = 'home', $type = 'continue', $extra_link = '',
  $extra_classes = '') {

  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  $class = drupal_html_class($type);
  if (strlen($link)) {
    if ($link[0] == '/') {
      $link .= '/' . $arg2 . $extra_link;
    }
    else {
      $link = '/' . $game . '/' . $link . '/' . $arg2 . $extra_link;
    }
    $button = <<< EOF
<div class="button-wrapper-$class button-wrapper $extra_classes">
  <a class="button-$class button" href="$link">
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
 * @param \stdClass|null $game_user
 *   The game user object.
 * @param string $sound
 *   The sound file's URL.
 * @param bool $force
 *   Force the code to instruct the client to play the sound even if user has
 *   not opted in.
 * @param string $number_of_loops
 *   The number of times to loop back to play the sound again.  Default is 0
 *   (play once).
 */
function zg_sound($game_user, $sound, $force = FALSE, $number_of_loops = '0') {
  if ($force || !zg_get_value($game_user, 'SoundOff', FALSE)) {
    if (substr($sound, 0, 4) != 'http') {
      $sound = $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['HTTP_HOST'] . '/' . file_directory_path() .
        '/sounds/' . $sound;
    }
    drupal_add_js([
      'zg' => [
        'sound' => $sound,
        'numberOfLoops' => $number_of_loops . '',
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
 * @param \stdClass|null $game_user
 *   The game user object.
 * @param string $song
 *   The song name.
 * @param bool $force
 *   Force the game client to play the song even if user has not opted in.
 */
function zg_song($game_user, $song, $force = FALSE) {
  $sound_file = $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['HTTP_HOST'] . '/' . file_directory_path() .
    '/sounds/' . $song . ' micro.mp3';
  firep($sound_file, 'sound file');
  zg_sound($game_user, $sound_file, $force);
}

/**
 * Tell the client to play a loop.
 *
 * @param \stdClass|null $game_user
 *   The game user object.
 * @param string $loop
 *   The loop name.
 * @param bool $force
 *   Force the game client to play the loop even if user has not opted in.
 */
function zg_loop($game_user, $loop, $force = FALSE) {
  $sound_file = $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['HTTP_HOST'] . '/' . file_directory_path() .
    '/sounds/' . $loop . ' loop.mp3';
  firep($sound_file, 'loop file');
  zg_sound($game_user, $sound_file, $force, '9');
}

/**
 * Recalc the income and expenses for a user.
 *
 * @param \stdClass $game_user
 *   The game user object.
 */
function zg_recalc_income(\stdClass &$game_user) {
  $sql = 'update users set income =
    coalesce((SELECT sum(land.payout * land_ownership.quantity)
    as income from land
    left join land_ownership
    on land_ownership.fkey_land_id = land.id and
    land_ownership.fkey_users_id = %d), 0)
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

  $sql = 'select income, expenses, money from users
    where id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);
  $game_user->income = $item->income;
  $game_user->expenses = $item->expenses;
  $game_user->money = $item->money;

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
  $arg1 = check_plain(arg(1));
  $arg2 = check_plain(arg(2));
  $arg3 = check_plain(arg(3));

  if ($arg1 == 'user') {
    $user_active = 'active';
  }
  if ($arg1 == 'user_favors') {
    $favor_active = 'active';
  }
  if ($arg1 == 'favor_perform') {
    $favor_active = 'active';
    $arg3 = '';
  }
  if ($arg1 == 'user_comp') {
    $comp_active = 'active';
  }

  // FIXME.
  $favor = 'Favor';

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
 * Find the icon path for a given type.  Saves it as the property 'icon_path'.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $obj
 *   The quest, etc. object.
 * @param string $type
 *   The type of object.
 */
function zg_get_icon_path(\stdClass $game_user, \stdClass $obj, string $type): void {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  if (property_exists($obj, 'meta') &&
    strpos($obj->meta, 'use_job_picture') !== FALSE) {
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

/**
 * Increase a user's competency point.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param string|int $id
 *   The competency name (string) or id (int).
 *
 * @return bool
 *   Whether the competency was increased or not.
 */
function zg_competency_gain(\stdClass $game_user, $id) {
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
      db_query($sql, $id, 1);
      $name = $id;

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

  zg_alter('competency_gain_wait', $game_user, $competency_gain_wait_time,
    $competency_gain_wait_time_str);

  if (zg_timed_bonus_in_effect($game_user, 'fast_comps_30')->allowed) {
    $competency_gain_wait_time = min($competency_gain_wait_time, 30);
  }
  if (zg_timed_bonus_in_effect($game_user, 'fast_comps_15')->allowed) {
    $competency_gain_wait_time = min($competency_gain_wait_time, 15);
  }

  if ((REQUEST_TIME - strtotime($data->timestamp)) < $competency_gain_wait_time) {
//    zg_slack($game_user, 'competency-gain', $item->name,
//     'Player "' . $game_user->username .
//      '" tried to gain comp ' . $id . ', "' . $item->name . '" but must wait ' .
//      (REQUEST_TIME - strtotime($data->timestamp)) . ' more seconds.');
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

    // Log if not already masterfully competent.
    if ($comp_after->level < 5) {
      zg_slack($game_user, 'competency-gain', $item->name,
        'Player "' . $game_user->username .
        '" gained for comp ' . $id . ', "' . $item->name . '" (now ' .
        $comp_after->use_count . ', "' . $comp_after->level_name_2 . '").');
    }

    // Leveled up?
    if ($comp_after->level > $comp_before->level) {
      zg_send_user_message(1, $game_user->id, 1,
        'You have increased your competency &quot;<span class="initial-caps">'
      . $item->name . '</span>&quot;!&nbsp; You are now '
      . $comp_after->level_name_2 . ' and have gained '
      . $comp_after->level . ' luck.', 'comp');

      $luck = $comp_after->level;
      $msg = 'Player "' . $game_user->username . '" gained ' . $luck .
        ' Luck via comp gain (previously ' . $game_user->luck .
        ' Luck, now ' . ($game_user->luck + $luck) . ') for becoming "' .
        $comp_after->level_name_2 . '" in comp ' .
        $id . ': "' . $item->name . '".';
      zg_luck($game_user, $luck, $game_user->luck, $luck,
        $game_user->luck + $luck, $msg, 'competency',
        $item->name);
      zg_slack($game_user, 'competency-gain', $item->name,
        'Player "' . $game_user->username .
        '" has attained comp level ' . $comp_after->level . ', "' .
        $comp_after->level_name_2 . '" for comp ' . $id . ', "' .
        $item->name . '".');
    }

  }

  // New competency.  Tell user about it if above level 6.
  else {
    $sql = 'insert into user_competencies
      (fkey_users_id, fkey_competencies_id, use_count) values
      (%d, %d, 1);';
    db_query($sql, $game_user->id, $id);
    zg_slack($game_user, 'competency-gain', $item->name,
      'Player "' . $game_user->username .
      '" has newly started learning comp ' . $id . ', "' . $item->name . '".');

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
  $game_user = zg_fetch_player_by_id($game_user->id);
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
  $game_user = zg_fetch_player_by_id($game_user->id);
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
function zg_luck(\stdClass &$game_user, $quantity, $amount_before, $amount_filled,
  $amount_now, $text, $type, $subtype) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $sql = 'update users set luck = luck + %d where id = %d;';
  db_query($sql, $quantity, $game_user->id);

  switch ($type) {

    case 'competency':
    case 'purchase':
      // FIXME: amount_now is really the amount before purchase.
      // The column name in the database should be changed.
      $sql = 'insert into purchases (fkey_users_id, type, subtype, amount_now, amount_purchased, purchase)
        values (%d, "%s", "%s", %d, %d, "%s");';
      db_query($sql, $game_user->id, $type, $subtype, $game_user->luck, $quantity, $text);
      break;

    case 'action':
    case 'change_party':
    case 'energy':
    case 'money':
      $sql = 'insert into luck_use (fkey_users_id, use_type, subtype, 
        amount_before, amount_filled, amount_now, luck_remaining, comment)
        values (%d, "%s", "%s", %d, %d, %d, %d, "%s");';
      db_query($sql, $game_user->id, $type, $subytpe, $amount_before,
        $amount_filled, $amount_now, $game_user->luck + $quantity, $text);
      break;
  }

  $slack_subtype = $type;

  if (in_array($slack_subtype, ['action', 'energy', 'money'])) {
    $slack_subtype .= '_refill';
  }

  zg_slack($game_user, 'luck', $slack_subtype, $text);

  // Reprocess user object.
  $game_user = zg_fetch_player_by_id($game_user->id);
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
      zg_slack($game_user, 'error', 'competency-missing',
        'competency_level for id ' . $id . 'doesn\'t exist!');
      zg_slack($game_user, 'error', 'game user', $game_user);
      die;
    }

  }
  else {
    $sql = 'select * from competencies where id = %d;';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);
  }

  $sql = 'select coalesce((select use_count from user_competencies
    where fkey_competencies_id = %d and fkey_users_id = %d), 0) as use_count;';
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
    if (FALSE && $game_user->meta == 'admin') {
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
 * Fetch a player object by phone ID or user ID.
 *
 * @param string|int $id
 *   The user ID or phone ID of the object.
 *
 * @return object|null
 *   The user object, if any.
 */
function zg_fetch_player_by_id($id) {
  $pl = zg_fetch_players_by_ids([$id]);
  if (is_array($pl) && count($pl)) {
//    zg_slack($pl[0], 'debug', 'game user', $pl[0]);
    return $pl[0];
  }
  return NULL;
}

/**
 * Retrieve the specified player objects.
 *
 * @param int[]|string[] $ids
 *   The user IDs (int) or the phone IDs (string).  All the elements of the
 *   array must be of the same type.
 *
 * @return object[]
 *   The player objects, if any.
 */
function zg_fetch_players_by_ids($ids) {

  if (!is_array($ids)) {
    $ids = [$ids];
  }

  // Find user from phone_id.
  $id_type = gettype($ids[0]);
  if ($id_type == 'string') {
    $where = 'WHERE users.phone_id in ("%s")';
    $id_string = implode('","', $ids);
  }
  else if ($id_type == 'integer') {
    $where = 'WHERE users.id in (%s)';
    $id_string = implode(',', $ids);
  }
  else {

    // Log this exception, unless running from cron.
    if ($_GET['q'] !== 'node') {
      zg_slack(NULL, 'error', 'zg_fetch_users_by_ids', 'type is: ' . $id_type);
    }
    return NULL;
  }

  $sql = 'SELECT distinct users.*,
    -- user_creations.datetime as startdate,
    `values`.party_title, `values`.party_icon, `values`.color,
    elected_positions.name as ep_name,
    elected_positions.id as ep_level,
    elected_officials.approval_rating,
    elected_positions.energy_bonus as ep_energy_bonus,
    elected_positions.can_broadcast_to_party,
  
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
  
    -- LEFT JOIN user_creations on user_creations.phone_id = users.phone_id
  
    LEFT JOIN event_points on event_points.fkey_users_id = users.id
  
    LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id

  ' . $where . ';';

  $result = db_query($sql, $id_string);
  $data = $phone_ids = $start_dates = [];

  while ($item = db_fetch_object($result)) {
    $item->id = (int) $item->id;
    $data[] = $item;
    $phone_ids[] = $item->phone_id;
  }

  $sql = 'select phone_id, MAX(datetime) as startdate from user_creations
    where phone_id in ("' . implode('","', $phone_ids) . '")
    group by phone_id;';
  $result = db_query($sql);
  while ($item = db_fetch_object($result)) {
    $start_dates[$item->phone_id] = $item->startdate;
  }

  foreach ($data as &$item) {
    $item->startdate = $start_dates[$item->phone_id] ?? 'Unknown';
  }

  $debug_full = FALSE;

  // Data retrieval error?
  if (count($data) != count($ids)) {
    zg_slack(NULL, 'error', 'zg_fetch_players_by_ids',
      'fetch of ' . count($ids) . ' returned ' . count($data) . ' results at ');
    zg_slack(NULL, 'error', 'zg_fetch_players_by_ids',
      'IDs: ' . implode(', ', $ids));
    zg_slack(NULL, 'error', 'zg_fetch_players_by_ids', $ids);
    $debug_full = TRUE;
  }

  foreach ($data as &$item) {

    if (empty($item->ep_level)) {
      $item->ep_level = 0;
    }

    // @todo: how does this happen?
    if ($item->actions_next_gain === '0000-00-00 00:00:00') {
      $item->actions_next_gain = date('Y-m-d H:i:s');
    }

    zg_alter('fetch_user', $item);
    if ($debug_full) {
      firep($item, 'zg_fetch_players_by_ids');
    }
    else {
      firep("$item->username ($item->id:$item->phone_id)", 'zg_fetch_players_by_ids');
    }
  }
  return $data;
}

/**
 * Fetch some of the extra attributes a user has.
 *
 * @param object $game_user
 *   The game user object.
 */
function zg_fetch_user_extra_attributes(&$game_user) {
  $sql = 'SELECT
  SUM( staff.extra_votes * staff_ownership.quantity ) AS extra_votes,
  SUM( staff.extra_defending_votes * staff_ownership.quantity )
    AS extra_defending_votes,
  SUM( staff.initiative_bonus * staff_ownership.quantity ) AS initiative,
  SUM( staff.endurance_bonus * staff_ownership.quantity ) AS endurance,
  SUM( staff.elocution_bonus * staff_ownership.quantity ) AS elocution
  FROM staff
  LEFT JOIN staff_ownership ON staff_ownership.fkey_staff_id = staff.id
  AND staff_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $staff_bonus = db_fetch_object($result);

  $sql = 'SELECT
  SUM( equipment.initiative_bonus * equipment_ownership.quantity ) AS initiative,
  SUM( equipment.endurance_bonus * equipment_ownership.quantity ) AS endurance,
  SUM( equipment.elocution_bonus * equipment_ownership.quantity ) AS elocution
  FROM equipment
  LEFT JOIN equipment_ownership
  ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $equipment_bonus = db_fetch_object($result);

  $game_user->extra_initiative = $staff_bonus->initiative + $equipment_bonus->initiative;
  $game_user->total_initiative = $game_user->extra_initiative * $game_user->initiative;
  $game_user->extra_endurance = $staff_bonus->endurance + $equipment_bonus->endurance;
  $game_user->total_endurance = $game_user->extra_endurance * $game_user->endurance;
  $game_user->extra_elocution = $staff_bonus->elocution + $equipment_bonus->elocution;
  $game_user->total_elocution = $game_user->extra_elocution * $game_user->elocution;
  $extra_votes = (int) $staff_bonus->extra_votes;
  $extra_defending_votes = (int) $staff_bonus->extra_defending_votes;
  zg_alter('extra_votes', $game_user, $extra_votes,
    $extra_defending_votes);
  $game_user->extra_votes = $extra_votes;
  $game_user->extra_defending_votes = $extra_defending_votes;
}

/**
 * Get the user ID from the phone ID.
 *
 * @param string $phone_id
 *   The phone ID to check.
 *
 * @return object|false
 *   The user ID, as an 'id' property of an object, or false if no results.
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
  return FALSE;
}

/**
 * Get the phone ID from the environment.
 *
 * @return false|string
 *   The phone ID, if any.
 */
function zg_get_phoneid() {
  $arg2 = arg(2);

  if (empty($arg2)) {
    return FALSE;
  }

  if ((substr($arg2, 0, 3) == 'fb=') || $arg2 == '') {
    mail('joseph@ziquid.com', 'fix ' . arg(0) . '/' . arg(1) . '/' . $arg2,
      'Referrer is ' . $_SERVER['HTTP_REFERER']);
    echo 'Uhoh! Bad URL ' . arg(0) . '/' . arg(1) . '/' . $arg2 .
      ' from <a href="' . $_SERVER['HTTP_REFERER'] . '">' .
      $_SERVER['HTTP_REFERER'] . '</a>';
    exit;
  }

  if ($arg2 == 'facebook') {
    return zg_get_fbid();
  }
  elseif ($arg2 === 'null') {
    drupal_goto('stlouis/playbook_null');
  }
  elseif (substr($arg2, 0, 3) === 'ms=') {
    return zg_get_msid();
  }
  elseif (substr($arg2, 0, 10) == 'landscape:') {
    return check_plain(substr($arg2, 10));
  }

  return check_plain($arg2);
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
 * Generate a zombie name.
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

  return /* 'Zombie ' . */ $first[mt_rand(0, count($first) - 1)] . ' '
    . $last[mt_rand(0, count($last) - 1)];
}

/**
 * Generate an Irish name.
 *
 * @see https://www.pinterest.com/pin/472385448387146151/
 *
 * @return string
 *   The Irish name.
 */
function zg_irish_name() {

  static $first = [
    'Aeden',
    'Braeden',
    'Cillian',
    'Donagh',
    'Eireann',
    'Fionbarr',
    'Grainne',
    'Houlihan',
    'Innes',
    'Ruaridh',
    'Kael',
    'Logan',
    'Mickey',
    'Morag',
    'Nigel',
    'Vonagh',
    'Paisley',
    'Ryne',
    'Quinn',
    'Sorcha',
    'Tory',
    'Urlaith',
    'Diarmid',
    'Conor',
    'Cadhgan',
    'Yseult',
    'Kameron',
  ];

  static $last = [
    'Allen',
    'O\'Brien',
    'Byrne',
    'O\'Connor',
    'Murphy',
    'Maguire',
    'O\'Riley',
    'O\'Rourke',
    'Fitzgerald',
    'Walsh',
    'Doherty',
    'Gallagher',
    'McCarthy',
    'O\'Fallon',
  ];

  return $first[mt_rand(0, count($first) - 1)] . ' '
    . $last[mt_rand(0, count($last) - 1)];
}

/**
 * Can I perform an action yet?
 *
 * @param \stdClass|int $id
 *   The user object or user ID.
 * @param string $key
 *   The key to check.
 *
 * @return object
 *   An object with whether the action can be performed; if FALSE, also the
 *   hours, minutes, and seconds until action can be performed.
 */
function zg_can_do_yet($id, $key) {
  if (is_object($id) && property_exists($id, 'id')) {
    $id = $id->id;
  }
  $can_do = new stdClass();
  $can_do->time_ready = (int) zg_get_value($id, $key, REQUEST_TIME);
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
 * @param \stdClass|int $id
 *   The user object or user ID.
 * @param string $bonus_key
 *   The key to check.
 *
 * @return \stdClass
 *   An object with whether a bonus is in effect and the hours, minutes, and
 *   seconds remaining, or FALSE.
 */
function zg_timed_bonus_in_effect($id, $bonus_key) {
  if (is_object($id) && property_exists($id, 'id')) {
    $id = $id->id;
  }
  $in_effect = new stdClass();
  $in_effect->time_ready = (int) zg_get_value($id, $bonus_key, REQUEST_TIME);
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
 * @param \stdClass|int $id
 *   The game user object or user ID.
 * @param string $timer_key
 *   The key to set.
 * @param int $time_secs
 *   The number of seconds to set for the timer.
 */
function zg_set_timer($id, $timer_key, $time_secs) {
  zg_set_value($id, $timer_key, REQUEST_TIME + $time_secs);
}

/**
 * Return the value for the key in the db if found, or $default if not found.
 *
 * @param object|int|null $id
 *   The game user object or user ID.
 * @param string $key
 *   The key to use for the query.
 * @param mixed $default
 *   The result to return if the KVP is not found.
 *
 * @return mixed
 *   The value, if found, or $default, if not found.
 */
function zg_get_value($id, $key, $default = NULL) {
  if (is_null($id)) {
    return $default;
  }

  if (is_object($id) && property_exists($id, 'id')) {
    $id = $id->id;
  }
  $sql = 'SELECT `value` FROM {user_attributes}
    WHERE `fkey_users_id` = %d AND `key` = "%s";';
  $item = db_query($sql, (int) $id, $key)->fetch_object();

  if (!$item) {
    return $default;
  }

  // Serialized data?  Undo it.
  if (property_exists($item, 'value') && substr($item->value, 0, 11) == 'serialized:') {
    return unserialize(substr($item->value, 11));
  }

  return !empty($item->value) ? $item->value : $default;
}

/**
 * Set a value for a certain key for the game user.
 *
 * Updates the value if the KVP already exists.
 *
 * @param object|int $id
 *   The game user object or user ID.
 * @param string $key
 *   The key to use for the value in the db.
 * @param string $value
 *   The value to set in the db.
 *
 * @return bool
 *   TRUE if the set succeeded; FALSE otherwise.
 */
function zg_set_value($id, $key, $value = 'set') {
  if (is_object($id) && property_exists($id, 'id')) {
    $id = $id->id;
  }

  // Non-string?  Serialize the data before storing.
  if (!is_string($value)) {
    $value = 'serialized:' . serialize($value);
  }
  $sql = 'INSERT INTO {user_attributes}
    (`fkey_users_id`, `key`, `value`) VALUES (%d, "%s", "%s")
    ON DUPLICATE KEY UPDATE `value` = "%s", last_update = CURRENT_TIMESTAMP;';
  return db_query($sql, $id, $key, $value, $value);
}


/**
 * Return the value for the key in the db if found, or $default if not found.
 *
 * @param \stdClass|int $id
 *   The game user object or user ID.
 * @param string $key
 *   The key to use for the query.
 * @param mixed $default
 *   The result to return if KVP is not found.
 *
 * @return mixed
 *   The value, if found, or $default, if not found.
 */
function zg_get_cache($id, $key, $default = NULL) {
  if (is_object($id) && property_exists($id, 'id')) {
    $id = $id->id;
  }
  $sql = 'SELECT `value` FROM {user_attributes}
    WHERE `fkey_users_id` = %d AND `key` = "%s";';
  $item = db_query($sql, (int) $id, $key)->fetch_object();

  // Serialized data?  Undo it.
  if (substr($item->value, 0, 11) == 'serialized:') {
    return unserialize(substr($item->value, 11));
  }

  return !empty($item->value) ? $item->value : $default;
}

/**
 * Cache a value for a certain key and domain.
 *
 * Updates the value if the DKV already exists.
 *
 * @param string $key
 *   The key to use for the value in the db.
 * @param string $value
 *   The value to set in the db.
 * @param string $domain
 *   The domain of the cache.  Defaults to the name of the calling function.
 *
 * @return bool
 *   TRUE if the set succeeded; FALSE otherwise.
 */
function zg_set_cache($key, $value, $domain = '') {
  // Not a string?  Serialize the data before storing.
  if (!is_string($value)) {
    $value = 'serialized:' . serialize($value);
  }

  // Set a default for domain.
  if (!strlen($domain)) {
    $bt = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 2);
    firep($bt, 'backtrace');
    $domain = 'backtrace';
  }

  $sql = 'INSERT INTO {cache}
    (cache_key, cache_value, cache_domain) VALUES ("%s", "%s", "%s")
    ON DUPLICATE KEY UPDATE cache_value = "%s", last_update = CURRENT_TIMESTAMP;';
  return db_query($sql, $key, $value, $domain, $value);
}

/**
 * Remove a KVP for a user.
 *
 * @param \stdClass|int $id
 *   The game user object or user ID.
 * @param string $key
 *   The key to remove.
 *
 * @return bool
 *   Whether the removal succeeded or not.
 */
function zg_remove_value($id, $key) {
  if (is_object($id) && property_exists($id, 'id')) {
    $id = $id->id;
  }
  $sql = 'delete from {user_attributes}
    where `fkey_users_id` = %d and `key` = "%s";';
  return db_query($sql, $id, $key);
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
 * @param string $destination
 *
 * @return string
 *   The button, as a string of HTML.
 */
function zg_luck_money_render_button(\stdClass $game_user, $destination = '') {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $normal_offer = zg_normal_luck_money_offer($game_user);
  [$offer, $comment] = zg_luck_money_offer($game_user);
  $formatted_offer = number_format($offer);

  if ($offer == $normal_offer) {
    $type = "Receive $formatted_offer $game_user->values (1&nbsp;$luck)";
  }
  else {
    $type = "Receive <span class='highlight'>$formatted_offer</span> $game_user->values (1&nbsp;$luck)";
  }

  $extra_link = '/money';
  if (strlen($destination)) {
    $extra_link .= '?destination=/' . $destination;
  }

  return zg_render_button('elders_do_fill',
    $type, $extra_link, 'big-80 slide-in-content');
}

/**
 * Return the amount of money the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return array
 *   The offer as an integer and explanation text, if any.
 */
function zg_luck_money_offer(\stdClass $game_user) {
  $offer = zg_normal_luck_money_offer($game_user);
  $comment = '';
  zg_alter('luck_money_offer', $game_user, $offer, $comment);
  return [$offer, $comment];
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
 * Return the amount of action the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return array
 *   The offer as an integer and explanation text, if any.
 */
function zg_luck_action_offer(\stdClass $game_user) {
  $offer = $game_user->actions_max;
  $comment = '';
  zg_alter('luck_action_offer', $game_user, $offer, $comment);
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
//      firep($function_name, 'zg_alter function name');
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
  $file_to_include = $module_path . '/includes/' . $type . '/' . $game . '_' .
    $include_file . '.inc';

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
//    firep($function_name, 'zg_includes_add function name');
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
 *   move, the verb used to display the move, and the equipment used.
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
    zg_alter('speed_increase', $game_user, $eq, $verb);
    $actions_to_move -= $eq->speed_increase;
  }

  $actions_to_move = max($actions_to_move, 6);

  // In a building?  1 extra action.
  if ($game_user->fkey_buildings_id > 0) {
    $actions_to_move++;
  }

  zg_alter('actions_to_move', $game_user, $actions_to_move, $eq);
  return [$from_hood, $to_hood, $actions_to_move, $verb, $eq];
}

/**
 * Get the list of supported game machine names.
 *
 * @return string[]
 *   The list of game machine names.
 */
function zg_get_game_list() {
  return [
    'stl1904',
//    'stl2124',
//    'cg',
//    'detroit',
//    'elysian',
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
  else if (arg(0) == 'cg') {
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
  $auth_url = 'https://www.facebook.com/dialog/oauth?client_id='
    . $app_id . '&redirect_uri=' . urlencode($canvas_page);

  [$encoded_sig, $payload] = explode('.', $signed_request, 2);
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
 * Send a message from a player to Slack.
 *
 * @param object|null $game_user
 *   The game user object, or NULL if from cron.
 * @param string $type
 *   The type of the message.
 * @param string $subtype
 *   The subtype of the message.
 * @param string $message
 *   The message to post into a channel.
 *
 * @return bool|object
 *   Whether the send was successful or not.
 */
function zg_slack($game_user, $type, $subtype, $message) {

  // Not a scalar?  Print its contents.
  if (is_object($message) || is_array($message)) {
    $message = zg_print_r($message);
  }

  // Only cron, bot, error, debug, or fetch_user messages can be playerless.
  if ($game_user === NULL && ($type == 'cron' || $type == 'bots' || $type == 'error'
    || $type == 'debug' || $subtype == 'zg_fetch_users_by_ids'
    || $_GET['q'] == 'node')) {
    return zg_send_slack_message($type, $type, $subtype, $message);
  }

  // Not a user object?  complain.
  if (!is_object($game_user)) {
    $bt = debug_backtrace(DEBUG_BACKTRACE_PROVIDE_OBJECT, 2);
    $message = 'FIXME: zg_slack had no $game_user object: (' .
      basename($bt[0]['file']) . ':' . $bt[0]['line'] . '): ' . $message;
    zg_send_slack_message('error', $type, $subtype, $message);
    return FALSE;
  }

  // No meta property?  complain.
  if (!property_exists($game_user, 'meta')) {
    $bt = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 2);
    $message = 'FIXME: $game_user parameter to zg_slack() had no meta property: (' .
      basename($bt[0]['file']) . ':' . $bt[0]['line'] . '): ' . $message;
    zg_send_slack_message('error', $type, $subtype, $message);
    return FALSE;
  }

  // Admin player or dev box?  Log only to the devs channel.
  if ($_SERVER['HTTP_HOST'] == 'uslce.lndo.site' || $game_user->meta == 'admin') {
    return zg_send_slack_message('devs', $type, $subtype, $message);
  }

  // Bot?  Log only to the bots channel.
  if (substr($game_user->meta, 0, 3) == 'ai_' ||
    $game_user->meta == 'zombie') {

    // We don't need to track bot pages.
    if ($type == 'pages') {
      return TRUE;
    }
    return zg_send_slack_message('bots', $type, $subtype, $message);
  }

  return zg_send_slack_message($type, $type, $subtype, $message);
}

/**
 * Send a message to Slack.
 *
 * @param string $channel
 *   The channel of the message.
 * @param string $type
 *   The type of the message.
 * @param string $subtype
 *   The type of the message.
 * @param string $message
 *   The message to post into a channel.
 *
 * @return bool|object
 *   Whether the send was successful or not.
 */
function zg_send_slack_message($channel, $type, $subtype, $message) {
  global $game;
//  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $bt = debug_backtrace(DEBUG_BACKTRACE_PROVIDE_OBJECT | DEBUG_BACKTRACE_IGNORE_ARGS, 2);
  if ($bt[1]['function'] != 'zg_slack') {
    slack_send_message('zg_send_slack_message() erroneously called from '
      . $bt[1]['function'] . ' at ' . basename($bt[1]['file']) . ':' .
      $bt[1]['line'] . '.', '#uslce-errors');
  }

  $type = mb_strtolower($type);
  $datetime = new DateTime();
  $datetimestring = $datetime->format('H:i:s.v');
  $slack_message = '(' . $datetimestring . ': ' . $type . '/' . $subtype . '): ' .
    nl2br($message) . ' /' . $_GET['q'];
  $slack_channel = '#uslce';

  switch ($channel) {

    case 'bot':
      $slack_channel .= '-bots';
      break;

    case 'debug':
    case 'error':
      $slack_channel .= '-errors';
      $slack_message = '(' .
        basename($bt[1]['file']) . ':' . $bt[1]['line'] . '): ' . $slack_message;
      break;

    case 'bots':
    case 'cron':
    case 'competency-gain':
    case 'devs':
    case 'errors':
    case 'loot':
    case 'luck':
    case 'pages':
    case 'ran-out-of':
      $slack_channel .= '-' . $channel;
      break;
  }

//  dpm($slack_message, 'would send this message to ' . $slack_channel);

  // Luck refill message?  Also send to 'ran-out-of' channel for comparison.
  if ($channel == 'luck' && substr($subtype, -7) == '_refill') {
    slack_send_message($slack_message, '#uslce-ran-out-of');
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
  $elected_official_type = $item->type ?? '';
  firep($elected_official_type, 'elected_official_type');

  $clan_sql = 'where clan_messages.fkey_neighborhoods_id = %d';
  $clan_id_to_use = $game_user->fkey_clans_id;
  $limit = 20;
  $all_limit = 100;

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

    if (count($data)) {
      $clan_sql = 'where clan_messages.fkey_neighborhoods_id in (%s)';
      $clan_id_to_use = implode(',', $data);
      $limit = 50;
      $all_limit = 150;
    }
  }

  $sql = '
  (
  select user_messages.timestamp, user_messages.message,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  user_messages.private, user_messages.id as msg_id,
  0 AS is_announcement,
  user_messages.fkey_users_from_id, user_messages.fkey_users_to_id,
  "user" as type,
  subtype,
  neighborhoods.name as location
  from user_messages
  left join users on user_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  user_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
  where fkey_users_to_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select challenge_messages.timestamp, challenge_messages.message,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, challenge_messages.id as msg_id,
  0 AS is_announcement,
  challenge_messages.fkey_users_from_id, challenge_messages.fkey_users_to_id,
  "challenge" as type,
  "" as subtype,
  neighborhoods.name as location
  from challenge_messages
  left join users on challenge_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  challenge_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
  where fkey_users_to_id = %d
  order by timestamp DESC limit %d
  )

  union
  -- HOOD MESSAGES

  (
  select neighborhood_messages.timestamp, neighborhood_messages.message,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, neighborhood_messages.id as msg_id,
  0 AS is_announcement,
  neighborhood_messages.fkey_users_from_id, 0 as fkey_users_to_id,
  "hood" as type,
  "" as subtype,
  neighborhoods.name as location
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
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
  where neighborhood_messages.fkey_neighborhoods_id = %d
  order by timestamp DESC limit %d
  )

  union
  -- CLAN MESSAGES

  (
  select clan_messages.timestamp, clan_messages.message,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  UPPER(clans.acronym) as clan_acronym,
  0 AS private, clan_messages.id as msg_id,
  clan_messages.is_announcement,
  clan_messages.fkey_users_from_id, 0 AS fkey_users_to_id,
  "clan" as type,
  "" as subtype,
  neighborhoods.name as location
  from clan_messages
  left join users on clan_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    clan_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
  ' . $clan_sql . '
  order by timestamp DESC limit %d
  )

  union
  -- VALUES (PARTY) MESSAGES

  (
  select values_messages.timestamp, values_messages.message,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, values_messages.id as msg_id,
  0 AS is_announcement,
  values_messages.fkey_users_from_id, 0 AS fkey_users_to_id,
  "values" as type,
  "" as subtype,
  neighborhoods.name as location
  from values_messages
  left join users on values_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
    values_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
  where values_messages.fkey_values_id = %d
--    AND values_messages.fkey_neighborhoods_id = %d
  order by timestamp DESC limit %d
  )

  union

  (
  select system_messages.timestamp, system_messages.message,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  0 AS private, system_messages.id as msg_id,
  0 AS is_announcement,
  system_messages.fkey_users_from_id, 0 AS fkey_users_to_id,
  "mayor" as type,
  subtype,
  neighborhoods.name as location
  from system_messages
  left join users on system_messages.fkey_users_from_id = users.id
  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id
  LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
  system_messages.fkey_users_from_id
  LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
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

  zg_alter('get_all_messages', $game_user, $data);
  return $data;
}

/**
 * Fetch all new user messages for a user.
 *
 * Retrieves all messages above a given message ID.  If the player is checking
 * the player's own messages, include private messages.
 *
 * @param object $game_user
 *   The game_user object.
 * @param int $check_user_id
 *   The userID of the user to check.
 * @param int $msg_id
 *   The message ID of messages already received.
 *
 * @return array
 *   The messages.
 */
function zg_get_new_user_messages($game_user, $check_user_id, $msg_id = 0) {

  // Standard view?  Show in descending order.  New messages show in ascending.
  $order = ($msg_id == 0) ? 'DESC' : 'ASC';

  // Not looking at yourself? Don't show private messages.
  if ($game_user->id != $check_user_id) {
    $no_private = ' AND (private = 0 OR user_messages.fkey_users_from_id = %d) ';
  }
  else {
    $no_private = '';
  }

  $sql = '
  select user_messages.timestamp, user_messages.message,
  user_messages.fkey_users_from_id, user_messages.fkey_users_to_id,
  users.username, users.phone_id, users.values,
  users.level,
  elected_positions.name as ep_name,
  clan_members.is_clan_leader,
  clans.acronym as clan_acronym,
  user_messages.private, user_messages.id as msg_id,
  0 AS is_announcement,
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
  LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id

  where fkey_users_to_id = %d AND user_messages.id > %d
  ' . $no_private . '
  order by msg_id ' . $order . ' LIMIT 50;';
  $data = [];

  $result = db_query($sql, $check_user_id, $msg_id, $game_user->id);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  zg_alter('get_new_user_messages', $game_user, $data);
  return $data;
}

/**
 * Fetch the new user messages count for current user.
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
 * Format a message item for display.
 *
 * @param object $game_user
 *   The game user object.
 * @param int $check_user_id
 *   The userID of the person whose messages area being checked.
 * @param object $message
 *   The message object.
 */
function zg_format_message($game_user, $check_user_id, &$message) {
  global $game, $phone_id;

  $message->display = new \stdClass();
  $message->display->timestamp = zg_format_date($message->timestamp);
  $message->display->msg_id = 'news-item-' . $message->type . '-' . $message->msg_id;

  if ($message->private) {
    $message->display->private_css = 'private';
    $message->display->private_text = '(private)';
  }
  else {
    $message->display->private_css = $message->display->private_text = '';
  }

  $message->display->private_css .= ' ' . $message->type . ' ' .
    $message->type . '-' . $message->subtype;

  if ($message->is_announcement) {
    $message->display->item_css = 'announcement';
  }
  else {
    $message->display->item_css = '';
  }

  if (empty($message->username)) {
    $message->display->username = '';
    $message->display->reply = '';
  }
  else {
    $message->display->username = 'from ' .
      zg_render_player($message, $message->type . '_msg');
    if (!in_array($message->username, ['USLCE Game', 'The Socialite'])) {
      $message->display->reply = zg_render_button('user', 'View / Respond', '/id:' .
        $message->fkey_users_from_id);
    }
    else {
      $message->display->reply = '';
    }
  }
  // Allow user to delete own messages.
  if (($message->type == 'user') &&
  ($message->fkey_users_to_id == $game_user->id ||
    $message->fkey_users_from_id == $game_user->id)) {
    $arg2 = check_plain(arg(2));
    $q = $_GET['q'];
    $message->display->delete = <<< EOF
      <div class="message-delete">
        <a href="/$game/msg_delete/$arg2/$message->msg_id?destination=/$q">
          <img src="/sites/default/files/images/delete.png" width="16" height="16"/>
        </a>
      </div>
EOF;
  }
  else {
    $message->display->delete = '';
  }

  $message->display->message = zg_expand_shortcode($message->message);
}

/**
 * Expand shortcodes embedded in messages, such as [yt:...].
 *
 * @param string $message
 *   The message to parse.
 *
 * @return string
 *   The parsed message.
 */
function zg_expand_shortcode($message) {
  $message = preg_replace(
    [
      '/\[yt:([-A-Za-z0-9]+)\]/i',
    ],
    [
      ' <a href="externals://youtu.be/\1" class="needs-web-title">YouTube: \1</a> ',
    ],
    $message
  );
  return $message;

}

function zg_short_format_int($num) {
  $num = floor($num);
  if ($num < 1000) {
    return (string) $num;
  }
  if ($num < 10000) {
    $num = floor($num / 100);
    return number_format($num / 10, 1) . 'K';
  }
  if ($num < 1000000) {
    $num = floor($num / 1000);
    return number_format($num, 0) . 'K';
  }
  if ($num < 10000000) {
    $num = floor($num / 100000);
    return number_format($num / 10, 1) . 'M';
  }
  if ($num < 1000000000) {
    $num = floor($num / 1000000);
    return number_format($num, 0) . 'M';
  }
  if ($num < 10000000000) {
    $num = floor($num / 100000000);
    return number_format($num / 10, 1) . 'B';
  }
  if ($num < 1000000000000) {
    $num = floor($num / 1000000000);
    return number_format($num, 0) . 'B';
  }
  if ($num < 10000000000000) {
    $num = floor($num / 100000000000);
    return number_format($num / 10, 1) . 'T';
  }
  if ($num < 1000000000000000) {
    $num = floor($num / 1000000000000);
    return number_format($num, 0) . 'T';
  }
}

/**
 * Format items in a message for display.
 *
 * Also saves the highest messageID found.
 *
 * @param object $game_user
 *   The game user object.
 * @param int $check_user_id
 *   The userID of the person whose messages area being checked.
 * @param object[] $messages
 *   An array of message objects.
 */
function zg_format_messages($game_user, $check_user_id, array &$messages) {
  global $game, $phone_id;

  $max_user_message = 0;
  foreach ($messages as &$item) {
    zg_format_message($game_user, $check_user_id, $item);
    if ($item->type == 'user') {
      $max_user_message = max((int) $item->msg_id, $max_user_message);
    }
  }
  firep($max_user_message, 'max user message');
  zg_set_value($game_user, 'max_user_message', $max_user_message);

  $url = "/$game/msg_count/$phone_id/$max_user_message";
  drupal_add_js(['zg' => ['check_messages_url' => $url]], 'setting');
}

/**
 * Render a player object in a certain display.
 *
 * @param object $pl
 *   The player object.
 * @param string $display
 *   The rendering display.  Defaults to html.
 *
 * @return string
 *   The html rendering of the player object.
 */
function zg_render_player(object &$pl, $display = 'html') {
  $function_name = 'zg_render_player_' . $display;

  if (function_exists($function_name)) {
    return $function_name($pl);
  }
  else {
    return zg_render_player_html($pl);
  }
}

/**
 * Render a player as html.
 *
 * @param object|null $pl
 *   The player object.
 *
 * @return string|false
 *   The html rendering of the player object, or FALSE if no player obj exists.
 */
function zg_render_player_html(object &$pl) {
	if (!is_object($pl)) {
		return FALSE;
	}

  if (!property_exists($pl, 'display')) {
    $pl->display = new \stdClass();
  }
  $pl->display->clan_acronym = '';
  if (!empty($pl->clan_acronym)) {
    $pl->display->clan_acronym = "($pl->clan_acronym)";
  }

  if ($pl->is_clan_leader) {
    $pl->display->clan_acronym .= '*';
  }

  $pl->display->class = 'player-' . drupal_strtolower($pl->values);

  $pl->display->rendered_html = '<span class="user-display user-display-html">' .
    $pl->ep_name . ' <span class="' .
    $pl->display->class . '">' . $pl->username . '</span> ' .
    $pl->display->clan_acronym . '</span>';
  return $pl->display->rendered_html;
}

/**
 * Render a user as clan view.
 *
 * @param object $user
 *   The user object.
 *
 * @return string
 *   The clan view html rendering of the user object.
 */
function zg_render_user_clan(&$user) {
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));

  if (!property_exists($user, 'display')) {
    $user->display = new \stdClass();
  }
  $user->display->clan_acronym = '';
//  if (!empty($user->clan_acronym)) {
//    $user->display->clan_acronym = "($user->clan_acronym)";
//  }

  if ($user->is_clan_leader) {
    $user->display->clan_acronym .= '*';
  }

  $user->display->level = t('Level:') . ' ' . $user->level;
  $user->display->location = t('Location:') . ' ' . $user->location;

  $player_class = 'player_username';
  $last_access = REQUEST_TIME - strtotime($user->last_access);
  firep([REQUEST_TIME, strtotime($user->last_access), $last_access], 'last access times');
  if ($last_access < 300) {
    $player_class .= ' online_now';
  }
  elseif ($last_access < 900) {
    $player_class .= ' online_recently';
  }
  else {
    $player_class .= ' not_online';
  }

  $user->display->username = '<a class="' . $player_class . '" href="/' . $game .
    '/user/' . $arg2 . '">' . $user->username . '</a>' . $user->display->clan_acronym;

  if (strlen($user->ep_name)) {
    $user->display->username = '<span class="player_seat">' . $user->ep_name .
      '</span> ' . $user->display->username;
  }

  $user->display->user_clan = $user->display->username .
    '<span class="player_level">' . $user->display->level . '</span>' .
    '<span class="player_location">' . $user->display->location . '</span>';
  return $user->display->user_clan;
}

/**
 * Render a user as clan message view.
 *
 * @param object $user
 *   The user object.
 *
 * @return string
 *   The clan view html rendering of the user object.
 */
function zg_render_user_clan_msg(&$user) {
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));

  if (!property_exists($user, 'display')) {
    $user->display = new \stdClass();
  }
  $user->display->clan_acronym = '';
  if (strlen($user->clan_acronym)) {
    $user->display->clan_acronym = "($user->clan_acronym)";
  }

  if ($user->is_clan_leader) {
    $user->display->clan_acronym .= '*';
  }

  $user->display->level = $user->level;
  $user->display->location = $user->location;

  $player_class = 'player_username';
  $last_access = REQUEST_TIME - strtotime($user->last_access);
  firep([REQUEST_TIME, strtotime($user->last_access), $last_access], 'last access times');
  if ($last_access < 300) {
    $player_class .= ' online_now';
  }
  elseif ($last_access < 900) {
    $player_class .= ' online_recently';
  }
  else {
    $player_class .= ' not_online';
  }

  $user->display->username = '<a class="' . $player_class . '" href="/' . $game .
    '/user/' . $arg2 . '">' . $user->username . '</a>' . $user->display->clan_acronym;

  if (strlen($user->ep_name)) {
    $user->display->username = '<span class="player_seat">' . $user->ep_name .
      '</span> ' . $user->display->username;
  }

  $user->display->user_clan = $user->display->username .
    '<span class="player_level">(' . $user->display->level . '/' .
    $user->display->location . ')</span>';
  return $user->display->user_clan;
}

/**
 * Render a user in the debate list view.
 *
 * @param object $user
 *   The user object.
 *
 * @return string
 *   The debate list view html rendering of the user.
 */
function zg_render_user_debates_list(&$user) {
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));

  if (!property_exists($user, 'display')) {
    $user->display = new \stdClass();
  }

  $user->display->icon = $game . '_clan_' . $user->party_icon . '.png';
  $user->display->clan_acronym = '';
  if (strlen($user->clan_acronym)) {
    $user->display->clan_acronym = "($user->clan_acronym)";

    $icon_path = file_directory_path() . '/images/' . $game . '_clan_' .
      strtolower($user->clan_acronym) . '.png';

    if (file_exists($_SERVER['DOCUMENT_ROOT'] . base_path() . $icon_path)) {
      $user->display->icon = $game . '_clan_' . strtolower($user->clan_acronym) . '.png';
    }
  }

  if ($user->is_clan_leader) {
    $user->display->clan_acronym .= '*';
  }

  $user->display->level = $user->level;
  $user->display->location = $user->location;

  $player_class = 'player_username player-' . drupal_strtolower($user->values);
  $last_access = REQUEST_TIME - strtotime($user->last_access);
  firep([REQUEST_TIME, strtotime($user->last_access), $last_access], 'last access times');
  if ($last_access < 300) {
    $player_class .= ' online_now';
  }
  elseif ($last_access < 900) {
    $player_class .= ' online_recently';
  }
  else {
    $player_class .= ' not_online';
  }

  $user->display->username = '<a class="' . $player_class . '" href="/' . $game .
    '/user/' . $arg2 . '/id:' . $user->id . '">' . $user->username . '</a>' .
    '<span class="player_clan">' . $user->display->clan_acronym . '</span>';

  if (strlen($user->ep_name)) {
    $user->display->username = '<span class="player_seat">' . $user->ep_name .
      '</span> ' . $user->display->username;
  }

  $user->display->debates_list =
    '<div class="user-display user-display-debates-list">' .
    '<img class="clan-icon" src="/sites/default/files/images/' .
    $user->display->icon . '"/>' . $user->display->username .
    ' <span class="player_level">(' . $user->display->level . '/' .
    $user->display->location . ')</span></div>';
  return $user->display->debates_list;
}

/**
 * Render a player's info into HTML for use in the header.
 *
 * @param object|null $pl
 *   The player object.
 *
 * @return string|false
 *   The html rendering of the player object, or FALSE if no player obj exists.
 */
function zg_render_player_header(&$pl) {
	if (!is_object($pl)) {
		return FALSE;
	}

  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  $phone_id = zg_get_phoneid();

  if (!property_exists($pl, 'display')) {
    $pl->display = new \stdClass();
  }
  $pl->display->clan_acronym = '';
  if (empty($pl->clan_acronym)) {
    $pl->display->clan = t('No Clan');
    $clan_link = "clan/$arg2/0";
  }
  else {
    $pl->display->clan = "Clan: $pl->clan_name ($pl->clan_acronym)";
    $clan_link = "clan/$arg2/{$pl->fkey_clans_id}";
  }

  if ($pl->is_clan_leader) {
    $pl->display->clan .= '*';
  }

  $pl->display->party = "Party: $pl->party_title";

  $level_a1 = $level_a2 = $pl->display->level_tip = '';
  if ($pl->skill_points && $phone_id === $pl->phone_id) {
    $level_a1 = '<a href="/' . $game . '/increase_skills/' . $arg2 . '/none">';
    $level_a2 = '</a>';
    $pl->display->level_tip = t('Tap to assign skill points');
  }
  $pl->display->level_num = $level_a1 . $pl->level . $level_a2;
  $pl->display->level = t('Level: ') . $pl->display->level_num;

  $pl->display->username = '<span class="player_seat">' . $pl->ep_name . '</span>' .
    ' <a class="player_username" href="/' . $game . '/user/' . $arg2 .
    '">' . $pl->username . '</a>';

  $pl->display->location = '<a href="/' . $game . '/move/' . $arg2 . '/0">' .
    $pl->location . '</a>';
  $alder = zg_get_hood_alder($pl->fkey_neighborhoods_id);
  firep($alder, 'Alder player');
  $hood_values = strlen($alder->values) ? drupal_strtolower($alder->values) : 'nothing';
  $hood_color = strlen($alder->color) ? $alder->color . 'aa' : '333a';

  $pl->display->user_header = '<div>' . $pl->display->username . '</div><div>' .
    '<span class="player_level">' . $pl->display->level . '</span>' .
    '<a class="player_clan" href="/' . $game . '/' . $clan_link . '">' . $pl->display->clan . '</a>' .
    '<span class="player_party">' . $pl->display->party . '</span></div>' .
    '<div><div class="player_location location-values-' . $hood_values . '" ' .
    'style="border-color: #' . $hood_color . ';">' .
    $pl->display->location . '</div></div>';
//   . '<pre>' . print_r($user, TRUE) . '</pre>';
  return $pl->display->user_header;
}

/**
 * Get the specified neighborhood's Alder info.
 * @param $hood
 *
 * @return null|object|\stdClass
 *   The value of the current hood's Alder, if any.
 */
function zg_get_hood_alder($hood) {
  firep($hood, 'hood');
  $sql = 'SELECT users.id FROM users
  inner join elected_officials on elected_officials.fkey_users_id = users.id
  WHERE fkey_neighborhoods_id = %d
  and elected_officials.fkey_elected_positions_id = 1;';
  $result = db_query($sql, $hood);
  $alder = db_fetch_object($result);
  firep($alder, 'alder id');
  if (is_object($alder) && property_exists($alder, 'id')) {
    return zg_fetch_player_by_id((int) $alder->id);
  }
  return NULL;
}

/**
 * Get Admin or staff online presence.
 *
 * @return string
 *   A string depicting any online presence.
 */
function zg_get_online_presence() {
  $sql = 'select count(*) as count from users
    where meta = "admin"
    and last_access > (CURRENT_TIMESTAMP - 300);';
  $result = db_query($sql);
  $admin = db_fetch_object($result);
  if ($admin->count) {
    return 'admin-online';
  }
  $sql = 'select count(*) as count from users
    where meta = "staff"
    and last_access > (CURRENT_TIMESTAMP - 300);';
  $result = db_query($sql);
  $staff = db_fetch_object($result);
  if ($staff->count) {
    return 'staff-online';
  }
  return '';
}

/**
 * Format a date according to game guidelines.
 *
 * @param int|string $epoch
 *   The date to format, converted to an int, if necessary.
 *
 * @return string
 *   The date, as a string.
 */
function zg_format_date($epoch) {

  if (is_string($epoch)) {
    $epoch = strtotime($epoch);
  }

  $time_diff = REQUEST_TIME - $epoch;
  // firep("time difference: $time_diff");

  if ($time_diff < 10) {
    return t('A few seconds ago');
  }
  elseif ($time_diff < 60) {
    return t('@time seconds ago', ['@time' => $time_diff]);
  }
  elseif ($time_diff < 120) {
    return t('1 minute ago');
  }
  elseif ($time_diff < 3600) {
    return t('@time minutes ago', ['@time' => floor($time_diff / 60)]);
  }
  elseif ($time_diff < 7200) {
    return t('1 hour ago');
  }
  elseif ($time_diff < 86400) {
    return t('@time hours ago', ['@time' => floor($time_diff / 3600)]);
  }
  elseif ($time_diff < 172800) {
    return t('A day ago');
  }
  elseif ($time_diff < 1728000) {
    return t('@time days ago', ['@time' => floor($time_diff / 86400)]);
  }
  else {
    return date('d M Y', $epoch);
  }

}

/**
 * Get the title of a web page.
 *
 * @param string $url
 *   The url to check.
 *
 * @return string|null
 *   The title, if any was found, or NULL.
 */
function zg_get_web_title($url) {
//  if ($title = zg_get_cache($url)) {
//    return $title;
//  }

  $page = file_get_contents($url);
  $title = preg_match('/<title[^>]*>(.*?)<\/title>/ims', $page, $match)
    ? $match[1] : NULL;

  zg_set_cache($url, $title);
  return $title;
}

/**
 * Print any variable or set of variables in a condensed format.
 *
 * @return string
 *   The condensed values.
 */
function zg_print_r() {
  $indent = 0;
  $output = '';
  $args = func_get_args();
  foreach ($args as $k => $v) {
    $output .= zg_printdented($indent, $k, $v);
  }
  return $output;
}

function zg_printdented($indent, $k, $v) {
  $output = str_repeat('  ', $indent) . $k . ' => ';
  if (is_scalar($v) || is_null($v)) {
    $output .= zg_get_var_type($v) . ' ' . PHP_EOL;
  }
  else {
    $all_scalars = is_array($v);
    foreach ($v as $vv) {
      $all_scalars &= is_scalar($vv);
    }
    if ($all_scalars) {
      $v2 = [];
      foreach ($v as $vk => $vv) {
        $v2[$vk] = zg_get_var_type($vv);
      }
      $output .= '[' . implode(', ', $v2) . ']' . PHP_EOL;
    }
    else {
      $output .= zg_get_var_type($v) . ' ' . PHP_EOL;
      foreach ($v as $vk => $vv) {
        $output .= zg_printdented($indent + 1, $vk, $vv);
      }
    }
  }
  return $output;
}

/**
 * Return a string with the type of the variable and its value.
 *
 * @param mixed $var
 *   The variable to find the type of.
 *
 * @return string
 *   The type of the variable and its value.
 */
function zg_get_var_type($var): string {
  $type = gettype($var);

  switch ($type) {
    case 'string':
      return '"' . $var . '"';

    case 'integer':
    case 'float':
    case 'double':
      return $var;

    case 'boolean':
      return $var ? 'TRUE' : 'FALSE';

    case 'array':
    case 'object':
      return '(' . substr($type, 0, 3) . ')';

    case 'NULL':
      return 'NULL';

    default:
      return '(' . substr($type, 0, 3) . ') ' . $var;
  }
}

function zg_server_is_distressed() {
  return zg_get_loadavg() >= 16;
}

function zg_server_is_busy() {
  return zg_get_loadavg() >= 8;
}

/**
 * Get the load average.  Doesn't work on Winders.
 *
 * @return int
 *   The load average over the last minute.
 */
function zg_get_loadavg() {
  // FIXME get real load avg on Winders.
  if (!function_exists('sys_getloadavg')) {
    return 1;
  }

  $load = sys_getloadavg();
  return $load[0];
}
