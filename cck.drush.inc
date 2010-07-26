<?php
function cck_drush_command() {
  $items = array();
  
  $items['cck'] = array(
    'description' => "List current content types.",
    'examples' => array(
      'drush cck' => 'List all the content types.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['cck-list'] = array(
    'description' => "List current fields for a content type.",
    'arguments' => array(
      'content_type' => 'Content type you whose field you would like see.',
      'field_name' => 'Field you would like see information about.',
    ),
    'examples' => array(
      'drush cck-list content_type' => 'List all of the CCK fields for this content type.',
      'drush cck-list content_type field_name' => 'Display more detailed information about a field.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['cck-add-textfield'] = array(
    'description' => "Create a basic textfield for a content type.",
    'arguments' => array(
      'content_type' => 'Content type you would like to create field on.',
      'field' => 'The field name you would like to use, add the field_ prefix.',
    ),
    'options' => array(
      '--label' => 'Human readable label.',
    ),
    'examples' => array(
      'drush cck-add-textfield content_type field_name --label="Label"' => 'Create a basic textfield on a content type with a field name and a human readable label.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['cck-delete'] = array(
    'description' => "Delete a field from a content type.",
    'arguments' => array(
      'content_type' => 'Content type whose field you would like to delete.',
      'field_names' => 'A space separated list of field you would like to delete.',
    ),
    'examples' => array(
      'drush cck-delete content_type field_name' => 'Delete this field from the content type.',
      'drush cck-delete content_type field_one field_two' => 'Delete fields from the content type.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  
  $items['cck-add-integer'] = array(
    'description' => "Create a numeric integer field for a content type.",
    'arguments' => array(
      'content_type' => 'Content type you would like to create field on.',
      'field' => 'The field name you would like to use, add the field_ prefix.',
    ),
    'options' => array(
      '--label' => 'Human readable label.',
    ),
    'examples' => array(
      'drush cck-add-integer content_type field_name --label="Label"' => 'Create a numeric integer field on a content type with a field name and a human readable label.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

function drush_drush_help($section) {
  switch ($section) {
    case 'drush:cck':
      return dt("This command will list your content types.");
    case 'drush:cck-list':
      return dt("This command will list your CCK content fields for a provided content type.");
    case 'drush:cck-add-textfield':
      return dt("This command will add a CCK textfield.");
    case 'drush:cck-delete':
      return dt("This command will delete CCK content fields.");
    case 'drush:cck-add-decimal':
      return dt("This command will add a CCK integer field.");
  }
}

function drush_cck() {
  $content_types = content_types();
  foreach (array_keys($content_types) as $content_type) {
    drush_print($content_type); 
  }
}

function drush_cck_list($content_type, $field_name = NULL) {
  $type = content_types($content_type);
  $fields = $type['fields'];
  // $field_types = _content_field_types();
  $info_table = array();
  $info_table['content type'] = $content_type;
  $info_table['field'] = $field_name;
  
  // Maybe swap this all for a nice big table view.
  if($field = $fields[$field_name]) {
    $options = array('type', 'required', 'multiple');
    foreach($options as $option) {
      $info_table[$option] = $field[$option];
    }
    drush_print_table(drush_key_value_to_array_table($info_table));
  }
  else {
    foreach (array_keys($fields) as $field) {
      drush_print($field);
    }
  }
}

function drush_cck_add_textfield($content_type, $field_name) {
  $field = array(
    'field_name' => $field_name,
    'type_name' => $content_type,
    'type' => 'text',
    'widget_type' => 'text_textfield',
  );
  
  // Look into $additional_options to do this kind of stuff and then just loop through any extra arguments.
  if($label = drush_get_option('label')) {
    $field['label'] = $label;
  }
  
  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');
  print_r(content_field_instance_create($field));
}

function drush_cck_delete($type_name) {
  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');
  
  $fields = func_get_args();
  array_shift($fields); // Remove content type name.
  foreach($fields as $field_name) {
    print_r(content_field_instance_delete($field_name, $type_name));
  }
}

function drush_cck_add_integer($content_type, $field_name) {
  $field = array(
    'field_name' => $field_name,
    'type_name' => $content_type,
    'type' => 'number_integer',
    'widget_type' => 'number',
  );
  
  // Look into $additional_options to do this kind of stuff and then just loop through any extra arguments.
  if($label = drush_get_option('label')) {
    $field['label'] = $label;
  }
  
  include_once('./'. drupal_get_path('module', 'content') .'/includes/content.crud.inc');
  print_r(content_field_instance_create($field));
}