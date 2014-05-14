<?php
/**
 * @file
 * Variable API module. Extended API.
 */

/**
 * Build variable information, which is cached by language
 */
function variable_build_info($options) {
  $options += array('language' => variable_language());
  variable_module_include();
  $variables = array();
  foreach (module_implements('variable_info') as $module) {
    $result = call_user_func($module . '_variable_info', $options);
    if (isset($result) && is_array($result)) {
      $defaults = array('module' => $module);
      foreach ($result as $name => $variable) {
        $variable_build = variable_build_variable($name, $variable, $options, $defaults);
        $variables += $variable_build;
      }
    }
  }

  // Last chance for modules to alter variable info
  drupal_alter('variable_info', $variables, $options);
  return $variables;
}

/**
 * Build variable groups information
 */
function variable_build_groups() {
  variable_module_include();
  $groups = module_invoke_all('variable_group_info');
  drupal_alter('variable_group_info', $groups);
  return $groups;
}

/**
 * Build single variable
 *
 * Some variables may spawn into multiple ones
 */
function variable_build_variable($name, $variable, $options, $defaults = array()) {
  // Set global defaults and type defaults for each variable
  $variable += $defaults;
  $variable += array('group' => 'others', 'name' => $name, 'description' => '');
  $variable += variable_type_defaults($variable['type']);
  // Add group information
  if (($group_info = variable_group($variable['group'])) && isset($group_info['access']) ) {
    $variable += array('access' => $group_info['access']);
  }
  if (isset($variable['build_callback'])) {
    $variable_build = call_user_func($variable['build_callback'], $variable, $options);
  }
  else {
    $variable_build = array($name => $variable);
  }
  // If the variable has children, we have to build each of them too
  if (isset($variable_build[$name]) && $children = array_diff(array_keys($variable_build), array($name))) {
    $variable_build[$name] += array('children' => $children);
    $defaults['group'] = $variable['group'];
    $defaults['parent'] = $name;
    foreach ($children as $key) {
      $variable_build = array_merge($variable_build, variable_build_variable($key, $variable_build[$key], $options, $defaults));
    }
  }
  return $variable_build;
}

/**
 * Get form element properties for a variable type
 */
function variable_type_defaults($type) {
  $values = &variable_static('variable_type_defaults');
  if (!$values) {
    variable_module_include();
    $values = module_invoke_all('variable_type_defaults');
    drupal_alter('variable_type_defaults', $values);
  }
  return isset($values[$type]) ? $values[$type] : array();
}

/**
 * Implementation of hook_variable_form_defaults()
 */
function variable_variable_type_defaults() {
  $types['string'] = array(
    'form_element' => array('#type' => 'textfield'),
    'localize' => TRUE,
  );
  $types['mail_address'] = array(
    'form_element' => array('#type' => 'textfield'),
  );
  $types['drupal_path'] = array(
    'form_element' => array('#type' => 'textfield', '#size' => 40),
  );
  $types['file_path'] = array(
    'form_element' => array('#type' => 'textfield', '#size' => 30, '#maxlength' => 255),
  );
  $types['text'] = array(
    'form_element' => array('#type' => 'textarea'),
    'localize' => TRUE,
  );
  $types['mail_text'] = array(
    //'form_callback' => 'variable_form_element_mail_text',
    'build_callback' => 'variable_mail_build',
    'multiple' => array('subject', 'body'),
    'localize' => TRUE,
  );
  $types['url'] = array(
    'form_element' => array('#type' => 'textfield', '#size' => 30, '#maxlength' => 255),
  );
  // Enable/Disable
  $types['enable'] = array(
    'form_element' => array('#type' => 'radios', '#options' => array(t('Disabled'), t('Enabled'))),
  );
  $types['number'] = array(
    'form_element' => array('#type' => 'textfield', '#size' => 15, '#maxlength' => 10),
  );
  // TRUE / FALSE value, checkbox
  $types['boolean'] = array(
    'form_element' => array('#type' => 'checkbox'),
  );
  // Select single option from multiple choices
  $types['select'] = array(
    'form_element' => array('#type' => 'select'),
    'form_callback' => 'variable_form_element_options',
  );
  // Select multiple options from multiple choices
  $types['options'] = array(
    'form_element' => array('#type' => 'checkboxes'),
    'form_callback' => 'variable_form_element_options',
  );
  return $types;
}

/**
 * Build mail variables
 */
function variable_mail_build($variable, $options) {
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
 * General function to include variable definitions for all modules
 */
function variable_module_include() {
  static $run = FALSE;

  if (!$run) {
    $run = TRUE;
    $modules_enabled = array_keys(module_list());
    $modules_included = array('system', 'node', 'user', 'taxonomy');
    foreach ($modules_enabled as $module) {
      if (in_array($module, $modules_included)) {
        module_load_include('variable.inc', 'variable', 'includes/' . $module);
      }
      else {
        module_load_include('variable.inc', $module);
      }
    }
  }
}

