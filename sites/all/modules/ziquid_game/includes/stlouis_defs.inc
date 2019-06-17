<?php

/**
 * @file
 * StLouis game definitions.
 *
 * Synced with CG: no
 * Synced with 2114: no
 */

define(EVENT_NONE, 0);
define(EVENT_DONE, 1);
define(EVENT_DEBATE, 2);
define(EVENT_ST_PATTYS, 3);
define(EVENT_APRIL_FOOLS, 4);
define(EVENT_NEW_NAMES, 5);
define(EVENT_PRE_MAY, 6);
define(EVENT_CINCO_DE_MAYO, 7);
define(EVENT_PRE_DOUBLE_LUCK_MONEY, 8);
define(EVENT_DOUBLE_LUCK_MONEY, 9);

define(EVENT_CHRISTMAS, 'event_christmas');
define(EVENT_DOUBLE_LUCK_ENERGY, 'event_double_luck_energy');

define(EVENT_FAST_COMPETENCIES, 'event_fast_competencies');
define(EVENT_FLAG_DAY, 'event_flag_day');

define(EVENT_HALLOWEEN, 'event_halloween');

define(EVENT_PRE_MEMORIAL_DAY, 'event_pre_memorial_day');
define(EVENT_MEMORIAL_DAY_1, 'event_memorial_day_1');
define(EVENT_MEMORIAL_DAY_2, 'event_memorial_day_2');
define(EVENT_MEMORIAL_DAY_3, 'event_memorial_day_3');
define(EVENT_MEMORIAL_DAY_4, 'event_memorial_day_4');
define(EVENT_MOVE_3, 'event_move_3');
define(EVENT_NYE_PARTY, 'event_nye_party');

define(EVENT_POLL_JUN_01_2018, 'event_poll_jun_01_2018');
define(EVENT_ROT13_COMPS, 'event_rot13_comps');
define(EVENT_STATS_ONLY_HOOD, 'event_stats_only_hood');
define(EVENT_THANKSGIVING, 'event_thanksgiving');

define(MISSION_NONE, 1000);
define(MISSION_MAY, 1005);
define(MISSION_JUN, 1006);
define(MISSION_JUL, 1007);
define(MISSION_OCT, 1010);

$debate_wait_time = 900; // 1200
$zombie_debate_wait = 180; // 300
$event_type = [];
$month_mission = MISSION_NONE;
$energy_wait = 300;
$energy_wait_str = '5 minutes';
$competency_gain_wait_time = 180;
$competency_gain_wait_time_str = '3 minutes';

global $game;

$today = date('Y-m-d', REQUEST_TIME - 3600);
switch ($today) {

  /* ------ JANUARY ------ */
  case '2019-01-01':
  case '2019-01-02':
  case '2019-01-03':
  case '2019-01-04':
  case '2019-01-05':
  case '2019-01-06':
    $event_type[] = EVENT_CHRISTMAS;
    break;

  /* ------ MARCH ------ */
  case '2013-03-08':
    $event_type = EVENT_DEBATE;
    $debate_wait_time = 600; // 900; // 1200;
    break;

  case '2013-03-09':
    $event_type = EVENT_DONE;
    break;

  case '2018-03-17':
    $event_type = EVENT_ST_PATTYS;
    break;

  case '2018-03-18':
    $event_type = EVENT_NEW_NAMES;
    break;

  /* ------ APRIL ------ */
  case '2018-04-01':
    $event_type = EVENT_APRIL_FOOLS;
    break;

  case '2018-04-30':
    $event_type = EVENT_PRE_MAY;
    break;

  /* ------ MAY ----- */
  case '2018-05-01':
  case '2018-05-02':
  case '2018-05-03':
  case '2018-05-04':
    $month_mission = MISSION_MAY;
    break;

  case '2018-05-05':
  case '2018-05-06':
    $month_mission = MISSION_MAY;
    $event_type = EVENT_CINCO_DE_MAYO;
    break;

  case '2018-05-07':
  case '2018-05-08':
  case '2018-05-09':
  case '2018-05-10':
  case '2018-05-11':
  case '2018-05-12':
  case '2018-05-13':
    $month_mission = MISSION_MAY;
    break;

  case '2018-05-14':
  case '2018-05-15':
    $event_type = EVENT_DOUBLE_LUCK_MONEY;
    break;

  case '2018-05-18':
  case '2018-05-19':
  case '2018-05-20':
    $event_type = EVENT_CINCO_DE_MAYO;
    break;

  case '2018-05-22':
    $event_type = EVENT_MOVE_3;
    break;

  case '2018-05-24':
    $event_type = EVENT_PRE_MEMORIAL_DAY;
    break;

  case '2018-05-25':
    $event_type = EVENT_MEMORIAL_DAY_1;
    break;

  case '2018-05-26':
    $event_type = EVENT_MEMORIAL_DAY_2;
    break;

  case '2018-05-27':
    $event_type = EVENT_MEMORIAL_DAY_3;
    break;

  case '2018-05-28':
    $event_type = EVENT_MEMORIAL_DAY_4;
    break;

  /* ------ JUNE ------ */
  case '2018-06-01':
  case '2018-06-02':
  case '2018-06-03':
    $event_type = EVENT_POLL_JUN_01_2018;
    break;

  case '2018-06-12':
    $event_type = EVENT_FAST_COMPETENCIES;
    $competency_gain_wait_time = 30;
    $competency_gain_wait_time_str = '30 seconds';
    break;

  case '2018-06-14':
    $event_type = EVENT_FLAG_DAY;
    break;

  case '2018-06-16':
  case '2018-06-17':
  case '2018-06-18':
  case '2018-06-19':
  case '2018-06-23':
  case '2018-06-24':
    $month_mission = MISSION_JUN;
    break;

  case '2018-06-30':
    $event_type = EVENT_STATS_ONLY_HOOD;
    break;

  /* ------ JULY ------ */
  case '2018-07-01':
  case '2018-07-02':
  case '2018-07-03':
  case '2018-07-04':
    $event_type = EVENT_STATS_ONLY_HOOD;
    break;

  case '2018-07-10':
  case '2018-07-11':
    $event_type = EVENT_ROT13_COMPS;
    break;

  /* ------ OCTOBER ------ */
  case '2018-10-23':
  case '2018-10-24':
  case '2018-10-25':
  case '2018-10-26':
  case '2018-10-27':
  case '2018-10-28':
  case '2018-10-29':
  case '2018-10-30':
  case '2018-10-31':
    $event_type = EVENT_HALLOWEEN;
    break;

  /* ------ NOVEMBER ------ */
  case '2018-11-21':
  case '2018-11-22':
  case '2018-11-23':
  case '2018-11-24':
  case '2018-11-25':
  case '2018-11-26':
  case '2018-11-27':
  case '2018-11-28':
  case '2018-11-29':
  case '2018-11-30':
    $event_type = EVENT_THANKSGIVING;
    break;

  /* ------ DECEMBER ------ */
  case '2018-12-12':
  case '2018-12-13':
  case '2018-12-14':
  case '2018-12-15':
  case '2018-12-16':
  case '2018-12-17':
    $event_type = EVENT_DOUBLE_LUCK_ENERGY;
    break;

  case '2018-12-22':
  case '2018-12-23':
  case '2018-12-24':
  case '2018-12-25':
  case '2018-12-26':
  case '2018-12-27':
  case '2018-12-28':
  case '2018-12-29':
  case '2018-12-30':
  case '2018-12-31':
    $event_type[] = EVENT_CHRISTMAS;
    break;

  case '2019-06-16':
  case '2019-06-17':
  case '2019-06-18':
  case '2019-06-19':
  case '2019-06-23':
  case '2019-06-24':
//    $month_mission = MISSION_JUN;
    break;
}

$alderman = 'Alderman';
$aldermen = 'Aldermen';
$alder_short = 'Alder';
$alders_short = 'Alders';
$beauty = 'Beauty';
$city = 'City';
$clan = 'Clan';
$competencies = 'Competencies';
$debate = 'Debate';
$debate_tab = 'Debates';
$elders = 'Elders';
$election = 'Election';
$election_tab = 'Elections';
$elocution = 'Elocution';
$endurance = 'Endurance';
$equipment = 'Equipment';
$experience = 'Influence';
$experience_short = 'Inf';
$game_name_full = 'Uprising: St. Louis';
$hood = 'Neighborhood';
$initiative = 'Initiative';
$land = 'Income';
$land_plural = 'Income';
$luck = 'Luck';
$party = 'Political Party';
$party_small = 'Party';
$politics = 'Politics';
$quest = 'Mission';
$residence = 'Residence';
$system = 'System';
$top20 = 'Top 20';

$game_settings = [
  'competencies' => [],
];

$game_text = [
  'action' => 'Action',
  'actions' => 'Actions',
  'actions_short' => 'Acts',
  'agents' => 'Agents',
  'clan_eq' => 'Clan Equipment',
  'clan_eq_short' => 'Clan Eqp',
  'elders' => 'Elders',
  'elocution' => 'Elocution',
  'elocution_short' => 'Elo',
  'endurance' => 'Endurance',
  'endurance_short' => 'End',
  'energy' => 'Energy',
  'experience' => 'Influence',
  'experience_short' => 'Inf',
  'hood' => 'Neighborhood',
  'hood_lower' => 'neighborhood',
  'hood_short' => '\'hood',
  'hood_values' => [
    'beauty' => 'BEAU',
    'entertainment' => 'ENTR',
    'financial' => 'FNCE',
    'health' => 'HLTH',
    'intelligence' => 'INTL',
    'strength' => 'STRG',
  ],
  'initiative' => 'Initiative',
  'initiative_short' => 'Ini',
  'investment' => 'Investment',
  'investment_plural' => 'Investments',
  'job' => 'Job',
  'job_plural' => 'Jobs',
  'land' => 'Income',
  'mayor' => 'Mayor',
  'mayor_tab' => 'Mayor',
  'menu' => [
    'actions' => 'Actions',
    'aides' => 'Aides',
    'clan' => 'Clan',
    'debates' => 'Debates',
    'elders' => 'Elders',
    'elections' => 'Elections',
    'forum' => 'Forum',
    'help' => 'Help',
    'missions' => 'Missions',
    'move' => 'Move',
    'profile' => 'Profile',
  ],
  'qg_quests' => 'Missions',
  'quest' => 'Mission',
  'quest_lower' => 'mission',
  'staff' => 'Staff',
  'staff_tab' => 'Staff/Agents',
];

if ($event_type == EVENT_NEW_NAMES) {
  $game_settings['competencies']['uncouth'] = 'joelene';
  $game_settings['competencies']['talkative'] = 'cunning linguist';
}

/*
// Boxing day.
if ((date('Y-m-d') == '2012-12-26') || arg(2) == 'abc123') {
  $debate = 'Box';
  $debate_tab = 'Boxing';
  $top20 = 'Top Boxers';
}
*/

$beauty_lower = strtolower($beauty);
$city_lower = strtolower($city);
$debate_lower = strtolower($debate);
$election_lower = strtolower($election);
$election_tab_lower = strtolower($election_tab);
$elocution_lower = strtolower($elocution);
$equipment_lower = strtolower($equipment);
$experience_lower = strtolower($experience);
$experience_short_lower = strtolower($experience_short);
$hood_lower = strtolower($hood);
$initiative_lower = strtolower($initiative);
$land_lower = strtolower($land);
$land_plural_lower = strtolower($land_plural);
$party_lower = strtolower($party);
$party_small_lower = strtolower($party_small);
$politics_lower = strtolower($politics);
$quest_lower = strtolower($quest);
$residence_lower = strtolower($residence);
$system_lower = strtolower($system);

// Function-wide defs.
if (arg(0) != 'node' && arg(0) != 'devel' && arg(0) != 'admin') {
  $game = check_plain(arg(0));
}
$arg2 = check_plain(arg(2));
$arg3 = check_plain(arg(3));
$fetch_user = 'zg_fetch_user';
$fetch_header = 'zg_fetch_header';
$slack_channel = '#uslce-data';

// Add events to include list.
foreach ($event_type as $event) {
  zg_includes_add($event, 'event');
}
