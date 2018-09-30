Drupal.behaviors.stlouis = function (context) {

  if (Drupal.settings.stlouis.party_icon) {
    $('body').addClass('party-' + Drupal.settings.stlouis.party_icon);
  }

  if (Drupal.settings.stlouis.enabled_alpha) {
    var level = parseInt(Drupal.settings.stlouis.level);
    // level = 40;
    var red = Math.floor(180 - (level / 5));
    var green = Math.floor(200 - (level / 1.2));
    var blue = Math.floor(180 - (level / 1.5));
    $('body').addClass('alpha');
    $('body.alpha').css('background-color', 'rgb(' + red + ', ' + green + ', ' + blue + ')');
  }

};
