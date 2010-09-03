<?php


/**
 * Implementation of hook_drush_command().
*
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function rebuild_project_paths_drush_command() {
  $items = array();

  $items['rebuild-project-paths'] = array(
    'description' => "Rebuilds the system table when you move projects around.",
    'aliases' => array('rpp'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE, // No bootstrap at all.
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function rebuild_project_paths_drush_help($section) {
  switch ($section) {
    case 'drush:rebuild-project-paths':
      return dt("Rebuilds project paths in the system table.");
  }
}

/**
 * Command callback for drush rebuild-project-paths
 */
function drush_rebuild_project_paths() {
  $result = db_query('SELECT filename, name FROM {system}');

  while ($mod = db_fetch_array($result)) {
    exec('find . -name ' . basename($mod['filename']), $modpath_arr);
    if (count($modpath_arr) == 1) {
      $modpath = ltrim($modpath_arr[0], './');
      $dbpath = $mod['filename'];
      if ($dbpath != $modpath) {
        drush_print("Updating " . $mod['name']. "'s path");
        db_query("UPDATE system SET filename='%s' WHERE name='%s'", $modpath, $mod['name']);
      }
    }
    unset($modpath_arr);
  }
}
