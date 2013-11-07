<?php
/*BKT14 Tự động*/
function _datalogger_auto_export_rainfall_month_excel($nid, $year, $month, $form_state){
   //error_reporting(E_ALL ^ E_NOTICE);
  error_reporting(0);
  require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel.php';

	module_load_include('inc', 'datalogger', 'datalogger.admin');
  
  $co_mua = false;
  
  require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel/IOFactory.php';
  $name_qtv = $form_state['values']['txtNameQTV'];
	$name_nlb = $form_state['values']['txtNameNLB'];
	$name_ks = $form_state['values']['txtNameKS'];
	$name_pt = $form_state['values']['txtNamePT'];
  
	if($nid != ''){
		$name_station = node_load($nid);
	}
	else{
		$name_station = '';
	}
  
	$dem_luong_mua_trong_dot = array();
	//lay du lieu cua tram
	$month_days_number = cal_days_in_month(CAL_GREGORIAN, substr($month, -2), $year);
  // We get railfall here
  
  $query = 'SELECT DISTINCT f.field_rainfall_send_time_value, f.entity_id, fm.field_rainfall_measuring_value, n.title
                    FROM {field_data_field_rainfall_send_time} f ';
  $query .= ' LEFT JOIN {field_data_field_rainfall_station} fs ON fs.entity_id=f.entity_id';
  //field_data_field_rainfall_measuring
  $query .= ' LEFT JOIN {field_data_field_rainfall_measuring} fm ON fm.entity_id=f.entity_id';
  $query .= ' LEFT JOIN {node} n ON n.nid=f.entity_id';
  $query .= '  WHERE f.bundle = \'rainfall\' AND f.entity_type = \'node\'  AND n.type=\'rainfall\'';
  $query .= '  AND fs.field_rainfall_station_nid = '. $nid;
  $time_min = mktime(0,10,0,substr($month, -2), 1,$year);
  $month_days_number = cal_days_in_month(CAL_GREGORIAN, substr($month, -2), $year);
  $time_max = mktime(24,00,00,substr($month, -2), $month_days_number,$year);
  
  if(isset($time_min)&&isset($time_max)) $query .= '  AND f.field_rainfall_send_time_value >= '.$time_min .' AND  f.field_rainfall_send_time_value <= '.$time_max;
  $query .= ' GROUP BY f.field_rainfall_send_time_value';
  // Result is returned as a iterable object that returns a stdClass object on each iteration
  $result = db_query($query);
  
  $in_array = array();
  $hours_miss_sms = array();
  
  $rainfall_in_ten_minutes = array();
  
  foreach ($result as $record) {
    $day =  intval(date('d',$record->field_rainfall_send_time_value));
    
    $hour = (int)substr($record->title, -8, 2); 
    $minute = (int)substr($record->title, -5, 2);
    
    if ($hour == '00' && $minute == '00') {           // first hour minute -> chuyen sang ngay hom truoc
      $day--;
      if ($day <1 ) continue;
      $record->title = sprintf("%02d/%02d/%04d 23:60:00", $day, $month, $year);
    }     
    
    if (!isset($in_array[$day])) $in_array[$day] = array();
    $in_array[$day][$record->entity_id] = $record;
    $hours_miss_sms[$day][$hour] = $record;
    $rainfall_in_ten_minutes[$day]["$hour-$minute"] = $record->field_rainfall_measuring_value ;
  }
  //dpm($in_array); return;
	for($day = 1; $day <= $month_days_number; $day++){
    
    $dem_luong_mua_trong_dot[$day-1] = array();
	
		$day_time = ($day) < 10 ? '0'.($day) : $day;
		
		$max_rainfall_in_day = 0;
		$max_total_rainfall_in_day = 0;
		$max_total_time_rainfall_in_day = 0;
		$hours_in_day = array();
    $month_day = $month.$day_time;
    
		if(count($in_array) > 0){
      $co_mua = true;
      
			foreach($in_array[$day] as $rainfall){
     
				$hour = (int)substr($rainfall->title, -8, 2); 
        $minute = (int)substr($rainfall->title, -5, 2);
        $second = (int)substr($rainfall->title, -2, 2);
        if($minute ==0 && $second == 0){
          if($hour == 0) $hour = '23';
          else $hour -=1;
        }
        array_push($dem_luong_mua_trong_dot[$day-1],0);
				$rainfall_measuring = (float)$rainfall->field_rainfall_measuring_value;
        
				if($rainfall_measuring > 0){
          $vitri = count($dem_luong_mua_trong_dot[$day-1]);
          $dem_luong_mua_trong_dot[$day-1][$vitri] = $rainfall_measuring;
					$hours_in_day[$hour][] = $rainfall;
					$total_rainfall_in_day += $rainfall_measuring;
					$time_rainfall_in_day +=10;
					$max_total_time_rainfall_in_day = ($max_total_time_rainfall_in_day < $time_rainfall_in_day) ? $time_rainfall_in_day:$max_total_time_rainfall_in_day;
					$max_total_rainfall_in_day = ($max_total_rainfall_in_day < $total_rainfall_in_day) ? $total_rainfall_in_day :$max_total_rainfall_in_day;
				}
				else{
					$total_rainfall_in_day = 0;
					$time_rainfall_in_day =0;
				}
			}
      
			for($i = 0; $i <= 23; $i++) {
				$number = count($hours_in_day[$i]);
				$total_rainfall_in_hour = 0;
				$time_rainfall_in_hour = 0;
				$max_rainfall_in_hour = 0; 
        
        if(!isset($hours_miss_sms[$day][$i])) {
          $hours_in_day[$i] = array(
            'max_rainfall_in_hour' => 0, 
						'total_rainfall_in_hour' => -999,
						'time_rainfall_in_hour' => 0);
        }
        
        foreach($in_array[$day] as $miss_sms) {
          $minute = (int)substr($miss_sms->title, -5, 2);
          $second = (int)substr($miss_sms->title, -2, 2);
          if($minute == 0 && $second == 0 && !isset($hours_miss_sms[$day][$i])){
            $hours_in_day[$i - 1] = array(
              'max_rainfall_in_hour' => 0, 
              'total_rainfall_in_hour' => -999,
              'time_rainfall_in_hour' => 0);
          }
        }
        
				if($number > 0){
					for($j = 0; $j < $number; $j++){
						$rainfall_measuring = (float)$hours_in_day[$i][$j]->field_rainfall_measuring_value ;
						$total_rainfall_in_hour += $rainfall_measuring;
						$time_rainfall_in_hour +=10;
						$max_rainfall_in_hour = ($max_rainfall_in_hour < $rainfall_measuring) ? $rainfall_measuring : $max_rainfall_in_hour; 
					}
					$hours_in_day[$i] = array('max_rainfall_in_hour' => $max_rainfall_in_hour, 
						'total_rainfall_in_hour' => $total_rainfall_in_hour, 
						'time_rainfall_in_hour' => $time_rainfall_in_hour);
				}
			}
		}
		
    $max_rainfall_in_day = $hours_in_day[0]['total_rainfall_in_hour'];
    $max_time_in_hours = $hours_in_day[0]['time_rainfall_in_hour'];
    for($i=0; $i < 24; $i++){
      if($hours_in_day[$i]['total_rainfall_in_hour'] >= $max_rainfall_in_day){
        $max_rainfall_in_day = $hours_in_day[$i]['total_rainfall_in_hour'];
        $max_time_in_hours = $hours_in_day[$i]['time_rainfall_in_hour'];
      }
    }
    
		$day_of_month[$day]= array('max_total_rainfall_in_day' => $max_total_rainfall_in_day,
			'max_total_time_rainfall_in_day' => $max_total_time_rainfall_in_day, 
			'max_rainfall_in_day' => $max_rainfall_in_day,
			'hours_in_day' => $hours_in_day,
      'max_total_time_in_hours' => $max_time_in_hours
      );   
	}
  
	/*-----------------------------------------------------------------------------------------------------------*/
  
  if (!$co_mua) {
    drupal_set_message(t('Rainfall stations in the no data to report'),'error');
    return;
  }
  
  $objPHPExcel = new PHPExcel();
  $objPHPExcel->getProperties()->setCreator("Datalogger")
                 ->setLastModifiedBy("Datalogger")
                 ->setTitle("Datalogger BKT14")
                 ->setSubject("BKT14")
                 ->setDescription("Document for Office 2007 XLSX")
                 ->setKeywords("Datalogger")
                 ->setCategory("Datalogger");
                 
  //-----------------------------------------------------------------------------------------------
	//Sheet 1
	//------------------------------------------------------------------------------------------------
  $styleAlignments = array(
					'alignment' => array(
					'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
          
					)
	);
  $styleAlignments_right = array(
					'alignment' => array(
					'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_RIGHT,
					)
	);
  $styleAlignments_left = array(
					'alignment' => array(
					'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT,
					)
	);
  
  $styleThinBlackBorderOutline = array(
		'borders' => array(
			'outline' => array(
				'style' => PHPExcel_Style_Border::BORDER_THIN,
				'color' => array('argb' => 'FF000000'),
			),
		),
	);
  
  $styleThinBlackBorder_left = array(
		'borders' => array(
			'left' => array(
				'style' => PHPExcel_Style_Border::BORDER_THIN,
				'color' => array('argb' => 'FF000000'),
			),
		),
	);
  
  $styleThinBlackBorder_bottom = array(
		'borders' => array(
			'bottom' => array(
				'style' => PHPExcel_Style_Border::BORDER_THIN,
				'color' => array('argb' => 'FF000000'),
			),
		),
	);
  
  $styleDashedBlackBorder_bottom = array ( //Border -----
    'borders' => array(
			'bottom' => array(
				'style' => PHPExcel_Style_Border::BORDER_HAIR,
				'color' => array('argb' => 'FF000000'),
			),
		),
  );
  
  $styleDoubleBlackBorderOutline = array ( //Border ||
    'borders' => array(
			'outline' => array(
				'style' => PHPExcel_Style_Border::BORDER_DOUBLE,
				'color' => array('argb' => 'FF000000'),
			),
		),
  );
  
  $font_bold = array(
		'font' => array(
		'bold' => true
		)
	);
  
	$objPHPExcel->setActiveSheetIndex(0);
  $objPHPExcel->getActiveSheet()->setTitle('Trang 1');
  
  //font
  $objPHPExcel->getActiveSheet()->getStyle('A1:O40')->getFont()->setName('Times New Roman');
	$objPHPExcel->getActiveSheet()->getStyle('A1:O40')->getFont()->setSize(10);
	
  //thiet lap header
 $objPHPExcel->getActiveSheet()->setCellValue('H1', 'LƯỢNG GIÁNG THUỶ GIỜ QUAN TRẮC TỰ ĐỘNG HÀNG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A3', 'Trạm:'.$name_station->title);
  $objPHPExcel->getActiveSheet()->mergeCells('A3:D3');
  $objPHPExcel->getActiveSheet()->setCellValue('A7', 'Ngày\Giờ:');
	$objPHPExcel->getActiveSheet()->setCellValue('G3', 'Kinh độ:'.$name_station->field_station_longgitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('G3:J3');
  $objPHPExcel->getActiveSheet()->setCellValue('L3', 'Vĩ độ:'.$name_station->field_station_latitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('L3:N3');
	$objPHPExcel->getActiveSheet()->setCellValue('A4', 'Đơn vị:mm');
  $objPHPExcel->getActiveSheet()->mergeCells('A4:B4');
	$objPHPExcel->getActiveSheet()->setCellValue('G4', 'Tháng:'.substr($month,-2));
  $objPHPExcel->getActiveSheet()->mergeCells('G4:H4');
	$objPHPExcel->getActiveSheet()->setCellValue('L4', 'Năm:'.$year);
	$objPHPExcel->getActiveSheet()->mergeCells('L4:M4');
  
  //Thiet lap border
  for($day = 0; $day <= $month_days_number; $day++) {
    for($j = 0; $j <= 14; $j++) {
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($day+7))->applyFromArray($styleDashedBlackBorder_bottom);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($day+7))->applyFromArray($styleThinBlackBorder_left);
      
      if($day % 10 == 0 && $day / 10 != 3) {
        $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($day+7))->applyFromArray($styleThinBlackBorder_bottom);
      }
      
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($month_days_number + 7))->applyFromArray($styleThinBlackBorder_bottom);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($month_days_number + 8))->applyFromArray($styleThinBlackBorder_bottom);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($month_days_number + 8))->applyFromArray($styleThinBlackBorder_left);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($month_days_number + 9))->applyFromArray($styleThinBlackBorder_left);
    }
  }
  $objPHPExcel->getActiveSheet()->getStyle('A7:O'.($month_days_number+9))->applyFromArray($styleDoubleBlackBorderOutline);
  
  //So thu tu gio.
  for($i=0; $i < 14; $i++) {
    $objPHPExcel->getActiveSheet()->setCellValue(chr(66+$i).'7',$i+1);
  }
  
  //Gan gia tri cho cac o  
	$now = getdate();
	$day_in_month = 1;
	for($day=0; $day < $month_days_number; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue('A'.($day+8),$day+1);
		for($j = 0; $j < 14; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			if(!$total) $total = '-';
      
      if($day + 1 == intval($now["mday"]) && $j >= intval($now["hours"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
        $total = '';
      }
      
      if($day + 1 > intval($now["mday"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
        $total = '';
      }
      
			$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).($day+8),$total);
      
      if($total > 0) {
        $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j).($day+8))->getNumberFormat()->setFormatCode('0.0');
      }
		}
		$day_in_month++;
	}
  
  //Tong so dong cuoi thang.
  $objPHPExcel->getActiveSheet()->setCellValue('A'.($month_days_number + 8), 'Tổng số');
  for($j = 0; $j < 14; $j++) {
      $objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).($month_days_number+8), 
      '=SUMIF('.chr(66+$j).'8:'.chr(66+$j).($month_days_number+7).',"<>-999",'.chr(66+$j).'8:'.chr(66+$j).($month_days_number+7).')');
    
  }
  
  $objPHPExcel->getActiveSheet()->getStyle('H1')->applyFromArray($styleAlignments);
	$objPHPExcel->getActiveSheet()->getStyle('A7:O40')->applyFromArray($styleAlignments);
  
  // Chinh do rong cac o
  $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(10);
  for($i=0; $i<14; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(66+$i))->setWidth(5.5);
	}  
  
  for($i=7; $i<39; $i++){
    $objPHPExcel->getActiveSheet()
    ->getRowDimension($i)
    ->setRowHeight(18);
  }
  
  //-----------------------------------------------------------------------------------------------
	//Sheet 2
	//------------------------------------------------------------------------------------------------
  
  $objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(1);
	 $objPHPExcel->getActiveSheet()->setTitle('Trang 2');
	
  //Thiet lap header
  $objPHPExcel->getActiveSheet()->setCellValue('H1', 'LƯỢNG GIÁNG THUỶ GIỜ QUAN TRẮC TỰ ĐỘNG HÀNG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A3', 'Trạm:'.$name_station->title);
  $objPHPExcel->getActiveSheet()->mergeCells('A3:D3');
	$objPHPExcel->getActiveSheet()->setCellValue('G3', 'Kinh độ:'.$name_station->field_station_longgitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('G3:J3');
  $objPHPExcel->getActiveSheet()->setCellValue('L3', 'Vĩ độ:'.$name_station->field_station_latitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('L3:M3');
	$objPHPExcel->getActiveSheet()->setCellValue('A4', 'Đơn vị:mm');
  $objPHPExcel->getActiveSheet()->mergeCells('A4:B4');
	$objPHPExcel->getActiveSheet()->setCellValue('G4', 'Tháng:'.substr($month,-2));
  $objPHPExcel->getActiveSheet()->mergeCells('G4:H4');
	$objPHPExcel->getActiveSheet()->setCellValue('L4', 'Năm:'.$year);
  
  //font
  $objPHPExcel->getActiveSheet()->getStyle('A1:O37')->getFont()->setName('Times New Roman');
	$objPHPExcel->getActiveSheet()->getStyle('A1:O37')->getFont()->setSize(10);
  
  //Thiet lap border
  for($day = 0; $day <= $month_days_number; $day++) {
    for($i = 0; $i <= 12; $i++) {
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($day+7))->applyFromArray($styleDashedBlackBorder_bottom);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($day+7))->applyFromArray($styleThinBlackBorder_left);
      
      if($day % 10 == 0 && $day / 10 != 3) {
        $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($day+7))->applyFromArray($styleThinBlackBorder_bottom);
      }
      
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($month_days_number + 7))->applyFromArray($styleThinBlackBorder_bottom);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($month_days_number + 8))->applyFromArray($styleThinBlackBorder_bottom);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($month_days_number + 8))->applyFromArray($styleThinBlackBorder_left);
      $objPHPExcel->getActiveSheet()->getStyle(chr(65+$i).($month_days_number + 9))->applyFromArray($styleThinBlackBorder_left);
    }
  }
  $objPHPExcel->getActiveSheet()->getStyle('A7:M'.($month_days_number+9))->applyFromArray($styleDoubleBlackBorderOutline);
  
  //Thu tu gio trang 2
  for($i = 0; $i <= 10; $i++) {
    $objPHPExcel->getActiveSheet()->setCellValue(chr(65+$i).'7',$i + 15);
    
    if($i == 10) {
      $objPHPExcel->getActiveSheet()->setCellValue(chr(65 + $i).'7','Tổng số');
      $objPHPExcel->getActiveSheet()->setCellValue(chr(65 + $i + 1).'7','Max 60ph');
      $objPHPExcel->getActiveSheet()->setCellValue(chr(65 + $i + 2).'7','Max 30ph');
    }
  }
  $day_in_month = 1;
	for($day = 0; $day < $month_days_number; $day++){
   
    for($j=14; $j<24; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			if(!$total) $total = '-';
      
      if($day + 1 == intval($now["mday"]) && $j >= intval($now["hours"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
        $total = '';
      }
      
      if($day + 1 > intval($now["mday"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
        $total = '';
      }
      
			$objPHPExcel->getActiveSheet()->setCellValue(chr(65+$j-14).($day+8),$total);
		}
    
    for($j = 0; $j <= 12; $j++) {
      if($total > 0) {
        $objPHPExcel->getActiveSheet()->getStyle(chr(65+$j).($day+8))->getNumberFormat()->setFormatCode('0.0');
      }
    }
    
		$day_in_month++;
	}
  
  //Tong so dong cuoi thang.
  for($j = 0; $j < 10; $j++) {
      $objPHPExcel->getActiveSheet()->setCellValue(chr(65+$j).($month_days_number+8), 
      '=SUMIF('.chr(65+$j).'8:'.chr(65+$j).($month_days_number+7).',"<>-999",'.chr(65+$j).'8:'.chr(65+$j).($month_days_number+7).')');
  }
  
  //In trong muc "Tong so" max30, max 60
  $max_rainfall_60 = array();
  $max_rainfall_30 = array();
  
  for($day = 1; $day <= $month_days_number; $day++) {  
    for($h = 0; $h < 24; $h++) {
      for ($m = 0; $m < 60; $m+=10) {
        
        $max_rainfall_60[$day]["$h-$m"] = 0;
        $max_rainfall_30[$day]["$h-$m"] = 0;
        
        for ($op = 0; $op <=2; $op++) {   // 30 phut
          $h2 = $h;
          $m2 = $m + $op * 10;
          if ($m2 >= 60) {$m2 -= 60; $h2++;}
          
          $max_rainfall_30[$day]["$h-$m"] += $rainfall_in_ten_minutes[$day]["$h2-$m2"]; 
             
        }
        
        for ($op = 0; $op <=5; $op++) {   // 60 phut
          $h2 = $h;
          $m2 = $m + $op * 10;
          
          if ($m2 >= 60) {$m2 -= 60; $h2++;}
          
          $max_rainfall_60[$day]["$h-$m"] += $rainfall_in_ten_minutes[$day]["$h2-$m2"];          
        }
      }  
    }
    
    // tim max 30, 60 trong tung ngay $day
    $max30[$day] = 0;
    foreach ($max_rainfall_30[$day] as $m) {
      if ($max30[$day] < $m) $max30[$day] = $m;
    }
    
    $max60[$day] = 0;
    foreach ($max_rainfall_60[$day] as $m) {
      if ($max60[$day] < $m) $max60[$day] = $m;
    }
    
    //Tong so
    $objPHPExcel->getActiveSheet()->setCellValue('K'.($day+7),
    '=SUM(SUMIF(\'Trang 1\'!B'.($day+7).':O'.($day+7).',"<>-999"),SUMIF(A'.($day+7).':J'.($day+7).',"<>-999"))');
    
    $sum = $objPHPExcel->getActiveSheet()->getCell('K'.($day+7))->getCalculatedValue();
    if($sum == '0') {$objPHPExcel->getActiveSheet()->setCellValue('K'.($day+7),'-');}
    
    //Max 60 phut
    if(!$max60[$day]) {$max60[$day] = '-' ;}
    $objPHPExcel->getActiveSheet()->setCellValue('L'.($day+7), $max60[$day]);
    
    //Max 30 phut
    if(!$max30[$day]) {$max30[$day] = '-' ;}
    $objPHPExcel->getActiveSheet()->setCellValue('M'.($day+7), $max30[$day]);
    
    if($day + 1 > intval($now["mday"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
      $objPHPExcel->getActiveSheet()->setCellValue('K'.($day+7), '');
      $objPHPExcel->getActiveSheet()->setCellValue('L'.($day+7), '');
      $objPHPExcel->getActiveSheet()->setCellValue('M'.($day+7), '');
    }
	}
  
  //Style
  $objPHPExcel->getActiveSheet()->getStyle('H1')->applyFromArray($styleAlignments);
  $objPHPExcel->getActiveSheet()->getStyle('A7:M40')->applyFromArray($styleAlignments);
  
  for($i=0; $i<10; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(65+$i))->setWidth(5,5);
	}
	for($i=0; $i<3; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(75+$i))->setWidth(12);
	}
   for($i=7; $i<39; $i++){
    $objPHPExcel->getActiveSheet()
    ->getRowDimension($i)
    ->setRowHeight(18);
  }
  //-----------------------------------------------------------------------------------------------
	//Sheet 3
	//------------------------------------------------------------------------------------------------
  
  $objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(2);
   $objPHPExcel->getActiveSheet()->setTitle('Trang 3');
	//thiet lap chieu cao cac row
  $objPHPExcel->getActiveSheet()->setCellValue('H6', 'BKT-14');
  $objPHPExcel->getActiveSheet()->mergeCells('H6:I7');
  $objPHPExcel->getActiveSheet()->getStyle('H6')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('H6')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('H6')->getFont()->setSize(11);
  $objPHPExcel->getActiveSheet()->getStyle('H6')->applyFromArray($font_bold);
  
  //font
  $objPHPExcel->getActiveSheet()->getStyle('A1:O46')->getFont()->setName('Times New Roman');
  
	$objPHPExcel->getActiveSheet()->setCellValue('F2', 'BỘ TÀI NGUYÊN VÀ MÔI TRƯỜNG');
	$objPHPExcel->getActiveSheet()->getStyle('F2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F2')->getFont()->setSize(11);
  $objPHPExcel->getActiveSheet()->getStyle('F2')->applyFromArray($font_bold);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F3', 'TRUNG TÂM KHÍ TƯỢNG THUỶ VĂN QUỐC GIA');
	$objPHPExcel->getActiveSheet()->getStyle('F3')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F3')->getFont()->setSize(12);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F9', 'BẢNG SỐ LIỆU KHÍ TƯỢNG TỰ ĐỘNG');
	$objPHPExcel->getActiveSheet()->getStyle('F9')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F9')->getFont()->setSize(18);
  $objPHPExcel->getActiveSheet()->getStyle('F9')->applyFromArray($font_bold);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F10', 'VỀ LƯỢNG MƯA');
	$objPHPExcel->getActiveSheet()->getStyle('F10')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F10')->getFont()->setSize(11);
  $objPHPExcel->getActiveSheet()->getStyle('F10')->applyFromArray($font_bold);
	
	$objPHPExcel->getActiveSheet()->setCellValue('E11', 'THÁNG: ');
  $objPHPExcel->getActiveSheet()->getStyle('E11')->getFont()->setSize(12);
  
  
	$objPHPExcel->getActiveSheet()->setCellValue('F11', substr($month,-2));
  $objPHPExcel->getActiveSheet()->getStyle('F11')->applyFromArray($styleAlignments_left);
	$objPHPExcel->getActiveSheet()->setCellValue('G11', 'NĂM: ');
  $objPHPExcel->getActiveSheet()->getStyle('G11')->getFont()->setSize(12);
  
  
	$objPHPExcel->getActiveSheet()->setCellValue('H11', $year);
  $objPHPExcel->getActiveSheet()->getStyle('H11')->applyFromArray($styleAlignments_left);
	$objPHPExcel->getActiveSheet()->setCellValue('B14', 'Trạm(chữ): ');
  $objPHPExcel->getActiveSheet()->setCellValue('C14', $name_station->title);
	$objPHPExcel->getActiveSheet()->setCellValue('F14', 'Hạng');
	$objPHPExcel->getActiveSheet()->setCellValue('H14', '(Mã số):');
  $objPHPExcel->getActiveSheet()->setCellValue('I14', $name_station->field_station_code['und'][0]['value']);
  
  $tinh = array(
    quang_binh => ('Quảng Bình'),
    quang_tri => ('Quảng Trị'),
    thua_thien_hue => ('Thừa Thiên - Huế'),
    da_nang => ('Đà Nẵng'),
    quang_nam => ('Quảng Nam'),
    quang_ngai => ('Quảng Ngãi'),
  );
  
  $ten_tinh = $tinh[$name_station->field_station_province_city['und'][0]['value']];
  
	$objPHPExcel->getActiveSheet()->setCellValue('B16', 'Tỉnh(T.P): ');
	$objPHPExcel->getActiveSheet()->setCellValue('C16', $ten_tinh);
  $objPHPExcel->getActiveSheet()->setCellValue('E16', 'Huyện(Q): ');
  $objPHPExcel->getActiveSheet()->setCellValue('F16', $name_station->field_station_address['und'][0]['value']);
  
	$objPHPExcel->getActiveSheet()->setCellValue('B18', 'Vĩ độ bắc: ');
  $objPHPExcel->getActiveSheet()->setCellValue('C18', $name_station->field_station_latitude['und'][0]['value']);
  
  $objPHPExcel->getActiveSheet()->setCellValue('F18', '         Kinh độ đông: ');
	$objPHPExcel->getActiveSheet()->setCellValue('H18', $name_station->field_station_longgitude['und'][0]['value']);
  
	$objPHPExcel->getActiveSheet()->setCellValue('B20', 'Độ cao vườn quan trắc trên mặt biển:');
	$objPHPExcel->getActiveSheet()->setCellValue('B22', 'Máy số:');
	$objPHPExcel->getActiveSheet()->setCellValue('E22', 'Kiểu:');
	$objPHPExcel->getActiveSheet()->setCellValue('G22', '      Nước sản xuất:');
	$objPHPExcel->getActiveSheet()->setCellValue('B25', 'Họ, tên quan trắc viên:');
	$objPHPExcel->getActiveSheet()->setCellValue('E25',  $name_qtv );
	$objPHPExcel->getActiveSheet()->setCellValue('B28', 'Ghi chú:');
	$objPHPExcel->getActiveSheet()->setCellValue('B35', 'Họ,tên người lập bảng:');
	$objPHPExcel->getActiveSheet()->setCellValue('E35', $name_nlb);
	
	$objPHPExcel->getActiveSheet()->setCellValue('B37', 'Họ, tên và nhận xét của kiểm soát viên:');
	$objPHPExcel->getActiveSheet()->setCellValue('G37', $name_ks);
	$objPHPExcel->getActiveSheet()->setCellValue('B39', 'Họ, tên và nhận xét của kiểm soát viên lần cuối:');
	$objPHPExcel->getActiveSheet()->setCellValue('G39', $name_pt);
  
  $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(7);
  
  $objPHPExcel->getActiveSheet()->getStyle('A1:J46')->applyFromArray($styleThinBlackBorderOutline);
  
  $objPHPExcel->setActiveSheetIndex(0);   // back to first sheet
  
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  
  header('Content-type: application/vnd.ms-excel');
  header('Content-Disposition: attachment; filename="Báo cáo BKT14 tự động - Trạm ' . $name_station->title . ' - ' . substr($month,-2) . '-' . $year . '.xls"');
  $objWriter->save('php://output');
  drupal_exit();
}

