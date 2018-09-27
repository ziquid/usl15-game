Drupal.behaviors.stlouis = function (context) {

  if (Drupal.settings.stlouis.party_icon) {
    $('body').addClass('party-' + Drupal.settings.stlouis.party_icon);
  }

  if (Drupal.settings.stlouis.meta == 'beta' ||
    Drupal.settings.stlouis.meta == 'admin') {
    var level = parseInt(Drupal.settings.stlouis.level);
    var red = 180;
    var green = Math.floor(200 - (level / 2));
    var blue = Math.floor(180 - (level / 4));
    $('body').addClass('beta');
    $('body.beta').css('background-color', 'rgb(' + red + ', ' + green + ', ' + blue + ')');
  }

};
