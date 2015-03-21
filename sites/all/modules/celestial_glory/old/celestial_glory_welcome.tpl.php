<?php

//  global $mobi, $api_theme;
  global $game, $phone_id;

// we won't have gone through fetch_user() yet, so set these here
  $game = check_plain(filter_xss(arg(0)));
  $phone_id = check_plain(filter_xss(arg(2)));
	db_set_active('game_' . $game);
	
  $sql = 'insert into users set phone_id = "%s", username = "", experience = 0,
    level = 1, fkey_neighborhoods_id = 1, fkey_values_id = 1,
    `values` = "Faith",
    money = 500, energy = 200, energy_max = 200';
  $result = db_query($sql, $phone_id);

  $sql = 'insert into user_creations set datetime = "%s", phone_id = "%s",
    remote_ip = "%s";';
  $result = db_query($sql, date('Y-m-d H:i:s'), $phone_id,
    $_SERVER['REMOTE_ADDR']);
  
  $game_user = _celestial_glory_fetch_user(); // refetch stats  
  _celestial_glory_header($game_user);

  echo <<< EOF
<p>&nbsp;</p>
<div class="title">
Welcome to Celestial Glory!
</div>
<p>&nbsp;</p>
<div class="welcome">
  <div class="wise_old_man_large">
  </div>
	<p>You walk into Church for the first day in a new ward.&nbsp; Bishop Danielson
	  comes out to greet you and your family.</p>
	<p class="second">&quot;Welcome to the ward!&nbsp; How are you all?&nbsp We're
	  so glad to have you in our ward.&nbsp; I'll show you around so that you know
	  where all of the meetings are.&quot;</p>
	<p class="second">He looks down at you.&nbsp; &quot;I see you have a young
	  child.&nbsp; I'll show you where Nursery is.&quot;</p>
	<p class="second">Yes, you are two years old.&nbsp; You're going to Nursery.</p>
	<div class="subtitle">
		How to play
	</div>
	<ul>
	  <li>Learn doctrine as you attend your meetings</li>
	  <li>Interact with other players as you serve in callings</li>
	  <li>Try your hand at various callings, including Sunday School teacher,
	    Cubmaster, and even Bishop!</li>
	</ul>
</div>
<div class="welcome-continue"><a
  href="/$game/quests/$phone_id">Continue</a></div>
EOF;
    
  db_set_active('default');