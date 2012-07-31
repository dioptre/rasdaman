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
 *  All String constants used by the WCPS are defined as String constants.
 * 
 * 
 * @author <a href="mailto:a.rezaeim@jacobs-university.de">Alireza Rezaei Mahdiraji</a>
 */
public interface WCPSConstants {
    
    /**
     * General messages  
     */
    public static final String MSG_A                            = "a";
    public static final String MSG_ABS                          = "abs";
    public static final String MSG_ADD_SUBSET                   = "add subset";
    public static final String MSG_ADD_AXIS_ITERATOR            = "over: add axis iterator";  
    public static final String MSG_AXIS                         = "axis";
    public static final String MSG_AXIS_ITERATOR                = "axisIterator";
    public static final String MSG_AXIS_TYPE                    = "axisType";
    public static final String MSG_AND                          = "and";
    public static final String MSG_B                            = "b";
    public static final String MSG_BINARY_OP                    = "binaryOp";
    public static final String MSG_BIT                          = "bit";
    public static final String MSG_BOOLEAN_AND                  = "booleanAnd";
    public static final String MSG_BOOLEAN_OR                   = "booleanOr";
    public static final String MSG_BOOLEAN_XOR                  = "booleanXor";
    public static final String MSG_BOOLEAN_CONSTANT             = "booleanConstant";
    public static final String MSG_BOOLEAN_NOT                  = "booleanNot";
    public static final String MSG_BOOLEAN_EQUALNUMERIC         = "booleanEqualNumeric";
    public static final String MSG_BOOLEAN_EQUAL_STRING         = "booleanEqualString";
    public static final String MSG_BOOLEAN_NOT_EQUAL_STRING     = "booleanNotEqualString";
    public static final String MSG_BOOLEAN_NOTEQUALNUMERIC      = "booleanNotEqualNumeric";
    public static final String MSG_BOOLEAN_LESSTHAN             = "booleanLessThan";
    public static final String MSG_BOOLEAN_GREATERTHAN          = "booleanGreaterThan";
    public static final String MSG_BOOLEAN_LESSOREQUAL          = "booleanLessOrEqual";
    public static final String MSG_BOOLEAN_GREATEROREQUAL       = "booleanGreaterOrEqual";
    public static final String MSG_BOOLEAN_SCALAR_EXPR          = "Boolean Scalar Expr SUCCESS";
    public static final String MSG_BOUNDING_BOX                 = "Bounding Box";
    public static final String MSG_BUILDING_METADATA            = "building metadata...";
    public static final String MSG_CAST                         = "cast";
    public static final String MSG_CELL_DOMAIN                  = "cell domain";
    public static final String MSG_CELL_DOMAIN_ELEMENT          = "CellDomainElement";
    public static final String MSG_CHILD                        = "child";
    public static final String MSG_COMPLEX                      = "complex";
    public static final String MSG_COMPLEX_CONSTANT             = "complexConstant";
    public static final String MSG_COMPONENT                    = "component";
    public static final String MSG_CONST                        = "const";
    public static final String MSG_CONSTANT                     = "constant";
    public static final String MSG_CONDENSE                     = "condense";
    public static final String MSG_CONSTRUCT                    = "construct";
    public static final String MSG_COORD                        = "coord";
    public static final String MSG_COVERAGE                     = "coverage";
    public static final String MSG_COVERAGE_NAME                = "coverageName";
    public static final String MSG_COVERAGE_ITERATOR            = "coverageIterator";
    public static final String MSG_CREATE                       = "create";
    public static final String MSG_CREATING_INTERVALEXPR        = "Creating IntervalExpr of two indexes";
    public static final String MSG_CRS                          = "crs";
    public static final String MSG_CRS_C                        = "CRS";
    public static final String MSG_CRS_METADATA                 = "crs metadata";
    public static final String MSG_CRS_SET                      = "crsSet";
    public static final String MSG_CRS_TRANSFORM                = "crsTransform";
    public static final String MSG_DIV                          = "/";
    public static final String MSG_DIV_S                        = "div";
    public static final String MSG_DOMAIN                       = "domain";
    public static final String MSG_DOMAIN_METADATA              = "domain metadata";
    public static final String MSG_DOMAIN_METADATA_CAMEL        = "DomainMetadata";
    public static final String MSG_DYNAMIC_TYPE                 = "dynamic_type";
    public static final String MSG_ENCODE                       = "encode";
    public static final String MSG_EQUAL                        = "=";
    public static final String MSG_EQUALS                       = "equals";
    public static final String MSG_EXTEND                       = "extend";
    public static final String MSG_EXTRA_PARAMETERS             = "extraParameters";
    public static final String MSG_FALSE                        = "false";
    public static final String MSG_FIELD                        = "field";
    public static final String MSG_FIELD_SELECT                 = "fieldSelect";
    public static final String MSG_FORMAT                       = "format";
    public static final String MSG_GRID_COVERAGE                = "GridCoverage";
    public static final String MSG_IDENTIFIER                   = "identifier";
    public static final String MSG_IMAGE_CRS                    = "imageCRS";
    public static final String MSG_IMAGE_CRSDOMAIN              = "imageCrsDomain";
    public static final String MSG_INDEX_EXPR                   = "IndexExpr";
    public static final String MSG_INTERPOLATION_DEFAULT        = "interpolationDefault";
    public static final String MSG_INTERPOLATION_METHOD         = "interpolationMethod";
    public static final String MSG_INTERPOLATION_SET            = "interpolationSet";
    public static final String MSG_ITERATOR                     = "iterator";
    public static final String MSG_LOWER_BOUND                  = "lowerBound";
    public static final String MSG_NOT_EQUALS                   = "notEqual";
    public static final String MSG_NAME                         = "name";
    public static final String MSG_NONE                         = "none";
    public static final String MSG_NULL                         = "null";
    public static final String MSG_NULL_RESISTANCE              = "nullResistance";
    public static final String MSG_NULL_SET                     = "nullSet";
    public static final String MSG_NUM                          = "num";
    public static final String MSG_NUMERIC_ABS                  = "numericAbs";
    public static final String MSG_NUMERIC_ADD                  = "numericAdd";
    public static final String MSG_NUMERIC_CONSTANT             = "numericConstant";
    public static final String MSG_NUMERIC_DIV                  = "numericDiv";
    public static final String MSG_NUMERIC_MINUS                = "numericMinus";
    public static final String MSG_NUMERIC_MULT                 = "numericMult";
    public static final String MSG_NUMERIC_SCALAR_EXPR          = "NumericScalarExpr";
    public static final String MSG_NUMERIC_SCALAR_EXPR_CONDENSE = "NumericScalarExpr condense";
    public static final String MSG_NUMERIC_SQRT                 = "numericSqrt";
    public static final String MSG_NUMERIC_UNARY_MINUS          = "numericUnaryMinus";
    public static final String MSG_LESS_THAN                    = "lessThan";
    public static final String MSG_LESS_OR_EQUAL                = "lessOrEqual";
    public static final String MSG_GREATER_THAN                 = "greaterThan";
    public static final String MSG_GREATER_OR_EQUAL             = "greaterOrEqual";
    public static final String MSG_IM                           = "im";
    public static final String MSG_IN                           = "in";
    public static final String MSG_ITERATORVAR                  = "iteratorVar";
    public static final String MSG_MAX                          = "max";
    public static final String MSG_MIN                          = "min";
    public static final String MSG_MINUS                        = "-";
    public static final String MSG_MINUS_S                      = "minus";
    public static final String MSG_MULT                         = "mult";
    public static final String MSG_NOT                          = "not";
    public static final String MSG_OFFSETS                      = "Offsets";
    public static final String MSG_OR                           = "or";
    public static final String MSG_OP1                          = "op1";
    public static final String MSG_OP2                          = "op2";
    public static final String MSG_OPERATOR                     = "Operator";
    public static final String MSG_OPERATION                    = "operation";
    public static final String MSG_OP_PLUS                      = "opPlus";
    public static final String MSG_OP_MULT                      = "opMult";
    public static final String MSG_OP_MAX                       = "opMax";
    public static final String MSG_OP_MIN                       = "opMin";
    public static final String MSG_OP_AND                       = "opAnd";
    public static final String MSG_OP_OR                        = "opOr";
    public static final String MSG_OVER                         = "over";
    public static final String MSG_OVERLAY                      = "overlay";
    public static final String MSG_PARAM                        = "param";
    public static final String MSG_PLUS_S                       = "plus";
    public static final String MSG_QUERY                        = "query";
    public static final String MSG_XOR                          = "xor";
    public static final String MSG_PLUS                         = "+";
    public static final String MSG_PLUS_I                       = "+i";
    public static final String MSG_PROCESS_COVERAGE_REQUEST     = "ProcessCoveragesRequest";
    public static final String MSG_RANGE_CONSTRUCTOR            = "rangeConstructor";
    public static final String MSG_REDUCE                       = "reduce";
    public static final String MSG_REFERENCE_TO                 = "reference to";
    public static final String MSG_RE                           = "re";
    public static final String MSG_SCALE                        = "scale";
    public static final String MSG_SET_IDENTIFIER               = "setIdentifier";
    public static final String MSG_SET_CRSSET                   = "setCrsSet";
    public static final String MSG_SET_INTERPOLATION_DEFAULT    = "setInterpolationDefault";
    public static final String MSG_SET_INTERPOLATION_SET        = "setInterpolationSet";
    public static final String MSG_SET_NULL_SET                 = "setNullSet";
    public static final String MSG_SLICE                        = "slice";
    public static final String MSG_SLICING_POSITION             = "slicingPosition";
    public static final String MSG_SQRT                         = "sqrt";
    public static final String MSG_STAR                         = "*";
    public static final String MSG_STORE                        = "store";
    public static final String MSG_STING_CONSTANT               = "stringConstant";
    public static final String MSG_STING_IDENTIFIER             = "stringIdentifier";
    public static final String MSG_SRS_NAME                     = "srsName";
    public static final String MSG_SCALARS                      = "scalars";
    public static final String MSG_TEXT                         = "text";
    public static final String MSG_TEMP                         = "temp";
    public static final String MSG_TRANSLATION                  = "translation";
    public static final String MSG_TRIM                         = "trim";
    public static final String MSG_TYPE                         = "type";
    public static final String MSG_TRUE                         = "true";
    public static final String MSG_TWO_INDEXES                  = "two indexes";
    public static final String MSG_UNARY_OP                     = "unaryOp";
    public static final String MSG_UNARY_OPERATOR               = "Unary Operator";
    public static final String MSG_UNARY_PLUS                   = "unaryPlus";
    public static final String MSG_UNARY_MINUS                  = "unaryMinus";
    public static final String MSG_UNSIGNED_INT                 = "unsigned int";
    public static final String MSG_UPPER_BOUND                  = "upperBound";
    public static final String MSG_USING                        = "using";
    public static final String MSG_VALUE                        = "value";
    public static final String MSG_VALUE_LISt                   = "value list";    
    public static final String MSG_VAR                          = "var";
    public static final String MSG_VARIABLE_REF                 = "variableRef";
    public static final String MSG_WHERE                        = "where";
    public static final String MSG_X                            = "X";
    public static final String MSG_XML_HEADER                   = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
    public static final String MSG_XMLNS                        = "xmlns=\"http://www.opengis.net/wcps/1.0\" service=\"WCPS\" version=\"1.0.0\"";
    public static final String MSG_XML_SYNTAX                   = "xmlSyntax";
    public static final String MSG_Y                            = "Y";
    
    /**
     * Error messages  
     */
    public static final String ERRTXT_CANNOT_BE_LARGER             = "cannot be larger than upper bound";
    public static final String ERRTXT_CANNOT_BUILD_COVERAGE        = "Cannot build coverage metadata";
    public static final String ERRTXT_COULD_BUILD_CONST_COVERAGE   = "Could not build constant coverage metadata !!!";
    public static final String ERRTXT_COULD_NOT_FIND_AXIS          = "Could not find an axis node";
    public static final String ERRTXT_COULD_NOT_PARSE_COMPLEX_CONST= "Could not parse Complex Constant";
    public static final String ERRTXT_COULD_NOT_PARSE_FLOAT        = "Could not parse float or integer";
    public static final String ERRTXT_COULD_NOT_PARSE_REAL_PART    = "number for real part of complex number";
    public static final String ERRTXT_COULD_NOT_PARSE_IMG_PART     = "number for imaginary part of complex number";
    public static final String ERRTXT_CONST_DIMS_DOESNOT_MATCH     = "The number of constants in the list do not "
            + "match the dimensions specified !";
    public static final String ERRTXT_INTERNAL_ERROR_THIS          = "Internal error: this should have been a variable name";
    public static final String ERRTXT_INVALID_BOUNDING_BOX         = "Invalid bounding box: null element encountered.";
    public static final String ERRTXT_INVALID_CELL_DOMAIN          = "Invalid cell domain element: Bounds may not be null";
    public static final String ERRTXT_INVALID_CELL_DOMAIN_LOWER    = "Invalid cell domain element: Lower bound";
    public static final String ERRTXT_INVALID_METADATA             = "Invalid metadata while cloning CellDomainElement. This is a software bug in WCPS.";
    public static final String ERRTXT_INVALID_OFFSET               = "Invalid offsets: null element encountered.";
    public static final String ERRTXT_INVALID_CRS                  = "Invalid CRS name: null element encountered";
    public static final String ERRTXT_IS_NOT_RECOGNIZED            = "is not recognized!";
    public static final String ERRTXT_UNEXPECTED_BINARY            = "Unexpected binary operation";
    public static final String ERRTXT_UNEXPECTED_BINARY_EXPR_NODE  = "Unexpected Binary Expression node";
    public static final String ERRTXT_UNEXPECTED_NULL_NODE         = "Unexpected null node";
    public static final String ERRTXT_UNKNOWN_CONDENSE_OP          = "Unknown condense operation";
    public static final String ERRTXT_UNKNOWN_METADATAEXPR         = "Unknown MetadataExpr operation";
    public static final String ERRTXT_UNKNOWN_NODE                 = "Unknown node in AxisIterator";
    public static final String ERRTXT_UNKNOWN_SET_METADATA_EXPR    = "Unknown SetMetadataExpr operation";
    public static final String ERRTXT_UNKNOWN_PROC_COMPLEX_CONST   = "Unknown node while processing complex constant";

     
}
