<?php
/**
 * @file
 * Documents hooks provided by Variable Realm API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provides information about controller classes and weights for variable realms.
 *
 * Modules implementing this hook must also implement any bootstrap hook (for
 * example hook_boot).
 *
 * This information is used to dynamically load realms upon request and to build
 * exportable realms as Features. For this reason this must be defined in the
 * main module and available at bootstrap time when the realm is going to be
 * used.
 *
 * @see i18n_variable_variable_realm_controller()
 *
 * @return array
 *   Associative array keyed by realm name. Each element is an array containing:
 *   - 'class', Class name implementing RealmControllerInterface.
 *   - 'weight', Default weight for this realm.
 */
function hook_variable_realm_controller() {
  $realm['language'] = array(
    'weight' => 100,
    'class' => 'VariableStoreRealmController',
  );
  return $realm;
}

/**
 * Provides human readable information about existing realms.
 *
 * This information is mainly used to build exportable realms as Features.
 *
 * @see i18n_variable_variable_realm_info()
 *
 * @return array
 *   Array keyed by realm name which contains the following elements:
 *   - 'title', Humam readable name for the realm.
 *   - 'keys', Associative array with human readable names for keys.
 *   - 'keys callback', Callback function to provide the keys.
 *   - 'default key', The default key.
 *   - 'options', Array of variable names that may be set for this realm. If not set
 *     any variable will be allowed for this realm.
 *   - 'list callback', Callback function to provide variable list for this realm.
 *   - 'select', Boolean flag whether variables for this realm can be selected from a list.
 *   - 'select path', Path to variable selection form (optional).
 *   - 'variable name', Name for variables that belong to this realm: e.g. 'multilingual' variable/s
 *   - 'variable class', CSS class name for annotated variables in system settings forms.
 *   - 'form settings', Boolean flag, whether realm variables should be handled automatically
 *     in system settings forms.
 *   - 'form switcher', Boolean flag, whether a realm switcher should be auto-generated
 *     for settings forms which contain variables that belong to this realm.
 *
 */
function hook_variable_realm_info() {
  $realm['language'] = array(
    'title' => t('Language'),
    'keys' => locale_language_list('name', TRUE),
    'default key' => language_default('language'),
    'options' => _i18n_variable_variable_realm_list(),
    'select' => TRUE,
    'select path' => 'admin/config/regional/i18n/variable',
    'variable name' => t('multilingual'),
    'variable class' => 'i18n-variable',
    'form settings' => TRUE,
  );
  return $realm;
}

/**
 * Alter variable realm information provided by modules.
 *
 * @see hook_variable_realm_info().
 */
function hook_variable_realm_info_alter(&$realms) {
}

/**
 * Allow other modules to act on realm switching.
 *
 * This hook is invoked right after the realm key is switched but before
 * the global variables are rebuilt.
 *
 * @param $realm_name
 *   Realm that is switched.
 * @param $realm_key
 *   New realm key.
 */
function hook_variable_realm_switch($realm_name, $realm_key) {
}

/**
 * Alter the list of realm page parameters.
 *
 * These parameters are used in settings forms, overriding realm keys by using
 * special $_GET variables. The purpose of this hook is to allow other modules
 * to set predefined realm keys for settings forms, like variable_realm_union.
 */
function hook_variable_realm_params_alter(&$realm_params) {
}

/**
 * Alter the list of variables configurable for a realm before the list is saved
 * to the database (in a variable).
 *
 * @param $variables
 *   Array of variable names.
 * @param $realm_name
 *   The name of the realm we are changing the list for.
 */
function hook_variable_realm_variable_list_alter(&$variables, $realm_name) {
}

/**
 * @} End of "addtogroup hooks".
 */
