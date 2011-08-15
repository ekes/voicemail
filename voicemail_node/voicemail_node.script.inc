<?php

/**
 * @file
 *   voipscripts for voicemail node.
 */

/**
 * Basic default script to read voipnode contents, and record a message.
 */
function _voicemail_node_get_basic_script($node) {
  $body = strip_tags($node->body);

  $script = new VoipScript('voicemail_node_basic');
$script->addLog('voicemail_node_basic');
  if (! empty($node->voicemail_enabled)) {
    if (is_array($node->field_voicemail_greeting) && ! empty($node->field_voicemail_greeting[0]['filepath'])) {
      $greeting = url($node->field_voicemail_greeting[0]['filepath'], array('absolute' => true));
    }
    else {
      $greeting = t('Please leave your message after the tone, and press pound to finish.');
    }
    $script->addGosub('voicemail_basic', array('eid' => $node->voipextension_number, 'prompt' => $greeting));
  }
  else {
    $script->addSay(t('Voicemail not enabled on this extension'));
  }
  $script->addReturn();

  return $script;
}

