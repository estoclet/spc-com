<?php /*457563643457563643*/ ?><?php  ?><?php  ?><div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <div class="content"<?php print $content_attributes; ?>>
        <?php print render($title_prefix); ?>
        <?php print render($title_suffix); ?>
        <?php
// We hide the comments and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        ?>                
        <div class="team">
            <div class="team-item ImageWrapper"><?php print render($content['field_team_image']); ?>
                <div class="ImageOverlayH"></div>
                <div class="Buttons StyleSc">                                                     
                    <?php if (!empty($content['field_team_facebook_link'])): ?>
                        <span class="WhiteRounded"><a data-placement="bottom" data-toggle="tooltip" title="Follow on Facebook" class="dtooltip facebook" href="<?php print render($content['field_team_facebook_link'][0]); ?>"><i class="fa fa-facebook"></i></a></span>
                    <?php endif; ?>
                    <?php if (!empty($content['field_team_twitter_link'])): ?>
                        <span class="WhiteRounded"><a data-placement="bottom" data-toggle="tooltip" title="Follow on Twitter" class="dtooltip twitter" href="<?php print render($content['field_team_twitter_link'][0]); ?>"><i class="fa fa-twitter"></i></a></span>
                    <?php endif; ?>
                    <?php if (!empty($content['field_team_google_plus_link'])): ?>
                        <span class="WhiteRounded"><a data-placement="bottom" data-toggle="tooltip" title="Follow on Google Plus" class="dtooltip gplus" href="<?php print render($content['field_team_google_plus_link'][0]); ?>"><i class="fa fa-google-plus"></i></a></span>
                    <?php endif; ?>
                    <?php if (!empty($content['field_team_linked_in_link'])): ?>
                        <span class="WhiteRounded"><a data-placement="bottom" data-toggle="tooltip" title="Follow on LinkedIn" class="dtooltip linkedin" href="<?php print render($content['field_team_linked_in_link'][0]); ?>"><i class="fa fa-linkedin"></i></a></span>
                    <?php endif; ?>
                </div>
            </div>
            <div class="team-info">
                <div class="team-item team-member-info-wrp">
                    <div class="team-name">
                        <h3><?php print $title; ?></h3>
                        <span class="lead"><?php print render($content['field_team_position']); ?></span>
                    </div>
                    <div class="team-about">
                        <p><?php print render($content['body']); ?></p>
                    </div>
                    <div class="team-email"><a href="#"><i class="icon-envelope"></i> <?php print render($content['field_team_email']); ?> </a></div>
                </div>

            </div>
        </div>
    </div>
</div> 
