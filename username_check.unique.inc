<?php
// $Id$

/**
 * @file
 * AJAX callbacks for the username_check module.
 */

/**
 * Main AJAX function: originality check menu callback.
 */
function username_check_callback() {
  $output = array();

  $username = $_GET['username'];

  // Verify the syntax of the given name
  $ret = user_validate_name($username);
  if ($ret) {
    $output['allowed'] = FALSE;
    $output['msg'] = $ret;
  }
  else {
    // Access Rules check
    $ret = drupal_is_denied('user', $username);
    if ($ret) {
      $output['allowed'] = FALSE;
      $output['msg'] = t('The username %username is not allowed.', array('%username' => $username));
    }
    else {
      $username = check_plain($username);
      $ret = _username_check_is_user_exists($username);

      if ($ret) {
        $output['allowed'] = FALSE;
        $output['msg'] = t('The name %username is already taken.', array('%username' => $username));
      }
      else {
        $output['allowed'] = TRUE;
        $output['msg'] = t('The username %username is available.', array('%username' => $username));
      }
    }
  }

  drupal_page_header();

  print drupal_to_js($output);
  exit();
}

/**
 * Query user table to check if such username already exists.
 */
function _username_check_is_user_exists($username) {
  return db_result(db_query("SELECT COUNT(u.name) FROM {users} u WHERE u.name = '%s'", $username));
}
