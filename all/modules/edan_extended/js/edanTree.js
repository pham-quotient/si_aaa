jQuery(window).load(function(){
  // // If there's a location.hash set, call and initialize the AAA Player.
  // if(location.hash) {
  //   // Remove the hash.
  //   var dataDamsPath = location.hash.replace('#', '');
  //   // Call the Build Viewer jQuery plugin.
  //   jQuery('#player').buildPlayer('.edan-row', dataDamsPath);
  //   // Initialize the Player
  //   initPlayers( jQuery('.wellcomePlayer') );
  // }
});


jQuery(document).ready(function($) {

  var recordId = $('.edan-row').attr('data-ead-id');

  if(typeof recordId !== 'undefined') {
  
    var $thisTree = $('#tree').fancytree({
      // extensions: ['wide']
      source: {
        url: '/get_edan_ead_tree/ajax?recordId=' + recordId
      }
      ,checkbox: true
      ,icon: false
      ,generateIds: true
      ,idPrefix: ''
      ,selectMode: 3
      ,autoCollapse: true
      ,lazyLoad: function(event, data){

        // If a custom error is recieved, handle accordingly.
        if(data.node.key === 'custom-error') {
          data.result = $.Deferred(function (dfd) {
            setTimeout(function () {
              dfd.reject(new Error("TEST ERROR"));
            }, 250);
          });
        } else {
          data.result = {
            type: 'GET'
            ,dataType: 'json'
            ,url: '/get_edan_ead_tree/ajax?recordId=' + recordId + '&refId=' + data.node.key
          };
        }

      }
      ,renderNode: function(event, data) {
        data.node.extraClasses = 'ws-wrap';
      }
      ,postProcess: function(event, data) {

        // console.log(data.response);

        var eadXmlData = JSON.parse(Drupal.settings.ead_xml_data);

        // Modify the titles.
        $.each(data.response, function(result_key, result_value) {

          if('damsPath' in result_value) {

            var nodeValueKey = eadXmlData[result_value.key],
                damsPathParts = result_value.level.replace('Folder ', ''),
                damsPathParts = damsPathParts.split(', '),
                damsPathPartsArray = damsPathParts[1].split('-'),
                folderLabel = (damsPathPartsArray.length > 1) 
                  ? (damsPathPartsArray[1]-damsPathPartsArray[0])+1 + ' Folders' 
                  : nodeValueKey['data-folder-title'] + ', ' + nodeValueKey['data-folder-date'];

            data.response[result_key].title = '<span data-damspath="' + result_value.damsPath + '">' + damsPathParts[0] + ': ' + folderLabel + '</span>';
          }
          else if((result_value.level.indexOf('Box') > 0) || (result_value.level.indexOf('Folder') > 0)) {
            // console.log(result_value.level);
          }
          else {
            data.response[result_key].title = result_value.level + ': ' + result_value.title;
          }
        });
      }
      ,expand: function(event, data) {

        var eadXmlData = JSON.parse(Drupal.settings.ead_xml_data),
            childNodes = data.node.getChildren();

        $.each(childNodes, function(node_key, node_value) {

          if(typeof node_value.data.damsPath !== 'undefined') {

            var damsPathsObj = node_value.data.damsPath.split(' '),
                hasChildren = node_value.hasChildren();

            if(!hasChildren && (damsPathsObj.length > 1)) {
              $.each(damsPathsObj, function(dams_key, dams_value) {
                var nodeValueKey = eadXmlData[node_value.key];
                node_value.addChildren({
                  title: '<span data-damspath="' + dams_value + '">Folder ' + (dams_key + 1) + ': ' + nodeValueKey['data-folder-title'] + ', ' + nodeValueKey['data-folder-date'] + '</span>'
                });
              });
            }

          }
        });

        // var level = data.node.getLevel(),
        //     titlePadding = '';

        //     console.log($(data.node));

        // switch(level) {
        //   case 2:
        //     data.node.extraClasses = 'padding-level-2';
        //     break;
        //   case 3:
        //     data.node.extraClasses = 'padding-level-3';
        //     break;
        //   case 4:
        //     data.node.extraClasses = 'padding-level-4';
        //     break;
        //   case 5:
        //     data.node.extraClasses = 'padding-level-5';
        //     break;
        //   default:
        //     data.node.extraClasses = '';
        // }

      }
      ,click: function(event, data) {
        // logEvent(event, data, ", targetType=" + data.targetType);
        // If the response is not a custom error, and targetType is title, go ahead and process stuff.
        if((data.node.key !== 'custom-error') && (data.targetType === 'title')) {

          if(data.node.title.indexOf('data-damspath') !== -1) {

            var dataDamsPath = $(data.node.title).attr('data-damspath');

            if(typeof dataDamsPath !== 'undefined') {
              var damsPathsObj = dataDamsPath.split(' ');
              if(damsPathsObj.length === 1) {
                // Call the Build Viewer jQuery plugin.
                $('#player').buildPlayer('.edan-row', dataDamsPath);
                // Write the location hash to the address bar.
                parent.location.hash = dataDamsPath;
                // Initialize the Player
                initPlayers($('.wellcomePlayer'));

                // Query for DAMS images, so they can be offered as downloads.
                $.ajax({
                  type: 'GET'
                  ,dataType: 'json'
                  ,url: '/get_edan_dams/ajax'
                  ,data: ({query: dataDamsPath})
                  ,success: function(results) {

                    $('#download-modal').find('ul').remove();

                    var heading = $('<h3 />').text('Image List');
                    var ul = $('<ul />').attr('style', 'list-style: none; padding-left: 1.4rem;');
                    var imagesObject = [];

                    $.each(results, function(image_path_key, image_path_value) {
                      var li = $('<li />')
                        .attr('style', 'padding: 2rem; display: inline;')
                        .html('<a href="' + image_path_value + '" target="_blank"><img src="' + image_path_value + '&max_w=80" alt="Image #' + (image_path_key+1) + '"></a>');
                      $(ul).append(li);

                      imagesObject.push({
                        path: image_path_value,
                      });

                      $(ul).attr('data-images-json', JSON.stringify(imagesObject));

                    });

                    $('#download-modal').append(heading, ul);
                    $('#player').append();

                    $('#download-button-container').show();

                  }
                });
              }
            }

          }

        }
      }
    }).on('click', '.fancytree-title', function(event){
      // Add a click handler to all node titles (using event delegation).
      var node = $.ui.fancytree.getNode(event);
      // If the response is not a custom error, go ahead and process stuff.
      if(node.key !== 'custom-error') {
        node.toggleExpanded();
      }
    });

  }

  // Check for node children.
  var treeChecker = setInterval(firstInit, 500);

  function firstInit() {

    var tree = $("#tree").fancytree("getTree"),
        activeNode = tree.getActiveNode(),
        eadXmlData = JSON.parse(Drupal.settings.ead_xml_data);

    // If there are no active nodes, expand the first tree node.
    if(tree && !activeNode) {

      // Traverse all nodes recursively.
      tree.visit(function(node){

        if(!node.selected) {

          // Expand the first node, then see if the AAA Viewer can be loaded.
          node.setExpanded(true).done(function(){

            var nodeChildren = node.getChildren();

            if(nodeChildren) {

              // Stop checking for node children.
              clearInterval(treeChecker);

              var nodeValueKey = eadXmlData[nodeChildren[0].key];
              var dataDamsPath = (typeof nodeValueKey !== 'undefined') ? nodeValueKey['data-damspath-1'] : false;
              
              // If there's a location.hash set, call and initialize the AAA Player.
              if(location.hash) {
                // Remove the hash, overwrite the dataDamsPath var.
                dataDamsPath = location.hash.replace('#', '');
              }

              // If we have a legitimate DAMS path, load the AAA viewer.
              if(dataDamsPath && (dataDamsPath.indexOf('CollectionsOnline') !== -1)) {
                // Call the Build Viewer jQuery plugin.
                jQuery('#player').buildPlayer('.edan-row', dataDamsPath);
                // Initialize the Player
                initPlayers( jQuery('.wellcomePlayer') );
              }

            }

          });

          return false;
        }
      });
    }

  }

  // activeNode.setExpanded(true).done(function(){
  //   // The 'done' function is called after the expansion animation finished.
  //   alert("Node was expanded");
  // }).fail(function(){
  //   // The 'fail' function is called on error
  // }).always(function(){
  //   // The 'always' function is always called.
  // });

  function logEvent(event, data, msg) {
    var args = $.isArray(args) ? args.join(', ') :
    msg = msg ? ': ' + msg : '';
    $.ui.fancytree.info("Event('" + event.type + "', node=" + data.node + ")" + msg);
  }

});