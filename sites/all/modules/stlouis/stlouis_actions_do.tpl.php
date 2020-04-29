<?php

/**
 * @file
 * Template for special actions.
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

global $game, $phone_id, $action;
include drupal_get_path('module', $game) . '/game_defs.inc';
include drupal_get_path('module', $game) . '/' . $game . '_actions.inc';
include drupal_get_path('module', $game) . '/' . $game . '_actions_do.inc';
$game_user = $fetch_user();

if ($game_user->meta == 'frozen') {
  $fetch_header($game_user);
  db_set_active();

?>
<div class="title">Frozen!</div>
<div class="subtitle">You have been tagged and cannot perform any actions</div>
<div class="subtitle">Call on a teammate to unfreeze you!</div>
<?php

  return;
}

$data = actionlist();
$action_found = FALSE;

foreach ($data as $item) {
  if ($item->id == $action_id) {
    $action = $item;
    $action_found = TRUE;
  }
}

// Hacking!
if ((!$action_found) && (substr($arg2, 0, 3) != 'ai-')) {

  game_karma($game_user, "Trying to perform action that is not available", -20);

  db_set_active();
  drupal_goto($game . '/home/' . $arg2);
}

// FIXME: Show list.
if (substr($_GET['target'], 0, 7) == 'letter_') {

  echo <<< EOF
<div class="title">
$action->name
</div>
<div class="subtitle">
Please select a target
</div>
EOF;

  db_set_active();
  return;
}

// Target is required but no target specified.
if (($_GET['target'] == 0) && ($action->target != 'none')) {

  zg_fetch_header($game_user);
  db_set_active();
  ?>
  <div class="election-failed">Failure</div>
  <div class="subtitle">You must choose a target</div>
  <?php
  zg_button('actions', 'Try again');
  return;
}

// FIXME: what if there is no target?  This entire block of code assumes
// that there is a target.
$sql = 'SELECT username, phone_id, elected_positions.name as ep_name,
  elected_positions.id as ep_id, experience, fkey_neighborhoods_id, luck,
  level from users

  LEFT OUTER JOIN elected_officials
  ON elected_officials.fkey_users_id = users.id
  LEFT OUTER JOIN elected_positions
  ON elected_positions.id = elected_officials.fkey_elected_positions_id

  where users.id = %d';

$result = db_query($sql, $_GET['target']);
$target = db_fetch_object($result);

if (!is_object($target)) {
  $target = new stdClass();
}

$target->id = (int) $_GET['target'];
firep($target, 'Target of Action');

$name = str_replace('%value', $game_user->values, $action->name);
$title = "$name $action->preposition $target->ep_name $target->username";
$action_succeeded = $can_do_again = TRUE;
$outcome_reason = '<div class="land-succeeded">' . t('Success!') .
  '</div>';
$ai_output = 'action-succeeded';

// Check to see if prerequisites are met.
if ($game_user->actions < $action->cost) {
  $action_succeeded = FALSE;
  $outcome_reason = '<div class="land-failed">' . t('Out of Action!') .
    '</div>';
  if (substr($phone_id, 0, 3) == 'ai-') {
    $ai_output = 'action-failed no-action';
  }
  $outcome_reason .= zg_render_button('elders_do_fill',
    'Refill your Action (1&nbsp;Luck)',
    '/action?destination=/' . $game . '/actions/' . $arg2,
    'big-68');
}

if (($game_user->money < $action->values_cost) &&
  ($action->values_cost > 0)) {

  $action_succeeded = FALSE;
  $outcome_reason = '<div class="land-failed">' .
  t('Out of @value!', ['@value' => $game_user->values]) .
    '</div>';

  if (substr($phone_id, 0, 3) == 'ai-') {
    $ai_output = 'action-failed no-money';
  }
  $offer = zg_luck_money_offer($game_user);
  $outcome_reason .= '<div class="try-an-election-wrapper"><div
    class="try-an-election"><a href="/' . $game . '/elders_do_fill/' .
    $arg2 . '/money?destination=/' . $game . '/actions/' . $arg2 .
    '">Receive ' . $offer . ' ' . $game_user->values .
    ' (1&nbsp;Luck)</a></div></div>';
}

$action_function = $game . '_action_' .
  strtolower(str_replace(
    [' ', '%', "'", '.', '(', ')'],
    ['_', '', '', '', '', ''],
    $action->name));

if (!function_exists($action_function)) {
  $action_function = '_' . $action_function . '_function';
}

if ($action_succeeded && function_exists($action_function)) {
  $action_succeeded = $action_function($outcome_reason, $target, $can_do_again, $action);
}

if ($action_succeeded) {

  // Special case for investigate someone.
  if ($action_function == '_stlouis_action_investigate_a_public_official_function') {
    $show_all = '?comp_show_level=curious';
  }
  elseif ($action_function == '_stlouis_action_investigate_a_clan_member_function') {
    $show_all = '?comp_show_level=yes';
  }
  else {
    $show_all = '';
  }

  // Special case for meet someone new in Fairground Park.
  if (($action_function == '_stlouis_action_meet_someone_new_function') &&
    ($game_user->fkey_neighborhoods_id == 80)) {
    $show_all = '?want_jol=yes';
  }

  // Decrement available actions.
  $sql = 'update users set actions = actions - %d where id = %d;';
  $result = db_query($sql, $action->cost,  $game_user->id);

  // Start the actions clock if needed.
  if ($game_user->actions == $game_user->actions_max) {
    $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 180),
    $game_user->id);
  }

  // Affect competencies.
  if ($action->competency_enhanced_1 != 0) {
    game_competency_gain($game_user, (int) $action->competency_enhanced_1);
  }
  if ($action->competency_enhanced_2 != 0) {
    game_competency_gain($game_user, (int) $action->competency_enhanced_2);
  }

  // Affect influence.
  if ($action->influence_change != 0) {
    $inf_change = $action->influence_change;

    // No target - actions affect player.
    if ($action->target == 'none') {
      $target_name = 'Your';
      $target_id = $game_user->id;
    }

    // There is a target involved.
    else {
      $target_name = $target->ep_name . ' ' . $target->username . '\'s';
      $target_id = $_GET['target'];
    }

    $sql = 'update users set experience = greatest(experience + %d, 0)
      where id = %d;';
    $result = db_query($sql, $inf_change,  $target_id);
    $outcome_reason .= '<div class="action-effect">' . $target_name .
         ' ' . $experience . ' is ' .
    (($inf_change > 0) ? 'increased' : 'decreased') .
        ' by ' . abs($inf_change) . '</div>';

    // Save the record of what happened, if positive and not done to yourself.
    if (($game_user->id != $target_id) && ($inf_change > 0)) {
      $sql = 'insert into challenge_history
        (type, fkey_from_users_id, fkey_to_users_id, fkey_neighborhoods_id,
        fkey_elected_positions_id, won, desc_short, desc_long) values
        ("gift", %d, %d, %d, %d, %d, "%s", "%s");';
      $result = db_query($sql, $game_user->id, $target_id,
        $game_user->fkey_neighborhoods_id, $target->ep_id, 0,
        "$game_user->username gave a gift of $experience to " .
        substr($target_name, 0, strlen($target_name) - 2) . '.',
        "$game_user->username gave a gift of $inf_change $experience " .
        'to ' . substr($target_name, 0, strlen($target_name) - 2) .
        ' (currently ' . $target->experience . ').');
    }

  }

  // Affect ratings.
  if ($action->rating_change != 0) {

    $rat_change = $action->rating_change;

    if (($action->target == 'none') || ($action->rating_change >= 0.10)) {

      // No target or larger positive ratings - actions affect player.

      $target_name = 'Your';
      $target_id = $game_user->id;

    }
    else {

      // Target, negative, or smaller positive ratings.
      $target_name = $target->ep_name . ' ' . $target->username . '\'s';
      $target_id = $_GET['target'];

    }

    // Affect rating.
    $sql = 'update elected_officials
      set approval_rating = greatest(least(approval_rating + %f, 100), 0)
      where fkey_users_id = %d;';
    $result = db_query($sql, $rat_change,  $target_id);

    // Get new rating.
    $sql = 'select approval_rating from elected_officials
        where fkey_users_id = %d;';
    $result = db_query($sql, $target_id);
    $rating = db_fetch_object($result);

    $outcome_reason .= '<div class="action-effect">' . $target_name .
        ' approval rating is changed by ' .
    $rat_change . '% (now at ' . $rating->approval_rating . '%)</div>';

  }

  // Change hood rating.
  if ($action->neighborhood_rating_change != 0) {

    $rat_change = $action->neighborhood_rating_change;

    // Affect rating.
    $sql = 'update neighborhoods
        set rating = greatest(0, rating + %f) where id = %d;';
    $result = db_query($sql, $rat_change,  $game_user->fkey_neighborhoods_id);

    // Get new rating.
    $sql = 'select name, rating from neighborhoods
        where id = %d;';
    $result = db_query($sql, $game_user->fkey_neighborhoods_id);
    $hood = db_fetch_object($result);

    $outcome_reason .= '<div class="action-effect">' . $hood->name .
        '\'s neighborhood ' . $beauty_lower . ' rating is changed by ' .
    $rat_change . ' (now at ' . $hood->rating . ')</div>';

  }

  // Values COST (ie, what you pay).
  if ($action->values_cost != 0) {
    $sql = 'update users set money = money - %d where id = %d;';
    $result = db_query($sql, $action->values_cost, $game_user->id);
    $outcome_reason .= '<div class="action-effect">Your ' .
    $game_user->values . ' is decreased by ' . $action->values_cost .
        '</div>';
  }

  // Values CHANGE (ie, what target gets).
  if ($action->values_change != 0) {

    $target_name = $target->ep_name . ' ' . $target->username . '\'s';
    $target_id = $_GET['target'];

    $sql = 'select id, money, meta_int from users where id = %d;';
    $result = db_query($sql, $target_id);

    if ($action->values_change > 0) {
      $verb = 'increased';
      $money = $action->values_change;
    }
    else {
      $verb = 'decreased';
      $item = db_fetch_object($result);
      $money = -min(-$action->values_change, $item->money);
      zg_alter('actions_do_money_lost', $item, $action, $money);
    }

    $sql = 'update users set money = money + %d where id = %d;';
    $result = db_query($sql, $money, $target_id);
    $outcome_reason .= '<div class="action-effect">' . $target_name . ' ' .
      $game_user->values . ' is ' . $verb . ' by ' . abs($money) . '</div>';

    if ($action->values_change < 0) {
      $can_do_again = FALSE;
      $money = abs(floor($money / 2));
      $sql = 'update users set money = money + %d where id = %d;';
      db_query($sql, $money, $game_user->id);
      $outcome_reason .= '<div class="action-effect">You gain half</div>';
      zg_alter('actions_do_half_money_gained', $game_user, $action, $money);
    }

  }

  if ($action->actions_change != 0) {

    $target_name = $target->ep_name . ' ' . $target->username . '\'s';
    $target_id = $_GET['target'];

    $sql = 'select actions from users where id = %d;';
    $result = db_query($sql, $target_id);

    if ($action->actions_change > 0) {
      $verb = 'increased';
      $act_change = $action->actions_change;
    }
    else {
      $verb = 'decreased';
      $item = db_fetch_object($result);
      $act_change = -min(-$action->actions_change, $item->actions);
    }

    $sql = 'update users set actions = greatest(actions + %d, 0) where id = %d;';
    $result = db_query($sql, $act_change, $target_id);
    $outcome_reason .= '<div class="action-effect">' . $target_name .
     ' Action is ' . $verb . ' by ' . abs($act_change) . '</div>';

    if ($action->actions_change < 0) {
      $can_do_again = FALSE;

      $sql = 'update users set actions = actions + %d where id = %d;';
      $result = db_query($sql, abs(floor($act_change / 2)), $game_user->id);
      $outcome_reason .= '<div class="action-effect">You gain half</div>';

    }

  }

  // Chance of loss - equipment.
  if ($action->fkey_equipment_id) {

    $sql = 'select * from equipment where id = %d;';
    $result = db_query($sql, $action->fkey_equipment_id);
    $eq = db_fetch_object($result);
firep($eq);

    // Did it wear out?
    // 110 instead of 100% to give a little extra chance of having it work.
    if ($eq->chance_of_loss >= mt_rand(1, 110)) {

//firep($eq->name . ' wore out!');
      game_equipment_use($game_user, $eq->id, 1);
      // FIXME: do this before _stlouis_header so that upkeep is accurate.

      $stuff = strtolower($eq->name);
      if (substr($stuff, 0, 2) == 'a ') $stuff = substr($stuff, 2);

      $sql = 'select message from equipment_failure_reasons
        where fkey_equipment_id = %d
        order by rand() limit 1;';
      $result = db_query($sql, $eq->id);
      $failure = db_fetch_object($result);

      if ($failure->message != '') {
        $outcome_reason .= '<div class="subtitle">' . $failure->message .
          '</div>';
      }
      else {
        $outcome_reason .= '<div class="subtitle">' .
          t('Your @stuff has/have worn out',
          ['@stuff' => $stuff]) . '</div>';
      }

      $sql = 'select quantity from equipment_ownership
        where fkey_equipment_id = %d and fkey_users_id = %d;';
      $result = db_query($sql, $eq->id, $game_user->id);
      $eo = db_fetch_object($result);

      if ($eo->quantity == 0) {
        $can_do_again = FALSE;
      }

    }
    else {
      $outcome_reason .= '<div class="subtitle">&nbsp;</div>';
    }

  }

  // Chance of loss - aides.
  // Any staff for this action?
  if ($action->fkey_staff_id) {

    $sql = 'select * from staff where id = %d;';
    $result = db_query($sql, $action->fkey_staff_id);
    $st = db_fetch_object($result);
firep($st);

    // Did it wear out?
    if ($st->chance_of_loss >= mt_rand(1, 110)) {

firep($st->name . ' has run away!');
      $sql = 'update staff_ownership set quantity = quantity - 1
        where fkey_staff_id = %d and fkey_users_id = %d;';
      $result = db_query($sql, $st->id, $game_user->id);

      // Player expenses need resetting?
      // Subtract upkeep from your expenses.
      if ($st->upkeep > 0) {
        $sql = 'update users set expenses = expenses - %d where id = %d;';
        $result = db_query($sql, $st->upkeep, $game_user->id);

      // FIXME: do this before _stlouis_header so that upkeep is accurate.
      }

      $sql = 'select message from staff_failure_reasons
        where fkey_staff_id = %d
        order by rand() limit 1;';
      $result = db_query($sql, $st->id);
      $failure = db_fetch_object($result);

      if ($failure->message != '') {
        $outcome_reason .= '<div class="subtitle">' . $failure->message .
          '</div>';
      }
      else {
        $outcome_reason .= '<div class="subtitle">' .
          t('Your @staff has/have run away or been caught',
            ['@staff' => strtolower($st->name)]) . '</div>';
      }

      $sql = 'select quantity from staff_ownership
        where fkey_staff_id = %d and fkey_users_id = %d;';
      $result = db_query($sql, $st->id, $game_user->id);
      $so = db_fetch_object($result);

      if ($so->quantity == 0) {
        $can_do_again = FALSE;
      }

    }
    else {
      $outcome_reason .= '<div class="subtitle">&nbsp;</div>';
    }

  }

  $outcome_reason .= zg_render_button('user', 'View ' . $target->ep_name . ' ' . $target->username, '/id:' . $_GET['target'], 'big-68');

  if ($can_do_again) {
    $outcome_reason .= zg_render_button('actions_do', "Do it again", '/' . $action_id . '?target=' . $_GET['target'], 'big-68');
  }
  else {
    $outcome_reason .= zg_render_button('', "Can't do it again", '', 'big-68');
  }

  $outcome_reason .= zg_render_button('actions', 'Perform a different action', '', 'big-68');

  // Reprocess user object.
  $game_user = $fetch_user();

}
else {

  // Failed - try a different action.
  $outcome_reason .= zg_render_button('actions', 'Perform a different action', '', 'big-68');
  $ai_output = 'action-failed';
}

$fetch_header($game_user);

  echo <<< EOF
<div class="title">
$title
</div>
$outcome_reason
EOF;

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"$ai_output " .
    filter_xss($outcome_reason, []) .
    " \"/>\n-->";
}

db_set_active();
