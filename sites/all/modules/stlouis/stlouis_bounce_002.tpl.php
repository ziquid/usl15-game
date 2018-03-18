<?php
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  db_set_active('default');
?>
<style>
  body {
    margin: 6px auto;
    width: 100%;
  }
  p {
    margin: 6px;
  }
</style>

  <p>Uprising: St. Louis, Community Edition</p>
  <p>March 6, 2018 Beta release 0.2.0</p>
  <h4>Please note this release is for testing only</h4>
  <h4>Some things may change</h4>
  <h4>All accounts have been reset!</h4>
  <h4>If you bought Luck during the Alpha testing, email zipport@ziquid.com
    to have our support staff re-credit it to your account</h4>
<p>18 Mar: The new Debate Coach will critique your last three debates,
  giving insight into your debate performance.</p>
<p>17 Mar: Happy St. Patrick's Day!</p>
<p>17 Mar: New equipment, Android phone, for purchase gives you 2 initiative.</p>
<p>17 Mar: New equipment, Android tablet, for purchase gives you 3 endurance.</p>
<p>17 Mar: Added flower seeds and the ability to plant flowers.</p>
<p>16 Mar: New investment: Savings Account.</p>
<p>14 Mar: New agent for loot: Art critic.</p>
<p>14 Mar: New action: Give to a clan member.</p>
<p>14 Mar: New job: Handy-person.</p>
<p>14 Mar: Lowered max amount of currency to gain/lose per debate to 1/10 of hourly income.</p>
<p>14 Mar: Wait for competencies to occur lowered to 3 minutes (from 5).</p>
<p>13 Mar: Show chance of loot for quests.</p>
<p>13 Mar: Made the names of hoods on the map easier to read.</p>
<p>12 Mar: Opened two new hoods for elections.</p>
<p>12 Mar: Fixed the Write on a Billboard action.</p>
<p>11 Mar: Changed debate timeout to 15 minutes instead of 20.</p>
<p>11 Mar: Fixed an issue where some colors were not showing up on the map.</p>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
