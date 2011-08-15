<?php
/**
 * @file
 *   Voice Mail Message API to do operations on the Messages sent and Received.
 */


class VoicemailMessage {
  /**
   * {voicemail_message} database fields.
   */
  public $fid; // File ID
  public $caller_name; // Caller Name from the VoIP Extension
  public $caller_number; // Caller Phone Number if any
  public $call_id; // ID of call leaving message
  public $sent_on; // Message sent date

  // VoicemailMessageInstance object
  private $voicemailBoxes;

  // File object loaded from fid
  private $file;

  /**
   * Load selected instance.
   *
   * Should it be restricted to one-per-fid?
   *
   * @param $fid
   *   An integer having message id.
   *
   * @return
   *   An object having all the details of the message if success else Boolean FALSE
   */
  public function __construct($fid = 0) {
    if ($fid) {
      $fields = drupal_schema_fields_sql('voicemail_message');
      $fields_string = implode(', ', $fields);
      $result = db_query("SELECT $fields_string FROM {voicemail_message} WHERE fid = %d", $fid);

      if ($result) {
        $message = db_fetch_object($result);
        foreach ($fields as $field) {
          $this->$field = $message->$field;
        }
        // only load $file if required.
      
        $this->voicemailBoxes = VoicemailMessageInstance::loadMessageBoxes($this);
      }
      else {
        throw new Exception('Message not retrieved.');
      }
    }
  }

  public function getFile($flush = FALSE) {
    if (! $flush && is_object($this->file)) {
      return $this->file;
    }

    $this->file = db_fetch_object(db_query('SELECT f.* FROM {files} f WHERE f.fid = %d', $this->fid));
    // error handling
    return $this->file;
  }

  /**
   * Create a new message.
   *
   * @param $eid
   *   Extension ID this message is for.
   * @param $file
   *   File ID/string url/string path for the audio file of the message.
   * @param $callerNumber
   *   String Number this message is from. Optional.
   * @param $callerName
   *   String name of the caller this message is from. Optional.
   * @param $sentOn
   *   Unix timestamp of time of message. Optional.
   *
   * @return
   *   Message object.
   */
  public static function createMessage($fid, $caller_number = '', $caller_name = '', $call_id = '', $sent_on = 0) {
watchdog('DEBUG', 'VoicemailMessage::createMessage( ' . $fid . ', ' . $caller_number . ', ' . $caller_name . ', ' . $call_id . ', ' . $sent_on . ') ');
    $message = new VoicemailMessage();
    $message->fid = $fid;
    $message->caller_number = $caller_number;
    $message->caller_name = $caller_name;
    $message->call_id = $call_id;

    if (empty($sent_on)) {
      $sent_on = time();
    }
    $message->sent_on = $sent_on;

    $file = $message->getFile();
    if ($file->status == FILE_STATUS_TEMPORARY) {
      $target_dir = variable_get('voicemail_path', file_directory_path() . '/voicemail');
      if (! file_check_directory($target_dir, FILE_CREATE_DIRECTORY)) {
        throw new Exception(t('Target directory path %path invalid', array('%path' => $target_dir)));
      }
      if (! file_move($file, $target_dir . '/' . $file->filename)) {
        throw new Exception(t('Failed to save file to %path', array('%path' => $target_dir . '/' . $file->filename)));
      }
      $file->status = FILE_STATUS_PERMANENT;
      drupal_write_record('files', $file, 'fid');
    }
    $message->getFile(TRUE);
   
    $message->voicemailBoxes = VoicemailMessageInstance::loadMessageBoxes($this);
 
    drupal_write_record('voicemail_message', $message);

    return $message;
  }

  public function saveMessage() {
    return drupal_write_record('voicemail_messages', $this, 'fid');
  }

  public function deleteMessage() {
    if (file_delete($this->filepath)) {
      db_query('DELETE FROM {voicemail_box} WHERE fid=%d', $this->fid);
      db_query('DELETE FROM {voicemail_messages} WHERE fid=%d', $this->fid);
      // unset properties
    }
    else {
      throw new Exception('Error deleting message.');
    }
  }

  public function getMailbox($eid) {
    return $this->voicemailBoxes[$eid];
  }

  public function addMailbox($eid, $folder, $is_new = TRUE) {
    $this->voicemailBoxes[$eid] = VoicemailMessageInstance::addMessageBox($this, $eid, $folder, $is_new);
  }

  public function removeMailbox($eid) {
    if (! isset($this->voicemailBoxes[$eid])) {
      throw new Exception(t('Message not in mailbox %extension', array('%extension' => $eid)));
    }
    $this->voicemailBoxes[$eid]->removeMailbox();
    unset($this->voicemailBoxes[$eid]);
    if (! count($this->voicemailBoxes)) {
      $this->deleteMessage();
    }
  }
}

/**
 * An individial instance of a message, in an extension voicemail folder.
 */
class VoicemailMessageInstance {
  private $message;
  private $eid;
  private $fid;
  public $folder;
  public $is_new;

  private function __construct(&$message, $instance) {
    $this->message = $message;
    $this->fid = $message->fid;
    $this->eid = $instance->eid;
    $this->folder = $instance->folder;
    $this->is_new = $instance->folder;
  }

  static public function loadMessageBoxes(&$message) {
    $boxes = array();

    $fields = drupal_schema_fields_sql('voicemail_box');
    $fields = implode(', ', $fields);
    $result = db_query("SELECT $fields FROM {voicemail_box} WHERE fid = %d", $message->fid);
   
    if ($result) {
      while ($box = db_fetch_object($result)) {
        $boxes[$box->eid] = new VoicemailMessageInstance($message, $box);
      }
    }
    else {
      $boxes = array();
    }

    return $boxes;
  }

  static public function addMessageBox(&$message, $eid, $folder, $is_new = TRUE) {
    $box = (object) array('eid' => $eid, 'fid' => $message->fid, 'folder' => $folder, 'is_new' => $is_new);
    drupal_write_record('voicemail_box', $box);
    return new VoicemailMessageInstance($message, $box);
  }

  static public function setIsnew($eid, $fid, $status) {
    return db_query('UPDATE {voicemail_box} SET is_new = %d WHERE eid = %d AND fid = %d', $status, $eid, $fid);
  }

  public function moveMailbox($folder) {
    $this->folder = $folder;
    drupal_write_record('voicemail_box', $this, array('eid', 'fid'));
  }

  public function removeMailbox() {
    db_query("DELETE FROM {voicemail_box} WHERE eid = %d AND fid = %d", $this->eid, $this->fid);
  }
}
