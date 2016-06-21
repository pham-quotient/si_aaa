<?php
/**
 * Viewing and Reproduction Requests (via Aeon)
 *
 * @file edan_extended_request.inc
 * @version 0.1
 */

/**
 * Implements hook_form().
 */
function edan_extended_reproduction_request_form( $form, &$form_state, $record = array() ) {

   //dumper($record);

  $form = array();

  $form['formFieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prepare Your Request'),
  );

  // Request type
  $form['formFieldset']['RequestType'] = array(
    '#type' => 'select',
    '#title' => t('Original material format:'),
    '#required' => TRUE,
    '#options' => _edan_extended_material_format_options(),
    '#default_value' => 'Manuscript material',
  );

  // Format
  $form['formFieldset']['Format'] = array(
    '#type' => 'select',
    '#title' => t('Format for delivery:'),
    '#required' => TRUE,
    '#options' => _edan_extended_delivery_format_options(),
    '#default_value' => 'PDF',
  );

  // Intended Use
  $form['formFieldset']['IntendedUse'] = array(
    '#type' => 'textfield',
    '#title' => t('Intended Use'),
    // '#description' => t(''),
    '#size' => 65,
    '#maxlength' => 255,
    '#required' => TRUE,
    // '#default_value' => '',
  );

  // Notes
  $form['formFieldset']['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Use this space for your notes'),
    '#rows' => 5,
    '#cols' => 6,
    '#attributes' => array('maxlength' => 2000),
  );

  // Checkbox: Keep for my review
  $form['formFieldset']['UserReview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep for my review'),
    '#description' => t('If this box is checked, your request will not be 
      processed until you submit it from the "Review Requests" listing 
      under the "Request" menu in the left hand navigational bar.'),
  );

  // CollectionID
  $form['CollectionID'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record['CollectionID']) ? $record['CollectionID'] : '',
  );

  // Reference IDs
  $form['ft_1'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record['ft_1']) ? implode(',', $record['ft_1']) : '',
  );
	$form['ft_2'] = array(
	 '#type' => 'hidden',
	 '#default_value' => isset($record['ft_1']) ? implode(',', $record['ft_1']) : '',
  );

  // EAD ID
  $form['ead_id'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record['ead_id']) ? $record['ead_id'] : '',
  );
  $form['collectionTitle'] = array(
	  '#type' => 'hidden',
		'#default_value' => isset($record['collection_title']) ? $record['collection_title'] : '',
  );
	//dumper($record);
	foreach ($record['ft_1'] as $ft_1_key){
		$ft_array = $record[$ft_1_key];
		$form[$ft_1_key] = array(
			'#tree' => TRUE,
		);	
		foreach ($ft_array as $key => $value){
		  $form[$ft_1_key][$key] = array(
	      '#type' => 'hidden',
		    '#default_value' => $value,
      );
		}
	}		

  // // The type (e.g. Box)
  // $form['type'] = array(
  //   '#type' => 'hidden',
  //   // '#default_value' => (isset($record['type'])) ? $record['type'] : '',
  // );

  // // What kind of request.
  // $form['request'] = array(
  //   '#type' => 'hidden',
  //   // '#default_value' => (isset($record['request'])) ? $record['request'] : '',
  // );

  // From the current AAA website.
  // <input type="hidden" name="CollectionID" value="7211">
  // <input type="hidden" name="type" value="Box">
  // <input type="hidden" name="request" value="GenericRequestManuscriptPhotodup">

  $form['formFieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['#attributes'] = array('target' => '_blank');
  return $form;
}

/**
 * Material format options.
 */
function _edan_extended_material_format_options() {
  return array(
    'Manuscript material' => t('Manuscript material'),
    'Audio recording' => t('Audio recording'),
    'Video recording' => t('Video recording'),
    'Multiple types' => t('Multiple types'),
  );
}

/**
 * Delivery format options.
 */
function _edan_extended_delivery_format_options() {
  return array(
    'PDF' => t('PDF'),
    'JPG' => t('Digital Image'),
    'MP3' => t('Mp3'),
  );
}

/**
 * Implements hook_form_validate().
 */
function edan_extended_reproduction_request_form_validate($form, &$form_state) {
  //dumper('Form submitted... validation logic goes here.');
	if(empty($form_state['values']['ft_2']) || !isset($form_state['values']['ft_2'])) {
    form_set_error('file', t('<strong>Error</strong>: No items selected. Please select at least one item.'));
  }
}

/**
 * Implements hook_form_submit().
 */
function edan_extended_reproduction_request_form_submit($form, &$form_state) {
  //dumper('Form submitted and validated... processing logic goes here.');
	$curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, 'https://aeon.aaa.si.edu/Aeon.dll');
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_setopt($curl,CURLOPT_POST, 1);
	curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);
	$curl_fields = array(
  'AeonForm' => 'EADRequest',
	'DocumentType' => 'manuscript',
	'GroupingIdentifier' => 'ItemVolume',
	'GroupingOption_ItemVolume' => 'FirstValue',
	'SubmitButton' => 'Submit Request',
	'ItemTitle' => $form_state['values']['collectionTitle'],
	'RequestType' => 'Copy',
	'SkipOrderEstimate' => 'Yes',
	'ItemInfo4' => $form_state['values']['RequestType'],
	'Format' => $form_state['values']['Format'],
	'ItemInfo5' => $form_state['values']['IntendedUse'],
	'Notes' => $form_state['values']['notes'],
	'userreview' => $form_state['values']['UserReview'],
	);
  $curl_post = http_build_query($curl_fields);
	$html_post_string = 'EADRequest&DocumentType=manuscript&GroupingIdentifier=ItemVolume&GroupingOption_ItemVolume=FirstValue&SubmitButton='.urlencode('Submit Request') . '&ItemTitle='.urlencode($form_state['values']['collectionTitle']);
	$html_post_string .= '&RequestType=Copy&SkipOrderEstimate=Yes&ItemInfo4=' . urlencode($form_state['values']['RequestType']) . '&Format=' . urlencode($form_state['values']['Format']) . '&ItemInfo5=' . urlencode($form_state['values']['IntendedUse']);
	$html_post_string .= '&Notes=' . urlencode($form_state['values']['notes']) . '&userreview=' . urlencode($form_state['values']['UserReview']);
	$selected_items = explode(',',$form_state['values']['ft_2']);
	for ($ctr = 0; $ctr < sizeof($selected_items); $ctr++) {
    $selected_ref = $selected_items[$ctr];
		$field_ctr = $ctr+1;
		$selected_box = array();
		$selected_box['ItemVolume_' . $field_ctr] = 'Box ' . $form_state['values'][$selected_ref]['Box'];
		$selected_box['ItemPublisher_' . $field_ctr] = $form_state['values'][$selected_ref]['Folder'] . ', ' . $form_state['values'][$selected_ref]['FolderTitle'];
		$selected_box['Request'] = $field_ctr;
    $selected_box['ItemSubtitle_' . $field_ctr] = $form_state['values'][$selected_ref]['Series'];
    $curl_post .= '&'.http_build_query($selected_box);
		$html_post_string .= '&ItemVolume_' . $field_ctr . '='.urlencode('Box ' . $form_state['values'][$selected_ref]['Box']);
		$html_post_string .= '&ItemPublisher_' . $field_ctr . '=' . urlencode($form_state['values'][$selected_ref]['Folder'] . ', ' . $form_state['values'][$selected_ref]['FolderTitle']);
		$html_post_string .= '&Request=' . $field_ctr . '&ItemSubtitle_' . $field_ctr . '=' . urlencode($form_state['values'][$selected_ref]['Series']);
  }
  curl_setopt($curl,CURLOPT_POSTFIELDS,$curl_post);
  $result = curl_exec($curl);
  // also get the error and response code
  $errors = curl_error($curl);
  $response = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	$redirectURL = curl_getinfo($curl,CURLINFO_EFFECTIVE_URL );
	curl_close($curl);
  if ($response == '200') {
		echo '<form action="' . $redirectURL . '" method="post" name="htmlForm" accept-charset="UTF-8" accept="multipart/form-data" enctype="text/plain">';
	  echo "<input type='hidden' name='AeonForm' value='". $html_post_string . "'>";
    echo '</form><script language="JavaScript">document.htmlForm.submit();</script>';
		drupal_exit();
  }
}

/**
 * Implements hook_form().
 */
function edan_extended_viewing_request_form( $form, &$form_state, $record = array() ) {
   //dumper($record);
  $form = array();
  $form['formFieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prepare Your Request'),
  );

  // Preferred start date
  $form['formFieldset']['ScheduledStartDate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Preferred start date'),
    '#required' => TRUE,
    '#date_format' => 'm-d-Y',
  );

  // Preferred end date
  $form['formFieldset']['ScheduledEndDate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Preferred end date'),
    '#required' => TRUE,
    '#date_format' => 'm-d-Y',
  );

  // Notes
  $form['formFieldset']['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Use this space for your notes'),
    '#rows' => 5,
    '#cols' => 6,
    '#attributes' => array('maxlength' => 2000),
  );

  // Checkbox: Keep for my review
  $form['formFieldset']['UserReview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep for my review'),
    '#description' => t('If this box is checked, your request will not be 
      processed until you submit it from the "Review Requests" listing 
      under the "Request" menu in the left hand navigational bar.'),
  );

  // CollectionID
  $form['CollectionID'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record['CollectionID']) ? $record['CollectionID'] : '',
  );

  // Reference IDs
  $form['ft_1'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record['ft_1']) ? implode(',', $record['ft_1']) : '',
  );
	$form['ft_2'] = array(
	 '#type' => 'hidden',
	 '#default_value' => isset($record['ft_1']) ? implode(',', $record['ft_1']) : '',
  );
  // EAD ID
  $form['ead_id'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record['ead_id']) ? $record['ead_id'] : '',
  );

  $form['collectionTitle'] = array(
	  '#type' => 'hidden',
		'#default_value' => isset($record['collection_title']) ? $record['collection_title'] : '',
  );
	//dumper($record);
	foreach ($record['ft_1'] as $ft_1_key){
		$ft_array = $record[$ft_1_key];
		$form[$ft_1_key] = array(
			'#tree' => TRUE,
		);	
		foreach ($ft_array as $key => $value){
		  $form[$ft_1_key][$key] = array(
	      '#type' => 'hidden',
		    '#default_value' => $value,
      );
		}
	}		
	// // The type (e.g. Box)
  // $form['type'] = array(
  //   '#type' => 'hidden',
  //   // '#default_value' => (isset($record['type'])) ? $record['type'] : '',
  // );

  // // What kind of request.
  // $form['request'] = array(
  //   '#type' => 'hidden',
  //   // '#default_value' => (isset($record['request'])) ? $record['request'] : '',
  // );

  // From the current AAA website.
  // <input type="hidden" name="CollectionID" value="7211">
  // <input type="hidden" name="type" value="Box">
  // <input type="hidden" name="request" value="GenericRequestManuscriptPhotodup">
  $form['formFieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['#attributes'] = array('target' => '_blank');
  return $form;
}
/**
 * Implements hook_form_validate().
 */
function edan_extended_viewing_request_form_validate($form, &$form_state) {
	if(empty($form_state['values']['ft_2']) || !isset($form_state['values']['ft_2'])) {
    form_set_error('file', t('<strong>Error</strong>: No items selected. Please select at least one item.'));
  }
 if (!empty($form_state['values']['ScheduledStartDate']) && !empty($form_state['values']['ScheduledEndDate'])) {
	 if (!is_array($form_state['values']['ScheduledStartDate']) && !is_array($form_state['values']['ScheduledEndDate']) && strtotime($form_state['values']['ScheduledStartDate']) > strtotime($form_state['values']['ScheduledEndDate'])) {
	   form_set_error('file', t('<strong>Error</strong>: Start Date cannot exceed the End Date.'));
	 }
 } 
 //dumper('Form submitted... validation logic goes here.');
}
/**
 * Implements hook_form_submit().
 */
function edan_extended_viewing_request_form_submit($form, &$form_state) {
  $curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, 'https://aeon.aaa.si.edu/Aeon.dll');
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_setopt($curl,CURLOPT_POST, 1);
	curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);
	$box_number = '';
	$curl_fields = array(
	  'AeonForm' => 'EADRequest',
	  'DocumentType' => 'manuscript',
	  'GroupingIdentifier' => 'ItemVolume',
	  'GroupingOption_ItemVolume' => 'FirstValue',
	  'SubmitButton' => 'Submit Request',
	  'ItemTitle' => $form_state['values']['collectionTitle'],
	  'ItemEdition' =>  $form_state['values']['ScheduledStartDate'],
	  'ItemPlace' => $form_state['values']['ScheduledEndDate'],
	  'RequestType' => 'Loan',
	  'Notes' => $form_state['values']['notes'],
	  'userreview' => $form_state['values']['UserReview'],
	);
  $curl_post = http_build_query($curl_fields);
	$html_post_string = 'EADRequest&DocumentType=manuscript&GroupingIdentifier=ItemVolume&GroupingOption_ItemVolume=FirstValue&SubmitButton='.urlencode('Submit Request') . '&ItemTitle='.urlencode($form_state['values']['collectionTitle']);
	$html_post_string .= '&ItemEdition='.urlencode($form_state['values']['ScheduledStartDate']) . '&ItemPlace=' . urlencode($form_state['values']['ScheduledEndDate']) . '&RequestType=Loan&Notes=' . urlencode($form_state['values']['notes']);
	$html_post_string .= '&userreview=' . $form_state['values']['UserReview'];
	$selected_items = explode(',',$form_state['values']['ft_2']);
	$ctr = 1;
	foreach($selected_items as $selected_ref) {
    if ($form_state['values'][$selected_ref]['Box'] != $box_number){
		  $box_number = $form_state['values'][$selected_ref]['Box'];
		  $selected_box = array();
		  $selected_box['ItemVolume_' . $ctr] = 'Box ' . $form_state['values'][$selected_ref]['Box'];
		  $selected_box['Request'] = $ctr;
      $selected_box['ItemSubtitle_' . $ctr] = $form_state['values'][$selected_ref]['Series'];
			$curl_post .= '&'.http_build_query($selected_box);
			$html_post_string .= '&ItemVolume_' . $ctr . '='.urlencode('Box ' . $form_state['values'][$selected_ref]['Box']);
			$html_post_string .= '&Request='.$ctr . '&ItemSubtitle_' . $ctr . '=' . urlencode($form_state['values'][$selected_ref]['Series']);
			$ctr++;
		}	
  }
	curl_setopt($curl,CURLOPT_POSTFIELDS,$curl_post);
  $result = curl_exec($curl);
  // also get the error and response code
  $errors = curl_error($curl);
  $response = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	$redirectURL = curl_getinfo($curl,CURLINFO_EFFECTIVE_URL );
	curl_close($curl);
  if ($response == '200') {
		echo '<form action="' . $redirectURL . '" method="post" name="htmlForm" accept-charset="UTF-8" accept="multipart/form-data" enctype="text/plain">';
	  echo "<input type='hidden' name='AeonForm' value='". $html_post_string . "'>";
    echo '</form><script language="JavaScript">document.htmlForm.submit();</script>';
		drupal_exit();
  }
}

/**
 * Clean Collection Title
 * (1) Removes line breaks
 * (2) Multiple consecutive spaces
 * (3) Adds a space before the first parentheses
 * (4) Removes the note
 * (5) Removes anything after the ending parentheses
 */
function _edan_extended_clean_collection_title( $title = '' ) {

  $collection_title = '';

  if(!empty($title)) {
    // Remove line breaks.
    $collection_title = preg_replace( "/\r|\n/", ' ', $title );
    // Replace multiple spaces with one space.
    $collection_title = preg_replace( '/\s+/', ' ', $collection_title );
    // Add a space at the first parentheses.
    $collection_title = preg_replace( '/\(/', ' (', $collection_title );
    // Remove note.
    $collection_title = preg_replace( '/\sAAA\..+/', '', $collection_title );
    // Remove anything after the ending perentheses.
    $collection_title = preg_replace( '/\).+/', ')', $collection_title );
  }

  return $collection_title;
}

/**
 * Get Instructional Text
 */
function _edan_extended_get_instructional_text( $request = false ) {

  $out = '';

  switch($request) {
    case 'viewing':
      $out .= '<p>You have requested an appointment to use the collection container listed below in the Archives of American ' . "\n";
      $out .= 'Art\'s Washington, D.C. reading room. Please complete the form below. Upon submitting your request you can ' . "\n";
      $out .= 'return to the container inventory and make additional requests. Each container requires a separate request.</p>' . "\n";
      break;
    case 'reproduction':
      $out .= '<p>Please complete this form to assist us in fulfilling your request for digital reproduction(s) ' . "\n";
      $out .= 'from the container(s) listed below.</p>' . "\n";
      $out .= '<p>You may wish to read our <a href="http://www.aaa.si.edu/askus/reproductions/index.cfm">Rights and ' . "\n";
      $out .= 'Reproductions</a> page to learn about request types and fees.</p>' . "\n";
      break;
  }

  $out .= '<p>If you need help determining whether this is appropriate for your research, please use the ' . "\n";
  $out .= '<a href="http://www.aaa.si.edu/askus">AskUs form</a> or email aaarepros@si.edu.</p>' . "\n";

  return $out;

}

/**
 * Builds the Request page.
 */
function _edan_extended_request_page_build( $request = '' ) {

  $out = $collection = '';
  $record = array();

   //dumper( 'Posted Data: ' . json_encode($_POST, JSON_PRETTY_PRINT) );

  if(empty($_POST) || !isset($_POST['ft_1'])) {
    // Get the referrer.
    $url = parse_url($_SERVER['HTTP_REFERER']);
    drupal_set_message( t('<strong>Error</strong>: No items selected. Please select at least one item.'), 'error' );
    drupal_goto( $url['path'] );
  }

  // Array of refIds
  $record['ft_1'] = !empty($_POST['ft_1']) ? $_POST['ft_1'] : '';

  if(!is_array($record['ft_1'])) {
    $record['ft_1'] = explode(',', $record['ft_1']);
  }

  $record['ead_id'] = str_replace('edanead-', '', $_POST['ead_id']);
	//$result = _edan_record_call_edan('contentUrl', array('record_type' => 'edanead', 'record_id' => $record['ead_id']));
  $params = array('url' => 'edanead' . ":" . $record['ead_id']);
  $result = _edan_call_edan('getContent', $params);
  //dumper($result);
	if($result && !empty($record['ft_1'])) {

    $record['CollectionID'] = $result['content']['record_ID'];

    // Transform the XML to what's needed, within list items.
    $result['xml_transformation'] = _edan_extended_get_ead_xml($result['content']['filelocation']);

    $dom = new DOMDocument();
    $dom->loadHTML($result['xml_transformation']);

    // Locate the metadata we need by traversing the list items
    // in the XML transformation.
    $lis = $dom->getElementsByTagName('li');
    $i = 0;
    foreach ($lis as $li) {
      foreach ($li->attributes as $attr_key => $attr) {
        $name = $attr->nodeName;
        $value = $attr->nodeValue;
        $values[$i][$attr_key] = $value;
      }
      $i++;
    }

    foreach($values as $val_key => $val_value) {
      if( in_array($val_value['data-parent-ref-id'], $record['ft_1']) ) {
         $ft_1_key = $val_value['data-parent-ref-id'];
        // Clean up the collection title (weird, right?)
        $collection_title = _edan_extended_clean_collection_title( $val_value['data-titleproper'] );

        $items = array(
          '<strong>Collection:</strong> ' . $collection_title,
          '<strong>Series:</strong> ' . $val_value['data-series-title'] . ', ' . $val_value['data-series-date'],
          '<strong>Box:</strong> ' . 'Box ' . $val_value['data-box'],
          '<strong>Folder:</strong> ' . 'Folder ' . $val_value['data-folder'] . ' - ' . $val_value['data-folder-title'] . ' ' . $val_value['data-folder-date'],
        );
        $record[$ft_1_key]['Series'] = $val_value['data-series-title'] . ', ' . $val_value['data-series-date'];
				$record[$ft_1_key]['Box'] = $val_value['data-box'];
				$record[$ft_1_key]['Folder'] = 'Folder ' . $val_value['data-folder'];
				$record[$ft_1_key]['FolderTitle'] = $val_value['data-folder-title'] . ' ' . $val_value['data-folder-date'];
			 $rendered_lists[] = theme_item_list(array(
          'items' => $items,
          'title' => '',
          'type' => 'ul',
          'attributes' => array(),
        ));
      }
    }
		$record['collection_title'] = $collection_title;
    // Process the title of the page.
    $types = _edan_extended_request_types();
    $title = $types[$request];

    // Get instructional text.
    $out .= _edan_extended_get_instructional_text( $request );
    
    // Loop through and display the requested items.
    foreach($rendered_lists as $rlist_key => $rlist) {
			$out .= '<div class="edan-extended-row chosen-items">' . "\n";
      $out .= '<div class="edan-extended-row-left checkbox-container">' . "\n";
      $out .= '<input type="checkbox" name="ContainerIDs" class="form-checkbox container_id" value="' . $record['ft_1'][$rlist_key] . '" ' . ((isset($_POST['ft_2']) && !in_array($record['ft_1'][$rlist_key],explode(',',$_POST['ft_2'])))? '' : 'checked="checked"') . '>' . "\n";
      $out .= '</div>' . "\n";
      $out .= '<div class="edan-extended-row-right collection-metadata">' . "\n" . $rlist . "\n" . '</div>' . "\n";
      $out .= '</div>' . "\n";
    }

    // Select All and Select None buttons.
    $out .= '<div class="edan-extended-row buttons-container">' . "\n";
    $out .= '<input type="button" id="selectAll" class="button edan-extended-button" value="Select All">' . "\n";
    $out .= '<input type="button" id="selectNone" class="button edan-extended-button" value="Select None">' . "\n";
    $out .= '<p><em>Requested items will be grouped by box in your research account.</em></p>' . "\n";
    $out .= '</div>' . "\n";
    //dumper($record);

    // Add Javascript
    drupal_add_js(
      drupal_get_path('module', 'edan_extended') . '/js/edanExtendedRequest.js',
      array('scope' => 'footer', 'weight' => 2)
    );

    // Get the form.
    $form = drupal_get_form('edan_extended_' . $request . '_request_form', $record);
    $out .= drupal_render($form);
  }

  return $out;

}

/**
 * Output the page
 */
$output = _edan_extended_request_page_build($request);
