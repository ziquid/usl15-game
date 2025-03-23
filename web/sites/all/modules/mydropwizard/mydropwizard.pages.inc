<?php

/**
 * @file
 * Page callbacks called by myDropWizard service.
 */

function mydropwizard_request_vitals_page() {
  if ($_SERVER['REQUEST_METHOD'] !== 'POST' || empty($_POST['payload_id'])) {
    drupal_set_header($_SERVER['SERVER_PROTOCOL'] . ' 400 Bad Request');
    return '<p>' . t("Bad request") . '</p>';
  }

  module_load_include('inc', 'mydropwizard', 'mydropwizard.data');
  mydropwizard_send_vitals_data($_POST['payload_id']);

  return '<p>' . t("Ok") . "</p>";
}

