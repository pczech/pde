<?php

/**
 * @file
 * Contains all admin pages, settings, and validate.
 */


// @ignore sniffer_commenting_inlinecomment_spacingafter:file


/**
 * Artwork type form builder.
 */
function artwork_type_form($form, &$form_state, $artwork_type, $op = 'edit') {
  pde_trace_funct();
  // dpm($form);
  // dpm($form_state);
  // dpm($artwork_type);
  // dpm($op);

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($artwork_type->label) ? $artwork_type->label : '',
    '#description' => t('The human-readable name of this artwork type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#title' => t('Type'),
    '#default_value' => isset($artwork_type->type) ? $artwork_type->type : '',
    '#maxlength' => 32,
    '#description' => t('A unique machine-readable name for this artwork type.
      It must only contain lowercase letters, numbers, and underscores.'),
    '#machine_name' => array(
      'exists' => 'artwork_types_eapi',
      'source' => array('label'),
    ),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' =>
    isset($artwork_type->description) ? $artwork_type->description : '',
    '#description' => t('A description of this artwork type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $actions = &$form['actions'];
  $actions['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  if ($op == 'delete') {
    $actions['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 45,
    );
  }

  return $form;
}

/**
 * Artwork type form submit handler.
 */
function artwork_type_form_submit($form, &$form_state) {
  pde_trace_funct();

  $artwork_type = entity_ui_form_submit_build_entity($form, $form_state);
  // dpm($artwork_type, 'artwork_type');
  // Save and go back.
  $artwork_type->save();
  $form_state['redirect'] = 'admin/structure/artworks';
}
