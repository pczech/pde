<?php

/**
 * @file
 * Commands to test Entity examples.
 */

// @ignore sniffer_commenting_inlinecomment_spacingafter:file
// @ignore sniffer_commenting_functioncomment_missingparamtype:file
// @ignore sniffer_commenting_functioncomment_missingreturntype:file


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

  $entity = entity_create($type, $data);
}
