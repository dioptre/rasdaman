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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
*/

// This version was created based on rview. Let's see if the other programs compile
// also.

#ifndef _TEMPLATE_INST_RASLIB_
#define _TEMPLATE_INST_RASLIB_

#include <vector>

#include <raslib/attribute.hh>
#include <raslib/itertype.hh>
#include <raslib/dlist.hh>
#include <raslib/minterval.hh>

#include <rasodmg/tiling.hh>
#include <rasodmg/stattiling.hh>
#include <rasodmg/iterator.hh>
#include <rasodmg/ref.hh>
#include <rasodmg/object.hh>
#include <rasodmg/set.hh>
#include <rasodmg/collection.hh>
#include <rasodmg/gmarray.hh>
#include <rasodmg/transaction.hh>
#include <rasodmg/marray.hh>
#include <rasodmg/dirdecompose.hh>

template class r_Ref<r_Object>;
template class r_Ref<r_Minterval>;
template class r_Collection<r_Transaction::GenRefElement *>;
template class r_Set<r_Transaction::GenRefElement *>;
template class r_Iterator<r_GMarray *>;
template class r_Iterator<r_Ref<r_GMarray > >;
template class r_Collection<r_Ref<r_GMarray> >;
template class r_Collection<r_GMarray *>;
template class r_Set<r_GMarray *>;
template class r_Iterator<r_Ref<r_Object> >;
template class r_IterType<r_Attribute>;
template class r_Collection<r_Ref<r_Object> >;
template class r_Set<r_Ref<r_Object> >;
template class r_Iterator<r_Ref_Any>;
template class r_Ref<r_GMarray>;
template class r_Collection<r_Ref_Any>;
template class std::vector<r_Minterval>;
template class r_Iterator<r_Transaction::GenRefElement *>;
template class r_Set<r_Ref<r_GMarray> >;
template class r_Ref<r_Set<r_Ref<r_GMarray> > >;
template class r_Set<r_Ref_Any>;
template class r_Marray<r_ULong>;
template class r_Marray<r_Char>;

template std::ostream& operator << (std::ostream& os, const std::vector<r_Minterval>& list);
template std::ostream& operator << (std::ostream& os, const std::vector<r_Dir_Decompose>& list);
template std::ostream& operator << (std::ostream& os, const std::vector<r_Access>& list);
template std::ostream& operator << (std::ostream& os, const std::vector<double>& list);

#endif
