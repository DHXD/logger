<?php
/*
 * Menu page callback.
 */
function _datalogger_map() {
	$view = views_get_view_result('viet_ban_do');
	dpm($view);
	//return $view[0]->field_field_station_longgitude[0]['raw']['safe_value'];
	$s = '';
	
	$external_js = 'http://www.vietbando.com/maps/API/VBDMapAPI.js?key=5lJdIlKaLKfA1UMwdmzgbRYOTt/AH+9wl6NQH/f0pue8AFCloQ7qJw==';
	
  drupal_add_js('document.write(unescape("%3Cscript src=\''. $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');
  
	$html = <<< ABC
  
  <div id="container" style="height:500px; width:930px;position:relative; margin:10px; border:1px #b1c4d5 solid;"></div>
  <div id"test"></div>
  <script>
    loadMap();
  </script>
ABC;

  return $html;
}

function _datalogger_vietbando() {
	$rows = views_get_view_result('viet_ban_do');
	
	
	dpm($rows);
	//echo $rows[0]->field_field_station_alarm;
	//drupal_add_js("var view = " . drupal_to_js($view) . ";", 'inline');

	$external_js = 'http://www.vietbando.com/maps/API/VBDMapAPI.js?key=5lJdIlKaLKfA1UMwdmzgbRYOTt/AH+9wl6NQH/f0pue8AFCloQ7qJw==';
	
  drupal_add_js('document.write(unescape("%3Cscript src=\''. $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');
  	$s = "  function loadMap() {";
	$s .= "var station = [];\n";
	
	foreach ($rows as $k => $row) {
		$s .= <<< MANG
		
		station[$k] = new Object();
		station[$k].tentram = "{$row->node_title}";
		station[$k].matram = {$row->_field_data['nid']['entity']->field_station_code['und'][0]['value']};
		station[$k].status=0;
		//Nid
		station[$k].nid = {$row->nid};
		//Dia chi
		station[$k].diachi = "{$row ->field_field_station_address[0]['rendered']['#markup']}";
		//Lat
		station[$k].lat = {$row->_field_data['nid']['entity']->field_station_geo_location['und'][0]['lat']};
		//Lon
		station[$k].lon = {$row->_field_data['nid']['entity']->field_station_geo_location['und'][0]['lon']};
		//Kinh do
		station[$k].kinhdo = "{$row->_field_data['nid']['entity']->field_station_latitude['und'][0]['value']}";
		//Vi do
		station[$k].vido = "{$row ->_field_data['nid']['entity']->field_station_longgitude['und'][0]['value']}";
		//SDT
		station[$k].sdt = {$row->field_field_station_sim_number[0]['rendered']['#markup']};
		//Dien ap
		station[$k].dienap = '';
		//Luong mua 24h
		station[$k].luongmua24h = "0.0mm";
		//Luong mua gan nhat
		station[$k].luongmuagannhat = "0.0mm";
		//Trang thai
		station[$k].trangthai ={$row->field_field_station_status[0]['rendered']['#access']} ;
					
MANG;
		if(count($row->field_field_station_alarm) == 0){
			//Binh thuong
			$s .= <<< MANG
			
			station[$k].canhbao = 0;
			
MANG;
		}else{
			$s .= <<< MANG
			
			station[$k].canhbao = 1;
			
MANG;
			if($row->field_field_station_alarm[0]['raw']['value'] == 'alarm_rainfall'){
				//Mua
				$s .= <<< MANG
				
				station[$k].canhbaostatus = 0;
			
MANG;
			}else if($row->field_field_station_alarm[0]['raw']['value'] == 'alarm_true'){
				//Mua to	
				$s .= <<< MANG
				
				station[$k].canhbaostatus = 1;
			
MANG;
			}
		}
	}
	
	$s.= <<< MANG2
		if (VBrowserIsCompatible() ) { 
			var map = new VMap(document.getElementById('container'));
			map.setCenter(new VLatLng(16.8152328, 106.680505), 7);
			var markers = [];  
			var contentString = [];
			for(var i =0;i<station.length;i++){
				var pt = new VLatLng(station[i].lat,station[i].lon);
				if(station[i].trangthai==1){
					if(station[i].canhbao == 0){
						markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/orange.png", new VSize(15, 15)), false);
					}else{
						if(station[i].canhbaostatus == 0){
							markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/blue.png", new VSize(15, 15)), false);
						}else{
							markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/red.png", new VSize(15, 15)), false);
						}	
					}
				}else{
					markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/black.png", new VSize(15, 15)), false);
				}
				
		 		map.addOverlay(markers[i]);
				
				contentString[i] = '<div style =" width: 200px; height: 100px"><div id="popup" class="olPopup" style="position: absolute; overflow: visible; width: 190px; height: 100px; z-index: 751;"><div id="popup_GroupDiv" style="position: absolute; overflow: visible; top: 0px; left: 0px; height: 100%; width: 100%;"><div id="popup_contentDiv" class="olFramedCloudPopupContent" style="width: 190px; height: 110px; position: absolute; z-index: 1; left: 0px; top: 0px;"><div class="menu"><ul><li><a class="hide" href="';
				contentString[i] += '/station/Quảng-Hợp';
				contentString[i] += '"><b>Trạm ';
				contentString[i] += station[i].tentram;
				contentString[i] += '</b></a></li><li><a class="hide">Thông tin trạm</a><ul><li><a title="ID">ID: <b>';
				contentString[i] += station[i].matram;
				contentString[i] += '</b></a></li><li><a title="Địa chỉ">Địa chỉ: <b>';
				contentString[i] +=station[i].diachi;
				contentString[i] += '</b></a></li><li><a title="Kinh độ: ';
				contentString[i] += station[i].kinhdo;
				contentString[i] += '">Kinh độ: <b>';
				contentString[i] += station[i].kinhdo;
				contentString[i] += '</b></a></li><li><a title="Vĩ độ: ';
				contentString[i] += station[i].vido;
				contentString[i] += '">Vĩ độ: <b>';
				contentString[i] += station[i].vido;
				contentString[i] +='</b></a></li><li><a title="Số ĐT">Số ĐT: <b>';
				contentString[i] += station[i].sdt;
				contentString[i] += '</b></a></li></ul><!--[if lte IE 6]></td></tr></table></a><![endif]--></li><li><a class="hide">Trạng thái hoạt động</a><!--[if lte IE 6]><a>Trạng thái hoạt động<table><tr><td><![endif]--><ul><li><a title="Trạng thái: Hoạt Động">Trạng thái: <b>';
				contentString[i] += 'Hoạt Động';
				contentString[i] += '</b></a></li><li><a title="Điện áp hiện tại: ';
				contentString[i] += '13.00V';
				contentString[i] += '">Điện áp hiện tại: <b>';
				contentString[i] += '13.00V';
				contentString[i] += '</b></a></li><li><a title="Điện áp ngắt: ';
				contentString[i] += '11V';
				contentString[i] += '">Điện áp ngắt: <b>';
				contentString[i] += '11V';
				contentString[i] += '</b></a></li><li><a title="Điện áp phóng: ';
				contentString[i] += '12V';
				contentString[i] += '">Điện áp phóng: <b>';
				contentString[i]+= '12V';
				contentString[i] += '</b></a></li></ul><!--[if lte IE 6]></td></tr></table></a><![endif]--></li><li><a class="hide">Thông tin lượng mưa</a><!--[if lte IE 6]><a>Thông tin lượng mưa<table><tr><td><![endif]--><ul><li><a title="Tình trạng: ';
				contentString[i] += 'undefined';
				contentString[i] += '">Tình trạng: <b>';
				contentString[i] += 'Không';
				contentString[i] += '</b></a></li><li><a title="Lượng mưa 24h: ';
				contentString[i] += '0.0mm';
				contentString[i] += '">Lượng mưa 24h: <b>';
				contentString[i] += '0.0mm';
				contentString[i] += '</b></a></li><li><a title="Lượng mưa gần nhất: ';
				contentString[i] += '0.0mm';
				contentString[i] += '">Lượng mưa gần nhất: <b>';
				contentString[i] += '0.0mm';
				contentString[i] += '</b></a></li><li><a title="Thời gian truyền tin: ';
				contentString[i] += 'null';
				contentString[i] += '">Thời gian truyền tin: <b>';
				contentString[i] += 'null';
				contentString[i] += '</b></a></li><li><a title="Báo cáo dữ liệu mưa" href="';
				contentString[i] += '/manage/report/rainfalls/month/';
				contentString[i] += station[i].nid;
				contentString[i] += '"><b>Báo cáo dữ liệu mưa</b></a></li><li><a title="Thống kê số lương mưa theo bảng" href="';
				contentString[i] += '/views/statistics/search/';
				contentString[i] += station[i].nid;
				contentString[i] += '"><b>Thống kê số lương mưa theo bảng</b></a></li><li><a title="Thống kê số lượng mưa theo biểu đồ" href="';
				contentString[i] += '/manager/bieu-do-mua-theo-gio';
				contentString[i] += '"><b>Thống kê số lượng mưa theo biểu đồ</b></a></li></ul><!--[if lte IE 6]></td></tr></table></a><![endif]--></li></ul></div></div></div></div>';
				  contentString[i] += '';
				/*
				contentString[i] += '<div id="popup_FrameDecorationDiv_0" style="position: absolute; overflow: hidden; width: 216px; height: 116px; left: 0px; bottom: 21px; right: 22px; top: 10px;"><img id="popup_FrameDecorationImg_0" src="/sites/domua.khituongthuyvan.net/modules/features/openlayers/themes/default_dark/img/cloud-popup-relative.png" style="width: 1276px; height: 736px; position: absolute; left: 0px; top: 0px;"></div>';
				*/
				/*
				contentString[i] += '<div id="popup_FrameDecorationDiv_1" style="position: absolute; overflow: hidden; width: 22px; height: 116px; bottom: 21px; right: 0px; top: 10px;"><img id="popup_FrameDecorationImg_1" src="/sites/domua.khituongthuyvan.net/modules/features/openlayers/themes/default_dark/img/cloud-popup-relative.png" style="width: 1276px; height: 736px; position: absolute; left: -1238px; top: 0px;"></div>';
				*/
				/*
				contentString[i] += '<div id="popup_FrameDecorationDiv_2" style="position: absolute; overflow: hidden; width: 216px; height: 21px; left: 0px; bottom: 22px; right: 22px;"><img id="popup_FrameDecorationImg_2" src="/sites/domua.khituongthuyvan.net/modules/features/openlayers/themes/default_dark/img/cloud-popup-relative.png" style="width: 1276px; height: 736px; position: absolute; left: 0px; top: -629px;"></div>';
				*/
				/*
				contentString[i] += '<div id="popup_FrameDecorationDiv_3" style="position: absolute; overflow: hidden; width: 22px; height: 21px; bottom: 22px; right: 0px;"><img id="popup_FrameDecorationImg_3" src="/sites/domua.khituongthuyvan.net/modules/features/openlayers/themes/default_dark/img/cloud-popup-relative.png" style="width: 1276px; height: 736px; position: absolute; left: -1238px; top: -629px;"></div></div>';
				*/
				contentString[i] += '';		
			}
			
			function addInfoMarker(currentIndex){
				VEvent.addListener(
					marker,
					'mouseover',
					function(obj){
						//console.log('over');
						//alert('ok');
						var content = obj.openInfoWindow(contentString[currentIndex]);
					}
				);	
			}
			
			for(var i = 0;i<markers.length;i++){
				var marker = markers[i];
				var currentIndex = i;
				addInfoMarker(currentIndex);
				
			}
			
		}
MANG2;

	$s.="}";
	
	drupal_add_js( <<< EEE
		$s
EEE
		, 'inline');
	
	$html = <<< ABC
	<div id="openlayers-views-map-searchfrm">
		<div id="olvm-title-div" name="olvm-title-div" style="float:left; position:relative; margin:4px 5px 0px 0px; font-weight:bold;">Tìm kiếm:</div>
		<div id="olvm-title-div" name="olvm-title-div" style="float:left; position:relative; margin:0px 10px 0px 0px;">
			<label for="olvm-title-txt" style="position: absolute; top: 0px; left: 0px; margin: 4px 5px 5px 6px; color: rgb(119, 119, 119); font-weight: normal; opacity: 1;" class="label-text">Tên trạm</label>
			<input type="text" id="olvm-title-txt" name="olvm-title-txt" class="form-text">
		</div>
		<div id="olvm-phone-div" name="olvm-phone-div" style="float:left; position:relative; margin:0px 10px 0px 0px;">
			<label for="olvm-phone-txt" style="position: absolute; top: 0px; left: 0px; margin: 4px 5px 5px 6px; color: rgb(119, 119, 119); font-weight: normal; opacity: 1;" class="label-text">Số điện thoại</label>
			<input type="text" id="olvm-phone-txt" name="olvm-phone-txt" class="form-text">
		</div>
		<input type="button" id="olvm-search-btn" name="olvm-search-btn" value="Tìm kiếm" class="form-submit">
	</div>
  <div id="container" style="height:500px; width:930px;position:relative; margin:10px; border:1px #b1c4d5 solid;"></div>
  <div id"test"></div>
  <script>
    loadMap();
  </script>
  
ABC;
	return $html;
} 


