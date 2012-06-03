<?php
/**
 * @file
 * Variable API module. Form library.
 */

/**
 * Build form element for a variable
 */
function variable_form_element($variable, $options = array()) {
  $variable = variable_build($variable);
  $variable = variable_build_options($variable, $options);
  if (!empty($variable['element callback'])) {
    $element = call_user_func($variable['element callback'], $variable, $options);
  }
  elseif (isset($variable['options'])) {
    $element = variable_form_element_options($variable, $options);
  }
  else {
    $element = variable_form_element_default($variable, $options);
  }
  if (!empty($variable['validate callback'])) {
    $element['#element_validate'][] = 'variable_form_element_validate';
  }
  if (!empty($options['form parents'])) {
    $element['#parents'] = $options['form parents'];
    $element['#parents'][] = $variable['name'];
  }
  $element += array('#access' => variable_access($variable));
  if (!empty($variable['required'])) {
    $element += array('#required' => TRUE);
  }
  // Add variable data to element so we can use it later fo validation, etc..
  $element['#variable'] = $variable;
  return $element;
}

/**
 * Build array form element
 */
function variable_form_element_array($variable, $options = array()) {
  // This will be possibly a fieldset with tree value
  $element = variable_form_element_default($variable, $options);
  // We may have a multiple element base that will default to plain textfield
  $item = $variable['repeat'];
  $value = variable_get_value($variable, $options);
  // Compile names and defaults for all elements
  $names = $defaults = array();
  if (!empty($variable['multiple'])) {
    // If we've got a multiple value, it will be an array with known elements
    $names = $variable['multiple'];
  }
  else {
    // Array with unknown elements, we add an element for each existing one
    $names = $value ? array_combine(array_keys($value), array_keys($value)) : array();
  }
  // Now build elements with the right names
  foreach ($names as $key => $title) {
    if (isset($value[$key]) && is_array($value[$key])) {
      // This element is an array, we cannot edit it but we need to add it to the form
      $element[$key] = array('#type' => 'value', '#value' => $value[$key]);
      $element['variable_element_array_' . $key] = array('#type' => 'item', '#title' => $title, '#markup' => variable_format_array($value[$key]));
    }
    else {
      $element[$key] = $item['element'] + array('#title' => $title, '#default_value' => isset($value[$key]) ? $value[$key] : '');
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
 * Build form element for unknown variable.
 *
 * This is not an editable form element but a form item.
 */
function variable_form_element_unknown($variable, $options = array()) {
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $type = variable_get_type($variable['type']) + array('element' => array());
  $element = $variable['element'] + array(
    '#title' => $variable['title'],
    '#description' => $variable['description'],
    '#markup' => variable_format_value($variable, $options),
  ) + $type['element'];
  return $element;
}

/**
 * Build form element for text_format variable.
 */
function variable_form_element_text_format($variable, $options = array()) {
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $type = variable_get_type($variable['type']) + array('element' => array());
  $element = $variable['element'] + array(
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  ) + $type['element'];
  $value = variable_get_value($variable, $options);
  if (isset($value) && is_array($value)) {
    if (isset($value['value'])) {
      $element['#default_value'] = $value['value'];
    }
    if (isset($value['format'])) {
      $element['#format'] = $value['format'];
    }
  }
  return $element;
}

/**
 * Build options variables
 */
function variable_form_element_options($variable, $options = array()) {
  $element = variable_form_element_default($variable, $options);
  $element['#options'] = $variable['options'];
  // Depending on the number of options this may be radios or a drop-down.
  // However if there are nested options (an option is an array) it should be always a drop-down.
  if (empty($element['#type'])) {
    $element['#type'] = count($variable['options']) > 4 || array_filter(array_map('is_array', $variable['options'])) ? 'select' : 'radios';
  }
  return $element;
}

/**
 * Implement validate callback
 */
function variable_form_element_validate($element, &$form_state, $form) {
  $variable = $element['#variable'];
  variable_include($variable);
  $variable['value'] = isset($element['#value']) ? $element['#value'] : NULL;
  if ($error = call_user_func($variable['validate callback'], $variable)) {
    form_error($element, $error);
  }
}

/**
 * Add default buttons to a form and set its prefix.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $options
 *   Aditional options to pass on, like variable realm.
 *
 * @return
 *   The form structure.
 *
 * @see system_settings_form_submit()
 * @ingroup forms
 */
function variable_settings_form($form, $options = array()) {
  $form['#options'] = $options;
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'variable_settings_form_submit';
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }
  return $form;
}

/**
 * Execute the system_settings_form.
 *
 * If you want node type configure style handling of your checkboxes,
 * add an array_filter value to your form.
 */
function variable_settings_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  // This may contain some realm options.
  $options = $form['#options'];
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    // Using this function will invoke variable_update hook.
    variable_set_value($key, $value, $options);
  }

  drupal_set_message(t('The configuration options have been saved.'));
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
    $variable_name = isset($element[$name]['#variable_name']) ? $element[$name]['#variable_name'] : $name;
    $variable = _variable_variable($variable_name);
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
