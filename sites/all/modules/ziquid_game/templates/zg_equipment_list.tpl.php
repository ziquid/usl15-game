<?php

/**
 * @file
 * Game equipment list.
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 * Ready for MVC separation: done
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: no
 * .
 */

global $game, $phone_id;

/* ------ CONTROLLER ------ */

include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
$game_user = zg_fetch_player();
$ai_output = 'equipment-prices';

// Fix expenses in case they are out of whack.
zg_recalc_income($game_user);
$data = zg_fetch_visible_equip($game_user);
$next = zg_fetch_next_equip($game_user);

$output = [];
foreach ($data as $item) {
  $output[] = zg_render_equip($game_user, $item, $ai_output);
}

// Show next one.
if (!empty($next)) {
  $output[] = zg_render_equip($game_user, $next, $ai_output, ['soon' => TRUE]);
}

/* ------ VIEW ------ */

zg_fetch_header($game_user);
zg_show_aides_menu($game_user);
db_set_active();
zg_show_ai_output($phone_id, $ai_output);
?>

<?php if ($game_user->level < 15): ?>
  <ul>
    <li>Purchase <?php print $equipment_lower; ?> to help you and your aides</li>
  </ul>
<?php endif; ?>

<div id="all-equip">
  <?php foreach ($output as $out): ?>
    <?php print $out; ?>
  <?php endforeach; ?>
</div>
