var delta_timezone = 0;
(function ($){
  $(document).ready(function(){

		$(function () { 
		
				var e = {min:0, max:0};
				var min = $('#edit-day').val();
				//console.log(min);
				var max = $('#edit-day-to').val();
				//console.log(max);
				var station = $('#edit-station').val();
				
				//e.max   = Date.parse($('#edit-day-to').val()); // /1000
				
				var digitpattern = /\d+/g,				
				m = min.match(digitpattern);				
				var min_d = m[1] + " " + m[0] + ", " + m[2] + " " + m[3] + ":" +	m[4];
				e.min = Date.parse(min_d +' UTC'); // /1000
				
				x = Date.parse(min_d);				
				delta_timezone = e.min-x;
				
				m = max.match(digitpattern);	
				var max_d = m[1] + " " + m[0] + ", " + m[2] + " " + m[3] + ":" +	m[4];
				e.max = Date.parse(max_d +' UTC'); // /1000
				
				//console.log(max_d);
				//console.log(min_d, max_d);
				//console.log(e);
		
				// See source code from the JSONP handler at https://github.com/highslide-software/highcharts.com/blob/master/samples/data/from-sql.php
				//$.getJSON('http://www.highcharts.com/samples/data/from-sql.php?callback=?', function(data) {
				
				var u = 'http://113.160.225.111:8181/js/manage/chart/rainfalls';
				//u = 'http://www.highcharts.com/samples/data/from-sql.php';
				
				$.getJSON(u + '?start='+ Math.round(e.min) + '&end='+ Math.round(e.max)+'&station='+ station  +'&callback=?', function(data) {
					//console.log(data);
					// Add a null value for the end date 
					//data = [].concat(data, [[Date.UTC(2011, 9, 14, 19, 59), null, null, null, null]]);
					//console.log(data);
					// create the chart
					$('#edit-person div.fieldset-wrapper').highcharts('StockChart', {
						chart : {
							type: 'column',
							zoomType: 'x'
						},

						navigator : {
							adaptToUpdatedData: false,
							series : {
								data : data
							}
						},

						scrollbar: {
							liveRedraw: false
						},
						
						title: {
							text: 'Biểu đồ dữ liệu mưa theo thời gian'
						},
						
						/*subtitle: {
							text: ''
						},*/
						
						rangeSelector : {
							
							
							buttonTheme: { // styles for the buttons
								//fill: 'none',
								//stroke: 'none',
								//'stroke-width': 0,
								//width: '50px',								
								//r: 8,
								style: {
									//color: '#039',
									//fontWeight: 'bold',
									fontSize: '8px'
									//width: '50px'
								},
								states: {
									hover: {
									},
									select: {
										fill: '#039',
										style: {
											color: 'white'
										}
									}
								}
							},
							
							buttons: [
							/*{
								type: 'hour',
								count: 1,
								text: '1h'
							}, */
							{
								type: 'day',
								count: 1,
								text: '1 ngày'
							}, {
								type: 'month',
								count: 1,
								text: '1 tháng'
							}, {
								type: 'year',
								count: 1,
								text: '1 năm'
							}, {
								type: 'all',
								text: 'Tất cả'								
							}],
							inputEnabled: false, // it supports only days
							selected : 4 // all
						},
						
						xAxis : {
							events : {
								afterSetExtremes : afterSetExtremes
							},
							minRange: 3600 * 1000 // one hour
						},

						series : [{
							name: "Luong mua (mm): ",
							data : data,
							dataGrouping: {
								enabled: false
							}
						}]
					});
				});

				
				
		});

		
  });
})(jQuery);


/**
 * Load new data depending on the selected min and max
 */
function afterSetExtremes(e) {

	var url,
		currentExtremes = this.getExtremes(),
		range = e.max - e.min;
		//console.log(currentExtremes);
	var chart = jQuery('#edit-person div.fieldset-wrapper').highcharts();
	var station = jQuery('#edit-station').val();
	//console.log(range, e);
	e.max =  e.max + delta_timezone ;
	e.min = e.min + delta_timezone ;
	chart.showLoading('Đang nạp dữ liệu...');
	jQuery.getJSON('/js/manage/chart/rainfalls?start='+ Math.round(e.min) + '&station='+ station  +
			'&end='+ Math.round(e.max) +'&callback=?', function(data) {
		
		chart.series[0].setData(data);
		chart.hideLoading();
	});
	
}

function print_std_time(unix_timestamp){
	var date = new Date(unix_timestamp*1000);
// hours part from the timestamp
	var hours = date.getHours();
	// minutes part from the timestamp
	var minutes = date.getMinutes();
	// seconds part from the timestamp
	var seconds = date.getSeconds();

	// will display time in 10:30:23 format
	var formattedTime = hours + ':' + minutes + ':' + seconds;
	console.log(formattedTime);
}