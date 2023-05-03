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
  $game_user = zg_fetch_player();

  // Save the message, if any.
  $message_orig = check_plain($_GET['message']);
  $message = zg_filter_profanity($message_orig);
  firep($message, 'message');

  $errors = [];

  if (strlen($message) > 0 && strlen($message) < 3) {
    $errors[] = '<div class="message-error">Your message must be at least 3
    characters long.</div>';
    $message = '';
  }

  if (substr($message, 0, 3) == 'XXX') {
    $errors[] = '<div class="message-error">Your message contains words that are not
    allowed.&nbsp; Please rephrase.&nbsp; ' . $message . '</div>';
    $message = '';
  }

  if (strlen($message)) {
    $sql = 'insert into clan_messages (fkey_users_from_id,
    fkey_neighborhoods_id, message) values (%d, %d, "%s");';
    db_query($sql, $game_user->id, $clan_id, $message);
    $message_orig = '';
  }

  // Fetch clan data here.
  $clan_data = [];

  if ($clan_id > 0) {
    $clan_data['messages'] = [
      'title' => t('@clan (@tla) clan messages',
        ['@clan' => $game_user->clan_name, '@tla' => $game_user->clan_acronym]),
//      'button' => zg_render_button('clan_msg', 'old page',
//        '/' . $game_user->fkey_clans_id),
      'items' => zg_get_clan_messages($game_user, $clan_id, $game, $arg2, $message_orig),
      'errors' => $errors,
    ];
    $clan_data['announcements'] = [
      'title' => t('@clan (@tla) clan announcements',
        ['@clan' => $game_user->clan_name, '@tla' => $game_user->clan_acronym]),
//      'button' => zg_render_button('clan_announcements', 'old page',
//        '/' . $game_user->fkey_clans_id),
      'items' => zg_get_clan_announcements($game_user, $clan_id, $game, $arg2, $message_orig),
      'errors' => $errors,
    ];
  }

  // Add list of all clans in the party if player is in a party.
  if (TRUE || $game_user->fkey_values_id) {
    $clan_data['all_clans'] = [
      'title' => t('all @party clans', ['@party' => $game_user->party_title]),
      //      'button' => zg_render_button('clan_list_available', 'old page'),
      'items' => zg_get_all_clans($game_user->fkey_values_id),
      'errors' => $errors,
    ];
  }

  // In a clan?  Show the member list.
  if ($clan_id > 0) {
    $clan_data['members'] = [
      'title' => t('@clan (@tla) clan members',
        ['@clan' => $game_user->clan_name, '@tla' => $game_user->clan_acronym]),
      'items' => zg_get_clan_members($clan_id),
      'errors' => $errors,
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

/**
 * Fetch and render a list of all members of a specific clan.
 *
 * @param int $clan_id
 *   The clan id.
 *
 * @return string[]
 *   The rendered list of clan members, if any.
 */
function zg_get_clan_members($clan_id) {
  $member_ids = $rendered_users = [];

  $sql = 'SELECT users.id from users
    inner join clan_members on users.id = clan_members.fkey_users_id
    where clan_members.fkey_clans_id = %d;';
  $result = db_query($sql, $clan_id);

  while ($item = db_fetch_object($result)) {
    $member_ids[] = (int) $item->id;
  }
  firep($member_ids, 'clan member ids');

  $users = zg_fetch_players_by_ids($member_ids);

  foreach ($users as $user) {
    firep($user, 'clan member');
    $rendered_users[] = '<li class="overlay-list-item">' . zg_render_player($user, 'clan') . '</li>';
  }

  if (count($rendered_users)) {
    $rendered_users[] = '</ul>';
    array_unshift($rendered_users, '<ul>');
  }

  return $rendered_users;
}

/**
 * Fetch and render a list of messages in a specific clan.
 *
 * @param object $game_user
 *   The game user object.
 * @param int $clan_id
 *   The clan id.
 * @param string $game
 *   The machine name of the game.
 * @param string $arg2
 *   Arg2.
 * @param string $message_orig
 *   The original message, if any.
 *
 * @return string[]
 *   The rendered list of clan messages, if any.
 */
function zg_get_clan_messages($game_user, $clan_id, $game, $arg2, $message_orig) {
  $rendered_messages = [
    '<div class="send-message">
      <form method=get action="/' . $game . '/clan/' . $arg2 . '/' . $clan_id. '">
        <textarea class="message-textarea" name="message" rows="2">' . $message_orig . '</textarea>
        <div class="send-message-send-wrapper">
          <input class="send-message-send" type="submit" value="Send"/>
        </div>
      </form>
    </div>',
    '<ul>',
  ];

  // FIXME: move SQL to a function.
  $sql = 'select clan_messages.*, users.username, users.phone_id,
    users.level,
    elected_positions.name as ep_name,
    clan_members.is_clan_leader,
    UPPER(clans.acronym) as clan_acronym, clans.name as clan_name,
    0 AS private, clan_messages.id as msg_id,
    clans.rules as clan_rules,
    "clan" as type,
    "" as subtype,
    neighborhoods.name as location

    from clan_messages 
    
    left join users on clan_messages.fkey_users_from_id = users.id
    
    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id
    
    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id
    
    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
      clan_messages.fkey_users_from_id
    
    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
    
    LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
    
    where clan_messages.fkey_neighborhoods_id = %d
--      AND clan_messages.is_announcement = 0
    order by id DESC
    LIMIT 50;';

  $result = db_query($sql, $clan_id);
  $data = [];
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  zg_format_messages($game_user, $game_user->id, $data);

  foreach ($data as $item) {

    // FIXME: render messages in a function.
    $rendered_messages[] = <<< EOF
    <li class="overlay-list-item">
      <div class="news-item $item->type {$item->display->item_css}" id="{$item->display->msg_id}">
        <div class="dateline">
          {$item->display->timestamp} {$item->display->username} {$item->display->private_text}
        </div>
        <div class="message-body {$item->display->private_css}">
          {$item->display->delete}
          <p>{$item->display->message}</p>{$item->display->reply}
        </div>
      </div>
    </li>
EOF;

  }

  $rendered_messages[] = '</ul>';
  return $rendered_messages;
}

/**
 * Fetch and render a list of announcements in a specific clan.
 *
 * @param object $game_user
 *   The game user object.
 * @param int $clan_id
 *   The clan id.
 * @param string $game
 *   The machine name of the game.
 * @param string $arg2
 *   Arg2.
 * @param string $message_orig
 *   The original message, if any.
 *
 * @return string[]
 *   The rendered list of clan announcements, if any.
 */
function zg_get_clan_announcements($game_user, $clan_id, $game, $arg2, $message_orig) {
  $rendered_messages = [
    '<div class="send-message">
      <form method=get action="/' . $game . '/clan/' . $arg2 . '/' . $clan_id. '">
        <textarea class="message-textarea" name="message" rows="2">' . $message_orig . '</textarea>
        <div class="send-message-send-wrapper">
          <input class="send-message-send" type="submit" value="Send"/>
        </div>
      </form>
    </div>',
    '<ul>',
  ];

  // FIXME: move SQL to a function.
  $sql = 'select clan_messages.*, users.username, users.phone_id,
    users.level,
    elected_positions.name as ep_name,
    clan_members.is_clan_leader,
    UPPER(clans.acronym) as clan_acronym, clans.name as clan_name,
    0 AS private, clan_messages.id as msg_id,
    clans.rules as clan_rules,
    "clan" as type,
    "" as subtype,
    neighborhoods.name as location

    from clan_messages 
    
    left join users on clan_messages.fkey_users_from_id = users.id
    
    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id
    
    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id
    
    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id =
      clan_messages.fkey_users_from_id
    
    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id
    
    LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id
    
    where clan_messages.fkey_neighborhoods_id = %d
      AND clan_messages.is_announcement = 1
    order by id DESC
    LIMIT 50;';

  $result = db_query($sql, $clan_id);
  $data = [];
  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  zg_format_messages($game_user, $game_user->id, $data);

  foreach ($data as $item) {

    // FIXME: render messages in a function.
    $rendered_messages[] = <<< EOF
    <li class="overlay-list-item">
      <div class="news-item $item->type {$item->display->item_css}" id="{$item->display->msg_id}">
        <div class="dateline">
          {$item->display->timestamp} {$item->display->username} {$item->display->private_text}
        </div>
        <div class="message-body {$item->display->private_css}">
          {$item->display->delete}
          <p>{$item->display->message}</p>{$item->display->reply}
        </div>
      </div>
    </li>
EOF;

  }

  $rendered_messages[] = '</ul>';
  return $rendered_messages;
}
