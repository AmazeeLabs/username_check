if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $("#edit-username-button").bind("click", function() {
      var username = $("input[@id='edit-name']").val();
      $.getJSON(Drupal.settings.username_check.ajaxUrl,
        {username: username},
         function (data) {
           var message = $("div[@id='username-message']");
           message.html(data.msg);
           message.removeClass('username-message-progress');
           if(data.exists === true){
              message.removeClass('username-accepted');
              message.addClass('username-rejected');
           }
           else{
              message.removeClass('username-rejected');
              message.addClass('username-accepted');
           }
           message.show();
         });
      return false;
    });
    
    $("div[@class='username-message']").ajaxStart(function(){
      $(this).html(Drupal.settings.username_check.msgWait);
      $(this).removeClass('username-accepted');
      $(this).removeClass('username-rejected');
      $(this).addClass('username-message-progress');
      $(this).show();
    });


    $("div[@class='username-message']").hide();
  });
}