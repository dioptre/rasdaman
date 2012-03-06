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
 * Defines a discrete slider widget. The values that the widget can take are
 * provided by the developer
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0
 */


Rj.namespace('Rj.Widget');

Rj.Widget.DiscreteSliderWidget = new JS.Class(Rj.Widget.SliderWidget, {

  initialize : function(min, max, orientation, sliderLevels, defaultSlideLevel){
    if(!(sliderLevels instanceof Array) || sliderLevels[0] === null || sliderLevels[0] === undefined){
      throw "Please use an array for the sliderLevels";
    }
    this.callSuper();
    this.min = min || sliderLevels[0];
    this.max = max || sliderLevels[sliderLevels.length - 1];
    this.orientation = orientation;
    this.sliderLevels = sliderLevels;
    this.slideLevel = defaultSlideLevel || sliderLevels[0];
    this.oldValue = this.slideLevel;
  },

  renderTo : function(selector){
    var id = Rj.getId(selector);
    this.id = id;
    var self = this;
    jQuery('#' + id).slider({
      value: this.slideLevel,
      oldValue : this.slideLevel,
      min: this.min,
      max: this.max,
      step: 1,
      orientation : this.orientation,
      change: function(event, ui){
        self.slideLevel = self.range ? ui.values : ui.value;
        self.fireEvent("changevalue", self.slideLevel);
      },
      slide: function(event, ui){
        var direction = ui.value > self.oldValue ? 'RIGHT' : 'LEFT';
        var closestValue = self.findClosestValue(self.oldValue, direction);
        var currentMouseValue = ui.value;
        if(Math.abs(currentMouseValue - self.oldValue) > Math.abs(closestValue - currentMouseValue)){
          self.oldValue = closestValue;
          $(this).slider('value', closestValue);
          self.fireEvent("slidevalue", closestValue);
        }
        return false
      }
    });
  },

  findClosestValue : function(value, direction){
    var self = this;
    var i, closest;
    if(direction == 'RIGHT'){
      for(i = 0;  i < self.sliderLevels.length; i++){
        if(value == self.sliderLevels[i]){
          if((i < self.sliderLevels.length - 1)){
            closest = self.sliderLevels[i+1];
          }
          else{
            closest = self.sliderLevels[i];
          }
        }
      }
    }
    else{
      for(i = 0; i < self.sliderLevels.length; i++){
        if(value == self.sliderLevels[i]){
          if(i > 0){
            closest = self.sliderLevels[i-1];
          }
          else{
            closest = self.sliderLevels[i];
          }
        }
      }
    }
    return closest;
  }

})
