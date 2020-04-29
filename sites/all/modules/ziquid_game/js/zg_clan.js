Drupal.behaviors.zg_clan = function (context) {
  var swiper = new Swiper('.swiper-container', {
    loop: true,
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
