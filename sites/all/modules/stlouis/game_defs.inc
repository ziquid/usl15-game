<?php

define(EVENT_NONE, 0);
define(EVENT_DONE, 1);
define(EVENT_DEBATE, 2);
define(EVENT_ST_PATTYS, 3);

$debate_time = 900; // 1200
$zombie_debate_wait = 180; // 300
$event_type = EVENT_NONE;
$energy_wait = 300;
$energy_wait_str = '5 minutes';

global $game;

switch(date('Y-m-d', time() - 3600)) {

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

}

$alderman = 'Alderman';
$aldermen = 'Aldermen';
$alder_short = 'Alder';
$alders_short = 'Alders';
$beauty = 'Beauty';
$city = 'City';
$competencies = 'Competencies';
$debate = 'Debate';
$debate_tab = 'Debates';
$elders = 'Elders';
$election = 'Election';
$election_tab = 'Election';
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

