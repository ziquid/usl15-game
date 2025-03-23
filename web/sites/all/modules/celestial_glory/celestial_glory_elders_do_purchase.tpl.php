<?php

global $game, $phone_id, $purchasing_luck;

$fetch_user = '_' . arg(0) . '_fetch_user';
$fetch_header = '_' . arg(0) . '_header';

$purchasing_luck = TRUE;
$game_user = $fetch_user();

if (($_SERVER['REMOTE_ADDR'] == '66.211.170.66') ||
  ($_SERVER['REMOTE_ADDR'] == '173.0.81.1') ||
  ($_SERVER['REMOTE_ADDR'] == '173.0.81.33') ||
  ($_SERVER['REMOTE_ADDR'] == '173.0.82.126') || // <-- paypal sandbox
  (strpos($_SERVER['HTTP_USER_AGENT'], 'com.ziquid.celestialglory') !== FALSE) ||
  (strpos($_SERVER['HTTP_USER_AGENT'], 'com.cheek.celestialglory')
    !== FALSE)) {


  // IOS receipt data attached -- check it.
  if (arg(4) == 'withAppleReceipt') {

    $receipt_data = $_POST['receiptdata'];
    $receipt_json = json_encode(['receipt-data' => $receipt_data]);
    $appleURL = 'https://buy.itunes.apple.com/verifyReceipt';

    $params = ['http' => [
      'method' => 'POST',
      'content' => $receipt_json,
    ]];

    $ctx = stream_context_create($params);
    $fp = fopen($appleURL, 'rb', FALSE, $ctx);

    if (!$fp)
      mail('joseph@ziquid.com', 'unable to verify Apple receipt',
        'could not fopen() ' . $appleURL . '.');

    $response_json = stream_get_contents($fp);

    if ($response_json === FALSE)
      mail('joseph@ziquid.com', 'unable to verify Apple receipt',
        'could not read data from ' . $appleURL . 'due to error
' . $php_errormsg . '.');

    $response = json_decode($response_json);
ob_start();
var_dump($response);
$response_dump = ob_get_contents();
ob_end_clean();

    mail('joseph@ziquid.com', 'iOS receipt check response',
      'receipt_data is ' . $receipt_data .
      'response is: ' . $response_dump . '
response_json is: ' . $response_json . '
response status is: ' . $response->status);

    // Uhoh!  Receipt not validated!
    if ($response->status !== 0) {
      echo 'NO';
      exit;
    }

    if (substr($response->receipt->bid, 0, 11) !== 'com.ziquid.') {
      // Uhoh! Hack!  FIXME -- debit karma.
      echo 'NO';
      exit;
    }

  }

  $luck = 10;

  // FIXME: replace with switch().

  // Paypal.
  if (arg(3) == '30') $luck = 30;
  if (arg(3) == '35') $luck = 35;

  // Google.
  if (arg(3) == 'luck_35') $luck = 35;

  // Blackberry.
  if (arg(3) == 'buy_luck_35') $luck = 35;

  // Apple.
  if (arg(3) == 'com.ziquid.uslce.luck.35') $luck = 35;
  if (arg(3) == 'com.ziquid.celestialglory.luck.35') $luck = 35;
  if (arg(3) == 'com.ziquid.celestial_glory.luck.35') $luck = 35;

  // Blackberry.
  if (arg(3) == 'buy_luck_120') $luck = 120;

  // Paypal.
  if (arg(3) == '130') $luck = 130;
  if (arg(3) == '150') $luck = 150;

  // Google.
  if (arg(3) == 'luck.150') $luck = 150;

  // Apple.
  if (arg(3) == 'com.ziquid.uslce.luck.150') $luck = 150;
  if (arg(3) == 'com.ziquid.celestialglory.luck.150') $luck = 150;
  if (arg(3) == 'com.ziquid.celestial_glory.luck.150') $luck = 150;

  // Paypal.
  if (arg(3) == '320') $luck = 320;

  // Google.
  if (arg(3) == 'luck.320') $luck = 320;

  // Paypal.
  if (arg(3) == '700') $luck = 700;
  if (arg(3) == '1000') $luck = 1000;
  if (arg(3) == '4500') $luck = 4500;

  // Stop iOS luck hacking.
  if (arg(4) == 'abc123') $luck = 0;

  $msg = 'User ' . $game_user->username . ' purchased ' . $luck .
    ' Luck (currently ' . $game_user->luck . ') at URL ' .
    $_SERVER['REQUEST_URI'] . ' (IP Address ' . $_SERVER['REMOTE_ADDR']
    . ')';

  game_luck_use($game_user, $luck, $msg, 'purchase', '');
}
//  drupal_goto($game . '/elders/' . $phone_id);
// competency_gain($game_user, 'lucky', 3);

echo 'YES';
exit;
