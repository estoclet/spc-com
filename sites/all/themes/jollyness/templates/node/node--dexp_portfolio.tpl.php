<?php /*457563643457563643*/ ?><?php  ?><?php  ?><div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>
  <?php
  // We hide the comments and links now so that we can render them later.
  hide($content['comments']);
  hide($content['links']);
  hide($content['field_portfolio_images']);
  ?>
  <div class="content row"<?php print $content_attributes; ?>>
    <div class="col-lg-8 col-md-8 col-sm-6 col-xs-12">
      <?php print render($content['field_portfolio_images']) ?>
    </div>
    <div class="col-lg-4 col-md-4 -col-sm-6 col-xs-12">
      <h3 class="portfolio-detail"><?php print t('Job Description');?></h3>
      <?php
      print render($content);
      ?>
    </div>
  </div>
  <?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>
</div>