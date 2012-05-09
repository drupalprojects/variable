<?php
/**
 * @file
 * Realm Admin - Pages and forms.
 */

// Include API
require_once drupal_get_path('module', 'variable_realm_admin') . '/variable_realm_admin.inc';

/**
 * Lists available realms.
 */
function variable_realm_admin_realm_overview($form, $form_state) {
  $header = array(
    t('Realm name'),
    t('Weight'),
    t('Options'),
  );

  $realms = array();
  foreach (variable_realm_info() as $realm => $info) {
    $options = array();
    // Add list of configured variables + configuration link
    if (!empty($info['select'])) {
      $options[] = l(t('Configure'), 'admin/config/system/variable/realm/' . $realm . '/configure');
      $options[] = l(t('Edit'), 'admin/config/system/variable/realm/' . $realm . '/edit');
    }
    // Get weight from realm controller.
    $controller = variable_realm_controller($realm);
    $realms[] = array(
      l($info['title'], 'admin/config/system/variable/realm/' . $realm),
      $controller['weight'],
      implode(' | ' , $options)
    );
  }

  $form['realms'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $realms,
    '#empty' => t('No realms available.'),
  );

  return $form;
}

/**
 * Lists available realms.
 */
function variable_realm_admin_realm_info($realm_name) {
  $info = variable_realm_info($realm_name);
  $build['title'] = array(
    '#type' => 'item',
    '#title' => t('Name'),
    '#markup' => $info['title'],
  );
  $build['keys'] = array(
    '#theme' => 'item_list',
    '#title' => t('Keys'),
    '#items' => variable_realm_keys($realm_name),
  );
  $build['variables'] = array(
    '#theme' => 'item_list',
    '#title' => t('Variables'),
    '#items' => variable_realm_admin_variables_conf($realm_name),
  );
  return $build;
}

/**
 * Edit variables for realm.
 */
function variable_realm_admin_realm_edit($realm_name) {
  module_load_include('form.inc', 'variable_realm');
  // @todo Build key selector (for now include first key)
  $realm_key = key(variable_realm_keys($realm_name));
  $build['form'] = drupal_get_form('variable_realm_edit_variables_form', $realm_name, $realm_key);
  return $build;
}
