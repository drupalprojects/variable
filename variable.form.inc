<?php
// $Id$
/**
 * @file
 * Variable API module. Form library.
 */

/**
 * Build form element for a variable
 */
function variable_form_element($variable) {
  $variable = variable_build($variable);
  if (!empty($variable['element callback'])) {
    $element = call_user_func($variable['element callback'], $variable);
  }
  elseif (!empty($variable['multiple'])) {
    $element = variable_form_element_multiple($variable);
  }
  elseif (isset($variable['options'])) {
    $element = variable_form_element_options($variable);
  }
  else {
    $element = variable_form_element_default($variable); 
  }
  $element += array('#access' => variable_access($variable));
  return $element;
}

/**
 * Build multiple form element
 */
function variable_form_element_multiple($variable) {
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $element = $variable['element'] + array(
    '#type' => 'fieldset',
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  );
  foreach ($variable['children'] as $name => $item) {
    $element[$name] = variable_form_element($item);
  }
  return $element;
}

/**
 * Build default form element
 */
function variable_form_element_default($variable) {
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $type = variable_get_type($variable['type']) + array('element' => array());
  $element = $variable['element'] + array(
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  ) + $type['element'];
  $value = variable_get_value($variable);
  if (isset($value)) {
    $element['#default_value'] = $value;
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
