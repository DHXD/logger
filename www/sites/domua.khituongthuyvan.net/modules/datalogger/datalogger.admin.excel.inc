<?php
	error_reporting(E_ALL);
require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel.php';

function _export_excel($args){
	//node id
	$nid = $args[0];
	//year month
	// $year_month = $args[1];
	 $rainfalls = $args[1];
  // $rainfalls = views_get_view_result('view_export_excel',NULL, $nid, $year_month,all);
	//ten nguoi quan trac vien
	$name_qtv = $args[2];
	//ten nguoi lap bao cao
	$name_nlb =$args[3];
	//ten nguoi kiem soat
	$name_ks = $args[4];
	//ten nguoi phuc tham
	$name_pt = $args[5];
	
  $objPHPExcel = new PHPExcel();

  $objPHPExcel->getProperties()->setCreator("Datalogger")
                 ->setLastModifiedBy("Datalogger")
                 ->setTitle("Office 2007 XLSX Document")
                 ->setSubject("Office 2007 XLSX Document")
                 ->setDescription("Document for Office 2007 XLSX")
                 ->setKeywords("office 2007 openxml php")
                 ->setCategory("Test result file");
  
  // $rainfalls = views_get_view_result('view_export_excel',NULL, $nid, $year_month,all);
  //tong so rows
  $rowcount = count($rainfalls);
	$limit_row = 152;
  //tong so sheet
  $total_page = (int)$rowcount/$limit_row + 1; //mot ngay co 152 rows
  $j = 0;
	$row = 0;
  for($page = 0; $page < $total_page; $page++)
  {
		if($page != 0)
    {
      $objPHPExcel->createSheet();
    }
 

  // for( ; $row < $page*$limit_row; $row++){
		// $objPHPExcel->createSheet();
		//sheet luôn bắt đầy từ 0
    $objPHPExcel->setActiveSheetIndex($page);
    //$objPHPExcel->setActiveSheetIndex(0);
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
		$objPHPExcel->getActiveSheet()->setCellValue('F15', 'Quan trắc viên');
    $objPHPExcel->getActiveSheet()->setCellValue('F16', 'Người lập bảng');
    $objPHPExcel->getActiveSheet()->setCellValue('F17', 'Người kiểm soát');
    $objPHPExcel->getActiveSheet()->setCellValue('F18', 'Người phúc thẩm');
    
    
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
    $objPHPExcel->getActiveSheet()->getStyle('A2:J152')->applyFromArray($styleThinBlackBorderOutline);

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
                    'bottom'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
                    'right'		=> array('style' => PHPExcel_Style_Border::BORDER_THIN)
                  )
         ));

    $objPHPExcel->getActiveSheet()->setSharedStyle($sharedStyle1, "A7:E152");
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
    $objPHPExcel->getActiveSheet()->getStyle('A2:E165')->applyFromArray($styleAlignments);
    $objPHPExcel->getActiveSheet()->getStyle('F2:I10')->applyFromArray($styleAlignments);
    $objPHPExcel->getActiveSheet()->getStyle('F2:I10')->applyFromArray(
        array(
            'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::VERTICAL_CENTER,
            )
          )
    );

    $objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(40);
    $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(25);
    $objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(15);
    $objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(15);
    $objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(15);
    $objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(15);
    $objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth(10);
    $objPHPExcel->getActiveSheet()->getColumnDimension('I')->setWidth(15);
    
    

    $cell = $objPHPExcel->getActiveSheet();
    $i = 8;
    for(; $row < $limit_row * ($page + 1); $row++)
    {
			if($row > $rowcount){
				break;
			}
      $date = $rainfalls[$row]->node_title;
      $interval = $rainfalls[$row]->field_field_rainfall_measuring[0]['raw']['value'];
      $total = $rainfalls[$row]->field_field_rainfall_total[0]['raw']['value'];
      $cell->setCellValue('A' . $i, $date);
      $cell->setCellValue('B'.$i,$interval);//luong mưa 10'
      $cell->setCellValue('C'.$i,"10");//thoi gian keo dai 10'
      $cell->setCellValue('D'.$i,$total);//luong mua mot dot quan trac
      $cell->setCellValue('E'.$i,"60");//thoi gian keo dai mot dot quan trac
      $i++;  
    }
    if($nid != null){
			$name_station = node_load($nid);
			//trạm đo mưa
			$objPHPExcel->getActiveSheet()->setCellValue('C1', $name_station->title);
		}
    // //tháng
    // $objPHPExcel->getActiveSheet()->setCellValue('G1', substr($year_month,4,2));
    // //năm
    // $objPHPExcel->getActiveSheet()->setCellValue('I1', substr($year_month,0,4));//tháng
    $objPHPExcel->getActiveSheet()->setCellValue('G1',date('m',time()));
    //năm
    $objPHPExcel->getActiveSheet()->setCellValue('I1', date('Y',time()));
    $objPHPExcel->getActiveSheet()->setCellValue('H8' , '=MAX(B8:B152)');
    $objPHPExcel->getActiveSheet()->setCellValue('I8', '10');//
    $objPHPExcel->getActiveSheet()->setCellValue('H10' , '=MAX(D8:D152)');
    $objPHPExcel->getActiveSheet()->setCellValue('I10', '60');

    $objPHPExcel->getActiveSheet()->setCellValue('G15', "$name_qtv");
    $objPHPExcel->getActiveSheet()->setCellValue('G16', "$name_nlb");
    $objPHPExcel->getActiveSheet()->setCellValue('G17', "$name_ks");
    $objPHPExcel->getActiveSheet()->setCellValue('G18', "$name_pt");
     
    //set font
    $objPHPExcel->getActiveSheet()->getStyle('A1:J152')->getFont()->setName('Times New Roman');
    $objPHPExcel->getActiveSheet()->getStyle('A2:J7')->getFont()->setSize(12);
    $objPHPExcel->getActiveSheet()->getStyle('B8:E152')->getFont()->setSize(12);
    $objPHPExcel->getActiveSheet()->getStyle('B1')->getFont()->setSize(14);
    $objPHPExcel->getActiveSheet()->getStyle('F1:H1')->getFont()->setSize(12);
    $objPHPExcel->getActiveSheet()->getStyle('J1')->getFont()->setSize(10);
    $objPHPExcel->getActiveSheet()->getStyle('F4:G7')->getFont()->setSize(10);
    $objPHPExcel->getActiveSheet()->getStyle('H8:J11')->getFont()->setSize(10);
    $objPHPExcel->getActiveSheet()->getStyle('A8:A152')->getFont()->setSize(12);
    
    $objPHPExcel->getActiveSheet()->setTitle('Trang '.($page+1));
    
  }
  
  $objPHPExcel->setActiveSheetIndex(0);
  
  /** Include PHPExcel_IOFactory */
  require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel/IOFactory.php';
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
  //$objWriter->save(str_replace('.php', '.xlsx', __FILE__));
  
  
  header('Content-type: application/vnd.ms-excel');
  header('Content-Disposition: attachment; filename="Báo cáo dữ liệu mưa - Trạm ' . $name_station->title . ' - ' . date('m',time()) . '-' . date('Y',time()) . '.xlsx"');
  $objWriter->save('php://output');
  drupal_exit();
}