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
 * @class Gauge extends OutputWidget
 *
 * Defines a Gauge widget.
 *
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0
 */

Rj.namespace('Rj.Widget');

Rj.Widget.Gauge = new JS.Class(Rj.Widget.OutputWidget, {
    
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
    initialize: function(query, minVal, maxVal, value, dialRadius, smallTick, largeTick, minAngle, maxAngle, offsetX, offsetY, canvasWidth, canvasHeight, textColor, pathColor){
        this.query = query;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.dialRadius = dialRadius || 100;
        this.smallTick = smallTick || 1;
        this.largeTick = largeTick || 10;
        this.minVal = minVal || 0;
        this.maxVal = maxVal || 100;
        this.value = value || 0;
        this.minAngle = minAngle || 0;
        this.maxAngle = maxAngle || 0;
        this.offsetX = offsetX || 160;
        this.offsetY = offsetY || 135;
        this.canvasWidth = canvasWidth || 350;
        this.canvasHeight = canvasHeight || 250;
        this.id = "";
        this.callSuper(query, null);
        this.textColor = textColor || "#000";
        this.pathColor = pathColor || "#777";
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
     * @event gaugechange - fires when the values of the display changes
     */
    setValue: function(value){
        this.value = value;  
        this.fireEvent("gaugechange", this.value);
    },
    
    /**
     * @override Rj.Widget.BaseWidget.renderTo
     */
    renderTo: function(selector){
        this.id = selector;
        var r = new Raphael(this.id, this.canvasWidth, this.canvasHeight);
        var gauge =new wso2vis.ctrls.CGauge() .dialRadius(this.dialRadius) .smallTick(this.smallTick) .largeTick(this.largeTick) .minVal(this.minVal) .maxVal(this.maxVal) 
        .ltlen(18) .stlen(15) .needleCenterRadius(10) .needleBottom(20)	       
        if(this.minAngle){
            gauge = gauge.minAngle(this.minAngle);
        }
        if(this.maxAngle){
            gauge = gauge.maxAngle(this.maxAngle);
        }
        gauge = gauge.create(r, this.offsetX, this.offsetY);		
        gauge.setValue(this.value);
        //setting colors of text and ticks
        var self = this;
        var paths = $("#" + this.id).find("path");
        var i = 0;
        paths.each(function(){
            $(this).attr("stroke", self.pathColor);
            i++;
            if(i+1 == paths.size()){
                return false;
            }    
        });
        $("#" + this.id).find("text").each(function(){
            $(this).attr("stroke", self.textColor)
        });
        this.addListener("gauge", "gaugechange", function(value){
            gauge.setValue(value);
        });
    }
    
})