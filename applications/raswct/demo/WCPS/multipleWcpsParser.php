<?php

/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/

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
function getCoverage($coverageId) {
  $coverageRaw = file_get_contents('http://kahlua.eecs.jacobs-university.de:8080/petascope?service=WCS&version=2.0.0&request=GetCoverage&coverageId=' . $coverageId);
  if (!strstr($coverageRaw, '<ows:ExceptionText>')) {
    $coverage = simplexml_load_string($coverageRaw);
    $lowDomainLimit = (string) $coverage->domainSet->Grid->limits->GridEnvelope->low;
    $highDomainLimit = (string) $coverage->domainSet->Grid->limits->GridEnvelope->high;
    $domainAxisLabel = (string) $coverage->domainSet->Grid->axisLabels;

    $values = explode(',', (string) $coverage->rangeSet->DataBlock->tupleList);

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

$coverages = explode(",", $_GET['coverageId']);
$results = array();
$names = array();
$i = 0;
foreach ($coverages as $coverage) {
  $res = getCoverage($coverage);
  $results[] = $res['data'];
  $domainInfo = $res['domainInfo'];
  $names[] = $coverages[$i];
  ++$i;
}

print json_encode(array('domainInfo' => $domainInfo, 'data' => $results, 'names' => $names));





