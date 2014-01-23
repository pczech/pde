<?php

/**
 * @file
 * Artwork entity controller class.
 */

// @ignore sniffer_commenting_inlinecomment_spacingafter:file


/**
 * Controller class for artworks.
 *
 * Extends the DrupalDefaultEntityController class.
 */
class ArtworkController extends DrupalDefaultEntityController {
  /**
   * Create empty artwork entity object.
   */
  public function create($type = '') {
    return (object) array(
      'aid' => '',
      'type' => $type,
      'title' => '',
    );
  }

  /**
   * Save the artwork entity record.
   */
  public function save($artwork) {
    det_trace_funct();
    // dpm($artwork);

    $transaction = db_transaction();

    try {
      // Determine if this is a new artwork.
      $is_new_artwork = empty($artwork->aid);

      if ($is_new_artwork) {
        $artwork->created = REQUEST_TIME;
      }
      $artwork->changed = REQUEST_TIME;
      $artwork->timestamp = REQUEST_TIME;

      // Field Attach API
      // Give modules the opportunity to prepare field data for saving.
      field_attach_presave('artwork', $artwork);

      // If this is a new artwork...
      if ($is_new_artwork) {
        // Save the new artwork record according to the schema:
        // aid - null (a new serial value will be set automatically)
        // vid - null ($artwork->vid doesn't exist as no revision data is ready)
        // type - $artwork->type ('painting' for example)
        // title - $artwork->title
        // created - $artwork->created
        // changed - $artwork->changed
        // The values not provided in $artwork (vid) will be populated in
        // $artwork and in the database with the default values from the
        // schema, as well as a single serial (aid) field.
        // dpm($artwork);
        drupal_write_record('artwork', $artwork);
        // dpm($artwork);

        // Save a new artwork_revision record according to the schema:
        // aid - $artwork->aid
        // vid - null (new serial value will be set automatically)
        // title - $artwork->title
        // timestamp - $artwork->timestamp
        // dpm($artwork);
        drupal_write_record('artwork_revision', $artwork);
        // dpm($artwork);

        // Update revision reference in the artwork record.
        $this->dbUpdateArtworkVid($artwork);

        // Save attached fields in DB.
        field_attach_insert('artwork', $artwork);

        // Invoke hook_entity_insert in all enabled modules that implement it.
        // Include arguments to pass to the hook - $entity, $type.
        module_invoke_all('entity_insert', $artwork, 'artwork');
      }
      else {
        // Update existing record, indicated by 'aid' key.
        // aid - given record
        // vid - $artwork->vid (will be updated)
        // type - $artwork->type ('painting' for example)
        // title - $artwork->title
        // created - null
        // changed - $artwork->changed
        // dpm($artwork);
        drupal_write_record('artwork', $artwork, 'aid');

        if (!empty($artwork->revision)) {
          // Save the new revision. vid is the primary key, so unset it and
          // then a new serial value will be set automatically to the new
          // record.
          unset($artwork->vid);
          // Save a new artwork_revision record according to the schema:
          // aid - $artwork->aid
          // vid - null (new serial value will be set automatically)
          // title - $artwork->title
          // timestamp - $artwork->timestamp
          // dpm($artwork);
          drupal_write_record('artwork_revision', $artwork);
          // dpm($artwork);
        }
        else {
          // Update existing record:
          // aid - $artwork->aid
          // vid - $artwork->vid
          // title - $artwork->title
          // timestamp - $artwork->timestamp
          // dpm($artwork);
          drupal_write_record('artwork_revision', $artwork, 'vid');
          // dpm($artwork);
        }

        // Update revision reference in the artwork record.
        $this->dbUpdateArtworkVid($artwork);

        // Update attached fields in DB.
        field_attach_update('artwork', $artwork);

        // Invoke hook_entity_update in all enabled modules that implement it.
        // Include arguments to pass to the hook - $entity, $type.
        module_invoke_all('entity_update', $artwork, 'artwork');
      }

      // Ignore slave server temporarily to give time for the saved order to
      // be propagated to the slave.
      db_ignore_slave();

      return $artwork;
    }
    catch (Exception $e) {
      dpm($e);
      $transaction->rollback();
      watchdog_exception('artwork', $e);
      return FALSE;
    }
  }

  /**
   * Assign the new artwork revision to artwork record in DB.
   *
   * Revision ID (vid) must be present before handling attached fields as
   * thay as attached to given revision.
   */
  protected function dbUpdateArtworkVid($artwork) {
    db_update('artwork')
      ->fields(array('vid' => $artwork->vid))
      ->condition('aid', $artwork->aid)
      ->execute();
  }

  /**
   * Delete the artwork entity record.
   */
  public function delete($aid) {
    det_trace_funct();

    if (empty($aid)) {
      // Nothing to delete.
      return TRUE;
    }

    // Handle multiple artworks.
    $aids = array($aid);
    $transaction = db_transaction();

    try {
      db_delete('artwork')
        ->condition('aid', $aids, 'IN')
        ->execute();

      db_delete('artwork_revision')
        ->condition('aid', $aids, 'IN')
        ->execute();

      $artworks = $this->load($aids);
      foreach ($artworks as $artwork) {
        field_attach_delete('artwork', $artwork);
      }

      // Invoke hook_entity_delete in all enabled modules that implement it.
      // Include arguments to pass to the hook - $entity, $type.
      module_invoke_all('entity_delete', $artwork, 'artwork');
      db_ignore_slave();

      // Clear the page and block and artwork caches.
      cache_clear_all();
      $this->resetCache();
      return TRUE;
    }
    catch (Exception $e) {
      dpm($e);
      $transaction->rollback();
      watchdog_exception('artwork', $e);
      return FALSE;
    }
  }
}
