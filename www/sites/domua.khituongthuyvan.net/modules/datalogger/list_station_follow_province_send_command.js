//thay doi gia tri tram do mưa theo tinh thanh quan ly
(function ($){
  $(document).ready(function(){
    $('#edit-field-station-province-city-value').change( function () {
			var filter = $(this).val();
			// /*
			var index2 = $('#edit-field-command-station-nid').val();
			console.log(index2);
      
			$('#edit-field-command-station-nid').empty();
			var tram = $('#edit-field-command-station-nid');
			if(tram.prop) {
				var options = tram.prop('options');
			}
			else {
				var options = tram.attr('options');
			}
			$.each(Drupal.settings.datalogger.list_station_province[filter], function(key, text) {
				var nid = key.substr(4);
				options[options.length] = new Option(text, nid);
			});		
			$('#edit-field-command-station-nid').val(index2);
			// */
		}).change();
  });
})(jQuery);
