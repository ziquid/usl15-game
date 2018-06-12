<?php

/**
 * @file stlouis_changelog.tpl.php
 * The game's changelog.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 */

global $game, $phone_id;

include drupal_get_path('module', arg(0)) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);
db_set_active('default');

?>
<div class="news">
<a href="/<?php echo $game; ?>/help/<?php echo $arg2; ?>" class="button">Help</a>
<a href="/<?php echo $game; ?>/changelog/<?php echo $arg2; ?>" class="button active">Changelog</a>
</div>

<div class="help">
  <div class="title">
    Uprising: St. Louis Changelog
  </div>

  <div class="subtitle">
    Jun 12, 2018
  </div>
  <ul>
    <li>
      New action: Evacuate Forest Park
    </li>
    <li>
      Added support for Fast Competencies, Level 98
    </li>
  </ul>

  <div class="subtitle">
    Jun 10, 2018
  </div>
  <ul>
    <li>
      Added level 97, Security Cameras, Flag Lapel Pins
    </li>
  </ul>

  <div class="subtitle">
    Jun 7, 2018
  </div>
  <ul>
    <li>
      Added a Personal Trainer to increase energy
    </li>
    <li>
      The cost and effectiveness of Planting Flower Seeds grows as your green thumb grows
    </li>
    <li>
      Added level 96
    </li>
  </ul>

  <div class="subtitle">
    Jun 6, 2018
  </div>
  <ul>
    <li>
      A new aide, the Shady Clerk, will give you details about votes against you
    </li>
  </ul>

  <div class="subtitle">
    Jun 5, 2018
  </div>
  <ul>
    <li>
      A new aide, the Socialite, will tell you if anyone is gossiping about you
    </li>
  </ul>

  <div class="subtitle">
    Jun 4, 2018
  </div>
  <ul>
    <li>
      New bg images
    </li>
  </ul>

  <div class="subtitle">
    Jun 3, 2018
  </div>
  <ul>
    <li>
      Added three new income items, level 95
    </li>
  </ul>

  <div class="subtitle">
    Jun 2, 2018
  </div>
  <ul>
    <li>
      Ranking Committee Members can now PNG
    </li>
    <li>
      Finished filters for Equipment
    </li>
    <li>
      Differentiates between PNG, comp enhancement, and user messages on homepage
    </li>
  </ul>

  <div class="subtitle">
    Jun 1, 2018
  </div>
  <ul>
    <li>
      Added Level 94, Free donuts in Southwest Garden!
    </li>
    <li>
      The Income tab in Aides now remembers which filter you have applied.
    </li>
  </ul>

  <div class="subtitle">
    May 31, 2018
  </div>
  <ul>
    <li>
      Added Fundraising Missions.
    </li>
  </ul>

  <div class="subtitle">
    May 30, 2018
  </div>
  <ul>
    <li>
      New Changelog!
    </li>
  </ul>
</div>
