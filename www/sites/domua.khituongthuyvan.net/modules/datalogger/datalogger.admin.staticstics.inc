<?php


/**
*begin: 11-09-12
*/

function _log_rainfall_form(){
	GLOBAL $station_name;
  $form['logStation'] = array(
     '#type' => 'select',
     '#title' => t('Chọn trạm: '),
     '#options' => $station_name,
		 '#attributes' => array('disabled' => 'disabled'),
  );
  
   $form['logDate'] = array(
     '#type' => 'date_popup',
		 '#attributes' => array('disabled' => 'disabled'),
  );
  
  $form['logSubmit'] = array(
     '#type' => 'submit',
     '#value' => t('Xem nhật ký'),
		 '#attributes' => array('disabled' => 'disabled'),
  );
  
  $headers = array(
    1 => t('STT'),
    2 => t('Thời gian'),
    3 => t('Tên file xử lý'),
    4 => t('Trạng thái'),
    5 => t('Mô tả'),
  );
  
  $rows = array(
    array(1,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(2,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(3,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(4,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(5,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(6,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(7,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(8,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(9,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(10,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(11,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(12,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(13,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(14,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(15,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(16,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(17,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(18,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(19,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(20,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(21,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(22,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(23,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(24,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(25,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(26,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(27,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(28,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
    array(29,'18/08/2012 16:17:00','D:\SETUP\devc++\binutils-2.15.91-20040904-1\include\091253_20120818165827','Thành công',''),    
   
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('Hiện tại không có thông tin trong ngay'),
  );
  
  return $form;
}

