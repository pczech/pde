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

  // dpm($wrapper->value()); returns entity object.
  // user: stdClass object
  // node: stdClass object
  // ingredient: Entity object (see hook_entity_info())

  $rows = array();
  // It is possible to use foreach ($wrapper ... ) to iterate properties,
  // because it implements IteratorAggregate interface. The individual
  // properties can be accessed by name, for example $wrapper->name.
  foreach ($wrapper as $key => $property) {
    if ($property instanceof EntityValueWrapper) {
      $rows[$key] = $property->value();
    }
    else {
      // For example: user->roles property is of class EntityListWrapper,
      // then label is created as list of array values.
      $rows[$key] = $property->label();
    }
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
 *   An object or an associative array.
 *
 * @return
 *   The created entity wrapped in an EntityMetadataWrapper.
 */
function pde_cmd_entity_create($type, $data) {
  pde_trace_funct();

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


/**
 * UPDATE an entity and display its properties.
 *
 * @param $type
 *   Entity type.
 * @param $id
 *   Entity ID.
 * @param $data
 *   An object or an associative array.
 */
function pde_cmd_entity_update($type, $id, $data) {
  pde_trace_funct();

  $wrapper = entity_metadata_wrapper($type, $id);

  // Updating properties requires to define 'setter callback', otherwise
  // exception "EntityMetadataWrapperException: Entity property name doesn't
  // support writing." is thrown. See pde_02_entity_property_info_alter().
  foreach ($data as $property => $value) {
    $wrapper->$property = $value;
  }

  $wrapper->save();
}


/**
 * DELETE an entity.
 */
function pde_cmd_entity_delete($type, $id) {
  pde_trace_funct();

  $wrapper = entity_metadata_wrapper($type, $id);
  $wrapper->delete();
}
