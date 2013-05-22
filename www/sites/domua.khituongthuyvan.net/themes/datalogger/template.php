<?php
 
 
global $user;
global $site_name;
global $khuvuc;

$module_path = drupal_get_path('module', 'datalogger');
drupal_add_js($module_path . '/jquery.infieldlabel.min.js');
drupal_add_js($module_path . '/swfobject/swfobject.js');
drupal_add_js( <<< CODE
  var flashvars = {
    khuvuc: {$khuvuc[2]}
  };
  var params = {
    menu: "false",
    wmode: "transparent"
  };
  var attributes = {
    id: "myDynamicContent",
    name: "myDynamicContent",
    style:"z-index: -1000; margin: 0 auto;"
  };

  swfobject.embedSWF("/sites/domua.khituongthuyvan.net/modules/datalogger/scripts/swfobject/bannerFlash.swf", "bannerFlash", "960", "180", "9.0.0","/sites/domua.khituongthuyvan.net/modules/datalogger/scripts/swfobject/expressInstall.swf", flashvars, params, attributes);

CODE
 ,'inline');

if (in_array('administrator', array_values($user->roles)) || in_array('control datalogger', array_values($user->roles))) {
  // variable_set('site_name', t('Phần mềm điều khiển trạm đo mưa tự động'));
  variable_set('site_name', t('Software control automatic rain gauge stations'));
} else {
  // variable_set('site_name', t('Hệ thống khai thác dữ liệu đo mưa tự động 2012'));
  variable_set('site_name', t('Data mining system automatic rain gauge 2012'));
}

if(in_array('manage data', array_values($user->roles)) && in_array('control datalogger', array_values($user->roles)) || $user->uid == 1){
  drupal_add_css(path_to_theme() . '/css/do.css');
} else if(in_array('control datalogger', array_values($user->roles))){
  drupal_add_css(path_to_theme() . '/css/cam.css');
} else if(in_array('manage data', array_values($user->roles))){
  drupal_add_css(path_to_theme() . '/css/xanhla.css');
} else if($user->uid == 0){
  drupal_add_css(path_to_theme() . '/css/xanhdatroi.css');
}

function datalogger_preprocess_html(&$variables) {

  // switch ($_REQUEST['q']) {
  // case '/stations':
    // drupal_set_title(t('List All Datalogger'));
    // break;
  // }

  if (!empty($variables['page']['featured'])) {
    $variables['classes_array'][] = 'featured';
  }

  if (!empty($variables['page']['triptych_first'])
    || !empty($variables['page']['triptych_middle'])
    || !empty($variables['page']['triptych_last'])) {
    $variables['classes_array'][] = 'triptych';
  }

  if (!empty($variables['page']['footer_firstcolumn'])
    || !empty($variables['page']['footer_secondcolumn'])
    || !empty($variables['page']['footer_thirdcolumn'])
    || !empty($variables['page']['footer_fourthcolumn'])) {
    $variables['classes_array'][] = 'footer-columns';
  }

  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
	
	drupal_add_css(path_to_theme() . '/css/dropdown.css', array('group' => CSS_THEME, 'preprocess' => FALSE));
	drupal_add_css(path_to_theme() . '/css/dropdown_ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'preprocess' => FALSE));

	drupal_add_js(drupal_get_path('module', 'datalogger') . '/jquery_msgBox/Scripts/jquery.msgBox.js');
	drupal_add_css(drupal_get_path('module', 'datalogger') . '/jquery_msgBox/Styles/msgBoxLight.css', array('group' => CSS_THEME, 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function datalogger_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

function datalogger_page_alter(&$page){
	global $user, $admin_khuvuc;
	$user_path = explode('/', current_path())[0];
	
	if($user_path.'/'=='user/'){
		$user_edit = explode('/', current_path())[1];
		$user_edit_fields = user_load($user_edit);
		if($user_edit!=$user->uid && !in_array($user->name, $admin_khuvuc)){
			header( 'Location: /user');
		}
		//drupal_set_message('_page_alter: '.$user->uid.'-'.$user->name.'-'.$user_edit_fields->name.'</br>');
	}
}

/**
 * Override or insert variables into the page template.
 */
function datalogger_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
      );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function datalogger_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  drupal_add_css(drupal_get_path('theme', 'datalogger') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function datalogger_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
	
	//Thiết lập title cho các view với các tham số khách nhau
	// if($variables['nghe_an'])
	
	
}

/**
 * Override or insert variables into the node template.
 */
function datalogger_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';    
  }
	
	$node = &$variables['node'];	
}

/**
 * Override or insert variables into the block template.
 */
function datalogger_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements theme_menu_tree().
 */
function datalogger_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_field__field_type().
 */
function datalogger_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}

function datalogger_preprocess_page(&$variables) {
	if(count(arg()) == 5 && arg(0) == 'view' && arg(1) == 'list' && arg(2) == 'station'){
		$area = arg(3);
		$areas = _datalogger_areas($area);
		$province = arg(4);
		$provinces = _datalogger_provinces($province);
		if(!empty($area)&& $province != 'all'){
			$province = t('List of automatic rainfall stations '.$provinces);
			drupal_set_title($province);
		}else{
			if(!empty($area)&& $province == 'all'){
				$area = t('List of meteorological stations '.$areas);
				drupal_set_title($area);
			}
		}
	}
	
	if (drupal_is_front_page()) {
  
    global $khuvuc;
    
    require 'khuvuc.php';
					
		//drupal_set_title(t('LIST OF DATALOGGER STATIONS OF @area', array('@area' => $khuvuc)));
  }
  
  $block = module_invoke('superfish', 'block_view', '1');
  if ($block['content']) {
      $output = "<div class=\"links clearfix\">\n";
      $output .= "<div class=\"element-invisible\">".$block['content']."</div>\n";
      $output .= "</div>\n";
      $variables['menu1'] = $block['content'];
      
  }
}
   
function _datalogger_provinces($arg){
	$list = array('ho_chi_minh'=> 'Ho Chi Minh',
	'hai_phong'=> 'Hai Phong',
	'da_nang'=> 'Da Nang',
	'ha_giang'=> 'Ha Giang',
	'cao_bang'=> 'Cao Bang',
	'lai_chau'=> 'Lai Chau',
	'lao_cai'=> 'Lao Cai',
	'tuyen_quang'=> 'Tuyen Quang',
	'lang_son'=> 'Lang Son',
	'bac_kan'=> 'Bac Kan',
	'thai_nguyen'=> 'Thai Nguyen',
	'yen_bai'=> 'Yen Bai',
	'son_la'=> 'Son La',
	'phu_tho'=> 'Phu Tho',
	'vinh_phuc'=> 'Vinh Phuc',
	'quang_ninh'=> 'Quang Ninh',
	'bac_giang'=> 'Bac Giang',
	'bac_ninh'=> 'Bac Ninh',
	'ha_noi'=> 'Ha Noi',
	'hai_duong'=> 'Hai Duong',
	'hung_yen'=> 'Hung Yen',
	'hoa_binh'=> 'Hoa Binh',
	'ha_nam'=> 'Ha Nam',
	'nam_dinh'=> 'Nam Dinh',
	'thai_binh'=> 'Thai Binh',
	'ninh_binh'=> 'Ninh Binh',
	'thanh_hoa'=> 'Thanh Hoa',
	'nghe_an'=> 'Nghe An',
	'ha_tinh'=> 'Ha Tinh',
	'quang_binh'=> 'Quang Binh',
	'quang_tri'=> 'Quang Tri',
	'thua_thien_hue'=> 'Thua Thien Hue',
	'quang_nam'=> 'Quang Nam',
	'quang_ngai'=> 'Quang Ngai',
	'kom_tum'=> 'Kon Tum',
	'binh_dinh'=> 'Binh Dinh',
	'giai_lai'=> 'Gia Lai',
	'phu_yen'=> 'Phu Yen',
	'dak_lak'=> 'Dak Lak',
	'khanh_hoa'=> 'Khanh Hoa',
	'lam_dong'=> 'Lam Dong',
	'binh_phuoc'=> 'Binh Phuớc',
	'binh_duong'=> 'Binh Duong',
	'ninh_thuan'=> 'Ninh Thuan',
	'tay_ninh'=> 'Tay Ninh',
	'binh_thuan'=> 'Binh Thuan',
	'dong_nai'=> 'Dong Nai',
	'long_an'=> 'Long An',
	'dong_thap'=> 'Dong Thap',
	'an_giang'=> 'An Giang',
	'ba_ria_vung_tau'=> 'Ba Ria - Vung Tau',
	'tien_gian'=> 'Tien Giang',
	'kien_giang'=> 'Kien Giang',
	'can_tho'=> 'Can Tho',
	'ben_tre'=> 'Ben Tre',
	'vinh_long'=> 'Vinh Long',
	'tra_vinh'=> 'Tra Vinh',
	'soc_trang'=> 'Soc Trang',
	'bac_lieu'=> 'Bac Lieu',
	'ca_mau'=> 'Ca Mau',
	'dien_bien'=> 'Dien Bien',
	'dak_nong'=> 'Dak Nong',
	'hau_giang'=> 'Hau Giang',);
	return $list[$arg];
}

function _datalogger_areas($arg){
	$list = array('north_central_region' => 'North Central Region',
		'mid_central_region' => 'Middle Central Region',
		'central_highlands' => 'Central Highlands Region',
		'south_central_region' => 'South Central Region',
		'south_region' => 'Southern Region',);
	return $list[$arg];
}

  
