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
