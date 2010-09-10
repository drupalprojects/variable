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
  return drupal_render($display);
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
  return drupal_render($display);
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