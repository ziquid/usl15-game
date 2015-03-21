<?php

//  global $mobi, $api_theme;
  global $game, $phone_id;

  $game_user = _celestial_glory_fetch_user();
  _celestial_glory_header($game_user);
  
  echo <<< EOF
<div class="title">
Visit Bishop Danielson
</div>
<div class="elders-menu">
<div class="menu-option"><a href="/$game/bishop_ask_reset/$phone_id">Reset
  your character</a></div>
EOF;
/*
  if ($game_user->level > 6) {
// only allow users to change parties if they can join one

  	echo <<< EOF
<div class="menu-option"><a href="/$game/choose_clan/$phone_id/0">Join a 
  different political party</a></div>
EOF;

  }
*/
  echo <<< EOF
</div>
EOF;

  db_set_active('default');