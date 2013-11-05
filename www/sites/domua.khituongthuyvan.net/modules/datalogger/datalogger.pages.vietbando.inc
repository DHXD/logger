<?php

/*
 * Menu page callback.
 */
function _datalogger_map() {
	$view = views_get_view_result('viet_ban_do');
	dpm($view);
	//return $view[0]->field_field_station_longgitude[0]['raw']['safe_value'];
	$s = '';
	
	$external_js = 'http://www.vietbando.com/maps/API/VBDMapAPI.js?key=E43pcmZHdUhoao9ejBKG/CwvXxcHv+NwVcvW0H3Rg+o=';
	
  drupal_add_js('document.write(unescape("%3Cscript src=\''. $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');
  
	$html = <<< ABC
  
  <div id="container" style="height:500px; width:930px;position:relative; margin:10px; border:1px #b1c4d5 solid;"></div>
  <div id"test"></div>
  <script>
    function loadMap()
    {
        if (VBrowserIsCompatible())
        {
           var map = new VMap(document.getElementById('container'));
           map.setCenter(new VLatLng(10.8152328, 106.680505), 4);
        }
    }
    loadMap();
  </script>
ABC;

  return $html;
}

function full_url()
{
    $s = &$_SERVER;
    $ssl = (!empty($s['HTTPS']) && $s['HTTPS'] == 'on') ? true:false;
    $sp = strtolower($s['SERVER_PROTOCOL']);
    $protocol = substr($sp, 0, strpos($sp, '/')) . (($ssl) ? 's' : '');
    $port = $s['SERVER_PORT'];
    $port = ((!$ssl && $port=='80') || ($ssl && $port=='443')) ? '' : ':'.$port;
    $host = isset($s['HTTP_X_FORWARDED_HOST']) ? $s['HTTP_X_FORWARDED_HOST'] : isset($s['HTTP_HOST']) ? $s['HTTP_HOST'] : $s['SERVER_NAME'];
    return $protocol . '://' . $host . $port . $s['REQUEST_URI'];
}

function _datalogger_vietbando() {
	//$tram = node_load(3126181);
	//dpm($tram);
	$rows = views_get_view_result('viet_ban_do');
	//dpm($rows);
	$full_url = full_url();
	//echo $full_url;
	
	switch($full_url){
		case "http://admin.domua.khituongthuyvan.net/" :
			$url = "5lJdIlKaLKfA1UMwdmzgbRYOTt/AH+9wl6NQH/f0pue8AFCloQ7qJw==";
		break;	
		case "http://117.6.129.235:8181:8181/" ://Bac Trung Bo ok
			$url = "E43pcmZHdUhoao9ejBKG/CwvXxcHv+NwVcvW0H3Rg+o=";
		break;
		case "http://192.168.100.48:8181:8181/" ://Bac Trung Bo ok
			$url = "E43pcmZHdUidihFQnTP4CE1G6xwG7qEy8G+MvpOPjSE=";
		break;
		
		case "http://113.160.225.111:8181:8181/" ://Trung Trung Bo ok
			$url = "E43pcmZHdUgB7c6vDr0v6Wr68q59Z7acJs8eK4rSnNE=";
		break;
		case "http://192.168.100.101:8181:8181/" ://Trung Trung Bo ok
			$url = "E43pcmZHdUidihFQnTP4CJtdkyDsS/v6lpaNbNJdod8=";
		break;
		
		case "http://kttvntb.gov.vn:8181:8181/" : //Nam Trung Bo ok
			$url = "6+ZvZ+bVk7kGAj00YOiWwcamFLHxDOuHjt1PK+LvY4w=";
		break;
		case "http://10.152.52.10:8181:8181/" : //Nam Trung Bo ok
			$url = "E43pcmZHdUgN4UJdBQ1FOJnHWmdWDvfeahGSQ+dGfz0=";
		break;
		
		case "http://kttvtaynguyen.dyndns.org/" : //Tay Nguyen ok
			$url = "6+ZvZ+bVk7nAf8Ske2bWNvDalMywr5HD5WT957Dk47EkXwf1b0S+dQ==";
		break;
		case "http://210.245.110.15:8181:8181/" : //Nam Bo ok
			$url = "PxVLItwK6WpL/gxKoctolsN6g0viC8l/XlqTfK7KTG4=";
		break;
	}
	
	//dpm($rows);
	//echo $rows[0]->field_field_station_alarm;
	//drupal_add_js("var view = " . drupal_to_js($view) . ";", 'inline');

  $external_js = 'http://www.vietbando.com/maps/API/VBDMapAPI.js?key=' .$url;

	
 drupal_add_js('document.write(unescape("%3Cscript src=\''. $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');
  
  $s = "  function loadMap() {";
	$s .= "var station = [];\n";
	
	foreach ($rows as $k => $row) {
		$s .= <<< MANG
		
		station[$k] = new Object();
		station[$k].tentram = "{$row->node_title}";

		station[$k].matram =" {$row->_field_data['nid']['entity']->field_station_code['und'][0]['value']}";

		station[$k].status=0;
		//Nid
		station[$k].nid = {$row->nid};
		//Dia chi
		station[$k].diachi = "{$row ->field_field_station_address[0]['rendered']['#markup']}";
		//Lat
		station[$k].lat = "{$row->_field_data['nid']['entity']->field_station_geo_location['und'][0]['lat']}";
		//Lon
		station[$k].lon = "{$row->_field_data['nid']['entity']->field_station_geo_location['und'][0]['lon']}";
		//Kinh do
		station[$k].kinhdo = "{$row->_field_data['nid']['entity']->field_station_latitude['und'][0]['value']}";
		//Vi do
		station[$k].vido = "{$row ->_field_data['nid']['entity']->field_station_longgitude['und'][0]['value']}";
		//SDT
		station[$k].sdt = "{$row->field_field_station_sim_number[0]['rendered']['#markup']}";
		//Dien ap
		station[$k].dienap ="{$row->field_field_station_power[0]['rendered']['#markup']}";
		//Luong mua 24h
		station[$k].luongmua24h ="{$row->field_field_station_rain_24[0]['rendered']['#markup']}";
		//Luong mua gan nhat
		station[$k].luongmuagannhat ="{$row->field_field_station_rain_nearest[0]['rendered']['#markup']}";
		//Trang thai
		station[$k].trangthai ="{$row->field_field_station_status[0]['raw']['value']}" ;
    station[$k].trangthaigiatri ="{$row->field_field_station_status[0]['rendered']['#markup']}" ;
		//Chu ky truyen tin
		station[$k].chukytruyentin = "{$row->field_field_station_transmission_speed[0]['rendered']['#markup']}";
		station[$k].tinhtrang = "{$row->field_field_station_alarm[0]['rendered']['#markup']}";
		if(station[$k].chukytruyentin == ""){
			station[$k].chukytruyentin = "Chưa có dữ liệu";	
		}else{
      station[$k].chukytruyentin =station[$k].chukytruyentin.concat(" phút");
      
    }
    station[$k].thoigiantruyentin = "{$row->field_field_station_report_time[0]['rendered']['#markup']}";
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
			if($row->field_field_station_alarm[0]['raw']['value'] == 'alarm_false'){
				//Binh Thuong
				$s .= <<< MANG
				
				station[$k].canhbao = 0;
			
MANG;
			}else if($row->field_field_station_alarm[0]['raw']['value'] == 'alarm_rainfall'){
				//Mua 	
				$s .= <<< MANG
				
				station[$k].canhbao = 1;
			
MANG;
			}else if($row->field_field_station_alarm[0]['raw']['value'] == 'alarm_true'){
      //Mua to	
				$s .= <<< MANG
				
				station[$k].canhbao = 2;
			
MANG;
      }
		}
	}
	
	$s.= <<< MANG2
		if (VBrowserIsCompatible() ) { 
			var map = new VMap(document.getElementById('container'));

MANG2;
	switch($full_url){
		case "http://admin.domua.khituongthuyvan.net/" :
			$s.=<<<MANG2
			map.setCenter(new VLatLng(18.8152328, 105.680505), 7);
MANG2;
		break;	
		case "http://117.6.129.235:8181:8181/" ://Bac Trung Bo ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(18.8152328, 106.680505), 7);
MANG2;
		case "http://192.168.100.48:8181:8181/" ://Bac Trung Bo ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(18.8152328, 106.680505), 7);
MANG2;
		break;
		case "http://113.160.225.111:8181:8181/" ://Trung Trung Bo ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(16.8152328, 105.680505), 7);
MANG2;
	case "http://192.168.100.101:8181:8181/" ://Trung Trung Bo ok
				$s.=<<<MANG2
				map.setCenter(new VLatLng(16.8152328, 105.680505), 7);
MANG2;
		break;
		case "http://kttvntb.gov.vn:8181:8181/" : //Nam Trung Bo ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(12.8152328, 107.680505), 7);
MANG2;
		case "http://10.152.52.10:8181:8181/" : //Nam Trung Bo ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(12.8152328, 107.680505), 7);
MANG2;
		break;
		case "http://kttvtaynguyen.dyndns.org/" : //Tay Nguyen ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(12.8152328, 107.680505), 7);
MANG2;
		break;
		case "http://210.245.110.15:8181:8181/" : //Nam Bo ok
			$s.=<<<MANG2
			map.setCenter(new VLatLng(10.8152328, 105.680505), 7);
MANG2;
		break;
	}
	
	$s.=<<<MANG2

			var markers = [];  
			var contentString = [];
			for(var i =0;i<station.length;i++){
				var pt = new VLatLng(station[i].lat,station[i].lon);
				if(station[i].trangthai=='station_active'){
					if(station[i].canhbao == 0){
						markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/orange1.png", new VSize(15, 15)), false);
					}else if(station[i].canhbao == 1){
            markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/blue.png", new VSize(15, 15)), false);
					}else if(station[i].canhbao == 2){
            markers[i] = new VMarker(pt, new VIcon("/sites/domua.khituongthuyvan.net/icons/red.png", new VSize(15, 15)), false);
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
				contentString[i] += station[i].trangthaigiatri;
				contentString[i] += '</b></a></li><li><a title="Điện áp hiện tại: ';
				contentString[i] += station[i].dienap;
				contentString[i] += '">Điện áp hiện tại: <b>';
				contentString[i] += station[i].dienap;
				contentString[i] += '</b></a></li>';
        
				contentString[i] += '</ul><!--[if lte IE 6]></td></tr></table></a><![endif]--></li><li><a class="hide">Thông tin lượng mưa</a><!--[if lte IE 6]><a>Thông tin lượng mưa<table><tr><td><![endif]--><ul><li><a title="Tình trạng: ';
				contentString[i] += 'undefined';
				contentString[i] += '">Tình trạng: <b>';
				contentString[i] += station[i].tinhtrang;
				contentString[i] += '</b></a></li><li><a title="Lượng mưa 24h: ';
				contentString[i] += station[i].luongmua24h;
				contentString[i] += '">Lượng mưa 24h: <b>';
				contentString[i] += station[i].luongmua24h;
				contentString[i] += '</b></a></li><li><a title="Lượng mưa gần nhất: ';
				contentString[i] +=station[i].luongmuagannhat;
				contentString[i] += '">Lượng mưa gần nhất: <b>';
				contentString[i] += station[i].luongmuagannhat;
				contentString[i] += '</b></a></li><li><a title="Chu kỳ truyền tin: ';
				contentString[i] += station[i].chukytruyentin;;
				contentString[i] += '">Chu kỳ truyền tin: <b>';
				contentString[i] += station[i].chukytruyentin;
        contentString[i] += '</b></a></li><li><a title="Thời gian truyền tin: ';
				contentString[i] += station[i].thoigiantruyentin;;
				contentString[i] += '">Thời gian truyền tin: <b>';
				contentString[i] += station[i].thoigiantruyentin;
				contentString[i] += '</b></a></li><li><a title="Báo cáo dữ liệu mưa" href="';
				contentString[i] += '/manage/report/rainfalls/month/';
				contentString[i] += station[i].nid;
				contentString[i] += '"><b>Báo cáo dữ liệu mưa</b></a></li><li><a title="Thống kê số lương mưa theo bảng" href="';
				contentString[i] += '/views/statistics/search/';
				contentString[i] += station[i].nid;
				contentString[i] += '"><b>Thống kê số lương mưa theo bảng</b></a></li><li><a title="Thống kê số lượng mưa theo biểu đồ" href="';
				contentString[i] += '/manage/chart/rainfalls/';
        contentString[i] += station[i].nid;
				contentString[i] += '"><b>Thống kê số lượng mưa theo biểu đồ</b></a></li></ul><!--[if lte IE 6]></td></tr></table></a><![endif]--></li></ul></div></div></div></div>';
				  contentString[i] += '';
			
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
		/*
		*SearchId
		*/
		function searchTenTram(input,station){		
			var results = [];
			for(var i = 0; i < station.length; i++){
				if(station[i].tentram.toLowerCase().indexOf(input.toLowerCase()) > -1){
					var k = results.length;
					results[k] = new Object();
					results[k].contentString = i;
					results[k].station = station[i];
				}
			}		
			return results;
		}
		function searchSdtTram(input,station){
			var keys = [];
			var result = [];
			var tram = [];
			for(var i = 0;i < station.length;i++){
				if(station[i].sdt.toString().indexOf(input) > -1){
					console.log(station[i].sdt.indexOf(input));
					return i;
					break;
				}
				
			}	
		}
		/**
		*
		*/
		jQuery(document).ready(function($) {
			$( "#olvm-search-btn" ).click(function() {
				var tentram = $("#olvm-title-txt").val();
				console.log(tentram);
				var sdt = $("#olvm-phone-txt").val();
				console.log(sdt);
				if(tentram == '' && sdt == ''){
					return;
				}else if(tentram != '' && sdt ==''){
					var results = searchTenTram(tentram,station);
					if(results.length == 0){
						return;
					}else if(results.length == 1){
						var lat = results[0].station.lat;
						var lon = results[0].station.lon;
						console.log(results[0].contentString);
						var pt = new VLatLng(lat,lon);
						map.setCenter(new VLatLng(lat,lon),8);
						console.log(results[0].contentString);
						markers[results[0].contentString].openInfoWindow(contentString[results[0].contentString]);
					}else if(results.length >1){
						confirm_feature(results);
					}					
				}else if(tentram == '' && sdt !=''){
					var result = searchSdtTram(sdt,station);
					console.log(result);
					console.log(result);
					var lat = station[result].lat;
					console.log(lat);
					var lon = station[result].lon;
					console.log(lon);
					var pt = new VLatLng(lat,lon);
					console.log(pt);
					//map.SetCenter(pt,7);	
					map.setCenter(new VLatLng(lat,lon),8);
					markers[result].openInfoWindow(contentString[result]);
				}else if(tentram !='' && sdt !=''){
					return;
				}
			});
			//Focus
			jQuery(document).ready(function(){
				$("#olvm-title-txt").focus(function(){
						$(this).prev("label").hide();
						//$('label.label-hide[for="' + $(this)[0].id + '"]').hide();
					}).blur(function() {
						if($(this).val() == '') {
							//$('label.label-hide[for="' + $(this)[0].id + '"]').show();
							$(this).prev("label").show();
						}
				});
					$("#olvm-phone-txt").focus(function(){
						$(this).prev("label").hide();
					}).blur(function() {
						if($(this).val() == '') {
							$(this).prev("label").show();
						}
				});
			});
		});		
		
		function confirm_feature(features) {
			feature = features[0];
			jQuery.msgBox({
				title: "Tìm kiếm trạm",
				content: "Tìm thấy trạm "+ feature.station.tentram+". Bạn có muốn di chuyển bản đồ đến trạm "+feature.station.tentram+" không?",
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
			var lat = feature.station.lat;
			var lon = feature.station.lon;
			var pt = new VLatLng(lat,lon);
			map.setCenter(new VLatLng(lat,lon),8);
			markers[feature.contentString].openInfoWindow(contentString[feature.contentString]);				
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


