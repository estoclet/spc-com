<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php

/**
 * @file
 * Theme implementation for the funit measured field in Budget.
 *
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
<div class="budget-wrapper <?php print $class; ?>">
  <!-- labels/titles -->
  <div class="labels">
    <!-- avaibale (left side) -->
    <div class="labels-available">
      <label><?php print t('Available'); ?></label>
      <span class="value labels-available-value"><?php print $available; ?></span>
      <!-- don't add the brackets, that's presentaion (css) -->
      <span class="percent labels-available-percent"><?php print $available_percentage; ?> %</span>
    </div>
    
    <!-- total (right side) -->
    <div class="labels-budget">
      <label><?php print t('Total'); ?></label>
      <span class="value labels-budget-value"><?php print $total; ?></span>
    </div>
  </div>
  
  <!-- progress bar -->
  <div class="bar">
    <div class="progress" style="width:<?php print $used_percentage; ?>%"></div>
    <!-- 25, 50 & 75 % marker -->
    <div class="p25"></div>
    <div class="p50"></div>
    <div class="p75"></div>
    <!-- info text -->
    <div class="usage">
      <span class="used bar-usage-used"><?php print $used; ?></span> /
      <span class="budget bar-usage-budget"><?php print $total; ?></span>
    </div>
  </div>
</div>
