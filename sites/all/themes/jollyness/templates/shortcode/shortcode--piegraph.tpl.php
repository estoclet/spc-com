<?php /*457563643457563643*/ ?><?php  ?><?php  ?><div class="skills_boxes">
  <span class="chart" data-bar-color="#21c2f8" data-percent="<?php print $percent; ?>"><span class="percent"></span></span>
    <div class="title"><h3><?php print $title; ?></h3></div>
    <p><?php print $content; ?></p>
</div><!-- end skills_boxes -->
<?php 
  global $base_url;
  $theme_path = drupal_get_path('theme', 'jollyness'); 
?>
<script src="<?php print $base_url.'/'.$theme_path.'/templates/shortcode/js/jquery.easypiechart.min.js';?>"></script>
<script type="text/javascript">
    jQuery(document).ready(function($) {
        $('.chart').easyPieChart({
            easing: 'easeOutBounce',
            size: 200,
            animate: 2000,
            lineWidth: 2,
            lineCap: 'square',
            lineWidth : 3,
            barColor: false,
            trackColor: '#F5F5F5',
            scaleColor: false,
            onStep: function(a,b,c) {
              $(this.el).find('span.percent').text(Math.round(a) + '%');               
            }
        });
    });
</script>