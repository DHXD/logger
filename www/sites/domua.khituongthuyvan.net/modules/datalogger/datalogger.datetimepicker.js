
if (AnyTime)

(function ($){
  $(document).ready(function(){

		 $('#edit-day,#edit-day-to').AnyTime_picker(
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