<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php $has_sub_title = false;
if (strpos($content, '~')): ?>
    <?php
    $has_sub_title = true;
    $title_array = explode('~', $content);
    $main_title = $title_array[0];
    $sub_title = $title_array[1];
    ?>
    <?php endif; ?>
<div class="<?php print $class; ?>">
<?php if ($has_sub_title): ?>
        <h1><?php print $main_title; ?></h1>
        <hr>        
        <p class="lead"><?php print $sub_title; ?></p>
    <?php else: ?>
        <h1><?php print $content; ?></h1>
        <hr> 
<?php endif; ?>
</div> 