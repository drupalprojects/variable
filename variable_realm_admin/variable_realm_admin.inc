<?php
/**
 * @file
 * Variable Realm Admin - API
 */

/**
 * Check whether the given realm has settings. Requires the 'settings'
 * parameter in the realms information to exist and not be set to FALSE.
 */
function variable_realm_admin_has_settings($realm) {
  $info = variable_realm_info($realm);
  return isset($info['settings']) && $info['settings'] !== FALSE;
}

/**
 * Check whether the given realm has a settings configuration page. Requires
 * info[settings][admin] not to be set to FALSE. Defaults to TRUE, as long as
 * the realm has settings.
 *
 * @see variable_realm_admin_has_settings().
 */
function variable_realm_admin_has_settings_ui($realm) {
  $info = variable_realm_info($realm);
  return variable_realm_admin_has_settings($realm) &&
    (!isset($info['settings']['admin']) || $info['settings']['admin'] !== FALSE);
}

/**
 * Retrieve the default key for the given realm, or check if a given key is the
 * default key.
 */
function variable_realm_admin_default_key($realm, $key = NULL) {
  $info = variable_realm_info($realm);
  $defaults = &drupal_static(__FUNCTION__, array());
  if (!isset($defaults[$realm])) {
    // first try callback
    if (isset($info['default key callback'])) {
      $callback = $info['default key callback'];
      $arguments = isset($info['default key arguments']) ? $info['default key arguments'] : array();
      $defaults[$realm] = call_user_func_array($callback, $arguments);
    }
    // second see if we have a simple key instead
    else if (isset($info['default key'])) {
      $defaults[$realm] = $info['default key'];
    }
    else {
      $defaults[$realm] = FALSE;
    }
  }
  return empty($key) ? $defaults[$realm] : $defaults[$realm] != $key;
}

/**
 * Get list of all "valid" variables for a given realm. Validity is determined
 * by the value of a realm-specific key in the each variable's info array.
 *
 * For example, all variables for the i18n realm have to have the key
 * 'localize' set to TRUE.
 */
function variable_realm_admin_variables_list_all($realm) {
  $info = variable_realm_info($realm);
  $key = isset($info['settings']['variable key']) ? $info['settings']['variable key'] : NULL;

  $variables = array();
  if (isset($key)) {
    foreach (variable_get_info() as $name => $variable) {
      if (!empty($variable[$key])) {
        $variables[] = $name;
      }
    }
  }
  drupal_alter('variable_realm_variables', $variables, $realm);
  return $variables;
}

/**
 * Get a realm switcher block for the given realm.
 */
function _variable_realm_admin_forms_switcher($realm, $current, $reset = FALSE) {
  $info = variable_realm_info($realm);
  $blocks = &drupal_static(__FUNCTION__, array());
  if (!isset($blocks[$realm]) || $reset) {
    if (isset($info['forms']['switcher callback'])) {
      $callback = $info['forms']['switcher callback'];
      $arguments = isset($info['forms']['switcher arguments']) ? $info['forms']['switcher arguments'] : array();
      array_unshift($arguments, $current);
      array_unshift($arguments, $realm);
      $blocks[$realm] = call_user_func_array($callback, $arguments);
    }
    else {
      $blocks[$realm] = FALSE;
    }
  }
  return $blocks[$realm];
}


/**
 * Return the name for the variable type described by the given realm, e.g.
 * "multilingual variable". Falls back to "@realm variable".
 */
function _variable_realm_admin_variable_type_name($realm, $plural = TRUE) {
  $info = variable_realm_info($realm);
  if ($plural) {
    return isset($info['variable name plural']) ? t($info['variable name plural']) : t('@realm variables', array('@realm' => variable_realm_admin_realm_title($realm)));
  }
  else {
    return isset($info['variable name']) ? t($info['variable name']) : t('@realm variable', array('@realm' => variable_realm_admin_realm_title($realm)));
  }
}

/**
 * Return a css name for form elements belonging to given realm.
 */
function _variable_realm_admin_variable_type_cssname($realm) {
  $info = variable_realm_info($realm);
  return isset($info['forms']['variable css']) ? $info['forms']['variable css'] : 'variable-realm-' . $realm;
}

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

/**
 * Get list of variables for given realm or check whether a variable belongs to
 * the given realm. Returns low-level variable names.
 *
 * @param $realm
 *   The name of the realm.
 * @param $name
 *   The name of a variable.
 */
function variable_realm_admin_variables_list($realm, $name = NULL) {
  static $variables;
  if (!isset($variables)) {
    // Note that variables being a global static, any other module can initialize or alter it.
    $variables = &drupal_static(__FUNCTION__, array());
  }
  if (!isset($variables[$realm])) {
    $variable_name = variable_realm_admin_variables_list_name($realm);
    $variables[$realm] = variable_get($variable_name, array());
  }
  return $name ? in_array($name, $variables[$realm]) : $variables[$realm];
}

/**
 * Get or set the list of high-level variables for a given realm.
 *
 * If a new set of variable names is given, this function will automatically
 * update the respective low-level list of variables provided by
 * variable_realm_admin_variables_list().
 *
 * @see variable_realm_admin_realm_settings_submit().
 *
 * @param $realm
 *   The name of the realm.
 * @param $data
 *   A new list of high-level variable names.
 */
function variable_realm_admin_variables_conf($realm, $data = NULL) {
  $confname = variable_realm_admin_variables_list_name($realm, 'conf');

  if (isset($data)) {
    // Save high-level variable names
    variable_set($confname, $data);

    // Rebuild low-level variables list
    variable_realm_admin_variables_list_update($realm, $data);

    // Notify other modules
    module_invoke_all('variable_realm_variables_update', $realm, $data);
  }

  return variable_get($confname, array());
}
