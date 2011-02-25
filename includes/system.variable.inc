<?php
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Implements hook_variable_info().
 */
function system_variable_info($options) {
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
    'type' => 'select_number', 
    'title' => t('Number of posts on main page'),
    'default' => 10,
    'options' => array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30),
    'description' => t('The maximum number of posts displayed on overview pages such as the front page.', array(), $options),
  ) + $defaults;

  $variables['site_403'] = array(
    'type' => 'drupal_path',
    'title' => t('Default 403 (access denied) page'),
    'default' => '',
    'description' => t('This page is displayed when the requested document is denied to the current user. Leave blank to display a generic "access denied" page.'),
  ) + $defaults;
  $variables['site_404'] = array(
    'type' => 'drupal_path',
    'title' => t('Default 404 (not found) page'),
    'default' => '',
    'description' => t('This page is displayed when no other content matches the requested document. Leave blank to display a generic "page not found" page.'),
  ) + $defaults;
  
  // Feed settings
  $defaults = array('module' => 'system', 'group' => 'system_feed_settings');  
  $variables['feed_description'] = array(
    'type' => 'text',
    'title' => t('Feed description'),
    'default' => '',
    'description' => t('Description of your site, included in each feed.')
  ) + $defaults;
  $variables['feed_default_items'] = array(
    'type' => 'select_number',
    'title' => t('Number of items in each feed'),
    'default' => 10,
    'options' => array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30),
    'description' => t('Default number of items to include in each feed.')
  ) + $defaults;
  $variables['feed_item_length'] = array(
    'type' => 'select',
    'title' => t('Feed content'),
    'default' => 'fulltext',
    'options' => array(
      'title' => t('Titles only', array(), $options),
      'teaser' => t('Titles plus teaser', array(), $options),
      'fulltext' => t('Full text', array(), $options)
    ),
    'description' => t('Global setting for the default display of content items in each feed.')
  ) + $defaults;

  // Regional settings
  $defaults = array('module' => 'system', 'group' => 'system_regional_settings');  
  $variables['site_default_country'] = array(
    'type' => 'select',
    'options' => 'country',
    'title' => t('Default country'),
  ) + $defaults;
  $variables['date_first_day'] = array(
    'type' => 'select',
    'options' => 'weekday',
    'title' => t('First day of week'),
    'default' => 0,
  ) + $defaults;
  $variables['date_default_timezone'] = array(
    'type' => 'select',
    'options' => 'timezone',
    'title' => t('Default time zone'),
    'default callback' => 'date_default_timezone_get',
  ) + $defaults;
  $variables['configurable_timezones'] = array(
    'type' => 'boolean',
    'title' => t('Users may set their own time zone.'),
    'default' => 1,
  ) + $defaults;
  $variables['empty_timezone_message'] = array(
    'type' => 'boolean',
    'title' => t('Remind users at login if their time zone is not set.'),
    'default' => 0,
    'description' => t('Only applied if users may set their own time zone.')
  ) + $defaults;
  $variables['user_default_timezone'] = array(
    'type' => 'select',
    'title' => t('Time zone for new users'),
    'default' => DRUPAL_USER_TIMEZONE_DEFAULT,
    'options' => array(
      DRUPAL_USER_TIMEZONE_DEFAULT => t('Default time zone.', array(), $options),
      DRUPAL_USER_TIMEZONE_EMPTY   => t('Empty time zone.', array(), $options),
      DRUPAL_USER_TIMEZONE_SELECT  => t('Users may set their own time zone at registration.', array(), $options),
    ),
    'description' => t('Only applied if users may set their own time zone.')
  ) + $defaults;
  
  // Regional settings
  $defaults = array('module' => 'system', 'group' => 'system_site_information');  
  $variables['maintenance_mode'] = array(
    'type' => 'boolean',
    'title' => t('Put site into maintenance mode'),
    'default' => 0,
    'description' => t('When enabled, only users with the "Use the site in maintenance mode" <a href="@permissions-url">permission</a> are able to access your site to perform maintenance; all other visitors see the maintenance mode message configured below. Authorized users can log in directly via the <a href="@user-login">user login</a> page.', array('@permissions-url' => url('admin/people/permissions'), '@user-login' => url('user'))),
  ) + $defaults;;
  $variables['maintenance_mode_message'] = array(
    'type' => 'text',
    'title' => t('Maintenance mode message'),
    'default' => t('@site is currently under maintenance. We should be back shortly. Thank you for your patience.', array('@site' => variable_get('site_name', 'Drupal')), $options),
    'description' => t('Message to show visitors when the site is in maintenance mode.')
  ) + $defaults;
  // Theme settings, we may want to localize the logo
  $variables['theme_settings'] = array(
    'type' => 'array',
    'title' => t('Global theme settings.'),
    'group' => 'system_theme_settings',
    'default callback' => 'system_variable_theme_defaults',
    'localize' => TRUE,
  );
  $variables['theme_[theme]_settings'] = array(
    'type' => 'multiple',
    'multiple' => 'theme',
    'title' => t('Theme settings', array(), $options),
    'description' => t('Logo, icons and other specific theme settings.'),
    'repeat' => array(
      'type' => 'array',
      'default callback' => 'system_variable_theme_defaults',
    ),
    'group' => 'system_theme_settings',
    'localize' => TRUE,
  );
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function system_variable_group_info() {
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
  $groups['system_regional_settings'] = array(
    'title' => t('Regional settings'),
    'description' => t('Regional settings'),
    'access' => 'administer site configuration',
  );
  $groups['system_theme_settings'] = array(
    'title' => t('Theme settings'),
    'description' => t('Theme settings'),
    'access' => 'administer site configuration',
  );  
  return $groups;
}

/**
 * Implements hook_variable_type_info().
 */
function system_variable_type_info() {
  $type['drupal_path'] = array(
    'title' => t('Drupal path'),
    'element' => array(
      '#type' => 'textfield', '#size' => 40,
      '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    ),
    'localize' => TRUE,
  );  
  $type['file_path'] = array(
    'title' => t('File path'),
    'default' => conf_path() . '/files',
    'element' => array('#type' => 'textfield', '#maxlength' => 255, '#after_build' => array('system_check_directory')),
  );  
  $type['select_country'] = array(
    'title' => t('Country'),
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
    'default' => '',
    'options callback' => 'country_get_list',
    'element' => array('#type' => 'select', '#attributes' => array('class' => array('country-detect'))),
  );
  return $type;
}

/**
 * Implements hook_variable_option_info()
 */
function system_variable_option_info() {
  $options['weekday'] = array(
    'title' => t('Day of week'),
    'callback' => 'system_variable_option_weekday',
  );
  $options['theme'] = array(
    'title' => t('Theme'),
    'callback' => 'system_variable_option_theme',
    'cache' => TRUE,
  );
  $options['country'] = array(
    'title' => t('Country'),
    'callback' => 'country_get_list',
    'cache' => TRUE,
  );
  $options['timezone'] = array(
    'title' => t('Time zone'),
    'callback' => 'system_time_zones',
    'cache' => TRUE,
  );
  return $options;
}

/**
 * Callback for theme options
 */
function system_variable_option_theme($variable, $options) {
  $list = array();
  foreach (list_themes() as $theme) {
    $list[$theme->name] = $theme->info['name'];
  }
  return $list;  
}

/**
 * Callback for weekday options
 */
function system_variable_option_weekday($variable, $options) {
  return array (
    0 => t('Sunday', array(), $options),
    1 => t('Monday', array(), $options),
    2 => t('Tuesday', array(), $options),
    3 => t('Wednesday', array(), $options),
    4 => t('Thursday', array(), $options),
    5 => t('Friday', array(), $options),
    6 => t('Saturday', array(), $options)
  );  
}

/**
 * Callback for default theme settings
 */
function system_variable_theme_defaults($variable, $options) {
  return array(
    'default_logo' => 1, 
    'logo_path' => '', 
    'default_favicon' => 1, 
    'favicon_path' => '',
    'favicon_mimetype' => 'image/vnd.microsoft.icon',
  );
}