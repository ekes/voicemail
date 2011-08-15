<?php

/**
 * @file
 *   voipscripts for voicemail node.
 */

/**
 * Basic default script to read voipnode contents, and record a message.
 */
function _voicemail_get_basic_script($extension, $prompt='', $timeout=5, $end_key='#', $max_length=3600, $format='mp3', $play_beep=TRUE) {
  $eid = $extension['eid'];
  $script = new VoipScript('voicemail_basic');
  
  $script->addRecord($prompt, $timeout, $end_key, $max_length, $format, $play_beep);
   /*   - recording_path, with the local location of the file recorded
    *   - recording_public_url, with the remote location of the file recorded if available, otherwise will store local location.
    *   - recording_duration, with the duration in seconds of the recording
    *   - recording_digits, the key (if any) pressed to end the recording
    *   - recording_fid, with the fid of the file recorded
    */

// @todo thought - what about adding access control so the public_url is only available to the 
//       caller that recorded the message. Can be based on the temporary status & filename/path?

  $args = array('@message' => '%recording_public_url');
  $script->addSay(t('You said @message', $args));

  $script->addLabel('accept menu');
  $prompt = t("To accept this recording, press 1.  To record it once again, press 2. To return, press the pound key.");
  $timeout = 5;
  $end_key = '';
  $num_digits = 1;
  $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
  $script->addGotoIf('accept recording', "^%input_digits == '1'");
  $script->addGotoIf('start', "^%input_digits == '2'");
  $script->addGotoIf('end record', "^%input_digits == '#'");
  $script->addSay(t('Invalid input received. Please try again.'));
  $script->addGoto('accept menu');

  $script->addLabel('accept recording');
  $script->addGosub('voicemail_save_message', array('eid' => $eid, 'fid' => '%recording_fid', 'caller_number' => '%caller_number', 'caller_name' => '%caller_name', 'call_id' => '%call_id', 'sender_eid' => 0, 'sent_on' => time()));
  $script->addGoto('end record');

  $script->addLabel('no input received');
  $script->addSay(t("No input received. Please try again."));
  $script->addGoto('start');

  $script->addLabel('end record');
  return $script;
}

/**
 * Script just to save a new message in an inbox.
 *
 * @param $eid
 *   Extension mailbox recieving message.
 * @param $fid
 *   Message file fid.
 * @param $caller_number
 *   VoipCall caller number leaving message.
 * @param $caller_name
 *   VoipCall caller_name leaving message.
 * @param $call_id
 *   VoipCall call_id leaving message.
 * @param $sender_eid
 *   Associated extension leaving the message.
 * @param $sent_on
 *   Time message was sent.
 */
function _voicemail_get_save_message_script($eid, $fid, $caller_number = '', $caller_name = '', $call_id = '', $sent_on = 0) {
  $script = new VoipScript('voicemail_save_message');
watchdog('DEBUG', '_voicemail_get_save_message_script(' . $eid . ', ' . $fid . ', ' . $caller_number . ', ' . $caller_name . ', ' . $call_id . ', '. $sent_on . ')');
  try {
    $message = VoicemailMessage::createMessage($fid, $caller_number, $caller_name, $call_id, $sent_on);
    $message->addMailbox($eid, 'inbox');
    $script->addSay(t('Message saved'));
  }
  catch (Exception $e) {
    watchdog('voicemail', $e->getMessage(), WATCHDOG_ERROR);
    $script->addSay(t('Error saving message'));
  }

  return $script;
}

/**
 * Script to set message isnew value.
 *
 * Script equivalent of page callbacks voicemail_box_file_*read_page().
 * 
 * @param $eid.
 *   Extension mailbox recieving message.
 * @param $fid
 *   Message file fid.
 * @param $isnew
 *   Bool to set the message to.
 */
function _voicemail_set_isnew_script($eid, $fid, $is_new) {
  $script = new VoipScript('voicemail_set_isnew');
  VoicemailMessageInstance::setIsnew($eid, $fid, $is_new);
  $script->addReturn();
  return $script;
}

/**
 * Script to delete message.
 *
 * Script equivalent of page callbacks voicemail_box_file_remove_page().
 *
 * @param $eid.
 *   Extension mailbox recieving message.
 * @param $fid
 *   Message file fid.
 */
function _voicemail_remove_message_script($eid, $fid) {
  $script = new VoipScript('voicemail_remove_message');
  try {
    $message = new VoicemailMessage($fid);
    $message->removeMailbox($eid);
    $script->addSay(t('Message deleted.'));
  }
  catch (Exception $e) {
    watchdog('voicemail', t("Failed to remove message from inbox: @error", array('@error' => $e->getMessage)), WATCHDOG_ERROR);
    $script->addSay(t('Error deleting message.'));
  }
  $script->addReturn();
  return $script;
}
