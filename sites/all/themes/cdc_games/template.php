<?php

/**
 * Implements theme_placeholder().
 */
function cdc_games_placeholder($text) {
  return '<span class="nowrap highlight">' . check_plain($text) . '</span>';
}
