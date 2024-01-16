<?php

/**
 * @file
 * Game game definitions.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * .
 */

if (!defined('EVENT_NONE')) {
define("EVENT_NONE", 0);
define("EVENT_DONE", 1);
define("EVENT_DEBATE", 2);
define("EVENT_ST_PATTYS_MAP", 3);
define("EVENT_APRIL_FOOLS", 4);
define("EVENT_NEW_NAMES", 5);
define("EVENT_PRE_MAY", 6);
define("EVENT_CINCO_DE_MAYO", 7);
define("EVENT_PRE_DOUBLE_LUCK_MONEY", 8);
define("EVENT_DOUBLE_LUCK_MONEY", 9);

define("EVENT_CHRISTMAS", 'event_christmas');
define("EVENT_DEAD_PRESIDENTS", 'event_dead_presidents');
define("EVENT_DOUBLE_LUCK_ACTION", 'event_double_luck_action');
define("EVENT_DOUBLE_LUCK_ENERGY", 'event_double_luck_energy');

define("EVENT_FAST_COMPETENCIES", 'event_fast_competencies');
define("EVENT_FLAG_DAY", 'event_flag_day');
define("EVENT_GROUNDHOG_DAY", 'event_groundhog_day');
define("EVENT_HALLOWEEN", 'event_halloween');
define("EVENT_INDEPENDENCE_DAY", 'event_independence_day');
define("EVENT_LABOR_DAY", 'event_labor_day');

define("EVENT_PRE_MEMORIAL_DAY", 'event_pre_memorial_day');
define("EVENT_MEMORIAL_DAY_1", 'event_memorial_day_1');
define("EVENT_MEMORIAL_DAY_2", 'event_memorial_day_2');
define("EVENT_MEMORIAL_DAY_3", 'event_memorial_day_3');
define("EVENT_MEMORIAL_DAY_4", 'event_memorial_day_4');
define("EVENT_MOTHERS_DAY", 'event_mothers_day');
define("EVENT_MOVE_3", 'event_move_3');
define("EVENT_NYE_PARTY", 'event_nye_party');
define("EVENT_ONE_MINUTE_COMPS", 'event_one_minute_comps');
define("EVENT_POLL_JUN_01_2018", 'event_poll_jun_01_2018');
define("EVENT_ROT13_COMPS", 'event_rot13_comps');
define("EVENT_STATS_ONLY_HOOD", 'event_stats_only_hood');
define("EVENT_ST_PATTYS", 'event_st_pattys');
define("EVENT_SUPPERBOWL", 'event_supperbowl');
define("EVENT_THANKSGIVING", 'event_thanksgiving');

define("MISSION_NONE", 1000);
define("MISSION_MAY", 1005);
define("MISSION_JUN", 1006);
define("MISSION_JUL", 1007);
define("MISSION_OCT", 1010);
}

$debate_wait_time = 900; // 1200
$zombie_debate_wait = 180; // 300
$event_type = [];
$month_mission = MISSION_NONE;
$energy_wait = 300;
$energy_wait_str = '5 minutes';
$competency_gain_wait_time = 180;
$competency_gain_wait_time_str = '3 minutes';

global $game;

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
  'last_update' => 'Jan 16, 2024',
  'version' => 'Version:',
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
  'extra defending votes' => 'Extra Def Votes',
  'extra votes' => 'Extra Votes',
  'hood' => 'Neighborhood',
  'hood_lower' => 'neighborhood',
  'hood_short' => '\'hood',
  'hood_values' => [
    'beauty' => 'BEA',
    'chaos' => 'CHA',
    'faith' => 'FAI',
    'finance' => 'FIN',
    'health' => 'HEA',
    'intelligence' => 'INT',
    'strength' => 'STR',
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
  'money' => 'Money',
  'qg_quests' => 'Missions',
  'quest' => 'Mission',
  'quest_lower' => 'mission',
  'staff' => 'Staff',
  'staff_tab' => 'Staff/Agents',
];

// Function-wide defs.
if (arg(0) != 'node' && arg(0) != 'devel' && arg(0) != 'admin') {
  $game = check_plain(arg(0));

  // Pretend usl_esa is stlouis for now.
  if ($game == 'usl_esa') {
    $game = 'stlouis';
  }
}
$arg2 = check_plain(arg(2));
$arg3 = check_plain(arg(3));
$fetch_user = 'zg_fetch_player';
$fetch_header = 'zg_fetch_header';
$slack_channel = '#uslce';
