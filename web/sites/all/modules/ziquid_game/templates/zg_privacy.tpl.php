<?php

/**
 * @file
 * The game's privacy policy.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game;
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

$phone_id = zg_get_phoneid();
if ($phone_id) {
  $game_user = zg_fetch_player();
  zg_fetch_header($game_user);
}
db_set_active();

?>
<?php if ($phone_id): ?>
<div class="news">
  <a href="/<?php echo $game; ?>/help/<?php echo $arg2; ?>" class="button">Help</a>
  <a href="/<?php echo $game; ?>/privacy/<?php echo $arg2; ?>" class="button active">Privacy</a>
  <a href="external://uprising-st-louis.wikia.com/wiki/Uprising_St._Louis_Wiki" class="button">Wiki</a>
  <a href="/<?php echo $game; ?>/changelog/<?php echo $arg2; ?>" class="button">Changelog</a>
</div>
<?php endif; ?>

<div class="help">
<div class="title">
  Privacy Policy for Uprising: St. Louis Political RPG (“USL”)
</div>

<p>
  Ziquid Design Studio, LLC, operates this game, USL, and one of our main priorities is the privacy of our gamers. This Privacy Policy document contains the types of information that are collected and recorded by the game, and how we use them.
</p>

<p>
  If you have additional questions or require more information about our Privacy Policy, do not hesitate to contact us at <strong>zipport@ziquid.com</strong>.
</p>

<div class="subtitle">
  Log Files
</div>

<p>
  USL follows a standard procedure of using log files. These files log gamers when they play the mobile game. The information collected by log files include internet protocol (IP) addresses, client, Internet Service Provider (ISP), date and time stamp, referring/exit pages, and possibly the number of clicks. These are not linked to any information that is personally identifiable. The purpose of the information is for analyzing trends, administering the app, tracking players' movement on the mobile game, and gathering demographic information.
</p>

<div class="subtitle">
  Cookies and Web Beacons
</div>

<p>
  USL uses 'cookies'. These cookies are used to store information including gamers’ preferences and the pages on the mobile game that the gamers played. The information is used to optimize the experience by customizing our mobile game based on device gamers type and/or other information.
</p>

<div class="subtitle">
  Children's Information
</div>

<p>
  Another part of our priority is adding protection for children while playing our game. We encourage parents and guardians to observe, participate in, and/or monitor and guide their game activity.
</p>

<p>
  USL does not knowingly collect any Personal Identifiable Information from children under the age of 13. If you think that your child provided this kind of information on our mobile game, we strongly encourage you to contact us immediately and we will do our best efforts to promptly remove such information from our records.
</p>

<div class="subtitle">
  Online Privacy Policy Only
</div>

<p>
  This Privacy Policy applies only to our game activities and is valid for players of our mobile game with regards to the information that they share in USL. This policy is not applicable to any information collected offline or via channels other than this mobile game.
</p>

<div class="subtitle">
  Consent
</div>

<p>
  By using our mobile game, you hereby consent to our Privacy Policy and agree to its Terms and Conditions.
</p>
</div>
