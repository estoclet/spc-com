<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php
/**
 * @file
 * royalsliderfield.tpl.php
 */
?>

<div<?php print drupal_attributes($attributes_array); ?>>
  <?php foreach ($slides_processed as $slide): ?>
      <?php print drupal_render($slide); ?>
  <?php endforeach; ?>
</div>
