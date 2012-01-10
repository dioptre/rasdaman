<?php

/**

 * Gets information about a WCPS coverage and returns it in a JSON encode of an array,
 * which can be easily accessed through javascript.
 * 
 * The parameter it needs is the coverage id, which is passed through GET['coverageId']
 * 
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @version 1.0
 */
define('COVERAGE_URL', 'http://kahlua.eecs.jacobs-university.de:8080/petascope?service=WCS&version=2.0.0&request=GetCoverage&coverageId=');

function getCoverage($coverageId, $threshold) {
  $coverageRaw = file_get_contents(COVERAGE_URL . $coverageId);
  if (!strstr($coverageRaw, '<ows:ExceptionText>')) {
    $coverage = simplexml_load_string($coverageRaw);
    $lowDomainLimit = (string) $coverage->domainSet->Grid->limits->GridEnvelope->low;
    $highDomainLimit = (string) $coverage->domainSet->Grid->limits->GridEnvelope->high;
    $domainAxisLabel = (string) $coverage->domainSet->Grid->axisLabels;

    $values = explode(',', (string) $coverage->rangeSet->DataBlock->tupleList);
    foreach($values as &$value){
      $value = $value > $threshold ? 1 : 0;
    }

    $results = array(
        'domainInfo' => array(
            'lowLimit' => $lowDomainLimit,
            'highLimit' => $highDomainLimit,
            'axisLabel' => $domainAxisLabel
        ),
        'data' => $values
    );
  } else {
    $errorRaw = explode('<ows:ExceptionText>', $coverageRaw);
    $error = explode('</ows:ExceptionText>', $errorRaw[1]);
    $results = array(
        'error' => trim($error[0])
    );
  }
  return $results;
}

$results = getCoverage($_GET['coverageId'], $_GET['skip']);

print json_encode(array('domainInfo' => $results['domainInfo'], 'data' => $results['data']));





