var isoLand = $('#all-land').isotope({
  itemSelector: '.land',
  layoutMode: 'fitRows'
});

$('#news-all').bind('click', function() {
  isoLand.isotope({ filter: ".land" });
  $(".news-buttons button").removeClass("active");
  $("#news-all").addClass("active");
});

$('#news-jobs').bind('click', function() {
  isoLand.isotope({ filter: ".land-job" });
  $(".news-buttons button").removeClass("active");
  $("#news-jobs").addClass("active");
});

$('#news-investments').bind('click', function() {
  isoLand.isotope({ filter: ".land-investment" });
  $(".news-buttons button").removeClass("active");
  $('#news-investments').addClass("active");
});
