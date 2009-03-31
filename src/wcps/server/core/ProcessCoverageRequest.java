package wcps.server.core;
import org.w3c.dom.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


import org.odmg.Database;
import org.odmg.DBag;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.QueryException;
import org.odmg.Transaction;
import org.xml.sax.SAXException;
import rasj.RasGMArray;
import rasj.RasImplementation;


// A ProcessCoverage request is just a list of rasdaman queries.

public class ProcessCoverageRequest {

    private ArrayList<CoverageIterator> iterators;
    private IRasNode where;
    private IRasNode coverageExpr;
    private MetadataSource source;
    private String url;
    private String database;
    private String mime;
    
    public ProcessCoverageRequest(String url, String database, Node node, MetadataSource source) throws WCPSException {
        super();	
	this.source = source;
	this.url = url;
	this.database = database;
	Node x = node.getFirstChild();
	if (!x.getNodeName().equals("processCoverage"))
	    throw new WCPSException("The document contains an unrecognized node : " + x.getNodeName());
	x = x.getFirstChild();
	iterators = new ArrayList<CoverageIterator>();
	while (x != null) {
	    if (x.getNodeName().equals("#text")) {
		x = x.getNextSibling();
		continue;
	    }
	    System.err.println("The current node is: " + x.getNodeName());
	    if (x.getNodeName().equals("coverageIterator")) {
		iterators.add(new CoverageIterator(x, this));
	    } else if (x.getNodeName().equals("where")) {
		where = new BooleanScalarExprType(x.getFirstChild(), this);	
	    } else if (x.getNodeName().equals("encode")) {
		EncodeDataExprType encode;
		encode = new EncodeDataExprType(x, this);
		coverageExpr = encode;
		mime = encode.getMime();		
	    } else {
		// It has to be a scalar Expr Type
		coverageExpr = new ScalarExprType(x, this);		
		mime = "text/plain";
	    }	     
	    x = x.getNextSibling();
	}			
    }


    public String getMime() {
	return mime;	    
    }

    public MetadataSource getMetadataSource() {
	return source;
    }
    
    public Boolean isIteratorDefined(String iteratorName) {
	Iterator<CoverageIterator> it = iterators.iterator();
	while (it.hasNext()) {
	    CoverageIterator tmp = it.next();
	    if (iteratorName.equals(tmp.getIteratorName())) {
		return true;
	    }
	}
	return false;
    }

    public String getQuery() {
	String result = "select " + coverageExpr.toRasQL() + " from ";
	Iterator<CoverageIterator> it = iterators.iterator();
    boolean first = true;
	while (it.hasNext()) {
        if (first) {
            first = false;
        } else {
            result += ", ";
        }
	    result += it.next().toRasQL();
    }
	if (where != null)
	    result += " where " + where.toRasQL();
	return result;
	
    }

    public Iterator<String> getCoverages(String iteratorName) throws WCPSException {
	for (int i = 0; i< iterators.size(); ++i) {
	    if (iterators.get(i).getIteratorName().equals(iteratorName)) {
		return iterators.get(i).getCoverages();
	    }
	}
	throw new WCPSException("Iterator " + iteratorName + " not defined");
    }

    public List<byte[]> execute() throws ResourceException {
	ArrayList<byte[]> results = new ArrayList<byte[]>();
	if (coverageExpr != null) {
            Implementation impl = new RasImplementation( url );
            Database db = impl.newDatabase();
	    try {
        	db.open( database, Database.OPEN_READ_ONLY );
            } catch( ODMGException odmge ) {
    	        try {
    		    db.close();
            	}
            	catch (ODMGException e) {}
                throw new ResourceException( "Could not connect to rasdaman at " + url + ", database " + database, odmge );
    	    }
    	    Transaction tr = impl.newTransaction();
    	    tr.begin();
    	    OQLQuery q = impl.newOQLQuery();
            DBag resultSet;
            try {
                q.create( this.getQuery() );
            	resultSet = (DBag) q.execute();
            	if( resultSet != null ) {
                    Iterator resultIterator = resultSet.iterator();
                    while( resultIterator.hasNext() ) {
			Object current = resultIterator.next();
			try{
            	    	    RasGMArray resultArray = (RasGMArray) current;
			    results.add( resultArray.getArray() );
			} catch (ClassCastException e) { // not a RasGMarray
			    if (!mime.equals("text/plain"))
				throw new ResourceException("Incompatible mime and data type!");
			    System.err.println("result="+current.toString());
			    results.add( current.toString().getBytes() );
			    
			}
			
			
/*			if (mime.equals("text/plain")) {
			    System.err.println("dataType is :" + resultArray.getBaseTypeSchema().toString());			
			}*/
                        
                    }
            	}
            } catch (QueryException qe) {
        	tr.commit();
                try {
                    db.close();
                } catch (ODMGException odmge) {}
                throw new ResourceException ( "Could not evaluate rasdaman query: '" + getQuery() + "'", qe );
            }
    	    tr.commit();
    	    try {
    	        db.close();
    	    } catch (ODMGException odmge) {}
    	}
	if (mime.equals("text/plain")) {
	    
	}
        return results;

    }
	    
}
