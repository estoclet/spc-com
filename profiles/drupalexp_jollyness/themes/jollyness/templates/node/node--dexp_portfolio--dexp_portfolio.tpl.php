<?php /*457563643457563643*/ ?><?php  ?><?php  ?><div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($title_prefix); ?>
    <?php print render($title_suffix); ?>
    <?php
    // We hide the comments and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    hide($content['body']);
    //$original_image = file_create_url($node->field_portfolio_images['und'][0]['uri']);
    $lightboxrel = 'portfolio_' . $nid;
    $portfolio_images = field_get_items('node', $node, 'field_portfolio_images');
    $first_image = '';
    if ($portfolio_images) {
      foreach ($portfolio_images as $k => $portfolio_image) {
        if ($k == 0) {
          $first_image = file_create_url($portfolio_image['uri']);
        } else {
          $image_path = file_create_url($portfolio_image['uri']);
          print '<a href="' . $image_path . '" rel="lightbox[' . $lightboxrel . ']" style="display:none">&nbsp;</a>';
        }
      }
    }
    ?>
    <div class="portfolio-item-inner">
      <?php print render($content['field_portfolio_images']); ?>
      <div class="portfolio-item-overlay">
        <div class="portfolio-item-tools">
          <a href="<?php print $node_url; ?>" class="view-details"><span class="fa fa-link"></span></a>
          <a href="<?php print $first_image; ?>" data-toggle="tooltip" title="<?php print t('View Large')?>" rel="lightbox[<?php print $lightboxrel;?>]" class="zoom"><span class="fa fa-expand"></span></a>
        </div>
        <a href="<?php print $node_url; ?>" class="title"><?php print $title; ?></a>
      </div>
    </div>
  </div>
</div> 