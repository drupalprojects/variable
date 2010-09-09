<?php
/**
 * @file
 * Variable API module. Form library.
 */

/**
 * Build form element for variable
 */
function variable_form_element($name, $variable) {
  if ($callback = variable_type_defaults($variable['type'], 'form_callback')) {
    return call_user_func($callback, $name, $variable);
  }
  else {
    $element = isset($variable['form_element']) ? $variable['form_element'] : array();
    $element += array(
      '#title' => $variable['name'],
      '#default_value' => variable_get($name, $variable['default_value']),
      '#description' => isset($variable['description']) ? $variable['description'] : '', 
    );  
    $element += variable_type_defaults($variable['type'], 'form_element', array());
    return $element;
  }
}

/**
 * Format mail text variables ('subject', 'body')
 */
function variable_form_element_mail_text($name, $variable) {
  $element = array(
    '#type' => 'fieldset',
    '#title' => $variable['name'],
    '#description' => $variable['description'],
    '#collapsible' => TRUE, '#collapsed' => TRUE,
  );
  $element[$name . '_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get($name . '_subject', $variable['default_value']['subject']),
    '#maxlength' => 180,  
  );
  $element[$name . '_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' =>  variable_get($name . '_body', $variable['default_value']['body']),
    '#rows' => 15,  
  );  
  
  return $element;
}