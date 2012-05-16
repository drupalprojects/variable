<?php
/**
 * @file
 * Variable hooks for variable_realm_union.
 */

/**
 * Implements hook_variable_realm_info_alter();
 */
function variable_realm_union_variable_realm_info_alter(&$realms) {
  $union_realms = variable_realm_union();
  foreach ($realms as $realm_name => &$realm_info) {
    if (isset($union_realms[$realm_name])) {
      $realm_info += array(
        'keys callback' => '_variable_realm_union_realm_keys',
        'list callback' => '_variable_realm_union_variable_list',
      );
    }
  }
}

/**
 * Produce keys for union realm.
 */
function _variable_realm_union_realm_keys($realm_name, $realm_info) {
  $union = array_combine($realm_info['union'], $realm_info['union']);
  $all_realm_keys = array_map('variable_realm_keys', $union);
  // First build all combinations of realm keys.
  $combine = array(array('keys' => array(), 'names' => array()));
  foreach ($all_realm_keys as $realm => $realm_keys) {
    $new_combine = array();
    foreach ($combine as $index => $data) {
      foreach ($realm_keys as $new_key => $new_name) {
        $keys = $data['keys'];
        $names = $data['names'];
        $keys[$realm] = $new_key;
        $names[$realm] = $new_name;
        $new_combine[] = array('keys' => $keys, 'names' => $names);
      }
    }
    $combine = $new_combine;
  }
  // Now build all realm keys for the combinations.
  $keys = array();
  foreach ($combine as $data) {
    $key = variable_realm_union_build_key($realm_name, $data['keys']);
    $name = variable_realm_union_build_name($realm_name, $data['names']);
    $keys[$key] = $name;
  }
  return $keys;
}

/**
 * Produce keys for union realm.
 */
function _variable_realm_union_variable_list($realm_name, $realm_info) {
  $union = array_combine($realm_info['union'], $realm_info['union']);
  $variables = array_map('variable_realm_get_variable_list', $union);
  return call_user_func_array('array_intersect', $variables);
}

/**
 * Implements hook_variable_settings_form_alter().
 *
 * Make sure realm switchers are added for all realms in the union.
 */
function variable_realm_union_variable_settings_form_alter(&$form, &$form_state, $form_id) {
  foreach (variable_realm_union() as $realm_name => $union_realms) {
    if (!empty($form['#realm_variables'][$realm_name]) && empty($form[VARIABLE_REALM_FORM_SWITCHER . $realm_name])) {
      // Check we have selectors for the other realms.
      foreach ($union_realms as $realm) {
        $info = variable_realm_info($realm);
        if (!empty($info['form switcher']) && empty($form[VARIABLE_REALM_FORM_SWITCHER . $realm])) {
          $current_key = variable_realm_form_key_current($realm);
          $form += variable_realm_form_key_selector($realm, $current_key);
        }
      }
    }
  }
}
