<?php
// Abbott Handerson Thayer and Thayer family papers, 1851-1999, bulk 1881-1950
// http://aaa.geekt.in/object/edanmdm/siris_arc_209598/
//
// Betty Parsons Gallery records and personal papers, circa 1920-1991, bulk 1946-1983
// http://aaa.geekt.in/object/edanmdm/siris_arc_209348/
//
// Alexander Calder papers, 1926-1967
// http://aaa.geekt.in/object/edanmdm/siris_arc_209447/
// 
// dumper($docs,0);
// dumper($docs_edanead);
// dumper($tags);
?>

<?php
if(isset($docs['content']['descriptiveNonRepeating'])):

  require(drupal_get_path('module', 'edan_extended') . '/includes/edan_extended_ead_record.inc');

else:
?>

  <div class="edan-search-result" id="<?php echo $docs['id']; ?>">
    <div class="edan-row">
      <?php
      $list = app_util_make_list_from_array($docs['content'], 'edanList', false, false);
      print $list;
      ?>
    </div>
  </div>

<?php endif; ?>
