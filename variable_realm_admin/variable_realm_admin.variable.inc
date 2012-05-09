<?php
/**
 * @file
 * Variable Realms Admin module
 */

/**
 * Implements hook_variable_info().
 */
function variable_realm_admin_variable_info() {
  variable_realm_admin_info();

  $variables = array();
  foreach (variable_realm_info() as $realm => $info) {
    // Only provide the high level *_conf variable if there is a UI for setting it.
    if (variable_realm_admin_has_settings_ui($realm)) {
      $conf = variable_realm_admin_variables_list_name($realm, 'conf');
      $variables[$conf] = array(
        'title' => t('@realm variables, high-level variable names.', array('@realm' => $info['title'])),
        'type' => 'array',
        'group' => 'variable',
      );
    }
    // Always provide the low level *_list variable as long as the realm has settings.
    if (variable_realm_admin_has_settings($realm)) {
      $list = variable_realm_admin_variables_list_name($realm, 'list');
      $variables[$list] = array(
        'title' => t('@realm variables, real variable names.', array('@realm' => $info['title'])),
        'type' => 'array',
        'group' => 'variable',
      );
    }
  }
  return $variables;
}

/**
 * Implements hook_variable_realm_info_alter().
 *
 * @todo move into i18n_variable module
 * @todo add documentation to variable_realm
 */
function variable_realm_admin_variable_realm_info_alter(&$info) {
  if (isset($info['language'])) {

    $info['language'] += array(
      // Callback which is used to determine the default key, e.g. the default
      // language code for the language realm.
      'default key callback' => 'language_default',
      'default key arguments' => array('language'),

      'settings' => array(
        // Used for storing selected variables belonging to realm; actual variables come in two forms: 
        // - [config prefix]_conf : list of high-level variable names
        // - [config prefix]_list : list of real variable names
        'config prefix' => 'i18n_variable',

        // The variable key needs to be set for respective variables in variable
        // info for the variables to show up in the configuration form for this
        // realm.
        'variable key' => 'localize',

        // Whether to allow configuration pages for variable settings. Defaults to
        // TRUE. Can be disabled for generated realms, e.g. realm unions.
        'admin' => TRUE,
      ),
      // A human name for variables which are dependent on this realm. Defaults
      // to "[title] variable". Used to highlight form elements as belonging to
      // this realm.
      'variable name' => 'multilingual variable',

      // A human name for variables which are dependent on this realm (plural
      // form). Defaults to "[title] variables".
      'variable name plural' => 'multilingual variables',

      // Values related to the modification of system forms
      'forms' => array(
        // A callback used for generating switch links for system forms.
        'switcher callback' => 'variable_realm_admin_forms_switcher',
        'switcher arguments' => array(),

        // A css name which is attached to form elements belonging to this realm.
        'variable css' => 'i18n-variable',
      ),
    );
  }

  // strings for potx
  t('multilingual variable');
  t('multilingual variables');
}

/**
 * Implements hook_variable_realm_variables_alter().
 *
 * Add support for legacy $conf['i18n_variables'] array on behave of i18n_variable module.
 */
function i18n_variable_variable_realm_variables_alter(&$variables, $realm) {
  if ($realm == 'i18n') {
    $conf = variable_get('i18n_variables', array());
    $variables += $conf;
  }
}
