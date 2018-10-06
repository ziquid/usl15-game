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
  $vars['body_classes'] = str_replace('page-stlouis',
    'game-' . arg(0) . ' page-' . arg(1),
    $vars['body_classes']);
}
