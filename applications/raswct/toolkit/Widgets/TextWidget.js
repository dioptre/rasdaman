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
 * @class TextWidget extends InputWidget
 *
 * Defines a widget which allows the user to input text queries.
 * 
 * @author Mircea Dumitru <m.dumitru@jacobs-university.de>
 * @author Vlad Merticariu <v.merticariu@jacobs-university.de>
 * @package raswct
 * @version 1.0
 */

Rj.namespace('Rj.Widget');

Rj.Widget.TextWidget = new JS.Class(Rj.Widget.InputWidget, {
    
  /**
     * Standard class constructor
     * @param <int> rows - the number of rows the textarea widget has
     * @param <int> cols - the number of cols the textarea widget has
     * @param <string> submitValue - the text displayed on the submit button
     * @param <string> value - the default text to be displayed in the textarea
     */
  initialize: function(rows, cols, submitValue, value){
    this.rows = parseInt(rows) || 5;
    this.cols = parseInt(cols) || 15;
    this.submitValue = submitValue || 'Submit';
    this.textAreaId = '';
    this.callSuper(value);
  },
    
  /**
     * @override Rj.BaseWidget.renderTo
     */
  renderTo: function(selector){
    var id = Rj.getId(selector);
    this.textAreaId = 'textarea-'+id;
    var htmlVal = '<textarea id="textarea-' + id + '" rows = "' + this.rows + '" cols = "' + this.cols + '">';
    htmlVal += this.value;        
    htmlVal += '</textarea>';        
    htmlVal += '<input style="float:right;" type = "submit" value = "' + this.submitValue + '" id = "' + id + '-submit" />';
    var self = this;
    jQuery("#" + id + '-submit').live('click', function(){
      self.fireEvent("changevalue", jQuery("#" + self.textAreaId).val());
    })
    jQuery('#' + id).html(htmlVal);
  },
    
  /**
     * Changes the text in the widget
     * @param <string> value - the new text to be displayed in the widget
     */
  setValue: function(value){
    jQuery('#' + this.textAreaId).val(value);
    this.value = value;
  }
})
