<?php
/**
 * @file
 * Class implementations for Purlieu module.
 */

/**
 * Our custom controller for the admin ui.
 *
 * The EntityDefaultUIController can be found in
 * sites/all/modules/contrib/entity/includes/entity.ui.inc
 */
class PurlieuUIController extends EntityDefaultUIController {

}

/**
 * Our custom entity class.
 *
 * The Entity class we're overriding is in
 * sites/all/modules/contrib/entity/includes/entity.inc
 */
class Purlieu extends Entity {
  /**
   * Override this in order to implement a custom default URI.
   */
  protected function defaultUri() {
    return array('path' => 'purlieu/' . drupal_html_class($this->identifier()));
  }

  /**
   * Returns the path used to view the entity.
   */
  public function path() {
    if ($this->id) {
      return 'purlieu/' . $this->id;
    }
  }

  // Method for calculating the adjust rate of some amount.
  // $rate is the billing rate.
  // $hours is the number of hours worked.
  // $usefee determines which Fee value is to be used.
  //   (True - Costs, False - Actuals)
  public function calculate($rate, $hours, $usefee = TRUE) {
    // Get out quick on zero.
    if ($hours == 0) {
      return 0;
    }

    // Remember these are all two-digit decimals.
    $vhs = $this->vhs / 100;
    $fringe = $this->fringe / 100;
    $rent = $this->rent;
    $telecom = $this->telecom;
    $indirect = $this->indirect / 100;
    // Which fee to use?
    $fee = ($usefee ? $this->fee / 100 : $this->actfee / 100);

    switch ($this->formula) {
      case 'standard':
        $ret = ($rate * (1 + $vhs) * (1 + $fringe) + $rent + $telecom)
          * (1 + $indirect) * (1 + $fee);
        break;

      case 'westat':
        $ret = ($rate * (1 + $fringe + $vhs)) * (1 + $indirect) * (1 + $fee);
        break;

      default:
        drupal_set_message(t('The @formula formula type is unknown.', array('@formula' => $this->formula)));
        $ret = 1;
    }

    // They don't want full accuracy.
//    $ret = round($ret, 2);

    // Calculate the resulting cost.
    $ret = round($ret * $hours, 2);
    return $ret;
  }
}

/**
 * Our custom controller for the purlieu type.
 *
 * We're choosing to extend the controller provided by the entity module for
 * full CRUD support for entity families.
 *
 * The EntityAPIController is found in
 * sites/all/modules/contrib/entity/includes/entity.controller.inc
 */
class PurlieuController extends EntityAPIController {
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    $build['created'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="entity-property entity-property-created">'
        . '<div class="entity-property-label">' . t('Created') . '</div>'
        . '<div>' . format_date($entity->created, 'custom', 'Y M j H:i A') . '</div>'
        . '</div>',
        );
    $build['changed'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="entity-property entity-property-changed">'
        . '<div class="entity-property-label">' . t('Updated') . '</div>'
        . '<div>' . format_date($entity->changed, 'custom', 'Y M j H:i A') . '</div>'
        . '</div>',
        );

    return $build;
  }

  /**
   * Override save method.
   * Populate created and updated dates automatically.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    global $user;
    if (isset($entity->is_new)) {
      $entity->created = REQUEST_TIME;
      $entity->uid = $user->uid;
    }

    $entity->changed = REQUEST_TIME;

    return parent::save($entity, $transaction);
  }
}

class PurlieuMetadataController extends EntityDefaultMetadataController {
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $info[$this->type]['properties']['id'] = array(
      'label' => t('Purlieu ID'),
      'type' => 'integer',
      'description' => t('Purlieu ID'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'id',
      );

    $info[$this->type]['properties']['vid'] = array(
      'label' => t('Revision ID'),
      'type' => 'integer',
      'description' => t('Purlieu Revision ID'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'vid',
      );

    $info[$this->type]['properties']['name'] = array(
      'label' => t("Name"),
      'type' => 'text',
      'description' => t("The user-friendly name of the purlieu."),
      'schema field' => 'name',
      );

    $info[$this->type]['properties']['uid'] = array(
      'label' => t("Author"),
      'type' => 'user',
      'description' => t("The author of the node."),
      'required' => TRUE,
      'schema field' => 'uid',
      );

    $info[$this->type]['properties']['created'] = array(
      'label' => t("Created"),
      'type' => 'datestamp',
      'description' => t("The date this was created."),
      'required' => TRUE,
      'schema field' => 'created',
      );

    $info[$this->type]['properties']['changed'] = array(
      'label' => t("Updated"),
      'type' => 'date',
      'description' => t("The date this was updated."),
      'required' => TRUE,
      'schema field' => 'changed',
      );

    $info[$this->type]['properties']['organization'] = array(
      'label' => t("Organization"),
      'type' => 'text',
      'description' => t("The organization name of the purlieu."),
      'schema field' => 'organization',
      );

     $info[$this->type]['properties']['employee_type'] = array(
      'label' => t("Employee Type"),
      'type' => 'text',
      'description' => t("The employee type of the purlieu."),
      'schema field' => 'employee_type',
      );

    $info[$this->type]['properties']['location'] = array(
      'label' => t("Location"),
      'type' => 'text',
      'description' => t("The office name of the purlieu."),
      'schema field' => 'location',
      );
    $info[$this->type]['properties']['rent'] = array(
      'label' => t("Rent Overhead"),
      'type' => 'decimal',
      'description' => t("The rent overhead of the purlieu."),
      'schema field' => 'rent',
      );

    $info[$this->type]['properties']['telecom'] = array(
      'label' => t("Telecom Overhead"),
      'type' => 'decimal',
      'description' => t("The telecom overhead of the purlieu."),
      'schema field' => 'telecom',
      );

    $info[$this->type]['properties']['vhs'] = array(
      'label' => t("VHS"),
      'type' => 'decimal',
      'description' => t("The VHS rate of the purlieu."),
      'schema field' => 'vhs',
      );

    $info[$this->type]['properties']['fringe'] = array(
      'label' => t("Fringe Rate"),
      'type' => 'decimal',
      'description' => t("The fringe rate of the purlieu."),
      'schema field' => 'fringe',
      );

    $info[$this->type]['properties']['indirect'] = array(
      'label' => t("Indirect Rate"),
      'type' => 'decimal',
      'description' => t("The indirect costs rate of the purlieu."),
      'schema field' => 'indirect',
      );

    $info[$this->type]['properties']['fee'] = array(
      'label' => t("Fee for Costs"),
      'type' => 'decimal',
      'description' => t("The Fee rate for Costs of the purlieu."),
      'schema field' => 'fee',
      );

    $info[$this->type]['properties']['actfee'] = array(
      'label' => t("Fee for Actuals"),
      'type' => 'decimal',
      'description' => t("The Fee rate for Actuals of the purlieu."),
      'schema field' => 'actfee',
      );

    $info[$this->type]['properties']['formula'] = array(
      'label' => t("Formula"),
      'type' => 'text',
      'description' => t("The formula to use when calculating with this purlieu."),
      'schema field' => 'formula',
      );

    return $info;
  }
}
