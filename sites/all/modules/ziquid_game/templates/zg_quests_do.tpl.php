<?php

/**
 * @file
 * Do a quest.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * .
 */

global $game, $phone_id;

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();

$sql = 'select `group` from quests where quests.id = %d;';
$result = db_query($sql, $quest_id);
$data = db_fetch_object($result);
$group_id = $data->group;
$qgo = zg_fetch_quest_groups($game_user, $group_id);
$game_quest = $quest_action = &$qgo->q[$quest_id];
zg_alter('quest_item', $game_user, $game_quest);
firep($game_quest, 'game quest at zg_quests_do_.tpl.php');
$quest_link = 'quest_groups';

$quest_succeeded = TRUE;
$ai_output = 'quest-succeeded';
if (zg_get_default('has_quest_actions')) {
  $quest_action = $game_quest->quest_actions[check_plain(arg(4))];
  $outcome_reason = '<div class="quest-action-succeeded">' . $quest_action->response .
    '</div>';
}
else {
  $outcome_reason = '<div class="quest-succeeded">' . t('Success!') .
    '</div>';
}
// Check to see if quest prerequisites are met.
if (($game_user->energy < $quest_action->required_energy) &&
  ($game_user->level >= 6)) {

  // Unlimited quests below level 6.
  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">' . t('Not enough Energy!') .
    '</div><div class="try-an-election-wrapper">
    <div class="try-an-election"><a
    href="/' . $game . '/elders_do_fill/' . $arg2 . '/energy?destination=/' .
    $game . '/' . $quest_link . '/' . $arg2 . '/' . $game_quest->group .
    '%3fshow_expanded=' . $game_quest->group . '%23group-' .
    $game_quest->group . '">Refill
    your Energy (1&nbsp;' . $luck . ')</a></div></div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed not-enough-energy';

  zg_competency_gain($game_user, 'too tired');
}

// FIXME: Need to be drunk for quest 45!
if ($quest_id == 45 && zg_competency_level($game_user, 'drunk')->level == 0) {
  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">' . t('Not drunk enough!') .
    '</div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed not-drunk-enough';
//  zg_competency_gain($game_user, 'sober');
}

// FIXME: Need to be sober for quest 46!
if ($quest_id == 46 && zg_competency_level($game_user, 'sober')->level == 0) {
  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed">' . t('Not sober enough!') .
    '</div>';
  $extra_html = '<p>&nbsp;</p><p class="second">&nbsp;</p>';
  $ai_output = 'quest-failed not-sober-enough';
//  zg_competency_gain($game_user, 'drunk');
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
      width="48" class="not-yet"></a></div>&nbsp;x' .
      $game_quest->equipment_1_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-equipment-' .
      $game_quest->fkey_equipment_1_required_id;

    zg_competency_gain($game_user, 'hole in pockets');
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
      width="48" class="not-yet"></a></div>&nbsp;x' . $game_quest->equipment_2_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-equipment-' .
      $game_quest->fkey_equipment_2_required_id;

    zg_competency_gain($game_user, 'hole in pockets');
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
      width="48" class="not-yet"></a></div>&nbsp;x' . $game_quest->equipment_3_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-equipment-' .
      $game_quest->fkey_equipment_3_required_id;

    zg_competency_gain($game_user, 'hole in pockets');
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
      width="48" class="not-yet"></div>&nbsp;x' . $game_quest->staff_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-staff-' .
      $game_quest->fkey_staff_required_id;

    zg_competency_gain($game_user, 'friendless');
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
      width="48" class="not-yet"></a></div>&nbsp;x' . $game_quest->land_required_quantity .
      '</div>';
    $ai_output = 'quest-failed need-land-' .
      $game_quest->fkey_land_required_id;

    zg_competency_gain($game_user, 'homeless');
  }
}

// Wrong hood.
if (($quest_action->fkey_neighborhoods_id != 0) &&
  ($quest_action->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id)) {
  if ($game_quest->hood_is_habitable) {
    $go_button = zg_render_button('move', 'Go There',
      '/' . $quest_action->fkey_neighborhoods_id);
  }
  else {
    $go_button = '<p>' .
      t('Unfortunately, you cannot figure out how to get there.') .
      '</p>';
  }
  $quest_succeeded = FALSE;
  $outcome_reason = '<div class="quest-failed-wrapper">
    <div class="quest-failed">' .
    t('Wrong @hood!', ['@hood' => $hood_lower]) .
    '</div>
    <p>This ' . $quest_lower . ' can only be performed in <strong>' .
      $game_quest->hood . '.
    </strong></p>' .
    $go_button .
    '</div>';
  $ai_output = 'quest-failed wrong-hood';
  zg_competency_gain($game_user, 'lost');
}

$sql = 'select percent_complete, bonus_given from quest_completion
  where fkey_users_id = %d and fkey_quests_id = %d;';
$result = db_query($sql, $game_user->id, $quest_id);
$pc = db_fetch_object($result);

// Get quest completion stats.
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

$quest_completion_html = '';

if ($quest_succeeded) {
  zg_competency_gain($game_user, 'quester');

  // Quest-specific competency to add?
  if ($quest_action->fkey_enhanced_competencies_id > 0) {
    zg_competency_gain($game_user, (int) $quest_action->fkey_enhanced_competencies_id);
  }

  $old_energy = $game_user->energy;
  $game_user->energy -= $quest_action->required_energy;
  $game_user->experience += $quest_action->experience;
  $money_added += mt_rand($quest_action->min_money, $quest_action->max_money);
  $game_user->money += $money_added;

  // Don't save quests group if 1000 or over.
  if ($game_quest->group >= 1000) {
    $sql = 'update users set energy = energy - %d,
      experience = experience + %d, money = money + %d
      where id = %d;';
    db_query($sql, $quest_action->required_energy,
      $quest_action->experience, $money_added, $game_user->id);
  }
  else {
    // Save all updated stats.
    $sql = 'update users set energy = energy - %d,
      experience = experience + %d, money = money + %d,
      fkey_last_played_quest_groups_id = %d
      where id = %d;';
    db_query($sql, $quest_action->required_energy,
      $quest_action->experience, $money_added, $quest_action->group,
      $game_user->id);
  }

  // Start the energy clock again.
  if ($old_energy == $game_user->energy_max) {
    $sql = 'update users set energy_next_gain = "%s" where id = %d;';
    db_query($sql, date('Y-m-d H:i:s',
      REQUEST_TIME + $energy_wait), $game_user->id);
  }

  // Update percentage completion.
  // No entry yet, add one.
  if (empty($pc->percent_complete)) {
    $sql = 'insert into quest_completion (fkey_users_id, fkey_quests_id,
      percent_complete) values (%d, %d, %d);';
    $result = db_query($sql, $game_user->id, $quest_id,
      $quest_action->percent_complete);
  }
  else {
    $sql = 'update quest_completion set percent_complete = least(
      percent_complete + %d, %d) where fkey_users_id = %d and
      fkey_quests_id = %d;';
    db_query($sql,
      floor($quest_action->percent_complete / $percentage_divisor),
      $percentage_target, $game_user->id, $quest_id);
  }

  $percent_complete = min($pc->percent_complete +
    floor($quest_action->percent_complete / $percentage_divisor),
    $percentage_target);

  // If s/he has completed the quest for the first time in a round,
  // give him/her a bonus.
  if ($percent_complete == $percentage_target) {

    if ($pc->bonus_given < $percentage_divisor) {
      zg_competency_gain($game_user, 'quest finisher');
      $game_user->experience += $quest_action->experience;
      $game_user->money += $money_added;

      $sql = 'update users set experience = experience + %d, money = money + %d
        where id = %d;';
      db_query($sql, $quest_action->experience, $money_added, $game_user->id);
      $sql = 'update quest_completion set bonus_given = bonus_given + 1
        where fkey_users_id = %d and fkey_quests_id = %d;';
      db_query($sql, $game_user->id, $quest_id);

      $quest_completion_html .= <<< EOF
<div class="title loot">{$game_text['quest']} Completed!</div>
<p>You have completed this $quest_lower and gained an extra <strong>$money_added
  $game_user->values and $game_quest->experience $experience</strong>!&nbsp; Complete
  all ${quest_lower}s in this group for an extra reward.</p>
EOF;
    }

    // At least 100% completed and somewhere to move?  Move!
    if ($percent_complete >= 100 &&
      (substr($game_quest->meta, 0, 5) == 'move:')) {
      $sql = 'select * from neighborhoods where id = %d;';
      $new_hood = db_query($sql, substr($game_quest->meta, 5))->fetch_object();
      $quest_completion_html .= <<< EOF
<p>You are now at <strong>{$new_hood->name}</strong>.</p>
EOF;
      $game_user->fkey_neighborhood_id = $new_hood->id;
      $game_user->location = $new_hood->name;
      $sql = 'update users set fkey_neighborhoods_id = %d where id = %d;';
      db_query($sql, $new_hood->id, $game_user->id);
    }

    // Did they complete all quests in the group?
    $sql = 'select * from quest_group_completion
      where fkey_users_id = %d and fkey_quest_groups_id = %d;';
    $result = db_query($sql, $game_user->id, $game_quest->group);
    $qgc = db_fetch_object($result);

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

      if (($quest_group->completed == $quest_group->total) &&
        ($quest_group->ready_for_bonus == 1)) {

        // Woohoo! User just completed an entire group!
        $quest_completion_html .= <<< EOF
<div class="title loot">Congratulations!</div>
<p>You have completed all {$quest_lower}s in this group and have gained extra skill
points!</p>
<p class="second"><a href="/$game/increase_skills/$arg2/none">You
have <span class="highlighted">$quest_group->completed</span> new skill points
to spend</a></p>
EOF;
        zg_competency_gain($game_user, 'quest groupie');

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

      if ($quest_group->completed == ($quest_group->total * 2)) {

        // Woohoo! User just completed an entire group the second time!
        $sql = 'select * from quest_group_bonus
          where fkey_quest_groups_id = %d;';
        $result = db_query($sql, $game_quest->group);
        $item = db_fetch_object($result);
        $eq_id = $item->fkey_equipment_id;
        $land_id = $item->fkey_land_id;
        $st_id = $item->fkey_staff_id;

        // Anything to give him/her?
        if (($eq_id + $land_id + $st_id) > 0) {
          zg_competency_gain($game_user, 'second-mile saint');

          // Equipment bonus.
          if ($eq_id > 0) {

            $game_equipment = zg_fetch_equip_by_id($game_user->id, $eq_id);
            list($eq_success, $eq_reason, $eq_details) =
              zg_equipment_gain($game_user, $eq_id, 1, 0);

            if (!$eq_success) {
              slack_send_message('could not give 2nd-round eq bonus for quest ' . $game_quest->id .
                " due to $eq_success, $eq_reason, $eq_details",
                $slack_channel);
              $response = slack_send_message($game_user, $slack_channel);
              if ($response !== TRUE) {
                firep($response, 'slack response');
              }
              $response = slack_send_message($game_quest, $slack_channel);
              if ($response !== TRUE) {
                firep($response, 'slack response');
              }
              $response = slack_send_message($game_equipment, $slack_channel);
              if ($response !== TRUE) {
                firep($response, 'slack response');
              }
            }

            $quest_completion_html .= zg_render_equip($game_user,
              $game_equipment,
              $ai_output,
              [
                'equipment-succeeded' => 'quest-bonus',
                'quest-do-again-id' => $game_quest->id,
              ]
            );
            $outcome_reason = '';
          }

          // FIXME: land bonus here.

          // Staff bonus.
          if ($st_id > 0) {

            $game_staff = zg_fetch_staff_by_id($game_user, $st_id);
            list($st_success, $st_reason, $st_details) =
              zg_staff_gain($game_user, $st_id, 1, 0);

            if (!$st_success) {
              slack_send_message('could not give 2nd-round st bonus for quest ' . $game_quest->id .
                " due to $st_success, $st_reason, $st_details",
                $slack_channel);
              $response = slack_send_message($game_user, $slack_channel);
              if ($response !== TRUE) {
                firep($response, 'slack response');
              }
              $response = slack_send_message($game_quest, $slack_channel);
              if ($response !== TRUE) {
                firep($response, 'slack response');
              }
              $response = slack_send_message($game_staff, $slack_channel);
              if ($response !== TRUE) {
                firep($response, 'slack response');
              }
            }

            $quest_completion_html .= zg_render_staff($game_user,
              $game_staff,
              $ai_output,
              [
                'equipment-succeeded' => 'quest-bonus',
                'quest-do-again-id' => $game_quest->id,
              ]
            );
            $outcome_reason = '';
          }

          // Update quest_groups_completion.
          $sql = 'update quest_group_completion set times_completed = 2
          where fkey_users_id = %d and fkey_quest_groups_id = %d;';
          $result = db_query($sql, $game_user->id, $game_quest->group);
        }
        else {
          // We don't have a bonus yet.
          $quest_completion_html .= <<< EOF
<div class="title loot">Congratulations!</div>
<div class="quest-icon"><img
src="/sites/default/files/images/quests/stlouis-soon.png"></div>
<div class="quest-details">
<div class="quest-name loot">You have completed all {$quest_lower}s in
  this group a second time!</div>
<div class="quest-description">Unfortunately, we have nothing to give you
  yet &mdash; we're still coding it!</div>
<p class="second">&nbsp;</p>
</div>
EOF;
        }
      }
    }
  }

  // Check for loot -- equipment.
  if ($quest_action->chance_of_loot > 0 && $quest_action->chance_of_loot < 30) {
    $game_equipment = zg_fetch_equip_by_id($game_user->id, $quest_action->fkey_loot_equipment_id);
    $under_limit = $game_equipment->quantity_limit > (int) $game_equipment->quantity;

    // Haven't gotten any of this loot yet?  Bump loot chance up to 30%.
    if ($game_equipment->quantity == 0) {
      $quest_action->chance_of_loot = 30;
    }
  }

  if ((($game_user->level <= 6 && $quest_action->chance_of_loot > 0)
      || $quest_action->chance_of_loot >= mt_rand(1, 99))
    && ($under_limit || $game_equipment->quantity_limit == 0)) {

    $loot = zg_fetch_equip_by_id($game_user, $quest_action->fkey_loot_equipment_id);
    $cumulative_expenses = $game_user->expenses + $loot->upkeep;
    if ((int) $game_user->income >= $cumulative_expenses) {

      // FIXME: Special case for Drunken Stupor.
      if ($quest_action->fkey_loot_equipment_id == 36) {
        zg_competency_gain($game_user, 'drunk');
      }

      zg_equipment_gain($game_user, $quest_action->fkey_loot_equipment_id);
      zg_competency_gain($game_user, 'looter');
      $loot_html = zg_render_equip($game_user, $loot, $ai_output,
        ['equipment-succeeded' => 'loot']);
    }
  }

  // Check for loot -- staff.
  if ($quest_action->chance_of_loot_staff > 0 && $quest_action->chance_of_loot_staff < 30) {
    $game_staff = zg_fetch_staff_by_id($game_user->id, $quest_action->fkey_loot_staff_id);
    $under_limit = $game_staff->quantity_limit > (int) $game_staff->quantity;

    // Haven't gotten any of this loot yet?  Bump loot chance up to 30%.
    if ($game_staff->quantity == 0) {
      $quest_action->chance_of_loot_staff = 30;
    }
  }

  if ((($game_user->level <= 6 && $quest_action->chance_of_loot_staff > 0)
      || $quest_action->chance_of_loot_staff >= mt_rand(1, 99))
    && ($under_limit || $game_staff->quantity_limit == 0)) {

    $loot = zg_fetch_staff_by_id($game_user, $quest_action->fkey_loot_staff_id);
    $cumulative_expenses = $game_user->expenses + $loot->upkeep;
    if ((int) $game_user->income >= $cumulative_expenses) {

      zg_staff_gain($game_user, $quest_action->fkey_loot_staff_id);
      zg_competency_gain($game_user, 'looter');
      $loot_html = zg_render_staff($game_user, $loot, $ai_output,
        ['equipment-succeeded' => 'loot']);
    }
  }

  // Refetch user object, update game quest object.
  $game_user = zg_fetch_user();
  $game_quest->completed_percent = $percent_complete;
  list($game_quest->rgb, $game_quest->width, $game_quest->completed_percent_overlay) =
    zg_get_quest_completion($game_quest->completed_percent,
      $percentage_target, $percentage_divisor);
  $game_quest->exp_added_str = "You gained <strong>$game_quest->experience</strong>";
  $game_quest->money_added_str = "You gained <strong>$money_added</strong>";
  $game_quest->loot_html = $loot_html;
  $game_quest->quest_completion_html = $quest_completion_html;
  $game_quest->quest_actions = zg_fetch_quest_actions($game_user, $game_quest);
}
else {
  $loot_html = $quest_completion_html = '';
}
$game_quest->outcome = $outcome_reason;

/* ------ VIEW ------ */
zg_fetch_header($game_user);
zg_show_ai_output($phone_id, $ai_output);

$sql = 'select name from quest_groups where id = %s;';
$result = db_query($sql, $game_quest->group);
$qg = db_fetch_object($result);
//firep($qg);

$location = str_replace('%location', $location, $qg->name);

// Show beginning quests, keep location from user.
if ($game_user->level < 6) {
  $location = $older_missions_html = $newer_missions_html = '';
}

// FIXME: just show arrows if zg_fetch_quest_groups() has groups before or
// after this one.  don't query the db by hand.
$sql = 'select name from quest_groups where id = %s;';
$qgoo = db_query($sql, $game_quest->group - 1)->fetch_object();

if (!empty($qgoo->name) && ($game_quest->group <= 1000)) {
  $older_group = $game_quest->group - 1;
  $older_missions_html = <<< EOF
<a href="/$game/$quest_link/$arg2/$older_group#group-{$older_group}">&lt;&lt;</a>
EOF;
}

$sql = 'select min(required_level) as min from quests where `group` = %d;';
$qgno = db_query($sql, $game_quest->group + 1)->fetch_object();

if (!empty($qgno->min) && ($qgno->min <= $game_user->level + 1) &&
  ($game_quest->group <= 1000)) {
  $newer_group = $game_quest->group + 1;
  $newer_missions_html = <<< EOF
<a href="/$game/$quest_link/$arg2/$newer_group#group-{$newer_group}">&gt;&gt;</a>
EOF;
}

$loc_quests = t('@location @quests', [
  '@location' => $location,
  '@quests' => "{$game_text['quest']}s",
]);

  $loc_quests = <<< EOF
<a href="/$game/quest_groups/$arg2#group-{$game_quest->group}">$loc_quests</a>
EOF;

$title = "<span class=\"quest-group-title\" $older_missions_html $loc_quests $newer_missions_html</span>";

// Reread quest group object.
$qgo = zg_fetch_quest_groups($game_user, $group_id);
$qgo->showExpanded = TRUE;
if ($game_user->level >= 6) {
  $qgo->titleHtml = $title;
}

$game_quest->optionShowBeforeTitle = TRUE;
array_unshift($qgo->q, $game_quest);
//firep($game_quest, 'game_quest object at show');
echo <<< EOF
<div class="swiper-container">
  <div class="swiper-wrapper">
EOF;
zg_show_quest_group_slide($game_user, $qgo);
echo <<< EOF
  </div>
</div>
EOF;

db_set_active('default');
