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

  // Fetch clan data here.
  $clan_data = [];
  if ($clan_id > 0) {
    $clan_data['messages'] = [
      'title' => t('@clan (@tla) clan messages',
        ['@clan' => $game_user->clan_name, '@tla' => $game_user->clan_acronym]),
      'button' => zg_render_button('clan_msg', 'old page', '/90'),
    ];
    $clan_data['announcements'] = [
      'title' => t('@clan (@tla) clan announcements',
        ['@clan' => $game_user->clan_name, '@tla' => $game_user->clan_acronym]),
      'button' => zg_render_button('clan_announcements', 'old page', '/90'),
    ];
  }

  // Add list of all clans in the party if player is in a party.
  if (TRUE || $game_user->fkey_values_id) {
    $clan_data['all_clans'] = [
      'title' => t('all @party clans', ['@party' => $game_user->party_title]),
      'button' => zg_render_button('clan_list_available', 'old page'),
      'items' => zg_get_all_clans($game_user->fkey_values_id),
    ];
  }

  // In a clan?  Show the member list.
  if ($clan_id > 0) {
    $clan_data['members'] = [
      'title' => t('@clan (@tla) clan members',
        ['@clan' => $game_user->clan_name, '@tla' => $game_user->clan_acronym]),
      'button' => zg_render_button('clan_list', 'old page', '/90'),
    ];
  }

  db_set_active();
  drupal_add_js($module_path . '/js/zg_clan.js', 'module',
    'footer');
  return theme('zg_clan', $game_user, $clan_data);
}

/**
 * Fetch and render a list of all clans in a specific party.
 *
 * @param int $party_id
 *   The party (values) id.
 *
 * @return string[]
 *   The rendered list of clans, if any.
 */
function zg_get_all_clans($party_id) {
  $items = [];
  $sql = 'SELECT count( clan_members.id ) AS members, clans.name, clans.acronym,
    clans.rules
    FROM clan_members
    LEFT JOIN clans ON clan_members.fkey_clans_id = clans.id
    LEFT JOIN users ON clan_members.fkey_users_id = users.id
    WHERE users.fkey_values_id = %d
    GROUP BY clans.id
    ORDER BY members DESC';

  $result = db_query($sql, $party_id);
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }

  foreach ($data as $item) {
    firep($item, 'clan');

    if (empty($item->rules)) {
      $item->rules = 'No rules';
    }

    $items[] = <<< EOF
<li class="overlay-list-item"><dt>{$item->name} ({$item->acronym}): $item->members members</dt>
<dd>Rules: $item->rules</dd></li>
EOF;
  }

  if (count($items)) {
    $items[] = '</ul>';
    array_unshift($items, '<ul>');
  }

  return $items;
}
