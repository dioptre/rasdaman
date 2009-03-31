package grammar;

/**
 * ComplexConst
 * @author: Andrei Aiordachioaie
 */
public class ComplexConst
{
    String re, im;

    /**
     * ComplexLit constructor comment.
     */
    public ComplexConst()
    {
        super();
    }

    public ComplexConst(String val)
    {
        int pos = val.indexOf("+i", 0);

        if (pos != -1)
        {
            re = val.substring(0, pos - 1);
            im = val.substring(pos + 2, val.length());
        }
    }

    public String toXML()
    {
        return "<complexConstant><re>" + re + "</re><im>" + im + "</im></complexConstant";
    }
}


//~ Formatted by Jindent --- http://www.jindent.com
