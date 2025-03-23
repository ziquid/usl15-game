Drupal.behaviors.game_show_aides_staff = function (context) {

  // STAFF pages

  var isoStaff = $('#all-staff').isotope({
    itemSelector: '.land',
    layoutMode: 'fitRows'
  });

  $('button#staff-all').bind('click', function() {
    isoStaff.isotope({ filter: ".land" });
    $(".news-buttons button").removeClass("active");
    $("#staff-all").addClass("active");
    Cookies.set('staff', 'all', { expires: 365 });
  });

  $('button#staff-ini').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-ini" });
    $(".news-buttons button").removeClass("active");
    $("#staff-ini").addClass("active");
    Cookies.set('staff', 'ini', { expires: 365 });
  });

  $('button#staff-end').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-end" });
    $(".news-buttons button").removeClass("active");
    $("#staff-end").addClass("active");
    Cookies.set('staff', 'end', { expires: 365 });
  });

  $('button#staff-elo').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-elo" });
    $(".news-buttons button").removeClass("active");
    $("#staff-elo").addClass("active");
    Cookies.set('staff', 'elo', { expires: 365 });
  });

  $('button#staff-move').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-move" });
    $(".news-buttons button").removeClass("active");
    $("#staff-move").addClass("active");
    Cookies.set('staff', 'move', { expires: 365 });
  });

  $('button#staff-other').bind('click', function() {
    isoStaff.isotope({ filter: "*:not(.staff-ini, .staff-end, .staff-elo, .staff-move)" });
    $(".news-buttons button").removeClass("active");
    $("#staff-other").addClass("active");
    Cookies.set('staff', 'other', { expires: 365 });
  });

  $('button#staff-staff').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-staff" });
    $(".news-buttons button").removeClass("active");
    $("#staff-staff").addClass("active");
    Cookies.set('staff', 'staff', { expires: 365 });
  });

  $('button#staff-agent').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-agent" });
    $(".news-buttons button").removeClass("active");
    $("#staff-agent").addClass("active");
    Cookies.set('staff', 'agent', { expires: 365 });
  });

  $('button#staff-buy').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-buy" });
    $(".news-buttons button").removeClass("active");
    $("#staff-buy").addClass("active");
    Cookies.set('staff', 'buy', { expires: 365 });
  });

  $('button#staff-sal').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-sal" });
    $(".news-buttons button").removeClass("active");
    $("#staff-sal").addClass("active");
    Cookies.set('staff', 'sal', { expires: 365 });
  });

  $('button#staff-upk').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-upk" });
    $(".news-buttons button").removeClass("active");
    $("#staff-upk").addClass("active");
    Cookies.set('staff', 'upk', { expires: 365 });
  });

  $('button#staff-eng').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-eng" });
    $(".news-buttons button").removeClass("active");
    $("#staff-eng").addClass("active");
    Cookies.set('staff', 'eng', { expires: 365 });
  });

  $('button#staff-act').bind('click', function() {
    isoStaff.isotope({ filter: ".staff-act" });
    $(".news-buttons button").removeClass("active");
    $("#staff-act").addClass("active");
    Cookies.set('staff', 'act', { expires: 365 });
  });

  var staffCookie = Cookies.get('staff');

  switch (staffCookie) {
    case "all":
      $('button#staff-all').click();
      break;

    case "ini":
      $('button#staff-ini').click();
      break;

    case "end":
      $('button#staff-end').click();
      break;

    case "elo":
      $('button#staff-elo').click();
      break;

    case "move":
      $('button#staff-move').click();
      break;

    case "other":
      $('button#staff-other').click();
      break;

    case "staff":
      $('button#staff-staff').click();
      break;

    case "agent":
      $('button#staff-agent').click();
      break;

    case "buy":
      $('button#staff-buy').click();
      break;

    case "sal":
      $('button#staff-sal').click();
      break;

    case "upk":
      $('button#staff-upk').click();
      break;

    case "eng":
      $('button#staff-eng').click();
      break;

    case "act":
      $('button#staff-act').click();
      break;
  }
};
