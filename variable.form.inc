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
  $name = $variable['name'];
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
  foreach (variable_build_multiple($variable) as $name => $item) {
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
  return $variable['element'] + array(
    '#title' => $variable['title'],
    '#description' => $variable['description'],
    '#default_value' => variable_get_value($variable),
  ) + $type['element'];
}

/**
 * Build options variables
 */
function variable_form_element_options($variable) {
  $variable = variable_build_options($variable);
  $element = variable_form_element_default($variable);
  $element['#options'] = $variable['options'];
  return $element;
}