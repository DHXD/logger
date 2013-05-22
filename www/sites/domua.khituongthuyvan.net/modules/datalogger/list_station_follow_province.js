//thay doi gia tri tram do mưa theo tinh thanh quan ly
(function ($){
  $(document).ready(function(){
    var function_change = function () {
    
      console.log('change....' + this.id);
      
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
      console.log('$(this).val()', $(this).val());
			// /*
			var index = tram.val();
			console.log(index);
			tram.empty();
			
			if(tram.prop) {
				var options = tram.prop('options');
			}
			else {
				var options = tram.attr('options');
			}
      
      console.log(['Drupal.settings.datalogger.list_station_province', Drupal.settings.datalogger.list_station_province]);
      
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
