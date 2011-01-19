<?php
// $Id$
/**
 * @file
 * Variable API module. Form library.
 */

/**
 * Build form element for a variable
 */
function variable_form_element($variable, $options = array()) {
  $variable = variable_build($variable, $options);
  if (!empty($variable['element callback'])) {
    $element = call_user_func($variable['element callback'], $variable, $options);
  }
  elseif (isset($variable['options'])) {
    $element = variable_form_element_options($variable, $options);
  }
  else {
    $element = variable_form_element_default($variable, $options); 
  }
  $element += array('#access' => variable_access($variable));
  return $element;
}

/**
 * Build multiple form element
 */
function variable_form_element_multiple($variable, $options = array()) {
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $element = $variable['element'] + array(
    '#type' => 'fieldset',
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  );
  foreach ($variable['children'] as $name => $item) {
    $element[$name] = variable_form_element($item, $options);
  }
  return $element;
}

/**
 * Build default form element
 */
function variable_form_element_default($variable, $options = array()) {
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $type = variable_get_type($variable['type']) + array('element' => array());
  $element = $variable['element'] + array(
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  ) + $type['element'];
  $value = variable_get_value($variable, $options);
  if (isset($value)) {
    $element['#default_value'] = $value;
  }
  return $element;
}

/**
 * Build options variables
 */
function variable_form_element_options($variable, $options = array()) {
  $element = variable_form_element_default($variable, $options);
  $element['#options'] = $variable['options'];
  return $element;
}
