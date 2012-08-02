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
 * @class LedDisplay extends OutputWidget
 *
 * Defines a Led Display widget.
 *
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0
 */

Rj.namespace('Rj.Widget');

Rj.Widget.LedDisplay = new JS.Class(Rj.Widget.OutputWidget, {
    
    /**
     * Standard class constructor
     * @param <BaseQuery> query - the query that this widget will be able to modify
     * @param <float> value - the initial value displayed
     * @param <int> count - the number of digits of the display
     * @param <float> scale - float between 0 and 1 to scale the display
     * @param <int> offsetX - the X distance from canvas
     * @param <int> offsetY - the Y distance from canvas
     * @param <int> canvasWidth - the width of the canvas
     * @param <int> canvasHeight - the height of the canvas
     */
    initialize: function(query, value, count, scale, offsetX, offsetY, canvasWidth, canvasHeight){
        this.query = query;
        this.value = value || 0;
        this.count = count || 6;
        this.scale = scale || 0.4;
         this.offsetX = offsetX || 300;
        this.offsetY = offsetY || 80;
        this.canvasWidth = canvasWidth || 350;
        this.canvasHeight = canvasHeight || 200;
        this.id = "";
        this.callSuper(query, null);
    },
    
    /**
     * Getter for the value attribute
     */
    getValue: function(){
        return this.value;
    },
    
    /**
     * Setter for the value attribute
     * @param <float> value - the new value of the counter, with 2 digits precision
     * @event ledchange - fires when the values of the display changes
     */
    setValue: function(value){
      this.value = value;  
      this.fireEvent("ledchange", this.value);
    },
    
    /**
     * @override Rj.Widget.BaseWidget.renderTo
     */
    renderTo: function(selector){
        this.id = selector;
        var r = new Raphael(this.id, this.canvasWidth, this.canvasHeight);
        var led = new wso2vis.ctrls.SSegArray().initialValue(this.value).count(this.count).scale(this.scale).create(r, this.offsetX, this.offsetY);
         this.addListener("led", "ledchange", function(value){
            led.setValue(value);
        });
    }
    
})