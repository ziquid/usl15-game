<?php

/**
 * @file
 * Includes for new users.
 */

/**
 * Init the includes.
 */
function game_new_user_init() {
  firep("we have a new user!");
}

// HOOKS

/**
 * Implements hook_fetch_quest_group_ID().
 */
function game_new_user_fetch_quest_group_0($game_user, &$qg) {

//  firep($qg, 'quest group sent to hook_fetch_quest_group');

  // Hide what you've got to hide.
  if ($game_user->level < 6) {
    $qg->name = '';
    $qg->swiperPaginationClasses = $qg->swiperArrowClasses = 'hide';
    if ($qg->quest_completion->completed === 0) {
      $qg->questGroupCompletionClasses = 'hide';
    }
  }

  $newUserHelpShown = game_get_value($game_user, 'newUserHelpShown', []);
  firep($newUserHelpShown, 'new user help shown');

  // No experience: intro text, hide extra quests.
  if ($game_user->experience == 0) {
    $qg->description = <<< EOF
<div class="new-user">
<div class="wise_old_man_small">
</div>
<p>The city elder continues.</p>
<p class="second">&quot;If you wish to lead this city,
  you must learn more about it.</p>
<p class="second">&quot;Perform your first mission by touching its picture or title.&quot;</p>
</div>
EOF;
    $qg->descriptionShowExpanded = TRUE;
    unset($qg->q[2], $qg->q[3], $qg->q[4]);
  }
  elseif ($game_user->level == 1) {

    // Have done a quest once.
    $qg->description = <<< EOF
<div class="new-user">
<div class="wise_old_man_small">
</div>
<p>&quot;Congratulations!&nbsp; Your stats have increased and you found an item!</p>
<p class="second">&quot;Finish your first mission by touching the <strong>Do Again</strong>
  button.&quot;</p>
</div>
EOF;
    $qg->descriptionShowExpanded = TRUE;
    unset($qg->q[3], $qg->q[4]);
  }
  elseif (!array_key_exists('01SecondMission', $newUserHelpShown)) {
    $qg->description = <<< EOF
<div class="new-user">
  <div class="wise_old_man_small">
  </div>
  <p>&quot;You have finished a mission!</p>
  <p class="second">&quot;blah.&quot;</p>
</div>
EOF;
    $qg->descriptionShowExpanded = TRUE;
    $newUserHelpShown['01SecondMission'] = TRUE;
    game_set_value($game_user, 'newUserHelpShown', $newUserHelpShown);
  }

  elseif ($game_user->level == 3) {
    $qg->description = <<< EOF
<div class="new-user">
  <div class="wise_old_man_small">
  </div>
  <p>&quot;You have attained Level 3!</p>
  <p class="second">&quot;Each time you level up, your Energy and Actions refill.&nbsp;
    Your first mission is complete, so perform the next mission by touching it.&quot;</p>
</div>
EOF;
    $qg->descriptionShowExpanded = TRUE;
  }

  elseif (FALSE) {
    $qg->description = <<< EOF
<div class="welcome">
  <div class="wise_old_man_small">
  </div>
  <p>The city elder continues.</p>
  <p class="second">&quot;If you wish to lead this city,
    you will need to learn more about it.&nbsp; I have some things for you to
    do now.&nbsp; I will come back later when you are ready for more.</p>
    <p class="second">&quot;To perform a mission, touch its picture or title.&quot;</p>
  <ul>
    <li>Each mission completed gives you more $game_user->values and
      $experience</li>
    <li>Wait and rest for a few minutes if you run out of Energy</li>
  </ul>
</div>
EOF;

    //  firep($qg, 'quest group sent to hook_fetch_quest_group');
  }

}

/**
 * Implements hook_fetch_quest_groups().
 */
function game_new_user_fetch_quest_groups($game_user, &$qgs) {

//  firep($qgs, 'quest group sent to hook_fetch_quest_group');

  // New user: hide extra groups.
  if ($game_user->level < 6) {
    foreach ($qgs as $k => &$qg) {
      if ($k > 0) {
        unset($qgs[$k]);
      }
    }
  }
}
