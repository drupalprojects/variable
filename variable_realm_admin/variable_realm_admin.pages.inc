<?php
/**
 * @file
 * Realm Admin - Pages and forms.
 */

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
      $controller->getDefaultWeight(),
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
  $controller = variable_realm_controller($realm_name);
  $build['title'] = array(
    '#type' => 'item',
    '#title' => t('Name'),
    '#markup' => $controller->getTitle(),
  );
  $build['keys'] = array(
    '#theme' => 'item_list',
    '#title' => t('Keys'),
    '#items' => $controller->getAllKeys(),
  );
  $build['variables'] = array(
    '#theme' => 'item_list',
    '#title' => t('Variables'),
    '#items' => $controller->getEnabledVariables(),
  );
  if ($controller->getInfo('select')) {
    $build['options'] = array(
      '#theme' => 'item_list',
      '#title' => t('Options'),
      '#items' => array(
        l(t('Configure'), 'admin/config/system/variable/realm/' . $realm_name . '/configure'),
        l(t('Edit'), 'admin/config/system/variable/realm/' . $realm_name . '/edit'),
      ),
    );
  }
  return $build;
}

/**
 * Edit variables for realm.
 */
function variable_realm_admin_realm_edit($realm_name) {
  module_load_include('form.inc', 'variable_realm');
  $realm_key = variable_realm_form_key_current($realm_name);
  $build['select'] = variable_realm_form_key_selector($realm_name, $realm_key);
  $build['form'] = drupal_get_form('variable_realm_edit_variables_form', $realm_name, $realm_key);
  return $build;
}
