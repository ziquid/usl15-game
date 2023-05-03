<?php

/**
 * @file
 * Clan pages.
 *
 * Synced with CG: N/A
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: done
 * Controller moved to callback include: yes
 * View only in theme template: yes
 * All db queries in controller: yes
 * Minimal function calls in view: yes
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

db_set_active('game_' . $game);
include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
zg_fetch_header($pl);
db_set_active();
?>

<div class="swiper-container page-clan">
  <div class="swiper-wrapper">
    <?php foreach ($clan_data as $data): ?>
      <div class="swiper-slide"><div class="landscape-slide-overlay">
          <div class="overlay-title"><?php print $data['title']; ?></div>
          <?php foreach ($data['errors'] as $error): ?>
            <?php print $error; ?>
          <?php endforeach; ?>
          <?php if (array_key_exists('items', $data)): ?>
            <?php foreach ($data['items'] as $item): ?>
            <?php print $item; ?>
            <?php endforeach; ?>
          <?php endif; ?>
          <?php if (array_key_exists('button', $data)): ?>
            <div class="overlay-tip"><?php print $data['button']; ?></div>
          <?php endif; ?>
      </div></div>
    <?php endforeach; ?>
  </div>
  <div class="swiper-pagination"></div>
  <div class="swiper-button-prev"></div>
  <div class="swiper-button-next"></div>
</div>
