<?php

/**
 * @file
 * AJAX callbacks for the username_check module.
 */

/**
 * Invalid username.
 */
define('USERNAME_CHECK_INVALID_USERNAME', 'invalid_username');

/**
 * Invalid email.
 */
define('USERNAME_CHECK_INVALID_EMAIL', 'invalid_email');

/**
 * User is blocked.
 */
define('USERNAME_CHECK_USER_IS_BLOCKED', 'user_is_blocked');

/**
 * Username already exists.
 */
define('USERNAME_CHECK_USERNAME_EXISTS', 'username_exists');

/**
 * Username already exists and is logged in user.
 */
define('USERNAME_CHECK_USERNAME_EXISTS_LOGGED_IN', 'username_exists_logged_in');

/**
 * Email already exists.
 */
define('USERNAME_CHECK_EMAIL_EXISTS', 'email_exists');


/**
 * Main AJAX function: originality check menu callback.
 */
function username_check_callback() {
  $output = array();

  $username = $_GET['username'];

  $ret = user_validate_name($username);
  if ($ret) {
    $output['query'] = $username;
    $output['reason'] = USERNAME_CHECK_INVALID_USERNAME;
    $output['allowed'] = FALSE;
    $output['msg'] = $ret;
  }
  else {
    $ret = user_is_blocked($username);
    if ($ret) {
      $output['query'] = $username;
      $output['reason'] = USERNAME_CHECK_USER_IS_BLOCKED;
      $output['allowed'] = FALSE;
      $output['msg'] = t('The username %username is not allowed.', array('%username' => $username));
    }
    else {
      $username = check_plain($username);
      $ret = _username_check_is_user_exists($username);

      if ($ret) {
        $output['query'] = $username;
        $output['reason'] = USERNAME_CHECK_USERNAME_EXISTS;
        $output['allowed'] = FALSE;
        $output['msg'] = t('The username %username is already taken. If this is you, please ' . l('login,' , 'user') . ' or if you\'ve forgotten your password, ' . l('request a new password','user/password') . '.', array('%username' => $username));
      }
      else {
        $output['allowed'] = TRUE;
      }
    }
  }

  // Check if other modules have something to add here.
  drupal_alter('username_check_callback_output', $output);

  drupal_json_output($output);
}

/**
 * Query user table to check if such username is already exists.
 */
function _username_check_is_user_exists($username) {
  return db_query("SELECT COUNT(u.name) count FROM {users} u WHERE LOWER(u.name) = LOWER(:username)", array(':username' => $username))->fetchField();
}




/**
 * Main AJAX function: originality check menu callback for user profile 
 */
function username_check_profile_callback() {
  $output = array();

  $username = $_GET['profile'];  

  $ret = user_validate_name($username);
  if ($ret) {
    $output['query'] = $username;
    $output['reason'] = USERNAME_CHECK_INVALID_USERNAME;
    $output['allowed'] = FALSE;
    $output['msg'] = $ret;
  }
  else {
    $ret = user_is_blocked($username);
    if ($ret) {
      $output['query'] = $username;
      $output['reason'] = USERNAME_CHECK_USER_IS_BLOCKED;
      $output['allowed'] = FALSE;
      $output['msg'] = t('The username %username is not allowed.', array('%username' => $username));
    }
    else {
      $username = check_plain($username);
      // check to see if this username is the current users username
      $ret = _username_check_is_current_user($username);
      if ($ret) {
        $output['query'] = $username;
        $output['reason'] = USERNAME_CHECK_USERNAME_EXISTS_LOGGED_IN;
          $output['allowed'] = TRUE;
          $output['msg'] = t('The username %username is your username.' , array('%username' => $username));
      } else {

        $ret = _username_check_is_user_exists($username);
        if ($ret) {
          $output['query'] = $username;
          $output['reason'] = USERNAME_CHECK_USERNAME_EXISTS;
          $output['allowed'] = FALSE;
          $output['msg'] = t('The username %username is already taken.' , array('%username' => $username));
        }
        else {
          $output['allowed'] = TRUE;
        }
      }
    }
  }

  // Check if other modules have something to add here.
  drupal_alter('username_check_callback_output', $output);

  drupal_json_output($output);
}

/**
 * Query user table to check if this is the current user.
 */
function _username_check_is_current_user($username) {
  global $user;
  return db_query("SELECT COUNT(u.name) count FROM {users} u WHERE LOWER(u.name) = LOWER(:username) AND u.uid =" . $user->uid, array(':username' => $username))->fetchField();
}









/**
 * Main AJAX function: originality check menu callback for email.
 */
function username_check_mail_callback() {
  $output = array();

  $mail = $_GET['mail'];

  $ret = user_validate_mail($mail);
  if ($ret) {
    $output['query'] = $mail;
    $output['reason'] = USERNAME_CHECK_INVALID_EMAIL;
    $output['allowed'] = FALSE;
    $output['msg'] = $ret;
  }
  else {
    $ret = user_is_blocked($mail);
    if ($ret) {
      $output['query'] = $mail;
      $output['reason'] = USERNAME_CHECK_USER_IS_BLOCKED;
      $output['allowed'] = FALSE;
      $output['msg'] = t('The e-mail address %mail is not allowed.', array('%mail' => $mail));
    } else {
      $mail = check_plain($mail);
      $ret = _username_check_is_mail_exists($mail);

      if ($ret) {
        $output['query'] = $mail;
        $output['reason'] = USERNAME_CHECK_EMAIL_EXISTS;
        $output['allowed'] = FALSE;
        $output['msg'] = t('The e-mail address %mail is already in the system, you have an account here. Please ' . l('login,' , 'user') . ' or if you\'ve forgotten your password, ' . l('request a new password','user/password') .'.', array('%mail' => $mail));
      }
      else {
        $output['allowed'] = TRUE;
      }
    }
  }

  // Check if other modules have something to add here.
  drupal_alter('username_check_callback_output', $output);

  drupal_json_output($output);
}

/**
 * Query user table to check if such mail is already exists.
 */
function _username_check_is_mail_exists($mail) {
  return db_query("SELECT COUNT(u.mail) count FROM {users} u WHERE LOWER(u.mail) = LOWER(:mail)", array(':mail' => $mail))->fetchField();
}
