<?php

/**
 * @file
 * Land functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 * Ready for phpcbf: no
 * .
 */

/**
 * Fetch the list of land to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass[]
 *   The array of land objects.
 */
function zg_fetch_visible_land(\stdClass $game_user) {
  if ($game_user->meta == 'admin') {
    $sql_to_add = 'WHERE land.id > 0';
  }
  else {
    $sql_to_add = 'WHERE (((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND

  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

    AND required_level <= %d
    AND active = 1
  )

  OR land_ownership.quantity > 0 ';
  }

  $data = [];
  $sql = 'SELECT land.*, land_ownership.quantity,
  competencies.name as competency, comp1.name as competency_name_1
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
    competencies.id
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

  ' . $sql_to_add . '
  ORDER BY fkey_enhanced_competencies_id, required_level ASC';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  while ($item = db_fetch_object($result)) {
    $item->competency = zg_competency_name($item->competency);
    $item->competency_name_1 = zg_competency_name($item->competency_name_1);
    $data[] = $item;
  }
  return $data;
}

/**
 * Fetch the next land item to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass
 *   The next land item to show, if any.
 */
function zg_fetch_next_land(\stdClass $game_user) {
  $sql = 'SELECT land.*, land_ownership.quantity
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  WHERE ((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND
  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

  AND required_level > %d
  AND active = 1
  ORDER BY required_level ASC LIMIT 1';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);
  return db_fetch_object($result);
}

/**
 * Fetch the specified land item.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $land_id
 *   The land id.
 *
 * @return \stdClass
 *   The land item, if any.
 */
function zg_fetch_land_by_id(\stdClass $game_user, $land_id) {
  $sql = 'SELECT land.*, land_ownership.quantity,
  competencies.name as competency, comp1.name as competency_name_1
  FROM land

  LEFT OUTER JOIN land_ownership ON land_ownership.fkey_land_id = land.id
  AND land_ownership.fkey_users_id = %d

  LEFT OUTER JOIN competencies on land.fkey_required_competencies_id =
    competencies.id
  left join competencies as comp1 on fkey_enhanced_competencies_id = comp1.id

  WHERE land.id = %d;';
  $result = db_query($sql, $game_user->id, $land_id);
  $item = db_fetch_object($result);
  if (is_object($item)) {
    $item->competency = zg_competency_name($item->competency);
    $item->competency_name_1 = zg_competency_name($item->competency_name_1);
    zg_get_icon_path($game_user, $item, 'land');
  }
  return $item;
}

function zg_show_land(\stdClass $game_user, \stdClass $item, $options = []) {

  // FIXME: support $options['max-affordable'] from CG.

  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  static $party_title;

  if (empty($party_title)) {
    $party_title = preg_replace('/^The /', '', $game_user->party_title);
  }

  $soon = ($options['soon']) ? 'soon' : '';
  $header = ($options['land-succeeded'] == 'success') ?
    '<div class="land-succeeded">Success</div>' : '';

  switch ($options['land-buy-succeeded']) {

    case 'buy-success':
      if ($item->type == 'job') {
        $header = '<div class="land-succeeded">' . t('You got a job!') .
          '</div>';
      }
      elseif ($item->type == 'goods') {
        $header = '<div class="land-succeeded">' . t('Goods Purchased') .
          '</div>';
      }
      else {
        $header = '<div class="land-succeeded">' .
          t('Investment Purchased') . '</div>';
      }
      break;

    case 'failed no-money':
      $header = '<div class="land-failed">' .
        t('Out of @value!', ['@value' => $game_user->values]) . '</div>' .
        zg_luck_money_render_button($game_user, "$game/land/$arg2");
      $failed = 'failed';
      break;

    case 'failed not-required-level':
      $header = '<div class="title">' .
        t('∆ Player Level <span>Insufficient</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-enough-land':
      $header = '<div class="title">' .
        t('∆ Quantity <span>Insufficient</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-required-competency':
      $header = '<div class="title">' .
        t('Insufficient Competency') .
        '</div>';
      $failed = 'failed';
      zg_competency_gain($game_user, 'incompetent');
      break;

    case 'failed not-required-hood':

      // FIXME: show land available in other hoods.
      $header = '<div class="title">' .
        t('∆ Location <span>Incorrect</span> ∆') .
        '</div>';
      $failed = 'failed';
      zg_competency_gain($game_user, 'lost');
      break;

    case 'failed not-required-party':
      $header = '<div class="title">' .
        t('∆ @party <span>Incorrect</span> ∆',
          ['@party' => $party]) .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-active':
    case 'failed is-loot':
      $header = '<div class="title">' .
        t('∆ Item <span>Unavailable</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed cant-sell':
      $header = '<div class="title">' .
        t('∆ Item <span>Unsalable</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;
  }

  $description = str_replace('%party', "<em>$party_title</em>",
    $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) {
    $quantity = t('None');
  }

  $land_price = $item->price + ($item->quantity * $item->price_increase);

  if (($land_price % 1000) == 0) {
    $land_price = ($land_price / 1000) . 'K';
  }

  $payout = $item->payout;

  if ((($payout % 1000) == 0) && ($payout > 0)) {
    $payout = ($payout / 1000) . 'K';
  }

  $can_buy = $can_sell = TRUE;

  // FIXME firep() all reason why can't buy nor sell.
  if ($item->fkey_neighborhoods_id != 0 &&
    $item->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id) {
    $can_buy = FALSE;
  }

  if ($item->fkey_values_id != 0 &&
    $item->fkey_values_id != $game_user->fkey_values_id) {
    $can_buy = FALSE;
  }

  if ($item->required_level > $game_user->level) {
    $can_buy = FALSE;
  }

  if (!$item->active) {
    $can_buy = FALSE;
    firep('Cannot buy because ACTIVE is FALSE', $item->name);
  }

  if ($item->is_loot != 0) {
    $can_buy = FALSE;
  }

  if ($item->can_sell != 1) {
    $can_sell = FALSE;
  }

  if ($item->quantity < 1) {
    $can_sell = FALSE;
  }

  $current = ($item->type == 'job' && $item->quantity > 0) ? t('Current') : '';
  $inactive = ($item->active) ? '' : t('(Inactive)');

  if ($can_buy) {
    $icon_button = <<< EOF
<a href="/$game/land_buy/$arg2/$item->id/1">
  <img src="/sites/default/files/images/land/$game-$item->id.png?2" width="96"
    border="0">
</a>
EOF;
    $name_link = <<< EOF
<a href="/$game/land_buy/$arg2/$item->id/1">
  <span class="type">
    $inactive $current $item->type:
  </span>
  $item->name
</a>
EOF;
    $buy_button = $apply_button = <<< EOF
<div class="land-buy-button">
  <a href="/$game/land_buy/$arg2/$item->id/1">
    Buy
  </a>
</div>
EOF;
    // Current job?  Change 'Apply' button to 'Work Overtime' button.
    if ($item->type == 'job' && !strlen($current)) {
      $apply_button = <<< EOF
<div class="land-buy-button">
  <a href="/$game/land_buy/$arg2/$item->id/1">
    Apply
  </a>
</div>
EOF;
    }
    elseif ($item->type == 'job' && strlen($current)) {
      $can_work_again = zg_can_do_yet($game_user, 'can_work_again');
//      firep($can_work_again, 'can work again');
      if ($can_work_again->allowed) {
        $apply_button = <<< EOF
<div class="land-buy-button">
  <a href="/$game/land_work/$arg2/$item->id/1">
    Work Overtime
  </a>
</div>
EOF;
      }
      else {
        $work_again = sprintf('%02d', $can_work_again->hours_remaining) .
          ':' . sprintf('%02d', $can_work_again->minutes_remaining) .
          ':' . sprintf('%02d', $can_work_again->seconds_remaining);
        $apply_button = <<< EOF
<div class="land-buy-button not-yet">
  <!--<a href="/$game/land_work/$arg2/$item->id/1">-->
    Can Work Overtime in $work_again
  <!--</a>-->
</div>
EOF;
      }
    }
  }
  else {
    // Can't buy.
    $icon_button = <<< EOF
  <img src="/sites/default/files/images/land/$game-$item->id.png?2" width="96"
    border="0">
EOF;
    $name_link = <<< EOF
<span class="type">
  $inactive $current $item->type:
</span>
$item->name
EOF;
    $buy_button = <<< EOF
<div class="land-buy-button not-yet">
  Can't Buy
</div>
EOF;
    $apply_button = <<< EOF
<div class="land-buy-button not-yet">
  Can't Apply
</div>
EOF;
  }

  if ($can_sell) {
    $sell_button = <<< EOF
<div class="land-sell-button">
  <a href="/$game/land_sell/$arg2/$item->id/1">
    Sell
  </a>
</div>
EOF;
  }
  else {
    $sell_button = <<< EOF
<div class="land-sell-button not-yet">
  Can't Sell
</div>
EOF;
  }

  if ($soon) {
    echo <<< EOF
<div class="land $soon $failed">
  <div class="land-icon">
      <img src="/sites/default/files/images/land/$game-soon.png?1" width=96
        border="0">
  </div>
  <div class="land-details">
    <div class="land-name">
        <span class="type">$inactive $current $item->type:</span>
        $item->name
    </div>
    <div class="land-description">&laquo; $description &raquo;</div>
EOF;
  }
  else {
    echo <<< EOF
<div class="land land-$item->type $failed">
  $header
  <div class="land-icon">
    $icon_button
  </div>
  <div class="land-details">
    <div class="land-name">
      $name_link
    </div>
    <div class="land-description">$description</div>
EOF;
  }

  $reqs = '';
  if ($item->type == 'investment' || $item->type == 'goods') {
    echo <<< EOF
    <div class="land-owned">Owned: $quantity</div>
EOF;
    $reqs .= <<< EOF
    <div class="land-cost">Cost: $land_price $game_user->values</div>
EOF;
  }

  if ($game_user->level < $item->required_level) {
    $reqs .= <<< EOF
    <div class="land-required_level">Player: Level $item->required_level</div>
EOF;
  }

  if ($item->fkey_required_competencies_id > 0) {
//    $check = zg_competency_level($game_user,
//      (int) $item->fkey_required_competencies_id);
//firep($check);
    $comp_name = zg_competency_level_name($item->required_competencies_level,
      TRUE);

    $reqs .= <<< EOF
    <div class="land-cost initial-caps">
      $item->competency Competency:<br/>
      <span>$comp_name (Level $item->required_competencies_level)</span>
    </div>
EOF;

  }

  if (!empty($reqs)) {
    echo <<< EOF
    <!--<div class="land-requisites">// Requisites //</div>-->
    $reqs
EOF;
  }

  echo <<< EOF
    <div class="land-disbursements">
    <div class="land-payout">
      Income: +$payout $game_user->values every 60 minutes
    </div>
EOF;

  // Competencies.
  if (isset($item->competency_name_1)) {
    echo <<< EOF
    <!--<div class="land-payout">
      Competency enhanced:
      <span class="initial-caps">$item->competency_name_1</span>
    </div>-->
    <div class="competency-tag">
      $item->competency_name_1
    </div>
EOF;
  }

  echo '</div></div>';

  // BUY buttons.
  if ($soon) {
    echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button not-yet">
      Can't Buy
    </div>
    <div class="land-sell-button not-yet">
      Can't Sell
    </div>
  </div>
EOF;
  }
  elseif ($item->type == 'investment' || $item->type == 'goods') {

    // Attempt to buy.
    if ($options['land-buy-succeeded']) {
      echo <<< EOF
<div class="land-button-wrapper">
  <form action="/$game/land_buy/$arg2/$item->id/use-quantity">
    <div class="quantity">
      <select name="quantity">
EOF;
      foreach ([1, 5, 10, 25, 50, 100] as $option) {
        if ($option == $orig_quantity) {
          echo '<option selected="selected" value="' . $option . '">' .
            $option . '</option>';
        }
        else {
          echo '<option value="' . $option . '">' . $option . '</option>';
        }
      }
      echo <<< EOF
      </select>
    </div>
    <input class="land-buy-button" type="submit" Value="Buy"/>
  </form>
</div>
EOF;

    }

    // Normal show.
    else {
      echo <<< EOF
  <div class="land-button-wrapper">
    $buy_button
    $sell_button
  </div>
EOF;
    }

  }
  else {

    // Job.
    echo <<< EOF
  <div class="land-button-wrapper">
    $apply_button
  </div>
EOF;
  }

  echo '</div>';
}

function zg_land_gain(\stdClass &$game_user, $id, $quantity = 1, $ex_price = 0) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  $sql = 'select * from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $lo = db_fetch_object($result);

  // Lo entry already there.
  if (!empty($lo)) {
    $sql = 'update land_ownership set quantity = quantity + %d
      where fkey_land_id = %d and fkey_users_id = %d;';
    db_query($sql, $quantity, $id, $game_user->id);
  }
  else {
    $sql = 'insert into land_ownership
      (fkey_users_id, fkey_land_id, quantity) values
      (%d, %d, %d);';
    db_query($sql, $game_user->id, $id, $quantity);
  }

  if ($quantity >= 100) {
    zg_competency_gain($game_user, 'big spender');
  }

  // Start the income clock, if needed.
  if (substr($game_user->income_next_gain, 0, 4) == '0000') {
    $sql = 'update users set income_next_gain = "%s" where id = %d;';
    db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 3600),
      $game_user->id);
  }

  // Pay for the land, if needed.
  if ($ex_price != 0) {
    $sql = 'update users set money = money - %d where id = %d;';
    db_query($sql, $ex_price, $game_user->id);
  }

  // Reprocess user object.
  zg_recalc_income($game_user);
  $game_user = zg_fetch_player_by_id($game_user->id);
  return TRUE;
}

function zg_land_lose(\stdClass &$game_user, $id, $quantity = 1, $ex_price = 0) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';

  // FIXME: refactor to use zg_fetch_land_by_id().
  $sql = 'select * from land where id = %d;';
  $result = db_query($sql, $id);
  $land = db_fetch_object($result);

  $sql = 'select * from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $lo = db_fetch_object($result);

  // Nothing there?
  if (empty($lo)) {
    return FALSE;
  }

  // Not enough?
  if ($lo->quantity < $quantity) {
    return FALSE;
  }

  // Using all?  Remove entry.
  if ($lo->quantity == $quantity) {
    $sql = 'delete from land_ownership
      where fkey_users_id = %d and fkey_land_id = %d;';
    db_query($sql, $game_user->id, $id);
  }
  else {
    $sql = 'update land_ownership set quantity = quantity - %d
      where fkey_land_id = %d and fkey_users_id = %d;';
    db_query($sql, $quantity, $id, $game_user->id);
  }

  // Reduce income, increase money.
  $sql = 'update users set income = income - %d, money = money + %d where id = %d;';
  db_query($sql, ($land->payout * $quantity), $ex_price, $game_user->id);
  $game_user->income -= ($land->payout * $quantity);

  // Start the income clock, if needed.
  if (substr($game_user->income_next_gain, 0, 4) == '0000') {
    $sql = 'update users set income_next_gain = "%s" where id = %d;';
    db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 3600),
      $game_user->id);
  }

  // Reprocess user object.
  zg_recalc_income($game_user);
  $game_user = zg_fetch_player_by_id($game_user->id);
  return TRUE;
}

/**
 * Show a picture of required land.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $land_id
 *   The land id.
 * @param int $req_quantity
 *   The quantity required.
 */
function zg_required_land(\stdClass $game_user, $land_id, $req_quantity = 1) {
  global $game;
  include drupal_get_path('module', 'zg') . '/includes/' . $game . '_defs.inc';
  $st = zg_fetch_land_by_id($game_user, $land_id);

  if ($st->quantity >= $req_quantity) {
    $not_yet = $a_start = $a_end = '';
  }
  else {
    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/land_buy/' .
      $arg2 . '/' . $land_id . '/' .
      ($req_quantity - $st->quantity) . '">';
    $a_end = '</a>';
  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      <img class="$not_yet" src="{$st->icon_path}" width="48">
    $a_end
  </div>&nbsp;x$req_quantity
</div>
EOF;
}
