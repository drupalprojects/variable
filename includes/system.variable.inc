<?php
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Implements hook_variable_info().
 */
function system_variable_info($options) {
  $language = $options['language'];
  // Site configuration
  $defaults = array('module' => 'system', 'group' => 'system_site_information');
  $variables['site_name'] = array(
    'type' => 'string',
    'title' => t('Name', array(), $options),
    'default' => 'Drupal',
    'description' => t('The name of this website.', array(), $options),
    'element' => array('#required' => TRUE),
  ) + $defaults;
  $variables['site_mail'] = array(
    'type' => 'mail_address',
    'title' => t('E-mail address', array(), $options),
    'default' => ini_get('sendmail_from'),
    'description' => t("The <em>From</em> address in automated e-mails sent during registration and new password requests, and other notifications. (Use an address ending in your site's domain to help prevent this e-mail being flagged as spam.)", array(), $options),
    'element' => array('#required' => TRUE),
  ) + $defaults;
  $variables['site_slogan'] = array(
    'type' => 'text',
    'title' => t('Slogan', array(), $options),
    'default' => '',
    'description' => t("Your site's motto, tag line, or catchphrase (often displayed alongside the title of the site).", array(), $options)
  ) + $defaults;
  $variables['site_mission'] = array(
    'type' => 'text',
    'title' => 'Mission',
    'default' => '',
    'description' => t("Your site's mission or focus statement (often prominently displayed on the front page).", array(), $options)
  ) + $defaults;
  $variables['site_footer'] = array(
    'type' => 'text',
    'title' => t('Footer message', array(), $options),
    'default' => '',
    'description' => t('This text will be displayed at the bottom of each page. Useful for adding a copyright notice to your pages.', array(), $options)
  ) + $defaults;
  $variables['anonymous'] = array(
    'type' => 'string',
    'title' => t('Anonymous user', array(), $options),
    'default' => t('Anonymous', array(), $options),
    'description' => t('The name used to indicate anonymous users.', array(), $options),
    'element' => array('#required' => TRUE),
  ) + $defaults;
  $variables['site_frontpage'] = array(
    'type' => 'drupal_path',
    'title' => t('Default front page', array(), $options),
    'default' => 'node',
    'description' => t('The home page displays content from this relative URL. If unsure, specify "node".', array(), $options),
    'element' => array('#required' => TRUE),
    'required' => TRUE,
  ) + $defaults;
  $variables['default_nodes_main'] = array(
    'type' => 'select', 
    'title' => t('Number of posts on main page'),
    'default' => 10,
    'options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30)),
    'description' => t('The maximum number of posts displayed on overview pages such as the front page.', array(), $options),
  ) + $defaults;

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function system_variable_group_info() {
  $groups['system_site_information'] = array(
    'title' => t('Site information'),
    'description' => t('Site information'),
    'access' => 'administer site configuration',
    'path' => 'admin/settings'
  );
  return $groups;
}
