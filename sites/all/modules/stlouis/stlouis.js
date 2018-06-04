Drupal.behaviors.stlouis = function (context) {

  if (Drupal.settings.stlouis.party_icon) {
    $('body').addClass('party-' + Drupal.settings.stlouis.party_icon);
  }

};
