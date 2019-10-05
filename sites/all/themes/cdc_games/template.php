<?php

/**
 * Implements theme_placeholder().
 */
function cdc_games_placeholder($text) {
  return '<span class="nowrap highlight">' . check_plain($text) . '</span>';
}

/**
 * Implements template_preprocess_page.
 */
function cdc_games_preprocess_page(&$vars) {
  $game = $arg0 = drupal_html_class(arg(0));
  $page = $arg1 = drupal_html_class(arg(1));

  // Pretend usl_esa is stlouis for now.
  if ($game == 'usl-esa') {
    $game = 'stlouis';
  }

  $vars['body_classes'] = str_replace('page-' . $arg0,
    'game-' . $game . ' page-' . $page,
    $vars['body_classes']);
}

// Backported from Drupal 7.
if (!function_exists('drupal_html_class')) {
  function drupal_html_class($class) {

    // The output of this function will never change, so this uses a normal
    // static instead of drupal_static().
    static $classes = [];
    if (!isset($classes[$class])) {
      $classes[$class] = drupal_clean_css_identifier(drupal_strtolower($class));
    }
    return $classes[$class];
  }
}
