<?php

/**
 * @file functions-no.inc
 * Misc NO functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

/**
 * Show goals
 *
 * @param $game_user
 *   The game user object
*/
function NO_show_goal($game_user) {

  global $game;

  $sql = 'select goals.* from goals
    left join goals_achieved on fkey_goals_id = goals.id
    and fkey_users_id = %d
    where goals_achieved.id is NULL
    order by goals.id asc limit 1;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  if (substr(arg(1), 0, strlen($item->type)) == $item->type) {
    $current = 'current';
  }

  $finished = eval($item->code_to_check);

  if ($finished) {

    // Save an entry showing that this goal has been achieved.
    $sql = 'insert into goals_achieved
      (fkey_users_id, fkey_goals_id)
      values (%d, %d);';
    $result = db_query($sql, $game_user->id, $item->id);

    // Congratulate user.
    $extra_title = <<< EOF
<div class="title goal-achieved">
  Goal Achievement
  <span class="arrows">&raquo; </span>
  <span class="nowrap">$item->title</span>
</div>
EOF;

    $next = t('Next Goal');
    $current = 'current';

    // Find next goal.
    $sql = 'select goals.* from goals
      left join goals_achieved on fkey_goals_id = goals.id
      and fkey_users_id = %d
      where goals_achieved.id is NULL
      order by goals.id asc limit 1;';
    $result = db_query($sql, $game_user->id);
    $item = db_fetch_object($result);

  }
  else {

    $extra_title = '';
    $next = t('Current Goal');

  }

  // No goal.
  if (empty($item->title) && empty($extra_title)) {
    return;
  }

  if (empty($item->title)) {
    $item->title = t('None');
  }

  $description = t($item->description, [
    '@level' => $game_user->level,
  ]);

  echo <<< EOF
<div class="goals $current">
  $extra_title
  <div class="title">$next <span class="arrows">&raquo; </span>
    <span class="nowrap">$item->title</span>
  </div>
  <div class="desc-wrapper">
    <a class="toggle" onClick="javascript:toggle_goal_desc(); return false;"
      href="#">
      +
    </a>
    <p id="goal-desc">$description</p>
  </div>
</div>

<script type="text/javascript">
Element.prototype.hasClass = function(cName) {
  return this.className &&
    new RegExp('(^|\\\\s)' + cName + '(\\\\s|$)').test(this.className);
};
Element.prototype.removeClass = function(className) {
  this.className = this.className.replace(
    new RegExp('(^|\\\\s)' + className + '(\\\\s|$)', 'm'), ''
  );
};

  function toggle_goal_desc() {
    var d = document.getElementById('goal-desc');
    if (d.hasClass('show')) {
      d.removeClass('show');
    }
    else {
      d.className = d.className + ' show';
    }
  }
</script>
EOF;
}

/**
 * Required clan equipment
 *
 * @param $game_user
 *   The game user object.
 * @param int $equipment_id
 *   The object id for equipment.
 * @param int $req_quantity
 *   The number of items required.
*/
function NO_clan_required_equipment($game_user, $equipment_id, $req_quantity) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $sql = 'select quantity from clan_equipment_ownership
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $equipment_id, $game_user->fkey_clans_id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {
    $not_yet = $a_start = $a_end = '';
  }
  else {

    $a_start = $a_end = '';
    $not_yet = 'not-yet';

  }

  echo <<< EOF
<div class="quest-required">
  $a_start
    <img class="$not_yet"
      src="/sites/default/files/images/equipment/$game-$equipment_id.png"
      width="48">
  $a_end
  <div class="quest-required-num">
    Clan
    <br/>
    x$req_quantity
  </div>
</div>
EOF;

}

/**
 * Required land
 *
 * @param $game_user
 *   The game user object.
 * @param int $land_id
 *   The land id.
 * @param $req_quantity
 *   The number of items required.
*/
function NO_required_land($game_user, $land_id, $req_quantity) {

  global $game;
  $arg2 = check_plain(arg(2));

  $sql = 'select quantity from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $land_id, $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/land_buy/' .
      $arg2 . '/' . $land_id . '/' .
      ($req_quantity - $quantity->quantity) . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      <img class="$not_yet"
        src="/sites/default/files/images/land/$game-$land_id.png"
        width="48">
    $a_end
  </div>&nbsp;x$req_quantity
</div>
EOF;

}

/**
 * NO elected position
 *
 * @param $game_user
 *   The game user object.
 * @param int $id
 *   The object id.
 * @param $name
 *   The name of the object.
*/
function NO_required_elected_position($game_user, $id, $name) {

  global $game;
  $arg2 = check_plain(arg(2));

  if ($game_user->ep_level == $id) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/hierarchies/' .
      $arg2 . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      $ep_short: $name
    $a_end
  </div>
</div>
EOF;

}

/**
 * NO competency
 *
 * @param $game_user
 *   The game user object.
 * @param int $id
 *   The object id.
 * @param int $level
 *   The comp. level.
 * @param $name
 *   The name of the object.
*/
function NO_required_competency($game_user, $id, $level, $name) {

  global $game;
  $arg2 = check_plain(arg(2));

  $check = game_competency_level($game_user, $id);
  $comp_name = game_competency_level_name($level, TRUE);

  if ($check->level >= $level) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/user_comp/' .
      $arg2 . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      $name Competency:<br/>
      <span>$comp_name (Level $level)</span>
    $a_end
  </div>
</div>
EOF;

}

/**
 * NO enhanced competency
 *
 * @param $game_user
 *   The game user object.
 * @param int $id
 *   The object id.
 * @param $name
 *   The name of the object.
*/
function NO_enhanced_competency($game_user, $id, $name) {

  global $game;

  echo <<< EOF
<div class="quest-payout">Competency Enhanced:<br/>
  <span class="highlight">
    $name
  </span>
</div>
EOF;

}

/**
 * NO is the quest complete?
 *
 * @param $guest_user
 *   The game user object
 * @param int $quest_id
 *   The id for quest
*/
function NO_is_quest_complete($game_user, $quest_id) {

  $sql = 'select percent_complete from quest_completion
    where fkey_users_id = %d and fkey_quests_id = %d;';
  $result = db_query($sql, $game_user->id, $quest_id);
  $item = db_fetch_object($result);

  return ($item->percent_complete >= 100);

}

/**
 * NO is the quest started?
 *
 * @param $game_user
 *   The game user object
 * @param int $quest_id
 *   The id for quest
*/
function NO_is_quest_started($game_user, $quest_id) {

  $sql = 'select percent_complete from quest_completion
    where fkey_users_id = %d and fkey_quests_id = %d;';
  $result = db_query($sql, $game_user->id, $quest_id);
  $item = db_fetch_object($result);

  return ($item->percent_complete > 0);

}

/**
 * NO on screen
 *
 * @param $screen
 * 
*/
function NO_on_screen($screen) {

  return (arg(1) == $screen);

}

/**
 * NO job
 *
 * @param $game_user
 *   The game user object
*/
function NO_has_job($game_user) {

  $sql = 'SELECT sum(payout) as job_income
    FROM land_ownership AS lo
    LEFT JOIN land ON lo.fkey_land_id = land.id
    WHERE fkey_users_id = %d
    AND land.type = "job";';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  return ($item->job_income > 0);

}

/**
 * NO message sent
 *
 * @param $game_user
 *   The game user object
*/
function NO_has_sent_message($game_user) {

  $sql = 'SELECT count(id) as count
    FROM user_messages
    WHERE fkey_users_from_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  return ($item->count > 0);

}

/**
 * NO equipment
 *
 * @param $game_user
 *   The game user object
 * @param int $id
 *   The object id
 * @param int $quantity
 *   The number of object
*/
function NO_has_equipment($game_user, $id, $quantity) {

  $sql = 'SELECT quantity
    FROM equipment_ownership
    WHERE fkey_users_id = %d
    AND fkey_equipment_id = %d;';
  $result = db_query($sql, $game_user->id, $id);
  $item = db_fetch_object($result);

  return ($item->quantity >= $quantity);

}

/**
 * NO clan equipment
 *
 * @param $game_user
 *   The game user object
 * @param int $id
 *   The object id
 * @param int $quantity
 *   The number of object
*/
function NO_has_clan_equipment($game_user, $id, $quantity) {

  $sql = 'SELECT quantity
    FROM clan_equipment_ownership
    WHERE fkey_clans_id = %d
    AND fkey_equipment_id = %d;';
  $result = db_query($sql, $game_user->fkey_clans_id, $id);
  $item = db_fetch_object($result);

  return ($item->quantity >= $quantity);

}

/**
 * NO show actions menu
 *
 * @param $game_user
 *   The game user object
*/
function NO_show_actions_menu($game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));

  if (arg(3) == '') $normal_active = 'active';
  if (arg(3) == 'clan') $clan_active = 'active';

  echo <<< EOF
<div class="news">
  <a href="/$game/actions/$arg2" class="button $normal_active">Normal</a>
  <a href="/$game/actions/$arg2/clan" class="button $clan_active">Clan</a>
</div>
EOF;

  _show_goal($game_user);

}

/**
 * NO show clan menu
 *
 * @param $game_user
 *   The game user object
*/
function NO_show_clan_menu($game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  $arg3 = check_plain(arg(3));

  if (arg(1) == 'clan_list') $list_active = 'active';
  if (arg(1) == 'clan_msg') $msg_active = 'active';
  if (arg(1) == 'clan_announcements') $ann_active = 'active';
  if (arg(1) == 'clan_admin') $admin_active = 'active';

  echo <<< EOF
<div class="news">
  <a href="/$game/clan_list/$arg2/$arg3"
    class="button $list_active">Members</a>
  <a href="/$game/clan_msg/$arg2/$arg3"
    class="button $msg_active">Messages</a>
  <a href="/$game/clan_announcements/$arg2/$arg3"
    class="button $ann_active">Announcements</a>
  <a href="/$game/clan_list/$arg2/$arg3"
    class="button not-yet $admin_active">Admin</a>
</div>
EOF;

  _show_goal($game_user);

}

/**
 * NO clan equipment gain
 *
 * @param $game_user
 *   The game user object
 * @param int $id
 *   The object id
 * @param int $quantity
 *   The number of object
*/
function NO_clan_equipment_gain(&$game_user, $id, $quantity) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from clan_equipment_ownership
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $id, $game_user->fkey_clans_id);
  $eo = db_fetch_object($result);

  // Eo entry already there.
  if (!empty($eo)) {

    if (($eq->quantity_limit > 0) &&
      (($eo->quantity + $quantity) > $eq->quantity_limit))
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];

    $sql = 'update clan_equipment_ownership set quantity = quantity + %d
      where fkey_equipment_id = %d and fkey_clans_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->fkey_clans_id);

  }
  else {

    if (($eq->quantity_limit > 0) && ($quantity > $eq->quantity_limit))
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];

    $sql = 'insert into clan_equipment_ownership
      (fkey_clans_id, fkey_equipment_id, quantity) values
      (%d, %d, %d);';
    $result = db_query($sql, $game_user->fkey_clans_id, $id, $quantity);

  }

  return [TRUE, 'success', 0];

}

/**
 * NO clan equipment lose
 *
 * @param $game_user
 *   The game user object
 * @param int $id
 *   The object id
 * @param int $quantity
 *   The number of object
*/
function NO_clan_equipment_lose(&$game_user, $id, $quantity) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from clan_equipment_ownership
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $id, $game_user->fkey_clans_id);
  $eo = db_fetch_object($result);

  // Nothing there?
  if (empty($eo)) return FALSE;

  // Not enough?
  if ($eo->quantity < $quantity) return FALSE;

  // Using all?  Remove entry.
  if ($eo->quantity == $quantity) {

    $sql = 'delete from clan_equipment_ownership
      where fkey_clans_id = %d and fkey_equipment_id = %d;';
    $result = db_query($sql, $game_user->fkey_clans_id, $id);

  }
  else {

    $sql = 'update clan_equipment_ownership set quantity = quantity - %d
      where fkey_equipment_id = %d and fkey_clans_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->fkey_clans_id);

  }

  return TRUE;

}

/**
 * NO election won
 *
 * @param $game_user
 *   The game user object
 * @param int $id
 *   The object id
*/
function NO_election_won($game_user, $id) {
  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  db_query($sql, $game_user->id);

  $sql = 'insert into elected_officials
    (fkey_users_id, fkey_elected_positions_id) values
      (%d, %d);';
  db_query($sql, $game_user->id, $id);

  return TRUE;
}

/**
 * NO hierarchy
 *
 * @param $game_user
 *   The game user object
 * @param int $level
 *   The level of the object
*/
function NO_hierarchy_status($game_user, $level) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $ret = new StdClass();

  $sql = 'select * from elected_positions where id = %d;';
  $result = db_query($sql, $level);
  $req = db_fetch_object($result);

  $ret->target[0] =
    t('Attainment: <span>Level @level</span>',
      ['@level' => $req->min_level]);
  $ret->progress[0] = t('Attainment: <span>Level @level</span>',
    ['@level' => $game_user->level]);
  $ret->passed[0] = ($game_user->level >= $req->min_level);

  switch ($level) {

    case 0:

      $ret->qualified = $ret->passed[0];
      break;


    case 1:

      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Harvesting', 1);

      $ret->qualified = $ret->passed[0] & $ret->passed[1];
      break;


    case 2:

      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Cooking', 1);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Harvesting', 2);

      $ret->target[] =
        t('Favors Completed: <span>10</span>');
      $ret->progress[] =
        t('Favors Completed: <span>@c</span>',
        ['@c' => $game_user->favors_completed]);
      $ret->passed[] = ($game_user->favors_completed >= 10);

      $ret->target[] =
        t('Initiated Favors Completed: <span>10</span>');
      $ret->progress[] =
        t('Initiated Favors Completed: <span>@c</span>',
        ['@c' => $game_user->favors_asked_completed]);
      $ret->passed[] = ($game_user->favors_asked_completed >= 10);

      $ret->target[] =
        t('Initiated Favors Not Completed: <span>5</span>');
      $ret->progress[] =
        t('Initiated Favors Not Completed: <span>@c</span>',
        ['@c' => $game_user->favors_asked_noncompleted]);
      $ret->passed[] = ($game_user->favors_asked_noncompleted >= 5);

      $ret->qualified = $ret->passed[0] & $ret->passed[1] & $ret->passed[2] &
        $ret->passed[3] & $ret->passed[4] & $ret->passed[5];
      break;


    case 3:

//      list($ret->target[], $ret->progress[], $ret->passed[]) =
//        _hierarchy_competency_check($game_user, 'Enriching', 1);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Purifying', 1);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Cooking', 2);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Harvesting', 3);

      $ret->target[] = t('Clan membership: <span>TRUE</span>');
      $ret->progress[] = t('Clan membership: <span>@m (@c)</span>',
        [
          '@m' => ($game_user->fkey_clans_id > 0) ? 'TRUE' : 'FALSE',
          '@c' => $game_user->clan_acronym,
        ]
      );
      $ret->passed[] = ($game_user->debates_won >= 10);

      $ret->target[] = t('@debate Wins: <span>10</span>',
        ['@debate' => $debate]);
      $ret->progress[] = t('@debate Wins: <span>@c</span>',
        ['@debate' => $debate, '@c' => $game_user->debates_won]);
      $ret->passed[] = ($game_user->debates_won >= 10);

      $ret->target[] = t('@debate Losses: <span>5</span>',
        ['@debate' => $debate]);
      $ret->progress[] = t('@debate Losses: <span>@c</span>',
        ['@debate' => $debate, '@c' => $game_user->debates_lost]);
      $ret->passed[] = ($game_user->debates_lost >= 5);

      $ret->target[] =
        t('Favors Completed: <span>30</span>');
      $ret->progress[] =
        t('Favors Completed: <span>@c</span>',
        ['@c' => $game_user->favors_completed]);
      $ret->passed[] = ($game_user->favors_completed >= 30);

      $ret->target[] =
        t('Initiated Favors Completed: <span>30</span>');
      $ret->progress[] =
        t('Initiated Favors Completed: <span>@c</span>',
        ['@c' => $game_user->favors_asked_completed]);
      $ret->passed[] = ($game_user->favors_asked_completed >= 30);

      $ret->target[] =
        t('Initiated Favors Not Completed: <span>15</span>');
      $ret->progress[] =
        t('Initiated Favors Not Completed: <span>@c</span>',
        ['@c' => $game_user->favors_asked_noncompleted]);
      $ret->passed[] = ($game_user->favors_asked_noncompleted >= 15);

      $ret->qualified = $ret->passed[0] & $ret->passed[1] & $ret->passed[2] &
        $ret->passed[3] & $ret->passed[4] & $ret->passed[5] &
        $ret->passed[6] & $ret->passed[7] & $ret->passed[8] &
        $ret->passed[9];
      break;

    default:

      $ret->target[] = t('Contemplate Inner Peace');
      $ret->progress[] = t('Not Yet');;
      $ret->passed[1] = FALSE;

      $ret->qualified = $ret->passed[0] & $ret->passed[1];

  }

  return $ret;

}

/**
 * NO hierarchy comp check
 *
 * @param $game_user
 *   The game user object
 * @param $req_comp
 *   The required competency
 * @param int $req_level
 *   The required level
*/
function NO_hierarchy_competency_check($game_user, $req_comp, $req_level = 1) {

  $check = game_competency_level($game_user, $req_comp);
  $target =
    t('@name Competency:<br/>' .
      '<span>@comp (Level @l)</span>',
      [
        '@name' => $req_comp,
        '@comp' => game_competency_level_name($req_level),
        '@l' => $req_level,
      ]
    );
  $progress =
    t('@name Competency:<br/><span>@comp (Level @l)</span>',
      [
        '@name' => $req_comp,
        '@comp' => $check->level_name,
        '@l' => $check->level
      ]
    );
  $passed = ($check->level >= $req_level);

  return [$target, $progress, $passed];

}

/**
 * NO equipment wear
 *
 * @param $game_user
 *   The game user object
 * @param int $id
 *   The object id
*/
function NO_equipment_check_wear_out(&$game_user, $id) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);
  $failure_reason = '&nbsp;';
  $lost = FALSE;

  // Did it wear out?
  if ($eq->chance_of_loss >= mt_rand(1,110)) {
    // 110 instead of 100% to give a little extra chance of having it work.
firep($eq->name . ' wore out!');
    $lost = equipment_lose($game_user, $id, 1);

    $stuff = strtolower($eq->name);
    if (substr($stuff, 0, 2) == 'a ') $stuff = substr($stuff, 2);

    $sql = 'select message from equipment_failure_reasons
      where fkey_equipment_id = %d
      order by rand() limit 1;';
    $result = db_query($sql, $id);
    $failure = db_fetch_object($result);

    if ($failure->message != '') {
      $failure_reason = $failure->message;
    }
    else {
      $failure_reason = t('Your @stuff has/have worn out',
        ['@stuff' => $stuff]);
    }
  }

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  return [$lost, $failure_reason, $eo->quantity];
}

/**
 * NO show favor
 *
 * @param $game_user
 *   The game user object
 * @param $item
 *   The favor object
 * @param $view
 *   The role of the object
*/
function NO_show_favor($game_user, $item, $view = 'initiator', $bounty = -1) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  static $location, $party_title;

  if (empty($location)) {

    $sql = 'select name from `neighborhoods` where id = %d;';
    $result = db_query($sql, $game_user->fkey_neighborhoods_id);
    $data = db_fetch_object($result);
    $location = preg_replace('/^The /', '', $data->name);

  }

  if (empty($party_title)) {

    $sql = 'select party_title from `values` where id = %d;';
    $result = db_query($sql, $game_user->fkey_values_id);
    $data = db_fetch_object($result);
    $party_title = preg_replace('/^The /', '', $data->party_title);

  }

  if (substr($item->description, 0, 3) != '<p ') $laquo = '&laquo;';
  if (substr($item->description, -1) != '>') $raquo = '&raquo;';

  $name = t($item->name, [
    '%party' => $party_title,
    '@username' => $game_user->username,
    '@hood' => $hood,
    '@location' => $location,
  ]);

  if (($view == 'runner') || ($view == 'perform'))
    $item->initiator_description = $item->runner_description;

  $description = t($item->initiator_description, [
    '%party' => $party_title,
    '@username' => $game_user->username,
    '@hood' => $hood,
    '@location' => $location,
  ]);

  $active = ($item->active) ? '' : ' (inactive)';

  if (!empty($item->favor_id)) {
    $item->req_id = $item->id;
    $item->id = $item->favor_id;
  }

  switch ($view) {

    case 'initiator':

      $a_start = "<a href=\"/$game/favors_selected/$arg2/$item->id\">";
      $a_end = '</a>';
      $link_text = 'Select &raquo;&raquo;';
      break;

    case 'runner':

      $a_start = "<a href=\"/$game/favor_perform/$arg2/$item->req_id\">";
      $a_end = '</a>';
      $link_text = 'Perform';
      break;

    case 'perform':

      $a_start = $a_end = $link_text = '';
      break;

  }

  echo <<< EOF
<div class="quests">
  <div class="quest-icon">
    $a_start
      <img src="/sites/default/files/images/favors/$game-$item->id.png?0"
        border="0" width="96"/>
    $a_end
  </div>

  <div class="quest-details">
    <div class="quest-name">
      $a_start
        $active $name
      $a_end
    </div>
    <div class="quest-description">
      $laquo $description $raquo
    </div>
EOF;

  if ($view == 'initiator') echo <<< EOF
    <div class="quest-requisites">
      // Initiator Requisites //
    </div>
    <div class="quest-experience">
      $game_user->values: $item->values_cost
    </div>
    <div class="quest-experience">
      {$actions}: $item->actions_cost
    </div>
EOF;

  if ($view != 'perform') {

    echo <<< EOF
    <div class="quest-requisites">
      // Runner Requisites //
    </div>
    <div class="quest-experience">
      {$actions}: $item->runner_actions_cost
    </div>
EOF;

    // Due date.
    if (!empty($item->time_due)) {

      $time_due = game_format_date(strtotime($item->time_due));

      echo <<< EOF
    <div class="quest-experience">
      Perform By: <span class="highlight">$time_due</span>
    </div>
EOF;

    }

    // Required elected position.
    if ($item->fkey_required_elected_positions_id > 0) {

      _required_elected_position($game_user,
        $item->fkey_required_elected_positions_id, $item->ep_name);

    }

    // Required competencies.
    if ($item->fkey_required_competencies_id > 0 &&
      $item->required_competencies_level > 0) {

      _required_competency($game_user, $item->fkey_required_competencies_id,
        $item->required_competencies_level, $item->cr_name);

    }

  }

  echo <<< EOF
    <div class="quest-requisites">
      // Runner Disbursements //
    </div>
EOF;

  // Enhanced competencies.
  if ($item->fkey_enhanced_competencies_id > 0) {

    _enhanced_competency($game_user, $item->fkey_enhanced_competencies_id,
      $item->ce_name);

  }

  if ($view != 'perform') {

    $bounty_values_min = floor($item->values_cost / 2);
    $bounty_values_max = $item->values_cost * 5;
    $bounty = "$bounty_values_min to $bounty_values_max";

  }

  echo <<< EOF
    $no_reqs
    <div class="quest-disbursements">
      // Bounty //
    </div>
    <div class="quest-experience">
      $bounty $game_user->values
    </div>
  </div>
EOF;

  if ($view != 'perform') echo <<< EOF
  <div class="quest-perform-button-wrapper">
    <div class="quest-perform-button">
      $a_start
        $link_text
      $a_end
    </div>
  </div>
EOF;

  echo '</div>';
}
