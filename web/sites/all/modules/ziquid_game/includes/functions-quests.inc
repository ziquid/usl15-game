<?php

/**
 * @file
 * Quest functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * .
 */

/**
 * Fetch the list of quest groups to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param null|int $id
 *   Which group to fetch, or all if null.
 *
 * @return \stdClass|\stdClass[]
 *   A quest group object or the entire array of quest group objects.
 */
function zg_fetch_quest_groups(\stdClass $game_user, $id = NULL) {
  $data = [];

  // Admin?  Show all quests.
  $active_quests = ($game_user->meta == 'admin') ? '' :
    'and quests.active = 1 and required_level <= %d';

  // SQL for quests.
  $sql_quests = 'select quests.*,
  quest_completion.percent_complete as completed_percent,
  neighborhoods.name as hood, (neighborhoods.xcoor > 0) as hood_is_habitable, 
  comp1.name as competency_name_1
  from quests
  LEFT OUTER JOIN neighborhoods
  ON quests.fkey_neighborhoods_id = neighborhoods.id
  LEFT OUTER JOIN quest_completion
  ON quest_completion.fkey_quests_id = quests.id
  AND quest_completion.fkey_users_id = %d
  LEFT JOIN competencies as comp1
  ON fkey_enhanced_competencies_id = comp1.id
  where `group` = %d
  ' . $active_quests . '
  order by required_level, id ASC;';

  // Fetch a single group or all quest groups.
  if (is_numeric($id)) {
    $sql = 'SELECT qg.* FROM `quest_groups` qg
      where qg.id = %d;';
  }
  else {
    $sql = 'SELECT qg.* FROM `quest_groups` qg
    order by qg.id ASC';
  }

  // Allow altering of SQL query.
  zg_alter('fetch_quest_groups_sql', $game_user, $sql);

  $result = db_query($sql, $id);
  while ($item = db_fetch_object($result)) {
    $data[$item->id] = $item;
  }

  foreach ($data as &$item) {
    list($item->quest_completion,
      $item->percentage_target,
      $item->percentage_divisor,
      $item->next_group_html) =
    zg_fetch_quest_group_stats($game_user, $active_quests, $item->id);

    // Show?  if a specific group was requested, always show it.
    $show = is_numeric($id) ? TRUE : FALSE;

    // Always show Introductory missions.
    if ($item->id == 0) {
      $show = TRUE;
    }

    // If they have finished the previous <1000 group, show this one.
    if ($item->id < 1000 && $item->id > 0) {
      $last = $data[$item->id - 1];
      if (is_numeric($last->quest_completion->completed) &&
        ($last->quest_completion->completed > 0 &&
        $last->quest_completion->completed == $last->quest_completion->total) ||
        ($last->percentage_divisor > 1)) {
        $show = TRUE;
      }
    }

    // Highlighted in this hood?  Show it.
    if ($item->highlight_in_hood == $game_user->fkey_neighborhoods_id) {
      $show = TRUE;
    }

    // Allow altering of show flag.
    $show |= zg_alter('flag_show_quest_group_' . $item->id, $game_user, $item);

    if (!$show) {
      if ($game_user->meta == 'admin') {
        $item->class = 'obfuscate admin-obfuscate clickable';
      }
      else {
        $item->class = 'obfuscate';

        // Normal group?  Obfuscate.  Otherwise, hide.
        if ($item->id < 1000) {
          $item->name = '???';
          $item->description = t('Complete a previous mission group to see this one.');
          $item->quest_completion->completed = '???';
          $item->quest_completion->total = '???';
        }
        else {
          $item->class = 'hidden';
        }
      }
    }
    else {
      $item->class = 'clickable';
    }

    // Clickable?  Grab quest objects to show.
    if (strpos($item->class, 'clickable') !== FALSE) {
      $item->q = [];
      $order_in_group = 0;
      $result = db_query($sql_quests, $game_user->id, $item->id, $game_user->level + 1);
      while ($q = db_fetch_object($result)) {

        list($q->rgb, $q->width, $q->completed_percent_overlay) =
          zg_get_quest_completion($q->completed_percent,
          $item->percentage_target, $item->percentage_divisor);

        zg_get_icon_path($game_user, $q, 'quests');

        $q->description = str_replace(
          ['  ', '&th;', '&q;'],
          ['&nbsp; ', '<span class="thought">', '<div class="quote">'],
          $q->description);

        $q->order_in_group = ++$order_in_group;

        if ($order_in_group == 1 || $game_user->meta == 'admin') {
          $q->should_be_shown = TRUE;
        }
        else {
          $q->should_be_shown = ($prior_completed_percent > 0);
        }

        if ($q->should_be_shown) {
          $q->quest_actions = zg_fetch_quest_actions($game_user, $q);
        }
        else {
          $item->quest_completion->total = '???';
        }
        $prior_completed_percent = $q->completed_percent;

        $item->q[$q->id] = $q;
      }
      if (!count($item->q)) {
        $item->class = 'hidden';
      }
    }

//firep($item, 'items');
    // Allow altering of individual qgos.
    zg_alter('fetch_quest_group_' . $item->id, $game_user, $item);
  }

  // Allow altering of full qgo object.
  zg_alter('fetch_quest_groups', $game_user, $data);
//  firep($data, 'data');
  return is_numeric($id) ? $data[$id] : $data;
}

/**
 * Fetch actions for a quest, if any.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $q
 *   The quest object.
 *
 * @return bool|\stdClass[]
 *   The actions associated with the quest, if any, or FALSE if none.
 */
function zg_fetch_quest_actions(\stdClass $game_user, \stdClass $q) {

  if (!zg_get_default('has_quest_actions')) {
    return FALSE;
  }

  $actions = [];
  $sql = 'select * from quest_actions where fkey_quests_id = %d;';
  $result = db_query($sql, $q->id);
  while ($item = db_fetch_object($result)) {
    $actions[$item->id] = $item;
  }

  // No quest actions defined?  Synthesize one from the quest itself.
  if (!count($actions)) {
    $actions[0] = clone $q;
    $actions[0]->fkey_quests_id = $q->id;
    $actions[0]->id = 0;
    $actions[0]->action = $q->name;
    $actions[0]->response = t('Success!');
  }

  foreach ($actions as $action) {

    if (substr($action->meta, 0, 11) == 'completion:') {
      $completion_req = (int) substr($action->meta, 11);
      if ($completion_req > $q->completed_percent) {
        unset($actions[$action->id]);
        continue;
      }
    }

    $action->response = str_replace(
      ['  ', '&th;'],
      ['&nbsp; ', '<span class="thought">'],
      $action->response);
    foreach ($action as $prop => $value) {
      if (in_array($prop, ['id', 'active'])) {
        continue;
      }
      if ($value === '0' || $value === '') {
        if (property_exists($q, $prop)) {
          $action->$prop = $q->$prop;
        }
      }
    }

  }

  // Allow altering of quest actions.
  zg_alter('fetch_quest_actions_for_quest_' . $q->id, $game_user, $actions);
  zg_alter('fetch_quest_actions', $game_user, $actions);
//  firep($q, 'quest at zg_fetch_quest_actions');
//  firep($actions, 'actions at zg_fetch_quest_actions');
  return $actions;
}

/**
 * Fetch the list of highlighted quest groups to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass[]
 *   The array of quest group objects.
 */
function zg_fetch_highlighted_quest_groups(\stdClass $game_user) {
  $data = $data2 = [];
  $sql = 'SELECT qg.*, q.required_level FROM `quest_groups` qg
    left join quests q on q.group = qg.id
    where q.required_level <= %d;';
//    group by qg.id';
  $result = db_query($sql, $game_user->level);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  foreach ($data as $item) {
    // Allow altering of qgos.
    zg_alter('fetch_quest_group', $game_user, $item);

    if ($item->highlight_in_hood == $game_user->fkey_neighborhoods_id) {
      $data2[] = $item;
    }
  }

  return $data2;
}

/**
 * Fetch the latest played quest group.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return mixed
 *   The latest quest group played.
 */
function zg_fetch_latest_quest_group(\stdClass $game_user) {
  if ($lqg = zg_get_value($game_user, 'actual_last_quest_groups_id')) {

    // Use zfqg($gu)[$id] instead of zfqg($gu, $id) so we get correct show flag.
    $lqgo = zg_fetch_quest_groups($game_user)[$lqg];
    if (strpos($lqgo->class, 'clickable') !== FALSE) {
      return $lqg;
    }
  }
  return $game_user->fkey_last_played_quest_groups_id;
}

/**
 * Show a quest group name, with a link to go to it.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $item
 *   The quest group object.
 * @param string $ai_output
 *   Unused.
 */
function zg_show_quest_group(\stdClass $game_user, \stdClass $item, &$ai_output) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $link = 'quest_groups';

  echo <<< EOF
  <span class="title">
    <div class="quest-group-name">
      <a href="/$game/$link/$arg2/$item->id#group-{$item->id}">
        $item->name
      </a>
    </div>
  </span>
EOF;
}

/**
 * Show a quest group object as a slide.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $qgo
 *   The quest group object.
 * @param array $options
 *   Various options to control the display.
 */
function zg_show_quest_group_slide(\stdClass $game_user, \stdClass $qgo, array $options = []) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  if (strpos($qgo->class, 'hidden') !== FALSE) {
    return;
  }

  if (strpos($qgo->class, 'clickable') !== FALSE) {
    $onclick = <<< EOF
onclick="jQuery(this).removeClass('overview');"
EOF;
  }
  else {
    $onclick = '';
  }

  if ($qgo->showExpanded) {
    $overview = '';
  }
  else {
    $overview = 'overview';
  }

  $description_class = strlen($qgo->description) ? '' : 'empty';
  if ($qgo->descriptionShowExpanded) {
    $description_class .= ' show-expanded';
  }
  if (!strlen($qgo->titleHtml)) {
    $qgo->titleHtml = <<< EOF
<div class="quest-group-name fit-box $qgo->class">$qgo->name {$game_text['qg_quests']}</div>
EOF;
  }
//firep($qgo, 'quest group object');
  echo <<< EOF
<div class="swiper-slide {$overview} quest-group-{$qgo->id}" data-hash="group-{$qgo->id}" $onclick>
  <span class="title">
EOF;

  $titleShown = FALSE;

  // Show any quest images.
  if (isset($qgo->q)) {
    echo '<div class="quest-images">';
    foreach ($qgo->q as $q) {
//firep($q, 'quest object');
      // Mark quests that can't be performed in this hood.
      $q->wrong_hood = $q->not_yet = '';
      if ($q->fkey_neighborhoods_id &&
        ($q->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id)) {
        $q->wrong_hood = 'wrong-hood';
        list(, , $q->actions_to_move,) = zg_get_actions_to_move($game_user,
          $game_user->fkey_neighborhoods_id, $q->fkey_neighborhoods_id);

        // Color code the wrong-hood icon.
        if ($q->actions_to_move > $game_user->actions) {
          $q->wrong_hood_color = 'red';
        }
        elseif ($q->actions_to_move > ($game_user->actions / 2)) {
          $q->wrong_hood_color = 'orange';
        }
        elseif ($q->actions_to_move > ($game_user->actions / 4)) {
          $q->wrong_hood_color = 'yellow';
        }
        else {
          $q->wrong_hood_color = 'green';
        }

        // 6 Actions or less and the user has 6 available?  Make it green.
        if ($q->actions_to_move <= 6 && $game_user->actions >= 6) {
          $q->wrong_hood_color = 'green';
        }
      }

      if ($q->required_level > $game_user->level) {
        $q->not_yet = 'not-yet';
        // Not yet trumps wrong hood.
        $q->wrong_hood = '';
      }

      if (!$titleShown && !$q->optionShowBeforeTitle) {
        echo <<< EOF
      {$qgo->titleHtml}
      <div class="quest-group-completion {$qgo->questGroupCompletionClasses}">
      <strong>{$qgo->quest_completion->completed}</strong> of {$qgo->quest_completion->total}
      {$game_text['quest']}s complete $qgo->next_group_html</div>
    <div class="description $description_class">
      $qgo->description
    </div>
EOF;
        $titleShown = TRUE;
      }

      if (strlen($q->wrong_hood) || strlen($q->not_yet)) {
        echo <<< EOF
          <div class="{$q->not_yet}{$q->wrong_hood}-wrapper overview-only">
EOF;
      }
      echo <<< EOF
<img class="overview-only {$q->wrong_hood}{$q->not_yet}"
  src="{$q->icon_path}"
  style="box-shadow: #{$q->rgb} 2px 2px 4px; border-color: #{$q->rgb}">
EOF;
      if (strlen($q->wrong_hood)) {
        echo <<< EOF
          <img class="wrong-hood-overlay colorize-{$q->wrong_hood_color}"
            src="http://www.clker.com/cliparts/a/Y/q/g/B/G/simple-black-car-th.png"
            alt="{$q->actions_to_move} actions to move">
        </div>
EOF;
      }
      if (strlen($q->not_yet)) {
        echo <<< EOF
          <span class="not-yet-overlay">Soon</span>
        </div>
EOF;
      }
      zg_show_quest_slide($game_user, $q);
    }
    echo '</div>';
  }

  if (!$titleShown) {
    echo <<< EOF
      {$qgo->titleHtml}
      <div class="quest-group-completion {$qgo->questGroupCompletionClasses}">
      <strong>{$qgo->quest_completion->completed}</strong> of {$qgo->quest_completion->total}
      {$game_text['quest']}s complete $qgo->next_group_html</div>
    <div class="description $description_class">
      $qgo->description
    </div>
EOF;
  }

  echo '</span></div>';
}

/**
 * Get quest group stats.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $group_to_show
 *   Which group to show.
 *
 * @return array
 *   Quest group stats array.
 */
function zg_fetch_quest_group_stats(\stdClass $game_user, $active_quests, $group_to_show) {

  // Get quest group stats.
  $sql = 'SELECT sum(bonus_given) as completed, count(quests.id) as total
    FROM `quests`
    left outer join quest_completion
    on quest_completion.fkey_quests_id = quests.id
    and fkey_users_id = %d
    where `group` = %d ' . $active_quests . ';';
  $result = db_query($sql, $game_user->id, $group_to_show, $game_user->level);
  $quest_group = db_fetch_object($result);
  $quest_group->completed += 0;

  $sql = 'SELECT times_completed FROM `quest_group_completion`
  where fkey_users_id = %d and fkey_quest_groups_id = %d;';
  $result = db_query($sql, $game_user->id, $group_to_show);
  $quest_group_completion = db_fetch_object($result);

  $percentage_target = 100;
  $percentage_divisor = 1;
  $next_group_html = '';

  if (is_object($quest_group_completion) &&
    property_exists($quest_group_completion, 'times_completed') &&
    $quest_group_completion->times_completed > 0) {
    $next_group_html = t('(2nd round)');
    $percentage_target = 200;
    $percentage_divisor = 2;
    $quest_group->completed -=
      ($quest_group->total * min($quest_group_completion->times_completed, 1));
  }

  return [
    $quest_group,
    $percentage_target,
    $percentage_divisor,
    $next_group_html,
  ];
}

/**
 * Get the percentage complete visual cues for a quest.
 *
 * @param int $completed_percent
 *   The percentage complete.
 * @param int $percentage_target
 *   Which percentage is considered fully complete.
 * @param int $percentage_divisor
 *   The divisor to get the percentage_target down to 100.
 *
 * @return array
 *   The RGB value corresponding to the percentage done, the width of a
 *   progress bar, and some CSS.
 */
function zg_get_quest_completion($completed_percent, $percentage_target, $percentage_divisor) {
  if (empty($completed_percent)) {
    $completed_percent = 0;
  }
  if ($completed_percent > floor($percentage_target / 2)) {
    $rgb = dechex(floor(($percentage_target - $completed_percent) /
      (6 * $percentage_divisor) + 4)) . 'c0';
  }
  else {
    $rgb = 'c' . dechex(floor(($completed_percent) /
      (4 * $percentage_divisor))) . '0';
  }

  if ($completed_percent == 100) {
    $css_overlay = '';
  }
  elseif ($completed_percent == 0) {
    $css_overlay = 'clip-path: circle(1%);';
  }
  else {
    // Figure out overlay.
    $pts = [[50, 0]];
    $cp = $completed_percent;
    if ($cp >= 12.5) {
      $pts[] = [100, 0];
    }
    else {
      $pts[] = [50 + $cp * 4, 0];
    }

    if ($cp >= 37.5) {
      $pts[] = [100, 100];
    }
    elseif ($cp > 12.5) {
      $pts[] = [100, ($cp - 12.5) * 4];
    }

    if ($cp >= 62.5) {
      $pts[] = [0, 100];
    }
    elseif ($cp > 37.5) {
      $pts[] = [(62.5 - $cp) * 4, 100];
    }

    if ($cp >= 87.5) {
      $pts[] = [0, 0];
    }
    elseif ($cp > 62.5) {
      $pts[] = [0, (87.5 - $cp) * 4, 100];
    }

    if ($cp > 87.5) {
      $pts[] = [($cp - 87.5) * 4, 0];
    }

    $css_overlay = 'clip-path: polygon(50% 50%';
    foreach ($pts as $pt) {
      $x = floor($pt[0]);
      $y = floor($pt[1]);
      if ($x == 100) {
        $x = 110;
      }
      if ($y == 100) {
        $y = 110;
      }
      $css_overlay .= ', ' . $x . '% ' . $y . '%';
    }
    $css_overlay .= ');';
  }
  $width = floor($completed_percent * 94 / $percentage_target) + 2;
  return [$rgb, $width, $css_overlay];
}

/**
 * Show the quest as a slide.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $item
 *   The quest object.
 */
function zg_show_quest_slide(\stdClass $game_user, \stdClass $item) {

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
//firep($item, 'quest given to zg_show_quest_slide()');
  $sql = 'select id from quests where `group` = %d
    order by id asc limit 1;';
  $result = db_query($sql, $item->group);
  $data = db_fetch_object($result);
  $seq_num = $item->id - $data->id;

  $name = str_replace(
    ['@clan', '@hood', '@location', '%party', '@username', '  ', '&th;'],
    [
      $game_user->clan_name, $game_text['hood'], $game_user->location,
      $game_user->party_title, $game_user->username, '&nbsp; ',
      '<span class="thought">',
    ],
    $item->name);

  $description = str_replace(
    ['@clan', '@hood', '@location', '%party', '@username', '  ', '&th;'],
    [
      $game_user->clan_name, $game_text['hood'], $game_user->location,
      $game_user->party_title, $game_user->username, '&nbsp; ',
      '<span class="thought">',
    ],
    $item->description);

  if (empty($item->completed_percent)) {
    $item->completed_percent = 0;
  }

  $active = ($item->active) ? '' : ' (inactive)';
  $img = $item->icon_path;
  $dont_show_at_all = FALSE;

  $wrong_hood = $wrong_hood_wrapper = $wrong_hood_overlay = $comment_start
    = $comment_end = $go_there = $do_again_button = '';

  // Has quest actions?  Don't allow clicking on image or title.
  if (property_exists($item, 'quest_actions') && is_array($item->quest_actions)) {
    $comment_start = '<!-- ';
    $comment_end = ' -->';
  }

  if (property_exists($item, 'quest_completion_html')) {
    $item->quest_completion_html = '<div class="quest-completed-wrapper">' .
      $item->quest_completion_html . '</div>';
  }

  // Need a specific hood?  Tell him/her to move.
  if ($item->fkey_neighborhoods_id &&
      ($item->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id)) {
    if ($item->optionShowBeforeTitle) {
      $dont_show_at_all = TRUE;
    }
    else {
      $wrong_hood = 'wrong-hood';
      $wrong_hood_wrapper = 'wrong-hood-wrapper';
      $description = t('This @quest can only be performed in %hood.',
        ['@quest' => $game_text['quest_lower'], '%hood' => $item->hood]);
      $comment_start = '<!-- ';
      $comment_end = ' -->';
      if ($item->hood_is_habitable) {
        $wrong_hood_overlay = <<< EOF
<img class="wrong-hood-overlay colorize-{$item->wrong_hood_color}"
  src="http://www.clker.com/cliparts/a/Y/q/g/B/G/simple-black-car-th.png"
  alt="{$item->actions_to_move} actions to move">
EOF;
        $go_there = t('Go there');
        if ($item->actions_to_move > 0) {
          $go_there .= " ({$item->actions_to_move}@)";
        }
        $go_there = <<< EOF
<form action="/$game/move/$arg2/$item->fkey_neighborhoods_id">
  <div class="quests-perform-button-wrapper $wrong_hood_wrapper">
    <input class="quests-perform-button $wrong_hood" type="submit" value="{$go_there}"/>
    $wrong_hood_overlay
  </div>
</form>
EOF;
        $wrong_hood_overlay = $comment_start . $wrong_hood_overlay . $comment_end;
      }
    }
  }

  // Need a higher level?
  if ($item->required_level > $game_user->level) {
    $wrong_hood = 'not-yet';
    $wrong_hood_wrapper = 'not-yet-wrapper';
    $description .= '<div class="not-yet">' . t('Requires level @level',
      ['@quest' => $game_text['quest_lower'], '@level' => $item->required_level]) .
      '</div>';
    $comment_start = '<!-- ';
    $comment_end = ' -->';
    $wrong_hood_overlay = <<< EOF
<span class="not-yet-overlay">Soon</span>
EOF;
    $go_there = '';
  }

  // Shouldn't be shown yet?
  if (!$item->should_be_shown) {
    $wrong_hood = 'not-yet';
    $wrong_hood_wrapper = 'not-yet-wrapper';
    $name = t('Perform the Previous @quest', ['@quest' => $game_text['quest']]);
    $description = '<div class="not-yet">before you can perform this one.</div>';
    $comment_start = '<!-- ';
    $comment_end = ' -->';
    $wrong_hood_overlay = <<< EOF
<span class="not-yet-overlay">Soon</span>
EOF;
    $go_there = '';
  }

  // Show the quest!
  if (property_exists($item, 'outcome')) {
    echo $item->outcome;
  }

  if (property_exists($item, 'loot_html')) {
    echo $item->loot_html;
  }

  if (property_exists($item, 'quest_completion_html')) {
    echo $item->quest_completion_html;
  }

  if ($dont_show_at_all) {
    return;
  }

  echo <<< EOF
<div class="quests">
  <div class="quest-icon $wrong_hood_wrapper">
    {$comment_start}<a href="/$game/quests_do/$arg2/$item->id">{$comment_end}
      <img src="$img" border="0" width="96" class="round4 $wrong_hood"
      style="box-shadow: #{$item->rgb} 3px 3px 6px; border-color: #{$item->rgb}">
      $wrong_hood_overlay
      <div class="quest-icon-baseline-overlay"></div>
      <div class="quest-icon-overlay" style="$item->completed_percent_overlay"></div>
    {$comment_start}</a>{$comment_end}
    <div class="quest-complete">
      <div class="quest-complete-percentage"
        style="background-color: #{$item->rgb}; width: {$item->width}px">
        &nbsp;
      </div>
      <div class="quest-complete-text">
        $item->completed_percent% complete
      </div>
    </div>
  </div>

  <div class="quest-details">
    <div class="quest-num">$item->group.$seq_num</div>
    <div class="quest-name $wrong_hood">
      {$comment_start}<a href="/$game/quests_do/$arg2/$item->id">{$comment_end}
        $name $active
      {$comment_start}</a>{$comment_end}
    </div>
    <div class="quest-description {$wrong_hood}">
      $description
    </div>
EOF;

  // Disbursements and Requirements.
  if (strlen($wrong_hood)) {
    // Wrong hood?  Show nothing.
  }
  elseif (property_exists($item, 'quest_actions') && is_array($item->quest_actions)) {
    // Quest actions?  Show them instead of disbursements and reqs.
    foreach ($item->quest_actions as $action) {
      print zg_render_quest_action($game_user, $action);
    }
  }
  elseif (empty($item->exp_added_str)) {
    echo <<< EOF
    <div class="quest-disbursements">
      // Disbursements //
    </div>
    <div class="quest-experience">
      +$item->experience {$game_text['experience']},
      +$item->min_money to $item->max_money $game_user->values
    </div>
EOF;

    $chance_of_loot = $item->chance_of_loot + $item->chance_of_loot_staff;

    if ($chance_of_loot > 0) {
      $chance_of_loot_html = 'chance_of_loot';
      if ($chance_of_loot <= 20) {
        $chance_of_loot_html = 'low_chance_of_loot';
      }
      elseif ($chance_of_loot >= 40) {
        $chance_of_loot_html = 'high_chance_of_loot';
      }
      echo zg_get_html($chance_of_loot_html);
    }

    echo <<< EOF
    <div class="quest-requisites">
      // Consumptions //
    </div>
    <div class="quest-required_energy">
      Requires $item->required_energy ${game_text['energy']}
    </div>
EOF;

    if ($item->clan_equipment_1_consumed_quantity > 0) {
      echo '<div class="quest-required_stuff">';
      _clan_required_equipment($game_user,
        $item->fkey_clan_equipment_1_consumed_id,
        $item->clan_equipment_1_consumed_quantity);
      echo '</div>';
    }

    if ($item->land_required_quantity +
      $item->equipment_1_required_quantity +
      $item->equipment_2_required_quantity +
      $item->equipment_3_required_quantity +
      $item->clan_equipment_1_required_quantity +
      $item->staff_required_quantity > 0) {

      echo <<< EOF
    <div class="quest-requisites">
      // Requisites //
    </div>
EOF;

      // Required land.
      if ($item->land_required_quantity > 0) {
        zg_required_land($game_user, $item->fkey_land_required_id,
          $item->land_required_quantity);
      }

      // Required equipment.
      if ($item->equipment_1_required_quantity +
        $item->equipment_2_required_quantity +
        $item->equipment_3_required_quantity +
        $item->clan_equipment_1_required_quantity > 0) {

        echo '<div class="quest-required_stuff">';

        if ($item->equipment_1_required_quantity > 0) {
          zg_required_equipment($game_user, $item->fkey_equipment_1_required_id,
            $item->equipment_1_required_quantity);
        }

        if ($item->equipment_2_required_quantity > 0) {
          zg_required_equipment($game_user, $item->fkey_equipment_2_required_id,
            $item->equipment_2_required_quantity);
        }

        if ($item->equipment_3_required_quantity > 0) {
          zg_required_equipment($game_user, $item->fkey_equipment_3_required_id,
            $item->equipment_3_required_quantity);
        }

        if ($item->clan_equipment_1_required_quantity > 0) {
          _clan_required_equipment($game_user,
            $item->fkey_clan_equipment_1_required_id,
            $item->clan_equipment_1_required_quantity);
        }

        echo '</div>';
      }

      // Required staff.
      if ($item->staff_required_quantity > 0) {
        zg_required_staff($game_user, $item->fkey_staff_required_id,
          $item->staff_required_quantity);
      }

      // Required party.
      if ($item->need_party > 0) {
        echo <<< EOF
    <div class="quest-required_energy">
      You must belong to a non-Underground $party_lower
    </div>
EOF;
      }

      // Required clan.
      if ($item->need_clan > 0) {
        echo <<< EOF
    <div class="quest-required_energy">
      You must belong to a clan
    </div>
EOF;
      }

      // Required job.
      if ($item->need_job > 0) {
        echo <<< EOF
    <div class="quest-required_energy">
      You must have a job
    </div>
EOF;
      }

    }
  }
  else {
    // Quest performed -- show accomplishments.
    echo <<< EOF
    <div class="quest-experience">
      $item->exp_added_str <strong>{$game_text['experience']}</strong>
    <br>
      $item->money_added_str <strong>$game_user->values</strong>
    </div>
  <!--</div>-->
  <!--$item->loot_html-->
  <!--<div class="quest-do-again">
    <div class="quest-do-again-inside">
      <a href="/$game/quests_do/$arg2/$item->id">
        Do Again
      </a>
    </div>-->
EOF;
    $do_again_button = zg_render_button('quests_do', 'Do Again', '/' . $item->id);
  }

  echo <<< EOF
  </div>
  $do_again_button
  $go_there
</div>
EOF;
}

/**
 * Render a quest action.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $qa
 *   The quest action object.
 *
 * @return string
 *   The rendered HTML.
 */
function zg_render_quest_action(\stdClass $game_user, \stdClass $qa) {

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  //firep($qa, 'quest action given to zg_render_quest_action()');

  $action = str_replace(
    ['@clan', '@hood', '@location', '%party', '@username', '  ', '&th;'],
    [
      $game_user->clan_name, $game_text['hood'], $game_user->location,
      $game_user->party_title, $game_user->username, '&nbsp; ',
      '<span class="thought">',
    ],
    $qa->action);
  $response = str_replace(
    ['@clan', '@hood', '@location', '%party', '@username', '  ', '&th;'],
    [
      $game_user->clan_name, $game_text['hood'], $game_user->location,
      $game_user->party_title, $game_user->username, '&nbsp; ',
      '<span class="thought">',
    ],
    $qa->response);

  $description = '';

  if (empty($qa->completed_percent)) {
    $qa->completed_percent = 0;
  }

  $active = ($qa->active) ? '' : ' (inactive)';

  $wrong_hood = $wrong_hood_wrapper = $wrong_hood_overlay = $comment_start
    = $comment_end = $go_there = '';

  // Need a specific hood?  Tell him/her to move.
  if ($qa->fkey_neighborhoods_id &&
    ($qa->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id)) {
    $wrong_hood = 'wrong-hood';
    $wrong_hood_wrapper = 'wrong-hood-wrapper';
    $description = t('This @quest @action can only be completed in %hood.',
      [
        '@quest' => $game_text['quest_lower'],
        '@action' => $game_text['action_lower'],
        '%hood' => $qa->hood,
      ]);
    $comment_start = '<!-- ';
    $comment_end = ' -->';
    $wrong_hood_overlay = <<< EOF
<img class="wrong-hood-overlay colorize-{$qa->wrong_hood_color}"
  src="http://www.clker.com/cliparts/a/Y/q/g/B/G/simple-black-car-th.png"
  alt="{$qa->actions_to_move} actions to move">
EOF;
    $go_there = t('Go there');
    if ($qa->actions_to_move > 0) {
      $go_there .= " ({$qa->actions_to_move}@)";
    }
    $go_there = <<< EOF
<form action="/$game/move/$arg2/$qa->fkey_neighborhoods_id">
  <div class="quests-perform-button-wrapper $wrong_hood_wrapper">
    <input class="quests-perform-button $wrong_hood" type="submit" value="{$go_there}"/>
    $wrong_hood_overlay
  </div>
</form>
EOF;
    $wrong_hood_overlay = $comment_start . $wrong_hood_overlay . $comment_end;
  }

  // Need a higher level?
  if ($qa->required_level > $game_user->level) {
    $wrong_hood = 'not-yet';
    $wrong_hood_wrapper = 'not-yet-wrapper';
    $description .= '<div class="not-yet">' . t('Requires level @level',
        ['@quest' => $game_text['quest_lower'], '@level' => $qa->required_level]) .
      '</div>';
    $comment_start = '<!-- ';
    $comment_end = ' -->';
    $wrong_hood_overlay = <<< EOF
<span class="not-yet-overlay">Soon</span>
EOF;
    $go_there = '';
  }

  $action_button = zg_render_button('quests_do', $action,
    '/' . $qa->fkey_quests_id . '/' . $qa->id, $wrong_hood);

  // Show the quest action!
  $output = <<< EOF
$qa->quest_completion_html
$qa->outcome
$qa->loot_html
$action_button
<div class="quest-action">
  <div class="quest-details">
      $active
    <div class="quest-description {$wrong_hood}">
      $description
    </div>
EOF;

  // Disbursements and Requirements.
  if (strlen($wrong_hood)) {
    // Wrong hood?  Show nothing.
  }
  elseif (empty($qa->exp_added_str)) {

    // No quest performed -- show disbursements and requirements.
    $chance_of_loot = $qa->chance_of_loot + $qa->chance_of_loot_staff;
    if ($chance_of_loot > 0) {
      $chance_of_loot_html = 'chance_of_loot';
      if ($chance_of_loot <= 20) {
        $chance_of_loot_html = 'low_chance_of_loot';
      }
      elseif ($chance_of_loot >= 40) {
        $chance_of_loot_html = 'high_chance_of_loot';
      }
//      $output .= zg_get_html($chance_of_loot_html);
    }

    $output .= <<< EOF
    <div class="quest-experience">
      Provides +$qa->experience {$game_text['experience']}, 
      +$qa->min_money to $qa->max_money $game_user->values
    </div>
EOF;

    $output .= <<< EOF
    <div class="quest-required_energy">
      Requires $qa->required_energy ${game_text['energy']}
    </div>
EOF;

    if ($qa->clan_equipment_1_consumed_quantity > 0) {
      $output .= '<div class="quest-required_stuff">' .
        zg_render_clan_required_equipment($game_user,
        $qa->fkey_clan_equipment_1_consumed_id,
        $qa->clan_equipment_1_consumed_quantity) .
      '</div>';
    }

    if ($qa->land_required_quantity +
      $qa->equipment_1_required_quantity +
      $qa->equipment_2_required_quantity +
      $qa->equipment_3_required_quantity +
      $qa->clan_equipment_1_required_quantity +
      $qa->staff_required_quantity > 0) {

      // Required land.
      if ($qa->land_required_quantity > 0) {
        $output .= zg_render_required_land($game_user, $qa->fkey_land_required_id,
          $qa->land_required_quantity);
      }

      // Required equipment.
      if ($qa->equipment_1_required_quantity +
        $qa->equipment_2_required_quantity +
        $qa->equipment_3_required_quantity +
        $qa->clan_equipment_1_required_quantity > 0) {

        $output .= '<div class="quest-required_stuff">';

        if ($qa->equipment_1_required_quantity > 0) {
          $output .= zg_render_required_equipment($game_user,
            $qa->fkey_equipment_1_required_id,
            $qa->equipment_1_required_quantity);
        }

        if ($qa->equipment_2_required_quantity > 0) {
          $output .= zg_render_required_equipment($game_user,
            $qa->fkey_equipment_2_required_id,
            $qa->equipment_2_required_quantity);
        }

        if ($qa->equipment_3_required_quantity > 0) {
          $output .= zg_render_required_equipment($game_user,
            $qa->fkey_equipment_3_required_id,
            $qa->equipment_3_required_quantity);
        }

        if ($qa->clan_equipment_1_required_quantity > 0) {
          $output .= zg_render_clan_required_equipment($game_user,
            $qa->fkey_clan_equipment_1_required_id,
            $qa->clan_equipment_1_required_quantity);
        }

        $output .= '</div>';
      }

      // Required staff.
      if ($qa->staff_required_quantity > 0) {
        $output .= zg_render_required_staff($game_user,
          $qa->fkey_staff_required_id,
          $qa->staff_required_quantity);
      }

      // Required party.
      if ($qa->need_party > 0) {
        $output .= <<< EOF
    <div class="quest-required_energy">
      You must belong to a non-Underground $party_lower
    </div>
EOF;
      }

      // Required clan.
      if ($qa->need_clan > 0) {
        $output .= <<< EOF
    <div class="quest-required_energy">
      You must belong to a clan
    </div>
EOF;
      }

      // Required job.
      if ($qa->need_job > 0) {
        $output .= <<< EOF
    <div class="quest-required_energy">
      You must have a job
    </div>
EOF;
      }

    }
  }
  else {
    // Quest action performed -- show accomplishments.
    $output .= <<< EOF
    <div class="quest-experience">
      $qa->exp_added_str <strong>{$game_text['experience']}</strong>
    <br>
      $qa->money_added_str <strong>$game_user->values</strong>
    </div>
EOF;
    $do_again_button = zg_render_button('quests_do', 'Do Again', '/' . $qa->id);
  }

  $output .= <<< EOF
  </div>
  $do_again_button
  $go_there
</div>
EOF;

  return $output;
}
