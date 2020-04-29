<?php

/**
 * @file
 * Clan page helper include file.
 *
 * Synced with CG: N/A
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: done
 * Controller moved to callback include: done
 * View only in theme template: N/A
 * All db queries in controller: yes
 * Minimal function calls in view: yes
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

/**
 * Clan page callback.
 *
 * This landscape-only page replaces all of the following pages:
 * - clan_announcements
 * - clan_list
 * - clan_list_available
 * - clan_msg
 * .
 *
 * @param string $phone_id
 *   The phone ID of the player.
 * @param string $clan_id
 *   The clan ID to show.
 *
 * @return mixed|string|string[]|void|null
 *   The clan page HTML, or access denied if the player has no access.
 */
function zg_clan_callback($phone_id, $clan_id) {

  if (empty($phone_id) || !is_numeric($clan_id)) {
    return drupal_access_denied();
  }

  global $game, $phone_id;
  $module_path = drupal_get_path('module', 'zg');
  $game = check_plain(arg(0));
  include $module_path . '/includes/' . $game . '_defs.inc';
  $game_user = zg_fetch_user();

  // FIXME: fetch clan data here.
  $clan_data = [
    'messages' => [
      'title' => t('@clan clan messages', ['@clan' => $game_user->clan_name]),
      'button' => zg_render_button('clan_msg', 'old page', '/90'),
    ],
    'announcements' => [
      'title' => t('@clan clan announcements', ['@clan' => $game_user->clan_name]),
      'button' => zg_render_button('clan_announcements', 'old page', '/90'),
    ],
    'other_clans' => [
      'title' => t('other clans', ['@clan' => $game_user->clan_name]),
      'button' => zg_render_button('clan_list_available', 'old page'),
    ],
    'members' => [
      'title' => t('@clan clan members', ['@clan' => $game_user->clan_name]),
      'button' => zg_render_button('clan_list', 'old page', '/90'),
    ],
  ];

  db_set_active();
  drupal_add_js($module_path . '/js/zg_clan.js', 'module',
    'footer');
  return theme('zg_clan', $game_user, $clan_data);
}
