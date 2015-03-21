<?php

//  global $mobi, $api_theme;
  global $game, $phone_id;

  $game_user = _celestial_glory_fetch_user();

  $sql = 'delete from users where id = %d;';
  $result = db_query($sql, $game_user->id);
  
  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  
  drupal_goto($game . '/welcome/' . $phone_id);