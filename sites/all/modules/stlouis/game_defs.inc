<?php

  define(EVENT_NONE, 0);
  define(EVENT_DONE, 1);
  define(EVENT_DEBATE, 2);

  $debate_time = 1200;
  $zombie_debate_wait = 180; // 300
  $event_type = EVENT_NONE;

  global $game;

  if ($game == 'stlouis') {

    switch(date('Y-m-d', time() - 3600)) {

      case '2013-03-08':
        $event_type = EVENT_DEBATE;
        $debate_time = 600; // 900; // 1200;
        break;

      case '2013-03-09':
        $event_type = EVENT_DONE;
        break;

    }

    $alderman = 'Alderman';
    $aldermen = 'Aldermen';
    $alder_short = 'Alder';
    $alders_short = 'Alders';
    $beauty = 'Beauty';
    $city = 'City';
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
    $land = 'Business';
    $land_plural = 'Businesses';
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


  }

  if ($game == 'celestial_glory') {

    $alderman = 'Chief Priest';
    $aldermen = 'Chief Priests';
    $alder_short = 'CP';
    $alders_short = 'CPs';
    $beauty = 'Piety';
    $city = 'Land';
    $debate = 'Challenge';
    $debate_tab = 'Challenges';
    $elders = 'Elders';
    $election = 'Challenge';
    $election_tab = 'Trial';
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

  }

  if ($game == 'robber_barons') {

    $beauty = 'Piety';
    $debate = 'Debate';
    $elders = 'Elders';
    $election = 'Election';
    $election_tab = 'Election';
    $elocution = 'Elocution';
    $equipment = 'Equipment';
    $experience = 'Influence';
    $experience_short = 'Inf';
    $game_name_full = 'Robber Barons: The Gilded Age';
    $hood = 'Neighborhood';
    $initiative = 'Initiative';
    $land = 'Business';
    $land_plural = 'Businesses';
    $luck = 'Luck';
    $party = 'Political Party';
    $party_small = 'Party';
    $politics = 'Politics';
    $quest = 'Task';
    $residence = 'Residence';
    $system = 'System';

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

