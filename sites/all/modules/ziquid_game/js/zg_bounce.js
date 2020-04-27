Drupal.behaviors.zg_bounce = function (context) {

  function clickToContinue() {
    window.location.href = $('.button-continue').attr('href');
  }
  setTimeout(clickToContinue, 2400);
};
