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
    variable_realm_delete_all($realm_name, NULL, $name);
    drupal_set_message(t('Deleted existing values of %name from realm variables.', array('%name' => $name)));
  }
}
