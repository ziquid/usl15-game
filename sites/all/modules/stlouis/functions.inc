<?php

/**
 * @file functions.inc
 * Misc functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

function game_button($link = 'home', $type = 'continue', $extra_link = '') {

  global $game, $phone_id;
  $arg2 = check_plain(arg(2));
  $link = $game . '/' . $link . '/' . $arg2 . $extra_link;

  switch ($type) {

    case 'continue':
      echo <<< EOF
<div class="subtitle">
  <a href="/$link">
    <img src="/sites/default/files/images/{$game}_continue.png" width=266/>
  </a>
</div>
EOF;
    break;

  }

}

function NO_sound($name) {

  global $game;

echo <<< EOF
  <script type="text/javascript">
    var mp3 = document.createElement("audio");
    mp3.setAttribute('src', '/sites/default/files/sounds/{$game}_$name.mp3');
    mp3.load();
    document.documentElement.appendChild(mp3);
    mp3.play();
  </script>
EOF;

  return;

  echo <<< EOF
<audio src="/sites/default/files/sounds/{$game}_$name.mp3" autoplay="true">
  <embed src="/sites/default/files/sounds/{$game}_$name.mp3" autostart="true"
    loop="false" hidden="true"/>
</audio>
EOF;

}


function NO_required_staff($game_user, $staff_id, $req_quantity) {

  global $game;
  $arg2 = check_plain(arg(2));

  $sql = 'select quantity from staff_ownership
    where fkey_staff_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $staff_id, $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/staff_buy/' .
      $arg2 . '/' . $staff_id . '/' .
      ($req_quantity - $quantity->quantity) . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      <img class="$not_yet"
        src="/sites/default/files/images/staff/$game-$staff_id.png"
        width="48">
    $a_end
  </div>&nbsp;x$req_quantity
</div>
EOF;

}


function NO_required_equipment($game_user, $equipment_id, $req_quantity) {

  global $game;
  $arg2 = check_plain(arg(2));

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $equipment_id, $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/equipment_buy/' .
      $arg2 . '/' . $equipment_id . '/' .
      ($req_quantity - $quantity->quantity) . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required">
  $a_start
    <img class="$not_yet"
      src="/sites/default/files/images/equipment/$game-$equipment_id.png"
      width="48">
  $a_end
  <div class="quest-required-num">
    x$req_quantity
  </div>
</div>
EOF;

}


function NO_clan_required_equipment($game_user, $equipment_id, $req_quantity) {

  global $game;
  $arg2 = check_plain(arg(2));
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $sql = 'select quantity from clan_equipment_ownership
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $equipment_id, $game_user->fkey_clans_id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $a_start = $a_end = '';
    $not_yet = 'not-yet';

  }

  echo <<< EOF
<div class="quest-required">
  $a_start
    <img class="$not_yet"
      src="/sites/default/files/images/equipment/$game-$equipment_id.png"
      width="48">
  $a_end
  <div class="quest-required-num">
    Clan
    <br/>
    x$req_quantity
  </div>
</div>
EOF;

}


function NO_required_land($game_user, $land_id, $req_quantity) {

  global $game;
  $arg2 = check_plain(arg(2));

  $sql = 'select quantity from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $land_id, $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/land_buy/' .
      $arg2 . '/' . $land_id . '/' .
      ($req_quantity - $quantity->quantity) . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      <img class="$not_yet"
        src="/sites/default/files/images/land/$game-$land_id.png"
        width="48">
    $a_end
  </div>&nbsp;x$req_quantity
</div>
EOF;

}


function NO_required_elected_position($game_user, $id, $name) {

  global $game;
  $arg2 = check_plain(arg(2));

  if ($game_user->ep_level == $id) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/hierarchies/' .
      $arg2 . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      $ep_short: $name
    $a_end
  </div>
</div>
EOF;

}


function NO_required_competency($game_user, $id, $level, $name) {

  global $game;
  $arg2 = check_plain(arg(2));

  $check = game_competency_level($game_user, $id);
  $comp_name = competency_level_name($level, TRUE);

  if ($check->level >= $level) {

    $not_yet = $a_start = $a_end = '';

  }
  else {

    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/user_comp/' .
      $arg2 . '">';
    $a_end = '</a>';

  }

  echo <<< EOF
<div class="quest-required_stuff">Requires
  <div class="quest-required_equipment">
    $a_start
      $name Competency:<br/>
      <span>$comp_name (Level $level)</span>
    $a_end
  </div>
</div>
EOF;

}


function NO_enhanced_competency($game_user, $id, $name) {

  global $game;

  echo <<< EOF
<div class="quest-payout">Competency Enhanced:<br/>
  <span class="highlight">
      $name
  </span>
</div>
EOF;

}


function _show_goal($game_user) {
return;
  global $game;

  $sql = 'select goals.* from goals
    left join goals_achieved on fkey_goals_id = goals.id
    and fkey_users_id = %d
    where goals_achieved.id is NULL
    order by goals.id asc limit 1;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  if (substr(arg(1), 0, strlen($item->type)) == $item->type)
    $current = 'current';

  $finished = eval($item->code_to_check);

  if ($finished) {

    // Save an entry showing that this goal has been achieved.
    $sql = 'insert into goals_achieved
      (fkey_users_id, fkey_goals_id)
      values (%d, %d);';
    $result = db_query($sql, $game_user->id, $item->id);

    // Congratulate user.
    $extra_title =<<< EOF
<div class="title goal-achieved">
  Goal Achievement
  <span class="arrows">&raquo; </span>
  <span class="nowrap">$item->title</span>
</div>
EOF;

    $next = t('Next Goal');
    $current = 'current';

    // Find next goal.
    $sql = 'select goals.* from goals
      left join goals_achieved on fkey_goals_id = goals.id
      and fkey_users_id = %d
      where goals_achieved.id is NULL
      order by goals.id asc limit 1;';
    $result = db_query($sql, $game_user->id);
    $item = db_fetch_object($result);

  }
  else {

    $extra_title = '';
    $next = t('Current Goal');

  }

  // No goal.
  if (empty($item->title) && empty($extra_title)) return;

  if (empty($item->title)) $item->title = t('None');

  $description = t($item->description, array(
    '@level' => $game_user->level,
  ));

  echo <<< EOF
<div class="goals $current">
  $extra_title
  <div class="title">$next <span class="arrows">&raquo; </span>
    <span class="nowrap">$item->title</span>
  </div>
  <div class="desc-wrapper">
    <a class="toggle" onClick="javascript:toggle_goal_desc(); return false;"
      href="#">
      +
    </a>
    <p id="goal-desc">$description</p>
  </div>
</div>

<script type="text/javascript">
Element.prototype.hasClass = function(cName) {
  return this.className &&
    new RegExp('(^|\\\\s)' + cName + '(\\\\s|$)').test(this.className);
};
Element.prototype.removeClass = function(className) {
  this.className = this.className.replace(
    new RegExp('(^|\\\\s)' + className + '(\\\\s|$)', 'm'), ''
  );
};

  function toggle_goal_desc() {
    var d = document.getElementById('goal-desc');
    if (d.hasClass('show')) {
      d.removeClass('show');
    }
    else {
      d.className = d.className + ' show';
    }
  }
</script>
EOF;

}


function NO_is_quest_complete($game_user, $quest_id) {

  $sql = 'select percent_complete from quest_completion
    where fkey_users_id = %d and fkey_quests_id = %d;';
  $result = db_query($sql, $game_user->id, $quest_id);
  $item = db_fetch_object($result);

  return ($item->percent_complete >= 100);

}


function NO_is_quest_started($game_user, $quest_id) {

  $sql = 'select percent_complete from quest_completion
    where fkey_users_id = %d and fkey_quests_id = %d;';
  $result = db_query($sql, $game_user->id, $quest_id);
  $item = db_fetch_object($result);

  return ($item->percent_complete > 0);

}


function NO_on_screen($screen) {

  return (arg(1) == $screen);

}


function NO_has_job($game_user) {

  $sql = 'SELECT sum(payout) as job_income
    FROM land_ownership AS lo
    LEFT JOIN land ON lo.fkey_land_id = land.id
    WHERE fkey_users_id = %d
    AND land.type = "job";';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  return ($item->job_income > 0);

}


function NO_has_sent_message($game_user) {

  $sql = 'SELECT count(id) as count
    FROM user_messages
    WHERE fkey_users_from_id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);

  return ($item->count > 0);

}


function NO_has_equipment($game_user, $id, $quantity) {

  $sql = 'SELECT quantity
    FROM equipment_ownership
    WHERE fkey_users_id = %d
    AND fkey_equipment_id = %d;';
  $result = db_query($sql, $game_user->id, $id);
  $item = db_fetch_object($result);

  return ($item->quantity >= $quantity);

}


function NO_has_clan_equipment($game_user, $id, $quantity) {

  $sql = 'SELECT quantity
    FROM clan_equipment_ownership
    WHERE fkey_clans_id = %d
    AND fkey_equipment_id = %d;';
  $result = db_query($sql, $game_user->fkey_clans_id, $id);
  $item = db_fetch_object($result);

  return ($item->quantity >= $quantity);

}


function game_show_land($game_user, $item, $options = []) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  static $party_title;

  if (empty($party_title)) {
    $sql = 'select party_title from `values` where id = %d;';
    $result = db_query($sql, $game_user->fkey_values_id);
    $data = db_fetch_object($result);
    $party_title = preg_replace('/^The /', '', $data->party_title);
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
      else {
        $header = '<div class="land-succeeded">' .
          t('Investment Purchased') . '</div>';
      }
      break;

    case 'failed no-money':
      $offer = game_luck_money_offer($game_user);
      $header = '<div class="title">' .

          // FIXME remove special characters from text here.
          t('∆ @value <span>Insufficient</span> ∆',
            array('@value' => $game_user->values)) . '
        </div>
        <div class="try-an-election-wrapper">
          <div class="try-an-election">
            <a href="/' . $game . '/elders_do_fill/' . $arg2 .
              '/money?destination=/' . $game . '/land/' . $arg2 . '">' .
              t('Receive @offer @values (1&nbsp;@luck',
                array(
                  '@offer' => $offer,
                  '@values' => $game_user->values,
                  '@luck' => $luck,
                )) . ')
            </a>
          </div>
        </div>';

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
      competency_gain($game_user, 'incompetent');
      break;

    case 'failed not-required-hood':

      // FIXME: show land available in other hoods.
      $header = '<div class="title">' .
        t('∆ Location <span>Incorrect</span> ∆') .
        '</div>';
      $failed = 'failed';
      competency_gain($game_user, 'lost');
      break;

    case 'failed not-required-party':
      $header = '<div class="title">' .
        t('∆ @party <span>Incorrect</span> ∆',
          array('@party' => $party)) .
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
  if (empty($quantity)) $quantity = t('None');

  $land_price = $item->price + ($item->quantity *
    $item->price_increase);

  if (($land_price % 1000) == 0)
    $land_price = ($land_price / 1000) . 'K';

  $payout = $item->payout;

  if ((($payout % 1000) == 0) && ($payout > 0))
    $payout = ($payout / 1000) . 'K';

  $can_buy = $can_sell = TRUE;

  // FIXME firep() all reason why can't buy nor sell.
  if ($item->fkey_neighborhoods_id != 0 &&
    $item->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id)
    $can_buy = FALSE;

  if ($item->fkey_values_id != 0 &&
    $item->fkey_values_id != $game_user->fkey_values_id)
    $can_buy = FALSE;

  if ($item->required_level > $game_user->level)
    $can_buy = FALSE;

  if (!$item->active) {
    $can_buy = FALSE;
    firep('Cannot buy because ACTIVE is FALSE', $item->name);
  }

  if ($item->is_loot != 0)
    $can_buy = FALSE;

  if ($item->can_sell != 1)
    $can_sell = FALSE;

  if ($item->quantity < 1)
    $can_sell = FALSE;

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
    $buy_button = <<< EOF
<div class="land-buy-button">
  <a href="/$game/land_buy/$arg2/$item->id/1">
    Buy
  </a>
</div>
EOF;
    // Current job?  Change 'Apply' button to 'Work Overtime' button.
    if (strlen($current) == 0) {
      $apply_button = <<< EOF
<div class="land-buy-button">
  <a href="/$game/land_buy/$arg2/$item->id/1">
    Apply
  </a>
</div>
EOF;
    }
    else {
      $can_work_again = game_can_do_yet($game_user, 'can_work_again');
      firep($can_work_again, 'can work again');
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

  if ($item->type == 'investment') {
    echo <<< EOF
    <div class="land-owned">Owned: $quantity</div>
EOF;
    $reqs .= <<< EOF
    <div class="land-cost">Cost: $land_price $game_user->values</div>
EOF;
  }

  if ($game_user->level < $item->required_level) $reqs .= <<< EOF
    <div class="land-required_level">Player: Level $item->required_level</div>
EOF;

  if ($item->fkey_required_competencies_id > 0) {

    $check = game_competency_level($game_user,
      (int) $item->fkey_required_competencies_id);
firep($check);
    $comp_name = competency_level_name($item->required_competencies_level,
      TRUE);

    $reqs .= <<< EOF
    <div class="land-cost initial-caps">
      $item->competency Competency:<br/>
      <span>$comp_name (Level $item->required_competencies_level)</span>
    </div>
EOF;

  }

  if (!empty($reqs)) echo <<< EOF
    <!--<div class="land-requisites">// Requisites //</div>-->
    $reqs
EOF;

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
  else if ($item->type == 'investment') {

    // Attempt to buy.
    if ($options['land-buy-succeeded']) {
      echo <<< EOF
<div class="land-button-wrapper">
  <form action="/$game/land_buy/$arg2/$item->id/use-quantity">
    <div class="quantity">
      <select name="quantity">
EOF;
      foreach (array(1, 5, 10, 25, 50, 100) as $option) {
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

// Sometimes the income gets hacked -- fix here.
function game_recalc_income(&$game_user) {

  $sql = 'update users set income =
    (SELECT sum(land.payout * land_ownership.quantity)
    as income from land
    left join land_ownership
    on land_ownership.fkey_land_id = land.id and
    land_ownership.fkey_users_id = %d)
    where id = %d;';
  db_query($sql, $game_user->id, $game_user->id);

  $sql = 'SELECT sum(equipment.upkeep * equipment_ownership.quantity)
    as expenses from equipment
    left join equipment_ownership
    on equipment_ownership.fkey_equipment_id = equipment.id and
    equipment_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $eq = db_fetch_object($result);

  $sql = 'SELECT sum(staff.upkeep * staff_ownership.quantity)
    as expenses from staff
    left join staff_ownership
    on staff_ownership.fkey_staff_id = staff.id and
    staff_ownership.fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);
  $st = db_fetch_object($result);

  $sql = 'update users set expenses = %d where id = %d;';
  db_query($sql, $eq->expenses + $st->expenses, $game_user->id);

  $sql = 'select income, expenses from users
    where id = %d;';
  $result = db_query($sql, $game_user->id);
  $item = db_fetch_object($result);
  $game_user->income = $item->income;
  $game_user->expenses = $item->expenses;

}


function NO_show_actions_menu($game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));

  if (arg(3) == '') $normal_active = 'active';
  if (arg(3) == 'clan') $clan_active = 'active';

  echo <<< EOF
<div class="news">
  <a href="/$game/actions/$arg2" class="button $normal_active">Normal</a>
  <a href="/$game/actions/$arg2/clan" class="button $clan_active">Clan</a>
</div>
EOF;

  _show_goal($game_user);

}

function game_show_aides_menu($game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  $title = '';

  if (substr(arg(1), 0, 4) == 'land') {
    $land_active = 'active';
  }
  if (arg(1) == 'land') {
    $title = t('Available @land', ['@land' => $land_plural]);
  }

  if (substr(arg(1), 0, 9) == 'equipment') {
    $equip_active = 'active';
  }
  if (arg(1) == 'equipment') {
    $title = t('Purchase @equip', ['@equip' => $equipment]);
  }

  if (substr(arg(1), 0, 5) == 'staff') {
    $staff_active = 'active';
  }
  if (arg(1) == 'staff') {
    $title = t('Hire @staff', ['@staff' => $game_text['staff']]);
  }

  echo <<< EOF
<div class="news">
  <a href="/$game/land/$arg2" class="button $land_active">$land_plural</a>
  <a href="/$game/equipment/$arg2" class="button $equip_active">$equipment</a>
  <!--<a href="/$game/weapons/$arg2" class="button $weapons_active">$weapons</a>-->
  <a href="/$game/staff/$arg2" class="button $staff_active">{$game_text['staff']}</a>
  <a href="/$game/agents/$arg2" class="button">Agents</a>
</div>
EOF;

  if ($land_active) {
    echo <<< EOF
<div class="news secondary">
  <div class="news-buttons">
    <button id="news-all" class="active">All</button>
    <button id="news-jobs">{$game_text['job_plural']}</button>
    <button id="news-investments">{$game_text['investment_plural']}</button>
  </div>
</div>
EOF;

    drupal_add_js(drupal_get_path('module', 'stlouis')
      . '/game_show_aides_menu.js', 'module', 'footer');
  }

  _show_goal($game_user);

 if (!empty($title)) echo <<< EOF
<div class="title">
  $title
</div>
EOF;

}


function game_show_elections_menu($game_user) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));

  if (substr(arg(1), 0, 7) == 'debates') {
    $debates_active = 'active';
  }
  if (substr(arg(1), 0, 9) == 'elections') {
    $elections_active = 'active';
  }
  if (substr(arg(1), 0, 3) == 'top') {
    $top_active = 'active';
  }
  if (substr(arg(1), 0, 5) == 'top20') {
    $top20_active = 'active';
  }
  if (substr(arg(1), 0, 12) == 'top_alderman') {
    $topalders_active = 'active';
  }

  echo <<< EOF
<div class="news">
<a href="/$game/debates/$arg2" class="button $debates_active">{$debate_tab}</a>
<a href="/$game/elections/$arg2" class="button $elections_active">{$election_tab}</a>
<a href="/$game/top20/$arg2" class="button $top20_active">$top20</a>
<a href="/$game/top_aldermen/$arg2" class="button $topalders_active">Top $alders_short</a>
</div>
EOF;

  _show_goal($game_user);

}


function _show_profile_menu($game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  $arg3 = check_plain(arg(3));

  if (arg(1) == 'user') $user_active = 'active';
  if (arg(1) == 'user_favors') $favor_active = 'active';
  if (arg(1) == 'favor_perform') {
    $favor_active = 'active';
    $arg3 = '';
  }
  if (arg(1) == 'user_comp') $comp_active = 'active';

  echo <<< EOF
<div class="news">
  <a href="/$game/user/$arg2/$arg3" class="button $user_active">Profile</a>
  <!--<a href="/$game/user_favors/$arg2/$arg3"
    class="button $favor_active">{$favor}s</a>-->
  <a href="/$game/user_comp/$arg2/$arg3"
    class="button $comp_active">$competencies</a>
</div>
EOF;

  _show_goal($game_user);

}


function NO_show_clan_menu($game_user) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  $arg3 = check_plain(arg(3));

  if (arg(1) == 'clan_list') $list_active = 'active';
  if (arg(1) == 'clan_msg') $msg_active = 'active';
  if (arg(1) == 'clan_announcements') $ann_active = 'active';
  if (arg(1) == 'clan_admin') $admin_active = 'active';

  echo <<< EOF
<div class="news">
  <a href="/$game/clan_list/$arg2/$arg3"
    class="button $list_active">Members</a>
  <a href="/$game/clan_msg/$arg2/$arg3"
    class="button $msg_active">Messages</a>
  <a href="/$game/clan_announcements/$arg2/$arg3"
    class="button $ann_active">Announcements</a>
  <a href="/$game/clan_list/$arg2/$arg3"
    class="button not-yet $admin_active">Admin</a>
</div>
EOF;

  _show_goal($game_user);

}


function _show_quest($game_user, $item, $percentage_target,
  $percentage_divisor, $quest_group, $party_title, $outcome = '',
  $exp_added_str = '', $money_added_str = '', $disbursements_doubled = '',
  $consumption_html = '', $disbursement_html = '',
  $quest_completion_html = '') {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));

  static $location;

  if (empty($location)) {

    $sql = 'select name from `neighborhoods` where id = %d;';
    $result = db_query($sql, $game_user->fkey_neighborhoods_id);
    $data = db_fetch_object($result);
    $location = preg_replace('/^The /', '', $data->name);

  }

  $sql = 'select id from quests where `group` = %d
    order by id asc limit 1;';
  $result = db_query($sql, $item->group);
  $data = db_fetch_object($result);
  $seq_num = $item->id - $data->id;

  if (substr($item->description, 0, 3) != '<p ') $laquo = '&laquo;';
  if (substr($item->description, -1) != '>') $raquo = '&raquo;';

  $name = t($item->name, array(
    '@clan' => $game_user->clan_name,
    '@hood' => $hood,
    '@location' => $location,
    '%party' => $party_title,
    '@username' => $game_user->username,
  ));

  $description = t($item->description, array(
    '@clan' => $game_user->clan_name,
    '@hood' => $hood,
    '@location' => $location,
    '%party' => $party_title,
    '@username' => $game_user->username,
  ));

  if (empty($item->percent_complete)) $item->percent_complete = 0;

  if ($item->percent_complete > floor($percentage_target / 2)) {

    $rgb = dechex(floor(($percentage_target - $item->percent_complete) /
      (4 * $percentage_divisor))) . 'c0';

  }
  else {

    $rgb = 'c' . dechex(floor(($item->percent_complete) /
      (4 * $percentage_divisor))) . '0';

  }

  $width = floor($item->percent_complete * 94 / $percentage_target) + 2;

  $active = ($item->active) ? '' : ' (inactive)';

  if (strpos($item->meta, 'use_job_picture') !== FALSE) {

    $sql = 'SELECT land.id FROM `land_ownership`
      LEFT JOIN land ON fkey_land_id = land.id
      WHERE fkey_users_id = %d
      AND land.type = "job";';
    $result = db_query($sql, $game_user->id);
    $job = db_fetch_object($result);

    $img = '/sites/default/files/images/land/' . "$game-$job->id.png?3";

  }
  else {

    $img = '/sites/default/files/images/quests/' . "$game-$item->id.png?3";

  }

  echo <<< EOF
$outcome
<div class="quests">
  <div class="quest-icon">
    <a href="/$game/quests_do/$arg2/$item->id">
      <img src="$img" border="0" width="96"/>
    </a>
    <div class="quest-complete">
      <div class="quest-complete-percentage"
        style="background-color: #$rgb; width: {$width}px">
        &nbsp;
      </div>
      <div class="quest-complete-text">
        $item->percent_complete% complete
      </div>
    </div>
  </div>

  <div class="quest-details">
    <div class="quest-num">$item->group.$seq_num</div>
    <div class="quest-name">
      <a href="/$game/quests_do/$arg2/$item->id">
        $name $active
      </a>
    </div>
    <div class="quest-description">
      $laquo $description $raquo
    </div>
EOF;

  // Quest performed?
  if (empty($exp_added_str)) {

    // No quest performed -- show requirements.
    echo <<< EOF
    <div class="quest-requisites">
      // Consumptions //
    </div>
    <div class="quest-required_energy">
      $item->required_energy $energy
    </div>
EOF;

  if ($item->clan_equipment_1_consumed_quantity > 0) {

    echo '<div class="quest-required_stuff">';

    _clan_required_equipment($game_user,
      $item->fkey_clan_equipment_1_consumed_id,
      $item->clan_equipment_1_consumed_quantity);

    echo '</div>';

  }

  if ($item->land_required_quantity +
    $item->equipment_1_required_quantity +
    $item->equipment_2_required_quantity +
    $item->equipment_3_required_quantity +
    $item->clan_equipment_1_required_quantity +
    $item->staff_required_quantity > 0) {

      echo <<< EOF
    <div class="quest-requisites">
      // Requisites //
    </div>
EOF;

      // Required land.
      if ($item->land_required_quantity > 0)
        _required_land($game_user, $item->fkey_land_required_id,
          $item->land_required_quantity);

      // Required equipment.
      if ($item->equipment_1_required_quantity +
        $item->equipment_2_required_quantity +
        $item->equipment_3_required_quantity +
        $item->clan_equipment_1_required_quantity > 0) {

        echo '<div class="quest-required_stuff">';

        if ($item->equipment_1_required_quantity > 0)
          _required_equipment($game_user, $item->fkey_equipment_1_required_id,
            $item->equipment_1_required_quantity);

        if ($item->equipment_2_required_quantity > 0)
          _required_equipment($game_user, $item->fkey_equipment_2_required_id,
            $item->equipment_2_required_quantity);

        if ($item->equipment_3_required_quantity > 0)
          _required_equipment($game_user, $item->fkey_equipment_3_required_id,
            $item->equipment_3_required_quantity);

        if ($item->clan_equipment_1_required_quantity > 0)
          _clan_required_equipment($game_user,
            $item->fkey_clan_equipment_1_required_id,
            $item->clan_equipment_1_required_quantity);

        echo '</div>';

      }

      // Required staff.
      if ($item->staff_required_quantity > 0)
        _required_staff($game_user, $item->fkey_staff_required_id,
          $item->staff_required_quantity);

      // Required party.
      if ($item->need_party > 0)
        echo <<< EOF
    <div class="quest-required_energy">
      You must belong to a non-Underground $party_lower
    </div>
EOF;

      // Required clan.
      if ($item->need_clan > 0)
        echo <<< EOF
    <div class="quest-required_energy">
      You must belong to a clan
    </div>
EOF;

      // Required job.
      if ($item->need_job > 0)
        echo <<< EOF
    <div class="quest-required_energy">
      You must have a job
    </div>
EOF;

    }

    echo <<< EOF
    <div class="quest-disbursements">
      // Disbursements //
    </div>
    <div class="quest-experience">
      +$item->experience $experience
    </div>
    <div class="quest-experience">
      +$item->min_money to $item->max_money $game_user->values
    </div>
EOF;

    if (($item->chance_of_loot + $item->chance_of_loot_staff) > 0)
      echo <<< EOF
    <div class="quest-loot">
      Equipment Disbursement Percentage, Estimated:
      <span>
        $item->chance_of_loot%
      </span>
    </div>
EOF;

  }

  // Quest performed -- show accomplishments.
  else {

    echo <<< EOF
    <div class="quest-requisites">
      // Consumed By You //
    </div>
    <div class="quest-required_energy">
      $item->required_energy $energy
    </div>
    $consumption_html
    <div class="quest-disbursements">
      // Disbursed To You //
    </div>
    <div class="quest-experience">
      $exp_added_str $experience $disbursements_doubled
    </div>
    <div class="quest-experience">
      $money_added_str $game_user->values $disbursements_doubled
    </div>
  </div>
  $disbursement_html
  $quest_completion_html
  <div class="quest-do-again">
    <div class="quest-do-again-inside">
      <a href="/$game/quests_do/$arg2/$item->id">
        Do Again
      </a>
    </div>
EOF;

  }

  echo <<< EOF
  </div>
</div>
EOF;

}


function _show_favor($game_user, $item, $view = 'initiator', $bounty = -1) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));

  static $location, $party_title;

  if (empty($location)) {

    $sql = 'select name from `neighborhoods` where id = %d;';
    $result = db_query($sql, $game_user->fkey_neighborhoods_id);
    $data = db_fetch_object($result);
    $location = preg_replace('/^The /', '', $data->name);

  }

  if (empty($party_title)) {

    $sql = 'select party_title from `values` where id = %d;';
    $result = db_query($sql, $game_user->fkey_values_id);
    $data = db_fetch_object($result);
    $party_title = preg_replace('/^The /', '', $data->party_title);

  }

  if (substr($item->description, 0, 3) != '<p ') $laquo = '&laquo;';
  if (substr($item->description, -1) != '>') $raquo = '&raquo;';

  $name = t($item->name, array(
    '%party' => $party_title,
    '@username' => $game_user->username,
    '@hood' => $hood,
    '@location' => $location,
  ));

  if (($view == 'runner') || ($view == 'perform'))
    $item->initiator_description = $item->runner_description;

  $description = t($item->initiator_description, array(
    '%party' => $party_title,
    '@username' => $game_user->username,
    '@hood' => $hood,
    '@location' => $location,
  ));

  $active = ($item->active) ? '' : ' (inactive)';

  if (!empty($item->favor_id)) {
    $item->req_id = $item->id;
    $item->id = $item->favor_id;
  }

  switch ($view) {

    case 'initiator':

      $a_start = "<a href=\"/$game/favors_selected/$arg2/$item->id\">";
      $a_end = '</a>';
      $link_text = 'Select &raquo;&raquo;';
      break;

    case 'runner':

      $a_start = "<a href=\"/$game/favor_perform/$arg2/$item->req_id\">";
      $a_end = '</a>';
      $link_text = 'Perform';
      break;

    case 'perform':

      $a_start = $a_end = $link_text = '';
      break;

  }

  echo <<< EOF
<div class="quests">
  <div class="quest-icon">
    $a_start
      <img src="/sites/default/files/images/favors/$game-$item->id.png?0"
        border="0" width="96"/>
    $a_end
  </div>

  <div class="quest-details">
    <div class="quest-name">
      $a_start
        $active $name
      $a_end
    </div>
    <div class="quest-description">
      $laquo $description $raquo
    </div>
EOF;

  if ($view == 'initiator') echo <<< EOF
    <div class="quest-requisites">
      // Initiator Requisites //
    </div>
    <div class="quest-experience">
      $game_user->values: $item->values_cost
    </div>
    <div class="quest-experience">
      {$actions}: $item->actions_cost
    </div>
EOF;

  if ($view != 'perform') {

    echo <<< EOF
    <div class="quest-requisites">
      // Runner Requisites //
    </div>
    <div class="quest-experience">
      {$actions}: $item->runner_actions_cost
    </div>
EOF;

    // Due date.
    if (!empty($item->time_due)) {

    $time_due = _stlouis_format_date(strtotime($item->time_due));

    echo <<< EOF
    <div class="quest-experience">
      Perform By: <span class="highlight">$time_due</span>
    </div>
EOF;

    }

    // Required elected position.
    if ($item->fkey_required_elected_positions_id > 0) {

      _required_elected_position($game_user,
        $item->fkey_required_elected_positions_id, $item->ep_name);

    }

    // Required competencies.
    if ($item->fkey_required_competencies_id > 0 &&
      $item->required_competencies_level > 0) {

      _required_competency($game_user, $item->fkey_required_competencies_id,
        $item->required_competencies_level, $item->cr_name);

    }

  }

  echo <<< EOF
    <div class="quest-requisites">
      // Runner Disbursements //
    </div>
EOF;

  // Enhanced competencies.
  if ($item->fkey_enhanced_competencies_id > 0) {

    _enhanced_competency($game_user, $item->fkey_enhanced_competencies_id,
      $item->ce_name);

  }

  if ($view != 'perform') {

    $bounty_values_min = floor($item->values_cost / 2);
    $bounty_values_max = $item->values_cost * 5;
    $bounty = "$bounty_values_min to $bounty_values_max";

  }

  echo <<< EOF
    $no_reqs
    <div class="quest-disbursements">
      // Bounty //
    </div>
    <div class="quest-experience">
      $bounty $game_user->values
    </div>
  </div>
EOF;

  if ($view != 'perform') echo <<< EOF
  <div class="quest-perform-button-wrapper">
    <div class="quest-perform-button">
      $a_start
        $link_text
      $a_end
    </div>
  </div>
EOF;

  echo '</div>';

}


function equipment_gain(&$game_user, $id, $quantity) {

  // BUG?  How do these get here?
  if ($id == 0) {
    mail('joseph@ziquid.com', $game_user->name . ' trying to get equipment 0!',
      $game_user->phone_id);
    return array(FALSE, 'bad-id', 0);
  }

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  // Check for enough income to cover upkeep.
  if ($game_user->income <
    $game_user->expenses + ($eq->upkeep * $quantity))
      return array(FALSE, 'not-enough-income', ($eq->upkeep * $quantity));

  // Eo entry already there.
  if (!empty($eo)) {

    if (($eq->quantity_limit > 0) &&
      (($eo->quantity + $quantity) > $eq->quantity_limit))
      return array(FALSE, 'quantity-exceeded', $eq->quantity_limit);

    $sql = 'update equipment_ownership set quantity = quantity + %d
      where fkey_equipment_id = %d and fkey_users_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->id);

  }
  else {

    if (($eq->quantity_limit > 0) && ($quantity > $eq->quantity_limit))
      return array(FALSE, 'quantity-exceeded', $eq->quantity_limit);

    $sql = 'insert into equipment_ownership
      (fkey_users_id, fkey_equipment_id, quantity) values
      (%d, %d, %d);';
    $result = db_query($sql, $game_user->id, $id, $quantity);

  }

  // Process upkeep.
  // Add upkeep to user expenses.
  if ($eq->upkeep > 0) {
    $sql = 'update users set expenses = expenses + %d where id = %d;';
    $result = db_query($sql, ($eq->upkeep * $quantity), $game_user->id);
    $game_user->expenses += ($eq->upkeep * $quantity);
  }

  return array(TRUE, 'success', 0);

}


function NO_clan_equipment_gain(&$game_user, $id, $quantity) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from clan_equipment_ownership
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $id, $game_user->fkey_clans_id);
  $eo = db_fetch_object($result);

  // Eo entry already there.
  if (!empty($eo)) {

    if (($eq->quantity_limit > 0) &&
      (($eo->quantity + $quantity) > $eq->quantity_limit))
      return array(FALSE, 'quantity-exceeded', $eq->quantity_limit);

    $sql = 'update clan_equipment_ownership set quantity = quantity + %d
      where fkey_equipment_id = %d and fkey_clans_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->fkey_clans_id);

  }
  else {

    if (($eq->quantity_limit > 0) && ($quantity > $eq->quantity_limit))
      return array(FALSE, 'quantity-exceeded', $eq->quantity_limit);

    $sql = 'insert into clan_equipment_ownership
      (fkey_clans_id, fkey_equipment_id, quantity) values
      (%d, %d, %d);';
    $result = db_query($sql, $game_user->fkey_clans_id, $id, $quantity);

  }

  return array(TRUE, 'success', 0);

}


function NO_equipment_lose(&$game_user, $id, $quantity) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  // Nothing there?
  if (empty($eo)) return FALSE;

  // Not enough?
  if ($eo->quantity < $quantity) return FALSE;

  // Using all?  Remove entry.
  if ($eo->quantity == $quantity) {

    $sql = 'delete from equipment_ownership
      where fkey_users_id = %d and fkey_equipment_id = %d;';
    $result = db_query($sql, $game_user->id, $id);

  }
  else {

    $sql = 'update equipment_ownership set quantity = quantity - %d
      where fkey_equipment_id = %d and fkey_users_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->id);

  }

  // Check for upkeep.
  // Subtract upkeep from user expenses.
  if ($eq->upkeep > 0) {
    $sql = 'update users set expenses = expenses - %d where id = %d;';
    $result = db_query($sql, ($eq->upkeep * $quantity), $game_user->id);
    $game_user->expenses -= ($eq->upkeep * $quantity);
  }

  return TRUE;

}


function NO_clan_equipment_lose(&$game_user, $id, $quantity) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from clan_equipment_ownership
    where fkey_equipment_id = %d and fkey_clans_id = %d;';
  $result = db_query($sql, $id, $game_user->fkey_clans_id);
  $eo = db_fetch_object($result);

  // Nothing there?
  if (empty($eo)) return FALSE;

  // Not enough?
  if ($eo->quantity < $quantity) return FALSE;

  // Using all?  Remove entry.
  if ($eo->quantity == $quantity) {

    $sql = 'delete from clan_equipment_ownership
      where fkey_clans_id = %d and fkey_equipment_id = %d;';
    $result = db_query($sql, $game_user->fkey_clans_id, $id);

  }
  else {

    $sql = 'update clan_equipment_ownership set quantity = quantity - %d
      where fkey_equipment_id = %d and fkey_clans_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->fkey_clans_id);

  }

  return TRUE;

}

function land_gain(&$game_user, $id, $quantity, $ex_price = 0) {

  global $game, $phone_id;

  $fetch_user = '_' . $game . '_fetch_user';

  $sql = 'select * from land where id = %d;';
  $result = db_query($sql, $id);
  $land = db_fetch_object($result);

  $sql = 'select * from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $lo = db_fetch_object($result);

  // Lo entry already there.
  if (!empty($lo)) {

    $sql = 'update land_ownership set quantity = quantity + %d
      where fkey_land_id = %d and fkey_users_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->id);

  }
  else {

    $sql = 'insert into land_ownership
      (fkey_users_id, fkey_land_id, quantity) values
      (%d, %d, %d);';
    $result = db_query($sql, $game_user->id, $id, $quantity);

  }

  // Process income.
  $sql = 'update users set income = income + %d where id = %d;';
  $result = db_query($sql, ($land->payout * $quantity), $game_user->id);
  $game_user->income += ($land->payout * $quantity);

  // Start the income clock, if needed.
  if (substr($game_user->income_next_gain, 0, 4) == '0000') {

    $sql = 'update users set income_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', time() + 3600),
      $game_user->id);

  }

  // Pay for the land, if needed.
  $sql = 'update users set money = money - %d
    where id = %d;';
  db_query($sql, $ex_price, $game_user->id);

  // Reprocess user object.
  game_recalc_income($game_user);
  $game_user = $fetch_user();
  return TRUE;
}

function land_lose(&$game_user, $id, $quantity, $ex_price = 0) {

  global $game, $phone_id;

  $fetch_user = '_' . $game . '_fetch_user';

  $sql = 'select * from land where id = %d;';
  $result = db_query($sql, $id);
  $land = db_fetch_object($result);

  $sql = 'select * from land_ownership
    where fkey_land_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $lo = db_fetch_object($result);


  // Nothing there?
  if (empty($lo)) return FALSE;

  // Not enough?
  if ($lo->quantity < $quantity) return FALSE;

  // Using all?  Remove entry.
  if ($lo->quantity == $quantity) {

    $sql = 'delete from land_ownership
      where fkey_users_id = %d and fkey_land_id = %d;';
    $result = db_query($sql, $game_user->id, $id);

  }
  else {

    $sql = 'update land_ownership set quantity = quantity - %d
      where fkey_land_id = %d and fkey_users_id = %d;';
    $result = db_query($sql, $quantity, $id, $game_user->id);

  }

  // Process income.
  $sql = 'update users set income = income - %d where id = %d;';
  $result = db_query($sql, ($land->payout * $quantity), $game_user->id);
  $game_user->income -= ($land->payout * $quantity);

  // Start the income clock, if needed.
  if (substr($game_user->income_next_gain, 0, 4) == '0000') {

    $sql = 'update users set income_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', time() + 3600),
      $game_user->id);

  }

  // Receive pay for the land, if needed.
  $sql = 'update users set money = money + %d
    where id = %d;';
  $result = db_query($sql, $ex_price, $game_user->id);

  game_recalc_income($game_user);

  // Reprocess user object.
  $game_user = $fetch_user();
  return TRUE;

}

function competency_gain($game_user, $id, $level = 1) {

  // If id is string, find numerical id.
  if (gettype($id) == 'string') {

    $sql = 'select * from competencies where name = "%s";';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);

    if (!empty($item)) {
      $id = (int) $item->id;
    }

    // Not found! Create it!
    else {

      $sql = 'insert into competencies (name, level) values ("%s", %d);';
      $result = db_query($sql, $id, $level);

      $sql = 'select * from competencies where name = "%s";';
      $result = db_query($sql, $id);
      $item = db_fetch_object($result);
      $id = (int) $item->id;

    }

  }
  else {
    $sql = 'select * from competencies where id = %d;';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);
  }

  $sql = 'select use_count, timestamp from user_competencies
    where fkey_competencies_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $data = db_fetch_object($result);

  if ((time() - strtotime($data->timestamp)) < 180) {
firep('Last gain of competency &quot;' . $item->name . '&quot; too recent');
    return FALSE;
  }

  // Already there.
  if (!empty($data)) {

    $comp_before = game_competency_level($game_user, $id);
    $sql = 'update user_competencies set use_count = use_count + 1
      where fkey_competencies_id = %d and fkey_users_id = %d;';
    db_query($sql, $id, $game_user->id);
    $comp_after = game_competency_level($game_user, $id);

    // Leveled up!
    if ($comp_after->level > $comp_before->level) {

      send_user_message(1, $game_user->id, 1,
        'You have increased your competency &quot;<span class="initial-caps">'
      . $item->name . '</span>&quot;!&nbsp; You are now '
      . $comp_after->level_name_2 . ' and have gained '
      . $comp_after->level . ' luck.');

      $sql = 'update users set luck = luck + %d where id = %d;';
      db_query($sql, $comp_after->level, $game_user->id);

    }

  }

  // New competency.
  else {

    $sql = 'insert into user_competencies
      (fkey_users_id, fkey_competencies_id, use_count) values
      (%d, %d, 1);';
    db_query($sql, $game_user->id, $id);

    send_user_message(1, $game_user->id, 1,
      'You have started learning a new competency, &quot;<span
      class="initial-caps">'
    . $item->name . '</span>&quot;!');

  }

  return TRUE;

}


function NO_election_won($game_user, $id) {

  $sql = 'delete from elected_officials where fkey_users_id = %d;';
  $result = db_query($sql, $game_user->id);

  $sql = 'insert into elected_officials
    (fkey_users_id, fkey_elected_positions_id) values
      (%d, %d);';
  $result = db_query($sql, $game_user->id, $id);

  return TRUE;

}


function NO_action_use(&$game_user, $quantity = 1) {

  if ($game_user->actions < $quantity) return FALSE;
  if ($quantity < 1) return FALSE;

  $fetch_user = '_' . arg(0) . '_fetch_user';

  $sql = 'update users set actions = actions - %d
    where id = %d;';
  $result = db_query($sql, $quantity, $game_user->id);

  // Start the actions clock, if needed.
  if ($game_user->actions == $game_user->actions_max) {

    $sql = 'update users set actions_next_gain = "%s" where id = %d;';
    $result = db_query($sql, date('Y-m-d H:i:s', time() + 180),
      $game_user->id);

  }

  // Reprocess user object.
  $game_user = $fetch_user();
  return TRUE;

}

function game_karma($game_user, $text, $quantity) {

  $sql = 'update users set karma = karma + %d where id = %d;';
  db_query($sql, $quantity, $game_user->id);

  $sql = 'insert into karma (fkey_users_id, text, amount) values
    (%d, "%s", %d);';
  db_query($sql, $game_user->id,
    "$game_user->username gained/lost $quantity karma " .
    "(was: $game_user->karma) for: $text", $quantity);

  return TRUE;
}

function game_competency_level($game_user, $id) {

  // Return the user's competency level [0..5] for a given competency.
  include drupal_get_path('module', check_plain(arg(0))) . '/game_defs.inc';

  // Find id from string.
  if (gettype($id) == 'string') {

    $sql = 'select * from competencies where name = "%s";';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);

    if (!empty($item)) {
      $id = $item->id;
    }
    else {
      echo 'competency_level for id ' . $id . ' doesn\'t exist!';
      slack_send_message('competency_level for id ' . $id . 'doesn\'t exist!',
        $slack_channel);
      slack_send_message(print_r($game_user, TRUE), $slack_channel);
      die;
    }

  }
  else {
    $sql = 'select * from competencies where id = %d;';
    $result = db_query($sql, $id);
    $item = db_fetch_object($result);
  }

  $sql = 'select use_count from user_competencies
    where fkey_competencies_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $comp = db_fetch_object($result);

  $level = 0;

  while ($level <= 5) {

    $min = competency_min_count($level, $item->level);
    $max = competency_min_count($level + 1, $item->level);

    if ($comp->use_count >= $min && $comp->use_count < $max) {
      $comp->level = $level;
      $comp->min = $min;
      $comp->max = $max;
      $comp->next = $max;

      // Cannot attain next level.
      if ($level == 5) {
        $comp->next = $comp->use_count;
      }

      break;
    }

    $level++;

  }

  $comp->level_name = competency_level_name($comp->level, TRUE);
  $comp->level_name_2 = competency_level_name($comp->level);
  return $comp;
}

function competency_min_count($level, $comp_level) {

  // Return the minimum use count for a given level [0..5] and competency level.
  // [1..3].
  $min_count = [
    1 => [
      0,
      20,
      60,
      150,
      300,
      500,
      PHP_INT_MAX
    ],
    2 => [
      0,
      10,
      30,
      75,
      150,
      250,
      PHP_INT_MAX
    ],
    3 => [
      0,
      2,
      6,
      15,
      30,
      50,
      PHP_INT_MAX
    ],
  ];

  return $min_count[$comp_level][$level];
}


function competency_level_name($id, $req = FALSE) {

  if ($req) {
    $competencies = [
      'No Comp\'cy',
      'Minimal Comp\'cy',
      'Marginal Comp\'cy',
      'Normal Comp\'cy',
      'More Comp\'cy',
      'Masterful Comp\'cy',
    ];
  }
  else {
    $competencies = [
      'Not Comp.',
      'Minimally Comp.',
      'Marginally Comp.',
      'Normally Comp.',
      'More Comp.',
      'Masterfully Comp.',
    ];
  }

  return $competencies[$id];
}


function fetch_user_by_id($id) {

  // Simple per-page cache.
  static $user_cache;

  if (!empty($user_cache[$id])) {
    firep("query for user $id resulted in cached object");
    // Jwc 05Jul2013 -- don't return cached results.
    // Until we can reliably invalidate the cache.
//    return $user_cache[$id];
  }

  // Find id from phone_id.
  if (gettype($id) == 'string') {

    $where = 'WHERE users.phone_id = "%s"';

  }
  else {

    $where = 'WHERE users.id = %d';

  }

  $sql = 'SELECT users.*,
    user_creations.datetime as startdate,
    `values`.party_title, `values`.party_icon, `values`.name,
    elected_positions.name as ep_name,
    elected_positions.id as ep_level,
    elected_officials.approval_rating,
    elected_positions.energy_bonus as ep_energy_bonus,

    clan_members.is_clan_leader,
    clans.name as clan_name, clans.acronym as clan_acronym,
    clans.id as fkey_clans_id,
    event_points.points,

    neighborhoods.name as location

    FROM `users`

    LEFT JOIN `values` ON users.fkey_values_id = `values`.id

    LEFT OUTER JOIN elected_officials
    ON elected_officials.fkey_users_id = users.id

    LEFT OUTER JOIN elected_positions
    ON elected_positions.id = elected_officials.fkey_elected_positions_id

    LEFT OUTER JOIN clan_members on clan_members.fkey_users_id = users.id

    LEFT OUTER JOIN clans on clan_members.fkey_clans_id = clans.id

    LEFT JOIN user_creations on user_creations.phone_id = users.phone_id

    LEFT JOIN event_points on event_points.fkey_users_id = users.id

    LEFT JOIN neighborhoods on users.fkey_neighborhoods_id = neighborhoods.id

    ' . $where . '
    ORDER by user_creations.datetime ASC
    LIMIT 1';

  $result = db_query($sql, $id);
  $item = db_fetch_object($result);

//   if (empty($item->ep_name)) $item->ep_name = 'Subjugate';
  if (empty($item->ep_level)) $item->ep_level = 0;

  $user_cache[$id] = $item;

firep($item);
  return $item;

}


function NO_hierarchy_status($game_user, $level) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $ret = new StdClass();

  $sql = 'select * from elected_positions where id = %d;';
  $result = db_query($sql, $level);
  $req = db_fetch_object($result);

  $ret->target[0] =
    t('Attainment: <span>Level @level</span>',
      array('@level' => $req->min_level));
  $ret->progress[0] = t('Attainment: <span>Level @level</span>',
    array('@level' => $game_user->level));
  $ret->passed[0] = ($game_user->level >= $req->min_level);

  switch ($level) {

    case 0:

      $ret->qualified = $ret->passed[0];
      break;


    case 1:

      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Harvesting', 1);

      $ret->qualified = $ret->passed[0] & $ret->passed[1];
      break;


    case 2:

      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Cooking', 1);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Harvesting', 2);

      $ret->target[] =
        t('Favors Completed: <span>10</span>');
      $ret->progress[] =
        t('Favors Completed: <span>@c</span>',
        array('@c' => $game_user->favors_completed));
      $ret->passed[] = ($game_user->favors_completed >= 10);

      $ret->target[] =
        t('Initiated Favors Completed: <span>10</span>');
      $ret->progress[] =
        t('Initiated Favors Completed: <span>@c</span>',
        array('@c' => $game_user->favors_asked_completed));
      $ret->passed[] = ($game_user->favors_asked_completed >= 10);

      $ret->target[] =
        t('Initiated Favors Not Completed: <span>5</span>');
      $ret->progress[] =
        t('Initiated Favors Not Completed: <span>@c</span>',
        array('@c' => $game_user->favors_asked_noncompleted));
      $ret->passed[] = ($game_user->favors_asked_noncompleted >= 5);

      $ret->qualified = $ret->passed[0] & $ret->passed[1] & $ret->passed[2] &
        $ret->passed[3] & $ret->passed[4] & $ret->passed[5];
      break;


    case 3:

//      list($ret->target[], $ret->progress[], $ret->passed[]) =
//        _hierarchy_competency_check($game_user, 'Enriching', 1);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Purifying', 1);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Cooking', 2);
      list($ret->target[], $ret->progress[], $ret->passed[]) =
        _hierarchy_competency_check($game_user, 'Harvesting', 3);

      $ret->target[] = t('Clan membership: <span>TRUE</span>');
      $ret->progress[] = t('Clan membership: <span>@m (@c)</span>',
        array(
          '@m' => ($game_user->fkey_clans_id > 0) ? 'TRUE' : 'FALSE',
          '@c' => $game_user->clan_acronym,
        )
      );
      $ret->passed[] = ($game_user->debates_won >= 10);

      $ret->target[] = t('@debate Wins: <span>10</span>',
        array('@debate' => $debate));
      $ret->progress[] = t('@debate Wins: <span>@c</span>',
        array('@debate' => $debate, '@c' => $game_user->debates_won));
      $ret->passed[] = ($game_user->debates_won >= 10);

      $ret->target[] = t('@debate Losses: <span>5</span>',
        array('@debate' => $debate));
      $ret->progress[] = t('@debate Losses: <span>@c</span>',
        array('@debate' => $debate, '@c' => $game_user->debates_lost));
      $ret->passed[] = ($game_user->debates_lost >= 5);

      $ret->target[] =
        t('Favors Completed: <span>30</span>');
      $ret->progress[] =
        t('Favors Completed: <span>@c</span>',
        array('@c' => $game_user->favors_completed));
      $ret->passed[] = ($game_user->favors_completed >= 30);

      $ret->target[] =
        t('Initiated Favors Completed: <span>30</span>');
      $ret->progress[] =
        t('Initiated Favors Completed: <span>@c</span>',
        array('@c' => $game_user->favors_asked_completed));
      $ret->passed[] = ($game_user->favors_asked_completed >= 30);

      $ret->target[] =
        t('Initiated Favors Not Completed: <span>15</span>');
      $ret->progress[] =
        t('Initiated Favors Not Completed: <span>@c</span>',
        array('@c' => $game_user->favors_asked_noncompleted));
      $ret->passed[] = ($game_user->favors_asked_noncompleted >= 15);

      $ret->qualified = $ret->passed[0] & $ret->passed[1] & $ret->passed[2] &
        $ret->passed[3] & $ret->passed[4] & $ret->passed[5] &
        $ret->passed[6] & $ret->passed[7] & $ret->passed[8] &
        $ret->passed[9];
      break;

    default:

      $ret->target[] = t('Contemplate Inner Peace');
      $ret->progress[] = t('Not Yet');;
      $ret->passed[1] = FALSE;

      $ret->qualified = $ret->passed[0] & $ret->passed[1];

  }

  return $ret;

}


function NO_hierarchy_competency_check($game_user, $req_comp, $req_level = 1) {

  $check = game_competency_level($game_user, $req_comp);
  $target =
    t('@name Competency:<br/>' .
      '<span>@comp (Level @l)</span>',
      array(
        '@name' => $req_comp,
        '@comp' => competency_level_name($req_level),
        '@l' => $req_level,
      )
    );
  $progress =
    t('@name Competency:<br/><span>@comp (Level @l)</span>',
      array(
        '@name' => $req_comp,
        '@comp' => $check->level_name,
        '@l' => $check->level
      )
    );
  $passed = ($check->level >= $req_level);

  return array($target, $progress, $passed);

}


function show_equipment($game_user, $item, $options = array()) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  static $party_title;

  $description = str_replace('%clan', "<em>$party_title</em>",
    $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) $quantity = '<em>None</em>';

  $equipment_price = $item->price + ($item->quantity *
    $item->price_increase);

  if (!$item->is_loot) $ai_output .= " $item->id=$equipment_price";

  if (($equipment_price % 1000) == 0)
    $equipment_price = ($equipment_price / 1000) . 'K';

  if ($equipment_price === '0K') {
    $equipment_price = 'Free';
  }
  else {
    $equipment_price .= ' ' . $game_user->values;
  }

  if ($item->quantity_limit > 0) {
    $quantity_limit = '<em>(Limited to ' . $item->quantity_limit . ')</em>';
  }
  else {
    $quantity_limit = '';
  }

  if (strpos($_SERVER['REQUEST_URI'], 'equipment') !== FALSE) {
    $link = 'equipment';
  }
  else {
    $link = 'weapons';
  }

  $soon = ($options['soon']) ? 'soon' : '';
  $clan = ($options['clan']) ? 'clan' : '';

  if ($soon) {
    $icon = "$game-soon.png";
    $a_start = $a_end = '';
  }
  else {
    $icon = "$game-$item->id.png";
    $a_start = "<a href=\"/$game/{$link}_buy/$arg2/$item->id/1\">";
    $a_end = '</a>';
  }

  // Cannot buy.
  if ($clan | $item->is_loot) {
    $a_start = $a_end = '';
  }

  echo <<< EOF
<div class="land">
  <div class="land-icon">
    $a_start
      <img src="/sites/default/files/images/equipment/$icon"
        border="0" width="96"/>
    $a_end
  </div>
  <div class="land-details">
    <div class="land-name">
      $a_start
        $item->name
      $a_end
    </div>
    <div class="land-description">
      $description
    </div>
EOF;

  if (empty($soon)) echo <<< EOF
    <div class="land-owned">
      Owned: $quantity $quantity_limit
    </div>
EOF;

  echo <<< EOF
    <div class="land-requisites">
      // Requisites //
    </div>
EOF;

  if ($item->required_level > $game_user->level) echo <<< EOF
    <div class="land-required_level">
      Player: Level $item->required_level
    </div>
EOF;

  echo <<< EOF
    <div class="land-cost">
      Cost: $equipment_price
    </div>
EOF;

  if ($item->energy_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      Energy: +$item->energy_bonus immediate energy bonus
    </div>
EOF;

  }

  if ($item->energy_increase > 0) {

    echo <<< EOF
    <div class="land-payout">
      Energy: +$item->energy_increase every 5 minutes
    </div>
EOF;

  }

  if ($item->initiative_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $initiative: +$item->initiative_bonus
    </div>
EOF;

  }

  if ($item->endurance_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $endurance: +$item->endurance_bonus
    </div>
EOF;

  }

  if ($item->endurance_bonus < 0) {

    echo <<< EOF
    <div class="land-payout negative">
      $endurance: $item->endurance_bonus
    </div>
EOF;

  }

  if ($item->elocution_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $elocution: +$item->elocution_bonus
    </div>
EOF;

  if (!$item->is_loot) $ai_output .= "/+elo:$item->elocution_bonus";

  }

  if ($item->speed_increase > 0) {

    echo <<< EOF
    <div class="land-payout">
      Speed Increase: $item->speed_increase fewer Action
        needed to move to a new $hood_lower
    </div>
EOF;

  if ((!$item->is_loot) && ($item->quantity == 0))
    $ai_output .= "/+spd:$item->speed_increase";

  }

  if ($item->upkeep > 0) {

    echo <<< EOF
    <div class="land-payout negative">
      Upkeep: $item->upkeep every 60 minutes
    </div>
EOF;

  }

  if ($item->chance_of_loss > 0) {

    $lifetime = floor(100 / $item->chance_of_loss);
    $use = ($lifetime == 1) ? 'use' : 'uses';
    echo <<< EOF
    <div class="land-payout negative">
      Expected Lifetime: $lifetime $use
    </div>
EOF;

  }

  // Grab each action for the equipment.
  $data2 = array();
  $sql = 'select * from actions where fkey_equipment_id = %d;';
  $result = db_query($sql, $item->id);

  while ($action = db_fetch_object($result)) $data2[] = $action;

  foreach ($data2 as $action) {
firep($action);

    $cost = "Cost: $action->cost Action";
    if ($action->values_cost > 0)
      $cost .= ", $action->values_cost $game_user->values";

    $name = str_replace('%value', $game_user->values, $action->name);

    echo <<< EOF
    <div class="land-action">
      Action: $name
    </div>
    <div class="land-description">
      $action->description
    </div>
    <div class="land-action-cost">
      $cost
    </div>
EOF;

    if ($action->influence_change < 0) {

      $inf_change = -$action->influence_change;

      echo <<< EOF
    <div class="land-payout negative">
       Effect: Target's {$experience_lower} is reduced by $inf_change
    </div>
EOF;

    }

    if (($action->rating_change < 0.10) && ($action->rating_change != 0.0)) {

      $rat_change = abs($action->rating_change);

      if ($action->rating_change < 0.0) {

        echo <<< EOF
    <div class="land-payout negative">
      Effect: $target approval rating is reduced by $rat_change%
    </div>
EOF;

      }
      else {

        echo <<< EOF
    <div class="land-payout">
      Effect: $target approval rating is increased by $rat_change%
    </div>
EOF;

      }

    }

    if ($action->rating_change >= 0.10) {

      $rat_change = $action->rating_change;

      echo <<< EOF
    <div class="land-payout">
      Effect: Your approval rating is increased by $rat_change%
    </div>
EOF;

    }

    if ($action->neighborhood_rating_change < 0.0) {

      $rat_change = -$action->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout negative">
      Effect: Neighborhood $beauty_lower rating is reduced by $rat_change
    </div>
EOF;

    }

    if ($action->neighborhood_rating_change > 0.0) {

      $rat_change = $action->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout">
      Effect: Neighborhood $beauty_lower rating is increased by $rat_change
    </div>
EOF;

    }

    if ($action->values_change < 0) {

      $val_change = -$action->values_change;

      echo <<< EOF
    <div class="land-payout negative">
      Effect: Target's $game_user->values is reduced by $val_change
    </div>
EOF;

    }

  }

  echo '</div>';

  if (empty($soon) && empty($clan)) {

    if (!$item->is_loot) {

      echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button">
      <a href="/$game/{$link}_buy/$arg2/$item->id/1">
        Buy
      </a>
    </div>
EOF;

    }
    else {

      echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button not-yet">
      Can't Buy
    </div>
EOF;

    }

    if ($item->can_sell) {

      echo <<< EOF
    <div class="land-sell-button">
      <a href="/$game/{$link}_sell/$arg2/$item->id/1">
        Sell
      </a>
    </div>
  </div>
EOF;

    }
    else {

      echo <<< EOF
    <div class="land-sell-button not-yet">
      Can't Sell
    </div>
  </div>
EOF;

    }

  }

  echo '</div>';

}


function NO_equipment_check_wear_out(&$game_user, $id) {

  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);
  $failure_reason = '&nbsp;';
  $lost = FALSE;

  // Did it wear out?
  if ($eq->chance_of_loss >= mt_rand(1,110)) {
    // 110 instead of 100% to give a little extra chance of having it work.
firep($eq->name . ' wore out!');
    $lost = equipment_lose($game_user, $id, 1);

    $stuff = strtolower($eq->name);
    if (substr($stuff, 0, 2) == 'a ') $stuff = substr($stuff, 2);

    $sql = 'select message from equipment_failure_reasons
      where fkey_equipment_id = %d
      order by rand() limit 1;';
    $result = db_query($sql, $id);
    $failure = db_fetch_object($result);

    if ($failure->message != '') {

      $failure_reason = $failure->message;

    }
    else {

      $failure_reason = t('Your @stuff has/have worn out',
        array('@stuff' => $stuff));

    }

  }

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  return(array($lost, $failure_reason, $eo->quantity));

}


function send_user_message($from, $to, $private = 0, $message) {

  $sql = 'insert into user_messages (fkey_users_from_id,
    fkey_users_to_id, private, message) values (%d, %d, %d, "%s");';
  db_query($sql, $from, $to, $private, $message);

}

function game_zombie_name() {

  $first = array(
    'Angry',
    'Beastly',
    'Creepy',
    'Dreadful',
    'Eerie',
    'Ferocious',
    'Gruesome',
    'Heinous',
    'Infected',
    'Jaundiced',
    'Killer',
  ); // https://s-media-cache-ak0.pinimg.com/736x/d5/87/a4/d587a4917ff097a6ed2697e5b84dc4c7.jpg

  $last = array(
    'Quaint Stench',
    'Rottie',
    'Stinkbag',
    'Twitcher',
    'Undead',
    'Valkyrie',
    'Walker',
    'Xtra Creep',
    'Yard Bump',
    'Zero',
  );

  return 'Zombie ' . $first[mt_rand(0, count($first) - 1)] . ' '
    . $last[mt_rand(0, count($last) - 1)];

}

/**
 * Can I perform an action yet?
 *
 * @param \stdClass $game_user
 *   The user object.
 *
 * @param string $key
 *   The key to check.
 *
 * @return \stdClass
 *   An object with whether the action can be performed; if FALSE, also the
 *   hours, minutes, and seconds until action can be performed.
 */
function game_can_do_yet($game_user, $key) {
  global $game;

  $get_value = '_' . $game . '_get_value';
  $can_do = new stdClass();
  $can_do->time_ready = (int) $get_value($game_user->id, $key, time());
  $can_do->time_remaining = $can_do->time_ready - time();

  if ($can_do->time_remaining > 0) {
    $can_do->allowed = FALSE;
    $can_do->hours_remaining = floor($can_do->time_remaining / 3600);
    $minutes_remaining_in_sec = $can_do->time_remaining % 3600;
    $can_do->minutes_remaining = floor($minutes_remaining_in_sec / 60);
    $can_do->seconds_remaining = floor($minutes_remaining_in_sec % 60);
  }
  else {
    $can_do->allowed = TRUE;
  }

  return $can_do;
}

/**
 * Return the amount of money the game offers for one Luck.
 *
 * @param \stdClass $game_user
 *   The game's user object.
 *
 * @return int
 *   The offer.
 */
function game_luck_money_offer($game_user) {
  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $offer = ($game_user->income - $game_user->expenses) * 5;
  $offer = min($offer, $game_user->level * 1000);
  $offer = max($offer, $game_user->level * 100);

  if ($event_type == EVENT_DOUBLE_LUCK_MONEY) {
    $offer *= 2;
  }

  return $offer;
}
