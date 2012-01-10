/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann / rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/

/**
 * Defines several functions exposed across the project as utilities
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */


/**
 * Creates namespaces to be used for scoping variables and classes so that they are not global.
 * Specifying the last node of a namespace implicitly creates all other nodes. 
 * @param <string> namespace1
 * @param <string> namespace2
 * @param <string> etc
 * @return <object> The namespace object. (If multiple arguments are passed, this will be the last namespace created)
 * @method namespace
 */
Rj.namespace = function(){
  var len1 = arguments.length,
  i = 0,
  len2,
  j,
  main,
  ns,
  sub,
  current;
                
  for(; i < len1; ++i) {
    main = arguments[i];
    ns = arguments[i].split('.');
    current = window[ns[0]];
    if (current === undefined) {
      current = window[ns[0]] = {};
    }
    sub = ns.slice(1);
    len2 = sub.length;
    for(j = 0; j < len2; ++j) {
      current = current[sub[j]] = current[sub[j]] || {};
    }
  }
  return current;
}


/**
 * The default generator-id
 */
Rj.currentId = 1;

/**
 * Returns the id of an element that matches the given selector. If the element
 * does not have an id, one will be generated for it
 * @param <string> selector 
 */
Rj.getId = function(selector){
  if(!jQuery(selector).attr('id')){
    jQuery(selector).attr('id', 'rj-' + Rj.currentId);    
  }
  return jQuery(selector).attr('id');
}
