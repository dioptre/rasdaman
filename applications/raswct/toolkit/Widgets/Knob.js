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
 * @class Knob extends InputWidget
 *
 * Defines a knob widget.
 *
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0
 */

Rj.namespace('Rj.Widget');

Rj.Widget.Knob = new JS.Class(Rj.Widget.InputWidget, {
    
    /**
     * Standard class constructor
     * @param <string> label - the initial label displayed under the knob
     * @param <array> selectOpts - an array with the values displayed on the knob
     * @param <int> dialRadius - the knob radius
     * @param <int> smallTick - the interval delimited by the small ticks
     * @param <int> largeTick - the interval delimited by the large ticks
     * @param <int> minVal - the smallest value displayed on the knob
     * @param <int> maxVal - the largest values displayed on the knob
     * @param <int> knobVal - the initial value of the knob
     * @param <bool> snap - if true, the knob will jump only between large ticks
     * @param <int> minAngle - the angle at which the ticks begin
     * @param <int> maxAngle - the angle at which the ticks end
     * @param <int> offsetX - the X distance from canvance where the knob is position
     * @param <int> offsetY - the Y distance from canvance where the knob is position
     * @param <int> canvasWidth - the width of the canvas in which the knob is displayed
     * @param <int> canvasHeight - the height of the canvas in which the knob is displayed
     * @param <string> color - the color of the tickes and the text
     */
    initialize: function( minVal, maxVal, selectOpts, label, dialRadius, smallTick, largeTick, knobVal, snap, minAngle, maxAngle, offsetX, offsetY, canvasWidth, canvasHeight, color){
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.dialRadius = dialRadius || 40;
        this.smallTick = smallTick || 4;
        this.largeTick = largeTick || 20;
        this.label = label || this.minVal;
        this.knobVal = knobVal || 0;
        this.snap = snap || false;
        this.selectOpts = selectOpts || null;
        this.minAngle = minAngle || 0;
        this.maxAngle = maxAngle || 0;
        this.offsetX = offsetX || 80;
        this.offsetY = offsetY || 80;
        this.canvasWidth = canvasWidth || 200;
        this.canvasHeight = canvasHeight || 200;
        this.color = color || "#777";
        this.callSuper(this.knobVal);
    },
    
    /**
     * Standard getter for the knobVal attribute
     */
    getValue: function(){
        return this.knobVal;
    },
    
    /**
     * @override Rj.Widget.BaseWidget.renderTo
     * @event knobchange - fires when the knob value is changed
     */
    renderTo: function(selector){
        var r = Raphael(selector, this.canvasWidth, this.canvasHeight);
        var knob = new wso2vis.ctrls.Knob().dialRadius(this.dialRadius).smallTick(this.smallTick) .largeTick(this.largeTick).startVal(this.knobVal).snap(this.snap);
        if (this.selectOpts){
            knob = knob.selectOpts(this.selectOpts);
        }
        else{
            knob = knob.minVal(this.minVal).maxVal(this.maxVal);
        }
        if(this.minAngle){
            knob = knob.minAngle(this.minAngle);
        }
        if(this.maxAngle){
            knob = knob.maxAngle(this.maxAngle);
        }
        knob.create(r,this.offsetX,this.offsetY);
        var knobLabel = new wso2vis.ctrls.Label() .text(this.label) .create(r, this.offsetX, this.offsetY + 2*this.dialRadius);
        var self = this;
        knob.onChange = function(val){
            self.knobVal = val;
            if(self.selectOpts){
                knobLabel.update(self.selectOpts[Math.round(val)]);
            }
            else{
                knobLabel.update(val != 0? Math.round(val) : "0");
            }
            self.fireEvent("knobchange", self.knobVal);
        }
        //setting colors of text and ticks
        $("#" + selector).find("path").each(function(){
            $(this).attr("stroke", self.color)
            });
        $("#" + selector).find("text").each(function(){
            $(this).attr("stroke", self.color)
            });
    }
})