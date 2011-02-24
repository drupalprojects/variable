<?php
// $Id$
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * Implements hook_variable_option_info()
 */
function taxonomy_variable_option_info() {
  $options['vocabulary_vid'] = array(
    'title' => t('Vocabulary'),
    'callback' => 'taxonomy_variable_vocabulary_vid_list',
  );
  $options['vocabulary_name'] = array(
    'title' => t('Vocabulary'),
    'callback' => 'taxonomy_variable_vocabulary_name_list',
  );
  return $options;
}

/**
 * Options callback for vocabulary
 */
function taxonomy_variable_vocabulary_vid_list($variable, $options) {
  static $list;
  if (!isset($list)) {
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $list[$vocab->vid] = $vocab->name;
    };
  }
  return $list;
}

/**
 * Options callback for vocabulary
 */
function taxonomy_variable_vocabulary_name_list($variable, $options) {
  static $list;
  if (!isset($list)) {
    foreach (taxonomy_get_vocabularies() as $vocab) {
      $list[$vocab->machine_name] = $vocab->name;
    };
  }
  return $list;
}