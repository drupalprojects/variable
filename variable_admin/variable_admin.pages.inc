<?php
// $Id$
/**
 * @file
 * Variable API module - Admin UI
 */

/**
 * Variable overview, by group
 */
function variable_admin_page_overview() {
  $groups = $display = array();
  foreach (variable_info() as $name => $variable) {
    if (empty($variable['parent'])) {
      $variable += array('group' => t('Others'));
      $groups[$variable['group']][$name] = $variable; 
    }
  }
  foreach ($groups as $group => $variables) {
    $display[] = array(
      '#type' => 'fieldset',
      '#title' => $group,
      '#collapsible' => TRUE,
      '#children' => variable_admin_list($variables),
    );
  }
  $output = drupal_render($display);
  $output .= drupal_get_form('variable_admin_reset_form');
  return $output;
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
      '#collapsible' => TRUE,
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
function variable_admin_page_edit($variable) {
  drupal_set_title($variable['title']);
  return drupal_get_form('variable_edit_form', $variable);
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