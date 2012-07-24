/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
package petascope.util;

/**
 *
 * @author <a href="mailto:a.rezaeim@jacobs-university.de">Alireza Rezaei Mahdiraji</a>
 */
public interface WCPSConstants {
    
    public static final String A                            = "a";
    public static final String ABS                          = "abs";
    public static final String ADD_SUBSET                   = "add subset";
    public static final String AXIS                         = "axis";
    public static final String AXIS_ITERATOR                = "axisIterator";
    public static final String AXIS_TYPE                    = "axisType";
    public static final String AND                          = "and";
    public static final String B                            = "b";
    public static final String BINARY_OP                    = "binaryOp";
    public static final String BIT                          = "bit";
    public static final String BOOLEAN_AND                  = "booleanAnd";
    public static final String BOOLEAN_OR                   = "booleanOr";
    public static final String BOOLEAN_XOR                  = "booleanXor";
    public static final String BOOLEAN_CONSTANT             = "booleanConstant";
    public static final String BOOLEAN_NOT                  = "booleanNot";
    public static final String BOOLEAN_EQUALNUMERIC         = "booleanEqualNumeric";
    public static final String BOOLEAN_EQUAL_STRING         = "booleanEqualString";
    public static final String BOOLEAN_NOT_EQUAL_STRING     = "booleanNotEqualString";
    public static final String BOOLEAN_NOTEQUALNUMERIC      = "booleanNotEqualNumeric";
    public static final String BOOLEAN_LESSTHAN             = "booleanLessThan";
    public static final String BOOLEAN_GREATERTHAN          = "booleanGreaterThan";
    public static final String BOOLEAN_LESSOREQUAL          = "booleanLessOrEqual";
    public static final String BOOLEAN_GREATEROREQUAL       = "booleanGreaterOrEqual";
    public static final String BOOLEAN_SCALAR_EXPR          = "Boolean Scalar Expr SUCCESS";

    
    public static final String BOUNDING_BOX                 = "Bounding Box";
    public static final String CNNOT_BE_LARGER              = "cannot be larger than upper bound";
    public static final String CAST                         = "cast";
    public static final String CELL_DOMAIN                  = "cell domain";
    public static final String CELL_DOMAIN_ELEMENT          = "CellDomainElement";
    public static final String CHILD                        = "child";
    public static final String COMPLEX_CONSTANT             = "complexConstant";
    public static final String COMPONENT                    = "component";
    public static final String CONST                        = "const";
    public static final String CONSTANT                     = "constant";
    public static final String CONDENSE                     = "condense";
    public static final String CONSTRUCT                    = "construct";
    public static final String COORD                        = "coord";
    public static final String COULD_NOT_FIND_AXIS          = "Could not find an axis node";
    public static final String COVERAGE                     = "coverage";
    public static final String COVERAGE_NAME                = "coverageName";
    public static final String COVERAGE_ITERATOR            = "coverageIterator";
    public static final String CREATE                       = "create";
    public static final String CREATING_INTERVALEXPR        = "Creating IntervalExpr of two indexes";
    public static final String CRS                          = "crs";
    public static final String CRS_C                        = "CRS";
    public static final String CRS_METADATA                 = "crs metadata";
    public static final String CRS_SET                      = "crsSet";
    public static final String CRS_TRANSFORM                = "crsTransform";
    public static final String DIV                          = "/";
    public static final String DIV_S                        = "div";
    public static final String DOMAIN                       = "domain";
    public static final String DOMAIN_METADATA              = "domain metadata";
    public static final String DOMAIN_METADATA_CAMEL        = "DomainMetadata";

    public static final String ENCODE                       = "encode";
    public static final String EQUAL                        = "=";
    public static final String EQUALS                       = "equals";
    public static final String EXTEND                       = "extend";

    public static final String EXTRA_PARAMETERS             = "extraParameters";
    public static final String FALSE                        = "false";
    public static final String FIELD                        = "field";
    public static final String FIELD_SELECT                 = "fieldSelect";
    public static final String FORMAT                       = "format";
    public static final String IDENTIFIER                   = "identifier";
    public static final String IMAGE_CRS                    = "imageCRS";
    public static final String IMAGE_CRSDOMAIN              = "imageCrsDomain";
    public static final String INDEX_EXPR                   = "IndexExpr";
    
    public static final String INTERNAL_ERROR_THIS          = "Internal error: this should have been a variable name";
    public static final String INTERPOLATION_DEFAULT        = "interpolationDefault";
    public static final String INTERPOLATION_METHOD         = "interpolationMethod";
    public static final String INTERPOLATION_SET            = "interpolationSet";
    public static final String INVALID_BOUNDING_BOX         = "Invalid bounding box: null element encountered.";
    public static final String INVALID_CELL_DOMAIN          = "Invalid cell domain element: Bounds may not be null";
    public static final String INVALID_CELL_DOMAIN_LOWER    = "Invalid cell domain element: Lower bound";
    public static final String INVALID_METADATA             = "Invalid metadata while cloning CellDomainElement. This is a software bug in WCPS.";


    public static final String INVALID_OFFSET               = "Invalid offsets: null element encountered.";
    public static final String INVALID_CRS                  = "Invalid CRS name: null element encountered";
    
    public static final String ITERATOR                     = "iterator";
    public static final String LOWER_BOUND                  = "lowerBound";
   
    public static final String NOT_EQUALS                   = "notEqual";
    public static final String NAME                         = "name";
    public static final String NULL                         = "null";
    public static final String NULL_RESISTANCE              = "nullResistance";
    public static final String NULL_SET                     = "nullSet";
    public static final String NUM                          = "num";
    
    public static final String NUMERIC_ABS                  = "numericAbs";
    public static final String NUMERIC_ADD                  = "numericAdd";
    public static final String NUMERIC_CONSTANT             = "numericConstant";
    public static final String NUMERIC_DIV                  = "numericDiv";
    public static final String NUMERIC_MINUS                = "numericMinus";
    public static final String NUMERIC_MULT                 = "numericMult";
    public static final String NUMERIC_SCALAR_EXPR          = "NumericScalarExpr";
    public static final String NUMERIC_SCALAR_EXPR_CONDENSE = "NumericScalarExpr condense";
    public static final String NUMERIC_SQRT                 = "numericSqrt";
    public static final String NUMERIC_UNARY_MINUS          = "numericUnaryMinus";

    public static final String LESS_THAN                    = "lessThan";
    public static final String LESS_OR_EQUAL                = "lessOrEqual";
    public static final String GREATER_THAN                 = "greaterThan";
    public static final String GREATER_OR_EQUAL             = "greaterOrEqual";
    public static final String IM                           = "im";
    public static final String IN                           = "in";
    public static final String IS_NOT_RECOGNIZED            = "is not recognized!";


    public static final String ITERATORVAR                  = "iteratorVar";
    public static final String MAX                          = "max";
    public static final String MIN                          = "min";
    public static final String MINUS                        = "-";
    public static final String MINUS_S                      = "minus";
    public static final String MULT                         = "mult";

    
    public static final String NOT                          = "not";
    
    public static final String OFFSETS                      = "Offsets";
    public static final String OR                           = "or";
    public static final String OP1                          = "op1";
    public static final String OP2                          = "op2";
    public static final String OPERATOR                     = "Operator";
    public static final String OPERATION                    = "operation";
    public static final String OP_PLUS                      = "opPlus";
    public static final String OP_MULT                      = "opMult";
    public static final String OP_MAX                       = "opMax";
    public static final String OP_MIN                       = "opMin";
    public static final String OP_AND                       = "opAnd";
    public static final String OP_OR                        = "opOr";
    public static final String OVERLAY                      = "overlay";
    public static final String PARAM                        = "param";
    public static final String PLUS_S                       = "plus";
    public static final String QUERY                        = "query";
    public static final String XOR                          = "xor";
    public static final String PLUS                         = "+";
    public static final String PLUS_I                       = "+i";
    public static final String PROCESS_COVERAGE_REQUEST     = "ProcessCoveragesRequest";

    
    public static final String RANGE_CONSTRUCTOR            = "rangeConstructor";
    public static final String REDUCE                       = "reduce";
    public static final String REFERENCE_TO                 = "reference to";

    
    public static final String RE                           = "re";
    public static final String SCALE                        = "scale";
    
    public static final String SET_IDENTIFIER               = "setIdentifier";
    public static final String SET_CRSSET                   = "setCrsSet";
    public static final String SLICE                        = "slice";
    public static final String SLICING_POSITION             = "slicingPosition";
    public static final String SQRT                         = "sqrt";

    
    public static final String STAR                         = "*";
    public static final String STORE                        = "store";
    public static final String STING_CONSTANT               = "stringConstant";
    public static final String STING_IDENTIFIER             = "stringIdentifier";
    public static final String SRS_NAME                     = "srsName";
    public static final String SCALARS                      = "scalars";
    public static final String TEXT                         = "text";
    public static final String TRANSLATION                  = "translation";
    public static final String TRIM                         = "trim";
    public static final String TYPE                         = "type";
    public static final String TRUE                         = "true";
    public static final String TWO_INDEXES                  = "two indexes";
    public static final String UNEXPECTED_BINARY            = "Unexpected binary operation";
    public static final String UNEXPECTED_BINARY_EXPR_NODE  = "Unexpected Binary Expression node";
    public static final String UNEXPECTED_NULL_NODE         = "Unexpected null node";
    public static final String UNIARY_OP                    = "unaryOp";
    public static final String UNIARY_OPERATOR              = "Unary Operator";
    
    public static final String UNIARY_PLUS                  = "unaryPlus";
    public static final String UNIARY_MINUS                 = "unaryMinus";
    public static final String UNKNOWN_METADATAEXPR         = "Unknown MetadataExpr operation";
    public static final String UNKNOWN_NODE                 = "Unknown node in AxisIterator";
    public static final String UPPER_BOUND                  = "upperBound";
  
    public static final String VALUE                        = "value";
    public static final String VAR                          = "var";
    public static final String VARIABLE_REF                 = "variableRef";
    public static final String WHERE                        = "where";
    public static final String X                            = "X";
    public static final String XML_HEADER                   = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
    public static final String XMLNS                        = "xmlns=\"http://www.opengis.net/wcps/1.0\" service=\"WCPS\" version=\"1.0.0\"";
    public static final String XML_SYNTAX                   = "xmlSyntax";
    public static final String Y                            = "Y";







    
}
