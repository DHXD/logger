<?php

function _datalogger_excel(){
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
  //$rainfalls = views_get_view_result('luong_mua_excel','page', $nid, $month_day);
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
  
  //$time_test = mktime(00,00,00, 9 , 19, $year);
  //dpm($time_test);
  
  // Set station
  //if(isset($query_station)) $query .= '  AND fs.field_rainfall_station_nid = '.$query_station;
  if(isset($time_min)&&isset($time_max)) $query .= '  AND f.field_rainfall_send_time_value >= '.$time_min .' AND  f.field_rainfall_send_time_value <= '.$time_max;
  $query .= ' GROUP BY f.field_rainfall_send_time_value';
  // Result is returned as a iterable object that returns a stdClass object on each iteration
  $result = db_query($query);
  
  //dpm($query);//return;
  
  $in_array = array();
  foreach ($result as $record) {
    //dpm($record->field_rainfall_send_time_value);
    
    $day =  date('d',$record->field_rainfall_send_time_value );
    
    $hour = (int)substr($record->title, -8, 2); 
    $minute = (int)substr($record->title, -5, 2);
    if ($hour == '00' && $minute == '00') {           // first hour minute -> chuyen sang ngay hom truoc
      $day--;
      if ($day <1 ) continue;
      $record->title = sprintf("%02d/%02d/%04d 23:60:00", $day, $month, $year);
    }     
    
    if (!isset($in_array[$day])) $in_array[$day] = array();
    $in_array[$day][$record->entity_id] = $record;
    //dpm($day);
  }
  
  //dpm($in_array);return ;
	for($day = 1; $day <= $month_days_number; $day++){
    
    $dem_luong_mua_trong_dot[$day-1] = array();
		if(($day) < 10){
			$day_time = '0'.($day);
		}
		else{
			$day_time = $day;
		}
    
		$max_rainfall_in_day = 0;
		$max_total_rainfall_in_day = 0;
		$max_total_time_rainfall_in_day = 0;
		$hours_in_day = array();
    $month_day = $month.$day_time;
    //dpm($month_day);
    
    //if ($day == 20) dpm($rainfalls);
    //dpm($rainfalls);
    
		if(count($in_array) > 0){
      $co_mua = true;
      
			foreach($in_array[$day_time] as $rainfall){
     
        //$rainfall = $rainfall[0];
        //dpm($rainfall->node_title);
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
      
      //if ($day == 18) dpm($hours_in_day);
			for($i = 23; $i >= 0; $i--) {
				$number = count($hours_in_day[$i]);
				$total_rainfall_in_hour = 0;
				$time_rainfall_in_hour = 0;
				$max_rainfall_in_hour = 0; 
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
				else{
					$hours_in_day[$i] = array('max_rainfall_in_hour' => 0, 
						'total_rainfall_in_hour' => 0,
						'time_rainfall_in_hour' => 0);
				}
				
			}
		}
		else{
			for($i = 23; $i >=0; $i--){
				$hours_in_day[$i] = array('max_rainfall_in_hour' => 0, 
						'total_rainfall_in_hour' => 0, 
						'time_rainfall_in_hour' => 0);
			}
		}
    //dpm($dem_luong_mua_trong_dot[$day -1]);
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
   //dpm($day_of_month[$day]);
	}
  //dpm($day_of_month[]);
	/*-----------------------------------------------------------------------------------------------------------*/
	//dpm($dem_luong_mua_trong_dot[2]);
  if (!$co_mua) {
    drupal_set_message(t('Rainfall stations in the no data to report'),'error');
    return;
  }
  
  //dpm($day_of_month[18]);  return;
  $objPHPExcel = new PHPExcel();
  $objPHPExcel->getProperties()->setCreator("Datalogger")
				 ->setLastModifiedBy("Datalogger")
                 ->setTitle("Datalogger BKT14")
                 ->setSubject("BKT14")
                 ->setDescription("Document for Office 2007 XLSX")
                 ->setKeywords("Datalogger")
                 ->setCategory("Datalogger");
	//$objPHPExcel->setActiveSheetIndex();
	
	//-----------------------------------------------------------------------------------------------
	//Sheet 1
	//------------------------------------------------------------------------------------------------
	
	$objPHPExcel->setActiveSheetIndex(0);
	$objPHPExcel->getActiveSheet()->setCellValue('H1', 'LƯỢNG GIÁNG THUỶ GIỜ QUAN TRẮC TỰ ĐỘNG HÀNG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A2', 'Trạm:');
	$objPHPExcel->getActiveSheet()->setCellValue('B2', $name_station->title);
	$objPHPExcel->getActiveSheet()->setCellValue('D2', 'Kinh độ:');
	$objPHPExcel->getActiveSheet()->setCellValue('F2', $name_station->field_station_longgitude['und'][0]['value']);
	$objPHPExcel->getActiveSheet()->setCellValue('G2', 'Vĩ độ:');
	$objPHPExcel->getActiveSheet()->setCellValue('I2', $name_station->field_station_latitude['und'][0]['value']);
	$objPHPExcel->getActiveSheet()->setCellValue('J2', 'Đơn vị:');
	
	$objPHPExcel->getActiveSheet()->setCellValue('L2', 'Tháng:');
	$objPHPExcel->getActiveSheet()->setCellValue('M2', substr($month,-2,2));
	$objPHPExcel->getActiveSheet()->setCellValue('N2', 'Năm:');
	$objPHPExcel->getActiveSheet()->setCellValue('O2', $year);
	
	//Thiet lap header
	for($i=0; $i<14; $i++){
		$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$i).'3',$i+1);
		//$objPHPExcel->getActiveSheet()->getStyle(chr(66+$i).'3')->setWidth(10);
	}
	//Gan gia tri cho cac o

	$day_in_month = 1;
	
	for($day=0; $day<$month_days_number; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue('A'.($day+4),$day+1);
		for($j=0; $j<14; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			if(!$total) $total = '-';
			$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).($day+4),$total);
		}
		$day_in_month++;
	}
	
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
	
	$objPHPExcel->getActiveSheet()->getStyle('A3:O34')->applyFromArray($styleAlignments);
	$objPHPExcel->getActiveSheet()->getStyle('F2')->applyFromArray($styleAlignments_right);
	$objPHPExcel->getActiveSheet()->getStyle('I2')->applyFromArray($styleAlignments_right);
	//$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(5);
	for($i=0; $i<15; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(65+$i))->setWidth(5);
	}
	
	//-----------------------------------------------------------------------------------------------
	//Sheet 2
	//------------------------------------------------------------------------------------------------
	
	$objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(1);
	$objPHPExcel->getActiveSheet()->setCellValue('H1', 'LƯỢNG GIÁNG THUỶ GIỜ QUAN TRẮC TỰ ĐỘNG HÀNG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A2', 'Trạm:');
	$objPHPExcel->getActiveSheet()->setCellValue('D2', 'Kinh độ:');
	$objPHPExcel->getActiveSheet()->setCellValue('F2', 'Vĩ độ:');
	$objPHPExcel->getActiveSheet()->setCellValue('H2', 'Đơn vị:');
	$objPHPExcel->getActiveSheet()->setCellValue('J2', 'Tháng:');
	$objPHPExcel->getActiveSheet()->setCellValue('L2', 'Năm:');
	
	//Thiet lap header
	$j = 0;
	for($day=14; $day<24; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).'3',$day+1);
		$j ++;
	}
	|$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j+1).'3',"Tổng số");
	$day_in_month = 1;
	for($day=0; $day<$month_days_number; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue('A'.($day+4),$day+1);
    for($j=14; $j<24; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			if(!$total) $total = '-';
			$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j-14).($day+4),$total);
		}
		$day_in_month++;
	}
  
	$objPHPExcel->getActiveSheet()->getStyle('A3:N34')->applyFromArray($styleAlignments);
	
	for($i=0; $i<11; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(65+$i))->setWidth(7);
	}
	for($i=0; $i<3; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(76+$i))->setWidth(10);
	}
	
	//-----------------------------------------------------------------------------------------------
	//Sheet 3
	//------------------------------------------------------------------------------------------------
	$objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(2);
	$objPHPExcel->getActiveSheet()->setCellValue('F1', 'BỘ TÀI NGUYÊN VÀ MÔI TRƯỜNG');
	$objPHPExcel->getActiveSheet()->getStyle('F1')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F2', 'TRUNG TÂM KHÍ TƯỢNG THUỶ VĂN QUỐC GIA');
	$objPHPExcel->getActiveSheet()->getStyle('F2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F6', 'BẢNG SỐ LIỆU KHÍ TƯỢNG TỰ ĐỘNG');
	$objPHPExcel->getActiveSheet()->getStyle('F6')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F7', 'VỀ LƯỢNG MƯA');
	$objPHPExcel->getActiveSheet()->getStyle('F7')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	
	$objPHPExcel->getActiveSheet()->setCellValue('E8', 'THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('F8', substr($month,-2,2));
	$objPHPExcel->getActiveSheet()->setCellValue('G8', 'NĂM');
	$objPHPExcel->getActiveSheet()->setCellValue('H8', $year);
	
	$objPHPExcel->getActiveSheet()->setCellValue('B11', 'Trạm(chữ)');
	$objPHPExcel->getActiveSheet()->setCellValue('D11', $name_station->title);
	$objPHPExcel->getActiveSheet()->setCellValue('F11', 'Hạng');
	$objPHPExcel->getActiveSheet()->setCellValue('H11', '(Mã số):');
	$objPHPExcel->getActiveSheet()->setCellValue('B12', 'Tỉnh(T.P)');
	$objPHPExcel->getActiveSheet()->setCellValue('C12', $name_station->field_station_province_city['und'][0]['value']);
	
	$objPHPExcel->getActiveSheet()->setCellValue('B13', 'Vĩ độ bắc :');
	
	$objPHPExcel->getActiveSheet()->setCellValue('B14', 'Độ cao vườn quan trắc trên mặt biển:');
	$objPHPExcel->getActiveSheet()->setCellValue('B15', 'Máy số:');
	$objPHPExcel->getActiveSheet()->setCellValue('E15', 'Kiểu:');
	$objPHPExcel->getActiveSheet()->setCellValue('H15', 'Nước sản xuất:');
	$objPHPExcel->getActiveSheet()->setCellValue('B16', 'Họ, tên quan trắc viên:');
	$objPHPExcel->getActiveSheet()->setCellValue('E16',  $name_qtv );
	$objPHPExcel->getActiveSheet()->setCellValue('B18', 'Ghi chú:');
	$objPHPExcel->getActiveSheet()->setCellValue('B21', 'Họ,tên người lập bảng:');
	$objPHPExcel->getActiveSheet()->setCellValue('E21', $name_nlb);
	
	$objPHPExcel->getActiveSheet()->setCellValue('B23', 'Họ, tên và nhận xét của kiểm soát viên:');
	$objPHPExcel->getActiveSheet()->setCellValue('G23', $name_ks);
	$objPHPExcel->getActiveSheet()->setCellValue('B25', 'Họ, tên và nhận xét của kiểm soát viên lần cuối:');
	$objPHPExcel->getActiveSheet()->setCellValue('G25', $name_pt);
	
	
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
	
	header('Content-Disposition: attachment; filename="Báo cáo dữ liệu mưa theo tháng (BKT14) - Trạm ' . $name_station->title . ' - ' . substr($month,-2,2) . '-' . $year . '.xls"');
	$objWriter->save('php://output');
	//$f = file_directory_temp() . '/Bao cao du lieu mua_' . time() . '.xls';
	//$objWriter->save($f);
	//files($f);
	//drupal_unlink($f);
  
	drupal_exit();
	
}