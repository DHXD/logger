<?php
/*
*
*** BKT14***
*
*/
function _datalogger_report_rainfall_month_form($form, &$form_state) {
	drupal_set_title(t('Report monthly rainfall data (BKT14 form)'));
 
	_datalogger_select_area_province_station($form, $form_state);
  
	$form['stations_fieldset']['day']['#access']=FALSE;
  
  $report_BKT = array(
    0 => 'Báo cáo BKT14', 
    1 => 'BKT14 Tự động',
  );
  
  $form['report_BKT14'] = array(
    '#type' => 'radios',
    '#title' => 'Chọn loại báo cáo',
    '#options' => $report_BKT,
    '#default_value' => 0,
  );
  
	$form['submit'] = array(
      "#type" => "submit",
      "#value" => t("Report"),
      "#submit" => array("_datalogger_report_rainfall_month_submit_BKT14"),
  );
  
  return $form;
}

function _datalogger_report_rainfall_month_submit_BKT14(&$form, &$form_state) {
	module_load_include('inc', 'datalogger', 'datalogger.admin.excel');
	
	$nid = $form_state['values']['station'];
	$year = $form_state['values']['year'];	
  $month = $form_state['values']['month'];
  
  $report_BKT = $form_state['values']['report_BKT14'];
  
  if($month < 10){
		$month = '0'.$month;
	}
	$year_month = $year.$month;
  
  if($report_BKT == 0) {
    module_load_include('inc', 'datalogger', 'datalogger.admin.excel.month');
    _datalogger_export_rainfall_month_excel($nid, $year, $year_month, $form_state);
  }
  else {
    module_load_include('inc', 'datalogger', 'datalogger.admin.auto.excel.month');
    _datalogger_auto_export_rainfall_month_excel($nid, $year, $year_month, $form_state);
  }
  
}

function _datalogger_calculate_total_rain_hour_inc(&$nodes) {
  $module_path = drupal_get_path('module', 'datalogger');

  module_load_include('inc', 'datalogger', 'datalogger.admin');
  
  $arg = arg(1);
  
  $names = array();
  
  set_time_limit ( 360);
  
  if (is_object($nodes))  $nodes = array(0 => $nodes);
  
  foreach ($nodes as $node) {
    $v = $node->field_rainfall_measuring[$node->language][0]['value'];
    $smsID = $node->field_rainfall_sms_id[$node->language][0]['value'];
    
    //dpm($nodes);
    
    if ($smsID) {
    
      // lay sms
      global $databases;
      
      $other_database = $databases['default']['default'];
      $other_database['prefix'] = '';
      
      // replace 'YourDatabaseKey' with something that's unique to your module
      Database::addConnectionInfo('YourDatabaseKey', 'default', $other_database);
      db_set_active('YourDatabaseKey');
      
      // Query object
      $query = db_select("inbox", "i");
      
      // Adding fields    
      $query->fields('i', array('ID', 'UpdatedInDB','SenderNumber', 'RecipientID', 'TextDecoded', 'Processed'));
      
      $query->condition('i.ID', $smsID, '=');
     
      // Pagination
      $query->range(0, 1);
      
      
      // Executing query
      $result = $query->execute();
      
      db_set_active(); // without the paramater means set back to the default for the site
      
      // Looping for filling the table rows
      $row = $result->fetchObject();
      
      $sms = $row->TextDecoded;
      
      //dpm($sms);
      
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
      
      $minute = $node->field_rainfall_time_send_sms['und'][0]['value'] ? 
        intval(substr($node->field_rainfall_time_send_sms['und'][0]['value'], -4, 1))  // 0, 1, 2, 3, 4, 5
        : 7;
      $op_idx = ($minute == 0 ? 1 : 7 - $minute);
      
      $node->field_rainfall_total_hour['und'][0]['value'] = $total_rain_hour / 10;
      $node->field_rainfall_op_idx['und'][0]['value'] = $op_idx;
      
      node_save($node);
    } 
  }
  return 'helloworld';
}

















