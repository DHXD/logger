<?php

global $timeInterval;
$timeInterval = array('10', '20', '30', '60', '120', '180', '360');

function _command_form($form, &$form_state) {
  global $user;
  
  $module_path = drupal_get_path('module', 'datalogger');
  
  drupal_add_js($module_path . '/datalogger_form_command.js');

  
	$options_commands = array(
    //Data call command
    1 => t('Real Data Call'),
    2 => t('Save Data Call'),
    //Date and Time Set
    3 => t('Date $ Time Set'),
    4 => t('GPRS Network Time Synchronization'),
    //ID $ Password Set
    5 => t('Set ID'),
    6 => t('Get ID'),
    7 => t('Set Password'),
    8 => t('Get Password'),
    //database Buffer Clear
    9 => t('All Database Buffer Clear'),
    10 => t('Specific Date Database Buffer Clear'),
    //Rain fall config
    11 => t('Accumulated Rain Fall Value Set'),
    12 => t('Set Rain Fall Clear Time'),
    13 => t('Get Rain Fall Clear Time'),
    //Datalogger config
   // 14 => t('Self Power Reset'),
    15 => t('Set Active / De-Active Battery Power Voltage'),
    16 => t('Get Active / De-Active Battery Power Voltage'),
    17 => t('Set Battery Voltage Offset'),
    18 => t('Get Battery Voltage Offset'),
    19 => t('Set Auto Power Management Enable'),
    20 => t('Set Auto Power Management Disable'),
    21 => t('Get Auto Power Management Enable / Disable'),
    22 => t('Go To Sleep Mode'),
    //Communication setting
    23 => t('Set Data Send Interval Minutes'),
    24 => t('Get Data Send Interval Minutes'),
    25 => t('Set Communication Type GSM'),
    26 => t('Set Communication Type GPRS'),
    27 => t('Set Communication Type Auto'),
    28 => t('Get Communication Type'),
    //GPRS network setting
    29 => t('Set GPRS Network Setting'),
    30 => t('Get GPRS Network Setting'),
    31 => t('Set Host Network Infomation'),
    32 => t('Get Host Network Infomation'),
    33 => t('Set Host Phone Number'),
    34 => t('Get Host Phone Number'),
    //Get datalogger firmware version
    35 => t('Get Datalogger Firmware Version'),
		//Alarm Options
		36 => t('Set the Alarm Interval and Level for rainfall alarming'),
		37 => t('Get the Alarm Interval and Level for rainfall alarming'),
		38 => t('Reset All'),);	
    
  if (arg(1) == '0') {
    
    $div_class = 'control-datalogger';
    
    _datalogger_select_area_province_station($form, $form_state, $div_class);
    
    unset ($form['stations_fieldset']['selectReport']);
    unset ($form['stations_fieldset']['day']);
    unset ($form['stations_fieldset']['month']);
    unset ($form['stations_fieldset']['year']); 
    unset ($form['person']);//['#access']=FALSE;    
    unset ($form['notice_excel_2007']);
    
  }
  //dpm($form);
  
  if ($user->uid != 1)  {
    unset($options_commands[19]);   // Set Auto Power Management Enable disabled for not root
    unset($options_commands[22]);   // Go to Sleep mode
  }

	//cập nhật trạng thái lênh chưa được gửi về dạng đã được gửi để thực hiện gửi lệnh mới.
	if(count(arg()) == 4){
		if(arg(2) == 'editsentcommand'){
			db_query("UPDATE `sentitems` SET Status = 'SendingOKNoReport' WHERE ID=:id",array(':id' => arg(3)));
		}		
	}
	
	if(count(arg()) == 2){
		$nid = intval(arg(1));
		$node = node_load($nid);
		drupal_set_title(t('Rainfall stations: '). ($node->title));
	}
  $default = arg(2) ? arg(2) :
    (!empty($form_state['values']['command_selects']) ? $form_state['values']['command_selects'] : 1);
  $title = $options_commands[$default];
  
  
  if (arg(1) === '0') {  // lock command
    $options_commands = array($default => $options_commands[$default]);
    //dpm('lock');
    //dpm(arg(1));
    //dpm($_REQUEST);
  }
  
  $form['command_selects'] = array(
    "#type" => "select",
    "#title" => t("Command"),
    '#options' => $options_commands,
    '#default_value' => $default,
    '#ajax' => array(
      'callback' => '_command_callback',
      'wrapper' => 'command-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
	$station_nid = !empty($form_state['values']['station_nid']) ? $form_state['values']['station_nid'] : arg();
  if ($station_nid[1] == 0) $station_nid = null;
	$form['station_nid'] = array(
    '#type' => 'hidden',    
    '#value' => $station_nid,
  );
	// */
	// */
	
  /*
	// Old
	$form['station_nid'] = array(
    '#type' => 'hidden',    
    '#value' => intval(arg(1)),
  );
	*/
  
  $form['command_fieldset'] = array(
    '#title' => t("Parameter of %title", array('%title' => $title)),
    '#prefix' => '<div id="command-div">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  
  
	
  switch ($default) {
    //Data call command
    case 1 :     
      break;
    case 2 :
      // $form['command_fieldset']['description'] = array(
        // '#type' => 'item',
        // // '#title' => 'a',
        // // '#description' => t('<b>Khoảng thời gian không được cách nhau quá 24h</b>'),
      // );
      $form['command_fieldset']['date_time_from']= array(
        '#type' => 'date_popup',
        '#title' => 'Từ Ngày', 
        '#date_format' => 'd/m/Y H:i:s', 
        '#default_value' => date('d/m/Y H:i:s', $_SERVER['REQUEST_TIME']),
        ////'#required' => TRUE,				
      );
      
      $form['command_fieldset']['date_time_to']= array(
        '#type' => 'date_popup',
        '#title' => 'Tới Ngày', 
        '#date_format' => 'd/m/Y H:i:s', 
        '#default_value' => date('d/m/Y H:i:s', $_SERVER['REQUEST_TIME']),    
        //'#required' => TRUE,
      );
      break;
    case 3 : 
      $form['command_fieldset']['date_time_set']= array(
      '#type' => 'date_popup',
      '#title' => 'Date $ Time', 
      '#date_format' => 'd/m/Y H:i:s',  
			'#required' => TRUE,
      );
      break;
    case 4 : 
      break;
    //ID $ Password Set
    case 5 :
      $form['command_fieldset']['set_input_id']= array(
      '#type' => 'textfield',
      '#title' => 'ID',
      '#maxlength' => '6',
      '#default_value' => '123456',
      );
      break;
    case 6 :
      break;
    case 7 :
      $form['command_fieldset']['set_pass'] = array(
        '#type' => 'textfield',
      '#title' => t('Password'),
      '#maxlength' => '4',
      '#default_value' => '8888',
      );
      break;
    case 8 :
      break;
    //database Buffer Clear
    case 9 :
      break;
    case 10 :
      $form['command_fieldset']['date_buffer_clear']= array(
      '#type' => 'date_popup',
      '#date_format' => 'd/m/Y',
			//'#required' => TRUE,
      );
      break;
    //Rain fall config
    case 11 :
      // $title =  t('Accumulated Rain Fall Value Set');
      $form['command_fieldset']['accumulated_rain_fall']= array(
      '#type' => 'textfield',
      '#title' => t('Accumulated Rain Fall Value'),
      '#default_value' => '100',
      '#maxlength' => '5',
      //'#required' => TRUE,
      );
      break;
    case 12 :
      // $title =  t('Set Rain Fall Clear Time');
			global $clearTime ;
      $form['command_fieldset']['set_rain_fall_clear_time']= array(
      // '#type' => 'date_popup',
      // '#date_format' => 'd:m:Y H:i',
      // '#date_format' => 'Y H:i',
			// '#default_value' => date('H:i', time()),
			//'#required' => TRUE,
			"#type" => "select",
			// "#title" => t("Command"),
			'#options' => $clearTime,
			'#default_value' => 0,
      );
      break;
    case 13 :
      // $title =  t('Get Rain Fall Clear Time');
      break;
    //Datalogger config
    case 14 :
      // $title =  t('Self Power Reset');
      break;
    case 15 :
      // $title =  t('Set Active / De-Active Battery Power Voltage');
      $form['command_fieldset']['bat_on']= array(
      '#type' => 'textfield',
      '#title' => t('Active Battery Power Voltage (V)'),
      // '#title' => 'Điện áp nguồn ắc quy hoạt động (V)',
      '#default_value' => 0011.0,
      '#maxlength' => 5,
      // '#description' => t('Dạng phải nhập: xxx.x'),
      //'#required' => TRUE,
      );
      $form['command_fieldset']['bat_off']= array(
      '#type' => 'textfield',
      '#title' => t('De-Active Battery Power Voltage (V)'),
      // '#title' => 'Điện áp nguồn ắc quy ngừng hoạt động (V)',
      '#default_value' => 0013.0,
      '#maxlength' => 5,
      // '#description' => t('Dạng phải nhập: xxx.x'),
      //'#required' => TRUE,
      );
      break;
    case 16 :
      // $title =  t('Get Active / De-Active Battery Power Voltage');
      break;
    case 17 :
      // $title =  t('Set Battery Voltage Offset');
      $form['command_fieldset']['set_bat_vol']= array(
      '#type' => 'textfield',
      // '#title' => 'Phần bù điện áp pin (V)',
      '#title' => t('Battery Voltage Offset (V)'),
      '#default_value' => 1.5,
      '#maxlength' => 4,
      '#description' => t('Dạng phải nhập: xx.x'),
      //'#required' => TRUE,
      );
      break;
    case 18 :
      // $title =  t('Get Battery Voltage Offset');
      // $form['command_fieldset']['get_bat_vol']= array(
      // '#type' => 'textfield',
      // '#title' => t('Battery Voltage Offset (V)'),
      // // '#title' => 'Phần bù điện áp pin (V)',
      // '#default_value' => 1.5,
      // '#maxlength' => 5,
      // '#description' => t('Dạng phải nhập: xx.x'),
      // //'#required' => TRUE,
      // );
      break;
    case 19 :
      // $title =  t('Set Auto Power Management Enable');
      break;
    case 20 :
      // $title =  t('Set Auto Power Management Disable');
      break;
    case 21 :
      // $title =  t('Get Auto Power Management Enable / Disable');
      break;
    case 22 :
      // $title =  t('Go To Sleep Mode');
      break;
    //Comunication setting
    case 23 :
      // $title =  t('Set Data Send Interval Minutes');
			
			global $timeInterval;
      $form['command_fieldset']['set_interval']= array(
      // '#type' => 'textfield',
      // // '#title' => 'Thời gian truyên số liệu',
      '#title' => t('Data Send Interval Minutes'),
      // '#default_value' => 10,
      // '#maxlength' => 4,
      //'#required' => TRUE,
			"#type" => "select",
			// "#title" => t("Command"),
			'#options' => $timeInterval,
			'#default_value' => 0,
      );
      break;
    case 24 :
      // $title =  t('Get Data Send Interval Minutes');
      break;
    case 25 :
      // $title =  t('Set Communication Type GSM');
      break;
    case 26 :
      // $title =  t('Set Communication Type GPRS');
      break;
    case 27 :
      // $title =  t('Set Communication Type Auto');
      break;
    case 28 :
      // $title =  t('Get Communication Type');
      break;
    //GPRS network setting
    case 29 :
      // $title =  t('Set GPRS Network Setting');
      $form['command_fieldset']['host_gprs']= array(
        '#type' => 'textfield',
        '#title' => t('Host'),
        '#default_value' => '192.168.151.255',
        '#maxlength' => 15,
        '#description' => 'xxx.xxx.xxx.xxx',
        '#required' => true,
      );
      
      $form['command_fieldset']['id_gprs']= array(
        '#type' => 'textfield',
        '#title' => t('ID'),
        '#default_value' => 123456,
        '#maxlength' => 6,
        '#required' => true,
        '#description' => t('ID phải chứa 6 kí tự.'),
      );
      $form['command_fieldset']['pass_gprs'] = array(
        '#type' => 'password_confirm',
        '#maxlength' => 25,
        '#required' => true,
      );
      break;
    case 30 :
      // $title =  t('Get GPRS Network Setting');
      break;
    case 31 :
      // $title =  t('Set Host Network Infomation');
      $form['command_fieldset']['set_host']= array(
      '#type' => 'textfield',
      '#title' => t('Host'),
      '#default_value' => '192.168.151.255',
      '#maxlength' => 15,
      '#required' => true,
      );
      
      $form['command_fieldset']['set_port']= array(
      '#type' => 'textfield',
      '#title' => t('Port'),
      '#default_value' => 8000,
      '#maxlength' => 6,
      // '#required' => true,
      );    
      break;
    case 32 :
      // $title =  t('Get Host Network Infomation');
      break;
    case 33 :
      // $title =  t('Set Host Phone Number');
      $form['command_fieldset']['host_phone']= array(
      '#type' => 'textfield',
      '#title' => t('Host Phone Number'),
      // '#title' => 'Số điện thoại máy chủ',
      '#default_value' => '800010001',
      '#maxlength' => 15,
      '#required' => true,
      );
      break;
    case 34 :
      // $title =  t('Get Host Phone Number');
      break;
    //Get datalogger firmware version
    case 35 :
      // $title =  t('Get Datalogger Firmware Version');
      break;
		case 36 :
      $form['command_fieldset']['set_alarm']= array(
      '#type' => 'textfield',
      '#title' => t('Interval'),
      '#default_value' => '60',
      '#maxlength' => 3,
      '#required' => true,
      );
      
      $form['command_fieldset']['set_level']= array(
      '#type' => 'textfield',
      '#title' => t('Level'),
      '#default_value' => '100.0',
      '#maxlength' => 4,
      '#required' => true,
      );    
      break;
		case 37 :
      // $title =  t('Get Datalogger Firmware Version');
      break;
		case 38 :
      // $title =  t('Get Datalogger Firmware Version');
      break;
  }

  if (count(array_keys($form['command_fieldset'])) == 4) {    // empty 
    $form['command_fieldset']['empty']= array(
      '#markup' => t('(Không có).'),      
    );
  }
  
	$form["submit"] = array(
      "#type" => "submit",
      "#value" => t("Send Now!"),
      "#submit" => array(
          //"_send_command_submit"
      ),
			'#ajax' => array(
      'callback' => '_datalogger_submit_command_callback',
      'wrapper' => 'ajax-command-submit-div',
      'name' => 'submit1',
      'method' => 'replace',
      'effect' => 'fade',
			),
  );
  	
    
	$form['submit_command_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Command Result'),
    '#prefix' => '<div id="ajax-command-submit-div">',		
    '#suffix' => '</div>',
    //'#attributes' => array('style' => 'display: none;'),    
		// '#value' =>_datalogger_new_result_sms() ,
    '#weight' => 1000,
  );
  	
	
  $form['#validate'][] = '_command_validate';
	// }
  
	
  return $form;
}


function _datalogger_submit_command_callback($form, &$form_state) {

  //digi_debug($form_state, 0, 1);
	
  $node_command = _send_command_submit($form, $form_state);
  
  $element = &$form['submit_command_fieldset'];
  
  if ($node_command) {
    $form['submit_command_fieldset']['#title'] = t('Command Result') . ': ' . date('d/m/Y - H:i:s', time());
    //$element['#markup'] = "Clicked submit";
    
    $node_view = node_view($node_command);
    if ($node_command->nid) {
      $form['submit_command_fieldset']['date_time_from'] = array(    
        '#markup' => drupal_render($node_view),    
      );
    }
    else {
      
    }
  }
  else {
  }
  
	
  return $element;
}


function _command_validate($form, &$form_state) {
  switch ($form_state["values"]["command_selects"]) {
    case 2 :
      $dt_from = $form_state['values']['date_time_from'];
			$number = $form_state['values']['accumulated_rain_fall'];
      if(empty($dt_from)){
				_datalogger_check_empty('date_time_from', 'Từ ngày');
      }
      $dt_to = $form_state['values']['date_time_to'];
			if(empty($dt_from)){
				_datalogger_check_empty('date_time_to', 'Tới ngày');
      }
      $interval = (strtotime($dt_to) - strtotime($dt_from))/(24*60*60);
      if($interval < 0) {
        form_set_error('date_time_to',t("Thời gian <b>Tới ngày</b> không được nhỏ hơn <b>Từ ngày</b>"));
      }
      else{
        if($interval > 1) {
          form_set_error('date_time_to',t("Thời gian <b>Tới ngày</b> không được lớn hơn <b>Từ ngày</b> quá 24h"));
        }
      }
      break;
    //Date and Time Set
    case 3 : 
      break;
    case 4 : 
      break;
    //ID $ Password Set
    case 5 :
      $id = $form_state['values']['set_input_id'];
			if(empty($id)){
				_datalogger_check_empty('set_input_id', 'ID');
      }
      elseif (preg_match('/[^a-zA-Z0-9]/i',$id)){
         form_set_error('set_input_id',t('Trường <b>ID</b> không được chứa kí tự đặc biệt.<br>Chỉ được chứa chữ cái, và số'));
      }
      else
        if(strlen($id) != 6) {
          form_set_error('set_input_id',t('Trường <b>ID</b> không đúng định dạng.Bắt buộc phải chứa 6 kí tự'));
        }      
      break;
    case 6 :
      break;
    case 7 :
			$pass = $form_state['values']['set_pass'] ;
			if(empty($pass)){
				_datalogger_check_empty('set_pass', 'Password');
      }
      elseif (preg_match('/[^0-9]/i',$pass)){
         form_set_error('set_pass',t('Trường <b>'.t('Password').'</b> không được chứa kí tự đặc biệt.<br>Chỉ được chứa chữ số'));
      }
      break;
    case 8 :
      break;
    //database Buffer Clear
    case 9 :
      break;
    case 10 :
			$day = $form_state['values']['date_buffer_clear'];
			if(empty($day)){
				_datalogger_check_empty('date_buffer_clear','Ngày tháng');
      }
      break;
    //Rain fall config
    case 11 :
    // ['command_fieldset']['accumulated_rain_fall']
      $number = $form_state['values']['accumulated_rain_fall'];
			if(empty($number)){
				_datalogger_check_empty('accumulated_rain_fall', 'Accumulated Rain Fall Value');
      }
      else{
        // if(($number%10) != 0 || $number < 0){
          // form_set_error('accumulated_rain_fall',t('Giá trị trường <b>').t('Accumulated Rain Fall Value').t('</b> phải dương  và là bội của 10'));
        // }
				// else
				if (preg_match('/[^0-9.]/i',$number)){
         form_set_error('accumulated_rain_fall',t('Trường <b>').t('Accumulated Rain Fall Value').t('</b> chỉ được chứa số.'));
				}
      }
      break;
    case 12 :
			// $time = $form_state['values']['set_rain_fall_clear_time'];
			// dsm($time);
			// if(empty($time)){
				// _datalogger_check_empty('set_rain_fall_clear_time','Thời gian');
      // }
      break;
    case 13 :
      break;
    //Datalogger config
    case 14 :
      break;
    case 15 :
      // $title =  t('Set Active / De-Active Battery Power Voltage');
      $batOn = $form_state['values']['bat_on'];
      $batOff = $form_state['values']['bat_off'];
      
			if(empty($batOn)){
				_datalogger_check_empty('bat_on', 'Active Battery Power Voltage (V)');
      }
      else if (preg_match('/[^0-9.]/i',$batOn)){
         form_set_error('bat_on',t('Trường <b>').t('Active Battery Power Voltage (V)').t('</b> chỉ được chứa số.'));
      }
      
			if(empty($batOff)){
				_datalogger_check_empty('bat_on', 'De-Active Battery Power Voltage (V)');
      }
      else if (preg_match('/[^0-9.]/i',$batOff)){
         form_set_error('bat_off',t('Trường <b>').t('De-Active Battery Power Voltage (V)').t('</b> chỉ được chứa số.'));
      }
      break;
    case 16 :
      // $title =  t('Get Active / De-Active Battery Power Voltage');
      break;
    case 17 :
      // $title =  t('Set Battery Voltage Offset');   
      $vol = $form_state['values']['set_bat_vol'];
			if(empty($vol)){
				_datalogger_check_empty('set_bat_vol', 'Battery Voltage Offset (V)');
      }
      else if (preg_match('/[^0-9.]/i',$vol)){
         form_set_error('set_bat_vol',t('Trường <b>').t('Battery Voltage Offset (V)').t('</b> chỉ được chứa số.'));
      }
      break;
    case 18 :
      // $vol = $form_state['values']['get_bat_vol'];
			// if(empty($vol)){
				// _datalogger_check_empty('get_bat_vol', 'Battery Voltage Offset (V)');
      // }
      break;
    case 19 :
      // $title =  t('Set Auto Power Management Enable');
      break;
    case 20 :
      // $title =  t('Set Auto Power Management Disable');
      break;
    case 21 :
      // $title =  t('Get Auto Power Management Enable / Disable');
      break;
    case 22 :
      // $title =  t('Go To Sleep Mode');
      break;
    //Comunication setting
    case 23 :
      // $title =  t('Set Data Send Interval Minutes');
      // $interval = $form_state['values']['set_interval'];
			
			// if(empty($interval)){
				// _datalogger_check_empty('set_interval', 'Data Send Interval Minutes');
      // }
      // else{
        // if(($interval % 10) != 0  || $interval > 180 || $interval < 0){
          // form_set_error('set_interval',t('Trường <b>').t('Data Send Interval Minutes').t('</b> Phải là bội của 10, không âm và nhỏ hơn 180 .')); 
        // }       
        // else if (preg_match('/[^0-9]/i',$interval)){
          // form_set_error('set_interval',t('Trường <b>').t('Data Send Interval Minutes').t('</b> chỉ được chứa số.'));
        // }
      // }
      break;
    case 24 :
      // $title =  t('Get Data Send Interval Minutes');
      break;
    case 25 :
      // $title =  t('Set Communication Type GSM');      
      break;
    case 26 :
      // $title =  t('Set Communication Type GPRS');
      break;
    case 27 :
      // $title =  t('Set Communication Type Auto');
      break;
    case 28 :
      // $title =  t('Get Communication Type');
      break;
    //GPRS network setting
    case 29 :
      // $title =  t('Set GPRS Network Setting');
      $host = $form_state['values']['host_gprs'];
      $id = $form_state['values']['id_gprs'];
      $pass = $form_state['values']['pass_gprs']; 
			if(empty($host)){
				_datalogger_check_empty('id_gprs', 'Host');
      }
      else{
        if(!filter_var($host, FILTER_VALIDATE_IP)){
          form_set_error('host_gprs',t('Trường <b>').t('Host').t('</b> không đúng định dạng. xxx.xxx.xxx.xxx<br>Hay nhập lại.'));
        }      
      }
      break;
    case 30 :
      // $title =  t('Get GPRS Network Setting');
      break;
    case 31 :
      $host = $form_state['values']['set_host'];
			if(empty($host)){
				_datalogger_check_empty('set_host', 'Host');
      }
      $port = $form_state['values']['set_port'];
			if(empty($port)){
				_datalogger_check_empty('set_port', 'Port');
      }
      if(!filter_var($host, FILTER_VALIDATE_IP)){
          form_set_error('set_host',t('Trường <b>').t('Host').t('</b> không đúng định dạng. xxx.xxx.xxx.xxx<br>Hay nhập lại.'));
      }
      else{
        if (preg_match('/[^0-9]/i',$port)){
          form_set_error('set_port',t('Trường <b>').t('Port').t('</b> chỉ được chứa ký tự số.'));
        }       
      }
      break;
    case 32 :
      // $title =  t('Get Host Network Infomation');
      break;
    case 33 :
      // $title =  t('Set Host Phone Number');
      $host = $form_state['values']['host_phone'];
      if(empty($host)){
        _datalogger_check_empty('host_phone','Host Phone Number');
      }
      break;
    case 34 :
      // $title =  t('Get Host Phone Number');
      break;
    //Get datalogger firmware version
    case 35 :
      // $title =  t('Get Datalogger Firmware Version');
      break;
		case 36 :
			$interval = $form_state['values']['set_alarm'];
			if(empty($interval)){
				_datalogger_check_empty('set_alarm', 'Interval');
      }
			elseif (preg_match('/[^0-9]/i',$interval)){
          form_set_error('set_alarm',t('Trường <b>').t('Interval').t('</b> chỉ được chứa ký tự số.'));
      }
			
      $level = $form_state['values']['set_level'];
			if(empty($level)){
				_datalogger_check_empty('set_level', 'Level');
      }
			//elseif (preg_match('/([0-9]|\.)/i', $level)){
      if (!preg_match('/^[0-9\.]+$/', $level)){         // include .
          form_set_error('set_level',t('Trường <b>').t('Level').t('</b> chỉ được chứa ký tự số và dấu chấm thập phân.'));
      }
			break;
		case 37 :
			break;
		case 38 :
			break;
  }
}


function _send_command_submit($form, &$form_state) {

  $nid = intval(arg(1)) ? intval(arg(1)) : $form_state['values']['nid'];
  
  if($nid == 0 && $form_state['values']['station']) {
    $nid = $form_state['values']['station'];
    $form_state['values']['station_nid'] = array('command', $nid);
    
    //dpm(array('nid', $nid));
  }
  
  //dpm($form_state[values]);
  
  //dpm($nid);
  
  
  
  $form['nid'] = array(
    '#type' => 'hidden', 
    '#value' => $nid,
  );
  
	if(!empty($form_state['values']['select'])){
		// $select = $form_state['values']['select'];
		$select = 1;
	}
	else{
		$select = $form_state["values"]["command_selects"];
	}
	
  return _datalogger_send_command($nid, $select,$form, $form_state);
}


function _datalogger_send_command($nid, $select, $form, &$form_state) {
	define('DATEymdHis', date('ymdHis',time()));  
  define('DATEymdHi', date('ymdHi',time()));  
  define('DATEYmdHi', date('YmdHi',time()));  
  
  $nids = $form_state['values']['station_nid'];
  
  //dpm(array('nids', $nid));
  
	for($i_nid = 1; $i_nid < count($nids); $i_nid++ ) {
    $nid = intval($nids[$i_nid]);
    
    
		if($nid) {  
			$node = node_load($nid);
      
      //dpm($node);
      
			$code = _datalogger_language_field($node, 'field_station_code');
      for ($i = strlen($code); $i < 6; $i++)  $code = '0' . $code;    // add 0 to max 6 chars.
      
      $phone = _datalogger_language_field($node, 'field_station_sim_number');
      
      if (!$phone) {
        drupal_set_message(t('Lỗi đọc số điện thoại trạm.'));
        return 0;   // node
      }
			$modem = node_load(_datalogger_language_field($node, 'field_station_modem', 'nid')); 
      switch ($select) {
        case 1 :
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'I:STATUS ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          $arg = array('Lấy số liệu mưa hiện tại','I:STATUS', _datalogger_result($message,'I:STATUS'),'',$nid);
          $check = true;
          break;
        case 2 :
          $dt_from = $form_state['values']['date_time_from'];
          $from = _datalogger_convert_date('ymdHi',$dt_from);
          $dt_to = $form_state['values']['date_time_to'];
          $to = _datalogger_convert_date('ymdHi',$dt_to);
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'S:'.$from.'-'.$to;
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Save Data Call','2', _datalogger_result($message),'');
          $arg = array('Lấy dữ liệu đã lưu trên Datalogger','S:', _datalogger_result($message,'S:'),'',$nid);
          break;
        //Date and Time Set
        case 3 : 
				// dsm($form_state['values']['date_time_set']);
          $date = _datalogger_convert_date($strFormat = 'YmdHis', $form_state['values']['date_time_set']);
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'T:'.$date;
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array(('Date $ Time Set'),'3', _datalogger_result($message),'');
          $arg = array(('Thiết lập thời gian'),'T:', _datalogger_result($message,'T:'),'',$nid);

          break;
        case 4 : 
          // $title =  t('GPRS Network Time Synchronization');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'T:NITZ                                                      ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('GPRS Network Time Synchronization','4', _datalogger_result($message),'');
          $arg = array('Đồng bộ thời gian mạng GPRS','T:NITZ', _datalogger_result($message,'T:NITZ'),'',$nid);
          break;
        //CODE $ Password Set
        case 5 :
          // $title =  t('Set CODE');
          $id = $form_state['values']['set_input_id'];
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'D:'.$id.'                                                    ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set CODE','5', _datalogger_result($message),'');
          $arg = array('Thiết lập ID','D:', _datalogger_result($message,'D:'),'',$nid);
          break;
        case 6 :
          // $title =  t('Get CODE');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'D:?                                                         ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          
          // $arg = array('Get CODE','6', _datalogger_result($message),'');
          $arg = array('Lấy thông tin ID','D:', _datalogger_result($message,'D:'),'',$nid);
          break;
        case 7 :
          $pass = $form_state['values']['set_pass'];
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'W:'.$pass;
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Password','7', _datalogger_result($message),'');
          $arg = array('Thiết lập mật khẩu','W:', _datalogger_result($message,'W:'),'',$nid);
          break;
        case 8 :
          // $title =  t('Get Password');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'W:?                                                         ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Password','8', _datalogger_result($message),'');
          $arg = array('Lấy thông tin mật khẩu','P:', _datalogger_result($message,'W:'),'',$nid);
          break;
        //database Buffer Clear
        case 9 :
          // $title =  t('All Database Buffer Clear');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'R:FORMAT ALL                                                ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('All Database Buffer Clear','9', _datalogger_result($message),'');
          $arg = array('Xóa bộ đệm dữ liệu','R:FORMAT ALL', _datalogger_result($message,'R:FORMAT ALL'),'',$nid);
          break;
        case 10 :
          // $title =  t('Specific Date Database Buffer Clear');
          $day = _datalogger_convert_date('ymd',$form_state['values']['date_buffer_clear']);
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'R:FORMAT DAY:'.$day.'                                         ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Specific Date Database Buffer Clear','10', _datalogger_result($message),'');
          $arg = array('Xóa bộ đệm dữ liệu theo ngày','R:FORMAT DAY:', _datalogger_result($message,'R:FORMAT DAY:'),'',$nid);
          break;
        //Rain fall config
        case 11 :
          // $title =  t('Accumulated Rain Fall Value Set');
          $number = $form_state['values']['accumulated_rain_fall'];
          $number = (float)$number * 10;
          $number = (int)$number;
          
          $len = strlen($number);
          if($len < 5 ){
            for($i = 0; $i < (5 - $len); $i++){
              $number = ' '.$number;
            }
          }
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'R:SET:'.$number.'                                                 ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Accumulated Rain Fall Value Set','11', _datalogger_result($message),'');
          $arg = array('Thiết lập gía trị lượng mưa tích lũy','R:SET:', _datalogger_result($message,'R:SET:'),'',$nid);
          break;
        case 12 :
          // $title =  t('Set Rain Fall Clear Time');
          global $clearTime2 ;
          // dsm($form_state['values']['set_rain_fall_clear_time']);
          $time = $clearTime2[$form_state['values']['set_rain_fall_clear_time']];
          // dsm($time);
          // dsm($clearTime2[0]);
          // $time = _datalogger_convert_date('Hi',$form_state['values']['set_rain_fall_clear_time']);
          // dsm($time);
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'R:CLEAR TIME:'.$time.'                                           ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Rain Fall Clear Time','12', _datalogger_result($message),'');
          $arg = array('Thiết lập thời gian xóa dữ liệu mưa','R:CLEAR:', _datalogger_result($message,'R:CLEAR:'),'',$nid);
          // dsm(strlen($message));
          // dsm($message);
          break;
        case 13 :
          // $title =  t('Get Rain Fall Clear Time');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'R:CLEAR TIME:?                                              ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Rain Fall Clear Time','13', _datalogger_result($message),'');
          $arg = array('Lấy thời gian xóa dữ liệu mưa','R:CLEAR TIME:', _datalogger_result($message,'R:CLEAR TIME:'),'',$nid);
          break;
        //Datalogger config
        /*
        case 14 :
          // $title =  t('Self Power Reset');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:RESET                                                     ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Self Power Reset','14', _datalogger_result($message),'');
          $arg = array('Thiết lập lại nguồn điện','P:RESET', _datalogger_result($message,'P:RESET'),'',$nid);
          break;*/
        case 15 :
          // $title =  t('Set Active / De-Active Battery Power Voltage');
          $batOn = number_format($form_state['values']['bat_on'],1);
          $lenOn = strlen($batOn);
          if($lenOn < 5){
            for($i=0; $i < (5 - $lenOn); $i++){
              $batOn = '0'.$batOn;
            }
          }
          $batOff = number_format($form_state['values']['bat_off'],1);
          $lenOff = strlen($batOff);
          if($lenOff < 5){
            for($i=0; $i < (5 - $lenOff); $i++){
              $batOff = '0'.$batOff;
            }
          }
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:BAT ON:'.$batOn.'V, OFF:'.$batOff.'V                                 ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Active / De-Active Battery Power Voltage','15', _datalogger_result($message),'');
          $arg = array('Thiết lập điện áp nguồn ắc quy hoạt động/ngừng hoạt động','P:BAT ON:', _datalogger_result($message,'P:BAT ON:'),'',$nid);
          break;
        case 16 :
          // $title =  t('Get Active / De-Active Battery Power Voltage');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'P:BAT ON:?                                                  ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Active / De-Active Battery Power Voltage','16', _datalogger_result($message),'');
          $arg = array('Lấy điện áp nguồn điện ắc quy hoạt độn/ngừng hoạt động','P:BAT ON:', _datalogger_result($message,'P:BAT ON:'),'',$nid);
          break;
        case 17 :
          // $title =  t('Set Battery Voltage Offset');
          $vol = number_format($form_state['values']['set_bat_vol'],1);
          $lenVol = strlen($vol);
          if($lenVol < 4){
            for($i=0; $i < (4 - $lenVol); $i++){
              $vol = '0'.$vol;
            }
          }
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:BAT OFFSET:+'.$vol.'V                                         ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Battery Voltage Offset','17', _datalogger_result($message),'');
          $arg = array('Thiết lập phần bù điện áp ắc quy','P:BAT OFFSET:', _datalogger_result($message,'P:BAT OFFSET:'),'',$nid);
          break;
        case 18 :
          // $title =  t('Get Battery Voltage Offset');
          // $vol = number_format($form_state['values']['get_bat_vol'],1);
          // $lenVol = strlen($vol);
          // if($lenVol < 4){
            // for($i=0; $i < (4 - $lenVol); $i++){
              // $vol = '0'.$vol;
            // }
          // }
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'P:BAT OFFSET:?                                        ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Battery Voltage Offset','18', _datalogger_result($message),'');
          $arg = array('Lấy phần bù điện áp ắc quy','P:BAT OFFSET:', _datalogger_result($message,'P:BAT OFFSET:'),'',$nid);
          // dsm($message);
          // dsm(strlen($message));
          break;
        case 19 :
          // $title =  t('Set Auto Power Management Enable');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:AUTO PWR MANAGEMENT:ON                                    ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Auto Power Management Enable','19', _datalogger_result($message),'');
          $arg = array('Thiết lập tự động quản lý nguồn điện áp','P:AUTO PWR MANAGEMENT:ON', _datalogger_result($message,'P:AUTO PWR MANAGEMENT:ON'),'',$nid);
          break;
        case 20 :
          // $title =  t('Set Auto Power Management Disable');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:AUTO PWR MANAGEMENT:OFF                                   ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Auto Power Management Disable','20', _datalogger_result($message),'');
          $arg = array('Thiết lập không tự động quản lý nguồn điện','P:AUTO PWR MANAGEMENT:OFF', _datalogger_result($message,'P:AUTO PWR MANAGEMENT:OFF'),'',$nid);
          break;
        case 21 :
          // $title =  t('Get Auto Power Management Enable / Disable');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'P:AUTO PWR MANAGEMENT:?                                     ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Auto Power Management Enable / Disable','21', _datalogger_result($message),'');
          $arg = array('Lấy thông tin quản lý nguồn','P:AUTO PWR MANAGEMENT:', _datalogger_result($message,'P:AUTO PWR MANAGEMENT:'),'',$nid);
          break;
        case 22 :
          // $title =  t('Go To Sleep Mode');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:GOTO SLEEP MODE                                           ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Go To Sleep Mode','22', _datalogger_result($message),'');
          $arg = array('Chuyển sang chế độ ngủ','P:GOTO SLEEP MODE', _datalogger_result($message,'P:GOTO SLEEP MODE'),'',$nid);
          break;
        //Comunication setting
        case 23 :
          // $title =  t('Set Data Send Interval Minutes');
          global $timeInterval;
          $interval = $timeInterval[$form_state['values']['set_interval']];
          // dsm($interval);
          $lenint = strlen($interval);
          if($lenint < 4){
            for($i = 0; $i < (4 - $lenint); $i++ ){
              $interval =' '.$interval;
            }
          }
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'C:INTERVAL:'.$interval.'                                             ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Data Send Interval Minutes','23', _datalogger_result($message),'');
          $arg = array('Thiết lập thời gian truyền dữ liệu','C:INTERVAL:', _datalogger_result($message,'C:INTERVAL:'),'',$nid);
          // dsm(strlen($message));
          // dsm($message);
          break;
        case 24 :
          // $title =  t('Get Data Send Interval Minutes');
          $sms =  'S' . $code . 'GET  '.DATEymdHis.'C:INTERVAL:?                                                ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Data Send Interval Minutes','24', _datalogger_result($message),'');
          $arg = array('Lấy thời gian truyền dữ liệu','C:INTERVAL:', _datalogger_result($message,'C:INTERVAL:'),'',$nid);
          break;
        case 25 :
          // $title =  t('Set Communication Type GSM');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'C:COMM:GSM                                                  ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Communication Type GSM','25', _datalogger_result($message),'');
          $arg = array('Thiết lập kết nối dạng GSM','C:COMM:GSM', _datalogger_result($message,'C:COMM:GSM'),'',$nid);
          break;
        case 26 :
          // $title =  t('Set Communication Type GPRS');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'C:COMM:GPRS                                                 ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Communication Type GPRS','26', _datalogger_result($message),'');
          $arg = array('Thiết lập kết nối dạng GPRS','C:COMM:GPRS', _datalogger_result($message,'C:COMM:GPRS'),'',$nid);
          break;
        case 27 :
          // $title =  t('Set Communication Type Auto');
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'C:COMM:AUTO                                                 ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Communication Type Auto','27', _datalogger_result($message),'');
          $arg = array('Thiết lập kết nối dạng tự động','C:COMM:AUTO', _datalogger_result($message,'C:COMM:AUTO'),'',$nid);
          break;
        case 28 :
          // $title =  t('Get Communication Type');     
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'C:COMM:?                                                    ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Communication Type','28', _datalogger_result($message),'');
          $arg = array('Lấy về trạng thái kết nối','C:COMM:', _datalogger_result($message,'C:COMM:'),'',$nid);
          break;
        //GPRS network setting
        case 29 :
          // $title =  t('Set GPRS Network Setting');
          $host = $form_state['values']['host_gprs'];
          $id = $form_state['values']['id_gprs'];
          $pass = $form_state['values']['pass_gprs'];
          
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'G:APN:'.$host.',ID:'.$id.',PW:'.$pass;
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set GPRS Network Setting','29', _datalogger_result($message),'');
          $arg = array('Thiết lập mạng GPRS','G:APN:', _datalogger_result($message,'G:APN:'),'',$nid);
          break;
        case 30 :
          // $title =  t('Get GPRS Network Setting');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'G:APN:?                                                     ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          
          // $arg = array('Get GPRS Network Setting','30', _datalogger_result($message),'');
          $arg = array('Lấy thiết lập mạng GPRS','G:APN:', _datalogger_result($message,'G:APN:'),'',$nid);
          break;
        case 31 :
          // $title =  t('Set Host Network Infomation');
          $host = $form_state['values']['set_host'];
          $port = $form_state['values']['set_port'];
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'N:HOST:'.$host.':'.$port;
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Host Network Infomation','31', _datalogger_result($message),'');
          $arg = array('Thiết lập mạng máy chủ','N:HOST:', _datalogger_result($message,'N:HOST:'),'',$nid);
          break;
        case 32 :
          // $title =  t('Get Host Network Infomation');      
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'N:HOST:?                                                    ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Host Network Infomation','32', _datalogger_result($message),'');
          $arg = array('Lấy thông tin mạng máy chủ','N:HOST:', _datalogger_result($message,'N:HOST:'),'',$nid);
          break;
        case 33 :
          // $title =  t('Set Host Phone Number');
          $sim = $form_state['values']['host_phone'];
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'H:HOST PHONE NO:'.$sim;
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Set Host Phone Number','33', _datalogger_result($message),'');
          $arg = array('Thiết lập số điện thoại máy chủ','H:HOST PHONE NO:', _datalogger_result($message,'H:HOST PHONE NO:'),'',$nid);
          break;
        case 34 :
          // $title =  t('Get Host Phone Number');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'H:HOST PHONE NO:?                                           '; 
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';
          // $arg = array('Get Host Phone Number','34', _datalogger_result($message),'');
          $arg = array('Lấy số điện thoại máy chủ','H:HOST PHONE NO:', _datalogger_result($message,'H:HOST PHONE NO:'),'',$nid);
          break;
        //Get datalogger firmware version
        case 35 :
          // $title =  t('Get Datalogger Firmware Version');
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'V:VERSION:?                                                 ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';			
          // $arg = array('Get Datalogger phiên Version','35', _datalogger_result($message),'');
          $arg = array('Lấy phiên bản Datalogger hiện tại','V:VERSION:', _datalogger_result($message,'V:VERSION:'),'',$nid);
          break;
        case 36 :
          $interval = $form_state['values']['set_alarm'];
          if(strlen($interval) < 3){
            $interval = '0'.$interval;	
          }
          $level = $form_state['values']['set_level'];
          
          $level = floatval($level) * 10;   
          $level = intval($level);
          
          if(strlen($level) < 4){
            $lenstr = strlen($level);
            for($i = 0; $i < 4-$lenstr; $i++){
              $level = ' '.$level;
            }
          }
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'A:ALARM INTERVAL:'.$interval.', LEVEL:'.$level.'                            ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';			
          // dsm($message);
          // dsm(strlen($message));
          // $arg = array('Get Datalogger phiê Version','35', _datalogger_result($message),'');
          $arg = array('Thiết lập thời gian và lượng mưa cảnh báo', 'A:ALARM INTERVAL:', _datalogger_result($message,'A:ALARM INTERVAL:'), '', $nid);
          break;
        case 37 :
          $sms = 'S' . $code . 'GET  '.DATEymdHis.'A:ALARM INTERVAL:?                                          ';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';			
          // dsm($message);
          // dsm(strlen($message));
          // $arg = array('Get Datalogger phiê Version','35', _datalogger_result($message),'');
          $arg = array('Lấy về thời gian và lượng mưa cảnh báo','A:ALARM INTERVAL:', _datalogger_result($message,'A:ALARM INTERVAL:'),'',$nid);
          break;
        case 38 :
        //reset all config			
          $sms = 'S' . $code . 'SET  '.DATEymdHis.'P:RESET';
          $sms = _datalogger_add_space($sms, 84);
          $message = 'H'.$sms._datalogger_CheckSum($sms).';';			
          $arg = array('Khởi động lại Datalogger', 'P:RESET', _datalogger_result($message,'P:RESET'),'',$nid);
          break;
      }

      
      $type = substr($sms, 7, 3);
      $arg['type'] = $type;
			
			// dsm('first '.$arg[1]);
			
			$arg[1] = str_replace(' ', '_', trim($arg[1]));
			
      $node_command = _datalogger_insert_node_command($arg);

      _datalogger_insert_outbox($phone, $message, $modem->title);	
      // if($check != true){
        // $form_state['redirect'] = 'node/' . $node_command->nid . '/refresh';
      // }else{
        // $form_state['redirect'] = 'view/rainfall/default/refresh';
      // }
    }
	}
  
	drupal_set_message(t('Command has been sent for delivering outbox.'));
  
  return $node_command;
}	


function _datalogger_language_field(&$node, $field_name, $value = 'value') {
  $all_languages = array('und', 'vi', 'en');  
  foreach($all_languages as $lang) {
    if (isset($node->{$field_name}[$lang]['0'][$value])) {
      return $node->{$field_name}[$lang]['0'][$value];      
    }
  }
  return '';
}

function _datalogger_add_space($sms, $length){
	$len = strlen($sms);
	if($len < $length){
		for($i = 0; $i < ($length - $len); $i++){
			$sms .= ' ';  
		}
	}
	return $sms;
}


/*Check sum sms*/
function _datalogger_CheckSum($str){
  $chk = 0;
  for($k = 0; $k < strlen($str); $k++){
    $o = ord(substr($str,$k,1));
    $chk ^= $o;
  }
  $res = substr("0" . dechex($chk),-2);
  return strtoupper($res);
}

/*Chèn node mới vào bàng command*/

function _datalogger_insert_node_command($arg){
	$node = new stdClass();
  $node->title = $arg[0];
  $node->type = 'command';
  $node->created = time();
  $node->changed = $node->created;
  $node->status = 1; // Published?
  $node->promote = 0; // Display on front page?
  $node->sticky = 0; // Display top of page?
  $node->format = 1; // Filtered HTML?
  $node->uid = 1; // Content owner uid (author)?
  $node->language = 'und';
	
  node_object_prepare($node);
  $node->field_command_code[$node->language][0]['value'] = $arg[1];
  $node->field_command_parameter[$node->language][0]['value'] = $arg[2];
  $node->field_command_result[$node->language][0]['value'] = $arg[3]; 
  $node->field_command_station[$node->language][0]['nid'] = $arg[4];  
	$node->field_command_type[$node->language][0]['value'] = $arg['type'];
  $node = node_submit($node);
  node_save($node);
	return $node;
}


/**
*begin: 28-09-12
*
*/

// function _datalogger_insert_outbox($phone, $sms){
  // /*cach 1:*/
  // $sql = "INSERT INTO outbox(DestinationNumber, TextDecoded, CreatorID) VALUES('$phone', '$sms', '')";
  // $nid = db_query($sql);
// }
function _datalogger_insert_outbox($phone, $sms, $modem){
  /*cach 1:*/
  // dsm('thong tin modem gui tin '.$modem);
  $sql = "INSERT INTO outbox(DestinationNumber, TextDecoded, CreatorID, SenderID) VALUES('$phone', '$sms', '', '$modem')";
  $nid = db_query($sql);
	// dsm($nid);
}

function _datalogger_check_empty($arg1, $arg2){
  return form_set_error($arg1,t('Trường <b>').t($arg2).t('</b> không được để trống'));
}

/* for ahah */
function _command_callback($form, &$form_state) {
  return $form['command_fieldset'];
}


/**s
*begin: 25-09-12
*/

function _datalogger_convert_date($strFormat = 'ymdhis', $strDate){
  return date($strFormat, strtotime($strDate));
}