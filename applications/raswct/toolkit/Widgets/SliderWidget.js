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
 * @class SliderWidget extends InputWidget
 *
 * Defines a slider widget.
 * 
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0
 */

Rj.namespace('Rj.Widget', 'Rj.Constants');

Rj.Constants.SliderWidget = {
  VERTICAL : 'vertical',
  HORIZONTAL : 'horizontal'
}


Rj.Widget.SliderWidget = new JS.Class(Rj.Widget.InputWidget, {
    
  /**
   * Standard class constructor
   * @param <int> min - the minimum value of the slider
   * @param <int> max - the maximum value of the slider
   * @param <string> orientation - the axis on which the slider is displayed,
   * can be "horizontal" or "vertical"
   * @param <int> step - defines the smallest unit with which the slider can move
   * @param <int> slideLevel - the default position of de slider when loaded
   */
  initialize: function(min, max, orientation, step, slideLevel){
    this.min = min || 0;
    this.max = max || 1;
    this.orientation = orientation;
    this.step = step || 1;
    this.slideLevel = slideLevel || this.min;
    this.id = '';
    this.callSuper(this.slideLevel);
  },
  
  /**
   * Standard getter for the slideLevel attribute
   */
  getSlideLevel: function(){
    return this.slideLevel;
  },
  
  /**
   * Standard setter for the slideLevel attribute
   * @param <int> slideLevel - the new value to which the slider is set to
   */
  setSlideLevel: function(slideLevel){
    this.slideLevel = slideLevel;
    jQuery('#' + this.id).slider('value', slideLevel);
  },
  
  /**
   * @override Rj.Widget.BaseWidget.renderTo
   * @event changevalue - fires when the slider's value is changed by drag&stop
   * @event slidevalue - fires when the slider's value is changed by a simple drag
   */
  renderTo: function(selector){
    var id = Rj.getId(selector);
    this.id = id;
    var self = this;
    jQuery('#' + id).slider({
      value: this.slideLevel,
      min: this.min,
      max: this.max,
      step: this.step,
      orientation : this.orientation,
      change: function(event, ui){
        this.slideLevel = ui.value;
        self.fireEvent("changevalue", ui.value);
      },
      slide: function(event, ui){
        self.fireEvent("slidevalue", ui.value);
      }
    });
  }
  
})
