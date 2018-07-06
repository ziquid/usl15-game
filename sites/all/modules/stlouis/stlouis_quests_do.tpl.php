<?php

/**
 * @file stlouis_quests_do.tpl.php
 * Do a quest.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

global $game, $phone_id;

include drupal_get_path('module', arg(0)) . '/game_defs.inc';
$game_user = $fetch_user();

$sql = 'select name from neighborhoods where id = %d;';
$result = db_query($sql, $game_user->fkey_neighborhoods_id);
$data = db_fetch_object($result);
$location = $data->name;

$sql = 'select party_title from `values` where id = %d;';
$result = db_query($sql, $game_user->fkey_values_id);
$data = db_fetch_object($result);
$party_title = preg_replace('/^The /', '', $data->party_title);

$data = [];
$sql = 'select quests.*, neighborhoods.name as hood from quests
  LEFT OUTER JOIN neighborhoods
  ON quests.fkey_neighborhoods_id = neighborhoods.id
  where quests.id = %d;';
$result = db_query($sql, $quest_id);
$game_quest = db_fetch_object($result); // limited to 1 in DB
//firep($game_quest);

//   if ($event_type == EVENT_QUESTS_100)
//     $game_quest->required_energy = min($game_quest->required_energy, 100);

// April Fools!  33% chance that loot will be April Fools loot.
//   if (($game_quest->fkey_loot_equipment_id > 0)
//     && (date('m-d') == '04-01')
//     && (mt_rand(0, 2) == 0)) {
//     $sql = 'select fkey_equipment_id from loot_april_fools
//       order by rand() limit 1;';
//     $result = db_query($sql);
//     $loot_april_fools = db_fetch_object($result);
//     $game_quest->fkey_loot_equipment_id = $loot_april_fools->fkey_equipment_id;
//   }

game_alter('quest_item', $game_user, $game_quest);

$quest_succeeded = TRUE;
$outcome_reason = '<div class="quest-succeeded">' . t('Success!') .
  '</div>';
$ai_output = 'quest-succeeded';

// Check to see if quest prerequisites are met.
if (($game_user->energy < $game_quest->required_energy) &&

  // Unlimited quests below level 6.
  ($game_user->level >= 6)) {

  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">' . t('Not enough Energy!') .
    '</div><div class="try-an-election-wrapper">
    <div class="try-an-election"><a
    href="/' . $game . '/elders_do_fill/' . $arg2 . '/energy?destination=/' .
    $game . '/quests/' . $arg2 . '/' . $game_quest->group . '">Refill
    your Energy (1&nbsp;' . $luck . ')</a></div></div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed not-enough-energy';

  game_competency_gain($game_user, 'too tired');
}

// Need to be drunk for quest 45!
if ($quest_id == 45 && game_competency_level($game_user, 'drunk')->level == 0) {
  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">' . t('Not drunk enough!') .
    '</div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed not-drunk-enough';
//  game_competency_gain($game_user, 'sober');
}

// Need to be sober for quest 46!
if ($quest_id == 46 && game_competency_level($game_user, 'sober')->level == 0) {
  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">' . t('Not sober enough!') .
    '</div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed not-sober-enough';
//  game_competency_gain($game_user, 'drunk');
}

if ($game_quest->equipment_1_required_quantity > 0) {

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $game_quest->fkey_equipment_1_required_id,
    $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity < $game_quest->equipment_1_required_quantity) {

    $quest_succeeded = FALSE;
    $outcome_reason = '<div class="quest-failed">' . t('Failed!') .
      '</div><div class="quest-required_stuff missing centered">Missing
      <div class="quest-required_equipment"><a href="/' . $game .
      '/equipment_buy/' .
      $arg2 . '/' . $game_quest->fkey_equipment_1_required_id . '/' .
      ($game_quest->equipment_1_required_quantity - $quantity->quantity) .
      '"><img src="/sites/default/files/images/equipment/' .
      $game . '-' . $game_quest->fkey_equipment_1_required_id . '.png"
      width="48"></a></div>&nbsp;x' .
      $game_quest->equipment_1_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-equipment-' .
      $game_quest->fkey_equipment_1_required_id;

    game_competency_gain($game_user, 'hole in pockets');
  }

}

if ($game_quest->equipment_2_required_quantity > 0) {

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $game_quest->fkey_equipment_2_required_id,
    $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity < $game_quest->equipment_2_required_quantity) {

    $quest_succeeded = FALSE;
    $outcome_reason = '<div class="quest-failed">' . t('Failed!') .
      '</div><div class="quest-required_stuff missing centered">Missing
      <div class="quest-required_equipment"><a href="/' . $game . '/equipment_buy/' .
      $arg2 . '/' . $game_quest->fkey_equipment_2_required_id . '/' .
      ($game_quest->equipment_2_required_quantity - $quantity->quantity) . '"><img
      src="/sites/default/files/images/equipment/' .
      $game . '-' . $game_quest->fkey_equipment_2_required_id . '.png"
      width="48"></a></div>&nbsp;x' . $game_quest->equipment_2_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-equipment-' .
      $game_quest->fkey_equipment_2_required_id;

    game_competency_gain($game_user, 'hole in pockets');

  }

}

if ($game_quest->equipment_3_required_quantity > 0) {

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $game_quest->fkey_equipment_3_required_id,
    $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity < $game_quest->equipment_3_required_quantity) {

    $quest_succeeded = FALSE;
    $outcome_reason = '<div class="quest-failed">' . t('Failed!') .
      '</div><div class="quest-required_stuff missing centered">Missing
      <div class="quest-required_equipment"><a href="/' . $game . '/equipment_buy/' .
      $arg2 . '/' . $game_quest->fkey_equipment_3_required_id . '/' .
      ($game_quest->equipment_3_required_quantity - $quantity->quantity) . '"><img
      src="/sites/default/files/images/equipment/' .
      $game . '-' . $game_quest->fkey_equipment_3_required_id . '.png"
      width="48"></a></div>&nbsp;x' . $game_quest->equipment_3_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-equipment-' .
      $game_quest->fkey_equipment_3_required_id;

    game_competency_gain($game_user, 'hole in pockets');

  }

}

if ($game_quest->staff_required_quantity > 0) {

  $sql = 'select quantity from staff_ownership
    where fkey_staff_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $game_quest->fkey_staff_required_id,
    $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity < $game_quest->staff_required_quantity) {

    $quest_succeeded = FALSE;
    $outcome_reason = '<div class="quest-failed">' . t('Failed!') .
      '</div><div class="quest-required_stuff missing centered">Missing
      <div class="quest-required_equipment"><img
      src="/sites/default/files/images/staff/' .
      $game . '-' . $game_quest->fkey_staff_required_id . '.png"
      width="48"></div>&nbsp;x' . $game_quest->staff_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-staff-' .
      $game_quest->fkey_staff_required_id;

    game_competency_gain($game_user, 'friendless');

  }

}

if ($game_quest->land_required_quantity > 0) {

  $sql = 'select quantity from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $game_quest->fkey_land_required_id,
    $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity < $game_quest->land_required_quantity) {

    $quest_succeeded = FALSE;
    $outcome_reason = '<div class="quest-failed">' . t('Failed!') .
      '</div><div class="quest-required_stuff missing centered">Missing
      <div class="quest-required_equipment"><a href="/' . $game . '/land_buy/' .
      $arg2 . '/' . $game_quest->fkey_land_required_id . '/' .
      ($game_quest->land_required_quantity - $quantity->quantity) . '"><img
      src="/sites/default/files/images/land/' .
      $game . '-' . $game_quest->fkey_land_required_id . '.png"
      width="48"></a></div>&nbsp;x' . $game_quest->land_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-land-' .
      $game_quest->fkey_land_required_id;

    game_competency_gain($game_user, 'homeless');

  }

}

// Wrong hood.
if (($game_quest->group > 0) && ($game_quest->fkey_neighborhoods_id != 0) &&
  ($game_quest->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id)) {

  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">'
  . t('Wrong @hood!', array('@hood' => $hood_lower))
  . '</div>
      <p>This ' . $quest_lower . ' can only be completed in '
      . $game_quest->hood . '.
      </p>
    <div class="try-an-election-wrapper">
      <div class="try-an-election">
        <a href="/' . $game . '/move/' . $arg2 . '/'
        . $game_quest->fkey_neighborhoods_id . '">
          Go there
        </a>
      </div>
    </div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed wrong-hood';

  game_competency_gain($game_user, 'lost');
}


$sql = 'select percent_complete, bonus_given from quest_completion
  where fkey_users_id = %d and fkey_quests_id = %d;';
$result = db_query($sql, $game_user->id, $quest_id);
$pc = db_fetch_object($result);
//firep($pc);

// Get quest completion stats.
$sql = 'SELECT times_completed FROM `quest_group_completion`
    where fkey_users_id = %d and fkey_quest_groups_id = %d;';
  $result = db_query($sql, $game_user->id, $game_quest->group);
  $quest_group_completion = db_fetch_object($result);
//firep($quest_group_completion);

  $percentage_target = 100;
  $percentage_divisor = 1;

  if ($quest_group_completion->times_completed > 0) {

    $percentage_target = 200;
    $percentage_divisor = 2;

  }

$quest_completion_html = '';

if ($quest_succeeded) {
  game_competency_gain($game_user, 'quester');

  // Quest-specific competency to add?
  if ($game_quest->fkey_enhanced_competencies_id > 0) {
    game_competency_gain($game_user, (int) $game_quest->fkey_enhanced_competencies_id);
  }

  $old_energy = $game_user->energy;
  $game_user->energy -= $game_quest->required_energy;
  $game_user->experience += $game_quest->experience;
  $money_added += mt_rand($game_quest->min_money, $game_quest->max_money);
  $game_user->money += $money_added;

  // Don't save quests group if 1000 or over.
  if ($game_quest->group >= 1000) {
    $sql = 'update users set energy = energy - %d,
      experience = experience + %d, money = money + %d
      where id = %d;';
    db_query($sql, $game_quest->required_energy,
      $game_quest->experience, $money_added, $game_user->id);
  }
  else {

    // Save all updated stats.
    $sql = 'update users set energy = energy - %d,
      experience = experience + %d, money = money + %d,
      fkey_last_played_quest_groups_id = %d
      where id = %d;';
    db_query($sql, $game_quest->required_energy,
      $game_quest->experience, $money_added, $game_quest->group,
      $game_user->id);
  }

  // Start the energy clock again.
  if ($old_energy == $game_user->energy_max) {
    $sql = 'update users set energy_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', time() + $energy_wait), $game_user->id);
  }

  // Update percentage completion.
  // No entry yet, add one.
  if (empty($pc->percent_complete)) {
    $sql = 'insert into quest_completion (fkey_users_id, fkey_quests_id,
      percent_complete) values (%d, %d, %d);';
    $result = db_query($sql, $game_user->id, $quest_id,
     $game_quest->percent_complete);
  }
  else {
    $sql = 'update quest_completion set percent_complete = least(
      percent_complete + %d, %d) where fkey_users_id = %d and
      fkey_quests_id = %d;';
    $result = db_query($sql,
      floor($game_quest->percent_complete / $percentage_divisor),
      $percentage_target, $game_user->id, $quest_id);
  }

  $percent_complete = min($pc->percent_complete +
    floor($game_quest->percent_complete / $percentage_divisor),
    $percentage_target);

  // If they have completed the quest for the first time in a round, give them a bonus.
  if ($percent_complete == $percentage_target) {

    if ($pc->bonus_given < $percentage_divisor) {

      game_competency_gain($game_user, 'quest finisher');

      $game_user->experience += $game_quest->experience;
      $game_user->money += $money_added;

      $sql = 'update users set experience = experience + %d, money = money + %d
        where id = %d;';
      $result = db_query($sql, $game_quest->experience, $money_added,
        $game_user->id);

      $sql = 'update quest_completion set bonus_given = bonus_given + 1
        where fkey_users_id = %d and fkey_quests_id = %d;';
      $result = db_query($sql, $game_user->id, $quest_id);

      $quest_completion_html .=<<< EOF
<div class="title loot">$quest Completed!</div>
<p>You have completed this $quest_lower and gained an extra $money_added
  $game_user->values and $game_quest->experience $experience!&nbsp; Complete
  all ${quest_lower}s in this group for an extra reward.</p>
EOF;

    }

    // Did they complete all quests in the group?
    $sql = 'select * from quest_group_completion
      where fkey_users_id = %d and fkey_quest_groups_id = %d;';
    $result = db_query($sql, $game_user->id, $game_quest->group);
    $qgc = db_fetch_object($result);
//firep($qgc);

    if (empty($qgc) || $qgc->times_completed == 0) {

      // If no quest_group bonus has been given.
      // Get quest group stats.
      $sql = 'SELECT sum( bonus_given ) AS completed,
        count( quests.id ) AS total, quest_groups.ready_for_bonus
        FROM `quests`
        LEFT OUTER JOIN quest_completion
        ON quest_completion.fkey_quests_id = quests.id
        AND fkey_users_id = %d
        LEFT JOIN quest_groups
        ON quests.group = quest_groups.id
        WHERE `group` = %d
        AND quests.active =1';
      $result = db_query($sql, $game_user->id, $game_quest->group);
      $quest_group = db_fetch_object($result);
//firep($quest_group);

      if (($quest_group->completed == $quest_group->total) &&
        ($quest_group->ready_for_bonus == 1)) {

        // Woohoo! User just completed an entire group!
        $quest_completion_html .=<<< EOF
<div class="title loot">Congratulations!</div>
<p>You have completed all {$quest_lower}s in this group and have gained extra skill
points!</p>
<p class="second"><a href="/$game/increase_skills/$arg2/none">You
have <span class="highlighted">$quest_group->completed</span> new skill points
to spend</a></p>
EOF;
        game_competency_gain($game_user, 'quest groupie');

        // Update user stats.
        $sql = 'update users set skill_points = skill_points + %d
          where id = %d;';
        $result = db_query($sql, $quest_group->completed, $game_user->id);

        // Update quest_groups_completion.
        if (empty($qgc)) {
          $sql = 'insert into quest_group_completion (fkey_users_id,
            fkey_quest_groups_id, times_completed) values (%d, %d, 1);';
          $result = db_query($sql, $game_user->id, $game_quest->group);
        }
        else {
          $sql = 'update quest_group_completion set times_completed = 1
            where fkey_users_id = %d and fkey_quest_groups_id = %d;';
          $result = db_query($sql, $game_user->id, $game_quest->group);
        }

        $quest_group_completion->times_completed = 1;
        $percentage_target = 200;
        $percentage_divisor = 2;

      }

    }

    // What? They've completed a 2nd time?
    if ($qgc->times_completed == 1) {

      // Get quest group stats.
      $sql = 'SELECT sum( bonus_given ) AS completed,
        count( quests.id ) AS total, quest_groups.ready_for_bonus
        FROM `quests`
        LEFT OUTER JOIN quest_completion
        ON quest_completion.fkey_quests_id = quests.id
        AND fkey_users_id = %d
        LEFT JOIN quest_groups
        ON quests.group = quest_groups.id
        WHERE `group` = %d
        AND quests.active =1';
      $result = db_query($sql, $game_user->id, $game_quest->group);
      $quest_group = db_fetch_object($result);
//firep($quest_group);

      if ($quest_group->completed == ($quest_group->total * 2)) {

        // Woohoo! User just completed an entire group the second time!
//          game_competency_gain($game_user, 'second-mile saint');

        $sql = 'select * from quest_group_bonus
          where fkey_quest_groups_id = %d;';
        $result = db_query($sql, $game_quest->group);

        // Limited to 1 in db.
        $item = db_fetch_object($result);
//firep($item);
        $eq_id = $item->fkey_equipment_id;
        $land_id = $item->fkey_land_id;
        $st_id = $item->fkey_staff_id;

        if (($eq_id + $land_id + $st_id) > 0) {

          // Anything to give him/her?
          // Equipment bonus.
          if ($eq_id > 0) {

            $data = [];
            $sql = 'SELECT equipment.*, equipment_ownership.quantity
              FROM equipment

              LEFT OUTER JOIN equipment_ownership
              ON equipment_ownership.fkey_equipment_id = equipment.id
              AND equipment_ownership.fkey_users_id = %d

              WHERE equipment.id = %d;';
            $result = db_query($sql, $game_user->id, $eq_id);
            $game_equipment = db_fetch_object($result); // limited to 1 in DB

// give the stuff
            // No record exists - insert one.
            if ($game_equipment->quantity == '') {
              $sql = 'insert into equipment_ownership
                (fkey_equipment_id, fkey_users_id, quantity)
                values (%d, %d, %d);';
              $result = db_query($sql, $eq_id, $game_user->id, 1);
            }
            else { // existing record - update it
              $sql = 'update equipment_ownership set quantity = quantity + 1
                where fkey_equipment_id = %d and fkey_users_id = %d;';
              $result = db_query($sql, $eq_id, $game_user->id);
            }

// tell the user about it
            $quest_completion_html .=<<< EOF
<div class="quest-succeeded title loot">Congratulations!</div>
<div class="subsubtitle">You have completed the second round of {$quest_lower}s!</div>
<div class="subsubtitle">Here is your bonus:</div>
<div class="quest-icon"><img
src="/sites/default/files/images/equipment/$game-{$eq_id}.png" width="96"></div>
<div class="quest-details">
<div class="quest-name loot">$game_equipment->name</div>
<div class="quest-description">$game_equipment->description</div>
EOF;

            if ($game_equipment->energy_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Energy: +$game_equipment->energy_bonus immediate energy bonus
    </div>
EOF;

            }

            if ($game_equipment->energy_increase > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Energy: +$game_equipment->energy_increase every $energy_wait_str
    </div>
EOF;

            }

            if ($game_equipment->initiative_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">$initiative: +$game_equipment->initiative_bonus
    </div>
EOF;

            }

            if ($game_equipment->endurance_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Endurance: +$game_equipment->endurance_bonus
    </div>
EOF;

            }

            if ($game_equipment->elocution_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">$elocution: +$game_equipment->elocution_bonus
    </div>
EOF;

            }

            if ($game_equipment->speed_increase > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Speed Increase: $game_equipment->speed_increase fewer Action
    needed to move to a new $hood_lower
    </div>
EOF;

            }

            if ($game_equipment->upkeep > 0) {
              $quest_completion_html .=<<< EOF
  <div class="quest-payout negative">Upkeep: $game_equipment->upkeep every 60 minutes</div>
EOF;
              game_recalc_income($game_user);
            }

            if ($game_equipment->chance_of_loss > 0) {

              $lifetime = floor(100 / $game_equipment->chance_of_loss);
              $use = ($lifetime == 1) ? 'use' : 'uses';
              $quest_completion_html .=<<< EOF
  <div class="quest-payout negative">Expected Lifetime: $lifetime $use</div>
EOF;

            }

            $quest_completion_html .= '</div>';

          }

          // FIXME: land bonus here


          // Staff bonus.
          if ($st_id > 0) {

            $data = array();
            $sql = 'SELECT staff.*, staff_ownership.quantity
              FROM staff

              LEFT OUTER JOIN staff_ownership
              ON staff_ownership.fkey_staff_id = staff.id
              AND staff_ownership.fkey_users_id = %d

              WHERE staff.id = %d;';
            $result = db_query($sql, $game_user->id, $st_id);

            // Limited to 1 in DB.
            $game_staff = db_fetch_object($result);

            // Give the stuff.
            // No record exists - insert one.
            if ($game_staff->quantity == '') {
              $sql = 'insert into staff_ownership
                (fkey_staff_id, fkey_users_id, quantity)
                values (%d, %d, %d);';
              $result = db_query($sql, $st_id, $game_user->id, 1);
            }
            else {

              // Existing record - update it.
              $sql = 'update staff_ownership set quantity = quantity + 1 where
                fkey_staff_id = %d and fkey_users_id = %d;';
              $result = db_query($sql, $st_id, $game_user->id);
            }

            // Tell the user about it.
            $quest_completion_html .=<<< EOF
<div class="quest-succeeded title loot">Congratulations!</div>
<div class="subsubtitle">You have completed the second round of {$quest_lower}s!</div>
<div class="subsubtitle">Here is your bonus:</div>
<div class="quest-icon"><img
src="/sites/default/files/images/staff/$game-{$st_id}.png" width="96"></div>
<div class="quest-details">
<div class="quest-name loot">$game_staff->name</div>
<div class="quest-description">$game_staff->description</div>
EOF;

            if ($game_staff->energy_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Energy: +$game_staff->energy_bonus immediate energy bonus
    </div>
EOF;

            }

            if ($game_staff->energy_increase > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Energy: +$game_staff->energy_increase every $energy_wait_str
    </div>
EOF;

            }

            if ($game_staff->initiative_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">$initiative: +$game_staff->initiative_bonus
    </div>
EOF;

            }

            if ($game_staff->endurance_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">$endurance: +$game_staff->endurance_bonus
    </div>
EOF;

            }

            if ($game_staff->elocution_bonus > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">$elocution: +$game_staff->elocution_bonus
    </div>
EOF;

            }

            if ($game_staff->speed_increase > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Speed Increase: $game_staff->speed_increase fewer Action
    needed to move to a new $hood_lower
    </div>
EOF;

            }

            if ($game_staff->extra_votes > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Extra Votes: $game_staff->extra_votes</div>
EOF;

            }

            if ($game_staff->extra_defending_votes > 0) {

              $quest_completion_html .=<<< EOF
  <div class="quest-payout">Extra Defending Votes: $game_staff->extra_defending_votes</div>
EOF;

            }

            if ($game_staff->upkeep > 0) {
              $quest_completion_html .=<<< EOF
  <div class="quest-payout negative">Upkeep: $game_staff->upkeep every 60 minutes</div>
EOF;
              game_recalc_income($game_user);
            }

            if ($game_staff->chance_of_loss > 0) {

              $lifetime = floor(100 / $game_staff->chance_of_loss);
              $use = ($lifetime == 1) ? 'use' : 'uses';
              $quest_completion_html .=<<< EOF
  <div class="quest-payout negative">Expected Lifetime: $lifetime $use</div>
EOF;

            }

            $quest_completion_html .= '</div>';

          }

        // Update quest_groups_completion.
        $sql = 'update quest_group_completion set times_completed = 2
          where fkey_users_id = %d and fkey_quest_groups_id = %d;';
        $result = db_query($sql, $game_user->id, $game_quest->group);

//          $quest_group_completion->times_completed = 1;
//          $percentage_target = 200;
//          $percentage_divisor = 2;

        }
        else {

          // We don't have a bonus yet.
          $quest_completion_html .=<<< EOF
<div class="title loot">Congratulations!</div>
<div class="quest-icon"><img
src="/sites/default/files/images/quests/stlouis-soon.png"></div>
<div class="quest-details">
<div class="quest-name loot">You have completed all {$quest_lower}s in
  this group a second time!</div>
<div class="quest-description">Unfortunately, we have nothing to give you
  yet!&nbsp; We're still coding it!</div>
<p class="second">&nbsp;</p>
</div>
EOF;
        }

      }

    }

  }

  if ($percent_complete > floor($percentage_target / 2)) {
    $rgb = dechex(floor(($percentage_target - $percent_complete) /
      (4 * $percentage_divisor))) . 'c0';
  }
  else {
    $rgb = 'c' . dechex(floor(($percent_complete) /
      (4 * $percentage_divisor))) . '0';
  }
  $width = floor($percent_complete * 94 / $percentage_target) + 2;

  // Check for loot -- equipment.
  $sql = 'SELECT equipment.quantity_limit, equipment_ownership.quantity
    FROM equipment

    LEFT OUTER JOIN equipment_ownership
    ON equipment_ownership.fkey_equipment_id = equipment.id
    AND equipment_ownership.fkey_users_id = %d

    WHERE equipment.id = %d;';
  $result = db_query($sql, $game_user->id, $game_quest->fkey_loot_equipment_id);
  $game_equipment = db_fetch_object($result);
  $limit = $game_equipment->quantity_limit > (int) $game_equipment->quantity;

  // Haven't gotten any of this loot yet?  Bump loot chance up to 30%.
  if ($game_quest->chance_of_loot > 0 && $game_quest->chance_of_loot < 30
    && $game_equipment->quantity == 0) {
    $game_quest->chance_of_loot = 30;
  }

  if ((($game_user->level <= 6 && $game_quest->chance_of_loot > 0)
      || $game_quest->chance_of_loot >= mt_rand(1, 99))
    && ($limit || $game_equipment->quantity_limit == 0)) {

    $sql = 'select * from equipment where id = %d;';
    $result = db_query($sql, $game_quest->fkey_loot_equipment_id);
    $loot = db_fetch_object($result);

    $cumulative_expenses = $game_user->expenses + $loot->upkeep;
    if ((int) $game_user->income >= $cumulative_expenses) {

      // Special case for Drunken Stupor.
      if ($game_quest->fkey_loot_equipment_id == 36) {
        game_competency_gain($game_user, 'drunk');
      }

      $loot_html =<<< EOF
<div class="title loot">You Found</div>
<div class="quest-icon"><img
 src="/sites/default/files/images/equipment/$game-$loot->id.png" width="96"></div>
<div class="quest-details">
  <div class="quest-name loot">$loot->name</div>
  <div class="quest-description">$loot->description</div>
EOF;

      if ($loot->initiative_bonus > 0) {
        $loot_html .=<<< EOF
    <div class="quest-payout">$initiative: +$loot->initiative_bonus
      </div>
EOF;
      }
      else if ($loot->initiative_bonus < 0) {
        $loot_html .=<<< EOF
    <div class="quest-payout negative">$initiative: $loot->initiative_bonus
      </div>
EOF;
      }

      if ($loot->endurance_bonus > 0) {
        $loot_html .=<<< EOF
  <div class="quest-payout">$endurance: +$loot->endurance_bonus
    </div>
EOF;
      }
      else if ($loot->endurance_bonus < 0) {
        $loot_html .=<<< EOF
  <div class="quest-payout negative">$endurance: $loot->endurance_bonus
    </div>
EOF;
      }

      if ($loot->elocution_bonus > 0) {
        $loot_html .=<<< EOF
    <div class="quest-payout">$elocution: +$loot->elocution_bonus
      </div>
EOF;
      }
      else if ($loot->elocution_bonus < 0) {
        $loot_html .=<<< EOF
    <div class="quest-payout negative">$elocution: $loot->elocution_bonus
      </div>
EOF;
      }

      // Upkeep.
      if ($loot->upkeep > 0) {
        $loot_html .=<<< EOF
    <div class="quest-payout negative">Upkeep: $loot->upkeep every 60 minutes</div>
EOF;
      }

      $loot_html .=<<< EOF
    <p class="second">&nbsp;</p>
  </div>
EOF;

      // Add/update db entry.
      game_competency_gain($game_user, 'looter');

      $sql = 'SELECT equipment.*, equipment_ownership.quantity
        FROM equipment

        LEFT OUTER JOIN equipment_ownership
        ON equipment_ownership.fkey_equipment_id = equipment.id
        AND equipment_ownership.fkey_users_id = %d

        WHERE equipment.id = %d;';
      $result = db_query($sql, $game_user->id,
        $game_quest->fkey_loot_equipment_id);

      // Limited to 1 in DB.
      $game_equipment = db_fetch_object($result);

      // No record exists - insert one.
      if ($game_equipment->quantity == '') {
        $sql = 'insert into equipment_ownership (fkey_equipment_id,
          fkey_users_id, quantity) values (%d, %d, 1);';
        $result = db_query($sql, $game_quest->fkey_loot_equipment_id,
          $game_user->id);
      }
      else {

        // Existing record - update it.
        $sql = 'update equipment_ownership set quantity = quantity + 1 where
          fkey_equipment_id = %d and fkey_users_id = %d;';
        $result = db_query($sql, $game_quest->fkey_loot_equipment_id,
          $game_user->id);
      }

      if ($loot->upkeep > 0) {
        game_recalc_income($game_user);
      }

    }

  }

  // Check for loot -- staff.
  $sql = 'SELECT staff.quantity_limit,staff_ownership.quantity
    FROM staff

    LEFT OUTER JOIN staff_ownership
    ON staff_ownership.fkey_staff_id = staff.id
    AND staff_ownership.fkey_users_id = %d

    WHERE staff.id = %d;';
  $result = db_query($sql, $game_user->id,
    $game_quest->fkey_loot_staff_id);
  $game_staff = db_fetch_object($result);
  $limit = $game_staff->quantity_limit > (int) $game_staff->quantity;

  if ((($game_user->level <= 6 && $game_quest->chance_of_loot_staff > 0)
      || $game_quest->chance_of_loot_staff >= mt_rand(1, 99))
    && ($limit || $game_staff->quantity_limit == 0)) {

    $sql = 'select * from staff where id = %d;';
    $result = db_query($sql, $game_quest->fkey_loot_staff_id);
    $loot = db_fetch_object($result);

    $loot_html .=<<< EOF
<div class="title loot">You Found</div>
<div class="quest-icon"><img
 src="/sites/default/files/images/staff/$game-$loot->id.png" width="96"></div>
<div class="quest-details">
  <div class="quest-name loot">$loot->name</div>
  <div class="quest-description">$loot->description</div>
EOF;

      if ($loot->initiative_bonus > 0) {

        $loot_html .=<<< EOF
    <div class="quest-payout">$initiative: +$loot->initiative_bonus
      </div>
EOF;

  }

  if ($loot->endurance_bonus > 0) {

    $loot_html .=<<< EOF
  <div class="quest-payout">$endurance: +$loot->endurance_bonus
    </div>
EOF;

  }

  if ($loot->elocution_bonus > 0) {

    $loot_html .=<<< EOF
    <div class="quest-payout">$elocution: +$loot->elocution_bonus
      </div>
EOF;

  }

  // Upkeep.
  if ($loot->upkeep > 0) {
    $loot_html .=<<< EOF
    <div class="quest-payout negative">Upkeep: $loot->upkeep every 60 minutes</div>
EOF;
  }

  $loot_html .=<<< EOF
    <p class="second">&nbsp;</p>
  </div>
EOF;

    game_competency_gain($game_user, 'looter');

    // Add/update db entry.
    $sql = 'SELECT staff.*, staff_ownership.quantity
      FROM staff

      LEFT OUTER JOIN staff_ownership
      ON staff_ownership.fkey_staff_id = staff.id
      AND staff_ownership.fkey_users_id = %d

      WHERE staff.id = %d;';
    $result = db_query($sql, $game_user->id,
      $game_quest->fkey_loot_staff_id);
    $game_staff = db_fetch_object($result);

    // No record exists - insert one.
    if ($game_staff->quantity == '') {
      $sql = 'insert into staff_ownership (fkey_staff_id,
        fkey_users_id, quantity) values (%d, %d, 1);';
      $result = db_query($sql, $game_quest->fkey_loot_staff_id,
        $game_user->id);
    }
    else {

      // Existing record - update it.
      $sql = 'update staff_ownership set quantity = quantity + 1 where
        fkey_staff_id = %d and fkey_users_id = %d;';
      $result = db_query($sql, $game_quest->fkey_loot_staff_id,
        $game_user->id);
    }

    if ($loot->upkeep > 0) {
      game_recalc_income($game_user);
    }

  }

  $game_user = $fetch_user();
  $fetch_header($game_user);

  if ($game_user->level < 6 and $game_user->experience > 0) {

  echo <<< EOF
<ul>
<li>Each $quest_lower gives you more $game_user->values and $experience</li>
<li>Wait and rest for a few minutes if you run out of Energy</li>
</ul>
EOF;

   }

  $description = str_replace('%party', "<em>$party_title</em>",
    $game_quest->description);

  echo <<< EOF
<div class="quests">
$outcome_reason
<div class="quest-icon"><a
  href="/$game/quests_do/$arg2/$game_quest->id"><img
  src="/sites/default/files/images/quests/$game-$game_quest->id.png" width="96"
  border="0"></a><div class="quest-complete"><div class="quest-complete-percentage"
    style="background-color: #$rgb; width: {$width}px">&nbsp;</div>
    <div class="quest-complete-text">$percent_complete%
      complete</div></div></div>
<div class="quest-details">
  <div class="quest-name"><a
    href="/$game/quests_do/$arg2/$game_quest->id">$game_quest->name</a></div>
  <div class="quest-description">$description</div>
  <div class="quest-experience">You gained <strong>$money_added
    $game_user->values</strong></div>
  <div class="quest-money">You gained <strong>$game_quest->experience
    $experience</strong></div>
    </div>
  $loot_html
  $quest_completion_html
<div class="quest-do-again">
  <div class="quest-do-again-inside"><a
    href="/$game/quests_do/$arg2/$game_quest->id">Do
    Again</a></div>
</div>
</div>
EOF;

//  game_show_quest($game_user, $game_quest, $percent_complete,
//    $percentage_divisor, $quest_group, $party_title, $outcome_reason,
//    "You gained <strong>$game_quest->experience</strong>",
//    "You gained <strong>$money_added</strong>",'',
//    '', $loot_html, $quest_completion_html);

}
else {

      // Failed!
      $fetch_header($game_user);

  if ($game_user->level < 6 and $game_user->experience > 0) {

    echo <<< EOF
<ul>
<li>Each $quest_lower gives you more $game_user->values and $experience</li>
<li>Wait and rest for a few minutes if you run out of Energy</li>
</ul>
EOF;

  }

  $sql = 'SELECT times_completed FROM `quest_group_completion`
    where fkey_users_id = %d and fkey_quest_groups_id = %d;';
  $result = db_query($sql, $game_user->id, $game_quest->group);
  $quest_group_completion = db_fetch_object($result);

  $percentage_target = 100;
  $percentage_divisor = 1;

  if ($quest_group_completion->times_completed > 0) {

    $percentage_target = 200;
    $percentage_divisor = 2;

  }

  $percent_complete = $pc->percent_complete + 0;

  if ($percent_complete > floor($percentage_target / 2)) {

    $rgb = dechex(floor(($percentage_target - $percent_complete) /
      (4 * $percentage_divisor))) . 'c0';

  }
  else {

    $rgb = 'c' . dechex(floor(($percent_complete) /
      (4 * $percentage_divisor))) . '0';

  }

  $width = floor($percent_complete * 94 / $percentage_target) + 2;

  echo <<< EOF
<div class="quests">
$outcome_reason
<div class="quest-icon"><a
  href="/$game/quests_do/$arg2/$game_quest->id"><img
  src="/sites/default/files/images/quests/$game-$game_quest->id.png"
  border="0" width="96"></a>
  <div class="quest-complete"><div class="quest-complete-percentage"
    style="background-color: #$rgb; width: {$width}px">&nbsp;</div>
    <div class="quest-complete-text">$percent_complete%
      complete</div></div></div>
<div class="quest-details">
  <div class="quest-name"><a
    href="/$game/quests_do/$arg2/$game_quest->id">$game_quest->name</a></div>
  <div class="quest-experience">+$game_quest->experience $experience,
  +$game_quest->min_money to $game_quest->max_money $game_user->values</div>
  <div class="quest-required_energy">Requires $game_quest->required_energy
    Energy</div>
  $extra_html
  <br/><br/>
</div>
</div>
EOF;

}

if (substr($phone_id, 0, 3) == 'ai-')
  echo "<!--\n<ai \"$ai_output\"/>\n-->";

$sql = 'select name from quest_groups where id = %s;';
$result = db_query($sql, $game_quest->group);
$qg = db_fetch_object($result);
firep($qg);

$location = str_replace('%location', $location, $qg->name);

// Show beginning quests, keep location from user.
if ($game_user->level < 6) {

  $location = $older_missions_html = $newer_missions_html = '';
  $sql_quest_neighborhood = 'where fkey_neighborhoods_id = 0';

}
else {

  // Show location-specific quests.
  $sql_quest_neighborhood = 'where ((fkey_neighborhoods_id = 0 and
    required_level >= 6) or fkey_neighborhoods_id = ' .
    $game_user->fkey_neighborhoods_id . ')';

}

$sql = 'select name from quest_groups where id = %s;';
$result = db_query($sql, $game_quest->group - 1);
$qgo = db_fetch_object($result);

if (!empty($qgo->name) && ($game_quest->group <= 1000)) {

  $older_group = $game_quest->group - 1;
  $older_missions_html =<<< EOF
<a href="/$game/quests/$arg2/$older_group">&lt;&lt;</a>
EOF;

}

$sql = 'select min(required_level) as min from quests
  where `group` = %d;';
$result = db_query($sql, $game_quest->group + 1);
$item = db_fetch_object($result);
firep($item);

if (!empty($item->min) && ($item->min <= $game_user->level + 1) &&
  ($group_to_show <= 1000)) {

  $newer_group = $game_quest->group + 1;
  $newer_missions_html =<<< EOF
<a href="/$game/quests/$arg2/$newer_group">&gt;&gt;</a>
EOF;

}

$quests = "{$quest}s";

echo <<< EOF
<div class="title">
$older_missions_html $location $quests $newer_missions_html
</div>
EOF;

// Get quest group stats.
$sql = 'SELECT sum(bonus_given) as completed, count(quests.id) as total
  FROM `quests`
  left outer join quest_completion
  on quest_completion.fkey_quests_id = quests.id
  and fkey_users_id = %d
  where `group` = %d and quests.active = 1;';
$result = db_query($sql, $game_user->id, $game_quest->group);

$quest_group = db_fetch_object($result);
//firep($quest_group);

// Haha! Typecasting!
$quest_group->completed += 0;

if ($quest_group_completion->times_completed > 0) {

  $next_group_html = t('(2nd round)');
  $quest_group->completed -=
    ($quest_group->total * min($quest_group_completion->times_completed, 1));

}

echo <<< EOF
<div class="quest-group-completion">
<strong>$quest_group->completed</strong> of $quest_group->total {$quest}s
complete $next_group_html
</div>
EOF;

// Show each quest.
$data = [];
$sql = 'select quests.*, quest_completion.percent_complete,
  neighborhoods.name as hood from quests
  LEFT OUTER JOIN neighborhoods
  ON quests.fkey_neighborhoods_id = neighborhoods.id
  LEFT OUTER JOIN quest_completion
  ON quest_completion.fkey_quests_id = quests.id
  AND quest_completion.fkey_users_id = %d where `group` = %d
  and required_level <= %d
  and active = 1 order by required_level ASC;';
$result = db_query($sql, $game_user->id, $game_quest->group,
  $game_user->level);

while ($item = db_fetch_object($result)) $data[] = $item;

foreach ($data as $item) {

//    if ($event_type == EVENT_QUESTS_100)
//      $item->required_energy = min($item->required_energy, 100);

  $description = str_replace('%party', "<em>$party_title</em>",
    $item->description);

  if (empty($item->percent_complete)) $item->percent_complete = 0;

  if ($item->percent_complete > floor($percentage_target / 2)) {

    $rgb = dechex(floor(($percentage_target - $item->percent_complete) /
      (4 * $percentage_divisor))) . 'c0';

  }
  else {

    $rgb = 'c' . dechex(floor(($item->percent_complete) /
      (4 * $percentage_divisor))) . '0';

  }

  $width = floor($item->percent_complete * 94 / $percentage_target) + 2;
  game_alter('quest_item', $game_user, $item);

// firep($rgb);

  if (($game_quest->group > 0) &&
    (($item->fkey_neighborhoods_id != 0) &&
    ($item->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id))) {

    // Show quests in other hoods?
    echo <<< EOF
<div class="quests wrong-hood">
  <div class="quest-icon">
    <img src="/sites/default/files/images/quests/$game-$item->id.png"
      border="0" width="96"/>
    <div class="quest-complete">
      <div class="quest-complete-percentage"
        style="background-color: #$rgb; width: {$width}px">
        &nbsp;
      </div>
      <div class="quest-complete-text">
        $item->percent_complete% complete
      </div>
    </div>
  </div>
  <div class="quest-details">
    <div class="quest-name">
      $item->name $active
    </div>
    <div class="quest-description">
      This $quest_lower can only be completed in $item->hood.
    </div>
  </div>
  <form action="/$game/move/$arg2/$item->fkey_neighborhoods_id">
    <div class="quests-perform-button-wrapper">
      <input class="quests-perform-button" type="submit" value="Go there"/>
    </div>
  </form>
</div>
EOF;

  }
  else {

    // Quest in my hood.
    game_show_quest($game_user, $item, $percentage_target,
      $percentage_divisor, $quest_group, $party_title);
  }

}

  // Don't show extra quests at first.
//  if ($game_user->level > 1) {

  $data = array();
  $sql = 'select * from quests where `group` = %d and required_level = %d
    and (fkey_neighborhoods_id = 0 or fkey_neighborhoods_id = %d)
    and active = 1 order by required_level ASC;';
  $result = db_query($sql, $game_quest->group, $game_user->level + 1,
    $game_user->fkey_neighborhoods_id);

  while ($item = db_fetch_object($result)) $data[] = $item;

  foreach ($data as $item) {

//      if ($event_type == EVENT_QUESTS_100)
//        $item->required_energy = min($item->required_energy, 100);

    $description = str_replace('%party', "<em>$party_title</em>",
      $item->description);
firep($description);
    game_alter('quest_item', $game_user, $item);

    echo <<< EOF
<div class="quests-soon">
<div class="quest-name">$item->name</div>
<div class="quest-description">$description</div>
<div class="quest-required_level">Requires level $item->required_level</div>
<div class="quest-experience">+$item->min_money to $item->max_money
  $game_user->values, +$item->experience $experience</div>
<div class="quest-required_energy">Requires $item->required_energy
  Energy</div>
</div>
EOF;

  }

//  }

db_set_active('default');
