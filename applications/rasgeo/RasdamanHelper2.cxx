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
 * Copyright 2003 - 2011 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */

/*
 * Contributed to rasdaman by Alexander Herzig, Landcare Research New Zealand
 */

#include "RasdamanHelper2.h"
#define ctxrhelper "RasdamanHelper2"

// rasdaman
#include <sstream>
#include <new>
#include "raslib/mintervaltype.hh"
#include "raslib/collectiontype.hh"
#include "raslib/marraytype.hh"

// postgresql
#include "libpq-fe.h"

#include <limits>
#include <cmath>
#include "nmlog.h"

RasdamanHelper2::RasdamanHelper2(RasdamanConnector* rasconn)
{
    this->m_pRasconn = rasconn;
    this->m_pRasconn->connect();
    m_maximgsize = 134217728;
}

RasdamanHelper2::~RasdamanHelper2()
{
    if (this->m_transaction.get_status() == r_Transaction::active)
        this->m_transaction.abort();
}

double RasdamanHelper2::doesCollectionExist(std::string collname) throw (r_Error)
{
    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);
    int ret = -1;

    try
    {
        r_Ref_Any any = this->m_pRasconn->getDatabase().lookup_object(collname.c_str());
        if (any != 0)
            ret = any.get_oid().get_local_oid();
    }
    catch (r_Error& re)
    {
        if (re.get_kind() != r_Error::r_Error_ObjectUnknown)
            throw re;
    }

    this->m_transaction.commit();
    return ret;
}

void RasdamanHelper2::insertCollection(std::string collname, r_Type::r_Type_Id rtype,
                                       bool asCube)
{
    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_write);

    r_Ref<r_Set< r_Ref< r_GMarray > > > imgSet;

    this->createCollection(
        const_cast<r_Database&>(this->m_pRasconn->getDatabase()),
        imgSet, rtype, asCube);

    const_cast<r_Database&>(this->m_pRasconn->getDatabase()).set_object_name(
        *imgSet, collname.c_str());

    this->m_transaction.commit();
}

void RasdamanHelper2::insertUserCollection(std::string collname,
        std::string colltypename)
{
    NMDebugCtx(ctxrhelper, << "...");

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_write);

    std::stringstream qstr;
    qstr << "create collection " << collname << " " << colltypename;
    r_OQL_Query qins(qstr.str().c_str());
    r_oql_execute(qins);
    this->m_transaction.commit();

    NMDebugCtx(ctxrhelper, << "done!");
}

void RasdamanHelper2::createCollection(r_Database& db,
                                       r_Ref< r_Set< r_Ref< r_GMarray > > >& imgSet,
                                       r_Type::r_Type_Id rtype, bool asCube) throw (r_Error)
{
    std::string typestr = this->getTypePrefixString(rtype) + "Set";
    if (asCube)
        typestr += "3";
    bool unk = false;
    switch (rtype)
    {
    case r_Type::CHAR:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Char> > >;
        break;
    case r_Type::BOOL:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Boolean> > >;
        break;
    case r_Type::ULONG:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_ULong> > >;
        break;
    case r_Type::USHORT:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_UShort> > >;
        break;
    case r_Type::LONG:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Long> > >;
        break;
    case r_Type::SHORT:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Short> > >;
        break;
    case r_Type::OCTET:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Octet> > >;
        break;
    case r_Type::DOUBLE:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Double> > >;
        break;
    case r_Type::FLOAT:
        imgSet = new(&db, typestr.c_str())
        r_Set< r_Ref< r_Marray<r_Float> > >;
        break;
    default:
        imgSet = 0;
        unk = true;
        break;
    }

    if (unk)
        throw r_Error(r_Error::r_Error_TypeInvalid);
    else if (imgSet == 0)
        throw r_Error(r_Error::r_Error_MemoryAllocation);
}

void RasdamanHelper2::dropCollection(std::string collname)
{
    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_write);

    std::string qstr = "drop collection " + collname;
    r_OQL_Query q(qstr.c_str());

    r_oql_execute(q);

    this->m_transaction.commit();
}

void RasdamanHelper2::dropImage(std::string collname,
                                double localImgOID)
{
    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_write);

    std::string qstr = "delete from $1 as m where oid(m) = $2";
    r_OQL_Query quer(qstr.c_str());
    quer << collname.c_str() << (r_Long)localImgOID;

    r_oql_execute(quer);

    this->m_transaction.commit();
}

unsigned int RasdamanHelper2::getTypeSize(r_Type::r_Type_Id rtype) throw (r_Error)
{
    unsigned int ret = 0;

    switch (rtype)
    {
    case r_Type::CHAR:
        ret = sizeof(unsigned char);
        break;
    case r_Type::ULONG:
        ret = sizeof(unsigned long);
        break;
    case r_Type::USHORT:
        ret = sizeof(unsigned short);
        break;
    case r_Type::LONG:
        ret = sizeof(long);
        break;
    case r_Type::SHORT:
        ret = sizeof(short);
        break;
    case r_Type::OCTET:
        ret = sizeof(char);
        break;
    case r_Type::DOUBLE:
        ret = sizeof(double);
        break;
    case r_Type::FLOAT:
        ret = sizeof(float);
        break;
    default:
        throw r_Error(r_Error::r_Error_TypeInvalid);
        break;
    }

    return ret;
}

std::string RasdamanHelper2::getTypePrefixString(r_Type::r_Type_Id rtype)
{

    std::string ret = "";
    switch (rtype)
    {
    case r_Type::CHAR:
        ret = "Grey";
        break;
    case r_Type::ULONG:
        ret = "ULong";
        break;
    case r_Type::USHORT:
        ret = "UShort";
        break;
    case r_Type::LONG:
        ret = "Long";
        break;
    case r_Type::SHORT:
        ret = "Short";
        break;
    case r_Type::OCTET:
        ret = "Octet";
        break;
    case r_Type::DOUBLE:
        ret = "Double";
        break;
    case r_Type::FLOAT:
        ret = "Float";
        break;
    default:
        ret = "Unknwon";
        break;
    }
    return ret;
}

std::vector<double> RasdamanHelper2::getImageOIDs(std::string collname)
{
    NMDebugCtx(ctxrhelper, << "...");
    std::vector<double> soids;

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    std::string qstr = "select oid(m) from $1 as m";
    r_OQL_Query q(qstr.c_str());
    q << collname.c_str();

    NMDebugAI(<< q.get_query() << " ... " << endl);

    r_Set< r_Ref_Any > resSet;
    r_oql_execute(q, resSet);

    NMDebugAI(<< "... ");
    if (!resSet.is_empty())
    {
        r_Iterator< r_Ref_Any > iter = resSet.create_iterator();

        for (iter.reset(); iter.not_done(); iter++)
        {
            r_Primitive* id = (r_Primitive*)(*iter);
            soids.push_back(id->get_double());
            NMDebug(<< id->get_double() << " ");
        }
    }
    NMDebug(<< endl);

    this->m_transaction.commit();
    NMDebugCtx(ctxrhelper, << "done!")
    return soids;
}

r_Minterval RasdamanHelper2::getImageSdom(std::string collname, double localImgOID)
{
    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    std::string qstr = "select sdom(m) from $1 as m where oid(m) = $2";
    r_OQL_Query q(qstr.c_str());
    q << collname.c_str() << (r_Long)localImgOID;

    r_Set< r_Ref_Any > resSet;
    r_oql_execute(q, resSet);

    if (!resSet.is_empty())
    {
        r_Minterval* mval = (r_Minterval*)(*resSet.create_iterator());
        r_Minterval mint(*mval);
        this->m_transaction.commit();
        return mint;
    }

    this->m_transaction.commit();
    return r_Minterval();
}

r_Marray_Type *RasdamanHelper2::getMarrayType(std::string collname)
{
    NMDebugCtx(ctxrhelper, << "...");
    r_Marray_Type *martype = 0;

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    r_Ref_Any any = this->m_pRasconn->getDatabase().lookup_object(
                        collname.c_str());

    r_Set<r_GMarray>* coll((r_Set<r_GMarray>*)any.get_memory_ptr());

    r_Collection_Type* colltype = (r_Collection_Type*)coll->get_type_schema();
    martype = (r_Marray_Type*)colltype->element_type().clone();

    this->m_transaction.abort();

    NMDebugCtx(ctxrhelper, << "done!");
    return martype;
}

r_Type::r_Type_Id RasdamanHelper2::getBaseTypeId(std::string collname)
{
    NMDebugCtx(ctxrhelper, << "...");
    r_Type::r_Type_Id rtype = r_Type::UNKNOWNTYPE;

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    r_Ref_Any any = this->m_pRasconn->getDatabase().lookup_object(
                        collname.c_str());

    r_Set<r_GMarray>* coll((r_Set<r_GMarray>*)any.get_memory_ptr());

    r_Collection_Type* colltype = (r_Collection_Type*)coll->get_type_schema();
    r_Marray_Type *martype = (r_Marray_Type*)&colltype->element_type();
    r_Base_Type* basetype = const_cast<r_Base_Type*>(&martype->base_type());
    if (basetype->isPrimitiveType())
    {
        rtype = ((r_Primitive_Type*)basetype)->type_id();
    }
    else if (basetype->isStructType())
    {
        r_Structure_Type* stype = ((r_Structure_Type*)basetype);
        rtype = stype->resolve_attribute((unsigned int)0).type_of().type_id();
    }

    this->m_transaction.commit();

    NMDebugCtx(ctxrhelper, << "done!");
    return rtype;
}

void RasdamanHelper2::getImageBuffer(std::string collname, double localImgOID,
                                     char* buf, r_Minterval& sdom)
{
    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    std::string qstr = "select m$1 from $2 as m where oid(m) = $3";
    r_OQL_Query quer(qstr.c_str());
    quer << sdom << collname.c_str() << (r_Long)localImgOID;

    r_Set< r_Ref_Any > resSet;
    r_oql_execute(quer, resSet);

    if (!resSet.is_empty())
    {
        r_GMarray* ar = (r_GMarray*)(*resSet.create_iterator()).get_memory_ptr();
        r_Bytes size = ar->get_array_size();
        memcpy((void*)buf, (const void*)ar->get_array(), size);
    }

    this->m_transaction.commit();
}

std::string RasdamanHelper2::getBaseTypeName(std::string collname)
{
    std::string rstr = "Unknown";

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    r_Ref_Any any = this->m_pRasconn->getDatabase().lookup_object(
                        collname.c_str());

    r_Set<r_GMarray>* coll((r_Set<r_GMarray>*)any.get_memory_ptr());

    r_Collection_Type* colltype = (r_Collection_Type*)coll->get_type_schema();
    r_Marray_Type *martype = (r_Marray_Type*)&colltype->element_type();
    r_Base_Type* basetype = const_cast<r_Base_Type*>(&martype->base_type());
    rstr = basetype->name();

    this->m_transaction.abort();
    return rstr;
}

unsigned int RasdamanHelper2::getBaseTypeElementCount(std::string collname)
{
    NMDebugCtx(ctxrhelper, << "...");

    unsigned int ret = 1;

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    r_Ref_Any any = this->m_pRasconn->getDatabase().lookup_object(
                        collname.c_str());

    r_Set<r_GMarray>* coll((r_Set<r_GMarray>*)any.get_memory_ptr());

    r_Collection_Type* colltype = (r_Collection_Type*)coll->get_type_schema();
    r_Marray_Type *martype = (r_Marray_Type*)&colltype->element_type();
    r_Base_Type *basetype = const_cast<r_Base_Type*>(&martype->base_type());
    if (basetype->isStructType())
        ret = ((r_Structure_Type*)basetype)->count_elements();

    this->m_transaction.abort();

    NMDebugAI(<< "counted " << ret << " elements per pixel" << endl);
    NMDebugCtx(ctxrhelper, << "done!");
    return ret;
}

unsigned int RasdamanHelper2::getBaseTypeSize(std::string collname)
{
    unsigned int len = -1;

    //this->m_pRasconn->connect();
    this->m_transaction.begin(r_Transaction::read_only);

    r_Ref_Any any = this->m_pRasconn->getDatabase().lookup_object(
                        collname.c_str());

    r_Set<r_GMarray>* coll((r_Set<r_GMarray>*)any.get_memory_ptr());

    r_Collection_Type* colltype = (r_Collection_Type*)coll->get_type_schema();
    r_Marray_Type *martype = (r_Marray_Type*)&colltype->element_type();
    r_Base_Type* basetype = const_cast<r_Base_Type*>(&martype->base_type());
    len = basetype->size();
    this->m_transaction.abort();
    return len;
}

double RasdamanHelper2::insertImage(std::string collname,
                                    char* buf, r_Point& shift, r_Minterval& sdom, bool bRowMajor2ColMajor,
                                    std::string marraytypename)
{
    NMDebugCtx(ctxrhelper, << "...");

    // get type information about the collection
    r_Type::r_Type_Id tid = this->getBaseTypeId(collname);
    NMDebugAI(<< "collection's pixel base type: " << this->getDataTypeString(tid) << endl);

    // get the number of elements (in case we've got a struct type)
    unsigned int nelem = this->getBaseTypeElementCount(collname);

    // get a list of oids available prior to inserting the new image
    std::vector<double> preoids = this->getImageOIDs(collname);

    // create an initial image
    this->m_transaction.begin(r_Transaction::read_write);

    std::string qstr = "insert into $1 values marray x in [";

    // format the spatial domain string
    std::stringstream sdomstr;
    for (int d = 0; d < sdom.dimension(); d++)
    {
        sdomstr << shift[d] << ":" << shift[d];
        if (d == sdom.dimension()-1)
            sdomstr << "]";
        else
            sdomstr << ",";
    }
    qstr += sdomstr.str();

    // format the values string depending on the nubmer of elements
    // of the base type
    string numconst = this->getNumConstChar(tid);
    NMDebugAI(<< "numeric constant is: " << numconst << endl);
    if (nelem == 1)
    {
        qstr += " values 0" + numconst;
    }
    else // struct type
    {
        qstr += " values {";
        for (int e=0; e < nelem; ++e)
        {
            qstr += "0" + numconst;
            if (e < nelem -1)
                qstr += ", ";
            else
                qstr += "}";
        }
    }

    NMDebugAI( << "dummy grid query: " << qstr << std::endl);

    r_OQL_Query qins(qstr.c_str());
    qins << collname.c_str();
    r_oql_execute(qins);
    this->m_transaction.commit();

    //NMDebugInd(1, << "dummy grid inserted!" << std::endl);

    // update the initially created image with the actual values

    // get the oid of the initially updated image
    std::vector<double> voids = this->getImageOIDs(collname);
    double oid = -1; //voids[voids.size()-1];
    if (voids.size() > 0)
        oid = voids[voids.size()-1];

    // since we're not quite sure whether the new oid is always the
    // last in the oid result set retrieved, we'd better check them
    // all
//  for (int post=voids.size()-1; post >= 0; post--)
//  {
//      oid = voids[post];
//      for (int pre=preoids.size()-1; pre >= 0; pre--)
//      {
//          //NMDebugInd(1, << voids[post] << " == " << preoids[pre] << std::endl);
//          if (voids[post] == preoids[pre])
//          {
//              oid = -1;
//              break;
//          }
//      }
//      if (oid != -1)
//          break;
//  }

    NMDebugAI( << "local oid of new image is #" << oid << std::endl);

    // if we get a null pointer, we quit here
    if (buf != 0 && oid != -1)
    {
        this->updateImage(collname, oid, buf, shift, sdom, bRowMajor2ColMajor,
                          marraytypename);
    }

    //NMDebugInd(1, << "updated initial image with buffer content!" << std::endl);

    NMDebugCtx(ctxrhelper, << "done!");
    return oid;
}

void RasdamanHelper2::updateImage(std::string collname, double imgid,
                                  char* buf, r_Point& shift, r_Minterval& sdom, bool bRowMajor2ColMajor,
                                  std::string marraytypename) throw (r_Error)
{
    NMDebugCtx(ctxrhelper, << "...");

    // query type information
    r_Marray_Type *martype = this->getMarrayType(collname);
    const r_Base_Type &basetype = (*martype).base_type();
    unsigned nelem = 1;
    unsigned int elemsize = 0;
    r_Type::r_Type_Id tid;
    if (basetype.isPrimitiveType())
    {
        tid = ((r_Primitive_Type&)basetype).type_id();
        elemsize = basetype.size();
    }
    else if (basetype.isStructType())
    {
        r_Structure_Type *stype = ((r_Structure_Type*)&basetype);
        nelem = stype->count_elements();
        tid = stype->resolve_attribute((unsigned int)0).type_of().type_id();
        elemsize = stype->resolve_attribute((unsigned int)0).type_of().size();
    }
    unsigned int pixelsize = elemsize * nelem;

    delete martype;

//  r_Type::r_Type_Id tid = this->getBaseTypeId(collname);
//  unsigned int nelem = this->getBaseTypeElementCount(collname);
//  unsigned int elemsize = this->getBaseTypeSize(collname);
//  unsigned int pixelsize = elemsize * nelem;

    // construct typename for flat base types
    // and take the user specified typename for structs
    if (nelem == 1)
    {
        std::string appendix = "";
        if (sdom.dimension() == 2)
            appendix = "Image";
        else if (sdom.dimension() == 3)
            appendix = "Cube";

        marraytypename = this->getTypePrefixString(tid) + appendix;
    }
    NMDebugAI( << "Marray type: " << marraytypename << endl);

    int nlayers;
    if (sdom.dimension() == 2)
        nlayers = 1;
    else if (sdom.dimension() == 3)
        nlayers = sdom[2].get_extent();
    else
        throw r_Error(r_Error::r_Error_FeatureNotSupported);

    // initiate the r_GMarray, re-organise the array, if applicable,
    // and update the image
    r_Ref< r_GMarray > img;
    try
    {
        img = new (marraytypename.c_str()) r_GMarray(sdom, pixelsize);

        if (bRowMajor2ColMajor)
        {
            // we use the 3D restricted version for now, because its greater performance
            // as long as we don't support creating nD collections, we should stick with this
            // one
            this->rowBuf2ColBuf(buf, img->get_array(), pixelsize, nelem,
                                sdom[0].get_extent(), sdom[1].get_extent(), nlayers);
            //      this->rowBuf2ColBuf(buf, img->get_array(), pixelsize, sdom);
        }
        else
        {
            memcpy((void*)img->get_array(), (const void*)buf, sdom.cell_count() * pixelsize);
        }

        // update initially created image
        std::string qstr = "update $1 as m set m assign shift($2, $3) where oid(m) = $4";
        r_OQL_Query q(qstr.c_str());
        q << collname.c_str() << *img << shift << (r_Long)imgid;

        this->m_transaction.begin(r_Transaction::read_write);
        NMDebugAI(<< q.get_query() << endl);
        r_oql_execute(q);
        this->m_transaction.commit();

        // clean up
        img->r_deactivate();
    }
    catch (r_Error& e)
    {
        img->r_deactivate();
        NMErr(ctxrhelper, << "image update failed: " << e.what() << endl);
        throw;
    }

    NMDebugCtx(ctxrhelper, << "done!");
}

void RasdamanHelper2::createMDD(r_Ref< r_GMarray >& image, r_Minterval sdom,
                                r_Type::r_Type_Id rtype) throw (r_Error)
{
    std::string appendix = "";
    if (sdom.dimension() == 2)
        appendix = "Image";
    else if (sdom.dimension() == 3)
        appendix = "Cube";
    else
        throw r_Error(r_Error::r_Error_TypeInvalid);

    std::string typestr = this->getTypePrefixString(rtype) + appendix;

    switch (rtype)
    {
    case r_Type::CHAR:
        image = new (typestr.c_str()) r_Marray<r_Char>(sdom);
        break;
    case r_Type::BOOL:
        image = new (typestr.c_str()) r_Marray<r_Boolean>(sdom);
        break;
    case r_Type::ULONG:
        image = new (typestr.c_str()) r_Marray<r_ULong>(sdom);
        break;
    case r_Type::USHORT:
        image = new (typestr.c_str()) r_Marray<r_UShort>(sdom);
        break;
    case r_Type::LONG:
        image = new (typestr.c_str()) r_Marray<r_Long>(sdom);
        break;
    case r_Type::SHORT:
        image = new (typestr.c_str()) r_Marray<r_Short>(sdom);
        break;
    case r_Type::OCTET:
        image = new (typestr.c_str()) r_Marray<r_Octet>(sdom);
        break;
    case r_Type::DOUBLE:
        image = new (typestr.c_str()) r_Marray<r_Double>(sdom);
        break;
    case r_Type::FLOAT:
        image = new (typestr.c_str()) r_Marray<r_Float>(sdom);
        break;
    default:
        throw r_Error(r_Error::r_Error_TypeInvalid);
        return;
        break;
    }

    if (image == 0)
        throw r_Error(r_Error::r_Error_MemoryAllocation);
}

void RasdamanHelper2::colBuf2RowBuf(char* colbuf, char* rowbuf, r_Type::r_Type_Id rtype,
                                    int ncols, int nrows, int nlayers) throw (r_Error)
{
    // map content of col-based colbuf to row-based rowbuf
    int col, row, layer;
    for (row = 0; row < nrows; row++)
    {
        for(col = 0; col < ncols; col++)
        {
            for (layer = 0; layer < nlayers; layer++)
            {
                switch (rtype)
                {
                case r_Type::CHAR:
                case r_Type::BOOL:
                    ((unsigned char*) rowbuf)[row * ncols + col + layer*ncols*nrows]
                    = ((unsigned char*) colbuf)[col * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::ULONG:
                    ((unsigned int*) rowbuf)[row * ncols + col + layer*ncols*nrows]
                    = ((unsigned int*) colbuf)[col * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::USHORT:
                    ((unsigned short*) rowbuf)[row * ncols + col + layer*ncols*nrows]
                    = ((unsigned short*) colbuf)[col * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::LONG:
                    ((int*) rowbuf)[row * ncols + col + layer*ncols*nrows] = ((int*) colbuf)[col
                            * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::SHORT:
                    ((short*) rowbuf)[row * ncols + col + layer*ncols*nrows] = ((short*) colbuf)[col
                            * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::OCTET:
                    ((signed char*) rowbuf)[row * ncols + col + layer*ncols*nrows]
                    = ((signed char*) colbuf)[col * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::DOUBLE:
                    ((double*) rowbuf)[row * ncols + col + layer*ncols*nrows] = ((double*) colbuf)[col
                            * nrows + row + layer*ncols*nrows];
                    break;
                case r_Type::FLOAT:
                    ((float*) rowbuf)[row * ncols + col + layer*ncols*nrows] = ((float*) colbuf)[col
                            * nrows + row + layer*ncols*nrows];
                    break;
                default:
                    throw r_Error(r_Error::r_Error_TypeInvalid);
                    break;
                }
            }
        }
    }
}

void RasdamanHelper2::colBuf2RowBuf(char* colbuf, char* rowbuf, unsigned int pixelsize,
                                    r_Minterval& sdom)
{
    vector<int> cmdims;
    cmdims.resize(sdom.dimension());

    int npix = 1;
    for (int d=0; d < sdom.dimension(); ++d)
    {
        cmdims[d] = sdom[d].get_extent();
        npix *= sdom[d].get_extent();
    }

    // swap col and row dim (i.e. from row-major to col-major)
    cmdims[0] = sdom[1].get_extent();
    cmdims[1] = sdom[0].get_extent();

    vector<int> cmidx;
    vector<int> rmidx;
    cmidx.resize(cmdims.size());
    rmidx.resize(cmdims.size());

    int rmpix;
    for (int pix=0; pix < npix; ++pix)
    {
        cmidx = this->offset2index(pix, cmdims);
        rmidx = cmidx;
        rmidx[0] = cmidx[1];
        rmidx[1] = cmidx[0];
        rmpix = this->index2offset(sdom, rmidx);

        memcpy((void*)(rowbuf + rmpix * pixelsize),
               (const void*)(colbuf + pix * pixelsize), pixelsize);
    }
}

void RasdamanHelper2::rowBuf2ColBuf(char* rowbuf, char* colbuf, unsigned int pixelsize,
                                    r_Minterval& sdom)
{
    vector<int> cmdims;
    cmdims.resize(sdom.dimension());

    int npix = 1;
    for (int d=0; d < sdom.dimension(); ++d)
    {
        cmdims[d] = sdom[d].get_extent();
        npix *= sdom[d].get_extent();
    }

    // swap col and row dim (i.e. from row-major to col-major)
    cmdims[0] = sdom[1].get_extent();
    cmdims[1] = sdom[0].get_extent();

    vector<int> cmidx;
    vector<int> rmidx;
    cmidx.resize(cmdims.size());
    rmidx.resize(cmdims.size());

    int cmpix;
    for (int pix=0; pix < npix; ++pix)
    {
        rmidx = offset2index(pix, sdom);
        cmidx = rmidx;
        cmidx[0] = rmidx[1];
        cmidx[1] = rmidx[0];
        cmpix = this->index2offset(cmdims, cmidx);

        memcpy((void*)(colbuf + cmpix * pixelsize),
               (const void*)(rowbuf + pix * pixelsize), pixelsize);
    }
}

void RasdamanHelper2::rowBuf2ColBuf(char* rowbuf, char* colbuf, unsigned int pixelsize,
                                    unsigned int nelem, int ncols, int nrows, int nlayers)
{
    int elemsize = pixelsize / nelem;
    int elemoffsize = elemsize;

    if (nelem == 1)
    {
        elemsize = pixelsize;
        elemoffsize = 0;
    }

    int row, col, layer, elem, elemoff;
    for (row = 0; row < nrows; ++row)
    {
        for(col = 0; col < ncols; ++col)
        {
            for (layer = 0; layer < nlayers; ++layer)
            {
                for (elem = 0, elemoff = 0;
                        elem < nelem;
                        ++elem, elemoff += elemoffsize)
                {
                    memcpy((void*)(colbuf +  (col * nrows + row + layer*ncols*nrows)
                                   * pixelsize + elemoff),
                           (const void*)(rowbuf + (row * ncols + col + layer*ncols*nrows)
                                         * pixelsize + elemoff),
                           elemsize);
                }
            }
        }
    }
}

void RasdamanHelper2::colBuf2RowBuf(char* colbuf, char* rowbuf, unsigned int pixelsize,
                                    unsigned int nelem, int ncols, int nrows, int nlayers)
{
    int elemsize = pixelsize / nelem;
    int elemoffsize = elemsize;

    if (nelem == 1)
    {
        elemsize = pixelsize;
        elemoffsize = 0;
    }

    int row, col, layer, elem, elemoff;
    for (row = 0; row < nrows; ++row)
    {
        for(col = 0; col < ncols; ++col)
        {
            for (layer = 0; layer < nlayers; ++layer)
            {
                for (elem = 0, elemoff = 0;
                        elem < nelem;
                        ++elem, elemoff += elemoffsize)
                {
                    memcpy((void*)(rowbuf + (row * ncols + col + layer*ncols*nrows)
                                   * pixelsize + elemoff),
                           (const void*)(colbuf +  (col * nrows + row + layer*ncols*nrows)
                                         * pixelsize + elemoff),
                           elemsize);
                }
            }
        }
    }
}


void RasdamanHelper2::rowBuf2ColBuf(char* rowbuf, char* colbuf, r_Type::r_Type_Id rtype,
                                    int ncols, int nrows, int nlayers) throw (r_Error)
{
    // map content of row-based rowbuf to col-based colbuf
    int col, row, layer;
    for (row = 0; row < nrows; row++)
    {
        for (col = 0; col < ncols; col++)
        {
            for (layer = 0; layer < nlayers; layer++)
            {
                switch (rtype)
                {
                case r_Type::CHAR:
                case r_Type::BOOL:
                    ((unsigned char*) colbuf)[col * nrows + row + layer*ncols*nrows]
                    = ((unsigned char*) rowbuf)[row * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::ULONG:
                    ((unsigned int*) colbuf)[col * nrows + row + layer*ncols*nrows]
                    = ((unsigned int*) rowbuf)[row * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::USHORT:
                    ((unsigned short*) colbuf)[col * nrows + row + layer*ncols*nrows]
                    = ((unsigned short*) rowbuf)[row * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::LONG:
                    ((int*) colbuf)[col * nrows + row + layer*ncols*nrows] = ((int*) rowbuf)[row
                            * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::SHORT:
                    ((short*) colbuf)[col * nrows + row + layer*ncols*nrows] = ((short*) rowbuf)[row
                            * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::OCTET:
                    ((signed char*) colbuf)[col * nrows + row + layer*ncols*nrows]
                    = ((signed char*) rowbuf)[row * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::DOUBLE:
                    ((double*) colbuf)[col * nrows + row + layer*ncols*nrows] = ((double*) rowbuf)[row
                            * ncols + col + layer*ncols*nrows];
                    break;
                case r_Type::FLOAT:
                    ((float*) colbuf)[col * nrows + row + layer*ncols*nrows] = ((float*) rowbuf)[row
                            * ncols + col + layer*ncols*nrows];
                    break;
                default:
                    throw r_Error(r_Error::r_Error_TypeInvalid);
                    break;
                }
            }
        }
    }
}

std::vector<int> RasdamanHelper2::offset2index(int offset, std::vector<int>& sdom)
{
    std::vector<int> idx;
    idx.resize(sdom.size());

    for (int d=0; d < sdom.size(); ++d)
    {
        idx[d] = offset % sdom[d];
        offset /= sdom[d];
    }

    return idx;
}

std::vector<int> RasdamanHelper2::offset2index(int offset, r_Minterval& sdom)
{
    std::vector<int> idx;
    idx.resize(sdom.dimension());

    for (int d=0; d < sdom.dimension(); ++d)
    {
        idx[d] = offset % sdom[d].get_extent();
        offset /= sdom[d].get_extent();
    }

    return idx;
}

int RasdamanHelper2::index2offset(r_Minterval& sdom, std::vector<int>& index)
{
    int offset = 0;
    int mult = 1;
    for (int d=sdom.dimension()-1; d >=0; --d)
    {
        for (int r=d; r >=0; --r)
        {
            if (r == d)
                mult = index[d];
            else
                mult *= sdom[r].get_extent();
        }
        offset += mult;
    }

    return offset;
}

int RasdamanHelper2::index2offset(std::vector<int>& sdom, std::vector<int>& index)
{
    int offset = 0;
    int mult = 1;
    for (int d=sdom.size()-1; d >=0; --d)
    {
        for (int r=d; r >=0; --r)
        {
            if (r == d)
                mult = index[d];
            else
                mult *= sdom[r];
        }
        offset += mult;
    }

    return offset;
}



std::string RasdamanHelper2::getNumConstChar(r_Type::r_Type_Id rtype) throw (r_Error)
{
    std::string numconst;
    switch (rtype)
    {
    case r_Type::OCTET:
        numconst =  "o";
        break;
    case r_Type::CHAR:
        numconst =  "c";
        break;
    case r_Type::BOOL:
        //numconst = "";
        break;
    case r_Type::ULONG:
        numconst =  "ul";
        break;
    case r_Type::USHORT:
        numconst =  "us";
        break;
    case r_Type::LONG:
        numconst =  "l";
        break;
    case r_Type::SHORT:
        numconst =  "s";
        break;
    case r_Type::DOUBLE:
        numconst =  "d";
        break;
    case r_Type::FLOAT:
        numconst =  "f";
        break;
    default:
        throw r_Error(r_Error::r_Error_TypeInvalid);
        break;
    }

    return numconst;
}

bool RasdamanHelper2::isNMMetaAvailable(void)
{
    bool ret = true;

    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        ret = false;
    }

    std::stringstream query;
    PGresult* res;
    query << "select * from pg_class where relname = 'nm_meta'";
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebugInd(1, << "meta data table 'nm_meta' not found!" << std::endl);
        ret = false;
    }
    PQclear(res);
    return ret;
}

void RasdamanHelper2::writeNMMetadata(
    string collname,
    long oid,
    long epsgcode,
    string crsname,
    double minx,
    double maxx,
    double miny,
    double maxy,
    double minz,
    double maxz,
    double csx,
    double csy,
    double csz,
    string pixeltype,
    double stats_min,
    double stats_max,
    double stats_mean,
    double stats_stddev,
    string RATName) throw (r_Error)
{
    NMDebugCtx(ctxrhelper, << "...");

    // establish connection
    //NMDebugInd(1, << "connecting with '" << this->m_pRasconn->getDbName() << "' ... ");

    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        throw r_Error(r_Error::r_Error_General);
        return;
    }

    // check, whether the nm_meta is installed at all
    if (!this->isNMMetaAvailable())
        return;

    // write values into data base, if necessary create new record in table
    std::stringstream columns;
    columns << "(img_id, coll_name, epsg_code, " <<
            "crs_name, minx, maxx, miny, maxy, minz, maxz, " <<
            "cellsize_x, cellsize_y, cellsize_z, pixeltype, " <<
            "stats_min, stats_max, stats_mean, stats_stddev, attrtable_name)";

    string c = ", ";
    string cso = ", '";
    string csc = "', ";
    string csco = "', '";
    std::stringstream values;
    values.precision(14);
    values << "(" << oid << cso << collname << csc << epsgcode <<
           cso << crsname << csc << minx << c << maxx << c <<
           miny << c << maxy << c << minz << c << maxz << c <<
           csx << c << csy << c << csz << cso << pixeltype << csc <<
           stats_min << c << stats_max << c << stats_mean << c << stats_stddev <<
           cso << RATName << "')";


    PGresult* res;
    stringstream query;
    query << "select * from nm_meta where img_id = " << oid;
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        // insert operation
        PQclear(res);
        query.str(std::string());
        query << "insert into nm_meta " << columns.str() << " values " << values.str();
        NMDebugAI(<< "insert query ... " << std::endl << query.str() << std::endl);

        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    }
    else
    {
        // update operation
        PQclear(res);
        query.str(std::string());
        query << "update nm_meta set " << columns.str() << " = " << values.str() << " where img_id = " << oid;
        NMDebugAI(<< "update query ... " << std::endl << query.str() << std::endl);

        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    }

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        NMErr(ctxrhelper, << "writing to nm_meta failed!" << endl << PQresultErrorMessage(res));
    }

    PQclear(res);

    NMDebugCtx(ctxrhelper, << "done!");
}

std::vector<double> RasdamanHelper2::getNMMetaGeoDomain(long oid)
{
    // init return value with empty x/y region
    std::vector<double> dom;
    double dmax = std::numeric_limits<double>::max();
    double dmin = std::numeric_limits<double>::max() * -1;
    for (int e=0; e < 4; e++)
    {
        if (std::fmod((float)e, (float)2) == 0)
            dom.push_back(dmax);
        else
            dom.push_back(dmin);
    }
    // z region is as big as possible
    dom.push_back(dmin);
    dom.push_back(dmax);

    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        return dom;
    }

    // check, whether the nm_meta is installed at all
    if (!this->isNMMetaAvailable())
        return dom;

    std::stringstream query;
    query << "select minx, maxx, miny, maxy, minz, maxz " <<
          "from nm_meta where img_id = " << oid;
    PGresult* res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebugInd(1, << "could not find oid #" << oid << " in nm_meta!" << std::endl);
        PQclear(res);
        return dom;
    }

    for (int f=0; f < 6; f++)
        dom[f] = atof(PQgetvalue(res, 0, f));

    return dom;
}

std::vector<double> RasdamanHelper2::getNMMetaCellSize(long oid)
{
    NMDebugCtx(ctxrhelper, << "...");

    std::vector<double> cellsize;

    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        return cellsize;
    }

    // check, whether the nm_meta is installed at all
    if (!this->isNMMetaAvailable())
        return cellsize;

    std::stringstream query;
    query << "select cellsize_x, cellsize_y, cellsize_z " <<
          "from nm_meta where img_id = " << oid;
    PGresult* res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebugAI(<< "could not find oid #" << oid << " in nm_meta!" << std::endl);
        NMDebugCtx(ctxrhelper, << "done!");
        PQclear(res);
        return cellsize;
    }

    NMDebugAI(<< "cellsize per dim: ");
    for (int f=0; f < 3; f++)
    {
        cellsize.push_back(atof(PQgetvalue(res, 0, f)));
        NMDebug(<< cellsize[f] << "  ");
    }
    NMDebug(<< std::endl);

    NMDebugCtx(ctxrhelper, << "done!");
    return cellsize;
}

int
RasdamanHelper2::writeNMRAT(std::string filename, double oid, int band)
throw(r_Error)
{
    NMDebugCtx(ctxrhelper, << "...");

    // -------------------------------------------------------------
    // let's check whether we've got everything

    GDALDataset* pDs = (GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly);
    if (pDs == 0)
    {
        NMErr(ctxrhelper, << "failed opening data set '" << filename << "'!");
        return 0;
    }

    if (band < 1 || band > pDs->GetRasterCount())
    {
        NMDebugInd(1, << "given band number is outside available band range!" <<
                   std::endl);
        return 0;
    }

    GDALRasterBand* pBand = pDs->GetRasterBand(band);
    const GDALRasterAttributeTable* pRAT = pBand->GetDefaultRAT();

    if (pRAT == 0)
    {
        NMDebugInd(1, << filename << " does not have a RAT, so we skip that part"
                   << std::endl);
        NMDebugCtx(ctxrhelper, << "done!");
        return 0;
    }

    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        GDALClose(pDs);
        throw r_Error(r_Error::r_Error_General);
        return 0;
    }

    // check, whether the nm_meta is installed at all
    if (!this->isNMMetaAvailable())
        return 0;

    // -------------------------------------------------------------

    std::stringstream query;
    query.precision(14);
    PGresult* res;

    // analyse the table structure
    int ncols = pRAT->GetColumnCount();
    int nrows = pRAT->GetRowCount();

    std::vector< std::string > colnames;
    std::vector< ::GDALRATFieldType > coltypes;

    // the first field is going to store the row index 'rowidx', which starts
    // at 0 and is incremented for each further row; this is necessary
    // to support indexed raster layers which refer to attributes by
    // their rowindex of the associated attribute table (e.g. ERDAS IMAGINE files *.img)

    string k = ", ";
    string s = " ";

    query << "create table nmrat_" << oid << " ("
          "rowidx integer unique NOT NULL,";

    int c, r;
    for (c=0; c < ncols; c++)
    {
        colnames.push_back(pRAT->GetNameOfCol(c));
        coltypes.push_back(pRAT->GetTypeOfCol(c));

        string typestr = "";
        switch (coltypes[c])
        {
        case GFT_Integer:
            typestr = "integer";
            break;
        case GFT_Real:
            typestr = "double precision";
            break;
        case GFT_String:
            typestr = "text";
            break;
        }

        query << colnames[c] << s << typestr << k;
    }

    query << "constraint nmrat_" << oid << "_pkey primary key (rowidx))";

    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        NMErr(ctxrhelper, "creating raster attribute table for image '" <<
              oid << "' failed: " << endl << PQresultErrorMessage(res));
        PQclear(res);
        return 0;
    }
    NMDebug(<< "done!" << endl);
    query.str("");
    PQclear(res);

    NMDebugInd(1, << "copying table content ... ");
    // copy the table body into the postgres table
    for (r=0; r < nrows; r++)
    {
        query << "insert into nmrat_" << oid << " values (" << r << k;
        for (c=0; c < ncols; c++)
        {
            switch (coltypes[c])
            {
            case GFT_Integer:
                query << pRAT->GetValueAsInt(r, c);
                break;
            case GFT_Real:
                query << pRAT->GetValueAsDouble(r, c);
                break;
            case GFT_String:
                query << "'" << pRAT->GetValueAsString(r, c) << "'";
                break;
            }

            if (c < ncols -1)
                query << k;
            else
                query << ")";
        }

        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << "failed copying row " << r << "/" << nrows << " for table 'nmrat_" <<
                  oid << "': " << endl << PQresultErrorMessage(res));
        }

        query.str("");
        PQclear(res);
    }
    NMDebug(<< " done!" << endl);

    // --------------------------------------------------------------

    // write the name of this table into the nm_meta table
    query << "update nm_meta set attrtable_name = 'nmrat_" << oid << "' " <<
          "where img_id = " << oid;
    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        NMDebug(<< std::endl);
        NMErr(ctxrhelper, << "failed writing RAT name 'nmrat_" << oid
              << "' into nm_meta table: " << endl << PQresultErrorMessage(res));
        PQclear(res);
        return 0;
    }
    NMDebug(<< "done!" << endl);

    // disconnect from postgres
    PQclear(res);

    return 1;
}

int RasdamanHelper2::getWCPSTypeId(r_Type::r_Type_Id rtype)
{
    std::string sdt = "";
    int t;
    switch (rtype)
    {
    case r_Type::CHAR:
        t = 3;
        sdt = "unsigned char";
        break;
    case r_Type::BOOL:
        t = 1;
        sdt = "boolean";
        break;
    case r_Type::ULONG:
        t = 9;
        sdt = "unsigned long";
        break;
    case r_Type::USHORT:
        t = 5;
        sdt = "unsigned short";
        break;
    case r_Type::LONG:
        t = 8;
        sdt = "long";
        break;
    case r_Type::SHORT:
        t = 4;
        sdt = "short";
        break;
    case r_Type::OCTET:
        t = 2;
        sdt = "char";
        break;
    case r_Type::DOUBLE:
        t = 11;
        sdt = "double";
        break;
    case r_Type::FLOAT:
        t = 10;
        sdt = "float";
        break;
    default:
        t = 11;
        sdt = "double";
        break;
    }

    const PGconn* conn = this->m_pRasconn->getPetaConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getPetaDbName() << "' failed!");
        return -1;
    }

    std::stringstream query;
    query << "select id from ps_datatype where datatype = '" << sdt << "'";
    PGresult* res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebugInd(1, << "couldn't find data type '" << sdt << "' in ps_datatype!" << std::endl);
        PQclear(res);
        return -1;
    }
    t = atoi(PQgetvalue(res, 0, 0));

    return t;
}

int RasdamanHelper2::writePSMetadata(std::string collname,
                                     std::string crs, r_Type::r_Type_Id rtype, int nbands,
                                     double xmin, double xmax,
                                     double ymin, double ymax,
                                     double zmin, double zmax,
                                     int xpix, int ypix, int zpix)
{
    NMDebugCtx(ctxrhelper, << "...");

    const PGconn* conn = this->m_pRasconn->getPetaConnection();
    if (PQstatus(conn) != CONNECTION_OK)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << "connection to '" <<
              this->m_pRasconn->getPetaDbName() << "' failed!");
        return 0;
    }

    // some vars
    string coll = collname;

    // even more vars
    int image_id;
    int crs_id;
    int x_id;
    int y_id;
    int z_id;

    std::stringstream columns;
    std::stringstream values;
    values.precision(14);
    std::stringstream query;
    PGresult* res;

    // let's have a look, whether the collection is already present or not
    bool bUpdate;
    query << "select * from ps_coverage where name = '" << coll << "'";
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
        bUpdate = false;
    else
        bUpdate = true;
    PQclear(res);
    query.str("");


    // format the nullvalue for the coverage, which is used
    // by ps_coverage and ps_nullset
    string nullvalue = "'0'";
    if (nbands > 1)
    {
        nullvalue = "'{";
        for (unsigned int b=0; b < nbands; ++b)
        {
            nullvalue += "0";
            if (b < nbands -1)
                nullvalue += ",";
        }
        nullvalue += "}'";
    }

    // insert the collection name
    if (!bUpdate)
    {
        query << "insert into ps_coverage (name, nulldefault, interpolationtypedefault," <<
              "nullresistancedefault, type) values ('" << coll << "', " << nullvalue
              << ", 5, 2, 'RectifiedGridCoverage')";
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
            PQclear(res);
            return 0;
        }
        NMDebug(<< endl);
        PQclear(res);
        query.str("");
    }


    // get the image id of the collection
    query << "select id from ps_coverage where name = '" << coll << "'";
    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << PQresultErrorMessage(res));
        PQclear(res);
        return 0;
    }
    NMDebug(<< "done!" << endl);
    image_id = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    query.str("");


    // set the cell domain (i.e. the lower and upper pixel bounds of each dimension)
    /// ToDo: here I'm just using the first oid of the collection, is that always
    // the same oid for a particular collection or does it change?
    r_Minterval cellDom = this->getImageSdom(coll,
                          this->getImageOIDs(coll)[0]);
    columns << "(coverage, i, lo, hi)";
    for (int d=0; d < cellDom.dimension(); d++)
    {
        values << "(" << image_id << ", " << d << ", " <<
               cellDom[d].low() << ", " << cellDom[d].high() << ")";

        if (!bUpdate)
        {
            query << "insert into ps_celldomain " << columns.str() <<
                  " values " << values.str();
        }
        else
        {
            query << "update ps_celldomain set " << columns.str() << " = "
                  << values.str() << " where " <<
                  "coverage = " << image_id << " and i = " << d;
        }
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
        }
        else
            NMDebug(<< "done!" << endl);
        PQclear(res);
        query.str("");
        values.str("");
    }
    columns.str("");

    // set the geo domain (i.e. the bounds in terms of real world coordinate values)
    double mindom[3] = {xmin, ymin, zmin};
    double maxdom[3] = {xmax, ymax, zmax};
    int   typedom[3] = {1,2,5};
    char  namedom[3] = {'x','y','z'};
    columns << "(coverage, i, name, type, numLo, numHi)";
    for (int d=0; d < cellDom.dimension(); d++)
    {
        values << "(" << image_id << ", " << d << ", '" << namedom[d] << "', " <<
               typedom[d] << ", " << mindom[d] << ", " << maxdom[d] << ")";
        if (!bUpdate)
        {
            query << "insert into ps_domain " << columns.str() << " values " <<
                  values.str();
        }
        else
        {
            query << "update ps_domain set " << columns.str() << " = " << values.str() <<
                  " where coverage = " << image_id << " and i = " << d;
        }
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
        }
        else
            NMDebug(<< "done!" << endl);
        PQclear(res);
        query.str("");
        values.str("");
    }
    columns.str("");

    // defines the cell type
    if (!bUpdate)
    {
        for (int b=0; b < nbands; ++b)
        {
            int datatype = this->getWCPSTypeId(rtype); // the wcps data type
            columns << "(coverage, i, name, type)";
            values << "(" << image_id << ", " << b << ", '" << b << "', " << datatype << ")";
            query << "insert into ps_range " << columns.str() << " values " << values.str();
            NMDebugInd(1, << "'" << query.str() << "' ... ");
            res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                NMDebug(<< endl);
                NMErr(ctxrhelper, << PQresultErrorMessage(res));
            }
            else
                NMDebug(<< "done!" << endl);
            PQclear(res);
            query.str("");
            columns.str("");
            values.str("");
        }
    }

    // interpolation related information
    // since it isn't implemented yet in petascope, just default values for ensuring db consistency are set
    if (!bUpdate)
    {
        query << "insert into ps_interpolationset (coverage, interpolationType, nullResistance) values (" <<
              image_id << ", 5, 2)";
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
        }
        else
            NMDebug(<< "done!" << endl);
        PQclear(res);
        query.str("");
    }

    // null set
    if (!bUpdate)
    {
        query << "insert into ps_nullset (coverage, nullvalue) values (" << image_id << ", "
              << nullvalue << ")";
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
        }
        else
            NMDebug(<< "done!" << endl);
        PQclear(res);
        query.str("");
    }

    // setting the crs details
    columns << "(coverage, low1, high1, low2, high2)";
    values << "(" << image_id << ", " << xmin << ", " << xmax << ", " <<
           ymin << ", " << ymax << ")";
    if (!bUpdate)
    {
        query << "insert into ps_crsdetails " << columns.str() <<
              " values " << values.str();
    }
    else
    {
        query << "update ps_crsdetails set " << columns.str() << " = " << values.str() <<
              " where coverage = " << image_id;
    }
    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << PQresultErrorMessage(res));
    }
    else
        NMDebug(<< "done!" << endl);
    PQclear(res);
    query.str("");
    columns.str("");
    values.str("");

    // the rest of the operations are only relevant for first time insertion
    // but no for updating, so we quit here, if we're updating
    if (bUpdate)
    {
        NMDebugCtx(ctxrhelper, << "done!");
        return 1;
    }

    // get the id of the image's geo crs;
    int geoCrs_id = -1;
    query << "select id from ps_crs where name = '" << crs << "'";
    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) >= 1)
        geoCrs_id = atoi(PQgetvalue(res, 0, 0));
    NMDebug(<< "done!" << endl);
    PQclear(res);
    query.str("");

    // get the id of the pixel crs (we use that for all axes other than x and y, 'cause we
    // just don't know better at this stage)
    int pixCrs_id = -1;
    query.str("select id from ps_crs where name = 'CRS:1'");
    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << PQresultErrorMessage(res));
        NMDebugInd(1, << "... aborting writing petascope meta data!" << endl);
        PQclear(res);
        return 0;
    }
    pixCrs_id = atoi(PQgetvalue(res, 0, 0));
    NMDebug(<< "done!" << endl);
    PQclear(res);
    query.str("");

    // get image axis ids
    std::vector<long> axisIds;
    for (int d=0; d < cellDom.dimension(); d++)
    {
        query << "select id from ps_domain where coverage = " << image_id
              << " and type = " << (d > 2 ? 5 : typedom[d]);
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQntuples(res) < 1)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
            PQclear(res);
            return 0;
        }
        axisIds.push_back(atoi(PQgetvalue(res, 0, 0)));
        PQclear(res);
        query.str("");
        NMDebug(<< "done!" << endl);
    }

    // set the crs-id of the image axis (we assume they're all identical, and that we are dealing
    // with x, and y axes!!)
    columns << "(axis, crs)";
    for (int d=0; d < cellDom.dimension(); d++)
    {
        values << "(" << axisIds[d] << ", "
               << (d < 2 ? (geoCrs_id > 0 ? geoCrs_id : pixCrs_id) : pixCrs_id) << ")";
        query << "insert into ps_crsset " << columns.str() << " values " << values.str();
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << PQresultErrorMessage(res));
        }
        else
            NMDebug(<< "done!" << endl);
        PQclear(res);
        query.str("");
        values.str("");
    }
    columns.str("");

    NMDebugCtx(ctxrhelper, << "done!");

    return 1;
}

int RasdamanHelper2::deletePSMetadata(std::string collname)
{
//  NMDebugCtx(ctxrhelper, << "...");

    // establish connection
    const PGconn* conn = this->m_pRasconn->getPetaConnection();
    if (PQstatus(conn) != CONNECTION_OK)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getPetaDbName() << "' failed!");
        return 0;
    }

    std::stringstream query;
    PGresult* res;

    query << "delete from ps_coverage where name = '" << collname << "'";
    NMDebugInd(1, << query.str() << " ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << "failed deleting entry for '" <<
              collname << "' in ps_coverage!");
    }
    else
        NMDebug(<< "done!" << std::endl);
    PQclear(res);

//  NMDebugCtx(ctxrhelper, << "done!");
    return 1;
}

int RasdamanHelper2::deleteNMMetadata(std::string collname, double oid)
{
//  NMDebugCtx(ctxrhelper, << "...");

    // establish connection
    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (PQstatus(conn) != CONNECTION_OK)
    {
        NMDebug(<< endl);
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        return 0;
    }

    std::stringstream query;
    PGresult* res;

    // get the image oid and the images' RAT
    if (oid == -1)
        query << "select img_id, attrtable_name from nm_meta where coll_name = '" << collname << "'";
    else
        query << "select img_id, attrtable_name from nm_meta where img_id = " << oid;

    NMDebugInd(1, << "'" << query.str() << "' ... ");
    res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    int nentries = PQntuples(res);
    if (nentries < 1)
    {
        NMDebug(<< endl);
        NMDebugInd(1, << "Nothing to be done. No entry for '"
                   << collname << " #" << oid << "' in nm_meta!" << std::endl);
        PQclear(res);
        return 1;
    }
    NMDebug(<< "done!" << endl);

    std::vector<std::string> tabnames;
    std::vector<long> oids;
    for (int r=0; r < nentries; r++)
    {
        oids.push_back(atoi(PQgetvalue(res, r ,0)));
        tabnames.push_back(PQgetvalue(res, r, 1));
    }
    PQclear(res);
    query.str("");

    // remove image meta data from nm_meta table
    for (int r=0; r < nentries; r++)
    {
        query << "delete from nm_meta where img_id = " << oids[r];
        NMDebugInd(1, << "'" << query.str() << "' ... ");
        res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            NMDebug(<< endl);
            NMErr(ctxrhelper, << "failed deleting nm_meta meta "
                  "data for '" << collname << " #" << oids[r] << "'!");
        }
        else
            NMDebug(<< "done!" << endl);
        PQclear(res);
        query.str("");
    }

    // if there is a raster attribute table available for this image, delete it...
    for (int r=0; r < nentries; r++)
    {
        if (!tabnames[r].empty())
        {
            query << "drop table " << tabnames[r];
            NMDebugInd(1, << "'" << query.str() << "' ... ");
            res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                NMDebug(<< endl);
                NMErr(ctxrhelper, << "failed deleting attribute table of '"
                      << collname << " #" << oids[r] << "'!");
            }
            else
                NMDebug(<< "done!" << endl);
            PQclear(res);
        }
    }

//  NMDebugCtx(ctxrhelper, << "done!");

    return 1;
}

std::string RasdamanHelper2::getNMMetaCrsName(long oid)
{
    NMDebugCtx(ctxrhelper, << "...");

    // if we can't find any crs description, we return the petascope descr for
    // the pixel-based crs
    std::string crs_name = "CRS:1";

    const PGconn* conn = this->m_pRasconn->getRasConnection();
    if (conn == 0)
    {
        NMErr(ctxrhelper, << "connection with '" << this->m_pRasconn->getRasDbName() << "' failed!");
        return crs_name;
    }

    // check, whether the nm_meta is installed at all
    if (!this->isNMMetaAvailable())
        return crs_name;

    std::stringstream query;
    query << "select crs_name " <<
          "from nm_meta where img_id = " << oid;
    PGresult* res = PQexec(const_cast<PGconn*>(conn), query.str().c_str());
    if (PQntuples(res) < 1)
    {
        NMDebugAI(<< "could not find oid #" << oid << " in nm_meta!" << std::endl);
        NMDebugCtx(ctxrhelper, << "done!");
        PQclear(res);
        return crs_name;
    }

    crs_name = PQgetvalue(res, 0, 0);
    NMDebugAI(<< "crs_name: " << crs_name << std::endl);

    NMDebugCtx(ctxrhelper, << "done!");
    return crs_name;
}

bool RasdamanHelper2::checkDbConnection(void)
{
    bool bhealthy = true;

    try
    {
        this->m_pRasconn->connect();
        this->m_transaction.begin();
        this->m_transaction.commit();
    }
    catch (r_Error& re)
    {
        bhealthy = false;
    }

    return bhealthy;
}

std::string RasdamanHelper2::getDataTypeString(r_Type::r_Type_Id type)
{
    string stype;
    switch (type)
    {
    case r_Type::CHAR:
    case r_Type::BOOL:
        stype = "unsigned char";
        break;
    case r_Type::ULONG:
        stype = "unsigned long";
        break;
    case r_Type::USHORT:
        stype = "unsinged short";
        break;
    case r_Type::LONG:
        stype = "long";
        break;
    case r_Type::SHORT:
        stype = "short";
        break;
    case r_Type::OCTET:
        stype = "signed char";
        break;
    case r_Type::DOUBLE:
        stype = "double";
        break;
    case r_Type::FLOAT:
        stype = "float";
        break;
    default:
        stype = "unknown";
        break;
    }
    return stype;
}

