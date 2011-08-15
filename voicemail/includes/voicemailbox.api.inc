<?php
/**
 * @file
 *
 * Voice Mail Box API
 */

class VoicemailBox {
  /**
   * {voicemail_box_settings}
   */
  public $eid;
  public $send_notifications;
  public $enabled;

  private function __construct($eid) {
    if ($eid) {
      $this->eid = $eid;
    }
    else {
      throw new VoicemailBoxInvalidException(t('Invalid extension'));
    }
  }

  static function createMailbox($eid, $enabled = NULL, $send_notifications = NULL) {
    if (is_null($enabled)) {
      $enabled = variable_get('voicemail_default_enabled', TRUE);
    }
    if (is_null($send_notifications)) {
      $send_notifications = variable_get('voicemail_default_send_notifications', FALSE);
    }

    $mailbox = new self($eid);

    $mailbox->send_notifications = $send_notifications;
    $mailbox->enabled = $enabled;
    // saving already because all settings here; and it will throw an error if it exists already.
    if (! drupal_write_record('voicemail_box_settings', $mailbox)) {
      throw new Exception(t('Failed to create new mailbox'));
    }

    return $mailbox;
  }

  static function loadMailbox($eid) {
    $mailbox = new self($eid);
    $fields = drupal_schema_fields_sql('voicemail_box_settings');
    $fields_string = implode(', ', $fields);
    $result = db_query("SELECT $fields_string FROM {voicemail_box_settings} WHERE eid = %d", $eid);
    if ($result) {
      $box = db_fetch_object($result);
      if (! empty($box)) {
        foreach ($fields as $field) {
          $mailbox->$field = $box->$field;
        }
      }
      else {
        throw new VoicemailBoxNotExistsException(t('Mailbox does not exist'));
      }
    }
    else {
      throw new Exception('Mailbox not loaded.');
    }

    return $mailbox;
  }
 
  /**
   * Save the Voicemail variables programatically by calling save function of
   * this API.
   *
   * @return type
   */
  public function save() {
    drupal_write_record('voicemail_box_settings', $this, 'eid');
  }

  static public function delete($eid) {
    db_query('DELETE FROM {voicemail_box_settings} WHERE eid = %d', $eid);
  }

  /**
   * Check that Voicemail is active or not.
   *
   * @return
   *   Boolean TRUE if success else FALSE
   */
  public function getEnabled() {
    return $this->enabled;
  }

  public function setEnabled($value) {
    $this->enabled = (bool) $value;
  }

  public function getEid() {
    return $this->eid;
  }

  // send_notifications are just not developed, yet

}

class VoicemailBoxNotExistsException extends Exception {}
class VoicemailBoxInvalidException extends Exception {}
