<?php

/**
 * @file
 * The game's changelog.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
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
    Oct 12, 2018
  </div>
  <ul>
    <li>
      Lemonade Stand quests
    </li>
  </ul>

  <div class="subtitle">
    Oct 7, 2018
  </div>
  <ul>
    <li>
      Welcome wizard updates
    </li>
  </ul>

  <div class="subtitle">
    Oct 6, 2018
  </div>
  <ul>
    <li>
      Added a tagline
    </li>
    <li>
      Enabled code: GiveMeMyTongue
    </li>
  </ul>

  <div class="subtitle">
    Oct 1, 2018
  </div>
  <ul>
    <li>
      Cars icons to indicate quests in other hoods
    </li>
    <li>
      City Elder allows access to pre-release features
    </li>
  </ul>

  <div class="subtitle">
    Sep 29, 2018
  </div>
  <ul>
    <li>
      Levels 122-5
    </li>
    <li>
      Formal Wear for sale in CWE
    </li>
  </ul>

  <div class="subtitle">
    Aug 27, 2018
  </div>
  <ul>
    <li>
      Level 121
    </li>
    <li>
      Hot Dog Stand
    </li>
  </ul>

  <div class="subtitle">
    Aug 15, 2018
  </div>
  <ul>
    <li>
      Level 120
    </li>
    <li>
      Support for Android 9
    </li>
  </ul>

  <div class="subtitle">
    Jul 17, 2018
  </div>
  <ul>
    <li>
      Level 119
    </li>
    <li>
      Top 20 for Debates, Debate win/loss ratio, Income, and Cash on hand are active
    </li>
  </ul>

  <div class="subtitle">
    Jul 16, 2018
  </div>
  <ul>
    <li>
      Level 118
    </li>
    <li>
      Stats shown for players are now based on competencies
    </li>
  </ul>

  <div class="subtitle">
    Jul 14, 2018
  </div>
  <ul>
    <li>
      Level 117, Investigate a Clan Member
    </li>
    <li>
      Many more stats are shown for players being investigated
    </li>
    <li>
      New competency!
    </li>
  </ul>

  <div class="subtitle">
    Jul 11, 2018
  </div>
  <ul>
    <li>
      Level 116
    </li>
  </ul>

  <div class="subtitle">
    Jul 10, 2018
  </div>
  <ul>
    <li>
      Level 115, rot13 competencies
    </li>
  </ul>

  <div class="subtitle">
    Jul 09, 2018
  </div>
  <ul>
    <li>
      Level 114, another competency, finished Gateway Arch Museum missions
    </li>
  </ul>

  <div class="subtitle">
    Jul 06, 2018
  </div>
  <ul>
    <li>
      Level 113, more party seats
    </li>
  </ul>

  <div class="subtitle">
    Jul 05, 2018
  </div>
  <ul>
    <li>
      Level 112
    </li>
  </ul>

  <div class="subtitle">
    Jul 04, 2018
  </div>
  <ul>
    <li>
      Level 111, Fair St. Louis
    </li>
  </ul>

  <div class="subtitle">
    Jul 03, 2018
  </div>
  <ul>
    <li>
      Gateway Arch Museum Mission
    </li>
    <li>
      Covered Wagons
    </li>
    <li>
      Level 110
    </li>
  </ul>

  <div class="subtitle">
    Jul 02, 2018
  </div>
  <ul>
    <li>
      Level 109
    </li>
  </ul>

  <div class="subtitle">
    Jun 30, 2018
  </div>
  <ul>
    <li>
      Level 108, Stats-only missions
    </li>
  </ul>

  <div class="subtitle">
    Jun 25, 2018
  </div>
  <ul>
    <li>
      Level 107
    </li>
  </ul>

  <div class="subtitle">
    Jun 24, 2018
  </div>
  <ul>
    <li>
      Zombies!!!
    </li>
  </ul>

  <div class="subtitle">
    Jun 23, 2018
  </div>
  <ul>
    <li>
      New aide, Florist, can check for planting
    </li>
    <li>
      Extended Father's Day event, Level 106
    </li>
    <li>
      New players can see homepage at any level
    </li>
    <li>
      Fundraising missions are complete
    </li>
  </ul>

  <div class="subtitle">
    Jun 21, 2018
  </div>
  <ul>
    <li>
      Link to Wiki page from Help screen, Level 105
    </li>
  </ul>

  <div class="subtitle">
    Jun 20, 2018
  </div>
  <ul>
    <li>
      Level 104
    </li>
  </ul>

  <div class="subtitle">
    Jun 19, 2018
  </div>
  <ul>
    <li>
      New party seats: Welcome Committee Member and Welcome Committee Chair
      get notifications of all new users who start the game
    </li>
    <li>
      Level 103
    </li>
    <li>
      Meet Someone New now boosts target influence by 20
    </li>
  </ul>

  <div class="subtitle">
    Jun 18, 2018
  </div>
  <ul>
    <li>
      Level 102, Father's Day event extended another day
    </li>
  </ul>

  <div class="subtitle">
    Jun 16, 2018
  </div>
  <ul>
    <li>
      Father's Day Missions
    </li>
    <li>
      Level 101
    </li>
  </ul>

  <div class="subtitle">
    Jun 15, 2018
  </div>
  <ul>
    <li>
      When leveling up and when getting your daily bonus, competencies can be
      enhanced every
      fifteen seconds for a few minutes
    </li>
    <li>
      Level 100
    </li>
  </ul>

  <div class="subtitle">
    Jun 14, 2018
  </div>
  <ul>
    <li>
      Admin accounts don't count in election challenges nor show up in PNG or Roll Call lists
    </li>
    <li>
      Location on the Home Page is underlined in the color of the Alderman's party
    </li>
    <li>
      Game change: When an Alderman loses his/her seat, only neighborhood officials
      lose their seats &mdash; party, district, and city officials will still retain their
      seats
    </li>
    <li>
      Flag Day Event
    </li>
    <li>
      Level 99
    </li>
  </ul>

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
