package wcps.server.core;

import java.math.BigInteger;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

// This class implements WCPS metadata. For information on what each field means, see the WCPS standard. This class provides extensive error cheching and well as various utility functions.

class Metadata implements Cloneable {

    private static final DateFormat iso8601 = new SimpleDateFormat( "yyyy-MM-dd'T'HH:mm:ss" );

    private List<CellDomainElement> cellDomain;
    private String coverageName;
    private List<DomainElement> domain;
    private InterpolationMethod interpolationDefault;
    private Set<InterpolationMethod> interpolationSet;
    private String nullDefault;
    private Set<String> nullSet;
    private List<RangeElement> range;

    public Metadata( List<CellDomainElement> cellDomain, List<RangeElement> range, Set<String> nullSet, String nullDefault, Set<InterpolationMethod> interpolationSet, InterpolationMethod interpolationDefault, String coverageName, List<DomainElement> domain ) throws InvalidMetadataException {

        if( cellDomain == null || range == null || coverageName == null || nullSet == null || interpolationSet == null ) {
            throw new InvalidMetadataException( "Cell domain, range list, coverage name, null set, and interpolation set cannot be null" );
        }

        if( cellDomain.size() == 0 ) {
            throw new InvalidMetadataException( "Invalid cell domain: At least one element is required" );
        }
        this.cellDomain = cellDomain;

        if( range.size() == 0 ) {
            throw new InvalidMetadataException( "At least one range element is required" );
        }
        this.range = new ArrayList<RangeElement>( range.size() );
        Iterator<RangeElement> ir = range.iterator();
        while( ir.hasNext() ) {
            RangeElement next = ir.next();
            Iterator<RangeElement> j = this.range.iterator();
            while( j.hasNext() ) {
                if( j.next().getName().equals( next.getName() ) ) {
                    throw new InvalidMetadataException( "Duplicate range element name encountered" );
                }
            }
            this.range.add( next );
        }

        if( nullSet.size() == 0 ) { // TODO: check with Peter
            throw new InvalidMetadataException( "Invalid null set: At least one null value is required" );
        }
        if( nullDefault == null ) {
            throw new InvalidMetadataException( "Invalid null default: Null default cannot be null" );
        }
        if( !nullSet.contains( nullDefault ) ) {
            throw new InvalidMetadataException( "Invalid null default: Default null value " + nullDefault + " is not part of the null set" );
        }
        Iterator<String> ns = nullSet.iterator();
        while( ns.hasNext() ) {
            String nullVal = ns.next();
            List<String> nulls = SDU.str2string( nullVal );
            if( nulls.size() != range.size() ) {
                throw new InvalidMetadataException( "Invalid null value: " + nullVal + " must have " + range.size() + " axes according to the range specified for coverage " + coverageName );
            }
            Iterator<String> i = nulls.iterator();
            Iterator<RangeElement> j = range.iterator();
            while( j.hasNext() ) {
                RangeElement re = j.next();
                if( re.isBoolean() ) {
                    SDU.str2boolean( i.next() );
                }
                else if( re.isIntegral() ) {
                    SDU.str2integer( i.next() );
                }
                else if( re.isFloating() ) {
                    SDU.str2double( i.next() );
                }
                else if( re.isComplex() ) {
                    SDU.str2complex( i.next() );
                }
            }
        }
        this.nullSet = nullSet;
        this.nullDefault = nullDefault;

        if( interpolationSet.size() == 0 ) { // TODO: check with Peter
            throw new InvalidMetadataException( "Invalid interpolation set: At least one interpolation method is required" );
        }
        if( interpolationDefault == null ) {
            interpolationDefault = new InterpolationMethod( "none", "none" );
        }
        boolean defaultContainedInSet = false;
        Iterator<InterpolationMethod> is = interpolationSet.iterator();
        while( is.hasNext() ) {
            if( interpolationDefault.equals( is.next() ) ) {
                defaultContainedInSet = true;
            }
        }
        if( !defaultContainedInSet ) {
            throw new InvalidMetadataException( "Ivanlid interpolation default: Default interpolation method (" + interpolationDefault.getInterpolationType() + "," + interpolationDefault.getNullResistance() + ") is not part of the interpolation set" );
        }
        this.interpolationSet = interpolationSet;
        this.interpolationDefault = interpolationDefault;

        this.coverageName = coverageName;

        if( domain != null ) {
            if( domain.size() != cellDomain.size() ) {
                throw new InvalidMetadataException( "Domain and cell domain must have equal number of elements" );
            }
            this.domain = new ArrayList<DomainElement>( domain.size() );
            Iterator<DomainElement> i = domain.iterator();
            while( i.hasNext() ) {
                DomainElement next = i.next();
                Iterator<DomainElement> j = this.domain.iterator();
                while( j.hasNext() ) {
                    DomainElement previous = j.next();
                    if( previous.getName().equals( next.getName() ) ) {
                        throw new InvalidMetadataException( "Duplicate domain element name encountered" );
                    }
                    if( previous.getType().equals( "temporal" ) && next.getType().equals( "temporal" ) ) {
                        throw new InvalidMetadataException( "Domain can contain at most one temporal axis" );
                    }
                    if( previous.getType().equals( "elevation" ) && next.getType().equals( "elevation" ) ) {
                        throw new InvalidMetadataException( "Domain can contain at most one elevation axis" );
                    }
                    if( previous.getType().equals( "x" ) && next.getType().equals( "x" ) ) {
                        throw new InvalidMetadataException( "Domain can contain at most one x axis" );
                    }
                    if( previous.getType().equals( "y" ) && next.getType().equals( "y" ) ) {
                        throw new InvalidMetadataException( "Domain can contain at most one y axis" );
                    }
                    if( next.getType().equals( "x" ) ) {
                        boolean l = false;
                        Iterator<DomainElement> k = domain.iterator();
                        while( k.hasNext() ) {
                            if( k.next().getType().equals( "y" ) ) {
                                l = true;
                            }
                        }
                        if( l == false ) {
                            throw new InvalidMetadataException( "If domain contains a x axis, it must contain a y axis as well" );
                        }
                    }
                    else if( next.getType().equals( "y" ) ) {
                        boolean l = false;
                        Iterator<DomainElement> k = domain.iterator();
                        while( k.hasNext() ) {
                            if( k.next().getType().equals( "x" ) ) {
                                l = true;
                            }
                        }
                        if( l == false ) {
                            throw new InvalidMetadataException( "If domain contains a y axis, it must contain a x axis as well" );
                        }
                    }
                }
                this.domain.add( next );
            }
        }

    }

    public Metadata clone() {

        try {
            List<CellDomainElement> cd = new ArrayList<CellDomainElement>( cellDomain.size() );
            Iterator<CellDomainElement> i = cellDomain.iterator();
            while( i.hasNext() ) {
                cd.add( i.next().clone() );
            }

            List<RangeElement> r = new ArrayList<RangeElement>( range.size() );
            Iterator<RangeElement> j = range.iterator();
            while( j.hasNext() ) {
                r.add( j.next().clone() );
            }

            List<DomainElement> d = new ArrayList<DomainElement>( domain.size() );
            Iterator<DomainElement> k = domain.iterator();
            while( k.hasNext() ) {
                d.add( k.next().clone() );
            }

            Set<String> ns = new HashSet<String>( nullSet.size() );
            Iterator<String> l = nullSet.iterator();
            while( l.hasNext() ) {
                ns.add( new String( l.next() ) );
            }

            Set<InterpolationMethod> is = new HashSet<InterpolationMethod>( interpolationSet.size() );
            Iterator<InterpolationMethod> m = interpolationSet.iterator();
            while( m.hasNext() ) {
                is.add( m.next().clone() );
            }

            return new Metadata( cd, r, ns, new String( nullDefault ), is, interpolationDefault.clone(), new String( coverageName ), d );
        }
        catch( InvalidMetadataException ime ) {
            throw new RuntimeException( "Invalid metadata while cloning Metadata. This is a software bug in WCPS.", ime );
        }

    }


//     public CellDomainElement getCellDomain( int index ) {
//
//         return cellDomain.get( index );
//
//     }

     public Iterator<CellDomainElement> getCellDomainIterator() {

         return cellDomain.iterator();

     }

     public Iterator<DomainElement> getDomainIterator() {
	 return domain.iterator();
     }

     public int getDimension() {

         return cellDomain.size();

     }


//     public DomainElement getDomainByType( String type ) {
//
//         Iterator<DomainElement> i = domain.iterator();
//         DomainElement de;
//         while( i.hasNext() ) {
//             de = i.next();
//             if( de.getType().equals( type ) ) {
//                 return de;
//             }
//         }
//         return null;
//
//     }

     public int getDomainIndexByName( String name ) {

         Iterator<DomainElement> i = domain.iterator();
         for( int index = 0; i.hasNext(); index++ ) {
             if( i.next().getName().equals( name ) ) {
                 return index;
             }
         }
         return -1;
     }

//     public int getDomainIndexByType( String type ) {
//
//         Iterator<DomainElement> i = domain.iterator();
//         for( int index = 0; i.hasNext(); index++ ) {
//             if( i.next().getName().equals( type ) ) {
//                 return index;
//             }
//         }
//         return -1;
//
//     }

//     public String getDomainType( int index ) {
//
//         return domain.get( index ).getType();
//
//     }

        public String getNullDefault() {

            return nullDefault;

        }

        public Set<String> getNullSet() {

            return nullSet;

        }

//     public String getRangeType( int index ) {
//
//         return range.get( index ).getType();
//
//     }

//     public String getRangeType( String name ) {
//
//         String type = null;
//         Iterator<RangeElement> i = range.iterator();
//         RangeElement re;
//         while( i.hasNext() ) {
//             re = i.next();
//             if( re.getName().equals( name ) ) {
//                 type = re.getType();
//             }
//         }
//         return type;
//
//     }

//     public double getResolution( int i ) {
//
//         DomainElement de = domain.get( i );
//         Double deLo = de.getNumLo();
//         Double deHi = de.getNumHi();
//         if( deLo == null ) {
//             return -1;
//         }
//
//         CellDomainElement cde = cellDomain.get( i );
//         BigInteger cdeLo = cde.getLo();
//         BigInteger cdeHi = cde.getHi();
//
//         return (deHi - deLo) / (cdeHi.subtract( cdeLo ).doubleValue() + 1);
//
//     }

//     public boolean isCellDomainEqualTo( Metadata m ) {
//
//         if( m == null ) {
//             return false;
//         }
//
//         Iterator<CellDomainElement> i = cellDomain.iterator();
//         Iterator<CellDomainElement> j = m.cellDomain.iterator();
//         while( i.hasNext() && j.hasNext() ) {
//             if( !i.next().equals( j.next() ) ) {
//                 return false;
//             }
//         }
//         if( i.hasNext() || j.hasNext() ) {
//             return false;
//         }
//         return true;
//
//     }

//     public boolean isCrsValid( String crs ) {
//         return crs != null && (crs.equals( "" ) || crs.startsWith( "urn:ogc:def:crs:EPSG::" ));
//     }

//     public boolean isDomainEqualTo (Metadata m) {
//
//         if( m == null ) {
//             return false;
//         }
//
//         Iterator<DomainElement> i = domain.iterator();
//         Iterator<DomainElement> j = m.domain.iterator();
//         while( i.hasNext() && j.hasNext() ) {
//             if( !i.next().equals( j.next() ) ) {
//                 return false;
//             }
//         }
//         if( i.hasNext() || j.hasNext() ) {
//             return false;
//         }
//         return true;
//
//     }

//     public boolean isInterpolationMethodValid( String im ) {
//
//         return im != null && im.equals( "nearest neighbor" );
//
//     }

    public boolean isRangeBoolean() {

        Iterator<RangeElement> i = range.iterator();
        while( i.hasNext() ) {
            if( !i.next().isBoolean() ) {
                return false;
            }
        }
        return true;

    }

    public boolean isRangeComplex() {

        Iterator<RangeElement> i = range.iterator();
        while( i.hasNext() ) {
            if( !i.next().isComplex() ) {
                return false;
            }
        }
        return true;

    }

    public boolean isRangeIntegral() {

        Iterator<RangeElement> i = range.iterator();
        while( i.hasNext() ) {
            if( !i.next().isIntegral() ) {
                return false;
            }
        }
        return true;

    }

//     public boolean isRangeEqualTo (Metadata m) {
//
//         if( m == null ) {
//             return false;
//         }
//
//         Iterator<RangeElement> i = range.iterator();
//         Iterator<RangeElement> j = m.range.iterator();
//         while( i.hasNext() && j.hasNext() ) {
//             if( !i.next().equals( j.next() ) ) {
//                 return false;
//             }
//         }
//         if( i.hasNext() || j.hasNext() ) {
//             return false;
//         }
//         return true;
//
//     }

    public boolean isRangeFloating() {

        Iterator<RangeElement> i = range.iterator();
        while( i.hasNext() ) {
            if( !i.next().isFloating() ) {
                return false;
            }
        }
        return true;

    }

    public boolean isRangeNumeric() {

        Iterator<RangeElement> i = range.iterator();
        while( i.hasNext() ) {
            if( !i.next().isNumeric() ) {
                return false;
            }
        }
        return true;

    }

//     public boolean isResolutionEqualTo( Metadata m ) {
//
//         if( m == null || getDimension() != m.getDimension() ) {
//             return false;
//         }
//
//         Iterator<CellDomainElement> cdIterator1 = cellDomain.iterator();
//         Iterator<CellDomainElement> cdIterator2 = m.cellDomain.iterator();
//         Iterator<DomainElement> dIterator1 = domain.iterator();
//         Iterator<DomainElement> dIterator2 = m.domain.iterator();
//         double resolution1;
//         double resolution2;
//         CellDomainElement cde;
//         DomainElement de;
//         Double deLo;
//         Double deHi;
//         BigInteger cdeLo;
//         BigInteger cdeHi;
//         while( cdIterator1.hasNext() ) {
//             cde = cdIterator1.next();
//             de = dIterator1.next();
//             deLo = de.getNumLo();
//             deHi = de.getNumHi();
//             if( deLo == null ) {
//                 resolution1 = -1;
//             }
//             cdeLo = cde.getLo();
//             cdeHi = cde.getHi();
//             resolution1 = (deHi - deLo) / (cdeHi.subtract( cdeLo ).doubleValue() + 1);
//             cde = cdIterator1.next();
//             de = dIterator1.next();
//             deLo = de.getNumLo();
//             deHi = de.getNumHi();
//             if( deLo == null ) {
//                 resolution2 = -1;
//             }
//             cdeLo = cde.getLo();
//             cdeHi = cde.getHi();
//             resolution2 = (deHi - deLo) / (cdeHi.subtract( cdeLo ).doubleValue() + 1);
//             if( resolution1 != resolution2 ) {
//                 return false;
//             }
//         }
//         return true;
//
//     }

//     public boolean isTypeValid( String type ) {
//         return type != null && (type.equals( "char" ) || type.equals( "unsigned char" ) || type.equals( "short" ) || type.equals( "unsigned short" ) || type.equals( "int" ) || type.equals( "unsigned int" ) || type.equals( "long" ) || type.equals( "unsigned long" ) || type.equals( "float" ) || type.equals( "double" )); // TODO: fix
//     }

//     public void removeFromCellDomain( int index ) throws InvalidMetadataException {
//
//         cellDomain.remove( index );
//         if( cellDomain.size() == 0 ) {
//             throw new InvalidMetadataException( "Metadata transformation: Cell domain cannot be empty" );
//         }
//
//     }

//     public void removeFromDomain( int index ) throws InvalidMetadataException {
//
//         domain.remove( index );
//         if( domain.size() == 0 ) {
//             throw new InvalidMetadataException( "Metadata transformation: Domain cannot be empty" );
//         }
//
//     }

    public void setCoverageName( String coverageName ) throws InvalidMetadataException {

        if( coverageName == null ) {
            throw new InvalidMetadataException( "Metadata transformation: Coverage name cannot be null" );
        }
        this.coverageName = coverageName;

    }

//     public void setCrs( String crs ) throws InvalidMetadataException {
//
//         if( crs == null ) {
//             throw new InvalidMetadataException( "Metadata transformation: CRS cannot be null" );
//         }
//         if( !(crs.equals( "" ) || crs.startsWith( "urn:ogc:def:crs:EPSG::" )) ) {
//             throw new InvalidMetadataException( "Metadata transformation: Invalid CRS" );
//         }
//         this.crs = crs;
//
//     }

//     public void setInterpolationMethodList( List<String> interpolationMethodList ) throws InvalidMetadataException {
//
//         this.interpolationMethodList = interpolationMethodList;
//         Iterator<String> is = interpolationMethodList.iterator();
//         while( is.hasNext() ) {
//             String next = is.next();
//             if( !(next.equals( "nearest neighbor" ) || next.equals( "bilinear" ) || next.equals( "bicubic" ) || next.equals( "lost area" ) || next.equals( "barycentric" )) ) {
//                 throw new InvalidMetadataException( "Metadata transformation: Invalid interpolation method" );
//             }
//         }
//
//     }

//     public void setNullValue( String nullValue ) {
//
//         this.nullValue = nullValue;
//
//     }

    public void setRangeType( String type ) throws InvalidMetadataException {

        Iterator<RangeElement> i = range.iterator();
        while( i.hasNext() ) {
            i.next().setType( type );
        }

    }

//     public void setRangeType( List<String> types ) throws InvalidMetadataException {
//
//         if( types.size() != range.size() ) {
//             throw new InvalidMetadataException( "Invalid range type: New type has " + types.size() + " elements, but range has " + range.size() + " elements" );
//         }
//         Iterator<RangeElement> i = range.iterator();
//         Iterator<String> j = types.iterator();
//         while( i.hasNext() ) {
//             i.next().setType( j.next() );
//         }
//
//     }

//     public void updateCellDomain( int index, BigInteger lo, BigInteger hi ) throws InvalidMetadataException {
//
//         cellDomain.set( index, new CellDomainElement( lo, hi ) );
//
//     }

//     public void updateDomain( int index, Double numLo, Double numHi, String strLo, String strHi ) throws InvalidMetadataException {
//
//         DomainElement old = domain.get( index );
//         domain.set( index, new DomainElement( old.getName(), old.getType(), numLo, numHi, strLo, strHi ) );
//
//     }

    public void updateNulls( Set<String> nullSet, String nullDefault ) throws InvalidMetadataException {

        if( nullSet.size() == 0 ) { // TODO: check with Peter
            throw new InvalidMetadataException( "Invalid null set: At least one null value is required" );
        }
        if( nullDefault == null ) {
            nullDefault = "0"; // TODO: "false" for booleans, but check whether it is supported by rasdaman
        }
        if( !nullSet.contains( nullDefault ) ) {
            throw new InvalidMetadataException( "Invalid null default: Default null value " + nullDefault + " is not part of the null set" );
        }
        this.nullSet = nullSet;
        this.nullDefault = nullDefault;

    }

}
