<?php

/**
 * Just a file entry wrapped with audiofield theme.
 */
class voicemail_views_handler_field_file_delete extends views_handler_field {
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
  }

  function render_script($values) {
    $eid = $values->{$this->aliases['eid']};
    $fid = $values->{$this->field_alias};
    $script =  new voipScriptStore();
    $script->addGosub('voicemail_remove_message', array('eid' => $eid, 'fid' => $fid));
    return $script;
  }
    
  function render($values) {
    $eid = $values->{$this->aliases['eid']};
    $fid = $values->{$this->field_alias};
    // @todo make string user definable in field config.
    $link = l(t('Delete'), 'voicemail/box/' . $eid . '/' . $fid . '/delete', array( 'query' => drupal_get_destination()));
    return $link;
  }
}
