<?php

/**
 * @file
 * Stlouis choose name.
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

$d = zg_get_default(
    [
      'new_user_comm_member_msg',
      'welcome_page_1_speech',
      'welcome_page_2_speech',
    ]
  ) + zg_get_html(
    [
      'tagline',
      'welcome_page_1',
      'welcome_page_2',
    ]
  );

if (FALSE && $game_user->level < 6) {

echo <<< EOF
<div class="title">
<img src="/sites/default/files/images/{$game}_title.png"/>
</div>
<p>&nbsp;</p>
<div class="welcome">
<div class="wise_old_man_small">
</div>
<p>&quot;You're not influential enough yet for this page.&nbsp;
Come back at level 6.&quot;</p>
<p class="second">&nbsp;</p>
<p class="second">&nbsp;</p>
<p class="second">&nbsp;</p>
</div>
<div class="subtitle"><a
href="/$game/quests/$arg2"><img
src="/sites/default/files/images/{$game}_continue.png"/></a></div>
EOF;

  db_set_active();
  return;
}

$username = trim(check_plain($_GET['username']));

if (strlen($username) > 0 and strlen($username) < 3) {
  $error_msg .= '<div class="username-error">Your name must be at least 3
    characters long.</div>';
  $username = '';
}

$isdupusername = FALSE;

// Check for duplicate usernames.
if ($username != '') {
  $sql = 'SELECT * FROM users WHERE username = "%s"';
  $result = db_query($sql, $username);
  $isdupusername = ($result->num_rows > 0);
firep('$isdupusername = ' . $isdupusername);
}

// If they have chosen a username and it's not a dupe.
if ($username != '' && !$isdupusername) {
  $sql = 'update users set username = "%s" where id = %d;';
  $result = db_query($sql, $username, $game_user->id);

  // First timer.
  if (empty($game_user->username) || $game_user->username == '(new player)') {
    db_set_active();
    drupal_goto($game . '/debates/' . $arg2);
  }
  else {

    // Existing player.
    if (($game_user->username != $username) && ($game_user->username != '(new player)')) {

      // Existing player, new name.
      $message = "I've changed my name from <em>$game_user->username</em> to
        <em>$username</em>.&nbsp; Please call me <em>$username</em> from now
        on.";
      zg_send_user_message($game_user->id, $game_user->id, FALSE, $message);
      $sql = 'update users set luck = luck - 10 where id = %d;';
      $result = db_query($sql, $game_user->id);
      // FIXME: record Luck usage in db.
    }

    // FIXME: current workflow just goes to /user/ if name is the same.  Instead
    // Show an error message and ask for username again.
    db_set_active();
    drupal_goto($game . '/user/' . $arg2);
  }

}
else {

  // Haven't chosen a username on this screen, or chose a duplicate.
  // Set an error message if a dup.
  if ($isdupusername) {

    $msgUserDuplicate = <<< EOF
<p class="subtitle">Sorry!</p>
<p>
  The username <em>$username</em> already exists.
  Please choose a different name and try again.
</p>
EOF;

  }
  else {
    $msgUserDuplicate = '';
  }

  if (empty($game_user->username) || $game_user->username == '(new player)') {
    $quote = "By the way, what's your name?";
  }
  else {

    // Allow them to navigate out of this.
    zg_fetch_header($game_user);
    $quote = "Hello, <em>$game_user->username</em>!&nbsp; What would you like
      your new name to be?";

    if ($game_user->luck < 10) {
      // FIXME: this code still shows the form, below.  It should show a link
      // to buy more Luck.
      echo <<< EOF
  <div class="land-failed">Not enough $luck!</div>
  <div class="subtitle">
  <a href="/$game/elders/$arg2">
    <img src="/sites/default/files/images/{$game}_continue.png"/>
  </a>
  </div>
EOF;

    }

  }

  $happy = strlen($msgUserDuplicate) ? 'sad' : 'happy';

  db_set_active();

  /* ------ VIEW ------ */
  ?>
  <div class="title">
    <img src="/sites/default/files/images/<?php print $game; ?>_title.png">
  </div>
  <div class="tagline">
    <?php print $d['tagline']; ?>
  </div>

  <div class="speech-bubble-wrapper">
    <div class="wise_old_man <?php print $happy; ?>">
    </div>
    <div class="speech-bubble">
      <div class="message-error highlight"><?php print $msgUserDuplicate; ?></div>
      <p class="error"><?php print $error_msg; ?></p>
      <p class="quote"><?php print $quote; ?></p>
      <div class="ask-name">
        <form method=get action="/<?php print $game; ?>/choose_name/<?php print $arg2; ?>">
          <input type="text" name="username" width="20" maxlength="20"/>
          <input type="submit" value="Submit"/>
        </form>
      </div>
    </div>
  </div>

<?php
}
