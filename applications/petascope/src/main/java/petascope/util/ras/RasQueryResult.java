package petascope.util.ras;

import java.util.ArrayList;
import java.util.List;
import rasj.RasGMArray;

/**
 * Simple implementation which stores the mdds/scalars from the result as lists.
 *
 * @author Dimitar Misev
 */
public class RasQueryResult extends AbstractRasQueryResult {
    
    private final List<String> scalars;
    private final List<byte[]> mdds;

    public RasQueryResult(Object result) {
        super(result);
        scalars = new ArrayList<String>();
        mdds = new ArrayList<byte[]>();
        inspect();
    }

    public void mdd(RasGMArray res) {
        mdds.add(res.getArray());
    }

    public void scalar(Object res) {
        scalars.add(res.toString());
    }

    public List<byte[]> getMdds() {
        return mdds;
    }

    public List<String> getScalars() {
        return scalars;
    }
}
