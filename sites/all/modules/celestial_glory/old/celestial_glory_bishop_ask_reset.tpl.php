<?php

//  global $mobi, $api_theme;
  global $game, $phone_id;

  $game_user = _celestial_glory_fetch_user();
  _celestial_glory_header($game_user);

  echo <<< EOF
<div class="title">
Do you really want to reset your character?
</div>
<div class="subtitle">
You will lose all spirituality, stats, and faith your character has collected
</div>
<div class="elders-menu">
<div class="menu-option"><a href="/$game/bishop_do_reset/$phone_id">Yes, I want to reset my character</a></div>
</div>
EOF;
    
  db_set_active('default');