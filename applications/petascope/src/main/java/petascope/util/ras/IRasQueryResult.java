package petascope.util.ras;

import rasj.RasGMArray;

/**
 * A rasdaman query result holder.
 * 
 * @author Dimitar Misev
 */
public interface IRasQueryResult {

    /**
     * @return the original result
     */
    Object getResult();

    /**
     * Inspect the original result, by executing the relevant methods bellow (mdd, scalar, integer, oid)
     */
    void inspect();

    /**
     * Executed when the result contains mdds
     */
    void mdd(RasGMArray res);

    /**
     * Executed when the result contains scalars
     */
    void scalar(Object res);
}
