<?php

/**
 * @file zg_clan_list_available.tpl.php
 * Game available clan list page
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: done
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: yes
 * .
 */

/* ------ CONTROLLER ------ */

global $game, $phone_id;
$pl = zg_fetch_player();

if (empty($pl->username) || $pl->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

$sql = 'SELECT party_title from `values` where id = %d;';

$result = db_query($sql, $pl->fkey_values_id);
$values = db_fetch_object($result);

$clans = [];
$sql = 'SELECT count( clan_members.id ) AS members, clans.name, clans.acronym,
  clans.rules
  FROM clan_members
  LEFT JOIN clans ON clan_members.fkey_clans_id = clans.id
  LEFT JOIN users ON clan_members.fkey_users_id = users.id
  WHERE users.fkey_values_id = %d
  GROUP BY clans.id
  ORDER BY members DESC';

$result = db_query($sql, $pl->fkey_values_id);
while ($clan = db_fetch_object($result)) {
  $clans[] = $clan;
}

if (count($clans)) {
  $html['prefix'] =<<< EOF
<div class="title">Available $values->party_title Clans</div>
<div class="subtitle">To join a clan, go to the <em>Actions</em> screen and choose
<em>Join a clan</em>.&nbsp; You will need to know the clan's three letter
acronym.</div>
<div class="clan-list">
EOF;

  foreach ($clans as $clan) {
    firep($clan);

    if (empty($clan->rules)) {
      $clan->rules = 'No rules';
    }

    $html['clan_' . $clan->acronym] = <<< EOF
<h4>{$clan->name} ({$clan->acronym}): $clan->members members</h4>
Rules: $clan->rules
EOF;

  }
  $html['suffix'] = '</div>';
}
else {
  $html['no_clans'] =<<< EOF
<h4 class="title">No clans available</h4>
<p class="subtitle">There aren't any clans!&nbsp; Why don't you create one?</p>
EOF;
}

/* ------ VIEW ------ */

zg_fetch_header($pl);
db_set_active();

foreach ($html as $clan) {
  print $clan;
}
