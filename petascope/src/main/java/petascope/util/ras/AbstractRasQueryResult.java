package petascope.util.ras;

import org.odmg.DBag;
import rasj.RasGMArray;

/**
 * Provides a convenient implementation of {@link #inspect()}.
 *
 * @author Dimitar Misev
 */
public abstract class AbstractRasQueryResult implements IRasQueryResult {
    
    protected final Object result;
    /**
     * Prevent multiple inspections
     */
    private boolean inspected;

    public AbstractRasQueryResult(Object result) {
        this.result = result;
        this.inspected = false;
    }

    public Object getResult() {
        return result;
    }

    public void inspect() {
        if (inspected) {
            return;
        }
        if (result instanceof DBag) {
            DBag bag = (DBag) result;
            for (Object e : bag) {
                if (e instanceof RasGMArray) {
                    mdd((RasGMArray) e);
                } else {
                    scalar(e);
                }
            }
        } else if (result instanceof Integer) {
            scalar(result);
        }
        inspected = true;
    }
}
