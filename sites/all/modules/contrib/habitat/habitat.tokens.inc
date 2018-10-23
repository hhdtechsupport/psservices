<?php

/**
 * @file
 * Provide a Habitat token "[site:habitat]".
 */

/**
 * Implements hook_token_info().
 */
function habitat_token_info() {
  // Add our token to the "site" token list.
  $info['tokens']['site']['habitat'] = array(
    'name' => t("Current habitat"),
    'description' => t('The name of the current habitat defined in settings.php.'),
    );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function habitat_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'site') {
    foreach ($tokens as $name => $original) {
      switch ($name){
        case 'habitat':
          $variable = variable_get('habitat_variable', 'fetcher_environment');
          $replacements[$original] = variable_get($variable, '');
          break;
      }
    }
  }
  return $replacements;
}
