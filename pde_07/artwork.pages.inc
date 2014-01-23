<?php

/**
 * @file
 * Artwork entity management pages' callbacks.
 */

// @ignore sniffer_commenting_inlinecomment_spacingafter:file


/**
 * Overview types page callback.
 */
function artwork_pcb_overview_types() {
  det_trace_funct();

  foreach (_artwork_types() as $type => $info) {
    $type_url = str_replace('_', '-', $type);
    $label = t('View @type', array('@type' => $info->name));
    $items[] = l($label, 'admin/structure/artworks/manage/' . $type_url);
  }

  return theme('item_list', array('items' => $items));
}


/**
 * Manage types page callback.
 */
function artwork_pcb_manage_types($artwork_type) {
  det_trace_funct();

  return $artwork_type->name . ': ' . $artwork_type->description;
}


/**
 * Add page callback.
 */
function artwork_pcb_add_page() {
  det_trace_funct();

  $item = menu_get_item();
  $links = system_admin_menu_block($item);
  // dpm($item);
  // dpm($links);

  foreach ($links as $link) {
    $items[] = l($link['title'], $link['href'], $item['localized_options'])
      . ': ' . filter_xss_admin($link['description']);
  }

  // dpm($items);
  return theme('item_list', array('items' => $items));
}


/**
 * Add artwork callback.
 */
function artwork_pcb_add($type) {
  det_trace_funct();

  $type = isset($type) ? str_replace('-', '_', $type) : NULL;
  $types = _artwork_types();
  if (empty($types[$type])) {
    return MENU_NOT_FOUND;
  }

  $artwork = entity_get_controller('artwork')->create($type);
  drupal_set_title(t('Create @name',
    array('@name' => $types[$type]->name)), PASS_THROUGH);

  // Every artwork type has a unique dynamic name of the form, so that
  // hook_form_alter implementations may uniquely target a specific artwork
  // type, however all artwork types have the same form creation function.
  // The mapping of various form IDs to common form builder function is
  // realized in hook_forms.

  // By default, when drupal_get_form() is called, the system will look for a
  // function with the same name as the form ID, and use that function to build
  // the form. If no such function is found, Drupal calls hook_forms.
  return drupal_get_form('artwork_form_' . $type, $artwork);
}


/**
 * View artwork callback.
 */
function artwork_pcb_view($artwork, $view_mode = 'full') {
  det_trace_funct();
  // dpm($artwork);

  if ($view_mode == 'teaser') {
    // @todo handle teaser mode.
    dpm($view_mode);
  }

  // Prepare field data prior to display.
  // INFO: $artwork is reference to object, so it can be modified if passed to
  // a function.
  field_attach_prepare_view('artwork', array($artwork->aid => $artwork), $view_mode);
  // dpm($artwork);

  // Invoke hook_entity_prepare_view() to allow other modules to alter the
  // objects during ENTITY_build_content() or ENTITY_view_multiple() phases of
  // rendering.
  entity_prepare_view('artwork', array($artwork->aid => $artwork));
  // dpm($artwork);

  // Create renderable array for the attached fields and return it.
  return field_attach_view('artwork', $artwork, $view_mode);
}


/**
 * Edit artwork callback.
 */
function artwork_pcb_edit($artwork) {
  det_trace_funct();
  // dpm($artwork);

  $types = _artwork_types();
  drupal_set_title(t('<em>Edit @type</em> @title', array(
    '@type' => $types[$artwork->type]->name,
    '@title' => $artwork->title)), PASS_THROUGH);

  return drupal_get_form('artwork_form_' . $artwork->type, $artwork);
}


/**
 * Creates form for "artwork_$type_form".
 */
function artwork_form_any($form, &$form_state, $artwork) {
  det_trace_funct();
  // dpm($form);
  // dpm($form_state);
  // dpm($artwork);

  // $form['#id'] = 'artwork-form';

  // Save the artwork object for later, in case we need it.
  $form_state['artwork'] = $artwork;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $artwork->title,
    '#weight' => -5,
    '#required' => TRUE,
  );

  $form['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision'),
    '#access' => user_access('administer artworks'),
    '#default_value' => 0,
  );

  // Buttons.
  $form['buttons'] = array(
    '#weight' => 100,
  );

  if (1) {
    $form['buttons']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 5,
      '#submit' => array('artwork_form_any_submit_save'),
    );
  }

  if (!empty($artwork->aid)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#access' => user_access('delete artworks'),
      '#weight' => 15,
      '#submit' => array('artwork_form_any_submit_delete'),
    );
  }

  $form['#validate'][] = 'artwork_form_any_validate';

  // Field Attach API
  // The field_attach_form() call is what passes our form off to the Field
  // system so that any fields we've added to this artwork type can be added
  // to the form.
  field_attach_form('artwork', $artwork, $form, $form_state);

  return $form;
}


/**
 * For test.
 *
 * It is called if custom (in $form['#validate']) validation function is not
 * specified. See $form['#validate'][] = 'artwork_form_any_validate'.
 */
function artwork_form_painting_validate() {
  det_trace_funct();
}


/**
 * For test.
 *
 * It is called if custom (in $form['#validate']) validation function is not
 * specified. See $form['#validate'][] = 'artwork_form_validate'.
 */
function artwork_form_sculpture_validate() {
  det_trace_funct();
}


/**
 * Artwork form validation function.
 */
function artwork_form_any_validate($form, &$form_state) {
  det_trace_funct();
  // dpm($form);
  // dpm($form_state);

  // Field Attach API
  // In this case any validation we'd do (checking for required fields) the
  // Form API already does for us. All we need do is tell the Field system
  // to do its own validation, by calling field_attach_form_validate.
  $artwork = $form_state['artwork'];

  // dpm($artwork);
  // Fields attached to the entity artwork by Field Attach API, are added to
  // $artwork object by field_attach_form_validate.
  field_attach_form_validate('artwork', $artwork, $form, $form_state);
  // dpm($artwork);
}


/**
 * For test.
 *
 * It is called if custom (in $form['buttons']['save']['#submit']) submit
 * function is not specified. See buttons definition in the form.
 */
function artwork_form_painting_submit() {
  det_trace_funct();
}


/**
 * For test.
 *
 * It is called if custom (in $form['buttons']['save']['#submit']) submit
 * function is not specified. See buttons definition in the form.
 */
function artwork_form_sculpture_submit() {
  det_trace_funct();
}


/**
 * Submit function for 'save' button.
 */
function artwork_form_any_submit_save($form, &$form_state) {
  det_trace_funct();
  // dpm($form);
  // dpm($form_state);

  $artwork = $form_state['artwork'];
  // dpm($artwork)

  // Fields attached to the entity by Field Attach API, are already present in
  // $artwork object, so it is not needed to add them here.
  $artwork->title = $form_state['values']['title'];
  $artwork->revision = $form_state['values']['revision'];

  // Field Attach API
  // Notify fields widgets. For what?..
  field_attach_submit('artwork', $artwork, $form, $form_state);

  // Save the artwork.
  _artwork_save($artwork);

  drupal_set_message(t('Artwork saved'));

  $form_state['redirect'] = 'artwork/' . $artwork->aid;
}


/**
 * Submit function for 'delete' button.
 */
function artwork_form_any_submit_delete($form, &$form_state) {
  det_trace_funct();
  // dpm($form);
  // dpm($form_state);

  $artwork = $form_state['artwork'];

  // Redirect to delete page.
  $form_state['redirect'] = 'artwork/' . $artwork->aid . '/delete';
}


/**
 * Artwork delete confirm form constructor.
 */
function artwork_form_any_delete_cfm($form, &$form_state, $artwork) {
  det_trace_funct();

  $form['aid'] = array(
    '#type' => 'value',
    '#value' => $artwork->aid,
  );

  $form = confirm_form($form,
      t('Are you sure you want to delete %title?', array('%title' => $artwork->title)),
      'artwork/' . $artwork->aid);

  // dpm($form);
  return $form;
}


/**
 * Submit function for deleting 'confirm' button.
 */
function artwork_form_any_delete_cfm_submit($form, &$form_state) {
  det_trace_funct();
  // dpm($form);
  // dpm($form_state);

  // 'confirm' parameter is added by confirm_form
  if ($form_state['values']['confirm']) {
    $artwork = artwork_id_load($form_state['values']['aid']);
    _artwork_delete($form_state['values']['aid']);

    watchdog('artwork', '@type: deleted %title', array(
      '@type' => $artwork->type, '%title' => $artwork->title));

    $types = _artwork_types();
    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => $types[$artwork->type]->name,
      '%title' => $artwork->title)));
  }
  $form_state['redirect'] = '<front>';
}
