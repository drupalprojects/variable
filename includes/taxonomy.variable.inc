<?php
// $Id$
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Implements hook_variable_type_info()
 */
function taxonomy_variable_type_info() {
  $type['select_taxonomy_vocabulary'] = array(
    'title' => t('Vocabulary'),
    'element' => array('#type' => 'select'),
    'options callback' => 'taxonomy_variable_vocabulary_list',
    'element callback' => 'variable_form_element_options',
    'format callback' => 'variable_format_option',
    'localize' => TRUE,
  );
  return $type;
}

/**
 * Options callback for vocabulary
 */
function taxonomy_variable_vocabulary_list() {
  static $list;
  if (!isset($list)) {
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $list[$vocab->vid] = $vocab->name;
    };
  }
  return $list;
}