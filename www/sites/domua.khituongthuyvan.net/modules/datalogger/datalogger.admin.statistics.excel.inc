<?php
error_reporting(E_ALL);
require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel.php';

function _export_excel($args){
 
	$nid = $args[0];
	$rainfalls = $args[1];
	
  $objPHPExcel = new PHPExcel();
  $objPHPExcel->getProperties()->setCreator("Datalogger")
                 ->setLastModifiedBy("Datalogger")
                 ->setTitle("Office 2007 XLSX Document")
                 ->setSubject("Office 2007 XLSX Document")
                 ->setDescription("Document for Office 2007 XLSX")
                 ->setKeywords("office 2007 openxml php")
                 ->setCategory("Test result file");
								 
  $rowcount = count($rainfalls);
	$limit_row = 152;
	$limit_border = $limit_row + 6;
  $total_page = (int)$rowcount/$limit_row; //mot ngay co 152 rows
  $j = 0;
	$row = 0;
  
  for($page = 0; $page < $total_page; $page++)
  {
		if($page != 0)
    {
      $objPHPExcel->createSheet();
    }
		//sheet luôn bắt đầy từ 0
		//thiet lap cac tieu de tren cac cot
    $objPHPExcel->setActiveSheetIndex($page);
    $objPHPExcel->getActiveSheet()->setCellValue('A1', 'Trạm');
    $objPHPExcel->getActiveSheet()->setCellValue('A2', 'LƯỢNG GIÁNG THỦY');
    $objPHPExcel->getActiveSheet()->setCellValue('A3', 'Ngày');
    $objPHPExcel->getActiveSheet()->setCellValue('B3', 'Lượng quan trắc');
    $objPHPExcel->getActiveSheet()->setCellValue('B5', '(mm)');
    $objPHPExcel->getActiveSheet()->setCellValue('C3', 'Lượng tích lũy');
    $objPHPExcel->getActiveSheet()->setCellValue('C5', '(mm)');
   
		//gom cac cell lai voi nhau
    $objPHPExcel->getActiveSheet()->mergeCells('B1:C1');
    $objPHPExcel->getActiveSheet()->mergeCells('A2:C2');
    $objPHPExcel->getActiveSheet()->mergeCells('A3:A6');
    $objPHPExcel->getActiveSheet()->mergeCells('B3:B4');
    $objPHPExcel->getActiveSheet()->mergeCells('C3:C4');
    $objPHPExcel->getActiveSheet()->mergeCells('B5:B6');
    $objPHPExcel->getActiveSheet()->mergeCells('C5:C6');

		//tao duong vien quanh cac cell
    $styleThinBlackBorderOutline = array(
      'borders' => array(
        'outline' => array(
          'style' => PHPExcel_Style_Border::BORDER_THIN,
          'color' => array('argb' => 'FF000000'),
        ),
      ),
    );
    $objPHPExcel->getActiveSheet()->getStyle('A1:A'.$limit_border)->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('A1:C1')->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('A2:C2')->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('A3:A'.$limit_border)->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('A3:B'.$limit_border)->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('A3:C'.$limit_border)->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('B3:C4')->applyFromArray($styleThinBlackBorderOutline);
    $objPHPExcel->getActiveSheet()->getStyle('A3:C6')->applyFromArray($styleThinBlackBorderOutline);
		for($i = 7; $i <= $limit_border; $i++){
			$objPHPExcel->getActiveSheet()->getStyle('A'.$i.':C'.$i)->applyFromArray($styleThinBlackBorderOutline);
		}

		//can chinh vao giua cho cac cell
		$objPHPExcel->getActiveSheet()->getStyle('A1:C'.$limit_border)->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
    $objPHPExcel->getActiveSheet()->getStyle('A1:C'.$limit_border)->getAlignment()->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
		
		//thiet lap do rong cac cell
    $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(25);
    $objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(18);
    $objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(18);

    $cell = $objPHPExcel->getActiveSheet();
    $i = 7;
    
   /* for(; $row < $limit_row * ($page + 1); $row++)
    {
			if($row > $rowcount){
				break;
			}
      $date = $rainfalls[$row]->node_title;
      $interval = $rainfalls[$row]->field_field_rainfall_measuring[0]['raw']['value'];
      $total = $rainfalls[$row]->field_field_rainfall_total[0]['raw']['value'];
      $cell->setCellValue('A' . $i, $date);
      $cell->setCellValue('B'.$i,$interval);//luong mưa 10'
      $cell->setCellValue('C'.$i,$total);//luong mua mot dot quan trac
      $i++;  
    }*/
       // drupal_exit(); 		
		//ten tram do mua
    if($nid != null){
			$name_station = node_load($nid);
			$objPHPExcel->getActiveSheet()->setCellValue('B1', $name_station->title);
		}
		
    //set font
    $objPHPExcel->getActiveSheet()->getStyle('A1:C'.$limit_border)->getFont()->setName('Times New Roman');
    $objPHPExcel->getActiveSheet()->getStyle('B1')->getFont()->setSize(16);
    $objPHPExcel->getActiveSheet()->getStyle('A1:C6')->getFont()->setSize(14);
    $objPHPExcel->getActiveSheet()->getStyle('A7:C'.$limit_border)->getFont()->setSize(12);
    
    $objPHPExcel->getActiveSheet()->setTitle('Trang '.($page+1));
  }
  //drupal_exit();
  //return "abd";
  $objPHPExcel->setActiveSheetIndex(0);
  require_once 'sites/all/libraries/PHPExcel/Classes/PHPExcel/IOFactory.php';
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
  //$objWriter->save(str_replace('.php', '.xlsx', __FILE__));
  header('Content-type: application/vnd.ms-excel');
  header('Content-Disposition: attachment; filename="Báo cáo dữ liệu mưa - Trạm ' . $name_station->title . ' - ' . date('m',time()) . '-' . date('Y',time()) . '.xlsx"');
  $objWriter->save('php://output');
  drupal_exit(); 
}

