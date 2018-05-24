<?php

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
define(EVENT_MOVE_3, 'event_move_3');
define(EVENT_PRE_MEMORIAL_DAY, 'event_pre_memorial_day');
define(EVENT_MEMORIAL_DAY_1, 'event_memorial_day_1');
define(EVENT_MEMORIAL_DAY_2, 'event_memorial_day_2');
define(EVENT_MEMORIAL_DAY_3, 'event_memorial_day_3');
define(EVENT_MEMORIAL_DAY_4, 'event_memorial_day_4');

define(MISSION_NONE, 1000);
define(MISSION_MAY, 1005);
define(MISSION_JUN, 1006);

$debate_time = 900; // 1200
$zombie_debate_wait = 180; // 300
$event_type = EVENT_NONE;
$month_mission = MISSION_NONE;
$energy_wait = 300;
$energy_wait_str = '5 minutes';

global $game;

$today = date('Y-m-d', time() - 3600);
switch($today) {

  case '2013-03-08':
    $event_type = EVENT_DEBATE;
    $debate_time = 600; // 900; // 1200;
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

  case '2018-04-01':
    $event_type = EVENT_APRIL_FOOLS;
    break;

  case '2018-04-30':
    $event_type = EVENT_PRE_MAY;
    break;

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
  'investment' => 'Investment',
  'investment_plural' => 'Investments',
  'job' => 'Job',
  'job_plural' => 'Jobs',
  'staff' => 'Staff',
];

if ($event_type == EVENT_NEW_NAMES) {
  $game_settings['competencies']['uncouth'] = 'joelene';
  $game_settings['competencies']['talkative'] = 'cunning linguist';
}

/*
// boxing day
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
$game = check_plain(arg(0));
$arg2 = check_plain(arg(2));
$arg3 = check_plain(arg(3));
$fetch_user = '_' . $game . '_fetch_user';
$fetch_header = '_' . $game . '_header';
