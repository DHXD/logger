
if (AnyTime)

(function ($){
  $(document).ready(function(){
		 $('form#-datalogger-chart-rainfall-form #edit-day, form#-datalogger-chart-rainfall-form #edit-day-to').AnyTime_picker(
		 //AnyTime.picker( "edit-day,edit-day-to",
				{ format: "%d/%m/%Y %H:%i", firstDOW: 1 ,
				labelDayOfMonth: "Ngày trong tháng",
				labelHour: "Giờ",
				labelMinute: "Phút",
				labelMonth: "Tháng",
				labelYear: "Năm",
				labelTitle: "Chọn thời gian"
				} );
			
					
		
	});

  
})(jQuery);