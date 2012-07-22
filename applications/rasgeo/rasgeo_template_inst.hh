/*
    template instantiation for rmanman utilities

*/

#ifndef __RMANMAN_TEMPLATE_INST_
#define __RMANMAN_TEMPLATE_INST_

//// RASLIB INCLUDES ------------------------------------------------
#include <rasodmg/ref.hh>
#include <raslib/primitivetype.hh>
#include <raslib/pointtype.hh>

//// RASLIB INSTANTIATION ------------------------------------
template class r_Ref< r_Primitive_Type >;
template class r_Ref< r_Primitive >;
template class r_Ref< r_Point >;
template class r_Ref< r_Type >;
template class r_Ref< r_Char >;
template class r_Set< r_Ref < r_Marray<r_Char> > >;
template class r_Set< r_Ref < r_Marray<r_ULong> > >;
template class r_Set< r_Ref < r_Marray<r_UShort> > >;
template class r_Set< r_Ref < r_Marray<r_Long> > >;
template class r_Set< r_Ref < r_Marray<r_Short> > >;
template class r_Set< r_Ref < r_Marray<r_Octet> > >;
template class r_Set< r_Ref < r_Marray<r_Double> > >;
template class r_Set< r_Ref < r_Marray<r_Float> > >;

template class r_Marray< char>;
template class r_Ref< r_Marray < char > > ;

template class r_Marray<r_UShort>;
template class r_Marray<r_Long>;
template class r_Marray<r_Octet>;
template class r_Marray<r_Double>;
template class r_Marray<r_Float>;
template class r_Marray<r_Short>;

#endif
