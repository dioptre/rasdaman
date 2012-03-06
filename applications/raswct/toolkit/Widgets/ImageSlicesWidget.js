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
 * @class ImageSlicesWidget extends InputWidget
 *
 * InputWidget is a simple grouper class that helps better define the
 * relationships between widgets
 *
 * @author Alex Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0.0
 */

Rj.namespace("Rj.Widget");

Rj.Widget.ImageSlicesWidget = new JS.Class(Rj.Widget.BaseWidget, {
  
  /**
   * Delay in sending the queries in the initialization in miliseconds
   */
  INIT_TIMEOUT : 1,
  
  initialize : function(configSet){
    this.sliceConfig = [];
    this.slices = [];
    for(var i = 0; i < configSet.length; i++){
      configSet[i].slider.orientation = configSet[i].slider.orientation ? configSet[i].slider.orientation : Rj.Constants.SliderWidget.HORIZONTAL;
      configSet[i].slider.step = configSet[i].slider.step ? configSet[i].slider.step : 1;
      this.sliceConfig[i] =  {
        query : configSet[i].query,
        callback : configSet[i].callback,
        slider : configSet[i].slider,
        name : i,
        imageSelector : Rj.Util.generateRandomString(),
        sliderSelector : Rj.Util.generateRandomString(),
        loaderSelector : Rj.Util.generateRandomString(),
        sliceSelector : Rj.Util.generateRandomString()
      };
    }
  },

  createOneSlice : function(sliceConfig, name){
    this.slices[name] = {
      imageDiv : '<div id="' + sliceConfig.imageSelector + '"></div>',
      sliderDiv : '<div class="slider" id="' + sliceConfig.sliderSelector + '"></div>',
      loaderDiv : '<div style="display:none" class="loader" id="' + sliceConfig.loaderSelector + '"></div>',
      slider : new Rj.Widget.SliderWidget(sliceConfig.slider.min, sliceConfig.slider.max, sliceConfig.slider.orientation ? sliceConfig.slider.orientation : Rj.Constants.SliderWidget.HORIZONTAL, sliceConfig.slider.step ? sliceConfig.slider.step : 1, sliceConfig.slider.min, false),
      sliceConfig : sliceConfig,
      query : sliceConfig.query,
      callback : sliceConfig.callback
    };
    this.addOneSliderListener(this.slices[name].slider, this.slices[name]);

  },

  createSlices : function(){
    for(var i = 0; i < this.sliceConfig.length; i++){
      this.createOneSlice(this.sliceConfig[i], this.sliceConfig[i].name);
    }
  },

  renderOneSlice : function(container, slice){
    var sliceDiv = jQuery("<div/>", {
      id : slice.sliceConfig.sliceSelector
    });
    container.append(sliceDiv);
    sliceDiv.append(slice.imageDiv, slice.sliderDiv, slice.loaderDiv);
    slice.slider.renderTo("#" + slice.sliceConfig.sliderSelector);

  },

  renderSlices : function(container){
    this.createSlices();
    for(var i = 0; i < this.slices.length; i++){
      this.renderOneSlice(container, this.slices[i]);
    }
  },

  addOneSliderListener : function(slider, slice){
    var self = this;
    slider.addListener("3dApp", "slidevalue", function(value){
      slice.query = slice.callback.call(self, value, slice.query);
      self.updateSliceImage(slice);
    });
  },

  enableLoader : function(slice){
    jQuery(slice.sliceConfig.loaderSelector).show();
  },

  disableLoader : function(slice){
    jQuery(slice.sliceConfig.loaderSelector).hide();
  },

  updateSliceImage : function(slice){
    var self = this;
    this.enableLoader(slice);
    var executor = new Rj.Executor.QueryExecutor(slice.query);
    executor.callback(function(response){
      var data = JSON.parse(response);
      var imageId = data.url.split(".png")[0];

      var newImage = $("<img/>");
      newImage.attr({
        id : imageId,
        src : data.url
      });      
      
      newImage.load(function(){
        jQuery('#' + slice.sliceConfig.imageSelector).html(newImage);
        self.disableLoader(slice);
      })
    })
  },
  
  addDefaultCss : function() {
    var defaultCss = {
      'margin-bottom' : '50px'
    };
    for(i = 0; i < this.slices.length; i++){
      jQuery('#' + this.slices[i].sliceConfig.sliceSelector).css(defaultCss);
      console.log('#' + this.slices[i].sliceConfig.sliceSelector);
    }
  },

  renderTo : function(selector){
    var self = this;
    var mainDiv = jQuery(selector);
    this.renderSlices(mainDiv);
    for(var i = 0; i < this.slices.length; i++){
      var slice = this.slices[i];
      slice.query = slice.callback.call(this, slice.sliceConfig.slider.min, slice.query);
      (function(j){
        setTimeout(function(){
          self.updateSliceImage(self.slices[j]);
        }, Rj.Widget.ImageSlicesWidget.INIT_TIMEOUT * j);
      })(i);
    }
    this.addDefaultCss();
  }
});