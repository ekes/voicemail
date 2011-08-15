<?php

/**
 * Just a file entry wrapped with audiofield theme.
 */
class voicemail_views_handler_field_isnew_toggle extends views_handler_field {
  function element_type() {
    // voipviews aware: if call just toggle; otherwise display link.
    if (is_a($this->view->display_handler, 'voipviews_views_plugin_display')) {
      return 'voipviews';
    }
    else {
      return 'span';
    }
  }

  function init(&$view, $options) {
    parent::init($view, $options);
    $this->additional_fields['eid'] = array('table' => 'voicemail_box', 'field' => 'eid');
    $this->additional_fields['is_new'] = array('table' => 'voicemail_box', 'field' => 'is_new');
  }

  function render_script($values) {
    $eid = $values->{$this->aliases['eid']};
    $fid = $values->{$this->field_alias};
    $is_new = $values->{$this->aliases['is_new']};

    $script = new voipScriptStore();
    $script->addGosub('voicemail_set_isnew', array('eid' => $eid, 'fid' => $fid, 'is_new' => $is_new ? 0 : 1));
    return $script;
  }
 
  function render($values) {
    $eid = $values->{$this->aliases['eid']};
    $fid = $values->{$this->field_alias};
    $is_new = $values->{$this->aliases['is_new']};
    if ($is_new) {
      $output = l(t('Mark as seen'), 'voicemail/box/' . $eid . '/' . $fid . '/read', array( 'query' => drupal_get_destination()));
    }
    else {
      $output = l(t('Mark not seen'), 'voicemail/box/' . $eid . '/' . $fid . '/unread', array( 'query' => drupal_get_destination()));
    }
    return $output;
  }
}
