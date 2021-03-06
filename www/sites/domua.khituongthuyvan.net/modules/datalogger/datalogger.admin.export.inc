<?php
/***********************************************************************************************/
function _datalogger_ftp_export() {

  $done = (arg(2) == 'done');
  if ($done) return 'Xong.';
  
  $operations = array();
  
  $content_type = "station";
  $all_nodes = node_load_multiple(array(), array('type' => $content_type));
  
  foreach($all_nodes as $station)  {
    $operations[] = array("_datalogger_ftp_txt", array( $station->nid));
  }
  
  //put all that information into our batch array
  $batch = array(
    'operations' => $operations,
    'title' => ('Xuat du lieu ra txt'),
  );
 
  //Get the batch process all ready!
  batch_set($batch);
  
  batch_process('export/ftp/done');
  
  dpm($batch); 
  
  return 'aaa';
 
}

function _datalogger_ftp_txt_export_inc($tram) {
//function _datalogger_ftp_txt_export_inc() {
  //$content_type = "station";
  //$all_nodes = node_load_multiple(array(), array('type' => $content_type));
  //dpm($all_nodes); return '1';
  
  //$tram = 2967; // Ba long.
  //$tram = 2961; // Hoa thanh.
  $query_station = $tram;
  
  $node_station = node_load($query_station);
  
  //dpm ($node_station); return;
  
  $ten = $node_station->title;
  $matram = $node_station->field_station_code['und'][0]['value'];
  $ten_tinh = $node_station->field_station_province_city['und'][0]['value'];
  
  $tinh = array(
    "quang_binh" =>31,
    "quang_tri" => 32,
    "thua_thien_hue" => 33,
    "da_nang" => 04,
    "quang_nam" => 34,
    "quang_ngai" => 35,
    );
  
  $matinh = $tinh[$ten_tinh];
  
  //$d = 6; $m = 11; $y = 2013;
  $now = getdate();
  $h = $now["hours"];
  $d = $now["mday"]; $m = $now["mon"]; $y = $now["year"];
  
  $d_min = $d; $m_min = $m; $y_min = $y;
  
  $time_min = yesterday_time($d_min, $m_min, $y_min)->format('Y-m-d') . "  19:00"; 
  //$time_min = "2014-03-25". "  19:00";
  $time_max = tomorow_time($d, $m, $y)->format('Y-m-d') . "  00:00";
  
  $query = 'SELECT DISTINCT f.field_rainfall_send_time_value, f.entity_id
                FROM datalogger_field_data_field_rainfall_send_time f ';
  $query .= ' LEFT JOIN datalogger_field_data_field_rainfall_station fs ON fs.entity_id=f.entity_id';
  $query .= '  WHERE f.bundle = \'rainfall\' AND f.entity_type = \'node\' ';
 
  
 
  // Set station
  if(isset($query_station)) $query .= '  AND fs.field_rainfall_station_nid = '.$query_station;
  if(isset($time_min)&&isset($time_max)) $query .= '  AND f.field_rainfall_send_time_value >= '.strtotime($time_min) .
                                                   ' AND  f.field_rainfall_send_time_value <= '.strtotime($time_max);
  $query .= ' GROUP BY f.field_rainfall_send_time_value, f.entity_id';
 
  //dpm($query); return;
  
  // Result is returned as a iterable object that returns a stdClass object on each iteration
  $result = db_query($query);
  
  $in_array = array();
  foreach ($result as $record) {
    $in_array[] = $record->entity_id;
  }
  
  $nodes = node_load_multiple($in_array);
  
  $a = array();
  //$nodes_query_title = array();
  $sms_id = array();
  foreach ($nodes as &$node) {
    $a[$node->title] = &$node;
    //$nodes_query_title[$node->title] = 1;
    if(isset($node->field_rainfall_sms_id['und'][0]['value'])) {
      $sms_id[]  = $node->field_rainfall_sms_id['und'][0]['value'];
    }
  }
  
  
  
  if (!count($sms_id)) $sms_id = array(-1);
  $sms_id_s = implode(',', $sms_id);
  
  $sql = "SELECT TextDecoded, ID FROM inbox
          WHERE ID IN ($sms_id_s)";
          
  $result = db_query($sql);
  
  $sms = array();
  foreach ($result as $r) {
    $sms[$r->ID] = $r->TextDecoded;
  }
  
  $ten = convert_vi_to_en($ten);
  
  $out .= sprintf("%06d %s\r\n", $matram, $ten);
  //$out .= " Time   Obs   R24h   R19h   RnOb      Batt         R10p..\r\n";
  
  $rain = array();
  $RnOb = 0;
  $R24h = 0;
  $R19h = 0;
  
  $yesterday = yesterday_time($d, $m, $y);
  $tomorow = tomorow_time($d, $m, $y);
  
  for($i = 1 - 5; $i <=24; $i++) {
    if($i < 0) {
      $d_ = $yesterday->format('d');
      $m_ = $yesterday->format('m');
      $y_ = $yesterday->format('Y');
    }
    elseif ($i == 24) {
      $d_ = $tomorow->format('d');
      $m_ = $tomorow->format('m');
      $y_ = $tomorow->format('Y');
    }
    else {
      $d_ = $d; $m_ = $m; $y_ = $y;
    }
      
    $hour = $i < 0 ? 24+$i : $i;
    $time = sprintf("%02d/%02d/%4d %02d:00:00", $d_, $m_, $y_, ($i == 24 ? 0 : $hour));
    $node = &$a[$time];
    
    $batt = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 133, 3));
    $batt = $batt / 10.0;
    
    $batt = number_format($batt, 1, '.', '') . ($batt < 10 ? '0' : '');
    
    $rain[0] = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 36, 5));
    $rain[1] = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 53, 5));
    $rain[2] = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 70, 5));
    $rain[3] = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 87, 5));
    $rain[4] = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 104, 5));
    $rain[5] = intval(substr($sms[$node->field_rainfall_sms_id['und'][0]['value']], 121, 5));
        
    $RnOb = $rain[0] + $rain[1]+ $rain[2]+ $rain[3]+ $rain[4]+ $rain[5];
    if($i >= 1)
    {
      $R24h = $R24h + $RnOb;
    }
    
    if($i <=19)
    {
      $R19h = $R19h + $RnOb;
    }
    if($i == 20) {
      $R19h = 0;
      $R19h = $R19h + $RnOb;
    }
    if($i > 20) {
      $R19h = $R19h + $RnOb;
    }
    
    if( $i == $h) {
    
      if($h == 0) {
        $h_ = 24;
        $d_ = $yesterday->format('d');
        $m_ = $yesterday->format('m');
        $y_ = $yesterday->format('Y');
      }
      else {
        $h_ = $h;
        $d_ = $d; $m_ = $m; $y_ = $y;
      }   

      $out .= sprintf("%04d%02d%02d%02d0000%5d", $y_, $m_, $d_, $h_, "60");
      $out .= sprintf("%7d", $R24h);
      $out .= sprintf("%7d", $R19h);
      $out .= sprintf("%7d", $RnOb);
      $out .= sprintf("%10sv", $batt);
      $out .= sprintf("%10d%10d%10d%10d%10d%10d\r\n", $rain[0], $rain[1], $rain[2], $rain[3], $rain[4], $rain[5]);
      
    }
  } 
  
  if(intval($h) == 0) {
    $h_ = 24;
    $d_ = $yesterday->format('d');
    $m_ = $yesterday->format('m');
    $y_ = $yesterday->format('Y');
  }
  else {
    $h_ = $h;
    $d_ = $d;
    $m_ = $m;
    $y_ = $y;
  }
 
  $file_path = creat_folder($matinh, $matram, $y_, $m_, $d_);
  write_to_text($out, $matram, $y_, $m_, $d_, $h_, $file_path);
  
  drupal_set_message('Xuat du lieu xong tram ' . $ten);
}

function convert_vi_to_en($str) {
  $str = preg_replace("/( )/", '', $str);
  $str = preg_replace("/(à|á|ạ|ả|ã|â|ầ|ấ|ậ|ẩ|ẫ|ă|ằ|ắ|ặ|ẳ|ẵ)/", 'a', $str);
  $str = preg_replace("/(è|é|ẹ|ẻ|ẽ|ê|ề|ế|ệ|ể|ễ)/", 'e', $str);
  $str = preg_replace("/(ì|í|ị|ỉ|ĩ)/", 'i', $str);
  $str = preg_replace("/(ò|ó|ọ|ỏ|õ|ô|ồ|ố|ộ|ổ|ỗ|ơ|ờ|ớ|ợ|ở|ỡ)/", 'o', $str);
  $str = preg_replace("/(ù|ú|ụ|ủ|ũ|ư|ừ|ứ|ự|ử|ữ)/", 'u', $str);
  $str = preg_replace("/(ỳ|ý|ỵ|ỷ|ỹ)/", 'y', $str);
  $str = preg_replace("/(đ)/", 'd', $str);
  $str = preg_replace("/(À|Á|Ạ|Ả|Ã|Â|Ầ|Ấ|Ậ|Ẩ|Ẫ|Ă|Ằ|Ắ|Ặ|Ẳ|Ẵ)/", 'A', $str);
  $str = preg_replace("/(È|É|Ẹ|Ẻ|Ẽ|Ê|Ề|Ế|Ệ|Ể|Ễ)/", 'E', $str);
  $str = preg_replace("/(Ì|Í|Ị|Ỉ|Ĩ)/", 'I', $str);
  $str = preg_replace("/(Ò|Ó|Ọ|Ỏ|Õ|Ô|Ồ|Ố|Ộ|Ổ|Ỗ|Ơ|Ờ|Ớ|Ợ|Ở|Ỡ)/", 'O', $str);
  $str = preg_replace("/(Ù|Ú|Ụ|Ủ|Ũ|Ư|Ừ|Ứ|Ự|Ử|Ữ)/", 'U', $str);
  $str = preg_replace("/(Ỳ|Ý|Ỵ|Ỷ|Ỹ)/", 'Y', $str);
  $str = preg_replace("/(Đ)/", 'D', $str);
  return $str;
}

function write_to_text($str,$matram, $year, $month, $day, $hour, $file_path)  {
 
  $file_name = sprintf("%06d_%4d-%02d-%02d-%02d.00.00", $matram, $year, $month, $day, $hour);
  $file = fopen($file_path . "\\" . $file_name .".txt", 'w');
  //dpm($file_name);
  fwrite($file, $str);
  fclose($file);
}

function creat_folder($matinh, $matram, $year, $month, $day) {
  $file_path = sprintf("E:\KTTV_3_DataDemo\Success\%02d\%06d\%4d\%02d\%02d", $matinh, $matram, $year, $month, $day);
  
  if(!file_exists($file_path)) {
    mkdir($file_path, 0700, true);
  }
  return $file_path;
}

function yesterday_time($d, $m, $y) {
  $t = sprintf("%02d-%02d-%4d", $d, $m, $y);
  $today = new DateTime($t);
  $interval = new DateInterval('P1D');
  $yesterday = $today->sub($interval);
  //dpm($yesterday, "yesterday");
  return $yesterday;
}

function tomorow_time($d, $m, $y) {
  $t = sprintf("%02d-%02d-%4d", $d, $m, $y);
  $today = new DateTime($t);
  $interval = new DateInterval('P1D');
  $tomorow = $today->add($interval);
  //dpm($tomorow, "tomorow");
  return $tomorow;
}

// Ham tra ve cac nid cua tram
function _datalogger_return_nid($number) {
  $content_type = "station";
  $all_nodes = node_load_multiple(array(), array('type' => $content_type));
  
  foreach ($all_nodes as &$node) {
    if ($node->field_station_sim_number['und'][0]['value'] == $number) {
      return $node->nid;      
    }
  }
  return 0;
}
/****************************************/









