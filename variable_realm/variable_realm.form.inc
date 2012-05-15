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
  // Hook for modules to alter this variable list.
  drupal_alter('variable_realm_variable_list', $variables, $realm_name);
  // And save the list to a variable.
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
  $realm_info = variable_realm_info($realm_name);
  $form['realm_name'] = array('#type' => 'value', '#value' => $realm_name);
  $form['realm_key'] = array('#type' => 'value', '#value' => $realm_key);
  $options['realm'] = variable_realm($realm_name, $realm_key);
  if ($variable_list = variable_realm_get_variable_list($realm_name)) {
    // Group variables by variable group for vertical tabls
    $group_list = array();
    foreach ($variable_list as $variable_name) {
      $variable_info = variable_get_info($variable_name, $options);
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
      // Set form parents for this variable / group.
      $options['form parents'] = array('variables', $group);
      $form['variables'][$group] += variable_edit_subform($group_variables, $options);
    }
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  }
  else {
    $form['message']['#markup'] = '<h3>' . t('No variables have been selected as %realm_name specific.', array('%realm_name' => $realm_info['title'])) . '</h3>';
  }
  if (!empty($realm_info['select path'])) {
    $form['select']['#markup'] = t('To select variables to be configured, see <a href="!select-url">%realm_name variable settings</a>.', array(
      '%realm_name' => $realm_info['title'],
      '!select-url' => url($realm_info['select path'])
    ));
  }
  return $form;
}

/**
 * Edit variables for realm.
 */
function variable_realm_edit_variables_form_submit($form, &$form_state) {
  $realm_name = $form_state['values']['realm_name'];
  $realm_key = $form_state['values']['realm_key'];
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
    '#title' => t('There are %name variables in this form', array('%name' => $realm_info['variable name'])),
    '#weight' => -100,
    '#description' => t('Check you are editing the variables for the right %realm value or select the desired %realm.', array('%realm' => $realm_info['title'])),
  );
  // Replace only this element on current query string, there may be others.
  $current_query = $_GET;
  unset($current_query['q']);
  foreach (variable_realm_keys($realm_name) as $realm_key => $key_name) {
    $query[VARIABLE_REALM_QUERY_STRING . $realm_name] = $realm_key;
    $link =  l($key_name, $_GET['q'], array('query' => $query + $current_query));
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
  if ($key = variable_realm_params($realm_name)) {
    return $key;
  }
  elseif ($current_key = variable_realm_status($realm_name)) {
    return $current_key;
  }
  elseif ($default_key) {
    return $default_key;
  }
  else {
    $realm_info = variable_realm_info($realm_name);
    return $realm_info['default key'];
  }
}

/**
 * Alter settings form and return list of found variables.
 */
function _variable_realm_variable_settings_form_alter(&$form, $realm_name, $variables) {
  $result = array();
  foreach (element_children($form) as $field) {
    if (count(element_children($form[$field])) && empty($form[$field]['#tree'])) {
      // Rewrite fieldsets recursively.
      $result += _variable_realm_variable_settings_form_alter($form[$field], $realm_name, $variables);
    }
    elseif (in_array($field, $variables)) {
      if (isset($form[$field]['#variable_realm'])) {
        // Oh-oh, variable already taken by another realm.
        _variable_realm_variable_settings_form_conflict($field);
        $form[$field]['#disabled'] = TRUE;
      }
      else {
        // Mark variable as already taken by a realm
        $form[$field]['#variable_realm'] = $realm_name;
      }
      _variable_realm_variable_settings_form_mark($realm_name, $form[$field]);
      // Addd field => name to result
      $result[$field] = !empty($form[$field]['#title']) ? $form[$field]['#title'] : $field;
    }
  }
  return $result;
}

/**
 * Mark variable as belonging to a realm.
 */
function _variable_realm_variable_settings_form_mark($realm_name, &$element) {
  $realm_info = variable_realm_info($realm_name);
  // Add form field class (i18n-variable) and description text.
  $element['#attributes']['class'][] = $realm_info['variable class'];
  $element['#description'] = !empty($element['#description']) ? $element['#description'] : '';
  $element['#description'] .= ' <strong>' . t('This is a @name variable.', array('@name' => $realm_info['variable name'])) . '</strong> ';
}

/**
 * Warning about variable conflict.
 */
function _variable_realm_variable_settings_form_conflict($variable) {
  static $warnings;
  if (!isset($warnings[$variable])) {
    $warnings[$variable] = TRUE;
    drupal_set_message(t('There are conflicting realm variables in the form. The variable %name is enabled for more than one realm. Review your realm settings', array('%name' => variable_name($variable))), 'warning');
  }
}

/**
 * Save realm variables and remove them from form.
 */
function variable_realm_variable_settings_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  foreach ($form['#realm_keys'] as $realm_name => $realm_key) {
    $realm_info = variable_realm_info($realm_name);
    //$language = i18n_language($form_state['values']['i18n_variable_language']);
    //unset($form_state['values']['i18n_variable_language']);
    $variables = array_keys($form['#realm_variables'][$realm_name]);
    foreach ($variables as $variable_name) {
      if (isset($form_state['values'][$variable_name])) {
        if ($op == t('Reset to defaults')) {
          variable_realm_del($realm_name, $realm_key, $variable_name);
        }
        else {
          $value = $form_state['values'][$variable_name];
          if (is_array($value) && isset($form_state['values']['array_filter'])) {
            $value = array_keys(array_filter($value));
          }
          variable_realm_set($realm_name, $realm_key, $variable_name, $value);
        }
        // If current is not default realm key, we don't set any global variable (without realm)
        if ($realm_key != $realm_info['default key']) {
          unset($form_state['values'][$variable_name]);
        }
      }
    }
  }
  // The form will go now through system_settings_form_submit()
}

/**
 * Save multilingual variables and remove them from form.
 *
 * Note the theme variable has already been set into 'variable_realm_theme_settings'
 */
function variable_realm_variable_theme_form_submit($form, &$form_state) {
  foreach ($form['#realm_keys'] as $realm_name => $realm_key) {
    $realm_info = variable_realm_info($realm_name);
    $settings = variable_get('variable_realm_theme_settings');
    $variable_name = $form['#realm_theme'][$realm_name];
    variable_realm_set($realm_name, $realm_key, $variable_name, $settings);
    // If current is default language, we allow global (without language) variables to be set too
    if ($realm_key == $realm_info['default key']) {
      variable_set($variable_name, $settings);
    }
    variable_del('variable_realm_theme_settings');
  }
}
