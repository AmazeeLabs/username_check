if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var username = '';
    $('#edit-username-check-button').show();
    $('#edit-username-check-button').click(function() {
      username = $("#username-check-wrapper input").val();

      $.ajax({
        url: Drupal.settings.usernameCheck.ajaxUrl,
        data: {username: username},
        dataType: 'json',
        beforeSend: function() {
          $("#username-check-message").
            removeClass('username-check-message-accepted').
            removeClass('username-check-message-rejected').
            addClass('username-check-message-progress').
            html(Drupal.settings.usernameCheck.msgWait).
            show();
        },
        success: function(ret){
          if(ret['allowed']){
            $("#username-check-message").
              removeClass('username-check-message-progress').
              addClass('username-check-message-accepted');
          }
          else {
            $("#username-check-message").
              removeClass('username-check-message-progress').
              addClass('username-check-message-rejected');
          }
          $("#username-check-message").
            removeClass('username-check-message-progress').
            html(ret['msg']);
        }
      });
      return false;
    });
  });
}