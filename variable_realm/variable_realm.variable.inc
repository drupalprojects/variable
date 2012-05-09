<?php
/**
 * @file
 * Variable hooks.
 */

/**
 * Implements hook_variable_realm_info().
 */
function variable_realm_variable_realm_info() {
  $realm['global'] = array(
    'title' => t('Global'),
    'keys' => array(
      'default' => t('All variables')
    ),
  );
  return $realm;
}

/**
 * Implements hook_variable_info().
 */
function variable_realm_variable_info($options) {
  $variables['variable_realm_list[variable_realm]'] = array(
    'type' => 'array',
    'title' => t('Realm list', array(), $options),
    'description' => t('List of variables that can be set for a realm.', array(), $options),
  );
  return $variables;
}

/**
 * Implements hook_variable_type_info().
 */
function variable_realm_variable_type_info() {
  $type['variable_realm'] = array(
    'title' => t('Variable realm'),
    'type' => 'select',
    'options callback' => 'variable_realm_list',
  );
  return $type;
}
