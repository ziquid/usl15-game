<?php

/**
 * @file
 * Enter the user's password.
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
 * Removal of game_defs include: N/A
 * .
 */

global $game, $phone_id;

// We won't have gone through fetch_user() yet, so set these here.
$game = check_plain(arg(0));
$phone_id = zg_get_phoneid();
$arg2 = check_plain(arg(2));
db_set_active('game_' . $game);

$d = zg_get_html(
  [
    'tagline',
    'authenticate',
    'authenticate_speech',
    'authentication_error',
  ]
);

$sql = 'select * from users where phone_id = "%s";';
$result = db_query($sql, $phone_id);
$game_user = db_fetch_object($result);
firep($game_user, 'game_user object');
zg_check_authkey($game_user);

// Check for authorized client.
if ((strpos($_SERVER['HTTP_USER_AGENT'], '(com.ziquid.') === FALSE) &&

  // Paypal IPN.
  ($_SERVER['REMOTE_ADDR'] != '66.211.170.66') &&
  ($_SERVER['REMOTE_ADDR'] != '173.0.81.1') &&
  ($_SERVER['REMOTE_ADDR'] != '173.0.81.33') &&
  ($_SERVER['REMOTE_ADDR'] != '173.0.81.65') &&
  ($_SERVER['REMOTE_ADDR'] != '173.0.81.140') &&

  // Web users.
  ($user->roles[4] != 'web game access') &&

  // Identified facebook user.
  (substr($arg2, 0, 3) != 'fb=') &&

  // AI player.
  (substr($arg2, 0, 3) != 'ai-') &&

  // Unidentified facebook user.
  ($arg2 != 'facebook') &&

  // Unidentified MS user.
  (substr($arg2, 0, 3) != 'ms=')
) {
  print $d['authentication_error'];
  db_set_active();
  exit;
}

$password = key_exists('password', $_GET) ? trim(check_plain($_GET['password'])) : '';
if ($password == trim($game_user->password) || password_verify($password, trim($game_user->password))) {
  $user_agent = $_SERVER['HTTP_USER_AGENT'];
  $ip_addr = ip_address();

  $extra_stuff_pos = stripos($user_agent, '(com.ziquid');

  // Remove our added stuff, if present.
  if ($extra_stuff_pos !== FALSE) {
    $user_agent = trim(substr($user_agent, 0, $extra_stuff_pos));
  }

  zg_set_value($game_user, 'user_agent', $user_agent);
  zg_set_value($game_user, 'last_IP', $ip_addr);

  db_set_active();
  drupal_goto("$game/home/$arg2");
}

zg_slack($game_user, 'pages', 'authenticate',
  "\"Authenticate\" for Player \"$game_user->username\".");

/* ------ VIEW ------ */
?>
<div class="title">
  <img src="/sites/default/files/images/<?php print $game; ?>_title.png">
</div>
<div class="tagline">
  <?php print $d['tagline']; ?>
</div>

<?php print $d['authenticate']; ?>
<?php zg_speech($game_user, $d['authenticate_speech']); ?>
<?php
   db_set_active();
