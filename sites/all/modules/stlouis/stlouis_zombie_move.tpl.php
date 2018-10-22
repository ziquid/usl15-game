<?php

/**
 * @file stlouis_zombie_move.tpl.php
 * Stlouis zombie move page
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 */

  global $game, $phone_id;

  include drupal_get_path('module', $game) . '/game_defs.inc';
  $game_user = $fetch_user();
  $fetch_header($game_user);

  $sql = 'select * from users
    where id = %d;';
  $result = db_query($sql, $zombie_id);
  $zombie = db_fetch_object($result);

  $sql = 'select fkey_clans_id from clan_members
    where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $clan_player = db_fetch_object($result);
  $sql = 'select fkey_clans_id from clan_members
    where fkey_users_id = %d;';
  $result = db_query($sql, $zombie_id);
  $clan_zombie = db_fetch_object($result);

  $sql = 'select name from neighborhoods where id = %d;';
  $result = db_query($sql, $hood_id);
  $item = db_fetch_object($result);
  $location = $item->name;

  // Move them!
  if (($game_user->debates_won >= ($game_user->level * 100) &&
    ($clan_player->fkey_clans_id == $clan_zombie->fkey_clans_id) &&
    ($clan_player->fkey_clans_id > 0) &&
    ($zombie->fkey_values_id == $game_user->fkey_values_id) &&
    ($zombie->meta == 'zombie'))
    || $game_user->meta == 'admin') {

      $sql = 'update users set fkey_neighborhoods_id = %d
        where id = %d;';
      $result = db_query($sql, $hood_id, $zombie_id);

      echo '<div class="land-succeeded">' . t('Success!') .
        '</div>';

      echo <<< EOF
<div class="subtitle">$zombie->username has moved to $location.</div>
<div class="title">
  <a href="/$game/home/$arg2">
    <img src="/sites/default/files/images/{$game}_continue.png"/>
  </a>
</div>
EOF;

//      mail('joseph@cheek.com', "Zombie $zombie_id has moved to $location",
//        "due to action from $game_user->username.");

  }
  else {

      echo <<< EOF
<div class="subtitle">$zombie->username cannot move to $location.</div>
<div class="subtitle">Perhaps he or she has switched allegiances already?</div>
<div class="title">
  <a href="/$game/home/$arg2">
    <img src="/sites/default/files/images/{$game}_continue.png"/>
  </a>
</div>
EOF;

//      mail('joseph@cheek.com', "Zombie $zombie_id cannot move to $location",
//        "due to action from $game_user->username.");

  }

  db_set_active('default');
