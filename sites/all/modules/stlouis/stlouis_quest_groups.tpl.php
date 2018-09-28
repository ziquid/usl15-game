<?php

/**
 * List of quest groups.
 *
 * @file stlouis_quest_groups.tpl.php
 *
 * Synced with CG: N/A
 * Synced with 2114: N/A
 * Ready for phpcbf: yes
 */

global $game, $phone_id;

// ------ CONTROLLER ------
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$quest_groups = game_fetch_quest_groups($game_user);

// ------ VIEW ------
$fetch_header($game_user);
?>

<div class="swiper-container">
  <div class="swiper-wrapper">
    <?php
      foreach ($quest_groups as $quest_group):
    ?>
      <?php game_show_quest_group_slide($game_user, $quest_group); ?>
    <?php
      endforeach;
    ?>
  </div>
  <div class="swiper-pagination"></div>
  <div class="swiper-button-prev"></div>
  <div class="swiper-button-next"></div>
</div>

<?php
db_set_active('default');
