<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php
/**
 * @file
 *
 * Theme implementation to display a Maintenance Countdown page while off-line.
 *
 * You still can use these preprecess functions in your theme:
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $js; ?>
</head>
<body class="<?php print $classes; ?>">
  <div id="page">
    <div id="header">
      <div id="logo-title">

        <?php if (!empty($logo)): ?>
          <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>

        <div id="name-and-slogan">
          <?php if (!empty($site_name)): ?>
            <h1 id="site-name">
              <a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>

          <?php if (!empty($site_slogan)): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div>
      </div>

      <?php if (!empty($header)): ?>
        <div id="header-region">
          <?php print $header; ?>
        </div>
      <?php endif; ?>

    </div>

    <div id="container" class="clearfix">

      <div id="main" class="column"><div id="main-squeeze">

        <div id="content">
          <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php if (!empty($messages)): print $messages; endif; ?>
          <div id="content-content" class="clearfix">
            <?php print $content; ?>
          </div>
        </div>

        <div id="countdown_dashboard" class="clear-block">

          <div class="dash weeks_dash">
              <span class="dash_title"><?php print t('weeks')?></span>
              <div class="digit"></div>
              <div class="digit"></div>
            </div>

          <div class="dash days_dash">
            <span class="dash_title"><?php print t('days')?></span>
            <div class="digit"></div>
            <div class="digit"></div>
          </div>

          <div class="dash hours_dash">
            <span class="dash_title"><?php print t('hours')?></span>
            <div class="digit"></div>
            <div class="digit"></div>
          </div>

          <div class="dash minutes_dash">
            <span class="dash_title"><?php print t('minutes')?></span>
            <div class="digit"></div>
            <div class="digit"></div>
          </div>

          <div class="dash seconds_dash">
            <span class="dash_title"><?php print t('seconds')?></span>
            <div class="digit"></div>
            <div class="digit"></div>
          </div>

        </div>
        <!-- Countdown dashboard end -->

        <?php if (!empty($subsc)): ?>
        <div class="subscribe-email-form"><div class="email-inner">
          <?php print $subsc; ?>
        </div></div>
        <?php endif; ?>

        <?php if ($time_up_message || $reload_button): ?>
        <div id="complete_message" class="clear-block">
          <?php if ($time_up_message) : print $time_up_message; endif; ?>
          <?php if ($reload_button) : print $reload_button; endif; ?>
        </div>
        <?php endif; ?>

      </div></div>

    </div>

  </div>

  <?php print $mcd_js; ?>
  <script type="text/javascript">
  <!--//--><![CDATA[//><!--
  <?php print $mcd_js_countdown; ?>
  //--><!]]>
  </script>
</body>
</html>