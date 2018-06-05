<?php

  global $game, $phone_id;

  $fetch_user = '_' . arg(0) . '_fetch_user';
  $fetch_header = '_' . arg(0) . '_header';

  $game_user = $fetch_user();
  $arg2 = check_plain(arg(2));

/*
  if ($game == 'celestial_glory') {

    $error_msg =<<< EOF
<p>Bishop Danielson returns.</p>
<p class="second">&quot;Your family won't be here for a few more
  minutes.&nbsp; I'll take you to play with some other members of our ward.&nbsp;
  They like to scripture chase.&nbsp; You might find it fun!
</p>
EOF;

  }
*/
  if ($game_user->level < 6) {

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

    db_set_active('default');
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
  if ($username != "") {
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
    if (empty($game_user->username)) {

      drupal_goto($game . '/debates/' . $arg2);

    }
    else {

      // Changing existing name.
      if ($game_user->username != $username) {

        // Only do this if they chose something new.
        $message = "I've changed my name from <em>$game_user->username</em> to
          <em>$username</em>.&nbsp; Please call me <em>$username</em> from now
          on.";
        $sql = 'insert into user_messages (fkey_users_from_id,
          fkey_users_to_id, message) values (%d, %d, "%s");';
        $result = db_query($sql, $game_user->id, $game_user->id, $message);

        $sql = 'update users set luck = luck - 10 where id = %d;';
        $result = db_query($sql, $game_user->id);

      }

      drupal_goto($game . '/user/' . $arg2);

    }

  }
  else {

    // Haven't chosen a username on this screen, or chose a duplicate.

    // Set an error message if a dup.
    if ($isdupusername) {

      $msgUserDuplicate =<<< EOF
<div class="message-error big">Sorry!</div>
  <p>The username <em>$username</em> already exists.</p>
  <p class="second">Please choose a different name and try again.</p>
EOF;

    }
    else {
      $msgUserDuplicate = '<p>&nbsp;</p>';
    }

  if (empty($game_user->username)) {

    $quote = "By the way, what's your name?";

  }
  else {

    // Allow them to navigate out of this.
    $fetch_header($game_user);
    $quote = "Hello, <em>$game_user->username</em>!&nbsp; What would you like
      your new name to be?";

    if ($game_user->luck < 10) {

      echo <<< EOF
<div class="land-failed">Not enough $luck!</div>
<div class="subtitle">
  <a href="/$game/elders/$arg2">
    <img src="/sites/default/files/images/{$game}_continue.png"/>
  </a>
</div>
EOF;

      db_set_active('default');

    }

  }

    echo <<< EOF
$msgUserDuplicate
<div class="welcome">
  <div class="wise_old_man_small">
  </div>$error_msg
  <p class="second">&quot;$quote&quot;</p>
  <p class="second">&nbsp;</p>
  <div class="ask-name">
    <form method=get action="/$game/choose_name/$arg2">
      <input type="text" name="username" width="20" maxlength="20"/>
      <input type="submit" value="Submit"/>
    </form>
  </div>
</div>
EOF;

  }

  db_set_active('default');
