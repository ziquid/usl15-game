<?php

/**
 * @file
 * Stlouis clan messages page.
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
$game_user = zg_fetch_user();

if (empty($game_user->username) || $game_user->username == '(new player)') {
  db_set_active();
  drupal_goto($game . '/choose_name/' . $arg2);
}

if ($game_user->fkey_clans_id != $clan_id) {
  // FIXME: debit karma.
  db_set_active();
  drupal_goto($game . '/home/' . $arg2);
}

zg_fetch_header($game_user);

// Save the message, if any.
$message_orig = check_plain($_GET['message']);
$message = zg_filter_profanity($message_orig);
firep($message, 'message');

if (strlen($message) > 0 && strlen($message) < 3) {
  echo '<div class="message-error">Your message must be at least 3
    characters long.</div>';
  $message = '';
}

if (substr($message, 0, 3) == 'XXX') {
  echo '<div class="message-error">Your message contains words that are not
    allowed.&nbsp; Please rephrase.&nbsp; ' . $message . '</div>';
  $message = '';
}

if (!empty($message)) {
  $sql = 'insert into clan_messages (fkey_users_from_id,
    fkey_neighborhoods_id, message) values (%d, %d, "%s");';
  $result = db_query($sql, $game_user->id, $clan_id, $message);
  $message_orig = '';
}

echo <<< EOF
<div class="news">
  <a href="/$game/clan_list/$arg2/$clan_id" class="button">Clan List</a>
  <a href="/$game/clan_msg/$arg2/$clan_id" class="button active">Clan Messages</a>
  <a href="/$game/clan_announcements/$arg2/$clan_id"
    class="button">Announcements</a>
</div>
<div class="title">Clan Messages</div>
<div class="message-title">Send a clan message</div>
<div class="send-message">
  <form method=get action="/$game/clan_msg/$arg2/$clan_id">
    <textarea class="message-textarea" name="message" rows="2">$message_orig</textarea>
    <br/>
    <div class="send-message-send-wrapper">
      <input class="send-message-send" type="submit" value="Send"/>
    </div>
  </form>
</div>
<div class="news">
  <div class="messages-title">
    Messages
  </div>
EOF;

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
$msg_shown = FALSE;

$data = [];
while ($item = db_fetch_object($result)) {
  $data[] = $item;
}
zg_format_messages($game_user, $game_user->id, $data);
db_set_active();

foreach ($data as $item) {
  $msg_shown = TRUE;
  echo <<< EOF
    <div class="news-item $item->type {$item->display->item_css}" id="{$item->display->msg_id}">
      <div class="dateline">
        {$item->display->timestamp} {$item->display->username} {$item->display->private_text}
      </div>
      <div class="message-body {$item->display->private_css}">
        {$item->display->delete}
        <p>{$item->display->message}</p>{$item->display->reply}
      </div>
    </div>
EOF;

}
