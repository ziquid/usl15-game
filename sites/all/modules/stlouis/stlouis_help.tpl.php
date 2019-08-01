<?php

/**
 * @file stlouis_help.tpl.php
 * Help screen.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);

echo <<< EOF
<div class="news">
  <button class="active">Help</button>
  <a href="external://uprising-st-louis.wikia.com/wiki/Uprising_St._Louis_Wiki" class="button">Wiki</a>
</div>
<div class="help">
<div class="title">
Help / FAQ
</div>

<div class="subtitle">
Terms of Service
</div>
<p>Since this product includes user-generated content (ie, you can choose
your own usernames and send each other messages and post forum messages),
we need to tell you that we moderate these for appropriateness.&nbsp;
We have a filter in place for profanity on user messages; you can block
users from sending you messages at all; and you can post forum messages
or send e-mail to us at zipport@ziquid.com if you have complaints or
need assistance.&nbsp; We occasionally lock or delete harassing forum
threads and delete user accounts for abuse.</p>

<div class="subtitle">
Goal of the Game
</div>
<p>Your goal is to build your character to as high a level as you can.&nbsp;
Gain experience by completing {$quest_lower}s and participating in
{$debate_lower}s.</p>

<div class="subtitle">
{$game_text['quest']}s
</div>
<p>{$game_text['quest']}s help you become more experienced, both in $game_user->values
and in $experience.</p>

<div class="subtitle">
{$debate}s
</div>
<p>{$debate}s match your wits against other players across the game.&nbsp; Those
players with the highest $experience and $elocution win the most {$debate_lower}s.</p>

<div class="subtitle">
Elections
</div>
<p>Once you become influential enough you can participate in elections to
gain even more experience.&nbsp; If you become elected to a position, you
get in-game bonuses.&nbsp; You also get bonuses if you belong to the
same political party as the elected officials in your neighborhood.</p>

<div class="subtitle">
Aides
</div>
<p>Aides are non-playing characters and other items that help you attain your
goals.&nbsp; They could be {$land_plural_lower} who generate income for you or people
who allow you to perform more actions.</p>

<div class="subtitle">
Staff
</div>
<p>Staff are aides who help you in public, legitimate ways.&nbsp; They may
provide $initiative_lower or endurance bonuses or make new actions available.</p>

<div class="subtitle">
Agents
</div>
<p>Agents are aides who help you in covert, clandestine, or illegal ways.&nbsp;
They also may provide $initiative_lower or endurance bonuses or make new actions
available.</p>

<div class="subtitle">
Values
</div>
<p>Values, such as <em>$game_user->values</em>, are shown in the top-left corner
of the screen.&nbsp; Use them to win and purchase Aides whom will help you
throughout the game.</p>

<div class="subtitle">
$experience
</div>
<p>$experience is a measure of how popular you are.&nbsp; The higher your
$experience_lower, the better chance you have of winning elections, whether you
are the challenger or the incumbent.</p>

<div class="subtitle">
$initiative
</div>
<p>$initiative is a measure of how well you conquer new projects.&nbsp; The
higher your $initiative, the better chance you have of winning elections
when you are the challenger.</p>

<div class="subtitle">
$endurance
</div>
<p>$endurance is a measure of how well you complete existing projects.&nbsp; The
higher your $endurance, the better chance you have of winning elections
when you are the incumbent.</p>

<div class="subtitle">
$elocution
</div>
<p>$elocution is a measure of your public speaking ability.&nbsp; The higher your
$elocution, the better chance you have of winning {$debate_lower}s, whether you
or another player initiates them.</p>
</div>

EOF;
db_set_active('default');
