<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php
  $image = '';
  if(isset($node->field_portfolio_images[$node->language])){
    $image = file_create_url($node->field_portfolio_images[$node->language][0]['uri']);
  }
	$portfolio_images = field_get_items('node', $node, 'field_portfolio_images');
	$image = '';
	if ($portfolio_images) {
		foreach ($portfolio_images as $k => $portfolio_image) {
			if ($k == 0) {
				$image = file_create_url($portfolio_image['uri']);
			}
		}
	}
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="portfolio-item-inner" style="background-image: url(<?php print $image;?>)">
    <?php print render($title_prefix); ?>
    <?php print render($title_suffix); ?>
    <div class="portfolio-item-overlay">
      <div class="portfolio-item-tools">
      <a href="<?php print $node_url;?>" data-toggle="tooltip" data-original-title="<?php print t('View Details');?>" data-placement="bottom" class="view-details dtooltip"><span class="fa fa-link"></span></a>
      <a href="<?php print $image;?>" rel="lightbox" data-toggle="tooltip" data-original-title="<?php print t('View Image');?>" data-placement="bottom" class="zoom dtooltip"><span class="fa fa-expand"></span></a>
      </div>
      <a href="<?php print $node_url;?>" class="title"><?php print $title;?></a>
    </div>
  </div>
</div>