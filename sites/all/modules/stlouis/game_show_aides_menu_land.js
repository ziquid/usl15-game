Drupal.behaviors.game_show_aides_land = function (context) {

  var isoLand = $('#all-land').isotope({
    itemSelector: '.land',
    layoutMode: 'fitRows'
  });

  $('button#land-all').bind('click', function() {
    isoLand.isotope({ filter: ".land" });
    $(".news-buttons button").removeClass("active");
    $("#land-all").addClass("active");
    Cookies.set('land', 'all', { expires: 365 });
  });

  $('button#land-jobs').bind('click', function() {
    isoLand.isotope({ filter: ".land-job" });
    $(".news-buttons button").removeClass("active");
    $("#land-jobs").addClass("active");
    Cookies.set('land', 'jobs', { expires: 365 });
  });

  $('button#land-investments').bind('click', function() {
    isoLand.isotope({ filter: ".land-investment" });
    $(".news-buttons button").removeClass("active");
    $('#land-investments').addClass("active");
    Cookies.set('land', 'investments', { expires: 365 });
  });

  var landCookie = Cookies.get('land');

  switch (landCookie) {
    case "all":
      $('button#land-all').click();
      break;

    case "jobs":
      $('button#land-jobs').click();
      break;

    case "investments":
      $('button#land-investments').click();
      break;
  }
};
