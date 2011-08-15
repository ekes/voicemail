<?php
/**
 * @file
 *
 * Admin pages and forms.
 */

/**
 * Menu callback; admin settings form.
 */
function voicemail_node_admin_settings() {
  $form = array();

  $form['voicemail_node_default_status'] = array(
    '#type' => 'radios',
    '#title' => t('Default voicemail box status'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    '#default_value' => variable_get('voicemail_node_default_status', 0),
  );

  return system_settings_form($form);
}


/**
 * Alter node type form to support Voicemail feature
 *
 * @param $form
 * @param $form_state
 */
function _voicemail_node_type_form_alter(&$form, &$form_state) {
  $voip_voicemail_weight = 34; // default weight of the fieldset.
  $content_type = $form['#node_type']->type;
  if (isset($form['submit']['#weight'])) {
    $voip_voicemail_weight = $form['submit']['#weight'] - 0.98;
  }
  // VoIP Node can be used for voicemail.
  if (variable_get('voipnode_use_'. $content_type, FALSE)) {
    $form['voipnode']['voicemail_node_use'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use this content type as a Voice Mail'),
      '#default_value' => variable_get('voicemail_node_use_'. $content_type, FALSE),
      '#attributes' => array(
        'onclick' => 'javascript: if (jQuery(this).attr("checked")) {
            jQuery("fieldset#node_type_form_voicemail_node_settings_id").show("slow");
            jQuery("fieldset#node_type_form_voicemail_node_configuration_id").show("slow");
          }
          else {
            jQuery("fieldset#node_type_form_voicemail_node_settings_id").hide("fast");
            jQuery("fieldset#node_type_form_voicemail_node_configuration_id").hide("fast");
          };'
      ),
    );
  }
  $voicemail_node_settings_style = 'display:none;';
  if (variable_get('voicemail_node_use_'. $content_type, FALSE)) {
    $voicemail_node_settings_style = 'display:block;';
  }
  $form['voipnode']['voicemail_node_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Voicemail Default Settings'),
    '#group' => 'additional_settings',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'style' => $voipnode_settings_style,
      'id' => 'node_type_form_voicemail_node_settings_id'
    ),
  );
  $default_status = variable_get('voicemail_node_default_status_'. $content_type, variable_get('voicemail_node_default_status', 0));
  $form['voipnode']['voicemail_node_settings']['voicemail_node_default_status'] = array(
    '#type' => 'radios',
    '#title' => t('Default voicemail box status'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    '#default_value' => $default_status ? $default_status : 0
  );

  if (variable_get('voicemail_node_use_'. $content_type, FALSE)) {
    $form['voipnode']['voicemail_node_fields_configuration'] = array(
      '#type' => 'fieldset',
      '#title' => t('Voicemail field configuration'),
      '#group' => 'additional_settings',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array(
        'id' => 'node_type_form_voicemail_node_configuration_id',
      ),
    );
    $voicemail_node_configuration_options = '<div class="voipnode-'. $content_type .'-configuration-options">';
    $voicemail_node_configuration_options .= l(t('Configure voicemail greeting field'), 'admin/content/node-type/'. $content_type .'/fields/field_voicemail_greeting');
    $voicemail_node_configuration_options .= '</div>';
    $form['voipnode']['voicemail_node_fields_configuration']['configuration_options'] = array(
      '#type' => 'markup',
      '#value' => $voicemail_node_configuration_options,
    );
  }
  $form['#submit'][] = 'voicemail_node_type_form_submit';
}

function _voicemail_node_type_form_submit($form, &$form_state) {
  module_load_include('inc', 'voipnode', 'voipnode.cck_fields');

  $content_type = $form_state['values']['type'];
  if ($form_state['values']['voicemail_node_use']) {
    // Enable voipnode on type. add fields and group.
    // Adding extensions to existing nodes is an admin task. 
    // Using the admin task existing disabled extensions can all be defaulted to enabled.
    _voicemail_node_change_audio_fields_status($content_type, 1);
    _voicemail_node_create_audio_fields($content_type);
    $group_name = 'group_voipnode_settings';
    $fields = array(
      'field_voicemail_greeting',
    );
    // Assign the voip extension fields to the selcted field group.
    foreach ($fields as $index => $field_name) {
      $form_values = array();
      $form_values['type_name']  = $content_type;
      $form_values['group']      = $group_name;
      $form_values['field_name'] = $field_name;
      fieldgroup_update_fields($form_values);
    }

    drupal_set_message(t('Voicemail has been associated with %content_type', array('%content_type' => $content_type)));
  }
  else {
    // Disable voipnode on type. Hide the audio fields, remove the fieldset, disable the extensions.
    _voicemail_node_change_audio_fields_status($content_type, 0);
    drupal_set_message(t('Voicemail has been disabled for %content_type', array('%content_type' => $content_type)));
  }
}
