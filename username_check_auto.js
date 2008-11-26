var usernameCheckTimer;
var usernameCheckUsername = '';

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var usernamePos = $('#edit-name').position();
    var usernameWidth = $('#edit-name').width();
    $('#username-check-informer').css({left: (usernamePos.left+usernameWidth+10)+'px', top: (usernamePos.top)+'px'}).show();
    
    $('#edit-name').
      keyup(function() {
        if($('#edit-name').val() != usernameCheckUsername) {
          clearTimeout(usernameCheckTimer);
          usernameCheckTimer = setTimeout('usernameCheck()', Drupal.settings.usernameCheck.delay*1000);
          
          if(!$("#username-check-informer").hasClass('username-check-informer-progress')) {
            $("#username-check-informer").
              removeClass('username-check-informer-accepted').
              removeClass('username-check-informer-rejected');
          }
              
          $("#username-check-message").
            hide();
        }
      }).
      blur(function() {
        if($('#edit-name').val() != usernameCheckUsername) {
          usernameCheck();
        }
      });
  });
}

function usernameCheck() {
  clearTimeout(usernameCheckTimer);
  usernameCheckUsername = $('#edit-name').val();

  $.ajax({
    url: Drupal.settings.usernameCheck.ajaxUrl,
    data: {username: usernameCheckUsername},
    dataType: 'json',
    beforeSend: function() {
      $("#username-check-informer").
        removeClass('username-check-informer-accepted').
        removeClass('username-check-informer-rejected').
        addClass('username-check-informer-progress');
    },
    success: function(ret){
      if(ret['allowed']){
        $("#username-check-informer").
          removeClass('username-check-informer-progress').
          addClass('username-check-informer-accepted');
        $("#edit-name").
          removeClass('error');
      }
      else {
        $("#username-check-informer").
          removeClass('username-check-informer-progress').
          addClass('username-check-informer-rejected');
        
        $("#username-check-message").
            addClass('username-check-message-rejected').
            html(ret['msg']).
            show();
      }
    }
   });
}