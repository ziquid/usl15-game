<?php

/**
 * @file
 * The game's changelog.
 *
 * Synced with CG: yes
 * Synced with 2114: N/A
 * Ready for phpcbf: done
 * Ready for MVC separation: yes
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_player();
zg_fetch_header($game_user);
db_set_active();

?>
<div class="news">
  <a href="/<?php echo $game; ?>/help/<?php echo $arg2; ?>" class="button">Help</a>
  <a href="/<?php echo $game; ?>/privacy/<?php echo $arg2; ?>" class="button">Privacy</a>
  <a href="externals://uprising-st-louis.wikia.com/wiki/Uprising_St._Louis_Wiki" class="button">Wiki</a>
  <a href="/<?php echo $game; ?>/changelog/<?php echo $arg2; ?>" class="button active">Changelog</a>
</div>

<div class="help">
  <div class="title">
    <?php print $game_name_full; ?> Changelog
  </div>

    <div class="subtitle">
        Jul 5, 2025
    </div>
    <ul>
        <li>
            Improved contrast by making the background darker.
        </li>
        <li>
            Fixed the debate button link on the User Profile page.
        </li>
    </ul>

    <div class="subtitle">
        Jul 1, 2025
    </div>
    <ul>
        <li>
            Updated the look and feel of the player debate list.
        </li>
        <li>
            Made a couple of buttons on the User Profile page more readable.
        </li>
    </ul>

    <div class="subtitle">
        Jun 10, 2025
    </div>
    <ul>
        <li>
            Finished moving all game assets behind an SSL connection.
        </li>
    </ul>

    <div class="subtitle">
        May 24, 2025
    </div>
    <ul>
        <li>
            iPhone 16 Pro Max did not display the game screen properly.  Fixed.
        </li>
    </ul>

    <div class="subtitle">
        Apr 27, 2025
    </div>
    <ul>
        <li>
            The map disappeared!  Fixed.
        </li>
        <li>
            Some players that should be zombies aren't becoming zombies.  Fixed.
        </li>
    </ul>

    <div class="subtitle">
        Mar 31, 2025
    </div>
    <ul>
        <li>
            Challenging for seats could result in a white screen.  Fixed.
        </li>
        <li>
            Trying to views clans before entering your name resulted in a green screen.  Fixed.
        </li>
        <li>
            Challenge results 'continue' button was almost unreadable.  Fixed.
        </li>
        <li>
            Challenge results in an unpopulated hood resulted in a white screen.  Fixed.
        </li>
    </ul>

    <div class="subtitle">
        Jan 18, 2024
    </div>
    <ul>
        <li>
            Some iPads showed fonts WAY TOO BIG.  Fixed.
        </li>
    </ul>

    <div class="subtitle">
        Jan 16, 2024
    </div>
    <ul>
        <li>
            USLCE didn't show the Elder during the welcome wizard.  Fixed.
        </li>
    </ul>

    <div class="subtitle">
        May 13, 2023
    </div>
    <ul>
        <li>
            More Talking Heads and more bugfixes.
        </li>
    </ul>

    <div class="subtitle">
        May 3, 2023
    </div>
    <ul>
        <li>
            More Elderly smiles and more bugfixes.
        </li>
    </ul>

  <div class="subtitle">
    Apr 30, 2023
  </div>
  <ul>
    <li>
      PHP 8.1 support.
    </li>
  </ul>

  <div class="subtitle">
    Jul 10, 2022
  </div>
  <ul>
    <li>
      Found some screens that were lost.  Debates, for one.
    </li>
  </ul>

  <div class="subtitle">
    Dec 29, 2020
  </div>
  <ul>
    <li>
      Is it snowing?
    </li>
  </ul>

  <div class="subtitle">
    Nov 20, 2020
  </div>
  <ul>
    <li>
      Jumping is now optional.
    </li>
  </ul>

  <div class="subtitle">
    Nov 1, 2020
  </div>
  <ul>
    <li>
      We needed more Faith, so I added some.
    </li>
  </ul>

  <div class="subtitle">
    Oct 31, 2020
  </div>
  <ul>
    <li>
      Added "GoldenPlatesFTW" cheat code.
    </li>
  </ul>

  <div class="subtitle">
    Jun 19, 2020
  </div>
  <ul>
    <li>
      Better theming for clan announcements.
    </li>
  </ul>


  <div class="subtitle">
    Jun 01, 2020
  </div>
  <ul>
    <li>
      Double Luck Action Event.
    </li>
  </ul>

  <div class="subtitle">
    May 23, 2020
  </div>
  <ul>
    <li>
      Added "Online Tutor" for "Raising the Grade" missions' second-round bonus.
    </li>
  </ul>

  <div class="subtitle">
    May 6, 2020
  </div>
  <ul>
    <li>
      Modernized the message look on the clan messages page
    </li>
  </ul>

  <div class="subtitle">
    Mar 25, 2020
  </div>
  <ul>
    <li>
      Emoji support 😁😁😁
    </li>
  </ul>

  <div class="subtitle">
    Mar 23, 2020
  </div>
  <ul>
    <li>
      New action: "Generate Fake News" increases your rating, the neighborhood
        beauty rating, and the new competency "Propaganda".  It requires a TV
        Anchor.
    </li>
  </ul>

  <div class="subtitle">
    Mar 9, 2020
  </div>
  <ul>
    <li>
      Added Ammunition count on main menu; temporarily removed all labels and
        counters except Beauty
    </li>
  </ul>

  <div class="subtitle">
    Mar 2, 2020
  </div>
  <ul>
    <li>
      Added badge in the header to show unread personal message count
    </li>
    <li>
      Elections live in Clayton/Tamm!
    </li>
  </ul>

  <div class="subtitle">
    Mar 1, 2020
  </div>
  <ul>
    <li>
      Recoded headers to break its layout less
    </li>
  </ul>

  <div class="subtitle">
    Feb 17, 2020
  </div>
  <ul>
    <li>
      Dead Presidents event
    </li>
  </ul>

  <div class="subtitle">
    Feb 02, 2020
  </div>
  <ul>
    <li>
      The SupperBowl!
    </li>
    <li>
      Groundhog Day missions
    </li>
  </ul>

  <div class="subtitle">
    Jan 29, 2020
  </div>
  <ul>
    <li>
      Added more music, 20% more money for Luck
    </li>
  </ul>

  <div class="subtitle">
    Dec 25, 2019
  </div>
  <ul>
    <li>
      Boxing Day is nearly upon us!
    </li>
  </ul>

  <div class="subtitle">
    Dec 12, 2019
  </div>
  <ul>
    <li>
      Added Privacy Policy and fixed the Help pages menu.
    </li>
  </ul>

  <div class="subtitle">
    Oct 13, 2019
  </div>
  <ul>
    <li>
      Enabled Media Mafia and United Workers Party.
    </li>
  </ul>

  <div class="subtitle">
    Sep 15, 2019
  </div>
  <ul>
    <li>
      Updated Elders screen.
    </li>
  </ul>

  <div class="subtitle">
    May 29, 2019
  </div>
  <ul>
    <li>
      Updated authenticate screen.
    </li>
  </ul>

  <div class="subtitle">
    Apr 21, 2019
  </div>
  <ul>
    <li>
      Updated choose name screen.
    </li>
  </ul>

  <div class="subtitle">
    Mar 31, 2019
  </div>
  <ul>
    <li>
      Updated home menu background image.
    </li>
  </ul>

  <div class="subtitle">
    Mar 01, 2019
  </div>
  <ul>
    <li>
      Better layout of debate / debate continue buttons.
    </li>
  </ul>

  <div class="subtitle">
    Feb 27, 2019
  </div>
  <ul>
    <li>
      Accounts wait 120 days without activity to become zombies.
    </li>
  </ul>

  <div class="subtitle">
    Jan 20, 2019
  </div>
  <ul>
    <li>
      You cannot challenge for a seat if the player who holds it is more than 15
        levels below you.
    </li>
    <li>
      Alderman positions no longer have an upper level limit.
    </li>
    <li>
      Elections are open now in West End.
    </li>
  </ul>

  <div class="subtitle">
    Dec 30, 2018
  </div>
  <ul>
    <li>
      Enabled code: WanderLust
    </li>
    <li>
      Moving to a new hood shows you the clan that controls the hood, if any.
    </li>
    <li>
      New equipment: Formal Dress
    </li>
  </ul>

  <div class="subtitle">
    Dec 28, 2018
  </div>
  <ul>
    <li>
      After moving for a mission, touching "Continue to Missions" will return
      you to that mission group, even if it was an event or special mission.
    </li>
    <li>
      Touching the "Missions" menu entry on the home page will also attempt to
        return you to the latest mission group, even if it was an event or a
        special mission.
    </li>
  </ul>

  <div class="subtitle">
    Dec 22, 2018
  </div>
  <ul>
    <li>
      Happy Hectic Holidays!
    </li>
  </ul>

  <div class="subtitle">
    Dec 12, 2018
  </div>
  <ul>
    <li>
      Double your Energy for one Luck, Dec 13-17!
    </li>
  </ul>

  <div class="subtitle">
    Dec 06, 2018
  </div>
  <ul>
    <li>
      Dog Walking quests
    </li>
    <li>
      Redid how the Socialite invites you to a party
    </li>
  </ul>

  <div class="subtitle">
    Oct 29, 2018
  </div>
  <ul>
    <li>
      Refilling Energy via Luck adds to your existing Energy, so that you can
      have more than your max Energy.
    </li>
  </ul>

  <div class="subtitle">
    Oct 24, 2018
  </div>
  <ul>
    <li>
      Dressing up for Halloween
    </li>
  </ul>

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
