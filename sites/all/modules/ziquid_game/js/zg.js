Drupal.behaviors.zg = function (context) {

  if (Drupal.settings.zg) {
    if (Drupal.settings.zg.party_icon) {
      $('body').addClass('party-' + Drupal.settings.zg.party_icon);
    }

    if (Drupal.settings.zg.enabled_alpha) {
      var level = parseInt(Drupal.settings.zg.level);
      // level = 1;
      var red = Math.max(level - 100, 0);
      var green = Math.max(Math.floor(100 - level), 0);
      var blue = level;
      if (blue > 100) {
        blue = 200 - blue;
      }
      red = Math.floor(red * 0.6);
      green = Math.floor(green * 0.6);
      blue = Math.floor(blue * 0.6);
      $('body').addClass('alpha');
      $('body.alpha').css('background-color', 'rgb(' + red + ', ' + green + ', ' + blue + ')');
      $('.background-color').css('background-color', 'rgb(' + red + ', ' + green + ', ' + blue + ')');
    }
  }

  jQuery(".fit-box").each(function() {
    var innerWidth = $(this).innerWidth();
    var scrollWidth = $(this)[0].scrollWidth;
    if (scrollWidth > innerWidth) {
      var scale = innerWidth / scrollWidth;
      $(this).css({'-webkit-transform': 'scale(' + scale + ')'});
    }
  });

};
