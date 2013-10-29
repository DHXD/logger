<?php
/*
 * Menu page callback.
 */
function _datalogger_map() {
	$view = views_get_view_result('viet_ban_do');
	dpm($view);
	//return $view[0]->field_field_station_longgitude[0]['raw']['safe_value'];
	$s = '';
	
	$external_js = 'http://www.vietbando.com/maps/API/VBDMapAPI.js?key=E43pcmZHdUgB7c6vDr0v6Wr68q59Z7acJs8eK4rSnNE=';
	
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

function _datalogger_vietbando() {
	$rows = views_get_view_result('viet_ban_do');
	
	
	//dpm($rows);
	//echo $rows[0]->field_field_station_alarm;
	//drupal_add_js("var view = " . drupal_to_js($view) . ";", 'inline');

  $external_js = 'http://www.vietbando.com/maps/API/VBDMapAPI.js?key=E43pcmZHdUgB7c6vDr0v6Wr68q59Z7acJs8eK4rSnNE=';
	
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
			map.setCenter(new VLatLng(16.8152328, 105.680505), 7);
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
				contentString[i] += '/manager/bieu-do-mua-theo-gio';
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
			//var str ="ha he hi";
			//console.log('--------------');
			//console.log(str.indexOf("hi"));
			var keys = [];
			var result = [];
			//console.log(input);
			//var abc = ["Mot","Hai","Ba"] 
			var tram = [];
			for(var i = 0;i < station.length;i++){
				//var tram[tram.length] = station[i].tentram; 
				
				if(station[i].tentram.toLowerCase().indexOf(input.toLowerCase()) > -1){
					console.log(station[i].tentram);
					//var tentram = station[i].tentram.toLowerCase();
					console.log(station[i].tentram.toLowerCase().indexOf(input.toLowerCase()));
					return i;
					break;
				}
			}		
		}
		function searchSdtTram(input,station){
			//var str ="ha he hi";
			console.log('1--------------');
			//console.log(str.indexOf("hi"));
			var keys = [];
			var result = [];
			//console.log(input);
			//var abc = ["Mot","Hai","Ba"] 
			var tram = [];
			for(var i = 0;i < station.length;i++){
				console.log('2--------------');
				//var tram[tram.length] = station[i].tentram; 
				//console.log(station[i].sdt);
				
				if(station[i].sdt.toString().indexOf(input) > -1){
					console.log('3--------------');
					console.log(station[i].sdt);
					//var tentram = station[i].tentram.toLowerCase();
					console.log(station[i].sdt.indexOf(input));
					return i;
					break;
				}
				
			}	
			//console.log('---------------------------------------------');
			//console.log(station[0].sdt.toString().indexOf("163"));	
			//return;
		}
		/**
		*
		*/
		jQuery(document).ready(function($) {
			$( "#olvm-search-btn" ).click(function() {
				console.log('ok');
				var tentram = $("#olvm-title-txt").val();
				console.log(tentram);
				var sdt = $("#olvm-phone-txt").val();
				console.log(sdt);
				if(tentram == '' && sdt == ''){
					return;
				}else if(tentram != '' && sdt ==''){
					var result = searchTenTram(tentram,station);
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
						//$('label.label-hide[for="' + $(this)[0].id + '"]').hide();
					}).blur(function() {
						if($(this).val() == '') {
							//$('label.label-hide[for="' + $(this)[0].id + '"]').show();
							$(this).prev("label").show();
						}
				});
			});
		});		
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


