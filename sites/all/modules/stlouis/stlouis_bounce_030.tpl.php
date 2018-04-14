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
  <p>April 9, 2018 Beta release 0.3.0</p>
  <h4>Please note this release is for testing only</h4>
  <h4>Some things may change</h4>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
<p>14 Apr: Added third Campaigning mission, changed Artistes to Anarchists.</p>
<p>13 Apr: Added IRA Aide, second Campaigning mission.</p>
<p>12 Apr: Added the first Campaigning mission.</p>
<p>11 Apr: Added Metro Bus Pass, VW Bug for getting around town.</p>
<p>10 Apr: Added Russian Hackers as bonus for second round of Basic Missions, Da.</p>
<p>10 Apr: Added Cleaning Crew Member with associated action and competency.</p>
<p>10 Apr: Fixed: not enough money when hiring staff doesn't offer a Luck purchase.</p>
<p>10 Apr: Lowered the number of uses necessary for competency gains.</p>
<p>10 Apr: Added Petty Thief with associated action and competency.</p>
<p>9 Apr: Fixed: looting staff/agents doesn't trigger the Looter competency.</p>
<p>9 Apr: Update to balance the Feel the Pulse of the City mission.</p>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
