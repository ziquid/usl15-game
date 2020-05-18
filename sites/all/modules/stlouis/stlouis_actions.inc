<?php

/**
 * @file stlouis_actions.inc
 * Stlouis actions
 *
 * Synced with CG: no
 * Synced with 2114: no
 */

function actionlist() {

  global $game, $phone_id;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $game_user = $fetch_user();

  $sql_to_add = '';
  $actions_active = 'AND actions.active = 1';

//  if (arg(3) == 'banking') {
//    $banking_active = 'active';
//    $actions_type = 'Banking';
//    $order_by = 'actions.id ASC';
//  }
//  else {
    $normal_active = 'active';
    $actions_type = 'Normal';
    $order_by = 'actions.required_level DESC';
//  }

  // Part of a clan?
  $sql = 'select * from {clan_members}
    left join clans on clans.id = clan_members.fkey_clans_id
    where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);
//  firep($item);

  // Part of a clan - exclude all those that need no clan.
  if (!empty($item)) {
    $sql_to_add .= ' and actions.need_no_clan <> 1 ';
    $subclan_name = "$item->name ($item->acronym)";
  }
  else {
    $sql_to_add .= ' and actions.need_clan <> 1 ';
  }

  if (!empty($item->is_clan_leader)) {

  // Clan leader - exclude all those that need no clan leader.

// We don't have any actions that need no clan leader, so line commented out below.
//    $sql_to_add .= ' and actions.need_no_clan_leader <> 1 ';

  }
  else {
    $sql_to_add .= ' and actions.need_clan_leader <> 1 ';
  }

  // Elected official?
  $sql = 'select *, elected_positions.type, elected_positions.group
    from elected_officials

    left join elected_positions
    on elected_officials.fkey_elected_positions_id = elected_positions.id

    where fkey_users_id = %d;';

  // Limited to 1 in db.
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);
//  firep($item);

  if (!empty($item)) {

    // An elected official - exclude all those that need no elected official.
    $sql_to_add .= ' and actions.need_no_elected_official <> 1
      and (actions.need_specific_elected_official = 0 or
      actions.need_specific_elected_official = ' .
      $item->fkey_elected_positions_id . ')
      and (actions.need_specific_official_group = 0 or
      actions.need_specific_official_group = ' .
      $item->group . ')';

  }
  else {
    $sql_to_add .= ' and actions.need_elected_official <> 1
      and actions.need_specific_elected_official = 0
      and actions.need_specific_official_group = 0';
  }

  // For testing - exclude all exclusions for admin.
  if ($game_user->meta == 'admin') {
    $sql_to_add = '';
    $actions_active = 'and (actions.active = 1 or actions.active = 0)';
  }

  // Actions that require staff.
  $data = [];
  $sql = 'SELECT staff_ownership.*, actions.*, staff.required_level,
  comp1.name as competency_name_1, comp2.name as competency_name_2
  from staff_ownership

  left join actions on staff_ownership.fkey_staff_id = actions.fkey_staff_id
  left join staff on staff_ownership.fkey_staff_id = staff.id
  left join competencies as comp1 on competency_enhanced_1 = comp1.id
  left join competencies as comp2 on competency_enhanced_2 = comp2.id

  where
  (staff_ownership.fkey_users_id = %d OR staff_ownership.fkey_users_id = 0)

  ' . $actions_active .
  ' AND quantity > 0 ' . $sql_to_add .
  ' ORDER BY ' . $order_by;
//  firep($sql, 'actions with staff');
  $result = db_query($sql, $game_user->id, $sql_to_add);

  while ($item = db_fetch_object($result)) {
    $item->competency_name_1 = game_competency_name($item->competency_name_1);
    $item->competency_name_2 = game_competency_name($item->competency_name_2);
    $data[] = $item;
  }

  // Actions that require equipment.
  $sql = 'SELECT equipment_ownership.*, actions.*, equipment.required_level,
  comp1.name as competency_name_1, comp2.name as competency_name_2
  from equipment_ownership

  left join actions on
  equipment_ownership.fkey_equipment_id = actions.fkey_equipment_id

  left join equipment on
  equipment_ownership.fkey_equipment_id = equipment.id

  left join competencies as comp1 on competency_enhanced_1 = comp1.id
  left join competencies as comp2 on competency_enhanced_2 = comp2.id

  where
  (equipment_ownership.fkey_users_id = %d
  OR equipment_ownership.fkey_users_id = 0)

  ' . $actions_active .
  ' AND quantity > 0 ' . $sql_to_add .
  ' ORDER BY actions.required_level DESC';
//  firep($sql, 'actions with equipment');
  $result = db_query($sql, $game_user->id);
  while ($item = db_fetch_object($result)) {
    $item->competency_name_1 = game_competency_name($item->competency_name_1);
    $item->competency_name_2 = game_competency_name($item->competency_name_2);
    $data[] = $item;
  }

  // Actions that require neither staff nor equipment.
  $sql = 'select actions.*,
  comp1.name as competency_name_1, comp2.name as competency_name_2
  from {actions}

  left join competencies as comp1 on competency_enhanced_1 = comp1.id
  left join competencies as comp2 on competency_enhanced_2 = comp2.id

  where actions.fkey_staff_id = 0 and actions.fkey_equipment_id = 0
  ' . $actions_active . ' ' . $sql_to_add .
  ' ORDER BY actions.name ASC';
//  firep($sql, 'actions without staff or equipment');
  $result = db_query($sql, $game_user->id);

  while ($item = db_fetch_object($result)) {
    $item->competency_name_1 = game_competency_name($item->competency_name_1);
    $item->competency_name_2 = game_competency_name($item->competency_name_2);
    $data[] = $item;
  }

  foreach ($data as &$action) {
    $game_preaction_function = 'game_preaction_' .
      strtolower(str_replace(
        [' ', '%', "'", '.', '(', ')'],
        ['_', '', '', '', '', ''],
        $action->name));
    $zg_preaction_function = 'zg_preaction_' .
      strtolower(str_replace(
        [' ', '%', "'", '.', '(', ')'],
        ['_', '', '', '', '', ''],
        $action->name));

    if (function_exists($game_preaction_function)) {
      $game_preaction_function($action);
      firep($action, 'action after game_preaction');
    }
    if (function_exists($zg_preaction_function)) {
      $zg_preaction_function($game_user, $action);
      firep($action, 'action after zg_preaction');
    }
  }

  return $data;
}

function game_action_target_list($target, $game_user) {

// Return a list of valid targets.

// Expensive query - goes to slave.
//   db_set_active('game_' . $game . '_slave1');

  $data2 = [];
  switch ($target) {

    case 'clan':

      // Users in your clan.
      $sql = 'SELECT users.username, users.id,
        clan_members.is_clan_leader, clans.acronym AS clan_acronym,
        NULL as ep_name
        FROM clan_members
        LEFT JOIN users ON clan_members.fkey_users_id = users.id
        LEFT JOIN clans ON clan_members.fkey_clans_id = clans.id
        WHERE fkey_clans_id = %d
        AND users.id <> %d
        ORDER BY level DESC;';

      $result = db_query($sql, $game_user->fkey_clans_id, $game_user->id);
      while ($user = db_fetch_object($result)) {
        $data2[] = $user;
      }
      break;

    case 'neighborhood':

      // Users in your neighborhood.
      $sql = 'SELECT users.username, users.id,
        clan_members.is_clan_leader, clans.acronym AS clan_acronym,
        NULL as ep_name
        FROM users
        LEFT OUTER JOIN clan_members ON clan_members.fkey_users_id = users.id
        LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
        WHERE fkey_neighborhoods_id = %d
        AND users.id <> %d
        AND users.username <> ""
        ORDER BY username ASC;';

      $result = db_query($sql, $game_user->fkey_neighborhoods_id,
        $game_user->id);
      while ($user = db_fetch_object($result)) $data2[] = $user;

      break;

    case 'neighborhood_higher_than_you_but_still_debateable':

      // People in your neighborhood who aren't on your wall nor in your clan
      // who are a higher level than you but are still debateable.
      $sql = 'SELECT users.id, users.username,
        clan_members.is_clan_leader, clans.acronym AS clan_acronym,
        NULL as ep_name
        FROM users
        LEFT OUTER JOIN clan_members
          ON clan_members.fkey_users_id = users.id
        LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
        LEFT OUTER JOIN elected_officials
          ON elected_officials.fkey_users_id = users.id
        LEFT OUTER JOIN `values` ON users.fkey_values_id = `values`.id
        WHERE users.fkey_neighborhoods_id = %d
          AND users.level >= %d
--          AND debates_last_time < "%s"
          AND users.id <> %d
          AND users.id not in
-- all clan members that are in his/her clan
            (select fkey_users_id as id from clan_members
              where fkey_clans_id = %d
            UNION
-- all users that have posted to his/her wall
            select fkey_users_from_id as id from user_messages
              where fkey_users_to_id = %d
            UNION
-- all users to which user has posted
            select fkey_users_to_id as id from user_messages
              where fkey_users_from_id = %d)
              AND username <> ""
        ORDER BY RAND()
        LIMIT 3;';

      $result = db_query($sql, $game_user->fkey_neighborhoods_id,
        $game_user->level, date('Y-m-d H:i:s', REQUEST_TIME - $debate_time),
        $game_user->id, $game_user->fkey_clans_id, $game_user->id,
        $game_user->id);
      while ($user = db_fetch_object($result)) $data2[] = $user;

      break;

    case 'neighborhood_not_met':

      // People in your neighborhood who aren't on your wall nor in your clan.
      $sql = 'SELECT users.id, users.username,
         clan_members.is_clan_leader, clans.acronym AS clan_acronym,
         NULL as ep_name
        FROM users

        LEFT OUTER JOIN clan_members
          ON clan_members.fkey_users_id = users.id

        LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id

        LEFT OUTER JOIN elected_officials
          ON elected_officials.fkey_users_id = users.id

        LEFT OUTER JOIN `values` ON users.fkey_values_id = `values`.id

        WHERE users.fkey_neighborhoods_id = %d
          AND users.id <> %d
          AND users.id not in

-- all clan members that are in his/her clan
            (select fkey_users_id as id from clan_members
              where fkey_clans_id = %d

            UNION
-- all users that have posted to his/her wall
            select fkey_users_from_id as id from user_messages
              where fkey_users_to_id = %d

            UNION
-- all users to which user has posted
            select fkey_users_to_id as id from user_messages
              where fkey_users_from_id = %d)
              AND username <> ""

        ORDER BY RAND()
        LIMIT 3;';

      $result = db_query($sql, $game_user->fkey_neighborhoods_id,
        $game_user->id, $game_user->fkey_clans_id, $game_user->id,
        $game_user->id);

      $allowed_string = '';
      while ($user = db_fetch_object($result)) {
        $data2[] = $user;
        $allowed_string .= $user->id . ',';
      }

      $allowed_string = substr($allowed_string, 0, strlen($allowed_string) - 1);
firep('allowed not_met users: ' . $allowed_string);
      break;


    case 'neighborhood_no_official_not_home':

      // Non-party users who aren't officials.
      $sql = 'SELECT users.id, users.username,
        clan_members.is_clan_leader, clans.acronym AS clan_acronym,
        NULL as ep_name
        FROM users
        LEFT OUTER JOIN clan_members
          ON clan_members.fkey_users_id = users.id
        LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
        LEFT OUTER JOIN elected_officials
          ON elected_officials.fkey_users_id = users.id
        LEFT OUTER JOIN elected_positions
          ON elected_officials.fkey_elected_positions_id =
          elected_positions.id
        LEFT OUTER JOIN `values` ON users.fkey_values_id = `values`.id
        WHERE users.fkey_neighborhoods_id = %d
          AND users.fkey_values_id <> %d
          AND username <> ""
          AND (elected_officials.fkey_elected_positions_id IS NULL
            OR elected_positions.type = 2)
          AND `values`.fkey_neighborhoods_id <> %d
          AND meta <> "admin"
        ORDER BY level ASC ;';

      $result = db_query($sql, $game_user->fkey_neighborhoods_id,
        $game_user->fkey_values_id, $game_user->fkey_neighborhoods_id);
      while ($user = db_fetch_object($result)) {
        $data2[] = $user;
      }

      break;

    case 'neighborhood_officials':

      // All elected officials in your hood.
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
            WHERE users.fkey_neighborhoods_id = %d

            ) AS blah ON blah.fkey_elected_positions_id = elected_positions.id

            LEFT OUTER JOIN clan_members
              ON clan_members.fkey_users_id = blah.id
            LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
            ORDER BY elected_positions.energy_bonus DESC, ep_id ASC;';

      $result = db_query($sql, $game_user->fkey_neighborhoods_id);
      while ($official = db_fetch_object($result)) {
        $data2[] = $official;
      }

      break;

    case 'officials':

      // Elected officials only.
      $data2 = [];
      $sql = 'SELECT DISTINCT elected_positions.id AS ep_id,
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
--            AND elected_positions.type = 1

            UNION

-- type 2: party positions

            SELECT elected_officials.fkey_elected_positions_id,
              elected_officials.approval_rating, users.*
            FROM elected_officials
            LEFT JOIN users ON elected_officials.fkey_users_id = users.id
            LEFT JOIN elected_positions
              ON elected_positions.id = elected_officials.fkey_elected_positions_id
            WHERE users.fkey_values_id = %d
            AND elected_positions.type = 2

            UNION

-- type 3: district positions

            SELECT elected_officials.fkey_elected_positions_id,
              elected_officials.approval_rating, users.*
            FROM elected_officials
            LEFT JOIN users ON elected_officials.fkey_users_id = users.id
            LEFT JOIN elected_positions
              ON elected_positions.id = elected_officials.fkey_elected_positions_id
            WHERE users.fkey_neighborhoods_id IN
              (SELECT id from neighborhoods where district = %d)
            AND elected_positions.type = 3
            
            UNION
            
-- type 4: city positions

            SELECT elected_officials.fkey_elected_positions_id,
              elected_officials.approval_rating, users.*
            FROM elected_officials
            LEFT JOIN users ON elected_officials.fkey_users_id = users.id
            LEFT JOIN elected_positions
              ON elected_positions.id = elected_officials.fkey_elected_positions_id
            WHERE elected_positions.type = 4

            ) AS blah ON blah.fkey_elected_positions_id = elected_positions.id

            LEFT OUTER JOIN clan_members
              ON clan_members.fkey_users_id = blah.id
            LEFT OUTER JOIN clans ON clan_members.fkey_clans_id = clans.id
            ORDER BY elected_positions.energy_bonus DESC, ep_id ASC;';

      $result = db_query($sql, $game_user->fkey_neighborhoods_id,
        $game_user->fkey_values_id, $district);
      while ($official = db_fetch_object($result)) {
        $data2[] = $official;
      }

      break;
  }

// Reset to master.
//  db_set_active('game_' . $game);
  return $data2;
}

// GAME PREACTION FUNCTIONS //

function game_preaction_plant_flower_seeds(&$action) {
  global $game;
  $fetch_user = '_' . $game . '_fetch_user';
  $game_user = $fetch_user();

  $comp_level = game_competency_level($game_user, (int) $action->competency_enhanced_1);
  $action->neighborhood_rating_change += ($comp_level->level * 0.02);
  $action->values_cost += ($comp_level->level * 10);
}

function game_preaction_spread_gossip(&$action) {
  global $game;
  $fetch_user = '_' . $game . '_fetch_user';
  $game_user = $fetch_user();

  $comp_level = game_competency_level($game_user, (int) $action->competency_enhanced_1);
  $action->rating_change -= ($comp_level->level * 0.03);
  $action->values_cost += ($comp_level->level);
}

/**
 * Preaction for steal some value.
 *
 * This function increases the amount stolen based on the user's competency.
 *
 * @param object $game_user
 *   The game user object.
 * @param object $action
 *   The action object.
 */
function zg_preaction_steal_some_value(&$game_user, &$action) {
  $comp_level = zg_competency_level($game_user, (int) $action->competency_enhanced_1);
  $action->values_change -= ($comp_level->level * 20);
  zg_alter('preaction_steal_some_value', $game_user, $action);
}

function game_preaction_support_your_local_official(&$action) {
  global $game;
  $fetch_user = '_' . $game . '_fetch_user';
  $game_user = $fetch_user();

  $comp_level = game_competency_level($game_user, (int) $action->competency_enhanced_1);
  $action->influence_change += ($comp_level->level * 3);
  $action->values_cost += ($comp_level->level);
}

function game_preaction_support_your_party_official(&$action) {
  global $game;
  $fetch_user = '_' . $game . '_fetch_user';
  $game_user = $fetch_user();

  $comp_level = game_competency_level($game_user, (int) $action->competency_enhanced_1);
  $action->influence_change += ($comp_level->level * 3);
  $action->values_cost += ($comp_level->level);
}

function game_preaction_throw_eggs(&$action) {
  global $game;
  $fetch_user = '_' . $game . '_fetch_user';
  $game_user = $fetch_user();

  $comp_level = game_competency_level($game_user, (int) $action->competency_enhanced_1);
  $action->influence_change -= ($comp_level->level * 2);
  $action->neighborhood_rating_change -= ($comp_level->level * 0.01);
  $action->values_cost += ($comp_level->level);
}
