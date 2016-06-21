/**
 * For the Aeon Collection Request Page
 */

jQuery(document).ready(function($) {

  $('#selectAll').on('click', function() {
    $('.checkbox-container input:checkbox').prop('checked', 1);
		var selectedIDs = []; 
		$('.checkbox-container input:checkbox').each(function() { selectedIDs.push($(this).val());})
		$('[name="ft_2"]').val(selectedIDs.join(',')) 
  });

  $('#selectNone').on('click', function() {
    $('.checkbox-container input:checkbox').prop('checked', 0);
		$('[name="ft_2"]').val('')
  });
  $('.checkbox-container input:checkbox').click(function(){
    if ($(this).is(":checked")){
			var selectedIDs = $('[name="ft_2"]').val().length > 0 ? $('[name="ft_2"]').val() + ',' + $(this).val() : $(this).val();
		}
		else{
		 if ($('[name="ft_2"]').val().split(',').indexOf($(this).val()) != -1) {
			 var selectedIDs = $('[name="ft_2"]').val().split(',');
			 selectedIDs.splice(selectedIDs.indexOf($(this).val()),1);
		}
	}
	$('[name="ft_2"]').val(selectedIDs);
	});		 
});
