<?php
/**
 * @file
 * Variable API module - Admin UI
 */

/**
 * Variable overview, by group
 */
function variable_admin_page_overview() {
  $display = array();
  foreach (variable_admin_build_groups() as $group => $variables) {
    $group_info = variable_group($group);
    $display[$group] = array(
      '#type' => 'fieldset',
      '#title' => $group_info['title'],
      '#collapsible' => TRUE, '#collapsed' => TRUE,
      '#children' => variable_admin_list($variables),
      '#description' => l(t('Edit group variables'), 'admin/settings/variable/group/' . $group),
    );
  }
  $output = drupal_render($display);
  $output .= drupal_get_form('variable_admin_reset_form');
  return $output;
}

/**
 * Variable group, edit all variables
 */
function variable_admin_page_group($group) {
  $groups = variable_admin_build_groups();
  if (!empty($groups[$group]) && $group_info = variable_group($group)) {
    drupal_set_title($group_info['title']);
    $variables = array_keys($groups[$group]);
    return drupal_get_form('variable_edit_form', $variables);
  }
  else {
    drupal_access_denied();
  }
}

/**
 * Build variable groups
 */
function variable_admin_build_groups() {
  $groups = array();
  foreach (variable_info() as $name => $variable) {
    if (empty($variable['parent'])) {
      $groups[$variable['group']][$name] = $variable;
    }
  }
  return $groups;
}

/**
 * Variable overview, by module
 */
function variable_admin_page_modules() {
  $groups = $display = array();
  foreach (variable_info() as $name => $variable) {
    if (empty($variable['parent'])) {
      $groups[$variable['module']][$name] = $variable;
    }
  }
  $modules = module_rebuild_cache();
  foreach ($groups as $module => $variables) {
    $display[$module] = array(
      '#type' => 'fieldset',
      '#title' => $modules[$module]->info['name'],
      '#collapsible' => TRUE, '#collapsed' => TRUE,
      '#children' => variable_admin_list($variables),
    );
  }
  $output = drupal_render($display);
  $output .= drupal_get_form('variable_admin_reset_form');
  return $output;
}

/**
 * Admin controls for the pages above
 */
function variable_admin_reset_form() {
  $form['controls'] = array('#type' => 'fieldset');
  $form['controls']['rebuild'] = array('#type' => 'submit', '#value' => t('Rebuild information'));
  return $form;
}

function variable_admin_reset_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  switch ($op) {
    case t('Rebuild information'):
      variable_cache_clear();
      break;
  }
}

/**
 * List of variables in db not declared by any module
 */
function variable_admin_page_undefined() {
  $variables = variable_info();
  $result = db_query("SELECT * FROM {variable} ORDER BY name");
  while ($variable = db_fetch_object($result)) {
    if (!isset($variables[$variable->name])) {
      $rows[] = array(
        $variable->name,
        check_plain($variable->value),
      );
    }
  }
  if (!empty($rows)) {
    $header = array(t('Name'), t('Value'));
    return theme('table', $header, $rows);
  }
  else {
    return t('No undefined variables found.');
  }
}

/**
 * Edit variable
 */
function variable_admin_page_edit($name) {
  drupal_set_breadcrumb(array(
    l(t('Administer'), 'admin'),
    l(t('Site configuration'), 'admin/settings'),
    l(t('Variables'), 'admin/settings/variable')
  ));
  $variable = variable_info($name);
  drupal_set_title($variable['title']);
  return drupal_get_form('variable_edit_form', $name);
}

/**
 * Print list of variables
 */
function variable_admin_list($list) {
  $header = array(t('Name'), t('Description'));
  $rows = array();
  foreach ($list as $name => $variable) {
    $rows[] = array(
      l($variable['title'], 'admin/settings/variable/edit/' . $name),
      $variable['description'],
    );
  }
  return theme('table', $header, $rows);
}