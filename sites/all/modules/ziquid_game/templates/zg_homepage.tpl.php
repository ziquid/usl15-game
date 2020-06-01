<?php

/**
 * @file
 * The game's main screen.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: done
 * Controller moved to callback include: no
 * View only in theme template: yes
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

/* ------ CONTROLLER ------ */

$version = 'v0.9.7b, Jun 01 2020';

global $game, $phone_id;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_user();
$message = check_plain($_GET['message']);

$d = zg_get_html(
  [
    'tagline',
  ]
);

zg_competency_gain($game_user, 'where the heart is');

if (substr($phone_id, 0, 3) == 'ai-') {
  echo "<!--\n<ai \"home\"/>\n-->";
}

$today = date('Y-m-d');

$sql = 'select residents, rating from neighborhoods where id = %d;';
$result = db_query($sql, $game_user->fkey_neighborhoods_id);
$item = db_fetch_object($result);
$beauty = (int) $item->rating;

if ($game_user->last_bonus_date != $today || $game_user->meta == 'admin') {
  $money = ($game_user->level * $item->residents) + $game_user->income -
    $game_user->expenses;
  $money = ceil($money);
  $extra_bonus = '';

firep("adding $money money because last_bonus_date = {$game_user->last_bonus_date}");

  $sql = 'update users set money = money + %d, last_bonus_date = "%s"
    where id = %d;';
  $result = db_query($sql, $money, $today, $game_user->id);
  $game_user = zg_fetch_user();

  $extra_bonus = '
<div class="speech-bubble-wrapper background-color">
  <div class="wise_old_man happy">
  </div>
  <div class="speech-bubble">
    <p class="bonus-text">Daily Bonus!</p>
    <p>You have received <strong>' .
    number_format($money) . ' ' . $game_user->values . '</strong>!</p>
    <p>For the next three minutes, competencies can be enhanced every 15 seconds.</p>
    <p>Come back tomorrow for another bonus!</p>
  </div>
</div>';

  // Fast comps for the next three minutes.
  zg_set_timer($game_user, 'fast_comps_15', 180);

  // Add competency for work.
  $sql = 'SELECT land.fkey_enhanced_competencies_id FROM `land`
  left join land_ownership on land_ownership.fkey_land_id = land.id
  left join users on users.id = land_ownership.fkey_users_id
  WHERE users.id = %d and land.type = "job"';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  if (isset($item->fkey_enhanced_competencies_id)) {
    zg_competency_gain($game_user, (int) $item->fkey_enhanced_competencies_id);
  }

}

// Get values of current hood's alder.
$alder = zg_get_hood_alder($game_user->fkey_neighborhoods_id);
firep($alder, 'current hood alder');
$alder_values = drupal_strtolower($alder->values);
$player_values = drupal_strtolower($game_user->values);

// AI bot?  No reason to spend cycles on this.
if (substr($phone_id, 0, 3) == 'ai-') {
  zg_fetch_header($game_user);
  db_set_active();
  return;
}

if (empty($game_user->referral_code)) {
  $good_code = FALSE;
  $count = 0;

  while (!$good_code && $count++ < 10) {
    $referral_code = '0000' .
      base_convert(mt_rand(0, pow(36, 5) - 1) . '', 10,
        36);
    $referral_code = strtoupper(substr($referral_code,
      strlen($referral_code) - 5, 5));
firep($referral_code);

    $sql = 'select referral_code from users where referral_code = "%s";';
    $result = db_query($sql, $referral_code);
    $item = db_fetch_object($result);

    // Code not already in use - use it!
    if (empty($item->referral_code)) {
      $good_code = TRUE;
      $sql = 'update users set referral_code = "%s" where id = %d;';
      $result = db_query($sql, $referral_code, $game_user->id);
      $game_user->referral_code = $referral_code;
    }
  }
}

$event_text = '';
zg_alter('homepage_event_notice', $game_user, $event_text);
zg_alter('homepage_menu', $game_user, $extra_menu);

$link = 'quest_groups';
$lqg = zg_fetch_latest_quest_group($game_user);
$show_expanded = ($game_user->level < 7) ? '?show_expanded=0' : '';

if ($game_user->fkey_clans_id > 0) {
  $clan_link = "clan_list/$arg2/{$game_user->fkey_clans_id}";
}
else {
  $clan_link = "clan_list_available/$arg2";
}

$debates_class = drupal_html_class($game_text['menu']['debates']) . '-menu';
$data = zg_get_all_messages($game_user);
zg_format_messages($game_user, $game_user->id, $data);
zg_alter('homepage_messages', $game_user, $data);
$msg_shown = FALSE;
$msg_options = '';

if ($game_user->fkey_clans_id) {
  $msg_options .= '<option value="clan">Clan</option>';
}
if ($game_user->can_broadcast_to_party || $game_user->meta == 'admin') {
  $msg_options .= '<option value="neighborhood">' . $hood . '</option>';
}

/* ------ VIEW ------ */

zg_fetch_header($game_user);
db_set_active();

echo <<< EOF
<div class="title portrait-only">
  <img src="/sites/default/files/images/{$game}_title.png">
</div>
<div class="tagline portrait-only">
  {$d['tagline']}
</div>
<a class="version" href="/$game/changelog/$arg2">
  $version
</a>
$extra_bonus
<div class="new-main-menu">
  <img src="/sites/default/files/images/{$game}_home_menu{$extra_menu}.jpg">
    <ul class="value-links portrait-only">
      <li>
        <span class="ammunition value first odd">
          50
        </span>
        <span class="ammunition label first odd">
          AMM
        </span>
      </li>
      <li>
        <span class="beauty value even">
          $beauty
        </span>
        <span class="beauty label even">
          BTY
        </span>
      </li>
      <li>
        <span class="chaos value odd">
          50
        </span>
        <span class="chaos label odd">
          CHA
        </span>
      </li>
      <li>
        <span class="faith value even">
          50
        </span>
        <span class="faith label even">
          FTH
        </span>
      </li>
      <li>
        <span class="finance value odd">
          50
        </span>
        <span class="finance label odd">
          FIN
        </span>
      </li>
      <li>
        <span class="health value even">
          50
        </span>
        <span class="health label even">
          HEA
        </span>
      </li>
      <li>
        <span class="intelligence value odd">
          50
        </span>
        <span class="intelligence label odd">
          INT
        </span>
      </li>
      <li>
        <span class="strength value even last">
          50
        </span>
        <span class="strength label even last">
          STR
        </span>
      </li>
    </ul>
    <ul class="menu-links">

      <li>
        <a class="actions-menu" href="/$game/actions/$arg2">
          {$game_text['menu']['actions']}
        </a>
      </li>

      <li>
        <a class="aides-menu" href="/$game/land/$arg2">
          {$game_text['menu']['aides']}
        </a>
      </li>

      <li>
        <a class="clan-menu portrait-only" href="/$game/$clan_link">
          {$game_text['menu']['clan']}
        </a>
      </li>

      <li>
        <a class="$debates_class" href="/$game/debates/$arg2">
          {$game_text['menu']['debates']}
        </a>
      </li>

      <li>
        <a class="elders-menu portrait-only" href="/$game/elders/$arg2">
          {$game_text['menu']['elders']}
        </a>
      </li>

      <li>
        <a class="elections-menu" href="/$game/elections/$arg2">
          {$game_text['menu']['elections']}
        </a>
      </li>

      <li>
        <a class="forum-menu" href="external://discord.gg/cFyt7w9">
          {$game_text['menu']['forum']}
        </a>
      </li>

      <li>
        <a class="help-menu" href="/$game/help/$arg2">
          {$game_text['menu']['help']}
        </a>
      </li>

      <li>
        <a class="missions-menu portrait-only"
          href="/$game/$link/$arg2{$show_expanded}#group-{$lqg}">
          {$game_text['menu']['missions']}
        </a>
      </li>

      <li>
        <a class="move-menu portrait-only" href="/$game/move/$arg2/0">
          {$game_text['menu']['move']}
        </a>
      </li>

      <li>
        <a class="profile-menu portrait-only" href="/$game/user/$arg2">
          {$game_text['menu']['profile']}
        </a>
      </li>

    </ul>
  </div>
  <div class="location portrait-only">
    <span class="location-$alder_values player-$player_values">$game_user->location</span>
  </div>
  $event_text
  <div class="news">
    <div class="title">
      News
    </div>
    <div class="news-buttons">
      <button id="news-all" class="active">All</button>
      <button id="news-user">Personal</button>
      <button id="news-challenge">{$election_tab}</button>
      <button id="news-clan">$party_small</button>
      <button id="news-mayor">${game_text['mayor_tab']}</button>
    </div>
    <div id="all-text">
      <div class="news-item clan clan-msg">
        <div class="message-title">Send a message to your clan</div>
        <div class="send-message">
        <form method=get action="/$game/party_msg/$arg2">
          <textarea class="message-textarea" name="message" rows="2">$message</textarea>
          <br>
          <div class="send-message-target">
            <select name="target">
              $msg_options
              <option value="values">$party</option>
            </select>
          </div>
          <div class="send-message-send-wrapper">
            <input class="send-message-send" type="submit" value="Send"/>
          </div>
        </form>
      </div>
    </div>
EOF;

foreach ($data as $item) {
  echo <<< EOF
    <div class="news-item $item->type" id="{$item->display->msg_id}">
      <div class="dateline">
        {$item->display->timestamp} {$item->display->username} {$item->display->private_text}
      </div>
      <div class="message-body {$item->display->private_css}">
        {$item->display->delete}
        <p>$item->message</p>{$item->display->reply}
      </div>
    </div>
EOF;
  $msg_shown = TRUE;

}

echo <<< EOF
  </div>
</div>
<script type="text/javascript">
var isoNews = $('#all-text').isotope({
itemSelector: '.news-item',
layoutMode: 'fitRows'
});

$("#news-all").bind("click", function() {
isoNews.isotope({ filter: "*:not(.clan-msg)" });
$(".news-buttons button").removeClass("active");
$("#news-all").addClass("active");
});

$('#news-user').bind('click', function() {
isoNews.isotope({ filter: ".user" });
$(".news-buttons button").removeClass("active");
$("#news-user").addClass("active");
});

$('#news-challenge').bind('click', function() {
isoNews.isotope({ filter: ".challenge" });
$(".news-buttons button").removeClass("active");
$("#news-challenge").addClass("active");
});

$('#news-clan').bind('click', function() {
isoNews.isotope({ filter: ".hood, .clan, .values" });
$(".news-buttons button").removeClass("active");
$("#news-clan").addClass("active");
});

$('#news-mayor').bind('click', function() {
isoNews.isotope({ filter: ".mayor" });
$(".news-buttons button").removeClass("active");
$("#news-mayor").addClass("active");
});
</script>
<!--  <div id="personal-text">-->
EOF;

