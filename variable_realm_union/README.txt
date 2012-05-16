
Drupal module: Variable Realm Union.
====================================

This an API module that allows combining two existing realms into a new one
whose keys are a combination of the other two.

An example of this module in action is the 'Domain+I18n Variables Integration' module
which is part of 'Domain Variable' module.

How to use it.
=============
To define a new domain that is a combination of two or more existing ones:

1. Implement hook_variable_realm_info() to define the realm name and properties.

function domain_i18n_variable_variable_realm_info() {
  $realm['domain_language'] = array(
    'title' => t('Domain+Language'),
    // Display on settings forms but without form switcher.
    'form settings' => TRUE,
    'form switcher' => FALSE,
    'variable name' => t('multilingual domain'),
  );
  return $realm;
}

2. Implement hook_variable_realm_controller() to define the Controller class to
    be used and which other realms it is a combination of. Example:

function domain_i18n_variable_variable_realm_controller() {
  $realm['domain_language'] = array(
    'weight' => 200,
    'class' => 'VariableStoreRealmController',
    'union' => array('domain', 'language'),
  );
  return $realm;
}
