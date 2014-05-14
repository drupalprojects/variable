<?php
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * System module variables
 */
function system_variable_info($options) {
  $language = $options['language'];
  // Site configuration
  $defaults = array('module' => 'system', 'group' => 'system_site_information');
  $variables['site_name'] = array(
    'type' => 'string',
    'title' => t('Name'),
    'default' => 'Drupal',
    'description' => t('The name of this website.'),
    'form_element' => array('#required' => TRUE),
  ) + $defaults;
  $variables['site_mail'] = array(
    'type' => 'mail_address',
    'title' => t('E-mail address'),
    'default' => ini_get('sendmail_from'),
    'description' => t("The <em>From</em> address in automated e-mails sent during registration and new password requests, and other notifications. (Use an address ending in your site's domain to help prevent this e-mail being flagged as spam.)"),
    'form_element' => array('#required' => TRUE),
  ) + $defaults;
  $variables['site_slogan'] = array(
    'type' => 'text',
    'title' => t('Slogan'),
    'default' => '',
    'description' => t("Your site's motto, tag line, or catchphrase (often displayed alongside the title of the site).")
  ) + $defaults;
  $variables['site_mission'] = array(
    'type' => 'text',
    'title' => 'Mission',
    'default' => '',
    'description' => t("Your site's mission or focus statement (often prominently displayed on the front page).")
  ) + $defaults;
  $variables['site_footer'] = array(
    'type' => 'text',
    'title' => t('Footer message'),
    'default' => '',
    'description' => t('This text will be displayed at the bottom of each page. Useful for adding a copyright notice to your pages.')
  ) + $defaults;
  $variables['anonymous'] = array(
    'type' => 'string',
    'title' => t('Anonymous user'),
    'default' => t('Anonymous', array(), $language->language),
    'description' => t('The name used to indicate anonymous users.'),
    'form_element' => array('#required' => TRUE),
  ) + $defaults;
  $variables['site_frontpage'] = array(
    'type' => 'drupal_path',
    'title' => t('Default front page'),
    'default' => 'node',
    'description' => t('The home page displays content from this relative URL. If unsure, specify "node".'),
    'form_element' => array('#required' => TRUE),
  ) + $defaults;

  return $variables;
}

/**
 * Node module variable groups
 */
function system_variable_group_info() {
  $groups['system_site_information'] = array(
    'title' => t('Site information'),
    'access' => 'administer site configuration',
    'path' => 'admin/settings'
  );
  // Group for variable that have no group
  $groups['others'] = array(
    'title' => t('Others'),
    'access' => 'administer site configuration',
  );
  return $groups;
}
