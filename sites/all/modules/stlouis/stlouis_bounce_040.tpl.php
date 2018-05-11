<?php
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  db_set_active('default');
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
  <h4>Some things may change</h4>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
<p><a href="http://www.ziquid.com/" target="_blank">External Link Test (iOS)</a></p>
<p><a href="external://www.ziquid.com/">External Link Test (android)</a></p>
<p>11 May: Support your Local Official now gives more support based on Loyal comp.</p>
<p>9 May: Released Android and iOS clients v 0.4.0.</p>
<p>6 May: Extended Cinco De Mayo event one more day.</p>
<p>5 May: Added Cinco De Mayo event with prize and two new competencies.</p>
<p>3 May: Added Chevy Cruze, BMW M3.  Opened a few more neighborhoods.</p>
<p>1 May: May Quests!  Opened Lindenwood Park.</p>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>
