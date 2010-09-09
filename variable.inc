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
  $info = module_invoke_all('variable_info', $options);
  foreach ($info as $name => $variable) {
    $variable += array('name' => $name);
    $variables += variable_build_variable($variable, $options);
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
function variable_build_variable($variable, $options) {
  // Set type defaults for each variable
  $variable += variable_type_defaults($variable['type']);
  if (isset($variable['build_callback'])) {
    return call_user_func($variable['build_callback'], $variable, $options);
  }
  else {
    return array($variable['name'] => $variable);
  }
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
    'form_callback' => 'variable_form_element_mail_text',
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
    'form_element' => array('#type' => 'radios'),
  )
  // Select multiple options from multiple choices
  $types['options'] = array(
    'form_element' => array('#type' => 'checkboxes'),
  );
  return $types;
}

/**
 * Build mail variables
 */
function variable_type_mail_build($variable, $options) {
  $build[$variable['name']] = $variable;
  $build[$variable['name'] . '_subject'] = array(
    'parent' => $variable['name'],
    'type' => 'string',
    'title' => t('Subject'),
    'default_value' => $variable['default_value']['subject'],
  );
  $build[$variable['name'] . '_body'] = array(
    'parent' => $variable['name'],
    'type' => 'text',
    'title' => t('Body'),
    'default_value' => $variable['default_value']['body'],
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

