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
/****************************************************************************
 *
 *
 * COMMENTS:
 *
 ****************************************************************************/

#include <rnprotocol.hh>
#include <assert.h>

using namespace rnp;
using namespace std;

#include "raslib/rminit.hh"
#include "debug.hh"


const RnpQuark Rnp::rnpProtocolId = 25112001;


const char* Rnp::endiannessNames[2]=
{
    "big endian", "little endian"
};

const char* Rnp::fragmentTypeNames[Rnp::fgt_HowMany]=
{
    "(fgt_none)", "Command", "OkAnswer", "ErrorAnswer"
};

const char* Rnp::dataTypeNames[Rnp::dtt_HowMany] =
{
    "(dtt_none)", "Asciiz", "Int32","Float","Double","Opaque","NullPtr"
};

const char* Rnp::errorTypeNames[Rnp::ert_HowMany] =
{
    "(ert_unknown)", "StlException", "OtherException"
};

const char* Rnp::errorParamNames[Rnp::erp_HowMany] =
{
    "(erp_none)", "StlWhatValue"
};

const char* Rnp::undefValue = "(undef)";

const char* Rnp::getFragmentTypeName(RnpQuark fType) throw()
{
    if(0<= fType && fType < fgt_HowMany) return fragmentTypeNames[fType];
    return undefValue;
}

const char* Rnp::getDataTypeName(RnpQuark dType) throw()
{
    if(0<= dType && dType < dtt_HowMany) return dataTypeNames[dType];
    return undefValue;
}

const char* Rnp::getErrorTypeName(RnpQuark eType) throw()
{
    if(0<= eType && eType < ert_HowMany) return errorTypeNames[eType];
    return undefValue;
}

const char* Rnp::getErrorParamName(RnpQuark eName) throw()
{
    if(0<= eName && eName < erp_HowMany) return errorParamNames[eName];
    return undefValue;
}

const char* Rnp::getEndiannessName(Rnp::Endianness endianness) throw()
{
    return endiannessNames[endianness];
}

Rnp::Endianness Rnp::detectHostEndianness() throw()
{
    unsigned int uInteger = 1;

    char* ptr = (char*)&uInteger;

    return *ptr==1 ? littleEndian : bigEndian;
}

RnpQuark Rnp::swapBytes(RnpQuark orig) throw()
{
    RnpQuark result = orig;

    char *buf = (char*)&result;

    char tmp = buf[0];
    buf[0] = buf[3];
    buf[3] = tmp;

    tmp    = buf[1];
    buf[1] = buf[2];
    buf[2] = tmp;

    return result;
}

//############## RNP Header #################################

bool RnpHeader::isRnpMessage() const throw()
{
    Rnp::Endianness hostEndianness = Rnp::detectHostEndianness();

    RnpQuark cProtocolId = hostEndianness == Rnp::littleEndian ? protocolId : Rnp::swapBytes(protocolId);

    return cProtocolId == Rnp::rnpProtocolId ? true:false;
}

Rnp::Endianness RnpHeader::getEndianness() const throw()
{
    return (Rnp::Endianness)messageEndianness;
}

RnpQuark RnpHeader::getTotalLength() const throw()
{
    Rnp::Endianness endianness = (Rnp::Endianness)messageEndianness;
    return endianness == Rnp::detectHostEndianness() ? totalMessageLength : Rnp::swapBytes(totalMessageLength);
}

bool RnpHeader::changeEndianness(Rnp::Endianness newEndianness) throw()
{
    Rnp::Endianness endianness = (Rnp::Endianness)messageEndianness;

    if(newEndianness == endianness) return false; // no change necessary

    messageEndianness = newEndianness;

    totalMessageLength = Rnp::swapBytes(totalMessageLength);
    nrFragments        = Rnp::swapBytes(nrFragments);
    serverType         = Rnp::swapBytes(serverType);
    authInfoStart      = Rnp::swapBytes(authInfoStart);
    authInfoLength     = Rnp::swapBytes(authInfoLength);
    comprInfoStart     = Rnp::swapBytes(comprInfoStart);
    comprInfoLength    = Rnp::swapBytes(comprInfoLength);
    dataStart          = Rnp::swapBytes(dataStart);
    dataLength         = Rnp::swapBytes(dataLength);

    return true;
}

RnpFragmentHeader* RnpHeader::getFirstFragment() const throw()
{
    return (RnpFragmentHeader*)((char*)this + dataStart);
}

//############ RNP Fragment ##################################

RnpFragmentHeader* RnpFragmentHeader::getNextFragment() const throw()
{
    char *ptr = (char*)this;

    return (RnpFragmentHeader*)(ptr + totalLength);
}

RnpParameter* RnpFragmentHeader::getFirstParameter() const throw()
{
    return (RnpParameter*)(this+1);
}

void RnpFragmentHeader::changeEndianness() throw()
{
    // there is no info about the initial endianness so be carefull
    fragmType   = Rnp::swapBytes(fragmType);
    command     = Rnp::swapBytes(command);
    nrParams    = Rnp::swapBytes(nrParams);
    totalLength = Rnp::swapBytes(totalLength);
}

//############ RNP Parameter ##################################
RnpParameter* RnpParameter::getNextParameter() const throw()
{
    char *ptr = (char*)this;
    return (RnpParameter*)(ptr + totalLength);
}

void* RnpParameter::getData() const throw()
{
    return (void*)(this+1);
}

RnpQuark RnpParameter::getDataLength() const throw()
{
    return dataLength;
}

RnpQuark RnpParameter::computeTotalAlignedLength() throw()
{
    totalLength = (sizeof(RnpParameter) + dataLength + 3) & 0xFFFFFFFC;
    return totalLength;
}

RnpQuark RnpParameter::getPaddLength() const throw()
{
    return totalLength - (sizeof(RnpParameter) + dataLength);
}

void RnpParameter::changeToHostEndianness() throw()
{
    // there is no info about the initial endianness so be carefull
    paramType   = Rnp::swapBytes(paramType);
    dataType    = Rnp::swapBytes(dataType);
    dataLength  = Rnp::swapBytes(dataLength);
    totalLength = Rnp::swapBytes(totalLength);

    RnpQuark *valPtr = (RnpQuark*)getData();
    switch(dataType)
    {
    case Rnp::dtt_Int32:
    case Rnp::dtt_Float32:
        *valPtr = Rnp::swapBytes(*valPtr);
        break;

    case Rnp::dtt_Double64:
        RnpQuark temp = Rnp::swapBytes(*valPtr);
        *valPtr       = Rnp::swapBytes(*(valPtr +1));
        *(valPtr+1)   = temp;
        break;
    }
}

void RnpParameter::changeToPartnerEndianness() throw()
{
    // there is no info about the initial endianness so be careful

    RnpQuark *valPtr = (RnpQuark*)getData();
    switch(dataType)
    {
    case Rnp::dtt_Int32:
    case Rnp::dtt_Float32:
        *valPtr = Rnp::swapBytes(*valPtr);
        break;

    case Rnp::dtt_Double64:
        RnpQuark temp = Rnp::swapBytes(*valPtr);
        *valPtr       = Rnp::swapBytes(*(valPtr +1));
        *(valPtr+1)   = temp;
        break;
    }
    paramType   = Rnp::swapBytes(paramType);
    dataType    = Rnp::swapBytes(dataType);
    dataLength  = Rnp::swapBytes(dataLength);
    totalLength = Rnp::swapBytes(totalLength);
}

//###########################################################

RnpProtocolEncoder::RnpProtocolEncoder() throw()
{
    commBuffer    = NULL;

    rnpHeader     = NULL;
    currFragment  = NULL;
    currParameter = NULL;
    allocated     = false;
    carrierHeaderSize = 0;
    finalEndianness = Rnp::detectHostEndianness();
}

RnpProtocolEncoder::~RnpProtocolEncoder() throw()
{
    if(commBuffer != NULL && allocated == true) delete commBuffer;
    // the other pointers are not pointing to allocated memory!!
}

void RnpProtocolEncoder::setBuffer(akg::CommBuffer* buffer) throw()
{
    if(commBuffer != NULL && allocated == true) delete commBuffer;

    commBuffer = buffer;
    allocated  = false;
}

bool RnpProtocolEncoder::allocateBuffer(int maxMessageLength) throw()
{
    if(commBuffer != NULL && allocated ==true) delete commBuffer;

    commBuffer = new akg::CommBuffer(maxMessageLength);
    allocated = true;
    return true;
}

bool RnpProtocolEncoder::adjustBufferSize(int differenceSize) throw()
{
    ENTER( "RnpProtocolEncoder::adjustBufferSize( differenceSize=" << differenceSize << " )" );

    if (commBuffer == 0)
    {
        TALK( "RnpProtocolEncoder::adjustBufferSize(): warning: null commBuffer, assert would fire." );
        return false;
    }
    assert(commBuffer != 0);

    if (differenceSize <= 0)
    {
        TALK( "RnpProtocolEncoder::adjustBufferSize(): warning: nonpositive differenceSize, assert would fire." );
        return false;
    }
    assert(differenceSize > 0);

    // we need to adjust the pointers to the new location
    char *orig = (char*)(commBuffer->getData());
    char *head = (char*)rnpHeader;
    char *frag = (char*)currFragment;
    char *para = (char*)currParameter;

    if(commBuffer->resize(commBuffer->getDataSize() + differenceSize + RNP_DEFAULTBUFFERSIZE) == true)
    {
        char *final  = (char*)(commBuffer->getData());
        rnpHeader    = (RnpHeader*)(final + (head - orig));
        currFragment = (RnpFragmentHeader*)(final + (frag - orig));
        currParameter= (RnpParameter*)(final + (para - orig));
        LEAVE( "RnpProtocolEncoder::adjustBufferSize() -> true" );
        return true;
    }

    LEAVE( "RnpProtocolEncoder::adjustBufferSize() -> false" );
    return false;
}

int RnpProtocolEncoder::getBufferSize() throw()
{
    if (commBuffer == 0)
    {
        TALK( "RnpProtocolEncoder::getBufferSize(): warning: null commBuffer, assert will fire." );
    }
    assert(commBuffer != 0);

    return commBuffer->getBufferSize();
}

void RnpProtocolEncoder::startMessage(RnpQuark serverType, int nCarrierHeaderSize) throw()
{
    if (commBuffer == 0)
    {
        TALK( "RnpProtocolEncoder::startMessage(): warning: null commBuffer, assert will fire." );
    }
    assert(commBuffer != NULL);

    carrierHeaderSize = nCarrierHeaderSize;

    commBuffer->clearToRead();

    commBuffer->reserve(sizeof(RnpHeader) + carrierHeaderSize);

    rnpHeader = (RnpHeader*)((char*)commBuffer->getData() + carrierHeaderSize);

    Rnp::Endianness hostEndianness = Rnp::detectHostEndianness();

    // the protocolID is always 25112001 little endian!!
    rnpHeader->protocolId         = hostEndianness == Rnp::littleEndian ? Rnp::rnpProtocolId : Rnp::swapBytes(Rnp::rnpProtocolId);

    rnpHeader->messageEndianness  = hostEndianness;
    rnpHeader->desiredEndianness  = hostEndianness;
    rnpHeader->majorVersion       = 1;
    rnpHeader->minorVersion       = 0;
    rnpHeader->totalMessageLength = sizeof(RnpHeader);
    rnpHeader->nrFragments        = 0;
    rnpHeader->serverType         = serverType;
    rnpHeader->authInfoStart      = 0;
    rnpHeader->authInfoLength     = 0;
    rnpHeader->comprInfoStart     = 0;
    rnpHeader->comprInfoLength    = 0;
    rnpHeader->dataStart          = sizeof(RnpHeader);
    rnpHeader->dataLength         = 0;

    currFragment = (RnpFragmentHeader*)(rnpHeader+1);

}

int RnpProtocolEncoder::getCarrierHeaderSize() throw()
{
    return carrierHeaderSize;
}

void RnpProtocolEncoder::setDesiredEndianness(Rnp::Endianness desiredEndianness) throw()
{
    rnpHeader->desiredEndianness  = desiredEndianness;
}
void RnpProtocolEncoder::setFinalEndianness(Rnp::Endianness endianness) throw()
{
    finalEndianness = endianness;
}

void RnpProtocolEncoder::startFragment(Rnp::FragmentType fType,RnpQuark command) throw()
{
    commBuffer->reserve(sizeof(RnpFragmentHeader));

    currFragment->fragmType   = fType;
    currFragment->command     = command;
    currFragment->nrParams    = 0;
    currFragment->totalLength = sizeof(RnpFragmentHeader);

    currParameter = currFragment->getFirstParameter();
    rnpHeader->nrFragments++;

}

void RnpProtocolEncoder::addStringParameter(RnpQuark parameterType,const char* str) throw()
{
    if(str != 0) addParameter(parameterType, Rnp::dtt_Asciiz, str, strlen(str)+1);
    else         addParameter(parameterType, Rnp::dtt_NullPtr, str, 0);
}

void RnpProtocolEncoder::addInt32Parameter(RnpQuark parameterType, int par) throw()
{
    addParameter(parameterType, Rnp::dtt_Int32, &par, sizeof(par));
}

void RnpProtocolEncoder::addFloat32Parameter(RnpQuark parameterType, float par) throw()
{
    addParameter(parameterType, Rnp::dtt_Float32, &par, sizeof(par));
}
void RnpProtocolEncoder::addDouble64Parameter(RnpQuark parameterType, double par) throw()
{
    addParameter(parameterType, Rnp::dtt_Double64, &par, sizeof(par));
}

void RnpProtocolEncoder::addOpaqueParameter(RnpQuark parameterType, const void *buf, int size) throw()
{
    if(buf != 0)  addParameter(parameterType, Rnp::dtt_Opaque, buf, size);
    else          addParameter(parameterType, Rnp::dtt_NullPtr, buf, 0);
}

void RnpProtocolEncoder::addParameter(RnpQuark parameterType, Rnp::DataType dtt, const void *data, int length) throw()
{
    commBuffer->reserve(sizeof(RnpParameter));

    currParameter->paramType   = parameterType;
    currParameter->dataType    = dtt;
    currParameter->dataLength  = length;
    currParameter->totalLength = currParameter->computeTotalAlignedLength();

    int paddlen = currParameter->getPaddLength();

    if(data != 0 ) commBuffer->read(data,length);
    if(paddlen)    commBuffer->reserve(paddlen);

    currFragment->nrParams++;
    currFragment->totalLength += currParameter->totalLength;
    currParameter = currParameter->getNextParameter();
}

void RnpProtocolEncoder::endFragment() throw()
{
    rnpHeader->totalMessageLength += currFragment->totalLength;
    rnpHeader->dataLength         += currFragment->totalLength;
    currFragment = currFragment->getNextFragment();
}

akg::CommBuffer* RnpProtocolEncoder::endMessage() throw()
{
    changeToPartnerEndianness(finalEndianness);
    return commBuffer;
}

bool RnpProtocolEncoder::changeToPartnerEndianness(Rnp::Endianness newEndianness) throw()
{
    if(newEndianness == rnpHeader->getEndianness()) return false;
    // so newEndianness is the same as the message endiannes, no change necessary

    // don't forget that the endianness is now the host endianness so,
    // after changed, the data can't be used correctly any more!!

    RnpFragmentHeader* lCurrFragment = rnpHeader->getFirstFragment();

    for(int fragment=0; fragment < rnpHeader->nrFragments; fragment++)
    {

        RnpParameter* lCurrParameter = lCurrFragment->getFirstParameter();
        for(int parameter = 0; parameter < lCurrFragment->nrParams; parameter++)
        {
            RnpParameter* nextParameter = lCurrParameter->getNextParameter();
            lCurrParameter->changeToPartnerEndianness();
            lCurrParameter = nextParameter;
        }

        RnpFragmentHeader* nextFragment = lCurrFragment->getNextFragment();
        lCurrFragment->changeEndianness();
        lCurrFragment = nextFragment;
    }

    rnpHeader->changeEndianness(newEndianness);

    return true;
}

//###### DECODER #######################

RnpProtocolDecoder::RnpProtocolDecoder() throw()
{
    // this memory doesn't belong to us, so do not deallocate!!
    commBuffer    = NULL;
    rnpHeader     = NULL;
    currFragment  = NULL;
    currParameter = NULL;
}

bool RnpProtocolDecoder::decode(akg::CommBuffer *buffer) throw()
{
    // Later, throw something intelligible!
    commBuffer = buffer;

    rnpHeader = (RnpHeader*)commBuffer->getData();

    if(rnpHeader->isRnpMessage() == false) return false;

    originalEndianness = rnpHeader->getEndianness();

    if(originalEndianness == Rnp::detectHostEndianness())
    {
        // -- test validity of message --
        if(testIntegrity() == false) return false;
    }
    else
    {
        // -- endianess of message    --
        changeToHostEndianness();
    }
    return true;
}

bool RnpProtocolDecoder::testIntegrity() const throw()
{
    // could be done better...
    ENTER( "RnpProtocolDecoder::testIntegrity()" );

    if(rnpHeader->isRnpMessage() == false)
    {
        RMInit::logOut << "Communication error: received invalid RNP header." << endl;
        return false;
    }

    bool ok = true;
    char *endOfMessage = (char*)rnpHeader + rnpHeader->getTotalLength();
    char *endOfHeader  = (char*)rnpHeader + sizeof(RnpHeader);
    int   maxLength    = commBuffer->getDataSize() - sizeof(RnpHeader);
    // max of every length

    RnpFragmentHeader* lCurrFragment = (RnpFragmentHeader*)getFirstFragment();
    for(int fragment=0; fragment < countFragments(); fragment++)
    {
        if( endOfHeader <= (char*)lCurrFragment && (char*)lCurrFragment < endOfMessage)
            ;
        else
        {
            ok = false;
            RMInit::logOut << "Communication error: RNP message corrupt: short header." << endl;
            break;
        }

        if(lCurrFragment->totalLength > maxLength)
        {
            ok = false;
            RMInit::logOut << "Communication error: RNP message corrupt: actual length (" << lCurrFragment->totalLength << ") larger than foreseen (" << maxLength << ")." << endl;
            break;
        }

        char *startOfParameters = (char*)lCurrFragment + sizeof(RnpFragmentHeader);
        char *endOfParameters   = (char*)lCurrFragment + lCurrFragment->totalLength;

        RnpParameter* lCurrParameter = (RnpParameter*)getFirstParameter();
        for(int parameter = 0; parameter < countParameters(); parameter++)
        {
            if( startOfParameters <= (char*)lCurrParameter && (char*)lCurrParameter < endOfParameters)
                ;
            else
            {
                ok = false;
                RMInit::logOut << "Communication error: RNP message corrupt: current parameter location outside parameter area." << endl;
                break;
            }

            if(lCurrParameter->totalLength > lCurrFragment->totalLength)
            {
                ok = false;
                RMInit::logOut << "Communication error: RNP message corrupt: current parameter length (" << lCurrParameter->totalLength << ") larger than total fragment size (" << lCurrFragment->totalLength << ")." << endl;
                break;
            }

            lCurrParameter = (RnpParameter*)getNextParameter();
        }

        if( (char*)lCurrParameter != endOfParameters)
        {
            ok = false;
// the counting seems to differ from the protocol specs;
// to avoid log flooding we disable it for the moment being -- PB 2005-aug-28
#ifdef RMANDEBUG
            RMInit::logOut << "Communication warning: puzzled by message: parameter count too small, found extra parameter(s). (this message can be ignored)" << endl;
#endif
        }

        if(ok == false)
            break;

        // we found a valid fragment, proceed to next one
        lCurrFragment = (RnpFragmentHeader*)getNextFragment();
    }

    LEAVE( "RnpProtocolDecoder::testIntegrity() -> " << ok );
    return ok;
}

bool RnpProtocolDecoder::changeToHostEndianness() throw()
{
    if(rnpHeader->changeEndianness(Rnp::detectHostEndianness()) == false ) return false;
    // so host endianness is the same as the message endiannes, no change necessary

    currFragment = (RnpFragmentHeader*)getFirstFragment();

    for(int fragment=0; fragment < countFragments(); fragment++)
    {
        currFragment->changeEndianness();

        currParameter = (RnpParameter*)getFirstParameter();
        for(int parameter = 0; parameter < countParameters(); parameter++)
        {
            currParameter->changeToHostEndianness();

            currParameter = (RnpParameter*)getNextParameter();
        }

        currFragment = (RnpFragmentHeader*)getNextFragment();
    }
    return true;
}

RnpQuark RnpProtocolDecoder::getDestinationServerType() const throw()
{
    return rnpHeader->serverType;
}

Rnp::Endianness RnpProtocolDecoder::getDesiredEndianness() const throw()
{
    return (Rnp::Endianness)rnpHeader->desiredEndianness;
}

Rnp::Endianness RnpProtocolDecoder::getOriginalEndianness() const throw()
{
    return originalEndianness;
}

int RnpProtocolDecoder::getMessageLength() const throw()
{
    return rnpHeader->totalMessageLength;
}

int RnpProtocolDecoder::getMessageVersion() const throw()
{
    return 1000*rnpHeader->majorVersion + rnpHeader->minorVersion;
}

RnpQuark RnpProtocolDecoder::countFragments() const throw()
{
    return rnpHeader->nrFragments;
}

const RnpFragmentHeader* RnpProtocolDecoder::getFirstFragment() const throw()
{
    currFragmentIdx = 0;

    return currFragment = (currFragmentIdx < rnpHeader->nrFragments ? rnpHeader->getFirstFragment() : 0);
}

const RnpFragmentHeader* RnpProtocolDecoder::getNextFragment() const throw()
{
    currFragmentIdx++;

    return currFragment = (currFragmentIdx < rnpHeader->nrFragments ? currFragment->getNextFragment() : 0);
}

RnpQuark RnpProtocolDecoder::getFragmentType() const throw()
{
    return currFragment->fragmType;
}

const char* RnpProtocolDecoder::getFragmentTypeName() const throw()
{
    return Rnp::getFragmentTypeName(currFragment->fragmType);
}
RnpQuark RnpProtocolDecoder::getCommand() const throw()
{
    return currFragment->command;
}

int RnpProtocolDecoder::countParameters() const throw()
{
    return currFragment->nrParams;
}

RnpQuark RnpProtocolDecoder::getFragmentLength() const throw()
{
    return currFragment->totalLength;
}

const RnpParameter* RnpProtocolDecoder::getFirstParameter() const throw()
{
    currParameterIdx = 0;

    return currParameter = ( currParameterIdx < currFragment->nrParams ? currFragment->getFirstParameter() : 0);
}

const RnpParameter* RnpProtocolDecoder::getNextParameter() const throw()
{
    currParameterIdx++;

    return currParameter = ( currParameterIdx < currFragment->nrParams ? currParameter->getNextParameter() : 0);
}

RnpQuark  RnpProtocolDecoder::getParameterType() const throw()
{
    return currParameter->paramType;
}

RnpQuark  RnpProtocolDecoder::getDataType() const throw()
{
    return currParameter->dataType;
}

const void* RnpProtocolDecoder::getData() const throw()
{
    return currParameter->getData();
}

const char* RnpProtocolDecoder::getDataAsString() const throw()
{
    if ( !( currParameter->dataType == Rnp::dtt_Asciiz || currParameter->dataType == Rnp::dtt_NullPtr) )
    {
        TALK( "RnpProtocolEncoder::getDataAsString(): warning: assert will fire." );
    }
    assert(currParameter->dataType == Rnp::dtt_Asciiz || currParameter->dataType == Rnp::dtt_NullPtr);

    return currParameter->dataType == Rnp::dtt_Asciiz ? (const char*)currParameter->getData() : (const char*)0;
}

int RnpProtocolDecoder::getDataAsInteger() const throw()
{
    if ( !( currParameter->dataType == Rnp::dtt_Int32 ) )
    {
        TALK( "RnpProtocolEncoder::getDataAsInteger(): warning: assert will fire." );
    }
    assert(currParameter->dataType == Rnp::dtt_Int32);

    return *(int*)currParameter->getData();
}

float RnpProtocolDecoder::getDataAsFloat() const throw()
{
    if ( !( currParameter->dataType == Rnp::dtt_Float32 ) )
    {
        TALK( "RnpProtocolEncoder::getDataAsFloat(): warning: assert will fire." );
    }
    assert(currParameter->dataType == Rnp::dtt_Float32);

    return *(float*)currParameter->getData();
}

double RnpProtocolDecoder::getDataAsDouble() const throw()
{
    if ( !( currParameter->dataType == Rnp::dtt_Double64 ) )
    {
        TALK( "RnpProtocolEncoder::getDataAsDouble(): warning: assert will fire." );
    }
    assert(currParameter->dataType == Rnp::dtt_Double64);

    return *(double*)currParameter->getData();
}

const void* RnpProtocolDecoder::getDataAsOpaque() const throw()
{
    if ( !( currParameter->dataType == Rnp::dtt_Opaque || currParameter->dataType == Rnp::dtt_NullPtr) )
    {
        TALK( "RnpProtocolEncoder::getDataAsOpaque(): warning: assert will fire." );
    }
    assert(currParameter->dataType == Rnp::dtt_Opaque || currParameter->dataType == Rnp::dtt_NullPtr);

    return currParameter->dataType == Rnp::dtt_Opaque ? (const void*)currParameter->getData() : (const void*)0;
}

int RnpProtocolDecoder::getDataLength() const throw()
{
    return currParameter->getDataLength();
}

void RnpProtocolDecoder::printRnpHeader(RnpHeader *lRnpHeader) const throw()
{
    cout<<"RnpHeader ID="<<lRnpHeader->protocolId<<endl;
    cout<<"  total fragments="<<lRnpHeader->nrFragments<<endl;
    cout<<"  sizeof header="<<sizeof(RnpHeader)<<endl;
    cout<<endl;
}

bool RnpProtocolDecoder::isRnpMessage() const throw()
{
    return rnpHeader->isRnpMessage();
}

