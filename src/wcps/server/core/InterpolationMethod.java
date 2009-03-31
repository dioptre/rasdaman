package wcps.server.core;

// A pair of an interpolation type and a null resistance. See the WCPS standard for an explanation of these.

class InterpolationMethod implements Cloneable {

    private String interpolationType;
    private String nullResistance;

    public InterpolationMethod( String interpolationType, String nullResistance ) throws InvalidMetadataException {

        if( interpolationType == null || !(interpolationType.equals( "nearest" ) || interpolationType.equals( "linear" ) || interpolationType.equals( "cubic" ) || interpolationType.equals( "quadratic" ) || interpolationType.equals( "none" )) ) {
            throw new InvalidMetadataException( "Invalid interpolation method: " + interpolationType + " is not a legal interpolation type" );
        }
        this.interpolationType = interpolationType;

        if( nullResistance == null || !(nullResistance.equals( "full" ) || nullResistance.equals( "none" ) || nullResistance.equals( "half" ) || nullResistance.equals( "other" )) ) {
            throw new InvalidMetadataException( "Invalid interpolation method: " + nullResistance + " is not a legal null resistance" );
        }
        this.nullResistance = nullResistance;

    }

    public InterpolationMethod clone() {

        try {
            return new InterpolationMethod( interpolationType, nullResistance );
        }
        catch( InvalidMetadataException ime ) {
            throw new RuntimeException( "Invalid metadata while cloning InterpolationMethod. This is a software bug in WCPS.", ime );
        }

    }

    public boolean equals( InterpolationMethod im ) {

        return interpolationType.equals( im.interpolationType ) && nullResistance.equals( im.nullResistance );

    }

    public String getInterpolationType() {

        return interpolationType;

    }

    public String getNullResistance() {

        return nullResistance;

    }

}
