<?php
/**
 * @file
 * Admin interface for Entity Family.
 */

/**
 * Form for adding / editing a family entity.
 */
function purlieu_form($form, &$form_state, $purlieu = NULL) {
  if (is_null($purlieu)) {
    $form['is_new'] = array('#type' => 'value', '#value' => TRUE);
  }
  else {
    $form['#purlieu'] = $purlieu;
    $form_state['purlieu'] = $purlieu;
//    $form['entity_type'] = array('#type' => 'value', '#value' => 'purlieu');
    $form_state['entity_type'] = 'purlieu';
    $form['id'] = array('#type' => 'value', '#value' => $purlieu->id);
    $form['is_new_revision'] = array('#type' => 'value', '#value' => TRUE);
  }

  $form['name'] = array(
    '#title' => t('Purlieu Name'),
    '#type' => 'textfield',
    '#default_value' => isset($purlieu->name) ? "$purlieu->name" : '',
//    '#default_value' => NULL,
//    '#disabled' => isset($purlieu->name),
    '#description' => t('The purlieu will be identified by this name.
      It will be automatically determined if left blank (recommended).'),
    '#required' => TRUE,
    '#weight' => -99,
    );

  $form['organization'] = array(
    '#title' => t('Organization'),
    '#type' => 'textfield',
    '#default_value' => isset($purlieu->organization) ? $purlieu->organization : NULL,
//    '#disabled' => isset($purlieu->organization),
    '#description' => t('What company the person works for.'),
    '#required' => TRUE,
    '#weight' => -50,
    );

//  $info = field_info_field('field_type');

  $form['employee_type'] = array(
    '#title' => t('Employee Type'),
    '#type' => 'textfield',
    '#default_value' => isset($purlieu->employee_type) ? $purlieu->employee_type : NULL,
//    '#disabled' => isset($purlieu->employee_type),
    '#description' => t('What kind of employee this is.'),
    '#required' => TRUE,
    '#weight' => -45,
    );

  $form['location'] = array(
    '#title' => t('Location'),
    '#type' => 'textfield',
    '#default_value' => isset($purlieu->location) ? $purlieu->location : NULL,
//    '#disabled' => isset($purlieu->location),
    '#description' => t('To which office is this person attached.'),
    '#required' => TRUE,
    '#weight' => -40,
    );

  // Properties.
  $form['rent'] = array(
    '#title' => t('Rent'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The Rent overhead for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->rent) ? $purlieu->rent : 0,
    '#field_prefix' => '$',
    );

  $form['telecom'] = array(
    '#title' => t('Telecom'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The Telecom overhead for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->telecom) ? $purlieu->telecom : 0,
    '#field_prefix' => '$',
    );

  $form['vhs'] = array(
    '#title' => t('VHS'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The VHS overhead for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->vhs) ? $purlieu->vhs : 0,
    '#field_suffix' => '%',
    );

  $form['fringe'] = array(
    '#title' => t('Fringe'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The Fringe overhead for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->fringe) ? $purlieu->fringe : 0,
    '#field_suffix' => '%',
    );

  $form['indirect'] = array(
    '#title' => t('Indirect'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The Indirect Overhead rate for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->indirect) ? $purlieu->indirect : 0,
    '#field_suffix' => '%',
    );

  $form['fee'] = array(
    '#title' => t('Costs Fee'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The Fee overhead on Costs for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->fee) ? $purlieu->fee : 0,
    '#field_suffix' => '%',
    );

  $form['actfee'] = array(
    '#title' => t('Actuals Fee'),
    '#type' => 'textfield',
    '#size' => 12,
    '#description' => t('The Fee overhead on Actuals for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->actfee) ? $purlieu->actfee : 0,
    '#field_suffix' => '%',
    );

  $formulae = array(
    'standard' => t('Standard'),
    'westat' => t('Westat'),
    );

  $form['formula'] = array(
    '#title' => t('Formula'),
    '#type' => 'radios',
    '#options' => $formulae,
    '#description' => t('The formula to use for this purlieu.'),
    '#required' => TRUE,
    '#default_value' => isset($purlieu->formula) ? $purlieu->formula : 'standard',
    );

  field_attach_form('purlieu', $purlieu, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($purlieu->name) ? t('Update purlieu') : t('Save Purlieu'),
      ),
    'delete' => array(
      '#type' => 'submit',
      '#value' => t('Delete Purlieu'),
      ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel changes'),
      ),
    '#weight' => 100,
    );

  return $form;
}

/**
 * Theme the Settings form.
 */
function theme_purlieu_form($variables) {
  $output = '';

  $form = $variables['form'];

  $output .= drupal_render($form['name']);

  $rows = array();
  $rows[] = array(
    drupal_render($form['organization']),
    drupal_render($form['employee_type']),
    drupal_render($form['location']),
    );

  $output .= theme('table', array('rows' => $rows, 'attributes' => array('style' => 'width: auto;')));

  $rows = array();
  $rows[] = array(
    drupal_render($form['rent']),
    drupal_render($form['telecom']),
    );

  $rows[] = array(
    drupal_render($form['vhs']),
    drupal_render($form['fringe']),
    drupal_render($form['indirect']),
    drupal_render($form['fee']),
    drupal_render($form['actfee']),
    );

  $output .= theme('table', array('rows' => $rows, 'attributes' => array('style' => 'width: auto;')));

  $rows = array();
  $rows[] = array(
    drupal_render($form['formula']),
    );

  $output .= theme('table', array('rows' => $rows, 'attributes' => array('style' => 'width: auto;')));

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Validation handler for family entity form.
 */
function _purlieu_form_validate($form, &$form_state) {
  if (empty($form_state['values']['name'])) {
    form_set_error('name', t('You must select a name for this Purlieu.'));
  }
}

/**
 * Submit handler for family entity form.
 */
function purlieu_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case $form_state['values']['submit']:
      // If there is no name, create one.
      if (empty($form_state['values']['name'])) {
        $form_state['values']['name'] = $form_state['values']['organization']
          . ' - '
          . $form_state['values']['employee_type']
          . ' - '
          . $form_state['values']['location'];
      }
      $purlieu = entity_ui_form_submit_build_entity($form, $form_state);
      $purlieu->save();

      drupal_set_message(t('Purlieu "@id" has been saved.', array('@id' => $purlieu->name)));
      break;

    case $form_state['values']['delete']:
      $purlieu = $form['#purlieu'];
      $name = $purlieu->name;

      $purlieu->delete();

      drupal_set_message(t('Purlieu "@id" has been deleted.', array('@id' => $name)));
      break;

    case $form_state['values']['cancel']:
      break;
  }

  $form_state['redirect'] = 'admin/structure/purlieu';
}

/**
 * Page callback with list of all entities.
 */
function purlieu_list() {
  $return = array();
  $lines_per_page = 15;

  $header = array(
    array('data' => t('Name'), 'field' => 'name', 'sort' => 'ASC'),
    array('data' => t('Organization'), 'field' => 'organization'),
    array('data' => t('Employee Type'), 'field' => 'name'),
    array('data' => t('Location'), 'field' => 'name'),
    array('data' => t('Author'), 'field' => 'uid'),
    array('data' => t('Created'), 'field' => 'created'),
    array('data' => t('Revisions')),
  );

  $ids = db_select('purlieu', 'p')
    ->fields('p', array('id'))
    ->extend('PagerDefault')->limit($lines_per_page)
    ->extend('TableSort')->orderByHeader($header)
    ->execute()
    ->fetchCol();

  $rows = array();
  $purlieus = entity_load('purlieu', $ids);

  $count_query = "SELECT COUNT(*) - 1 FROM {purlieu_revision} WHERE id = :id";

  foreach ($purlieus as $purlieu) {
    $row = array();
    $user = user_load($purlieu->uid);
    $count = db_query($count_query, array(':id' => $purlieu->identifier()))->fetchField();

    $row[] = l($purlieu->name, $purlieu->path());
    $row[] = check_plain($purlieu->organization);
    $row[] = check_plain($purlieu->employee_type);
    $row[] = check_plain($purlieu->location);
    $row[] = theme('username', array('account' => $user));
    $row[] = format_date($purlieu->created, 'custom', 'M j, Y - g:i A');
    $row[] = ($count > 0 ? $count : '');
    $rows[] = $row;
  }

  $return['list'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    'empty' => t('There are no purlieus created.')
  );

  $return['pager'] = array('#theme' => 'pager');

  return $return;
}

/**
 * Page callback with list of all revisions.
 */
function purlieu_revision_overview($purlieu) {
  drupal_set_title($purlieu->name);

  $output = '';

  $list = array(
    'organization',
    'employee_type',
    'location',
    );

  $stuff = array();
  $h2 = '<h2 class="purlieu-identifying">';
  foreach ($list as $property) {
    $stuff[] = $purlieu->$property;
  }
  $h2 .= implode(' - ', $stuff);
  $h2 .= '</h2>';

  $output .= $h2;

  $table = array(
    'header' => array(
      t('Revision'),
      t('User'),
      t('Date'),
      t('Rent') . '<small> ($)</small>',
      t('Telecom') . '<small> ($)</small>',
      t('VHS') . '<small> (%)</small>',
      t('Fringe') . '<small> (%)</small>',
      t('Indirect') . '<small> (%)</small>',
      t('Costs Fee') . '<small> (%)</small>',
      t('Actuals Fee') . '<small> (%)</small>',
      ),
    'rows' => array(),
    'attributes' => array(
      'id' => 'purlieu-revisions',
      'class' => array('purlieu-revisions-' . $purlieu->id),
      ),
    );

  $query = db_select('purlieu_revision', 'pr')
    ->fields('pr')
    ->condition('id', $purlieu->id)
    ->orderBy('pr.changed', 'desc')
    ->execute();
  foreach ($query as $row) {
    $account = user_load($row->uid);

    $table['rows'][] = array(
      array('data' => $row->vid, 'align' => 'center'),
      format_username($account),
      format_date($row->changed),
      number_format($row->rent, 2),
      number_format($row->telecom, 2),
      number_format($row->vhs, 2),
      number_format($row->fringe, 2),
      number_format($row->indirect, 2),
      number_format($row->fee, 2),
      number_format($row->actfee, 2),
      );
  }

  $output .= theme('table', $table);
  return $output;
}

/**
 * Calculate with a given Purlieu.
 */
function purlieu_calculate($form, &$form_state, $purlieu = NULL) {
  drupal_set_title(t('Test Calculation with @name', array('@name' => $purlieu->name)));

  $form['#purlieu'] = $purlieu;
  $form_state['entity_type'] = 'purlieu';
  $form['id'] = array('#type' => 'value', '#value' => $purlieu->id);

  $form['rate'] = array(
    '#title' => t('Hourly Rate'),
    '#type' => 'textfield',
    '#required' => TRUE,
    );

  $form['hours'] = array(
    '#title' => t('Number of Hours'),
    '#type' => 'textfield',
    '#required' => TRUE,
    );

  $form['usefee'] = array(
    '#title' => t('Use Fee'),
    '#type' => 'radios',
    '#options' => array(t('Actuals'), t('Costs')),
    '#required' => TRUE,
    );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Submit'));

  return $form;
}

/**
 * Form submission handler for Calculate with a given Purlieu.
 */
function purlieu_calculate_submit($form, &$form_state) {
  $purlieu = $form['#purlieu'];

  $adjusted_rate = $purlieu->calculate($form_state['values']['rate'], $form_state['values']['hours'], $form_state['values']['usefee']);

  drupal_set_message(t('With a rate of $@rate and @hours hours, that comes to $@adjusted (@fee fee).',
    array(
      '@rate' => number_format($form_state['values']['rate'], 2),
      '@hours' => number_format($form_state['values']['hours'], 2),
      '@fee' => ($form_state['values']['usefee'] ? 'costs' : 'actuals'),
      '@adjusted' => number_format($adjusted_rate, 2),
      )));

  $form_state['rebuild'] = TRUE;
}
