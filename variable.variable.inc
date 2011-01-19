<?php 
// $Id$
/**
 * @file
 * Variable module hook implementations
 */

/**
 * Implements hook_variable_group_info().
 */
function variable_variable_group_info() {
  // Group for variable that have no group
  $groups['default'] = array(
    'title' => t('Other'),
    'description' => t('Variables that don\'t belong to any other group'),
  );
  return $groups;
}

/**
 * Implements hook_variable_type_info()
 */
function variable_variable_type_info() {
  // TRUE / FALSE value, checkbox
  $type['boolean'] = array(
    'title' => t('Boolean'),
    'element' => array('#type' => 'checkbox'),
    'format callback' => 'variable_format_boolean',
  );
  // Default type for variables with no other type
  $type['default'] = array(
    'title' => t('Default'),
    'element' => array('#type' => 'textfield'),
    'access' => 'administer site configuration',
  );
  $type['drupal_path'] = array(
    'title' => t('Drupal path'),
    'element' => array('#type' => 'textfield', '#size' => 40),
  );
  // Enable/Disable 
  $type['enable'] = array(
    'title' => t('Enable'),
    'options' => array(t('Disabled'), t('Enabled')),
    'element' => array('#type' => 'radios'),
    'format callback' => 'variable_format_option',
  );
  $type['file_path'] = array(
    'title' => t('File path'),
    'default' => conf_path() . '/files',
    'element' => array('#type' => 'textfield', '#maxlength' => 255, '#after_build' => array('system_check_directory')),
  );
  // Multiple variable that will spawn into multiple elements
  $type['multiple'] = array(
    'title' => t('Multiple'),
    'element' => array('#type' => 'fieldset'),
    'build callback' => 'variable_build_multiple',
    'format callback' => 'variable_format_multiple',
    'element callback' => 'variable_form_element_multiple',
    'value callback' => 'variable_multiple_get_value',
    'default callback' => 'variable_multiple_get_default',
  );
  $type['mail_address'] = array(
    'title' => t('E-mail address'),
    'element' => array('#type' => 'textfield'),
    'token' => TRUE,
  );
  $type['mail_text'] = array(
    'title' => t('Mail text'),
    'multiple' => array('subject' => t('Subject'), 'body' => t('Body')),
    'build callback' => 'variable_build_mail_text',
    'format callback' => 'variable_format_mail_text',
    'element callback' => 'variable_form_element_multiple',
    'localize' => TRUE,
  );
  $type['number'] = array(
    'title' => t('Number'),
    'element' => array('#type' => 'textfield', '#size' => 15, '#maxlength' => 10),
  );
  // Select multiple options from multiple choices
  $type['options'] = array(
    'title' => t('Options'),
    'element' => array('#type' => 'checkboxes'),
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
  );
  // Select single option from multiple choices
  $type['select'] = array(
    'title' => t('Select'),
    'element' => array('#type' => 'radios'),
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
  );
  $type['string'] = array(
    'title' => t('String'),
    'element' => array('#type' => 'textfield'),
    'localize' => TRUE,
    'token' => TRUE,
  );
  $type['text'] = array(
    'title' => t('Text'),
    'element' => array('#type' => 'textarea'),
    'localize' => TRUE,
    'format callback' => 'filter_xss_admin',
    'token' => TRUE,
  );
  $type['url'] = array(
    'title' => t('URL'),
    'element' => array('#type' => 'textfield', '#size' => 30, '#maxlength' => 255),
  );
  return $type;
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
 * Build multiple variables
 */
function variable_build_multiple($variable, $options) {
  $variable += array('children' => array(), 'multiple values' => array());
  if (is_array($variable['multiple'])) {
    $default = variable_get_default($variable, $options);
    foreach ($variable['multiple'] as $key => $title) {
      $name = $variable['name'] . '_' . $key;
      // Be careful to respect previously set properties, add but do not override.
      $child = isset($variable['children'][$name]) ? $variable['children'][$name] : array();
      $child += $variable['multiple values'];
      $child += array(
        'name' => $name, 'index' => $key, 'title' => $title, 
        'type' => 'default', 'parent' => $variable['name'], 'module' => $variable['module'],
        'default' => is_array($default) ? (isset($default[$key]) ? $default[$key] : NULL) : $default,
      );
      $child += variable_get_type($child['type']);
      $variable['children'][$name] = $child;
    }
  }
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
 * Format multiple variable
 */
function variable_format_multiple($value, $variable, $options = array()) {
  return t('<Multiple>', array(), $options);
}

/**
 * Format mail variable
 */
function variable_format_mail_text($value, $variable, $options = array()) {
  return check_plain($value['subject']);
}

/**
 * Get value for multiple variable
 */
function variable_multiple_get_value($variable, $options = array()) {
  $variable = variable_build($variable, $options);
  $values = array();
  foreach ($variable['children'] as $child) {
    $values[$child['index']] = variable_get_value($child, $options);
  }
  return $values;
}

/**
 * Get defaults for multiple variable
 */
function variable_multiple_get_default($variable, $options = array()) {
  $variable = variable_build($variable, $options);
  $values = array();
  foreach ($variable['children'] as $child) {
    $values[$child['index']] = variable_get_default($child, $options);
  }  
  return $values;
}