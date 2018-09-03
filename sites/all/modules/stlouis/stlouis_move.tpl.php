<?php

/**
 * @file stlouis_move.tpl.php
 * Stlouis move
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

global $game, $phone_id;
include drupal_get_path('module', $game) . '/game_defs.inc' ;
$game_user = $fetch_user();
$fetch_header($game_user);

$sql = 'select name from neighborhoods where id = %d;';
$result = db_query($sql, $game_user->fkey_neighborhoods_id);
$item = db_fetch_object($result);
$location = $item->name;

if ($neighborhood_id == $game_user->fkey_neighborhoods_id &&
  $game_user->meta != 'admin') {

  echo <<< EOF
<div class="title">You are already in $location</div>
<div class="election-continue"><a href="0">Try again</a></div>
EOF;

  db_set_active('default');
  return;
}

if ($neighborhood_id > 0) {

  $sql = 'select * from neighborhoods where id = %d;';
  $result = db_query($sql, $game_user->fkey_neighborhoods_id);
  $cur_hood = db_fetch_object($result);
//firep($cur_hood);

  $sql = 'select * from neighborhoods where id = %d;';
  $result = db_query($sql, $neighborhood_id);
  $new_hood = db_fetch_object($result);
//firep($new_hood);

  $distance = floor(sqrt(pow($cur_hood->xcoor - $new_hood->xcoor, 2) +
    pow($cur_hood->ycoor - $new_hood->ycoor, 2)));

  $actions_to_move = floor($distance / 8);
  $verb = t('Walk');

  $sql = 'SELECT equipment.speed_increase as speed_increase, 
    action_verb from equipment 

    left join equipment_ownership
      on equipment_ownership.fkey_equipment_id = equipment.id
      and equipment_ownership.fkey_users_id = %d
      
    where equipment_ownership.quantity > 0
      
    order by equipment.speed_increase DESC limit 1;';

  $result = db_query($sql, $game_user->id);
  $eq = db_fetch_object($result);

  if ($eq->speed_increase > 0) {
    $verb = t($eq->action_verb);
    game_alter('speed_increase', $game_user, $eq->speed_increase, $verb);
    $actions_to_move -= $eq->speed_increase;
  }

  $actions_to_move = max($actions_to_move, 6);

  if (($game_user->meta == 'frozen') && ($actions_to_move > 6)) {

    echo <<< EOF
<div class="title">Frozen!</div>
<div class="subtitle">You have been tagged and cannot move more than
6 actions at a time</div>
<div class="subtitle">Call on a teammate to unfreeze you!</div>
<div class="try-an-election-wrapper"><div
class="try-an-election"><a href="/$game/home/$arg2">Go to the home page</a></div></div>
<div class="try-an-election-wrapper"><div
class="try-an-election"><a href="0">Let me choose again</a></div></div>
EOF;

    db_set_active('default');
    return;
  }

  game_alter('actions_to_move', $game_user, $actions_to_move);

  echo <<< EOF
<div class="title">$verb from $cur_hood->name to $new_hood->name</div>
<div class="subtitle">It will cost $actions_to_move Actions to move</div>
<div class="try-an-election-wrapper"><div
class="try-an-election"><a href="/$game/move_do/$arg2/$neighborhood_id">Yes,
I want to go</a></div></div>
<div class="try-an-election-wrapper"><div
class="try-an-election"><a href="0">No, let me choose again</a></div></div>
EOF;

  db_set_active('default');
  return;
}

if (substr(arg(2), 0, 4) == 'nkc ') {
  $coefficient = 1.875;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 9') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 8') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 7') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 6') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 5') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.4') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.3') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.2') !== FALSE) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4.1') !== FALSE) {
  $coefficient = 1;
}
else if ((stripos($_SERVER['HTTP_USER_AGENT'], 'BNTV') !== FALSE) &&
  (stripos($_SERVER['HTTP_USER_AGENT'], 'Android 4') !== FALSE)) {
  $coefficient = 1;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=800') !== FALSE) {
  $coefficient = 2.5;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=600') !== FALSE) {
  $coefficient = 1.875;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=533') !== FALSE) {
  $coefficient = 1.66;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=480') !== FALSE) {
  $coefficient = 1.5;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=400') !== FALSE) {
  $coefficient = 1.25;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=411') !== FALSE) {
  $coefficient = 1.25;
}
else if (stripos($_SERVER['HTTP_USER_AGENT'], 'width=360') !== FALSE) {
  $coefficient = 1.125;
}
else {
  $coefficient = 1;
}

$divisor = 2.5; // 800/320
$ext = '.jpg';
$nonce = date('Y-m-d-H-i-s-') . mt_rand();

$sql = 'select xcoor, ycoor from neighborhoods where id = %d;';
$result = db_query($sql, $game_user->fkey_neighborhoods_id);
$curpos = db_fetch_object($result);

$xcoor = floor(($curpos->xcoor - 10) * $coefficient / $divisor);
$ycoor = floor(($curpos->ycoor - 20) * $coefficient / $divisor);

$sql = 'select color from `values` where id = %d;';
$result = db_query($sql, $game_user->fkey_values_id);
$color = db_fetch_object($result);

echo <<< EOF
<div id="map_large">
  <div class="title">Move&nbsp;from&nbsp;$location to&nbsp;another&nbsp;$hood_lower</div>
  <div class="subtitle">Touch the map to zoom in</div>
  <div class="map-image">
    <a href="#">
      <img src="/sites/default/files/images/{$game}_map_large_colored$ext?a=$nonce" width="320">
    </a>
    <div class="map-marker" style="top: {$ycoor}px; left: {$xcoor}px; color: #$color->color;">X</div>
  </div>
</div>
<div id="map_large_bottom">
  <a href="#">
    <img src="/sites/default/files/images/{$game}_map_large_bottom_colored$ext?a=$nonce" width="320"/>
  </a>
</div>
<div id="map_mid">
<div class="title">Move&nbsp;from&nbsp;$location to&nbsp;another&nbsp;$hood_lower</div>
<div class="subtitle">Touch the map to select a new $hood_lower</div>
<div class="subtitle">Touch the upper left corner of the map to go back</div>
  <img src="/sites/default/files/images/{$game}_map_mid$ext?a=$nonce" width="320"
    usemap="#map_mid"/>
  <map name="map_mid">
    <area id="map_mid_back_click" shape="rect" coords="0,0,80,80" alt="Back" href="#" />
EOF;

$sql = 'select * from neighborhoods where xcoor > 0 or ycoor > 0;';
$result = db_query($sql);
$data = [];
while ($item = db_fetch_object($result)) {
  $data[] = $item;
}

$divisor = 2.15625; // 690/320
$xoff = 54; // offset of x
$yoff = 488; // offset of y

foreach ($data as $item) {
  $xcoor = floor(($item->xcoor - $xoff) * $coefficient / $divisor);
  $ycoor = floor(($item->ycoor - $yoff) * $coefficient / $divisor);
  echo "<area shape=\"circle\" coords=\"$xcoor,$ycoor,16\" href=\"$item->id\"
    alt=\"$item->name\" />";
}

echo <<< EOF
  </map>
</div>
<div id="map_bottom">
  <div class="title">Move&nbsp;from&nbsp;$location to&nbsp;another&nbsp;$hood_lower</div>
  <div class="subtitle">Touch the map to select a new $hood_lower</div>
  <div class="subtitle">Touch the upper left corner of the map to go back</div>
  <img src="/sites/default/files/images/{$game}_map_bottom$ext?a=$nonce" width="320"
    usemap="#map_bottom"/>
  <map name="map_bottom">
    <area id="map_bottom_back_click" shape="rect" coords="0,0,20,80" alt="Back" href="#" />
EOF;

//  $divisor = 2.15625; // 690/320

// Offset of x.
$xoff = 0;

// Offset of y.
$yoff = 900;

foreach ($data as $item) {
//firep($item);

  $xcoor = floor(($item->xcoor - $xoff) * $coefficient / $divisor);
  $ycoor = floor(($item->ycoor - $yoff) * $coefficient / $divisor);

  if ($xcoor >=16 /* && $xcoor < 320 */ &&
    $ycoor >= 16 /* && $ycoor <= 334 */) {
    echo "<area shape=\"circle\" coords=\"$xcoor,$ycoor,16\" href=\"$item->id\"
      alt=\"$item->name\" />\n";
  }

}

echo <<< EOF
  </map>
</div>
EOF;

if (game_user_has_trait($game_user, 'show_highlighted_quest_groups_on_map')) {
  // FIXME: add hood_id to the query.
  $hood_equip = game_fetch_visible_equip($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_equip as $item) {
    if ($item->fkey_neighborhoods_id == $game_user->fkey_neighborhoods_id) {
      if (!$title_shown) {
        echo <<< EOF
<br>
<div class="title">
  Useful Equipment in <span class="nowrap">$game_user->location</span>
</div>
EOF;
        $title_shown = TRUE;
      }
      game_show_equip($game_user, $item, $ai_output);
    }
  }

  // FIXME: add hood_id to the query.
  $hood_staff = game_fetch_visible_staff($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_staff as $item) {
    if ($item->fkey_neighborhoods_id == $game_user->fkey_neighborhoods_id) {
      if (!$title_shown) {
        echo <<< EOF
<br>
<div class="title">
  Useful Staff and Aides in <span class="nowrap">$game_user->location</span>
</div>
EOF;
        $title_shown = TRUE;
      }
      game_show_staff($game_user, $item, $ai_output);
    }
  }

  $hood_qgs = game_fetch_visible_quest_groups($game_user);
  $ai_output = '';
  $title_shown = FALSE;

  foreach ($hood_qgs as $item) {
    if (!$title_shown) {
      echo <<< EOF
<br>
<div class="title">
  Useful Missions in <span class="nowrap">$game_user->location</span>
</div>
EOF;
      $title_shown = TRUE;
      game_show_quest_group($game_user, $item, $ai_output);
    }
  }
}

// FIXME: move this to .js, update to use jQuery.
echo <<< EOF
<script type="text/javascript">

window.onload = function() {

document.getElementById('map_mid').style.display = 'none';
document.getElementById('map_bottom').style.display = 'none';

document.getElementById('map_large').onclick = function() {
  document.getElementById('map_large').style.display = 'none';
  document.getElementById('map_large_bottom').style.display = 'none';
  document.getElementById('map_mid').style.display = 'block';
  document.getElementById('map_bottom').style.display = 'none';
  return false;
};

document.getElementById('map_large_bottom').onclick = function() {
  document.getElementById('map_large').style.display = 'none';
  document.getElementById('map_large_bottom').style.display = 'none';
  document.getElementById('map_mid').style.display = 'none';
  document.getElementById('map_bottom').style.display = 'block';
  return false;
};

document.getElementById('map_mid_back_click').onclick = function() {
  document.getElementById('map_large').style.display = 'block';
  document.getElementById('map_large_bottom').style.display = 'block';
  document.getElementById('map_mid').style.display = 'none';
  document.getElementById('map_bottom').style.display = 'none';
  return false;
};

document.getElementById('map_bottom_back_click').onclick = function() {
  document.getElementById('map_large').style.display = 'block';
  document.getElementById('map_large_bottom').style.display = 'block';
  document.getElementById('map_mid').style.display = 'none';
  document.getElementById('map_bottom').style.display = 'none';
  return false;
};

}
</script>
EOF;

db_set_active('default');
