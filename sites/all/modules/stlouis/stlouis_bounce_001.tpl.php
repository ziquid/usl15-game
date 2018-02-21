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
  <p>February 1, 2018 Alpha release 0.1.0</p>
  <h4>Please note this release is for testing only</h4>
  <h4>Do not expect anything in this release to be permanent</h4>
  <p>Feb 21: Added PNG to actions</p>
  <p>Feb 19: Fixed top alder list</p>
  <p>Jan 30: Fixed resetting your player</p>
  <p>Jan 27: Fixed clan list, added clan rules</p>
  <p>Jan 24: Fixed referral codes</p>
  <p>Jan 9: Some Galaxy S8 devices have the home page and map screen off a little.
  We've fixed it some but let us know if your device still has the home page or
  map offset.  Email zipport@ziquid.com.</p>
  <p>Jan 8: Working to fix Luck purchases</p>
  <form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
    <input type="Submit" value="Continue to the game"/>
  </form>
