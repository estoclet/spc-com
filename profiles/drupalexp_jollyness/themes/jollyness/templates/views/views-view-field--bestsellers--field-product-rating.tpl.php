<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
?>
<div class="rating" style='text-align: left'>
    
<?php 
$nid = $row->{$field->field_alias};
$node = node_load($nid);
if (isset($node->field_product_rating[$node->language][0]['average'])) {
$average = $node->field_product_rating[$node->language][0]['average'];
    $stars = 0;
    if ($average >= 90) {
       $stars = 5;
    } else {
        if ($average >= 70 && $average < 90) {
            $stars = 4;
        } else {
            if ($average >=50 && average < 70) {
                $stars = 3;
            } else {
                if ($average >=30 && average < 50) {
                    $stars = 2;
                } else {
                    if ($average >10 && average < 30) {
                        $stars = 1;
                    } 
                }
            }
        }
    }
    $i = 0;
    while ($i < $stars) {
     print '<i class="fa fa-star"></i>';
     $i++;
    }

    $j = 0;
    while ($j < 5 - $stars) {
     print '<i class="fa fa-star-o"></i>';
     $j++;
    }
}
?>
</div>