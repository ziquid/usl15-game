<?php

/**
 * @file stlouis_bounce_40.tpl.php
 * Template for bounce 40.
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

  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  db_set_active();
?>
<style>
  body {
    margin: 6px auto;
    width: 88%;
  }
  p {
    margin: 6px;
  }
</style>

  <p>Uprising: St. Louis, Community Edition</p>
  <p>May 1, 2018 Beta release 0.4.0</p>
  <h4>Please note this release is for testing only</h4>
  <h4>Many things may change</h4>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
<p>25 May: Added Support your Party Official action, level 93.</p>
<p>24 May: Memorial Day teaser.</p>
<p>22 May: Taxi subsidies.</p>
<p>20 May: Newlines are preserved in user messages, you can delete messages sent, and prettier delete button.</p>
<p>20 May: Added number of actions necessary to run for an office uncontested.</p>
<p>20 May: Fixed display of necessary money, actions for PNG.</p>
<p>20 May: Fixed not being able to get back to the game after sending a short message on the home page.</p>
<p>20 May: Bumped up the number of messages shown on the home page.</p>
<p>20 May: Level 91 and darker textareas.</p>
<p>19 May: Added buttons on Income screen to filter jobs and investments.</p>
<p>19 May: Changed Work Overtime to be allowed every four hours.</p>
<p>19 May: Updated user profile page with net income.</p>
<p>19 May: Updated home page with forum link to Discord, game version, and current hood.</p>
<p>19 May: New EA logo, added level 89,  new competency!</p>
<p>18 May: Added 2nd-round bonus for FP missions.</p>
<p>18 May: Changed Work to Work Overtime.</p>
<p>18 May: Try the Cinco De Mayo party again this weekend!</p>
<p>14 May: Added Double Your Money event, Mutual Fund aide.</p>
<p>12 May: Added fitness club membership.</p>
<p>11 May: Support your Local Official now gives more support based on Loyal comp.</p>
<p>9 May: Released Android and iOS clients v 0.4.0.</p>
<p>6 May: Extended Cinco De Mayo event one more day.</p>
<p>5 May: Added Cinco De Mayo event with prize and two new competencies.</p>
<p>3 May: Added Chevy Cruze, BMW M3.  Opened a few more neighborhoods.</p>
<p>1 May: May Quests!  Opened Lindenwood Park.</p>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
