<?php
/**
* Implements hook_entity_property_info() for Purlieu module.
*
* @see entity_metadata_entity_property_info()
*/
function entity_metadata_purlieu_entity_property_info() {
  $info = array();

  // Add meta-data about the basic purlieu properties.
  $properties = &$info['purlieu']['properties'];

  $properties['id'] = array(
    'label' => t("Purlieu ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the purlieu."),
    );

  $properties['name'] = array(
    'label' => t("Name"),
    'type' => 'text',
    'description' => t("The user-friendly name of the purlieu."),
    );

  $properties['author'] = array(
    'label' => t("Author"),
    'type' => 'user',
    'description' => t("The author of the node."),
    'required' => TRUE,
    'schema field' => 'uid',
    );

  $properties['organization'] = array(
    'label' => t("Organization"),
    'type' => 'text',
    'description' => t("The organization name of the purlieu."),
    );

  $properties['employee_type'] = array(
    'label' => t("Employee Type"),
    'type' => 'text',
    'description' => t("The employee type of the purlieu."),
    );

  $properties['location'] = array(
    'label' => t("Location"),
    'type' => 'text',
    'description' => t("The office name of the purlieu."),
    );

  $properties['unloaded'] = array(
    'label' => t("Unloaded Rate"),
    'type' => 'decimal',
    'description' => t("The unloaded rate of the purlieu."),
    );

  $properties['rent'] = array(
    'label' => t("Rent Overhead"),
    'type' => 'decimal',
    'description' => t("The rent overhead of the purlieu."),
    );

  $properties['telecom'] = array(
    'label' => t("Telecom Overhead"),
    'type' => 'decimal',
    'description' => t("The telecom overhead of the purlieu."),
    );

  $properties['vhs'] = array(
    'label' => t("VHS"),
    'type' => 'decimal',
    'description' => t("The VHS rate of the purlieu."),
    );

  $properties['fringe'] = array(
    'label' => t("Fringe Rate"),
    'type' => 'decimal',
    'description' => t("The fringe rate of the purlieu."),
    );

  $properties['indirect'] = array(
    'label' => t("Indirect Rate"),
    'type' => 'decimal',
    'description' => t("The indirect costs rate of the purlieu."),
    );

  $properties['fee'] = array(
    'label' => t("Costs Fee Rate"),
    'type' => 'decimal',
    'description' => t("The Fee rate for Costs of the purlieu."),
    );

  $properties['actfee'] = array(
    'label' => t("Actuals Fee Rate"),
    'type' => 'decimal',
    'description' => t("The Fee rate for Actuals of the purlieu."),
    );

  return $info;
}
