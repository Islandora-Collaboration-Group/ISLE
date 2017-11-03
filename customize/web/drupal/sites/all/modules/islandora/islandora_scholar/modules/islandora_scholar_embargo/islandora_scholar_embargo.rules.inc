<?php
/**
 * @file
 * Primary rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function islandora_scholar_embargo_rules_event_info() {
  // Datastreams and object events are duplicated because tokens not being
  // present all the time isn't supported well.
  return array(
    'islandora_scholar_datastream_embargo_lifted' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Datastream Embargo Lifted'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t('Object having its embargo lifted.'),
        ),
        'datastream' => array(
          'type' => 'islandora_datastream',
          'label' => t('Embargoed Datastream'),
          'description' => t('The object datastream the embargo has been lifted from.'),
        ),
      ),
    ),
    'islandora_scholar_datastream_embargo_expired' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Datastream Embargo Expired'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t("Object who's embargo has expired."),
        ),
        'datastream' => array(
          'type' => 'islandora_datastream',
          'label' => t('Embargoed Datastream'),
          'description' => t('The object datastream the embargo has expired for.'),
        ),
      ),
    ),
    'islandora_scholar_datastreams_embargo_expired' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Datastreams Embargo Expired'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t("Object who's embargo has expired."),
        ),
        'datastreams' => array(
          'type' => 'list<text>',
          'label' => t('Embargoed Datastreams'),
          'description' => t('The object datastreams the embargo has expired for.'),
        ),
      ),
    ),
    'islandora_scholar_datastream_embargo_expiring' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Datastream Embargo Expiry Approaching'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t("Object who's embargo will expire shortly."),
        ),
        'datastream' => array(
          'type' => 'islandora_datastream',
          'label' => t('Embargoed Datastream'),
          'description' => t('The object datastream the embargo is expiring for.'),
        ),
      ),
    ),
    'islandora_scholar_object_embargo_lifted' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Object Embargo Lifted'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t('Object having its embargo lifted.'),
        ),
      ),
    ),
    'islandora_scholar_object_embargo_expired' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Object Embargo Expired'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t("Object who's embargo has expired."),
        ),
      ),
    ),
    'islandora_scholar_object_embargo_expiring' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Object Embargo Expiry Approaching'),
      'variables' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t("Object who's embargo will expire shortly."),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function islandora_scholar_embargo_rules_action_info() {
  return array(
    'islandora_scholar_embargo_lift_embargo' => array(
      'group' => t('Islandora Scholar Embargo'),
      'label' => t('Lift an Embargo'),
      'parameter' => array(
        'object' => array(
          'type' => 'islandora_object',
          'label' => t('Embargoed Object'),
          'description' => t('The object to remove the embargo from.'),
        ),
        'datastream' => array(
          'type' => array('islandora_datastream', 'list<text>'),
          'label' => t('Embargoed Datastream'),
          'description' => t('The object datastream to remove the embargo from.'),
          'optional' => TRUE,
        ),
      ),
    ),
  );
}
