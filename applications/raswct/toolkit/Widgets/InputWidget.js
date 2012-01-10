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
 * @class InputWidget extends BaseWidget
 * 
 * InputWidget is a simple grouper class that helps better define the 
 * relationships between widgets
 * 
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace('Rj.Widget');

Rj.Widget.InputWidget = new JS.Class(Rj.Widget.BaseWidget, {
    
    /**
     * Standard class constructor
     * @param <string> value - the value displayed in the widget
     */
    initialize: function(value){
        this.value = value || '';
        this.callSuper();
    },
    
    /**
     * Standard getter for the value attribute
     */
    getValue: function(){
        return this.value;
    },
    
    /**
     * Standard setter for the value attribute
     * @param <string> value - the new value to be set
     */
    setValue: function(value){
        this.value = value;
    }
})

