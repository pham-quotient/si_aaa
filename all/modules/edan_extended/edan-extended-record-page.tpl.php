<?php
// dumper($docs);
// dumper($docs_edanead);
// dumper($display_data);

// Process help text, coming from the user's entry in the 'AAA Viewer Settings' interface.
$help_text = array(rawurlencode(nl2br(variable_get('aaa_player_help_text'))));
$help_text_json = json_encode($help_text, JSON_HEX_QUOT | JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS);

$interviewee = !empty($docs['content']['indexedStructured']['name']) ? 
  '<p>' . $docs['content']['indexedStructured']['name'][0]['content'] . '</p>' : '';
$topics = !empty($docs['content']['indexedStructured']['topic']) ? 
  '<p>' . implode(', ', $docs['content']['indexedStructured']['topic']) . '</p>' : '';
$active_in = !empty($docs['content']['indexedStructured']['place']) ? 
  '<p>Active in: ' . implode(', ', $docs['content']['indexedStructured']['place']) . '</p>' : '';
?>

<div class="edan-record-container">
  <div class="edan-row" id="<?php echo $docs['type'] . ':' . $docs['content']['descriptiveNonRepeating']['record_ID']; ?>" data-element-value="<?php print $docs['set_name_fq']; ?>" data-help-text='<?php print $help_text_json; ?>' data-metadata-fields='<?php print $docs['metadata_fields']; ?>' data-aaa-player-url='<?php print variable_get('aaa_player_url'); ?>' >

    <div class="record-sub-heading">
      <?php print $interviewee; ?>
      <?php print $topics; ?>
      <?php print $active_in; ?>
      <?php if(!empty($docs_edanead)) : ?>
        <p style="font-weight: normal; font-size: 1.3rem;">EDAN ID: <?php print $docs_edanead['ead_id']; ?></p>
        <p style="font-weight: normal; font-size: 1.3rem;">XML: <a href="<?php print $docs_edanead['filelocation']; ?>" target="_blank"><?php print $docs_edanead['filelocation']; ?></a></p>
      <?php endif; ?>
    </div>

    <!-- AAA Player -->
    <div id="player">
    </div>

  </div>

  <!-- Tabs -->
  <div id="findingAidTabs" class="si-tabs responsive-tab edan-extended-tabs">

    <ul>
      <?php foreach($display_data as $list_key => $list_value) : ?>
        <?php if(!empty($list_value)) : ?>
          <li><a href="#<?php print str_replace(' ', '-', strtolower($list_key)); ?>"><?php print $list_key; ?></a></li>
        <?php endif; ?>
      <?php endforeach; ?>
    </ul>

    <div class="edan-extended-tab-content-container">

      <!-- Tab Content -->
      <div class="edan-extended-tab-content">

        <?php foreach($display_data as $display_key => $display_value) : ?>

          <?php if(!empty($display_value)) : ?>

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

          <?php endif; ?>
          
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
</div>