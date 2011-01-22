<?php
// $Id$
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Implements hook_variable_info().
 */
function menu_variable_info($options) {
  $variables['menu_main_links_source'] = array(
    'type' => 'select',
    'title' => t('Source for the Main links'),
    'default' => 'main-menu',
    'element' => array('#empty_option' => t('No Main links')),
    'description' => t('Select what should be displayed as the Main links (typically at the top of the page).', array(), $options),
    'group' => 'menu_settings'
  );
  // @todo get rid of this complex variables for description
  $menu_options = menu_get_menus();  
  $main = variable_get('menu_main_links_source', 'main-menu');
  $variables['menu_secondary_links_source'] = array(
    'type' => 'select_menu',
    'title' => t('Source for the Secondary links'),
    'default' => 'user-menu',
    'element' => array('#empty_option' => t('No Secondary links')),
    'description' => t('Select the source for the Secondary links. An advanced option allows you to use the same source for both Main links (currently %main) and Secondary links: if your source menu has two levels of hierarchy, the top level menu links will appear in the Main links, and the children of the active link will appear in the Secondary links.', array('%main' => $menu_options[$main]), $options),
    'group' => 'menu_settings'
  );
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function menu_variable_group_info() {
  $groups['menu_settings'] = array(
    'title' => t('Menu settings'),
    'access' => 'administer menu',
    'path' => 'admin/structure/menu/settings',
  );
  return $groups;
}

/**
 * Implements hook_variable_type_info()
 */
function menu_variable_type_info() {
  $type['select_menu'] = array(
    'title' => t('Menu'),
    'element' => array('#type' => 'select'),
    'options callback' => 'menu_get_menus',
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
    'localize' => TRUE,
  );
  return $type;
}