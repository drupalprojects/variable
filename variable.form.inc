<?php
/**
 * @file
 * Variable API module. Form library.
 */


/**
 * Build form element for a variable
 */
function variable_form_element($variable) {
  if (!empty($variable['form_callback'])) {
    $element = call_user_func($variable['form_callback'], $variable);
  }
  elseif (!empty($variable['children'])) {
    $element = variable_form_element_multiple($variable);
  }
  else {
    $element = variable_form_element_default($variable);
  }
  if (!empty($variable['access'])) {
    $element['#access'] = $variable['access'];
  }
  return $element;
}

/**
 * Build multiple form element
 */
function variable_form_element_multiple($variable) {
  $element = isset($variable['form_element']) ? $variable['form_element'] : array();
  $element += array(
    '#type' => 'fieldset',
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  );
  // Add child elements
  foreach ($variable['children'] as $key) {
    if ($info = variable_info($key)) {
      $element[$key] = variable_form_element($info);
    }
  }
  return $element;
}

/**
 * Build default form element
 */
function variable_form_element_default($variable) {
  $element = isset($variable['form_element']) ? $variable['form_element'] : array();
  $element += array(
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  );
  if (!isset($element['#default_value'])) {
    $element['#default_value'] = variable_get_value($variable['name']);
  }
  return $element;
}

/**
 * Build options variables
 */
function variable_form_element_options($variable) {
  $element = variable_form_element_default($variable);
  $element['#options'] = $variable['options'];
  return $element;
}