<?php 
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
    'description' => t("Variables that don't belong to any other group."),
  );
  $groups['debug'] = array(
    'title' => t('Debug'),
    'description' => t('Debug and development options.'),
  );
  return $groups;
}

/**
 * Implements hook_variable_type_info().
 */
function variable_variable_type_info() {
  // Array of values
  $type['array'] = array(
    'title' => t('Array'),
    'element' => array('#type' => 'fieldset', '#tree' => TRUE),
    // Properties for each array item
    'repeat' => array(
      'element' => array('#type' => 'textfield'),
    ),
    'format callback' => 'variable_format_array',
    'element callback' => 'variable_form_element_array',
    'default' => array(),
  );
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
  // Enable/Disable 
  $type['enable'] = array(
    'title' => t('Enable'),
    'options' => array(t('Disabled'), t('Enabled')),
    'default' => 0,
    'element' => array('#type' => 'radios'),
    'format callback' => 'variable_format_option',
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
    'localize' => TRUE,
    'type' => 'multiple',
  );
  $type['number'] = array(
    'title' => t('Number'),
    'element' => array('#type' => 'textfield', '#size' => 15, '#maxlength' => 10),
  );
  // Select multiple options from multiple choices
  $type['options'] = array(
    'title' => t('Options'),
    'options' => TRUE,
    'element' => array('#type' => 'checkboxes'),
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
  );
  // Select single option from multiple choices
  $type['select'] = array(
    'title' => t('Select'),
    'options' => TRUE,
    // This will become radios or drop-down depending on the number of options
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
  );
  // Select number from array of values
  $type['select_number'] = array(
    'title' => t('Select'),
    'options' => TRUE,
    'element callback' => 'variable_form_element_options',
    'options callback' => 'variable_options_select_number',
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
 * Implements hook_variable_option_info()
 */
function variable_variable_option_info() {
  $options['mail_part'] = array(
    'title' => t('Mail parts'),
    'options' => array('subject' => t('Subject'), 'body' => t('Body')),
  );
  return $options;
}

/**
 * Build multiple mail variable
 */
function variable_build_mail_text($variable, $options = array()) {
  $name = str_replace('[mail_part]', '', $variable['name']);
  // For mail text, children have different types
  $variable['children'][$name . 'subject']['type'] = 'string';
  $variable['children'][$name . 'body']['type'] = 'text';
  $variable = variable_build_multiple($variable, $options);
  return $variable;
}

/**
 * Format select variable
 */
function variable_format_option($value, $variable, $options = array()) {
  $variable = variable_build_options($variable, $options);
  if (isset($value)) {
    return isset($variable['options'][$value]) ? $variable['options'][$value] : t('<Invalid option>');
  }
  else {
    return isset($variable['empty']) ? $variable['empty'] : t('Empty');
  }
}

/**
 * Format array variable, handling nested arrays
 */
function variable_format_array($value, $variable = NULL, $options = array()) {
  if (empty($value)) {
    return t('Empty');
  }
  else {
    $elements = array();
    foreach ($value as $item) {
      $elements[] = is_array($item) ? '(' . variable_format_array($item) . ')' : check_plain($item);
    }
    return implode(', ', $elements);
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
 * Options callback for numeric select
 */
function variable_options_select_number($variable, $options = array()) {
  return drupal_map_assoc($variable['options']);
}