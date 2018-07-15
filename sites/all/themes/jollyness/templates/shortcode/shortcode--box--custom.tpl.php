<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php
if (strtolower(trim($type)) == 'icon') {
    $place_holder = '<i class="' . $icon . '"></i>';
}
if (strtolower(trim($type)) == 'img') {
    $place_holder = '<img alt="" src="' . $image . '">';
}
if($link!=""){
    $place_holder ="<a href=".$link.">".$place_holder."</a>";
}
?>
<div class="text-center <?php print $classes; ?>">
        <div class="icn-main-container">
            <div data-animate="fadeInUp" class="icn-container in dexp-animate fadeInUp">
                <div class="serviceicon"><?php print $place_holder; ?></div>
            </div>
        </div> 
        <div class="title"><h3><?php print $title; ?></h3></div>
        <p><?php print $content; ?></p>
</div>