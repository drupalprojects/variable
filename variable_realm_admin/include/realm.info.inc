<?php
/**
 * @file
 * Various helper functions for retrieving information from variable realm
 * information arrays.
 */

/**
 * Retrieve title of given realm.
 */
function variable_realm_admin_realm_title($realm) {
  $info = variable_realm_info($realm);
  return isset($info['title']) ? $info['title'] : $realm;
}

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
