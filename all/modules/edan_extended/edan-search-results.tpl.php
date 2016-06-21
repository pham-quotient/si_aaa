<?php
/**
 * EDAN Search Results Template
 *
 * @file edan-search-results.tpl.php
 * @version 0.1
 */
?>

<!-- Remove Facets Links -->
<div id="remove_facets_links">
  <?php print $filter_menus['remove_facets_links']; ?>
</div>

<!-- Render the Filter Tab Menus -->
<div id="filterTabs" class="si-tabs responsive-tab edan-extended-search-tabs">
  <?php print $filter_menus['list_items']; ?>
  <?php print $filter_menus['divs']; ?>
</div>

<div class="edan-search clearfix">

  <!-- Render the Top Pager -->
  <?php echo $pager; ?>

  <!-- Render the EDAN Search Results Summary -->
  <div class="edan-results-summary">
    <?php print($results_summary); ?>
  </div>

  <!-- Render the EDAN Search Results -->
  <div class="grid-3-region clearfix">

    <?php foreach ($docs as $doc_key => $doc_value): ?>

      <?php
      // Set up the record type and date.
      $record_type = $date = '';
      $record_type_class = !empty($doc_value['background_style']) ? ' edan-record-type-filled-background' : '';
      $text_shadow_style = !empty($doc_value['background_style']) ? 'text-shadow: 2px 2px #000' : '';
      if(!empty($doc_value['content']['freetext'])) {
        if(!empty($doc_value['content']['freetext']['objectType'][1]['content'])) {
          $record_type = '<div class="edan-record-type' . $record_type_class . '">' . $doc_value['content']['freetext']['objectType'][1]['content'] . '</div>';
        } else {
          $record_type = '<div class="edan-record-type' . $record_type_class . '">' . $doc_value['content']['freetext']['objectType'][0]['content'] . '</div>';
        }
        if(!empty($doc_value['content']['freetext']['date'][1]['content'])) {
          $date = '<div class="edan-date" style="' . $text_shadow_style . '">' . $doc_value['content']['freetext']['date'][1]['content'] . '</div>';
        }
        if(!empty($doc_value['content']['freetext']['date'][0]['content'])) {
          $date = '<div class="edan-date" style="' . $text_shadow_style . '">' . $doc_value['content']['freetext']['date'][0]['content'] . '</div>';
        }
      }
      // Set up the single search result grid item.
      $single_search_result = '<a href="' . $doc_value['local_record_link'] . '">' . "\n";
      $single_search_result .= '<div class="grid-3-region--' . $doc_value['grid_3_region_class'] . '" style="' . $doc_value['background_style'] . '">' . $record_type . "\n";
      $single_search_result .= '<h5 class="title" style="' . $text_shadow_style . '">' . $doc_value['record_title'] . '</h5>' . "\n";
      $single_search_result .= $date . "\n";
      $single_search_result .= '</div>' . "\n";
      $single_search_result .= '</a>' . "\n";
      ?>

      <?php print $single_search_result; ?>

    <?php endforeach; ?>

  </div>

  <!-- Render the Bottom Pager -->
  <?php echo $pager; ?>

</div>