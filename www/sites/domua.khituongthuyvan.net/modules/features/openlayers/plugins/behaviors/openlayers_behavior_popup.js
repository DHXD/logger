/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Javascript Drupal Theming function for inside of Popups
 *
 * To override
 *
 * @param feature
 *  OpenLayers feature object.
 * @return
 *  Formatted HTML.
 */
Drupal.theme.prototype.openlayersPopup = function(feature) {
  var output = '';

	output += '<div class="menu"><ul>';
	output += '<li><a class="hide" href="/station/'+feature.attributes.title.replace(/\s+/g,'-')+'"><b>Trạm '+feature.attributes.title+'</b></a></li>';
	output += '<li><a class="hide">Thông tin trạm</a>';
	output += '<!--[if lte IE 6]><a>Thông tin trạm<table><tr><td><![endif]-->';
	output += '<ul>';
	output += '<li><a title="ID">ID: <b>'+feature.attributes.field_station_code+'</b></a></li>';
	output += '<li><a title="Địa chỉ">Địa chỉ: <b>'+feature.attributes.field_station_address+'</b></a></li>';
	output += '<li><a title="Kinh độ: '+feature.attributes.field_station_longgitude+'">Kinh độ: <b>'+feature.attributes.field_station_longgitude+'</b></a></li>';
	output += '<li><a title="Vĩ độ: '+feature.attributes.field_station_latitude+'">Vĩ độ: <b>'+feature.attributes.field_station_latitude+'</b></a></li>';
	output += '<li><a title="Số ĐT">Số ĐT: <b>'+feature.attributes.field_station_sim_number+'</b></a></li>';
	output += '</ul>';
	output += '<!--[if lte IE 6]></td></tr></table></a><![endif]-->';
	output += '</li>';
	output += '<li><a class="hide">Trạng thái hoạt động</a>';
	output += '<!--[if lte IE 6]><a>Trạng thái hoạt động<table><tr><td><![endif]-->';
	output += '<ul>';
	output += '<li><a title="Trạng thái: '+feature.attributes.field_station_status+'">Trạng thái: <b>'+feature.attributes.field_station_status+'</b></a></li>';
	output += '<li><a title="Điện áp hiện tại: '+feature.attributes.field_station_power+'V">Điện áp hiện tại: <b>'+feature.attributes.field_station_power+'V</b></a></li>';
	output += '<li><a title="Điện áp ngắt: 11V">Điện áp ngắt: <b>11V</b></a></li>';
	output += '<li><a title="Điện áp phóng: 12V">Điện áp phóng: <b>12V</b></a></li></ul>';
	output += '<!--[if lte IE 6]></td></tr></table></a><![endif]-->';
	output += '</li>';
	output += '<li><a class="hide">Thông tin lượng mưa</a>';
	output += '<!--[if lte IE 6]><a>Thông tin lượng mưa<table><tr><td><![endif]-->';
	output += '<ul>';
	output += '<li><a title="Tình trạng: '+feature.attributes.field_station_alarm_1+'">Tình trạng: <b>'+feature.attributes.field_station_alarm+'</b></a></li>';
	output += '<li><a title="Lượng mưa 24h: '+feature.attributes.field_station_rain_24+'">Lượng mưa 24h: <b>'+feature.attributes.field_station_rain_24+'</b></a></li>';
	output += '<li><a title="Lượng mưa gần nhất: '+feature.attributes.field_station_rain_nearest+'">Lượng mưa gần nhất: <b>'+feature.attributes.field_station_rain_nearest+'</b></a></li>';
	output += '<li><a title="Thời gian truyền tin: '+feature.attributes.field_rainfall_time_send_sms+'">Thời gian truyền tin: <b>'+feature.attributes.field_rainfall_time_send_sms+'</b></a></li>';
	output += '<li><a title="Báo cáo dữ liệu mưa" href="/manage/report/rainfalls/month"><b>Báo cáo dữ liệu mưa</b></a></li>';
	output += '<li><a title="Thống kê số lương mưa theo bảng" href="/views/statistics"><b>Thống kê số lương mưa theo bảng</b></a></li>';
	output += '<li><a title="Thống kê số lượng mưa theo biểu đồ" href="/manager/bieu-do-mua-theo-gio"><b>Thống kê số lượng mưa theo biểu đồ</b></a></li>';
	output += '</ul>';
	output += '<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>';
	output += '</ul></div>';

  return output;
};

// Make sure the namespace exists
Drupal.openlayers.popup = Drupal.openlayers.popup || {};

/**
 * OpenLayers Popup Behavior
 */
Drupal.openlayers.addBehavior('openlayers_behavior_popup', function (data, options) {
  var map = data.openlayers;
  var layers = [];
  var selectedFeature;

  // For backwards compatiability, if layers is not
  // defined, then include all vector layers
  if (typeof options.layers == 'undefined' || options.layers.length == 0) {
    layers = map.getLayersByClass('OpenLayers.Layer.Vector');
  }
  else {
    for (var i in options.layers) {
      var selectedLayer = map.getLayersBy('drupalID', options.layers[i]);
      if (typeof selectedLayer[0] != 'undefined') {
        layers.push(selectedLayer[0]);
      }
    }
  }

  // if only 1 layer exists, do not add as an array.  Kind of a
  // hack, see https://drupal.org/node/1393460
  if (layers.length == 1) {
    layers = layers[0];
  }
	
  var popupSelect = new OpenLayers.Control.SelectFeature(layers,
    {
			hover: true,
      onSelect: function(feature) {
				while( map.popups.length ) {
					map.removePopup(map.popups[0]);
				}
				
        // Create FramedCloud popup.
        popup = new OpenLayers.Popup.FramedCloud(
          'popup',
          feature.geometry.getBounds().getCenterLonLat(),
          null,
          Drupal.theme('openlayersPopup', feature),
          null,
          true,
          function(evt) {
            while( map.popups.length ) {
              map.removePopup(map.popups[0]);
						}
            Drupal.openlayers.popup.popupSelect.unselect(selectedFeature);
          }
        );
				
        // Assign popup to feature and map.
        popup.panMapIfOutOfView = options.panMapIfOutOfView;
        popup.keepInMap = options.keepInMap;
        selectedFeature = feature;
        feature.popup = popup;
				
				OpenLayers.Util.modifyDOMElement(popup.div,null,null,null,null,null,"visible",null);
				OpenLayers.Util.modifyDOMElement(popup.groupDiv,null,null,null,null,null,"visible",null);
								
        Drupal.attachBehaviors();
        map.addPopup(popup);
      }
    }
  );

  map.addControl(popupSelect);
  popupSelect.activate();
  Drupal.openlayers.popup.popupSelect = popupSelect;
	
	var openlayers_views = jQuery("#openlayers-views-map-searchfrm");
	var ctrl_label_div = jQuery('<div/>').attr({id:'olvm-title-div', 
																							name:'olvm-title-div', 
																							style:'float:left; position:relative; margin:4px 5px 0px 0px; font-weight:bold;'
																						})
																			 .html('Tìm kiếm:');
	var ctrl_title_div = jQuery('<div/>').attr({id:'olvm-title-div', name:'olvm-title-div', style:'float:left; position:relative; margin:0px 10px 0px 0px;'});
	var ctrl_title_lbl = jQuery('<label/>').attr({
													for:'olvm-title-txt', 
													style:'position:absolute; top:0; left:0; margin:4px 5px 5px 6px; color:#777; font-weight:normal;'
												}).addClass("label-text").html("Tên trạm");
	var ctrl_title = jQuery('<input/>').attr({type:'text', id:'olvm-title-txt', name:'olvm-title-txt', value:''})
																		 .addClass("form-text");
	openlayers_views.append(ctrl_label_div, ctrl_title_div.append(ctrl_title_lbl, ctrl_title));

	var ctrl_phone_div = jQuery('<div/>').attr({id:'olvm-phone-div', name:'olvm-phone-div', style:'float:left; position:relative; margin:0px 10px 0px 0px;'});
	var ctrl_phone_lbl = jQuery('<label/>').attr({
													for:'olvm-phone-txt', 
													style:'position:absolute; top:0; left:0; margin:4px 5px 5px 6px; color:#777; font-weight:normal;'
												}).addClass("label-text").html("Số điện thoại");
	var ctrl_phone = jQuery('<input/>').attr({type:'text', id:'olvm-phone-txt', name:'olvm-phone-txt', value:''})
																		 .addClass("form-text");
	openlayers_views.append(ctrl_phone_div.append(ctrl_phone_lbl, ctrl_phone));
	
	var ctrl_search_btn = jQuery('<input/>').attr({type: 'button', id:'olvm-search-btn', name:'olvm-search-btn', value:'Tìm kiếm'})
																					.addClass("form-submit")
																					.bind("click", function(event){ btn_search_click(event); });
	openlayers_views.append(ctrl_search_btn);
	
	function btn_search_click(evt){
		var olvm_title_txt = jQuery('input[id=olvm-title-txt]').val();
		var olvm_phone_txt = jQuery('input[id=olvm-phone-txt]').val();
		if(olvm_title_txt!="" || olvm_phone_txt !=""){
			var isFound_title;
			var isFound_phone;
			var features_Found = [];
			
			var selectedLayer = map.getLayersBy('drupalID', 'view_map_stations_view_stations_map');
			if (typeof selectedLayer[0] != 'undefined') {
				for (var i in selectedLayer[0].features) {
					feature = selectedLayer[0].features[i];
					isFound_title = feature.attributes.title.search(new RegExp(olvm_title_txt, "i"));
					isFound_phone = feature.attributes.field_station_sim_number.search(new RegExp(olvm_phone_txt, "g"));
					if((olvm_phone_txt == "" && olvm_title_txt != "" && isFound_title != -1) || 
						 (olvm_title_txt == "" && olvm_phone_txt != "" && isFound_phone != -1) ||
						 (olvm_title_txt != "" && isFound_title != -1 && olvm_phone_txt != "" && isFound_phone != -1)) {	
						features_Found.push(feature);
					}
				}
			}
			
			if(features_Found.length == 1) {
				go_feature(features_Found[0]);
			} else if(features_Found.length > 1) {
				confirm_feature(features_Found);
			}
		}
	}
	
	function confirm_feature(features) {
		feature = features[0];
		jQuery.msgBox({
			title: "Tìm kiếm trạm",
			content: "Tìm thấy trạm "+feature.attributes.title+". Bạn có muốn di chuyển bản đồ đến trạm "+feature.attributes.title+" không?",
			type: "confirm",
			buttons: [{ value: "Yes" }, { value: "No" }, { value: "Cancel"}],
			success: function (result) {
				msgBox_result = result;
				if (result == "Yes") {
					go_feature(feature);
				} else if (result == "No"){
					features.splice(0, 1);
					if(features.length > 0) confirm_feature(features);
				}
			}
		});
	}
	
	function go_feature(feature){
		geo_location = feature.attributes.field_station_geo_location.replace(/[^. 0-9]+/g,'').replace(/^\s+|\s+$/g, '');
		longgitude = geo_location.split(' ')[0];
		latitude = geo_location.split(' ')[1];
		
		// Zoom & center
		var center= new OpenLayers.LonLat(longgitude,latitude - .5).transform(new OpenLayers.Projection("EPSG:4326"),map.getProjectionObject());
		map.setCenter(center, 9);
		
		popupSelect.onSelect(feature);
	}
	
	jQuery('label[for=olvm-title-txt]').inFieldLabels();
	jQuery('label[for=olvm-phone-txt]').inFieldLabels();
	
});
