<?php

/**
 * @file
 * Show a user's competencies.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of zg_defs include: no
 * .
 */

global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();

if (empty($game_user->username) || $game_user->username == '(new player)')  {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

// Refresh every 30 seconds.
drupal_set_html_head('<meta http-equiv="refresh" content="30">');

zg_fetch_header($game_user);
zg_show_profile_menu($game_user);

$phone_id_to_check = $phone_id;
if ($arg3 != '') {
  $phone_id_to_check = $arg3;
}

// Fetch user by phone ID or user ID.
if (substr($arg3, 0, 3) == 'id:') {
  $item = zg_fetch_user_by_id((int) substr($arg3, 3));
  $phone_id_to_check = $item->phone_id;
}
else {
  $item = zg_fetch_user_by_id($phone_id_to_check);
}

if (($phone_id_to_check == $phone_id) ||
  ($_GET['comp_show_level'] == 'yes') ||
  $game_user->meta == 'admin') {
  $show_all = TRUE;
}
else {
  $show_all = FALSE;
}


//  $party_title = preg_replace('/^The /', '', $item->party_title);

if (!empty($item->clan_acronym)) {
  $clan_acronym = "($item->clan_acronym)";
  $clan_link = $item->clan_name;
}
else {
  $clan_link = t('None');
}

if ($item->is_clan_leader) {
  $clan_acronym .= '*';
  $clan_link .= " (leader)";
}

if (($game_user->fkey_clans_id) &&
  ($game_user->fkey_clans_id == $item->fkey_clans_id)) {

    $clan_link = '<a href="/' . $game . '/clan_list/' . $arg2 .
      '/' . $game_user->fkey_clans_id . '">' . $clan_link . '</a>';

}

$sql = 'select count(*) as total from competencies;';
$result = db_query($sql);
$count = db_fetch_object($result);
$total_comps = $count->total - 1;

$sql = 'select count(*) as total from user_competencies
  where fkey_users_id = %d;';
$result = db_query($sql, $item->id);
$count = db_fetch_object($result);
$user_comps = $count->total;
$time_left = '';

zg_alter('competency_gain_wait', $game_user, $competency_gain_wait_time,
  $competency_gain_wait_time_str);

if ($phone_id_to_check == $phone_id || $game_user->meta == 'admin') {
  $stats = t('You have discovered @user out of @total competencies',
    ['@user' => $user_comps, '@total' => $total_comps]);

  $fast_comps_30 = zg_timed_bonus_in_effect($item, 'fast_comps_30');
  if ($fast_comps_30->allowed) {
    $competency_gain_wait_time = min($competency_gain_wait_time, 30);
    $competency_gain_wait_time_str = '30 seconds';
    $time_left = ' (' . $fast_comps_30->hours_remaining . 'h' .
      $fast_comps_30->minutes_remaining . 'm' . $fast_comps_30->seconds_remaining .
      's&nbsp;left)';
  }

  $fast_comps_15 = zg_timed_bonus_in_effect($item, 'fast_comps_15');
  if ($fast_comps_15->allowed) {
    $competency_gain_wait_time = min($competency_gain_wait_time, 15);
    $competency_gain_wait_time_str = '15 seconds';
    $time_left = ' (' . $fast_comps_15->hours_remaining . 'h' .
      $fast_comps_15->minutes_remaining . 'm' . $fast_comps_15->seconds_remaining .
      's&nbsp;left)';
  }
}
else {
  $stats = '';
}

echo <<< EOF
<div class="title">
$item->ep_name <span class="username">$item->username</span> $clan_acronym
</div>
<div class="subsubtitle">$stats</div>
<div class="subsubtitle">Recently-increased competencies are yellow<br>
Wait $competency_gain_wait_time_str before increasing them $time_left</div>
<div class="user-profile">
EOF;

// Show more stats if it's you or admin.
if ($phone_id_to_check == $phone_id || $game_user->meta == 'admin') {
  $sql = 'SELECT * FROM  `user_competencies`
    LEFT JOIN competencies ON fkey_competencies_id = competencies.id
    WHERE fkey_users_id = %d
    ORDER BY level DESC, name ASC;';
  $result = db_query($sql, $item->id);
  while ($item = db_fetch_object($result)) {
    $item->name = zg_competency_name($item->name);
    $comps[] = $item;
  }

  $old_level = 0;

  foreach ($comps as $comp) {

    $level = $comp->level;
    $comp = (object) array_merge(
      (array) $comp,
      (array) zg_competency_level($game_user,
        intval($comp->fkey_competencies_id))
    );
// firep($comp, 'competency');
    $pip = '';

    for ($c = 1; $c <= 5; $c++) {
      $competent = ($c <= $comp->level) ? 'attained' : '';
      $pip .= '<span class="competency-pip ' . $competent . '"></span>';
    }

    if ((REQUEST_TIME - strtotime($comp->timestamp)) < $competency_gain_wait_time) {
      $too_soon = 'too-soon';
    }
    else {
      $too_soon = '';
    }

    $need_more = $comp->next - $comp->use_count;

    if ($level != $old_level) {
      echo '<div class="title">~~ Level ' . $level
      . ' ~~</div>';
    }

    echo <<< EOF
<div class="heading wider initial-caps">$comp->name:</div>
<div class="value">
  $pip
  <span class="small $too_soon">
    &nbsp;($comp->level_name, next: +$need_more)
  </span>
</div>
<br>
EOF;

    $old_level = $level;
  }
}

db_set_active();
?>

</div>
