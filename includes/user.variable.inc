<?php
/**
 * @file
 * Variable API module. Definition for Drupal core variables
 */

/**
 * User module variables
 */
function user_variable_info($options) {
  $language = $options['language'];
  $defaults = array('module' => 'user', 'group' => 'user_settings');

  // $variables = array('#type' => 'fieldset', 'title' => t('User registration settings'));
  $variables['user_register'] = array(
    'type' => 'select',
    'title' => t('Public registrations'),
    'default' => 1,
    'options' => array(t('Only site administrators can create new user accounts.'), t('Visitors can create accounts and no administrator approval is required.'), t('Visitors can create accounts but administrator approval is required.'))
  ) + $defaults;
  $variables['user_email_verification'] = array(
    'type' => 'boolean',
    'title' => t('Require e-mail verification when a visitor creates an account'),
    'default' => TRUE,
    'description' => t('If this box is checked, new users will be required to validate their e-mail address prior to logging into the site, and will be assigned a system-generated password. With it unchecked, users will be logged in immediately upon registering, and may select their own passwords during registration.')
  ) + $defaults;
  $variables['user_registration_help'] = array(
    'type' => 'text',
    'title' => t('User registration guidelines'),
    'default' => '',
    'description' => t('This text is displayed at the top of the user registration form and is useful for helping or instructing your users.')
  ) + $defaults;

  // User e-mail settings.

  // These email tokens are shared for all settings, so just define
  // the list once to help ensure they stay in sync.
  $email_token_help = t('Available variables are:') .' !username, !site, !password, !uri, !uri_brief, !mailto, !date, !login_uri, !edit_uri, !login_url.';
  $variables['user_mail_register_admin_created'] = array(
    'type' => 'mail_text',
    'title' => t('Welcome, new user created by administrator'),
    'default' => array(
      'subject' => _user_mail_text('register_admin_created_subject', $language),
      'body' => _user_mail_text('register_admin_created_body', $language)
    ),
    'description' => t('Customize welcome e-mail messages sent to new member accounts created by an administrator.') .' '. $email_token_help,
  ) + $defaults;
  $variables['user_mail_register_no_approval_required'] = array(
    'type' => 'mail_text',
    'title' => t('Welcome, no approval required'),
    'default' => array(
      'subject' => _user_mail_text('register_no_approval_required_subject', $language),
      'body' =>  _user_mail_text('register_no_approval_required_body', $language)
     ),
    'description' => t('Customize welcome e-mail messages sent to new members upon registering, when no administrator approval is required.') .' '. $email_token_help
  ) + $defaults;
  $variables['user_mail_register_pending_approval'] = array(
    'type' => 'mail_text',
    'title' => t('Welcome, awaiting administrator approval'),
    'default' => array(
      'subject' => _user_mail_text('register_pending_approval_subject', $language),
      'body' =>  _user_mail_text('register_pending_approval_body', $language)
    ),
    'description' => t('Customize welcome e-mail messages sent to new members upon registering, when administrative approval is required.') .' '. $email_token_help,
  ) + $defaults;
  $variables['user_mail_password_reset'] = array(
    'type' => 'mail_text',
    'title' => t('Password recovery email'),
    'default' => array(
      'subject' => _user_mail_text('password_reset_subject', $language),
      'body' =>  _user_mail_text('password_reset_body', $language)
    ),
    'description' => t('Customize e-mail messages sent to users who request a new password.') .' '. $email_token_help,
  ) + $defaults;
  $variables['user_mail_status_activated'] = array(
    'type' => 'mail_text',
    'title' => t('Account activation email'),
    'default' => array(
      'subject' => _user_mail_text('status_activated_subject', $language),
      'body' =>  _user_mail_text('status_activated_body', $language)
    ),
    'description' => t('Enable and customize e-mail messages sent to users upon account activation (when an administrator activates an account of a user who has already registered, on a site where administrative approval is required).') .' '. $email_token_help,
  ) + $defaults;
  $variables['user_mail_status_activated_notify'] = array(
    'type' => 'boolean',
    'title' => t('Notify user when account is activated.'),
    'default' => TRUE,
  ) + $defaults;
  $variables['user_mail_status_blocked'] = array(
    'type' => 'mail_text',
    'title' => t('Account blocked email'),
    'default' => array(
      'subject' => _user_mail_text('status_blocked_subject', $language),
      'body' =>  _user_mail_text('status_blocked_body', $language)
    ),
    'description' => t('Enable and customize e-mail messages sent to users when their accounts are blocked.') .' '. $email_token_help,
  ) + $defaults;
  $variables['user_mail_status_blocked_notify'] = array(
    'type' => 'boolean',
    'title' => t('Notify user when account is blocked.'),
    'default' => FALSE,
  ) + $defaults;
  $variables['user_mail_status_deleted'] = array(
    'type' => 'mail_text',
    'title' => t('Account deleted email'),
    'default' => array(
      'subject' => _user_mail_text('status_deleted_subject', $language),
      'body' =>  _user_mail_text('status_deleted_body', $language)
    ),
    'description' => t('Enable and customize e-mail messages sent to users when their accounts are deleted.') .' '. $email_token_help,
  ) + $defaults;
  $variables['user_mail_status_deleted_notify'] = array(
    'type' => 'boolean',
    'title' => t('Notify user when account is deleted.'),
    'default' => FALSE,
  ) + $defaults;
  // User signatures.
  $variables['user_signatures'] = array(
    'type' => 'enable',
    'title' => t('Signature support'),
    'default' => 0,
  ) + $defaults;

  // Picture support

  $picture_support = variable_get('user_pictures', 0);
  $form['user_pictures'] = array(
    'type' => 'enable',
    'title' => t('Picture support'),
    'default' => 0,
  ) + $defaults;
  $variables['user_picture_path'] = array(
    'type' => 'file_path',
    'title' => t('Picture image path'),
    'default' => 'pictures',
    'description' => t('Subdirectory in the directory %dir where pictures will be stored.', array('%dir' => file_directory_path() .'/')),
  ) + $defaults;
  $variables['user_picture_default'] = array(
    'type' => 'url',
    'title' => t('Default picture'),
    'default' => '',
    'description' => t('URL of picture to display for users with no custom picture selected. Leave blank for none.'),
  ) + $defaults;
  $variables['user_picture_dimensions'] = array(
    'type' => 'string',
    'title' => t('Picture maximum dimensions'),
    'default' => '85x85',
    'description' => t('Maximum dimensions for pictures, in pixels.'),
    'form_element' => array('#size' => 15, '#maxlength' => 10),
  ) + $defaults;
  $variables['user_picture_file_size'] = array(
    'type' => 'number',
    'title' => t('Picture maximum file size'),
    'default' => 30,
    'description' => t('Maximum file size for pictures, in kB.'),
  ) + $defaults;
  $variables['user_picture_guidelines'] = array(
    'type' => 'text',
    'title' => t('Picture guidelines'),
    'default' => '',
    'description' => t("This text is displayed at the picture upload form in addition to the default guidelines. It's useful for helping or instructing your users."),
  ) + $defaults;

  return $variables;
}

/**
 * Node module variable groups
 */
function user_variable_group_info() {
  $groups['user_settings'] = array(
    'title' => t('User settings'),
    'access' => 'administer users',
    'path' => 'admin/user/settings',
  );
  return $groups;
}