<?php
/**
 * @file
 *
 * Default views for voicemail module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function voicemail_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'voicemail_box';
  $view->description = 'Voicemail box';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'files';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'sent_on' => array(
      'label' => 'Sent',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'sent_on',
      'table' => 'voicemail_message',
      'field' => 'sent_on',
      'relationship' => 'none',
    ),
    'caller_number' => array(
      'label' => 'Number',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'caller_number',
      'table' => 'voicemail_message',
      'field' => 'caller_number',
      'relationship' => 'none',
    ),
    'caller_name' => array(
      'label' => 'Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'caller_name',
      'table' => 'voicemail_message',
      'field' => 'caller_name',
      'relationship' => 'none',
    ),
    'voicemail_player' => array(
      'label' => 'Message',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_file' => 0,
      'exclude' => 0,
      'id' => 'voicemail_player',
      'table' => 'files',
      'field' => 'voicemail_player',
      'relationship' => 'none',
    ),
    'fid_isnew' => array(
      'label' => 'Mark read',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'fid_isnew',
      'table' => 'voicemail_message',
      'field' => 'fid_isnew',
      'relationship' => 'none',
    ),
    'fid_delete' => array(
      'label' => 'Delete',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'fid_delete',
      'table' => 'voicemail_message',
      'field' => 'fid_delete',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'eid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'eid',
      'table' => 'voicemail_box',
      'field' => 'eid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'voipuser' => 0,
        'page' => 0,
        'profile' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'folder' => array(
      'operator' => '=',
      'value' => 'inbox',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'folder_op',
        'identifier' => 'folder',
        'label' => 'VoiceMail: Folder',
        'optional' => 1,
        'remember' => 0,
      ),
      'case' => 1,
      'id' => 'folder',
      'table' => 'voicemail_box',
      'field' => 'folder',
      'relationship' => 'none',
    ),
    'is_new' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'is_new',
        'label' => 'Is new',
        'optional' => 1,
        'remember' => 1,
      ),
      'id' => 'is_new',
      'table' => 'voicemail_box',
      'field' => 'is_new',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'sent_on' => 'sent_on',
      'caller_number' => 'caller_number',
      'caller_name' => 'caller_name',
      'voicemail_player' => 'voicemail_player',
      'fid_isnew' => 'fid_isnew',
      'fid_delete' => 'fid_delete',
    ),
    'info' => array(
      'sent_on' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'caller_number' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'caller_name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'voicemail_player' => array(
        'separator' => '',
      ),
      'fid_isnew' => array(
        'separator' => '',
      ),
      'fid_delete' => array(
        'separator' => '',
      ),
    ),
    'default' => 'sent_on',
  ));

  $views[$view->name] = $view;

  return $views;
}
