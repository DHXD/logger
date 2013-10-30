<?php

function _datalogger_export_rainfall_month_excel($nid, $year, $month, $form_state){

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
  
  if(isset($time_min)&&isset($time_max)) $query .= '  AND f.field_rainfall_send_time_value >= '.$time_min .' AND  f.field_rainfall_send_time_value <= '.$time_max;
  $query .= ' GROUP BY f.field_rainfall_send_time_value';
  // Result is returned as a iterable object that returns a stdClass object on each iteration
  $result = db_query($query);
  
  $in_array = array();
  $in_array_1 = array();
  
  foreach ($result as $record) {
    $day =  intval(date('d',$record->field_rainfall_send_time_value));
    $hour = (int)substr($record->title, -8, 2); 
    $minute = (int)substr($record->title, -5, 2);



    // dpm($day, "day"); dpm($hour, "hour"); dpm($minute, "minute");
    // if ($day == '01' && $hour == '00') {
      // $day--;
      // $day = date('d',$record->field_rainfall_send_time_value + 3600);
      // break;
    // }



    if ($hour == '00' && $minute == '00') {           // first hour minute -> chuyen sang ngay hom truoc
      $day--;
      if ($day <1 ) continue;
      $record->title = sprintf("%02d/%02d/%04d 23:60:00", $day, $month, $year);
    }
    if (!isset($in_array[$day])) $in_array[$day] = array();
    $in_array[$day][$record->entity_id] = $record;
    
      $in_array_1[$day][$hour] = $record;
    
  }

  
  //dpm($in_array_1); return;
  $time_rainfall_last = $record;
  // $in_array_2 = array();
  //dpm(count($in_array)); return;

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

      //dpm($hours_in_day); return;
			for($i = 23; $i >= 0; $i--) {
				$number = count($hours_in_day[$i]);
        //dpm($number);

     
        $total_rainfall_in_hour = 0;

				$time_rainfall_in_hour = 0;
				$max_rainfall_in_hour = 0;
        
        $in_array_2['day'] = $day;
        $in_array_2['hour'] = $i;
        if(!isset($in_array_1[$day][$i])) {
          $total_rainfall_in_hour = -999;
          dpm($in_array_2);
        }
        
				if($number > 0){
					for($j = 0; $j < $number; $j++){
						$rainfall_measuring = (float)$hours_in_day[$i][$j]->field_rainfall_measuring_value ;
            


            if ($total_rainfall_in_hour < 0) $total_rainfall_in_hour = 0;   // truong hop -999


            //if ($total_rainfall_in_hour < 0) $total_rainfall_in_hour = 0;   // truong hop -999

            
						$total_rainfall_in_hour += $rainfall_measuring;
						$time_rainfall_in_hour += 10;
						$max_rainfall_in_hour = ($max_rainfall_in_hour < $rainfall_measuring) ? $rainfall_measuring : $max_rainfall_in_hour; 
					}
					$hours_in_day[$i] = array('max_rainfall_in_hour' => $max_rainfall_in_hour, 
						'total_rainfall_in_hour' => $total_rainfall_in_hour, 
						'time_rainfall_in_hour' => $time_rainfall_in_hour);
				}

				else{
					$hours_in_day[$i] = array('max_rainfall_in_hour' => 0, 
						'total_rainfall_in_hour' => -999,
						'time_rainfall_in_hour' => 0);
				}
			}
		}
		else{
			for($i = 23; $i >=0; $i--){
				$hours_in_day[$i] = array('max_rainfall_in_hour' => 0, 
						'total_rainfall_in_hour' => '', 
						'time_rainfall_in_hour' => '');

			}
		}
		// else{
			// for($i = 23; $i >=0; $i--){
				// $hours_in_day[$i] = array('max_rainfall_in_hour' => 0, 
						// 'total_rainfall_in_hour' => -999, 
						// 'time_rainfall_in_hour' => 0);
			// }
		// }
    //dpm($dem_luong_mua_trong_dot[$day -1]);
    $max_rainfall_in_day = $hours_in_day[0]['total_rainfall_in_hour'];
    $max_time_in_hours = $hours_in_day[0]['time_rainfall_in_hour'];
    for($i=0; $i < 24; $i++){
      if($hours_in_day[$i]['total_rainfall_in_hour'] >= $max_rainfall_in_day){
        $max_rainfall_in_day = $hours_in_day[$i]['total_rainfall_in_hour'];
        $max_time_in_hours = $hours_in_day[$i]['time_rainfall_in_hour'];
      }
    }
    
		$day_of_month[$day]= array(
      'max_total_rainfall_in_day' => $max_total_rainfall_in_day,
			'max_total_time_rainfall_in_day' => $max_total_time_rainfall_in_day, 
			'max_rainfall_in_day' => $max_rainfall_in_day,
			'hours_in_day' => $hours_in_day,
      'max_total_time_in_hours' => $max_time_in_hours
      );
	}
  // return;
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
  
	$objPHPExcel->setActiveSheetIndex(0);
  
  //
  $objPHPExcel->getActiveSheet()->setCellValue('C1', 'Trạm');
	$objPHPExcel->getActiveSheet()->setCellValue('I1', 'Tháng');
	$objPHPExcel->getActiveSheet()->setCellValue('M1', 'Năm');
	$objPHPExcel->getActiveSheet()->setCellValue('A2', 'Ngày \Giờ');
	//thiet lap header
	for($i =0; $i < 24; $i++){
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'2', $i+1);
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'3', '|');
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'4', $i);
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'5', $i+1);
	}
	$objPHPExcel->getActiveSheet()->setCellValue('AA2', 'Tổng số');
	$objPHPExcel->getActiveSheet()->setCellValue('AA3', 'mm');
	$objPHPExcel->getActiveSheet()->setCellValue('AA4', 'phút');
	$objPHPExcel->getActiveSheet()->setCellValue('AA5', '25');
  $objPHPExcel->getActiveSheet()->setCellValue('Y1', 'Trang 2');
	$k = 1;
	//gan gia tri mac dinh cho cac o

  if($month_days_number == 30) $binhnt_day_of_month = $month_days_number;
  if($month_days_number == 31) $binhnt_day_of_month = $month_days_number - 2;
  else $binhnt_day_of_month = $month_days_number;
	for($i = 0; $i < $binhnt_day_of_month; $i++){
		if($i%2 == 0){
			$objPHPExcel->getActiveSheet()->setCellValue(chr(65).($i+6), $k++);

			for($j = 0; $j < 24; $j++){
				$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($i+6), '-');
        $objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($i+7), '-');
			}

			$objPHPExcel->getActiveSheet()->mergeCells('A'.($i+6).':B'.($i+7));
      
			$objPHPExcel->getActiveSheet()->setCellValue('AA'.($i+6), '=SUMIF(C'.($i+6).':Z'.($i+6).',"<>-999",C'.($i+6).':Z'.($i+6).')');
      $objPHPExcel->getActiveSheet()->setCellValue('AA'.($i+7), '=SUM(C'.($i+7).':Z'.($i+7).')');
		}
	}

	$objPHPExcel->getActiveSheet()->mergeCells('A1:B1');
	$objPHPExcel->getActiveSheet()->mergeCells('Y1:AA1');
  $objPHPExcel->getActiveSheet()->mergeCells('R1:X1');
	$objPHPExcel->getActiveSheet()->mergeCells('A2:B5');
	$objPHPExcel->getActiveSheet()->mergeCells('C1:D1');
	$objPHPExcel->getActiveSheet()->mergeCells('E1:H1');
  $objPHPExcel->getActiveSheet()->mergeCells('O1:Q1');
	$objPHPExcel->getActiveSheet()->mergeCells('I1:J1');
	$styleThinBlackBorderOutline = array(
		'borders' => array(
			'outline' => array(
				'style' => PHPExcel_Style_Border::BORDER_THIN,
				'color' => array('rgb' => '000000'),
			),
		),	
	);

	$sharedStyle1 = new PHPExcel_Style();
	$sharedStyle1->applyFromArray(
		array(
				'borders' => array(
                  'top'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
                  'left'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
									'bottom'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
									'right'		=> array('style' => PHPExcel_Style_Border::BORDER_THIN)
								)
			 ));
	$objPHPExcel->getActiveSheet()->setSharedStyle($sharedStyle1, 'A1:AA35');
  
	//chinh vi tri cac cell 
	$objPHPExcel->getActiveSheet()->getStyle('A1:AA37')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('A1:AA37')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	//thiet lap chieu rong cac cell
	$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(3);
  $objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(3);
  for($c = 'C'; $c <= 'Z'; $c++)
    $objPHPExcel->getActiveSheet()->getColumnDimension($c)->setWidth(3.5);
	
	$objPHPExcel->getActiveSheet()->getColumnDimension('AA')->setWidth(6.8);
  $objPHPExcel->getActiveSheet()->setTitle('Trang 2');
	//thiet lap chieu cao cac row
  $objPHPExcel->getActiveSheet()->getRowDimension('1')->setRowHeight(40);
  for ($i=2;$i<37;$i++) {
    $objPHPExcel->getActiveSheet()->getRowDimension($i)->setRowHeight(19);
  }
	// 
  //font
  $objPHPExcel->getActiveSheet()->getStyle('A1:AA37')->getFont()->setName('Times New Roman');
	$objPHPExcel->getActiveSheet()->getStyle('A1:AA37')->getFont()->setSize(10);
  $objPHPExcel->getActiveSheet()->getStyle('A2:B5')->getFont()->setSize(9);
	//chen du lieu vao bao cao
	$objPHPExcel->getActiveSheet()->setCellValue('E1', $name_station->title);
	$objPHPExcel->getActiveSheet()->setCellValue('K1', substr($month,-2));
	$objPHPExcel->getActiveSheet()->setCellValue('O1',$year);
	
	$font_bold = array(
		'font' => array(
		'bold' => true
		)
	);
	$day_in_month = 1;
  $now = getdate();
  
  //dpm($day_of_month); return;

	for($day = 0; $day < $month_days_number - 1; $day++){
    
		if($day%2 == 0){ 
			for($j = 0; $j < 24 ; $j++){
				$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
				$time = $day_of_month[$day_in_month]['hours_in_day'][$j]['time_rainfall_in_hour'];

        
        if($day/2+1 > intval($now["mday"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
          $total = '';
          $time = '';
          $objPHPExcel->getActiveSheet()->setCellValue('AA'.($day+6), '');
          $objPHPExcel->getActiveSheet()->setCellValue('AA'.($day+7), '');
        }
        if($total == 0) $total = '-';
        if(!$time) $time = '-';


				$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($day+6),$total);
				$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($day+7),$time);
				if($total > 0 && $time > 0){
					$objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($day+6))->applyFromArray($font_bold);
					$objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($day+7))->applyFromArray($font_bold);
          $objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($day+6))->getNumberFormat()->setFormatCode('0.0');
				}
        
        if(!$total)
        {
          dpm("Hehe");
        }       
			}
			$day_in_month++;
		}
	}
  
  $str_rainfall = array();
  $str_time = array();
  for($j = 0; $j < 24; $j++){
		for($i = 0; $i <= 28; $i++){

      $rainfall_in_hour_1 = $objPHPExcel->getActiveSheet()->getCell(chr(67+$j).($i+6))->getValue();
      if($rainfall_in_hour_1 < 0) $rainfall_in_hour_1 = 0;
			if($i % 2 == 0){
        $str_rainfall[$j] += $rainfall_in_hour_1;

        $str_time[$j] += $objPHPExcel->getActiveSheet()->getCell(chr(67+$j).($i+7))->getValue();
			}
		}
	}
  
  //bat dau trang 3
  
  $objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(1);
  $objPHPExcel->getActiveSheet()->setCellValue('C1', 'Trạm');
	$objPHPExcel->getActiveSheet()->setCellValue('I1', 'Tháng');
	$objPHPExcel->getActiveSheet()->setCellValue('M1', 'Năm');
	$objPHPExcel->getActiveSheet()->setCellValue('A2', 'Ngày \Giờ');
  
	//thiet lap header
	for($i =0; $i < 24; $i++){
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'2', $i+1);
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'3', '|');
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'4', $i);
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$i).'5', $i+1);
	}
	$objPHPExcel->getActiveSheet()->setCellValue('AA2', 'Tổng số');
	$objPHPExcel->getActiveSheet()->setCellValue('AA3', 'mm');
	$objPHPExcel->getActiveSheet()->setCellValue('AA4', 'phút');
	$objPHPExcel->getActiveSheet()->setCellValue('AA5', '25');
  
	//gan gia tri mac dinh cho cac o

	for($i = 0; $i < $month_days_number*2 - 30; $i++){  

		if($i%2 == 0){
			$objPHPExcel->getActiveSheet()->setCellValue(chr(65).($i+6), $k++);
			for($j = 0; $j < 24; $j++){
				$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($i+6), '0');
        $objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($i+7), '0'); // i = 31: set value = 0 cho dong 37
			}
			$objPHPExcel->getActiveSheet()->mergeCells('A'.($i+6).':B'.($i+7));
			$objPHPExcel->getActiveSheet()->setCellValue('AA'.($i+6), '=SUMIF(C'.($i+6).':Z'.($i+6).',"<>-999",C'.($i+6).':Z'.($i+6).')');
      $objPHPExcel->getActiveSheet()->setCellValue('AA'.($i+7), '=SUM(C'.($i+7).':Z'.($i+7).')');
		}
	}
  
	$objPHPExcel->getActiveSheet()->setCellValue('A'.($month_days_number+7), 'Tổng số');
	$objPHPExcel->getActiveSheet()->setCellValue('B'.($month_days_number+7), 'mm');
	$objPHPExcel->getActiveSheet()->setCellValue('B'.($month_days_number+8), 'Phút');
  $objPHPExcel->getActiveSheet()->setCellValue('Y1', 'Trang 3');
	
	$objPHPExcel->getActiveSheet()->mergeCells('A'.($month_days_number + 7).':A'.($month_days_number + 8));
	$objPHPExcel->getActiveSheet()->mergeCells('A2:B5');
  $objPHPExcel->getActiveSheet()->mergeCells('A1:B1');
  $objPHPExcel->getActiveSheet()->mergeCells('R1:X1');
  $objPHPExcel->getActiveSheet()->mergeCells('Y1:AA1');
	$objPHPExcel->getActiveSheet()->mergeCells('C1:D1');
	$objPHPExcel->getActiveSheet()->mergeCells('E1:H1');
  $objPHPExcel->getActiveSheet()->mergeCells('O1:Q1');
	$objPHPExcel->getActiveSheet()->mergeCells('I1:J1');
	
	$styleThinBlackBorderOutline = array(
		'borders' => array(
			'outline' => array(
				'style' => PHPExcel_Style_Border::BORDER_THIN,
				'color' => array('argb' => 'FF000000'),
			),
		),	
	);

	$sharedStyle1 = new PHPExcel_Style();
	$sharedStyle1->applyFromArray(
		array(//'fill' 	=> array(
				'borders' => array(
                  'top'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
                  'left'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
									'bottom'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
									'right'		=> array('style' => PHPExcel_Style_Border::BORDER_THIN)
								)
			 ));
	$objPHPExcel->getActiveSheet()->setSharedStyle($sharedStyle1, 'A1:AA'.($month_days_number + 8));

	//chinh vi tri cac cell 
	$objPHPExcel->getActiveSheet()->getStyle('A1:AA'.($month_days_number + 8))->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('A1:AA'.($month_days_number + 8))->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	//thiet lap chieu rong cac cell
	$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(3);
  $objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(4.5);
  for($c = 'C'; $c <= 'Z'; $c++)
    $objPHPExcel->getActiveSheet()->getColumnDimension($c)->setWidth(3.5);
	
	$objPHPExcel->getActiveSheet()->getColumnDimension('AA')->setWidth(6.8);
  $objPHPExcel->getActiveSheet()->setTitle('Trang 3');
	//thiet lap chieu cao cac row
  $objPHPExcel->getActiveSheet()->getRowDimension('1')->setRowHeight(35);
  for ($i=2;$i<37;$i++) { 
    $objPHPExcel->getActiveSheet()->getRowDimension($i)->setRowHeight(16);
  }
  $objPHPExcel->getActiveSheet()->getRowDimension(($month_days_number + 7))->setRowHeight(23);
  $objPHPExcel->getActiveSheet()->getRowDimension(($month_days_number + 8))->setRowHeight(23);
  
	//font
  $objPHPExcel->getActiveSheet()->getStyle('A1:AA'.($month_days_number+8))->getFont()->setName('Times New Roman');
	$objPHPExcel->getActiveSheet()->getStyle('A1:AA'.($month_days_number+8))->getFont()->setSize(10);
  $objPHPExcel->getActiveSheet()->getStyle('A2:B5')->getFont()->setSize(9);
  
	//chen du lieu vao bao cao
	$objPHPExcel->getActiveSheet()->setCellValue('E1', $name_station->title);
	$objPHPExcel->getActiveSheet()->setCellValue('K1', substr($month,-1,2));
	$objPHPExcel->getActiveSheet()->setCellValue('O1',$year);

	$font_bold = array(
		'font' => array(
		'bold' => true
		)
	);
  
	for($i = 15; $i <= $month_days_number ; $i++){
    // binhnt edited here
    $day = ($i - 15) * 2;

    
		for($j = 0; $j < 24 ; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			$time = $day_of_month[$day_in_month]['hours_in_day'][$j]['time_rainfall_in_hour'];
        
      if($i + 1 > intval($now["mday"]) && intval(substr($month,-2)) >= intval($now["mon"])) {
         $total = '';
         $time = '';
         $objPHPExcel->getActiveSheet()->setCellValue('AA'.($day+6), '');
         $objPHPExcel->getActiveSheet()->setCellValue('AA'.($day+7), '');
      }
      if(!$total) $total = '';
      if(!$time) $time = '';
      $objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($day +6),$total);
			$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($day +7),$time);
			if($total > 0 && $time > 0){
				$objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($day+6))->applyFromArray($font_bold);
				$objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($day+7))->applyFromArray($font_bold);
         $objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($day + 6))->getNumberFormat()->setFormatCode('0.0');

			}

		}
		$day_in_month++;

	}

  for($j = 0; $j < 24; $j++){
		for($i = 0 ; $i < $month_days_number; $i++){
      $rainfall_in_hour_2 = $objPHPExcel->getActiveSheet()->getCell(chr(67+$j).($i+6))->getValue();
      if($rainfall_in_hour_2 < 0) $rainfall_in_hour_2 = 0;
			if($i%2 == 0){
        //$str_rainfall[$j] += $objPHPExcel->getActiveSheet()->getCell(chr(67+$j).($i+6))->getValue();
        $str_rainfall[$j] += $rainfall_in_hour_2;
        $str_time[$j] += $objPHPExcel->getActiveSheet()->getCell(chr(67+$j).($i+7))->getValue();
        
			}
		}

		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($month_days_number + 7), $str_rainfall[$j] );
		$objPHPExcel->getActiveSheet()->setCellValue(chr(67+$j).($month_days_number + 8), $str_time[$j] );
    $objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($month_days_number + 7))->applyFromArray($font_bold);
    $objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($month_days_number + 8))->applyFromArray($font_bold);
    if($str_rainfall[$j]>0)
    $objPHPExcel->getActiveSheet()->getStyle(chr(67+$j).($month_days_number + 7))->getNumberFormat()->setFormatCode('0.0');
	}
  $objPHPExcel->getActiveSheet()->getStyle('A'.($month_days_number + 7).':A'.($month_days_number + 8))->getAlignment()->setTextRotation(90);
	//ket thuc trang 3
	
  
	/*++++++++++++++++++tao sheet 2++++++++++++++++++++++++++*/
	$objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(2);
	$objPHPExcel->getActiveSheet()->setCellValue('B1', 'Trạm');
	$objPHPExcel->getActiveSheet()->setCellValue('F1', 'Tháng');
	$objPHPExcel->getActiveSheet()->setCellValue('H1', 'Năm');
	$objPHPExcel->getActiveSheet()->setCellValue('A2', 'LƯỢNG GIÁNG THỦY LỚN NHẤT');
	$objPHPExcel->getActiveSheet()->setCellValue('B3', 'Trong 60 phút');
	$objPHPExcel->getActiveSheet()->setCellValue('D3', 'Trong một đợt liên tục');
	$objPHPExcel->getActiveSheet()->setCellValue('F2', 'CỰC TRỊ TRONG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A3', 'Ngày');
	$objPHPExcel->getActiveSheet()->setCellValue('B4', 'Lượng');
	$objPHPExcel->getActiveSheet()->setCellValue('C4', 'Thời gian');
	$objPHPExcel->getActiveSheet()->setCellValue('D4', 'Lượng');
	$objPHPExcel->getActiveSheet()->setCellValue('E4', 'Thời gian');
	$objPHPExcel->getActiveSheet()->setCellValue('B5', '(mm)');
	$objPHPExcel->getActiveSheet()->setCellValue('C5', 'kéo dài');
	$objPHPExcel->getActiveSheet()->setCellValue('D5', '(mm)');
	$objPHPExcel->getActiveSheet()->setCellValue('E5', 'kéo dài');
	$objPHPExcel->getActiveSheet()->setCellValue('C6', '(phút)');
	$objPHPExcel->getActiveSheet()->setCellValue('E6', '(giờ, phút)');
	$objPHPExcel->getActiveSheet()->setCellValue('H4', 'Lượng');
	$objPHPExcel->getActiveSheet()->setCellValue('I4', 'Thời gian kéo dài');
	$objPHPExcel->getActiveSheet()->setCellValue('H6', '(mm)');
	$objPHPExcel->getActiveSheet()->setCellValue('I6', '(giờ, phút)');
	$objPHPExcel->getActiveSheet()->setCellValue('F8', 'Trong một giờ');
	
	$objPHPExcel->getActiveSheet()->setCellValue('F10', 'Trong một đợt liên tục');
	$objPHPExcel->getActiveSheet()->setCellValue('G13', 'GHI CHÚ');

	$objPHPExcel->getActiveSheet()->setCellValue('B7', '1');
	$objPHPExcel->getActiveSheet()->setCellValue('C7', '2');
	$objPHPExcel->getActiveSheet()->setCellValue('D7', '3');
	$objPHPExcel->getActiveSheet()->setCellValue('E7', '4');
	
	//phan ghi chú:
	$objPHPExcel->getActiveSheet()->setCellValue('F20', 'Quan trắc viên');
	$objPHPExcel->getActiveSheet()->setCellValue('F21', 'Người lập bảng');
	$objPHPExcel->getActiveSheet()->setCellValue('F22', 'Người kiểm soát');
	$objPHPExcel->getActiveSheet()->setCellValue('F23', 'Người phúc thẩm');
  //trang
  $objPHPExcel->getActiveSheet()->setCellValue('J1', 'Trang 4');
	
	
	$objPHPExcel->getActiveSheet()->mergeCells('A2:E2');
	$objPHPExcel->getActiveSheet()->mergeCells('A3:A6');
	$objPHPExcel->getActiveSheet()->mergeCells('B3:C3');
	$objPHPExcel->getActiveSheet()->mergeCells('D3:E3');
	$objPHPExcel->getActiveSheet()->mergeCells('F2:J3');
	$objPHPExcel->getActiveSheet()->mergeCells('F4:G7');
	$objPHPExcel->getActiveSheet()->mergeCells('H4:H5');
	$objPHPExcel->getActiveSheet()->mergeCells('I4:J5');
	$objPHPExcel->getActiveSheet()->mergeCells('I6:J6');
	$objPHPExcel->getActiveSheet()->mergeCells('F8:G9');
	$objPHPExcel->getActiveSheet()->mergeCells('H8:H9');
	$objPHPExcel->getActiveSheet()->mergeCells('I8:J9');
	$objPHPExcel->getActiveSheet()->mergeCells('F10:G11');
	$objPHPExcel->getActiveSheet()->mergeCells('H10:H11');
	$objPHPExcel->getActiveSheet()->mergeCells('I10:J11');

	$styleThinBlackBorderOutline = array(
		'borders' => array(
			'outline' => array(
				'style' => PHPExcel_Style_Border::BORDER_THIN,
				'color' => array('argb' => 'FF000000'),
			),
		),
	);
	$objPHPExcel->getActiveSheet()->getStyle('A2:J'.($month_days_number+7))->applyFromArray($styleThinBlackBorderOutline);

	$objPHPExcel->getActiveSheet()->getStyle('A2:E2')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('A3:A6')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('B3:C3')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('D3:E3')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('B4:B6')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('C4:C6')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('D4:D6')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('E4:E6')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('F4:G7')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('F2:J3')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('H4:H7')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('I4:J7')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('F8:G9')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('H8:H9')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('I8:J9')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('F10:G11')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('H10:H11')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->getStyle('I10:J11')->applyFromArray($styleThinBlackBorderOutline);


	$sharedStyle1 = new PHPExcel_Style();
	$sharedStyle1->applyFromArray(
		array(//'fill' 	=> array(
				'borders' => array(
                  'top'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
                  'left'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
									'bottom'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
									'right'		=> array('style' => PHPExcel_Style_Border::BORDER_THIN)
								)
			 ));

	$objPHPExcel->getActiveSheet()->setSharedStyle($sharedStyle1, 'A7:E'.($month_days_number+7));
	$objPHPExcel->getActiveSheet()->getStyle('B1')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
	$objPHPExcel->getActiveSheet()->getStyle('F1')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_RIGHT);
	$objPHPExcel->getActiveSheet()->getStyle('J1')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
	$objPHPExcel->getActiveSheet()->getStyle('G13')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
	$objPHPExcel->getActiveSheet()->getStyle('A2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);


	$objPHPExcel->getActiveSheet()->getStyle('A3')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('A3')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F2')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F4')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F4')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('H4')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('H4')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('I4')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('I4')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('H8')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('H8')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('I8')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('I8')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('H10')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('H10')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('I10')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('I10')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F8')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F8')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F10')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()->getStyle('F10')->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);

	$styleAlignments = array(
					'alignment' => array(
					'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
					)
	);
  
  $styleAlignmentsLeft = array(
					'alignment' => array(
					'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT,
					)
	);
	$objPHPExcel->getActiveSheet()->getStyle('A2:E165')->applyFromArray($styleAlignments);
  
  $objPHPExcel->getActiveSheet()->getStyle('G1')->applyFromArray($styleAlignmentsLeft);
  $objPHPExcel->getActiveSheet()->getStyle('I1')->applyFromArray($styleAlignmentsLeft);
	
  $objPHPExcel->getActiveSheet()->getStyle('F2:I10')->applyFromArray($styleAlignments);
	$objPHPExcel->getActiveSheet()->getStyle('F2:I10')->applyFromArray(
			array(
					'alignment' => array(
					'horizontal' => PHPExcel_Style_Alignment::VERTICAL_CENTER,
					)
				)
	);
	$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(5);
  for($c = 'C'; $c <= 'E'; $c++)
    $objPHPExcel->getActiveSheet()->getColumnDimension($c)->setWidth(10);
  
  for($c = 'F'; $c <= 'G'; $c++)
    $objPHPExcel->getActiveSheet()->getColumnDimension($c)->setWidth(10);
  
  for($c = 'H'; $c <= 'J'; $c++)
    $objPHPExcel->getActiveSheet()->getColumnDimension($c)->setWidth(8);
	
	//$objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth(8);
	
	//tram do mua
	$objPHPExcel->getActiveSheet()->setCellValue('C1', $name_station->title);
	
	$objPHPExcel->getActiveSheet()->setCellValue('G1',substr($month,-1,2));
	$objPHPExcel->getActiveSheet()->setCellValue('I1', $year);
	$objPHPExcel->getActiveSheet()->setCellValue('H8' , '=MAX(B8:B'.($month_days_number+7).')');
	$objPHPExcel->getActiveSheet()->setCellValue('I8', '=MAX(C8:C'.($month_days_number+7).')');
	$objPHPExcel->getActiveSheet()->setCellValue('H10' , '=MAX(D8:D'.($month_days_number+7).')');
	$objPHPExcel->getActiveSheet()->setCellValue('I10', '=MAX(E8:E'.($month_days_number+7).')');

	$objPHPExcel->getActiveSheet()->setCellValue('H20', $name_qtv);
	$objPHPExcel->getActiveSheet()->setCellValue('H21', $name_nlb);
	$objPHPExcel->getActiveSheet()->setCellValue('H22', $name_ks);
	$objPHPExcel->getActiveSheet()->setCellValue('H23', $name_pt);
	 
	//set font
	$objPHPExcel->getActiveSheet()->getStyle('A1:J'.($month_days_number+7))->getFont()->setName('Times New Roman');
	$objPHPExcel->getActiveSheet()->getStyle('A2:J7')->getFont()->setSize(12);
	$objPHPExcel->getActiveSheet()->getStyle('B8:E'.($month_days_number+7))->getFont()->setSize(12);
	$objPHPExcel->getActiveSheet()->getStyle('B1')->getFont()->setSize(14);
	$objPHPExcel->getActiveSheet()->getStyle('F1:H1')->getFont()->setSize(12);
	$objPHPExcel->getActiveSheet()->getStyle('J1')->getFont()->setSize(10);
	$objPHPExcel->getActiveSheet()->getStyle('F4:G7')->getFont()->setSize(10);
	$objPHPExcel->getActiveSheet()->getStyle('H8:J11')->getFont()->setSize(10);
	$objPHPExcel->getActiveSheet()->getStyle('A8:A'.($month_days_number+7))->getFont()->setSize(12);
	
	$objPHPExcel->getActiveSheet()->setTitle('Trang 4');
	//chen du lieu vao sheet 2007
	for($day = 1; $day <= $month_days_number; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue('A'.(7+$day),$day);
		$objPHPExcel->getActiveSheet()->setCellValue('B'.(7+$day),0);
		$objPHPExcel->getActiveSheet()->setCellValue('C'.(7+$day),0);
		$objPHPExcel->getActiveSheet()->setCellValue('D'.(7+$day),0);
		$objPHPExcel->getActiveSheet()->setCellValue('E'.(7+$day),0);
	}
	$max_mm_in_hours = 0;
  $max_time_in_hours = '';
  $max_mm_in_phase = 0;
  $max_time_in_phase = '';
	for($day = 1; $day <= $month_days_number; $day++){
    if($day_of_month[$day]['max_rainfall_in_day'] < 0) $day_of_month[$day]['max_rainfall_in_day'] = 0;
    if($day_of_month[$day]['max_rainfall_in_day'] >= $max_mm_in_hours) {
      $max_mm_in_hours = $day_of_month[$day]['max_rainfall_in_day'];
      $max_time_in_hours = $day_of_month[$day]['max_total_time_in_hours'];
     }
    if($day_of_month[$day]['max_total_rainfall_in_day']>=$max_mm_in_phase) {
      $max_mm_in_phase = $day_of_month[$day]['max_total_rainfall_in_day'];
      $max_time_in_phase = $day_of_month[$day]['max_total_time_rainfall_in_day'];
     }
		$objPHPExcel->getActiveSheet()->setCellValue('B'.($day+7), $day_of_month[$day]['max_rainfall_in_day'] ? $day_of_month[$day]['max_rainfall_in_day'] : '-');
		$objPHPExcel->getActiveSheet()->setCellValue('C'.($day+7), $day_of_month[$day]['max_total_time_in_hours'] ? $day_of_month[$day]['max_total_time_in_hours'] : '-');
		$objPHPExcel->getActiveSheet()->setCellValue('D'.($day+7), $day_of_month[$day]['max_total_rainfall_in_day'] ? $day_of_month[$day]['max_total_rainfall_in_day'] : '-');
		$objPHPExcel->getActiveSheet()->setCellValue('E'.($day+7), $day_of_month[$day]['max_total_time_rainfall_in_day'] ? $day_of_month[$day]['max_total_time_rainfall_in_day'] : '-');
	}
  // Binhnt add here
   $objPHPExcel->getActiveSheet()->setCellValue('H8' , $max_mm_in_hours);
	$objPHPExcel->getActiveSheet()->setCellValue('I8', $max_time_in_hours);
	$objPHPExcel->getActiveSheet()->setCellValue('H10' , $max_mm_in_phase);
	$objPHPExcel->getActiveSheet()->setCellValue('I10', $max_time_in_phase);
    //return 'aaaa';
	
  $objPHPExcel->setActiveSheetIndex(0);   // back to first sheet
  
	/** Include PHPExcel_IOFactory */  
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  
  //return;
  
  header('Content-type: application/vnd.ms-excel');
  header('Content-Disposition: attachment; filename="Báo cáo dữ liệu mưa theo tháng (BKT14) - Trạm ' . $name_station->title . ' - ' . substr($month,-2,2) . '-' . $year . '.xls"');
  $objWriter->save('php://output');
  //$f = file_directory_temp() . '/Bao cao du lieu mua_' . time() . '.xls';
  //$objWriter->save($f);
  //files($f);
  //drupal_unlink($f);
  
  drupal_exit();
}
////////////////////////

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
  
  if(isset($time_min)&&isset($time_max)) $query .= '  AND f.field_rainfall_send_time_value >= '.$time_min .' AND  f.field_rainfall_send_time_value <= '.$time_max;
  $query .= ' GROUP BY f.field_rainfall_send_time_value';
  // Result is returned as a iterable object that returns a stdClass object on each iteration
  $result = db_query($query);
  
  $in_array = array();
  foreach ($result as $record) {
    $day =  intval(date('d',$record->field_rainfall_send_time_value));
    $hour = (int)substr($record->title, -8, 2); 
    $minute = (int)substr($record->title, -5, 2);
    // dpm($day, "day"); dpm($hour, "hour"); dpm($minute, "minute");
    // if ($day == '01' && $hour == '00') {
      // $day--;
      // $day = date('d',$record->field_rainfall_send_time_value + 3600);
      // break;
    // }
    if ($hour == '00' && $minute == '00') {           // first hour minute -> chuyen sang ngay hom truoc
      $day--;
      if ($day <1 ) continue;
      $record->title = sprintf("%02d/%02d/%04d 23:60:00", $day, $month, $year);
    }     
    
    if (!isset($in_array[$day])) $in_array[$day] = array();
    $in_array[$day][$record->entity_id] = $record;
  }
  //return ;
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
  $font_bold = array(
		'font' => array(
		'bold' => true
		)
	);
	$objPHPExcel->setActiveSheetIndex(0);
  $objPHPExcel->getActiveSheet()->setTitle('Trang 1');
	//thiet lap chieu cao cac row
 $objPHPExcel->getActiveSheet()->setCellValue('H1', 'LƯỢNG GIÁNG THUỶ GIỜ QUAN TRẮC TỰ ĐỘNG HÀNG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A3', 'Trạm:'.$name_station->title);
  $objPHPExcel->getActiveSheet()->mergeCells('A3:D3');
  $objPHPExcel->getActiveSheet()->setCellValue('A7', 'Giờ:');
  $objPHPExcel->getActiveSheet()->getStyle('A7')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->setCellValue('G3', 'Kinh độ:'.$name_station->field_station_longgitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('G3:J3');
  $objPHPExcel->getActiveSheet()->setCellValue('L3', 'Vĩ độ:'.$name_station->field_station_latitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('L3:N3');
	$objPHPExcel->getActiveSheet()->setCellValue('A4', 'Đơn vị:mm');
  $objPHPExcel->getActiveSheet()->mergeCells('A4:B4');
	$objPHPExcel->getActiveSheet()->setCellValue('G4', 'Tháng:'.substr($month,-2,2));
  $objPHPExcel->getActiveSheet()->mergeCells('G4:H4');
	$objPHPExcel->getActiveSheet()->setCellValue('L4', 'Năm:'.$year);
	$objPHPExcel->getActiveSheet()->mergeCells('L4:M4');
	//Thiet lap header
	for($i=0; $i<14; $i++){
		$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$i).'7',$i+1);
    $objPHPExcel->getActiveSheet()->getStyle(chr(66+$i).'7')->applyFromArray($styleThinBlackBorderOutline);
    //$objPHPExcel->getActiveSheet()->getStyle(chr(66+$i).'3')->setWidth(10);
	}
  for($i=0; $i<$month_days_number; $i++){
    $objPHPExcel->getActiveSheet()->setCellValue('A'.($i+8),$i+1);
    $objPHPExcel->getActiveSheet()->getStyle('A'.($i+8),$i+1)->applyFromArray($styleThinBlackBorderOutline);
  }
  
  //Gan gia tri cho cac o

	$day_in_month = 1;
	
	for($day=0; $day<$month_days_number; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue('A'.($day+8),$day+1);
		for($j=0; $j<14; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			if(!$total) $total = '-';
			$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).($day+8),$total);
      $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j).($day+8))->applyFromArray($styleThinBlackBorderOutline);
		}
		$day_in_month++;
	}
  
  $objPHPExcel->getActiveSheet()->getStyle('H1')->applyFromArray($styleAlignments);
  //$objPHPExcel->getActiveSheet()->getStyle('F2')->applyFromArray($styleAlignments_right);
	//$objPHPExcel->getActiveSheet()->getStyle('I2')->applyFromArray($styleAlignments_right);
	$objPHPExcel->getActiveSheet()->getStyle('A7:O38')->applyFromArray($styleAlignments);
  //$objPHPExcel->getActiveSheet()->getStyle('A3:O34')->setWidth(5);
  //$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(7);
  for($i=0; $i<15; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(65+$i))->setWidth(5);
	}
  
  for($i=7; $i<39; $i++){
    $objPHPExcel->getActiveSheet()
    ->getRowDimension($i)
    ->setRowHeight(20);
  }
  //-----------------------------------------------------------------------------------------------
	//Sheet 2
	//------------------------------------------------------------------------------------------------
  
  $objPHPExcel->createSheet();
	$objPHPExcel->setActiveSheetIndex(1);
	 $objPHPExcel->getActiveSheet()->setTitle('Trang 2');
	//thiet lap chieu cao cac row
  $objPHPExcel->getActiveSheet()->setCellValue('H1', 'LƯỢNG GIÁNG THUỶ GIỜ QUAN TRẮC TỰ ĐỘNG HÀNG THÁNG');
	$objPHPExcel->getActiveSheet()->setCellValue('A3', 'Trạm:'.$name_station->title);
  $objPHPExcel->getActiveSheet()->mergeCells('A3:D3');
  $objPHPExcel->getActiveSheet()->setCellValue('A7', 'Giờ:');
  $objPHPExcel->getActiveSheet()->getStyle('A7')->applyFromArray($styleThinBlackBorderOutline);
	$objPHPExcel->getActiveSheet()->setCellValue('G3', 'Kinh độ:'.$name_station->field_station_longgitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('G3:J3');
  $objPHPExcel->getActiveSheet()->setCellValue('L3', 'Vĩ độ:'.$name_station->field_station_latitude['und'][0]['value']);
  $objPHPExcel->getActiveSheet()->mergeCells('L3:M3');
	$objPHPExcel->getActiveSheet()->setCellValue('A4', 'Đơn vị:mm');
  $objPHPExcel->getActiveSheet()->mergeCells('A4:B4');
	$objPHPExcel->getActiveSheet()->setCellValue('G4', 'Tháng:'.substr($month,-2,2));
  $objPHPExcel->getActiveSheet()->mergeCells('G4:H4');
	$objPHPExcel->getActiveSheet()->setCellValue('L4', 'Năm:'.$year);

  //Thiet lap header
  $j = 0;
	for($day=14; $day<24; $day++){
		
		$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).'7',$day+1);
    $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j).'7')->applyFromArray($styleThinBlackBorderOutline);
		$j ++;
	}
  $objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j).'7',"Tổng số");
  $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j).'7')->applyFromArray($styleThinBlackBorderOutline);
  $objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j+1).'7',"Max 60ph");
  $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j+1).'7')->applyFromArray($styleThinBlackBorderOutline);
  $objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j+2).'7',"Max 30ph");
  $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j+2).'7')->applyFromArray($styleThinBlackBorderOutline);
  $day_in_month = 1;
	for($day=0; $day<$month_days_number; $day++){
		$objPHPExcel->getActiveSheet()->setCellValue('A'.($day+8),$day+1);
    $objPHPExcel->getActiveSheet()->getStyle('A'.($day+8))->applyFromArray($styleThinBlackBorderOutline);
   
    for($j=14; $j<24; $j++){
			$total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
			if(!$total) $total = '-';
			$objPHPExcel->getActiveSheet()->setCellValue(chr(66+$j-14).($day+8),$total);
      $objPHPExcel->getActiveSheet()->getStyle(chr(66+$j-14).($day+8))->applyFromArray($styleThinBlackBorderOutline);
		}
		$day_in_month++;
	}
  
  //Lay tong so luong mua
  $day_in_month = 1; 
  $sum_rainfall_in_day = array($month_days_number);
  for($day=0; $day<$month_days_number; $day++){
    
    $sum_rainfall_in_day[$day] = 0;
    for($j=0; $j<24; $j++){
       $total = $day_of_month[$day_in_month]['hours_in_day'][$j]['total_rainfall_in_hour'];
       if(!$total) $total = 0; 
       $sum_rainfall_in_day[$day] += $total;
    }
    $day_in_month++;
  }
  //In trong muc "Tong so" max30, max 60
  for($day = 0; $day < $month_days_number; $day++){	
		$objPHPExcel->getActiveSheet()->setCellValue('M'.($day+8), $day_of_month[$day]['max_rainfall_in_day'] ? $day_of_month[$day]['max_rainfall_in_day'] : '-');
    $objPHPExcel->getActiveSheet()->getStyle('M'.($day+8))->applyFromArray($styleThinBlackBorderOutline);
    
    $objPHPExcel->getActiveSheet()->setCellValue('L'.($day+8), $sum_rainfall_in_day[$day]);
    $objPHPExcel->getActiveSheet()->getStyle('L'.($day+8))->applyFromArray($styleThinBlackBorderOutline);
    
	}
  //Style
  $objPHPExcel->getActiveSheet()->getStyle('H1')->applyFromArray($styleAlignments);
  //$objPHPExcel->getActiveSheet()->getStyle('F2')->applyFromArray($styleAlignments_right);
	//$objPHPExcel->getActiveSheet()->getStyle('I2')->applyFromArray($styleAlignments_right);
  $objPHPExcel->getActiveSheet()->getStyle('A7:N38')->applyFromArray($styleAlignments);
  for($i=0; $i<11; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(65+$i))->setWidth(5);
	}
	for($i=0; $i<3; $i++){
		$objPHPExcel->getActiveSheet()->getColumnDimension(chr(76+$i))->setWidth(10);
	}
   for($i=7; $i<39; $i++){
    $objPHPExcel->getActiveSheet()
    ->getRowDimension($i)
    ->setRowHeight(20);
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
  
	$objPHPExcel->getActiveSheet()->setCellValue('F2', 'BỘ TÀI NGUYÊN VÀ MÔI TRƯỜNG');
	$objPHPExcel->getActiveSheet()->getStyle('F2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F2')->getFont()->setName('Times New Roman');
  $objPHPExcel->getActiveSheet()->getStyle('F2')->getFont()->setSize(11);
  $objPHPExcel->getActiveSheet()->getStyle('F2')->applyFromArray($font_bold);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F3', 'TRUNG TÂM KHÍ TƯỢNG THUỶ VĂN QUỐC GIA');
	$objPHPExcel->getActiveSheet()->getStyle('F3')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F3')->getFont()->setName('Times New Roman');
  $objPHPExcel->getActiveSheet()->getStyle('F3')->getFont()->setSize(12);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F9', 'BẢNG SỐ LIỆU KHÍ TƯỢNG TỰ ĐỘNG');
	$objPHPExcel->getActiveSheet()->getStyle('F9')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F9')->getFont()->setName('Times New Roman');
  $objPHPExcel->getActiveSheet()->getStyle('F9')->getFont()->setSize(18);
  $objPHPExcel->getActiveSheet()->getStyle('F9')->applyFromArray($font_bold);
	
	$objPHPExcel->getActiveSheet()->setCellValue('F10', 'VỀ LƯỢNG MƯA');
	$objPHPExcel->getActiveSheet()->getStyle('F10')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
  $objPHPExcel->getActiveSheet()->getStyle('F10')->getFont()->setName('Times New Roman');
  $objPHPExcel->getActiveSheet()->getStyle('F10')->getFont()->setSize(11);
  $objPHPExcel->getActiveSheet()->getStyle('F10')->applyFromArray($font_bold);
	
	$objPHPExcel->getActiveSheet()->setCellValue('E11', 'THÁNG');
   $objPHPExcel->getActiveSheet()->getStyle('E11')->getFont()->setName('Times New Roman');
  $objPHPExcel->getActiveSheet()->getStyle('E11')->getFont()->setSize(12);
  
  
	$objPHPExcel->getActiveSheet()->setCellValue('F11', substr($month,-2,2));
  $objPHPExcel->getActiveSheet()->getStyle('F11')->applyFromArray($styleAlignments_left);
	$objPHPExcel->getActiveSheet()->setCellValue('G11', 'NĂM');
   $objPHPExcel->getActiveSheet()->getStyle('G11')->getFont()->setName('Times New Roman');
  $objPHPExcel->getActiveSheet()->getStyle('G11')->getFont()->setSize(12);
  
  
	$objPHPExcel->getActiveSheet()->setCellValue('H11', $year);
  $objPHPExcel->getActiveSheet()->getStyle('H11')->applyFromArray($styleAlignments_left);
	$objPHPExcel->getActiveSheet()->setCellValue('B14', 'Trạm(chữ)');
  $objPHPExcel->getActiveSheet()->setCellValue('D14', $name_station->title);
	$objPHPExcel->getActiveSheet()->setCellValue('F14', 'Hạng');
	$objPHPExcel->getActiveSheet()->setCellValue('H14', '(Mã số):');
	$objPHPExcel->getActiveSheet()->setCellValue('B15', 'Tỉnh(T.P)');
	$objPHPExcel->getActiveSheet()->setCellValue('C15', $name_station->field_station_province_city['und'][0]['value']);
  
	$objPHPExcel->getActiveSheet()->setCellValue('B16', 'Vĩ độ bắc :');
	
	$objPHPExcel->getActiveSheet()->setCellValue('B17', 'Độ cao vườn quan trắc trên mặt biển:');
	$objPHPExcel->getActiveSheet()->setCellValue('B18', 'Máy số:');
	$objPHPExcel->getActiveSheet()->setCellValue('E18', 'Kiểu:');
	$objPHPExcel->getActiveSheet()->setCellValue('H18', 'Nước sản xuất:');
	$objPHPExcel->getActiveSheet()->setCellValue('B19', 'Họ, tên quan trắc viên:');
	$objPHPExcel->getActiveSheet()->setCellValue('E19',  $name_qtv );
	$objPHPExcel->getActiveSheet()->setCellValue('B21', 'Ghi chú:');
	$objPHPExcel->getActiveSheet()->setCellValue('B24', 'Họ,tên người lập bảng:');
	$objPHPExcel->getActiveSheet()->setCellValue('E24', $name_nlb);
	
	$objPHPExcel->getActiveSheet()->setCellValue('B26', 'Họ, tên và nhận xét của kiểm soát viên:');
	$objPHPExcel->getActiveSheet()->setCellValue('G26', $name_ks);
	$objPHPExcel->getActiveSheet()->setCellValue('B28', 'Họ, tên và nhận xét của kiểm soát viên lần cuối:');
	$objPHPExcel->getActiveSheet()->setCellValue('G28', $name_pt);
  
  $objPHPExcel->getActiveSheet()->getStyle('A1:J34')->applyFromArray($styleThinBlackBorderOutline);
  
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
  
  //return;
  
  header('Content-type: application/vnd.ms-excel');
  header('Content-Disposition: attachment; filename="Báo cáo dữ liệu mưa theo tháng (BKT14) - Trạm ' . $name_station->title . ' - ' . substr($month,-2,2) . '-' . $year . '.xls"');
  $objWriter->save('php://output');
  //$f = file_directory_temp() . '/Bao cao du lieu mua_' . time() . '.xls';
  //$objWriter->save($f);
  //files($f);
  //drupal_unlink($f);
  
  drupal_exit();
}

