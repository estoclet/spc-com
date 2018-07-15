<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php
/**
* @file field.tpl.php
* Default template implementation to display the value of a field.
*
* This file is not used and is here as a starting point for customization only.
* @see theme_field()
*
* Available variables:
* - $items: An array of field values. Use render() to output them.
* - $label: The item label.
* - $label_hidden: Whether the label display is set to 'hidden'.
* - $classes: String of classes that can be used to style contextually through
* CSS. It can be manipulated through the variable $classes_array from
* preprocess functions. The default values can be one or more of the
* following:
* - field: The current template type, i.e., "theming hook".
* - field-name-[field_name]: The current field name. For example, if the
* field name is "field_description" it would result in
* "field-name-field-description".
* - field-type-[field_type]: The current field type. For example, if the
* field type is "text" it would result in "field-type-text".
* - field-label-[label_display]: The current label position. For example, if
* the label position is "above" it would result in "field-label-above".
*
* Other variables:
* - $element['#object']: The entity to which the field is attached.
* - $element['#view_mode']: View mode, e.g. 'full', 'teaser'...
* - $element['#field_name']: The field name.
* - $element['#field_type']: The field type.
* - $element['#field_language']: The field language.
* - $element['#field_translatable']: Whether the field is translatable or not.
* - $element['#label_display']: Position of label display, inline, above, or
* hidden.
* - $field_name_css: The css-compatible field name.
* - $field_type_css: The css-compatible field type.
* - $classes_array: Array of html class attribute values. It is flattened
* into a string within the variable $classes.
*
* @see template_preprocess_field()
* @see theme_field()
*
* @ingroup themeable
*/
?> 
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <div class="field-items"<?php print $content_attributes; ?>>
        <?php 
        
        $num_images = count($items);
        $lightboxrel = "product_".$num_images;
        foreach ($items as $delta => $item): ?>
            <?php 
                $path_image = file_create_url($item['#item']['uri']);
                if ($delta == 0) :?>
                <div class="ImageWrapper">
                    <div class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>>
                        <?php print render($item); ?>
                    </div>
                
                    <div class="ImageOverlayH"></div>
                    <div class="Button StyleMg">
                        <span class="WhiteRounded">
                            <a href="<?php print $path_image;?>" rel="lightbox[<?php print $lightboxrel;?>]">
                                <i class="fa fa-search"></i>
                            </a>
                        </span>
                    </div>
                </div>   
            <?php else:?>    
                <?php if ($delta == 1) :?>
                <div class="thumbnails clearfix">
                <?php endif; ?>    
                  <a href="<?php print $path_image;?>" class="zoom" title="" rel="lightbox[<?php print $lightboxrel;?>]">				
                    <img class="img-thumbnail" width="120" height="120" src="<?php print $path_image;?>" alt="" />
                  </a>
                <?php if ($delta == $num_images - 1) :?>      
                </div>
                <?php endif; ?>    
            <?php endif;?>
        
        <?php endforeach; ?>
    </div>
</div> 