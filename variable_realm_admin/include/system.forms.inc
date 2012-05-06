<?php
/**
 * @file
 * Variable Realms Admin module - Adjust system forms
 */

/**
 *
 * @todo separate handling for theme form???
 */
function variable_realm_admin_system_forms_alter(&$form, &$form_state, $form_id) {
  if ($variables = _variable_realm_admin_system_forms_settings($form)) {
    array_unshift($form['#submit'], 'variable_realm_admin_system_forms_submit');
    $form['#realm_variables'] = $variables;
    $form += variable_realm_admin_forms_realm_infoblock(array_unique(array_values($variables)));
  }
}

/**
 * Check for realm variables in form + mark them respectively.
 *
 * @param $form
 *   The form.
 * @return array
 *   List of variables which have a realm (realmvariable => realm).
 */
function _variable_realm_admin_system_forms_settings(&$form) {
  $result = array();
  foreach (element_children($form) as $field) {
    if (count(element_children($form[$field])) && empty($form[$field]['#tree'])) {
      $result += _variable_realm_admin_system_forms_settings($form[$field]);
    }
    elseif ($realm = variable_realm_admin_find_realm($field)) {
      // Add form field class (i18n-variable) and description text.
      $form[$field]['#attributes']['class'][] = _variable_realm_admin_variable_type_cssname($realm);
      $form[$field]['#description'] = !empty($form[$field]['#description']) ? $form[$field]['#description'] : '';
      $form[$field]['#description'] .= ' <strong>' . t('This is a @variable.', array('@variable' => _variable_realm_admin_variable_type_name($realm, FALSE))) . '</strong> ';
      // Addd field => realm to result
      $result[$field] = $realm;
    }
  }
  return $result;
}

/**
 * Submit handler for system settings forms.
 *
 * Saves values for realm variables to respective realms and removes their
 * values from the form unless the currently active realm is the default one.
 */
function variable_realm_admin_system_forms_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  // determine active realm keys
  $realms_current = variable_realm_current();
  if (isset($form_state['values']['variable_realm']['current'])) {
    $realms_current = $form_state['values']['variable_realm']['current'];
    unset ($form_state['values']['variable_realm']['current']);
  }

  // retrieve realm variables
  $variables = $form['#realm_variables'];
  foreach ($variables as $name => $realm) {
    $realm_key = $realms_current[$realm];
    // if there is a value for this variable
    if (isset($form_state['values'][$name])) {
      if ($op == t('Reset to defaults')) {
        variable_store_del($realm, $realm_key, $name);
        variable_realm_del($realm, $realm_key, $name);
      }
      else {
        $value = $form_state['values'][$name];
        if (is_array($value) && isset($form_state['values']['array_filter'])) {
          $value = array_keys(array_filter($value));
        }
        variable_store_set($realm, $realm_key, $name, $value);
        variable_realm_set($realm, $realm_key, $name, $value);
      }
      // Unless current realm key is a default key, remove the value to prevent
      // the global default value from being overwritten in
      // system_settings_form_submit().
      if (!variable_realm_admin_default_key($realm, $realm_key)) {
        dpm('unset value for ' . $name);
        unset($form_state['values'][$name]);
      }
    }
  }

  // Make sure the form shown after submission is still the same one
  $query_params = array();
  foreach ($variables as $name => $realm) {
    $query_params[VARIABLE_REALM_SELECTOR_QUERY_PARAM_PREFIX . $realm] = $realms_current[$realm];
  }
  $form_state['redirect'] = array(current_path(), array('query' => $query_params));
}

/**
 * Add an info block + realm selector at top of form.
 *
 * @see variable_realm_admin_init().
 *
 * @param $realms
 *   Active realms
 */
function variable_realm_admin_forms_realm_infoblock($realms) {
  $realms_current = variable_realm_current();

  $form = array(
    '#type' => 'fieldset',
    '#weight' => -100,
    '#title' => t('There are @realm-variables in this form.', array('@realm-variables' => _variable_realm_admin_realm_names_human($realms))),
    '#description' => t('Check you are editing the variables for the right realm(s). To configure which variables are available for which realm visit <a href="@variable-realms">variable realms configuration</a>.', array('@variable-realms' => url('admin/config/system/variable/realm'))),
  );

  foreach ($realms as $realm) {
    $current = $realms_current[$realm];

    // build realm switcher block
    $switcher = _variable_realm_admin_forms_switcher($realm, $current);
    if(!empty($switcher)) {
      $form[$realm . '_switcher'] = $switcher;
    }

    // store active realm
    $form['current'][$realm] = array('#type' => 'value', '#value' => $current);
  }

  drupal_alter('variable_realm_forms_infoblock', $form, $realms);
  return array('variable_realm' => $form);
}

/**
 * Default callback for generating a realm switcher block.
 */
function variable_realm_admin_forms_switcher($realm, $current) {
  $info = variable_realm_info($realm);
  $keys = variable_realm_keys($realm);

  // build list of switch links
  $items = array();
  foreach ($keys as $key => $name) {
    $link =  l($name, $_GET['q'], array('query' => array(VARIABLE_REALM_SELECTOR_QUERY_PARAM_PREFIX . $realm => $key) + drupal_get_query_parameters()));
    $items[] = $current == $key ? '<strong>' . $link . '</strong>' : $link;
  }
  return array(
    '#title' => t('Select @realm', array('@realm' => $info['title'])),
    '#type' => 'item',
    '#markup' => implode(' | ', $items),
  );
}

/**
 * Turns a list of realms into a string of form [realm1], [realm2], and [realm3].
 */
function _variable_realm_admin_realm_names_human($realms) {
  $realms = array_map('_variable_realm_admin_variable_type_name', $realms);
  if (count($realms) <= 1) {
    return reset($realms);
  }
  else if (count($realms) == 2) {
    return reset($realms) . ' ' . t('and') . ' ' . next($realms);
  }
  else {
    $last = array_pop($realms);
    return implode(', ', $realms) . ', ' . t('and') . ' ' . $last;
  }
}

