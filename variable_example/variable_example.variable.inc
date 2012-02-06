<?php
/**
 * @file
 * Variable API module. Definition for some xample variables
 */

/**
 * Implements hook_variable_info().
 */
function variable_example_variable_info($options) {
  // Simple text
  $variables['variable_example_text'] = array(
    'type' => 'text',
    'title' => t('Simple text', array(), $options),
    'default' => 'Example text.',
    'description' => t('Example of text variable.', array(), $options),
    'required' => TRUE,
    'group' => 'variable_example',
  );
  // Text with format
  $variables['variable_example_text_format'] = array(
    'type' => 'text_format',
    'title' => t('Text format', array(), $options),
    'default' => 'Example text with default format',
    'description' => t('Example of text variable with text format.', array(), $options),
    'required' => TRUE,
    'group' => 'variable_example',
  );
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function variable_example_variable_group_info() {
  $groups['variable_example'] = array(
    'title' => t('Examples'),
    'description' => t('Variable examples of different types.'),
    'access' => 'administer site configuration',
    'path' => array('admin/config/system/variable/example'),
  );
  return $groups;
}