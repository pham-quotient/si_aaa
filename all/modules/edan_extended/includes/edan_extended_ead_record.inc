<?php
// dumper($docs);
// dumper($docs_edanead);
// dumper($display_data);

// Process help text, coming from the user's entry in the 'AAA Viewer Settings' interface.
$help_text = array(rawurlencode(nl2br(variable_get('aaa_player_help_text'))));
$help_text_json = json_encode($help_text, JSON_HEX_QUOT | JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS);
// Populate the data-ead-id data attribute with the EAD record ID.
$data_ead_id = (!empty($docs_edanead['ead_id'])) ? ' data-ead-id="' . $docs_edanead['ead_id'] . '"' : '';

$topics = !empty($docs['content']['indexedStructured']['topic']) ? 
  '<p>' . implode(', ', $docs['content']['indexedStructured']['topic']) . '</p>' : '';
$active_in = !empty($docs['content']['indexedStructured']['place']) ? 
  '<p>Active in: ' . implode(', ', $docs['content']['indexedStructured']['place']) . '</p>' : '';
?>

<?php 
// DO WE NEED THIS??? 
print drupal_render($menu);
?>

<div class="edan-record-container">
  <div class="edan-row" id="<?php echo $docs['content']['descriptiveNonRepeating']['record_ID']; ?>" data-element-value="urlead:<?php print $docs_edanead['ead_id']; ?>" data-help-text='<?php print $help_text_json; ?>' <?php print $data_ead_id; ?> data-aaa-player-url='<?php print variable_get('aaa_player_url'); ?>'>
    <div class="record-sub-heading">
      <p><?php print $docs_edanead['creator']; ?></p>
      <?php print $topics; ?>
      <?php print $active_in; ?>
      <p style="font-weight: normal; font-size: 1.3rem;">EDAN ID: <?php print $docs_edanead['ead_id']; ?></p>
      <p style="font-weight: normal; font-size: 1.3rem;">XML: <a href="<?php print $docs_edanead['filelocation']; ?>" target="_blank"><?php print $docs_edanead['filelocation']; ?></a></p>
    </div>
    <!-- AAA Player -->
    <div id="player">
    </div>
    <div id="download-button-container">
      <input type="submit" id="download_modal_trigger" value="Single Images" class="form-submit">
      <input type="submit" id="create_zip_trigger" value="Create ZIP" class="form-submit">
    </div>
    <div class="edan-extended-preloader">
      <img src="/<?php echo drupal_get_path('module', 'edan_extended'); ?>/images/indicator-white-bg-72x72.gif" alt="preloader image">
    </div>
  </div>

  <!-- Tabs -->
  <div id="findingAidTabs" class="si-tabs responsive-tab edan-extended-tabs">

    <ul>
      <?php foreach($display_data as $list_key => $list_value) : ?>
        <li><a href="#<?php print str_replace(' ', '-', strtolower($list_key)); ?>"><?php print $list_key; ?></a></li>
      <?php endforeach; ?>
    </ul>

    <div class="edan-extended-tab-content-container">

      <!-- Tab Content -->
      <div class="edan-extended-tab-content">

        <?php foreach($display_data as $display_key => $display_value) : ?>

          <div id="<?php print str_replace(' ', '-', strtolower($display_key)); ?>">

            <?php foreach($display_value as $dkey => $dvalue) : ?>

              <?php if(!empty($dvalue)) : ?>
                <article role="article">
                  <header>
                    <?php print !is_integer($dkey) ? '<h3>' . $dkey . '</h3>' : ''; ?>
                    <?php if(is_array($dvalue)) : ?>
                      <?php foreach($dvalue as $dk => $dv) : ?>
                        <?php if(!empty($dv)) : ?>
                          <p><strong><?php print $dk; ?></strong>: <?php print $dv; ?></p>
                        <?php endif; ?>
                      <?php endforeach; ?>
                    <?php else : ?>
                      <?php print !empty($dvalue) ? '<p>' . $dvalue . '</p>' : ''; ?>
                    <?php endif; ?>
                  </header>
                </article>
              <?php endif; ?>

            <?php endforeach; ?>

          </div>
        <?php endforeach; ?>

      </div>
      <!-- // edan-extended-tab-content -->

      <div class="edan-extended-collection-tag-content">
        <h3>Tags</h3>
        <?php print $tags; ?>
      </div>
      <!-- // edan-extended-collection-tag-content -->

    </div>
    <!-- // edan-extended-tab-content-container -->

  </div>
  <!-- // Tabs -->

  <div id="download-modal" title="Downloads">
    <ul><li>No downloads available</li></ul>
    <!--
    <div class="single-image-container">
      <figure class="ui-draggable">
        <img src="" alt="">
      </figure>
    </div>
    -->
  </div>

</div>