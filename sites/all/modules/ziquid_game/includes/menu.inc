<?php

/**
 * @file
 * Menu functions for Ziquid games.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * .
 */

/**
 * Implements hook_menu().
 */
function zg_menu() {

  $items = [];
  $items['usl_esa/bounce/%'] = [
    'title' => 'USL_ESA Game Bounce',
    'page callback' => 'usl_esa_bounce_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stl1904/b2/%'] = [
    'title' => 'STL1904 Game Bounce',
    'page callback' => 'stl1904_bounce_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $items['%/bounce/%'] = [
    'title' => 'Ziquid Game Bounce',
    'page callback' => 'zg_bounce_callback',
    'page arguments' => [2],
    'access arguments' => ['access game content'],
    'type' => MENU_CALLBACK,
  ];

  $game_list = zg_get_game_list();
  foreach ($game_list as $game) {

    $items[$game . '/authenticate/%'] = [
      'title' => 'Ziquid Game Authenticate',
      'page callback' => 'zg_authenticate_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/bots/%'] = [
      'title' => 'Ziquid Game Bots',
      'page callback' => 'zg_bots_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/bounce/%'] = [
      'title' => 'Ziquid Game Bounce',
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

    $items[$game . '/choose_name/%'] = [
      'title' => 'Ziquid Game Choose Name',
      'page callback' => 'zg_choose_name_callback',
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

    $items[$game . '/clan/%/%'] = [
      'title' => 'Ziquid Game Clan Page',
      'page callback' => 'zg_clan_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
      'file' => 'includes/menu/zg_clan.inc',
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

    $items[$game . '/dashboard/%/%'] = [
      'title' => 'Ziquid Game Dashboard',
      'page callback' => 'zg_dashboard_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
      'file' => 'includes/menu/zg_dashboard.inc',
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

    $items[$game . '/extra_menu_link/%'] = [
      'title' => 'Ziquid Game Extra Menu Link Page',
      'page callback' => 'zg_extra_menu_link_callback',
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

    $items[$game . '/land_buy/%/%/%'] = [
      'title' => 'Ziquid Game Land Buy',
      'page callback' => 'zg_land_buy_callback',
      'page arguments' => [2, 3, 4],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/msg_count/%/%'] = [
      'title' => 'Ziquid Game Message Count',
      'page callback' => 'zg_msg_count_callback',
      'page arguments' => [2, 3],
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

    $items[$game . '/move_do/%/%'] = [
      'title' => 'Ziquid Game Move Do',
      'page callback' => 'zg_move_do_callback',
      'page arguments' => [2, 3],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];

    $items[$game . '/privacy/%'] = [
      'title' => 'Ziquid Game Privacy Policy',
      'page callback' => 'zg_privacy_callback',
      'page arguments' => [2],
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

    $items[$game . '/welcome/%'] = [
      'title' => 'Ziquid Game Welcome',
      'page callback' => 'zg_welcome_callback',
      'page arguments' => [2],
      'access arguments' => ['access game content'],
      'type' => MENU_CALLBACK,
    ];
  }

  /*

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

  $items[$game . '/enter_referral_code/%'] = [
    'title' => 'Ziquid Game Enter Referral Code',
    'page callback' => 'zg_enter_referral_code_callback',
    'page arguments' => [2],
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

  */
  return $items;
}

/**
 * Homepage callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The homepage HTML, or access denied if the player has no access.
 */
function zg_homepage_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_homepage', $phone_id);
}

/**
 * User profile callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The user profile page HTML, or access denied if the player has no access.
 */
function zg_userprofile_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_userprofile', $phone_id);
}

/**
 * User competencies callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The user competencies page HTML, or access denied if the player has no
 *   access.
 */
function zg_user_competencies_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_user_competencies', $phone_id);
}

/**
 * Clan list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $clan_id
 *   The clan ID to check.
 *
 * @return mixed|string|string[]|void|null
 *   The clan list page HTML, or access denied if the player has no access.
 */
function zg_clan_list_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_list', $phone_id, $clan_id);
}

/**
 * Clan list available callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The clan list available page HTML, or access denied if the player has no
 *   access.
 */
function zg_clan_list_available_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_list_available', $phone_id);
}

/**
 * Clan msg callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $clan_id
 *   The clan ID to check.
 *
 * @return mixed|string|string[]|void|null
 *   The clan msg page HTML, or access denied if the player has no access.
 */
function zg_clan_msg_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_msg', $phone_id, $clan_id);
}

/**
 * Clan announcements callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $clan_id
 *   The clan ID to check.
 *
 * @return mixed|string|string[]|void|null
 *   The clan announcements page, or access denied if the player has no access.
 */
function zg_clan_announcements_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('zg_clan_announcements', $phone_id, $clan_id);
}

/**
 * Block user toggle callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $phone_id_to_block
 *   The phone ID of the player to block.
 *
 * @return mixed|string|string[]|void|null
 *   The block user toggle page, or access denied if the player has no access.
 */
function zg_block_user_toggle_callback($phone_id, $phone_id_to_block) {

  if (empty($phone_id) || empty($phone_id_to_block)) {
    return drupal_access_denied();
  }

  return theme('zg_block_user_toggle', $phone_id,
    check_plain(filter_xss($phone_id_to_block, [])));
}

/**
 * Msg delete callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $msg_id
 *   The message ID to delete.
 *
 * @return mixed|string|string[]|void|null
 *   The msg delete page HTML, or access denied if the player has no access.
 */
function zg_msg_delete_callback($phone_id, $msg_id) {

  if (empty($phone_id) || !is_numeric($msg_id)) {
    return drupal_access_denied();
  }

  return theme('zg_msg_delete', $phone_id, $msg_id);
}

/**
 * Party msg callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The party msg page HTML, or access denied if the player has no
 *   access.
 */
function zg_party_msg_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_party_msg', $phone_id);
}

/**
 * Increase skills callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $skill
 *   The skill to increase.
 *
 * @return mixed|string|string[]|void|null
 *   The increase skills page HTML, or access denied if the player has no access.
 */
function zg_increase_skills_callback($phone_id, $skill) {

  if (empty($phone_id) || empty($skill)) {
    return drupal_access_denied();
  }

  return theme('zg_increase_skills', $phone_id, $skill);
}

/**
 * Privacy policy callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The privacy policy page HTML, or access denied if the player has no
 *   access.
 */
function zg_privacy_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_privacy', $phone_id);
}

/**
 * Quest groups callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The quest groups page HTML, or access denied if the player has no
 *   access.
 */
function zg_quest_groups_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $module_path = drupal_get_path('module', 'zg');
  drupal_add_js($module_path . '/js/zg_quest_groups.js', 'module',
    'footer');
  return theme('zg_quest_groups', $phone_id);
}

/**
 * Quests do callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $quest_id
 *   The quests ID to do.
 *
 * @return mixed|string|string[]|void|null
 *   The quests do page HTML, or access denied if the player has no access.
 */
function zg_quests_do_callback($phone_id, $quest_id) {

  if (empty($phone_id) || !is_numeric($quest_id)) {
    return drupal_access_denied();
  }

  return theme('zg_quests_do', $phone_id, $quest_id);
}

/**
 * Elders callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $module_path = drupal_get_path('module', 'zg');
  drupal_add_js($module_path . '/js/zg_elders.js', 'module',
    'footer');
  return theme('zg_elders', $phone_id);
}

/**
 * Elders ask reset callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders ask reset page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_ask_reset_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_ask_reset', $phone_id);
}

/**
 * Elders ask reset skills callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders ask reset skills page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_ask_reset_skills_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_ask_reset_skills', $phone_id);
}

/**
 * Elders do reset skills callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders do reset skills page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_do_reset_skills_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_do_reset_skills', $phone_id);
}

/**
 * Elders do fill callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $fill_type
 *   The type to fill.
 *
 * @return mixed|string|string[]|void|null
 *   The elders do fill page HTML, or access denied if the player has no access.
 */
function zg_elders_do_fill_callback($phone_id, $fill_type) {

  if (empty($phone_id) || empty($fill_type)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_do_fill', $phone_id, $fill_type);
}

/**
 * Elders rename callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders rename page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_rename_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_rename', $phone_id);
}

/**
 * Elders enable alpha callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders enable alpha page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_enable_alpha_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_enable_alpha', $phone_id);
}

/**
 * Elders set password callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders set password page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_set_password_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_set_password', $phone_id);
}

/**
 * Elders ask purchase callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders ask purchase page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_ask_purchase_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_ask_purchase', $phone_id);
}

/**
 * Elders do purchase callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders do purchase page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_do_purchase_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_do_purchase', $phone_id);
}

/**
 * Elders check receipt callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders check receipt page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_check_receipt_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_check_receipt', $phone_id);
}

/**
 * Google checkout callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The google checkout page HTML, or access denied if the player has no
 *   access.
 */
function zg_google_checkout_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_google_checkout', $phone_id);
}

/**
 * Welcome callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The welcome page HTML, or access denied if the player has no
 *   access.
 */
function zg_welcome_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  // Prime the cache.
  drupal_get_path('module', 'zg');

//  $game = check_plain(arg(0));
//  db_set_active('game_' . $game);
//  $pl = zg_fetch_user_by_id(zg_get_phoneid());
//  db_set_active();
//  zg_loop($pl, 'Knight Rider', TRUE);
  return theme('zg_welcome', $phone_id);
}

/**
 * Choose party callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $party_id
 *   The party ID to choose.
 *
 * @return mixed|string|string[]|void|null
 *   The choose party page HTML, or access denied if the player has no access.
 */
function zg_choose_party_callback($phone_id, $party_id) {

  if (empty($phone_id) || !is_numeric($party_id)) {
    return drupal_access_denied();
  }

  return theme('zg_choose_party', $phone_id, $party_id);
}

/**
 * Help callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The help page HTML, or access denied if the player has no
 *   access.
 */
function zg_help_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_help', $phone_id);
}

/**
 * Debates list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The debates list page HTML, or access denied if the player has no
 *   access.
 */
function zg_debates_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_debates_list', $phone_id);
}

/**
 * Debates challenge callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $opp_id
 *   The opponent's ID.
 *
 * @return mixed|string|string[]|void|null
 *   The debates challenge page HTML, or access denied if the player has no access.
 */
function zg_debates_challenge_callback($phone_id, $opp_id) {

  if (empty($phone_id) || !is_numeric($opp_id)) {
    return drupal_access_denied();
  }

  return theme('zg_debates_challenge', $phone_id, $opp_id);
}

/**
 * Elections list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elections list page HTML, or access denied if the player has no
 *   access.
 */
function zg_elections_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elections_list', $phone_id);
}

/**
 * Elections challenge callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $position_id
 *   The position ID to challenge.
 *
 * @return mixed|string|string[]|void|null
 *   The elections challenge page HTML, or access denied if the player has no access.
 */
function zg_elections_challenge_callback($phone_id, $position_id) {

  if (empty($phone_id) || !is_numeric($position_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elections_challenge', $phone_id, $position_id);
}

/**
 * Top20 list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The top20 list page HTML, or access denied if the player has no
 *   access.
 */
function zg_top20_list_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('zg_top20_list', $phone_id);
}

/**
 * Top aldermen list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The top aldermen list page HTML, or access denied if the player has no
 *   access.
 */
function zg_top_aldermen_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_top_aldermen_list', $phone_id);
}

/**
 * Top event points callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The top event points page HTML, or access denied if the player has no
 *   access.
 */
function zg_top_event_points_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_top_event_points', $phone_id);
}

/**
 * Choose name callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The choose name page HTML, or access denied if the player has no
 *   access.
 */
function zg_choose_name_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_choose_name', $phone_id);
}

/**
 * Enter referral code callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The enter referral code page HTML, or access denied if the player has no
 *   access.
 */
function zg_enter_referral_code_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_enter_referral_code', $phone_id);
}

/**
 * Land list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The land list page HTML, or access denied if the player has no
 *   access.
 */
function zg_land_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_land_list', $phone_id);
}

/**
 * Land buy callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $land_id
 *   The land ID to buy.
 * @param string $quantity
 *   The quantity to buy.
 *
 * @return mixed|string|string[]|void|null
 *   The land buy page HTML, or access denied if the player has no access.
 */
function zg_land_buy_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_land_buy', $phone_id, $land_id, $quantity);
}

/**
 * Land sell callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $land_id
 *   The land ID to sell.
 * @param string $quantity
 *   The quantity to sell.
 *
 * @return mixed|string|string[]|void|null
 *   The land sell page HTML, or access denied if the player has no access.
 */
function zg_land_sell_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_land_sell', $phone_id, $land_id, $quantity);
}

/**
 * Land work callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $land_id
 *   The land ID to work.
 * @param string $quantity
 *   The quantity to work.
 *
 * @return mixed|string|string[]|void|null
 *   The land work page HTML, or access denied if the player has no access.
 */
function zg_land_work_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
      ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_land_work', $phone_id, $land_id, $quantity);
}

/**
 * Staff list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The staff list page HTML, or access denied if the player has no
 *   access.
 */
function zg_staff_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_staff_list', $phone_id);
}

/**
 * Staff hire callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $staff_id
 *   The staff ID to hire.
 * @param string $quantity
 *   The quantity to hire.
 *
 * @return mixed|string|string[]|void|null
 *   The staff hire page HTML, or access denied if the player has no access.
 */
function zg_staff_hire_callback($phone_id, $staff_id, $quantity) {

  if (empty($phone_id) || !is_numeric($staff_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_staff_hire', $phone_id, $staff_id, $quantity);
}

/**
 * Staff fire callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $staff_id
 *   The staff ID to fire.
 * @param string $quantity
 *   The quantity to fire.
 *
 * @return mixed|string|string[]|void|null
 *   The staff fire page HTML, or access denied if the player has no access.
 */
function zg_staff_fire_callback($phone_id, $staff_id, $quantity) {

  if (empty($phone_id) || !is_numeric($staff_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_staff_fire', $phone_id, $staff_id, $quantity);
}

/**
 * Actions list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The action list page HTML, or access denied if the player has no
 *   access.
 */
function zg_actions_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_actions_list', $phone_id);
}

/**
 * Actions do callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $action_id
 *   The action ID to do.
 *
 * @return mixed|string|string[]|void|null
 *   The actions do page HTML, or access denied if the player has no access.
 */
function zg_actions_do_callback($phone_id, $action_id) {

  if (empty($phone_id) || !is_numeric($action_id)) {
    return drupal_access_denied();
  }

  return theme('zg_actions_do', $phone_id, $action_id);
}

/**
 * Equipment list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The equipment list page HTML, or access denied if the player has no
 *   access.
 */
function zg_equipment_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_equipment_list', $phone_id);
}

/**
 * Equipment buy callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $equipment_id
 *   The equipment ID to buy.
 * @param string $quantity
 *   The quantity to buy.
 *
 * @return mixed|string|string[]|void|null
 *   The equipment buy page HTML, or access denied if the player has no access.
 */
function zg_equipment_buy_callback($phone_id, $equipment_id, $quantity) {

  if (empty($phone_id) || !is_numeric($equipment_id) ||
    !(is_numeric($quantity) || ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_equipment_buy', $phone_id, $equipment_id, $quantity);
}

/**
 * Equipment sell callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $equipment_id
 *   The equipment ID to sell.
 * @param string $quantity
 *   The quantity to sell.
 *
 * @return mixed|string|string[]|void|null
 *   The equipment sell page HTML, or access denied if the player has no access.
 */
function zg_equipment_sell_callback($phone_id, $equipment_id, $quantity) {

  if (empty($phone_id) || !is_numeric($equipment_id) ||
    !(is_numeric($quantity) || ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('zg_equipment_sell', $phone_id, $equipment_id, $quantity);
}

/**
 * User message count callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $msg_id
 *   The last message ID shown.
 */
function zg_msg_count_callback($phone_id, $msg_id) {

  if (empty($phone_id) || !is_numeric($msg_id)) {
    return drupal_access_denied();
  }

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $game_user_id = zg_get_userid_from_phoneid($phone_id);
  $count = zg_get_new_user_message_count($game_user_id, (int) $msg_id);
  db_set_active();
  echo $count;
}

/**
 * Move callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $neighborhood_id
 *   The neighborhood ID to move to.
 *
 * @return mixed|string|string[]|void|null
 *   The move page HTML, or access denied if the player has no access.
 */
function zg_move_callback($phone_id, $neighborhood_id) {

  if (empty($phone_id) || !is_numeric($neighborhood_id)) {
    return drupal_access_denied();
  }

  return theme('zg_move', $phone_id, $neighborhood_id);
}

/**
 * Move do callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $neighborhood_id
 *   The neighborhood ID to do.
 *
 * @return mixed|string|string[]|void|null
 *   The move do page HTML, or access denied if the player has no access.
 */
function zg_move_do_callback($phone_id, $neighborhood_id) {

  if (empty($phone_id) || !is_numeric($neighborhood_id)) {
    return drupal_access_denied();
  }

  return theme('zg_move_do', $phone_id, $neighborhood_id);
}

/**
 * Authenticate callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The authenticate page HTML, or access denied if the player has no
 *   access.
 */
function zg_authenticate_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_authenticate', $phone_id);
}

/**
 * Bots callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The bots page HTML, or access denied if the player has no
 *   access.
 */
function zg_bots_callback($phone_id) {
  $start = microtime(TRUE);
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $game = check_plain(arg(0));
  db_set_active('game_' . $game);
  $game_user = zg_fetch_player_by_id(zg_get_phoneid());

  if ($game_user->meta !== 'admin') {
    return drupal_access_denied();
  }

  $stats = zg_ai_show_brief_stats();
  $ai_move = zg_move_ai();

  // Refresh every 30 seconds.
  db_set_active();
  header('Refresh: 10');
  print '<pre>' . $stats . '</pre>';
  print '<pre>' . $ai_move . '</pre>';
  $end = microtime(TRUE);
  print ($end - $start);
}

/**
 * Bounce callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The bounce page HTML, or access denied if the player has no
 *   access.
 */
function zg_bounce_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $game = check_plain(arg(0));
  if (!in_array($game, zg_get_game_list())) {
    return drupal_access_denied();
  }

  db_set_active('game_' . $game);
  $game_user = zg_fetch_player_by_id(zg_get_phoneid());
  $cheat = zg_get_value($game_user, 'GoldenPlatesFTW', FALSE);
  db_set_active();

  $module_path = drupal_get_path('module', 'zg');
  drupal_add_js($module_path . '/js/zg_bounce.js', 'module',
    'footer');

  $button = zg_render_button();
  if ($game == 'stlouis' &&
    ($game_user->meta == 'admin' || $game_user->meta == 'staff' || $cheat)) {
    $button .= "<p class='center'>or</p>" .
      zg_render_button('/cg/bounce', 'Read the Book of Mormon') .
      "<p class='center'>(if you wish)</p>";
  }

  if ($game == 'cg') {
    zg_song($game_user, 'Faith teaser', TRUE);
  }

  return theme('zg_bounce', $phone_id, $button);
}

/**
 * Usl_Esa bounce callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The esa bounce page HTML, or access denied if the player has no
 *   access.
 */
function usl_esa_bounce_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $game = 'stlouis';
  db_set_active('game_' . $game);
  $game_user = zg_fetch_player_by_id(zg_get_phoneid());
  db_set_active();

  if ($game_user->meta == 'admin' || $game_user->meta == 'staff') {
    zg_song($game_user, 'Jump teaser', TRUE);
    $button = zg_render_button('/stlouis/bounce') .
      "<p class='center'>(if you dare)</p>";
  }
  else {
    $button = '';
  }
  return theme('usl_esa_bounce', $button);
}

/**
 * STL1904 bounce callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The 1904 bounce page HTML, or access denied if the player has no
 *   access.
 */
function stl1904_bounce_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return 'hello!'; // zg_bounce_callback($phone_id);
}

/**
 * Elders preference callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The elders preference page HTML, or access denied if the player has no
 *   access.
 */
function zg_elders_preference_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_elders_preferences', $phone_id);
}

/**
 * Zombie move callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $zombie_id
 *   The zombie ID to move.
 * @param string $hood_id
 *   The hood ID to move to.
 *
 * @return mixed|string|string[]|void|null
 *   The zombie move page HTML, or access denied if the player has no access.
 */
function zg_zombie_move_callback($phone_id, $zombie_id, $hood_id) {

  if (empty($phone_id) || is_nan($zombie_id) || is_nan($hood_id)) {
    return drupal_access_denied();
  }

  return theme('zg_zombie_move', $phone_id, $zombie_id, $hood_id);
}

/**
 * Event info callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The event info page HTML, or access denied if the player has no
 *   access.
 */
function zg_event_info_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_event_info', $phone_id);
}

/**
 * Event prizes callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The event prizes page HTML, or access denied if the player has no
 *   access.
 */
function zg_event_prizes_callback($phone_id) {

  global $user;

  if (($user->roles[4] != 'web game access') ||
    ($phone_id != 'abc123')) {
    return drupal_access_denied();
  }

  return theme('zg_event_prizes', $phone_id);
}

/**
 * Error callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $error_code
 *   The error to show.
 *
 * @return mixed|string|string[]|void|null
 *   The error page HTML, or access denied if the player has no access.
 */
function zg_error_callback($phone_id, $error_code) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('zg_error', $phone_id, $error_code);
}
/**
 * Extra menu link callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The error page HTML, or access denied if the player has no access.
 */
function zg_extra_menu_link_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  $game = check_plain(arg(0));
  db_set_active('game_' . $game);
  $game_user = zg_fetch_player_by_id(zg_get_phoneid());
  zg_sound($game_user, 'applause.mp3');
  db_set_active();

  return theme('zg_extra_menu_link', $phone_id);
}

/**
 * Changelog callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The changelog page HTML, or access denied if the player has no
 *   access.
 */
function zg_changelog_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('zg_changelog', $phone_id);
}

/**
 * Implements hook_theme().
 */
function zg_theme() {

  // Theme "A" uses template "B" with args "C".
  return [

    'zg_homepage' => [
      'template' => 'templates/zg_homepage',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_userprofile' => [
      'template' => 'templates/zg_userprofile',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_user_competencies' => [
      'template' => 'templates/zg_user_competencies',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_clan' => [
      'template' => 'templates/landscape/zg_clan',
      'arguments' => [
        'game_user' => NULL,
        'clan_data' => [],
      ],
    ],

    'zg_clan_list' => [
      'template' => 'templates/zg_clan_list',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ],
    ],

    'zg_clan_list_available' => [
      'template' => 'templates/zg_clan_list_available',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_clan_msg' => [
      'template' => 'templates/zg_clan_msg',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ],
    ],

    'zg_clan_announcements' => [
      'template' => 'templates/zg_clan_announcements',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ],
    ],

    'zg_block_user_toggle' => [
      'template' => 'templates/zg_block_user_toggle',
      'arguments' => [
        'phone_id' => NULL,
        'phone_id_to_block' => NULL,
      ],
    ],

    'zg_msg_delete' => [
      'template' => 'templates/zg_msg_delete',
      'arguments' => [
        'phone_id' => NULL,
        'msg_id' => NULL,
      ],
    ],

    'zg_party_msg' => [
      'template' => 'templates/zg_party_msg',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_increase_skills' => [
      'template' => 'templates/zg_increase_skills',
      'arguments' => [
        'phone_id' => NULL,
        'skill' => NULL,
      ],
    ],

    'zg_privacy' => [
      'template' => 'templates/zg_privacy',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_quest_groups' => [
      'template' => 'templates/zg_quest_groups',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_quests_do' => [
      'template' => 'templates/zg_quests_do',
      'arguments' => [
        'phone_id' => NULL,
        'quest_id' => NULL,
      ],
    ],

    'zg_elders' => [
      'template' => 'templates/zg_elders',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_ask_reset' => [
      'template' => 'templates/zg_elders_ask_reset',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_ask_reset_skills' => [
      'template' => 'templates/zg_elders_ask_reset_skills',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_do_reset_skills' => [
      'template' => 'templates/zg_elders_do_reset_skills',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_do_fill' => [
      'template' => 'templates/zg_elders_do_fill',
      'arguments' => [
        'phone_id' => NULL,
        'fill_type' => NULL,
      ],
    ],

    'zg_elders_rename' => [
      'template' => 'templates/zg_elders_rename',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_enable_alpha' => [
      'template' => 'templates/zg_elders_enable_alpha',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_set_password' => [
      'template' => 'templates/zg_elders_set_password',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_ask_purchase' => [
      'template' => 'templates/zg_elders_ask_purchase',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elders_do_purchase' => [
      'template' => 'templates/zg_elders_do_purchase',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_check_receipt' => [
      'template' => 'templates/zg_elders_check_receipt',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_google_checkout' => [
      'template' => 'templates/zg_google_checkout',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_welcome' => [
      'template' => 'templates/zg_welcome',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_choose_party' => [
      'template' => 'templates/zg_choose_party',
      'arguments' => [
        'phone_id' => NULL,
        'party_id' => NULL,
      ],
    ],

    'zg_help' => [
      'template' => 'templates/zg_help',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elections_list' => [
      'template' => 'templates/zg_elections_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_elections_challenge' => [
      'template' => 'templates/zg_elections_challenge',
      'arguments' => [
        'phone_id' => NULL,
        'position_id' => NULL,
      ],
    ],

    'zg_debates_list' => [
      'template' => 'templates/zg_debates_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_debates_challenge' => [
      'template' => 'templates/zg_debates_challenge',
      'arguments' => [
        'phone_id' => NULL,
        'opp_id' => NULL,
      ],
    ],

    'zg_top20_list' => [
      'template' => 'templates/zg_top20_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_top_aldermen_list' => [
      'template' => 'templates/zg_top_aldermen_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_top_event_points' => [
      'template' => 'templates/zg_top_event_points',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_choose_name' => [
      'template' => 'templates/zg_choose_name',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_enter_referral_code' => [
      'template' => 'templates/zg_enter_referral_code',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_land_list' => [
      'template' => 'templates/zg_land_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_land_buy' => [
      'template' => 'templates/zg_land_buy',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_land_sell' => [
      'template' => 'templates/zg_land_sell',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_land_work' => [
      'template' => 'templates/zg_land_work',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_staff_list' => [
      'template' => 'templates/zg_staff_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_staff_hire' => [
      'template' => 'templates/zg_staff_hire',
      'arguments' => [
        'phone_id' => NULL,
        'staff_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_staff_fire' => [
      'template' => 'templates/zg_staff_fire',
      'arguments' => [
        'phone_id' => NULL,
        'staff_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_actions_list' => [
      'template' => 'templates/zg_actions_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_actions_do' => [
      'template' => 'templates/zg_actions_do',
      'arguments' => [
        'phone_id' => NULL,
        'action_id' => NULL,
      ],
    ],

    'zg_equipment_list' => [
      'template' => 'templates/zg_equipment_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_equipment_buy' => [
      'template' => 'templates/zg_equipment_buy',
      'arguments' => [
        'phone_id' => NULL,
        'equipment_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_equipment_sell' => [
      'template' => 'templates/zg_equipment_sell',
      'arguments' => [
        'phone_id' => NULL,
        'equipment_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'zg_move' => [
      'template' => 'templates/zg_move',
      'arguments' => [
        'phone_id' => NULL,
        'neighborhood_id' => NULL,
      ],
    ],

    'zg_move_do' => [
      'template' => 'templates/zg_move_do',
      'arguments' => [
        'phone_id' => NULL,
        'neighborhood_id' => NULL,
      ],
    ],

    'zg_authenticate' => [
      'template' => 'templates/zg_authenticate',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_bounce' => [
      'template' => 'templates/zg_bounce',
      'arguments' => [
        'phone_id' => NULL,
        'button' => zg_render_button(),
      ],
    ],

    'usl_esa_bounce' => [
      'template' => 'templates/usl_esa_bounce',
      'arguments' => [
        'button' => '',
      ],
    ],

    'zg_elders_preferences' => [
      'template' => 'templates/zg_elders_preferences',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_zombie_move' => [
      'template' => 'templates/zg_zombie_move',
      'arguments' => [
        'phone_id' => NULL,
        'zombie_id' => NULL,
        'hood_id' => NULL,
      ],
    ],

    'zg_event_info' => [
      'template' => 'templates/zg_event_info',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_event_prizes' => [
      'template' => 'templates/zg_event_prizes',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_changelog' => [
      'template' => 'templates/zg_changelog',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'zg_error' => [
      'template' => 'templates/zg_error',
      'arguments' => [
        'phone_id' => NULL,
        'error_code' => NULL,
      ],
    ],

    'zg_extra_menu_link' => [
      'template' => 'templates/zg_extra_menu_link',
      'arguments' => [
        'phone_id' => NULL,
        'button' => zg_render_button('home', 'Go Back Home'),
      ],
    ],

  ];

}
