<?php


/* BKT14*/
function _datalogger_report_rainfall_month_form($form, &$form_state) {
	drupal_set_title(t('Report monthly rainfall data (BKT14 form)'));
 
	_datalogger_select_area_province_station($form, $form_state);
  
  
	$form['stations_fieldset']['day']['#access']=FALSE;
	
	$form["submit2"] = array(
      "#type" => "submit",
      "#value" => t("Report"),
      "#submit" => array(
          "_datalogger_report_rainfall_month2_submit"
      ),
  ); 
  return $form;
}

function _datalogger_report_rainfall_month2_submit(&$form, &$form_state) {
	module_load_include('inc', 'datalogger', 'datalogger.admin.excel');
	module_load_include('inc', 'datalogger', 'datalogger.admin.excel.month');
	$nid = $form_state['values']['station'];
	$year = $form_state['values']['year'];	
  $month = $form_state['values']['month'];
  
  //dpm($form_state['values']);
  
	if($month < 10){
		$month = '0'.$month;
	}
	$year_month = $year.$month;
	
	
	// kiem tra trong ham _datalogger_export_rainfall_month_excel().
	///$rainfalls = views_get_view_result('view_export_excel','default', $nid, $year, $year_month, 'all');
	
	/*if(count($rainfalls) <= 0){
		drupal_set_message(t('Rainfall stations in the no data to report'),'error');
	}
	else{*/
		_datalogger_export_rainfall_month_excel($nid, $year, $year_month, $form_state);
	//}
	// */
}

function _datalogger_calculate_total_rain_hour_inc(&$nodes) {
  $module_path = drupal_get_path('module', 'datalogger');

  module_load_include('inc', 'datalogger', 'datalogger.admin');
  
  $arg = arg(1);
  
  $names = array();
  
  set_time_limit ( 360);
  
  //dpm('calc...');
  //dpm($nodes);
  
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

















