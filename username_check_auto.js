var usernameCheckTimer;
var usernameCheckUsername = '';

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var usernamePos = $('#username-check-wrapper input').position();
    var usernameWidth = $('#username-check-wrapper input').width();
    $('#username-check-informer').css({left: (usernamePos.left+usernameWidth+10)+'px', top: (usernamePos.top)+'px'}).show();
    
    $('#username-check-wrapper input').
      keyup(function() {
        if($('#username-check-wrapper input').val() != usernameCheckUsername) {
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
        if($('#username-check-wrapper input').val() != usernameCheckUsername) {
          usernameCheck();
        }
      });
  });
}

function usernameCheck() {
  clearTimeout(usernameCheckTimer);
  usernameCheckUsername = $('#username-check-wrapper input').val();

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
        $("#username-check-wrapper input").
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