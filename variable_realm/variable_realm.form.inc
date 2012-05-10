<?php
/**
 * @file
 * Administrative forms for variable realms.
 */

/**
 * Select variables for realm.
 */
function variable_realm_select_variables_form($form, &$form_state, $realm_name) {
  $current = variable_realm_get_variable_list($realm_name);
  $optional = variable_realm_get_variable_options($realm_name);
  // The final list will be the sum of both lists. We may have unknown variables
  // we want to preserve.
  $list = array_unique(array_merge($optional, $current));
  $form['realm_name'] = array('#type' => 'value', '#value' => $realm_name);
  $form['variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select variables to be set for this realm'),
    '#theme' => 'variable_table_select',
    '#tree' => TRUE,
  );
  foreach ($list as $name) {
    // Variable names may clash with form element names, so we need to replace '[' and ']'
    $safename = str_replace(array('[', ']'), array('<', '>'), $name);
    $form['variables'][$safename] = array(
      '#type' => 'checkbox',
      '#default_value' => in_array($name, $current),
      '#variable_name' => $name,
    );
  }
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

/**
 * Select variables for realm.
 */
function variable_realm_select_variables_form_submit($form, &$form_state) {
  // Get realm name and current list of variables.
  $realm_name = $form_state['values']['realm_name'];
  $old_variables = variable_realm_get_variable_list($realm_name);
  // Get main variable names
  $variables = $form_state['values']['variables'];
  $variables = array_keys(array_filter($variables));
  // Translate variable names
  foreach ($variables as $index => $name) {
    $variables[$index] = str_replace(array('<', '>'), array('[', ']'), $name);
  }
  variable_set('variable_realm_list_' . $realm_name, $variables);
  // Spawn multiple variables and translate into actual variables
  $new_list = variable_children($variables);
  // Delete variables from realm that are not in the new list.
  $old_list = variable_children($old_variables);
  foreach (array_diff($old_list, $new_list) as $name) {
    variable_realm_delete_variable($realm_name, $name);
    drupal_set_message(t('Deleted existing values of %name from realm variables.', array('%name' => $name)));
  }
}

/**
 * Edit variables for realm.
 */
function variable_realm_edit_variables_form($form, &$form_state, $realm_name, $realm_key) {
  $form['realm_name'] = array('#type' => 'value', '#value' => $realm_name);
  $form['realm_key'] = array('#type' => 'value', '#value' => $realm_key);
  // Group variables by variable group for vertical tabls
  $group_list = array();
  foreach (variable_realm_get_variable_list($realm_name) as $variable_name) {
    $variable_info = variable_get_info($variable_name);
    $group = $variable_info['group'];
    $group_list[$group][] = $variable_name;
  }
  $form['variables'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );
  foreach ($group_list as $group => $group_variables) {
    $group_info = variable_get_group($group);
    $form['variables'][$group] = array(
      '#type' => 'fieldset',
      '#title' => $group_info['title'],
      '#collapsible' => TRUE, '#collapsed' => TRUE,
    );
    $form['variables'][$group] += variable_realm_edit_subform($group_variables, $realm_name, $realm_key);
  }
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

/**
 * Edit variables for realm.
 */
function variable_realm_edit_variables_form_submit($form, &$form_state) {
  $realm_name = $form_state['values']['realm_name'];
  $realm_key = $form_state['values']['realm_key'];
  dpm($form_state['values']);
  foreach ($form_state['values']['variables'] as $group => $group_variables) {
    if (is_array($group_variables)) {
      foreach ($group_variables as $name => $value) {
        $current = variable_realm_get($realm_name, $realm_key, $name);
        if ($current !== $value) {
          variable_realm_set($realm_name, $realm_key, $name, $value);
        }
      }
    }
  }
}

/**
 * Key selector for realm forms.
 */
function variable_realm_form_key_selector($realm_name, $current_key) {
  $element_name = 'variable_realm_' . $realm_name . '_selector';
  $query_name = 'variable_realm_' . $realm_name . '_key';
  $realm_info = variable_realm_info($realm_name);
  $form[$element_name] = array(
    '#type' => 'fieldset',
    '#title' => t('Select value for %name realm', array('%name' => $realm_info['title'])),
    '#weight' => -100,
    '#description' => t('Check you are editing the variables for the right realm value or select the desired one.'),
  );
  // Replace only this element on current query string, there may be others.
  $current_query = $_GET;
  unset($current_query['q']);
  foreach (variable_realm_keys($realm_name) as $realm_key => $key_name) {
    $link =  l($key_name, $_GET['q'], array('query' => array($query_name => $realm_key) + $current_query));
    $items[] = $current_key == $realm_key ? '<strong>' . $link . '</strong>' : $link;
  }
  $form[$element_name]['select_key'] = array(
    '#type' => 'item',
    '#markup' => implode(' | ', $items),
  );
  return $form;
}

/**
 * Get current realm key from query string.
 */
function variable_realm_form_key_current($realm_name, $default_key = NULL) {
  $valid_keys = variable_realm_keys($realm_name);
  $query_name = 'variable_realm_' . $realm_name . '_key';
  if (isset($_GET[$query_name]) && isset($valid_keys[$_GET[$query_name]])) {
    return $_GET[$query_name];
  }
  else {
    return $default_key ? $default_key : key($valid_keys);
  }
}

/**
 * Form elements for variable list with realm values.
 */
function variable_realm_edit_subform($list, $realm_name, $realm_key) {
  module_load_include('form.inc', 'variable');
  $list = is_array($list) ? $list : array($list);
  $form = array();
  foreach ($list as $name) {
    if ($variable = variable_get_info($name)) {
      $variable['value'] = variable_realm_get($realm_name, $realm_key, $name);
      $form[$name] = variable_form_element($variable);
    }
  }
  return $form;
}
