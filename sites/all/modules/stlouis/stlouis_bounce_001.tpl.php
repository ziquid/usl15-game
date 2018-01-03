<?php
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  db_set_active('default');
?>
<style>
  body {
    margin: 6px auto;
  }
  p {
    margin: 6px;
  }
</style>

  <p>Uprising: St. Louis Community Edition</p>
  <p>January 3, 2018 Alpha release 0.0.1</p>
  <form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
    <input type="Submit" value="Continue to the game"/>
  </form>
