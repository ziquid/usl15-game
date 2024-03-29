<?php

/**
 * @file
 * Code required only for the update status settings form.
 */

/**
 * Form builder for the update settings tab.
 */
function mydropwizard_settings() {
  $form = array();

  $notify_emails = variable_get('mydropwizard_notify_emails', array());
  $form['mydropwizard_notify_emails'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail addresses to notify when updates are available'),
    '#rows' => 4,
    '#default_value' => implode("\n", $notify_emails),
    '#description' => t('Whenever your site checks for available updates and finds new releases, it can notify a list of users via e-mail. Put each address on a separate line. If blank, no e-mails will be sent.'),
  );

  $form['mydropwizard_check_frequency'] = array(
    '#type' => 'radios',
    '#title' => t('Check for updates'),
    '#default_value' => variable_get('mydropwizard_check_frequency', 1),
    '#options' => array(
      '1' => t('Daily'),
      '7' => t('Weekly'),
    ),
    '#description' => t('Select how frequently you want to automatically check for new releases of your currently installed modules and themes.'),
  );

  $form['mydropwizard_notification_threshold'] = array(
    '#type' => 'radios',
    '#title' => t('E-mail notification threshold'),
    '#default_value' => variable_get('mydropwizard_notification_threshold', 'all'),
    '#options' => array(
      'all' => t('All newer versions'),
      'security' => t('Only security updates'),
    ),
    '#description' => t('You can choose to send e-mail only if a security update is available, or to be notified about all newer versions. If there are updates available of Drupal core or any of your installed modules and themes, your site will always print a message on the <a href="@status_report">status report</a> page, and will also display an error message on administration pages if there is a security update.', array('@status_report' => url('admin/reports/status')))
  );

  $form = system_settings_form($form);
  // Custom valiation callback for the email notification setting.
  $form['#validate'][] = 'mydropwizard_settings_validate';
  // We need to call our own submit callback first, not the one from
  // system_settings_form(), so that we can process and save the emails.
  unset($form['#submit']);

  return $form;
}

/**
 * Validation callback for the settings form.
 *
 * Validates the email addresses and ensures the field is formatted correctly.
 */
function mydropwizard_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['mydropwizard_notify_emails'])) {
    $valid = array();
    $invalid = array();
    foreach (explode("\n", trim($form_state['values']['mydropwizard_notify_emails'])) as $email) {
      $email = trim($email);
      if (!empty($email)) {
        if (valid_email_address($email)) {
          $valid[] = $email;
        }
        else {
          $invalid[] = $email;
        }
      }
    }
    if (empty($invalid)) {
      $form_state['notify_emails'] = $valid;
    }
    elseif (count($invalid) == 1) {
      form_set_error('mydropwizard_notify_emails', t('%email is not a valid e-mail address.', array('%email' => reset($invalid))));
    }
    else {
      form_set_error('mydropwizard_notify_emails', t('%emails are not valid e-mail addresses.', array('%emails' => implode(', ', $invalid))));
    }
  }
}

/**
 * Submit handler for the settings tab.
 */
function mydropwizard_settings_submit($form, $form_state) {
  $op = $form_state['values']['op'];

  if ($op == t('Reset to defaults')) {
    unset($form_state['notify_emails']);
  }
  else {
    if (empty($form_state['notify_emails'])) {
      variable_del('mydropwizard_notify_emails');
    }
    else {
      variable_set('mydropwizard_notify_emails', $form_state['notify_emails']);
    }
    unset($form_state['notify_emails']);
    unset($form_state['values']['mydropwizard_notify_emails']);
  }
  system_settings_form_submit($form, $form_state);
}
