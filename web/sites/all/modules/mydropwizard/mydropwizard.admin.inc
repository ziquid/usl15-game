<?php

/**
 * @file
 * Admin form for mydropwizard module.
 */

function mydropwizard_admin_settings(&$form_state) {
  $form = array();

  $form['mydropwizard_username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('mydropwizard_username', ''),
  );

  $form['mydropwizard_api_key'] = array(
    '#title' => t('API key'),
    '#type' => 'password',
    '#default_value' => variable_get('mydropwizard_api_key', ''),
  );

  $form['mydropwizard_submission_mode'] = array(
    '#title' => t('Submission mode'),
    '#type' => 'radios',
    '#options' => array(
      'pull' => t('Pull'),
      'push' => t('Push'),
    ),
    '#description' => t("Most sites use <em>Pull</em>; the <em>Push</em> mode is only necessary if the site isn't publicly accessible and requires a special support plan."),
    '#default_value' => mydropwizard_get_submission_mode(),
  );

  $form['mydropwizard_site_id'] = array(
    '#title' => t('Site ID'),
    '#type' => 'textfield',
    '#states' => array(
      'visible' => array(
        ':input[name="mydropwizard_submission_mode"]' => array('value' => 'push'),
      ),
    ),
    '#description' => t("The site ID in myDropWizard's system. Only necessary for sites using the <em>Push</em> submission mode."),
    '#default_value' => variable_get('mydropwizard_site_id', ''),
  );
  
  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'mydropwizard_admin_settings_submit');
  return $form;
}

function mydropwizard_admin_settings_submit($form, &$form_state) {
  // Prevent API key getting cleared when form is submitted again.
  if ($form_state['values']['mydropwizard_api_key'] === '') {
    unset($form_state['values']['mydropwizard_api_key']);
  }
}

