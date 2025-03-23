Drupal.behaviors.zg_show_aides_equip = function (context) {

  // EQUIP pages

  var isoEquip = $('#all-equip').isotope({
    itemSelector: '.land',
    layoutMode: 'fitRows'
  });

  $('button#equip-all').bind('click', function() {
    isoEquip.isotope({ filter: ".land" });
    $(".news-buttons button").removeClass("active");
    $("#equip-all").addClass("active");
    Cookies.set('equip', 'all', { expires: 365 });
  });

  $('button#equip-ini').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-ini" });
    $(".news-buttons button").removeClass("active");
    $("#equip-ini").addClass("active");
    Cookies.set('equip', 'ini', { expires: 365 });
  });

  $('button#equip-end').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-end" });
    $(".news-buttons button").removeClass("active");
    $("#equip-end").addClass("active");
    Cookies.set('equip', 'end', { expires: 365 });
  });

  $('button#equip-elo').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-elo" });
    $(".news-buttons button").removeClass("active");
    $("#equip-elo").addClass("active");
    Cookies.set('equip', 'elo', { expires: 365 });
  });

  $('button#equip-move').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-move" });
    $(".news-buttons button").removeClass("active");
    $("#equip-move").addClass("active");
    Cookies.set('equip', 'move', { expires: 365 });
  });

  $('button#equip-other').bind('click', function() {
    isoEquip.isotope({ filter: "*:not(.equip-ini, .equip-end, .equip-elo, .equip-move)" });
    $(".news-buttons button").removeClass("active");
    $("#equip-other").addClass("active");
    Cookies.set('equip', 'other', { expires: 365 });
  });

  $('button#equip-buy').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-buy" });
    $(".news-buttons button").removeClass("active");
    $("#equip-buy").addClass("active");
    Cookies.set('equip', 'buy', { expires: 365 });
  });

  $('button#equip-sal').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-sal" });
    $(".news-buttons button").removeClass("active");
    $("#equip-sal").addClass("active");
    Cookies.set('equip', 'sal', { expires: 365 });
  });

  $('button#equip-upk').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-upk" });
    $(".news-buttons button").removeClass("active");
    $("#equip-upk").addClass("active");
    Cookies.set('equip', 'upk', { expires: 365 });
  });

  $('button#equip-eng').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-eng" });
    $(".news-buttons button").removeClass("active");
    $("#equip-eng").addClass("active");
    Cookies.set('equip', 'eng', { expires: 365 });
  });

  $('button#equip-act').bind('click', function() {
    isoEquip.isotope({ filter: ".equip-act" });
    $(".news-buttons button").removeClass("active");
    $("#equip-act").addClass("active");
    Cookies.set('equip', 'act', { expires: 365 });
  });

  var equipCookie = Cookies.get('equip');

  switch (equipCookie) {
    case "all":
      $('button#equip-all').click();
      break;

    case "ini":
      $('button#equip-ini').click();
      break;

    case "end":
      $('button#equip-end').click();
      break;

    case "elo":
      $('button#equip-elo').click();
      break;

    case "move":
      $('button#equip-move').click();
      break;

    case "other":
      $('button#equip-other').click();
      break;

    case "buy":
      $('button#equip-buy').click();
      break;

    case "sal":
      $('button#equip-sal').click();
      break;

    case "upk":
      $('button#equip-upk').click();
      break;

    case "eng":
      $('button#equip-eng').click();
      break;

    case "act":
      $('button#equip-act').click();
      break;
  }
};
