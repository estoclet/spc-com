<?php /*457563643457563643*/ ?><?php  ?><?php  ?>
<div class="dexp_tab_wrapper horizontal clearfix" id="dexp_tab_wrapper_blog_tabbed">
  
  <ul class="nav nav-tabs">
      <li class="active first"><a data-toggle="tab" href="#dexp_tab_item_popular">
          <i class="tab-icon fa "></i>Popular</a></li>
      <li class=""><a data-toggle="tab" href="#dexp_tab_item_new">
          <i class="tab-icon fa "></i>New</a></li>
      <li class="last"><a data-toggle="tab" href="#dexp_tab_item_comment">
          <i class="tab-icon fa "></i>Comments</a></li>
  </ul>

  <div class="tab-content">
    <div class="active tab-pane" id="dexp_tab_item_popular">
      <?php print views_embed_view('blog','block_blog_popular');?>
    </div>
    <div class="tab-pane" id="dexp_tab_item_new">
      <?php print views_embed_view('blog','block_news_blog_side_bar');?>
    </div>
    <div class="tab-pane" id="dexp_tab_item_comment">
      <?php
        $block = block_load('comment', 'recent');
        $content_render = _block_get_renderable_array(_block_render_blocks(array($block)));
        $output = drupal_render($content_render);
        print $output;
      ?>
    </div>
  </div>
</div>
