<?php

/**
 * @file
 * Commands to test Entity examples.
 */

// @ignore sniffer_commenting_inlinecomment_spacingafter:file
// @ignore sniffer_commenting_functioncomment_missingparamtype:file
// @ignore sniffer_commenting_functioncomment_missingreturntype:file


/**
 * Print the property names and values of the entity with the given type and ID.
 *
 * @param $entity
 *   Either the identifier of the entity, an Entity object or an
 *   EntityMetadataWrapper object.
 * @param $type
 *   Type of the entity. Only needed if $id
 *
 * @return
 *   Table output or, if unsucessful, returns FALSE.
 */
function pde_cmd_print_entity($entity, $type = NULL) {
  // @todo Temporary
  $wrapper = $entity;

  $header = 'Entity (' . $wrapper->type();
  $header .= ') - ID# ' . $wrapper->getIdentifier() . ':';

  $rows = array();
  foreach ($wrapper as $key => $property) {
    // dpm($key);
    // dpm($property);
    $rows[$key] = $property->value();
  }

  $table = '';
  foreach ($rows as $key => $property) {
    $table .= str_pad($key, 15) . ':  ' . $property . '</br>';
  }

  $output = '<pre>' . $header . '</br></br>' . $table . '</pre>';
  return $output;
}


/**
 * CREATE an entity and display its properties.
 *
 * @param $type
 *   Entity type
 * @param $data
 *   Either JSON string, an object or an associative array.
 *
 * @return
 *   The created entity wrapped in an EntityMetadataWrapper.
 */
function pde_cmd_entity_create($type, $data) {
  pde_trace_funct();

  $data = drupal_json_decode($data);

  // entity_create requires entity definition ( by hook_entity_info() ). The
  // schema to store entity in db defined by hook_schema() is not used now.
  // The function returns Entity object with properties defined in $data.
  $entity = entity_create($type, $data);
  $wrapper = entity_metadata_wrapper($type, $entity);
  // dpm($entity);
  // dpm($wrapper);
  // dpm($wrapper->getPropertyInfo());
  $wrapper->save();

  return $wrapper;
}


/**
 * READ an entity and display its properties.
 */
function pde_cmd_entity_read($type, $id) {
  pde_trace_funct();

  $wrapper = entity_metadata_wrapper($type, $id);
  // dpm($wrapper);
  // dpm($wrapper->getPropertyInfo());

  return pde_cmd_print_entity($wrapper);
}
