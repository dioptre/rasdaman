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
/*************************************************************
 *
 * 
 * COMMENTS:	Automaticaly generated
 *
 ************************************************************/


const QtNode::QtNodeType QtAbs::nodeType = QtNode::QT_ABS;

QtAbs::QtAbs(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtAbs::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {
			returnValue = computeOp( operand, Ops::OP_ABS );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtAbs::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtAbsObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}

void QtAbs::printAlgebraicExpression(ostream& s) {
	s << "abs(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtAbs::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtAbs", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ABS, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtAbs::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ABS, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtAbs::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtAbs::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtAbs::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtSqrt::nodeType = QtNode::QT_SQRT;

QtSqrt::QtSqrt(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtSqrt::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {
			returnValue = computeOp( operand, Ops::OP_SQRT );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtSqrt::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtSqrtObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtSqrt::printAlgebraicExpression(ostream& s) {
	s << "sqrt(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtSqrt::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtSqrt", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_SQRT, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtSqrt::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_SQRT, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtSqrt::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtSqrt::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtSqrt::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtExp::nodeType = QtNode::QT_EXP;

QtExp::QtExp(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtExp::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {
			returnValue = computeOp( operand, Ops::OP_EXP );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtExp::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtExpObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtExp::printAlgebraicExpression(ostream& s) {
	s << "exp(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtExp::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtExp", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_EXP, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtExp::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_EXP, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtExp::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtExp::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtExp::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtLog::nodeType = QtNode::QT_LOG;

QtLog::QtLog(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtLog::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_LOG );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtLog::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtLogObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtLog::printAlgebraicExpression(ostream& s) {
	s << "log(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtLog::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtLog", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_LOG, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtLog::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_LOG, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtLog::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtLog::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtLog::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtLn::nodeType = QtNode::QT_LN;

QtLn::QtLn(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtLn::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {			
			returnValue = computeOp( operand, Ops::OP_LN );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtLn::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtLnObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtLn::printAlgebraicExpression(ostream& s) {
	s << "ln(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtLn::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtLn", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_LN, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtLn::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_LN, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtLn::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtLn::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtLn::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtSin::nodeType = QtNode::QT_SIN;

QtSin::QtSin(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtSin::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {
			returnValue = computeOp( operand, Ops::OP_SIN );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtSin::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtSinObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtSin::printAlgebraicExpression(ostream& s) {
	s << "sin(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtSin::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtSin", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_SIN, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtSin::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_SIN, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtSin::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtSin::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtSin::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtCos::nodeType = QtNode::QT_COS;

QtCos::QtCos(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtCos::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_COS );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtCos::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtCosObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtCos::printAlgebraicExpression(ostream& s) {
	s << "cos(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtCos::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtCos", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_COS, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtCos::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_COS, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtCos::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtCos::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtCos::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtTan::nodeType = QtNode::QT_TAN;

QtTan::QtTan(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtTan::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_TAN );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtTan::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtTanObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtTan::printAlgebraicExpression(ostream& s) {
	s << "tan(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtTan::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtTan", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_TAN, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtTan::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_TAN, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtTan::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtTan::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtTan::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtSinh::nodeType = QtNode::QT_SINH;

QtSinh::QtSinh(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtSinh::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_SINH );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtSinh::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtSinhObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtSinh::printAlgebraicExpression(ostream& s) {
	s << "sinh(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtSinh::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtSinh", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_SINH, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtSinh::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_SINH, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtSinh::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtSinh::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtSinh::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtCosh::nodeType = QtNode::QT_COSH;

QtCosh::QtCosh(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtCosh::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_COSH );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtCosh::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtCoshObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtCosh::printAlgebraicExpression(ostream& s) {
	s << "cosh(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtCosh::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtCosh", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_COSH, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtCosh::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_COSH, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtCosh::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtCosh::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtCosh::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtTanh::nodeType = QtNode::QT_TANH;

QtTanh::QtTanh(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtTanh::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_TANH );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtTanh::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtTanhObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtTanh::printAlgebraicExpression(ostream& s) {
	s << "tanh(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtTanh::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtTanh", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_TANH, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtTanh::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_TANH, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtTanh::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtTanh::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtTanh::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

const QtNode::QtNodeType QtArcsin::nodeType = QtNode::QT_ARCSIN;

QtArcsin::QtArcsin(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtArcsin::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_ARCSIN );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtArcsin::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtArcsinObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtArcsin::printAlgebraicExpression(ostream& s) {
	s << "arcsin(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtArcsin::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtArcsin", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ARCSIN, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtArcsin::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ARCSIN, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtArcsin::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtArcsin::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        } 
        else
		RMInit::logOut << "Error: QtArcsin::checkType() - operand branch invalid." << endl;
	return dataStreamType;
}

const QtNode::QtNodeType QtArccos::nodeType = QtNode::QT_ARCCOS;

QtArccos::QtArccos(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtArccos::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_ARCCOS );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtArccos::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtArccosObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}
void QtArccos::printAlgebraicExpression(ostream& s) {
	s << "arccos(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtArccos::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtArccos", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ARCCOS, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtArccos::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ARCCOS, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtArccos::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtArccos::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else
		RMInit::logOut << "Error: QtArccos::checkType() - operand branch invalid." << endl;
	return dataStreamType;
}

const QtNode::QtNodeType QtArctan::nodeType = QtNode::QT_ARCTAN;

QtArctan::QtArctan(QtOperation* initInput): QtUnaryInduce(initInput) {}
QtData* QtArctan::evaluate(QtDataList* inputList) {
	QtData* returnValue = NULL;
	QtData* operand = NULL;

	if(getOperand(inputList, operand)) {
		try {		
			returnValue = computeOp( operand, Ops::OP_ARCTAN );
		}
		catch(...) {
			operand->deleteRef();
			throw;
		}
	}
	// delete old operand
	if(operand) operand->deleteRef();
	return returnValue;
}

void QtArctan::printTree(int tab, ostream& s, QtChildType mode) {
	s << SPACE_STR(tab).c_str() << "QtArctanObject " << getNodeType() << endl;
	QtUnaryInduce::printTree( tab + 2, s, mode );
}

void QtArctan::printAlgebraicExpression(ostream& s) {
	s << "arctan(";
	if(input)
		input->printAlgebraicExpression(s);
	else
	s << "<nn>";
	s << ")";
}

const QtTypeElement& QtArctan::checkType(QtTypeTuple* typeTuple) {
	RMDBCLASS( "QtArctan", "checkType( QtTypeTuple* )", "qlparser", __FILE__, __LINE__ )
	dataStreamType.setDataType( QT_TYPE_UNKNOWN );
	// check operand branches
	if(input) 
        {
	// get input types
	const QtTypeElement& inputType = input->checkType( typeTuple );
	RMDBGIF( 4, RMDebug::module_qlparser, "AutoGen", \
		RMInit::dbgOut << "Operand: " << flush; \
		inputType.printStatus( RMInit::dbgOut ); \
		RMInit::dbgOut << endl; \
	)
	if(inputType.getDataType() == QT_MDD) {
	const BaseType* baseType = ((MDDBaseType*)(inputType.getType()))->getBaseType();
	BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ARCTAN, baseType ));
	if(!resultBaseType) {
		RMInit::logOut << "Error: QtArctan::checkType() - induce operand type is not support" << endl;
		parseInfo.setErrorNo(366);
		throw parseInfo;
	}
	MDDBaseType* resultMDDType = new MDDBaseType( "tmp", resultBaseType );
	TypeFactory::addTempType( resultMDDType );
	dataStreamType.setType( resultMDDType );
	}
	else if(inputType.isBaseType()) {
		BaseType* baseType = (BaseType*)(inputType.getType());
		BaseType* resultBaseType = (BaseType*)(Ops::getResultType( Ops::OP_ARCTAN, baseType ));
		if(!resultBaseType) {
			RMInit::logOut << "Error: QtArctan::checkType() - operand type is not supported." << endl;
			parseInfo.setErrorNo(367);
			throw parseInfo;
	}
		dataStreamType.setType( resultBaseType );
	}
	else {
		RMInit::logOut << "Error: QtArctan::checkType() - operation is not supported for strings." << endl;
		parseInfo.setErrorNo(385);
		throw parseInfo;
	}
        }
        else 
		RMInit::logOut << "Error: QtArctan::checkType() - operand branch invalid." << endl;

	return dataStreamType;
}

