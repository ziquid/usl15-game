<?php

/**
 * @file
 * Menu functions for Ziquid games.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

/**
 * Implements hook_menu().
 */
function zg_menu() {

  $items = [];
  foreach (zg_get_game_list() as $game) {

    $items[$game . '/bounce/%'] = [
      'title' => 'Ziquid Game bounce',
      'page callback' => 'zg_bounce_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/changelog/%'] = [
      'title' => 'Ziquid Game Changelog',
      'page callback' => 'zg_changelog_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/choose_party/%/%'] = [
      'title' => 'Ziquid Game Choose Party',
      'page callback' => 'zg_choose_party_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/dashboard/%/%'] = [
      'title' => 'Ziquid Game Dashboard',
      'page callback' => 'zg_dashboard_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
      'file' => 'includes/menu/zg_dashboard.inc',
    ];

    $items[$game . '/elders/%'] = [
      'title' => 'Ziquid Game Elders',
      'page callback' => 'zg_elders_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/elders_ask_reset/%'] = [
      'title' => 'Ziquid Game Elders Ask Reset',
      'page callback' => 'zg_elders_ask_reset_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/elders_do_fill/%/%'] = [
      'title' => 'Ziquid Game Elders Do Fill',
      'page callback' => 'zg_elders_do_fill_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/elders_do_reset/%'] = [
      'title' => 'Ziquid Game Elders Do Reset',
      'page callback' => 'zg_elders_do_reset_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
      'file' => 'includes/menu/zg_elders_do_reset.inc',
    ];

    $items[$game . '/elders_enable_alpha/%'] = [
      'title' => 'Ziquid Game Elders Enable Alpha',
      'page callback' => 'zg_elders_enable_alpha_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/equipment/%'] = [
      'title' => 'Ziquid Game Equipment',
      'page callback' => 'zg_equipment_list_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/home/%'] = [
      'title' => 'Ziquid Game Home Page',
      'page callback' => 'zg_homepage_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/increase_skills/%/%'] = [
      'title' => 'Ziquid Game Increase Skills Page',
      'page callback' => 'zg_increase_skills_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/land/%'] = [
      'title' => 'Ziquid Game Land List',
      'page callback' => 'zg_land_list_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/move_do/%/%'] = [
      'title' => 'Ziquid Game Move Do',
      'page callback' => 'zg_move_do_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/quest_groups/%'] = [
      'title' => 'Ziquid Game Quest Groups List',
      'page callback' => 'zg_quest_groups_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/quests_do/%/%'] = [
      'title' => 'Ziquid Game Quests Do',
      'page callback' => 'zg_quests_do_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/staff/%'] = [
      'title' => 'Ziquid Game Staff',
      'page callback' => 'zg_staff_list_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/welcome/%'] = [
      'title' => 'Ziquid Game Welcome',
      'page callback' => 'zg_welcome_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];
  }

  /*

  $items[$game . '/user/%'] = [
    'title' => 'Ziquid Game User Profile Page',
    'page callback' => 'zg_userprofile_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/user_comp/%'] = [
    'title' => 'Ziquid Game User Competencies Page',
    'page callback' => 'zg_user_competencies_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/clan_list/%/%'] = [
    'title' => 'Ziquid Game Clan List Page',
    'page callback' => 'zg_clan_list_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/clan_list_available/%'] = [
    'title' => 'Ziquid Game Available Clan List Page',
    'page callback' => 'zg_clan_list_available_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/clan_msg/%/%'] = [
    'title' => 'Ziquid Game Clan Messages Page',
    'page callback' => 'zg_clan_msg_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/clan_announcements/%/%'] = [
    'title' => 'Ziquid Game Clan Announcements Page',
    'page callback' => 'zg_clan_announcements_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/block_user_toggle/%/%'] = [
    'title' => 'Ziquid Game Block User Toggle',
    'page callback' => 'zg_block_user_toggle_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/msg_delete/%/%'] = [
    'title' => 'Ziquid Game Message Delete Page',
    'page callback' => 'zg_msg_delete_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/party_msg/%'] = [
    'title' => 'Ziquid Game Party Message Page',
    'page callback' => 'zg_party_msg_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_ask_reset_skills/%'] = [
    'title' => 'Ziquid Game Elders Ask Reset Skills',
    'page callback' => 'zg_elders_ask_reset_skills_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_do_reset_skills/%'] = [
    'title' => 'Ziquid Game Elders Do Reset Skills',
    'page callback' => 'zg_elders_do_reset_skills_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_ask_purchase/%'] = [
    'title' => 'Ziquid Game Elders Ask Purchase',
    'page callback' => 'zg_elders_ask_purchase_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_do_purchase/%'] = [
    'title' => 'Ziquid Game Elders Do Purchase',
    'page callback' => 'zg_elders_do_purchase_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_check_receipt/%'] = [
    'title' => 'Ziquid Game Elders Check Receipt',
    'page callback' => 'zg_elders_check_receipt_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/google_checkout/%'] = [
    'title' => 'Ziquid Game Google Checkout',
    'page callback' => 'zg_google_checkout_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_set_password/%'] = [
    'title' => 'Ziquid Game Elders Set Password',
    'page callback' => 'zg_elders_set_password_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_rename/%'] = [
    'title' => 'Ziquid Game Elders Rename',
    'page callback' => 'zg_elders_rename_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/help/%'] = [
    'title' => 'Ziquid Game Help',
    'page callback' => 'zg_help_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/debates/%'] = [
    'title' => 'Ziquid Game Debates List',
    'page callback' => 'zg_debates_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/debates_challenge/%/%'] = [
    'title' => 'Ziquid Game Debates Challenge',
    'page callback' => 'zg_debates_challenge_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elections/%'] = [
    'title' => 'Ziquid Game Elections List',
    'page callback' => 'zg_elections_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elections_challenge/%/%'] = [
    'title' => 'Ziquid Game Elections Challenge',
    'page callback' => 'zg_elections_challenge_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/top20/%'] = [
    'title' => 'Ziquid Game Top 20 List',
    'page callback' => 'zg_top20_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/top_aldermen/%'] = [
    'title' => 'Ziquid Game Top Aldermen List',
    'page callback' => 'zg_top_aldermen_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/top_event_points/%'] = [
    'title' => 'Ziquid Game Top 100 Event Points',
    'page callback' => 'zg_top_event_points_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/choose_name/%'] = [
    'title' => 'Ziquid Game Choose Name',
    'page callback' => 'zg_choose_name_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/enter_referral_code/%'] = [
    'title' => 'Ziquid Game Enter Referral Code',
    'page callback' => 'zg_enter_referral_code_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/land_buy/%/%/%'] = [
    'title' => 'Ziquid Game Land Buy',
    'page callback' => 'zg_land_buy_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/land_sell/%/%/%'] = [
    'title' => 'Ziquid Game Land Sell',
    'page callback' => 'zg_land_sell_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/land_work/%/%/%'] = [
    'title' => 'Ziquid Game Land Work',
    'page callback' => 'zg_land_work_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/staff_hire/%/%/%'] = [
    'title' => 'Ziquid Game Staff Hire',
    'page callback' => 'zg_staff_hire_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/staff_fire/%/%/%'] = [
    'title' => 'Ziquid Game Staff Fire',
    'page callback' => 'zg_staff_fire_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/actions/%'] = [
    'title' => 'Ziquid Game Actions',
    'page callback' => 'zg_actions_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/actions_do/%/%'] = [
    'title' => 'Ziquid Game Actions Do',
    'page callback' => 'zg_actions_do_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/equipment_buy/%/%/%'] = [
    'title' => 'Ziquid Game Equipment Buy',
    'page callback' => 'zg_equipment_buy_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/equipment_sell/%/%/%'] = [
    'title' => 'Ziquid Game Equipment Sell',
    'page callback' => 'zg_equipment_sell_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/move/%/%'] = [
    'title' => 'Ziquid Game Move',
    'page callback' => 'zg_move_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/authenticate/%'] = [
    'title' => 'Ziquid Game Authenticate',
    'page callback' => 'zg_authenticate_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/elders_preferences/%'] = [
    'title' => 'Ziquid Game Preferences Page',
    'page callback' => 'zg_elders_preference_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/zombie_move/%/%/%'] = [
    'title' => 'Ziquid Game Zombie Move Page',
    'page callback' => 'zg_zombie_move_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/event_info/%'] = [
    'title' => 'Ziquid Game Event Info Page',
    'page callback' => 'zg_event_info_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/event_prizes/%'] = [
    'title' => 'Ziquid Game Event Prizes Page',
    'page callback' => 'zg_event_prizes_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items[$game . '/error/%/%'] = [
    'title' => 'Ziquid Game Error Page',
    'page callback' => 'zg_error_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];
  */
  return $items;
}

function zg_homepage_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_homepage', $phone_id);
}

function zg_userprofile_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_userprofile', $phone_id);
}

function zg_user_competencies_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_user_competencies', $phone_id);
}

function zg_clan_list_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_list', $phone_id, $clan_id);
}

function zg_clan_list_available_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_list_available', $phone_id);
}

function zg_clan_msg_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_msg', $phone_id, $clan_id);
}

function zg_clan_announcements_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_announcements', $phone_id, $clan_id);
}

function zg_block_user_toggle_callback($phone_id, $phone_id_to_block) {

  if (empty($phone_id) || empty($phone_id_to_block)) {
    return drupal_access_denied();
  }

  return theme('zg_block_user_toggle', $phone_id,
    check_plain(filter_xss($phone_id_to_block, [])));
}

function zg_msg_delete_callback($phone_id, $msg_id) {

  if (empty($phone_id) || !is_numeric($msg_id)) {
    return drupal_access_denied();
  }

  return theme('zg_msg_delete', $phone_id, $msg_id);
}

function zg_party_msg_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_party_msg', $phone_id);
}

function zg_increase_skills_callback($phone_id, $skill) {

  if (empty($phone_id) || empty($skill)) {
    return drupal_access_denied();
  }

  return theme('zg_increase_skills', $phone_id, $skill);
}

function zg_quest_groups_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $module_path = drupal_get_path('module', 'zg');
  drupal_add_js($module_path . '/js/zg_quest_groups.js', 'module', 'footer');
  return theme('zg_quest_groups', $phone_id);
}

function zg_quests_do_callback($phone_id, $quest_id) {

  if (empty($phone_id) || !is_numeric($quest_id)) {
    return drupal_access_denied();
  }

  return theme('zg_quests_do', $phone_id, $quest_id);
}

function zg_elders_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders', $phone_id);
}

function zg_elders_ask_reset_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_ask_reset', $phone_id);
}

function zg_elders_ask_reset_skills_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_ask_reset_skills', $phone_id);
}

function zg_elders_do_reset_skills_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_do_reset_skills', $phone_id);
}

function zg_elders_do_fill_callback($phone_id, $fill_type) {

  if (empty($phone_id) || empty($fill_type)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_do_fill', $phone_id, $fill_type);
}

function zg_elders_rename_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_rename', $phone_id);
}

function zg_elders_enable_alpha_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_enable_alpha', $phone_id);
}

function zg_elders_set_password_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_set_password', $phone_id);
}

function zg_elders_ask_purchase_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_ask_purchase', $phone_id);
}

function zg_elders_do_purchase_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_do_purchase', $phone_id);
}

function zg_elders_check_receipt_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_check_receipt', $phone_id);
}

function zg_google_checkout_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_google_checkout', $phone_id);
}

function zg_welcome_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  // Prime the cache.
  drupal_get_path('module', 'zg');

  return theme('zg_welcome', $phone_id);
}

function zg_choose_party_callback($phone_id, $party_id) {

  if (empty($phone_id) || !is_numeric($party_id)) {
    return drupal_access_denied();
  }

  return theme('zg_choose_party', $phone_id, $party_id);
}

function zg_help_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_help', $phone_id);
}

function zg_debates_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_debates_list', $phone_id);
}

function zg_debates_challenge_callback($phone_id, $position_id) {

  if (empty($phone_id) || !is_numeric($position_id)) {
    return drupal_access_denied();
  }

  return theme('zg_debates_challenge', $phone_id, $position_id);
}

function zg_elections_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elections_list', $phone_id);
}

function zg_elections_challenge_callback($phone_id, $position_id) {

  if (empty($phone_id) || !is_numeric($position_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elections_challenge', $phone_id, $position_id);
}

function zg_top20_list_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('zg_top20_list', $phone_id);
}

function zg_top_aldermen_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_top_aldermen_list', $phone_id);
}

function zg_top_event_points_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_top_event_points', $phone_id);
}

function zg_choose_name_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_choose_name', $phone_id);
}

function zg_enter_referral_code_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_enter_referral_code', $phone_id);
}

function zg_land_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_land_list', $phone_id);
}

function zg_land_buy_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_land_buy', $phone_id, $land_id, $quantity);
}

function zg_land_sell_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_land_sell', $phone_id, $land_id, $quantity);
}

function zg_land_work_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
      ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_land_work', $phone_id, $land_id, $quantity);
}

function zg_staff_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_staff_list', $phone_id);
}

function zg_staff_hire_callback($phone_id, $staff_id, $quantity) {

  if (empty($phone_id) || !is_numeric($staff_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_staff_hire', $phone_id, $staff_id, $quantity);
}

function zg_staff_fire_callback($phone_id, $staff_id, $quantity) {

  if (empty($phone_id) || !is_numeric($staff_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_staff_fire', $phone_id, $staff_id, $quantity);
}

function zg_actions_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_actions_list', $phone_id);
}

function zg_actions_do_callback($phone_id, $action_id) {

  if (empty($phone_id) || !is_numeric($action_id)) {
    return drupal_access_denied();
  }

  return theme('zg_actions_do', $phone_id, $action_id);
}

function zg_equipment_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_equipment_list', $phone_id);
}

function zg_equipment_buy_callback($phone_id, $equipment_id, $quantity) {

  if (empty($phone_id) || !is_numeric($equipment_id) ||
    !(is_numeric($quantity) || ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_equipment_buy', $phone_id, $equipment_id, $quantity);
}

function zg_equipment_sell_callback($phone_id, $equipment_id, $quantity) {

  if (empty($phone_id) || !is_numeric($equipment_id) ||
    !(is_numeric($quantity) || ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_equipment_sell', $phone_id, $equipment_id, $quantity);
}

function zg_move_callback($phone_id, $neighborhood_id) {

  if (empty($phone_id) || !is_numeric($neighborhood_id)) {
    return drupal_access_denied();
  }

  return theme('zg_move', $phone_id, $neighborhood_id);
}

function zg_move_do_callback($phone_id, $neighborhood_id) {

  if (empty($phone_id) || !is_numeric($neighborhood_id)) {
    return drupal_access_denied();
  }

  return theme('zg_move_do', $phone_id, $neighborhood_id);
}

function zg_authenticate_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_authenticate', $phone_id);
}

function zg_bounce_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('zg_bounce', $phone_id);
}

function zg_elders_preference_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_preferences', $phone_id);
}

function zg_zombie_move_callback($phone_id, $zombie_id, $hood_id) {

  if (empty($phone_id) || is_nan($zombie_id) || is_nan($hood_id)) {
    return drupal_access_denied();
  }

  return theme('zg_zombie_move', $phone_id, $zombie_id, $hood_id);
}

function zg_event_info_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_event_info', $phone_id);
}

function zg_event_prizes_callback($phone_id) {

  global $user;

  if (($user->roles[4] != 'web game access') ||
    ($phone_id != 'abc123')) {
    return drupal_access_denied();
  }

  return theme('zg_event_prizes', $phone_id);
}

function zg_error_callback($phone_id, $error_code) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('zg_error', $phone_id, $error_code);
}

function zg_changelog_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_changelog', $phone_id);
}

function zg_theme() {
  // theme "A" uses template "B" with args "C"

  return [

    'zg_homepage' => [
      'template' => 'templates/zg_homepage',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_userprofile' => [
      'template' => 'zg_userprofile',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_user_competencies' => [
      'template' => 'zg_user_competencies',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_clan_list' => [
      'template' => 'zg_clan_list',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ]
    ],

    'zg_clan_list_available' => [
      'template' => 'zg_clan_list_available',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_clan_msg' => [
      'template' => 'zg_clan_msg',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ]
    ],

    'zg_clan_announcements' => [
      'template' => 'zg_clan_announcements',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ]
    ],

    'zg_block_user_toggle' => [
      'template' => 'zg_block_user_toggle',
      'arguments' => [
        'phone_id' => NULL,
        'phone_id_to_block' => NULL,
      ]
    ],

    'zg_msg_delete' => [
      'template' => 'zg_msg_delete',
      'arguments' => [
        'phone_id' => NULL,
        'msg_id' => NULL,
      ]
    ],

    'zg_party_msg' => [
      'template' => 'zg_party_msg',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_increase_skills' => [
      'template' => 'templates/zg_increase_skills',
      'arguments' => [
        'phone_id' => NULL,
        'skill' => NULL,
      ]
    ],

    'zg_quest_groups' => [
      'template' => 'templates/zg_quest_groups',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_quests_do' => [
      'template' => 'templates/zg_quests_do',
      'arguments' => [
        'phone_id' => NULL,
        'quest_id' => NULL,
      ]
    ],

    'zg_elders' => [
      'template' => 'templates/zg_elders',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_elders_ask_reset' => [
      'template' => 'templates/zg_elders_ask_reset',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_elders_ask_reset_skills' => [
      'template' => 'zg_elders_ask_reset_skills',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_elders_do_reset_skills' => [
      'template' => 'zg_elders_do_reset_skills',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_elders_do_fill' => [
      'template' => 'templates/zg_elders_do_fill',
      'arguments' => [
        'phone_id' => NULL,
        'fill_type' => NULL,
      ]
    ],

    'zg_elders_rename' => [
      'template' => 'zg_elders_rename',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elders_enable_alpha' => [
      'template' => 'templates/zg_elders_enable_alpha',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elders_set_password' => [
      'template' => 'zg_elders_set_password',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elders_ask_purchase' => [
      'template' => 'zg_elders_ask_purchase',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elders_do_purchase' => [
      'template' => 'zg_elders_do_purchase',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_check_receipt' => [
      'template' => 'zg_elders_check_receipt',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_google_checkout' => [
      'template' => 'zg_google_checkout',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_welcome' => [
      'template' => 'templates/zg_welcome',
      'arguments' => [
        'phone_id' => NULL
      ]
    ],

    'zg_choose_party' => [
      'template' => 'templates/zg_choose_party',
      'arguments' => [
        'phone_id' => NULL,
        'party_id' => NULL,
      ]
    ],

    'zg_help' => [
      'template' => 'zg_help',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elections_list' => [
      'template' => 'zg_elections_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elections_challenge' => [
      'template' => 'zg_elections_challenge',
      'arguments' => [
        'phone_id' => NULL,
        'position_id' => NULL,
      ]
    ],

    'zg_debates_list' => [
      'template' => 'zg_debates_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_debates_challenge' => [
      'template' => 'zg_debates_challenge',
      'arguments' => [
        'phone_id' => NULL,
        'position_id' => NULL,
      ]
    ],

    'zg_top20_list' => [
      'template' => 'zg_top20_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_top_aldermen_list' => [
      'template' => 'zg_top_aldermen_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_top_event_points' => [
      'template' => 'zg_top_event_points',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_choose_name' => [
      'template' => 'zg_choose_name',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_enter_referral_code' => [
      'template' => 'zg_enter_referral_code',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_land_list' => [
      'template' => 'templates/zg_land_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_land_buy' => [
      'template' => 'zg_land_buy',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_land_sell' => [
      'template' => 'zg_land_sell',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_land_work' => [
      'template' => 'zg_land_work',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_staff_list' => [
      'template' => 'templates/zg_staff_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_staff_hire' => [
      'template' => 'zg_staff_hire',
      'arguments' => [
        'phone_id' => NULL,
        'staff_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_staff_fire' => [
      'template' => 'zg_staff_fire',
      'arguments' => [
        'phone_id' => NULL,
        'staff_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_actions_list' => [
      'template' => 'zg_actions_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_actions_do' => [
      'template' => 'zg_actions_do',
      'arguments' => [
        'phone_id' => NULL,
        'action_id' => NULL,
      ]
    ],

    'zg_equipment_list' => [
      'template' => 'templates/zg_equipment_list',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_equipment_buy' => [
      'template' => 'zg_equipment_buy',
      'arguments' => [
        'phone_id' => NULL,
        'equipment_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_equipment_sell' => [
      'template' => 'zg_equipment_sell',
      'arguments' => [
        'phone_id' => NULL,
        'equipment_id' => NULL,
        'quantity' => NULL,
      ]
    ],

    'zg_move' => [
      'template' => 'zg_move',
      'arguments' => [
        'phone_id' => NULL,
        'neighborhood_id' => NULL,
      ]
    ],

    'zg_move_do' => [
      'template' => 'templates/zg_move_do',
      'arguments' => [
        'phone_id' => NULL,
        'neighborhood_id' => NULL,
      ]
    ],

    'zg_authenticate' => [
      'template' => 'zg_authenticate',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_bounce' => [
      'template' => 'templates/zg_bounce',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_bounce_040' => [
      'template' => 'zg_bounce_040',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_elders_preferences' => [
      'template' => 'zg_elders_preferences',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_zombie_move' => [
      'template' => 'zg_zombie_move',
      'arguments' => [
        'phone_id' => NULL,
        'zombie_id' => NULL,
        'hood_id' => NULL,
      ]
    ],

    'zg_event_info' => [
      'template' => 'zg_event_info',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_event_prizes' => [
      'template' => 'zg_event_prizes',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_changelog' => [
      'template' => 'templates/zg_changelog',
      'arguments' => [
        'phone_id' => NULL,
      ]
    ],

    'zg_error' => [
      'template' => 'zg_error',
      'arguments' => [
        'phone_id' => NULL,
        'error_code' => NULL,
      ]
    ],

  ];

}

