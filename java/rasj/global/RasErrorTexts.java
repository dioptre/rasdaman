/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
*/
/** ***********************************************************
 * <pre>
 *
 * PURPOSE:
 * first part of three constituting RasErrorTexts.java:
 * - part 1: Java src
 * - part 2: generated from errtxts file
 * - part 3: Java src
 *
 *
 * COMMENTS:
 *
 * </pre>
 *********************************************************** */

package rasj.global;

import java.io.*;

// This file is created automatically - do not edit unless allowed explicitly (look for
// a message "changes can only be made after here")!

public abstract class RasErrorTexts
{

    public static String[] errorMessages = {

// end of RasErrorTexts.template1
"66:Exception: Memory allocation failed.",
"100:Exception: Internal error: DL parse error.",
"200:Exception: The result is no point.",
"201:Exception: The result is no interval.",
"202:Exception: Index violation ( index range [$low,$high], index $index ).",
"203:Exception: Dimension mismatch between $dim1 and $dim2.",
"204:Exception: Stream initialization overflow.",
"205:Exception: Result is no cell.",
"206:Serialisable exception r_Ebase_dbms: error in base DBMS.",
"207:Internal client exception in class $class, method $method: $code.",
"208:Exception: Access type $aType does not fit base type $bType.",
"209:Exception: RasType $type is unknown.",
"210:Exception: Base type $type is not supported yet.",
"211:Exception: Database is not open.",
"212:Exception: RPC layer connection to RasDaMan failed.",
"213:Exception: Wrong URL format (should be http://address:port)",
"214:Exception: Illegal java long value $val for server base type ULong.",
"215:Exception: Illegal java integer value $val for server base type UShort.",
"216:Exception: System collection is not writable.",
"217:Exception: System collection has no OID.",
"218:Exception: Conversion format is not supported.",
"219:Exception: The specified tile size is smaller than the length of the base type of the mdd object.",
"220:Exception: The tiling strategy in the storage layout is not compatible with the marray.",
"221:Exception: The domain passed as an argument was not initialised correctly (dimension is 0).",
"222:Exception: The type name or type structure does not represent a marray type.",
"223:Exception: The rc index requires a marray type that has a specified domain (with fixed borders in all dimensions).",
"224:Exception: The tile configuration is incompatible to the marray domain.",
"229:Exception: The parameterized query has invalid parameter format.",
"230:Exception: The r_Object was already assigned a type.",
"231:Exception: The Marray has no base type.",
"232:Exception: The interval has at least one open bound.",
"233:Exception: The intervals don't have the same dimension.",
"234:Exception: The string passed to the tiling object was not correct.",
"235:Exception: Connection to server already closed.",
"236:Exception: Error in compression engine",
"237:Exception: Client communication failure",
"238:Exception: Base type not supported by conversion/compression module.",
"239:Exception: Standard overlay using types larger than 16 bytes is not supported.",
"240:Exception: Insert into a RC index is not allowed.",
"241:Exception: No tiling defined at that region.  Update not possible.",
"300:Parsing error $errorNo in line $lineNo, column $columnNo: Unexpected name $token.",
"301:Parsing error $errorNo in line $lineNo, column $columnNo, token $token: All cell values of an MDD must be of the same type. ",
"302:Parsing error $errorNo in line $lineNo, column $columnNo, token $token: Number of cells specified does not match the number of cells of the given spatial domain.",
"303:Parsing error $errorNo in line $lineNo, column $columnNo, token $token: OId is not valid.",
"308:Parsing error: Unexpected end of query.",
"309:Parsing error: Unknown error.",
"310:Lexical analysing error $errorNo in line $lineNo, column $columnNo: Unexpected characters $token.",
"311:Parsing error $errorNo in line $lineNo, column $columnNo, token $token: Complex constructor must have both arguments of the same type (i.e. float or double). ",
"312:Parsing error $errorNo in line $lineNo, column $columnNo, token $token: Variable already defined. ",
"313:Parsing error $errorNo in line $lineNo, column $columnNo, token $token: Only constant interval bounds allowed. ",
"330:Preprocessing error $errorNo in line $lineNo, column $columnNo: Unexpected name $token: ",
"331:Preprocessing error $errorNo in line $lineNo, column $columnNo, token $token: attempt to redefine function.",
"332:Preprocessing error $errorNo in line $lineNo, column $columnNo, token $token: number of actual arguments for the called function differs from the number of formal arguments.",
"333:Preprocessing error $errorNo in line $lineNo, column $columnNo, token $token: the called function name is ambiguous, try the full qualified name.",
"349:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand out of range.",
"350:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: General. ",
"351:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Spatial domains of the binary induce operands are incompatible.",
"352:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand types are incompatible.",
"353:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of quantifier must be multidimensional.",
"354:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of quantifier must be of type r_Marray<d_Boolean>.",
"355:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Collection name is unknown.",
"356:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Specified domain does not intersect with spatial domain of MDD.",
"357:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Variable is unknown.",
"358:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Projection operand is not of type r_Marray<T>.",
"359:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Result of the where clause must be of type boolean.",
"360:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Type of operand is not supported.",
"361:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Multiple query targets are not supported.",
"362:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Specified domain dimensionality does not equal defined dimensionality of MDD.",
"363:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Cell base types of binary induce operation are incompatible.",
"364:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Cell base type and scalar type of binary induce operation are incompatible.",
"365:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Scalar types of binary operation are incompatible.",
"366:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Cell base type of unary induce operation is not supported.",
"367:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Scalar type of unary operation is not supported.",
"368:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Cell base type for induced dot operation must be complex.",
"369:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Scalar type for dot operation must be complex.",
"370:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Struct selector is not valid.",
"371:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Retrieval query must start with a SELECT statement.",
"372:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Update query must start with an INSERT, UPDATE, DELETE, DROP or CREATE statement.",
"373:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Unsatisfied MDD constant parameter.",
"380:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Data type can not be converted to selected data exchange format.",
"381:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Error in convertor of the selected data exchange format.",
"382:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Unknown conversion format.",
"383:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Parameter of oid function must be a persistent object of type MDD.",
"384:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: OId is not valid.",
"385:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operation is not supported on strings.",
"386:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Base name of oid is not matching the currently opened one.",
"387:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: System name of oid is not matching the currently used one.",
"388:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Interval bound must be either an integer expression or an asterisk.",
"389:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: No interval (in case of fixed bounds, the upper one can not be smaller than the lower one).",
"390:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Minterval dimension specifications must be either of type interval or integer.",
"391:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Spatial operation must be either of type minterval, point, or integer.",
"393:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of operation lo/hi must be of type interval.",
"394:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operation lo/hi can not be used for an open bound.",
"395:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of function sdom() must be of type MDD.",
"396:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Selection operation is not supported on this data type.",
"397:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of minterval selection must be of type integer.",
"398:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Index for minterval selection is out of range.",
"399:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of point selection must be of type integer.",
"400:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Domain of MDD constructor has to be defined.",
"401:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Can not evaluate domain expression to an minterval.",
"402:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Projected cell is not defined.",
"403:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Binary operation is not supported on these data types.",
"404:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Type of cell expression is not supported.",
"405:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: First operand of shift function must be of type MDD.",
"406:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Second operand of shift function must be of type Point.",
"407:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Dimensionality of MDD and point expression are not matching.",
"408:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Second operand of shift function must be a constant expression.",
"409:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Spatial domain shift of open bounds is not supported.",
"410:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of point expression must be of type integer.",
"411:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Index for point selection is out of range.",
"412:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Value expression must be either of type atomic or complex.",
"413:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Condition expression must be of type boolean.",
"415:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Operand of count_cells must be of type r_Marray<d_Boolean>.",
"416:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: First operand of scale function must be of type MDD.",
"417:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Second operand of scale function must be either of type Point, Integer or Float.",
"418:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Second operand of bit function must be of integral type.",
"419:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Could not scale the domain.",
"420:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Extend operation with open bounds is not supported.",
"421:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Target interval of extend operation does not cover MDD to be extended.",
"422:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Second operand of extend function must be an minterval.",
"499:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: Language feature is not supported.",
"510:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: The argument is outside the function domain.",
"511:Execution error $errorNo in line $lineNo, column $columnNo, near token $token: The function result exceeds the allowed range.",
"700:Admin error: General error creating RasDaMan database.",
"701:Admin error: Error creating table in tablespace RAS_DB_SCHEMA.",
"702:Admin error: Error inserting into table RAS_COUNTERS.",
"703:Admin error: Error creating table in tablespace RAS_DB_BLOB.",
"704:Admin error: Error creating index in tablespace RAS_DB_INDEX.",
"705:Admin error: Error inserting into table RAS_BASETYPENAMES.",
"706:Admin error: Error creating table in default tablespace.",
"707:Admin error: Error on COMMIT creating RasDaMan database.",
"708:Admin error: Database to be created already exists.",
"800:RasManager Error: Could not connect to RasServer $url.",
"801:RasManager Error: System overloaded, please try again later.",
"802:RasManager Error: Access denied, incorrect user/password.",
"803:RasManager Error: Access denied, no permission for operation.",
"804:RasManager Error: Access denied, capability refused.",
"805:RasManager Error: No suitable servers started, call administrator.",
"806:RasManager Error: Write transaction in progress, please retry again later.",
"807:RasManager Error: Requested database unknown.",
"808:RasManager Error: Request format error.",
"820:RNP Error: First parameter has to be the clientID (clientcomm internal).",
"821:RNP Error: Client ID invalid, probably a timeout occurred.",
"822:RNP Error: Unknown command in client request.",
"830:base DBMS Error: Cannot connect to base DBMS server (invalid connect string in rasmgr config file?).",
"900:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: Type in typedef definition not supported.",
"901:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: Set template type has to be a type reference.",
"902:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: Type reference not found.",
"903:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: MDD base type has to be a type reference or an atomic type.",
"904:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: MDD type must have a domain specification.",
"905:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: Struct type name exists already.",
"906:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: MDD type name exists already.",
"907:Evaluation error $errorNo in line $lineNo, column $columnNo, near token $token: Set type name exists already.",
"950:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update target must be an iterator variable.",
"951:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update source must be an expression resulting in an r_Marray<>.",
"952:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update base type does not match MDD base type.",
"953:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update domain is not within MDD definition domain.",
"954:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update target expression must be an assignable value (l-value).",
"955:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Collection name exists already.",
"956:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Unknown collection type.",
"957:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Unknown collection name.",
"958:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Allocation of new oid failed.",
"959:Update error $errorNo in line $lineNo, column $columnNo, near token $token: MDD and collection types are incompatible.",
"960:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Insert expression must be of type MDD.",
"961:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update domain must be of type Minterval.",
"962:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Number of update intervals must match source dimensionaltiy.",
"963:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Update domain dimensionality must match target MDD dimensionaltiy.",
"964:Update error $errorNo in line $lineNo, column $columnNo, near token $token: Type is not persistent.",
"965:Update error $errorNo: MDD type $token unknown.",
"966:Update error $errorNo: MDD type is missing.",
"1000:General error: RasDaMan tables inconsistent.",
"1001:General error: RasDaMan server incompatible with database.",
"1002:General error: Blob with zero length encountered.",
"1003:General error: Tile container for TC index not found.",
"1004:General error: Index of MDD Object is not defined.",
"1005:General error: Storage structure of MDD Object is not defined.",
"1006:General error: Unknown index type requested.",
"1007:General error: Illegal index type requested.",
"1008:General error: No valid collection type passed to MDD collection.",
"1009:General error: MDD object not valid or not persistent.",
"1010:General error: No valid MDD type passed to MDD object.",
"1011:General error: An illegal state has been reached.  This is caused by a compiler bug or a library bug.",
"1012:General error: Invalid collection type passed to MDD collection.",
"1013:General error: The name of the type is too long.",
"1014:General error: Invalid name of the object, should contain only [a-zA-Z0-9_]",
"2000:Internal error: There seems to be another database open.",
"2001:Internal error: Invalid OId type encountered.",
"2002:Internal error: Entry in user defined type not found.",
"2003:Internal error: Entry in user defined type out of bounds.",
"2004:Internal error: Transient index used instead of persistent index.",
"2005:Internal error: Index returned tiles multiple times.",
"2006:Internal error: Tile was not inserted into index.",
"2007:Internal error: Transient index access out of bounds.",
"2008:Internal error: MDD object exists multiple times in cache.",
"2009:Internal error: Some tile(s) were not inserted into the MDD object.",
"2010:Internal error: A conversion module returned an incorrect base type.",
"2011:Internal error: The collection type has no element type.",
"2012:Internal error: The marray type has no base type.",
"2013:Internal error: The property has no base type.",
"2014:Internal error: The scalar was passed a NULL value.",
"2015:Internal error: The index node that had to be split was not found in its parent.",
"2016:Internal error: The index found more cells than allowed.",
"2017:Internal error: The storage layout is incompatible with the index entries.",
"2018:Internal error: Object does not support swapping.",
"2019:Internal error: Error encountered during swapping.",
"2020:Internal error: Binary export for object is not supported.",
"2021:Internal error: Binary import for object is not supported.",
"2022:Internal error: Operands and result types don't match.",
"3000:Format conversion error: DEM area does not contain any non-null value, empty result generated.",
"10000:Unexpected internal server error.",
/** ***********************************************************
 * <pre>
 *
 * SOURCE:  RasErrorTexts.template2
 *
 * PACKAGE: rasj.global
 * CLASS:   RasErrorTexts
 *
 * PURPOSE:
 * third part of three constituting RasErrorTexts.java:
 * - part 1: Java src
 * - part 2: generated from errtxts file
 * - part 3: Java src
 *
 *
 * </pre>
 *********************************************************** */

// end of error code array, start of RasErrorTexts.template2

    ""	// this last, empty string is needed because the last one, generated from errtxts, has a "," at the end
    };

    public static String getErrorMessage( int errNo )
    {
	StringBuffer buf;
	String prefix = String.valueOf(errNo)+":";
	int index = 0;
	String retVal = null;

	while(index < errorMessages.length-1)	// last string is empty, see above
	    {
		if(errorMessages[index].startsWith(prefix))
		    {
			buf = new StringBuffer(errorMessages[index]);
     	                index = errorMessages[index].indexOf(":");
	                buf.delete(0,index+1);
			retVal = buf.toString();
			index = errorMessages.length+1;
		    }
                else
		    index++;

	    }
	if(retVal == null)
	    retVal = "No error message available for error number " + errNo + ".";

    return retVal;
    }

}


