// $ANTLR 3.1.2 src/java/grammar/wcps.g 2009-05-19 14:16:00
package grammar;

import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import org.antlr.runtime.tree.*;

public class wcpsParser extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "FOR", "IN", "LPAREN", "RPAREN", "COMMA", "WHERE", "RETURN", "ENCODE", "STORE", "OR", "XOR", "AND", "EQUALS", "NOTEQUALS", "LT", "GT", "LTE", "GTE", "PLUS", "MINUS", "MULT", "DIVIDE", "OVERLAY", "IDENTIFIER", "IMAGECRS", "IMAGECRSDOMAIN", "CRSSET", "NULLSET", "INTERPOLATIONDEFAULT", "INTERPOLATIONSET", "DOMAIN", "ALL", "SOME", "COUNT", "ADD", "AVG", "MIN", "MAX", "CONDENSE", "OVER", "USING", "COLON", "COVERAGE", "VALUE", "LIST", "SEMICOLON", "VALUES", "SETIDENTIFIER", "SETCRSSET", "SETNULLSET", "SETINTERPOLATIONDEFAULT", "SETINTERPOLATIONSET", "LBRACE", "RBRACE", "STRUCT", "CRSTRANSFORM", "SQRT", "ABS", "RE", "IM", "EXP", "LOG", "LN", "SIN", "COS", "TAN", "SINH", "COSH", "TANH", "ARCSIN", "ARCCOS", "ARCTAN", "NOT", "BIT", "INTEGERCONSTANT", "ROUND", "STRING", "SCALE", "LBRACKET", "RBRACKET", "TRIM", "SLICE", "EXTEND", "BOOLEAN", "CHAR", "SHORT", "LONG", "FLOAT", "DOUBLE", "COMPLEX", "COMPLEX2", "UNSIGNED", "DOT", "BOOLEANCONSTANT", "FLOATCONSTANT", "NEAREST", "LINEAR", "QUADRATIC", "CUBIC", "FULL", "NONE", "HALF", "OTHER", "NAME", "VARIABLE_DOLLAR", "NULLDEFAULT", "PHI", "DECIMALCONSTANT", "OCTALCONSTANT", "HEXACONSTANT", "WHITESPACE"
    };
    public static final int IMAGECRS=28;
    public static final int LT=18;
    public static final int HEXACONSTANT=113;
    public static final int LN=66;
    public static final int LOG=65;
    public static final int CHAR=88;
    public static final int COMPLEX=93;
    public static final int SETINTERPOLATIONDEFAULT=54;
    public static final int COUNT=37;
    public static final int EQUALS=16;
    public static final int COSH=71;
    public static final int NOT=76;
    public static final int INTEGERCONSTANT=78;
    public static final int EOF=-1;
    public static final int SINH=70;
    public static final int LBRACKET=82;
    public static final int RPAREN=7;
    public static final int NAME=107;
    public static final int LINEAR=100;
    public static final int TANH=72;
    public static final int FULL=103;
    public static final int USING=44;
    public static final int SIN=67;
    public static final int EXP=64;
    public static final int COS=68;
    public static final int TAN=69;
    public static final int RETURN=10;
    public static final int DOUBLE=92;
    public static final int NULLDEFAULT=109;
    public static final int DIVIDE=25;
    public static final int STORE=12;
    public static final int BOOLEANCONSTANT=97;
    public static final int RBRACE=57;
    public static final int SETNULLSET=53;
    public static final int CONDENSE=42;
    public static final int WHITESPACE=114;
    public static final int SEMICOLON=49;
    public static final int MULT=24;
    public static final int VALUE=47;
    public static final int LIST=48;
    public static final int COMPLEX2=94;
    public static final int ABS=61;
    public static final int DECIMALCONSTANT=111;
    public static final int CRSSET=30;
    public static final int SCALE=81;
    public static final int VARIABLE_DOLLAR=108;
    public static final int FLOATCONSTANT=98;
    public static final int IMAGECRSDOMAIN=29;
    public static final int NONE=104;
    public static final int OR=13;
    public static final int TRIM=84;
    public static final int GT=19;
    public static final int ROUND=79;
    public static final int QUADRATIC=101;
    public static final int ENCODE=11;
    public static final int PHI=110;
    public static final int OVER=43;
    public static final int COVERAGE=46;
    public static final int WHERE=9;
    public static final int RE=62;
    public static final int OVERLAY=26;
    public static final int GTE=21;
    public static final int LBRACE=56;
    public static final int MAX=41;
    public static final int INTERPOLATIONDEFAULT=32;
    public static final int FOR=4;
    public static final int FLOAT=91;
    public static final int SLICE=85;
    public static final int AND=15;
    public static final int LTE=20;
    public static final int LPAREN=6;
    public static final int EXTEND=86;
    public static final int IM=63;
    public static final int BOOLEAN=87;
    public static final int IN=5;
    public static final int COMMA=8;
    public static final int AVG=39;
    public static final int IDENTIFIER=27;
    public static final int SOME=36;
    public static final int ALL=35;
    public static final int ARCSIN=73;
    public static final int PLUS=22;
    public static final int ARCCOS=74;
    public static final int RBRACKET=83;
    public static final int DOT=96;
    public static final int ADD=38;
    public static final int SETIDENTIFIER=51;
    public static final int XOR=14;
    public static final int SETINTERPOLATIONSET=55;
    public static final int OTHER=106;
    public static final int VALUES=50;
    public static final int ARCTAN=75;
    public static final int NOTEQUALS=17;
    public static final int STRUCT=58;
    public static final int SHORT=89;
    public static final int OCTALCONSTANT=112;
    public static final int MIN=40;
    public static final int MINUS=23;
    public static final int SQRT=60;
    public static final int DOMAIN=34;
    public static final int CRSTRANSFORM=59;
    public static final int COLON=45;
    public static final int CUBIC=102;
    public static final int UNSIGNED=95;
    public static final int NULLSET=31;
    public static final int BIT=77;
    public static final int INTERPOLATIONSET=33;
    public static final int LONG=90;
    public static final int SETCRSSET=52;
    public static final int HALF=105;
    public static final int STRING=80;
    public static final int NEAREST=99;

    // delegates
    // delegators


        public wcpsParser(TokenStream input) {
            this(input, new RecognizerSharedState());
        }
        public wcpsParser(TokenStream input, RecognizerSharedState state) {
            super(input, state);
            this.state.ruleMemo = new HashMap[239+1];
             
             
        }
        
    protected TreeAdaptor adaptor = new CommonTreeAdaptor();

    public void setTreeAdaptor(TreeAdaptor adaptor) {
        this.adaptor = adaptor;
    }
    public TreeAdaptor getTreeAdaptor() {
        return adaptor;
    }

    public String[] getTokenNames() { return wcpsParser.tokenNames; }
    public String getGrammarFileName() { return "src/java/grammar/wcps.g"; }


    public static class wcpsRequest_return extends ParserRuleReturnScope {
        public WCPSRequest value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "wcpsRequest"
    // src/java/grammar/wcps.g:28:1: wcpsRequest returns [WCPSRequest value] : e1= forClause (e2= whereClause )? e3= returnClause ;
    public final wcpsParser.wcpsRequest_return wcpsRequest() throws RecognitionException {
        wcpsParser.wcpsRequest_return retval = new wcpsParser.wcpsRequest_return();
        retval.start = input.LT(1);
        int wcpsRequest_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.forClause_return e1 = null;

        wcpsParser.whereClause_return e2 = null;

        wcpsParser.returnClause_return e3 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 1) ) { return retval; }
            // src/java/grammar/wcps.g:29:2: (e1= forClause (e2= whereClause )? e3= returnClause )
            // src/java/grammar/wcps.g:29:4: e1= forClause (e2= whereClause )? e3= returnClause
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_forClause_in_wcpsRequest63);
            e1=forClause();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new WCPSRequest((e1!=null?e1.value:null)); 
            }
            // src/java/grammar/wcps.g:30:3: (e2= whereClause )?
            int alt1=2;
            int LA1_0 = input.LA(1);

            if ( (LA1_0==WHERE) ) {
                alt1=1;
            }
            switch (alt1) {
                case 1 :
                    // src/java/grammar/wcps.g:30:4: e2= whereClause
                    {
                    pushFollow(FOLLOW_whereClause_in_wcpsRequest72);
                    e2=whereClause();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.setWhere((e2!=null?e2.value:null)); 
                    }

                    }
                    break;

            }

            pushFollow(FOLLOW_returnClause_in_wcpsRequest83);
            e3=returnClause();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
            if ( state.backtracking==0 ) {
               retval.value.setReturn((e3!=null?e3.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 1, wcpsRequest_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "wcpsRequest"

    public static class forClause_return extends ParserRuleReturnScope {
        public ForClauseElements value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "forClause"
    // src/java/grammar/wcps.g:33:1: forClause returns [ForClauseElements value] : FOR v= variableName IN LPAREN list= coverageList RPAREN ( COMMA v= variableName IN LPAREN list= coverageList RPAREN )* ;
    public final wcpsParser.forClause_return forClause() throws RecognitionException {
        wcpsParser.forClause_return retval = new wcpsParser.forClause_return();
        retval.start = input.LT(1);
        int forClause_StartIndex = input.index();
        Object root_0 = null;

        Token FOR1=null;
        Token IN2=null;
        Token LPAREN3=null;
        Token RPAREN4=null;
        Token COMMA5=null;
        Token IN6=null;
        Token LPAREN7=null;
        Token RPAREN8=null;
        wcpsParser.variableName_return v = null;

        wcpsParser.coverageList_return list = null;


        Object FOR1_tree=null;
        Object IN2_tree=null;
        Object LPAREN3_tree=null;
        Object RPAREN4_tree=null;
        Object COMMA5_tree=null;
        Object IN6_tree=null;
        Object LPAREN7_tree=null;
        Object RPAREN8_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 2) ) { return retval; }
            // src/java/grammar/wcps.g:34:2: ( FOR v= variableName IN LPAREN list= coverageList RPAREN ( COMMA v= variableName IN LPAREN list= coverageList RPAREN )* )
            // src/java/grammar/wcps.g:34:4: FOR v= variableName IN LPAREN list= coverageList RPAREN ( COMMA v= variableName IN LPAREN list= coverageList RPAREN )*
            {
            root_0 = (Object)adaptor.nil();

            FOR1=(Token)match(input,FOR,FOLLOW_FOR_in_forClause98); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            FOR1_tree = (Object)adaptor.create(FOR1);
            adaptor.addChild(root_0, FOR1_tree);
            }
            pushFollow(FOLLOW_variableName_in_forClause102);
            v=variableName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, v.getTree());
            IN2=(Token)match(input,IN,FOLLOW_IN_in_forClause104); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            IN2_tree = (Object)adaptor.create(IN2);
            adaptor.addChild(root_0, IN2_tree);
            }
            LPAREN3=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_forClause106); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN3_tree = (Object)adaptor.create(LPAREN3);
            adaptor.addChild(root_0, LPAREN3_tree);
            }
            pushFollow(FOLLOW_coverageList_in_forClause110);
            list=coverageList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, list.getTree());
            RPAREN4=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_forClause112); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN4_tree = (Object)adaptor.create(RPAREN4);
            adaptor.addChild(root_0, RPAREN4_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new ForClauseElements((v!=null?v.value:null), (list!=null?list.value:null)); 
            }
            // src/java/grammar/wcps.g:36:4: ( COMMA v= variableName IN LPAREN list= coverageList RPAREN )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==COMMA) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // src/java/grammar/wcps.g:36:5: COMMA v= variableName IN LPAREN list= coverageList RPAREN
            	    {
            	    COMMA5=(Token)match(input,COMMA,FOLLOW_COMMA_in_forClause122); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA5_tree = (Object)adaptor.create(COMMA5);
            	    adaptor.addChild(root_0, COMMA5_tree);
            	    }
            	    pushFollow(FOLLOW_variableName_in_forClause126);
            	    v=variableName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, v.getTree());
            	    IN6=(Token)match(input,IN,FOLLOW_IN_in_forClause128); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    IN6_tree = (Object)adaptor.create(IN6);
            	    adaptor.addChild(root_0, IN6_tree);
            	    }
            	    LPAREN7=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_forClause130); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    LPAREN7_tree = (Object)adaptor.create(LPAREN7);
            	    adaptor.addChild(root_0, LPAREN7_tree);
            	    }
            	    pushFollow(FOLLOW_coverageList_in_forClause134);
            	    list=coverageList();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, list.getTree());
            	    RPAREN8=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_forClause136); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    RPAREN8_tree = (Object)adaptor.create(RPAREN8);
            	    adaptor.addChild(root_0, RPAREN8_tree);
            	    }
            	    if ( state.backtracking==0 ) {
            	       retval.value = new ForClauseElements((v!=null?v.value:null), (list!=null?list.value:null), retval.value); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop2;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 2, forClause_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "forClause"

    public static class whereClause_return extends ParserRuleReturnScope {
        public WhereClause value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "whereClause"
    // src/java/grammar/wcps.g:39:1: whereClause returns [WhereClause value] : WHERE e1= booleanScalarExpr ;
    public final wcpsParser.whereClause_return whereClause() throws RecognitionException {
        wcpsParser.whereClause_return retval = new wcpsParser.whereClause_return();
        retval.start = input.LT(1);
        int whereClause_StartIndex = input.index();
        Object root_0 = null;

        Token WHERE9=null;
        wcpsParser.booleanScalarExpr_return e1 = null;


        Object WHERE9_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 3) ) { return retval; }
            // src/java/grammar/wcps.g:40:2: ( WHERE e1= booleanScalarExpr )
            // src/java/grammar/wcps.g:40:4: WHERE e1= booleanScalarExpr
            {
            root_0 = (Object)adaptor.nil();

            WHERE9=(Token)match(input,WHERE,FOLLOW_WHERE_in_whereClause157); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            WHERE9_tree = (Object)adaptor.create(WHERE9);
            adaptor.addChild(root_0, WHERE9_tree);
            }
            pushFollow(FOLLOW_booleanScalarExpr_in_whereClause161);
            e1=booleanScalarExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new WhereClause((e1!=null?e1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 3, whereClause_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "whereClause"

    public static class returnClause_return extends ParserRuleReturnScope {
        public ReturnClause value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "returnClause"
    // src/java/grammar/wcps.g:42:1: returnClause returns [ReturnClause value] : RETURN e1= processingExpr ;
    public final wcpsParser.returnClause_return returnClause() throws RecognitionException {
        wcpsParser.returnClause_return retval = new wcpsParser.returnClause_return();
        retval.start = input.LT(1);
        int returnClause_StartIndex = input.index();
        Object root_0 = null;

        Token RETURN10=null;
        wcpsParser.processingExpr_return e1 = null;


        Object RETURN10_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 4) ) { return retval; }
            // src/java/grammar/wcps.g:43:2: ( RETURN e1= processingExpr )
            // src/java/grammar/wcps.g:43:4: RETURN e1= processingExpr
            {
            root_0 = (Object)adaptor.nil();

            RETURN10=(Token)match(input,RETURN,FOLLOW_RETURN_in_returnClause176); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RETURN10_tree = (Object)adaptor.create(RETURN10);
            adaptor.addChild(root_0, RETURN10_tree);
            }
            pushFollow(FOLLOW_processingExpr_in_returnClause180);
            e1=processingExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new ReturnClause((e1!=null?e1.value:null));  
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 4, returnClause_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "returnClause"

    public static class coverageList_return extends ParserRuleReturnScope {
        public CoverageList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageList"
    // src/java/grammar/wcps.g:45:1: coverageList returns [CoverageList value] : cname= coverageName ( COMMA next= coverageName )* ;
    public final wcpsParser.coverageList_return coverageList() throws RecognitionException {
        wcpsParser.coverageList_return retval = new wcpsParser.coverageList_return();
        retval.start = input.LT(1);
        int coverageList_StartIndex = input.index();
        Object root_0 = null;

        Token COMMA11=null;
        wcpsParser.coverageName_return cname = null;

        wcpsParser.coverageName_return next = null;


        Object COMMA11_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 5) ) { return retval; }
            // src/java/grammar/wcps.g:46:2: (cname= coverageName ( COMMA next= coverageName )* )
            // src/java/grammar/wcps.g:46:4: cname= coverageName ( COMMA next= coverageName )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageName_in_coverageList197);
            cname=coverageName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, cname.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new CoverageList((cname!=null?cname.value:null)); 
            }
            // src/java/grammar/wcps.g:47:3: ( COMMA next= coverageName )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==COMMA) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // src/java/grammar/wcps.g:47:4: COMMA next= coverageName
            	    {
            	    COMMA11=(Token)match(input,COMMA,FOLLOW_COMMA_in_coverageList204); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA11_tree = (Object)adaptor.create(COMMA11);
            	    adaptor.addChild(root_0, COMMA11_tree);
            	    }
            	    pushFollow(FOLLOW_coverageName_in_coverageList208);
            	    next=coverageName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, next.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new CoverageList((next!=null?next.value:null), retval.value); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 5, coverageList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageList"

    public static class processingExpr_return extends ParserRuleReturnScope {
        public ProcessingExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "processingExpr"
    // src/java/grammar/wcps.g:49:1: processingExpr returns [ProcessingExpr value] : (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr );
    public final wcpsParser.processingExpr_return processingExpr() throws RecognitionException {
        wcpsParser.processingExpr_return retval = new wcpsParser.processingExpr_return();
        retval.start = input.LT(1);
        int processingExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.encodedCoverageExpr_return e1 = null;

        wcpsParser.storeExpr_return e2 = null;

        wcpsParser.scalarExpr_return e3 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 6) ) { return retval; }
            // src/java/grammar/wcps.g:50:5: (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr )
            int alt4=3;
            alt4 = dfa4.predict(input);
            switch (alt4) {
                case 1 :
                    // src/java/grammar/wcps.g:50:7: e1= encodedCoverageExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_encodedCoverageExpr_in_processingExpr230);
                    e1=encodedCoverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new ProcessingExpr((e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:51:7: e2= storeExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_storeExpr_in_processingExpr242);
                    e2=storeExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new ProcessingExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:52:7: e3= scalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scalarExpr_in_processingExpr254);
                    e3=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new ProcessingExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 6, processingExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "processingExpr"

    public static class encodedCoverageExpr_return extends ParserRuleReturnScope {
        public EncodedCoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "encodedCoverageExpr"
    // src/java/grammar/wcps.g:54:1: encodedCoverageExpr returns [EncodedCoverageExpr value] : ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN ;
    public final wcpsParser.encodedCoverageExpr_return encodedCoverageExpr() throws RecognitionException {
        wcpsParser.encodedCoverageExpr_return retval = new wcpsParser.encodedCoverageExpr_return();
        retval.start = input.LT(1);
        int encodedCoverageExpr_StartIndex = input.index();
        Object root_0 = null;

        Token ENCODE12=null;
        Token LPAREN13=null;
        Token COMMA14=null;
        Token COMMA15=null;
        Token RPAREN16=null;
        wcpsParser.coverageExpr_return cov = null;

        wcpsParser.stringConstant_return format = null;

        wcpsParser.stringConstant_return params = null;


        Object ENCODE12_tree=null;
        Object LPAREN13_tree=null;
        Object COMMA14_tree=null;
        Object COMMA15_tree=null;
        Object RPAREN16_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 7) ) { return retval; }
            // src/java/grammar/wcps.g:55:2: ( ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN )
            // src/java/grammar/wcps.g:55:4: ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN
            {
            root_0 = (Object)adaptor.nil();

            ENCODE12=(Token)match(input,ENCODE,FOLLOW_ENCODE_in_encodedCoverageExpr272); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            ENCODE12_tree = (Object)adaptor.create(ENCODE12);
            adaptor.addChild(root_0, ENCODE12_tree);
            }
            LPAREN13=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_encodedCoverageExpr274); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN13_tree = (Object)adaptor.create(LPAREN13);
            adaptor.addChild(root_0, LPAREN13_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_encodedCoverageExpr278);
            cov=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, cov.getTree());
            COMMA14=(Token)match(input,COMMA,FOLLOW_COMMA_in_encodedCoverageExpr280); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA14_tree = (Object)adaptor.create(COMMA14);
            adaptor.addChild(root_0, COMMA14_tree);
            }
            pushFollow(FOLLOW_stringConstant_in_encodedCoverageExpr284);
            format=stringConstant();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, format.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new EncodedCoverageExpr((cov!=null?cov.value:null), (format!=null?input.toString(format.start,format.stop):null)); 
            }
            // src/java/grammar/wcps.g:56:3: ( COMMA params= stringConstant )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==COMMA) ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // src/java/grammar/wcps.g:56:4: COMMA params= stringConstant
                    {
                    COMMA15=(Token)match(input,COMMA,FOLLOW_COMMA_in_encodedCoverageExpr291); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA15_tree = (Object)adaptor.create(COMMA15);
                    adaptor.addChild(root_0, COMMA15_tree);
                    }
                    pushFollow(FOLLOW_stringConstant_in_encodedCoverageExpr295);
                    params=stringConstant();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, params.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.setParams((params!=null?input.toString(params.start,params.stop):null)); 
                    }

                    }
                    break;

            }

            RPAREN16=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_encodedCoverageExpr302); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN16_tree = (Object)adaptor.create(RPAREN16);
            adaptor.addChild(root_0, RPAREN16_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 7, encodedCoverageExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "encodedCoverageExpr"

    public static class storeExpr_return extends ParserRuleReturnScope {
        public StoreExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "storeExpr"
    // src/java/grammar/wcps.g:58:1: storeExpr returns [StoreExpr value] : STORE LPAREN e1= encodedCoverageExpr RPAREN ;
    public final wcpsParser.storeExpr_return storeExpr() throws RecognitionException {
        wcpsParser.storeExpr_return retval = new wcpsParser.storeExpr_return();
        retval.start = input.LT(1);
        int storeExpr_StartIndex = input.index();
        Object root_0 = null;

        Token STORE17=null;
        Token LPAREN18=null;
        Token RPAREN19=null;
        wcpsParser.encodedCoverageExpr_return e1 = null;


        Object STORE17_tree=null;
        Object LPAREN18_tree=null;
        Object RPAREN19_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 8) ) { return retval; }
            // src/java/grammar/wcps.g:59:5: ( STORE LPAREN e1= encodedCoverageExpr RPAREN )
            // src/java/grammar/wcps.g:59:7: STORE LPAREN e1= encodedCoverageExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            STORE17=(Token)match(input,STORE,FOLLOW_STORE_in_storeExpr319); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            STORE17_tree = (Object)adaptor.create(STORE17);
            adaptor.addChild(root_0, STORE17_tree);
            }
            LPAREN18=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_storeExpr321); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN18_tree = (Object)adaptor.create(LPAREN18);
            adaptor.addChild(root_0, LPAREN18_tree);
            }
            pushFollow(FOLLOW_encodedCoverageExpr_in_storeExpr325);
            e1=encodedCoverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN19=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_storeExpr327); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN19_tree = (Object)adaptor.create(RPAREN19);
            adaptor.addChild(root_0, RPAREN19_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new StoreExpr((e1!=null?e1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 8, storeExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "storeExpr"

    public static class coverageExpr_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageExpr"
    // src/java/grammar/wcps.g:61:1: coverageExpr returns [CoverageExpr value] : e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )* ;
    public final wcpsParser.coverageExpr_return coverageExpr() throws RecognitionException {
        wcpsParser.coverageExpr_return retval = new wcpsParser.coverageExpr_return();
        retval.start = input.LT(1);
        int coverageExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.coverageLogicTerm_return e1 = null;

        wcpsParser.coverageLogicTerm_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 9) ) { return retval; }
            // src/java/grammar/wcps.g:62:5: (e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )* )
            // src/java/grammar/wcps.g:62:7: e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageLogicTerm_in_coverageExpr350);
            e1=coverageLogicTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:63:9: (op= ( OR | XOR ) e2= coverageLogicTerm )*
            loop6:
            do {
                int alt6=2;
                alt6 = dfa6.predict(input);
                switch (alt6) {
            	case 1 :
            	    // src/java/grammar/wcps.g:63:10: op= ( OR | XOR ) e2= coverageLogicTerm
            	    {
            	    op=(Token)input.LT(1);
            	    if ( (input.LA(1)>=OR && input.LA(1)<=XOR) ) {
            	        input.consume();
            	        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
            	        state.errorRecovery=false;state.failed=false;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_coverageLogicTerm_in_coverageExpr374);
            	    e2=coverageLogicTerm();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new CoverageExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop6;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 9, coverageExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageExpr"

    public static class coverageLogicTerm_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageLogicTerm"
    // src/java/grammar/wcps.g:65:1: coverageLogicTerm returns [CoverageExpr value] : e1= coverageLogicFactor (op= ( AND ) e2= coverageLogicFactor )* ;
    public final wcpsParser.coverageLogicTerm_return coverageLogicTerm() throws RecognitionException {
        wcpsParser.coverageLogicTerm_return retval = new wcpsParser.coverageLogicTerm_return();
        retval.start = input.LT(1);
        int coverageLogicTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token AND20=null;
        wcpsParser.coverageLogicFactor_return e1 = null;

        wcpsParser.coverageLogicFactor_return e2 = null;


        Object op_tree=null;
        Object AND20_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 10) ) { return retval; }
            // src/java/grammar/wcps.g:66:5: (e1= coverageLogicFactor (op= ( AND ) e2= coverageLogicFactor )* )
            // src/java/grammar/wcps.g:66:7: e1= coverageLogicFactor (op= ( AND ) e2= coverageLogicFactor )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageLogicFactor_in_coverageLogicTerm400);
            e1=coverageLogicFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:67:9: (op= ( AND ) e2= coverageLogicFactor )*
            loop7:
            do {
                int alt7=2;
                alt7 = dfa7.predict(input);
                switch (alt7) {
            	case 1 :
            	    // src/java/grammar/wcps.g:67:10: op= ( AND ) e2= coverageLogicFactor
            	    {
            	    // src/java/grammar/wcps.g:67:13: ( AND )
            	    // src/java/grammar/wcps.g:67:14: AND
            	    {
            	    AND20=(Token)match(input,AND,FOLLOW_AND_in_coverageLogicTerm417); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    AND20_tree = (Object)adaptor.create(AND20);
            	    adaptor.addChild(root_0, AND20_tree);
            	    }

            	    }

            	    pushFollow(FOLLOW_coverageLogicFactor_in_coverageLogicTerm422);
            	    e2=coverageLogicFactor();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new CoverageExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop7;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 10, coverageLogicTerm_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageLogicTerm"

    public static class coverageLogicFactor_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageLogicFactor"
    // src/java/grammar/wcps.g:69:1: coverageLogicFactor returns [CoverageExpr value] : e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )? ;
    public final wcpsParser.coverageLogicFactor_return coverageLogicFactor() throws RecognitionException {
        wcpsParser.coverageLogicFactor_return retval = new wcpsParser.coverageLogicFactor_return();
        retval.start = input.LT(1);
        int coverageLogicFactor_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.coverageArithmeticExpr_return e1 = null;

        wcpsParser.coverageArithmeticExpr_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 11) ) { return retval; }
            // src/java/grammar/wcps.g:70:5: (e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )? )
            // src/java/grammar/wcps.g:70:7: e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor449);
            e1=coverageArithmeticExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null);
            }
            // src/java/grammar/wcps.g:71:9: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?
            int alt8=2;
            alt8 = dfa8.predict(input);
            switch (alt8) {
                case 1 :
                    // src/java/grammar/wcps.g:71:10: op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr
                    {
                    op=(Token)input.LT(1);
                    if ( (input.LA(1)>=EQUALS && input.LA(1)<=GTE) ) {
                        input.consume();
                        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    pushFollow(FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor481);
                    e2=coverageArithmeticExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
                    }

                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 11, coverageLogicFactor_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageLogicFactor"

    public static class coverageArithmeticExpr_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageArithmeticExpr"
    // src/java/grammar/wcps.g:73:1: coverageArithmeticExpr returns [CoverageExpr value] : e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )* ;
    public final wcpsParser.coverageArithmeticExpr_return coverageArithmeticExpr() throws RecognitionException {
        wcpsParser.coverageArithmeticExpr_return retval = new wcpsParser.coverageArithmeticExpr_return();
        retval.start = input.LT(1);
        int coverageArithmeticExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.coverageArithmeticTerm_return e1 = null;

        wcpsParser.coverageArithmeticTerm_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 12) ) { return retval; }
            // src/java/grammar/wcps.g:74:5: (e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )* )
            // src/java/grammar/wcps.g:74:7: e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr509);
            e1=coverageArithmeticTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:75:9: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*
            loop9:
            do {
                int alt9=2;
                alt9 = dfa9.predict(input);
                switch (alt9) {
            	case 1 :
            	    // src/java/grammar/wcps.g:75:10: op= ( PLUS | MINUS ) e2= coverageArithmeticTerm
            	    {
            	    op=(Token)input.LT(1);
            	    if ( (input.LA(1)>=PLUS && input.LA(1)<=MINUS) ) {
            	        input.consume();
            	        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
            	        state.errorRecovery=false;state.failed=false;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr533);
            	    e2=coverageArithmeticTerm();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new CoverageExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 12, coverageArithmeticExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageArithmeticExpr"

    public static class coverageArithmeticTerm_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageArithmeticTerm"
    // src/java/grammar/wcps.g:77:1: coverageArithmeticTerm returns [CoverageExpr value] : e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )* ;
    public final wcpsParser.coverageArithmeticTerm_return coverageArithmeticTerm() throws RecognitionException {
        wcpsParser.coverageArithmeticTerm_return retval = new wcpsParser.coverageArithmeticTerm_return();
        retval.start = input.LT(1);
        int coverageArithmeticTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.coverageArithmeticFactor_return e1 = null;

        wcpsParser.coverageArithmeticFactor_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 13) ) { return retval; }
            // src/java/grammar/wcps.g:78:5: (e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )* )
            // src/java/grammar/wcps.g:78:9: e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm561);
            e1=coverageArithmeticFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:79:9: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*
            loop10:
            do {
                int alt10=2;
                alt10 = dfa10.predict(input);
                switch (alt10) {
            	case 1 :
            	    // src/java/grammar/wcps.g:79:10: op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor
            	    {
            	    op=(Token)input.LT(1);
            	    if ( (input.LA(1)>=MULT && input.LA(1)<=DIVIDE) ) {
            	        input.consume();
            	        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
            	        state.errorRecovery=false;state.failed=false;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm584);
            	    e2=coverageArithmeticFactor();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new CoverageExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop10;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 13, coverageArithmeticTerm_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageArithmeticTerm"

    public static class coverageArithmeticFactor_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageArithmeticFactor"
    // src/java/grammar/wcps.g:81:1: coverageArithmeticFactor returns [CoverageExpr value] : e1= coverageValue (op= ( OVERLAY ) e2= coverageValue )* ;
    public final wcpsParser.coverageArithmeticFactor_return coverageArithmeticFactor() throws RecognitionException {
        wcpsParser.coverageArithmeticFactor_return retval = new wcpsParser.coverageArithmeticFactor_return();
        retval.start = input.LT(1);
        int coverageArithmeticFactor_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token OVERLAY21=null;
        wcpsParser.coverageValue_return e1 = null;

        wcpsParser.coverageValue_return e2 = null;


        Object op_tree=null;
        Object OVERLAY21_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 14) ) { return retval; }
            // src/java/grammar/wcps.g:82:5: (e1= coverageValue (op= ( OVERLAY ) e2= coverageValue )* )
            // src/java/grammar/wcps.g:82:7: e1= coverageValue (op= ( OVERLAY ) e2= coverageValue )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageValue_in_coverageArithmeticFactor612);
            e1=coverageValue();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:83:9: (op= ( OVERLAY ) e2= coverageValue )*
            loop11:
            do {
                int alt11=2;
                alt11 = dfa11.predict(input);
                switch (alt11) {
            	case 1 :
            	    // src/java/grammar/wcps.g:83:10: op= ( OVERLAY ) e2= coverageValue
            	    {
            	    // src/java/grammar/wcps.g:83:13: ( OVERLAY )
            	    // src/java/grammar/wcps.g:83:14: OVERLAY
            	    {
            	    OVERLAY21=(Token)match(input,OVERLAY,FOLLOW_OVERLAY_in_coverageArithmeticFactor628); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    OVERLAY21_tree = (Object)adaptor.create(OVERLAY21);
            	    adaptor.addChild(root_0, OVERLAY21_tree);
            	    }

            	    }

            	    pushFollow(FOLLOW_coverageValue_in_coverageArithmeticFactor633);
            	    e2=coverageValue();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new CoverageExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop11;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 14, coverageArithmeticFactor_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageArithmeticFactor"

    public static class coverageValue_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageValue"
    // src/java/grammar/wcps.g:85:1: coverageValue returns [CoverageExpr value] : (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom );
    public final wcpsParser.coverageValue_return coverageValue() throws RecognitionException {
        wcpsParser.coverageValue_return retval = new wcpsParser.coverageValue_return();
        retval.start = input.LT(1);
        int coverageValue_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.subsetExpr_return e5 = null;

        wcpsParser.unaryInducedExpr_return e2 = null;

        wcpsParser.scaleExpr_return e4 = null;

        wcpsParser.crsTransformExpr_return e3 = null;

        wcpsParser.coverageAtom_return e1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 15) ) { return retval; }
            // src/java/grammar/wcps.g:86:5: (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom )
            int alt12=5;
            alt12 = dfa12.predict(input);
            switch (alt12) {
                case 1 :
                    // src/java/grammar/wcps.g:86:7: e5= subsetExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_subsetExpr_in_coverageValue659);
                    e5=subsetExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e5!=null?e5.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:87:7: e2= unaryInducedExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_unaryInducedExpr_in_coverageValue672);
                    e2=unaryInducedExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = (e2!=null?e2.value:null); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:88:7: e4= scaleExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scaleExpr_in_coverageValue684);
                    e4=scaleExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e4!=null?e4.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:89:7: e3= crsTransformExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_crsTransformExpr_in_coverageValue696);
                    e3=crsTransformExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:90:7: e1= coverageAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_coverageValue708);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 15, coverageValue_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageValue"

    public static class coverageAtom_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageAtom"
    // src/java/grammar/wcps.g:92:1: coverageAtom returns [CoverageExpr value] : (e2= scalarExpr | e1= variableName | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr );
    public final wcpsParser.coverageAtom_return coverageAtom() throws RecognitionException {
        wcpsParser.coverageAtom_return retval = new wcpsParser.coverageAtom_return();
        retval.start = input.LT(1);
        int coverageAtom_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN22=null;
        Token RPAREN23=null;
        wcpsParser.scalarExpr_return e2 = null;

        wcpsParser.variableName_return e1 = null;

        wcpsParser.coverageExpr_return e7 = null;

        wcpsParser.coverageConstantExpr_return e3 = null;

        wcpsParser.coverageConstructorExpr_return e4 = null;

        wcpsParser.setMetaDataExpr_return e5 = null;

        wcpsParser.rangeConstructorExpr_return e6 = null;


        Object LPAREN22_tree=null;
        Object RPAREN23_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 16) ) { return retval; }
            // src/java/grammar/wcps.g:93:5: (e2= scalarExpr | e1= variableName | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr )
            int alt13=7;
            alt13 = dfa13.predict(input);
            switch (alt13) {
                case 1 :
                    // src/java/grammar/wcps.g:93:7: e2= scalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scalarExpr_in_coverageAtom731);
                    e2=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:94:7: e1= variableName
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_variableName_in_coverageAtom743);
                    e1=variableName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:95:7: LPAREN e7= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN22=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_coverageAtom753); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN22_tree = (Object)adaptor.create(LPAREN22);
                    adaptor.addChild(root_0, LPAREN22_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_coverageAtom757);
                    e7=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e7.getTree());
                    RPAREN23=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_coverageAtom759); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN23_tree = (Object)adaptor.create(RPAREN23);
                    adaptor.addChild(root_0, RPAREN23_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e7!=null?e7.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:96:7: e3= coverageConstantExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageConstantExpr_in_coverageAtom772);
                    e3=coverageConstantExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:97:7: e4= coverageConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageConstructorExpr_in_coverageAtom784);
                    e4=coverageConstructorExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e4!=null?e4.value:null)); 
                    }

                    }
                    break;
                case 6 :
                    // src/java/grammar/wcps.g:98:7: e5= setMetaDataExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_setMetaDataExpr_in_coverageAtom797);
                    e5=setMetaDataExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e5!=null?e5.value:null)); 
                    }

                    }
                    break;
                case 7 :
                    // src/java/grammar/wcps.g:99:7: e6= rangeConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_rangeConstructorExpr_in_coverageAtom810);
                    e6=rangeConstructorExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e6.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e6!=null?e6.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 16, coverageAtom_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageAtom"

    public static class scalarExpr_return extends ParserRuleReturnScope {
        public ScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "scalarExpr"
    // src/java/grammar/wcps.g:101:1: scalarExpr returns [ScalarExpr value] : (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN );
    public final wcpsParser.scalarExpr_return scalarExpr() throws RecognitionException {
        wcpsParser.scalarExpr_return retval = new wcpsParser.scalarExpr_return();
        retval.start = input.LT(1);
        int scalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN24=null;
        Token RPAREN25=null;
        wcpsParser.metaDataExpr_return e1 = null;

        wcpsParser.condenseExpr_return e2 = null;

        wcpsParser.booleanScalarExpr_return e3 = null;

        wcpsParser.numericScalarExpr_return e4 = null;

        wcpsParser.stringScalarExpr_return e5 = null;

        wcpsParser.scalarExpr_return e6 = null;


        Object LPAREN24_tree=null;
        Object RPAREN25_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 17) ) { return retval; }
            // src/java/grammar/wcps.g:102:5: (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN )
            int alt14=6;
            alt14 = dfa14.predict(input);
            switch (alt14) {
                case 1 :
                    // src/java/grammar/wcps.g:102:7: e1= metaDataExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_metaDataExpr_in_scalarExpr834);
                    e1=metaDataExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                        retval.value = new ScalarExpr((e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:103:7: e2= condenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_condenseExpr_in_scalarExpr847);
                    e2=condenseExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new ScalarExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:104:7: e3= booleanScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanScalarExpr_in_scalarExpr860);
                    e3=booleanScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                        retval.value = new ScalarExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:105:7: e4= numericScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_numericScalarExpr_in_scalarExpr874);
                    e4=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    if ( state.backtracking==0 ) {
                        retval.value = new ScalarExpr((e4!=null?e4.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:106:7: e5= stringScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_stringScalarExpr_in_scalarExpr887);
                    e5=stringScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    if ( state.backtracking==0 ) {
                        retval.value = new ScalarExpr((e5!=null?e5.value:null)); 
                    }

                    }
                    break;
                case 6 :
                    // src/java/grammar/wcps.g:107:7: LPAREN e6= scalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN24=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_scalarExpr898); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN24_tree = (Object)adaptor.create(LPAREN24);
                    adaptor.addChild(root_0, LPAREN24_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_scalarExpr902);
                    e6=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e6.getTree());
                    RPAREN25=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_scalarExpr904); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN25_tree = (Object)adaptor.create(RPAREN25);
                    adaptor.addChild(root_0, RPAREN25_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e6!=null?e6.value:null); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 17, scalarExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "scalarExpr"

    public static class metaDataExpr_return extends ParserRuleReturnScope {
        public MetaDataExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "metaDataExpr"
    // src/java/grammar/wcps.g:109:1: metaDataExpr returns [MetaDataExpr value] : (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | op= IMAGECRS LPAREN e1= coverageExpr RPAREN | op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN | op= CRSSET LPAREN e1= coverageExpr RPAREN | de= domainExpr | op= NULLSET LPAREN e1= coverageExpr RPAREN | op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN | op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN );
    public final wcpsParser.metaDataExpr_return metaDataExpr() throws RecognitionException {
        wcpsParser.metaDataExpr_return retval = new wcpsParser.metaDataExpr_return();
        retval.start = input.LT(1);
        int metaDataExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN26=null;
        Token RPAREN27=null;
        Token LPAREN28=null;
        Token RPAREN29=null;
        Token LPAREN30=null;
        Token COMMA31=null;
        Token RPAREN32=null;
        Token LPAREN33=null;
        Token RPAREN34=null;
        Token LPAREN35=null;
        Token RPAREN36=null;
        Token LPAREN37=null;
        Token COMMA38=null;
        Token RPAREN39=null;
        Token LPAREN40=null;
        Token COMMA41=null;
        Token RPAREN42=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.axisName_return e2 = null;

        wcpsParser.domainExpr_return de = null;

        wcpsParser.fieldName_return f1 = null;


        Object op_tree=null;
        Object LPAREN26_tree=null;
        Object RPAREN27_tree=null;
        Object LPAREN28_tree=null;
        Object RPAREN29_tree=null;
        Object LPAREN30_tree=null;
        Object COMMA31_tree=null;
        Object RPAREN32_tree=null;
        Object LPAREN33_tree=null;
        Object RPAREN34_tree=null;
        Object LPAREN35_tree=null;
        Object RPAREN36_tree=null;
        Object LPAREN37_tree=null;
        Object COMMA38_tree=null;
        Object RPAREN39_tree=null;
        Object LPAREN40_tree=null;
        Object COMMA41_tree=null;
        Object RPAREN42_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 18) ) { return retval; }
            // src/java/grammar/wcps.g:110:5: (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | op= IMAGECRS LPAREN e1= coverageExpr RPAREN | op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN | op= CRSSET LPAREN e1= coverageExpr RPAREN | de= domainExpr | op= NULLSET LPAREN e1= coverageExpr RPAREN | op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN | op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN )
            int alt16=8;
            switch ( input.LA(1) ) {
            case IDENTIFIER:
                {
                alt16=1;
                }
                break;
            case IMAGECRS:
                {
                alt16=2;
                }
                break;
            case IMAGECRSDOMAIN:
                {
                alt16=3;
                }
                break;
            case CRSSET:
                {
                alt16=4;
                }
                break;
            case DOMAIN:
                {
                alt16=5;
                }
                break;
            case NULLSET:
                {
                alt16=6;
                }
                break;
            case INTERPOLATIONDEFAULT:
                {
                alt16=7;
                }
                break;
            case INTERPOLATIONSET:
                {
                alt16=8;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 16, 0, input);

                throw nvae;
            }

            switch (alt16) {
                case 1 :
                    // src/java/grammar/wcps.g:110:7: op= IDENTIFIER LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IDENTIFIER,FOLLOW_IDENTIFIER_in_metaDataExpr928); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN26=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr930); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN26_tree = (Object)adaptor.create(LPAREN26);
                    adaptor.addChild(root_0, LPAREN26_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr934);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN27=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr936); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN27_tree = (Object)adaptor.create(RPAREN27);
                    adaptor.addChild(root_0, RPAREN27_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:111:7: op= IMAGECRS LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IMAGECRS,FOLLOW_IMAGECRS_in_metaDataExpr948); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN28=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr950); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN28_tree = (Object)adaptor.create(LPAREN28);
                    adaptor.addChild(root_0, LPAREN28_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr954);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN29=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr956); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN29_tree = (Object)adaptor.create(RPAREN29);
                    adaptor.addChild(root_0, RPAREN29_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:112:7: op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IMAGECRSDOMAIN,FOLLOW_IMAGECRSDOMAIN_in_metaDataExpr968); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN30=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr970); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN30_tree = (Object)adaptor.create(LPAREN30);
                    adaptor.addChild(root_0, LPAREN30_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr974);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    // src/java/grammar/wcps.g:112:48: ( COMMA e2= axisName )?
                    int alt15=2;
                    int LA15_0 = input.LA(1);

                    if ( (LA15_0==COMMA) ) {
                        alt15=1;
                    }
                    switch (alt15) {
                        case 1 :
                            // src/java/grammar/wcps.g:112:49: COMMA e2= axisName
                            {
                            COMMA31=(Token)match(input,COMMA,FOLLOW_COMMA_in_metaDataExpr977); if (state.failed) return retval;
                            if ( state.backtracking==0 ) {
                            COMMA31_tree = (Object)adaptor.create(COMMA31);
                            adaptor.addChild(root_0, COMMA31_tree);
                            }
                            pushFollow(FOLLOW_axisName_in_metaDataExpr981);
                            e2=axisName();

                            state._fsp--;
                            if (state.failed) return retval;
                            if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());

                            }
                            break;

                    }

                    RPAREN32=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr985); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN32_tree = (Object)adaptor.create(RPAREN32);
                    adaptor.addChild(root_0, RPAREN32_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:113:7: op= CRSSET LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,CRSSET,FOLLOW_CRSSET_in_metaDataExpr997); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN33=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr999); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN33_tree = (Object)adaptor.create(LPAREN33);
                    adaptor.addChild(root_0, LPAREN33_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1003);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN34=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1005); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN34_tree = (Object)adaptor.create(RPAREN34);
                    adaptor.addChild(root_0, RPAREN34_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:114:7: de= domainExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_domainExpr_in_metaDataExpr1017);
                    de=domainExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, de.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((de!=null?de.value:null)); 
                    }

                    }
                    break;
                case 6 :
                    // src/java/grammar/wcps.g:115:7: op= NULLSET LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NULLSET,FOLLOW_NULLSET_in_metaDataExpr1029); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN35=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr1031); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN35_tree = (Object)adaptor.create(LPAREN35);
                    adaptor.addChild(root_0, LPAREN35_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1035);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN36=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1037); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN36_tree = (Object)adaptor.create(RPAREN36);
                    adaptor.addChild(root_0, RPAREN36_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null),(e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 7 :
                    // src/java/grammar/wcps.g:116:7: op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,INTERPOLATIONDEFAULT,FOLLOW_INTERPOLATIONDEFAULT_in_metaDataExpr1049); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN37=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr1051); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN37_tree = (Object)adaptor.create(LPAREN37);
                    adaptor.addChild(root_0, LPAREN37_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1055);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA38=(Token)match(input,COMMA,FOLLOW_COMMA_in_metaDataExpr1057); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA38_tree = (Object)adaptor.create(COMMA38);
                    adaptor.addChild(root_0, COMMA38_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_metaDataExpr1061);
                    f1=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, f1.getTree());
                    RPAREN39=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1063); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN39_tree = (Object)adaptor.create(RPAREN39);
                    adaptor.addChild(root_0, RPAREN39_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (f1!=null?f1.value:null)); 
                    }

                    }
                    break;
                case 8 :
                    // src/java/grammar/wcps.g:117:7: op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,INTERPOLATIONSET,FOLLOW_INTERPOLATIONSET_in_metaDataExpr1075); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN40=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr1077); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN40_tree = (Object)adaptor.create(LPAREN40);
                    adaptor.addChild(root_0, LPAREN40_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1081);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA41=(Token)match(input,COMMA,FOLLOW_COMMA_in_metaDataExpr1083); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA41_tree = (Object)adaptor.create(COMMA41);
                    adaptor.addChild(root_0, COMMA41_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_metaDataExpr1087);
                    f1=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, f1.getTree());
                    RPAREN42=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1089); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN42_tree = (Object)adaptor.create(RPAREN42);
                    adaptor.addChild(root_0, RPAREN42_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (f1!=null?f1.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 18, metaDataExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "metaDataExpr"

    public static class domainExpr_return extends ParserRuleReturnScope {
        public DomainExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "domainExpr"
    // src/java/grammar/wcps.g:119:1: domainExpr returns [DomainExpr value] : DOMAIN LPAREN var= variableName COMMA axis= axisName COMMA crs= crsName RPAREN ;
    public final wcpsParser.domainExpr_return domainExpr() throws RecognitionException {
        wcpsParser.domainExpr_return retval = new wcpsParser.domainExpr_return();
        retval.start = input.LT(1);
        int domainExpr_StartIndex = input.index();
        Object root_0 = null;

        Token DOMAIN43=null;
        Token LPAREN44=null;
        Token COMMA45=null;
        Token COMMA46=null;
        Token RPAREN47=null;
        wcpsParser.variableName_return var = null;

        wcpsParser.axisName_return axis = null;

        wcpsParser.crsName_return crs = null;


        Object DOMAIN43_tree=null;
        Object LPAREN44_tree=null;
        Object COMMA45_tree=null;
        Object COMMA46_tree=null;
        Object RPAREN47_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 19) ) { return retval; }
            // src/java/grammar/wcps.g:120:2: ( DOMAIN LPAREN var= variableName COMMA axis= axisName COMMA crs= crsName RPAREN )
            // src/java/grammar/wcps.g:120:4: DOMAIN LPAREN var= variableName COMMA axis= axisName COMMA crs= crsName RPAREN
            {
            root_0 = (Object)adaptor.nil();

            DOMAIN43=(Token)match(input,DOMAIN,FOLLOW_DOMAIN_in_domainExpr1108); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            DOMAIN43_tree = (Object)adaptor.create(DOMAIN43);
            adaptor.addChild(root_0, DOMAIN43_tree);
            }
            LPAREN44=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_domainExpr1110); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN44_tree = (Object)adaptor.create(LPAREN44);
            adaptor.addChild(root_0, LPAREN44_tree);
            }
            pushFollow(FOLLOW_variableName_in_domainExpr1114);
            var=variableName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, var.getTree());
            COMMA45=(Token)match(input,COMMA,FOLLOW_COMMA_in_domainExpr1116); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA45_tree = (Object)adaptor.create(COMMA45);
            adaptor.addChild(root_0, COMMA45_tree);
            }
            pushFollow(FOLLOW_axisName_in_domainExpr1120);
            axis=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, axis.getTree());
            COMMA46=(Token)match(input,COMMA,FOLLOW_COMMA_in_domainExpr1122); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA46_tree = (Object)adaptor.create(COMMA46);
            adaptor.addChild(root_0, COMMA46_tree);
            }
            pushFollow(FOLLOW_crsName_in_domainExpr1126);
            crs=crsName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
            RPAREN47=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_domainExpr1128); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN47_tree = (Object)adaptor.create(RPAREN47);
            adaptor.addChild(root_0, RPAREN47_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new DomainExpr((var!=null?var.value:null), (axis!=null?axis.value:null), (crs!=null?crs.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 19, domainExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "domainExpr"

    public static class condenseExpr_return extends ParserRuleReturnScope {
        public CondenseExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "condenseExpr"
    // src/java/grammar/wcps.g:122:1: condenseExpr returns [CondenseExpr value] : (e1= reduceExpr | e2= generalCondenseExpr );
    public final wcpsParser.condenseExpr_return condenseExpr() throws RecognitionException {
        wcpsParser.condenseExpr_return retval = new wcpsParser.condenseExpr_return();
        retval.start = input.LT(1);
        int condenseExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.reduceExpr_return e1 = null;

        wcpsParser.generalCondenseExpr_return e2 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 20) ) { return retval; }
            // src/java/grammar/wcps.g:123:2: (e1= reduceExpr | e2= generalCondenseExpr )
            int alt17=2;
            int LA17_0 = input.LA(1);

            if ( ((LA17_0>=ALL && LA17_0<=MAX)) ) {
                alt17=1;
            }
            else if ( (LA17_0==CONDENSE) ) {
                alt17=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 17, 0, input);

                throw nvae;
            }
            switch (alt17) {
                case 1 :
                    // src/java/grammar/wcps.g:123:4: e1= reduceExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_reduceExpr_in_condenseExpr1145);
                    e1=reduceExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CondenseExpr((e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:124:4: e2= generalCondenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_generalCondenseExpr_in_condenseExpr1154);
                    e2=generalCondenseExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CondenseExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 20, condenseExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "condenseExpr"

    public static class reduceExpr_return extends ParserRuleReturnScope {
        public ReduceExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "reduceExpr"
    // src/java/grammar/wcps.g:126:1: reduceExpr returns [ReduceExpr value] : op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.reduceExpr_return reduceExpr() throws RecognitionException {
        wcpsParser.reduceExpr_return retval = new wcpsParser.reduceExpr_return();
        retval.start = input.LT(1);
        int reduceExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN48=null;
        Token RPAREN49=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN48_tree=null;
        Object RPAREN49_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 21) ) { return retval; }
            // src/java/grammar/wcps.g:127:2: (op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN )
            // src/java/grammar/wcps.g:127:4: op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            op=(Token)input.LT(1);
            if ( (input.LA(1)>=ALL && input.LA(1)<=MAX) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            LPAREN48=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_reduceExpr1187); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN48_tree = (Object)adaptor.create(LPAREN48);
            adaptor.addChild(root_0, LPAREN48_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_reduceExpr1191);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN49=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_reduceExpr1193); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN49_tree = (Object)adaptor.create(RPAREN49);
            adaptor.addChild(root_0, RPAREN49_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new ReduceExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 21, reduceExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "reduceExpr"

    public static class generalCondenseExpr_return extends ParserRuleReturnScope {
        public GeneralCondenseExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "generalCondenseExpr"
    // src/java/grammar/wcps.g:129:1: generalCondenseExpr returns [GeneralCondenseExpr value] : CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING se= scalarExpr ;
    public final wcpsParser.generalCondenseExpr_return generalCondenseExpr() throws RecognitionException {
        wcpsParser.generalCondenseExpr_return retval = new wcpsParser.generalCondenseExpr_return();
        retval.start = input.LT(1);
        int generalCondenseExpr_StartIndex = input.index();
        Object root_0 = null;

        Token CONDENSE50=null;
        Token OVER51=null;
        Token WHERE52=null;
        Token USING53=null;
        wcpsParser.condenseOpType_return op = null;

        wcpsParser.axisIteratorList_return ail = null;

        wcpsParser.booleanScalarExpr_return cond = null;

        wcpsParser.scalarExpr_return se = null;


        Object CONDENSE50_tree=null;
        Object OVER51_tree=null;
        Object WHERE52_tree=null;
        Object USING53_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 22) ) { return retval; }
            // src/java/grammar/wcps.g:130:2: ( CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING se= scalarExpr )
            // src/java/grammar/wcps.g:130:4: CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING se= scalarExpr
            {
            root_0 = (Object)adaptor.nil();

            CONDENSE50=(Token)match(input,CONDENSE,FOLLOW_CONDENSE_in_generalCondenseExpr1208); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            CONDENSE50_tree = (Object)adaptor.create(CONDENSE50);
            adaptor.addChild(root_0, CONDENSE50_tree);
            }
            pushFollow(FOLLOW_condenseOpType_in_generalCondenseExpr1212);
            op=condenseOpType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, op.getTree());
            OVER51=(Token)match(input,OVER,FOLLOW_OVER_in_generalCondenseExpr1214); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            OVER51_tree = (Object)adaptor.create(OVER51);
            adaptor.addChild(root_0, OVER51_tree);
            }
            pushFollow(FOLLOW_axisIteratorList_in_generalCondenseExpr1218);
            ail=axisIteratorList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ail.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new GeneralCondenseExpr((op!=null?op.value:null), (ail!=null?ail.value:null)); 
            }
            // src/java/grammar/wcps.g:131:3: ( WHERE cond= booleanScalarExpr )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0==WHERE) ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // src/java/grammar/wcps.g:131:4: WHERE cond= booleanScalarExpr
                    {
                    WHERE52=(Token)match(input,WHERE,FOLLOW_WHERE_in_generalCondenseExpr1225); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    WHERE52_tree = (Object)adaptor.create(WHERE52);
                    adaptor.addChild(root_0, WHERE52_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarExpr_in_generalCondenseExpr1229);
                    cond=booleanScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cond.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.setWhere((cond!=null?cond.value:null)); 
                    }

                    }
                    break;

            }

            USING53=(Token)match(input,USING,FOLLOW_USING_in_generalCondenseExpr1237); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            USING53_tree = (Object)adaptor.create(USING53);
            adaptor.addChild(root_0, USING53_tree);
            }
            pushFollow(FOLLOW_scalarExpr_in_generalCondenseExpr1241);
            se=scalarExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, se.getTree());
            if ( state.backtracking==0 ) {
               retval.value.setUsing((se!=null?se.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 22, generalCondenseExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "generalCondenseExpr"

    public static class axisIteratorList_return extends ParserRuleReturnScope {
        public AxisIteratorList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "axisIteratorList"
    // src/java/grammar/wcps.g:134:1: axisIteratorList returns [AxisIteratorList value] : vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )* ;
    public final wcpsParser.axisIteratorList_return axisIteratorList() throws RecognitionException {
        wcpsParser.axisIteratorList_return retval = new wcpsParser.axisIteratorList_return();
        retval.start = input.LT(1);
        int axisIteratorList_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN54=null;
        Token RPAREN55=null;
        Token COMMA56=null;
        Token LPAREN57=null;
        Token RPAREN58=null;
        wcpsParser.variableName_return vn = null;

        wcpsParser.axisName_return an = null;

        wcpsParser.intervalExpr_return ie = null;

        wcpsParser.variableName_return vn2 = null;

        wcpsParser.axisName_return an2 = null;

        wcpsParser.intervalExpr_return ie2 = null;


        Object LPAREN54_tree=null;
        Object RPAREN55_tree=null;
        Object COMMA56_tree=null;
        Object LPAREN57_tree=null;
        Object RPAREN58_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 23) ) { return retval; }
            // src/java/grammar/wcps.g:135:2: (vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )* )
            // src/java/grammar/wcps.g:135:4: vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_variableName_in_axisIteratorList1258);
            vn=variableName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, vn.getTree());
            pushFollow(FOLLOW_axisName_in_axisIteratorList1262);
            an=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, an.getTree());
            LPAREN54=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_axisIteratorList1264); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN54_tree = (Object)adaptor.create(LPAREN54);
            adaptor.addChild(root_0, LPAREN54_tree);
            }
            pushFollow(FOLLOW_intervalExpr_in_axisIteratorList1268);
            ie=intervalExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ie.getTree());
            RPAREN55=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_axisIteratorList1270); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN55_tree = (Object)adaptor.create(RPAREN55);
            adaptor.addChild(root_0, RPAREN55_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new AxisIteratorList(new AxisIterator((vn!=null?vn.value:null), (an!=null?an.value:null), (ie!=null?ie.value:null))); 
            }
            // src/java/grammar/wcps.g:137:2: ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )*
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( (LA19_0==COMMA) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // src/java/grammar/wcps.g:137:3: COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN
            	    {
            	    COMMA56=(Token)match(input,COMMA,FOLLOW_COMMA_in_axisIteratorList1278); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA56_tree = (Object)adaptor.create(COMMA56);
            	    adaptor.addChild(root_0, COMMA56_tree);
            	    }
            	    pushFollow(FOLLOW_variableName_in_axisIteratorList1282);
            	    vn2=variableName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, vn2.getTree());
            	    pushFollow(FOLLOW_axisName_in_axisIteratorList1286);
            	    an2=axisName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, an2.getTree());
            	    LPAREN57=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_axisIteratorList1288); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    LPAREN57_tree = (Object)adaptor.create(LPAREN57);
            	    adaptor.addChild(root_0, LPAREN57_tree);
            	    }
            	    pushFollow(FOLLOW_intervalExpr_in_axisIteratorList1292);
            	    ie2=intervalExpr();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, ie2.getTree());
            	    RPAREN58=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_axisIteratorList1294); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    RPAREN58_tree = (Object)adaptor.create(RPAREN58);
            	    adaptor.addChild(root_0, RPAREN58_tree);
            	    }
            	    if ( state.backtracking==0 ) {
            	       retval.value = new AxisIteratorList(new AxisIterator((vn2!=null?vn2.value:null), (an2!=null?an2.value:null), (ie2!=null?ie2.value:null)), retval.value); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop19;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 23, axisIteratorList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "axisIteratorList"

    public static class intervalExpr_return extends ParserRuleReturnScope {
        public IntervalExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "intervalExpr"
    // src/java/grammar/wcps.g:140:1: intervalExpr returns [IntervalExpr value] : (lo= indexExpr COLON hi= indexExpr | IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN );
    public final wcpsParser.intervalExpr_return intervalExpr() throws RecognitionException {
        wcpsParser.intervalExpr_return retval = new wcpsParser.intervalExpr_return();
        retval.start = input.LT(1);
        int intervalExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COLON59=null;
        Token IMAGECRSDOMAIN60=null;
        Token LPAREN61=null;
        Token COMMA62=null;
        Token RPAREN63=null;
        wcpsParser.indexExpr_return lo = null;

        wcpsParser.indexExpr_return hi = null;

        wcpsParser.coverageName_return e1 = null;

        wcpsParser.axisName_return e2 = null;


        Object COLON59_tree=null;
        Object IMAGECRSDOMAIN60_tree=null;
        Object LPAREN61_tree=null;
        Object COMMA62_tree=null;
        Object RPAREN63_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 24) ) { return retval; }
            // src/java/grammar/wcps.g:141:5: (lo= indexExpr COLON hi= indexExpr | IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN )
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==LPAREN||(LA20_0>=INTEGERCONSTANT && LA20_0<=ROUND)) ) {
                alt20=1;
            }
            else if ( (LA20_0==IMAGECRSDOMAIN) ) {
                alt20=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 20, 0, input);

                throw nvae;
            }
            switch (alt20) {
                case 1 :
                    // src/java/grammar/wcps.g:141:7: lo= indexExpr COLON hi= indexExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_indexExpr_in_intervalExpr1318);
                    lo=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, lo.getTree());
                    COLON59=(Token)match(input,COLON,FOLLOW_COLON_in_intervalExpr1320); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON59_tree = (Object)adaptor.create(COLON59);
                    adaptor.addChild(root_0, COLON59_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_intervalExpr1324);
                    hi=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, hi.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new IntervalExpr((lo!=null?lo.value:null), (hi!=null?hi.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:143:7: IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    IMAGECRSDOMAIN60=(Token)match(input,IMAGECRSDOMAIN,FOLLOW_IMAGECRSDOMAIN_in_intervalExpr1339); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    IMAGECRSDOMAIN60_tree = (Object)adaptor.create(IMAGECRSDOMAIN60);
                    adaptor.addChild(root_0, IMAGECRSDOMAIN60_tree);
                    }
                    LPAREN61=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_intervalExpr1341); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN61_tree = (Object)adaptor.create(LPAREN61);
                    adaptor.addChild(root_0, LPAREN61_tree);
                    }
                    pushFollow(FOLLOW_coverageName_in_intervalExpr1345);
                    e1=coverageName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA62=(Token)match(input,COMMA,FOLLOW_COMMA_in_intervalExpr1347); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA62_tree = (Object)adaptor.create(COMMA62);
                    adaptor.addChild(root_0, COMMA62_tree);
                    }
                    pushFollow(FOLLOW_axisName_in_intervalExpr1351);
                    e2=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN63=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_intervalExpr1353); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN63_tree = (Object)adaptor.create(RPAREN63);
                    adaptor.addChild(root_0, RPAREN63_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new IntervalExpr((e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 24, intervalExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "intervalExpr"

    public static class coverageConstantExpr_return extends ParserRuleReturnScope {
        public CoverageConstantExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageConstantExpr"
    // src/java/grammar/wcps.g:146:1: coverageConstantExpr returns [CoverageConstantExpr value] : COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT ;
    public final wcpsParser.coverageConstantExpr_return coverageConstantExpr() throws RecognitionException {
        wcpsParser.coverageConstantExpr_return retval = new wcpsParser.coverageConstantExpr_return();
        retval.start = input.LT(1);
        int coverageConstantExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COVERAGE64=null;
        Token OVER65=null;
        Token VALUE66=null;
        Token LIST67=null;
        Token LT68=null;
        Token GT69=null;
        wcpsParser.coverageName_return aname = null;

        wcpsParser.axisIteratorList_return iter = null;

        wcpsParser.constantList_return values = null;


        Object COVERAGE64_tree=null;
        Object OVER65_tree=null;
        Object VALUE66_tree=null;
        Object LIST67_tree=null;
        Object LT68_tree=null;
        Object GT69_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 25) ) { return retval; }
            // src/java/grammar/wcps.g:147:5: ( COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT )
            // src/java/grammar/wcps.g:147:7: COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT
            {
            root_0 = (Object)adaptor.nil();

            COVERAGE64=(Token)match(input,COVERAGE,FOLLOW_COVERAGE_in_coverageConstantExpr1379); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COVERAGE64_tree = (Object)adaptor.create(COVERAGE64);
            adaptor.addChild(root_0, COVERAGE64_tree);
            }
            pushFollow(FOLLOW_coverageName_in_coverageConstantExpr1383);
            aname=coverageName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            OVER65=(Token)match(input,OVER,FOLLOW_OVER_in_coverageConstantExpr1385); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            OVER65_tree = (Object)adaptor.create(OVER65);
            adaptor.addChild(root_0, OVER65_tree);
            }
            pushFollow(FOLLOW_axisIteratorList_in_coverageConstantExpr1389);
            iter=axisIteratorList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, iter.getTree());
            VALUE66=(Token)match(input,VALUE,FOLLOW_VALUE_in_coverageConstantExpr1391); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            VALUE66_tree = (Object)adaptor.create(VALUE66);
            adaptor.addChild(root_0, VALUE66_tree);
            }
            LIST67=(Token)match(input,LIST,FOLLOW_LIST_in_coverageConstantExpr1393); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LIST67_tree = (Object)adaptor.create(LIST67);
            adaptor.addChild(root_0, LIST67_tree);
            }
            LT68=(Token)match(input,LT,FOLLOW_LT_in_coverageConstantExpr1395); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LT68_tree = (Object)adaptor.create(LT68);
            adaptor.addChild(root_0, LT68_tree);
            }
            pushFollow(FOLLOW_constantList_in_coverageConstantExpr1399);
            values=constantList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, values.getTree());
            GT69=(Token)match(input,GT,FOLLOW_GT_in_coverageConstantExpr1401); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            GT69_tree = (Object)adaptor.create(GT69);
            adaptor.addChild(root_0, GT69_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new CoverageConstantExpr((aname!=null?aname.value:null), (iter!=null?iter.value:null), (values!=null?values.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 25, coverageConstantExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageConstantExpr"

    public static class constantList_return extends ParserRuleReturnScope {
        public ConstantList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "constantList"
    // src/java/grammar/wcps.g:150:1: constantList returns [ConstantList value] : c= constant ( SEMICOLON c= constant )* ;
    public final wcpsParser.constantList_return constantList() throws RecognitionException {
        wcpsParser.constantList_return retval = new wcpsParser.constantList_return();
        retval.start = input.LT(1);
        int constantList_StartIndex = input.index();
        Object root_0 = null;

        Token SEMICOLON70=null;
        wcpsParser.constant_return c = null;


        Object SEMICOLON70_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 26) ) { return retval; }
            // src/java/grammar/wcps.g:151:5: (c= constant ( SEMICOLON c= constant )* )
            // src/java/grammar/wcps.g:151:7: c= constant ( SEMICOLON c= constant )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_constant_in_constantList1432);
            c=constant();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, c.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new ConstantList((c!=null?c.value:null)); 
            }
            // src/java/grammar/wcps.g:151:59: ( SEMICOLON c= constant )*
            loop21:
            do {
                int alt21=2;
                int LA21_0 = input.LA(1);

                if ( (LA21_0==SEMICOLON) ) {
                    alt21=1;
                }


                switch (alt21) {
            	case 1 :
            	    // src/java/grammar/wcps.g:151:60: SEMICOLON c= constant
            	    {
            	    SEMICOLON70=(Token)match(input,SEMICOLON,FOLLOW_SEMICOLON_in_constantList1437); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    SEMICOLON70_tree = (Object)adaptor.create(SEMICOLON70);
            	    adaptor.addChild(root_0, SEMICOLON70_tree);
            	    }
            	    pushFollow(FOLLOW_constant_in_constantList1441);
            	    c=constant();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, c.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value.add((c!=null?c.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop21;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 26, constantList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "constantList"

    public static class coverageConstructorExpr_return extends ParserRuleReturnScope {
        public CoverageConstructorExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageConstructorExpr"
    // src/java/grammar/wcps.g:153:1: coverageConstructorExpr returns [CoverageConstructorExpr value] : COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr ;
    public final wcpsParser.coverageConstructorExpr_return coverageConstructorExpr() throws RecognitionException {
        wcpsParser.coverageConstructorExpr_return retval = new wcpsParser.coverageConstructorExpr_return();
        retval.start = input.LT(1);
        int coverageConstructorExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COVERAGE71=null;
        Token OVER72=null;
        Token VALUES73=null;
        wcpsParser.coverageName_return coverage = null;

        wcpsParser.axisIteratorList_return ail = null;

        wcpsParser.scalarExpr_return se = null;


        Object COVERAGE71_tree=null;
        Object OVER72_tree=null;
        Object VALUES73_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 27) ) { return retval; }
            // src/java/grammar/wcps.g:154:2: ( COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr )
            // src/java/grammar/wcps.g:154:4: COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr
            {
            root_0 = (Object)adaptor.nil();

            COVERAGE71=(Token)match(input,COVERAGE,FOLLOW_COVERAGE_in_coverageConstructorExpr1461); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COVERAGE71_tree = (Object)adaptor.create(COVERAGE71);
            adaptor.addChild(root_0, COVERAGE71_tree);
            }
            pushFollow(FOLLOW_coverageName_in_coverageConstructorExpr1465);
            coverage=coverageName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, coverage.getTree());
            OVER72=(Token)match(input,OVER,FOLLOW_OVER_in_coverageConstructorExpr1467); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            OVER72_tree = (Object)adaptor.create(OVER72);
            adaptor.addChild(root_0, OVER72_tree);
            }
            pushFollow(FOLLOW_axisIteratorList_in_coverageConstructorExpr1471);
            ail=axisIteratorList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ail.getTree());
            VALUES73=(Token)match(input,VALUES,FOLLOW_VALUES_in_coverageConstructorExpr1473); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            VALUES73_tree = (Object)adaptor.create(VALUES73);
            adaptor.addChild(root_0, VALUES73_tree);
            }
            pushFollow(FOLLOW_scalarExpr_in_coverageConstructorExpr1477);
            se=scalarExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, se.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new CoverageConstructorExpr((coverage!=null?coverage.value:null), (ail!=null?ail.value:null), (se!=null?se.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 27, coverageConstructorExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageConstructorExpr"

    public static class setMetaDataExpr_return extends ParserRuleReturnScope {
        public SetMetaDataExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "setMetaDataExpr"
    // src/java/grammar/wcps.g:157:1: setMetaDataExpr returns [SetMetaDataExpr value] : (op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN | op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN | op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN | op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN | op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN );
    public final wcpsParser.setMetaDataExpr_return setMetaDataExpr() throws RecognitionException {
        wcpsParser.setMetaDataExpr_return retval = new wcpsParser.setMetaDataExpr_return();
        retval.start = input.LT(1);
        int setMetaDataExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN74=null;
        Token COMMA75=null;
        Token RPAREN76=null;
        Token LPAREN77=null;
        Token COMMA78=null;
        Token RPAREN79=null;
        Token LPAREN80=null;
        Token COMMA81=null;
        Token RPAREN82=null;
        Token LPAREN83=null;
        Token COMMA84=null;
        Token COMMA85=null;
        Token RPAREN86=null;
        Token LPAREN87=null;
        Token COMMA88=null;
        Token COMMA89=null;
        Token RPAREN90=null;
        wcpsParser.stringConstant_return s = null;

        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.crsList_return crs = null;

        wcpsParser.rangeExprList_return rel = null;

        wcpsParser.fieldName_return fn = null;

        wcpsParser.interpolationMethod_return im = null;

        wcpsParser.interpolationMethodList_return iml = null;


        Object op_tree=null;
        Object LPAREN74_tree=null;
        Object COMMA75_tree=null;
        Object RPAREN76_tree=null;
        Object LPAREN77_tree=null;
        Object COMMA78_tree=null;
        Object RPAREN79_tree=null;
        Object LPAREN80_tree=null;
        Object COMMA81_tree=null;
        Object RPAREN82_tree=null;
        Object LPAREN83_tree=null;
        Object COMMA84_tree=null;
        Object COMMA85_tree=null;
        Object RPAREN86_tree=null;
        Object LPAREN87_tree=null;
        Object COMMA88_tree=null;
        Object COMMA89_tree=null;
        Object RPAREN90_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 28) ) { return retval; }
            // src/java/grammar/wcps.g:158:5: (op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN | op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN | op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN | op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN | op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN )
            int alt22=5;
            switch ( input.LA(1) ) {
            case SETIDENTIFIER:
                {
                alt22=1;
                }
                break;
            case SETCRSSET:
                {
                alt22=2;
                }
                break;
            case SETNULLSET:
                {
                alt22=3;
                }
                break;
            case SETINTERPOLATIONDEFAULT:
                {
                alt22=4;
                }
                break;
            case SETINTERPOLATIONSET:
                {
                alt22=5;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 22, 0, input);

                throw nvae;
            }

            switch (alt22) {
                case 1 :
                    // src/java/grammar/wcps.g:158:7: op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETIDENTIFIER,FOLLOW_SETIDENTIFIER_in_setMetaDataExpr1499); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN74=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1501); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN74_tree = (Object)adaptor.create(LPAREN74);
                    adaptor.addChild(root_0, LPAREN74_tree);
                    }
                    pushFollow(FOLLOW_stringConstant_in_setMetaDataExpr1505);
                    s=stringConstant();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, s.getTree());
                    COMMA75=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1507); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA75_tree = (Object)adaptor.create(COMMA75);
                    adaptor.addChild(root_0, COMMA75_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1511);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN76=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1513); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN76_tree = (Object)adaptor.create(RPAREN76);
                    adaptor.addChild(root_0, RPAREN76_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (s!=null?s.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:160:7: op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETCRSSET,FOLLOW_SETCRSSET_in_setMetaDataExpr1526); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN77=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1528); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN77_tree = (Object)adaptor.create(LPAREN77);
                    adaptor.addChild(root_0, LPAREN77_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1532);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA78=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1534); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA78_tree = (Object)adaptor.create(COMMA78);
                    adaptor.addChild(root_0, COMMA78_tree);
                    }
                    pushFollow(FOLLOW_crsList_in_setMetaDataExpr1538);
                    crs=crsList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    RPAREN79=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1540); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN79_tree = (Object)adaptor.create(RPAREN79);
                    adaptor.addChild(root_0, RPAREN79_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (crs!=null?crs.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:162:7: op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETNULLSET,FOLLOW_SETNULLSET_in_setMetaDataExpr1557); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN80=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1559); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN80_tree = (Object)adaptor.create(LPAREN80);
                    adaptor.addChild(root_0, LPAREN80_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1563);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA81=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1565); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA81_tree = (Object)adaptor.create(COMMA81);
                    adaptor.addChild(root_0, COMMA81_tree);
                    }
                    pushFollow(FOLLOW_rangeExprList_in_setMetaDataExpr1569);
                    rel=rangeExprList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, rel.getTree());
                    RPAREN82=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1571); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN82_tree = (Object)adaptor.create(RPAREN82);
                    adaptor.addChild(root_0, RPAREN82_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (rel!=null?rel.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:164:7: op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETINTERPOLATIONDEFAULT,FOLLOW_SETINTERPOLATIONDEFAULT_in_setMetaDataExpr1588); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN83=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1590); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN83_tree = (Object)adaptor.create(LPAREN83);
                    adaptor.addChild(root_0, LPAREN83_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1594);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA84=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1596); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA84_tree = (Object)adaptor.create(COMMA84);
                    adaptor.addChild(root_0, COMMA84_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_setMetaDataExpr1600);
                    fn=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, fn.getTree());
                    COMMA85=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1602); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA85_tree = (Object)adaptor.create(COMMA85);
                    adaptor.addChild(root_0, COMMA85_tree);
                    }
                    pushFollow(FOLLOW_interpolationMethod_in_setMetaDataExpr1606);
                    im=interpolationMethod();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, im.getTree());
                    RPAREN86=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1608); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN86_tree = (Object)adaptor.create(RPAREN86);
                    adaptor.addChild(root_0, RPAREN86_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (im!=null?im.value:null), (fn!=null?fn.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:166:7: op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETINTERPOLATIONSET,FOLLOW_SETINTERPOLATIONSET_in_setMetaDataExpr1628); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN87=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1630); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN87_tree = (Object)adaptor.create(LPAREN87);
                    adaptor.addChild(root_0, LPAREN87_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1634);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA88=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1636); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA88_tree = (Object)adaptor.create(COMMA88);
                    adaptor.addChild(root_0, COMMA88_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_setMetaDataExpr1640);
                    fn=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, fn.getTree());
                    COMMA89=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1642); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA89_tree = (Object)adaptor.create(COMMA89);
                    adaptor.addChild(root_0, COMMA89_tree);
                    }
                    pushFollow(FOLLOW_interpolationMethodList_in_setMetaDataExpr1646);
                    iml=interpolationMethodList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, iml.getTree());
                    RPAREN90=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1648); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN90_tree = (Object)adaptor.create(RPAREN90);
                    adaptor.addChild(root_0, RPAREN90_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (iml!=null?iml.value:null), (fn!=null?fn.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 28, setMetaDataExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "setMetaDataExpr"

    public static class crsList_return extends ParserRuleReturnScope {
        public CrsList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "crsList"
    // src/java/grammar/wcps.g:169:1: crsList returns [CrsList value] : LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE ;
    public final wcpsParser.crsList_return crsList() throws RecognitionException {
        wcpsParser.crsList_return retval = new wcpsParser.crsList_return();
        retval.start = input.LT(1);
        int crsList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE91=null;
        Token COMMA92=null;
        Token RBRACE93=null;
        wcpsParser.crsName_return crs = null;


        Object LBRACE91_tree=null;
        Object COMMA92_tree=null;
        Object RBRACE93_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 29) ) { return retval; }
            // src/java/grammar/wcps.g:170:5: ( LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE )
            // src/java/grammar/wcps.g:170:7: LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE91=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_crsList1677); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE91_tree = (Object)adaptor.create(LBRACE91);
            adaptor.addChild(root_0, LBRACE91_tree);
            }
            if ( state.backtracking==0 ) {
              retval.value = new CrsList();
            }
            // src/java/grammar/wcps.g:170:40: (crs= crsName ( COMMA crs= crsName )* )?
            int alt24=2;
            int LA24_0 = input.LA(1);

            if ( (LA24_0==STRING) ) {
                alt24=1;
            }
            switch (alt24) {
                case 1 :
                    // src/java/grammar/wcps.g:170:41: crs= crsName ( COMMA crs= crsName )*
                    {
                    pushFollow(FOLLOW_crsName_in_crsList1684);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((crs!=null?crs.value:null)); 
                    }
                    // src/java/grammar/wcps.g:170:81: ( COMMA crs= crsName )*
                    loop23:
                    do {
                        int alt23=2;
                        int LA23_0 = input.LA(1);

                        if ( (LA23_0==COMMA) ) {
                            alt23=1;
                        }


                        switch (alt23) {
                    	case 1 :
                    	    // src/java/grammar/wcps.g:170:82: COMMA crs= crsName
                    	    {
                    	    COMMA92=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsList1689); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA92_tree = (Object)adaptor.create(COMMA92);
                    	    adaptor.addChild(root_0, COMMA92_tree);
                    	    }
                    	    pushFollow(FOLLOW_crsName_in_crsList1693);
                    	    crs=crsName();

                    	    state._fsp--;
                    	    if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    	    if ( state.backtracking==0 ) {
                    	       retval.value.add((crs!=null?crs.value:null)); 
                    	    }

                    	    }
                    	    break;

                    	default :
                    	    break loop23;
                        }
                    } while (true);


                    }
                    break;

            }

            RBRACE93=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_crsList1702); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE93_tree = (Object)adaptor.create(RBRACE93);
            adaptor.addChild(root_0, RBRACE93_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 29, crsList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "crsList"

    public static class rangeExprList_return extends ParserRuleReturnScope {
        public RangeExprList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "rangeExprList"
    // src/java/grammar/wcps.g:172:1: rangeExprList returns [RangeExprList value] : LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE ;
    public final wcpsParser.rangeExprList_return rangeExprList() throws RecognitionException {
        wcpsParser.rangeExprList_return retval = new wcpsParser.rangeExprList_return();
        retval.start = input.LT(1);
        int rangeExprList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE94=null;
        Token COMMA95=null;
        Token RBRACE96=null;
        wcpsParser.rangeExpr_return re1 = null;

        wcpsParser.rangeExpr_return re2 = null;


        Object LBRACE94_tree=null;
        Object COMMA95_tree=null;
        Object RBRACE96_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 30) ) { return retval; }
            // src/java/grammar/wcps.g:173:5: ( LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE )
            // src/java/grammar/wcps.g:173:7: LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE94=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_rangeExprList1721); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE94_tree = (Object)adaptor.create(LBRACE94);
            adaptor.addChild(root_0, LBRACE94_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new RangeExprList(); 
            }
            // src/java/grammar/wcps.g:173:48: (re1= rangeExpr ( COMMA re2= rangeExpr )* )?
            int alt26=2;
            int LA26_0 = input.LA(1);

            if ( (LA26_0==STRUCT) ) {
                alt26=1;
            }
            switch (alt26) {
                case 1 :
                    // src/java/grammar/wcps.g:173:49: re1= rangeExpr ( COMMA re2= rangeExpr )*
                    {
                    pushFollow(FOLLOW_rangeExpr_in_rangeExprList1728);
                    re1=rangeExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, re1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((re1!=null?re1.value:null)); 
                    }
                    // src/java/grammar/wcps.g:173:91: ( COMMA re2= rangeExpr )*
                    loop25:
                    do {
                        int alt25=2;
                        int LA25_0 = input.LA(1);

                        if ( (LA25_0==COMMA) ) {
                            alt25=1;
                        }


                        switch (alt25) {
                    	case 1 :
                    	    // src/java/grammar/wcps.g:173:92: COMMA re2= rangeExpr
                    	    {
                    	    COMMA95=(Token)match(input,COMMA,FOLLOW_COMMA_in_rangeExprList1733); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA95_tree = (Object)adaptor.create(COMMA95);
                    	    adaptor.addChild(root_0, COMMA95_tree);
                    	    }
                    	    pushFollow(FOLLOW_rangeExpr_in_rangeExprList1737);
                    	    re2=rangeExpr();

                    	    state._fsp--;
                    	    if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) adaptor.addChild(root_0, re2.getTree());
                    	    if ( state.backtracking==0 ) {
                    	       retval.value.add((re2!=null?re2.value:null)); 
                    	    }

                    	    }
                    	    break;

                    	default :
                    	    break loop25;
                        }
                    } while (true);


                    }
                    break;

            }

            RBRACE96=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_rangeExprList1746); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE96_tree = (Object)adaptor.create(RBRACE96);
            adaptor.addChild(root_0, RBRACE96_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 30, rangeExprList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "rangeExprList"

    public static class interpolationMethodList_return extends ParserRuleReturnScope {
        public InterpolationMethodList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "interpolationMethodList"
    // src/java/grammar/wcps.g:175:1: interpolationMethodList returns [InterpolationMethodList value] : LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE ;
    public final wcpsParser.interpolationMethodList_return interpolationMethodList() throws RecognitionException {
        wcpsParser.interpolationMethodList_return retval = new wcpsParser.interpolationMethodList_return();
        retval.start = input.LT(1);
        int interpolationMethodList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE97=null;
        Token COMMA98=null;
        Token RBRACE99=null;
        wcpsParser.interpolationMethod_return e = null;


        Object LBRACE97_tree=null;
        Object COMMA98_tree=null;
        Object RBRACE99_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 31) ) { return retval; }
            // src/java/grammar/wcps.g:176:2: ( LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE )
            // src/java/grammar/wcps.g:176:4: LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE97=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_interpolationMethodList1762); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE97_tree = (Object)adaptor.create(LBRACE97);
            adaptor.addChild(root_0, LBRACE97_tree);
            }
            if ( state.backtracking==0 ) {
              retval.value = new InterpolationMethodList();
            }
            // src/java/grammar/wcps.g:176:53: (e= interpolationMethod ( COMMA e= interpolationMethod )* )?
            int alt28=2;
            int LA28_0 = input.LA(1);

            if ( (LA28_0==LPAREN) ) {
                alt28=1;
            }
            switch (alt28) {
                case 1 :
                    // src/java/grammar/wcps.g:176:54: e= interpolationMethod ( COMMA e= interpolationMethod )*
                    {
                    pushFollow(FOLLOW_interpolationMethod_in_interpolationMethodList1769);
                    e=interpolationMethod();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((e!=null?e.value:null)); 
                    }
                    // src/java/grammar/wcps.g:176:102: ( COMMA e= interpolationMethod )*
                    loop27:
                    do {
                        int alt27=2;
                        int LA27_0 = input.LA(1);

                        if ( (LA27_0==COMMA) ) {
                            alt27=1;
                        }


                        switch (alt27) {
                    	case 1 :
                    	    // src/java/grammar/wcps.g:176:103: COMMA e= interpolationMethod
                    	    {
                    	    COMMA98=(Token)match(input,COMMA,FOLLOW_COMMA_in_interpolationMethodList1774); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA98_tree = (Object)adaptor.create(COMMA98);
                    	    adaptor.addChild(root_0, COMMA98_tree);
                    	    }
                    	    pushFollow(FOLLOW_interpolationMethod_in_interpolationMethodList1778);
                    	    e=interpolationMethod();

                    	    state._fsp--;
                    	    if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e.getTree());
                    	    if ( state.backtracking==0 ) {
                    	       retval.value.add((e!=null?e.value:null)); 
                    	    }

                    	    }
                    	    break;

                    	default :
                    	    break loop27;
                        }
                    } while (true);


                    }
                    break;

            }

            RBRACE99=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_interpolationMethodList1786); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE99_tree = (Object)adaptor.create(RBRACE99);
            adaptor.addChild(root_0, RBRACE99_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 31, interpolationMethodList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "interpolationMethodList"

    public static class rangeExpr_return extends ParserRuleReturnScope {
        public RangeExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "rangeExpr"
    // src/java/grammar/wcps.g:178:1: rangeExpr returns [RangeExpr value] : STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )? RBRACE ;
    public final wcpsParser.rangeExpr_return rangeExpr() throws RecognitionException {
        wcpsParser.rangeExpr_return retval = new wcpsParser.rangeExpr_return();
        retval.start = input.LT(1);
        int rangeExpr_StartIndex = input.index();
        Object root_0 = null;

        Token STRUCT100=null;
        Token LBRACE101=null;
        Token COLON102=null;
        Token COLON103=null;
        Token COLON104=null;
        Token RBRACE105=null;
        wcpsParser.fieldName_return field = null;

        wcpsParser.scalarExpr_return expr = null;


        Object STRUCT100_tree=null;
        Object LBRACE101_tree=null;
        Object COLON102_tree=null;
        Object COLON103_tree=null;
        Object COLON104_tree=null;
        Object RBRACE105_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 32) ) { return retval; }
            // src/java/grammar/wcps.g:179:2: ( STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )? RBRACE )
            // src/java/grammar/wcps.g:179:4: STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            STRUCT100=(Token)match(input,STRUCT,FOLLOW_STRUCT_in_rangeExpr1799); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            STRUCT100_tree = (Object)adaptor.create(STRUCT100);
            adaptor.addChild(root_0, STRUCT100_tree);
            }
            LBRACE101=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_rangeExpr1801); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE101_tree = (Object)adaptor.create(LBRACE101);
            adaptor.addChild(root_0, LBRACE101_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value =new RangeExpr(); 
            }
            // src/java/grammar/wcps.g:180:2: (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )?
            int alt30=2;
            int LA30_0 = input.LA(1);

            if ( (LA30_0==INTEGERCONSTANT||LA30_0==STRING||LA30_0==NAME) ) {
                alt30=1;
            }
            switch (alt30) {
                case 1 :
                    // src/java/grammar/wcps.g:180:3: field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )*
                    {
                    pushFollow(FOLLOW_fieldName_in_rangeExpr1809);
                    field=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
                    COLON102=(Token)match(input,COLON,FOLLOW_COLON_in_rangeExpr1811); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON102_tree = (Object)adaptor.create(COLON102);
                    adaptor.addChild(root_0, COLON102_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_rangeExpr1815);
                    expr=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, expr.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((field!=null?field.value:null), (expr!=null?expr.value:null)); 
                    }
                    // src/java/grammar/wcps.g:181:3: ( COLON field= fieldName COLON expr= scalarExpr )*
                    loop29:
                    do {
                        int alt29=2;
                        int LA29_0 = input.LA(1);

                        if ( (LA29_0==COLON) ) {
                            alt29=1;
                        }


                        switch (alt29) {
                    	case 1 :
                    	    // src/java/grammar/wcps.g:181:4: COLON field= fieldName COLON expr= scalarExpr
                    	    {
                    	    COLON103=(Token)match(input,COLON,FOLLOW_COLON_in_rangeExpr1822); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COLON103_tree = (Object)adaptor.create(COLON103);
                    	    adaptor.addChild(root_0, COLON103_tree);
                    	    }
                    	    pushFollow(FOLLOW_fieldName_in_rangeExpr1826);
                    	    field=fieldName();

                    	    state._fsp--;
                    	    if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
                    	    COLON104=(Token)match(input,COLON,FOLLOW_COLON_in_rangeExpr1828); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COLON104_tree = (Object)adaptor.create(COLON104);
                    	    adaptor.addChild(root_0, COLON104_tree);
                    	    }
                    	    pushFollow(FOLLOW_scalarExpr_in_rangeExpr1832);
                    	    expr=scalarExpr();

                    	    state._fsp--;
                    	    if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) adaptor.addChild(root_0, expr.getTree());
                    	    if ( state.backtracking==0 ) {
                    	       retval.value.add((field!=null?field.value:null), (expr!=null?expr.value:null)); 
                    	    }

                    	    }
                    	    break;

                    	default :
                    	    break loop29;
                        }
                    } while (true);


                    }
                    break;

            }

            RBRACE105=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_rangeExpr1843); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE105_tree = (Object)adaptor.create(RBRACE105);
            adaptor.addChild(root_0, RBRACE105_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 32, rangeExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "rangeExpr"

    public static class rangeConstructorExpr_return extends ParserRuleReturnScope {
        public RangeConstructorExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "rangeConstructorExpr"
    // src/java/grammar/wcps.g:184:1: rangeConstructorExpr returns [RangeConstructorExpr value] : ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE ;
    public final wcpsParser.rangeConstructorExpr_return rangeConstructorExpr() throws RecognitionException {
        wcpsParser.rangeConstructorExpr_return retval = new wcpsParser.rangeConstructorExpr_return();
        retval.start = input.LT(1);
        int rangeConstructorExpr_StartIndex = input.index();
        Object root_0 = null;

        Token STRUCT106=null;
        Token LBRACE107=null;
        Token COLON108=null;
        Token SEMICOLON109=null;
        Token COLON110=null;
        Token RBRACE111=null;
        wcpsParser.fieldName_return field = null;

        wcpsParser.coverageExpr_return expr = null;


        Object STRUCT106_tree=null;
        Object LBRACE107_tree=null;
        Object COLON108_tree=null;
        Object SEMICOLON109_tree=null;
        Object COLON110_tree=null;
        Object RBRACE111_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 33) ) { return retval; }
            // src/java/grammar/wcps.g:185:5: ( ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE )
            // src/java/grammar/wcps.g:185:7: ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            // src/java/grammar/wcps.g:185:7: ( STRUCT )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==STRUCT) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // src/java/grammar/wcps.g:185:8: STRUCT
                    {
                    STRUCT106=(Token)match(input,STRUCT,FOLLOW_STRUCT_in_rangeConstructorExpr1860); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    STRUCT106_tree = (Object)adaptor.create(STRUCT106);
                    adaptor.addChild(root_0, STRUCT106_tree);
                    }

                    }
                    break;

            }

            LBRACE107=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_rangeConstructorExpr1864); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE107_tree = (Object)adaptor.create(LBRACE107);
            adaptor.addChild(root_0, LBRACE107_tree);
            }
            pushFollow(FOLLOW_fieldName_in_rangeConstructorExpr1868);
            field=fieldName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
            COLON108=(Token)match(input,COLON,FOLLOW_COLON_in_rangeConstructorExpr1870); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COLON108_tree = (Object)adaptor.create(COLON108);
            adaptor.addChild(root_0, COLON108_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_rangeConstructorExpr1874);
            expr=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, expr.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new RangeConstructorExpr((field!=null?field.value:null), (expr!=null?expr.value:null)); 
            }
            // src/java/grammar/wcps.g:186:9: ( SEMICOLON field= fieldName COLON expr= coverageExpr )*
            loop32:
            do {
                int alt32=2;
                int LA32_0 = input.LA(1);

                if ( (LA32_0==SEMICOLON) ) {
                    alt32=1;
                }


                switch (alt32) {
            	case 1 :
            	    // src/java/grammar/wcps.g:186:10: SEMICOLON field= fieldName COLON expr= coverageExpr
            	    {
            	    SEMICOLON109=(Token)match(input,SEMICOLON,FOLLOW_SEMICOLON_in_rangeConstructorExpr1887); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    SEMICOLON109_tree = (Object)adaptor.create(SEMICOLON109);
            	    adaptor.addChild(root_0, SEMICOLON109_tree);
            	    }
            	    pushFollow(FOLLOW_fieldName_in_rangeConstructorExpr1891);
            	    field=fieldName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
            	    COLON110=(Token)match(input,COLON,FOLLOW_COLON_in_rangeConstructorExpr1893); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COLON110_tree = (Object)adaptor.create(COLON110);
            	    adaptor.addChild(root_0, COLON110_tree);
            	    }
            	    pushFollow(FOLLOW_coverageExpr_in_rangeConstructorExpr1897);
            	    expr=coverageExpr();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, expr.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value.add((field!=null?field.value:null), (expr!=null?expr.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop32;
                }
            } while (true);

            RBRACE111=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_rangeConstructorExpr1903); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE111_tree = (Object)adaptor.create(RBRACE111);
            adaptor.addChild(root_0, RBRACE111_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 33, rangeConstructorExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "rangeConstructorExpr"

    public static class crsTransformExpr_return extends ParserRuleReturnScope {
        public CrsTransformExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "crsTransformExpr"
    // src/java/grammar/wcps.g:188:1: crsTransformExpr returns [CrsTransformExpr value] : CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN ;
    public final wcpsParser.crsTransformExpr_return crsTransformExpr() throws RecognitionException {
        wcpsParser.crsTransformExpr_return retval = new wcpsParser.crsTransformExpr_return();
        retval.start = input.LT(1);
        int crsTransformExpr_StartIndex = input.index();
        Object root_0 = null;

        Token CRSTRANSFORM112=null;
        Token LPAREN113=null;
        Token COMMA114=null;
        Token COMMA115=null;
        Token RPAREN116=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dcl = null;

        wcpsParser.fieldInterpolationList_return fil = null;


        Object CRSTRANSFORM112_tree=null;
        Object LPAREN113_tree=null;
        Object COMMA114_tree=null;
        Object COMMA115_tree=null;
        Object RPAREN116_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 34) ) { return retval; }
            // src/java/grammar/wcps.g:189:2: ( CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN )
            // src/java/grammar/wcps.g:189:4: CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            CRSTRANSFORM112=(Token)match(input,CRSTRANSFORM,FOLLOW_CRSTRANSFORM_in_crsTransformExpr1919); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            CRSTRANSFORM112_tree = (Object)adaptor.create(CRSTRANSFORM112);
            adaptor.addChild(root_0, CRSTRANSFORM112_tree);
            }
            LPAREN113=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_crsTransformExpr1921); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN113_tree = (Object)adaptor.create(LPAREN113);
            adaptor.addChild(root_0, LPAREN113_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_crsTransformExpr1925);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA114=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsTransformExpr1927); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA114_tree = (Object)adaptor.create(COMMA114);
            adaptor.addChild(root_0, COMMA114_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_crsTransformExpr1931);
            dcl=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dcl.getTree());
            COMMA115=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsTransformExpr1933); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA115_tree = (Object)adaptor.create(COMMA115);
            adaptor.addChild(root_0, COMMA115_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationList_in_crsTransformExpr1937);
            fil=fieldInterpolationList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, fil.getTree());
            RPAREN116=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_crsTransformExpr1939); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN116_tree = (Object)adaptor.create(RPAREN116);
            adaptor.addChild(root_0, RPAREN116_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new CrsTransformExpr((e1!=null?e1.value:null), (dcl!=null?dcl.value:null), (fil!=null?fil.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 34, crsTransformExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "crsTransformExpr"

    public static class fieldInterpolationList_return extends ParserRuleReturnScope {
        public FieldInterpolationList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "fieldInterpolationList"
    // src/java/grammar/wcps.g:192:1: fieldInterpolationList returns [FieldInterpolationList value] : LBRACE elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )* RBRACE ;
    public final wcpsParser.fieldInterpolationList_return fieldInterpolationList() throws RecognitionException {
        wcpsParser.fieldInterpolationList_return retval = new wcpsParser.fieldInterpolationList_return();
        retval.start = input.LT(1);
        int fieldInterpolationList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE117=null;
        Token COMMA118=null;
        Token RBRACE119=null;
        wcpsParser.fieldInterpolationElement_return elem = null;


        Object LBRACE117_tree=null;
        Object COMMA118_tree=null;
        Object RBRACE119_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 35) ) { return retval; }
            // src/java/grammar/wcps.g:193:2: ( LBRACE elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )* RBRACE )
            // src/java/grammar/wcps.g:193:4: LBRACE elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE117=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_fieldInterpolationList1956); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE117_tree = (Object)adaptor.create(LBRACE117);
            adaptor.addChild(root_0, LBRACE117_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1960);
            elem=fieldInterpolationElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new FieldInterpolationList((elem!=null?elem.value:null)); 
            }
            // src/java/grammar/wcps.g:194:3: ( COMMA elem= fieldInterpolationElement )*
            loop33:
            do {
                int alt33=2;
                int LA33_0 = input.LA(1);

                if ( (LA33_0==COMMA) ) {
                    alt33=1;
                }


                switch (alt33) {
            	case 1 :
            	    // src/java/grammar/wcps.g:194:4: COMMA elem= fieldInterpolationElement
            	    {
            	    COMMA118=(Token)match(input,COMMA,FOLLOW_COMMA_in_fieldInterpolationList1967); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA118_tree = (Object)adaptor.create(COMMA118);
            	    adaptor.addChild(root_0, COMMA118_tree);
            	    }
            	    pushFollow(FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1971);
            	    elem=fieldInterpolationElement();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value.add((elem!=null?elem.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop33;
                }
            } while (true);

            RBRACE119=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_fieldInterpolationList1978); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE119_tree = (Object)adaptor.create(RBRACE119);
            adaptor.addChild(root_0, RBRACE119_tree);
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 35, fieldInterpolationList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "fieldInterpolationList"

    public static class fieldInterpolationElement_return extends ParserRuleReturnScope {
        public FieldInterpolationElement value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "fieldInterpolationElement"
    // src/java/grammar/wcps.g:196:1: fieldInterpolationElement returns [FieldInterpolationElement value] : aname= fieldName method= interpolationMethod ;
    public final wcpsParser.fieldInterpolationElement_return fieldInterpolationElement() throws RecognitionException {
        wcpsParser.fieldInterpolationElement_return retval = new wcpsParser.fieldInterpolationElement_return();
        retval.start = input.LT(1);
        int fieldInterpolationElement_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.fieldName_return aname = null;

        wcpsParser.interpolationMethod_return method = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 36) ) { return retval; }
            // src/java/grammar/wcps.g:197:2: (aname= fieldName method= interpolationMethod )
            // src/java/grammar/wcps.g:197:4: aname= fieldName method= interpolationMethod
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_fieldName_in_fieldInterpolationElement1993);
            aname=fieldName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            pushFollow(FOLLOW_interpolationMethod_in_fieldInterpolationElement1997);
            method=interpolationMethod();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, method.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new FieldInterpolationElement((aname!=null?aname.value:null), (method!=null?method.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 36, fieldInterpolationElement_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "fieldInterpolationElement"

    public static class unaryInducedExpr_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "unaryInducedExpr"
    // src/java/grammar/wcps.g:199:1: unaryInducedExpr returns [CoverageExpr value] : (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr );
    public final wcpsParser.unaryInducedExpr_return unaryInducedExpr() throws RecognitionException {
        wcpsParser.unaryInducedExpr_return retval = new wcpsParser.unaryInducedExpr_return();
        retval.start = input.LT(1);
        int unaryInducedExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.fieldExpr_return e6 = null;

        wcpsParser.unaryArithmeticExpr_return e1 = null;

        wcpsParser.exponentialExpr_return e2 = null;

        wcpsParser.trigonometricExpr_return e3 = null;

        wcpsParser.booleanExpr_return e4 = null;

        wcpsParser.castExpr_return e5 = null;

        wcpsParser.rangeConstructorExpr_return e7 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 37) ) { return retval; }
            // src/java/grammar/wcps.g:200:5: (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr )
            int alt34=7;
            alt34 = dfa34.predict(input);
            switch (alt34) {
                case 1 :
                    // src/java/grammar/wcps.g:200:7: e6= fieldExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_fieldExpr_in_unaryInducedExpr2017);
                    e6=fieldExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e6.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e6!=null?e6.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:201:4: e1= unaryArithmeticExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_unaryArithmeticExpr_in_unaryInducedExpr2026);
                    e1=unaryArithmeticExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:202:7: e2= exponentialExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_exponentialExpr_in_unaryInducedExpr2038);
                    e2=exponentialExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:203:7: e3= trigonometricExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_trigonometricExpr_in_unaryInducedExpr2050);
                    e3=trigonometricExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:204:7: e4= booleanExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanExpr_in_unaryInducedExpr2062);
                    e4=booleanExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e4!=null?e4.value:null)); 
                    }

                    }
                    break;
                case 6 :
                    // src/java/grammar/wcps.g:205:7: e5= castExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_castExpr_in_unaryInducedExpr2074);
                    e5=castExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e5!=null?e5.value:null)); 
                    }

                    }
                    break;
                case 7 :
                    // src/java/grammar/wcps.g:206:7: e7= rangeConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_rangeConstructorExpr_in_unaryInducedExpr2086);
                    e7=rangeConstructorExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e7.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e7!=null?e7.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 37, unaryInducedExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "unaryInducedExpr"

    public static class unaryArithmeticExpr_return extends ParserRuleReturnScope {
        public CoverageExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "unaryArithmeticExpr"
    // src/java/grammar/wcps.g:208:1: unaryArithmeticExpr returns [CoverageExpr value] : (op= ( MINUS | PLUS ) e1= coverageAtom | op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN );
    public final wcpsParser.unaryArithmeticExpr_return unaryArithmeticExpr() throws RecognitionException {
        wcpsParser.unaryArithmeticExpr_return retval = new wcpsParser.unaryArithmeticExpr_return();
        retval.start = input.LT(1);
        int unaryArithmeticExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN120=null;
        Token RPAREN121=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object op_tree=null;
        Object LPAREN120_tree=null;
        Object RPAREN121_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 38) ) { return retval; }
            // src/java/grammar/wcps.g:209:5: (op= ( MINUS | PLUS ) e1= coverageAtom | op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN )
            int alt35=2;
            int LA35_0 = input.LA(1);

            if ( ((LA35_0>=PLUS && LA35_0<=MINUS)) ) {
                alt35=1;
            }
            else if ( ((LA35_0>=SQRT && LA35_0<=IM)) ) {
                alt35=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 35, 0, input);

                throw nvae;
            }
            switch (alt35) {
                case 1 :
                    // src/java/grammar/wcps.g:209:7: op= ( MINUS | PLUS ) e1= coverageAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)input.LT(1);
                    if ( (input.LA(1)>=PLUS && input.LA(1)<=MINUS) ) {
                        input.consume();
                        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    pushFollow(FOLLOW_coverageAtom_in_unaryArithmeticExpr2117);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:210:7: op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)input.LT(1);
                    if ( (input.LA(1)>=SQRT && input.LA(1)<=IM) ) {
                        input.consume();
                        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    LPAREN120=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_unaryArithmeticExpr2139); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN120_tree = (Object)adaptor.create(LPAREN120);
                    adaptor.addChild(root_0, LPAREN120_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_unaryArithmeticExpr2143);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN121=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_unaryArithmeticExpr2145); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN121_tree = (Object)adaptor.create(RPAREN121);
                    adaptor.addChild(root_0, RPAREN121_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((op!=null?op.getText():null), (e2!=null?e2.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 38, unaryArithmeticExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "unaryArithmeticExpr"

    public static class exponentialExpr_return extends ParserRuleReturnScope {
        public ExponentialExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "exponentialExpr"
    // src/java/grammar/wcps.g:212:1: exponentialExpr returns [ExponentialExpr value] : op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.exponentialExpr_return exponentialExpr() throws RecognitionException {
        wcpsParser.exponentialExpr_return retval = new wcpsParser.exponentialExpr_return();
        retval.start = input.LT(1);
        int exponentialExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN122=null;
        Token RPAREN123=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN122_tree=null;
        Object RPAREN123_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 39) ) { return retval; }
            // src/java/grammar/wcps.g:213:5: (op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN )
            // src/java/grammar/wcps.g:213:7: op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            op=(Token)input.LT(1);
            if ( (input.LA(1)>=EXP && input.LA(1)<=LN) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            LPAREN122=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_exponentialExpr2176); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN122_tree = (Object)adaptor.create(LPAREN122);
            adaptor.addChild(root_0, LPAREN122_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_exponentialExpr2180);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN123=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_exponentialExpr2182); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN123_tree = (Object)adaptor.create(RPAREN123);
            adaptor.addChild(root_0, RPAREN123_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new ExponentialExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 39, exponentialExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "exponentialExpr"

    public static class trigonometricExpr_return extends ParserRuleReturnScope {
        public TrigonometricExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "trigonometricExpr"
    // src/java/grammar/wcps.g:215:1: trigonometricExpr returns [TrigonometricExpr value] : op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.trigonometricExpr_return trigonometricExpr() throws RecognitionException {
        wcpsParser.trigonometricExpr_return retval = new wcpsParser.trigonometricExpr_return();
        retval.start = input.LT(1);
        int trigonometricExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN124=null;
        Token RPAREN125=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN124_tree=null;
        Object RPAREN125_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 40) ) { return retval; }
            // src/java/grammar/wcps.g:216:5: (op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN )
            // src/java/grammar/wcps.g:216:7: op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            op=(Token)input.LT(1);
            if ( (input.LA(1)>=SIN && input.LA(1)<=ARCTAN) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            LPAREN124=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_trigonometricExpr2225); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN124_tree = (Object)adaptor.create(LPAREN124);
            adaptor.addChild(root_0, LPAREN124_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_trigonometricExpr2229);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN125=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_trigonometricExpr2231); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN125_tree = (Object)adaptor.create(RPAREN125);
            adaptor.addChild(root_0, RPAREN125_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new TrigonometricExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 40, trigonometricExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "trigonometricExpr"

    public static class booleanExpr_return extends ParserRuleReturnScope {
        public BooleanExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "booleanExpr"
    // src/java/grammar/wcps.g:218:1: booleanExpr returns [BooleanExpr value] : (op= NOT e1= coverageExpr | op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN );
    public final wcpsParser.booleanExpr_return booleanExpr() throws RecognitionException {
        wcpsParser.booleanExpr_return retval = new wcpsParser.booleanExpr_return();
        retval.start = input.LT(1);
        int booleanExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN126=null;
        Token COMMA127=null;
        Token RPAREN128=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.indexExpr_return e2 = null;


        Object op_tree=null;
        Object LPAREN126_tree=null;
        Object COMMA127_tree=null;
        Object RPAREN128_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 41) ) { return retval; }
            // src/java/grammar/wcps.g:219:5: (op= NOT e1= coverageExpr | op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN )
            int alt36=2;
            int LA36_0 = input.LA(1);

            if ( (LA36_0==NOT) ) {
                alt36=1;
            }
            else if ( (LA36_0==BIT) ) {
                alt36=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 36, 0, input);

                throw nvae;
            }
            switch (alt36) {
                case 1 :
                    // src/java/grammar/wcps.g:219:7: op= NOT e1= coverageExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NOT,FOLLOW_NOT_in_booleanExpr2254); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_booleanExpr2258);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new BooleanExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:220:7: op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,BIT,FOLLOW_BIT_in_booleanExpr2270); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN126=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_booleanExpr2272); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN126_tree = (Object)adaptor.create(LPAREN126);
                    adaptor.addChild(root_0, LPAREN126_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_booleanExpr2276);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA127=(Token)match(input,COMMA,FOLLOW_COMMA_in_booleanExpr2278); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA127_tree = (Object)adaptor.create(COMMA127);
                    adaptor.addChild(root_0, COMMA127_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_booleanExpr2282);
                    e2=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN128=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_booleanExpr2284); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN128_tree = (Object)adaptor.create(RPAREN128);
                    adaptor.addChild(root_0, RPAREN128_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new BooleanExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 41, booleanExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "booleanExpr"

    public static class indexExpr_return extends ParserRuleReturnScope {
        public IndexExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "indexExpr"
    // src/java/grammar/wcps.g:223:1: indexExpr returns [IndexExpr value] : e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )* ;
    public final wcpsParser.indexExpr_return indexExpr() throws RecognitionException {
        wcpsParser.indexExpr_return retval = new wcpsParser.indexExpr_return();
        retval.start = input.LT(1);
        int indexExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token PLUS129=null;
        Token MINUS130=null;
        wcpsParser.indexTerm_return e1 = null;

        wcpsParser.indexTerm_return e2 = null;


        Object op_tree=null;
        Object PLUS129_tree=null;
        Object MINUS130_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 42) ) { return retval; }
            // src/java/grammar/wcps.g:224:5: (e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )* )
            // src/java/grammar/wcps.g:224:7: e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_indexTerm_in_indexExpr2308);
            e1=indexTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:225:3: (op= ( PLUS | MINUS ) e2= indexTerm )*
            loop38:
            do {
                int alt38=2;
                int LA38_0 = input.LA(1);

                if ( ((LA38_0>=PLUS && LA38_0<=MINUS)) ) {
                    alt38=1;
                }


                switch (alt38) {
            	case 1 :
            	    // src/java/grammar/wcps.g:225:4: op= ( PLUS | MINUS ) e2= indexTerm
            	    {
            	    // src/java/grammar/wcps.g:225:7: ( PLUS | MINUS )
            	    int alt37=2;
            	    int LA37_0 = input.LA(1);

            	    if ( (LA37_0==PLUS) ) {
            	        alt37=1;
            	    }
            	    else if ( (LA37_0==MINUS) ) {
            	        alt37=2;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        NoViableAltException nvae =
            	            new NoViableAltException("", 37, 0, input);

            	        throw nvae;
            	    }
            	    switch (alt37) {
            	        case 1 :
            	            // src/java/grammar/wcps.g:225:8: PLUS
            	            {
            	            PLUS129=(Token)match(input,PLUS,FOLLOW_PLUS_in_indexExpr2319); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            PLUS129_tree = (Object)adaptor.create(PLUS129);
            	            root_0 = (Object)adaptor.becomeRoot(PLUS129_tree, root_0);
            	            }

            	            }
            	            break;
            	        case 2 :
            	            // src/java/grammar/wcps.g:225:14: MINUS
            	            {
            	            MINUS130=(Token)match(input,MINUS,FOLLOW_MINUS_in_indexExpr2322); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            MINUS130_tree = (Object)adaptor.create(MINUS130);
            	            root_0 = (Object)adaptor.becomeRoot(MINUS130_tree, root_0);
            	            }

            	            }
            	            break;

            	    }

            	    pushFollow(FOLLOW_indexTerm_in_indexExpr2328);
            	    e2=indexTerm();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new IndexExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop38;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 42, indexExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "indexExpr"

    public static class indexTerm_return extends ParserRuleReturnScope {
        public IndexExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "indexTerm"
    // src/java/grammar/wcps.g:227:1: indexTerm returns [IndexExpr value] : e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor )=> (op= ( MULT | DIVIDE ) e2= indexFactor ) )* ;
    public final wcpsParser.indexTerm_return indexTerm() throws RecognitionException {
        wcpsParser.indexTerm_return retval = new wcpsParser.indexTerm_return();
        retval.start = input.LT(1);
        int indexTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.indexFactor_return e1 = null;

        wcpsParser.indexFactor_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 43) ) { return retval; }
            // src/java/grammar/wcps.g:228:5: (e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor )=> (op= ( MULT | DIVIDE ) e2= indexFactor ) )* )
            // src/java/grammar/wcps.g:228:7: e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor )=> (op= ( MULT | DIVIDE ) e2= indexFactor ) )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_indexFactor_in_indexTerm2353);
            e1=indexFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:229:6: ( (op= ( MULT | DIVIDE ) e2= indexFactor )=> (op= ( MULT | DIVIDE ) e2= indexFactor ) )*
            loop39:
            do {
                int alt39=2;
                int LA39_0 = input.LA(1);

                if ( ((LA39_0>=MULT && LA39_0<=DIVIDE)) && (synpred95_wcps())) {
                    alt39=1;
                }


                switch (alt39) {
            	case 1 :
            	    // src/java/grammar/wcps.g:229:7: (op= ( MULT | DIVIDE ) e2= indexFactor )=> (op= ( MULT | DIVIDE ) e2= indexFactor )
            	    {
            	    // src/java/grammar/wcps.g:229:44: (op= ( MULT | DIVIDE ) e2= indexFactor )
            	    // src/java/grammar/wcps.g:229:45: op= ( MULT | DIVIDE ) e2= indexFactor
            	    {
            	    op=(Token)input.LT(1);
            	    if ( (input.LA(1)>=MULT && input.LA(1)<=DIVIDE) ) {
            	        input.consume();
            	        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
            	        state.errorRecovery=false;state.failed=false;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_indexFactor_in_indexTerm2390);
            	    e2=indexFactor();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new IndexExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }


            	    }
            	    break;

            	default :
            	    break loop39;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 43, indexTerm_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "indexTerm"

    public static class indexFactor_return extends ParserRuleReturnScope {
        public IndexExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "indexFactor"
    // src/java/grammar/wcps.g:231:1: indexFactor returns [IndexExpr value] : (e= INTEGERCONSTANT | op= ROUND LPAREN e1= numericScalarExpr RPAREN | ( LPAREN e2= indexExpr RPAREN )=> ( LPAREN e2= indexExpr RPAREN ) );
    public final wcpsParser.indexFactor_return indexFactor() throws RecognitionException {
        wcpsParser.indexFactor_return retval = new wcpsParser.indexFactor_return();
        retval.start = input.LT(1);
        int indexFactor_StartIndex = input.index();
        Object root_0 = null;

        Token e=null;
        Token op=null;
        Token LPAREN131=null;
        Token RPAREN132=null;
        Token LPAREN133=null;
        Token RPAREN134=null;
        wcpsParser.numericScalarExpr_return e1 = null;

        wcpsParser.indexExpr_return e2 = null;


        Object e_tree=null;
        Object op_tree=null;
        Object LPAREN131_tree=null;
        Object RPAREN132_tree=null;
        Object LPAREN133_tree=null;
        Object RPAREN134_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 44) ) { return retval; }
            // src/java/grammar/wcps.g:232:5: (e= INTEGERCONSTANT | op= ROUND LPAREN e1= numericScalarExpr RPAREN | ( LPAREN e2= indexExpr RPAREN )=> ( LPAREN e2= indexExpr RPAREN ) )
            int alt40=3;
            int LA40_0 = input.LA(1);

            if ( (LA40_0==INTEGERCONSTANT) ) {
                alt40=1;
            }
            else if ( (LA40_0==ROUND) ) {
                alt40=2;
            }
            else if ( (LA40_0==LPAREN) && (synpred99_wcps())) {
                alt40=3;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 40, 0, input);

                throw nvae;
            }
            switch (alt40) {
                case 1 :
                    // src/java/grammar/wcps.g:232:7: e= INTEGERCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,INTEGERCONSTANT,FOLLOW_INTEGERCONSTANT_in_indexFactor2417); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new IndexExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:233:7: op= ROUND LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ROUND,FOLLOW_ROUND_in_indexFactor2430); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN131=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_indexFactor2432); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN131_tree = (Object)adaptor.create(LPAREN131);
                    adaptor.addChild(root_0, LPAREN131_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_indexFactor2436);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN132=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_indexFactor2438); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN132_tree = (Object)adaptor.create(RPAREN132);
                    adaptor.addChild(root_0, RPAREN132_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new IndexExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:234:7: ( LPAREN e2= indexExpr RPAREN )=> ( LPAREN e2= indexExpr RPAREN )
                    {
                    root_0 = (Object)adaptor.nil();

                    // src/java/grammar/wcps.g:235:9: ( LPAREN e2= indexExpr RPAREN )
                    // src/java/grammar/wcps.g:235:10: LPAREN e2= indexExpr RPAREN
                    {
                    LPAREN133=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_indexFactor2467); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN133_tree = (Object)adaptor.create(LPAREN133);
                    adaptor.addChild(root_0, LPAREN133_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_indexFactor2471);
                    e2=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN134=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_indexFactor2473); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN134_tree = (Object)adaptor.create(RPAREN134);
                    adaptor.addChild(root_0, RPAREN134_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e2!=null?e2.value:null); 
                    }

                    }


                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 44, indexFactor_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "indexFactor"

    public static class stringScalarExpr_return extends ParserRuleReturnScope {
        public StringScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "stringScalarExpr"
    // src/java/grammar/wcps.g:237:1: stringScalarExpr returns [StringScalarExpr value] : (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | e= STRING );
    public final wcpsParser.stringScalarExpr_return stringScalarExpr() throws RecognitionException {
        wcpsParser.stringScalarExpr_return retval = new wcpsParser.stringScalarExpr_return();
        retval.start = input.LT(1);
        int stringScalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token e=null;
        Token LPAREN135=null;
        Token RPAREN136=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object e_tree=null;
        Object LPAREN135_tree=null;
        Object RPAREN136_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 45) ) { return retval; }
            // src/java/grammar/wcps.g:239:5: (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | e= STRING )
            int alt41=2;
            int LA41_0 = input.LA(1);

            if ( (LA41_0==IDENTIFIER) ) {
                alt41=1;
            }
            else if ( (LA41_0==STRING) ) {
                alt41=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 41, 0, input);

                throw nvae;
            }
            switch (alt41) {
                case 1 :
                    // src/java/grammar/wcps.g:239:7: op= IDENTIFIER LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IDENTIFIER,FOLLOW_IDENTIFIER_in_stringScalarExpr2500); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN135=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_stringScalarExpr2502); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN135_tree = (Object)adaptor.create(LPAREN135);
                    adaptor.addChild(root_0, LPAREN135_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_stringScalarExpr2506);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN136=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_stringScalarExpr2508); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN136_tree = (Object)adaptor.create(RPAREN136);
                    adaptor.addChild(root_0, RPAREN136_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new StringScalarExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:240:7: e= STRING
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,STRING,FOLLOW_STRING_in_stringScalarExpr2521); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new StringScalarExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 45, stringScalarExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "stringScalarExpr"

    public static class scaleExpr_return extends ParserRuleReturnScope {
        public ScaleExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "scaleExpr"
    // src/java/grammar/wcps.g:242:1: scaleExpr returns [ScaleExpr value] : SCALE LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN ;
    public final wcpsParser.scaleExpr_return scaleExpr() throws RecognitionException {
        wcpsParser.scaleExpr_return retval = new wcpsParser.scaleExpr_return();
        retval.start = input.LT(1);
        int scaleExpr_StartIndex = input.index();
        Object root_0 = null;

        Token SCALE137=null;
        Token LPAREN138=null;
        Token COMMA139=null;
        Token COMMA140=null;
        Token RPAREN141=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;

        wcpsParser.fieldInterpolationList_return fil = null;


        Object SCALE137_tree=null;
        Object LPAREN138_tree=null;
        Object COMMA139_tree=null;
        Object COMMA140_tree=null;
        Object RPAREN141_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 46) ) { return retval; }
            // src/java/grammar/wcps.g:243:2: ( SCALE LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN )
            // src/java/grammar/wcps.g:243:4: SCALE LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            SCALE137=(Token)match(input,SCALE,FOLLOW_SCALE_in_scaleExpr2539); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            SCALE137_tree = (Object)adaptor.create(SCALE137);
            adaptor.addChild(root_0, SCALE137_tree);
            }
            LPAREN138=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_scaleExpr2541); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN138_tree = (Object)adaptor.create(LPAREN138);
            adaptor.addChild(root_0, LPAREN138_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_scaleExpr2545);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA139=(Token)match(input,COMMA,FOLLOW_COMMA_in_scaleExpr2547); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA139_tree = (Object)adaptor.create(COMMA139);
            adaptor.addChild(root_0, COMMA139_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_scaleExpr2551);
            dil=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
            COMMA140=(Token)match(input,COMMA,FOLLOW_COMMA_in_scaleExpr2553); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA140_tree = (Object)adaptor.create(COMMA140);
            adaptor.addChild(root_0, COMMA140_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationList_in_scaleExpr2557);
            fil=fieldInterpolationList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, fil.getTree());
            RPAREN141=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_scaleExpr2559); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN141_tree = (Object)adaptor.create(RPAREN141);
            adaptor.addChild(root_0, RPAREN141_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new ScaleExpr((e1!=null?e1.value:null), (dil!=null?dil.value:null), (fil!=null?fil.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 46, scaleExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "scaleExpr"

    public static class subsetExpr_return extends ParserRuleReturnScope {
        public SubsetExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "subsetExpr"
    // src/java/grammar/wcps.g:246:1: subsetExpr returns [SubsetExpr value] : (e1= trimExpr | e2= sliceExpr | e3= extendExpr );
    public final wcpsParser.subsetExpr_return subsetExpr() throws RecognitionException {
        wcpsParser.subsetExpr_return retval = new wcpsParser.subsetExpr_return();
        retval.start = input.LT(1);
        int subsetExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.trimExpr_return e1 = null;

        wcpsParser.sliceExpr_return e2 = null;

        wcpsParser.extendExpr_return e3 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 47) ) { return retval; }
            // src/java/grammar/wcps.g:247:2: (e1= trimExpr | e2= sliceExpr | e3= extendExpr )
            int alt42=3;
            alt42 = dfa42.predict(input);
            switch (alt42) {
                case 1 :
                    // src/java/grammar/wcps.g:247:4: e1= trimExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_trimExpr_in_subsetExpr2578);
                    e1=trimExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new SubsetExpr((e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:248:4: e2= sliceExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_sliceExpr_in_subsetExpr2587);
                    e2=sliceExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new SubsetExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:249:4: e3= extendExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_extendExpr_in_subsetExpr2596);
                    e3=extendExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new SubsetExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 47, subsetExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "subsetExpr"

    public static class trimExpr_return extends ParserRuleReturnScope {
        public TrimExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "trimExpr"
    // src/java/grammar/wcps.g:251:1: trimExpr returns [TrimExpr value] : (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN );
    public final wcpsParser.trimExpr_return trimExpr() throws RecognitionException {
        wcpsParser.trimExpr_return retval = new wcpsParser.trimExpr_return();
        retval.start = input.LT(1);
        int trimExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACKET142=null;
        Token RBRACKET143=null;
        Token TRIM144=null;
        Token LPAREN145=null;
        Token COMMA146=null;
        Token LBRACE147=null;
        Token RBRACE148=null;
        Token RPAREN149=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LBRACKET142_tree=null;
        Object RBRACKET143_tree=null;
        Object TRIM144_tree=null;
        Object LPAREN145_tree=null;
        Object COMMA146_tree=null;
        Object LBRACE147_tree=null;
        Object RBRACE148_tree=null;
        Object RPAREN149_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 48) ) { return retval; }
            // src/java/grammar/wcps.g:252:2: (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN )
            int alt43=2;
            alt43 = dfa43.predict(input);
            switch (alt43) {
                case 1 :
                    // src/java/grammar/wcps.g:252:4: e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_trimExpr2613);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    LBRACKET142=(Token)match(input,LBRACKET,FOLLOW_LBRACKET_in_trimExpr2615); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACKET142_tree = (Object)adaptor.create(LBRACKET142);
                    adaptor.addChild(root_0, LBRACKET142_tree);
                    }
                    pushFollow(FOLLOW_dimensionIntervalList_in_trimExpr2619);
                    dil=dimensionIntervalList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
                    RBRACKET143=(Token)match(input,RBRACKET,FOLLOW_RBRACKET_in_trimExpr2621); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACKET143_tree = (Object)adaptor.create(RBRACKET143);
                    adaptor.addChild(root_0, RBRACKET143_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new TrimExpr((e1!=null?e1.value:null), (dil!=null?dil.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:253:6: TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    TRIM144=(Token)match(input,TRIM,FOLLOW_TRIM_in_trimExpr2630); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    TRIM144_tree = (Object)adaptor.create(TRIM144);
                    adaptor.addChild(root_0, TRIM144_tree);
                    }
                    LPAREN145=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_trimExpr2632); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN145_tree = (Object)adaptor.create(LPAREN145);
                    adaptor.addChild(root_0, LPAREN145_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_trimExpr2636);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    COMMA146=(Token)match(input,COMMA,FOLLOW_COMMA_in_trimExpr2638); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA146_tree = (Object)adaptor.create(COMMA146);
                    adaptor.addChild(root_0, COMMA146_tree);
                    }
                    LBRACE147=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_trimExpr2640); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACE147_tree = (Object)adaptor.create(LBRACE147);
                    adaptor.addChild(root_0, LBRACE147_tree);
                    }
                    pushFollow(FOLLOW_dimensionIntervalList_in_trimExpr2644);
                    dil=dimensionIntervalList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
                    RBRACE148=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_trimExpr2646); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACE148_tree = (Object)adaptor.create(RBRACE148);
                    adaptor.addChild(root_0, RBRACE148_tree);
                    }
                    RPAREN149=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_trimExpr2648); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN149_tree = (Object)adaptor.create(RPAREN149);
                    adaptor.addChild(root_0, RPAREN149_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new TrimExpr((e2!=null?e2.value:null), (dil!=null?dil.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 48, trimExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "trimExpr"

    public static class sliceExpr_return extends ParserRuleReturnScope {
        public SliceExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "sliceExpr"
    // src/java/grammar/wcps.g:255:1: sliceExpr returns [SliceExpr value] : (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN );
    public final wcpsParser.sliceExpr_return sliceExpr() throws RecognitionException {
        wcpsParser.sliceExpr_return retval = new wcpsParser.sliceExpr_return();
        retval.start = input.LT(1);
        int sliceExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACKET150=null;
        Token RBRACKET151=null;
        Token SLICE152=null;
        Token LPAREN153=null;
        Token COMMA154=null;
        Token LBRACE155=null;
        Token RBRACE156=null;
        Token RPAREN157=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.dimensionPointList_return dpl = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LBRACKET150_tree=null;
        Object RBRACKET151_tree=null;
        Object SLICE152_tree=null;
        Object LPAREN153_tree=null;
        Object COMMA154_tree=null;
        Object LBRACE155_tree=null;
        Object RBRACE156_tree=null;
        Object RPAREN157_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 49) ) { return retval; }
            // src/java/grammar/wcps.g:256:2: (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN )
            int alt44=2;
            alt44 = dfa44.predict(input);
            switch (alt44) {
                case 1 :
                    // src/java/grammar/wcps.g:256:4: e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_sliceExpr2665);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    LBRACKET150=(Token)match(input,LBRACKET,FOLLOW_LBRACKET_in_sliceExpr2667); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACKET150_tree = (Object)adaptor.create(LBRACKET150);
                    adaptor.addChild(root_0, LBRACKET150_tree);
                    }
                    pushFollow(FOLLOW_dimensionPointList_in_sliceExpr2671);
                    dpl=dimensionPointList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpl.getTree());
                    RBRACKET151=(Token)match(input,RBRACKET,FOLLOW_RBRACKET_in_sliceExpr2673); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACKET151_tree = (Object)adaptor.create(RBRACKET151);
                    adaptor.addChild(root_0, RBRACKET151_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SliceExpr((e1!=null?e1.value:null), (dpl!=null?dpl.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:257:4: SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    SLICE152=(Token)match(input,SLICE,FOLLOW_SLICE_in_sliceExpr2680); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    SLICE152_tree = (Object)adaptor.create(SLICE152);
                    adaptor.addChild(root_0, SLICE152_tree);
                    }
                    LPAREN153=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_sliceExpr2682); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN153_tree = (Object)adaptor.create(LPAREN153);
                    adaptor.addChild(root_0, LPAREN153_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_sliceExpr2686);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    COMMA154=(Token)match(input,COMMA,FOLLOW_COMMA_in_sliceExpr2688); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA154_tree = (Object)adaptor.create(COMMA154);
                    adaptor.addChild(root_0, COMMA154_tree);
                    }
                    LBRACE155=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_sliceExpr2690); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACE155_tree = (Object)adaptor.create(LBRACE155);
                    adaptor.addChild(root_0, LBRACE155_tree);
                    }
                    pushFollow(FOLLOW_dimensionPointList_in_sliceExpr2694);
                    dpl=dimensionPointList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpl.getTree());
                    RBRACE156=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_sliceExpr2696); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACE156_tree = (Object)adaptor.create(RBRACE156);
                    adaptor.addChild(root_0, RBRACE156_tree);
                    }
                    RPAREN157=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_sliceExpr2698); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN157_tree = (Object)adaptor.create(RPAREN157);
                    adaptor.addChild(root_0, RPAREN157_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SliceExpr((e2!=null?e2.value:null), (dpl!=null?dpl.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 49, sliceExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "sliceExpr"

    public static class extendExpr_return extends ParserRuleReturnScope {
        public ExtendExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "extendExpr"
    // src/java/grammar/wcps.g:259:1: extendExpr returns [ExtendExpr value] : EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN ;
    public final wcpsParser.extendExpr_return extendExpr() throws RecognitionException {
        wcpsParser.extendExpr_return retval = new wcpsParser.extendExpr_return();
        retval.start = input.LT(1);
        int extendExpr_StartIndex = input.index();
        Object root_0 = null;

        Token EXTEND158=null;
        Token LPAREN159=null;
        Token COMMA160=null;
        Token RPAREN161=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;


        Object EXTEND158_tree=null;
        Object LPAREN159_tree=null;
        Object COMMA160_tree=null;
        Object RPAREN161_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 50) ) { return retval; }
            // src/java/grammar/wcps.g:260:2: ( EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN )
            // src/java/grammar/wcps.g:260:4: EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            EXTEND158=(Token)match(input,EXTEND,FOLLOW_EXTEND_in_extendExpr2713); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            EXTEND158_tree = (Object)adaptor.create(EXTEND158);
            adaptor.addChild(root_0, EXTEND158_tree);
            }
            LPAREN159=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_extendExpr2715); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN159_tree = (Object)adaptor.create(LPAREN159);
            adaptor.addChild(root_0, LPAREN159_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_extendExpr2719);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA160=(Token)match(input,COMMA,FOLLOW_COMMA_in_extendExpr2721); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA160_tree = (Object)adaptor.create(COMMA160);
            adaptor.addChild(root_0, COMMA160_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_extendExpr2725);
            dil=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
            RPAREN161=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_extendExpr2727); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN161_tree = (Object)adaptor.create(RPAREN161);
            adaptor.addChild(root_0, RPAREN161_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new ExtendExpr((e1!=null?e1.value:null), (dil!=null?dil.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 50, extendExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "extendExpr"

    public static class castExpr_return extends ParserRuleReturnScope {
        public CastExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "castExpr"
    // src/java/grammar/wcps.g:262:1: castExpr returns [CastExpr value] : LPAREN e1= rangeType RPAREN e2= coverageExpr ;
    public final wcpsParser.castExpr_return castExpr() throws RecognitionException {
        wcpsParser.castExpr_return retval = new wcpsParser.castExpr_return();
        retval.start = input.LT(1);
        int castExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN162=null;
        Token RPAREN163=null;
        wcpsParser.rangeType_return e1 = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LPAREN162_tree=null;
        Object RPAREN163_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 51) ) { return retval; }
            // src/java/grammar/wcps.g:263:5: ( LPAREN e1= rangeType RPAREN e2= coverageExpr )
            // src/java/grammar/wcps.g:263:7: LPAREN e1= rangeType RPAREN e2= coverageExpr
            {
            root_0 = (Object)adaptor.nil();

            LPAREN162=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_castExpr2745); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN162_tree = (Object)adaptor.create(LPAREN162);
            adaptor.addChild(root_0, LPAREN162_tree);
            }
            pushFollow(FOLLOW_rangeType_in_castExpr2749);
            e1=rangeType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN163=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_castExpr2751); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN163_tree = (Object)adaptor.create(RPAREN163);
            adaptor.addChild(root_0, RPAREN163_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_castExpr2755);
            e2=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new CastExpr((e2!=null?e2.value:null), (e1!=null?e1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 51, castExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "castExpr"

    public static class rangeType_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "rangeType"
    // src/java/grammar/wcps.g:265:1: rangeType returns [String value] : (type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 ) | UNSIGNED type= ( CHAR | SHORT | LONG ) );
    public final wcpsParser.rangeType_return rangeType() throws RecognitionException {
        wcpsParser.rangeType_return retval = new wcpsParser.rangeType_return();
        retval.start = input.LT(1);
        int rangeType_StartIndex = input.index();
        Object root_0 = null;

        Token type=null;
        Token UNSIGNED164=null;

        Object type_tree=null;
        Object UNSIGNED164_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 52) ) { return retval; }
            // src/java/grammar/wcps.g:266:5: (type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 ) | UNSIGNED type= ( CHAR | SHORT | LONG ) )
            int alt45=2;
            int LA45_0 = input.LA(1);

            if ( ((LA45_0>=BOOLEAN && LA45_0<=COMPLEX2)) ) {
                alt45=1;
            }
            else if ( (LA45_0==UNSIGNED) ) {
                alt45=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 45, 0, input);

                throw nvae;
            }
            switch (alt45) {
                case 1 :
                    // src/java/grammar/wcps.g:266:7: type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 )
                    {
                    root_0 = (Object)adaptor.nil();

                    type=(Token)input.LT(1);
                    if ( (input.LA(1)>=BOOLEAN && input.LA(1)<=COMPLEX2) ) {
                        input.consume();
                        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(type));
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    if ( state.backtracking==0 ) {
                       retval.value = new String((type!=null?type.getText():null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:267:7: UNSIGNED type= ( CHAR | SHORT | LONG )
                    {
                    root_0 = (Object)adaptor.nil();

                    UNSIGNED164=(Token)match(input,UNSIGNED,FOLLOW_UNSIGNED_in_rangeType2804); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    UNSIGNED164_tree = (Object)adaptor.create(UNSIGNED164);
                    adaptor.addChild(root_0, UNSIGNED164_tree);
                    }
                    type=(Token)input.LT(1);
                    if ( (input.LA(1)>=CHAR && input.LA(1)<=LONG) ) {
                        input.consume();
                        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(type));
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    if ( state.backtracking==0 ) {
                       retval.value = new String("unsigned " + (type!=null?type.getText():null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 52, rangeType_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "rangeType"

    public static class fieldExpr_return extends ParserRuleReturnScope {
        public SelectExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "fieldExpr"
    // src/java/grammar/wcps.g:269:1: fieldExpr returns [SelectExpr value] : e1= coverageAtom DOT e2= fieldName ;
    public final wcpsParser.fieldExpr_return fieldExpr() throws RecognitionException {
        wcpsParser.fieldExpr_return retval = new wcpsParser.fieldExpr_return();
        retval.start = input.LT(1);
        int fieldExpr_StartIndex = input.index();
        Object root_0 = null;

        Token DOT165=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.fieldName_return e2 = null;


        Object DOT165_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 53) ) { return retval; }
            // src/java/grammar/wcps.g:270:5: (e1= coverageAtom DOT e2= fieldName )
            // src/java/grammar/wcps.g:270:7: e1= coverageAtom DOT e2= fieldName
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageAtom_in_fieldExpr2837);
            e1=coverageAtom();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            DOT165=(Token)match(input,DOT,FOLLOW_DOT_in_fieldExpr2839); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            DOT165_tree = (Object)adaptor.create(DOT165);
            adaptor.addChild(root_0, DOT165_tree);
            }
            pushFollow(FOLLOW_fieldName_in_fieldExpr2843);
            e2=fieldName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new SelectExpr((e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 53, fieldExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "fieldExpr"

    public static class booleanScalarExpr_return extends ParserRuleReturnScope {
        public BooleanScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "booleanScalarExpr"
    // src/java/grammar/wcps.g:274:1: booleanScalarExpr returns [BooleanScalarExpr value] : e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )* ;
    public final wcpsParser.booleanScalarExpr_return booleanScalarExpr() throws RecognitionException {
        wcpsParser.booleanScalarExpr_return retval = new wcpsParser.booleanScalarExpr_return();
        retval.start = input.LT(1);
        int booleanScalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token OR166=null;
        Token XOR167=null;
        wcpsParser.booleanScalarTerm_return e1 = null;

        wcpsParser.booleanScalarTerm_return e2 = null;


        Object op_tree=null;
        Object OR166_tree=null;
        Object XOR167_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 54) ) { return retval; }
            // src/java/grammar/wcps.g:275:5: (e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )* )
            // src/java/grammar/wcps.g:275:7: e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_booleanScalarTerm_in_booleanScalarExpr2868);
            e1=booleanScalarTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:276:7: (op= ( OR | XOR ) e2= booleanScalarTerm )*
            loop47:
            do {
                int alt47=2;
                alt47 = dfa47.predict(input);
                switch (alt47) {
            	case 1 :
            	    // src/java/grammar/wcps.g:276:8: op= ( OR | XOR ) e2= booleanScalarTerm
            	    {
            	    // src/java/grammar/wcps.g:276:11: ( OR | XOR )
            	    int alt46=2;
            	    int LA46_0 = input.LA(1);

            	    if ( (LA46_0==OR) ) {
            	        alt46=1;
            	    }
            	    else if ( (LA46_0==XOR) ) {
            	        alt46=2;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        NoViableAltException nvae =
            	            new NoViableAltException("", 46, 0, input);

            	        throw nvae;
            	    }
            	    switch (alt46) {
            	        case 1 :
            	            // src/java/grammar/wcps.g:276:12: OR
            	            {
            	            OR166=(Token)match(input,OR,FOLLOW_OR_in_booleanScalarExpr2882); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            OR166_tree = (Object)adaptor.create(OR166);
            	            root_0 = (Object)adaptor.becomeRoot(OR166_tree, root_0);
            	            }

            	            }
            	            break;
            	        case 2 :
            	            // src/java/grammar/wcps.g:276:16: XOR
            	            {
            	            XOR167=(Token)match(input,XOR,FOLLOW_XOR_in_booleanScalarExpr2885); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            XOR167_tree = (Object)adaptor.create(XOR167);
            	            root_0 = (Object)adaptor.becomeRoot(XOR167_tree, root_0);
            	            }

            	            }
            	            break;

            	    }

            	    pushFollow(FOLLOW_booleanScalarTerm_in_booleanScalarExpr2891);
            	    e2=booleanScalarTerm();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new BooleanScalarExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null));
            	    }

            	    }
            	    break;

            	default :
            	    break loop47;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 54, booleanScalarExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "booleanScalarExpr"

    public static class booleanScalarTerm_return extends ParserRuleReturnScope {
        public BooleanScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "booleanScalarTerm"
    // src/java/grammar/wcps.g:278:1: booleanScalarTerm returns [BooleanScalarExpr value] : e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )* ;
    public final wcpsParser.booleanScalarTerm_return booleanScalarTerm() throws RecognitionException {
        wcpsParser.booleanScalarTerm_return retval = new wcpsParser.booleanScalarTerm_return();
        retval.start = input.LT(1);
        int booleanScalarTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.booleanScalarNegation_return e1 = null;

        wcpsParser.booleanScalarNegation_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 55) ) { return retval; }
            // src/java/grammar/wcps.g:279:2: (e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )* )
            // src/java/grammar/wcps.g:279:4: e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_booleanScalarNegation_in_booleanScalarTerm2913);
            e1=booleanScalarNegation();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:280:4: (op= AND e2= booleanScalarNegation )*
            loop48:
            do {
                int alt48=2;
                alt48 = dfa48.predict(input);
                switch (alt48) {
            	case 1 :
            	    // src/java/grammar/wcps.g:280:5: op= AND e2= booleanScalarNegation
            	    {
            	    op=(Token)match(input,AND,FOLLOW_AND_in_booleanScalarTerm2923); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    op_tree = (Object)adaptor.create(op);
            	    adaptor.addChild(root_0, op_tree);
            	    }
            	    pushFollow(FOLLOW_booleanScalarNegation_in_booleanScalarTerm2927);
            	    e2=booleanScalarNegation();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new BooleanScalarExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop48;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 55, booleanScalarTerm_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "booleanScalarTerm"

    public static class booleanScalarNegation_return extends ParserRuleReturnScope {
        public BooleanScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "booleanScalarNegation"
    // src/java/grammar/wcps.g:283:1: booleanScalarNegation returns [BooleanScalarExpr value] : (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom );
    public final wcpsParser.booleanScalarNegation_return booleanScalarNegation() throws RecognitionException {
        wcpsParser.booleanScalarNegation_return retval = new wcpsParser.booleanScalarNegation_return();
        retval.start = input.LT(1);
        int booleanScalarNegation_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.booleanScalarAtom_return e1 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 56) ) { return retval; }
            // src/java/grammar/wcps.g:285:2: (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom )
            int alt49=2;
            alt49 = dfa49.predict(input);
            switch (alt49) {
                case 1 :
                    // src/java/grammar/wcps.g:285:4: e1= booleanScalarAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanScalarAtom_in_booleanScalarNegation2950);
                    e1=booleanScalarAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:286:4: op= NOT e1= booleanScalarAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NOT,FOLLOW_NOT_in_booleanScalarNegation2959); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarAtom_in_booleanScalarNegation2963);
                    e1=booleanScalarAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new BooleanScalarExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 56, booleanScalarNegation_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "booleanScalarNegation"

    public static class booleanScalarAtom_return extends ParserRuleReturnScope {
        public BooleanScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "booleanScalarAtom"
    // src/java/grammar/wcps.g:288:1: booleanScalarAtom returns [BooleanScalarExpr value] : ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT );
    public final wcpsParser.booleanScalarAtom_return booleanScalarAtom() throws RecognitionException {
        wcpsParser.booleanScalarAtom_return retval = new wcpsParser.booleanScalarAtom_return();
        retval.start = input.LT(1);
        int booleanScalarAtom_StartIndex = input.index();
        Object root_0 = null;

        Token e=null;
        Token LPAREN168=null;
        Token RPAREN169=null;
        wcpsParser.booleanScalarExpr_return e1 = null;

        wcpsParser.stringScalarExpr_return s1 = null;

        wcpsParser.compOp_return cop = null;

        wcpsParser.stringScalarExpr_return s2 = null;

        wcpsParser.numericScalarExpr_return n1 = null;

        wcpsParser.numericScalarExpr_return n2 = null;


        Object e_tree=null;
        Object LPAREN168_tree=null;
        Object RPAREN169_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 57) ) { return retval; }
            // src/java/grammar/wcps.g:289:2: ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT )
            int alt50=4;
            alt50 = dfa50.predict(input);
            switch (alt50) {
                case 1 :
                    // src/java/grammar/wcps.g:289:4: LPAREN e1= booleanScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN168=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_booleanScalarAtom2978); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN168_tree = (Object)adaptor.create(LPAREN168);
                    adaptor.addChild(root_0, LPAREN168_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarExpr_in_booleanScalarAtom2982);
                    e1=booleanScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN169=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_booleanScalarAtom2984); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN169_tree = (Object)adaptor.create(RPAREN169);
                    adaptor.addChild(root_0, RPAREN169_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:290:4: s1= stringScalarExpr cop= compOp s2= stringScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_stringScalarExpr_in_booleanScalarAtom2993);
                    s1=stringScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, s1.getTree());
                    pushFollow(FOLLOW_compOp_in_booleanScalarAtom2997);
                    cop=compOp();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cop.getTree());
                    pushFollow(FOLLOW_stringScalarExpr_in_booleanScalarAtom3001);
                    s2=stringScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, s2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new BooleanScalarExpr((cop!=null?cop.value:null), (s1!=null?s1.value:null), (s2!=null?s2.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:291:4: n1= numericScalarExpr cop= compOp n2= numericScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_numericScalarExpr_in_booleanScalarAtom3011);
                    n1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, n1.getTree());
                    pushFollow(FOLLOW_compOp_in_booleanScalarAtom3015);
                    cop=compOp();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cop.getTree());
                    pushFollow(FOLLOW_numericScalarExpr_in_booleanScalarAtom3019);
                    n2=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, n2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new BooleanScalarExpr((cop!=null?cop.value:null), (n1!=null?n1.value:null), (n2!=null?n2.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:292:4: e= BOOLEANCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,BOOLEANCONSTANT,FOLLOW_BOOLEANCONSTANT_in_booleanScalarAtom3029); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new BooleanScalarExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 57, booleanScalarAtom_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "booleanScalarAtom"

    public static class numericScalarExpr_return extends ParserRuleReturnScope {
        public NumericScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "numericScalarExpr"
    // src/java/grammar/wcps.g:295:1: numericScalarExpr returns [NumericScalarExpr value] : e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )* ;
    public final wcpsParser.numericScalarExpr_return numericScalarExpr() throws RecognitionException {
        wcpsParser.numericScalarExpr_return retval = new wcpsParser.numericScalarExpr_return();
        retval.start = input.LT(1);
        int numericScalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token PLUS170=null;
        Token MINUS171=null;
        wcpsParser.numericScalarTerm_return e1 = null;

        wcpsParser.numericScalarTerm_return e2 = null;


        Object op_tree=null;
        Object PLUS170_tree=null;
        Object MINUS171_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 58) ) { return retval; }
            // src/java/grammar/wcps.g:296:2: (e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )* )
            // src/java/grammar/wcps.g:296:4: e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_numericScalarTerm_in_numericScalarExpr3047);
            e1=numericScalarTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:297:4: (op= ( PLUS | MINUS ) e2= numericScalarTerm )*
            loop52:
            do {
                int alt52=2;
                alt52 = dfa52.predict(input);
                switch (alt52) {
            	case 1 :
            	    // src/java/grammar/wcps.g:297:5: op= ( PLUS | MINUS ) e2= numericScalarTerm
            	    {
            	    // src/java/grammar/wcps.g:297:8: ( PLUS | MINUS )
            	    int alt51=2;
            	    int LA51_0 = input.LA(1);

            	    if ( (LA51_0==PLUS) ) {
            	        alt51=1;
            	    }
            	    else if ( (LA51_0==MINUS) ) {
            	        alt51=2;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        NoViableAltException nvae =
            	            new NoViableAltException("", 51, 0, input);

            	        throw nvae;
            	    }
            	    switch (alt51) {
            	        case 1 :
            	            // src/java/grammar/wcps.g:297:9: PLUS
            	            {
            	            PLUS170=(Token)match(input,PLUS,FOLLOW_PLUS_in_numericScalarExpr3058); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            PLUS170_tree = (Object)adaptor.create(PLUS170);
            	            root_0 = (Object)adaptor.becomeRoot(PLUS170_tree, root_0);
            	            }

            	            }
            	            break;
            	        case 2 :
            	            // src/java/grammar/wcps.g:297:15: MINUS
            	            {
            	            MINUS171=(Token)match(input,MINUS,FOLLOW_MINUS_in_numericScalarExpr3061); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            MINUS171_tree = (Object)adaptor.create(MINUS171);
            	            root_0 = (Object)adaptor.becomeRoot(MINUS171_tree, root_0);
            	            }

            	            }
            	            break;

            	    }

            	    pushFollow(FOLLOW_numericScalarTerm_in_numericScalarExpr3067);
            	    e2=numericScalarTerm();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new NumericScalarExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop52;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 58, numericScalarExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "numericScalarExpr"

    public static class numericScalarTerm_return extends ParserRuleReturnScope {
        public NumericScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "numericScalarTerm"
    // src/java/grammar/wcps.g:299:1: numericScalarTerm returns [NumericScalarExpr value] : e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )* ;
    public final wcpsParser.numericScalarTerm_return numericScalarTerm() throws RecognitionException {
        wcpsParser.numericScalarTerm_return retval = new wcpsParser.numericScalarTerm_return();
        retval.start = input.LT(1);
        int numericScalarTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token MULT172=null;
        Token DIVIDE173=null;
        wcpsParser.numericScalarFactor_return e1 = null;

        wcpsParser.numericScalarFactor_return e2 = null;


        Object op_tree=null;
        Object MULT172_tree=null;
        Object DIVIDE173_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 59) ) { return retval; }
            // src/java/grammar/wcps.g:300:2: (e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )* )
            // src/java/grammar/wcps.g:300:4: e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_numericScalarFactor_in_numericScalarTerm3086);
            e1=numericScalarFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/java/grammar/wcps.g:301:3: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*
            loop54:
            do {
                int alt54=2;
                alt54 = dfa54.predict(input);
                switch (alt54) {
            	case 1 :
            	    // src/java/grammar/wcps.g:301:4: op= ( MULT | DIVIDE ) e2= numericScalarFactor
            	    {
            	    // src/java/grammar/wcps.g:301:7: ( MULT | DIVIDE )
            	    int alt53=2;
            	    int LA53_0 = input.LA(1);

            	    if ( (LA53_0==MULT) ) {
            	        alt53=1;
            	    }
            	    else if ( (LA53_0==DIVIDE) ) {
            	        alt53=2;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        NoViableAltException nvae =
            	            new NoViableAltException("", 53, 0, input);

            	        throw nvae;
            	    }
            	    switch (alt53) {
            	        case 1 :
            	            // src/java/grammar/wcps.g:301:8: MULT
            	            {
            	            MULT172=(Token)match(input,MULT,FOLLOW_MULT_in_numericScalarTerm3096); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            MULT172_tree = (Object)adaptor.create(MULT172);
            	            root_0 = (Object)adaptor.becomeRoot(MULT172_tree, root_0);
            	            }

            	            }
            	            break;
            	        case 2 :
            	            // src/java/grammar/wcps.g:301:14: DIVIDE
            	            {
            	            DIVIDE173=(Token)match(input,DIVIDE,FOLLOW_DIVIDE_in_numericScalarTerm3099); if (state.failed) return retval;
            	            if ( state.backtracking==0 ) {
            	            DIVIDE173_tree = (Object)adaptor.create(DIVIDE173);
            	            root_0 = (Object)adaptor.becomeRoot(DIVIDE173_tree, root_0);
            	            }

            	            }
            	            break;

            	    }

            	    pushFollow(FOLLOW_numericScalarFactor_in_numericScalarTerm3105);
            	    e2=numericScalarFactor();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new NumericScalarExpr((op!=null?op.getText():null), retval.value, (e2!=null?e2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop54;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 59, numericScalarTerm_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "numericScalarTerm"

    public static class numericScalarFactor_return extends ParserRuleReturnScope {
        public NumericScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "numericScalarFactor"
    // src/java/grammar/wcps.g:303:1: numericScalarFactor returns [NumericScalarExpr value] : ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr );
    public final wcpsParser.numericScalarFactor_return numericScalarFactor() throws RecognitionException {
        wcpsParser.numericScalarFactor_return retval = new wcpsParser.numericScalarFactor_return();
        retval.start = input.LT(1);
        int numericScalarFactor_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token e=null;
        Token LPAREN174=null;
        Token RPAREN175=null;
        Token LPAREN176=null;
        Token RPAREN177=null;
        wcpsParser.numericScalarExpr_return e1 = null;

        wcpsParser.numericScalarFactor_return e10 = null;

        wcpsParser.complexConstant_return e2 = null;

        wcpsParser.condenseExpr_return e3 = null;


        Object op_tree=null;
        Object e_tree=null;
        Object LPAREN174_tree=null;
        Object RPAREN175_tree=null;
        Object LPAREN176_tree=null;
        Object RPAREN177_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 60) ) { return retval; }
            // src/java/grammar/wcps.g:304:5: ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr )
            int alt55=7;
            alt55 = dfa55.predict(input);
            switch (alt55) {
                case 1 :
                    // src/java/grammar/wcps.g:304:7: LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN174=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3125); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN174_tree = (Object)adaptor.create(LPAREN174);
                    adaptor.addChild(root_0, LPAREN174_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3129);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN175=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3131); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN175_tree = (Object)adaptor.create(RPAREN175);
                    adaptor.addChild(root_0, RPAREN175_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:305:7: op= MINUS e10= numericScalarFactor
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,MINUS,FOLLOW_MINUS_in_numericScalarFactor3143); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    root_0 = (Object)adaptor.becomeRoot(op_tree, root_0);
                    }
                    pushFollow(FOLLOW_numericScalarFactor_in_numericScalarFactor3148);
                    e10=numericScalarFactor();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e10.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e10!=null?e10.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:306:7: op= ROUND LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ROUND,FOLLOW_ROUND_in_numericScalarFactor3160); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN176=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3162); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN176_tree = (Object)adaptor.create(LPAREN176);
                    adaptor.addChild(root_0, LPAREN176_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3166);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN177=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3168); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN177_tree = (Object)adaptor.create(RPAREN177);
                    adaptor.addChild(root_0, RPAREN177_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:307:7: e= INTEGERCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,INTEGERCONSTANT,FOLLOW_INTEGERCONSTANT_in_numericScalarFactor3180); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:308:7: e= FLOATCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_numericScalarFactor3192); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;
                case 6 :
                    // src/java/grammar/wcps.g:309:7: e2= complexConstant
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_complexConstant_in_numericScalarFactor3204);
                    e2=complexConstant();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 7 :
                    // src/java/grammar/wcps.g:310:7: e3= condenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_condenseExpr_in_numericScalarFactor3216);
                    e3=condenseExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 60, numericScalarFactor_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "numericScalarFactor"

    public static class compOp_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "compOp"
    // src/java/grammar/wcps.g:312:1: compOp returns [String value] : ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE );
    public final wcpsParser.compOp_return compOp() throws RecognitionException {
        wcpsParser.compOp_return retval = new wcpsParser.compOp_return();
        retval.start = input.LT(1);
        int compOp_StartIndex = input.index();
        Object root_0 = null;

        Token EQUALS178=null;
        Token NOTEQUALS179=null;
        Token LT180=null;
        Token GT181=null;
        Token LTE182=null;
        Token GTE183=null;

        Object EQUALS178_tree=null;
        Object NOTEQUALS179_tree=null;
        Object LT180_tree=null;
        Object GT181_tree=null;
        Object LTE182_tree=null;
        Object GTE183_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 61) ) { return retval; }
            // src/java/grammar/wcps.g:313:2: ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE )
            int alt56=6;
            switch ( input.LA(1) ) {
            case EQUALS:
                {
                alt56=1;
                }
                break;
            case NOTEQUALS:
                {
                alt56=2;
                }
                break;
            case LT:
                {
                alt56=3;
                }
                break;
            case GT:
                {
                alt56=4;
                }
                break;
            case LTE:
                {
                alt56=5;
                }
                break;
            case GTE:
                {
                alt56=6;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 56, 0, input);

                throw nvae;
            }

            switch (alt56) {
                case 1 :
                    // src/java/grammar/wcps.g:313:4: EQUALS
                    {
                    root_0 = (Object)adaptor.nil();

                    EQUALS178=(Token)match(input,EQUALS,FOLLOW_EQUALS_in_compOp3234); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    EQUALS178_tree = (Object)adaptor.create(EQUALS178);
                    adaptor.addChild(root_0, EQUALS178_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("equals"); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:314:4: NOTEQUALS
                    {
                    root_0 = (Object)adaptor.nil();

                    NOTEQUALS179=(Token)match(input,NOTEQUALS,FOLLOW_NOTEQUALS_in_compOp3241); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    NOTEQUALS179_tree = (Object)adaptor.create(NOTEQUALS179);
                    adaptor.addChild(root_0, NOTEQUALS179_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("notEqual"); 
                    }

                    }
                    break;
                case 3 :
                    // src/java/grammar/wcps.g:315:4: LT
                    {
                    root_0 = (Object)adaptor.nil();

                    LT180=(Token)match(input,LT,FOLLOW_LT_in_compOp3248); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LT180_tree = (Object)adaptor.create(LT180);
                    adaptor.addChild(root_0, LT180_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("lessThan"); 
                    }

                    }
                    break;
                case 4 :
                    // src/java/grammar/wcps.g:316:4: GT
                    {
                    root_0 = (Object)adaptor.nil();

                    GT181=(Token)match(input,GT,FOLLOW_GT_in_compOp3255); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    GT181_tree = (Object)adaptor.create(GT181);
                    adaptor.addChild(root_0, GT181_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("greaterThan"); 
                    }

                    }
                    break;
                case 5 :
                    // src/java/grammar/wcps.g:317:4: LTE
                    {
                    root_0 = (Object)adaptor.nil();

                    LTE182=(Token)match(input,LTE,FOLLOW_LTE_in_compOp3262); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LTE182_tree = (Object)adaptor.create(LTE182);
                    adaptor.addChild(root_0, LTE182_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("lessOrEqual"); 
                    }

                    }
                    break;
                case 6 :
                    // src/java/grammar/wcps.g:318:4: GTE
                    {
                    root_0 = (Object)adaptor.nil();

                    GTE183=(Token)match(input,GTE,FOLLOW_GTE_in_compOp3269); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    GTE183_tree = (Object)adaptor.create(GTE183);
                    adaptor.addChild(root_0, GTE183_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("greaterOrEqual"); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 61, compOp_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "compOp"

    public static class dimensionIntervalList_return extends ParserRuleReturnScope {
        public DimensionIntervalList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionIntervalList"
    // src/java/grammar/wcps.g:320:1: dimensionIntervalList returns [DimensionIntervalList value] : elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )* ;
    public final wcpsParser.dimensionIntervalList_return dimensionIntervalList() throws RecognitionException {
        wcpsParser.dimensionIntervalList_return retval = new wcpsParser.dimensionIntervalList_return();
        retval.start = input.LT(1);
        int dimensionIntervalList_StartIndex = input.index();
        Object root_0 = null;

        Token COMMA184=null;
        wcpsParser.dimensionIntervalElement_return elem = null;


        Object COMMA184_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 62) ) { return retval; }
            // src/java/grammar/wcps.g:321:5: (elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )* )
            // src/java/grammar/wcps.g:321:7: elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3289);
            elem=dimensionIntervalElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionIntervalList((elem!=null?elem.value:null)); 
            }
            // src/java/grammar/wcps.g:322:9: ( COMMA elem= dimensionIntervalElement )*
            loop57:
            do {
                int alt57=2;
                int LA57_0 = input.LA(1);

                if ( (LA57_0==COMMA) ) {
                    int LA57_1 = input.LA(2);

                    if ( (LA57_1==INTEGERCONSTANT||LA57_1==STRING||LA57_1==NAME) ) {
                        alt57=1;
                    }


                }


                switch (alt57) {
            	case 1 :
            	    // src/java/grammar/wcps.g:322:10: COMMA elem= dimensionIntervalElement
            	    {
            	    COMMA184=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionIntervalList3302); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA184_tree = (Object)adaptor.create(COMMA184);
            	    adaptor.addChild(root_0, COMMA184_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3306);
            	    elem=dimensionIntervalElement();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value.add((elem!=null?elem.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop57;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 62, dimensionIntervalList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionIntervalList"

    public static class dimensionIntervalElement_return extends ParserRuleReturnScope {
        public DimensionIntervalElement value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionIntervalElement"
    // src/java/grammar/wcps.g:324:1: dimensionIntervalElement returns [DimensionIntervalElement value] : aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN ;
    public final wcpsParser.dimensionIntervalElement_return dimensionIntervalElement() throws RecognitionException {
        wcpsParser.dimensionIntervalElement_return retval = new wcpsParser.dimensionIntervalElement_return();
        retval.start = input.LT(1);
        int dimensionIntervalElement_StartIndex = input.index();
        Object root_0 = null;

        Token COLON185=null;
        Token LPAREN186=null;
        Token RPAREN187=null;
        wcpsParser.axisName_return aname = null;

        wcpsParser.crsName_return crs = null;

        wcpsParser.dimensionIntervalExpr_return die = null;


        Object COLON185_tree=null;
        Object LPAREN186_tree=null;
        Object RPAREN187_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 63) ) { return retval; }
            // src/java/grammar/wcps.g:325:5: (aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN )
            // src/java/grammar/wcps.g:325:7: aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_axisName_in_dimensionIntervalElement3331);
            aname=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionIntervalElement((aname!=null?aname.value:null)); 
            }
            // src/java/grammar/wcps.g:325:79: ( COLON crs= crsName )?
            int alt58=2;
            int LA58_0 = input.LA(1);

            if ( (LA58_0==COLON) ) {
                alt58=1;
            }
            switch (alt58) {
                case 1 :
                    // src/java/grammar/wcps.g:325:80: COLON crs= crsName
                    {
                    COLON185=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalElement3336); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON185_tree = (Object)adaptor.create(COLON185);
                    adaptor.addChild(root_0, COLON185_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionIntervalElement3340);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    if ( state.backtracking==0 ) {
                      retval.value.setCrs((crs!=null?crs.value:null)); 
                    }

                    }
                    break;

            }

            LPAREN186=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionIntervalElement3351); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN186_tree = (Object)adaptor.create(LPAREN186);
            adaptor.addChild(root_0, LPAREN186_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalExpr_in_dimensionIntervalElement3355);
            die=dimensionIntervalExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, die.getTree());
            RPAREN187=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionIntervalElement3357); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN187_tree = (Object)adaptor.create(RPAREN187);
            adaptor.addChild(root_0, RPAREN187_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value.setIntervalExpr((die!=null?die.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 63, dimensionIntervalElement_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionIntervalElement"

    public static class dimensionIntervalExpr_return extends ParserRuleReturnScope {
        public DimensionIntervalExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionIntervalExpr"
    // src/java/grammar/wcps.g:328:1: dimensionIntervalExpr returns [DimensionIntervalExpr value] : (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN );
    public final wcpsParser.dimensionIntervalExpr_return dimensionIntervalExpr() throws RecognitionException {
        wcpsParser.dimensionIntervalExpr_return retval = new wcpsParser.dimensionIntervalExpr_return();
        retval.start = input.LT(1);
        int dimensionIntervalExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COLON188=null;
        Token DOMAIN189=null;
        Token LPAREN190=null;
        Token COLON191=null;
        Token COLON192=null;
        Token RPAREN193=null;
        wcpsParser.scalarExpr_return e1 = null;

        wcpsParser.scalarExpr_return e2 = null;

        wcpsParser.coverageName_return e3 = null;

        wcpsParser.axisName_return e4 = null;

        wcpsParser.crsName_return e5 = null;


        Object COLON188_tree=null;
        Object DOMAIN189_tree=null;
        Object LPAREN190_tree=null;
        Object COLON191_tree=null;
        Object COLON192_tree=null;
        Object RPAREN193_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 64) ) { return retval; }
            // src/java/grammar/wcps.g:329:5: (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN )
            int alt59=2;
            alt59 = dfa59.predict(input);
            switch (alt59) {
                case 1 :
                    // src/java/grammar/wcps.g:329:7: e1= scalarExpr COLON e2= scalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scalarExpr_in_dimensionIntervalExpr3380);
                    e1=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COLON188=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3382); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON188_tree = (Object)adaptor.create(COLON188);
                    adaptor.addChild(root_0, COLON188_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_dimensionIntervalExpr3386);
                    e2=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new DimensionIntervalExpr((e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:330:7: DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    DOMAIN189=(Token)match(input,DOMAIN,FOLLOW_DOMAIN_in_dimensionIntervalExpr3396); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    DOMAIN189_tree = (Object)adaptor.create(DOMAIN189);
                    adaptor.addChild(root_0, DOMAIN189_tree);
                    }
                    LPAREN190=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionIntervalExpr3398); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN190_tree = (Object)adaptor.create(LPAREN190);
                    adaptor.addChild(root_0, LPAREN190_tree);
                    }
                    pushFollow(FOLLOW_coverageName_in_dimensionIntervalExpr3402);
                    e3=coverageName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    COLON191=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3404); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON191_tree = (Object)adaptor.create(COLON191);
                    adaptor.addChild(root_0, COLON191_tree);
                    }
                    pushFollow(FOLLOW_axisName_in_dimensionIntervalExpr3408);
                    e4=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    COLON192=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3410); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON192_tree = (Object)adaptor.create(COLON192);
                    adaptor.addChild(root_0, COLON192_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionIntervalExpr3414);
                    e5=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    RPAREN193=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionIntervalExpr3416); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN193_tree = (Object)adaptor.create(RPAREN193);
                    adaptor.addChild(root_0, RPAREN193_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new DimensionIntervalExpr((e3!=null?e3.value:null), (e4!=null?e4.value:null), (e5!=null?e5.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 64, dimensionIntervalExpr_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionIntervalExpr"

    public static class dimensionPointList_return extends ParserRuleReturnScope {
        public DimensionPointList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionPointList"
    // src/java/grammar/wcps.g:332:1: dimensionPointList returns [DimensionPointList value] : elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )* ;
    public final wcpsParser.dimensionPointList_return dimensionPointList() throws RecognitionException {
        wcpsParser.dimensionPointList_return retval = new wcpsParser.dimensionPointList_return();
        retval.start = input.LT(1);
        int dimensionPointList_StartIndex = input.index();
        Object root_0 = null;

        Token COMMA194=null;
        wcpsParser.dimensionPointElement_return elem1 = null;

        wcpsParser.dimensionPointElement_return elem2 = null;


        Object COMMA194_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 65) ) { return retval; }
            // src/java/grammar/wcps.g:333:5: (elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )* )
            // src/java/grammar/wcps.g:333:7: elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_dimensionPointElement_in_dimensionPointList3439);
            elem1=dimensionPointElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionPointList((elem1!=null?elem1.value:null)); 
            }
            // src/java/grammar/wcps.g:334:6: ( COMMA elem2= dimensionPointElement )*
            loop60:
            do {
                int alt60=2;
                int LA60_0 = input.LA(1);

                if ( (LA60_0==COMMA) ) {
                    alt60=1;
                }


                switch (alt60) {
            	case 1 :
            	    // src/java/grammar/wcps.g:334:7: COMMA elem2= dimensionPointElement
            	    {
            	    COMMA194=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionPointList3449); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA194_tree = (Object)adaptor.create(COMMA194);
            	    adaptor.addChild(root_0, COMMA194_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionPointElement_in_dimensionPointList3453);
            	    elem2=dimensionPointElement();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, elem2.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value.add((elem2!=null?elem2.value:null)); 
            	    }

            	    }
            	    break;

            	default :
            	    break loop60;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 65, dimensionPointList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionPointList"

    public static class dimensionPointElement_return extends ParserRuleReturnScope {
        public DimensionPointElement value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionPointElement"
    // src/java/grammar/wcps.g:336:1: dimensionPointElement returns [DimensionPointElement value] : (aname= axisName LPAREN dpe= dimensionPoint RPAREN | aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN );
    public final wcpsParser.dimensionPointElement_return dimensionPointElement() throws RecognitionException {
        wcpsParser.dimensionPointElement_return retval = new wcpsParser.dimensionPointElement_return();
        retval.start = input.LT(1);
        int dimensionPointElement_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN195=null;
        Token RPAREN196=null;
        Token COLON197=null;
        Token LPAREN198=null;
        Token RPAREN199=null;
        wcpsParser.axisName_return aname = null;

        wcpsParser.dimensionPoint_return dpe = null;

        wcpsParser.crsName_return crs = null;


        Object LPAREN195_tree=null;
        Object RPAREN196_tree=null;
        Object COLON197_tree=null;
        Object LPAREN198_tree=null;
        Object RPAREN199_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 66) ) { return retval; }
            // src/java/grammar/wcps.g:337:5: (aname= axisName LPAREN dpe= dimensionPoint RPAREN | aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN )
            int alt61=2;
            int LA61_0 = input.LA(1);

            if ( (LA61_0==INTEGERCONSTANT||LA61_0==STRING||LA61_0==NAME) ) {
                int LA61_1 = input.LA(2);

                if ( (LA61_1==COLON) ) {
                    alt61=2;
                }
                else if ( (LA61_1==LPAREN) ) {
                    alt61=1;
                }
                else {
                    if (state.backtracking>0) {state.failed=true; return retval;}
                    NoViableAltException nvae =
                        new NoViableAltException("", 61, 1, input);

                    throw nvae;
                }
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 61, 0, input);

                throw nvae;
            }
            switch (alt61) {
                case 1 :
                    // src/java/grammar/wcps.g:337:7: aname= axisName LPAREN dpe= dimensionPoint RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_axisName_in_dimensionPointElement3478);
                    aname=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
                    LPAREN195=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionPointElement3480); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN195_tree = (Object)adaptor.create(LPAREN195);
                    adaptor.addChild(root_0, LPAREN195_tree);
                    }
                    pushFollow(FOLLOW_dimensionPoint_in_dimensionPointElement3484);
                    dpe=dimensionPoint();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpe.getTree());
                    RPAREN196=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionPointElement3486); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN196_tree = (Object)adaptor.create(RPAREN196);
                    adaptor.addChild(root_0, RPAREN196_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new DimensionPointElement((aname!=null?aname.value:null), (dpe!=null?dpe.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:338:7: aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_axisName_in_dimensionPointElement3498);
                    aname=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
                    COLON197=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionPointElement3500); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON197_tree = (Object)adaptor.create(COLON197);
                    adaptor.addChild(root_0, COLON197_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionPointElement3504);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    LPAREN198=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionPointElement3506); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN198_tree = (Object)adaptor.create(LPAREN198);
                    adaptor.addChild(root_0, LPAREN198_tree);
                    }
                    pushFollow(FOLLOW_dimensionPoint_in_dimensionPointElement3510);
                    dpe=dimensionPoint();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpe.getTree());
                    RPAREN199=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionPointElement3512); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN199_tree = (Object)adaptor.create(RPAREN199);
                    adaptor.addChild(root_0, RPAREN199_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new DimensionPointElement((aname!=null?aname.value:null), (crs!=null?crs.value:null), (dpe!=null?dpe.value:null)); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 66, dimensionPointElement_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionPointElement"

    public static class dimensionPoint_return extends ParserRuleReturnScope {
        public ScalarExpr value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionPoint"
    // src/java/grammar/wcps.g:340:1: dimensionPoint returns [ScalarExpr value] : e1= scalarExpr ;
    public final wcpsParser.dimensionPoint_return dimensionPoint() throws RecognitionException {
        wcpsParser.dimensionPoint_return retval = new wcpsParser.dimensionPoint_return();
        retval.start = input.LT(1);
        int dimensionPoint_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.scalarExpr_return e1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 67) ) { return retval; }
            // src/java/grammar/wcps.g:341:5: (e1= scalarExpr )
            // src/java/grammar/wcps.g:341:7: e1= scalarExpr
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_scalarExpr_in_dimensionPoint3535);
            e1=scalarExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 67, dimensionPoint_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionPoint"

    public static class interpolationMethod_return extends ParserRuleReturnScope {
        public InterpolationMethod value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "interpolationMethod"
    // src/java/grammar/wcps.g:343:1: interpolationMethod returns [InterpolationMethod value] : LPAREN type= interpolationType COLON res= nullResistence RPAREN ;
    public final wcpsParser.interpolationMethod_return interpolationMethod() throws RecognitionException {
        wcpsParser.interpolationMethod_return retval = new wcpsParser.interpolationMethod_return();
        retval.start = input.LT(1);
        int interpolationMethod_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN200=null;
        Token COLON201=null;
        Token RPAREN202=null;
        wcpsParser.interpolationType_return type = null;

        wcpsParser.nullResistence_return res = null;


        Object LPAREN200_tree=null;
        Object COLON201_tree=null;
        Object RPAREN202_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 68) ) { return retval; }
            // src/java/grammar/wcps.g:344:2: ( LPAREN type= interpolationType COLON res= nullResistence RPAREN )
            // src/java/grammar/wcps.g:344:4: LPAREN type= interpolationType COLON res= nullResistence RPAREN
            {
            root_0 = (Object)adaptor.nil();

            LPAREN200=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_interpolationMethod3553); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN200_tree = (Object)adaptor.create(LPAREN200);
            adaptor.addChild(root_0, LPAREN200_tree);
            }
            pushFollow(FOLLOW_interpolationType_in_interpolationMethod3557);
            type=interpolationType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, type.getTree());
            COLON201=(Token)match(input,COLON,FOLLOW_COLON_in_interpolationMethod3559); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COLON201_tree = (Object)adaptor.create(COLON201);
            adaptor.addChild(root_0, COLON201_tree);
            }
            pushFollow(FOLLOW_nullResistence_in_interpolationMethod3563);
            res=nullResistence();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, res.getTree());
            RPAREN202=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_interpolationMethod3565); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN202_tree = (Object)adaptor.create(RPAREN202);
            adaptor.addChild(root_0, RPAREN202_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new InterpolationMethod((type!=null?type.value:null), (res!=null?res.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 68, interpolationMethod_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "interpolationMethod"

    public static class interpolationType_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "interpolationType"
    // src/java/grammar/wcps.g:346:1: interpolationType returns [String value] : type= ( NEAREST | LINEAR | QUADRATIC | CUBIC ) ;
    public final wcpsParser.interpolationType_return interpolationType() throws RecognitionException {
        wcpsParser.interpolationType_return retval = new wcpsParser.interpolationType_return();
        retval.start = input.LT(1);
        int interpolationType_StartIndex = input.index();
        Object root_0 = null;

        Token type=null;

        Object type_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 69) ) { return retval; }
            // src/java/grammar/wcps.g:347:2: (type= ( NEAREST | LINEAR | QUADRATIC | CUBIC ) )
            // src/java/grammar/wcps.g:347:4: type= ( NEAREST | LINEAR | QUADRATIC | CUBIC )
            {
            root_0 = (Object)adaptor.nil();

            type=(Token)input.LT(1);
            if ( (input.LA(1)>=NEAREST && input.LA(1)<=CUBIC) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(type));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            if ( state.backtracking==0 ) {
               retval.value = new String((type!=null?type.getText():null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 69, interpolationType_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "interpolationType"

    public static class nullResistence_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "nullResistence"
    // src/java/grammar/wcps.g:349:1: nullResistence returns [String value] : resistance= ( FULL | NONE | HALF | OTHER ) ;
    public final wcpsParser.nullResistence_return nullResistence() throws RecognitionException {
        wcpsParser.nullResistence_return retval = new wcpsParser.nullResistence_return();
        retval.start = input.LT(1);
        int nullResistence_StartIndex = input.index();
        Object root_0 = null;

        Token resistance=null;

        Object resistance_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 70) ) { return retval; }
            // src/java/grammar/wcps.g:350:2: (resistance= ( FULL | NONE | HALF | OTHER ) )
            // src/java/grammar/wcps.g:350:4: resistance= ( FULL | NONE | HALF | OTHER )
            {
            root_0 = (Object)adaptor.nil();

            resistance=(Token)input.LT(1);
            if ( (input.LA(1)>=FULL && input.LA(1)<=OTHER) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(resistance));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            if ( state.backtracking==0 ) {
               retval.value = new String((resistance!=null?resistance.getText():null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 70, nullResistence_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "nullResistence"

    public static class condenseOpType_return extends ParserRuleReturnScope {
        public CondenseOperationType value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "condenseOpType"
    // src/java/grammar/wcps.g:352:1: condenseOpType returns [CondenseOperationType value] : op= ( PLUS | MULT | MAX | MIN | AND | OR ) ;
    public final wcpsParser.condenseOpType_return condenseOpType() throws RecognitionException {
        wcpsParser.condenseOpType_return retval = new wcpsParser.condenseOpType_return();
        retval.start = input.LT(1);
        int condenseOpType_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;

        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 71) ) { return retval; }
            // src/java/grammar/wcps.g:353:2: (op= ( PLUS | MULT | MAX | MIN | AND | OR ) )
            // src/java/grammar/wcps.g:353:4: op= ( PLUS | MULT | MAX | MIN | AND | OR )
            {
            root_0 = (Object)adaptor.nil();

            op=(Token)input.LT(1);
            if ( input.LA(1)==OR||input.LA(1)==AND||input.LA(1)==PLUS||input.LA(1)==MULT||(input.LA(1)>=MIN && input.LA(1)<=MAX) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(op));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            if ( state.backtracking==0 ) {
               retval.value = new CondenseOperationType((op!=null?op.getText():null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 71, condenseOpType_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "condenseOpType"

    public static class fieldName_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "fieldName"
    // src/java/grammar/wcps.g:355:1: fieldName returns [String value] : name ;
    public final wcpsParser.fieldName_return fieldName() throws RecognitionException {
        wcpsParser.fieldName_return retval = new wcpsParser.fieldName_return();
        retval.start = input.LT(1);
        int fieldName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return name203 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 72) ) { return retval; }
            // src/java/grammar/wcps.g:356:2: ( name )
            // src/java/grammar/wcps.g:356:4: name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_fieldName3659);
            name203=name();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, name203.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new String((name203!=null?name203.value:null));
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 72, fieldName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "fieldName"

    public static class constant_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "constant"
    // src/java/grammar/wcps.g:358:1: constant returns [String value] : (e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT ) | e1= complexConstant );
    public final wcpsParser.constant_return constant() throws RecognitionException {
        wcpsParser.constant_return retval = new wcpsParser.constant_return();
        retval.start = input.LT(1);
        int constant_StartIndex = input.index();
        Object root_0 = null;

        Token e=null;
        wcpsParser.complexConstant_return e1 = null;


        Object e_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 73) ) { return retval; }
            // src/java/grammar/wcps.g:359:2: (e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT ) | e1= complexConstant )
            int alt62=2;
            int LA62_0 = input.LA(1);

            if ( (LA62_0==INTEGERCONSTANT||LA62_0==STRING||(LA62_0>=BOOLEANCONSTANT && LA62_0<=FLOATCONSTANT)) ) {
                alt62=1;
            }
            else if ( (LA62_0==LPAREN) ) {
                alt62=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 62, 0, input);

                throw nvae;
            }
            switch (alt62) {
                case 1 :
                    // src/java/grammar/wcps.g:359:4: e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT )
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)input.LT(1);
                    if ( input.LA(1)==INTEGERCONSTANT||input.LA(1)==STRING||(input.LA(1)>=BOOLEANCONSTANT && input.LA(1)<=FLOATCONSTANT) ) {
                        input.consume();
                        if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(e));
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    if ( state.backtracking==0 ) {
                       retval.value = (e!=null?e.getText():null); 
                    }

                    }
                    break;
                case 2 :
                    // src/java/grammar/wcps.g:360:4: e1= complexConstant
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_complexConstant_in_constant3693);
                    e1=complexConstant();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;

            }
            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 73, constant_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "constant"

    public static class complexConstant_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "complexConstant"
    // src/java/grammar/wcps.g:362:1: complexConstant returns [String value] : LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN ;
    public final wcpsParser.complexConstant_return complexConstant() throws RecognitionException {
        wcpsParser.complexConstant_return retval = new wcpsParser.complexConstant_return();
        retval.start = input.LT(1);
        int complexConstant_StartIndex = input.index();
        Object root_0 = null;

        Token re1=null;
        Token im1=null;
        Token LPAREN204=null;
        Token COMMA205=null;
        Token RPAREN206=null;

        Object re1_tree=null;
        Object im1_tree=null;
        Object LPAREN204_tree=null;
        Object COMMA205_tree=null;
        Object RPAREN206_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 74) ) { return retval; }
            // src/java/grammar/wcps.g:363:2: ( LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN )
            // src/java/grammar/wcps.g:363:4: LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN
            {
            root_0 = (Object)adaptor.nil();

            LPAREN204=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_complexConstant3708); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN204_tree = (Object)adaptor.create(LPAREN204);
            adaptor.addChild(root_0, LPAREN204_tree);
            }
            re1=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_complexConstant3712); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            re1_tree = (Object)adaptor.create(re1);
            adaptor.addChild(root_0, re1_tree);
            }
            COMMA205=(Token)match(input,COMMA,FOLLOW_COMMA_in_complexConstant3714); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA205_tree = (Object)adaptor.create(COMMA205);
            adaptor.addChild(root_0, COMMA205_tree);
            }
            im1=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_complexConstant3718); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            im1_tree = (Object)adaptor.create(im1);
            adaptor.addChild(root_0, im1_tree);
            }
            RPAREN206=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_complexConstant3720); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN206_tree = (Object)adaptor.create(RPAREN206);
            adaptor.addChild(root_0, RPAREN206_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new String((re1!=null?re1.getText():null) +"+i"+(im1!=null?im1.getText():null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 74, complexConstant_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "complexConstant"

    public static class stringConstant_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "stringConstant"
    // src/java/grammar/wcps.g:365:1: stringConstant returns [String value] : s= STRING ;
    public final wcpsParser.stringConstant_return stringConstant() throws RecognitionException {
        wcpsParser.stringConstant_return retval = new wcpsParser.stringConstant_return();
        retval.start = input.LT(1);
        int stringConstant_StartIndex = input.index();
        Object root_0 = null;

        Token s=null;

        Object s_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 75) ) { return retval; }
            // src/java/grammar/wcps.g:366:2: (s= STRING )
            // src/java/grammar/wcps.g:366:4: s= STRING
            {
            root_0 = (Object)adaptor.nil();

            s=(Token)match(input,STRING,FOLLOW_STRING_in_stringConstant3737); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            s_tree = (Object)adaptor.create(s);
            adaptor.addChild(root_0, s_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = (s!=null?s.getText():null); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 75, stringConstant_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "stringConstant"

    public static class name_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "name"
    // src/java/grammar/wcps.g:368:1: name returns [String value] : var= ( NAME | STRING | INTEGERCONSTANT ) ;
    public final wcpsParser.name_return name() throws RecognitionException {
        wcpsParser.name_return retval = new wcpsParser.name_return();
        retval.start = input.LT(1);
        int name_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 76) ) { return retval; }
            // src/java/grammar/wcps.g:369:2: (var= ( NAME | STRING | INTEGERCONSTANT ) )
            // src/java/grammar/wcps.g:369:4: var= ( NAME | STRING | INTEGERCONSTANT )
            {
            root_0 = (Object)adaptor.nil();

            var=(Token)input.LT(1);
            if ( input.LA(1)==INTEGERCONSTANT||input.LA(1)==STRING||input.LA(1)==NAME ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(var));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            if ( state.backtracking==0 ) {
               retval.value = (var!=null?var.getText():null); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 76, name_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "name"

    public static class crsName_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "crsName"
    // src/java/grammar/wcps.g:371:1: crsName returns [String value] : s= stringConstant ;
    public final wcpsParser.crsName_return crsName() throws RecognitionException {
        wcpsParser.crsName_return retval = new wcpsParser.crsName_return();
        retval.start = input.LT(1);
        int crsName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.stringConstant_return s = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 77) ) { return retval; }
            // src/java/grammar/wcps.g:372:2: (s= stringConstant )
            // src/java/grammar/wcps.g:372:4: s= stringConstant
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_stringConstant_in_crsName3781);
            s=stringConstant();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, s.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (s!=null?s.value:null); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 77, crsName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "crsName"

    public static class axisName_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "axisName"
    // src/java/grammar/wcps.g:374:1: axisName returns [String value] : type1= name ;
    public final wcpsParser.axisName_return axisName() throws RecognitionException {
        wcpsParser.axisName_return retval = new wcpsParser.axisName_return();
        retval.start = input.LT(1);
        int axisName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return type1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 78) ) { return retval; }
            // src/java/grammar/wcps.g:375:2: (type1= name )
            // src/java/grammar/wcps.g:375:4: type1= name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_axisName3798);
            type1=name();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, type1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new String((type1!=null?type1.value:null)); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 78, axisName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "axisName"

    public static class variableName_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "variableName"
    // src/java/grammar/wcps.g:377:1: variableName returns [String value] : var= ( VARIABLE_DOLLAR | NAME ) ;
    public final wcpsParser.variableName_return variableName() throws RecognitionException {
        wcpsParser.variableName_return retval = new wcpsParser.variableName_return();
        retval.start = input.LT(1);
        int variableName_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 79) ) { return retval; }
            // src/java/grammar/wcps.g:378:2: (var= ( VARIABLE_DOLLAR | NAME ) )
            // src/java/grammar/wcps.g:378:4: var= ( VARIABLE_DOLLAR | NAME )
            {
            root_0 = (Object)adaptor.nil();

            var=(Token)input.LT(1);
            if ( (input.LA(1)>=NAME && input.LA(1)<=VARIABLE_DOLLAR) ) {
                input.consume();
                if ( state.backtracking==0 ) adaptor.addChild(root_0, (Object)adaptor.create(var));
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            if ( state.backtracking==0 ) {
               retval.value = (var!=null?var.getText():null); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 79, variableName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "variableName"

    public static class coverageName_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageName"
    // src/java/grammar/wcps.g:380:1: coverageName returns [String value] : name ;
    public final wcpsParser.coverageName_return coverageName() throws RecognitionException {
        wcpsParser.coverageName_return retval = new wcpsParser.coverageName_return();
        retval.start = input.LT(1);
        int coverageName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return name207 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 80) ) { return retval; }
            // src/java/grammar/wcps.g:381:2: ( name )
            // src/java/grammar/wcps.g:381:4: name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_coverageName3836);
            name207=name();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, name207.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (name207!=null?name207.value:null); 
            }

            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);
            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }
        finally {
            if ( state.backtracking>0 ) { memoize(input, 80, coverageName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageName"

    // $ANTLR start synpred8_wcps
    public final void synpred8_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageLogicTerm_return e2 = null;


        // src/java/grammar/wcps.g:63:10: (op= ( OR | XOR ) e2= coverageLogicTerm )
        // src/java/grammar/wcps.g:63:10: op= ( OR | XOR ) e2= coverageLogicTerm
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=OR && input.LA(1)<=XOR) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_coverageLogicTerm_in_synpred8_wcps374);
        e2=coverageLogicTerm();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred8_wcps

    // $ANTLR start synpred9_wcps
    public final void synpred9_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageLogicFactor_return e2 = null;


        // src/java/grammar/wcps.g:67:10: (op= ( AND ) e2= coverageLogicFactor )
        // src/java/grammar/wcps.g:67:10: op= ( AND ) e2= coverageLogicFactor
        {
        // src/java/grammar/wcps.g:67:13: ( AND )
        // src/java/grammar/wcps.g:67:14: AND
        {
        match(input,AND,FOLLOW_AND_in_synpred9_wcps417); if (state.failed) return ;

        }

        pushFollow(FOLLOW_coverageLogicFactor_in_synpred9_wcps422);
        e2=coverageLogicFactor();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred9_wcps

    // $ANTLR start synpred15_wcps
    public final void synpred15_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageArithmeticExpr_return e2 = null;


        // src/java/grammar/wcps.g:71:10: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )
        // src/java/grammar/wcps.g:71:10: op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=EQUALS && input.LA(1)<=GTE) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_coverageArithmeticExpr_in_synpred15_wcps481);
        e2=coverageArithmeticExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred15_wcps

    // $ANTLR start synpred17_wcps
    public final void synpred17_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageArithmeticTerm_return e2 = null;


        // src/java/grammar/wcps.g:75:10: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )
        // src/java/grammar/wcps.g:75:10: op= ( PLUS | MINUS ) e2= coverageArithmeticTerm
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=PLUS && input.LA(1)<=MINUS) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_coverageArithmeticTerm_in_synpred17_wcps533);
        e2=coverageArithmeticTerm();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred17_wcps

    // $ANTLR start synpred19_wcps
    public final void synpred19_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageArithmeticFactor_return e2 = null;


        // src/java/grammar/wcps.g:79:10: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )
        // src/java/grammar/wcps.g:79:10: op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=MULT && input.LA(1)<=DIVIDE) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_coverageArithmeticFactor_in_synpred19_wcps584);
        e2=coverageArithmeticFactor();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred19_wcps

    // $ANTLR start synpred20_wcps
    public final void synpred20_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageValue_return e2 = null;


        // src/java/grammar/wcps.g:83:10: (op= ( OVERLAY ) e2= coverageValue )
        // src/java/grammar/wcps.g:83:10: op= ( OVERLAY ) e2= coverageValue
        {
        // src/java/grammar/wcps.g:83:13: ( OVERLAY )
        // src/java/grammar/wcps.g:83:14: OVERLAY
        {
        match(input,OVERLAY,FOLLOW_OVERLAY_in_synpred20_wcps628); if (state.failed) return ;

        }

        pushFollow(FOLLOW_coverageValue_in_synpred20_wcps633);
        e2=coverageValue();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred20_wcps

    // $ANTLR start synpred21_wcps
    public final void synpred21_wcps_fragment() throws RecognitionException {   
        wcpsParser.subsetExpr_return e5 = null;


        // src/java/grammar/wcps.g:86:7: (e5= subsetExpr )
        // src/java/grammar/wcps.g:86:7: e5= subsetExpr
        {
        pushFollow(FOLLOW_subsetExpr_in_synpred21_wcps659);
        e5=subsetExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred21_wcps

    // $ANTLR start synpred22_wcps
    public final void synpred22_wcps_fragment() throws RecognitionException {   
        wcpsParser.unaryInducedExpr_return e2 = null;


        // src/java/grammar/wcps.g:87:7: (e2= unaryInducedExpr )
        // src/java/grammar/wcps.g:87:7: e2= unaryInducedExpr
        {
        pushFollow(FOLLOW_unaryInducedExpr_in_synpred22_wcps672);
        e2=unaryInducedExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred22_wcps

    // $ANTLR start synpred25_wcps
    public final void synpred25_wcps_fragment() throws RecognitionException {   
        wcpsParser.scalarExpr_return e2 = null;


        // src/java/grammar/wcps.g:93:7: (e2= scalarExpr )
        // src/java/grammar/wcps.g:93:7: e2= scalarExpr
        {
        pushFollow(FOLLOW_scalarExpr_in_synpred25_wcps731);
        e2=scalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred25_wcps

    // $ANTLR start synpred27_wcps
    public final void synpred27_wcps_fragment() throws RecognitionException {   
        wcpsParser.coverageExpr_return e7 = null;


        // src/java/grammar/wcps.g:95:7: ( LPAREN e7= coverageExpr RPAREN )
        // src/java/grammar/wcps.g:95:7: LPAREN e7= coverageExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred27_wcps753); if (state.failed) return ;
        pushFollow(FOLLOW_coverageExpr_in_synpred27_wcps757);
        e7=coverageExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred27_wcps759); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred27_wcps

    // $ANTLR start synpred28_wcps
    public final void synpred28_wcps_fragment() throws RecognitionException {   
        wcpsParser.coverageConstantExpr_return e3 = null;


        // src/java/grammar/wcps.g:96:7: (e3= coverageConstantExpr )
        // src/java/grammar/wcps.g:96:7: e3= coverageConstantExpr
        {
        pushFollow(FOLLOW_coverageConstantExpr_in_synpred28_wcps772);
        e3=coverageConstantExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred28_wcps

    // $ANTLR start synpred29_wcps
    public final void synpred29_wcps_fragment() throws RecognitionException {   
        wcpsParser.coverageConstructorExpr_return e4 = null;


        // src/java/grammar/wcps.g:97:7: (e4= coverageConstructorExpr )
        // src/java/grammar/wcps.g:97:7: e4= coverageConstructorExpr
        {
        pushFollow(FOLLOW_coverageConstructorExpr_in_synpred29_wcps784);
        e4=coverageConstructorExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred29_wcps

    // $ANTLR start synpred31_wcps
    public final void synpred31_wcps_fragment() throws RecognitionException {   
        wcpsParser.metaDataExpr_return e1 = null;


        // src/java/grammar/wcps.g:102:7: (e1= metaDataExpr )
        // src/java/grammar/wcps.g:102:7: e1= metaDataExpr
        {
        pushFollow(FOLLOW_metaDataExpr_in_synpred31_wcps834);
        e1=metaDataExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred31_wcps

    // $ANTLR start synpred32_wcps
    public final void synpred32_wcps_fragment() throws RecognitionException {   
        wcpsParser.condenseExpr_return e2 = null;


        // src/java/grammar/wcps.g:103:7: (e2= condenseExpr )
        // src/java/grammar/wcps.g:103:7: e2= condenseExpr
        {
        pushFollow(FOLLOW_condenseExpr_in_synpred32_wcps847);
        e2=condenseExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred32_wcps

    // $ANTLR start synpred33_wcps
    public final void synpred33_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanScalarExpr_return e3 = null;


        // src/java/grammar/wcps.g:104:7: (e3= booleanScalarExpr )
        // src/java/grammar/wcps.g:104:7: e3= booleanScalarExpr
        {
        pushFollow(FOLLOW_booleanScalarExpr_in_synpred33_wcps860);
        e3=booleanScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred33_wcps

    // $ANTLR start synpred34_wcps
    public final void synpred34_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return e4 = null;


        // src/java/grammar/wcps.g:105:7: (e4= numericScalarExpr )
        // src/java/grammar/wcps.g:105:7: e4= numericScalarExpr
        {
        pushFollow(FOLLOW_numericScalarExpr_in_synpred34_wcps874);
        e4=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred34_wcps

    // $ANTLR start synpred35_wcps
    public final void synpred35_wcps_fragment() throws RecognitionException {   
        wcpsParser.stringScalarExpr_return e5 = null;


        // src/java/grammar/wcps.g:106:7: (e5= stringScalarExpr )
        // src/java/grammar/wcps.g:106:7: e5= stringScalarExpr
        {
        pushFollow(FOLLOW_stringScalarExpr_in_synpred35_wcps887);
        e5=stringScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred35_wcps

    // $ANTLR start synpred70_wcps
    public final void synpred70_wcps_fragment() throws RecognitionException {   
        wcpsParser.fieldExpr_return e6 = null;


        // src/java/grammar/wcps.g:200:7: (e6= fieldExpr )
        // src/java/grammar/wcps.g:200:7: e6= fieldExpr
        {
        pushFollow(FOLLOW_fieldExpr_in_synpred70_wcps2017);
        e6=fieldExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred70_wcps

    // $ANTLR start synpred71_wcps
    public final void synpred71_wcps_fragment() throws RecognitionException {   
        wcpsParser.unaryArithmeticExpr_return e1 = null;


        // src/java/grammar/wcps.g:201:4: (e1= unaryArithmeticExpr )
        // src/java/grammar/wcps.g:201:4: e1= unaryArithmeticExpr
        {
        pushFollow(FOLLOW_unaryArithmeticExpr_in_synpred71_wcps2026);
        e1=unaryArithmeticExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred71_wcps

    // $ANTLR start synpred74_wcps
    public final void synpred74_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanExpr_return e4 = null;


        // src/java/grammar/wcps.g:204:7: (e4= booleanExpr )
        // src/java/grammar/wcps.g:204:7: e4= booleanExpr
        {
        pushFollow(FOLLOW_booleanExpr_in_synpred74_wcps2062);
        e4=booleanExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred74_wcps

    // $ANTLR start synpred95_wcps
    public final void synpred95_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.indexFactor_return e2 = null;


        // src/java/grammar/wcps.g:229:7: (op= ( MULT | DIVIDE ) e2= indexFactor )
        // src/java/grammar/wcps.g:229:8: op= ( MULT | DIVIDE ) e2= indexFactor
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=MULT && input.LA(1)<=DIVIDE) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_indexFactor_in_synpred95_wcps2374);
        e2=indexFactor();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred95_wcps

    // $ANTLR start synpred99_wcps
    public final void synpred99_wcps_fragment() throws RecognitionException {   
        wcpsParser.indexExpr_return e2 = null;


        // src/java/grammar/wcps.g:234:7: ( LPAREN e2= indexExpr RPAREN )
        // src/java/grammar/wcps.g:234:8: LPAREN e2= indexExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred99_wcps2450); if (state.failed) return ;
        pushFollow(FOLLOW_indexExpr_in_synpred99_wcps2454);
        e2=indexExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred99_wcps2456); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred99_wcps

    // $ANTLR start synpred101_wcps
    public final void synpred101_wcps_fragment() throws RecognitionException {   
        wcpsParser.trimExpr_return e1 = null;


        // src/java/grammar/wcps.g:247:4: (e1= trimExpr )
        // src/java/grammar/wcps.g:247:4: e1= trimExpr
        {
        pushFollow(FOLLOW_trimExpr_in_synpred101_wcps2578);
        e1=trimExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred101_wcps

    // $ANTLR start synpred102_wcps
    public final void synpred102_wcps_fragment() throws RecognitionException {   
        wcpsParser.sliceExpr_return e2 = null;


        // src/java/grammar/wcps.g:248:4: (e2= sliceExpr )
        // src/java/grammar/wcps.g:248:4: e2= sliceExpr
        {
        pushFollow(FOLLOW_sliceExpr_in_synpred102_wcps2587);
        e2=sliceExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred102_wcps

    // $ANTLR start synpred116_wcps
    public final void synpred116_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.booleanScalarTerm_return e2 = null;


        // src/java/grammar/wcps.g:276:8: (op= ( OR | XOR ) e2= booleanScalarTerm )
        // src/java/grammar/wcps.g:276:8: op= ( OR | XOR ) e2= booleanScalarTerm
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=OR && input.LA(1)<=XOR) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_booleanScalarTerm_in_synpred116_wcps2891);
        e2=booleanScalarTerm();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred116_wcps

    // $ANTLR start synpred117_wcps
    public final void synpred117_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.booleanScalarNegation_return e2 = null;


        // src/java/grammar/wcps.g:280:5: (op= AND e2= booleanScalarNegation )
        // src/java/grammar/wcps.g:280:5: op= AND e2= booleanScalarNegation
        {
        op=(Token)match(input,AND,FOLLOW_AND_in_synpred117_wcps2923); if (state.failed) return ;
        pushFollow(FOLLOW_booleanScalarNegation_in_synpred117_wcps2927);
        e2=booleanScalarNegation();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred117_wcps

    // $ANTLR start synpred119_wcps
    public final void synpred119_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanScalarExpr_return e1 = null;


        // src/java/grammar/wcps.g:289:4: ( LPAREN e1= booleanScalarExpr RPAREN )
        // src/java/grammar/wcps.g:289:4: LPAREN e1= booleanScalarExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred119_wcps2978); if (state.failed) return ;
        pushFollow(FOLLOW_booleanScalarExpr_in_synpred119_wcps2982);
        e1=booleanScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred119_wcps2984); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred119_wcps

    // $ANTLR start synpred121_wcps
    public final void synpred121_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return n1 = null;

        wcpsParser.compOp_return cop = null;

        wcpsParser.numericScalarExpr_return n2 = null;


        // src/java/grammar/wcps.g:291:4: (n1= numericScalarExpr cop= compOp n2= numericScalarExpr )
        // src/java/grammar/wcps.g:291:4: n1= numericScalarExpr cop= compOp n2= numericScalarExpr
        {
        pushFollow(FOLLOW_numericScalarExpr_in_synpred121_wcps3011);
        n1=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        pushFollow(FOLLOW_compOp_in_synpred121_wcps3015);
        cop=compOp();

        state._fsp--;
        if (state.failed) return ;
        pushFollow(FOLLOW_numericScalarExpr_in_synpred121_wcps3019);
        n2=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred121_wcps

    // $ANTLR start synpred123_wcps
    public final void synpred123_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.numericScalarTerm_return e2 = null;


        // src/java/grammar/wcps.g:297:5: (op= ( PLUS | MINUS ) e2= numericScalarTerm )
        // src/java/grammar/wcps.g:297:5: op= ( PLUS | MINUS ) e2= numericScalarTerm
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=PLUS && input.LA(1)<=MINUS) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_numericScalarTerm_in_synpred123_wcps3067);
        e2=numericScalarTerm();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred123_wcps

    // $ANTLR start synpred125_wcps
    public final void synpred125_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.numericScalarFactor_return e2 = null;


        // src/java/grammar/wcps.g:301:4: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )
        // src/java/grammar/wcps.g:301:4: op= ( MULT | DIVIDE ) e2= numericScalarFactor
        {
        op=(Token)input.LT(1);
        if ( (input.LA(1)>=MULT && input.LA(1)<=DIVIDE) ) {
            input.consume();
            state.errorRecovery=false;state.failed=false;
        }
        else {
            if (state.backtracking>0) {state.failed=true; return ;}
            MismatchedSetException mse = new MismatchedSetException(null,input);
            throw mse;
        }

        pushFollow(FOLLOW_numericScalarFactor_in_synpred125_wcps3105);
        e2=numericScalarFactor();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred125_wcps

    // $ANTLR start synpred126_wcps
    public final void synpred126_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return e1 = null;


        // src/java/grammar/wcps.g:304:7: ( LPAREN e1= numericScalarExpr RPAREN )
        // src/java/grammar/wcps.g:304:7: LPAREN e1= numericScalarExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred126_wcps3125); if (state.failed) return ;
        pushFollow(FOLLOW_numericScalarExpr_in_synpred126_wcps3129);
        e1=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred126_wcps3131); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred126_wcps

    // $ANTLR start synpred131_wcps
    public final void synpred131_wcps_fragment() throws RecognitionException {   
        wcpsParser.complexConstant_return e2 = null;


        // src/java/grammar/wcps.g:309:7: (e2= complexConstant )
        // src/java/grammar/wcps.g:309:7: e2= complexConstant
        {
        pushFollow(FOLLOW_complexConstant_in_synpred131_wcps3204);
        e2=complexConstant();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred131_wcps

    // $ANTLR start synpred139_wcps
    public final void synpred139_wcps_fragment() throws RecognitionException {   
        wcpsParser.scalarExpr_return e1 = null;

        wcpsParser.scalarExpr_return e2 = null;


        // src/java/grammar/wcps.g:329:7: (e1= scalarExpr COLON e2= scalarExpr )
        // src/java/grammar/wcps.g:329:7: e1= scalarExpr COLON e2= scalarExpr
        {
        pushFollow(FOLLOW_scalarExpr_in_synpred139_wcps3380);
        e1=scalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,COLON,FOLLOW_COLON_in_synpred139_wcps3382); if (state.failed) return ;
        pushFollow(FOLLOW_scalarExpr_in_synpred139_wcps3386);
        e2=scalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred139_wcps

    // Delegated rules

    public final boolean synpred102_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred102_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred70_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred70_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred9_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred9_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred15_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred15_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred17_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred17_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred125_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred125_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred19_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred19_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred32_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred32_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred31_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred31_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred21_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred21_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred139_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred139_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred101_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred101_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred74_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred74_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred123_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred123_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred20_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred20_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred119_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred119_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred33_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred33_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred27_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred27_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred22_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred22_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred95_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred95_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred28_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred28_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred29_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred29_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred99_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred99_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred25_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred25_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred131_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred131_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred35_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred35_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred71_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred71_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred34_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred34_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred121_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred121_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred117_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred117_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred8_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred8_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred126_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred126_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred116_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred116_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }


    protected DFA4 dfa4 = new DFA4(this);
    protected DFA6 dfa6 = new DFA6(this);
    protected DFA7 dfa7 = new DFA7(this);
    protected DFA8 dfa8 = new DFA8(this);
    protected DFA9 dfa9 = new DFA9(this);
    protected DFA10 dfa10 = new DFA10(this);
    protected DFA11 dfa11 = new DFA11(this);
    protected DFA12 dfa12 = new DFA12(this);
    protected DFA13 dfa13 = new DFA13(this);
    protected DFA14 dfa14 = new DFA14(this);
    protected DFA34 dfa34 = new DFA34(this);
    protected DFA42 dfa42 = new DFA42(this);
    protected DFA43 dfa43 = new DFA43(this);
    protected DFA44 dfa44 = new DFA44(this);
    protected DFA47 dfa47 = new DFA47(this);
    protected DFA48 dfa48 = new DFA48(this);
    protected DFA49 dfa49 = new DFA49(this);
    protected DFA50 dfa50 = new DFA50(this);
    protected DFA52 dfa52 = new DFA52(this);
    protected DFA54 dfa54 = new DFA54(this);
    protected DFA55 dfa55 = new DFA55(this);
    protected DFA59 dfa59 = new DFA59(this);
    static final String DFA4_eotS =
        "\25\uffff";
    static final String DFA4_eofS =
        "\25\uffff";
    static final String DFA4_minS =
        "\1\6\24\uffff";
    static final String DFA4_maxS =
        "\1\142\24\uffff";
    static final String DFA4_acceptS =
        "\1\uffff\1\1\1\2\1\3\21\uffff";
    static final String DFA4_specialS =
        "\25\uffff}>";
    static final String[] DFA4_transitionS = {
            "\1\3\4\uffff\1\1\1\2\12\uffff\1\3\3\uffff\20\3\41\uffff\1\3"+
            "\1\uffff\3\3\20\uffff\2\3",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA4_eot = DFA.unpackEncodedString(DFA4_eotS);
    static final short[] DFA4_eof = DFA.unpackEncodedString(DFA4_eofS);
    static final char[] DFA4_min = DFA.unpackEncodedStringToUnsignedChars(DFA4_minS);
    static final char[] DFA4_max = DFA.unpackEncodedStringToUnsignedChars(DFA4_maxS);
    static final short[] DFA4_accept = DFA.unpackEncodedString(DFA4_acceptS);
    static final short[] DFA4_special = DFA.unpackEncodedString(DFA4_specialS);
    static final short[][] DFA4_transition;

    static {
        int numStates = DFA4_transitionS.length;
        DFA4_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA4_transition[i] = DFA.unpackEncodedString(DFA4_transitionS[i]);
        }
    }

    class DFA4 extends DFA {

        public DFA4(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 4;
            this.eot = DFA4_eot;
            this.eof = DFA4_eof;
            this.min = DFA4_min;
            this.max = DFA4_max;
            this.accept = DFA4_accept;
            this.special = DFA4_special;
            this.transition = DFA4_transition;
        }
        public String getDescription() {
            return "49:1: processingExpr returns [ProcessingExpr value] : (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr );";
        }
    }
    static final String DFA6_eotS =
        "\62\uffff";
    static final String DFA6_eofS =
        "\1\1\61\uffff";
    static final String DFA6_minS =
        "\1\7\11\uffff\1\0\47\uffff";
    static final String DFA6_maxS =
        "\1\71\11\uffff\1\0\47\uffff";
    static final String DFA6_acceptS =
        "\1\uffff\1\2\57\uffff\1\1";
    static final String DFA6_specialS =
        "\12\uffff\1\0\47\uffff}>";
    static final String[] DFA6_transitionS = {
            "\2\1\4\uffff\2\12\14\1\26\uffff\1\1\7\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA6_eot = DFA.unpackEncodedString(DFA6_eotS);
    static final short[] DFA6_eof = DFA.unpackEncodedString(DFA6_eofS);
    static final char[] DFA6_min = DFA.unpackEncodedStringToUnsignedChars(DFA6_minS);
    static final char[] DFA6_max = DFA.unpackEncodedStringToUnsignedChars(DFA6_maxS);
    static final short[] DFA6_accept = DFA.unpackEncodedString(DFA6_acceptS);
    static final short[] DFA6_special = DFA.unpackEncodedString(DFA6_specialS);
    static final short[][] DFA6_transition;

    static {
        int numStates = DFA6_transitionS.length;
        DFA6_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA6_transition[i] = DFA.unpackEncodedString(DFA6_transitionS[i]);
        }
    }

    class DFA6 extends DFA {

        public DFA6(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 6;
            this.eot = DFA6_eot;
            this.eof = DFA6_eof;
            this.min = DFA6_min;
            this.max = DFA6_max;
            this.accept = DFA6_accept;
            this.special = DFA6_special;
            this.transition = DFA6_transition;
        }
        public String getDescription() {
            return "()* loopback of 63:9: (op= ( OR | XOR ) e2= coverageLogicTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA6_10 = input.LA(1);

                         
                        int index6_10 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred8_wcps()) ) {s = 49;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index6_10);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 6, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA7_eotS =
        "\62\uffff";
    static final String DFA7_eofS =
        "\1\1\61\uffff";
    static final String DFA7_minS =
        "\1\7\11\uffff\1\0\47\uffff";
    static final String DFA7_maxS =
        "\1\71\11\uffff\1\0\47\uffff";
    static final String DFA7_acceptS =
        "\1\uffff\1\2\57\uffff\1\1";
    static final String DFA7_specialS =
        "\12\uffff\1\0\47\uffff}>";
    static final String[] DFA7_transitionS = {
            "\2\1\4\uffff\2\1\1\12\13\1\26\uffff\1\1\7\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA7_eot = DFA.unpackEncodedString(DFA7_eotS);
    static final short[] DFA7_eof = DFA.unpackEncodedString(DFA7_eofS);
    static final char[] DFA7_min = DFA.unpackEncodedStringToUnsignedChars(DFA7_minS);
    static final char[] DFA7_max = DFA.unpackEncodedStringToUnsignedChars(DFA7_maxS);
    static final short[] DFA7_accept = DFA.unpackEncodedString(DFA7_acceptS);
    static final short[] DFA7_special = DFA.unpackEncodedString(DFA7_specialS);
    static final short[][] DFA7_transition;

    static {
        int numStates = DFA7_transitionS.length;
        DFA7_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA7_transition[i] = DFA.unpackEncodedString(DFA7_transitionS[i]);
        }
    }

    class DFA7 extends DFA {

        public DFA7(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 7;
            this.eot = DFA7_eot;
            this.eof = DFA7_eof;
            this.min = DFA7_min;
            this.max = DFA7_max;
            this.accept = DFA7_accept;
            this.special = DFA7_special;
            this.transition = DFA7_transition;
        }
        public String getDescription() {
            return "()* loopback of 67:9: (op= ( AND ) e2= coverageLogicFactor )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA7_10 = input.LA(1);

                         
                        int index7_10 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred9_wcps()) ) {s = 49;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index7_10);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 7, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA8_eotS =
        "\62\uffff";
    static final String DFA8_eofS =
        "\1\2\61\uffff";
    static final String DFA8_minS =
        "\1\7\1\0\60\uffff";
    static final String DFA8_maxS =
        "\1\71\1\0\60\uffff";
    static final String DFA8_acceptS =
        "\2\uffff\1\2\56\uffff\1\1";
    static final String DFA8_specialS =
        "\1\uffff\1\0\60\uffff}>";
    static final String[] DFA8_transitionS = {
            "\2\2\4\uffff\3\2\6\1\5\2\26\uffff\1\2\7\uffff\1\2",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA8_eot = DFA.unpackEncodedString(DFA8_eotS);
    static final short[] DFA8_eof = DFA.unpackEncodedString(DFA8_eofS);
    static final char[] DFA8_min = DFA.unpackEncodedStringToUnsignedChars(DFA8_minS);
    static final char[] DFA8_max = DFA.unpackEncodedStringToUnsignedChars(DFA8_maxS);
    static final short[] DFA8_accept = DFA.unpackEncodedString(DFA8_acceptS);
    static final short[] DFA8_special = DFA.unpackEncodedString(DFA8_specialS);
    static final short[][] DFA8_transition;

    static {
        int numStates = DFA8_transitionS.length;
        DFA8_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA8_transition[i] = DFA.unpackEncodedString(DFA8_transitionS[i]);
        }
    }

    class DFA8 extends DFA {

        public DFA8(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 8;
            this.eot = DFA8_eot;
            this.eof = DFA8_eof;
            this.min = DFA8_min;
            this.max = DFA8_max;
            this.accept = DFA8_accept;
            this.special = DFA8_special;
            this.transition = DFA8_transition;
        }
        public String getDescription() {
            return "71:9: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA8_1 = input.LA(1);

                         
                        int index8_1 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred15_wcps()) ) {s = 49;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_1);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 8, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA9_eotS =
        "\62\uffff";
    static final String DFA9_eofS =
        "\1\1\61\uffff";
    static final String DFA9_minS =
        "\1\7\11\uffff\1\0\47\uffff";
    static final String DFA9_maxS =
        "\1\71\11\uffff\1\0\47\uffff";
    static final String DFA9_acceptS =
        "\1\uffff\1\2\57\uffff\1\1";
    static final String DFA9_specialS =
        "\12\uffff\1\0\47\uffff}>";
    static final String[] DFA9_transitionS = {
            "\2\1\4\uffff\11\1\2\12\3\1\26\uffff\1\1\7\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA9_eot = DFA.unpackEncodedString(DFA9_eotS);
    static final short[] DFA9_eof = DFA.unpackEncodedString(DFA9_eofS);
    static final char[] DFA9_min = DFA.unpackEncodedStringToUnsignedChars(DFA9_minS);
    static final char[] DFA9_max = DFA.unpackEncodedStringToUnsignedChars(DFA9_maxS);
    static final short[] DFA9_accept = DFA.unpackEncodedString(DFA9_acceptS);
    static final short[] DFA9_special = DFA.unpackEncodedString(DFA9_specialS);
    static final short[][] DFA9_transition;

    static {
        int numStates = DFA9_transitionS.length;
        DFA9_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA9_transition[i] = DFA.unpackEncodedString(DFA9_transitionS[i]);
        }
    }

    class DFA9 extends DFA {

        public DFA9(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 9;
            this.eot = DFA9_eot;
            this.eof = DFA9_eof;
            this.min = DFA9_min;
            this.max = DFA9_max;
            this.accept = DFA9_accept;
            this.special = DFA9_special;
            this.transition = DFA9_transition;
        }
        public String getDescription() {
            return "()* loopback of 75:9: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA9_10 = input.LA(1);

                         
                        int index9_10 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred17_wcps()) ) {s = 49;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index9_10);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 9, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA10_eotS =
        "\62\uffff";
    static final String DFA10_eofS =
        "\1\1\61\uffff";
    static final String DFA10_minS =
        "\1\7\11\uffff\1\0\47\uffff";
    static final String DFA10_maxS =
        "\1\71\11\uffff\1\0\47\uffff";
    static final String DFA10_acceptS =
        "\1\uffff\1\2\57\uffff\1\1";
    static final String DFA10_specialS =
        "\12\uffff\1\0\47\uffff}>";
    static final String[] DFA10_transitionS = {
            "\2\1\4\uffff\13\1\2\12\1\1\26\uffff\1\1\7\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA10_eot = DFA.unpackEncodedString(DFA10_eotS);
    static final short[] DFA10_eof = DFA.unpackEncodedString(DFA10_eofS);
    static final char[] DFA10_min = DFA.unpackEncodedStringToUnsignedChars(DFA10_minS);
    static final char[] DFA10_max = DFA.unpackEncodedStringToUnsignedChars(DFA10_maxS);
    static final short[] DFA10_accept = DFA.unpackEncodedString(DFA10_acceptS);
    static final short[] DFA10_special = DFA.unpackEncodedString(DFA10_specialS);
    static final short[][] DFA10_transition;

    static {
        int numStates = DFA10_transitionS.length;
        DFA10_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA10_transition[i] = DFA.unpackEncodedString(DFA10_transitionS[i]);
        }
    }

    class DFA10 extends DFA {

        public DFA10(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 10;
            this.eot = DFA10_eot;
            this.eof = DFA10_eof;
            this.min = DFA10_min;
            this.max = DFA10_max;
            this.accept = DFA10_accept;
            this.special = DFA10_special;
            this.transition = DFA10_transition;
        }
        public String getDescription() {
            return "()* loopback of 79:9: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA10_10 = input.LA(1);

                         
                        int index10_10 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred19_wcps()) ) {s = 49;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index10_10);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 10, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA11_eotS =
        "\62\uffff";
    static final String DFA11_eofS =
        "\1\1\61\uffff";
    static final String DFA11_minS =
        "\1\7\11\uffff\1\0\47\uffff";
    static final String DFA11_maxS =
        "\1\71\11\uffff\1\0\47\uffff";
    static final String DFA11_acceptS =
        "\1\uffff\1\2\57\uffff\1\1";
    static final String DFA11_specialS =
        "\12\uffff\1\0\47\uffff}>";
    static final String[] DFA11_transitionS = {
            "\2\1\4\uffff\15\1\1\12\26\uffff\1\1\7\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA11_eot = DFA.unpackEncodedString(DFA11_eotS);
    static final short[] DFA11_eof = DFA.unpackEncodedString(DFA11_eofS);
    static final char[] DFA11_min = DFA.unpackEncodedStringToUnsignedChars(DFA11_minS);
    static final char[] DFA11_max = DFA.unpackEncodedStringToUnsignedChars(DFA11_maxS);
    static final short[] DFA11_accept = DFA.unpackEncodedString(DFA11_acceptS);
    static final short[] DFA11_special = DFA.unpackEncodedString(DFA11_specialS);
    static final short[][] DFA11_transition;

    static {
        int numStates = DFA11_transitionS.length;
        DFA11_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA11_transition[i] = DFA.unpackEncodedString(DFA11_transitionS[i]);
        }
    }

    class DFA11 extends DFA {

        public DFA11(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 11;
            this.eot = DFA11_eot;
            this.eof = DFA11_eof;
            this.min = DFA11_min;
            this.max = DFA11_max;
            this.accept = DFA11_accept;
            this.special = DFA11_special;
            this.transition = DFA11_transition;
        }
        public String getDescription() {
            return "()* loopback of 83:9: (op= ( OVERLAY ) e2= coverageValue )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA11_10 = input.LA(1);

                         
                        int index11_10 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred20_wcps()) ) {s = 49;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index11_10);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 11, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA12_eotS =
        "\u00f5\uffff";
    static final String DFA12_eofS =
        "\14\uffff\1\137\2\uffff\3\137\1\uffff\1\137\u00e1\uffff";
    static final String DFA12_minS =
        "\12\6\1\15\1\6\1\7\2\6\3\7\1\6\1\7\1\116\5\6\1\70\1\116\12\uffff"+
        "\57\0\3\uffff\6\0\13\uffff\7\0\24\uffff\13\0\12\uffff\4\0\1\uffff"+
        "\6\0\11\uffff\3\0\13\uffff\12\0\50\uffff\10\0";
    static final String DFA12_maxS =
        "\1\154\11\6\1\51\1\154\1\140\1\154\1\6\3\140\1\154\1\140\1\153"+
        "\5\6\1\70\1\153\12\uffff\57\0\3\uffff\6\0\13\uffff\7\0\24\uffff"+
        "\13\0\12\uffff\4\0\1\uffff\6\0\11\uffff\3\0\13\uffff\12\0\50\uffff"+
        "\10\0";
    static final String DFA12_acceptS =
        "\34\uffff\1\1\2\uffff\1\2\4\uffff\1\3\1\4\71\uffff\1\5\u0095\uffff";
    static final String DFA12_specialS =
        "\46\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\1"+
        "\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\1\25\1\26\1\27\1\30"+
        "\1\31\1\32\1\33\1\34\1\35\1\36\1\37\1\40\1\41\1\42\1\43\1\44\1\45"+
        "\1\46\1\47\1\50\1\51\1\52\1\53\1\54\1\55\1\56\3\uffff\1\57\1\60"+
        "\1\61\1\62\1\63\1\64\13\uffff\1\65\1\66\1\67\1\70\1\71\1\72\1\73"+
        "\24\uffff\1\74\1\75\1\76\1\77\1\100\1\101\1\102\1\103\1\104\1\105"+
        "\1\106\12\uffff\1\107\1\110\1\111\1\112\1\uffff\1\113\1\114\1\115"+
        "\1\116\1\117\1\120\11\uffff\1\121\1\122\1\123\13\uffff\1\124\1\125"+
        "\1\126\1\127\1\130\1\131\1\132\1\133\1\134\1\135\50\uffff\1\136"+
        "\1\137\1\140\1\141\1\142\1\143\1\144\1\145}>";
    static final String[] DFA12_transitionS = {
            "\1\13\17\uffff\1\37\1\15\3\uffff\1\1\1\2\1\3\1\4\1\6\1\7\1"+
            "\10\1\5\7\11\1\12\3\uffff\1\24\4\uffff\1\25\1\26\1\27\1\30\1"+
            "\31\1\33\1\uffff\1\32\1\45\20\37\1\22\1\37\1\17\1\16\1\14\1"+
            "\44\2\uffff\3\34\12\uffff\1\21\1\20\10\uffff\2\23",
            "\1\46",
            "\1\47",
            "\1\50",
            "\1\51",
            "\1\52",
            "\1\53",
            "\1\54",
            "\1\55",
            "\1\56",
            "\1\57\1\uffff\1\57\6\uffff\1\57\1\uffff\1\57\17\uffff\2\57",
            "\1\61\17\uffff\1\116\1\62\3\uffff\1\67\1\73\1\74\1\75\1\77"+
            "\1\100\1\101\1\76\7\65\1\66\3\uffff\1\103\4\uffff\1\104\1\105"+
            "\1\106\1\107\1\110\1\112\1\uffff\1\111\1\124\4\117\3\120\11"+
            "\121\1\72\1\122\1\64\1\63\1\70\1\123\2\uffff\1\113\1\114\1\115"+
            "\11\37\1\uffff\1\71\1\60\10\uffff\2\102",
            "\2\137\4\uffff\3\137\1\130\1\131\1\132\1\133\1\134\1\135\5"+
            "\137\26\uffff\1\137\7\uffff\1\137\30\uffff\1\34\15\uffff\1\37",
            "\1\151\20\uffff\1\152\3\uffff\10\37\7\156\1\157\3\uffff\1"+
            "\37\4\uffff\6\37\1\uffff\1\37\21\uffff\1\37\1\uffff\1\154\1"+
            "\153\1\37\20\uffff\1\37\1\155\10\uffff\2\37",
            "\1\u0084",
            "\2\137\4\uffff\3\137\1\u0089\1\u008a\1\u008b\1\u008c\1\u008d"+
            "\1\u008e\1\u0087\1\u0088\1\u0085\1\u0086\1\137\26\uffff\1\137"+
            "\7\uffff\1\137\30\uffff\1\34\15\uffff\1\37",
            "\2\137\4\uffff\3\137\1\u009e\1\u009f\1\u00a0\1\u00a1\1\u00a2"+
            "\1\u00a3\1\u009b\1\u009c\1\u0099\1\u009a\1\137\26\uffff\1\137"+
            "\7\uffff\1\137\30\uffff\1\34\15\uffff\1\37",
            "\2\137\4\uffff\1\u00ae\1\u00af\1\u00ad\13\137\26\uffff\1\137"+
            "\7\uffff\1\137\30\uffff\1\34\15\uffff\1\37",
            "\1\u00bb\17\uffff\1\37\1\u00be\3\uffff\1\u00bc\7\37\7\u00c2"+
            "\1\u00c3\3\uffff\1\37\4\uffff\6\37\1\uffff\24\37\1\u00c0\1\u00bf"+
            "\1\u00bd\1\37\2\uffff\3\37\12\uffff\1\u00c4\1\u00c1\10\uffff"+
            "\2\37",
            "\2\137\4\uffff\16\137\26\uffff\1\137\7\uffff\1\137\30\uffff"+
            "\1\34\15\uffff\1\37",
            "\1\u00ed\1\uffff\1\u00ed\32\uffff\1\u00ed",
            "\1\u00ee",
            "\1\u00ef",
            "\1\u00f0",
            "\1\u00f1",
            "\1\u00f2",
            "\1\u00f3",
            "\1\u00f4\1\uffff\1\u00f4\32\uffff\1\u00f4",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff"
    };

    static final short[] DFA12_eot = DFA.unpackEncodedString(DFA12_eotS);
    static final short[] DFA12_eof = DFA.unpackEncodedString(DFA12_eofS);
    static final char[] DFA12_min = DFA.unpackEncodedStringToUnsignedChars(DFA12_minS);
    static final char[] DFA12_max = DFA.unpackEncodedStringToUnsignedChars(DFA12_maxS);
    static final short[] DFA12_accept = DFA.unpackEncodedString(DFA12_acceptS);
    static final short[] DFA12_special = DFA.unpackEncodedString(DFA12_specialS);
    static final short[][] DFA12_transition;

    static {
        int numStates = DFA12_transitionS.length;
        DFA12_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA12_transition[i] = DFA.unpackEncodedString(DFA12_transitionS[i]);
        }
    }

    class DFA12 extends DFA {

        public DFA12(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 12;
            this.eot = DFA12_eot;
            this.eof = DFA12_eof;
            this.min = DFA12_min;
            this.max = DFA12_max;
            this.accept = DFA12_accept;
            this.special = DFA12_special;
            this.transition = DFA12_transition;
        }
        public String getDescription() {
            return "85:1: coverageValue returns [CoverageExpr value] : (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA12_38 = input.LA(1);

                         
                        int index12_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_38);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA12_39 = input.LA(1);

                         
                        int index12_39 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_39);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA12_40 = input.LA(1);

                         
                        int index12_40 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_40);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA12_41 = input.LA(1);

                         
                        int index12_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_41);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA12_42 = input.LA(1);

                         
                        int index12_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_42);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA12_43 = input.LA(1);

                         
                        int index12_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_43);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA12_44 = input.LA(1);

                         
                        int index12_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_44);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA12_45 = input.LA(1);

                         
                        int index12_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_45);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA12_46 = input.LA(1);

                         
                        int index12_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_46);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA12_47 = input.LA(1);

                         
                        int index12_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_47);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA12_48 = input.LA(1);

                         
                        int index12_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_48);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA12_49 = input.LA(1);

                         
                        int index12_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_49);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA12_50 = input.LA(1);

                         
                        int index12_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_50);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA12_51 = input.LA(1);

                         
                        int index12_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_51);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA12_52 = input.LA(1);

                         
                        int index12_52 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_52);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA12_53 = input.LA(1);

                         
                        int index12_53 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_53);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA12_54 = input.LA(1);

                         
                        int index12_54 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_54);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA12_55 = input.LA(1);

                         
                        int index12_55 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_55);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA12_56 = input.LA(1);

                         
                        int index12_56 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_56);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA12_57 = input.LA(1);

                         
                        int index12_57 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_57);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA12_58 = input.LA(1);

                         
                        int index12_58 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_58);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA12_59 = input.LA(1);

                         
                        int index12_59 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_59);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA12_60 = input.LA(1);

                         
                        int index12_60 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_60);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA12_61 = input.LA(1);

                         
                        int index12_61 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_61);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA12_62 = input.LA(1);

                         
                        int index12_62 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_62);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA12_63 = input.LA(1);

                         
                        int index12_63 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_63);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA12_64 = input.LA(1);

                         
                        int index12_64 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_64);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA12_65 = input.LA(1);

                         
                        int index12_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_65);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA12_66 = input.LA(1);

                         
                        int index12_66 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_66);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA12_67 = input.LA(1);

                         
                        int index12_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_67);
                        if ( s>=0 ) return s;
                        break;
                    case 30 : 
                        int LA12_68 = input.LA(1);

                         
                        int index12_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_68);
                        if ( s>=0 ) return s;
                        break;
                    case 31 : 
                        int LA12_69 = input.LA(1);

                         
                        int index12_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_69);
                        if ( s>=0 ) return s;
                        break;
                    case 32 : 
                        int LA12_70 = input.LA(1);

                         
                        int index12_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_70);
                        if ( s>=0 ) return s;
                        break;
                    case 33 : 
                        int LA12_71 = input.LA(1);

                         
                        int index12_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_71);
                        if ( s>=0 ) return s;
                        break;
                    case 34 : 
                        int LA12_72 = input.LA(1);

                         
                        int index12_72 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_72);
                        if ( s>=0 ) return s;
                        break;
                    case 35 : 
                        int LA12_73 = input.LA(1);

                         
                        int index12_73 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_73);
                        if ( s>=0 ) return s;
                        break;
                    case 36 : 
                        int LA12_74 = input.LA(1);

                         
                        int index12_74 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_74);
                        if ( s>=0 ) return s;
                        break;
                    case 37 : 
                        int LA12_75 = input.LA(1);

                         
                        int index12_75 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_75);
                        if ( s>=0 ) return s;
                        break;
                    case 38 : 
                        int LA12_76 = input.LA(1);

                         
                        int index12_76 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_76);
                        if ( s>=0 ) return s;
                        break;
                    case 39 : 
                        int LA12_77 = input.LA(1);

                         
                        int index12_77 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_77);
                        if ( s>=0 ) return s;
                        break;
                    case 40 : 
                        int LA12_78 = input.LA(1);

                         
                        int index12_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_78);
                        if ( s>=0 ) return s;
                        break;
                    case 41 : 
                        int LA12_79 = input.LA(1);

                         
                        int index12_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_79);
                        if ( s>=0 ) return s;
                        break;
                    case 42 : 
                        int LA12_80 = input.LA(1);

                         
                        int index12_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_80);
                        if ( s>=0 ) return s;
                        break;
                    case 43 : 
                        int LA12_81 = input.LA(1);

                         
                        int index12_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_81);
                        if ( s>=0 ) return s;
                        break;
                    case 44 : 
                        int LA12_82 = input.LA(1);

                         
                        int index12_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_82);
                        if ( s>=0 ) return s;
                        break;
                    case 45 : 
                        int LA12_83 = input.LA(1);

                         
                        int index12_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_83);
                        if ( s>=0 ) return s;
                        break;
                    case 46 : 
                        int LA12_84 = input.LA(1);

                         
                        int index12_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_84);
                        if ( s>=0 ) return s;
                        break;
                    case 47 : 
                        int LA12_88 = input.LA(1);

                         
                        int index12_88 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_88);
                        if ( s>=0 ) return s;
                        break;
                    case 48 : 
                        int LA12_89 = input.LA(1);

                         
                        int index12_89 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_89);
                        if ( s>=0 ) return s;
                        break;
                    case 49 : 
                        int LA12_90 = input.LA(1);

                         
                        int index12_90 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_90);
                        if ( s>=0 ) return s;
                        break;
                    case 50 : 
                        int LA12_91 = input.LA(1);

                         
                        int index12_91 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_91);
                        if ( s>=0 ) return s;
                        break;
                    case 51 : 
                        int LA12_92 = input.LA(1);

                         
                        int index12_92 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_92);
                        if ( s>=0 ) return s;
                        break;
                    case 52 : 
                        int LA12_93 = input.LA(1);

                         
                        int index12_93 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_93);
                        if ( s>=0 ) return s;
                        break;
                    case 53 : 
                        int LA12_105 = input.LA(1);

                         
                        int index12_105 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_105);
                        if ( s>=0 ) return s;
                        break;
                    case 54 : 
                        int LA12_106 = input.LA(1);

                         
                        int index12_106 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_106);
                        if ( s>=0 ) return s;
                        break;
                    case 55 : 
                        int LA12_107 = input.LA(1);

                         
                        int index12_107 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_107);
                        if ( s>=0 ) return s;
                        break;
                    case 56 : 
                        int LA12_108 = input.LA(1);

                         
                        int index12_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_108);
                        if ( s>=0 ) return s;
                        break;
                    case 57 : 
                        int LA12_109 = input.LA(1);

                         
                        int index12_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_109);
                        if ( s>=0 ) return s;
                        break;
                    case 58 : 
                        int LA12_110 = input.LA(1);

                         
                        int index12_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_110);
                        if ( s>=0 ) return s;
                        break;
                    case 59 : 
                        int LA12_111 = input.LA(1);

                         
                        int index12_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_111);
                        if ( s>=0 ) return s;
                        break;
                    case 60 : 
                        int LA12_132 = input.LA(1);

                         
                        int index12_132 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_132);
                        if ( s>=0 ) return s;
                        break;
                    case 61 : 
                        int LA12_133 = input.LA(1);

                         
                        int index12_133 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_133);
                        if ( s>=0 ) return s;
                        break;
                    case 62 : 
                        int LA12_134 = input.LA(1);

                         
                        int index12_134 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_134);
                        if ( s>=0 ) return s;
                        break;
                    case 63 : 
                        int LA12_135 = input.LA(1);

                         
                        int index12_135 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_135);
                        if ( s>=0 ) return s;
                        break;
                    case 64 : 
                        int LA12_136 = input.LA(1);

                         
                        int index12_136 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_136);
                        if ( s>=0 ) return s;
                        break;
                    case 65 : 
                        int LA12_137 = input.LA(1);

                         
                        int index12_137 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_137);
                        if ( s>=0 ) return s;
                        break;
                    case 66 : 
                        int LA12_138 = input.LA(1);

                         
                        int index12_138 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_138);
                        if ( s>=0 ) return s;
                        break;
                    case 67 : 
                        int LA12_139 = input.LA(1);

                         
                        int index12_139 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_139);
                        if ( s>=0 ) return s;
                        break;
                    case 68 : 
                        int LA12_140 = input.LA(1);

                         
                        int index12_140 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_140);
                        if ( s>=0 ) return s;
                        break;
                    case 69 : 
                        int LA12_141 = input.LA(1);

                         
                        int index12_141 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_141);
                        if ( s>=0 ) return s;
                        break;
                    case 70 : 
                        int LA12_142 = input.LA(1);

                         
                        int index12_142 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_142);
                        if ( s>=0 ) return s;
                        break;
                    case 71 : 
                        int LA12_153 = input.LA(1);

                         
                        int index12_153 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_153);
                        if ( s>=0 ) return s;
                        break;
                    case 72 : 
                        int LA12_154 = input.LA(1);

                         
                        int index12_154 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_154);
                        if ( s>=0 ) return s;
                        break;
                    case 73 : 
                        int LA12_155 = input.LA(1);

                         
                        int index12_155 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_155);
                        if ( s>=0 ) return s;
                        break;
                    case 74 : 
                        int LA12_156 = input.LA(1);

                         
                        int index12_156 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_156);
                        if ( s>=0 ) return s;
                        break;
                    case 75 : 
                        int LA12_158 = input.LA(1);

                         
                        int index12_158 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_158);
                        if ( s>=0 ) return s;
                        break;
                    case 76 : 
                        int LA12_159 = input.LA(1);

                         
                        int index12_159 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_159);
                        if ( s>=0 ) return s;
                        break;
                    case 77 : 
                        int LA12_160 = input.LA(1);

                         
                        int index12_160 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_160);
                        if ( s>=0 ) return s;
                        break;
                    case 78 : 
                        int LA12_161 = input.LA(1);

                         
                        int index12_161 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_161);
                        if ( s>=0 ) return s;
                        break;
                    case 79 : 
                        int LA12_162 = input.LA(1);

                         
                        int index12_162 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_162);
                        if ( s>=0 ) return s;
                        break;
                    case 80 : 
                        int LA12_163 = input.LA(1);

                         
                        int index12_163 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_163);
                        if ( s>=0 ) return s;
                        break;
                    case 81 : 
                        int LA12_173 = input.LA(1);

                         
                        int index12_173 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_173);
                        if ( s>=0 ) return s;
                        break;
                    case 82 : 
                        int LA12_174 = input.LA(1);

                         
                        int index12_174 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_174);
                        if ( s>=0 ) return s;
                        break;
                    case 83 : 
                        int LA12_175 = input.LA(1);

                         
                        int index12_175 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_175);
                        if ( s>=0 ) return s;
                        break;
                    case 84 : 
                        int LA12_187 = input.LA(1);

                         
                        int index12_187 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_187);
                        if ( s>=0 ) return s;
                        break;
                    case 85 : 
                        int LA12_188 = input.LA(1);

                         
                        int index12_188 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_188);
                        if ( s>=0 ) return s;
                        break;
                    case 86 : 
                        int LA12_189 = input.LA(1);

                         
                        int index12_189 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_189);
                        if ( s>=0 ) return s;
                        break;
                    case 87 : 
                        int LA12_190 = input.LA(1);

                         
                        int index12_190 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_190);
                        if ( s>=0 ) return s;
                        break;
                    case 88 : 
                        int LA12_191 = input.LA(1);

                         
                        int index12_191 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_191);
                        if ( s>=0 ) return s;
                        break;
                    case 89 : 
                        int LA12_192 = input.LA(1);

                         
                        int index12_192 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_192);
                        if ( s>=0 ) return s;
                        break;
                    case 90 : 
                        int LA12_193 = input.LA(1);

                         
                        int index12_193 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_193);
                        if ( s>=0 ) return s;
                        break;
                    case 91 : 
                        int LA12_194 = input.LA(1);

                         
                        int index12_194 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_194);
                        if ( s>=0 ) return s;
                        break;
                    case 92 : 
                        int LA12_195 = input.LA(1);

                         
                        int index12_195 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_195);
                        if ( s>=0 ) return s;
                        break;
                    case 93 : 
                        int LA12_196 = input.LA(1);

                         
                        int index12_196 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_196);
                        if ( s>=0 ) return s;
                        break;
                    case 94 : 
                        int LA12_237 = input.LA(1);

                         
                        int index12_237 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_237);
                        if ( s>=0 ) return s;
                        break;
                    case 95 : 
                        int LA12_238 = input.LA(1);

                         
                        int index12_238 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_238);
                        if ( s>=0 ) return s;
                        break;
                    case 96 : 
                        int LA12_239 = input.LA(1);

                         
                        int index12_239 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_239);
                        if ( s>=0 ) return s;
                        break;
                    case 97 : 
                        int LA12_240 = input.LA(1);

                         
                        int index12_240 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_240);
                        if ( s>=0 ) return s;
                        break;
                    case 98 : 
                        int LA12_241 = input.LA(1);

                         
                        int index12_241 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_241);
                        if ( s>=0 ) return s;
                        break;
                    case 99 : 
                        int LA12_242 = input.LA(1);

                         
                        int index12_242 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_242);
                        if ( s>=0 ) return s;
                        break;
                    case 100 : 
                        int LA12_243 = input.LA(1);

                         
                        int index12_243 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_243);
                        if ( s>=0 ) return s;
                        break;
                    case 101 : 
                        int LA12_244 = input.LA(1);

                         
                        int index12_244 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 28;}

                        else if ( (synpred22_wcps()) ) {s = 31;}

                        else if ( (true) ) {s = 95;}

                         
                        input.seek(index12_244);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 12, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA13_eotS =
        "\104\uffff";
    static final String DFA13_eofS =
        "\104\uffff";
    static final String DFA13_minS =
        "\1\6\12\uffff\1\6\10\uffff\1\116\7\uffff\22\0\23\uffff\1\0\2\uffff";
    static final String DFA13_maxS =
        "\1\154\12\uffff\1\154\10\uffff\1\153\7\uffff\22\0\23\uffff\1\0"+
        "\2\uffff";
    static final String DFA13_acceptS =
        "\1\uffff\1\1\21\uffff\1\2\1\uffff\1\6\4\uffff\1\7\23\uffff\1\3"+
        "\23\uffff\1\4\1\5";
    static final String DFA13_specialS =
        "\34\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\1"+
        "\14\1\15\1\16\1\17\1\20\1\21\23\uffff\1\22\2\uffff}>";
    static final String[] DFA13_transitionS = {
            "\1\13\20\uffff\1\1\3\uffff\20\1\3\uffff\1\24\4\uffff\5\25\1"+
            "\32\1\uffff\1\32\21\uffff\1\1\1\uffff\3\1\20\uffff\2\1\10\uffff"+
            "\2\23",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\47\17\uffff\1\56\1\51\3\uffff\1\35\1\36\1\37\1\40\1\42"+
            "\1\43\1\44\1\41\7\45\1\46\3\uffff\1\56\4\uffff\6\56\1\uffff"+
            "\22\56\1\55\1\56\1\53\1\52\1\50\1\56\2\uffff\3\56\12\uffff\1"+
            "\54\1\34\10\uffff\2\56",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\101\1\uffff\1\101\32\uffff\1\101",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            ""
    };

    static final short[] DFA13_eot = DFA.unpackEncodedString(DFA13_eotS);
    static final short[] DFA13_eof = DFA.unpackEncodedString(DFA13_eofS);
    static final char[] DFA13_min = DFA.unpackEncodedStringToUnsignedChars(DFA13_minS);
    static final char[] DFA13_max = DFA.unpackEncodedStringToUnsignedChars(DFA13_maxS);
    static final short[] DFA13_accept = DFA.unpackEncodedString(DFA13_acceptS);
    static final short[] DFA13_special = DFA.unpackEncodedString(DFA13_specialS);
    static final short[][] DFA13_transition;

    static {
        int numStates = DFA13_transitionS.length;
        DFA13_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA13_transition[i] = DFA.unpackEncodedString(DFA13_transitionS[i]);
        }
    }

    class DFA13 extends DFA {

        public DFA13(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 13;
            this.eot = DFA13_eot;
            this.eof = DFA13_eof;
            this.min = DFA13_min;
            this.max = DFA13_max;
            this.accept = DFA13_accept;
            this.special = DFA13_special;
            this.transition = DFA13_transition;
        }
        public String getDescription() {
            return "92:1: coverageAtom returns [CoverageExpr value] : (e2= scalarExpr | e1= variableName | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA13_28 = input.LA(1);

                         
                        int index13_28 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_28);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA13_29 = input.LA(1);

                         
                        int index13_29 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_29);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA13_30 = input.LA(1);

                         
                        int index13_30 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_30);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA13_31 = input.LA(1);

                         
                        int index13_31 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_31);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA13_32 = input.LA(1);

                         
                        int index13_32 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_32);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA13_33 = input.LA(1);

                         
                        int index13_33 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_33);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA13_34 = input.LA(1);

                         
                        int index13_34 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_34);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA13_35 = input.LA(1);

                         
                        int index13_35 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_35);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA13_36 = input.LA(1);

                         
                        int index13_36 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_36);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA13_37 = input.LA(1);

                         
                        int index13_37 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_37);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA13_38 = input.LA(1);

                         
                        int index13_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_38);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA13_39 = input.LA(1);

                         
                        int index13_39 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_39);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA13_40 = input.LA(1);

                         
                        int index13_40 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_40);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA13_41 = input.LA(1);

                         
                        int index13_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_41);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA13_42 = input.LA(1);

                         
                        int index13_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_42);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA13_43 = input.LA(1);

                         
                        int index13_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_43);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA13_44 = input.LA(1);

                         
                        int index13_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_44);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA13_45 = input.LA(1);

                         
                        int index13_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 46;}

                         
                        input.seek(index13_45);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA13_65 = input.LA(1);

                         
                        int index13_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred28_wcps()) ) {s = 66;}

                        else if ( (synpred29_wcps()) ) {s = 67;}

                         
                        input.seek(index13_65);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 13, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA14_eotS =
        "\172\uffff";
    static final String DFA14_eofS =
        "\14\uffff\1\50\155\uffff";
    static final String DFA14_minS =
        "\2\6\7\uffff\1\6\1\15\1\6\1\7\2\6\2\0\2\uffff\16\0\13\uffff\1\0"+
        "\12\uffff\5\0\4\uffff\10\0\62\uffff";
    static final String DFA14_maxS =
        "\1\142\1\6\7\uffff\1\6\1\51\1\142\1\140\1\142\1\6\2\0\2\uffff\16"+
        "\0\13\uffff\1\0\12\uffff\5\0\4\uffff\10\0\62\uffff";
    static final String DFA14_acceptS =
        "\2\uffff\1\1\16\uffff\1\3\17\uffff\1\6\6\uffff\1\5\67\uffff\1\4"+
        "\30\uffff\1\2";
    static final String DFA14_specialS =
        "\17\uffff\1\0\1\1\2\uffff\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12"+
        "\1\13\1\14\1\15\1\16\1\17\13\uffff\1\20\12\uffff\1\21\1\22\1\23"+
        "\1\24\1\25\4\uffff\1\26\1\27\1\30\1\31\1\32\1\33\1\34\1\35\62\uffff}>";
    static final String[] DFA14_transitionS = {
            "\1\13\20\uffff\1\15\3\uffff\1\1\7\2\7\11\1\12\41\uffff\1\21"+
            "\1\uffff\1\17\1\16\1\14\20\uffff\1\21\1\20",
            "\1\23",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\24",
            "\1\25\1\uffff\1\25\6\uffff\1\25\1\uffff\1\25\17\uffff\2\25",
            "\1\27\20\uffff\1\32\3\uffff\1\30\7\41\7\35\1\36\41\uffff\1"+
            "\40\1\uffff\1\34\1\33\1\31\20\uffff\1\37\1\26",
            "\2\50\1\uffff\1\50\2\uffff\3\50\1\54\1\67\1\70\1\71\1\72\1"+
            "\73\5\50\21\uffff\2\50\3\uffff\1\50\7\uffff\1\50\30\uffff\1"+
            "\50\15\uffff\1\50",
            "\1\100\20\uffff\1\101\13\uffff\7\105\1\106\43\uffff\1\103"+
            "\1\102\22\uffff\1\104",
            "\1\107",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA14_eot = DFA.unpackEncodedString(DFA14_eotS);
    static final short[] DFA14_eof = DFA.unpackEncodedString(DFA14_eofS);
    static final char[] DFA14_min = DFA.unpackEncodedStringToUnsignedChars(DFA14_minS);
    static final char[] DFA14_max = DFA.unpackEncodedStringToUnsignedChars(DFA14_maxS);
    static final short[] DFA14_accept = DFA.unpackEncodedString(DFA14_acceptS);
    static final short[] DFA14_special = DFA.unpackEncodedString(DFA14_specialS);
    static final short[][] DFA14_transition;

    static {
        int numStates = DFA14_transitionS.length;
        DFA14_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA14_transition[i] = DFA.unpackEncodedString(DFA14_transitionS[i]);
        }
    }

    class DFA14 extends DFA {

        public DFA14(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 14;
            this.eot = DFA14_eot;
            this.eof = DFA14_eof;
            this.min = DFA14_min;
            this.max = DFA14_max;
            this.accept = DFA14_accept;
            this.special = DFA14_special;
            this.transition = DFA14_transition;
        }
        public String getDescription() {
            return "101:1: scalarExpr returns [ScalarExpr value] : (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA14_15 = input.LA(1);

                         
                        int index14_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_15);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA14_16 = input.LA(1);

                         
                        int index14_16 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_16);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA14_19 = input.LA(1);

                         
                        int index14_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred31_wcps()) ) {s = 2;}

                        else if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_19);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA14_20 = input.LA(1);

                         
                        int index14_20 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred32_wcps()) ) {s = 121;}

                        else if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_20);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA14_21 = input.LA(1);

                         
                        int index14_21 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred32_wcps()) ) {s = 121;}

                        else if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_21);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA14_22 = input.LA(1);

                         
                        int index14_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_22);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA14_23 = input.LA(1);

                         
                        int index14_23 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_23);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA14_24 = input.LA(1);

                         
                        int index14_24 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_24);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA14_25 = input.LA(1);

                         
                        int index14_25 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_25);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA14_26 = input.LA(1);

                         
                        int index14_26 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_26);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA14_27 = input.LA(1);

                         
                        int index14_27 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_27);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA14_28 = input.LA(1);

                         
                        int index14_28 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_28);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA14_29 = input.LA(1);

                         
                        int index14_29 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_29);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA14_30 = input.LA(1);

                         
                        int index14_30 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_30);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA14_31 = input.LA(1);

                         
                        int index14_31 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_31);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA14_32 = input.LA(1);

                         
                        int index14_32 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (true) ) {s = 33;}

                         
                        input.seek(index14_32);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA14_44 = input.LA(1);

                         
                        int index14_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_44);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA14_55 = input.LA(1);

                         
                        int index14_55 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_55);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA14_56 = input.LA(1);

                         
                        int index14_56 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_56);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA14_57 = input.LA(1);

                         
                        int index14_57 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_57);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA14_58 = input.LA(1);

                         
                        int index14_58 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_58);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA14_59 = input.LA(1);

                         
                        int index14_59 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred35_wcps()) ) {s = 40;}

                         
                        input.seek(index14_59);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA14_64 = input.LA(1);

                         
                        int index14_64 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_64);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA14_65 = input.LA(1);

                         
                        int index14_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_65);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA14_66 = input.LA(1);

                         
                        int index14_66 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_66);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA14_67 = input.LA(1);

                         
                        int index14_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_67);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA14_68 = input.LA(1);

                         
                        int index14_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_68);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA14_69 = input.LA(1);

                         
                        int index14_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_69);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA14_70 = input.LA(1);

                         
                        int index14_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_70);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA14_71 = input.LA(1);

                         
                        int index14_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 17;}

                        else if ( (synpred34_wcps()) ) {s = 96;}

                         
                        input.seek(index14_71);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 14, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA34_eotS =
        "\u008b\uffff";
    static final String DFA34_eofS =
        "\u008b\uffff";
    static final String DFA34_minS =
        "\1\6\12\uffff\1\6\1\uffff\1\6\4\uffff\1\6\7\uffff\1\70\1\116\64"+
        "\uffff\3\0\1\uffff\4\0\13\uffff\1\0\7\uffff\11\0\24\uffff\2\0\1"+
        "\uffff";
    static final String DFA34_maxS =
        "\1\154\12\uffff\1\154\1\uffff\1\154\4\uffff\1\154\7\uffff\1\70"+
        "\1\153\64\uffff\3\0\1\uffff\4\0\13\uffff\1\0\7\uffff\11\0\24\uffff"+
        "\2\0\1\uffff";
    static final String DFA34_acceptS =
        "\1\uffff\1\1\32\uffff\1\2\1\uffff\1\3\1\4\1\5\22\uffff\1\6\126"+
        "\uffff\1\7";
    static final String DFA34_specialS =
        "\120\uffff\1\0\1\1\1\2\1\uffff\1\3\1\4\1\5\1\6\13\uffff\1\7\7\uffff"+
        "\1\10\1\11\1\12\1\13\1\14\1\15\1\16\1\17\1\20\24\uffff\1\21\1\22"+
        "\1\uffff}>";
    static final String[] DFA34_transitionS = {
            "\1\13\17\uffff\1\34\1\15\3\uffff\20\1\3\uffff\1\1\4\uffff\5"+
            "\1\1\33\1\uffff\1\32\1\uffff\4\34\3\36\11\37\1\22\1\40\3\1\20"+
            "\uffff\2\1\10\uffff\2\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\1\17\uffff\2\1\3\uffff\20\1\3\uffff\1\1\4\uffff\6\1\1\uffff"+
            "\30\1\2\uffff\3\1\11\63\1\uffff\2\1\10\uffff\2\1",
            "",
            "\1\122\20\uffff\1\124\3\uffff\10\34\7\120\1\121\3\uffff\1"+
            "\34\4\uffff\6\34\1\uffff\1\34\21\uffff\1\34\1\uffff\1\126\1"+
            "\125\1\34\20\uffff\1\34\1\127\10\uffff\2\34",
            "",
            "",
            "",
            "",
            "\1\155\17\uffff\1\40\1\157\3\uffff\1\143\7\40\7\153\1\154"+
            "\3\uffff\1\40\4\uffff\6\40\1\uffff\24\40\1\161\1\160\1\156\1"+
            "\40\2\uffff\3\40\12\uffff\1\163\1\162\10\uffff\2\40",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0088",
            "\1\u0089\1\uffff\1\u0089\32\uffff\1\u0089",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            ""
    };

    static final short[] DFA34_eot = DFA.unpackEncodedString(DFA34_eotS);
    static final short[] DFA34_eof = DFA.unpackEncodedString(DFA34_eofS);
    static final char[] DFA34_min = DFA.unpackEncodedStringToUnsignedChars(DFA34_minS);
    static final char[] DFA34_max = DFA.unpackEncodedStringToUnsignedChars(DFA34_maxS);
    static final short[] DFA34_accept = DFA.unpackEncodedString(DFA34_acceptS);
    static final short[] DFA34_special = DFA.unpackEncodedString(DFA34_specialS);
    static final short[][] DFA34_transition;

    static {
        int numStates = DFA34_transitionS.length;
        DFA34_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA34_transition[i] = DFA.unpackEncodedString(DFA34_transitionS[i]);
        }
    }

    class DFA34 extends DFA {

        public DFA34(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 34;
            this.eot = DFA34_eot;
            this.eof = DFA34_eof;
            this.min = DFA34_min;
            this.max = DFA34_max;
            this.accept = DFA34_accept;
            this.special = DFA34_special;
            this.transition = DFA34_transition;
        }
        public String getDescription() {
            return "199:1: unaryInducedExpr returns [CoverageExpr value] : (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA34_80 = input.LA(1);

                         
                        int index34_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_80);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA34_81 = input.LA(1);

                         
                        int index34_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_81);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA34_82 = input.LA(1);

                         
                        int index34_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_82);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA34_84 = input.LA(1);

                         
                        int index34_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_84);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA34_85 = input.LA(1);

                         
                        int index34_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_85);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA34_86 = input.LA(1);

                         
                        int index34_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_86);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA34_87 = input.LA(1);

                         
                        int index34_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 28;}

                         
                        input.seek(index34_87);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA34_99 = input.LA(1);

                         
                        int index34_99 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_99);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA34_107 = input.LA(1);

                         
                        int index34_107 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_107);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA34_108 = input.LA(1);

                         
                        int index34_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_108);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA34_109 = input.LA(1);

                         
                        int index34_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_109);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA34_110 = input.LA(1);

                         
                        int index34_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_110);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA34_111 = input.LA(1);

                         
                        int index34_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_111);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA34_112 = input.LA(1);

                         
                        int index34_112 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_112);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA34_113 = input.LA(1);

                         
                        int index34_113 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_113);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA34_114 = input.LA(1);

                         
                        int index34_114 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_114);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA34_115 = input.LA(1);

                         
                        int index34_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 32;}

                         
                        input.seek(index34_115);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA34_136 = input.LA(1);

                         
                        int index34_136 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 138;}

                         
                        input.seek(index34_136);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA34_137 = input.LA(1);

                         
                        int index34_137 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 138;}

                         
                        input.seek(index34_137);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 34, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA42_eotS =
        "\u008a\uffff";
    static final String DFA42_eofS =
        "\u008a\uffff";
    static final String DFA42_minS =
        "\12\6\1\15\1\6\1\20\2\6\2\20\1\15\1\6\1\122\1\116\5\6\1\70\1\116"+
        "\3\uffff\153\0";
    static final String DFA42_maxS =
        "\1\154\11\6\1\51\1\154\1\122\1\142\1\6\3\122\1\142\1\122\1\153"+
        "\5\6\1\70\1\153\3\uffff\153\0";
    static final String DFA42_acceptS =
        "\34\uffff\1\1\1\2\1\3\153\uffff";
    static final String DFA42_specialS =
        "\37\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\1"+
        "\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\1\25\1\26\1\27\1\30"+
        "\1\31\1\32\1\33\1\34\1\35\1\36\1\37\1\40\1\41\1\42\1\43\1\44\1\45"+
        "\1\46\1\47\1\50\1\51\1\52\1\53\1\54\1\55\1\56\1\57\1\60\1\61\1\62"+
        "\1\63\1\64\1\65\1\66\1\67\1\70\1\71\1\72\1\73\1\74\1\75\1\76\1\77"+
        "\1\100\1\101\1\102\1\103\1\104\1\105\1\106\1\107\1\110\1\111\1\112"+
        "\1\113\1\114\1\115\1\116\1\117\1\120\1\121\1\122\1\123\1\124\1\125"+
        "\1\126\1\127\1\130\1\131\1\132\1\133\1\134\1\135\1\136\1\137\1\140"+
        "\1\141\1\142\1\143\1\144\1\145\1\146\1\147\1\150\1\151\1\152}>";
    static final String[] DFA42_transitionS = {
            "\1\13\20\uffff\1\15\3\uffff\1\1\1\2\1\3\1\4\1\6\1\7\1\10\1"+
            "\5\7\11\1\12\3\uffff\1\24\4\uffff\1\25\1\26\1\27\1\30\1\31\1"+
            "\33\1\uffff\1\32\21\uffff\1\22\1\uffff\1\17\1\16\1\14\3\uffff"+
            "\1\34\1\35\1\36\12\uffff\1\21\1\20\10\uffff\2\23",
            "\1\37",
            "\1\40",
            "\1\41",
            "\1\42",
            "\1\43",
            "\1\44",
            "\1\45",
            "\1\46",
            "\1\47",
            "\1\50\1\uffff\1\50\6\uffff\1\50\1\uffff\1\50\17\uffff\2\50",
            "\1\52\17\uffff\1\107\1\53\3\uffff\1\60\1\61\1\62\1\63\1\65"+
            "\1\66\1\67\1\64\7\56\1\57\3\uffff\1\74\4\uffff\1\75\1\76\1\77"+
            "\1\100\1\101\1\103\1\uffff\1\102\1\115\4\110\3\111\11\112\1"+
            "\72\1\113\1\55\1\54\1\70\1\114\2\uffff\1\104\1\105\1\106\12"+
            "\uffff\1\71\1\51\10\uffff\2\73",
            "\1\117\1\120\1\121\1\122\1\123\1\124\74\uffff\1\116",
            "\1\125\20\uffff\1\126\13\uffff\7\132\1\133\43\uffff\1\130"+
            "\1\127\22\uffff\1\131",
            "\1\134",
            "\1\141\1\142\1\143\1\144\1\145\1\146\1\137\1\140\1\135\1\136"+
            "\70\uffff\1\147",
            "\1\155\1\156\1\157\1\160\1\161\1\162\1\152\1\153\1\150\1\151"+
            "\70\uffff\1\154",
            "\1\164\1\165\1\163\102\uffff\1\166",
            "\1\167\20\uffff\1\172\3\uffff\1\170\7\uffff\7\176\1\177\43"+
            "\uffff\1\174\1\173\1\171\20\uffff\1\u0080\1\175",
            "\1\u0081",
            "\1\u0082\1\uffff\1\u0082\32\uffff\1\u0082",
            "\1\u0083",
            "\1\u0084",
            "\1\u0085",
            "\1\u0086",
            "\1\u0087",
            "\1\u0088",
            "\1\u0089\1\uffff\1\u0089\32\uffff\1\u0089",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff"
    };

    static final short[] DFA42_eot = DFA.unpackEncodedString(DFA42_eotS);
    static final short[] DFA42_eof = DFA.unpackEncodedString(DFA42_eofS);
    static final char[] DFA42_min = DFA.unpackEncodedStringToUnsignedChars(DFA42_minS);
    static final char[] DFA42_max = DFA.unpackEncodedStringToUnsignedChars(DFA42_maxS);
    static final short[] DFA42_accept = DFA.unpackEncodedString(DFA42_acceptS);
    static final short[] DFA42_special = DFA.unpackEncodedString(DFA42_specialS);
    static final short[][] DFA42_transition;

    static {
        int numStates = DFA42_transitionS.length;
        DFA42_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA42_transition[i] = DFA.unpackEncodedString(DFA42_transitionS[i]);
        }
    }

    class DFA42 extends DFA {

        public DFA42(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 42;
            this.eot = DFA42_eot;
            this.eof = DFA42_eof;
            this.min = DFA42_min;
            this.max = DFA42_max;
            this.accept = DFA42_accept;
            this.special = DFA42_special;
            this.transition = DFA42_transition;
        }
        public String getDescription() {
            return "246:1: subsetExpr returns [SubsetExpr value] : (e1= trimExpr | e2= sliceExpr | e3= extendExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA42_31 = input.LA(1);

                         
                        int index42_31 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_31);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA42_32 = input.LA(1);

                         
                        int index42_32 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_32);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA42_33 = input.LA(1);

                         
                        int index42_33 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_33);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA42_34 = input.LA(1);

                         
                        int index42_34 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_34);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA42_35 = input.LA(1);

                         
                        int index42_35 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_35);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA42_36 = input.LA(1);

                         
                        int index42_36 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_36);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA42_37 = input.LA(1);

                         
                        int index42_37 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_37);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA42_38 = input.LA(1);

                         
                        int index42_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_38);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA42_39 = input.LA(1);

                         
                        int index42_39 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_39);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA42_40 = input.LA(1);

                         
                        int index42_40 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_40);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA42_41 = input.LA(1);

                         
                        int index42_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_41);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA42_42 = input.LA(1);

                         
                        int index42_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_42);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA42_43 = input.LA(1);

                         
                        int index42_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_43);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA42_44 = input.LA(1);

                         
                        int index42_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_44);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA42_45 = input.LA(1);

                         
                        int index42_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_45);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA42_46 = input.LA(1);

                         
                        int index42_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_46);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA42_47 = input.LA(1);

                         
                        int index42_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_47);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA42_48 = input.LA(1);

                         
                        int index42_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_48);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA42_49 = input.LA(1);

                         
                        int index42_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_49);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA42_50 = input.LA(1);

                         
                        int index42_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_50);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA42_51 = input.LA(1);

                         
                        int index42_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_51);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA42_52 = input.LA(1);

                         
                        int index42_52 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_52);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA42_53 = input.LA(1);

                         
                        int index42_53 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_53);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA42_54 = input.LA(1);

                         
                        int index42_54 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_54);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA42_55 = input.LA(1);

                         
                        int index42_55 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_55);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA42_56 = input.LA(1);

                         
                        int index42_56 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_56);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA42_57 = input.LA(1);

                         
                        int index42_57 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_57);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA42_58 = input.LA(1);

                         
                        int index42_58 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_58);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA42_59 = input.LA(1);

                         
                        int index42_59 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_59);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA42_60 = input.LA(1);

                         
                        int index42_60 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_60);
                        if ( s>=0 ) return s;
                        break;
                    case 30 : 
                        int LA42_61 = input.LA(1);

                         
                        int index42_61 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_61);
                        if ( s>=0 ) return s;
                        break;
                    case 31 : 
                        int LA42_62 = input.LA(1);

                         
                        int index42_62 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_62);
                        if ( s>=0 ) return s;
                        break;
                    case 32 : 
                        int LA42_63 = input.LA(1);

                         
                        int index42_63 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_63);
                        if ( s>=0 ) return s;
                        break;
                    case 33 : 
                        int LA42_64 = input.LA(1);

                         
                        int index42_64 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_64);
                        if ( s>=0 ) return s;
                        break;
                    case 34 : 
                        int LA42_65 = input.LA(1);

                         
                        int index42_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_65);
                        if ( s>=0 ) return s;
                        break;
                    case 35 : 
                        int LA42_66 = input.LA(1);

                         
                        int index42_66 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_66);
                        if ( s>=0 ) return s;
                        break;
                    case 36 : 
                        int LA42_67 = input.LA(1);

                         
                        int index42_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_67);
                        if ( s>=0 ) return s;
                        break;
                    case 37 : 
                        int LA42_68 = input.LA(1);

                         
                        int index42_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_68);
                        if ( s>=0 ) return s;
                        break;
                    case 38 : 
                        int LA42_69 = input.LA(1);

                         
                        int index42_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_69);
                        if ( s>=0 ) return s;
                        break;
                    case 39 : 
                        int LA42_70 = input.LA(1);

                         
                        int index42_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_70);
                        if ( s>=0 ) return s;
                        break;
                    case 40 : 
                        int LA42_71 = input.LA(1);

                         
                        int index42_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_71);
                        if ( s>=0 ) return s;
                        break;
                    case 41 : 
                        int LA42_72 = input.LA(1);

                         
                        int index42_72 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_72);
                        if ( s>=0 ) return s;
                        break;
                    case 42 : 
                        int LA42_73 = input.LA(1);

                         
                        int index42_73 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_73);
                        if ( s>=0 ) return s;
                        break;
                    case 43 : 
                        int LA42_74 = input.LA(1);

                         
                        int index42_74 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_74);
                        if ( s>=0 ) return s;
                        break;
                    case 44 : 
                        int LA42_75 = input.LA(1);

                         
                        int index42_75 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_75);
                        if ( s>=0 ) return s;
                        break;
                    case 45 : 
                        int LA42_76 = input.LA(1);

                         
                        int index42_76 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_76);
                        if ( s>=0 ) return s;
                        break;
                    case 46 : 
                        int LA42_77 = input.LA(1);

                         
                        int index42_77 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_77);
                        if ( s>=0 ) return s;
                        break;
                    case 47 : 
                        int LA42_78 = input.LA(1);

                         
                        int index42_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_78);
                        if ( s>=0 ) return s;
                        break;
                    case 48 : 
                        int LA42_79 = input.LA(1);

                         
                        int index42_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_79);
                        if ( s>=0 ) return s;
                        break;
                    case 49 : 
                        int LA42_80 = input.LA(1);

                         
                        int index42_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_80);
                        if ( s>=0 ) return s;
                        break;
                    case 50 : 
                        int LA42_81 = input.LA(1);

                         
                        int index42_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_81);
                        if ( s>=0 ) return s;
                        break;
                    case 51 : 
                        int LA42_82 = input.LA(1);

                         
                        int index42_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_82);
                        if ( s>=0 ) return s;
                        break;
                    case 52 : 
                        int LA42_83 = input.LA(1);

                         
                        int index42_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_83);
                        if ( s>=0 ) return s;
                        break;
                    case 53 : 
                        int LA42_84 = input.LA(1);

                         
                        int index42_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_84);
                        if ( s>=0 ) return s;
                        break;
                    case 54 : 
                        int LA42_85 = input.LA(1);

                         
                        int index42_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_85);
                        if ( s>=0 ) return s;
                        break;
                    case 55 : 
                        int LA42_86 = input.LA(1);

                         
                        int index42_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_86);
                        if ( s>=0 ) return s;
                        break;
                    case 56 : 
                        int LA42_87 = input.LA(1);

                         
                        int index42_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_87);
                        if ( s>=0 ) return s;
                        break;
                    case 57 : 
                        int LA42_88 = input.LA(1);

                         
                        int index42_88 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_88);
                        if ( s>=0 ) return s;
                        break;
                    case 58 : 
                        int LA42_89 = input.LA(1);

                         
                        int index42_89 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_89);
                        if ( s>=0 ) return s;
                        break;
                    case 59 : 
                        int LA42_90 = input.LA(1);

                         
                        int index42_90 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_90);
                        if ( s>=0 ) return s;
                        break;
                    case 60 : 
                        int LA42_91 = input.LA(1);

                         
                        int index42_91 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_91);
                        if ( s>=0 ) return s;
                        break;
                    case 61 : 
                        int LA42_92 = input.LA(1);

                         
                        int index42_92 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_92);
                        if ( s>=0 ) return s;
                        break;
                    case 62 : 
                        int LA42_93 = input.LA(1);

                         
                        int index42_93 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_93);
                        if ( s>=0 ) return s;
                        break;
                    case 63 : 
                        int LA42_94 = input.LA(1);

                         
                        int index42_94 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_94);
                        if ( s>=0 ) return s;
                        break;
                    case 64 : 
                        int LA42_95 = input.LA(1);

                         
                        int index42_95 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_95);
                        if ( s>=0 ) return s;
                        break;
                    case 65 : 
                        int LA42_96 = input.LA(1);

                         
                        int index42_96 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_96);
                        if ( s>=0 ) return s;
                        break;
                    case 66 : 
                        int LA42_97 = input.LA(1);

                         
                        int index42_97 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_97);
                        if ( s>=0 ) return s;
                        break;
                    case 67 : 
                        int LA42_98 = input.LA(1);

                         
                        int index42_98 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_98);
                        if ( s>=0 ) return s;
                        break;
                    case 68 : 
                        int LA42_99 = input.LA(1);

                         
                        int index42_99 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_99);
                        if ( s>=0 ) return s;
                        break;
                    case 69 : 
                        int LA42_100 = input.LA(1);

                         
                        int index42_100 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_100);
                        if ( s>=0 ) return s;
                        break;
                    case 70 : 
                        int LA42_101 = input.LA(1);

                         
                        int index42_101 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_101);
                        if ( s>=0 ) return s;
                        break;
                    case 71 : 
                        int LA42_102 = input.LA(1);

                         
                        int index42_102 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_102);
                        if ( s>=0 ) return s;
                        break;
                    case 72 : 
                        int LA42_103 = input.LA(1);

                         
                        int index42_103 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_103);
                        if ( s>=0 ) return s;
                        break;
                    case 73 : 
                        int LA42_104 = input.LA(1);

                         
                        int index42_104 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_104);
                        if ( s>=0 ) return s;
                        break;
                    case 74 : 
                        int LA42_105 = input.LA(1);

                         
                        int index42_105 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_105);
                        if ( s>=0 ) return s;
                        break;
                    case 75 : 
                        int LA42_106 = input.LA(1);

                         
                        int index42_106 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_106);
                        if ( s>=0 ) return s;
                        break;
                    case 76 : 
                        int LA42_107 = input.LA(1);

                         
                        int index42_107 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_107);
                        if ( s>=0 ) return s;
                        break;
                    case 77 : 
                        int LA42_108 = input.LA(1);

                         
                        int index42_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_108);
                        if ( s>=0 ) return s;
                        break;
                    case 78 : 
                        int LA42_109 = input.LA(1);

                         
                        int index42_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_109);
                        if ( s>=0 ) return s;
                        break;
                    case 79 : 
                        int LA42_110 = input.LA(1);

                         
                        int index42_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_110);
                        if ( s>=0 ) return s;
                        break;
                    case 80 : 
                        int LA42_111 = input.LA(1);

                         
                        int index42_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_111);
                        if ( s>=0 ) return s;
                        break;
                    case 81 : 
                        int LA42_112 = input.LA(1);

                         
                        int index42_112 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_112);
                        if ( s>=0 ) return s;
                        break;
                    case 82 : 
                        int LA42_113 = input.LA(1);

                         
                        int index42_113 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_113);
                        if ( s>=0 ) return s;
                        break;
                    case 83 : 
                        int LA42_114 = input.LA(1);

                         
                        int index42_114 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_114);
                        if ( s>=0 ) return s;
                        break;
                    case 84 : 
                        int LA42_115 = input.LA(1);

                         
                        int index42_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_115);
                        if ( s>=0 ) return s;
                        break;
                    case 85 : 
                        int LA42_116 = input.LA(1);

                         
                        int index42_116 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_116);
                        if ( s>=0 ) return s;
                        break;
                    case 86 : 
                        int LA42_117 = input.LA(1);

                         
                        int index42_117 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_117);
                        if ( s>=0 ) return s;
                        break;
                    case 87 : 
                        int LA42_118 = input.LA(1);

                         
                        int index42_118 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_118);
                        if ( s>=0 ) return s;
                        break;
                    case 88 : 
                        int LA42_119 = input.LA(1);

                         
                        int index42_119 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_119);
                        if ( s>=0 ) return s;
                        break;
                    case 89 : 
                        int LA42_120 = input.LA(1);

                         
                        int index42_120 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_120);
                        if ( s>=0 ) return s;
                        break;
                    case 90 : 
                        int LA42_121 = input.LA(1);

                         
                        int index42_121 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_121);
                        if ( s>=0 ) return s;
                        break;
                    case 91 : 
                        int LA42_122 = input.LA(1);

                         
                        int index42_122 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_122);
                        if ( s>=0 ) return s;
                        break;
                    case 92 : 
                        int LA42_123 = input.LA(1);

                         
                        int index42_123 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_123);
                        if ( s>=0 ) return s;
                        break;
                    case 93 : 
                        int LA42_124 = input.LA(1);

                         
                        int index42_124 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_124);
                        if ( s>=0 ) return s;
                        break;
                    case 94 : 
                        int LA42_125 = input.LA(1);

                         
                        int index42_125 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_125);
                        if ( s>=0 ) return s;
                        break;
                    case 95 : 
                        int LA42_126 = input.LA(1);

                         
                        int index42_126 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_126);
                        if ( s>=0 ) return s;
                        break;
                    case 96 : 
                        int LA42_127 = input.LA(1);

                         
                        int index42_127 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_127);
                        if ( s>=0 ) return s;
                        break;
                    case 97 : 
                        int LA42_128 = input.LA(1);

                         
                        int index42_128 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_128);
                        if ( s>=0 ) return s;
                        break;
                    case 98 : 
                        int LA42_129 = input.LA(1);

                         
                        int index42_129 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_129);
                        if ( s>=0 ) return s;
                        break;
                    case 99 : 
                        int LA42_130 = input.LA(1);

                         
                        int index42_130 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_130);
                        if ( s>=0 ) return s;
                        break;
                    case 100 : 
                        int LA42_131 = input.LA(1);

                         
                        int index42_131 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_131);
                        if ( s>=0 ) return s;
                        break;
                    case 101 : 
                        int LA42_132 = input.LA(1);

                         
                        int index42_132 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_132);
                        if ( s>=0 ) return s;
                        break;
                    case 102 : 
                        int LA42_133 = input.LA(1);

                         
                        int index42_133 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_133);
                        if ( s>=0 ) return s;
                        break;
                    case 103 : 
                        int LA42_134 = input.LA(1);

                         
                        int index42_134 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_134);
                        if ( s>=0 ) return s;
                        break;
                    case 104 : 
                        int LA42_135 = input.LA(1);

                         
                        int index42_135 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_135);
                        if ( s>=0 ) return s;
                        break;
                    case 105 : 
                        int LA42_136 = input.LA(1);

                         
                        int index42_136 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_136);
                        if ( s>=0 ) return s;
                        break;
                    case 106 : 
                        int LA42_137 = input.LA(1);

                         
                        int index42_137 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 28;}

                        else if ( (synpred102_wcps()) ) {s = 29;}

                         
                        input.seek(index42_137);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 42, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA43_eotS =
        "\35\uffff";
    static final String DFA43_eofS =
        "\35\uffff";
    static final String DFA43_minS =
        "\1\6\34\uffff";
    static final String DFA43_maxS =
        "\1\154\34\uffff";
    static final String DFA43_acceptS =
        "\1\uffff\1\1\32\uffff\1\2";
    static final String DFA43_specialS =
        "\35\uffff}>";
    static final String[] DFA43_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\20\1\3\uffff\1\1\4\uffff\6\1\1\uffff"+
            "\1\1\21\uffff\1\1\1\uffff\3\1\3\uffff\1\34\14\uffff\2\1\10\uffff"+
            "\2\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA43_eot = DFA.unpackEncodedString(DFA43_eotS);
    static final short[] DFA43_eof = DFA.unpackEncodedString(DFA43_eofS);
    static final char[] DFA43_min = DFA.unpackEncodedStringToUnsignedChars(DFA43_minS);
    static final char[] DFA43_max = DFA.unpackEncodedStringToUnsignedChars(DFA43_maxS);
    static final short[] DFA43_accept = DFA.unpackEncodedString(DFA43_acceptS);
    static final short[] DFA43_special = DFA.unpackEncodedString(DFA43_specialS);
    static final short[][] DFA43_transition;

    static {
        int numStates = DFA43_transitionS.length;
        DFA43_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA43_transition[i] = DFA.unpackEncodedString(DFA43_transitionS[i]);
        }
    }

    class DFA43 extends DFA {

        public DFA43(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 43;
            this.eot = DFA43_eot;
            this.eof = DFA43_eof;
            this.min = DFA43_min;
            this.max = DFA43_max;
            this.accept = DFA43_accept;
            this.special = DFA43_special;
            this.transition = DFA43_transition;
        }
        public String getDescription() {
            return "251:1: trimExpr returns [TrimExpr value] : (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN );";
        }
    }
    static final String DFA44_eotS =
        "\35\uffff";
    static final String DFA44_eofS =
        "\35\uffff";
    static final String DFA44_minS =
        "\1\6\34\uffff";
    static final String DFA44_maxS =
        "\1\154\34\uffff";
    static final String DFA44_acceptS =
        "\1\uffff\1\1\32\uffff\1\2";
    static final String DFA44_specialS =
        "\35\uffff}>";
    static final String[] DFA44_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\20\1\3\uffff\1\1\4\uffff\6\1\1\uffff"+
            "\1\1\21\uffff\1\1\1\uffff\3\1\4\uffff\1\34\13\uffff\2\1\10\uffff"+
            "\2\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA44_eot = DFA.unpackEncodedString(DFA44_eotS);
    static final short[] DFA44_eof = DFA.unpackEncodedString(DFA44_eofS);
    static final char[] DFA44_min = DFA.unpackEncodedStringToUnsignedChars(DFA44_minS);
    static final char[] DFA44_max = DFA.unpackEncodedStringToUnsignedChars(DFA44_maxS);
    static final short[] DFA44_accept = DFA.unpackEncodedString(DFA44_acceptS);
    static final short[] DFA44_special = DFA.unpackEncodedString(DFA44_specialS);
    static final short[][] DFA44_transition;

    static {
        int numStates = DFA44_transitionS.length;
        DFA44_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA44_transition[i] = DFA.unpackEncodedString(DFA44_transitionS[i]);
        }
    }

    class DFA44 extends DFA {

        public DFA44(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 44;
            this.eot = DFA44_eot;
            this.eof = DFA44_eof;
            this.min = DFA44_min;
            this.max = DFA44_max;
            this.accept = DFA44_accept;
            this.special = DFA44_special;
            this.transition = DFA44_transition;
        }
        public String getDescription() {
            return "255:1: sliceExpr returns [SliceExpr value] : (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN );";
        }
    }
    static final String DFA47_eotS =
        "\144\uffff";
    static final String DFA47_eofS =
        "\1\1\143\uffff";
    static final String DFA47_minS =
        "\1\7\7\uffff\1\0\15\uffff\1\0\115\uffff";
    static final String DFA47_maxS =
        "\1\140\7\uffff\1\0\15\uffff\1\0\115\uffff";
    static final String DFA47_acceptS =
        "\1\uffff\1\2\74\uffff\1\1\45\uffff";
    static final String DFA47_specialS =
        "\10\uffff\1\0\15\uffff\1\1\115\uffff}>";
    static final String[] DFA47_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\1\10\1\26\14\1\21\uffff\2\1\3\uffff"+
            "\1\1\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA47_eot = DFA.unpackEncodedString(DFA47_eotS);
    static final short[] DFA47_eof = DFA.unpackEncodedString(DFA47_eofS);
    static final char[] DFA47_min = DFA.unpackEncodedStringToUnsignedChars(DFA47_minS);
    static final char[] DFA47_max = DFA.unpackEncodedStringToUnsignedChars(DFA47_maxS);
    static final short[] DFA47_accept = DFA.unpackEncodedString(DFA47_acceptS);
    static final short[] DFA47_special = DFA.unpackEncodedString(DFA47_specialS);
    static final short[][] DFA47_transition;

    static {
        int numStates = DFA47_transitionS.length;
        DFA47_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA47_transition[i] = DFA.unpackEncodedString(DFA47_transitionS[i]);
        }
    }

    class DFA47 extends DFA {

        public DFA47(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 47;
            this.eot = DFA47_eot;
            this.eof = DFA47_eof;
            this.min = DFA47_min;
            this.max = DFA47_max;
            this.accept = DFA47_accept;
            this.special = DFA47_special;
            this.transition = DFA47_transition;
        }
        public String getDescription() {
            return "()* loopback of 276:7: (op= ( OR | XOR ) e2= booleanScalarTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA47_8 = input.LA(1);

                         
                        int index47_8 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred116_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index47_8);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA47_22 = input.LA(1);

                         
                        int index47_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred116_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index47_22);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 47, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA48_eotS =
        "\77\uffff";
    static final String DFA48_eofS =
        "\1\1\76\uffff";
    static final String DFA48_minS =
        "\1\7\10\uffff\1\0\65\uffff";
    static final String DFA48_maxS =
        "\1\140\10\uffff\1\0\65\uffff";
    static final String DFA48_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA48_specialS =
        "\11\uffff\1\0\65\uffff}>";
    static final String[] DFA48_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\2\1\1\11\13\1\21\uffff\2\1\3\uffff"+
            "\1\1\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA48_eot = DFA.unpackEncodedString(DFA48_eotS);
    static final short[] DFA48_eof = DFA.unpackEncodedString(DFA48_eofS);
    static final char[] DFA48_min = DFA.unpackEncodedStringToUnsignedChars(DFA48_minS);
    static final char[] DFA48_max = DFA.unpackEncodedStringToUnsignedChars(DFA48_maxS);
    static final short[] DFA48_accept = DFA.unpackEncodedString(DFA48_acceptS);
    static final short[] DFA48_special = DFA.unpackEncodedString(DFA48_specialS);
    static final short[][] DFA48_transition;

    static {
        int numStates = DFA48_transitionS.length;
        DFA48_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA48_transition[i] = DFA.unpackEncodedString(DFA48_transitionS[i]);
        }
    }

    class DFA48 extends DFA {

        public DFA48(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 48;
            this.eot = DFA48_eot;
            this.eof = DFA48_eof;
            this.min = DFA48_min;
            this.max = DFA48_max;
            this.accept = DFA48_accept;
            this.special = DFA48_special;
            this.transition = DFA48_transition;
        }
        public String getDescription() {
            return "()* loopback of 280:4: (op= AND e2= booleanScalarNegation )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA48_9 = input.LA(1);

                         
                        int index48_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index48_9);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 48, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA49_eotS =
        "\14\uffff";
    static final String DFA49_eofS =
        "\14\uffff";
    static final String DFA49_minS =
        "\1\6\13\uffff";
    static final String DFA49_maxS =
        "\1\142\13\uffff";
    static final String DFA49_acceptS =
        "\1\uffff\1\1\11\uffff\1\2";
    static final String DFA49_specialS =
        "\14\uffff}>";
    static final String[] DFA49_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\1\1\7\uffff\10\1\41\uffff\1\13\1"+
            "\uffff\3\1\20\uffff\2\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA49_eot = DFA.unpackEncodedString(DFA49_eotS);
    static final short[] DFA49_eof = DFA.unpackEncodedString(DFA49_eofS);
    static final char[] DFA49_min = DFA.unpackEncodedStringToUnsignedChars(DFA49_minS);
    static final char[] DFA49_max = DFA.unpackEncodedStringToUnsignedChars(DFA49_maxS);
    static final short[] DFA49_accept = DFA.unpackEncodedString(DFA49_acceptS);
    static final short[] DFA49_special = DFA.unpackEncodedString(DFA49_specialS);
    static final short[][] DFA49_transition;

    static {
        int numStates = DFA49_transitionS.length;
        DFA49_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA49_transition[i] = DFA.unpackEncodedString(DFA49_transitionS[i]);
        }
    }

    class DFA49 extends DFA {

        public DFA49(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 49;
            this.eot = DFA49_eot;
            this.eof = DFA49_eof;
            this.min = DFA49_min;
            this.max = DFA49_max;
            this.accept = DFA49_accept;
            this.special = DFA49_special;
            this.transition = DFA49_transition;
        }
        public String getDescription() {
            return "283:1: booleanScalarNegation returns [BooleanScalarExpr value] : (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom );";
        }
    }
    static final String DFA50_eotS =
        "\26\uffff";
    static final String DFA50_eofS =
        "\26\uffff";
    static final String DFA50_minS =
        "\2\6\11\uffff\2\0\2\uffff\5\0\2\uffff";
    static final String DFA50_maxS =
        "\2\142\11\uffff\2\0\2\uffff\5\0\2\uffff";
    static final String DFA50_acceptS =
        "\2\uffff\1\2\1\uffff\1\3\5\uffff\1\4\2\uffff\1\1\10\uffff";
    static final String DFA50_specialS =
        "\13\uffff\1\0\1\1\2\uffff\1\2\1\3\1\4\1\5\1\6\2\uffff}>";
    static final String[] DFA50_transitionS = {
            "\1\1\20\uffff\1\4\3\uffff\1\2\7\uffff\10\4\43\uffff\2\4\1\2"+
            "\20\uffff\1\12\1\4",
            "\1\14\20\uffff\1\17\3\uffff\1\15\7\uffff\7\22\1\23\41\uffff"+
            "\1\15\1\uffff\1\21\1\20\1\15\20\uffff\1\15\1\13",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "",
            "",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "",
            ""
    };

    static final short[] DFA50_eot = DFA.unpackEncodedString(DFA50_eotS);
    static final short[] DFA50_eof = DFA.unpackEncodedString(DFA50_eofS);
    static final char[] DFA50_min = DFA.unpackEncodedStringToUnsignedChars(DFA50_minS);
    static final char[] DFA50_max = DFA.unpackEncodedStringToUnsignedChars(DFA50_maxS);
    static final short[] DFA50_accept = DFA.unpackEncodedString(DFA50_acceptS);
    static final short[] DFA50_special = DFA.unpackEncodedString(DFA50_specialS);
    static final short[][] DFA50_transition;

    static {
        int numStates = DFA50_transitionS.length;
        DFA50_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA50_transition[i] = DFA.unpackEncodedString(DFA50_transitionS[i]);
        }
    }

    class DFA50 extends DFA {

        public DFA50(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 50;
            this.eot = DFA50_eot;
            this.eof = DFA50_eof;
            this.min = DFA50_min;
            this.max = DFA50_max;
            this.accept = DFA50_accept;
            this.special = DFA50_special;
            this.transition = DFA50_transition;
        }
        public String getDescription() {
            return "288:1: booleanScalarAtom returns [BooleanScalarExpr value] : ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA50_11 = input.LA(1);

                         
                        int index50_11 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_11);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA50_12 = input.LA(1);

                         
                        int index50_12 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_12);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA50_15 = input.LA(1);

                         
                        int index50_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_15);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA50_16 = input.LA(1);

                         
                        int index50_16 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_16);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA50_17 = input.LA(1);

                         
                        int index50_17 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_17);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA50_18 = input.LA(1);

                         
                        int index50_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_18);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA50_19 = input.LA(1);

                         
                        int index50_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 13;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_19);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 50, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA52_eotS =
        "\144\uffff";
    static final String DFA52_eofS =
        "\1\1\143\uffff";
    static final String DFA52_minS =
        "\1\7\3\uffff\1\0\12\uffff\1\0\124\uffff";
    static final String DFA52_maxS =
        "\1\140\3\uffff\1\0\12\uffff\1\0\124\uffff";
    static final String DFA52_acceptS =
        "\1\uffff\1\2\74\uffff\1\1\45\uffff";
    static final String DFA52_specialS =
        "\4\uffff\1\0\12\uffff\1\1\124\uffff}>";
    static final String[] DFA52_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\11\1\1\4\1\17\3\1\21\uffff\2\1\3\uffff"+
            "\1\1\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA52_eot = DFA.unpackEncodedString(DFA52_eotS);
    static final short[] DFA52_eof = DFA.unpackEncodedString(DFA52_eofS);
    static final char[] DFA52_min = DFA.unpackEncodedStringToUnsignedChars(DFA52_minS);
    static final char[] DFA52_max = DFA.unpackEncodedStringToUnsignedChars(DFA52_maxS);
    static final short[] DFA52_accept = DFA.unpackEncodedString(DFA52_acceptS);
    static final short[] DFA52_special = DFA.unpackEncodedString(DFA52_specialS);
    static final short[][] DFA52_transition;

    static {
        int numStates = DFA52_transitionS.length;
        DFA52_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA52_transition[i] = DFA.unpackEncodedString(DFA52_transitionS[i]);
        }
    }

    class DFA52 extends DFA {

        public DFA52(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 52;
            this.eot = DFA52_eot;
            this.eof = DFA52_eof;
            this.min = DFA52_min;
            this.max = DFA52_max;
            this.accept = DFA52_accept;
            this.special = DFA52_special;
            this.transition = DFA52_transition;
        }
        public String getDescription() {
            return "()* loopback of 297:4: (op= ( PLUS | MINUS ) e2= numericScalarTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA52_4 = input.LA(1);

                         
                        int index52_4 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred123_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index52_4);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA52_15 = input.LA(1);

                         
                        int index52_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred123_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index52_15);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 52, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA54_eotS =
        "\144\uffff";
    static final String DFA54_eofS =
        "\1\1\143\uffff";
    static final String DFA54_minS =
        "\1\7\4\uffff\1\0\11\uffff\1\0\124\uffff";
    static final String DFA54_maxS =
        "\1\140\4\uffff\1\0\11\uffff\1\0\124\uffff";
    static final String DFA54_acceptS =
        "\1\uffff\1\2\74\uffff\1\1\45\uffff";
    static final String DFA54_specialS =
        "\5\uffff\1\0\11\uffff\1\1\124\uffff}>";
    static final String[] DFA54_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\13\1\1\5\1\17\1\1\21\uffff\2\1\3\uffff"+
            "\1\1\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA54_eot = DFA.unpackEncodedString(DFA54_eotS);
    static final short[] DFA54_eof = DFA.unpackEncodedString(DFA54_eofS);
    static final char[] DFA54_min = DFA.unpackEncodedStringToUnsignedChars(DFA54_minS);
    static final char[] DFA54_max = DFA.unpackEncodedStringToUnsignedChars(DFA54_maxS);
    static final short[] DFA54_accept = DFA.unpackEncodedString(DFA54_acceptS);
    static final short[] DFA54_special = DFA.unpackEncodedString(DFA54_specialS);
    static final short[][] DFA54_transition;

    static {
        int numStates = DFA54_transitionS.length;
        DFA54_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA54_transition[i] = DFA.unpackEncodedString(DFA54_transitionS[i]);
        }
    }

    class DFA54 extends DFA {

        public DFA54(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 54;
            this.eot = DFA54_eot;
            this.eof = DFA54_eof;
            this.min = DFA54_min;
            this.max = DFA54_max;
            this.accept = DFA54_accept;
            this.special = DFA54_special;
            this.transition = DFA54_transition;
        }
        public String getDescription() {
            return "()* loopback of 301:3: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA54_5 = input.LA(1);

                         
                        int index54_5 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred125_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index54_5);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA54_15 = input.LA(1);

                         
                        int index54_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred125_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index54_15);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 54, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA55_eotS =
        "\20\uffff";
    static final String DFA55_eofS =
        "\20\uffff";
    static final String DFA55_minS =
        "\2\6\6\uffff\1\0\7\uffff";
    static final String DFA55_maxS =
        "\2\142\6\uffff\1\0\7\uffff";
    static final String DFA55_acceptS =
        "\2\uffff\1\2\1\3\1\4\1\5\1\7\2\uffff\1\1\5\uffff\1\6";
    static final String DFA55_specialS =
        "\10\uffff\1\0\7\uffff}>";
    static final String[] DFA55_transitionS = {
            "\1\1\20\uffff\1\2\13\uffff\10\6\43\uffff\1\4\1\3\22\uffff\1"+
            "\5",
            "\1\11\20\uffff\1\11\13\uffff\10\11\43\uffff\2\11\22\uffff"+
            "\1\10",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA55_eot = DFA.unpackEncodedString(DFA55_eotS);
    static final short[] DFA55_eof = DFA.unpackEncodedString(DFA55_eofS);
    static final char[] DFA55_min = DFA.unpackEncodedStringToUnsignedChars(DFA55_minS);
    static final char[] DFA55_max = DFA.unpackEncodedStringToUnsignedChars(DFA55_maxS);
    static final short[] DFA55_accept = DFA.unpackEncodedString(DFA55_acceptS);
    static final short[] DFA55_special = DFA.unpackEncodedString(DFA55_specialS);
    static final short[][] DFA55_transition;

    static {
        int numStates = DFA55_transitionS.length;
        DFA55_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA55_transition[i] = DFA.unpackEncodedString(DFA55_transitionS[i]);
        }
    }

    class DFA55 extends DFA {

        public DFA55(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 55;
            this.eot = DFA55_eot;
            this.eof = DFA55_eof;
            this.min = DFA55_min;
            this.max = DFA55_max;
            this.accept = DFA55_accept;
            this.special = DFA55_special;
            this.transition = DFA55_transition;
        }
        public String getDescription() {
            return "303:1: numericScalarFactor returns [NumericScalarExpr value] : ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA55_8 = input.LA(1);

                         
                        int index55_8 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred126_wcps()) ) {s = 9;}

                        else if ( (synpred131_wcps()) ) {s = 15;}

                         
                        input.seek(index55_8);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 55, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA59_eotS =
        "\25\uffff";
    static final String DFA59_eofS =
        "\25\uffff";
    static final String DFA59_minS =
        "\1\6\4\uffff\1\6\15\uffff\1\0\1\uffff";
    static final String DFA59_maxS =
        "\1\142\4\uffff\1\6\15\uffff\1\0\1\uffff";
    static final String DFA59_acceptS =
        "\1\uffff\1\1\22\uffff\1\2";
    static final String DFA59_specialS =
        "\23\uffff\1\0\1\uffff}>";
    static final String[] DFA59_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\7\1\1\5\10\1\41\uffff\1\1\1\uffff"+
            "\3\1\20\uffff\2\1",
            "",
            "",
            "",
            "",
            "\1\23",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\uffff",
            ""
    };

    static final short[] DFA59_eot = DFA.unpackEncodedString(DFA59_eotS);
    static final short[] DFA59_eof = DFA.unpackEncodedString(DFA59_eofS);
    static final char[] DFA59_min = DFA.unpackEncodedStringToUnsignedChars(DFA59_minS);
    static final char[] DFA59_max = DFA.unpackEncodedStringToUnsignedChars(DFA59_maxS);
    static final short[] DFA59_accept = DFA.unpackEncodedString(DFA59_acceptS);
    static final short[] DFA59_special = DFA.unpackEncodedString(DFA59_specialS);
    static final short[][] DFA59_transition;

    static {
        int numStates = DFA59_transitionS.length;
        DFA59_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA59_transition[i] = DFA.unpackEncodedString(DFA59_transitionS[i]);
        }
    }

    class DFA59 extends DFA {

        public DFA59(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 59;
            this.eot = DFA59_eot;
            this.eof = DFA59_eof;
            this.min = DFA59_min;
            this.max = DFA59_max;
            this.accept = DFA59_accept;
            this.special = DFA59_special;
            this.transition = DFA59_transition;
        }
        public String getDescription() {
            return "328:1: dimensionIntervalExpr returns [DimensionIntervalExpr value] : (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA59_19 = input.LA(1);

                         
                        int index59_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred139_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 20;}

                         
                        input.seek(index59_19);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 59, _s, input);
            error(nvae);
            throw nvae;
        }
    }
 

    public static final BitSet FOLLOW_forClause_in_wcpsRequest63 = new BitSet(new long[]{0x0000000000000600L});
    public static final BitSet FOLLOW_whereClause_in_wcpsRequest72 = new BitSet(new long[]{0x0000000000000600L});
    public static final BitSet FOLLOW_returnClause_in_wcpsRequest83 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FOR_in_forClause98 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_variableName_in_forClause102 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_IN_in_forClause104 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_forClause106 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageList_in_forClause110 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_forClause112 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_forClause122 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_variableName_in_forClause126 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_IN_in_forClause128 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_forClause130 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageList_in_forClause134 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_forClause136 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_WHERE_in_whereClause157 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_whereClause161 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RETURN_in_returnClause176 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_processingExpr_in_returnClause180 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageName_in_coverageList197 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_coverageList204 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_coverageList208 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_encodedCoverageExpr_in_processingExpr230 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_storeExpr_in_processingExpr242 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_processingExpr254 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ENCODE_in_encodedCoverageExpr272 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_encodedCoverageExpr274 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_encodedCoverageExpr278 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_encodedCoverageExpr280 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringConstant_in_encodedCoverageExpr284 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_COMMA_in_encodedCoverageExpr291 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringConstant_in_encodedCoverageExpr295 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_encodedCoverageExpr302 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STORE_in_storeExpr319 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_storeExpr321 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_encodedCoverageExpr_in_storeExpr325 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_storeExpr327 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageLogicTerm_in_coverageExpr350 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_set_in_coverageExpr366 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicTerm_in_coverageExpr374 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_coverageLogicFactor_in_coverageLogicTerm400 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_AND_in_coverageLogicTerm417 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicFactor_in_coverageLogicTerm422 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor449 = new BitSet(new long[]{0x00000000003F0002L});
    public static final BitSet FOLLOW_set_in_coverageLogicFactor465 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor481 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr509 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_set_in_coverageArithmeticExpr525 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr533 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm561 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_coverageArithmeticTerm576 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm584 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_coverageValue_in_coverageArithmeticFactor612 = new BitSet(new long[]{0x0000000004000002L});
    public static final BitSet FOLLOW_OVERLAY_in_coverageArithmeticFactor628 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageValue_in_coverageArithmeticFactor633 = new BitSet(new long[]{0x0000000004000002L});
    public static final BitSet FOLLOW_subsetExpr_in_coverageValue659 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryInducedExpr_in_coverageValue672 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scaleExpr_in_coverageValue684 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_crsTransformExpr_in_coverageValue696 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_coverageValue708 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_coverageAtom731 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variableName_in_coverageAtom743 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_coverageAtom753 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_coverageAtom757 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_coverageAtom759 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstantExpr_in_coverageAtom772 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstructorExpr_in_coverageAtom784 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_setMetaDataExpr_in_coverageAtom797 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rangeConstructorExpr_in_coverageAtom810 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_metaDataExpr_in_scalarExpr834 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_scalarExpr847 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_scalarExpr860 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_scalarExpr874 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_scalarExpr887 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_scalarExpr898 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_scalarExpr902 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_scalarExpr904 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IDENTIFIER_in_metaDataExpr928 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr930 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr934 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr936 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IMAGECRS_in_metaDataExpr948 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr950 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr954 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr956 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IMAGECRSDOMAIN_in_metaDataExpr968 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr970 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr974 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_COMMA_in_metaDataExpr977 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_metaDataExpr981 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr985 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CRSSET_in_metaDataExpr997 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr999 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1003 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1005 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_domainExpr_in_metaDataExpr1017 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NULLSET_in_metaDataExpr1029 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr1031 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1035 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1037 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTERPOLATIONDEFAULT_in_metaDataExpr1049 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr1051 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1055 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_metaDataExpr1057 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_metaDataExpr1061 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1063 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTERPOLATIONSET_in_metaDataExpr1075 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr1077 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1081 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_metaDataExpr1083 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_metaDataExpr1087 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1089 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DOMAIN_in_domainExpr1108 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_domainExpr1110 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_variableName_in_domainExpr1114 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_domainExpr1116 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_domainExpr1120 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_domainExpr1122 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_domainExpr1126 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_domainExpr1128 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reduceExpr_in_condenseExpr1145 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_generalCondenseExpr_in_condenseExpr1154 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_reduceExpr1171 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_reduceExpr1187 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_reduceExpr1191 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_reduceExpr1193 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CONDENSE_in_generalCondenseExpr1208 = new BitSet(new long[]{0x000003000140A000L});
    public static final BitSet FOLLOW_condenseOpType_in_generalCondenseExpr1212 = new BitSet(new long[]{0x0000080000000000L});
    public static final BitSet FOLLOW_OVER_in_generalCondenseExpr1214 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_axisIteratorList_in_generalCondenseExpr1218 = new BitSet(new long[]{0x0000100000000200L});
    public static final BitSet FOLLOW_WHERE_in_generalCondenseExpr1225 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_generalCondenseExpr1229 = new BitSet(new long[]{0x0000100000000000L});
    public static final BitSet FOLLOW_USING_in_generalCondenseExpr1237 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_generalCondenseExpr1241 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variableName_in_axisIteratorList1258 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_axisIteratorList1262 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_axisIteratorList1264 = new BitSet(new long[]{0x0000000020000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_intervalExpr_in_axisIteratorList1268 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_axisIteratorList1270 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_axisIteratorList1278 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_variableName_in_axisIteratorList1282 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_axisIteratorList1286 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_axisIteratorList1288 = new BitSet(new long[]{0x0000000020000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_intervalExpr_in_axisIteratorList1292 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_axisIteratorList1294 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_indexExpr_in_intervalExpr1318 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_intervalExpr1320 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_intervalExpr1324 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IMAGECRSDOMAIN_in_intervalExpr1339 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_intervalExpr1341 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_intervalExpr1345 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_intervalExpr1347 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_intervalExpr1351 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_intervalExpr1353 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_COVERAGE_in_coverageConstantExpr1379 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_coverageConstantExpr1383 = new BitSet(new long[]{0x0000080000000000L});
    public static final BitSet FOLLOW_OVER_in_coverageConstantExpr1385 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_axisIteratorList_in_coverageConstantExpr1389 = new BitSet(new long[]{0x0000800000000000L});
    public static final BitSet FOLLOW_VALUE_in_coverageConstantExpr1391 = new BitSet(new long[]{0x0001000000000000L});
    public static final BitSet FOLLOW_LIST_in_coverageConstantExpr1393 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_LT_in_coverageConstantExpr1395 = new BitSet(new long[]{0x0000000000000040L,0x0000000600014000L});
    public static final BitSet FOLLOW_constantList_in_coverageConstantExpr1399 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_GT_in_coverageConstantExpr1401 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_constant_in_constantList1432 = new BitSet(new long[]{0x0002000000000002L});
    public static final BitSet FOLLOW_SEMICOLON_in_constantList1437 = new BitSet(new long[]{0x0000000000000040L,0x0000000600014000L});
    public static final BitSet FOLLOW_constant_in_constantList1441 = new BitSet(new long[]{0x0002000000000002L});
    public static final BitSet FOLLOW_COVERAGE_in_coverageConstructorExpr1461 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_coverageConstructorExpr1465 = new BitSet(new long[]{0x0000080000000000L});
    public static final BitSet FOLLOW_OVER_in_coverageConstructorExpr1467 = new BitSet(new long[]{0x0000000000000000L,0x0000180000000000L});
    public static final BitSet FOLLOW_axisIteratorList_in_coverageConstructorExpr1471 = new BitSet(new long[]{0x0004000000000000L});
    public static final BitSet FOLLOW_VALUES_in_coverageConstructorExpr1473 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_coverageConstructorExpr1477 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETIDENTIFIER_in_setMetaDataExpr1499 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1501 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringConstant_in_setMetaDataExpr1505 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1507 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1511 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1513 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETCRSSET_in_setMetaDataExpr1526 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1528 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1532 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1534 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_crsList_in_setMetaDataExpr1538 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1540 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETNULLSET_in_setMetaDataExpr1557 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1559 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1563 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1565 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_rangeExprList_in_setMetaDataExpr1569 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1571 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETINTERPOLATIONDEFAULT_in_setMetaDataExpr1588 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1590 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1594 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1596 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_setMetaDataExpr1600 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1602 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_setMetaDataExpr1606 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1608 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETINTERPOLATIONSET_in_setMetaDataExpr1628 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1630 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1634 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1636 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_setMetaDataExpr1640 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1642 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_interpolationMethodList_in_setMetaDataExpr1646 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1648 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_crsList1677 = new BitSet(new long[]{0x0200000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_crsList1684 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsList1689 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_crsList1693 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_crsList1702 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_rangeExprList1721 = new BitSet(new long[]{0x0600000000000000L});
    public static final BitSet FOLLOW_rangeExpr_in_rangeExprList1728 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_rangeExprList1733 = new BitSet(new long[]{0x0400000000000000L});
    public static final BitSet FOLLOW_rangeExpr_in_rangeExprList1737 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_rangeExprList1746 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_interpolationMethodList1762 = new BitSet(new long[]{0x0200000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_interpolationMethodList1769 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_interpolationMethodList1774 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_interpolationMethodList1778 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_interpolationMethodList1786 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRUCT_in_rangeExpr1799 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_rangeExpr1801 = new BitSet(new long[]{0x0200000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeExpr1809 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1811 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_rangeExpr1815 = new BitSet(new long[]{0x0200200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1822 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeExpr1826 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1828 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_rangeExpr1832 = new BitSet(new long[]{0x0200200000000000L});
    public static final BitSet FOLLOW_RBRACE_in_rangeExpr1843 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRUCT_in_rangeConstructorExpr1860 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_rangeConstructorExpr1864 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeConstructorExpr1868 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeConstructorExpr1870 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_rangeConstructorExpr1874 = new BitSet(new long[]{0x0202000000000000L});
    public static final BitSet FOLLOW_SEMICOLON_in_rangeConstructorExpr1887 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeConstructorExpr1891 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeConstructorExpr1893 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_rangeConstructorExpr1897 = new BitSet(new long[]{0x0202000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_rangeConstructorExpr1903 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CRSTRANSFORM_in_crsTransformExpr1919 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_crsTransformExpr1921 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_crsTransformExpr1925 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsTransformExpr1927 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_crsTransformExpr1931 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsTransformExpr1933 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_fieldInterpolationList_in_crsTransformExpr1937 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_crsTransformExpr1939 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_fieldInterpolationList1956 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1960 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_fieldInterpolationList1967 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1971 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_fieldInterpolationList1978 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldName_in_fieldInterpolationElement1993 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_fieldInterpolationElement1997 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldExpr_in_unaryInducedExpr2017 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryArithmeticExpr_in_unaryInducedExpr2026 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_exponentialExpr_in_unaryInducedExpr2038 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trigonometricExpr_in_unaryInducedExpr2050 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanExpr_in_unaryInducedExpr2062 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_castExpr_in_unaryInducedExpr2074 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rangeConstructorExpr_in_unaryInducedExpr2086 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_unaryArithmeticExpr2109 = new BitSet(new long[]{0x05F847FFF8801840L,0x000018060001D000L});
    public static final BitSet FOLLOW_coverageAtom_in_unaryArithmeticExpr2117 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_unaryArithmeticExpr2129 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_unaryArithmeticExpr2139 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_unaryArithmeticExpr2143 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_unaryArithmeticExpr2145 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_exponentialExpr2168 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_exponentialExpr2176 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_exponentialExpr2180 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_exponentialExpr2182 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_trigonometricExpr2205 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_trigonometricExpr2225 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_trigonometricExpr2229 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_trigonometricExpr2231 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_booleanExpr2254 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_booleanExpr2258 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BIT_in_booleanExpr2270 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_booleanExpr2272 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_booleanExpr2276 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_booleanExpr2278 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_booleanExpr2282 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_booleanExpr2284 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_indexTerm_in_indexExpr2308 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_PLUS_in_indexExpr2319 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_MINUS_in_indexExpr2322 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexTerm_in_indexExpr2328 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_indexFactor_in_indexTerm2353 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_indexTerm2382 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexFactor_in_indexTerm2390 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_INTEGERCONSTANT_in_indexFactor2417 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ROUND_in_indexFactor2430 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_indexFactor2432 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_indexFactor2436 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_indexFactor2438 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_indexFactor2467 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_indexFactor2471 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_indexFactor2473 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IDENTIFIER_in_stringScalarExpr2500 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_stringScalarExpr2502 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_stringScalarExpr2506 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_stringScalarExpr2508 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_stringScalarExpr2521 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SCALE_in_scaleExpr2539 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_scaleExpr2541 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_scaleExpr2545 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_scaleExpr2547 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_scaleExpr2551 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_scaleExpr2553 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_fieldInterpolationList_in_scaleExpr2557 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_scaleExpr2559 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trimExpr_in_subsetExpr2578 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_sliceExpr_in_subsetExpr2587 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_extendExpr_in_subsetExpr2596 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_trimExpr2613 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_LBRACKET_in_trimExpr2615 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_trimExpr2619 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_RBRACKET_in_trimExpr2621 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_TRIM_in_trimExpr2630 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_trimExpr2632 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_trimExpr2636 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_trimExpr2638 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_trimExpr2640 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_trimExpr2644 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_trimExpr2646 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_trimExpr2648 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_sliceExpr2665 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_LBRACKET_in_sliceExpr2667 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointList_in_sliceExpr2671 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_RBRACKET_in_sliceExpr2673 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SLICE_in_sliceExpr2680 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_sliceExpr2682 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_sliceExpr2686 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_sliceExpr2688 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_sliceExpr2690 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointList_in_sliceExpr2694 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_sliceExpr2696 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_sliceExpr2698 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EXTEND_in_extendExpr2713 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_extendExpr2715 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_extendExpr2719 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_extendExpr2721 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_extendExpr2725 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_extendExpr2727 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_castExpr2745 = new BitSet(new long[]{0x0000000000000000L,0x00000000FF800000L});
    public static final BitSet FOLLOW_rangeType_in_castExpr2749 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_castExpr2751 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_castExpr2755 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_rangeType2778 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_UNSIGNED_in_rangeType2804 = new BitSet(new long[]{0x0000000000000000L,0x0000000007000000L});
    public static final BitSet FOLLOW_set_in_rangeType2808 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_fieldExpr2837 = new BitSet(new long[]{0x0000000000000000L,0x0000000100000000L});
    public static final BitSet FOLLOW_DOT_in_fieldExpr2839 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_fieldExpr2843 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_booleanScalarExpr2868 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_OR_in_booleanScalarExpr2882 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_XOR_in_booleanScalarExpr2885 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_booleanScalarExpr2891 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_booleanScalarTerm2913 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_AND_in_booleanScalarTerm2923 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_booleanScalarTerm2927 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_booleanScalarAtom_in_booleanScalarNegation2950 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_booleanScalarNegation2959 = new BitSet(new long[]{0x000007F808800040L,0x000000060001C000L});
    public static final BitSet FOLLOW_booleanScalarAtom_in_booleanScalarNegation2963 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_booleanScalarAtom2978 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_booleanScalarAtom2982 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_booleanScalarAtom2984 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_booleanScalarAtom2993 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_booleanScalarAtom2997 = new BitSet(new long[]{0x0000000008000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringScalarExpr_in_booleanScalarAtom3001 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_booleanScalarAtom3011 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_booleanScalarAtom3015 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_booleanScalarAtom3019 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BOOLEANCONSTANT_in_booleanScalarAtom3029 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarTerm_in_numericScalarExpr3047 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_PLUS_in_numericScalarExpr3058 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_MINUS_in_numericScalarExpr3061 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarTerm_in_numericScalarExpr3067 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarTerm3086 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_MULT_in_numericScalarTerm3096 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_DIVIDE_in_numericScalarTerm3099 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarTerm3105 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3125 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3129 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3131 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MINUS_in_numericScalarFactor3143 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarFactor3148 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ROUND_in_numericScalarFactor3160 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3162 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3166 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3168 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTEGERCONSTANT_in_numericScalarFactor3180 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_numericScalarFactor3192 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_numericScalarFactor3204 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_numericScalarFactor3216 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EQUALS_in_compOp3234 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOTEQUALS_in_compOp3241 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LT_in_compOp3248 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GT_in_compOp3255 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LTE_in_compOp3262 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GTE_in_compOp3269 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3289 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_dimensionIntervalList3302 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3306 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_axisName_in_dimensionIntervalElement3331 = new BitSet(new long[]{0x0000200000000040L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalElement3336 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionIntervalElement3340 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionIntervalElement3351 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_dimensionIntervalExpr_in_dimensionIntervalElement3355 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionIntervalElement3357 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionIntervalExpr3380 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3382 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionIntervalExpr3386 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DOMAIN_in_dimensionIntervalExpr3396 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionIntervalExpr3398 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_dimensionIntervalExpr3402 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3404 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_dimensionIntervalExpr3408 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3410 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionIntervalExpr3414 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionIntervalExpr3416 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_dimensionPointElement_in_dimensionPointList3439 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_dimensionPointList3449 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointElement_in_dimensionPointList3453 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_axisName_in_dimensionPointElement3478 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionPointElement3480 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_dimensionPoint_in_dimensionPointElement3484 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionPointElement3486 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_axisName_in_dimensionPointElement3498 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionPointElement3500 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionPointElement3504 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionPointElement3506 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_dimensionPoint_in_dimensionPointElement3510 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionPointElement3512 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionPoint3535 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_interpolationMethod3553 = new BitSet(new long[]{0x0000000000000000L,0x0000007800000000L});
    public static final BitSet FOLLOW_interpolationType_in_interpolationMethod3557 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_interpolationMethod3559 = new BitSet(new long[]{0x0000000000000000L,0x0000078000000000L});
    public static final BitSet FOLLOW_nullResistence_in_interpolationMethod3563 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_interpolationMethod3565 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_interpolationType3582 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_nullResistence3607 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_condenseOpType3632 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_fieldName3659 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_constant3676 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_constant3693 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_complexConstant3708 = new BitSet(new long[]{0x0000000000000000L,0x0000000400000000L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_complexConstant3712 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_complexConstant3714 = new BitSet(new long[]{0x0000000000000000L,0x0000000400000000L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_complexConstant3718 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_complexConstant3720 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_stringConstant3737 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_name3754 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringConstant_in_crsName3781 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_axisName3798 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_variableName3815 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_coverageName3836 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred8_wcps366 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicTerm_in_synpred8_wcps374 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AND_in_synpred9_wcps417 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicFactor_in_synpred9_wcps422 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred15_wcps465 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticExpr_in_synpred15_wcps481 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred17_wcps525 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticTerm_in_synpred17_wcps533 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred19_wcps576 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticFactor_in_synpred19_wcps584 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_OVERLAY_in_synpred20_wcps628 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageValue_in_synpred20_wcps633 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_subsetExpr_in_synpred21_wcps659 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryInducedExpr_in_synpred22_wcps672 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred25_wcps731 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred27_wcps753 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_synpred27_wcps757 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred27_wcps759 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstantExpr_in_synpred28_wcps772 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstructorExpr_in_synpred29_wcps784 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_metaDataExpr_in_synpred31_wcps834 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_synpred32_wcps847 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_synpred33_wcps860 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred34_wcps874 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_synpred35_wcps887 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldExpr_in_synpred70_wcps2017 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryArithmeticExpr_in_synpred71_wcps2026 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanExpr_in_synpred74_wcps2062 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred95_wcps2366 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexFactor_in_synpred95_wcps2374 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred99_wcps2450 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_synpred99_wcps2454 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred99_wcps2456 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trimExpr_in_synpred101_wcps2578 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_sliceExpr_in_synpred102_wcps2587 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred116_wcps2881 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_synpred116_wcps2891 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AND_in_synpred117_wcps2923 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_synpred117_wcps2927 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred119_wcps2978 = new BitSet(new long[]{0x000007F808800040L,0x000000060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_synpred119_wcps2982 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred119_wcps2984 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred121_wcps3011 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_synpred121_wcps3015 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred121_wcps3019 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred123_wcps3057 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarTerm_in_synpred123_wcps3067 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred125_wcps3095 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_synpred125_wcps3105 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred126_wcps3125 = new BitSet(new long[]{0x000007F800800040L,0x000000040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred126_wcps3129 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred126_wcps3131 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_synpred131_wcps3204 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred139_wcps3380 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_synpred139_wcps3382 = new BitSet(new long[]{0x000007FFF8801840L,0x000000060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred139_wcps3386 = new BitSet(new long[]{0x0000000000000002L});

}