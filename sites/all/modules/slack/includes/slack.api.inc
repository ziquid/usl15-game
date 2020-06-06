<?php

/**
 * @file
 * Slack integration module API functions.
 */

/**
 * Send message to the Slack.
 *
 * @param string $message
 *   The message to send to the channel.
 * @param string $channel
 *   The channel in the Slack service to send the message.
 * @param string $username
 *   The bot name displayed in the channel.
 *
 * @return bool|object
 *   Slack response.
 */
function slack_send_message($message, $channel = '', $username = '') {
  $webhook_url = slack_get_webhook_url($channel);
  if (!$webhook_url) {
    return FALSE;
  }
  $message_options = [];
  if ($channel) {
    $message_options['channel'] = $channel;
  }
  else {
    $message_options['channel'] = slack_get_default_channel();
  }
  if ($username) {
    $message_options['username'] = $username;
  }
  else {
    $message_options['username'] = slack_get_default_username();
  }
  $result = _slack_send_message($webhook_url, $message, $message_options);
  return $result;
}

/**
 * Send message to the Slack with more options.
 *
 * @param string $webhook_url
 *   The URL to send the message to.
 * @param string $message
 *   The message sent to the channel.
 * @param array $message_options
 *   An associative array.  It can contain:
 *     - channel: The channel in the Slack service to send messages
 *     - username: The bot name displayed in the channel.
 *
 * @return object
 *   Can contain:
 *                          success      fail          fail
 *     - data:                ok         No hooks      Invalid channel specified
 *     - status message:      OK         Not found     Server Error
 *     - code:                200        404           500
 *     - error:               -          Not found     Server Error
 */
function _slack_send_message($webhook_url, $message, array $message_options = []) {
  $headers = [
    'Content-Type' => 'application/x-www-form-urlencoded',
  ];
  $message_options['text'] = $message;
  $sending_data = 'payload=' . drupal_to_js($message_options);
  $result = drupal_http_request($webhook_url, $headers, 'POST', $sending_data);
  return $result;
}

/**
 * Get Webhook URL for a given channel.
 *
 * @param string $channel
 *   The channel to which to send a message.
 *
 * @return string
 *   Get default Webhook URL.
 *
 * @todo Create a config page for these.
 */
function slack_get_webhook_url($channel) {
  $webhooks = [
    '#uslce' => 'TPRLET5NU/B01551Y6XPE/V1y9liW4o2lz615tkQ5rRRYI',
    '#uslce-bots' => 'TPRLET5NU/B014XF029V3/nktXgN3qcrb68UhRFXoUf4aH',
    '#uslce-competency-gain' => 'TPRLET5NU/B014J5HLAG7/bicbW7Il0SGtOF5kswFBERMT',
    '#uslce-cron' => 'TPRLET5NU/B014YV35PGS/sl0rtj5NJEeRRPMfQUAObwlw',
    '#uslce-loot' => 'TPRLET5NU/B015NMD0C8G/G6HIiQDruVJYmnKcgIYsR0zg',
    '#uslce-luck' => 'TPRLET5NU/B014J4905LP/HVr0oFE1vKtCEzLrud64KCNd',
    '#uslce-pages' => 'TPRLET5NU/B014J6QC4FR/i9NJ5SYNisAa1sVDXycGe8oE',
    '#uslce-ran-out-of' => 'TPRLET5NU/B014J5C8TGX/vWRKlUYeLFOHLp1Q37NKlD9X',
  ];
  if (array_key_exists($channel, $webhooks)) {
    $webhook = 'https://hooks' .
      '.slack.com' .
      '/services/' . $webhooks[$channel];
  }
  else {
    $webhook = variable_get('slack_webhook_url', NULL);
  }
  return $webhook;
}

/**
 * Get default Webhook URL.
 *
 * @return string
 *   Get default Webhook URL.
 */
function slack_get_default_webhook_url() {
  $webhook = variable_get('slack_webhook_url', NULL);
  return $webhook;
}

/**
 * Get default team channel.
 *
 * @return string
 *   Get default team channel
 */
function slack_get_default_channel() {
  $channel = variable_get('slack_channel', SLACK_DEFAULT_CHANNEL);
  if (empty($channel)) {
    $channel = SLACK_DEFAULT_CHANNEL;
  }
  return $channel;
}

/**
 * Get default Slack bot username.
 *
 * @return string
 *   Get default Slack bot username
 */
function slack_get_default_username() {
  $username = variable_get('slack_username', SLACK_DEFAULT_USERNAME);
  if (empty($username)) {
    $username = SLACK_DEFAULT_USERNAME;
  }
  return $username;
}
