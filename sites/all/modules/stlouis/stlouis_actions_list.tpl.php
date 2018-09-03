<?php

/**
 * @file stlouis_actions_list.tpl.php
 * List of actions
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  include drupal_get_path('module', $game) . '/' . $game .
    '_actions.inc';
  $game_user = $fetch_user();

  if ($game_user->level < 6) {

    echo <<< EOF
<div class="title">
<img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<p>&nbsp;</p>
<div class="welcome">
  <div class="wise_old_man_small">
  </div>
  <p>&quot;You're not yet influential enough for this page.&nbsp;
  Come back at level 6.&quot;</p>
  <p class="second">&nbsp;</p>
  <p class="second">&nbsp;</p>
  <p class="second">&nbsp;</p>
</div>
<div class="subtitle"><a
  href="/$game/quests/$arg2"><img
    src="/sites/default/files/images/{$game}_continue.png"/></a></div>
EOF;

    db_set_active('default');
    return;

  }

  $fetch_header($game_user);

  if (empty($game_user->username)) {
    db_set_active('default');
    drupal_goto($game . '/choose_name/' . $arg2);
  }
  $sql = 'select name, district from neighborhoods where id = %d;';
  $result = db_query($sql, $game_user->fkey_neighborhoods_id);
  $data = db_fetch_object($result);
  $location = $data->name;
  $district = $data->district;

  $sql = 'select party_title from `values` where id = %d;';
  $result = db_query($sql, $game_user->fkey_values_id);
  $data = db_fetch_object($result);
  $party_title = preg_replace('/^The /', '', $data->party_title);

  $sql_to_add = '';
  $actions_active = 'AND actions.active = 1';

  if ($game_user->meta == 'frozen') {

    echo <<< EOF
<div class="title">Frozen!</div>
<div class="subtitle">You have been tagged and cannot perform any actions</div>
<div class="subtitle">Call on a teammate to unfreeze you!</div>
EOF;

  db_set_active('default');
  return;

  }

//  if ($game == 'stlouis') {
//
//    if (arg(3) == 'banking') {
//      $banking_active = 'active';
//      $actions_type = 'Banking';
//      $order_by = 'actions.id ASC';
//    } else {
//      $normal_active = 'active';
//      $actions_type = 'Normal';
//      $order_by = 'required_level DESC';
//    }

//    echo <<< EOF
//<!--<div class="news">
//  <a href="/$game/actions/$arg2" class="button $normal_active">Normal</a>
//  <a href="/$game/actions/$arg2/banking" class="button $banking_active">Banking</a>
//</div>-->`
//EOF;

//  }

  if ($game_user->level < 20) {
    echo <<< EOF
<ul>
  <li>Use actions to affect your friends and opponents</li>
</ul>
EOF;
  }

  echo <<< EOF
<div class="title">
$actions_type Actions
</div>
EOF;

  $data = actionlist();

  if (count($data) == 0) {

    echo <<< EOF
<div class="subtitle">
  You have no agents and cannot perform any actions.
</div>
<div class="try-an-election-wrapper"><div
  class="try-an-election"><a href="/$game/staff/$arg2">Hire
  agents</a></div></div>
EOF;

    db_set_active('default');
    return;
  }

  foreach ($data as $item) {

    $description = str_replace(['%clan', '%subclan', '%value'],
      ["<em>$party_title</em>", "<em>$subclan_name</em>", $game_user->values],
      $item->description);

    if ($item->cost > 0) {
      $cost = "Cost: $item->cost Action";
    }
    else {
      $cost = '';
    }

    if ($item->values_cost > 0) {
      if (substr($cost, -6) == 'Action') {
        $cost .= ', ';
      }
      $cost .= "$item->values_cost $game_user->values";
    }

    if (!empty($item->fkey_equipment_id)) {
      $image = "/sites/default/files/images/equipment/$game-$item->fkey_equipment_id.png";
    }
    else {
      $image = "/sites/default/files/images/staff/$game-$item->fkey_staff_id.png";
    }

    if (is_file($_SERVER['DOCUMENT_ROOT'] .
      "/sites/default/files/images/actions/$game-{$item->id}.png"))
      $image = "/sites/default/files/images/actions/$game-$item->id.png";

    if ($item->target == 'none') {
      $target = t('Your');
    }
    else {
      $target = t('Target\'s');
    }

    $name = str_replace(['%clan', '%subclan', '%value'],
      ["<em>$party_title</em>", "<em>$subclan_name</em>", $game_user->values],
      $item->name);

    if ($item->active == 0) $name .= ' (inactive)';

    echo <<< EOF
<div class="land">
  <div class="land-icon"><img src="$image" border="0" width="96"></div>
  <div class="land-details">
    <div class="land-name">$name</div>
    <div class="land-description">$description</div>
    <div class="land-action-cost">$cost</div>
EOF;

    if ($item->influence_change < 0) {

      $inf_change = -$item->influence_change;

      echo <<< EOF
    <div class="land-payout negative">Effect: $target $experience is
      reduced by $inf_change</div>
EOF;

    }

    if ($item->influence_change > 0) {

      $inf_change = $item->influence_change;

      echo <<< EOF
    <div class="land-payout">Effect: $target $experience is
      increased by $inf_change</div>
EOF;

    }

    if (($item->rating_change < 0.10) && ($item->rating_change != 0.0)) {

      $rat_change = abs($item->rating_change);

      if ($item->rating_change < 0.0) {

        echo <<< EOF
    <div class="land-payout negative">Effect: $target approval rating is
      reduced by $rat_change%</div>
EOF;

      }
      else {

        echo <<< EOF
    <div class="land-payout">Effect: $target approval rating is
      increased by $rat_change%</div>
EOF;

      }

    }

    if ($item->rating_change >= 0.10) {

      $rat_change = $item->rating_change;

      echo <<< EOF
    <div class="land-payout">Effect: Your approval rating is
      increased by $rat_change%</div>
EOF;

    }

    if ($item->values_change > 0) {

      echo <<< EOF
    <div class="land-payout">Effect: $target $game_user->values is
      increased by $item->values_change</div>
EOF;

    }

    if ($item->values_change < 0) {

      $val_change = -$item->values_change;

      echo <<< EOF
    <div class="land-payout negative">Effect: $target $game_user->values is
      decreased by $val_change</div>
EOF;

    }

    if ($item->actions_change > 0) {

      echo <<< EOF
    <div class="land-payout">Effect: $target Action is
      increased by $item->actions_change</div>
EOF;

    }

    if ($item->actions_change < 0) {

      $val_change = -$item->actions_change;

      echo <<< EOF
    <div class="land-payout negative">Effect: $target Action is
      decreased by $val_change</div>
EOF;

    }

    if ($item->neighborhood_rating_change < 0.0) {

      $rat_change = -$item->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout negative">Effect: Neighborhood $beauty_lower
      rating is reduced by $rat_change</div>
EOF;

    }

    if ($item->neighborhood_rating_change > 0.0) {

      $rat_change = $item->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout">Effect: Neighborhood $beauty_lower rating is
      increased by $rat_change</div>
EOF;

    }

    // Competencies.
    if ($item->competency_enhanced_1 != 0) {
      echo <<< EOF
    <div class="land-payout">
      Effect: Competency enhanced:
      <span class="initial-caps">$item->competency_name_1</span>
    </div>
EOF;
    }
    if ($item->competency_enhanced_2 != 0) {
      echo <<< EOF
    <div class="land-payout">
      Effect: Competency enhanced:
      <span class="initial-caps">$item->competency_name_2</span>
    </div>
EOF;
    }

    echo <<< EOF
  </div>
  <form action="/$game/actions_do/$arg2/$item->id">
EOF;

  $get_value = '_' . $game . '_get_value';
  $next_major_action_time = game_get_value($game_user, 'next_major_action');
  $next_major_action_time_remaining = !empty($next_major_action_time) ?
    (int) $next_major_action_time - REQUEST_TIME : 0;

  if (($next_major_action_time_remaining > 0) && ($item->major_action > 0)) {

    $hours_remaining = sprintf('%02d',
      floor($next_major_action_time_remaining / 3600));
    $minutes_remaining_in_sec = $next_major_action_time_remaining % 3600;
    $minutes_remaining = sprintf('%02d',
      floor($minutes_remaining_in_sec / 60));
    $seconds_remaining = floor($minutes_remaining_in_sec % 60);

    echo <<< EOF
    <div class="land-perform-button-wrapper">
      <input class="land-perform-button not-yet" type="button" value="Not Yet"/>
    </div>
EOF;
       echo <<< EOF
       Available in $hours_remaining:$minutes_remaining:$seconds_remaining
EOF;

  }
  else {
    echo <<< EOF
    <div class="land-perform-button-wrapper">
      <input class="land-perform-button" type="submit" value="Do it"/>
    </div>
EOF;

    if ($item->target != 'none') {

      echo <<< EOF
    <div class="target">
      <select name="target">
        <option value="0">Select one</option>
EOF;

      // Which target?

      // Expensive query - goes to slave.
//      db_set_active('game_' . $game . '_slave1');
      switch ($item->target) {

        // Users in your clan.
        case 'clan':

        // Users in your neighborhood.
        case 'neighborhood':

        case 'neighborhood_higher_than_you_but_still_debateable':

        // People in your neighborhood who aren't on your wall nor in your clan
        // who are a higher level than you but are still debateable.
        case 'neighborhood_not_met':

        // People in your neighborhood who aren't on your wall nor in your clan.
        case 'neighborhood_no_official_not_home':

        // Non-party users who aren't officials.
        case 'neighborhood_no_official_not_home_not_babylonian':

        // All officials in your hood.
        case 'neighborhood_officials':

        // Type 1 in your hood, type 2 in your party, and all type 3.
        case 'officials':

        $data2 = game_action_target_list($item->target, $game_user);
          break;

        case 'officials_type_1':

          // Type 1 elected officials only.
          $data2 = [];
          $sql = 'SELECT elected_positions.id AS ep_id,
            elected_positions.group as ep_group,
            elected_positions.name AS ep_name, blah.*,
            clan_members.is_clan_leader, clans.acronym AS clan_acronym
            FROM elected_positions
            RIGHT JOIN (

-- type 1: neighborhood positions

            SELECT elected_officials.fkey_elected_positions_id,
              elected_officials.approval_rating, users.*
            FROM elected_officials
            LEFT JOIN users ON elected_officials.fkey_users_id = users.id
            LEFT JOIN elected_positions
              ON elected_positions.id = elected_officials.fkey_elected_positions_id
            WHERE users.fkey_neighborhoods_id = %d
            AND elected_positions.type = 1
            ) AS blah ON blah.fkey_elected_positions_id = elected_positions.id

            LEFT OUTER JOIN clan_members
              ON clan_members.fkey_users_id = blah.id
            LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
            ORDER BY elected_positions.energy_bonus DESC, ep_id ASC;';

          $result = db_query($sql, $game_user->fkey_neighborhoods_id,
            $game_user->fkey_values_id);
          while ($official = db_fetch_object($result)) $data2[] = $official;

          break;

        case 'officials_type_2':

          // Type 2 elected officials only.
          $data2 = [];
          $sql = 'SELECT elected_positions.id AS ep_id,
            elected_positions.group as ep_group,
            elected_positions.name AS ep_name, blah.*,
            clan_members.is_clan_leader, clans.acronym AS clan_acronym
            FROM elected_positions
            RIGHT JOIN (
            SELECT elected_officials.fkey_elected_positions_id,
              elected_officials.approval_rating, users.*
            FROM elected_officials
            LEFT JOIN users ON elected_officials.fkey_users_id = users.id
            LEFT JOIN elected_positions
              ON elected_positions.id = elected_officials.fkey_elected_positions_id
            WHERE users.fkey_values_id = %d
            AND elected_positions.type = 2
            ) AS blah ON blah.fkey_elected_positions_id = elected_positions.id

            LEFT OUTER JOIN clan_members
              ON clan_members.fkey_users_id = blah.id
            LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
            ORDER BY elected_positions.energy_bonus DESC, ep_id ASC;';

          $result = db_query($sql, $game_user->fkey_values_id);
          while ($official = db_fetch_object($result)) $data2[] = $official;

          break;

        case 'party':

          // Users in your political party.
          $data2 = [];
          $sql = 'SELECT users.username, users.id,
            clan_members.is_clan_leader, clans.acronym AS clan_acronym,
            NULL as ep_name
            FROM users
            LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = users.id
            LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
            WHERE fkey_values_id = %d
            AND users.id <> %d
            AND users.username <> ""
            ORDER BY username ASC;';

          $result = db_query($sql, $game_user->fkey_values_id, $game_user->id);
          while ($user = db_fetch_object($result)) $data2[] = $user;

          break;

        case 'wall_no_official':

          // Users on your wall who aren't officials.
          $data2 = [];
          $sql = 'SELECT DISTINCT user_messages.fkey_users_from_id AS id,
            users.username, clan_members.is_clan_leader,
            clans.acronym AS clan_acronym, NULL AS ep_name
            FROM users

            LEFT JOIN user_messages
              ON user_messages.fkey_users_from_id = users.id

            LEFT OUTER JOIN clan_members
              ON clan_members.fkey_users_id = users.id

            LEFT OUTER JOIN clans
              ON clan_members.fkey_clans_id = clans.id

            LEFT OUTER JOIN elected_officials
              ON users.id = elected_officials.fkey_users_id

            WHERE user_messages.fkey_users_to_id = %d
            AND users.id <> %d
            AND elected_officials.id IS NULL

            ORDER BY username ASC ;';

          $result = db_query($sql, $game_user->id, $game_user->id);
          while ($user = db_fetch_object($result)) $data2[] = $user;

          break;

      }

      // Reset to master.
//      db_set_active('game_' . $game);

      // Too many to list?  Separate by first letter.
      if ($game_user->meta == 'admin' &&
        count($data2) > 250) {

        $letters = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'. 'I', 'J',
          'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
          'X', 'Y', 'Z', 'Others'];

        // Show mini list.
        foreach ($letters as $letter) {

          echo '<option value="letter_' . $letter . '">' .
            $letter . '</option>';

        }

      }
      else {

        // Full list foreach().
        foreach ($data2 as $user) {
// firep($user);

          $clan_acronym = '';

          if (!empty($user->clan_acronym))
            $clan_acronym = "($user->clan_acronym)";

          if ($user->is_clan_leader)
            $clan_acronym .= '*';

          echo '<option value="' . $user->id . '">' .
            substr($user->ep_name . ' ' . $user->username . ' ' . $clan_acronym,
            0, 36) . '</option>';

        }

      }

      echo <<< EOF
      </select>
    </div>
EOF;

    }
  }
    echo <<< EOF
  </form>
</div>
EOF;

  }

  db_set_active('default');
