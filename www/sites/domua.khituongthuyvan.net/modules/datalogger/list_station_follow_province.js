//thay doi gia tri tram do mưa theo tinh thanh quan ly
(function ($){
  $(document).ready(function(){
	
		var id_tram = Drupal.settings.datalogger.list_default.id_tram;
		var tinh = Drupal.settings.datalogger.list_default.tinh;
		var b_time = Drupal.settings.datalogger.list_default.b_time;
		var e_time = Drupal.settings.datalogger.list_default.e_time;
		var temp = Drupal.settings.datalogger.list_default.temp;
		if(id_tram) {
			jQuery("select#edit-field-station-province-city-value option")
			 .each(function() { 

					if(jQuery(this).val() == tinh){  
									console.log( jQuery(this).attr("selected"));
								 jQuery(this).attr("selected","true");
					}
					
				//jQuery(this).selected = (jQuery(this).val() == "quang_tri"); 
			 
			 });
			 
			 jQuery("select#edit-field-rainfall-station-nid option")
			 .each(function() {
				 var select = jQuery(this).attr("selected");
				 
				 console.log(select);
				 //alert(jQuery(this).text());
				 //alert(select==true);
				 if(select==true){
							jQuery(this).attr("selected",false);
							
				 }
				 if(jQuery(this).val()==id_tram){  
										//console.log( jQuery(this).attr("selected"));
									 jQuery(this).attr("selected","true");
						}
					//jQuery(this).selected = (jQuery(this).val() == "quang_tri"); 
			 
			 });
			 
			 jQuery("input#edit-field-rainfall-send-time-value-min-datepicker-popup-0").val(b_time.substring(0,10));
			 jQuery("input#edit-field-rainfall-send-time-value-min-timepicker-popup-1").val(b_time.substring(11,16));
			 
			 jQuery("input#edit-field-rainfall-send-time-value-max-datepicker-popup-0").val(e_time.substring(0,10));
			 jQuery("input#edit-field-rainfall-send-time-value-max-timepicker-popup-1").val(e_time.substring(11,16));
			 if(!temp) {
				jQuery( "#edit-submit-search-rainfall" ).trigger( "click" );
			 }
			 
		}
		
	
	//$("#edit-day").attr("value", "New value");
	//alert("ok");
	
    var function_change = function () {
    
      //console.log('change....' + this.id);
      
      var station_id;
      
      switch(this.id) {
      case 'edit-province':
        station_id = '#edit-station';
        break;
      case 'edit-field-station-province-city-value':
        station_id = '#edit-field-rainfall-station-nid';
        break;
      }
      
      var tram = $(station_id);
      
			var filter = $(this).val();
      //console.log('$(this).val()', $(this).val());
			// /*
			var index = tram.val();
			//console.log(index);
			tram.empty();
			
			if(tram.prop) {
				var options = tram.prop('options');
			}
			else {
				var options = tram.attr('options');
			}
      
      //console.log(['Drupal.settings.datalogger.list_station_province', Drupal.settings.datalogger.list_station_province]);
      
			$.each(Drupal.settings.datalogger.list_station_province[filter], function(key, text) {
				var nid = key.substr(4);
				options[options.length] = new Option(text, nid);
			});		
			tram.val(index);
			
			// */
		}
    
    $('select#edit-field-station-province-city-value').change( function_change ).change();
    $('select#edit-province').change( function_change ).change();
  });
})(jQuery);
