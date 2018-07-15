<?php /*457563643457563643*/ ?><?php  ?><?php  ?>
<div class="text-center clearfix">
  <span class="blog_button">Latest News</span>
</div>

<ol id="timeline">
  <?php foreach ($rows as $id => $row):?>
    <li class="timeline-item col-xs-12 col-sm-6 col-md-6 col-lg-6">
      <?php print $row;?>
    </li>
  <?php endforeach;?>
</ol>