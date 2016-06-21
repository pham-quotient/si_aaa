jQuery(window).load(function(){
  // If this isn't an EAD record, check for a value in data-element-value, 
  // and initialize the AAA Player if a value is found.
  if((typeof jQuery('.edan-row').attr('data-ead-id') === 'undefined') 
      && (typeof jQuery('.edan-row').attr('data-element-value') !== 'undefined')) {

    // Call the Build Viewer jQuery plugin.
    jQuery('#player').buildPlayer('.edan-row');
    // Initialize the Player
    initPlayers( jQuery('.wellcomePlayer') );
  }
});

jQuery(document).ready(function($) {

  $('#findingAidTabs').responsiveTabs({
    startCollapsed: false
  });

  $('#download-modal').dialog({
    autoOpen: false,
    height: 425,
    width: '60%',
    modal: true,
    buttons: {
      Close: function() {
        $('body').removeClass('NoScrolling');
        $( this ).dialog( 'close' );
      }
    },
    close: function() {
      // Un-freeze the parent window to restore scrolling.
      $('body').removeClass('NoScrolling');
    }
  });

  $('#download_modal_trigger').on('click', function(){
    event.preventDefault();
    $('body').addClass('NoScrolling');
    $('#download-modal').dialog('open');
  });

  $('#create_zip_trigger').on('click', function(){

    event.preventDefault();

    $('.edan-extended-preloader').show();

    var imagesJson = $('#download-modal ul').attr('data-images-json');

    if(typeof imagesJson !== 'undefined') {

      $.ajax({
        type: 'POST'
        ,dataType: 'json'
        ,url: '/zip_files/ajax'
        ,data: ({files: JSON.parse(imagesJson)})
        ,success: function(result) {

          $('.edan-extended-preloader').hide();

          var path = result;
          $('#download-button-container').append('<input type="submit" id="download_zip_trigger" value="Download ZIP" class="form-submit" data-path="' + path + '">');

        }
      });
    }
  });

  $('#download-button-container').on('click', '#download_zip_trigger', function(){
    var download_path = $(this).attr('data-path');

    console.log(download_path);

    window.location.href = window.location.protocol + '//' + window.location.hostname + '/' + download_path;
  });

  /**
   * Aeon Request
   * Process data for the request, and submit the form.
   */
  $('.aaa_request').on('click', function() {
    // Set up the target page (/request/viewing or /request/reproduction)
    var target_page = '/request/' + $(this).attr('id');
    // Modify the action attribute to point to the target page.
    $('form#request').attr('action', target_page);
    // Submit the form.
    $('form#request').submit(function() {
      // Get Fancytree to generate the form elements.
      $('#tree').fancytree('getTree').generateFormElements();
      var ead_id = $('.edan-row').attr('data-ead-id'),
          input = $('<input>').attr('type', 'hidden').attr('name', 'ead_id').val( ead_id );
      $('form#request').append($(input));
       //console.log( "POST data:\n" + jQuery.param( $(this).serializeArray() );
    });
  });

});