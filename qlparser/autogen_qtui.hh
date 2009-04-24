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


//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtAbs : public QtUnaryInduce {
public:
	QtAbs(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtSqrt : public QtUnaryInduce {
public:
	QtSqrt(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtExp : public QtUnaryInduce {
public:
	QtExp(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtLog : public QtUnaryInduce {
public:
	QtLog(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtLn : public QtUnaryInduce {
public:
	QtLn(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtSin : public QtUnaryInduce {
public:
	QtSin(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtCos : public QtUnaryInduce {
public:
	QtCos(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtTan : public QtUnaryInduce {
public:
	QtTan(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtSinh : public QtUnaryInduce {
public:
	QtSinh(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtCosh : public QtUnaryInduce {
public:
	QtCosh(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtTanh : public QtUnaryInduce {
public:
	QtTanh(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtArcsin : public QtUnaryInduce {
public:
	QtArcsin(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtArccos : public QtUnaryInduce {
public:
	QtArccos(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

//@ManMemo: Module: {\bf qlparser}

/*@Doc:

*/

class QtArctan : public QtUnaryInduce {
public:
	QtArctan(QtOperation*);
	QtData* evaluate(QtDataList*);
	virtual void printTree(int, std::ostream& = std::cout, QtChildType = QT_ALL_NODES );
	virtual void printAlgebraicExpression(std::ostream& = std::cout);
	inline virtual const QtNodeType getNodeType() const;
	virtual const QtTypeElement& checkType(QtTypeTuple* = NULL );

private:
	static const QtNodeType nodeType;
};

