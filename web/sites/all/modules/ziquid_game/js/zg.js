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

    if (Drupal.settings.zg.snowstorm) {
      snowStorm.autoStart = true;
      snowStorm.excludeMobile = false;
      snowStorm.className = "snowMobileFoo";
      snowStorm.animationInterval = 100;
      snowStorm.flakesMax = 60;
      snowStorm.flakesMaxActive = 40;
      snowStorm.snowColor = '#777777';
      snowStorm.snowCharacter = '❄';
      snowStorm.flakeWidth = 14;
      snowStorm.flakeHeight = 14;
      snowStorm.useMeltEffect = true;
      // snowStorm.useTwinkleEffect = true;
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
  $('#menu-button-1').click(function () {
    var xPos = self.pageXOffset;
    var yPos = self.pageYOffset;
    if (xPos + yPos) {
      window.scrollTo({top: 0, left: 0, behavior: 'smooth'});
    }
    else {
      window.location.href = $(this).attr('data-home-link');
    }
  });

  // Menu toggle.
  $('#menu-toggle').click(function () {
    $('.menu-button').toggle(200, 'swing');
  });

  // Stats toggle.
  $('#stats-toggle').click(function () {
    $('#stats').toggle(200, 'swing');
    $('.stats-button').toggle(200, 'swing');
  });

  // People toggle.
  $('#people-toggle').click(function () {
    $('.people-button').toggle(200, 'swing');
  });

  // Tap to close.
  $('.tap-to-close').click(function () {
    console.log('tap to close!');
    $(this).hide(200, 'swing');
  });

  // Don't tap to close.
  $('.landscape-overlay').click(function (e) {
    console.log('landscape overlay!');
    e.stopPropagation();
  });

  jQuery(".fit-box").each(function () {
    var innerWidth = $(this).innerWidth();
    var scrollWidth = $(this)[0].scrollWidth;
    if (scrollWidth > innerWidth) {
      var scale = innerWidth / scrollWidth;
      $(this).css({'-webkit-transform': 'scale(' + scale + ')'});
    }
  });

  /* Video functions. */
  // Find videos.
  const zg_videos = context.querySelectorAll('video');
  const $zg_jqVideos = $(zg_videos);
  const zg_numVideos = zg_videos.length - 1;
  const zg_listenButton = context.querySelector('button#listen');
  console.log(zg_videos);

  // Add event listeners for end of video.
  function zg_videoEnded(event) {
    videoNum = $(this).data('video-num');
    console.log("video " + videoNum + " of " + zg_numVideos + " ended!");
    if (videoNum == zg_numVideos) {
      zg_listenButton.disabled = false;
      zg_showVideoNum(0);
    }
    else {
      zg_showVideoNum(videoNum + 1);
    }
  }

  zg_videos.forEach(function (currentValue, currentIndex, listObj) {
    currentValue.addEventListener('ended', zg_videoEnded)
  });

  // Play a video.
  async function zg_playVideo(video) {
    try {
      videoNum = $(video).data('video-num');
      console.log("video " + videoNum + " of " + zg_numVideos + " is starting to play!");
      await video.play();
    } catch (err) {
      console.log("video could not be played.");
      console.log(err);
      console.log(video);
      zg_listenButton.disabled = false;
    }
  }

  // Show a specific video and play it.
  function zg_showVideoNum(num) {
    $zg_jqVideos.hide();
    $zg_jqVideos.eq(num).show();
    $zg_jqVideos.eq(num).data('video-num', num);
    $zg_jqVideos.eq(num).attr('data-video-num', num);
    zg_playVideo(zg_videos[num]);
  }

  // Show or disable Listen button.
  if (zg_numVideos > 1) {
    zg_listenButton.addEventListener('click', function () {
      this.disabled = true;
      zg_showVideoNum(1);
    });
  }
  else {
    if (zg_listenButton) {
      zg_listenButton.disabled = true;
    }
  }
};
