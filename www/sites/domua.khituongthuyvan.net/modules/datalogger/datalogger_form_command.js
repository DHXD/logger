


  function datalogger_auto_refresh() {      
    var nid_command = 0;    // lay trong HTML ra
    
    var id = jQuery('div#ajax-command-submit-div > fieldset > div div.node').attr('id');
    
    var divs;
    if (id) {
      divs = id.split('-');
      
      nid_command = divs[1];
    
      //console.log(divs);
      
      jQuery.ajax({
                    //type: 'POST',
                    url: "/datalogger/command/refresh/ajax/" + nid_command,
                    //dataType: 'json',
                    success: function (data) {
                      var fieldset = jQuery('div#ajax-command-submit-div fieldset');
                      fieldset.find('div.fieldset-wrapper').html(data);
                    },
                    // Might want to use 'ui' instead of jQuery('#slider').
                    ////data: 'slider_value=' + jQuery('#slider').slider('values', 0)
                });
    }
    
    
    // check if data is available
    if (jQuery('.field-name-field-command-result .field-items .field-item').html() == '')
      setTimeout('datalogger_auto_refresh()', 3500);  
  }
  
  
  (function ($) {
      Drupal.behaviors.datalogger = {
        attach: function (context, settings) {
          /*$('fieldset#edit-submit-command-fieldset--8', context).change(function () {
              alert('Handler for .change() called.');
          });*/
          setTimeout('datalogger_auto_refresh()', 3500);
        }
      };
	}(jQuery));
    
    