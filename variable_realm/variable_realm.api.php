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
 * Modules implementing this hook must implement too any bootstrap hook.
 *
 * This information is used to dinamically load realms upon request and to build
 * exportable realms as Features. For this reason this must be defined in the main
 * module and available at bootstrap time when the realm is going to be used.
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
 */
function hook_variable_realm_info() {
  $realm['language'] = array(
    'title' => t('Language'),
    'keys' => locale_language_list('name', TRUE),
  );
  return $realm;
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
 * @} End of "addtogroup hooks".
 */
