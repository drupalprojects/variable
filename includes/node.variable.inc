<?php
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Node module variables
 */
function node_variable_info($options) {
  // Per content type options
  $defaults = array('module' => 'node', 'group' => 'node_type_settings', 'multiple callback' => 'node_type_get_names');
  $variables['teaser_length'] = array(
    'type' => 'multiple',
    'title' => t('Length of trimmed posts'),
    'multiple values' => array(
      'type' => 'select',
      'default' => 600,
      'options callback' => 'node_variable_option_list',
    ),
    'description' => t("The maximum number of characters used in the trimmed version of a post. Drupal will use this setting to determine at which offset long posts should be trimmed. The trimmed version of a post is typically used as a teaser when displaying the post on the main page, in XML feeds, etc. To disable teasers, set to 'Unlimited'. Note that this setting will only affect new or updated content and will not affect existing teasers.", array(), $options),
  ) + $defaults;
  $variables['node_preview'] = array(
    'type' => 'multiple',
    'title' => t('Preview before submitting'),
    'multiple values' => array(
      'type' => 'select',
      'default' => DRUPAL_OPTIONAL,
      'options callback' => 'node_variable_option_list',
    ),
    'description' => t('Must users preview posts before submitting?', array(), $options),
  ) + $defaults;
  $variables['node_options'] = array(
    //'build_callback' => 'node_variable_content_type_build',
    'type' => 'multiple',
    'title' => t('Default options', array(), $options),
    'multiple values' => array(
      'type' => 'options',
      'default' => array('status', 'promote'),
      'options callback' => 'node_variable_option_list',
    ),
    'description' => t('Users with the <em>administer nodes</em> permission will be able to override these options.', array(), $options),
  ) + $defaults;
  $variables['node_submitted'] = array(
    'type' => 'multiple',
    'title' => t('Display author and date information.', array(), $options),
    'multiple values' => array(
      'default' => TRUE,
      'type' => 'boolean',
    ),
    'description' => t('Author username and publish date will be displayed.', array(), $options),
  ) + $defaults;
  return $variables;
}

/**
 * Implements hook_variable_option_list().
 */
function node_variable_option_list($variable, $options = array()) {
  switch ($variable['parent']) {
    case 'teaser_length':
      return array(
        0 => t('Unlimited', array(), $options),
        200 => t('200 characters', array(), $options),
        400 => t('400 characters', array(), $options),
        600 => t('600 characters', array(), $options),
        800 => t('800 characters', array(), $options),
        1000 => t('1000 characters', array(), $options),
        1200 => t('1200 characters', array(), $options),
        1400 => t('1400 characters', array(), $options),
        1600 => t('1600 characters', array(), $options),
        1800 => t('1800 characters', array(), $options),
        2000 => t('2000 characters', array(), $options),
      );
    case 'node_preview':
      return array(
        DRUPAL_DISABLED => t('Disabled', array(), $options),
        DRUPAL_OPTIONAL => t('Optional', array(), $options),
        DRUPAL_REQUIRED => t('Required', array(), $options),
      );
    case 'node_options':
      return array(
        'status' => t('Published', array(), $options),
        'promote' => t('Promoted to front page', array(), $options),
        'sticky' => t('Sticky at top of lists', array(), $options),
        'revision' => t('Create new revision', array(), $options),     
      );
  }
}

/**
 * Node module variable groups
 */
function node_variable_group_info() {
  $groups['node_type_settings'] = array(
    'title' => t('Node type settings'),
    'description' => t('Settings for each node type.'),
    'access' => 'administer nodes',
    'path' => 'admin/content/node-settings', 
  );
  return $groups;
}

/**
 * Build content type variables. Spawn for each content type
 */
function node_variable_content_type_build($variable, $options) {
  unset($variable['build_callback']);
  $build = array();
  foreach (node_get_types() as $type) {
    $name = $variable['name'] . '_' . $type->type;
    $build[$name] = $variable;
    $build[$name]['title'] .= ' (' . $type->name . ')';
    $build[$name]['name'] = $name;
  }
  return $build;
}