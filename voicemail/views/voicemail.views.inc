<?php
/**
 * @file
 *
 * Views 2 support for Voice Mail
 */

/**
 * Implementation of hook_views_data().
 */
function voicemail_views_data() {
  $data = array();

  // table information
  $data['voicemail_box']['table']['group'] = t('VoiceMail');
  $data['voicemail_message']['table']['group'] = t('VoiceMail');

  $data['voicemail_box']['table']['join'] = array(
    'voipextension' => array(
      'left_field' => 'eid',
      'field' => 'eid',
    ),
    'files' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
    'voicemail_message' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );

  $data['voicemail_message']['table']['join'] = array(
    'voicemail_box' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
    'files' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );
 
  $data['voicemail_box']['eid'] = array(
    'title' => t('Extension'),
    'help' => t('Extension associated with voicemail box.'),
        'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title',
      'zero is null' => TRUE,
    ),
    'filter' => array(
       //
    ),
  );
  $data['voicemail_box']['is_new'] = array(
    'title' => t('Is new'),
    'help' => t('Message in mailbox is marked as new.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'isnew' => array(t('New'), ''),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Is new'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // probably could make its own handler...
  $data['voicemail_box']['folder'] = array(
    'title' => t('Folder'),
    'help' => t('The folder a message is in.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // alternatively can force the join on the files table.
  $data['files']['voicemail_player'] = array(
    'title' => t('Message'),
    'help' => t('Message in audiofield player.'),
    'field' => array(
      'field' => 'filepath',
      'handler' => 'voicemail_views_handler_field_audioplayer',
      'group' => 'Voicemail',
    ),
  );

  $data['voicemail_message']['caller_name'] = array(
    'title' => t('Caller name'),
    'help' => t('Name of caller of the voicemail.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['voicemail_message']['caller_number'] = array(
    'title' => t('Caller Number'),
    'help' => t('Caller Number of the voicemail.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['voicemail_message']['call_id'] = array(
    'title' => t('Call ID'),
    'help' => t('ID of the call leaving the voicemail.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['voicemail_message']['sent_on'] = array(
    'title' => t('Sent On'),
    'help' => t('Date and time on which voicemail was sent.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['voicemail_message']['fid_delete'] = array(
    'title' => t('Delete'),
    'help' => t('Link to remove the message from the voicemail box.'),
    'field' => array(
      'handler' => 'voicemail_views_handler_field_file_delete',
      'field' => 'fid',
    ),
  );
  $data['voicemail_message']['fid_isnew'] = array(
    'title' => t('Mark read'),
    'help' => t('Link to mark the message as no longer new.'),
    'field' => array(
      'handler' => 'voicemail_views_handler_field_isnew_toggle',
      'field' => 'fid',
    ),
  );

  return $data;
}

function voicemail_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'voicemail') . '/views',
    ),
    'handlers' => array(
      'voicemail_views_handler_field_audioplayer' => array(
        'parent' => 'views_handler_field_file',
      ),
      // psudo fields
      'voicemail_views_handler_field_file_delete' => array(
        'parent' => 'views_handler_field',
      ),
      'voicemail_views_handler_field_isnew_toggle' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
