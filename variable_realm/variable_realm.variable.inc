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
  $variables['variable_realm_list_[variable_realm]'] = array(
    'type' => 'multiple',
    'multiple' => 'variable_realm',
    'title' => t('Realm list', array(), $options),
    'description' => t('List of variables that can be set for a realm.', array(), $options),
    'repeat' => array(
      'type' => 'array',
    ),
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

/**
 * Implements hook_variable_settings_form_alter().
 */
function variable_realm_variable_settings_form_alter(&$form, &$form_state, $form_id) {
  module_load_include('form.inc', 'variable_realm');
  foreach (_variable_realm_variable_settings_form_list() as $realm_name => $variables) {
    $realm_info = variable_realm_info($realm_name);
    if ($form_id == 'system_theme_settings') {
      // Special treatment for theme settings
      if (!empty($form['var']) && !empty($form['var']['#value']) && in_array($form['var']['#value'], $variables)) {
        $realm_variables = element_children($form['theme_settings']);
        $realm_variables = array_merge($realm_variables, array('default_logo', 'logo_path', 'default_favicon', 'favicon_path'));
        _variable_realm_variable_settings_form_alter($form, $realm_name, $realm_variables);
        // Replace variable (theme) name so we use a temporary storage variable
        $form['#realm_variables'][$realm_name] = $realm_variables;
        $form['#realm_theme'][$realm_name] = $form['var']['#value'];
      }
    }
    elseif ($realm_variables = _variable_realm_variable_settings_form_alter($form, $realm_name, $variables)) {
      $form['#realm_variables'][$realm_name] = $realm_variables;
      //$form += i18n_variable_form_selector();
    }
    // Add realm values and subform realm / key selector.
    if (!empty($realm_variables)) {
      $current_key = variable_realm_form_key_current($realm_name);
      if (!empty($realm_info['form switcher'])) {
        $form += variable_realm_form_key_selector($realm_name, $current_key);
      }
      $form['#realm_keys'][$realm_name] = $current_key;
    }
  }
  if (!empty($form['#realm_theme'])) {
    // @todo Use unique variable name for each form.
    $form['var']['#value'] = 'variable_realm_theme_settings';
    $form['#submit'][] = 'variable_realm_variable_theme_form_submit';
    //$form += i18n_variable_form_selector();
  }
  elseif (!empty($form['#realm_variables'])) {
    array_unshift($form['#submit'], 'variable_realm_variable_settings_form_submit');
  }
}
