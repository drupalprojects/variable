<?php
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Node module variables
 */
function node_variable_info($options) {
  $language = $options['language'];
  // Post settings
  $defaults = array('module' => 'node', 'group' => 'node_post_settings');
  $variables['default_nodes_main'] = array(
    'type' => 'select',
    'title' => t('Number of posts on main page'),
    'default' => 10,
    'options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30)),
    'description' => t('The default maximum number of posts to display per page on overview pages such as the main page.')
  ) + $defaults;
  $variables['teaser_length'] = array(
    'type' => 'select',
    'title' => t('Length of trimmed posts'),
    'default' => 600,
    'options' => array(
      0 => t('Unlimited'),
      200 => t('200 characters'),
      400 => t('400 characters'),
      600 => t('600 characters'),
      800 => t('800 characters'),
      1000 => t('1000 characters'),
      1200 => t('1200 characters'),
      1400 => t('1400 characters'),
      1600 => t('1600 characters'),
      1800 => t('1800 characters'),
      2000 => t('2000 characters'),
    ),
    'description' => t("The maximum number of characters used in the trimmed version of a post. Drupal will use this setting to determine at which offset long posts should be trimmed. The trimmed version of a post is typically used as a teaser when displaying the post on the main page, in XML feeds, etc. To disable teasers, set to 'Unlimited'. Note that this setting will only affect new or updated content and will not affect existing teasers.")
  ) + $defaults;
  $variables['node_preview'] = array(
    'type' => 'options',
    'title' => t('Preview post'),
    'default' => 0,
    'options' => array(t('Optional'), t('Required')),
    'description' => t('Must users preview posts before submitting?'),
  ) + $defaults;

  // Content types
  $defaults = array('module' => 'node', 'access' => 'administer content types', 'edit_path' => 'admin/content/types', 'group' => t('Content types'));
  $form['node_options'] = array(
    'build_callback' => 'node_variable_content_type_build',
    'type' => 'options',
    'title' => t('Default options'),
    'default' => array('status', 'promote'),
    'options' => array(
      'status' => t('Published'),
      'promote' => t('Promoted to front page'),
      'sticky' => t('Sticky at top of lists'),
      'revision' => t('Create new revision'),
    ),
    'description' => t('Users with the <em>administer nodes</em> permission will be able to override these options.'),
  ) + $defaults;
  return $variables;
}

/**
 * Node module variable groups
 */
function node_variable_group_info() {
  $groups['node_post_settings'] = array(
    'title' => t('Post settings'),
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