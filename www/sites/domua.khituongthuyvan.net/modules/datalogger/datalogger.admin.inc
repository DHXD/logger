<?php
$stations = views_get_view_result('list_all_datalogger');
GLOBAL $station_name;
$station_name = array();
foreach($stations as $station){
	$station_name[$station->nid] = $station->node_title;
}

global $clearTime;
$clearTime = array('00:00','01:00','02:00','03:00','04:00','05:00','06:00','07:00','08:00','09:00','10:00','11:00','12:00','13:00','14:00','15:00','16:00','17:00','18:00','19:00','20:00','21:00','22:00','23:00');

global $clearTime3;
$clearTime3 = array(h0 => '00:00:00',h1 => '01:00:00',h2 => '02:00:00', h3 => '03:00:00',h4 => '04:00:00',h5 => '05:00:00',h6 => '06:00:00',h7 => '07:00:00', h8 => '08:00:00', h9 => '09:00:00', h10 => '10:00:00', h11 => '11:00:00', h12 => '12:00:00', h13 => '13:00:00', h14 => '14:00:00', h15 => '15:00:00', h16 => '16:00:00', h17 => '17:00:00', h18 => '18:00:00', h19 => '19:00:00', h20 => '20:00:00',h21 => '21:00:00',h22 =>'22:00:00', h23 => '23:00:00');

global $clearTime2;
$clearTime2 = array('0000','0100','0200','0300','0400','0500','0600','0700','0800','0900','1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300');

global $datalogger_pattern;
$datalogger_pattern = array(
		'I:STATUS',
		'S:',
		'T:',
		'T:NITZ',
		'D:',
		//'D:?',
		// 'P:',
		'W:',
		//'P:?',
		'R:FORMAT ALL',
		'R:FORMAT DAY:',
		'R:SET:',
		'R:CLEAR TIME:',
		//'R:CLEAR:?',
		//////'P:RESET', // duplicated
		'P:BAT ON:',
		//'P:BAT ON:?',
		'P:BAT OFFSET:',
		'P:AUTO PWR MANAGEMENT:',
		//'P:AUTO PWR MANAGEMENT:?',
		'P:GOTO SLEEP MODE',
		'C:INTERVAL:',
		//'C:INTERVAL:?',
		'C:COMM:',
		//'C:COMM:?',
		'G:APN:',
		'A:ALARM INTERVAL:',
		//'G:APN:?',
		'N:HOST:',
		//'N:HOST:?',
		'H:HOST PHONE NO:',
		//'H:HOST PHONE NO:?',
		//'V:VERSION:?',
		'V:VERSION:',
		'P:RESET');

	// GLOBAL $options_commands;
	// $options_commands = array(
    // //Data call command
    // 1 => t('Real Data Call'),
    // 2 => t('Save Data Call'),
    // //Date and Time Set
    // 3 => t('Date $ Time Set'),
    // 4 => t('GPRS Network Time Synchronization'),
    // //ID $ Password Set
    // 5 => t('Set ID'),
    // 6 => t('Get ID'),
    // 7 => t('Set Password'),
    // 8 => t('Get Password'),
    // //database Buffer Clear
    // 9 => t('All Database Buffer Clear'),
    // 10 => t('Specific Date Database Buffer Clear'),
    // //Rain fall config
    // 11 => t('Accumulated Rain Fall Value Set'),
    // 12 => t('Set Rain Fall Clear Time'),
    // 13 => t('Get Rain Fall Clear Time'),
    // //Datalogger config
    // 14 => t('Self Power Reset'),
    // 15 => t('Set Active / De-Active Battery Power Voltage'),
    // 16 => t('Get Active / De-Active Battery Power Voltage'),
    // 17 => t('Set Battery Voltage Offset'),
    // 18 => t('Get Battery Voltage Offset'),
    // 19 => t('Set Auto Power Management Enable'),
    // 20 => t('Set Auto Power Management Disable'),
    // 21 => t('Get Auto Power Management Enable / Disable'),
    // 22 => t('Go To Sleep Mode'),
    // //Communication setting
    // 23 => t('Set Data Send Interval Minutes'),
    // 24 => t('Get Data Send Interval Minutes'),
    // 25 => t('Set Communication Type GSM'),
    // 26 => t('Set Communication Type GPRS'),
    // 27 => t('Set Communication Type Auto'),
    // 28 => t('Get Communication Type'),
    // //GPRS network setting
    // 29 => t('Set GPRS Network Setting'),
    // 30 => t('Get GPRS Network Setting'),
    // 31 => t('Set Host Network Infomation'),
    // 32 => t('Get Host Network Infomation'),
    // 33 => t('Set Host Phone Number'),
    // 34 => t('Get Host Phone Number'),
    // //Get datalogger firmware version
    // 35 => t('Get Datalogger Firmware Version'),
		// //Alarm Options
		// 36 => t('Set the Alarm Interval and Level for rainfall alarming'),
		// 37 => t('Get the Alarm Interval and Level for rainfall alarming'),
		// 38 => t('Reset All'),);	

		
/**
 *Begin: 25/11. 
 *author:hungnm0302
 *Lay ten cac tram do mua theo role khu vuc
 */
/*thong ke luong mua theo gio*/
/**
 *Begin: 25/11. 
 *author:hungnm0302
 *Lay ten cac tram do mua theo role khu vuc
 */
/*Ham xu ly hien thi cac lenh chua duoc thuc hien*/
function _datalogger_check_error_send_command_form(){
	$page = arg(3);
	$headers = array(
    no => t('No.'),
		station_name => t('Station Name'),
    sending_date_time => t('Sending Date Time'),
    sender_number => t('Destination Number'),
    message_content => t('Text Decoded'),
		id => ID,
		Modem => t('Sender ID'),
    status => t('Status'),
		send_again => t('Send again'),
  );
  $num_per_page = 50;
  $offset=$num_per_page*$page;
	$total = db_query("SELECT count(*) total FROM `sentitems` WHERE Status= 'SendingError'");
	$number_rows = $total->fetch()->total;
	$last = $number_rows/$num_per_page;
	if(!is_int($last)){
		$last = (int)$last+1;
	}
	
  $result = db_query("SELECT * FROM `sentitems` WHERE Status= 'SendingError' LIMIT ". $offset.",".$num_per_page."");
	$rows = array();
  while($row = $result->fetchObject()){
		$number = $row->DestinationNumber;
		$station = views_get_view_result('view_station_find', Null, $number );
		if(!empty($station)){
			$station_name = $station[0]->node_title;
			$nid = $station[0]->nid;
			$cid = $row -> ID;
		}
		else{
			$station_name = null;	
		}
    
    $sms = $row -> TextDecoded;
    $len_sms = strlen($row->TextDecoded);
		
    $rows[] = array (
			$offset+=1,
			$station_name,
			$row -> SendingDateTime,
			$row -> DestinationNumber,
			$row -> TextDecoded,
			$cid,
			$row -> SenderID,
      $row -> Status ? t('Sending Error') : t('Sending Ok'),
			'<a href=http://admin.domua.khituongthuyvan.net/command/'.$nid.'/editsentcommand/'.$cid.'>'.t('Send again').'</a>',
		);
  }
  $retval=theme('table', array(
                'header' => $header,
                'rows' => $rows
                 ));
	$output = theme_table(
    array(
      "header" => $headers,
      "rows" => $rows,
      "attributes" => array(),
      "sticky" => true, // Table header will be sticky
      "caption" => t('Date').': '.date('d/m/Y',time()),
      "colgroups" => array(),
      "empty" => t("Table has no row!") // The message to be displayed if table is empty
    )
  ).theme("pager");
  $prevpage = $page-1;
  $nextpage = $page+1;
  if($page > 0){
    $output = $output . l(t("<< first"),"gammu/sentitems/error");
    $output = $output . l(t("< previous"),"gammu/sentitems/error/".$prevpage). "  ";
  }
	if($number_rows > $num_per_page){
		if($nextpage < $last){
			$output = $output . l(t("next >"),"gammu/sentitems/error/".$nextpage);
			$output = $output . l(t("last >>"),"gammu/sentitems/error/".($last-1));
		}
	}
  return $output;
}
 
 /**
 *Begin: 25/11. 
 *author:hungnm0302
 *Lay ten cac tram do mua theo role khu vuc
 */
function _datalogger_ftp_server(){
	try {
		/*
		$jail = dsm(drupal_realpath('public://'));
		
		$settings['username'] = 'root';
		$settings['password'] = 'root';
		$settings['advanced']['hostname'] = 'localhost';
		$settings['advanced']['port'] = '21';
		*/
		$jail = FTP_JAIL;
		$settings = array(
				// 'username' => FTP_USER,
				// 'password' => FTP_PASS,		
				'username' => 'root',
				'password' => 'root',
				'advanced' => array(
						// 'hostname' => FTP_HOST,
						// 'port' => FTP_PORT,
						'hostname' => 'localhost',
						'port' => '21',
				),
		);
	
		// $ftpserver = FileTransferFTP::factory($jail, $settings);
		$ftpserver = FileTransferFTP::factory($jail, $settings);
		$ftpserver->connect();
		$source = '/shareFile';
		$destination = drupal_realpath('public://shareFile//');
		// $ftpserver->copyDirectory($destination,$source);
		$ftpserver->copyFile($destination.'abc.jpg',$source);
	}
	catch(Exception $e) {
	}
}
 
 /**
 *Begin: 21/11. 
 *author:hungnm0302
 *Lay ten cac tram do mua theo role khu vuc
 */
 
function _datalogger_station_name_list(){
	global $user;
	if (!in_array('administrator', array_values($user->roles)) && !in_array('control datalogger', array_values($user->roles)) && !in_array('manage data', array_values($user->roles))) {
				$account = user_load($user->uid);
				// dsm($account);
				if(isset($account->field_station_region['und'])){
					foreach($account->field_station_region['und'] as $region){
						$stations[] = views_get_view_result('view_station_filter_parameter',Null,$region['value'], 'all','all');
					}
				}
	}
	else{
		$stations[] = views_get_view_result('view_station_filter_parameter',Null,'all', 'all','all');
	}
	foreach($stations[0] as $station){
		$station_name[$station->nid] = $station->node_title;
	}
	return $station_name;
} 

/**
	*Begin: 16/11. 
 *author:Nguyễn Mạnh Hùng
 */
 /*
function _datalogger_return_area($areas){
	$regions_list = array(
		north_central_region => t('North Central Region'),
		mid_central_region  => t('Central Central Region'),  
		south_central_region  => t('South Central Region'),
		south_region => t('South Region'),
		central_highlands => t('Central Highlands Region'),
	);
	// $regions = array();
	if($areas == Null){
		return $regions_list;
	}
	else{
		foreach($areas as $area){
			$regions[] = $regions_list[$area[0]];
		}
		return $regions;
	}
}

function _datalogger_return_city($citys){
	$provinces_list = array(
		ho_chi_minh => t('Ho Chi Minh'),
		hai_phong => t('Hai Phong'),
		ha_giang => t('Ha Giang'),
		cao_bang => t('Cao Bang'),
		lai_chau => t('Lai Chau'),
		lao_cai => t('Lao Cai'),
		tuyen_quang => t('Tuyen Quang'),
		lang_son => t('Lang Son'),
		bac_kan => t('Bac Kan'),
		thai_nguyen => t('Thai Nguyen'),
		yen_bai => t('Yen Bai'),
		son_la => t('Son La'),
		phu_tho => t('Phu Tho'),
		vinh_phuc => t('Vinh Phuc'),
		quang_ninh => t('Quang Ninh'),
		bac_giang => t('Bac Giang'),
		bac_ninh => t('Bac Ninh'),
		ha_noi => t('Ha Noi'),
		hai_duong => t('Hai Duong'),
		hung_yen => t('Hung Yen'),
		hoa_binh => t('Hoa Binh'),
		ha_nam => t('Ha Nam'),
		nam_dinh => t('Nam Dinh'),
		thai_binh => t('Thai Binh'),
		ninh_binh => t('Ninh Binh'),
		dong_nai => t('Dong Nai'),
		long_an => t('Long An'),
		bac_lieu => t('Bac Lieu'),
		dien_bien => t('Dien Bien'),
		paging0 => '------------------',				
		north_central_region => array(thanh_hoa => t('Thanh Hoa'),
		nghe_an => t('Nghe An'),
		ha_tinh => t('Ha Tinh'),),
		paging1 => '------------------',
		mid_central_region => array(quang_tri => t('Quang Tri'),
		quang_binh => t('Quang Binh'),
		thua_thien_hue => t('Thua Thien - Hue'),
		da_nang => t('Da Nang'),
		quang_nam => t('Quang Nam'),
		quang_ngai => t('Quang Ngai'),),
		paging2 => '------------------',
		south_central_region => array(binh_dinh => t('Binh Dinh'),
		phu_yen => t('Phu Yen'),
		khanh_hoa => t('Khanh Hoa'),
		ninh_thuan => t('Ninh Thuan'),
		binh_thuan => t('Binh Thuan'),),
		paging3 => '------------------',
		south_region => array(binh_duong => t('Binh Duong'),
		tay_ninh => t('Tay Ninh'),
		tien_gian => t('Tien Giang'),
		tra_vinh => t('Tra Vinh'),
		can_tho => t('Can Tho'),
		soc_trang => t('Soc Trang'),
		dong_thap => t('Dong Thap'),
		binh_phuoc => t('Binh Phuoc'),
		an_giang => t('An Giang'),
		kien_giang => t('Kien Giang'),
		ca_mau => t('Ca Mau'),
		ben_tre => t('Ben Tre'),
		ba_ria_vung_tau => t('Ba Ria - Vung Tau'),
		vinh_long => t('Vinh Long'),
		hau_giang => t('Hau Giang'),),
		paging4 => '------------------',
		central_highlands  => array(lam_dong => t('Lam Dong'),
		kom_tum => t('Kon Tum'),
		giai_lai => t('Gia Lai'),
		dak_lak => t('Dak Lak'),
		dak_nong => t('Dak Nong'),),
	);
	// $provinces[] = array();
	foreach($citys as $city){
		$provinces[] = $provinces_list[$city[0]];
	}
	return array_merge($provinces);
}
/*		
		
/**
 *Begin: 19/11. 
 *author:Nguyễn Mạnh Hùng
 *description:danh sach cac tram do mưa theo khu vực/tinh/tram
 */
function _datalogger_station_ajax($area, $province,$nid){
	$stations = views_get_view_result('view_station_filter_parameter',Null, $area, $province, $nid);
	return $stations;
}
		
		
/**
 *Begin: 17/11. 
 *author:Phan Văn Tuấn
 *description:tạo form xuất báo cáo theo năm
 */
//
function _datalogger_set_station_callback($form, &$form_state){
	return $form['station'];
}

function _datalogger_set_station_options($area, $province) {
    // $options = array(
        // 'colors' => array(
            // 'red' => 'Red',
            // 'green' => 'Green',
            // 'blue' => 'Blue'
        // ),
        // 'cities' => array(
            // 'paris' => 'Paris, France',
            // 'tokyo' => 'Tokyo, Japan',
            // 'newyork' => 'New York, US'
        // ),
        // 'animals' => array(
            // 'dog' => 'Dog',
            // 'cat' => 'Cat',
            // 'bird' => 'Bird'
        // ),  
    // );
    // if (isset($options[$key])) {
        // return $options[$key];
    // }
    // else {
        $stations = views_get_view_result('view_station_filter_parameter', NULL, $area, $province, 'all');
				foreach($stations as $station){
					$titles[] = array($station->nid => $station->node_title);
				}
        return $titles;
    // }
}

function _datalogger_report_rainfall_year_form($form, &$form_state){
	drupal_set_title(t('Report annual rainfall data'));
	_datalogger_select_area_province_station($form, $form_state);
  
  $form['stations_fieldset']['selectReport']['#access']=FALSE;
	$form['stations_fieldset']['month']['#access']=FALSE;
	$form['stations_fieldset']['day']['#access']=FALSE;
	$form['person']['txtNameQTV']['#access']=FALSE;
	$form['person']['txtNameNLB']['#access']=FALSE;
	$form['person']['txtNameKS']['#access']=FALSE;
	$form['person']['txtNamePT']['#access']=FALSE;
	
	
	$form["submit"] = array(
      "#type" => "submit",
      "#value" => t("Report"),
      "#submit" => array(
          "_datalogger_report_rainfall_year_form_submit"
      ),
  ); 
  return $form;
}

function _datalogger_ajax_dropdown_area(&$form, &$form_state){
	return $form['selectProvince'];
}

function _datalogger_ajax_dropdown_province(&$form, &$form_state){
	return $form['selectStation'];
}

function _datalogger_report_rainfall_year_form_submit(&$form, &$form_state) {
	// module_load_include('inc', 'datalogger', 'datalogger.admin.excel');
	module_load_include('inc', 'datalogger', 'datalogger.admin.statistics.excel');
	$nid = $form_state['values']['station'];
	$area = $form_state['values']['selectArea'];
	$province = $form_state['values']['province'];
	$year = $form_state['values']['year'];
	$rainfalls = views_get_view_result('view_export_excel', Null, $nid, $year, 'all', 'all');
	if(count($rainfalls) <= 0){
		drupal_set_message(t('Rainfall stations in the no data to report'),'error');
	}
	else{
		// $args = array($nid, $rainfalls, $name_qtv, $name_nlb, $name_ks, $name_pt);
		$args = array($nid, $rainfalls);
		_export_excel($args);
	}
}

/**
 *author:Phan Văn Tuấn
 *description:tạo form xuất báo cáo theo ngày
 */
function _datalogger_report_rainfall_all_station_in_day_form($form, &$form_state){
	drupal_set_title(t('Statistics of rainfall during the day'));
	_datalogger_select_area_province_station($form, $form_state);
  $form['stations_fieldset']['selectReport']['#access']=FALSE;
	$form['stations_fieldset']['year']['#access']=FALSE;
	$form['stations_fieldset']['month']['#access']=FALSE;
	$form['person']['txtNameQTV']['#access']=FALSE;
	$form['person']['txtNameNLB']['#access']=FALSE;
	$form['person']['txtNameKS']['#access']=FALSE;
	$form['person']['txtNamePT']['#access']=FALSE;
	
	
	$form['stations_fieldset']['selectStation']['station']['#multiple']= TRUE;//cho phep chon nhieu tram do mua
	$form['stations_fieldset']['selectStation']['station']['#size']= 10;//hien thi toi da la 10 phan tu
	
  $form["submit"] = array(
      "#type" => "submit",
      "#value" => t("Report"),
      "#submit" => array(
          "_datalogger_report_rainfall_all_station_in_day_submit"
      ),
  ); 
  return $form;
}

function _datalogger_report_rainfall_all_station_in_day_submit(&$form, &$form_state) {
	// module_load_include('inc', 'datalogger', 'datalogger.admin.excel');
	module_load_include('inc', 'datalogger', 'datalogger.admin.excel.stations.day');
	$region = $form['selectArea']['#options'][$form_state['values']['selectArea']];
	$province = $form['selectProvince']['province']['#options'][$form_state['values']['province']];
	$station_nids = $form_state['values']['station'];
	$day = new DateTime($form_state['values']['day']);
	$year = $day->format('Y');
	$month = $day->format('Ym');
	$day = $day->format('Ymd');
	$args = array($station_nids, $year, $month, $day);
	_datalogger_export_rainfall_month_excel($region, $province, $station_nids, $year, $month, $day);
}

function _datalogger_report_rainfall_day_form($form, &$form_state){
	drupal_set_title(t('Reports daily rainfall data'));
  
	_datalogger_select_area_province_station($form, $form_state);
	$form['stations_fieldset']['year']['#access']=FALSE;
	$form['stations_fieldset']['month']['#access']=FALSE;
  $form['stations_fieldset']['selectStation']['station']['#multiple']= TRUE;
  $form['stations_fieldset']['selectProvince']['province']['#multiple']= TRUE;
  
  $form['stations_fieldset']['selectReport']['#access']=FALSE;
	
  /*$form['person']['txtNameQTV']['#access']=FALSE;
	$form['person']['txtNameNLB']['#access']=FALSE;
	$form['person']['txtNameKS']['#access']=FALSE;
	$form['person']['txtNamePT']['#access']=FALSE;
  */
	
  
	//dpm($form);
  $form["submit"] = array(
      "#type" => "submit",
      "#value" => t("Report"),
      "#submit" => array(
          "_datalogger_report_rainfall_day_form_submit"
      ),
  ); 
  return $form;
}

function _datalogger_report_rainfall_day_form_submit(&$form, &$form_state) {
	//module_load_include('inc', 'datalogger', 'datalogger.admin.excel');
	module_load_include('inc', 'datalogger', 'datalogger.admin.excel.stations.day');
  //dpm($province);
  $region = $form['stations_fieldset']['selectArea']['#options'][$form_state['values']['selectArea']];
  //$province = $form['stations_fieldset']['selectProvince']['province']['#options'][$form_state['values']['province']];
  $province = array();
  for($sotinh = 0; $sotinh < count($form_state['values']['province']); $sotinh++){
    $tentinh = $form_state['values']['province'][0];
    //dpm($tentinh);
    array_push($province, $form['stations_fieldset']['selectProvince']['province']['#options'][$tentinh]);
  }
  //dpm($form);
  //dpm($form_state);
  $station_nids = array();
  //$station_names = array();
  $sotram = count($form_state['values']['station']);
  
  $nids = array_keys($form['stations_fieldset']['selectStation']['station']['#value']);
  
  for($i = 0; $i < $sotram; $i++){
    $station_nids[$i] = $nids[$i];
    
  }
  //dpm($station_nids);
  $day = new DateTime($form_state['values']['day']);
  $year = $day->format('Y');
  $month = $day->format('m');
  $day = $day->format('Ymd');
  
  //dpm($form);
  //dpm($form_state);
  $khang = array(
    'region' => $region,
    'province' => $province,
    'station_nids' => $station_nids,
    'day' => $day,
    'month' => $month,
    'year' => $year
  );
  
  $tramdomua = implode(",", $station_nids);
  //dpm($tramdomua);
  
	$rainfalls = views_get_view_result('view_rainfall_admin','page', $tramdomua, $day);
  //dpm($rainfalls);
  
  //_datalogger_export_rainfall_station_day_excel($region, $province, $station_nids, $year, $month, $day);
	if(count($rainfalls) <= 0){
		drupal_set_message(t('Rainfall stations in the no data to report'),'error');
	}
	else{
		// $args = array($nid, $rainfalls, $name_qtv, $name_nlb, $name_ks, $name_pt);
		//$args = array($nid, $rainfalls);
		//_export_excel($args);
    _datalogger_export_rainfall_station_day_excel($region, $province, $station_nids, $year, $month, $day);
	}
}



function datalogger_command_refresh_ajax() {

  $nid = intval(arg(4));
  $node = node_load($nid);
  echo '<div class="messages status">' . t('Updated on: !time.', array('!time' => date('d/m/Y - H:i:s', time()))) . '</div>';
  $nv = node_view($node);
  echo drupal_render($nv);
  drupal_exit();
}		
	  
/**
*begin: 09-10-12
*
*/

function _datalogger_get_station($view, $number){
	$display_id = 'default';
	$result = views_get_view_result($view, $display_id, $number);
	// $result->execute();
	return $result;
}

function test_view(){
	$number = '01634766847';
	$result = _datalogger_get_station('view_command_send', $number);
	$node_command = node_load($result[0]->nid);	
	return '';
}

function _datalogger_convert_number($number){
	$sim_number = '0'.substr($number,3);
	return $sim_number;
}

function _datalogger_check_inbox() {
  $cron = (arg(3) == 'remote');
  
  $processed = 0;
  
  $result = db_query("SELECT * FROM inbox WHERE Processed={$processed} ORDER BY `ID` DESC LIMIT 0,10");
  
  //$result = db_query("SELECT * FROM inbox WHERE Processed={$processed}  LIMIT 0,10");
  
  
  //dpm($result[0]->ID);
  
  header('Content-Type: text/plain');
  
  foreach ($result as $row) {
    // //tin nhắn từ datalogger
		$id_inbox = $row->ID;

    print "Inbox ID: {$row->ID}\n";
    
		$number = _datalogger_convert_number($row->SenderNumber);
		$sms = $row->TextDecoded;
    $smsID = $row->ID;
    if(_datalogger_startsWith($row->TextDecoded,'HS') && _datalogger_endsWith($row->TextDecoded,';')) {
      $len_sms = strlen($row->TextDecoded);
      //Xử lý tin nhắn được yêu cầu:88byte
      
      if($len_sms == 88){
        print "Command.\n";
        
				//Thực hiện chèn bản tin vào bảng command
				_datalogger_separate_sms_command($sms, $number);
				//Cập nhật các bản tin đã xử lý
				_datalogger_update_process_inbox(1,$id_inbox);
        
      }
      //Xử lý bản tin đo mưa tự động:140byte
      elseif($len_sms == 140) {
        print "Rain.\n";
      
        switch ($number) {
	
        default:
          // Chèn bản tin vào bảng đo mưa
          // dsm('into 140');
          _datalogger_separate_sms($sms,$number, $id_inbox);
          // Cập nhật các bản tin đã xử lý
          _datalogger_update_process_inbox(1, $id_inbox);
          //return $sms;
        }
      }			
			else {
        print "Other.\n";
        _datalogger_update_process_inbox(2, $id_inbox);
      }
    }
		else{
			_datalogger_update_process_inbox(2, $id_inbox);
		}
    flush (); // flush the output buffer
  }
	// return 'Ok';
  print "Ok.";
  drupal_exit();
}

function _datalogger_update_process_inbox($process, $id_inbox){
	$query="UPDATE inbox SET Processed=:process WHERE ID=:pr";
	$result = db_query($query, array(':process'=>$process, ':pr'=>$id_inbox));
}

/**
 *Cập nhật giá trị trả từ Datalogger cho node command.
 **/
function _datalogger_separate_sms_command($sms, $number){

	global $datalogger_pattern;
	$malenh = '';
	foreach ($datalogger_pattern as $pattern) {
		if (substr($sms, 25, strlen($pattern)) == $pattern) {
			$malenh = $pattern;
		}
	}
	
	//neu co ban tin canh bao luong mua
	if($malenh == 'A:ALARM INTERVAL:'){
		$stations = views_get_view_result('view_station_find', Null, $number);
		if(count($stations) >= 1){
			$station = node_load($stations[0]->nid);
			$station->field_station_alarm[$station->language][0]['value'] =  'alarm_true';	
			node_save($station);
		}
	}
	
	$malenh = str_replace(' ', '_', trim($malenh));
	
	$result = views_get_view_result('admin_command_from_phone_code', 'default', $number, $malenh);
	$nid = $result[0]->nid;
  
  
	if($nid != 0 || !empty($nid)){
		$node_command = node_load($nid);	  
  	$node_command->field_command_result["und"][0]["value"] = _datalogger_result($sms, $malenh);	
		node_save($node_command);
	}
}

/**
 *Phân tách bản tin đưa mưa tự động từ Datalogger gửi về
*/

function _datalogger_string_to_date($str){
	// $year = substr($str,0,2);
	$year = date('Y', time());
	$month = substr($str,2,2);
	$day = substr($str,4,2);
	$hour = substr($str,6,2);
	$minute = substr($str,8,2);
	$second = substr($str,10,2);
	return $day.'/'.$month.'/'.$year.' '.$hour.':'.$minute.':'.$second;
}
function _datalogger_string_to_unix_date($str){
	// $year = substr($str,0,2);
	//130106240000 -> 130107000000
	$year = date('Y', time());
	$month = substr($str, 2, 2);
	$day = substr($str, 4, 2);
	$hour = substr($str, 6, 2);
	$minute = substr($str, 8, 2);
	$second = substr($str, 10, 2);
	return mktime($hour,$minute,$second,$month,$day,$year);
}

function _datalogger_separate_sms($sms,$number, $smsID) {
  list($code, $timenow, $timestart, $arg1, $arg2, $arg3, $arg4, $arg5, $arg6, $chk) = explode(",", $sms);
	$id = substr($code,2,6);
	$is_valid = array();
	$measuaring = array();
	$accumulate = array();
	$battery = array();
	$arg = array($arg1, $arg2, $arg3, $arg4, $arg5, $arg6);
	
  $total_rain_hour = 0;
	for($i = 0; $i < 6; $i++){
		$opp = _datalogger_separate_opp($arg[$i]);
		$is_valid[] = $opp[0];
		$measuaring[] = $opp[1];
		$accumulate[] = $opp[2];
		$battery[] = $opp[3];
    $total_rain_hour += intval($opp[1]);
	}

	$station = _datalogger_get_station('view_station_find', $number);
	$nid = $station[0]->nid;
	$title = $station[0]->node_title;
	$arg = array($timestart, $nid, $is_valid, $measuaring, $accumulate, $battery, $smsID, $total_rain_hour);

	_datalogger_insert_node_rainfall($arg);
  
}

function _datalogger_separate_opp($sms){
  $arg = array(substr($sms,0,1), substr($sms,1,5), substr($sms,6,5), substr($sms,11,5));
	return $arg;
}



function _datalogger_return_str($sms, $pa){
	$pos =  strpos($sms, $pa[$i]);
	return trim(substr($sms,$pos,-3));	
}



function _datalogger_insert_node_rainfall(&$arg){
	global $clearTime3;
	$timestam = $arg[0];
	$nid = $arg[1];
	$station = node_load($nid);
  $smsID = $arg[6];
  $total_rain_hour = $arg[7];
  
  
  
	if( $nid != null || !empty($nid ) && (!empty($station) || $station != null)) {
		$time_unix = _datalogger_string_to_unix_date($timestam);
		$is_data_valids = $arg[2];
		$measuring_interval = $arg[3];//3
		$accumulate;//4
		$battery = $arg[5];//5
		
    
    for($i = 5; $i >= 0; $i--) {
    
			$is_data_valid = $is_data_valids[$i];
			$time = $time_unix - 60*10*$i;
			$send_time = date('dmYHis',$time);
			$hour_time = date('H:i:s',$time);
			$year = date('Y',$time);
			$year_month = date('Ym',$time);
			$year_month_date = date('Ymd',$time);
      
			if($is_data_valid == 0){
				$power = $battery[$i];
        
        // trung temp
        
				$rainfalls = views_get_view_result('view_find_rainfall_station', NULL, $nid, $send_time);
        
        //print('$send_time='.$send_time);
        
        $rainfalls = array();
        
				
				if(count($rainfalls) > 0 ){         // trung du lieu thi cap nhat lai cai cu theo gia tri moi
        
					foreach($rainfalls as $rainfall_old){
						$rainfall_old = node_load($rainfall_old->nid);
						//lượng mưa đo được trong 10p
						$rainfall_old->field_rainfall_measuring['und'][0]['value'] = $measuring_interval[$i]/10;
						// luong mua tich luy
						$rainfall_old->field_rainfall_total['und'][0]['value'] = $arg[4][$i]/10;
            //lượng mưa đo được trong 60p            
            $rainfall_old->field_rainfall_total_hour['und'][0]['value'] = $total_rain_hour / 10;
            
						
						//chen thoi gian chan vao node
						for($j = 0; $j < 24; $j++){
							if($clearTime3['h'.$j] == $hour_time){
								$rainfall->field_rainfall_hour_time[$rainfall->language][0]['value'] = $hour_time;
							}
						}
						
						node_save($rainfall_old);
				
					}
				}

				else{
					//tao node moi
					$rainfall = new stdClass();
					$rainfall->title = date('d/m/Y H:i:s',$time);
					$rainfall->type = "rainfall";
					$rainfall->created = time();
					$rainfall->changed = $rainfall->created;
					$rainfall->status = 1; // Published?
					$rainfall->promote = 0; // Display on front page?
					$rainfall->sticky = 0; // Display top of page?
					$rainfall->format = 1; // Filtered HTML?
					$rainfall->uid = 1; // Content owner uid (author)?
					$rainfall->language = 'und';

					node_object_prepare($rainfall); 
					$rainfall->field_rainfall_station[$rainfall->language][0]['nid'] = $nid;
					//lượng mưa đo được trong 10p
					$rainfall->field_rainfall_measuring[$rainfall->language][0]['value'] = $measuring_interval[$i]/10;
					//lượng mưa tich luy
					$rainfall->field_rainfall_total[$rainfall->language][0]['value'] = $arg[4][$i]/10;
          // luong mua tong trong 1 gio
          $rainfall->field_rainfall_total_hour['und'][0]['value'] = $total_rain_hour / 10;
          
					$rainfall->field_rainfall_send_time[$rainfall->language][0]['value'] = $time;
					$rainfall->field_rainfall_time_send_sms[$rainfall->language][0]['value'] = $send_time;
					$rainfall->field_rainfall_year[$rainfall->language][0]['value'] = $year;
					$rainfall->field_rainfall_year_month[$rainfall->language][0]['value'] = $year_month;
					$rainfall->field_rainfall_year_month_date[$rainfall->language][0]['value'] = $year_month_date;
          
          $rainfall->field_rainfall_sms_id[$rainfall->language][0]['value'] = $smsID;
          $rainfall->field_rainfall_op_idx[$rainfall->language][0]['value'] = ($i + 1);
					
					//chỉ trèn giá trị vào trường này khi thời gian là: 01:00:00, 02:00:00,...23:00:00h
					for($j = 0; $j < 24; $j++){
						if($clearTime3['h'.$j] == $hour_time){
							$rainfall->field_rainfall_hour_time[$rainfall->language][0]['value'] = $hour_time;
						}
					}
					
					$rainfall = node_submit($rainfall);
          
					node_save($rainfall);
					// dsm($rainfall);
				}
			}
		}
		$station->field_station_power['und'][0]['value'] = $power/10;
		$station->field_station_rain_nearest['und'][0]['value'] = $measuring_interval[0]/10;
		$station->field_station_rain_24['und'][0]['value'] = $arg[4][0]/10;
		
		//tinh trang dang hoat dong_nai
		$station->field_station_status['und'][0]['value'] = 'station_active';		
		
		//trang thai: co mua
		if($measuring_interval[0]/10 > 0){
			//canh bao co mua - luong nho
			$station->field_station_alarm['und'][0]['value'] = 'alarm_rainfall';	
		}
		else{
			//khong co mua - kho han
			$station->field_station_alarm['und'][0]['value'] = 'alarm_false';	
		}		
		node_save($station);
	}
	
}

function _datalogger_generate_rainfalls_form($form, &$form_state) {
  $form['station'] = array(
    '#type' => 'textfield',
    '#title' => 'Trạm Nid',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Sinh mưa',
  );
  return $form;
}

function _datalogger_generate_rainfalls_form_submit($form, &$form_state) {

  
  for($i = 1; $i <= 24; $i ++) {
    
    $station_nid = $form_state['values']['station'];
    
    
    
    $is_valid = 1;
  
    for($i = 0; $i < 6; $i++) {
      $accumulated[$i] = 0;
    }
    
    for($i = 0; $i < 6; $i++) {
      $measuaring[$i] = floatval(rand(6, 9)) / 10.0;
      $accumulated[$i] += $measuaring[$i];
      $battery[$i] = floatval(rand(124, 138)) / 10.0;
    }
    
    $arg = array('Lượng mưa ', $station_nid, $is_valid, $measuaring, $accumulated, $battery, $smsID);
    
    _datalogger_insert_node_rainfall($arg);
    
  }
  drupal_set_message('Tạo node thành công.');
  
}



/**
*begin: 03-10-12
*
*/

function _datalogger_startsWith($sms, $needle)
{
	return substr($sms, 0, 2) == $needle;
}
function _datalogger_endsWith($sms, $needle)
{
	return substr($sms, -strlen($needle)) == $needle;
}

/*Ham xu ly cac tin nhắn gửi tới*/
function _datalogger_new_sms_form() {
  // Initialising output
  $output = '';
 
  // Table header
  $header = array(
    array('data' => t('No.')),		
    array('data' => t('Station Name'), 'field' => 'StationName', 'sort' => 'ASC'),
    array('data' => t('Receiving Date Time'), 'field' => 'UpdatedInDB', 'sort' => 'ASC'),
    array('data' => t('Sender Number'), 'field' => 'SenderNumber', 'sort' => 'ASC'),
    array('data' => t('Message content')),
		array('data' => t('Modem'), 'field' => 'RecipientID', 'sort' => 'ASC'),
    array('data' => t('Processed'), 'field' => 'Processed', 'sort' => 'ASC'),
  );
 
  // Setting the sort conditions
  if(isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    $order = 'UpdatedInDB';
    
    // Which column will be sorted
    foreach ($header as $h) {
      if ($h['data'] == $_GET['order'] && isset($h['field'])) {
        $order = $h['field'];
        break;
      }
    }
  } else {
    $sort = 'ASC';
    $order = 'UpdatedInDB';
  }
 
  global $databases;
  
  $other_database = $databases['default']['default'];
  $other_database['prefix'] = '';
  
  // replace 'YourDatabaseKey' with something that's unique to your module
  Database::addConnectionInfo('YourDatabaseKey', 'default', $other_database);
  db_set_active('YourDatabaseKey');
  
  // Query object
  $query = db_select("datalogger_node", "n");
  $query->innerJoin("datalogger_field_data_field_station_sim_number", "s", "n.nid = s.entity_id");
  $query->innerJoin("inbox", "i", "CONCAT('0', substr(i.SenderNumber, 4)) = s.field_station_sim_number_value");
	
  // Adding fields
  $query->addField('n', 'title', 'StationName');
  $query->fields('i', array('UpdatedInDB','SenderNumber', 'RecipientID', 'TextDecoded', 'Processed'));
 
  // Set order by
  $query->orderBy($order, $sort);
 
  // Pagination
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(50);
	
  // Executing query
  $result = $query->execute();
  
  db_set_active(); // without the paramater means set back to the default for the site
  
  // Looping for filling the table rows
  while($row = $result->fetchObject()) {
  
    $sms = $row -> TextDecoded;
    $len_sms = strlen($row->TextDecoded);

    // highlight
    //Xử lý tin nhắn được yêu cầu:88byte
    if($len_sms == 88) {
      $sms = '<span style="background:blue;color:white;font-weight:bold">' . $sms . '</span>';
    }
    //Xử lý bản tin đo mưa tự động:140byte
    elseif($len_sms == 140) {
      $sms = _datalogger_regex_rain($sms);      
    }
		
    // Adding the rows
    //$rows[] = array($data->title, $data->type);
    $rows[] = array (
			$i+=1,
			$row -> StationName,
			$row -> UpdatedInDB,
			$row -> SenderNumber,
			$sms,
			$row -> RecipientID,
      $row -> Processed ? t('Yes') : t('No'),
			);
  }
	
  // Setting the output of the field
  $output = theme('pager').theme_table(
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(),
      'sticky' => true,
      'caption' => '',
      'colgroups' => array(),
      'empty' => t("Table has no row!") // The message to be displayed if table is empty
    )
  ).theme('pager');
 
  // Returning the output
  return $output;
}


function _datalogger_new_outbox_form (){
  // Initialising output
  $output = '';
 
  // Table header
  $header = array(
    array('data' => t('No.')),		
    array('data' => t('Station Name'), 'field' => 'StationName', 'sort' => 'ASC'),
    array('data' => t('Sending Date Time'), 'field' => 'UpdatedInDB', 'sort' => 'ASC'),
    array('data' => t('Delivery Date Time'), 'field' => 'SendingDateTime', 'sort' => 'ASC'),
    array('data' => t('Sending Time out'), 'field' => 'SendingTimeout', 'sort' => 'ASC'),
		array('data' => t('Destination Number'), 'field' => 'DestinationNumber', 'sort' => 'ASC'),
    array('data' => t('Message content')),
    array('data' => t('Modem'), 'field' => 'SenderID', 'sort' => 'ASC'),
  );
 
  // Setting the sort conditions
  if(isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if($_GET['sort'] == 'asc')
      $sort = 'ASC';
    else
      $sort = 'DESC';

    $order = 'UpdatedInDB';
    
    // Which column will be sorted
    foreach ($header as $h) {
      if ($h['data'] == $_GET['order'] && isset($h['field'])) {
        $order = $h['field'];
        break;
      }
    }
  } else {
    $sort = 'ASC';
    $order = 'UpdatedInDB';
  }
 
  global $databases;
  
  $other_database = $databases['default']['default'];
  $other_database['prefix'] = '';
  
  // replace 'YourDatabaseKey' with something that's unique to your module
  Database::addConnectionInfo('YourDatabaseKey', 'default', $other_database);
  db_set_active('YourDatabaseKey');
  
  // Query object
  $query = db_select("datalogger_node", "n");
  $query->innerJoin("datalogger_field_data_field_station_sim_number", "s", "n.nid = s.entity_id");
  $query->innerJoin("outbox", "o", "CONCAT('0', substr(o.DestinationNumber, 4)) = s.field_station_sim_number_value");
	
  // Adding fields
  $query->addField('n', 'title', 'StationName');
  $query->fields('o', array('UpdatedInDB', 'SendingDateTime', 'SendingTimeout', 'DestinationNumber', 'TextDecoded', 'SenderID'));
 
  // Set order by
  $query->orderBy($order, $sort);
 
  // Pagination
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(50);
	
  // Executing query
  $result = $query->execute();
  
  db_set_active(); // without the paramater means set back to the default for the site
  
  // Looping for filling the table rows
  while($row = $result->fetchObject()) {
  
    $sms = $row -> TextDecoded;
    $len_sms = strlen($row->TextDecoded);

    // highlight    
    //Xử lý tin nhắn được yêu cầu:88byte
    if($len_sms == 88) {
      $sms = '<span style="background:blue;color:white;font-weight:bold">' . $sms . '</span>';
    }
    //Xử lý bản tin đo mưa tự động:140byte
    elseif($len_sms == 140) {
      $sms = _datalogger_regex_rain($sms);      
    }
		
    // Adding the rows
    $rows[] = array (
			$i+=1,
			$row -> StationName,
			$row -> UpdatedInDB,
			$row -> SendingDateTime,
			$row -> SendingTimeout,
			$row -> DestinationNumber,
			$sms,
      $row -> SenderID,
			);
  }
	
  // Setting the output of the field
  $output = theme('pager').theme_table(
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(),
      'sticky' => true,
      'caption' => t('Date').': '.date('d/m/Y',time()),
      'colgroups' => array(),
      'empty' => t("Table has no row!") // The message to be displayed if table is empty
    )
  ).theme('pager');
 
  // Returning the output
  return $output;
}


function _datalogger_new_sentitems_form(){

	$page = arg(3);
	$headers = array(
    no => t('No.'),
		station_name => t('Station Name'),
    receiving_date_time => t('Sending Date Time'),
    delivery_date_time =>t('Delivery Date Time'),
    destination_number => t('Destination Number'),
    message_content => t('Message content'),
		Modem => t('Modem'),
    status => t('Status'),
  );
  $num_per_page = 50;
  $offset=$num_per_page*$page;
	$total = db_query("SELECT COUNT(*) total FROM `sentitems` ORDER BY  `sentitems`.`ID` DESC LIMIT 0,500");
	$number_rows = $total->fetch()->total;
	$last = $number_rows/$num_per_page;
	if(!is_int($last)){
		$last = (int)$last+1;
	}
	
  $result = db_query("SELECT * FROM `sentitems` ORDER BY  `sentitems`.`ID` DESC LIMIT ". $offset.",".$num_per_page."");
	
  /*$query = "SELECT * FROM `sentitems` ORDER BY  `sentitems`.`ID` DESC";
  $result = pager_query(db_rewrite_sql($query));*/
  
  
  $rows = array();
  while($row = $result->fetchObject()){
		$number = $row->SenderNumber;
		$station = views_get_view_result('view_station_find', Null, $number );
		if(!empty($station)){
			$station_name = $station[0]->node_title;
		}
		else{
			$station_name = null;	
		}
    
    $sms = $row -> TextDecoded;
    $len_sms = strlen($row->TextDecoded);

    // highlight
    
    //X? lý tin nh?n du?c yêu c?u:88byte
    if($len_sms == 88) {
      $sms = '<span style="color:blue;font-weight:bold">' . $sms . '</span>';
    }
    
    elseif($len_sms == 140) {
      $sms = _datalogger_regex_rain($sms);      
    }			
    
    $rows[] = array (
			$offset+=1,
			$station_name,
			//$row -> UpdatedInDB,
      $row -> SendingDateTime,
      $rorw -> DeliveryDateTime,
			$number,
			$sms,
			$row -> RecipientID,
      $row -> Status,
			);

  }
	
  /*$retval = theme('table', array(
                'header' => $headers,
                'rows' => $rows
                 ));*/
                 
     
  
     
	$output = theme_table(
    array(
      "header" => $headers,
      "rows" => $rows,
      "attributes" => array(),
      "sticky" => true, // Table header will be sticky
      "caption" => t('Date').': '.date('d/m/Y',time()),
      "colgroups" => array(),
      "empty" => t("Table has no row!") // The message to be displayed if table is empty
    )
  ).theme("pager");
  //).theme('pager', array('quantity' => $total));
	
  $prevpage = $page - 1;
  $nextpage = $page + 1;
  if($page > 0){
    $output = $output . l(t("<< first"),"gammu/sentintems/new");
    $output = $output . l(t("< previous"),"gammu/sentintems/new/".$prevpage). "  ";
  }
	if($number_rows > $num_per_page){
		if($nextpage < $last){
			$output = $output . l(t("next >"),"gammu/sentintems/new/".$nextpage);
			$output = $output . l(t("last >>"),"gammu/sentintems/new/".($last - 1));
		}
	}
  return $output;
}


function _datalogger_regex_rain($sms) {

  //$sms = 'HS092257,121207230000,121207230000,0 0 0 130,0 0 0 130,0 0 0 130,0 0 0 130,0 0 0 130,0 0 0 130,74;';

  $pattern = '/HS(\d\d\d\d\d\d),(\d\d)(\d\d)(\d\d)(\d\d)(\d\d)(\d\d),(\d\d)(\d\d)(\d\d)(\d\d)(\d\d)(\d\d),'
    . '(\d* *\d* *\d*) *(\d\d)(\d),'
    . '(\d* *\d* *\d*) *(\d\d)(\d),'
    . '(\d* *\d* *\d*) *(\d\d)(\d),'
    . '(\d* *\d* *\d*) *(\d\d)(\d),'
    . '(\d* *\d* *\d*) *(\d\d)(\d),'
    . '(\d* *\d* *\d*) *(\d\d)(\d),'
    . '(..);/';
  
  
  $replacement = '<span style="color:#fff">HS</span><em>${1}</em>,'
    . '<span style="color:red">${2}</span><span style="color:green">${3}</span><span style="color:blue">${4}</span>'
    . '<span style="background:#eee;text-decoration:underline;">'
      . '<span style="color:red">${5}</span><span style="color:green">${6}</span><span style="color:blue">${7}</span>'
    . '</span>'
    . ','
    . '<span style="color:red">${8}</span><span style="color:green">${9}</span><span style="color:blue">${10}</span>'
    . '<span style="background:#eee;text-decoration:underline;">'
      . '<span style="color:red">${11}</span><span style="color:green">${12}</span><span style="color:blue">${13}</span>'
    . '</span>'
  ;
  $n = 13;
  for ($i=1; $i<=6;$i++) {
    $replacement .= 
      ','
    . '<span style="background:#fff"><em>'
    . '<span style="color:blue">$' . ($n+1) . '</span> <span style="color:red">$' . ($n+2) . '</span><span style="color:#ffbbbb">$' . ($n+3) . '</span>'
    . '</em></span>'
    ;
    $n+=3;
  }
  $replacement .= ',<span style="color:#fff">$32</span>;';
  
  return preg_replace($pattern, $replacement, $sms);
}





/**
*begin: 11-09-12
*/

function _user_form(){
  $form['u_nameLogin'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên đăng nhập: '),
  );
  
  $form['u_lastName'] = array(
    '#type' => 'textfield',
    '#title' => t('Họ: '),
  );
  
  $form['u_firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên: '),
  );
  
   $form['u_nameDisplay'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên hiển thị: '),
  );
  
  $form['u_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Địa chỉ Email: '),
  );
  
  $form['u_createDate'] = array(
    '#type' => 'item',
    '#title' => t('Ngày được tạo: '.date('d/m/Y h:m:s')),
  );
  
  $form['u_loginDate'] = array(
    '#type' => 'item',
    '#title' => t('Ngày đăng nhập gần nhất: '.date('d/m/Y h:m:s')),
  );
  
  $form['u_actionDate'] = array(
    '#type' => 'item',
    '#title' => t('Ngày hoạt động gần nhất: '.date('d/m/Y h:m:s')),
  );
  
  $form['u_changePassDate'] = array(
    '#type' => 'item',
    '#title' => t('Thời điểm thay đổi password: '.date('d/m/Y h:m:s')),
  );
  
  $form['u_lockDate'] = array(
    '#type' => 'item',
    '#title' => t('Ngày bị khóa gần nhất:never'),
  );
  
  $form['u_isOnl'] = array(
    '#type' => 'item',
    '#title' => t('Người dùng đang online: true'),
  );
  
  $form['u_isLock'] = array(
    '#type' => 'item',
    '#title' => t('Bị khóa:false'),
  );
  
  $form['u_isAccept'] = array(
    '#type' => 'item',
    '#title' => t('Chấp nhận:false'),
  );
  
  $form['u_updatePass'] = array(
    '#type' => 'item',
    '#title' => t('Cập nhật password:false'),
  );
  
  $form['u_delete'] = array(
    '#type' => 'item',
    '#title' => t('Xóa bỏ:false'),
  );
  
  $form['sub_add'] = array(
     '#type' => 'submit',
     '#value' => t('Cập nhật'),    
  );
  
  $form['sub_cancel'] = array(
     '#type' => 'submit',
     '#value' => t('Xóa'),    
  );
  
  
  return $form;  
}

function _user_pass_form(){
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Quản lý password'),
  );
  
  $form['submitExportData'] = array(
     '#type' => 'submit',
     '#value' => t('Cập nhật'),    
  );
  
  return $form;  
}

function _user_profile_form(){
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Quản lý profile'),
  );
  
  $form['submitExportData'] = array(
     '#type' => 'submit',
     '#value' => t('Cập nhật'),    
  );
  
  return $form;  
}

function _user_group_form(){
  $form['u_gr_power'] = array(
    '#type' => 'select',
    '#title' => t('Quyền hạn: '),
    '#options' => array(
      1 => 'Admin',
      2 => 'Register user',
      3 => 'Quản lý', 
    ),
  );
  
  $form['u_pr_eff'] = array(
    '#type' => 'date_popup',
    '#title' => t('Ngày áp dụng: '),
  );
  
  $form['u_pr_exp'] = array(
    '#type' => 'date_popup',
    '#title' => t('Ngày hết hiệu lực: '),
  );
  
  $form['u_gr_add'] = array(
    '#type' => 'submit',
    '#value' => t('Thêm quyền hạn: '),
  );
  
  return $form;  
}

function _user_add_form(){
  drupal_set_title('Quản lý thông tin người dùng');
  $form['u_nameLogin'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên đăng nhập: '),
  );
  
  $form['u_lastName'] = array(
    '#type' => 'textfield',
    '#title' => t('Họ: '),
  );
  
  $form['u_firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên: '),
  );
  
  $form['u_nameDisplay'] = array(
    '#type' => 'textfield',
    '#title' => t('Tên hiển thị: '),
  );
  
  $form['u_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Địa chỉ Email: '),
  );
  
  $form['u_accept'] = array(
    '#type' => 'checkbox',
    '#title' => t('Chấp nhận: '),
  );   
  
  $form['u_notice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Thông báo: '),
  );
  
  $form['u_item'] = array(
    '#type' => 'item',
    '#title' => t('Tùy chọn nhập một password cho thành viên này, hoặc cho phép hệ thống tạo ra một password ngẫu nhiên: '),
  );
  
  $form['u_rand_pass'] = array(
    '#type' => 'checkbox',
    '#title' => t('Password ngẫu nhiên: '),
  );    
  
  $form['u_pass_confirm'] = array(
    '#type' => 'password_confirm',
  );
  
  $form['sub_add'] = array(
     '#type' => 'submit',
     '#value' => t('Tạo người dùng'),    
  );
  
  $form['sub_cancel'] = array(
     '#type' => 'submit',
     '#value' => t('Hủy bỏ'),    
  );
  return $form;  
}


function _datalogger_report_rainfall_month_submit(&$form, &$form_state) {
	module_load_include('inc', 'datalogger', 'datalogger.admin.excel');
	$nid = $form_state['values']['station'];
	$year = $form_state['values']['year'];
	$month = $form_state['values']['month'];
	if($month < 10){
		$month = '0'.$month;
	}
	$year_month = $year.$month;
	$name_qtv = $form_state['values']['txtNameQTV'];
	$name_nlb = $form_state['values']['txtNameNLB'];
	$name_ks = $form_state['values']['txtNameKS'];
	$name_pt = $form_state['values']['txtNamePT'];
	
	$rainfalls = views_get_view_result('view_export_excel','default', $nid, $year, $year_month, 'all');
	if(count($rainfalls) <= 0){
		drupal_set_message(t('Rainfall stations in the no data to report'),'error');
	}
	else{
		$args = array($nid, $rainfalls, $name_qtv, $name_nlb, $name_ks, $name_pt,$year. $month);
		_export_excel($args);
	}
}


function _init_rainfall_node($title, $time, $rainfall, $stationCode) {
  $node = new stdClass();
  $node->title = $title;
  $node->type = "rainfall";
  $node->created = time();
  $node->changed = $node->created;
  $node->status = 1; // Published?
  $node->promote = 0; // Display on front page?
  $node->sticky = 0; // Display top of page?
  $node->format = 1; // Filtered HTML?
  $node->uid = 1; // Content owner uid (author)?
  $node->language = "und";

  node_object_prepare($node);
  $node->field_rainfall_obsrp[$node->language][0]["value"] = $time;
  $node->field_rainfall_rp[$node->language][0]["value"] = $rainfall;
  $node->field_station_code[$node->language][0]["value"] = $stationCode;  

  $node = node_submit($node);
  node_save($node);
}


/**
*begin: 10-09-12
*/

// function _datalogger_menu_access_callback($permission, $node) {
  // //dpm($permission);
  // //dpm($node);
  
  // return user_access($permission) && $node->type == 'station';
// }

// function _datalogger_station_title_callback($node) {
  // if ($node->type == 'station') {
    // return 'Thông tin trạm';
  // }
  // else {
    // return 'View';
  // }
// }

/**
*begin: 09-09-12
*/

function _manage_data(){
    return 'Quản lý dữ liệu';
}

function _manage_statistics_form() {
  $nid = intval(arg(1));
  $node = node_load($nid);
  drupal_set_title('Trạm đo mưa: '.$node->title);
  $form['tieude'] = array(
    '#type' => 'item',
    '#title' => t('Thống kê khối lượng mưa ngày: ').date('d:m:Y'),
  );
  $form['ao'] = array(
    '#type' => 'item',
    '#title' => t('Tổng lượng mưa quan trắc gần nhất:0.4mm'),
  );$form['bo'] = array(
    '#type' => 'item',
    '#title' => t('Tổng lượng mưa từ 19h đến thời điểm hiện tại:1.7mm'),
  );
  $nid = intval(arg(1));
  $node = node_load($nid);
  drupal_set_title('Trạm đo mưa: '.$node->title);
  $headers = array(
    1 => 'Thời điểm',
    2 => 'ObsRp (phút)',
    3 => 'Rp (mm)',
  );
  
  $rows = array(
    array('18/08/2012 16:16:00','1','0,8'),    
    array('18/08/2012 16:17:00','1','0,8'),    
    array('18/08/2012 16:18:00','1','0,8'),    
    array('18/08/2012 16:19:00','1','0,8'),    
    array('18/08/2012 16:20:00','1','0,8'),    
    array('18/08/2012 16:21:00','1','0,8'),     
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('Hiện tại không có thông tin trong ngay'),
  );
  
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Tổng lượng mưa: 1,7mm'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Xuất dữ liệu'),
  );
  return $form;
}
function _manage_chart_form() {
  $nid = intval(arg(1));
  $node = node_load($nid);
  drupal_set_title('Trạm đo mưa: '.$node->title);
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Biểu đồ lượng mưa theo giờ'),
  );
  $nid = intval(arg(1));
  $node = node_load($nid);
  drupal_set_title('Trạm đo mưa: '.$node->title);
  $headers = array(
    1 => 'Thời điểm',
    2 => 'ObsRp (phút)',
    3 => 'Rp (mm)',
  );
  
  $rows = array(
    array('18/08/2012 16:16:00','1','0,8'),    
    array('18/08/2012 16:17:00','1','0,8'),    
    array('18/08/2012 16:18:00','1','0,8'),    
    array('18/08/2012 16:19:00','1','0,8'),    
    array('18/08/2012 16:20:00','1','0,8'),    
    array('18/08/2012 16:21:00','1','0,8'),    
    array('18/08/2012 16:22:00','1','0,8'),    
    array('18/08/2012 16:23:00','1','0,8'),    
    array('18/08/2012 16:24:00','1','0,8'),    
    array('18/08/2012 16:25:00','1','0,8'),    
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('Hiện tại không có thông tin trong ngay'),
  );
  
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Tổng lượng mưa: 1,7mm'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Xuất dữ liệu'),
  );
  return $form;
}
function _manage_rainfall_form($form, &$form_state) {
  // $node = node_load($nid);
  $nid = intval(arg(1));
  $node = node_load($nid);
  drupal_set_title('Trạm đo mưa: '.$node->title);
  $headers = array(
    1 => 'Thời điểm',
    2 => 'ObsRp (phút)',
    3 => 'Rp (mm)',
  );
  
  $rows = array(
    array('18/08/2012 16:16:00','1','0,8'),    
    array('18/08/2012 16:17:00','1','0,8'),    
    array('18/08/2012 16:18:00','1','0,8'),    
    array('18/08/2012 16:19:00','1','0,8'),      
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('Hiện tại không có thông tin trong ngay'),
  );
  
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Tổng lượng mưa: 1,7mm'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Xuất dữ liệu'),
  );
  return $form;
}

function _manage_info_form() {
  // $node = node_load($nid);
  $nid = intval(arg(1));
  $node = node_load($nid);
  drupal_set_title('Trạm đo mưa: '.$node->title);
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Thông tin tram'),
  );
  
  return $form;
}

/**
*begin: 08 - 08 - 2012
*/

function _datalogger_init_node($title, $field_station_code,
    $field_station_sim_number, $field_station_address, $field_station_longitude,
    $field_station_latitude, $field_station_password,
    $field_station_transmission_speed, $field_station_power,
    $field_station_diagnostic_error) {
  $node = new stdClass();
  $node->title = $title;
  $node->type = "station";
  $node->created = time();
  $node->changed = $node->created;
  $node->status = 1; // Published?
  $node->promote = 0; // Display on front page?
  $node->sticky = 0; // Display top of page?
  $node->format = 1; // Filtered HTML?
  $node->uid = 1; // Content owner uid (author)?
  $node->language = "und";

  node_object_prepare($node);
  $node->field_station_code[$node->language][0]["value"] = $field_station_code;
  $node->field_station_address[$node->language][0]["value"] = $field_station_address;
  $node->field_station_diagnostic_error[$node->language][0]["value"] = $field_station_diagnostic_error;
  $node->field_station_latitude[$node->language][0]["value"] = $field_station_latitude;
  $node->field_station_longitude[$node->language][0]["value"] = $field_station_longitude;
  $node->field_station_password[$node->language][0]["value"] = $field_station_password;
  $node->field_station_power[$node->language][0]["value"] = $field_station_power;
  $node->field_station_sim_number[$node->language][0]["value"] = $field_station_sim_number;
  $node->field_station_transmission_speed[$node->language][0]["value"] = $field_station_transmission_speed;

  $node = node_submit($node);
  node_save($node);
}

function _log_init_node($title, $event) {
  $node = new stdClass();
  $node->title = $title;
  $node->type = "log";
  $node->created = time();
  $node->changed = $node->created;
  $node->status = 1; // Published?
  $node->promote = 0; // Display on front page?
  $node->sticky = 0; // Display top of page?
  $node->format = 1; // Filtered HTML?
  $node->uid = 1; // Content owner uid (author)?
  $node->language = "und";

  node_object_prepare($node);
  // $node->field_[$node->language][0]["value"] = $body;
  $node->field_event_type[$node->language][0]["value"] = $event;

  $node = node_submit($node);
  node_save($node);

}

function _datalogger_rainfalls() {
  // for ($i = 1; $i < 227; $i ++) {
  // _datalogger_init_node("Station - " . $i, "Code - " . $i, "Address - " . $i,
  // 1, $i, $i,
  // "Password" . $i, $i, "0168123456" . (78 + $i), $i);
  // }
  return "Finished Create Stations.";

}

function _log_rainfalls() {
  // for ($i = 1; $i < 10; $i ++) {
  // // _log_init_node("Log - ".$i,"Description - ".$i,"Event type - ".$i);
  // _log_init_node("Log - ".$i,"Set Auto Power Management Nable");
  // }
  return "Finished Create Logs.";

}



function _view_translate_station() {
  return digipro_views_get_view_result("arg_all_datalogger", "page", null,
      true, false);
}

function _view_translate_log() {
  return digipro_views_get_view_result("logs_management", null, null, true,
      false);

}

function digipro_views_get_view_result($viewname, $display_id = NULL, $args = NULL,
  $content_only = false, $data_only = false) {
  $view = views_get_view($viewname);
  if(is_object($view)) {
    if(is_array($args)) {
      $view->set_arguments($args);
    }

    if(is_string($display_id)) {
      $view->set_display($display_id);
    } else {
      $view->init_display();
    }
    $view->pre_execute();
    $view->execute();
    global $language_content;
    switch ($language_content->language) {
      case "en":
        switch ($view->name) {
          case "logs_management":
            $view->field['title']->options['label'] = 'Name Log';
            $view->field['field_event_type']->options['label'] = 'Event Type';
            $view->field['created']->options['label'] = 'Post Date';
            $view->field['counter']->options['label'] = 'No.';
            break;
          case "arg_all_datalogger":
            $view->field['views_bulk_operations']->options['label'] = 'Select';
            $view->field['title']->options['label'] = 'Station Name';
            $view->field['counter']->options['label'] = 'No.';
            $view->field['nid']->options['label'] = 'Send Command';
            break;
        }
        break;
      case "vi":
        break;
    }

    if($data_only)
      return $view;
    $count = $view->total_rows;

    if($content_only)
      return $view->preview();
    else
      return array(
          'count' => $count,
          'content' => $view->preview()
      );
  } else {
    return t('View %viewname not found.', array(
        '%viewname' => $viewname
    ));
  }
}

