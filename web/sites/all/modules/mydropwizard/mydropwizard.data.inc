<?php

/**
 * @file
 * Functions for collecting and sending data to myDropWizard API.
 */

/**
 * Sends vitals data to myDropWizard.
 *
 * @param string $payload_id
 *   The payload id when using the 'pull' submission mode; NULL when using
 *   the 'push' submission mode.
 */
function mydropwizard_send_vitals_data($payload_id = NULL) {
  $api = mydropwizard_api();
  if (!$api->isReady()) {
    return;
  }

  $data = _mydropwizard_gather_vitals_data();

  if ($payload_id) {
    $response = $api->request('payload/' . $payload_id, array(
      'method' => 'PUT',
      'data' => json_encode($data),
    ));
  }
  else {
    $data['site_id'] = variable_get('mydropwizard_site_id', '');
    $response = $api->request('payload', array(
      'method' => 'POST',
      'data' => json_encode($data),
    ));
  }

  if ($response->code != 200 && $response->code != 201) {
    watchdog('mydropwizard', 'Error sending vitals: %code %error', array('%code' => $response->code, '%error' => $response->data), WATCHDOG_ERROR);
  }
  else {
    watchdog('mydropwizard', 'Vitals sent', array(), WATCHDOG_INFO);
  }
}

/**
 * Gathers all the vitals data.
 */
function _mydropwizard_gather_vitals_data() {
  $data = array();

  $system_data = _mydropwizard_get_raw_system_data();

  $data['vitals'] = array(
    'project' => _mydropwizard_gather_vitals_project_data($system_data),
    'module' => _mydropwizard_gather_vitals_module_data($system_data),
    'theme' => _mydropwizard_gather_vitals_theme_data($system_data),
    'library' => _mydropwizard_gather_vitals_library_data(),
    'requirement' => _mydropwizard_gather_vitals_requirement_data($system_data),
  );

  return $data;
}

/**
 * Gathers raw data from the system table.
 */
function _mydropwizard_get_raw_system_data($rebuild = TRUE) {
  if ($rebuild) {
    module_rebuild_cache();
    system_theme_data();
  }

  $result = db_query("SELECT name, type, status, info FROM {system}");
  while ($project = db_fetch_object($result)) {
    $project->info = unserialize($project->info);
    $projects[] = $project;
  }

  $projects[] = (object)array(
    'name' => 'drupal',
    'type' => 'core',
    'status' => 1,
    'info' => array(
      'core' => '6.x',
      'version' => VERSION,
      'project' => 'drupal',
      'hidden' => FALSE,
    ),
  );

  return $projects;
}

/**
 * Gathers the project vitals.
 */
function _mydropwizard_gather_vitals_project_data($system_data) {
  $projects = array();

  foreach ($system_data as $record) {
    if ($record->status || $record->type === 'theme') {
      $project_name = _mydropwizard_gather_vitals_get_project_name($record);
      $version = !empty($record->info['version']) ? $record->info['version'] : DRUPAL_CORE_COMPATIBILITY . '-1.x-dev';

      // Remove core compatibility from version numbers.
      if (strpos($version, DRUPAL_CORE_COMPATIBILITY . '-') === 0) {
        $version = substr($version, strlen(DRUPAL_CORE_COMPATIBILITY) + 1);
      }

      $projects[$project_name] = array(
        'name' => $project_name,
        'value' => $version,
      );
    }
  }

  return array_values($projects);
}

/**
 * Gets the project name for an extension.
 */
function _mydropwizard_gather_vitals_get_project_name($record) {
  if (!empty($record->info['project'])) {
    return $record->info['project'];
  }
  if (isset($record->info['package']) && (strpos($record->info['package'], 'Core') === 0)) {
    return 'drupal';
  }
  if (in_array($record->name, array('bluemarine', 'chameleon', 'garland', 'marvin', 'minnelli', 'pushbutton'))) {
    // Unfortunately, there's no way to tell if a theme is part of core,
    // so we must hard-code a list here.
    return 'drupal';
  }
  // If we have no other ideas, use the module name.
  return $record->name;
}

/**
 * Gathers the module vitals.
 */
function _mydropwizard_gather_vitals_module_data($system_data) {
  $modules = array();

  foreach ($system_data as $record) {
    if ($record->type == 'module' && empty($record->info['hidden'])) {
      $modules[$record->name] = array(
        'name' => $record->name,
        'value' => !empty($record->status) ? 'enabled' : 'disabled',
      );
    }
  }

  return array_values($modules);
}

/**
 * Gathers the theme vitals.
 */
function _mydropwizard_gather_vitals_theme_data($system_data) {
  $themes = array();

  foreach ($system_data as $record) {
    if ($record->type == 'theme' && empty($record->info['hidden'])) {
      $themes[$record->name] = array(
        'name' => $record->name,
        'value' => $record->status ? 'enabled' : 'disabled',
      );
    }
  }

  // @todo Make sure to mark the admin theme as enabled!

  return array_values($themes);
}

/**
 * Gathers the library vitals.
 */
function _mydropwizard_gather_vitals_library_data() {
  return array();
}

/**
 * Gathers the requirement vitals.
 */
function _mydropwizard_gather_vitals_requirement_data($system_data) {
  static $requirement_severity_map = NULL;

  // Load for the REQUIREMENT_* constants and support functions.
  require_once './includes/install.inc';

  if ($requirement_severity_map === NULL) {
    $requirement_severity_map = array(
      REQUIREMENT_INFO => 'info',
      REQUIREMENT_OK => 'ok',
      REQUIREMENT_WARNING => 'warning',
      REQUIREMENT_ERROR => 'error',
    );
  }

  // Call hook_requirements() for all enabled modules.
  $raw_requirements = array();
  foreach ($system_data as $record) {
    if ($record->status && $record->type == 'module') {
      module_load_install($record->name);
      $function_name = $record->name . '_requirements';
      if (function_exists($function_name)) {
        $module_raw_requirements = $function_name('runtime');
        if (is_array($module_raw_requirements) && !empty($module_raw_requirements)) {
          $raw_requirements = array_merge($raw_requirements, $module_raw_requirements);
        }
      }
    }
  }

  // Cleanup the PHP version (Drupal puts some HTML in there).
  $raw_requirements['php']['value'] = phpversion();

  // Convert to the vital format for mydropwizard.
  $requirements = array();
  foreach ($raw_requirements as $name => $raw_requirement) {
    $requirement = array(
      'name' => (string) $name,
      'value' => (string) $raw_requirement['value'],
    );

    if (!empty($raw_requirement['severity'])) {
      $requirement['status'] = $requirement_severity_map[$raw_requirement['severity']];
    }
    else {
      $requirement['status'] = 'info';
    }

    $requirements[] = $requirement;
  }

  return $requirements;
}

