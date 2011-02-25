<?php

/**
 * @file
 * Hooks provided by the Variable module.
 */

/**
 * @defgroup variable_api_hooks Variable API Hooks
 * @{
 * Functions to define and modify information about variables.
 * 
 * These hooks and all the related callbacks may be defined in a separate file
 * named module.variable.inc
 * 
 * @}
 */

/**
 * Define variables used by a module.
 * 
 * For the case of variable names that depend on some other parameter (like variables per content-type),
 * there's some special type of variables: Multiple variables. These can be defined like this:
 * 
 * $variables['node_options_[node_type]'] = array(
 *   'type' => 'multiple',
 *   'title' => t('Default options', array(), $options),
 *   'repeat' => array(
 *     'type' => 'options',
 *     'default' => array('status', 'promote'),
 *     'options callback' => 'node_variable_option_list',
 *   ),
 * );
 * 
 * This multiple variable will spawn into one variable for each node type. Note the variable name that includes
 * the property [node_type]. Values for [node_type] will be defined on hook_variable_option_info().
 * 
 * The 'repeat' property defines the properties of children variables. In this case the 'type' property is optional
 * and will default to 'multiple'.
 * 
 * @param $options
 *   'language' => Language object for which strings and defaults must be returned
 * 
 * @return
 *   An array of information defining the module's variables. The array
 *   contains a sub-array for each node variable, with the variable name
 *   as the key. Possible attributes:
 *   - "title": The human readable name of the variable, will be used in auto generated forms.
 *   - "type": Variable type, should be one of the defined on hook_variable_type_info().
 *   - "group": Group key, should be one of the defined on hook_variable_group_info().
 *   - "description": Variable description, will be used in auto generated forms.
 *   - "options": Array of selectable options, or option name as defined on hook_variable_option_info().
 *   - "options callback": Function to invoke to get the list of options.
 *   - "default": Default value.
 *   - "default callback": Function to invoke to get the default value.
 *   - "multiple": Array of multiple children variables to be created from this one.
 *   - "multiple callback": Function to invoke to get children variables.
 *   - "element": Form element properties to override the default ones for this variable type.
 *   - "element callback": Function to invoke to get a form element for this variable.
 *   - "module": Module to which this variable belongs. This property will be added automatically.
 *   - "repeat": Array of variable properties for children variables.
 */
function hook_variable_info($options) {
  
}
 
/**
 * Define types of variables used by a module.
 * 
 * @return
 *   An array of information defining variable types. The array contains
 *   a sub-array for each variable type, with the variable type as the key.
 *   
 *   The possible attributes are the same as for hook_variable_info(), with the
 *   type attributes being added on top of the variable attributes.
 *   
 *   A special attribute:
 *   - "type": Variable subtype, the properties for the subtype will be added to these ones.
 */
function hook_variable_type_info() {
  $type['mail_address'] = array(
    'title' => t('E-mail address'),
    'element' => array('#type' => 'textfield'),
    'token' => TRUE,
  );
  $type['mail_text'] = array(
    'title' => t('Mail text'),
    'multiple' => array('subject' => t('Subject'), 'body' => t('Body')),
    'build callback' => 'variable_build_mail_text',
    'localize' => TRUE,
    'type' => 'multiple',
  );
  return $type;  
}

/**
 * Define groups of variables used by a module.
 * 
 * Variable groups are used for presentation only, to display and edit the variables
 * on manageable groups. Groups can define a subset of a module's variables and can
 * be reused accross modules to group related variables.
 * 
 * A form to edit all variables in a group can be generated with:
 * 
 *   drupal_get_form('variable_group_form', group_name);
 * 
 * @return
 *   An array of information defining variable types. The array contains
 *   a sub-array for each variable group, with the group as the key.
 *   Possible attributes:
 *   - "title": The human readable name of the group. Must be localized.
 *   - "description": The human readable description of the group. Must be localized.
 *   - "access": Permission required to edit group's variables. Will default to 'administer site configuration'.
 *   - "path": Array of administration paths where these variables can be accessed.
 */
function hook_variable_group_info() {
  $groups['system_site_information'] = array(
    'title' => t('Site information'),
    'description' => t('Site information and maintenance mode'),
    'access' => 'administer site configuration',
    'path' => array('admin/config/system/site-information', 'admin/config/development/maintenance'),
  );
  $groups['system_feed_settings'] = array(
    'title' => t('Feed settings'),
    'description' => t('Feed settings'),
    'access' => 'administer site configuration',
  );  
}

/**
 * Define lists to be used for variable options or multiple variables.
 * 
 * These lists can be used for variable options or for children of multiple variables.
 * 
 * @return
 *   An array of information defining lists of options. The array contains
 *   a sub-array for each variable options, with the list name as the key.
 * 
 *   Possible attributes:
 *   - "title": The human readable name of the list of options. Must be localized.
 *   - "callback": A function to invoke to get the list of options.
 *   - "cache": Whether this list is cacheable, defaults to FALSE.
 */
function hook_variable_option_info() {
  $options['weekday'] = array(
    'title' => t('Day of week'),
    'callback' => 'system_variable_option_weekday',
  );
  $options['theme'] = array(
    'title' => t('Theme'),
    'callback' => 'system_variable_option_theme',
    'cache' => TRUE,
  );
  return $options;  
}