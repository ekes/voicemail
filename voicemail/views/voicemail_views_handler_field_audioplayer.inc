<?php

/**
 * Just a file entry wrapped with audiofield theme.
 */
class voicemail_views_handler_field_audioplayer extends views_handler_field_file {
  function render($values) {
    $audiofile = file_create_url($values->{$this->field_alias});
    $output = audiofield_get_player($audiofile, 'mp3'); // @todo wav - add mime to init 
    return $output;
  }
}
