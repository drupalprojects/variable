<?php
// $Id$
/**
 * @file
 * Variable API module. Extended API.
 */

/**
 * Build variable information, which is cached by language
 */
function variable_build_info($options) {
  variable_module_include();
  $variables = array();
  foreach (module_implements('variable_info') as $module) {
    $result = call_user_func($module . '_variable_info', $options);
    if (isset($result) && is_array($result)) {
      $defaults = array('module' => $module, 'group' => 'other', 'type' => 'default');
      foreach ($result as $name => $variable) {
        //$variable_build = variable_build_variable($name, $variable, $options, $defaults);
        $variables[$name] = $variable + array('name' => $name) + $defaults + variable_type_defaults($variable['type']);        
      }
    }
  }
  // Last chance for modules to alter variable infoa
  drupal_alter('variable_info', $variables, $options);
  return $variables;
}

/**
 * Build single variable
 * 
 * Some variables may spawn into multiple ones
 */
function variable_build($variable, $options = array()) {
  $options = variable_options($options);
  $variable = variable_variable($variable, $options);
  $variable += variable_get_type($variable['type']);
  if (isset($variable['options'])) {
    $variable = variable_build_options($variable, $options);
  }
  if (isset($variable['default callback'])) {
    $variable['default'] = call_user_func($variable['default callback'], $variable, $options);
  }
  return $variable;
}

/**
 * List variables for a group
 */
function variable_list_group($group) {
  $list = array();
  foreach (variable_get_info() as $name => $variable) {
    if ($variable['group'] == $group) {
      $list[$name] = $variable;
    }
  }
  return $list;
}

/**
 * List variables for a module
 */
function variable_list_module($module) {
  $list = array();
  foreach (variable_get_info() as $name => $variable) {
    if ($variable['module'] == $module) {
      $list[$name] = $variable;
    }
  }
  return $list;
}

/**
 * Get form element properties for a variable type
 */
function variable_type_defaults($type) {
  if (($info = variable_get_type($type)) && isset($info['defaults'])) {
    return $info['defaults'];
  } 
  else {
    return array();
  }
}

/**
 * Get variable group information
 * 
 * @param $name
 *   Group name. Will return all if not name.
 */
function variable_get_group($name = NULL) {
  $groups = &drupal_static('variable_groups');
  if (!isset($groups)) {
    variable_module_include();
    $groups = module_invoke_all('variable_group_info');
    drupal_alter('variable_group_info', $groups);
  }
  if ($name) {
    return isset($groups[$name]) ? $groups[$name] : NULL;
  }
  else {
    return $groups;
  }
}

/**
 * Variable type information
 */
function variable_get_type($type) {
  $info = &drupal_static(__FUNCTION__);
  if (!$info) {
    variable_module_include();
    $info = module_invoke_all('variable_type_info');
    drupal_alter('variable_type_info', $info);
  }
  if ($type) {
    return isset($info[$type]) ? $info[$type] : array();
  }
  else {
    return $info;
  }
}

/**
 * Build multiple variables
 */
function variable_build_multiple($variable) {
  $variable = variable_build($variable);
  $name = $variable['name'];
  $multiple = array();
  if (!empty($variable['multiple callback'])) {
    $multiple = call_user_func($variable['multiple callback'], $variable);
  }
  elseif (is_array($variable['multiple'])) {
    foreach ($variable['multiple'] as $key => $title) {
      $item_name = $name . '_' . $key;
      $multiple[$item_name] = array('multiple' => FALSE, 'title' => $title, 'key' => $key) + $variable;
    }
  }
  else {
    // No multiple variable just return the single variable
    $multiple[$name] = $variable;
  }
  // Build each variable again
  foreach ($multiple as $name => &$variable) {
    $variable['name'] = $name;
    $variable = variable_build($variable);
  }
  return $multiple;
}

/**
 * Build variable with multiple options
 */
function variable_build_options($variable, $options = array()) {
  if (!empty($variable['options']) && !is_array($variable['options'])) {
    variable_module_include();
    if (!empty($variable['options callback'])) {
      $variable['options'] = call_user_func($variable['options callback'], $variable, $options);
    }
    elseif ($options = module_invoke_all('variable_option_list', $variable, variable_options($options))) {
      $variable['options'] = $options;
    }
    else {
      $variable['options'] = array();
    }
  }
  return $variable;
}

/**
 * Build multiple mail variable
 */
function variable_build_mail_text($variable, $options = array()) {
  $name = $variable['name'];
  $variables[$name . '_subject'] = array(
    'type' => 'string',
    'title' => t('Subject'),
    'default' => $variable['default']['subject'],
  );
  $variables[$name . '_body'] = array(
    'type' => 'text',
    'title' => t('Body'),
    'default' => $variable['default']['body'],
  );
  return $variables;
}

/**
 * Build mail variables
 */
function variable_element_mail_text($variable, $options) {
  $build[$variable['name']] = $variable;
  $build[$variable['name'] . '_subject'] = array(
    'parent' => $variable['name'],
    'type' => 'string',
    'title' => t('Subject'),
    'default' => $variable['default']['subject'],
  );
  $build[$variable['name'] . '_body'] = array(
    'parent' => $variable['name'],
    'type' => 'text',
    'title' => t('Body'),
    'default' => $variable['default']['body'],
  );
  return $build;
}

/**
 * Format select variable
 */
function variable_format_option($value, $variable, $options = array()) {
  $variable = variable_build_options($variable);
  if (isset($value)) {
    return isset($variable['options'][$value]) ? $variable['options'][$value] : t('<Invalid option>');
  }
  else {
    return isset($variable['empty']) ? $variable['empty'] : t('Empty');
  }
}

/**
 * Format boolean variable
 */
function variable_format_boolean($value, $variable, $options = array()) {
  if (isset($value)) {
    return $value ? t('True') : t('False');
  }
  else {
    return t('Undefined');
  }
}
/**
 * Format mail variable
 */
function variable_format_mail_text($value, $variable, $options = array()) {
  return check_plain($value['subject']);
}

/**
 * General function to include variable definitions for all modules
 */
function variable_module_include() {
  static $included = NULL;
  static $modules = array('system', 'node', 'user', 'taxonomy');
  if (!$included) {
    $included = array();
    foreach ($modules as $module) {
      if (module_exists($module)) {
        $included[] = $module;
        module_load_include('variable.inc', 'variable', 'includes/' . $module);
      }
    }
  }
  return $included;
}

/**
 * Uninstall variables for module
 * 
 * This will be called from variable_modules_uninstalled(), no need to implement it directly.
 */
function variable_module_uninstall($module) {
  foreach (variable_list_module($module) as $variable) {
    foreach (variable_build_multiple($variable) as $name => $multiple) {
      variable_del($name);
    }
  }
}