<?php

/**
 * @file stlouis_elders_do_purchase.tpl.php
 * Stlouis elders do purchase.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: done.
 */

global $game, $phone_id, $purchasing_luck;
include drupal_get_path('module', $game) . '/game_defs.inc';
$purchasing_luck = TRUE;
$game_user = $fetch_user();

$ip_address = ip_address();
if (($ip_address == '66.211.170.66') ||
  // Paypal sandboxes.
  ($ip_address == '173.0.81.1') ||
  ($ip_address == '173.0.81.33') ||
  ($ip_address == '173.0.82.126') ||
  (strpos($_SERVER['HTTP_USER_AGENT'], 'com.ziquid.uslce') !== FALSE)) {

  // IOS receipt data attached -- check it.
  if (arg(4) == 'withAppleReceipt') {

    $receipt_data = $_POST['receiptdata'];
    $receipt_json = json_encode(['receipt-data' => $receipt_data]);
    $appleURL = 'https://buy.itunes.apple.com/verifyReceipt';

    $params = [
      'http' => [
        'method' => 'POST',
        'content' => $receipt_json,
      ],
    ];

    $ctx = stream_context_create($params);
    $fp = fopen($appleURL, 'rb', FALSE, $ctx);

    if (!$fp) {
      mail('joseph@ziquid.com', 'unable to verify Apple receipt',
        'could not fopen() ' . $appleURL . '.');
    }

    $response_json = stream_get_contents($fp);

    if ($response_json === FALSE) {
      mail('joseph@ziquid.com', 'unable to verify Apple receipt',
        'could not read data from ' . $appleURL . 'due to error' .
        $php_errormsg . '.');
    }

    $response = json_decode($response_json);
ob_start();
var_dump($response);
$response_dump = ob_get_contents();
ob_end_clean();

    mail('joseph@ziquid.com', 'iOS receipt check response',
      'receipt_data is ' . $receipt_data .
      'response is: ' . $response_dump . 'response_json is: ' . $response_json .
      'response status is: ' . $response->status);

    // Uhoh!  Receipt not validated!
    if ($response->status !== 0) {
      echo 'NO';
      exit;
    }

    if (substr($response->receipt->bid, 0, 11) !== 'com.ziquid.') {
      // Uhoh! Hack!  FIXME -- debit karma.
      $karma = 1000;
      echo 'NO';
      exit;
    }
  }

  $luck = 10;
  $type = 'purchase';
  $subtype = '';

  // Paypal.
  if (arg(3) == '30') {
    $luck = 30;
    $subtype = 'paypal';
  }
  if (arg(3) == '35') {
    $luck = 35;
    $subtype = 'paypal';
  }

  // Google.
  if (arg(3) == 'luck_35') {
    $luck = 35;
    $subtype = 'google';
  }

  // Blackberry.
  if (arg(3) == 'buy_luck_35') {
    $luck = 35;
    $subtype = 'blackberry';
  }

  // Apple.
  if (arg(3) == 'com.ziquid.uslce.luck.35') {
    $luck = 35;
    $subtype = 'apple';
  }
  if (arg(3) == 'com.ziquid.celestialglory.luck.35') {
    $luck = 35;
    $subtype = 'apple';
  }
  if (arg(3) == 'com.ziquid.celestial_glory.luck.35') {
    $luck = 35;
    $subtype = 'apple';
  }

  // Blackberry.
  if (arg(3) == 'buy_luck_120') {
    $luck = 120;
    $subtype = 'blackberry';
  }

  // Paypal.
  if (arg(3) == '130') {
    $luck = 130;
    $subtype = 'paypal';
  }
  if (arg(3) == '150') {
    $luck = 150;
    $subtype = 'paypal';
  }

  // Google.
  if (arg(3) == 'luck.150') {
    $luck = 150;
    $subtype = 'google';
  }

  // Apple.
  if (arg(3) == 'com.ziquid.uslce.luck.150') {
    $luck = 150;
    $subtype = 'apple';
  }
  if (arg(3) == 'com.ziquid.celestialglory.luck.150') {
    $luck = 150;
    $subtype = 'apple';
  }
  if (arg(3) == 'com.ziquid.celestial_glory.luck.150') {
    $luck = 150;
    $subtype = 'apple';
  }

  // Paypal.
  if (arg(3) == '320') {
    $luck = 320;
    $subtype = 'paypal';
  }

  // Google.
  if (arg(3) == 'luck.320') {
    $luck = 320;
    $subtype = 'google';
  }

  // Paypal.
  if (arg(3) == '700') {
    $luck = 700;
    $subtype = 'paypal';
  }
  if (arg(3) == '1000') {
    $luck = 1000;
    $subtype = 'paypal';
  }
  if (arg(3) == '4500') {
    $luck = 4500;
    $subtype = 'paypal';
  }

  // Stop iOS luck hacking.
  if (arg(4) == 'abc123') {
    // Uhoh! Hack!  FIXME -- debit karma.
    $karma = 1000;
    echo 'NO';
    exit;
  }

  $msg = 'User ' . $game_user->username . ' purchased ' . $luck .
    ' Luck via ' . $subtype . ' (currently ' . $game_user->luck . ') at URL ' .
    $_SERVER['REQUEST_URI'] . ' (IP Address ' . $ip_address
    . ')';
  game_luck_use($game_user, $luck, $msg, 'purchase', $subtype);
}

echo 'YES';
exit;
