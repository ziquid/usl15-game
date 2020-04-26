Drupal.behaviors.zg = function (context) {

  function zg_header_check_message_count(url) {
    messageCount = 0;
    $.ajax({
      url: url,
      success: function (data, status, xhr) {
        messageCount = data;
      },
      complete: function (xhr, status) {
        if (messageCount > 9) {
          $("#msg-badge").text("9+");
        }
        else if (messageCount > 0) {
          $("#msg-badge").text(messageCount);
        }
        else {
          $("#msg-badge").text("");
        }
      }
    });
  }

  if (Drupal.settings.zg) {
    if (Drupal.settings.zg.party_icon) {
      $('body').addClass('party-' + Drupal.settings.zg.party_icon);
    }

    if (Drupal.settings.zg.enabled_alpha) {
      $('body').addClass('alpha');
    }

    if (Drupal.settings.zg.check_message_count_url) {
      console.log(Drupal.settings.zg.check_message_count_url);
      setInterval(zg_header_check_message_count, 2020, Drupal.settings.zg.check_message_count_url);
      setTimeout(zg_header_check_message_count, 20, Drupal.settings.zg.check_message_count_url);
    }

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
    // $('body.game-stlouis').css('background-color', 'rgb(' + red + ', ' + green + ', ' + blue + ')');
    // $('.background-color').css('background-color', 'rgb(' + red + ', ' + green + ', ' + blue + ')');
  }


  // Menu button.
  $('#menu-button').click(function() {
    let xPos = self.pageXOffset;
    let yPos = self.pageYOffset;
    if (xPos + yPos) {
      window.scrollTo({top: 0, left: 0, behavior: 'smooth'});
    }
    else {
      window.location.href = $(this).attr('data-home-link');
    }
  });

  // Stats toggle.
  $('#stats-toggle').click(function() {
    $('#stats').toggle(200, 'swing');
  });

  jQuery(".fit-box").each(function () {
    var innerWidth = $(this).innerWidth();
    var scrollWidth = $(this)[0].scrollWidth;
    if (scrollWidth > innerWidth) {
      var scale = innerWidth / scrollWidth;
      $(this).css({'-webkit-transform': 'scale(' + scale + ')'});
    }
  });

};
