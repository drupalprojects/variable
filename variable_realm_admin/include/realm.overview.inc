<?php
/**
 * @file
 *
 */

/**
 * Lists available realms.
 */
function variable_realm_admin_realm_overview($form, $form_state) {
  variable_realm_admin_info();

  $header = array(
    t('Realm name'),
    t('Keys'),
    t('Configured variables'),
    t('Actual variable names'),
    t('Actions'),
  );

  $realms = array();
  foreach (variable_realm_info() as $realm => $info) {
    $title = $info['title'];
    $keys = $conf = $list = $links = '';

    // Build key info
    $keys_all = variable_realm_keys($realm);
    $key_default = variable_realm_admin_default_key($realm);
    if (count($keys_all) < 5) {
      if ($key_default && isset($keys_all[$key_default])) {
        $keys_all[$key_default] = '<strong>' . $keys_all[$key_default] . '</strong>';
      }
      $keys = implode(', ', $keys_all);
    }
    else {
      $keys = format_plural(count($keys_all), '1 key', '@count keys');
      if ($key_default && isset($keys_all[$key_default])) {
        $keys .= ' (' . t('default key = %key', array('%key' => $keys_all[$key_default])) . ')';
      }
      else {
        $keys .= ' (' . t('no default key') . ')';
      }
    }

    // Add list of configured variables + configuration link
    if (!empty($info['select'])) {
      $conf = implode(', ', variable_realm_admin_variables_conf($realm));
      $links = l(t('Configure'), 'admin/config/system/variable/realm/' . $realm);
      $title = l($title, 'admin/config/system/variable/realm/' . $realm);
    }

    // Add list of actual variables
    if (!empty($realm['select'])) {
      $list = implode(', ', variable_realm_admin_variables_list($realm));
    }
    $realms[] = array($title, $keys, $conf, $list, $links);
  }

  $form['realms'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $realms,
    '#empty' => t('No realms available.'),
  );

  return $form;
}

