<?php

/**
 * @file menu.inc
 * Menu functions for Ziquid games.
 *
 * Synced with CG: no
 * Synced with 2114: no
 */

function stlouis_menu() {

  $items = [];

  $items['stlouis/clan_announcements/%/%'] = [
    'title' => 'St. Louis Clan Announcements Page',
    'page callback' => 'stlouis_clan_announcements_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/block_user_toggle/%/%'] = [
    'title' => 'St. Louis Block User Toggle',
    'page callback' => 'stlouis_block_user_toggle_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/msg_delete/%/%'] = [
    'title' => 'St. Louis Message Delete Page',
    'page callback' => 'stlouis_msg_delete_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/party_msg/%'] = [
    'title' => 'St. Louis Party Message Page',
    'page callback' => 'stlouis_party_msg_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/quests/%'] = [
    'title' => 'St. Louis Quests List',
    'page callback' => 'stlouis_quests_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_ask_reset_skills/%'] = [
    'title' => 'St. Louis Elders Ask Reset Skills',
    'page callback' => 'stlouis_elders_ask_reset_skills_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_do_reset_skills/%'] = [
    'title' => 'St. Louis Elders Do Reset Skills',
    'page callback' => 'stlouis_elders_do_reset_skills_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_ask_purchase/%'] = [
    'title' => 'St. Louis Elders Ask Purchase',
    'page callback' => 'stlouis_elders_ask_purchase_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_do_purchase/%'] = [
    'title' => 'St. Louis Elders Do Purchase',
    'page callback' => 'stlouis_elders_do_purchase_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_check_receipt/%'] = [
    'title' => 'St. Louis Elders Check Receipt',
    'page callback' => 'stlouis_elders_check_receipt_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/google_checkout/%'] = [
    'title' => 'St. Louis Google Checkout',
    'page callback' => 'stlouis_google_checkout_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_set_password/%'] = [
    'title' => 'St. Louis Elders Set Password',
    'page callback' => 'stlouis_elders_set_password_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_rename/%'] = [
    'title' => 'St. Louis Elders Rename',
    'page callback' => 'stlouis_elders_rename_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/help/%'] = [
    'title' => 'St. Louis Help',
    'page callback' => 'stlouis_help_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/debates/%'] = [
    'title' => 'St. Louis Debates List',
    'page callback' => 'stlouis_debates_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/debates_challenge/%/%'] = [
    'title' => 'St. Louis Debates Challenge',
    'page callback' => 'stlouis_debates_challenge_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elections/%'] = [
    'title' => 'St. Louis Elections List',
    'page callback' => 'stlouis_elections_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elections_challenge/%/%'] = [
    'title' => 'St. Louis Elections Challenge',
    'page callback' => 'stlouis_elections_challenge_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/top20/%'] = [
    'title' => 'St. Louis Top 20 List',
    'page callback' => 'stlouis_top20_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/top_aldermen/%'] = [
    'title' => 'St. Louis Top Aldermen List',
    'page callback' => 'stlouis_top_aldermen_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/top_event_points/%'] = [
    'title' => 'St. Louis Top 100 Event Points',
    'page callback' => 'stlouis_top_event_points_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/enter_referral_code/%'] = [
    'title' => 'St. Louis Enter Referral Code',
    'page callback' => 'stlouis_enter_referral_code_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/land_sell/%/%/%'] = [
    'title' => 'St. Louis Land Sell',
    'page callback' => 'stlouis_land_sell_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/land_work/%/%/%'] = [
    'title' => 'St. Louis Land Work',
    'page callback' => 'stlouis_land_work_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/staff_hire/%/%/%'] = [
    'title' => 'St. Louis Staff Hire',
    'page callback' => 'stlouis_staff_hire_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/staff_fire/%/%/%'] = [
    'title' => 'St. Louis Staff Fire',
    'page callback' => 'stlouis_staff_fire_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/actions/%'] = [
    'title' => 'St. Louis Actions',
    'page callback' => 'stlouis_actions_list_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/actions_do/%/%'] = [
    'title' => 'St. Louis Actions Do',
    'page callback' => 'stlouis_actions_do_callback',
    'page arguments' => [2, 3],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/equipment_buy/%/%/%'] = [
    'title' => 'St. Louis Equipment Buy',
    'page callback' => 'stlouis_equipment_buy_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/equipment_sell/%/%/%'] = [
    'title' => 'St. Louis Equipment Sell',
    'page callback' => 'stlouis_equipment_sell_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/bounce_040/%'] = [
    'title' => 'St. Louis bounce',
    'page callback' => 'stlouis_bounce_040_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/elders_preferences/%'] = [
    'title' => 'St. Louis Preferences Page',
    'page callback' => 'stlouis_elders_preference_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/zombie_move/%/%/%'] = [
    'title' => 'St. Louis Zombie Move Page',
    'page callback' => 'stlouis_zombie_move_callback',
    'page arguments' => [2, 3, 4],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/event_info/%'] = [
    'title' => 'St. Louis Event Info Page',
    'page callback' => 'stlouis_event_info_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  $items['stlouis/event_prizes/%'] = [
    'title' => 'St. Louis Event Prizes Page',
    'page callback' => 'stlouis_event_prizes_callback',
    'page arguments' => [2],
    'access arguments' => ['access stlouis content'],
    'type' => MENU_CALLBACK,
  ];

  return $items;
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
function stlouis_clan_list_available_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_clan_list_available', $phone_id);

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
 *   The clan announcements page HTML, or access denied if the player has no access.
 */
function stlouis_clan_announcements_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_clan_announcements', $phone_id, $clan_id);

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
 *   The block user toggle page HTML, or access denied if the player has no access.
 */
function stlouis_block_user_toggle_callback($phone_id, $phone_id_to_block) {

  if (empty($phone_id) || empty($phone_id_to_block)) {
    return drupal_access_denied();
  }

  return theme('stlouis_block_user_toggle', $phone_id,
    check_plain(filter_xss($phone_id_to_block, [])));

}

/**
 * Message delete callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $msg_id
 *   The message ID to delete.
 *
 * @return mixed|string|string[]|void|null
 *   The message delete page HTML, or access denied if the player has no access.
 */
function stlouis_msg_delete_callback($phone_id, $msg_id) {

  if (empty($phone_id) || !is_numeric($msg_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_msg_delete', $phone_id, $msg_id);
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
function stlouis_party_msg_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_party_msg', $phone_id);
}

/**
 * Quests list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The quests list page HTML, or access denied if the player has no access.
 */
function stlouis_quests_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_quests_list', $phone_id);
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
function stlouis_elders_ask_reset_skills_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_ask_reset_skills', $phone_id);
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
function stlouis_elders_do_reset_skills_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_do_reset_skills', $phone_id);
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
function stlouis_elders_rename_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_rename', $phone_id);
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
function stlouis_elders_set_password_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_set_password', $phone_id);
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
function stlouis_elders_ask_purchase_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_ask_purchase', $phone_id);
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
function stlouis_elders_do_purchase_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_do_purchase', $phone_id);
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
function stlouis_elders_check_receipt_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_check_receipt', $phone_id);

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
function stlouis_google_checkout_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_google_checkout', $phone_id);

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
function stlouis_help_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_help', $phone_id);

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
function stlouis_debates_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_debates_list', $phone_id);

}

/**
 * Debates challenge callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $position_id
 *
 *
 * @return mixed|string|string[]|void|null
 *   The debates challenge page HTML, or access denied if the player has no access.
 */
function stlouis_debates_challenge_callback($phone_id, $position_id) {

  if (empty($phone_id) || !is_numeric($position_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_debates_challenge', $phone_id, $position_id);

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
function stlouis_elections_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elections_list', $phone_id);

}

/**
 * Elections challenge callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $position_id
 *
 *
 * @return mixed|string|string[]|void|null
 *   The elections challenge page HTML, or access denied if the player has no access.
 */
function stlouis_elections_challenge_callback($phone_id, $position_id) {

  if (empty($phone_id) || !is_numeric($position_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elections_challenge', $phone_id, $position_id);

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
function stlouis_top20_list_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('stlouis_top20_list', $phone_id);
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
function stlouis_top_aldermen_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_top_aldermen_list', $phone_id);

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
function stlouis_top_event_points_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_top_event_points', $phone_id);

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
function stlouis_enter_referral_code_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_enter_referral_code', $phone_id);

}

/**
 * Land sell callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $land_id
 *   The land ID to buy.
 * @param string $quantity
 *   The quantity to buy.
 *
 * @return mixed|string|string[]|void|null
 *   The land sell page HTML, or access denied if the player has no access.
 */
function stlouis_land_sell_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('stlouis_land_sell', $phone_id, $land_id, $quantity);
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
function stlouis_land_work_callback($phone_id, $land_id, $quantity) {

  if (empty($phone_id) || !is_numeric($land_id) || !(is_numeric($quantity) ||
      ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('stlouis_land_work', $phone_id, $land_id, $quantity);
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
function stlouis_staff_hire_callback($phone_id, $staff_id, $quantity) {

  if (empty($phone_id) || !is_numeric($staff_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('stlouis_staff_hire', $phone_id, $staff_id, $quantity);
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
function stlouis_staff_fire_callback($phone_id, $staff_id, $quantity) {

  if (empty($phone_id) || !is_numeric($staff_id) || !(is_numeric($quantity) ||
    ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('stlouis_staff_fire', $phone_id, $staff_id, $quantity);
}

/**
 * Actions list callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The actions list page HTML, or access denied if the player has no
 *   access.
 */
function stlouis_actions_list_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_actions_list', $phone_id);
}

/**
 * Actions do callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $action_id
 *   The action Id to do.
 *
 * @return mixed|string|string[]|void|null
 *   The actions do page HTML, or access denied if the player has no access.
 */
function stlouis_actions_do_callback($phone_id, $action_id) {

  if (empty($phone_id) || !is_numeric($action_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_actions_do', $phone_id, $action_id);
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
function stlouis_equipment_buy_callback($phone_id, $equipment_id, $quantity) {

  if (empty($phone_id) || !is_numeric($equipment_id) ||
    !(is_numeric($quantity) || ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('stlouis_equipment_buy', $phone_id, $equipment_id, $quantity);
}

/**
 * Equipment sell callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $equipment_id
 *   The equipment ID to buy.
 * @param string $quantity
 *   The quantity to buy.
 *
 * @return mixed|string|string[]|void|null
 *   The equipment sell page HTML, or access denied if the player has no access.
 */
function stlouis_equipment_sell_callback($phone_id, $equipment_id, $quantity) {

  if (empty($phone_id) || !is_numeric($equipment_id) ||
    !(is_numeric($quantity) || ($quantity === 'use-quantity'))) {
    return drupal_access_denied();
  }

  return theme('stlouis_equipment_sell', $phone_id, $equipment_id, $quantity);
}

/**
 * Bounce 040 callback function.
 *
 * @param string $phone_id
 *   The phone ID of the player.
 *
 * @return mixed|string|string[]|void|null
 *   The bounce 040 page HTML, or access denied if the player has no
 *   access.
 */
function stlouis_bounce_040_callback($phone_id) {
  if (empty($phone_id)) {
    return drupal_access_denied();
  }
  return theme('stlouis_bounce_040', $phone_id);
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
function stlouis_elders_preference_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_elders_preferences', $phone_id);

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
function stlouis_zombie_move_callback($phone_id, $zombie_id, $hood_id) {

  if (empty($phone_id) || is_nan($zombie_id) || is_nan($hood_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_zombie_move', $phone_id, $zombie_id, $hood_id);
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
function stlouis_event_info_callback($phone_id) {

  if (empty($phone_id)) {
    return drupal_access_denied();
  }

  return theme('stlouis_event_info', $phone_id);
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
function stlouis_event_prizes_callback($phone_id) {

  global $user;

  if (($user->roles[4] != 'web game access') ||
    ($phone_id != 'abc123')) {
    return drupal_access_denied();
  }

  return theme('stlouis_event_prizes', $phone_id);
}

/**
 * Implements hook_theme().
 */
function stlouis_theme() {

  // Theme "A" uses template "B" with args "C".
  return [

    'stlouis_clan_list_available' => [
      'template' => 'stlouis_clan_list_available',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_clan_announcements' => [
      'template' => 'stlouis_clan_announcements',
      'arguments' => [
        'phone_id' => NULL,
        'clan_id' => NULL,
      ],
    ],

    'stlouis_block_user_toggle' => [
      'template' => 'stlouis_block_user_toggle',
      'arguments' => [
        'phone_id' => NULL,
        'phone_id_to_block' => NULL,
      ],
    ],

    'stlouis_msg_delete' => [
      'template' => 'stlouis_msg_delete',
      'arguments' => [
        'phone_id' => NULL,
        'msg_id' => NULL,
      ],
    ],

    'stlouis_party_msg' => [
      'template' => 'stlouis_party_msg',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_quests_list' => [
      'template' => 'stlouis_quests_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_ask_reset_skills' => [
      'template' => 'stlouis_elders_ask_reset_skills',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_do_reset_skills' => [
      'template' => 'stlouis_elders_do_reset_skills',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_rename' => [
      'template' => 'stlouis_elders_rename',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_set_password' => [
      'template' => 'stlouis_elders_set_password',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_ask_purchase' => [
      'template' => 'stlouis_elders_ask_purchase',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_do_purchase' => [
      'template' => 'stlouis_elders_do_purchase',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_check_receipt' => [
      'template' => 'stlouis_elders_check_receipt',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_google_checkout' => [
      'template' => 'stlouis_google_checkout',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_help' => [
      'template' => 'stlouis_help',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elections_list' => [
      'template' => 'stlouis_elections_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elections_challenge' => [
      'template' => 'stlouis_elections_challenge',
      'arguments' => [
        'phone_id' => NULL,
        'position_id' => NULL,
      ],
    ],

    'stlouis_debates_list' => [
      'template' => 'stlouis_debates_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_debates_challenge' => [
      'template' => 'stlouis_debates_challenge',
      'arguments' => [
        'phone_id' => NULL,
        'position_id' => NULL,
      ],
    ],

    'stlouis_top20_list' => [
      'template' => 'stlouis_top20_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_top_aldermen_list' => [
      'template' => 'stlouis_top_aldermen_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_top_event_points' => [
      'template' => 'stlouis_top_event_points',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_enter_referral_code' => [
      'template' => 'stlouis_enter_referral_code',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_land_sell' => [
      'template' => 'stlouis_land_sell',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'stlouis_land_work' => [
      'template' => 'stlouis_land_work',
      'arguments' => [
        'phone_id' => NULL,
        'land_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'stlouis_staff_hire' => [
      'template' => 'stlouis_staff_hire',
      'arguments' => [
        'phone_id' => NULL,
        'staff_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'stlouis_staff_fire' => [
      'template' => 'stlouis_staff_fire',
      'arguments' => [
        'phone_id' => NULL,
        'staff_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'stlouis_actions_list' => [
      'template' => 'stlouis_actions_list',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_actions_do' => [
      'template' => 'stlouis_actions_do',
      'arguments' => [
        'phone_id' => NULL,
        'action_id' => NULL,
      ],
    ],

    'stlouis_equipment_buy' => [
      'template' => 'stlouis_equipment_buy',
      'arguments' => [
        'phone_id' => NULL,
        'equipment_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'stlouis_equipment_sell' => [
      'template' => 'stlouis_equipment_sell',
      'arguments' => [
        'phone_id' => NULL,
        'equipment_id' => NULL,
        'quantity' => NULL,
      ],
    ],

    'stlouis_bounce_040' => [
      'template' => 'stlouis_bounce_040',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_elders_preferences' => [
      'template' => 'stlouis_elders_preferences',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_zombie_move' => [
      'template' => 'stlouis_zombie_move',
      'arguments' => [
        'phone_id' => NULL,
        'zombie_id' => NULL,
        'hood_id' => NULL,
      ],
    ],

    'stlouis_event_info' => [
      'template' => 'stlouis_event_info',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

    'stlouis_event_prizes' => [
      'template' => 'stlouis_event_prizes',
      'arguments' => [
        'phone_id' => NULL,
      ],
    ],

  ];

}
