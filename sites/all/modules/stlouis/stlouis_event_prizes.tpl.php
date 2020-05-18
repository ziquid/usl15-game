<?php

/**
 * @file stlouis_event_prizes.tpl.php
 * stlouis event prizes page
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
  $game_user = $fetch_user();
  $fetch_header($game_user);
  if ($game_user->meta == 'admin') {
    db_set_active();
    return;
  }

  echo <<< EOF
<div class="title">
  Event Prizes
</div>
EOF;

  $prizes = [
    [100, 1, 37],
    [80, 1, 37],
    [60, 3, 37],
    [50, 5, 37],
    [40, 10, 37],
    [30, 30, 37],
    [25, 1, 38],
    [20, 1, 38],
    [15, 3, 38],
    [10, 5, 38],
    [5, 10, 38],
    [4, 10, 38],
    [3, 10, 38],
    [2, 10, 38],
    [1, 50, 38],
  ];

  foreach ($prizes as $prize) {
// top $top get $quantity gifts #$prize_id

    $top = $prize[0];
    $quantity = $prize[1];
    $prize_id = $prize[2];

    echo '<div class="title">Top ' . $top . ' players get ' . $quantity .
      ' of equipment #' . $prize_id . '</div>';

    mail('joseph@cheek.com', 'event prizes',
      'Top ' . $top . ' players get ' . $quantity .
      ' of equipment #' . $prize_id . '.');

    $sql = 'select fkey_users_id as id, users.username from event_points
      left join users on fkey_users_id = users.id
      order by points DESC

      limit %d;';
    $result = db_query($sql, $top);
    $data = [];
    while ($item = db_fetch_object($result)) $data[] = $item;

    foreach ($data as $user) {

      // Does user have any of this present?
      $sql = 'select quantity from equipment_ownership
        where fkey_users_id = %d
        and fkey_equipment_id = %d;';
      $result = db_query($sql, $user->id, $prize_id);
      $equip_quantity = db_fetch_object($result);

      // Create record.
      if (empty($equip_quantity)) {

        $sql = 'insert into equipment_ownership
          (fkey_users_id, fkey_equipment_id, quantity)
          values
          (%d, %d, %d);';

        $result = db_query($sql, $user->id, $prize_id, $quantity);

echo '<div class="subsubtitle">creating record for ' . $user->username .
  '</div>';

      }
      else {

        // Update record.
        $sql = 'update equipment_ownership
          set quantity = quantity + %d
          where fkey_users_id = %d
          and fkey_equipment_id = %d;';

        $result = db_query($sql, $quantity, $user->id, $prize_id);

echo '<div class="subsubtitle">updating record for ' . $user->username .
  '</div>';


      }

    }

  }

  db_set_active();
