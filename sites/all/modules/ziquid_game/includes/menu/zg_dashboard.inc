<?php

/**
 * @file
 * Show dashboard stats.
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
 * Callback for dashboard stats.
 *
 * @param string $phone_id
 *   Arg2.
 * @param string $stat
 *   Which stat to show.
 */
function zg_dashboard_callback($phone_id, $stat) {

  if (empty($phone_id) || empty($stat)) {
    drupal_access_denied();
    return;
  }

  global $game, $phone_id;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $game_user = zg_fetch_user();

  switch ($stat) {
    case 'sessions':
      // Number of active sessions in last 15 minutes.
      db_set_active();
      $sql = 'select count(sid) as anon_users from sessions
        where uid = 0 and timestamp >= "%s";';
      $result = db_query($sql, time() - 900)->fetch_object();
      break;

    case 'luck':
      // Amount of outstanding luck for players >= level 25.
      $sql = 'select sum(luck) as luck from users
        where level >= 25
        and meta <> "admin" and meta <> "zombie" and meta NOT LIKE "ai%";';
      $result = db_query($sql)->fetch_object();
      break;

    case 'accounts':
      // Number of player accounts >= level 25.
      $sql = 'select count(id) as accounts from users
        where level >= 25
        and meta <> "admin" and meta <> "zombie" and meta NOT LIKE "ai%";';
      $result = db_query($sql)->fetch_object();
      break;

    case 'officials':
      // All elected officials.
      $sql = 'SELECT ep.name, u.username FROM `elected_positions` ep
        left join elected_officials eo on ep.id = eo.fkey_elected_positions_id
        left join users u on eo.fkey_users_id = u.id;';
      $result = db_query($sql)->fetch_object();
      break;

  }

  echo drupal_json($result);
  db_set_active('default');
  end;
}
