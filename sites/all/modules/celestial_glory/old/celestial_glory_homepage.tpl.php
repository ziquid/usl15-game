<?php

//  global $mobi, $api_theme;
  global $game, $phone_id;

  $game_user = _celestial_glory_fetch_user();
  _celestial_glory_header($game_user);

  echo <<< EOF
<div class="title">
Main Menu
</div>
<div class="main-menu">
<div class="menu-option"><a href="/$game/meetings/$phone_id">Meetings</a></div>
<div class="menu-option"><a href="/$game/activities/$phone_id">Activities</a></div>
<div class="menu-option"><a href="/$game/bishop/$phone_id">Bishop</a></div>
<div class="menu-option"><a href="/$game/user/$phone_id">My Profile</a></div>
<div class="menu-option"><a href="/$game/help/$phone_id">Help / FAQ</a></div>
</div>
<div class="news">
<div class="title">
	News
</div>
<div class="dateline">
	18 Oct 2010
</div>
<div class="subtitle">
	Celestial Glory Preview I
</div>
<p>Thanks for helping test our first preview of <em>Celestial Glory</em>.&nbsp;
  We greatly appreciate your support.</p>
<p class="second">Thanks,</p>
<p class="second">The CheekDotCom team.</p>
</div> 
EOF;
    
  db_set_active('default');
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
                                                                                  
  