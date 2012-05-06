<?php
/**
 * @file
 * Variable Realms Admin module - Select realm-specific variables
 */

/**
 * Select realm-specific variables
 */
function variable_realm_admin_realm_settings($form, $form_state, $realm) {
  variable_realm_admin_info();
  $current = variable_realm_admin_variables_conf($realm);
  $variables = variable_realm_admin_variables_list_all($realm);

  // The final list will be the sum of both lists. We may have unknown variables we want to preserve.
  $list = array_unique(array_merge($variables, $current));
  $form['variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select %realm dependent variables', array('%realm' => variable_realm_admin_realm_title($realm))),
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
  $form['realm'] = array('#type' => 'value', '#value' => $realm);
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

/**
 * Handle form submission.
 */
function variable_realm_admin_realm_settings_submit($form, $form_state) {
  // Get realm
  $realm = $form_state['values']['realm'];
  unset($form_state['values']['realm']);
  // Get main variable names
  $variables = $form_state['values']['variables'];
  $variables = array_keys(array_filter($variables));
  // Translate variable names
  foreach ($variables as $index => $name) {
    $variables[$index] = str_replace(array('<', '>'), array('[', ']'), $name);
  }
  variable_realm_admin_variables_conf($realm, $variables);
}

