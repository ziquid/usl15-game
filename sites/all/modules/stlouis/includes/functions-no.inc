<?php

/**
 * @file functions-no.inc
 * Misc NO functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

function NO_sound($name) {

global $game;

echo <<< EOF
<script type="text/javascript">
  var mp3 = document.createElement("audio");
  mp3.setAttribute('src', '/sites/default/files/sounds/{$game}_$name.mp3');
  mp3.load();
  document.documentElement.appendChild(mp3);
  mp3.play();
</script>
EOF;
return;

echo <<< EOF
<audio src="/sites/default/files/sounds/{$game}_$name.mp3" autoplay="true">
<embed src="/sites/default/files/sounds/{$game}_$name.mp3" autostart="true"
loop="false" hidden="true"/>
</audio>
EOF;
}

function NO_clan_required_equipment($game_user, $equipment_id, $req_quantity) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $sql = 'select quantity from clan_equipment_ownership
  where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $equipment_id, $game_user->fkey_clans_id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity){
  $not_yet = $a_start = $a_end = '';
  }
  else{

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

function NO_required_land($game_user, $land_id, $req_quantity) {

  global $game;
  $arg2 = check_plain(arg(2));

  $sql = 'select quantity from land_ownership
  where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $land_id, $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity){

  $not_yet = $a_start = $a_end = '';

  }
  else{

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

function NO_required_elected_position($game_user, $id, $name) {

  global $game;
  $arg2 = check_plain(arg(2));

  if ($game_user->ep_level == $id){

  $not_yet = $a_start = $a_end = '';

  }
  else{

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

function NO_required_competency($game_user, $id, $level, $name) {

  global $game;
  $arg2 = check_plain(arg(2));

  $check = game_competency_level($game_user, $id);
  $comp_name = game_competency_level_name($level, TRUE);

  if ($check->level >= $level){

  $not_yet = $a_start = $a_end = '';

  }
  else{

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

function NO_is_quest_complete($game_user, $quest_id) {

  $sql = 'select percent_complete from quest_completion
  where fkey_users_id = %d and fkey_quests_id = %d;';
  $result = db_query($sql, $game_user->id, $quest_id);
  $item = db_fetch_object($result);

  return ($item->percent_complete >= 100);

}

function NO_is_quest_started($game_user, $quest_id) {

  $sql = 'select percent_complete from quest_completion
  where fkey_users_id = %d and fkey_quests_id = %d;';
  $result = db_query($sql, $game_user->id, $quest_id);
  $item = db_fetch_object($result);

  return ($item->percent_complete > 0);

}

function NO_on_screen($screen) {

  return (arg(1) == $screen);

}

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

function NO_has_sent_message($game_user) {

  $sql = 'SELECT count(id) as count
  FROM user_messages
  WHERE fkey_users_from_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  return ($item->count > 0);

}

function NO_has_equipment($game_user, $id, $quantity) {

  $sql = 'SELECT quantity
  FROM equipment_ownership
  WHERE fkey_users_id = %d
  AND fkey_equipment_id = %d;';
  $result = db_query($sql, $game_user->id, $id);
  $item = db_fetch_object($result);

  return ($item->quantity >= $quantity);

}

function NO_has_clan_equipment($game_user, $id, $quantity) {

  $sql = 'SELECT quantity
  FROM clan_equipment_ownership
  WHERE fkey_clans_id = %d
  AND fkey_equipment_id = %d;';
  $result = db_query($sql, $game_user->fkey_clans_id, $id);
  $item = db_fetch_object($result);

  return ($item->quantity >= $quantity);

}

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

function NO_clan_equipment_gain(&$game_user, $id, $quantity) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from clan_equipment_ownership
  where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $id, $game_user->fkey_clans_id);
  $eo = db_fetch_object($result);

  // Eo entry already there.
  if (!empty($eo)){

    if (($eq->quantity_limit > 0) &&
      (($eo->quantity + $quantity) > $eq->quantity_limit))
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];

    $sql = 'update clan_equipment_ownership set quantity = quantity + %d
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->fkey_clans_id);

  }
  else{

    if (($eq->quantity_limit > 0) && ($quantity > $eq->quantity_limit))
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];

    $sql = 'insert into clan_equipment_ownership
      (fkey_clans_id, fkey_equipment_id, quantity) values
      (%d, %d, %d);';
    $result = db_query($sql, $game_user->fkey_clans_id, $id, $quantity);

  }

  return [TRUE, 'success', 0];

}

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
  if ($eo->quantity == $quantity){

    $sql = 'delete from clan_equipment_ownership
      where fkey_clans_id = %d and fkey_equipment_id = %d;';
    $result = db_query($sql, $game_user->fkey_clans_id, $id);

  }
  else{

    $sql = 'update clan_equipment_ownership set quantity = quantity - %d
      where fkey_equipment_id = %d and fkey_clans_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->fkey_clans_id);

  }

  return TRUE;

}

function NO_election_won($game_user, $id) {
  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  db_query($sql, $game_user->id);

  $sql = 'insert into elected_officials
    (fkey_users_id, fkey_elected_positions_id) values
      (%d, %d);';
  db_query($sql, $game_user->id, $id);

  return TRUE;
}

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
