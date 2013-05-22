(function ($) {


  setTimeout(hamRefreshVector, 3000);
  
  // var hamRefresh = function() {
  
    // // /*
    // $.ajax({
      // url: "/view/stations/map",
      // context: document.body
    // }).done(function(data) { 
      // $('#view-id...').html(data.........);
    // });
		// // */
    
    // alert(1);
    
    
    // setTimeout(hamRefresh, 3000);
    
  // }
	
	// var hamRefreshVector = function() {
        // var map;
        // for (map in Drupal.settings.openlayers.maps) {
					// dsm(map);
          // $.each($('#' + map).data('openlayers')
            // .openlayers.getLayersByClass('OpenLayers.Layer.Vector'),
            // function(i, layer) {
              // layer.redraw();
              // layer.refresh();
							// wfs_layer.refresh();
            // }
          // );
        // }
				// setTimeout(hamRefreshVector, 3000);
      // }
		
	
	underway = new OpenLayers.Layer.Vector("WFS", {
            strategies: [new OpenLayers.Strategy.BBOX(), new OpenLayers.Strategy.Refresh({interval: 4000, force: true})],
            protocol: new OpenLayers.Protocol.WFS({
                url:  "http://root.domua.khituongthuyvan.net/view/stations",
                featureType: "ss2011_v03",
                featureNS: "http://csiro.au/underway",
                geometryName: "position"
            });
  
})(jQuery);