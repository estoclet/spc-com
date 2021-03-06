(function($){
  Drupal.behaviors.dexp_portfolio = {
    attach: function(context,settings) {
      if(context.location == null){
        $.each(settings.dexp_portfolio, function(filter, view){
          var $grid = $('#'+view,context),$filter = $('#'+filter,context);
          var $newitems = $('#'+view,context).find('.node').not('.shuffle-item');
          $grid.shuffle({
            itemSelector: '.node',
            speed: 500
          });
          $grid.shuffle('appended',$newitems);
          $grid.shuffle('shuffle', 'all');
        });
      }
      $.each(settings.dexp_portfolio, function(filter, view){
        var $grid = $('#'+view,context),$filter = $('#'+filter,context);
        $grid.shuffle({
          itemSelector: '.node',
          speed: 500
        });
        $filter.find('a').click(function(){
          var $this = $(this), filter = $this.data('filter');
          if(filter == '*'){
            $grid.shuffle('shuffle', 'all');
          }else{
            $grid.shuffle('shuffle', function($el, shuffle) {
              // Only search elements in the current group
              if (shuffle.group !== 'all' && $.inArray(shuffle.group, $el.data('groups')) === -1) {
                return false;
              }
              return $el.hasClass(filter);
            });
          }
          $(this).parents('.dexp-portfolio-filter').find('a').removeClass('active');
          $(this).addClass('active');
          return false;
        });
        $grid.shuffle('shuffle', 'all');
        $(window).load(function(){
          $grid.shuffle('shuffle', 'all');
        });
      });
    }
  }
})(jQuery);
/*
jQuery(document).ready(function($){
  var $grid = $('#page-dexp-portfolio');
  $grid.shuffle({
    itemSelector: '.node',
    speed: 500
  });
  $('ul.dexp-portfolio-filter li a').click(function(){
    var $this = $(this), filter = $this.data('filter');
    if(filter == '*'){
      $grid.shuffle('shuffle', 'all');
    }else{
      $grid.shuffle('shuffle', function($el, shuffle) {
        // Only search elements in the current group
        if (shuffle.group !== 'all' && $.inArray(shuffle.group, $el.data('groups')) === -1) {
          return false;
        }
        return $el.hasClass(filter);
      });
    }
    $(this).parents('.dexp-portfolio-filter').find('a').removeClass('active');
    $(this).addClass('active');
    return false;
  });
  $(window).load(function(){
    $grid.shuffle('shuffle', 'all');
  });
})
*/