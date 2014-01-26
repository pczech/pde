<?php

/**
 * @file
 * Artwork types getters.
 */

/**
 * Get an array of all artwork types objects, keyed by type name.
 *
 * The function reads directly artwork type db table.
 */
function artwork_types_db() {
  $types = db_select('artwork_type', 'a')
  ->fields('a')
  ->execute()
  ->fetchAllAssoc('type');

  return $types;
}


/**
 * Get an array of all artwork types, keyed by the type name.
 *
 * Can't be called in hook_entity_info.
 */
function artwork_types_eapi($name = NULL) {
  pde_trace_funct();

  $names = isset($name) ? array($name) : FALSE;
  $types = entity_load_multiple_by_name('artwork_type', $names);
  return isset($name) ? reset($types) : $types;
}
