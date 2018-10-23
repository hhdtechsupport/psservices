<?php
/**
 * @file
 * FISMA User functions.
 */

/**
 * Menu Tab.
 * Display fisma_user settings.
 */
function fisma_user_logging_settings_form($form, &$form_state) {
  global $user;
  // Make sure there are no unwanted visitors.
  if (!user_access(FISMA_USER_ADMIN_PERM)) {
    drupal_access_denied();
  }

  $form = array();
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'fisma_user') . '/fisma_user.css'
    );

  $form['enable'] = array(
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#title' => t('Enable notifications'),
    '#attributes' => array('class' => array('container-inline')),
    '#default_value' => variable_get('fisma_user_notify_enable', 0),
    );

  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  asort($roles);

  $form['#roles'] = $roles;
  $funcs = array(
    'create' => t('Create'),
    'update' => t('Update'),
    'delete' => t('Delete'),
    );
  $form['#funcs'] = $funcs;
  $notifications = variable_get('fisma_user_notify_role', array());

  $form['hide'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles to Notify of Account Action'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'collapsed' => array(
        ':input[name="enable"]' => array('value' => '0'),
        ),
      ),
    );

  $form['hide']['notify'] = array(
    '#type' => 'table',
    '#header' => array(t('Role'), t('Count')),
    '#attributes' => array(
      'id' => 'fisma-user-settings-form',
      ),
    '#states' => array(
      'invisible' => array(
        ':input[name="enable"]' => array('value' => '0'),
        ),
      ),
    );
  foreach ($funcs as $func => $title) {
    $form['hide']['notify']['#header'][] = $title;
  }

  foreach ($roles as $rid => $name) {
    $row = 'rid_' . $rid;
    // Role name.
    $form['hide']['notify'][$row]['name'] = array(
      '#type' => 'item',
      '#markup' => t($name),
      );

    // Get count of users with this role.
    $query = "SELECT COUNT(*) FROM {users_roles} WHERE rid = :rid ";
    $count = db_query($query, array(':rid' => $rid))->fetchField();
    $form['hide']['notify'][$row]['count'] = array(
      '#type' => 'item',
      '#markup' => $count,
      );

    // Check box for each function.
    foreach ($funcs as $func => $title) {
      $form['hide']['notify'][$row][$func] = array(
        '#type' => 'checkbox',
        '#default_value' => (isset($notifications[$func]) ? in_array($rid, $notifications[$func]) : NULL),
        );
    }
  }

  $form['emails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email subjects and bodies'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('The role(s) chosen above will be notified when a user account is created, updated, or deleted.')
      . ' ' . t('The subject and body fields for each type of email are specified below.')
      . ' ' . t('The following substitutions are available:')
      . '<ul><li>' . t('@actor - the person who performed the action.')
      . '</li><li>' . t('!user - the link to the user account.')
      . '</li><li>' . t('@name - the name on the user account.')
      . '</ul>',
    '#states' => array(
      'collapsed' => array(
        ':input[name="enable"]' => array('value' => '0'),
        ),
      ),
    '#prefix' => '<br />',
    );

  $form['emails']['create'] = array(
    '#type' => 'fieldset',
    '#title' => t('User created'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['emails']['create']['fisma_user_create_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('User created subject'),
    '#default_value' => t(variable_get('fisma_user_create_subject', 'User created')),
    );

  $form['emails']['create']['fisma_user_create_body'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#title' => t('User created body'),
    '#default_value' => t(variable_get('fisma_user_create_body',
        'New user !user created.')),
    );

  $form['emails']['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('User updated'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['emails']['update']['fisma_user_update_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('User updated subject'),
    '#default_value' => t(variable_get('fisma_user_update_subject', 'User updated')),
    );

  $form['emails']['update']['fisma_user_update_body'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#title' => t('User updated body'),
    '#default_value' => t(variable_get('fisma_user_update_body',
        'User !user updated.')),
    );

  $form['emails']['delete'] = array(
    '#type' => 'fieldset',
    '#title' => t('User deleted'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['emails']['delete']['fisma_user_delete_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('User deleted subject'),
    '#default_value' => t(variable_get('fisma_user_delete_subject', 'User deleted')),
    );

  $form['emails']['delete']['fisma_user_delete_body'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#title' => t('User deleted body'),
    '#default_value' => t(variable_get('fisma_user_delete_body',
        'User !user deleted.')),
    );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    );
  $form['#tree'] = TRUE;

  return $form;
}

/**
 * Validation handler.
 */
function fisma_user_logging_settings_form_validate($form, &$form_state) {
  // Don't do anything if they are turning it off.
  if ($form_state['values']['enable'] == 0) {
    return;
  }

  $roles = $form['#roles'];
  $funcs = $form['#funcs'];

  $notify = array();
  foreach ($roles as $rid => $name) {
    $row = 'rid_' . $rid;
    foreach ($funcs as $func => $title) {
      if ($form_state['values']['hide']['notify'][$row][$func]) {
        $notify[$func][] = $rid;
      }
    }
  }

  $missing = array_diff_key($funcs, $notify);
  // Were they all deleted?
  if (count($missing) == count($funcs)) {
    return;
  }
  if ($missing) {
    foreach ($missing as $func => $title) {
//      drupal_set_message(t('You did not select a recipient for the "@func" alert email.',
//        array('@func' => $title)), 'error');
      form_set_error('notify', t('You did not select a recipient for the "@func" alert email.',
        array('@func' => $title)));
    }
  }
}

/**
 * Submission handler.
 */
function fisma_user_logging_settings_form_submit($form, &$form_state) {
//  dpm($form_state['values'], 'values');
  $roles = $form['#roles'];
  $funcs = $form['#funcs'];

  variable_set('fisma_user_notify_enable', $form_state['values']['enable']);

  // Are they turning it on?
  if ($form_state['values']['enable']) {
    $notify = array();
    foreach ($roles as $rid => $name) {
      $row = 'rid_' . $rid;
      foreach ($funcs as $func => $title) {
        if ($form_state['values']['hide']['notify'][$row][$func]) {
          $notify[$func][] = $rid;
        }
      }
    }

    variable_set('fisma_user_notify_role', $notify);
  }
  else {
    // Turning off.
    variable_del('fisma_user_notify_role');
  }

  variable_set('fisma_user_create_subject', $form_state['values']['emails']['create']['fisma_user_create_subject']);
  variable_set('fisma_user_create_body', $form_state['values']['emails']['create']['fisma_user_create_body']);
  variable_set('fisma_user_update_subject', $form_state['values']['emails']['update']['fisma_user_update_subject']);
  variable_set('fisma_user_update_body', $form_state['values']['emails']['update']['fisma_user_update_body']);
  variable_set('fisma_user_delete_subject', $form_state['values']['emails']['delete']['fisma_user_delete_subject']);
  variable_set('fisma_user_delete_body', $form_state['values']['emails']['delete']['fisma_user_delete_body']);

  drupal_set_message(t('Configuration saved.'));
}

/**
 * Administration settings form.
 * Note that the User module detects and logs these events,
 * but the Cron run to clean up the table is in the System module.
 * Events include an expiration time based on the window settings
 * and will expire at the end of that time.
 *
 * @see system_settings_form()
 */
function fisma_user_flood_control_settings_form($form, &$form_state) {
  // Make sure there are no unwanted visitors.
  if (!user_access(FISMA_USER_ADMIN_PERM)) {
    drupal_access_denied();
  }

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'fisma_user') . '/fisma_user.css'
    );

  $attempts = drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 75, 100, 125, 150, 200, 250, 500));
  $window = array(0 => t('None (disabled)'))
    + drupal_map_assoc(array(60, 180, 300, 600, 900, 1800, 2700, 3600, 7200, 10800, 21600, 32400, 43200, 86400), 'format_interval');


  // User module flood events.
  // This form simply sets the variables that will be used by the User module.
  $form['login'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Login Failures') . '</h2>',
    );

  $form['user_failed_login_identifier_uid_only'] = array(
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#title' => t('Register login failures by the user ID only'),
    '#default_value' => (int) variable_get('user_failed_login_identifier_uid_only', FALSE),
    '#description' => t('If "yes", then log failures based on the user ID only.')
      . ' ' . t('This is the most secure option.')
      . ' ' . t('If you choose "no" then the failure is logged based the user ID plus the IP address.')
      . ' ' . t('This is less secure but more resistant to denial-of-service attacks that could lock out all users.'),
        // The default identifier is a combination of uid and IP address. This
        // is less secure but more resistant to denial-of-service attacks that
        // could lock out all users with public user names.
    '#attributes' => array('class' => array('container-inline')),
    );

  $form['ip'] = array(
    '#type' => 'fieldset',
    '#title' => t('By IP Address'),
    '#collapsible' => FALSE,
    );

  $form['ip']['user_failed_login_ip_limit'] = array(
    '#type' => 'select',
    '#title' => t('Failed login (IP) attempts'),
    '#options' => $attempts,
    '#default_value' => variable_get('user_failed_login_ip_limit', 50),
    );

  $form['ip']['user_failed_login_ip_window'] = array(
    '#type' => 'select',
    '#title' => t('Failed login (IP) window'),
    '#options' => $window,
    '#default_value' => variable_get('user_failed_login_ip_window', 3600),
    );

  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('By Username'),
    '#collapsible' => FALSE,
    );

  $form['name']['user_failed_login_user_limit'] = array(
    '#type' => 'select',
    '#title' => t('Failed login (username) attempts'),
    '#options' => $attempts,
    '#default_value' => variable_get('user_failed_login_user_limit', 5),
    );

  $form['name']['user_failed_login_user_window'] = array(
    '#type' => 'select',
    '#title' => t('Failed login (username) window'),
    '#options' => $window,
    '#default_value' => variable_get('user_failed_login_user_window', 21600),
    );

  // There are settings for the Contact module as well, but it might not be enabled.
  if (module_exists('contact')) {
    // Contact module flood events.
    $form['contact'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contact forms'),
      );

    $form['contact']['contact_threshold_limit'] = array(
      '#type' => 'select',
      '#title' => t('Sending e-mails limit'),
      '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 75, 100, 125, 150, 200, 250, 500)),
      '#default_value' => variable_get('contact_threshold_limit', 5),
      );

    $form['contact']['contact_threshold_window'] = array(
      '#type' => 'select',
      '#title' => t('Sending e-mails window'),
      '#options' => array(0 => t('None (disabled)')) + drupal_map_assoc(array(60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400), 'format_interval'),
      '#default_value' => variable_get('contact_threshold_window', 3600),
      );
  }

  $form['footer'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('If an account or IP is locked out, it is unlocked again when Cron runs next after the appropriate window has elapsed.') . '</p>',
    );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    );

  return $form;
}

/**
 * Submission handler for the Flood Control form.
 */
function fisma_user_flood_control_settings_form_submit($form, &$form_state) {
//  dpm($form_state['values'], __FUNCTION__);

  variable_set('user_failed_login_identifier_uid_only', $form_state['values']['user_failed_login_identifier_uid_only']);
  variable_set('user_failed_login_ip_limit', $form_state['values']['user_failed_login_ip_limit']);
  variable_set('user_failed_login_ip_window', $form_state['values']['user_failed_login_ip_window']);
  variable_set('user_failed_login_user_limit', $form_state['values']['user_failed_login_user_limit']);
  variable_set('user_failed_login_user_window', $form_state['values']['user_failed_login_user_window']);

  if (module_exists('contact')) {
    variable_set('contact_threshold_limit', $form_state['values']['contact_threshold_limit']);
    variable_set('contact_threshold_window', $form_state['values']['contact_threshold_window']);
  }

  drupal_set_message(t('Configuration saved.'));
}

/**
 * Government warning settings form.
 */
function fisma_user_login_warning_settings_form($form, &$form_state) {
  // Make sure there are no unwanted visitors.
  if (!user_access(FISMA_USER_ADMIN_PERM)) {
    drupal_access_denied();
  }

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'fisma_user') . '/fisma_user.css'
    );

  $form['fisma_user_warning_on_login'] = array(
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#title' => t('Show warning on Login page'),
    '#default_value' => variable_get('fisma_user_warning_on_login', 0),
    '#description' => t('If "yes," then the warning text will be displayed on the Login page.')
      . ' ' . t('If "no," then you may still show the text with the block settings.'),
    '#attributes' => array('class' => array('container-inline')),
    );

  $form['fisma_user_warning_position'] = array(
    '#type' => 'radios',
    '#options' => array(-1000 => t('Top'), 1000 => t('Bottom')),
    '#title' => t('Position of warning on Login page'),
    '#default_value' => variable_get('fisma_user_warning_position', -1000),
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      // Hide the settings when the cancel notify checkbox is disabled.
      'invisible' => array(
        ':input[name="fisma_user_warning_on_login"]' => array('value' => 0),
        ),
      ),
    );

  $warning_text = variable_get('fisma_user_login_warning_text', NULL);

  $form['fisma_user_login_warning_text'] = array(
    '#type' => 'text_format',
    '#rows' => 20,
    '#title' => t('Government site warning text'),
    '#format' => $warning_text['format'] ? $warning_text['format'] : 'filtered_html',
    '#required' => TRUE,
    '#default_value' => $warning_text ? $warning_text['value'] : $warning_text,
    '#description' => t('This text will be displayed on login pages.'),
    );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    );

  return $form;
}

/**
 * Submission handler for the Flood Control form.
 */
function fisma_user_login_warning_settings_form_submit($form, &$form_state) {
//  dpm($form_state['values'], __FUNCTION__);

  variable_set('fisma_user_warning_on_login', $form_state['values']['fisma_user_warning_on_login']);
  variable_set('fisma_user_warning_position', $form_state['values']['fisma_user_warning_position']);
  variable_set('fisma_user_login_warning_text', $form_state['values']['fisma_user_login_warning_text']);

  drupal_set_message(t('Configuration saved.'));
}

/**
 * Inactive Users settings form.
 */
function fisma_user_inactive_users_settings_form($form, &$form_state) {
  // Make sure there are no unwanted visitors.
  if (!user_access(FISMA_USER_ADMIN_PERM)) {
    drupal_access_denied();
  }

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'fisma_user') . '/fisma_user.css'
    );

  $warning_text = variable_get('fisma_user_login_warning_text', NULL);

  if (module_exists('password_policy')) {
    $policies = db_query("SELECT p.pid, p.name, p.description, p.expiration, "
      . "(SELECT GROUP_CONCAT(r.name SEPARATOR ', ') FROM password_policy_role pr INNER JOIN role r ON r.rid = pr.rid WHERE pr.pid = p.pid) AS roles "
      . "FROM {password_policy} p "
      . "WHERE p.enabled = 1 ");
    $table = array(
      'header' => array(t('Policy'), t('Description'), t('Password Expiration'), t('Applies To')),
      'rows' => array(),
      'caption' => t('Password Policy'),
      'empty' => t('No active password policies were found.'),
      );
    foreach ($policies as $policy) {
      $table['rows'][] = array(
        l($policy->name, 'admin/config/people/password_policy/' . $policy->pid),
        check_plain($policy->description),
        number_format($policy->expiration) . ' ' . t('days'),
        ucwords(check_plain($policy->roles)),
        );
    }

    $policy_list = theme('table', $table);
  }
  else {
    $policy_list = '<p>' . t('FISMA requirements also strongly suggest installing the <a href="@module">Password Policy</a> module.',
      array('@module' => 'https://drupal.org/project/password_policy')) . '</p>';
  }

  $form['policy_list'] = array(
    '#type' => 'markup',
    '#markup' => $policy_list,
    );

  $form['fisma_user_log_deactivation'] = array(
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#title' => t('Log users who are deactivated'),
    '#default_value' => variable_get('fisma_user_log_deactivation', 0),
    '#description' => t('If selected, the deactivation action will be recorded in the system log.'),
    '#attributes' => array('class' => array('container-inline')),
    );

  $action_log = variable_get('fisma_user_notify_role', array());
  if (!empty($action_log)) {
    $form['fisma_user_log_deactivation']['#description'] .= ' '
      . t('Since the Activity Logging function seems to be enabled, this should not be needed.');
  }

  $form['fisma_user_inactive_users_interval'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Deactivate inactive user after'),
    '#field_suffix' => ' ' . t('months'),
    '#required' => TRUE,
    '#default_value' => variable_get('fisma_user_inactive_users_interval', 0),
    '#description' => t('If a user has not logged in for this length of time, deactivate their account.')
      . ' ' . t('An admin user may reactivate them.')
      . ' ' . t('Specify 0 (zero) to disable this feature.'),
    );

  $lifetime_options = array(
    0 => t('None'),
    1 => t('One month'),
    2 => t('Two months'),
    3 => t('Three months'),
    6 => t('Six months'),
    9 => t('Nine months'),
    12 => t('One year'),
    24 => t('Two years'),
    );

  $form['fisma_user_default_account_lifetime'] = array(
    '#type' => 'radios',
    '#options' => $lifetime_options,
    '#title' => t('Default account lifetime'),
    '#required' => TRUE,
    '#default_value' => variable_get('fisma_user_default_account_lifetime', 0),
    '#description' => t('This module adds an expiration date to the user account edit page.')
      . ' ' . t('This is the default lifetime of a user account.')
      . ' ' . t('An admin user may override this interval.')
      . ' ' . t('Specify "none" to not set a default.')
      . '<br />' . t('A lifetime will not be set until the user account is edited.')
    );

  $cron_options = array(
    0 => t('Never'),
    21600 => t('6 hours'),
    43200 => t('12 hours'),
    86400 => t('1 day'),
    172800 => t('2 days'),
    259200 => t('3 days'),
    604800 => t('1 week'),
    1209600 => t('2 weeks'),
    );

  $form['fisma_user_inactive_cron_interval'] = array(
    '#type' => 'radios',
    '#options' => $cron_options,
    '#title' => t('How often to check'),
    '#required' => TRUE,
    '#default_value' => variable_get('fisma_user_inactive_cron_interval', 86400),
    '#description' => t('Select an interval of time to check for accounts that should be disabled as configured above.')
      . ' ' . t('For more information, see the online handbook entry for <a href="@cron-handbook">configuring cron jobs</a>.',
        array('@cron-handbook' => 'http://drupal.org/cron')),
    '#states' => array(
      'invisible' => array(
        ':input[name="fisma_user_default_account_lifetime"]' => array('value' => '0'),
        ),
      ),
    );

  $cron_limits = array(
    0 => t('No limit'),
    10 => 10,
    25 => 25,
    50 => 50,
    75 => 75,
    100 => 100,
    150 => 150,
    200 => 200,
    );

  $form['fisma_user_inactive_cron_limit'] = array(
    '#type' => 'radios',
    '#options' => $cron_limits,
    '#title' => t('Catch up limit'),
    '#required' => TRUE,
    '#default_value' => variable_get('fisma_user_inactive_cron_limit', 100),
    '#description' => t('Occasionally this facility may be installed on a system that has a large number of inactive users.')
      . ' ' . t('Trying to deactivate all of them at once may result in excessive time being required.')
      . ' ' . t('This value will limit how many accounts are reviewed in a single run.')
      . ' ' . t('Additional accounts will be caught on subsequent runs.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="fisma_user_default_account_lifetime"]' => array('value' => '0'),
        ),
      ),
    );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    );

  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    );

  return $form;
}

/**
 * Submission handler for the Inactive Users settings form.
 */
function fisma_user_inactive_users_settings_form_submit($form, &$form_state) {
//  dpm($form_state, __FUNCTION__);

  if ($form_state['values']['op'] == $form_state['values']['reset']) {
    variable_del('fisma_user_log_deactivation');
    variable_del('fisma_user_inactive_users_interval');
    variable_del('fisma_user_default_account_lifetime');
    variable_del('fisma_user_inactive_cron_interval');
    variable_del('fisma_user_inactive_cron_limit');
    drupal_set_message(t('Configuration reset.'));
  }
  else {
    variable_set('fisma_user_log_deactivation', $form_state['values']['fisma_user_log_deactivation']);
    variable_set('fisma_user_inactive_users_interval', $form_state['values']['fisma_user_inactive_users_interval']);
    variable_set('fisma_user_default_account_lifetime', $form_state['values']['fisma_user_default_account_lifetime']);
    variable_set('fisma_user_inactive_cron_limit', $form_state['values']['fisma_user_inactive_cron_limit']);
    drupal_set_message(t('Configuration saved.'));
  }
}
