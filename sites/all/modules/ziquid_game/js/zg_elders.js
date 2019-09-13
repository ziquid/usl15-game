Drupal.behaviors.zg_elders = function (context) {

  /**
   * Needs JQuery 1.4!
   *
  $.fn.queueAddClass = function(className) {
    this.queue('fx', function(next) {
      $(this).addClass(className);
      next();
    });
    return this;
  };

  $.fn.queueRemoveClass = function(className) {
    this.queue('fx', function(next) {
      $(this).removeClass(className);
      next();
    });
    return this;
  };

  $(".slide-in-content").delay(100).queueAddClass("slide-in-content-now");
  return;
   */

  var delay = 25;
  var delayDelta = 25;

  function startSlide($obj) {
    $obj.addClass("slide-in-content-now");
  }

  $(".slide-in-content").each(function (index) {
    setTimeout(startSlide, index * delayDelta + delay, $(this));
  });
};
