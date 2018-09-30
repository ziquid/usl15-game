Drupal.behaviors.stlouis_quest_groups = function (context) {
  var swiper = new Swiper('.swiper-container', {
    hashNavigation: {
      watchState: true
    },
    effect: 'coverflow', // 'cube', // 'coverflow', // 'flip',
    pagination: {
      el: '.swiper-pagination',
      dynamicBullets: true
    },
    navigation: {
      nextEl: '.swiper-button-next',
      prevEl: '.swiper-button-prev'
    }
  });
};
