<?php

/**
 * @file stlouis_clan_list_available.tpl.php
 * Stlouis available clan list page
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

  global $game, $phone_id;

  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $game_user = zg_fetch_player();

  if (empty($game_user->username) || $game_user->username == '(new player)') {
    db_set_active();
    drupal_goto($game . '/choose_name/' . $arg2);
  }

  zg_fetch_header($game_user);

  $sql = 'SELECT clan_title from `values`
    where id = %d;';

  $result = db_query($sql, $game_user->fkey_values_id);
  $values = db_fetch_object($result);

  echo <<< EOF
<div class="title">Available $values->clan_title Clans</div>
<div class="subtitle">To join a clan, go to the <em>Actions</em> screen and choose
  <em>Join a clan</em>.&nbsp; You will need to know the clan's three letter
  acronym.</div>
<div class="clan-list">
EOF;

  $data = [];
  $sql = 'SELECT count( clan_members.id ) AS members, clans.name, clans.acronym,
    clans.rules
    FROM clan_members
    LEFT JOIN clans ON clan_members.fkey_clans_id = clans.id
    LEFT JOIN users ON clan_members.fkey_users_id = users.id
    WHERE users.fkey_values_id = %d
    GROUP BY clans.id
    ORDER BY members DESC';

  $result = db_query($sql, $game_user->fkey_values_id);
  while ($item = db_fetch_object($result)) $data[] = $item;

  foreach ($data as $item) {
firep($item);

    if (empty($item->rules))
      $item->rules = 'No rules';

		echo <<< EOF
<h4>{$item->name} ({$item->acronym}): $item->members members</h4>
Rules: $item->rules
EOF;

  }

  echo '</div>';

  db_set_active();
