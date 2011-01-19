<?php
// $Id$
/**
 * @file
 * Variable API module. Extended API.
 */

/**
 * Build generic variable information
 */
function variable_build_info($type, $options) {
  variable_module_include();
  switch ($type) {
    case 'variable':
      return variable_build_list_info($options);
    case 'group':
      $info = variable_invoke_all('variable_group_info');
      drupal_alter('variable_group_info', $info);
      return $info;
    case 'type':
      $info = variable_invoke_all('variable_type_info');
      drupal_alter('variable_type_info', $info);
      return $info;
  }  
}

/**
 * Build variable information
 */
function variable_build_list_info($options) {
  $variables = array();
  foreach (module_implements('variable_info') as $module) {
    $result = call_user_func($module . '_variable_info', $options);
    if (isset($result) && is_array($result)) {
      foreach ($result as $name => $variable) {
        $variable += array('name' => $name, 'module' => $module, 'group' => 'default', 'type' => 'default');
        $variables[$name] = $variable + variable_get_type($variable['type']);      
      }
    }
  }
  // Last chance for modules to alter variable infoa
  drupal_alter('variable_info', $variables, $options);
  return $variables;
}

/**
 * Invoke hook on all modules, adding 'module' property
 */
function variable_invoke_all() {
  $args = func_get_args();
  $hook = $args[0];
  unset($args[0]);
  $return = array();
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    if (function_exists($function)) {
      $result = call_user_func_array($function, $args);
      if (isset($result) && is_array($result)) {
        foreach ($result as $key => $value) {
          $return[$key] = $value + array('module' => $module);
        }
      }
    }
  }
  return $return;
}

/**
 * Build single variable
 * 
 * Some variables may spawn into multiple ones
 */
function variable_build_variable($variable, $options = array()) {
  if (!empty($variable['built'])) {
    return $variable;
  }
  $options = _variable_options($options);
  $variable = _variable_variable($variable, $options);
  $type = variable_get_type($variable['type']);
  // Add type properties to variable
  $variable += $type;
  // Some of the callback may be in the module's variable.inc file
  variable_module_include($variable['module']);
  variable_module_include($type['module']);
  // Invoke variable callbacks
  foreach (array('options', 'multiple', 'default') as $key) {
    if (!empty($variable[$key . ' callback'])) {
      $variable[$key] = call_user_func($variable[$key . ' callback'], $variable, $options);
    }
  }
  if (isset($variable['build callback'])) {
    $variable = call_user_func($variable['build callback'], $variable, $options);
  }
  elseif (!empty($variable['multiple'])) {
    $variable = variable_build_multiple($variable, $options);
  }
  $variable['built'] = TRUE;
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
 * Build multiple variables
 */
function variable_build_multiple($variable, $options) {
  $variable += array('children' => array());
  if (is_array($variable['multiple'])) {
    $default = variable_get_default($variable, $options);
    foreach ($variable['multiple'] as $key => $title) {
      $name = $variable['name'] . '_' . $key;
      $variable['children'][$name] = array(
        'name' => $name, 'index' => $key, 'title' => $title, 
        'type' => $variable['type'], 'parent' => $variable['name'], 'module' => $variable['module'],
        'default' => is_array($default) ? (isset($default[$key]) ? $default[$key] : NULL) : $default,
      );
    }
  }
  return $variable;
}

/**
 * Fetch options for variable
 */
function variable_option_list($variable, $options) {
  variable_module_include();
  return module_invoke_all('variable_option_list', $variable, $options);
}

/**
 * Build multiple mail variable
 */
function variable_build_mail_text($variable, $options = array()) {
  $name = $variable['name'];
  $variable = variable_build_multiple($variable, $options);
  // For mail text, children have different types
  $variable['children'][$name . '_subject']['type'] = 'string';
  $variable['children'][$name . '_body']['type'] = 'text';
  return $variable;
}

/**
 * Format select variable
 */
function variable_format_option($value, $variable, $options = array()) {
  $variable = variable_build($variable, $options);
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
function variable_module_include($modules = NULL) {
  static $core_modules = array('system', 'node', 'user', 'taxonomy');
  static $included = array();
  $modules = $modules ? (is_array($modules) ? $modules : array($modules)) : $core_modules;
  foreach ($modules as $module) {
    if (!isset($included[$module])) {
      if (module_exists($module)) {
        if (in_array($module, $core_modules)) {
          $included[$module] = module_load_include('variable.inc', 'variable', 'includes/' . $module);
        }
        else {
          $included[$module] = module_load_include('variable.inc', $module);
        }
      }
    }
  }
}

/**
 * Uninstall variables for module
 * 
 * This will be called from variable_modules_uninstalled(), no need to implement it directly.
 */
function variable_module_uninstall($module) {
  foreach (variable_list_module($module) as $name => $variable) {
    variable_delete($variable);
  }
}