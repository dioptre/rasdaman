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
#include "rasdl/parse.hh"
#include "symbtbl.hh"
#include "yparse.hh"

#include <stdlib.h>

#include "debug/debug.hh"

#include "raslib/rmdebug.hh"
#include "catalogmgr/typefactory.hh"
#include "relcatalogif/alltypes.hh"

extern void output_scope(FILE*out,const YSymbolTable::Scope*scope);

Parse_info::Parse_info()
    : line(0), column(0), file(NULL), token(NULL)
{
}


Parse_info::Parse_info( long lineNo, int columnNo, const char* fileName, const char* tokenName )
    : line( lineNo ), column( columnNo ), file(NULL), token(NULL)
{
    if( fileName )
        file = strdup( fileName );

    if( tokenName )
        token = strdup( tokenName );
}



Parse_info::Parse_info( YWhere &where, const char* tokenName )
    : line( where.line ), column( where.column ), file(NULL), token(NULL)
{
    if( where.file )
        file = strdup( where.file );

    if( tokenName )
        token = strdup( tokenName );
}



Parse_info::Parse_info( const Parse_info& obj )
{
    line   = obj.line;
    column = obj.column;

    file  = obj.file  ? strdup( obj.file  ) : NULL;
    token = obj.token ? strdup( obj.token ) : NULL;
}



Parse_info::~Parse_info()
{
    if( file  )
    {
        free( file  );
        file  = NULL;
    }
    if( token )
    {
        free( token );
        token = NULL;
    }
}



const Parse_info&
Parse_info::operator=( const Parse_info& obj )
{
    if( this != &obj )
    {
        line   = obj.line;
        column = obj.column;

        file  = obj.file  ? strdup( obj.file  ) : NULL;
        token = obj.token ? strdup( obj.token ) : NULL;
    }

    return *this;
}



/* THE ATOM */
Parse_atom::Parse_atom()
{
    kind     =Atom;
    name     =NULL;
    symbol   =NULL;
}

Parse_atom::~Parse_atom()
{
}

void Parse_atom::insertData() const throw( r_Equery_execution_failed )
{
    RMDBGONCE(4, RMDebug::module_rasdl, "Parse_atom", "printData() kind " << kind << ", name " << name << ", symbol " << symbol)
    TALK( "Parse_atom::insertData: doing nothing with name=" << name << ", symbol=" << symbol)
}


void Parse_atom::setParseInfo( const Parse_info &token )
{
    parseInfo = token;
}


const Parse_info& Parse_atom::getParseInfo()
{
    return parseInfo;
}



/* THE TYPE */
Parse_type::Parse_type()
{
    kind      =Type;
    forward   =false;
};

Parse_type::~Parse_type()
{
}

const Type*
Parse_type::getType( const char* /*typeName*/ ) const
{
    std::cerr << "Internal error: getType() for a subclass of Parse_type not implemented." << std::endl;
    return NULL;
}

Parse_typereference::Parse_typereference()
{
    kind   =Typereference;
    type   =NULL;
}

Parse_typereference::~Parse_typereference()
{
}

void Parse_typereference::output(FILE*stream)const
{
    fprintf(stream,"%s",type->name);
};

const Type*
Parse_typereference::getType( const char* /*typeName*/ ) const
{
    const BaseType* catBaseType = TypeFactory::mapType( (char*)type->name );

    if( !catBaseType )
        // Error: Type reference not found..
        throw( r_Equery_execution_failed( 902, symbol->where.line, symbol->where.column, symbol->get_name() ) );

    return catBaseType;
}


/* composite */
Parse_composite::Parse_composite()
{
    kind      =Composite;
    elements  =NULL;
};

Parse_composite::Element::Element()
{
    readonly  =false;
    type      =NULL;
    name      =NULL;
    access    =Private;

    next      =NULL;
};

Parse_composite::Element::~Element()
{
}

void Parse_composite::Element::output(FILE*stream)const
{
    fprintf(stream,"   ");
    type->output(stream);
    fprintf(stream,"    %s;",name);

    if(readonly)
        fprintf(stream,"// ___readonly___\n");
    else
        fprintf(stream,"\n");
};

/* struct */
Parse_struct::Parse_struct()
{
    kind   =Struct;
};

Parse_struct::~Parse_struct()
{
}

void Parse_struct::output(FILE*stream)const
{
    fprintf(stream,"/* STRUCT -------------------------- %s */\n",name);
    fprintf(stream,"struct %s {\n",name);

    for(Element*scan=elements; scan!=NULL; scan=scan->next)
        scan->output(stream);

    fprintf(stream,"};\n");
};



void Parse_struct::insertData() const throw( r_Equery_execution_failed )
{
    ENTER( "Parse_struct::insertData" );
    RMDBGENTER(4, RMDebug::module_rasdl, "Parse_struct", "insertData()")

    // get catalog type structure
    StructType* catType = (StructType*)getType();
    TALK( "got type " << (char*)catType->getTypeName() );

    RMDBGMIDDLE(4, RMDebug::module_rasdl, "Parse_struct", "inserting type " << catType->getTypeName())

    if( TypeFactory::mapType( (char*)catType->getTypeName() ) )
        // Error: Struct type name exists already.
        throw( r_Equery_execution_failed( 905, symbol->where.line, symbol->where.column, symbol->get_name() ) );

    TALK( "adding to the database as cell struct type" );
    TypeFactory::addStructType( catType );

    RMDBGEXIT(4, RMDebug::module_rasdl, "Parse_struct", "insertData()")
    LEAVE( "Parse_struct::insertData" );
};



const Type*
Parse_struct::getType( const char* /*typeName*/ ) const
{
    unsigned int noElements=0;
    StructType*  structType=NULL;
    Element*     scan=NULL;

    RMDBGENTER(4, RMDebug::module_rasdl, "Parse_struct", "getType()")

    for( scan=elements; scan!=NULL; scan=scan->next, noElements++ );

    RMDBGMIDDLE(4, RMDebug::module_rasdl, "Parse_struct", "Struct " << name << " has " << noElements << " elements.")

    structType = new StructType( (char*)name, noElements );

    for( scan=elements; scan!=NULL; scan=scan->next)
        if( !scan->type )
            std::cerr << "Internal error: struct element doesn't deliver a catalog type" << std::endl;
        else
        {
            RMDBGMIDDLE(4, RMDebug::module_rasdl, "Parse_struct", "Scan->name " << scan->name)
            structType->addElement( (char*)(scan->name), (BaseType*)(scan->type->getType()) );
        }

    RMDBGEXIT(4, RMDebug::module_rasdl, "Parse_struct", "getType()" )

    return structType;
}



/* union */
Parse_union::Parse_union()
{
    kind   =Union;
};

Parse_union::~Parse_union()
{
}

void Parse_union::output(FILE*stream)const
{
    fprintf(stream,"/* UNION --------------------------- %s */\n",name);
    fprintf(stream,"struct %s {\n",name);

    for(Element*scan=elements; scan!=NULL; scan=scan->next)
        scan->output(stream);

    fprintf(stream,"};\n");
};

/* interface */
Parse_interface::Parse_interface()
{
    kind            =Interface;

    base_classes   =NULL;
    lifetime         =undefined;

    methods         =NULL;
    relationships   =NULL;
};

void print_access_mode(FILE*stream,Parse_composite::Access_mode access)
{
    switch(access)
    {
    case Parse_composite::Private:
    {
        fprintf(stream,"private:\n");
        break;
    };
    case Parse_composite::Public:
    {
        fprintf(stream,"public:\n");
        break;
    };
    case Parse_composite::Protected:
    {
        fprintf(stream,"protected:\n");
        break;
    };
    };
};

void Parse_interface::output(FILE*stream)const
{
    if(forward)
    {
        fprintf(stream,"class %s;/*<forward definition>*/\n",name);
        return;
    };

    fprintf(stream,"/* CLASS --------------------------- %s */\n",name);
    if(base_classes!=NULL)
    {
        fprintf(stream,"class %s : ",name);

        for(Base_class *scan=base_classes; scan!=NULL; scan=scan->next)
        {
            scan->output(stream);
            if(scan->next!=NULL)
                fprintf(stream,",");
        };

        fprintf(stream,"\n{\n");
    }
    else
        fprintf(stream,"class %s {\n",name);

//   output all symbols
    symbol->defines->output(stream);

    Access_mode   current_access=Private;

    if(methods!=NULL)
    {
        fprintf(stream,"// operations\n");
        methods->output(stream);
    };

    if(elements!=NULL)
    {
        fprintf(stream,"// attributes\n");

        for(Element *scan=elements; scan!=NULL; scan=scan->next)
        {
            if(scan->access!=current_access)
            {
                current_access=scan->access;
                print_access_mode(stream,current_access);
            };
            scan->output(stream);
        };
    };

    if(relationships!=NULL)
    {
        fprintf(stream,"//relationships\n");
    }

    fprintf(stream,"};\n");
};

Parse_interface::Base_class::Base_class()
{
    base_class   =NULL;
    access      =Private;

    next         =NULL;
};

void Parse_interface::Base_class::output(FILE*stream)const
{
    if(base_class==NULL)
        fprintf(stream,"<none>");
    else
        switch(access)
        {
        case Private:
            fprintf(stream,"private %s",base_class->name);
            break;
        case Public:
            fprintf(stream,"public %s",base_class->name);
            break;
        case Protected:
            fprintf(stream,"protected %s",base_class->name);
            break;
        };
};

Parse_interface::Method::Method()
{
    function   =NULL;
    access   =Private;

    next      =NULL;
};

void Parse_interface::Method::output(FILE*stream)const
{
    function->output(stream);
    fprintf(stream,";\n");
};

/* function */
Parse_function::Parse_function()
{
    kind         =Function;

    parameters   =NULL;
    return_type   =NULL;
};

void Parse_function::output(FILE*stream)const
{
    return_type->output(stream);
    fprintf(stream,"    %s(",name);

    if(parameters!=NULL)
    {
        parameters->output(stream);
    };
    fprintf(stream,")");
};

Parse_function::Parameter::Parameter()
{
    type   =NULL;
    state   =Unknown;
    name   =NULL;

    next   =NULL;
};

void Parse_function::Parameter::output(FILE*stream)const
{
    if(state==In)
        fprintf(stream,"const ");

    type->output(stream);

    if(state==Out)
        fprintf(stream,"&");
    else
        fprintf(stream," ");

    fprintf(stream,"%s",name);
};

/* operation */
Parse_operation::Parse_operation()
{
    kind         =Operation;
    scope_class   =NULL;
};

/* pointer */
Parse_pointer::Parse_pointer()
{
    kind      =Pointer;
    type      =NULL;
};

void Parse_pointer::output(FILE*stream)const
{
    type->output(stream);
    fprintf(stream,"*");
};

/* array */
Parse_array::Parse_array()
{
    kind   =Array;
    size   =0;
};

void Parse_array::output(FILE*stream)const
{
    type->output(stream);
    fprintf(stream,"[%i]",size);
};

/* alias */
Parse_alias::Parse_alias()
{
    kind   =Alias;

    type   =NULL;
    name   =NULL;
};

void Parse_alias::output(FILE*stream)const
{
    fprintf(stream,"/* TYPEDEF ------------------------- %s */\n",name);
    fprintf(stream,"typedef ");
    type->output(stream);
    fprintf(stream," %s",name);
    fprintf(stream,";\n\n");
};

void Parse_alias::insertData() const throw( r_Equery_execution_failed )
{
    ENTER( "Parse_alias::insertData" );
    RMDBGENTER(4, RMDebug::module_rasdl, "Parse_alias", "insertData()")

    // get catalog type structure

    const CType* catType = type->getType( name );
    if( !catType )
    {
        std::cerr << "Internal error: no type in alias definition." << std::endl;
        return;
    }
    TALK( "got type " << name );

    RMDBGIF(5, RMDebug::module_rasdl, "Parse_alias", \
    {
        \
        char* typeStructure = catType->getTypeStructure(); \
        RMInit::dbgOut << "Name " << catType->getTypeName() << ", structure " << typeStructure << std::endl; \
        free( typeStructure ); typeStructure = NULL; \
    } )

    switch( catType->getType() )
    {
    case MDDTYPE:
        if( TypeFactory::mapMDDType( (char*)catType->getTypeName() ) )
        {
            delete catType;
            // Error: MDD type name exists already.
            throw( r_Equery_execution_failed( 906, symbol->where.line, symbol->where.column, symbol->get_name() ) );
        }
        TALK( "adding to the database as MDD type" );
        TypeFactory::addMDDType( (MDDType*)catType );
        break;

    case SETTYPE:
        if( TypeFactory::mapType( (char*)catType->getTypeName() ) )
        {
            delete catType;
            // Error: Set type name exists already.
            throw( r_Equery_execution_failed( 907, symbol->where.line, symbol->where.column, symbol->get_name() ) );
        }
        TALK( "adding to the database as set type" );
        TypeFactory::addSetType( (SetType*)catType );
        break;

    default:
        delete catType;
        // Error: Type in typedef definition not supported
        throw( r_Equery_execution_failed( 900, symbol->where.line, symbol->where.column, symbol->get_name() ) );
    }

    delete catType;

    RMDBGEXIT( 4, RMDebug::module_rasdl, "Parse_alias", "insertData()")
    LEAVE( "Parse_alias::insertData" );
};

/* enumerator */
Parse_enum::Parse_enum()
{
    kind         =Enum;
    enumerators   =NULL;
};

void Parse_enum::output(FILE*stream)const
{
    fprintf(stream,"/* ENUMERATION --------------------- %s */\n",name);
    fprintf(stream,"enum %s {",name);

    const Enumerator   *scan=enumerators;
    while(scan!=NULL)
    {
        fprintf(stream,"%s=%i",scan->name,scan->value);

        scan=scan->next;
        if(scan!=NULL)
            fprintf(stream,",");
    };

    fprintf(stream,"};\n");
};

void Parse_enum::Enumerator::output(FILE*stream)const
{
    fprintf(stream,"// %s = %6i\n",name,value);
};

/* atomic types&classes */
void Parse_atomic::output(FILE*stream)const
{
    fprintf(stream,"%s",name);
};
/* any */
Parse_any::Parse_any()
{
    kind   =Any;
    name   ="r_Any";
};

/* void */
Parse_void::Parse_void()
{
    kind   =Void;
    name   ="void";
};

/* string */
Parse_string::Parse_string()
{
    kind      =String;
    name      ="r_String";

    length   =0;
};

/* boolean */
Parse_boolean::Parse_boolean()
{
    kind   =Boolean;
    name   ="r_Boolean";
};

const Type*
Parse_boolean::getType( const char* /*typeName*/ ) const
{
    return TypeFactory::mapType("Bool" );
}

/* float */
Parse_float::Parse_float()
{
    kind         =Float;
    name         ="float";
    accurance   =Single;
};

void Parse_float::output(FILE*stream)const
{
    switch(accurance)
    {
    case Single:
        fprintf(stream,"r_Float");
        break;
    case Double:
        fprintf(stream,"r_Double");
        break;
    }
};



const Type*
Parse_float::getType( const char* /*typeName*/ ) const
{
    const BaseType* type;

    if( accurance == Single )
        type = TypeFactory::mapType("Float");
    else
        type = TypeFactory::mapType("Double");

    return type;
}



/* int */
Parse_int::Parse_int()
{
    kind   =Integer;
    name   ="int";

    width   =Short;
    sign   =Signed;
};

void Parse_int::output(FILE*stream)const
{
    switch(sign)
    {
    case Signed:
        switch(width)
        {
        case Long:
            fprintf(stream,"r_Long ");
            break;
        case Short:
            fprintf(stream,"r_Short ");
            break;
        }
        break;
    case Unsigned:
        switch(width)
        {
        case Long:
            fprintf(stream,"r_ULong ");
            break;
        case Short:
            fprintf(stream,"r_UShort ");
            break;
        }
        break;
    }
};



const Type*
Parse_int::getType( const char* /*typeName*/ ) const
{
    const BaseType* type;

    if( sign == Signed )
    {
        switch(width)
        {
        case Long:
            type = TypeFactory::mapType("Long" );
            break;
        case Short:
            type = TypeFactory::mapType("Short");
            break;
        }
    }
    else
    {
        switch(width)
        {
        case Long:
            type = TypeFactory::mapType("ULong" );
            break;
        case Short:
            type = TypeFactory::mapType("UShort");
            break;
        }
    }

    return type;
}



/* octet */
Parse_octet::Parse_octet()
{
    kind   =Octet;
    name   ="r_Octet";
};



const Type*
Parse_octet::getType( const char* /*typeName*/ ) const
{
    return TypeFactory::mapType("Octet");
}


/* complex1 */
Parse_complex1::Parse_complex1()
{
    kind   = Complex1;
    name   = "r_Complex1";
};



const Type*
Parse_complex1::getType( const char* /*typeName*/ ) const
{
    return TypeFactory::mapType("Complex1");
}

/* complex2 */
Parse_complex2::Parse_complex2()
{
    kind   = Complex2;
    name   = "r_Complex2";
};



const Type*
Parse_complex2::getType( const char* /*typeName*/ ) const
{
    return TypeFactory::mapType("Complex2");
}




/* char */
Parse_char::Parse_char()
{
    kind   =Char;
    name   ="r_Char";
};

const Type*
Parse_char::getType( const char* /*typeName*/ ) const
{
    return TypeFactory::mapType("Char");
}



// forwiss atomics
Parse_atomic_templates::Parse_atomic_templates()
{
    kind         =Atomic_template;
    base_type   =NULL;
};

Parse_MDD::Parse_MDD()
{
    kind      =MDD;
    domain    =NULL;
};

void Parse_MDD::output(FILE*stream)const
{
    fprintf(stream,"r_Marray<");
    if(base_type!=NULL)
        base_type->output(stream);
    else
        fprintf(stream,"no_type");

    if(domain!=NULL)
    {
        fprintf(stream,"/*");
        char* stringDomain = domain->get_string_representation();
        fprintf(stream,"%s",stringDomain);
        free(stringDomain);
        fprintf(stream,"*/");
    };
    fprintf(stream,"> ");
};



const Type*
Parse_MDD::getType( const char* typeName ) const
{
    RMDBGENTER( 4, RMDebug::module_rasdl, "Parse_MDD", "getType()" )

    if( !base_type ||
            ( base_type->kind != Typereference && base_type->kind != Boolean &&
              base_type->kind != Float         && base_type->kind != Integer &&
              base_type->kind != Char          && base_type->kind != Octet   &&
              base_type->kind != Complex1    && base_type->kind != Complex2
            )
      )
        // Error: MDD base type has to be a type reference or an atomic type.
        throw( r_Equery_execution_failed( 903, parseInfo.line, parseInfo.column, parseInfo.token ) );

    //  if( !domain )
    //    // Error: MDD type must have a domain specification.
    //    throw( r_Equery_execution_failed( 904, parseInfo.line, parseInfo.column, parseInfo.token ) );

    if( !typeName )
    {
        std::cerr << "Internal error: mdd type needs a type name" << std::endl;
        return 0;
    }

    const BaseType* catBaseType = (BaseType*)base_type->getType();

    RMDBGIF(4, RMDebug::module_rasdl, "Parse_MDD", \
    {
        \
        char* typeStructure = catBaseType->getTypeStructure(); \
        RMInit::dbgOut << "  Base type name " << catBaseType->getTypeName() << ", structure " << typeStructure << std::endl; \
        free( typeStructure ); typeStructure = NULL; \
    } )

    RMDBGMIDDLE(4, RMDebug::module_rasdl, "Parse_MDD", "type name " << typeName << ", base typIe name " << catBaseType->getTypeName() )

    const MDDType* mddType;

    if( domain )
        mddType = new MDDDomainType( (char*)typeName, catBaseType, *domain );
    else if( dimensionality )
        mddType = new MDDDimensionType( (char*)typeName, catBaseType, (r_Dimension)dimensionality );
    else
        mddType = new MDDBaseType( (char*)typeName, catBaseType );

    RMDBGEXIT(4, RMDebug::module_rasdl, "Parse_MDD", "getType()")

    return mddType;
}



Parse_set::Parse_set()
{
    kind   =Set;
};

void Parse_set::output(FILE*stream)const
{
    fprintf(stream,"r_Set<r_Ref<");
    base_type->output(stream);
    fprintf(stream,"> >");
};



const Type*
Parse_set::getType( const char* typeName ) const
{
    RMDBGENTER(4, RMDebug::module_rasdl, "Parse_set", "getType()")

    if( !base_type || base_type->kind != Typereference )
        // Error: Set template type has to be a type reference.
        throw( r_Equery_execution_failed( 901, parseInfo.line, parseInfo.column, parseInfo.token ) );

    if( !typeName )
    {
        std::cerr << "Internal error: mdd type needs a type name" << std::endl;
        return NULL;
    }

    const char* baseTypeName = ((Parse_typereference*)base_type)->type->name;

    if( !baseTypeName )
    {
        std::cerr << "Internal error: set type needs a base type name" << std::endl;
        return NULL;
    }

    RMDBGMIDDLE(4, RMDebug::module_rasdl, "Parse_set", "Base type name " << baseTypeName)

    const MDDType* catBaseType = TypeFactory::mapMDDType( (char*)baseTypeName );

    if( !catBaseType )
        // Error: Type reference not found..
        throw( r_Equery_execution_failed( 902, base_type->symbol->where.line, base_type->symbol->where.column, base_type->name ) );

    RMDBGIF(5, RMDebug::module_rasdl, "Parse_set", \
    {
        \
        char* typeStructure = catBaseType->getTypeStructure(); \
        RMInit::dbgOut << "  Name " << catBaseType->getTypeName() << ", structure " << typeStructure << std::endl; \
        free( typeStructure ); typeStructure = NULL; \
    })

    RMDBGMIDDLE(4, RMDebug::module_rasdl, "Parse_set", "type name " << typeName << ", base type name " << baseTypeName )

    const SetType* setType = new SetType( (char*)typeName, catBaseType );

    RMDBGEXIT(4, RMDebug::module_rasdl, "Parse_set", "getType()")

    return setType;
}

