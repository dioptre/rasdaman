package wcps.server.core;

import java.math.BigInteger;

// A coverage axis in pixel coordinates. See the WCPS standard.

class CellDomainElement implements Cloneable {

    private BigInteger hi;
    private BigInteger lo;

    public CellDomainElement( BigInteger lo, BigInteger hi ) throws InvalidMetadataException {

        if( lo == null || hi == null ) {
            throw new InvalidMetadataException( "Invalid cell domain element: Bounds may not be null" );
        }
        if( lo.compareTo( hi ) == 1 ) {
            throw new InvalidMetadataException( "Invalid cell domain element: Lower bound cannot be larger than upper bound" );
        }
        this.lo = lo;
        this.hi = hi;

    }

    public CellDomainElement clone() {

        try {
            return new CellDomainElement( BigInteger.ZERO.add( lo ), BigInteger.ZERO.add( hi ) );
        }
        catch( InvalidMetadataException ime ) {
            throw new RuntimeException( "Invalid metadata while cloning CellDomainElement. This is a software bug in WCPS.", ime );
        }

    }

    public boolean equals( CellDomainElement cde ) {

        return lo.equals( cde.lo ) && hi.equals( cde.hi );

    }

    public BigInteger getHi() {

        return hi;

    }

    public BigInteger getLo() {

        return lo;

    }

}
