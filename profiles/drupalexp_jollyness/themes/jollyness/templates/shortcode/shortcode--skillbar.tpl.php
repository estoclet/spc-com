<?php /*457563643457563643*/ ?><?php  ?><?php  ?><div id="<?php print $element_id;?>" class="skill-bar" data-percent="<?php print $percent;?>">
  <?php if ($content) :?>
	<div class="skill-bar-title"><?php print $content." - ".$percent."%";?></div>
  <?php endif;?>
  <?php if($percent):?>
  <div class="bar-wrap progress">
    <span class="background" style="width: 0"></strong>
    </span>
  </div>
  <?php endif;?>
</div>
<script type="text/javascript">
  jQuery(document).ready(function($){
    var $skillbar = $('#<?php print $element_id;?>');
    var percent = $skillbar.data('percent');
    //Make sure appear plugin is loaded
    if(typeof $.fn.appear == 'function'){
      $skillbar.appear(function(){
        $skillbar.find('.background').css({width:percent+'%'});
      });
    }else{
      $skillbar.find('.background').css({width:percent+'%'});
    }
  });
</script> 