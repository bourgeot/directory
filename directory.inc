<?php

//directory.inc
/**
 * Create a single term and add to a specific vocabulary if not already present
 *  @vocab - the machine name of the vocabulary
 *  @term - the human readable name of the term to be added
 *
 *  @desc - the humna readable description of the term to be added (Optional)
 *  @weight - the weight of the term in the vocabulary list (Optional)
 */
function _create_term($vocab_name, $term, $desc = '', $weight = 0, $add_parent = 0)  {
  $added = NULL;
  $added = taxonomy_get_term_by_name($term);
  $vid = variable_get($vocab_name . '_vocabulary', 0);
  if ($added == '' || $added == NULL || $vid > 0)  {
    $new_term = array(
      'name' => $term,
      'description' => $desc,
      'parent' => array($add_parent),
      'weight' => $weight,
      'vid' => $vid,
    );
    $term_obj = (object) $new_term;
    taxonomy_term_save($term_obj);
    //in case you want it.
    if ($term_obj->tid > 0) {
      return $term_obj->tid;
    }
    else { return 'trouble'; }
  }
}
/*
 * use sia.uits interface to get the current ua org structure.
 */
function _fetch_ua_orgs($url = '') {
  if ($url == '') {
    //get them all
    $url = 'https://siaapps.uits.arizona.edu/home/orgs/ua_orgs/index.json';
  }
  $objs = array();
  $f = file_get_contents($url);
  if( $f !== FALSE ) {
    $objs = json_decode($f);
    //
    $o = array();
  $needOne = true;
    do {
      $obj = array_pop($objs);
      /* I am going to rebuild the hierarchy to flatten out the object structure a bit.
       * obj.UaOrg, obj.ParentUAOrg, and obj.ChildUAOrg all have internal structure.
  * Child Ua Org is a list of objects that have ids.
       * Rather than step through all of it I am going to rely on the fact that
       * there is a single parent and multiple children.
       * If an object has no children (leaf node) then build it.
       * The remaining nodes are intermediates.
       * Sort the leaves by parent id and build the parent.
       * Then look that id up in 'data' and get its parent.
       * First build the flat terms and ignore internal structure, but
       * Note it. I wrote this earlier in javascript so that's what's below.
       * var element = new orgNode(obj.UaOrg.id, obj.UaOrg.name,
       * obj.UaOrg.parent_id, obj.UaOrg.parent_name);
       */
       //create the term and reference it.
      if ($obj->UaOrg->finOnly != 1 || $obj->UaOrg->hr_code != '') {
        $o[$obj->UaOrg->hr_code] = array(
          'org_id' => $obj->UaOrg->hr_code,
          'dept_name' => $obj->UaOrg->hr_name,
          'org_parent' => $obj->UaOrg->hr_parent_code,
          'org_parent_name' => $obj->UaOrg->hr_parent_name,
          'tid' => 0,
        );
      }
    } while (count($objs) > 0);


  return $o;
  }
  else {
    return FALSE;
  }
}
function directory_loader($vocabulary_name) {
  $vid = variable_get($vocabulary_name . '_vocabulary', 0);
  if ($vid == 0) {
      return FALSE;
  }
  else {
    $count = 0;
    switch ($vocabulary_name) {
      case 'ua_organizations':
        $orgs = _fetch_ua_orgs();
        //result has a bunch of objects in it. Transform the data to an array where each
        //entry is indexed by its dept id and is itself an array.
        if(!empty($orgs)) {
          foreach($orgs as $org) {
            $orgs[$org['org_id']]['tid'] = _create_term($vocabulary_name, $org['dept_name'], $org['org_id'], $count +1);
            $count++;
            //drupal_set_message( $org['tid'] . 'created');
    //$orgs[$org['org_id']]['tid'] = $zans;
          }
          //now build structure
          reset($orgs);
    //dsm($orgs);
    $term_array = taxonomy_get_tree($vid, 0);
          foreach($term_array as $term) {
    $parent_set = array();
    $term_org = $orgs[$term->description];
    $parent_org = $orgs[$term_org['org_parent']];
    $ptid = $parent_org['tid'];
    if ($term->tid == $ptid) {
      $ptid = 0;
    }
    $term->parent = $ptid;
    taxonomy_term_save($term);

            //drupal_set_message( 'using ' . $org['tid']);
/*            if($orgs[$term->description]['org_parent_name'] <> '') {
              drupal_set_message('with ' . $term->tid . ' looking for ' . $orgs[$term->description]['org_parent_name'] );
              //$parent_array = taxonomy_get_term_by_name($orgs[$term->description]['org_parent_name']);
    $parent_term = taxonomy_term_load($orgs[$term->description][
              if(count($parent_array) > 0) {
                foreach ($parent_array as $pterm) {
                  $parent_set[] = $pterm->tid;
                }
                $term->parent = $parent_set;
                taxonomy_term_save($term);
              }
              //drupal_set_message('made it past term create loop. orgs has ' . count($orgs) . ' terms.');            }
            }
*/
          }
        }
      break;
      case 'ua_locations':
      break;
      case 'ua_affiliations':
        $set = array(
          'student',
          'admit',
          'employee',
          'studentworker',
          'gradasst',
          'faculty',
          'staff',
          'affiliate',
          'dcc',
          'retiree',
          'member',
          'former-student',
          'former-employee',
          'former-studentworker',
          'former-gradasst',
          'former-faculty',
          'former-staff',
          'former-affiliate',
          'former-dcc',
          'former-member',
        );
        foreach($set as $letter) {
          _create_term($vocabulary_name, $letter);
          $count++;
        }
      break;
      case 'alphabet':
        $set =  array(
          'A',
          'B',
          'C',
          'D',
          'E',
          'F',
          'G',
          'H',
          'I',
          'J',
          'K',
          'L',
          'M',
          'N',
          'O',
          'P',
          'Q',
          'R',
          'S',
          'T',
          'U',
          'V',
          'W',
          'X',
          'Y',
          'Z',
        );
        foreach($set as $letter) {
          _create_term($vocabulary_name, $letter);
          $count++;
        }
      break;
      case 'months':
        $set =  array(
          'January',
          'February',
          'March',
          'April',
          'May',
          'June',
          'July',
          'August',
          'September',
          'October',
          'November',
          'December',
        );
        foreach($set as $letter) {
          _create_term($vocabulary_name, $letter, '', $count + 1);
          $count++;
        }
      break;
      case 'directory_ids':
        //first, amass all the currently active users and see if they have
        //terms associated with their names. If so, fine. If not, add them.
        $count = count(create_new_ids());
      break;
    }
    drupal_set_message('Added ' . $count . ' terms to ' . $vocabulary_name . '.', 'status');
  }
}
