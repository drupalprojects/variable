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
 * Build array form element
 */
function variable_form_element_array($variable, $options = array()) {
  // This will be possibly a textfield with tree value
  $element = variable_form_element_default($variable, $options);
  // We may have a multiple element base that will default to plain textfield
  $item = $variable['repeat'];
  $value = variable_get_value($variable, $options);
  if (!empty($variable['multiple'])) {
    // If we've got a multiple value, it will be an array with known elements
    foreach ($variable['multiple'] as $key => $title) {
      $element[$key] = $item['element'] + array('#title' => $title, '#default_value' => isset($value[$key]) ? $value[$key] : '');
    }
  }
  else {
    // Undefined array, add all the elements plus a blank field
    if (!in_array('', $value)) {
      $value[] = '';
    }
    foreach ($value as $key => $item_value) {
      $element[$key] = $item['element'] + array('#default_value' => isset($value[$key]) ? $value[$key] : '');
    }
  }
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

/**
 * Form to select variables
 */
function theme_variable_table_select($variables) {
  $element = $variables['element'];
  $header = isset($element['#header']) ? $element['#header'] : array('element' => '', 'title' => t('Name'), 'description' => t('Description'));
  $fields = array_keys($header);
  $rows = array();
  foreach (element_children($element) as $name) {
    $variable = _variable_variable($name);
    $row = array();
    foreach ($fields as $field) {
      if ($field == 'element') {
        $row[] = drupal_render($element[$name]);
      }
      else {
        $row[] = isset($variable[$field]) ? $variable[$field] : '';
      }
    }
    $rows[] = $row;
  }
  // Add a "Select all" checkbox.
  drupal_add_js('misc/tableselect.js');
  $header['element'] = array('class' => array('select-all'));
  
  return theme('table', array('header' => array_values($header), 'rows' => $rows));
}