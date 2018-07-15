<?php /*457563643457563643*/ ?><?php  ?><?php  ?><div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>
  <?php
// We hide the comments and links now so that we can render them later.
  hide($content['comments']);
  hide($content['links']);
  hide($content['field_portfolio_images']);
  hide($content['field_portfolio_url']);
  ?>
  <div class="row">
    <div class="col-sm-6 col-xs-12">
      <?php print render($content['field_portfolio_images']); ?>
    </div>
    <div class="col-sm-6 col-xs-12">
      <h3 class="portfolio-title"><a href="<?php print $node_url;?>"><?php print $title;?></a></h3>
      <?php print render($content); ?>
      <a class="btn btn-primary float-left" href="<?php print $node_url;?>"><?php print t('VIEW DETAILS');?> </a>
      <?php print render($content['field_portfolio_url']);?>
    </div>
  </div>
</div>
<div class="hr"></div>