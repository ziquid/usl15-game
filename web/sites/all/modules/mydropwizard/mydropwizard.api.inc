<?php

/**
 * @file
 * Defines class for interacting with the myDropWizard API.
 */

class myDropWizardApi {

  const ENDPOINT_BASE = 'https://portal.mydropwizard.com/api/v1';

  /**
   * The authentication token.
   *
   * Can be one of three values:
   * - NULL if we haven't tried to get it yet
   * - FALSE if we tried and failed
   * - The token string
   *
   * @var null|bool|string
   */
  protected $token = NULL;

  /**
   * Checks that we have enough data to make an API call.
   */
  public function isReady() {
    $token = $this->getToken();
    return !empty($token);
  }

  /**
   * Gets the configuration necessary for interacting with the API.
   */
  protected function getConfiguration() {
    return array(
      'username' => variable_get('mydropwizard_username', ''),
      'password' => variable_get('mydropwizard_api_key', ''),
      'mode' => mydropwizard_get_submission_mode(),
      'site_id' => variable_get('mydropwizard_site_id', ''),
    );
  }

  /**
   * Checks that we have the necessary configuration.
   */
  protected function checkConfiguration($config) {
    if (empty($config['username']) || empty($config['password']) || empty($config['mode'])) {
      return FALSE;
    }
    if ($config['mode'] === 'push' && empty($config['site_id'])) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Gets the absolute API URL for a given path.
   */
  protected function getUrl($path) {
    return rtrim(variable_get('mydropwizard_api_endpoint', static::ENDPOINT_BASE), '/') . '/' . $path;
  }

  /**
   * Gets authorization token.
   */
  protected function getToken() {
    if ($this->token === NULL) {
      $config = $this->getConfiguration();
      if (!$this->checkConfiguration($config)) {
        watchdog('mydropwizard', 'Incomplete configuration - check your settings!', array(), WATCHDOG_ERROR);
        $this->token = FALSE;
      }
      else {
        $this->token = base64_encode("{$config['username']}:{$config['password']}");
      }
    }

    return $this->token;
  }

  /**
   * Makes a request to the API.
   *
   * @param string $path
   *   The relative path to the API.
   * @param array $options
   *   The request options, same as drupal_http_request() in Drupal 7.
   *
   * @return object
   *   The response, same as drupal_http_request().
   *
   * @throws \RuntimeException
   */
  public function request($path, $options = array()) {
    $options['headers']['Content-Type'] = 'application/json';
    $options['headers']['Accept'] = 'application/json';

    if ($token = $this->getToken()) {
      $options['headers']['Authorization'] = 'Basic ' . $token;
      return drupal_http_request($this->getUrl($path), $options['headers'], !empty($options['method']) ? $options['method'] : 'GET', !empty($options['data']) ? $options['data'] : NULL);
    }

    throw new \RuntimeException("Unable to connect to myDropWizard API");
  }

}

