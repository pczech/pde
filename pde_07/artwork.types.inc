<?php

/**
 * @file
 * Artwork entity definition function.
 */

// @ignore sniffer_commenting_inlinecomment_spacingafter:file


/**
 * Define bundles of artwork entity.
 */
function _artwork_types() {
  // det_trace_funct();

  $types = &drupal_static(__FUNCTION__);

  if (empty($types)) {
    $types['painting'] = (object) array(
      'type' => 'painting',
      'name' => t('Painting'),
      'description' => t('A picture made with paint.'),
    );

    $types['sculpture'] = (object) array(
      'type' => 'sculpture',
      'name' => t('Sculpture'),
      'description' => t('A carving made out of stone or wood.'),
    );
  }

  // dpm($types);
  return $types;
}
