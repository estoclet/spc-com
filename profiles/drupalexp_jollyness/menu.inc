<?php
function drupalexp_profile_menu_items(&$items){
	$items[] = array(
    'link_title' => st('Portfolio'),
    'link_path' => 'portfolio',
    'menu_name' => 'main-menu',
		'weight' => 1,
  );
	$items[] = array(
    'link_title' => st('Blog'),
    'link_path' => 'blog',
    'menu_name' => 'main-menu',
		'weight' => 2,
  );
}