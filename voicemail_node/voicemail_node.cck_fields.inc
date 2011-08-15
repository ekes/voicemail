<?php
/**
 * @file
 *   CCK voicemail greeting field.
 */

/**
 * Internal Functions
 */

/**
 * Change the status of the cck audio fields to the status of the voicemail box.
 * @param $node_type
 *   A valid content type, in which the audio fields needs to be changed.
 * @param $status
 *   Boolean value for the status, enabled/disabled
 */
function _voicemail_node_change_audio_fields_status($node_type, $status = 1) {
  $fields = _voicemail_node_audio_fields_definitions();
  foreach ($fields as $key => $field) {
    db_query("UPDATE {". content_field_tablename() ."} SET active = %d WHERE field_name = '%s' AND module = '%s'", $status, $field['field_name'], 'filefield');
    db_query("UPDATE {". content_instance_tablename() ."} SET widget_active = %d WHERE field_name = '%s' AND type_name = '%s' AND widget_module = '%s'", $status, $field['field_name'], $node_type, 'audiofield');
  }
  content_clear_type_cache(TRUE);
} 

/**
 * Setup and created the provided audio cck fields from the cck_fields file.
 * Uses the core crud operations of the content module.
 * @param $node_type
 *   The content type in which these fields needs to be created.
 */
function _voicemail_node_create_audio_fields($node_type) {
  // Need to load the CCK include file where content_field_instance_create() is defined.
  module_load_include('inc', 'content', 'includes/content.crud');
  $fields_to_create = _voicemail_node_audio_fields_definitions();
  foreach ($fields_to_create as $key => $field) {
    if (!content_fields($field['field_name'], $node_type)) {
      $field['type_name'] = $node_type;
      content_field_instance_create($field);
    }
  }
} 

/**
 * VOIP CCK field and group settings
 */

/**
 * Returns an array of the fields names that we control.
 */
function _voicemail_node_audio_fields_names() {
  $definitions = _voicemail_node_audio_fields_definitions();
  return array_keys($definitions);
}

/**
 * Store and returns field definitions, from audiofield cck fields
 * It stores three cck fields, Audio name, Audio description and Audio greeting
 * @param $field_type
 *   If the field type is specified then it returns only specific audio field.
 * @return
 *   An array having the structured format of all or the required audio cck fields.
 */
function _voicemail_node_audio_fields_definitions($field_type = '') {
  $fields = array();
  $fields['field_voicemail_greeting'] =  array (
    'label' => 'Voicemail greeting',
    'field_name' => 'field_voicemail_greeting',
    'type' => 'filefield',
    'widget_type' => 'audiofield_widget',
    'change' => 'Change basic information',
    'weight' => '15',
    'file_extensions' => 'mp3',
    'progress_indicator' => 'bar',
    'file_path' => '',
    'max_filesize_per_file' => '',
    'max_filesize_per_node' => '',
    'description' => '',
    'group' => 'group_voipnode_settings',
    'required' => 0,
    'multiple' => '0',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'op' => 'Save field settings',
    'module' => 'filefield',
    'widget_module' => 'audiofield',
    'columns' =>
    array (
      'fid' =>
      array (
        'type' => 'int',
        'not null' => false,
        'views' => true,
      ),
      'list' =>
      array (
        'type' => 'int',
        'size' => 'tiny',
        'not null' => false,
        'views' => true,
      ),
      'data' =>
      array (
        'type' => 'text',
        'serialize' => true,
        'views' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
      'full' =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
      4 =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
      'token' =>
      array (
        'format' => 'audiofield_embedded',
        'exclude' => 0,
      ),
    ),
  );

  if (isset($fields[$field_type])) {
    return $fields[$field_type];
  }
  else {
    return $fields;
  }
}
