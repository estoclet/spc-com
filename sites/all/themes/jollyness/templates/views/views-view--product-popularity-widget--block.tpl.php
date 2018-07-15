<?php /*457563643457563643*/ ?><?php  ?><?php  ?>
<div class="dexp_tab_wrapper horizontal clearfix" id="dexp_tab_wrapper_widget_tabbed">
  
  <ul class="nav nav-tabs">
      <li class="active first"><a data-toggle="tab" href="#dexp_tab_item_best_seller">
          <i class="tab-icon fa "></i>Best Sellers</a></li>
      <li class=""><a data-toggle="tab" href="#dexp_tab_item_top_rating">
          <i class="tab-icon fa "></i>Top Rating</a></li>
  </ul>

  <div class="tab-content">
    <div class="active tab-pane" id="dexp_tab_item_best_seller">
      <?php print views_embed_view('bestsellers','block_bestseller');?>
    </div>
    <div class="tab-pane" id="dexp_tab_item_top_rating">
      <?php print views_embed_view('top_products_rating','block');?>
    </div>
   
  </div>
</div>
