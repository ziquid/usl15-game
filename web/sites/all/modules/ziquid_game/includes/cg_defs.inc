<?php

/**
 * @file
 * Celestial Glory game definitions.
 *
 * Synced with CG: duh
 * Synced with 2114: no
 * .
 */

include __DIR__ . '/game_defs.inc';

//$debate_wait_time = 900; // 1200
//$zombie_debate_wait = 180; // 300
//$event_type = [];
//$month_mission = MISSION_NONE;
//$energy_wait = 300;
//$energy_wait_str = '5 minutes';
//$competency_gain_wait_time = 180;
//$competency_gain_wait_time_str = '3 minutes';

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

  case '2020-01-28':
    $event_type[] = 'event_jan_28_2020';
    break;

  case '2020-01-29':
    $event_type[] = 'event_jan_29_2020';
    break;

  /* ------ FEBRUARY ------ */
  case '2020-02-01':
    $event_type[] = EVENT_GROUNDHOG_DAY;
    break;

  case '2020-02-02':
    $event_type[] = EVENT_SUPPERBOWL;
    $event_type[] = EVENT_GROUNDHOG_DAY;
    break;

  case '2020-02-03':
  case '2020-02-04':
  case '2020-02-05':
  case '2020-02-06':
  case '2020-02-07':
  case '2020-02-08':
  case '2020-02-09':
  case '2020-02-10':
    $event_type[] = EVENT_GROUNDHOG_DAY;
    break;

  case '2020-02-12':
  case '2020-02-13':
  case '2020-02-14':
  case '2020-02-15':
  case '2020-02-16':
  case '2020-02-17':
    $event_type[] = EVENT_DEAD_PRESIDENTS;
    break;

  /* ------ MARCH ------ */
  case '2013-03-08':
    $event_type = EVENT_DEBATE;
    $debate_wait_time = 600; // 900; // 1200;
    break;

  case '2013-03-09':
    $event_type = EVENT_DONE;
    break;

  case '2020-03-16':
  case '2020-03-17':
    $event_type[] = EVENT_ST_PATTYS;
    break;

  case '2018-03-17':
    $event_type = EVENT_ST_PATTYS_MAP;
    break;

  case '2018-03-18':
    $event_type = EVENT_NEW_NAMES;
    break;

  /* ------ APRIL ------ */
  case '2018-04-01':
    $event_type = EVENT_APRIL_FOOLS;
    break;

  case '2020-04-06':
  case '2020-04-07':
  case '2020-04-08':
  case '2020-04-09':
  case '2020-04-10':
    $event_type[] = EVENT_ONE_MINUTE_COMPS;
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

  case '2020-05-08':
  case '2020-05-09':
  case '2020-05-10':
  case '2020-05-11':
  case '2020-05-12':
  case '2020-05-13':
  case '2020-05-14':
  case '2020-05-15':
  case '2020-05-16':
  case '2020-05-17':
  case '2020-05-18':
    $event_type[] = EVENT_MOTHERS_DAY;
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

  case '2020-06-01':
  case '2020-06-02':
  case '2020-06-03':
    $event_type[] = EVENT_DOUBLE_LUCK_ACTION;
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

  case '2020-07-04':
    $event_type[] = EVENT_INDEPENDENCE_DAY;
    break;

  case '2018-07-10':
  case '2018-07-11':
    $event_type = EVENT_ROT13_COMPS;
    break;

  /* ------ SEPTEMBER ------ */
  case '2020-09-07':
  case '2020-09-08':
  case '2020-09-09':
  case '2020-09-10':
    $event_type[] = EVENT_LABOR_DAY;
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
/*
$date = date('Y-m-d');

if (($date >= '2016-03-30') && ($date <= '2016-03-31'))
  $event_type = EVENT_PRE_APR1;

if ($date == '2016-04-01') $event_type = EVENT_QUESTS_100;

if (($date >= '2016-04-02') && ($date <= '2016-04-05'))
  $event_type = EVENT_PRE_MERCH_QUESTS;

if (($date >= '2016-04-06') && ($date <= '2016-04-14'))
  $event_type = EVENT_MERCH_QUESTS;

if (($date >= '2016-04-15') && ($date <= '2016-04-18'))
  $event_type = EVENT_GATHER_AMETHYST;

if ($date == '2016-04-19') $event_type = EVENT_AMETHYST_DONE;

if (($date >= '2016-04-29') && ($date <= '2016-05-02')) {
  $event_type = EVENT_SPEED_ENERGY;
  $energy_wait = 60;
  $energy_wait_str = '1 minute';
}

if (($date >= '2016-05-06') && ($date <= '2016-05-09')) {
  $event_type = EVENT_MOTHERS_DAY;
}

if (($date >= '2016-10-21') && ($date <= '2016-10-25')) {
  $event_type = EVENT_DEBATEBOT_ANNOUNCEMENT;
}

if (($date >= '2016-10-26') && ($date <= '2016-10-31')) {
  $event_type = EVENT_ZOMBIE_ANNOUNCEMENT;
}

if (($date >= '2017-03-11') && ($date <= '2017-03-17')) {
  $event_type = EVENT_CRAFTING_ANNOUNCEMENT;
}

if (($date >= '2017-03-22') && ($date <= '2017-03-31')) {
  $event_type = EVENT_SPRING_QUESTS;
}

if (date('m-d') == '04-01') {
  $event_type = EVENT_APRIL_FOOLS;
}

if (($date >= '2017-04-02') && ($date <= '2017-04-07')) {
  $event_type = EVENT_SPRING_QUESTS;
}
*/

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

  case '2020-01-28':
    $event_type[] = 'event_jan_28_2020';
    break;

  case '2020-01-29':
    $event_type[] = 'event_jan_29_2020';
    break;

  /* ------ FEBRUARY ------ */
  case '2020-02-01':
    $event_type[] = EVENT_GROUNDHOG_DAY;
    break;

  case '2020-02-02':
    $event_type[] = EVENT_SUPPERBOWL;
    $event_type[] = EVENT_GROUNDHOG_DAY;
    break;

  case '2020-02-03':
  case '2020-02-04':
  case '2020-02-05':
  case '2020-02-06':
  case '2020-02-07':
  case '2020-02-08':
  case '2020-02-09':
  case '2020-02-10':
    $event_type[] = EVENT_GROUNDHOG_DAY;
    break;

  case '2020-02-12':
  case '2020-02-13':
  case '2020-02-14':
  case '2020-02-15':
  case '2020-02-16':
  case '2020-02-17':
    $event_type[] = EVENT_DEAD_PRESIDENTS;
    break;

  /* ------ MARCH ------ */
  case '2013-03-08':
    $event_type = EVENT_DEBATE;
    $debate_wait_time = 600; // 900; // 1200;
    break;

  case '2013-03-09':
    $event_type = EVENT_DONE;
    break;

  case '2020-03-16':
  case '2020-03-17':
    $event_type[] = EVENT_ST_PATTYS;
    break;

  case '2018-03-17':
    $event_type = EVENT_ST_PATTYS_MAP;
    break;

  case '2018-03-18':
    $event_type = EVENT_NEW_NAMES;
    break;

  /* ------ APRIL ------ */
  case '2018-04-01':
    $event_type = EVENT_APRIL_FOOLS;
    break;

  case '2020-04-06':
  case '2020-04-07':
  case '2020-04-08':
  case '2020-04-09':
  case '2020-04-10':
    $event_type[] = EVENT_ONE_MINUTE_COMPS;
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

  case '2020-05-08':
  case '2020-05-09':
  case '2020-05-10':
  case '2020-05-11':
  case '2020-05-12':
  case '2020-05-13':
  case '2020-05-14':
  case '2020-05-15':
  case '2020-05-16':
  case '2020-05-17':
  case '2020-05-18':
    $event_type[] = EVENT_MOTHERS_DAY;
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

  case '2020-06-01':
  case '2020-06-02':
  case '2020-06-03':
    $event_type[] = EVENT_DOUBLE_LUCK_ACTION;
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

  case '2020-07-04':
    $event_type[] = EVENT_INDEPENDENCE_DAY;
    break;

  case '2018-07-10':
  case '2018-07-11':
    $event_type = EVENT_ROT13_COMPS;
    break;

  /* ------ SEPTEMBER ------ */
  case '2020-09-07':
  case '2020-09-08':
  case '2020-09-09':
  case '2020-09-10':
    $event_type[] = EVENT_LABOR_DAY;
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
  case '2020-11-20':
  case '2020-11-21':
  case '2020-11-22':
  case '2020-11-23':
  case '2020-11-24':
  case '2020-11-25':
  case '2020-11-26':
  case '2020-11-27':
  case '2020-11-28':
  case '2020-11-29':
  case '2018-11-30':
    $event_type[] = EVENT_THANKSGIVING;
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
}

$alderman = 'Chief Priest';
$aldermen = 'Chief Priests';
$alder_short = 'CP';
$alders_short = 'CPs';
$beauty = 'Piety';
$city = 'Land';
$clan = 'Clan';
$competencies = 'Competencies';
$debate = 'Challenge';
$debate_tab = 'Challenges';
$elders = 'Elders';
$election = 'Challenge';
$election_tab = 'Offices';
$elocution = 'Cunning';
$endurance = 'Defense';
$equipment = 'Supplies for Use';
$experience = 'Spirituality';
$experience_short = 'Spir';
$game_name_full = 'Celestial Glory';
$hood = 'Region';
$initiative = 'Attack';
$land = 'Goods for Trade or Barter';
$land_plural = 'Goods for Trade or Barter';
$luck = 'Luck';
$party = 'Family';
$party_small = 'Family';
$politics = 'Heritage';
$quest = 'Quest';
$residence = 'Residence';
$system = 'System';
$top20 = 'Top 20';

$game_settings = [
  'competencies' => [],
];

$game_text = array_merge($game_text, [
  'action' => 'Action',
  'actions' => 'Actions',
  'actions_short' => 'Acts',
  'agents' => 'Agents',
  'clan_eq' => 'Clan Equipment',
  'clan_eq_short' => 'Clan Eqp',
  'elders' => 'Elders',
  'elocution' => 'Cunning',
  'elocution_short' => 'Cun',
  'endurance' => 'Defense',
  'endurance_short' => 'Def',
  'energy' => 'Energy',
  'experience' => 'Spirituality',
  'experience_short' => 'Spir',
  'extra defending votes' => 'Extra Def Votes',
  'extra votes' => 'Extra Votes',
  'hood' => 'Region',
  'hood_lower' => 'region',
  'hood_short' => 'region',
  'hood_values' => [
    'beauty' => 'BEA',
    'chaos' => 'CHA',
    'faith' => 'FAI',
    'finance' => 'FIN',
    'health' => 'HEA',
    'intelligence' => 'INT',
    'strength' => 'STR',
  ],
  'initiative' => 'Attack',
  'initiative_short' => 'Att',
  'investment' => 'Investment',
  'investment_plural' => 'Investments',
  'job' => 'Job',
  'job_plural' => 'Jobs',
  'land' => 'Goods',
  'mayor' => 'Prophet',
  'mayor_tab' => 'System',
  'menu' => [
    'actions' => 'Actions',
    'aides' => 'Aides',
    'clan' => 'Clan',
    'debates' => 'Trials',
    'elders' => 'Elders',
    'elections' => 'Challenge',
    'forum' => 'Forum',
    'help' => 'Help',
    'missions' => 'Quests',
    'move' => 'Move',
    'profile' => 'Profile',
  ],
  'money' => 'Money',
  'qg_quests' => 'Quests',
  'quest' => 'Quest',
  'quest_lower' => 'quest',
  'staff' => 'Staff',
  'staff_tab' => 'Staff/Agents',
]);

if ($event_type == EVENT_NEW_NAMES) {
  $game_settings['competencies']['uncouth'] = 'joelene';
  $game_settings['competencies']['talkative'] = 'cunning linguist';
}

// Boxing day.
if ((date('m-d') == '12-26')) {
  $debate = 'Box';
  $debate_tab = 'Boxing';
  $top20 = 'Top Boxers';
  $game_text['menu']['debates'] = 'Boxing';
}

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

$slack_channel = '#cg';

// Add events to include list.
foreach ($event_type as $event) {
  zg_includes_add($event, 'event');
}
