// $ANTLR 3.2 Sep 23, 2009 12:02:23 wcps.g 2012-04-03 13:32:27
package petascope.wcps.grammar;

import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import org.antlr.runtime.tree.*;

public class wcpsParser extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "FOR", "IN", "LPAREN", "RPAREN", "COMMA", "WHERE", "RETURN", "ENCODE", "STORE", "OR", "XOR", "AND", "EQUALS", "NOTEQUALS", "LT", "GT", "LTE", "GTE", "PLUS", "MINUS", "MULT", "DIVIDE", "OVERLAY", "IDENTIFIER", "IMAGECRS", "IMAGECRSDOMAIN", "CRSSET", "NULLSET", "INTERPOLATIONDEFAULT", "INTERPOLATIONSET", "DOMAIN", "ALL", "SOME", "COUNT", "ADD", "AVG", "MIN", "MAX", "CONDENSE", "OVER", "USING", "COLON", "COVERAGE", "VALUE", "LIST", "SEMICOLON", "VALUES", "SETIDENTIFIER", "SETCRSSET", "SETNULLSET", "SETINTERPOLATIONDEFAULT", "SETINTERPOLATIONSET", "LBRACE", "RBRACE", "STRUCT", "CRSTRANSFORM", "SQRT", "ABS", "RE", "IM", "EXP", "LOG", "LN", "SIN", "COS", "TAN", "SINH", "COSH", "TANH", "ARCSIN", "ARCCOS", "ARCTAN", "NOT", "BIT", "INTEGERCONSTANT", "ROUND", "STRING", "SCALE", "LBRACKET", "RBRACKET", "TRIM", "SLICE", "EXTEND", "BOOLEAN", "CHAR", "SHORT", "LONG", "FLOAT", "DOUBLE", "COMPLEX", "COMPLEX2", "UNSIGNED", "DOT", "BOOLEANCONSTANT", "FLOATCONSTANT", "NEAREST", "LINEAR", "QUADRATIC", "CUBIC", "FULL", "NONE", "HALF", "OTHER", "NAME", "VARIABLE_DOLLAR", "NULLDEFAULT", "PHI", "WHITESPACE"
    };
    public static final int IMAGECRS=28;
    public static final int LT=18;
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
    public static final int WHITESPACE=111;
    public static final int SEMICOLON=49;
    public static final int MULT=24;
    public static final int VALUE=47;
    public static final int LIST=48;
    public static final int COMPLEX2=94;
    public static final int ABS=61;
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
            this.state.ruleMemo = new HashMap[244+1];
             
             
        }
        
    protected TreeAdaptor adaptor = new CommonTreeAdaptor();

    public void setTreeAdaptor(TreeAdaptor adaptor) {
        this.adaptor = adaptor;
    }
    public TreeAdaptor getTreeAdaptor() {
        return adaptor;
    }

    public String[] getTokenNames() { return wcpsParser.tokenNames; }
    public String getGrammarFileName() { return "wcps.g"; }


    public static class wcpsRequest_return extends ParserRuleReturnScope {
        public WCPSRequest value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "wcpsRequest"
    // wcps.g:36:1: wcpsRequest returns [WCPSRequest value] : e1= forClause (e2= whereClause )? e3= returnClause ;
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
            // wcps.g:37:2: (e1= forClause (e2= whereClause )? e3= returnClause )
            // wcps.g:37:4: e1= forClause (e2= whereClause )? e3= returnClause
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
            // wcps.g:38:3: (e2= whereClause )?
            int alt1=2;
            int LA1_0 = input.LA(1);

            if ( (LA1_0==WHERE) ) {
                alt1=1;
            }
            switch (alt1) {
                case 1 :
                    // wcps.g:38:4: e2= whereClause
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
    // wcps.g:41:1: forClause returns [ForClauseElements value] : FOR v= coverageVariable IN LPAREN list= coverageList RPAREN ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )* ;
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
        wcpsParser.coverageVariable_return v = null;

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
            // wcps.g:42:2: ( FOR v= coverageVariable IN LPAREN list= coverageList RPAREN ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )* )
            // wcps.g:42:4: FOR v= coverageVariable IN LPAREN list= coverageList RPAREN ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )*
            {
            root_0 = (Object)adaptor.nil();

            FOR1=(Token)match(input,FOR,FOLLOW_FOR_in_forClause98); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            FOR1_tree = (Object)adaptor.create(FOR1);
            adaptor.addChild(root_0, FOR1_tree);
            }
            pushFollow(FOLLOW_coverageVariable_in_forClause102);
            v=coverageVariable();

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
            // wcps.g:44:4: ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==COMMA) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // wcps.g:44:5: COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN
            	    {
            	    COMMA5=(Token)match(input,COMMA,FOLLOW_COMMA_in_forClause122); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA5_tree = (Object)adaptor.create(COMMA5);
            	    adaptor.addChild(root_0, COMMA5_tree);
            	    }
            	    pushFollow(FOLLOW_coverageVariable_in_forClause126);
            	    v=coverageVariable();

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
    // wcps.g:47:1: whereClause returns [WhereClause value] : WHERE e1= booleanScalarExpr ;
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
            // wcps.g:48:2: ( WHERE e1= booleanScalarExpr )
            // wcps.g:48:4: WHERE e1= booleanScalarExpr
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
    // wcps.g:50:1: returnClause returns [ReturnClause value] : RETURN e1= processingExpr ;
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
            // wcps.g:51:2: ( RETURN e1= processingExpr )
            // wcps.g:51:4: RETURN e1= processingExpr
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
    // wcps.g:53:1: coverageList returns [CoverageList value] : cname= coverageName ( COMMA next= coverageName )* ;
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
            // wcps.g:54:2: (cname= coverageName ( COMMA next= coverageName )* )
            // wcps.g:54:4: cname= coverageName ( COMMA next= coverageName )*
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
            // wcps.g:55:3: ( COMMA next= coverageName )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==COMMA) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // wcps.g:55:4: COMMA next= coverageName
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
    // wcps.g:57:1: processingExpr returns [ProcessingExpr value] : (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr );
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
            // wcps.g:58:5: (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr )
            int alt4=3;
            alt4 = dfa4.predict(input);
            switch (alt4) {
                case 1 :
                    // wcps.g:58:7: e1= encodedCoverageExpr
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
                    // wcps.g:59:7: e2= storeExpr
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
                    // wcps.g:60:7: e3= scalarExpr
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
    // wcps.g:62:1: encodedCoverageExpr returns [EncodedCoverageExpr value] : ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN ;
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
            // wcps.g:63:2: ( ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN )
            // wcps.g:63:4: ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN
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
            // wcps.g:64:3: ( COMMA params= stringConstant )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==COMMA) ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // wcps.g:64:4: COMMA params= stringConstant
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
    // wcps.g:66:1: storeExpr returns [StoreExpr value] : STORE LPAREN e1= encodedCoverageExpr RPAREN ;
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
            // wcps.g:67:5: ( STORE LPAREN e1= encodedCoverageExpr RPAREN )
            // wcps.g:67:7: STORE LPAREN e1= encodedCoverageExpr RPAREN
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
    // wcps.g:69:1: coverageExpr returns [CoverageExpr value] : e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )* ;
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
            // wcps.g:70:5: (e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )* )
            // wcps.g:70:7: e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )*
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
            // wcps.g:71:9: (op= ( OR | XOR ) e2= coverageLogicTerm )*
            loop6:
            do {
                int alt6=2;
                alt6 = dfa6.predict(input);
                switch (alt6) {
            	case 1 :
            	    // wcps.g:71:10: op= ( OR | XOR ) e2= coverageLogicTerm
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
    // wcps.g:73:1: coverageLogicTerm returns [CoverageExpr value] : e1= coverageLogicFactor (op= AND e2= coverageLogicFactor )* ;
    public final wcpsParser.coverageLogicTerm_return coverageLogicTerm() throws RecognitionException {
        wcpsParser.coverageLogicTerm_return retval = new wcpsParser.coverageLogicTerm_return();
        retval.start = input.LT(1);
        int coverageLogicTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.coverageLogicFactor_return e1 = null;

        wcpsParser.coverageLogicFactor_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 10) ) { return retval; }
            // wcps.g:74:5: (e1= coverageLogicFactor (op= AND e2= coverageLogicFactor )* )
            // wcps.g:74:7: e1= coverageLogicFactor (op= AND e2= coverageLogicFactor )*
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
            // wcps.g:75:9: (op= AND e2= coverageLogicFactor )*
            loop7:
            do {
                int alt7=2;
                alt7 = dfa7.predict(input);
                switch (alt7) {
            	case 1 :
            	    // wcps.g:75:10: op= AND e2= coverageLogicFactor
            	    {
            	    op=(Token)match(input,AND,FOLLOW_AND_in_coverageLogicTerm416); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    op_tree = (Object)adaptor.create(op);
            	    adaptor.addChild(root_0, op_tree);
            	    }
            	    pushFollow(FOLLOW_coverageLogicFactor_in_coverageLogicTerm420);
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
    // wcps.g:77:1: coverageLogicFactor returns [CoverageExpr value] : e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )? ;
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
            // wcps.g:78:5: (e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )? )
            // wcps.g:78:7: e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor447);
            e1=coverageArithmeticExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null);
            }
            // wcps.g:79:9: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?
            int alt8=2;
            alt8 = dfa8.predict(input);
            switch (alt8) {
                case 1 :
                    // wcps.g:79:10: op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr
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

                    pushFollow(FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor479);
                    e2=coverageArithmeticExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
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
    // wcps.g:81:1: coverageArithmeticExpr returns [CoverageExpr value] : e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )* ;
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
            // wcps.g:82:5: (e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )* )
            // wcps.g:82:7: e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr507);
            e1=coverageArithmeticTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:83:9: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*
            loop9:
            do {
                int alt9=2;
                alt9 = dfa9.predict(input);
                switch (alt9) {
            	case 1 :
            	    // wcps.g:83:10: op= ( PLUS | MINUS ) e2= coverageArithmeticTerm
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

            	    pushFollow(FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr531);
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
    // wcps.g:85:1: coverageArithmeticTerm returns [CoverageExpr value] : e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )* ;
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
            // wcps.g:86:5: (e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )* )
            // wcps.g:86:9: e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm559);
            e1=coverageArithmeticFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:87:9: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*
            loop10:
            do {
                int alt10=2;
                alt10 = dfa10.predict(input);
                switch (alt10) {
            	case 1 :
            	    // wcps.g:87:10: op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor
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

            	    pushFollow(FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm582);
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
    // wcps.g:89:1: coverageArithmeticFactor returns [CoverageExpr value] : e1= coverageValue (op= OVERLAY e2= coverageValue )* ;
    public final wcpsParser.coverageArithmeticFactor_return coverageArithmeticFactor() throws RecognitionException {
        wcpsParser.coverageArithmeticFactor_return retval = new wcpsParser.coverageArithmeticFactor_return();
        retval.start = input.LT(1);
        int coverageArithmeticFactor_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.coverageValue_return e1 = null;

        wcpsParser.coverageValue_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 14) ) { return retval; }
            // wcps.g:90:5: (e1= coverageValue (op= OVERLAY e2= coverageValue )* )
            // wcps.g:90:7: e1= coverageValue (op= OVERLAY e2= coverageValue )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageValue_in_coverageArithmeticFactor610);
            e1=coverageValue();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:91:9: (op= OVERLAY e2= coverageValue )*
            loop11:
            do {
                int alt11=2;
                alt11 = dfa11.predict(input);
                switch (alt11) {
            	case 1 :
            	    // wcps.g:91:10: op= OVERLAY e2= coverageValue
            	    {
            	    op=(Token)match(input,OVERLAY,FOLLOW_OVERLAY_in_coverageArithmeticFactor625); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    op_tree = (Object)adaptor.create(op);
            	    adaptor.addChild(root_0, op_tree);
            	    }
            	    pushFollow(FOLLOW_coverageValue_in_coverageArithmeticFactor629);
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
    // wcps.g:93:1: coverageValue returns [CoverageExpr value] : (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom );
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
            // wcps.g:94:5: (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom )
            int alt12=5;
            alt12 = dfa12.predict(input);
            switch (alt12) {
                case 1 :
                    // wcps.g:94:7: e5= subsetExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_subsetExpr_in_coverageValue655);
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
                    // wcps.g:95:7: e2= unaryInducedExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_unaryInducedExpr_in_coverageValue668);
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
                    // wcps.g:96:7: e4= scaleExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scaleExpr_in_coverageValue680);
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
                    // wcps.g:97:7: e3= crsTransformExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_crsTransformExpr_in_coverageValue692);
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
                    // wcps.g:98:7: e1= coverageAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_coverageValue704);
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
    // wcps.g:100:1: coverageAtom returns [CoverageExpr value] : (e2= scalarExpr | e1= coverageVariable | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr );
    public final wcpsParser.coverageAtom_return coverageAtom() throws RecognitionException {
        wcpsParser.coverageAtom_return retval = new wcpsParser.coverageAtom_return();
        retval.start = input.LT(1);
        int coverageAtom_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN20=null;
        Token RPAREN21=null;
        wcpsParser.scalarExpr_return e2 = null;

        wcpsParser.coverageVariable_return e1 = null;

        wcpsParser.coverageExpr_return e7 = null;

        wcpsParser.coverageConstantExpr_return e3 = null;

        wcpsParser.coverageConstructorExpr_return e4 = null;

        wcpsParser.setMetaDataExpr_return e5 = null;

        wcpsParser.rangeConstructorExpr_return e6 = null;


        Object LPAREN20_tree=null;
        Object RPAREN21_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 16) ) { return retval; }
            // wcps.g:101:5: (e2= scalarExpr | e1= coverageVariable | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr )
            int alt13=7;
            alt13 = dfa13.predict(input);
            switch (alt13) {
                case 1 :
                    // wcps.g:101:7: e2= scalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scalarExpr_in_coverageAtom727);
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
                    // wcps.g:102:7: e1= coverageVariable
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageVariable_in_coverageAtom739);
                    e1=coverageVariable();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // wcps.g:103:7: LPAREN e7= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN20=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_coverageAtom749); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN20_tree = (Object)adaptor.create(LPAREN20);
                    adaptor.addChild(root_0, LPAREN20_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_coverageAtom753);
                    e7=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e7.getTree());
                    RPAREN21=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_coverageAtom755); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN21_tree = (Object)adaptor.create(RPAREN21);
                    adaptor.addChild(root_0, RPAREN21_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new CoverageExpr((e7!=null?e7.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // wcps.g:104:7: e3= coverageConstantExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageConstantExpr_in_coverageAtom768);
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
                    // wcps.g:105:7: e4= coverageConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageConstructorExpr_in_coverageAtom780);
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
                    // wcps.g:106:7: e5= setMetaDataExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_setMetaDataExpr_in_coverageAtom793);
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
                    // wcps.g:107:7: e6= rangeConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_rangeConstructorExpr_in_coverageAtom806);
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
    // wcps.g:109:1: scalarExpr returns [ScalarExpr value] : (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN );
    public final wcpsParser.scalarExpr_return scalarExpr() throws RecognitionException {
        wcpsParser.scalarExpr_return retval = new wcpsParser.scalarExpr_return();
        retval.start = input.LT(1);
        int scalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN22=null;
        Token RPAREN23=null;
        wcpsParser.metaDataExpr_return e1 = null;

        wcpsParser.condenseExpr_return e2 = null;

        wcpsParser.booleanScalarExpr_return e3 = null;

        wcpsParser.numericScalarExpr_return e4 = null;

        wcpsParser.stringScalarExpr_return e5 = null;

        wcpsParser.scalarExpr_return e6 = null;


        Object LPAREN22_tree=null;
        Object RPAREN23_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 17) ) { return retval; }
            // wcps.g:110:5: (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN )
            int alt14=6;
            alt14 = dfa14.predict(input);
            switch (alt14) {
                case 1 :
                    // wcps.g:110:7: e1= metaDataExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_metaDataExpr_in_scalarExpr830);
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
                    // wcps.g:111:7: e2= condenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_condenseExpr_in_scalarExpr843);
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
                    // wcps.g:112:7: e3= booleanScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanScalarExpr_in_scalarExpr856);
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
                    // wcps.g:113:7: e4= numericScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_numericScalarExpr_in_scalarExpr870);
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
                    // wcps.g:114:7: e5= stringScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_stringScalarExpr_in_scalarExpr883);
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
                    // wcps.g:115:7: LPAREN e6= scalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN22=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_scalarExpr894); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN22_tree = (Object)adaptor.create(LPAREN22);
                    adaptor.addChild(root_0, LPAREN22_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_scalarExpr898);
                    e6=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e6.getTree());
                    RPAREN23=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_scalarExpr900); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN23_tree = (Object)adaptor.create(RPAREN23);
                    adaptor.addChild(root_0, RPAREN23_tree);
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
    // wcps.g:117:1: metaDataExpr returns [MetaDataExpr value] : (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | op= IMAGECRS LPAREN e1= coverageExpr RPAREN | op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN | op= CRSSET LPAREN e1= coverageExpr RPAREN | de= domainExpr | op= NULLSET LPAREN e1= coverageExpr RPAREN | op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN | op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN );
    public final wcpsParser.metaDataExpr_return metaDataExpr() throws RecognitionException {
        wcpsParser.metaDataExpr_return retval = new wcpsParser.metaDataExpr_return();
        retval.start = input.LT(1);
        int metaDataExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN24=null;
        Token RPAREN25=null;
        Token LPAREN26=null;
        Token RPAREN27=null;
        Token LPAREN28=null;
        Token COMMA29=null;
        Token RPAREN30=null;
        Token LPAREN31=null;
        Token RPAREN32=null;
        Token LPAREN33=null;
        Token RPAREN34=null;
        Token LPAREN35=null;
        Token COMMA36=null;
        Token RPAREN37=null;
        Token LPAREN38=null;
        Token COMMA39=null;
        Token RPAREN40=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.axisName_return e2 = null;

        wcpsParser.domainExpr_return de = null;

        wcpsParser.fieldName_return f1 = null;


        Object op_tree=null;
        Object LPAREN24_tree=null;
        Object RPAREN25_tree=null;
        Object LPAREN26_tree=null;
        Object RPAREN27_tree=null;
        Object LPAREN28_tree=null;
        Object COMMA29_tree=null;
        Object RPAREN30_tree=null;
        Object LPAREN31_tree=null;
        Object RPAREN32_tree=null;
        Object LPAREN33_tree=null;
        Object RPAREN34_tree=null;
        Object LPAREN35_tree=null;
        Object COMMA36_tree=null;
        Object RPAREN37_tree=null;
        Object LPAREN38_tree=null;
        Object COMMA39_tree=null;
        Object RPAREN40_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 18) ) { return retval; }
            // wcps.g:118:5: (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | op= IMAGECRS LPAREN e1= coverageExpr RPAREN | op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN | op= CRSSET LPAREN e1= coverageExpr RPAREN | de= domainExpr | op= NULLSET LPAREN e1= coverageExpr RPAREN | op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN | op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN )
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
                    // wcps.g:118:7: op= IDENTIFIER LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IDENTIFIER,FOLLOW_IDENTIFIER_in_metaDataExpr924); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN24=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr926); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN24_tree = (Object)adaptor.create(LPAREN24);
                    adaptor.addChild(root_0, LPAREN24_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr930);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN25=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr932); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN25_tree = (Object)adaptor.create(RPAREN25);
                    adaptor.addChild(root_0, RPAREN25_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // wcps.g:119:7: op= IMAGECRS LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IMAGECRS,FOLLOW_IMAGECRS_in_metaDataExpr944); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN26=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr946); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN26_tree = (Object)adaptor.create(LPAREN26);
                    adaptor.addChild(root_0, LPAREN26_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr950);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN27=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr952); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN27_tree = (Object)adaptor.create(RPAREN27);
                    adaptor.addChild(root_0, RPAREN27_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // wcps.g:120:7: op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IMAGECRSDOMAIN,FOLLOW_IMAGECRSDOMAIN_in_metaDataExpr964); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN28=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr966); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN28_tree = (Object)adaptor.create(LPAREN28);
                    adaptor.addChild(root_0, LPAREN28_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr970);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    // wcps.g:120:48: ( COMMA e2= axisName )?
                    int alt15=2;
                    int LA15_0 = input.LA(1);

                    if ( (LA15_0==COMMA) ) {
                        alt15=1;
                    }
                    switch (alt15) {
                        case 1 :
                            // wcps.g:120:49: COMMA e2= axisName
                            {
                            COMMA29=(Token)match(input,COMMA,FOLLOW_COMMA_in_metaDataExpr973); if (state.failed) return retval;
                            if ( state.backtracking==0 ) {
                            COMMA29_tree = (Object)adaptor.create(COMMA29);
                            adaptor.addChild(root_0, COMMA29_tree);
                            }
                            pushFollow(FOLLOW_axisName_in_metaDataExpr977);
                            e2=axisName();

                            state._fsp--;
                            if (state.failed) return retval;
                            if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());

                            }
                            break;

                    }

                    RPAREN30=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr981); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN30_tree = (Object)adaptor.create(RPAREN30);
                    adaptor.addChild(root_0, RPAREN30_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // wcps.g:121:7: op= CRSSET LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,CRSSET,FOLLOW_CRSSET_in_metaDataExpr993); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN31=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr995); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN31_tree = (Object)adaptor.create(LPAREN31);
                    adaptor.addChild(root_0, LPAREN31_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr999);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN32=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1001); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN32_tree = (Object)adaptor.create(RPAREN32);
                    adaptor.addChild(root_0, RPAREN32_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // wcps.g:122:7: de= domainExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_domainExpr_in_metaDataExpr1013);
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
                    // wcps.g:123:7: op= NULLSET LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NULLSET,FOLLOW_NULLSET_in_metaDataExpr1025); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN33=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr1027); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN33_tree = (Object)adaptor.create(LPAREN33);
                    adaptor.addChild(root_0, LPAREN33_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1031);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN34=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1033); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN34_tree = (Object)adaptor.create(RPAREN34);
                    adaptor.addChild(root_0, RPAREN34_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null),(e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 7 :
                    // wcps.g:124:7: op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,INTERPOLATIONDEFAULT,FOLLOW_INTERPOLATIONDEFAULT_in_metaDataExpr1045); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN35=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr1047); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN35_tree = (Object)adaptor.create(LPAREN35);
                    adaptor.addChild(root_0, LPAREN35_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1051);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA36=(Token)match(input,COMMA,FOLLOW_COMMA_in_metaDataExpr1053); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA36_tree = (Object)adaptor.create(COMMA36);
                    adaptor.addChild(root_0, COMMA36_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_metaDataExpr1057);
                    f1=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, f1.getTree());
                    RPAREN37=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1059); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN37_tree = (Object)adaptor.create(RPAREN37);
                    adaptor.addChild(root_0, RPAREN37_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new MetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (f1!=null?f1.value:null)); 
                    }

                    }
                    break;
                case 8 :
                    // wcps.g:125:7: op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,INTERPOLATIONSET,FOLLOW_INTERPOLATIONSET_in_metaDataExpr1071); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN38=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_metaDataExpr1073); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN38_tree = (Object)adaptor.create(LPAREN38);
                    adaptor.addChild(root_0, LPAREN38_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_metaDataExpr1077);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA39=(Token)match(input,COMMA,FOLLOW_COMMA_in_metaDataExpr1079); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA39_tree = (Object)adaptor.create(COMMA39);
                    adaptor.addChild(root_0, COMMA39_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_metaDataExpr1083);
                    f1=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, f1.getTree());
                    RPAREN40=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_metaDataExpr1085); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN40_tree = (Object)adaptor.create(RPAREN40);
                    adaptor.addChild(root_0, RPAREN40_tree);
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
    // wcps.g:127:1: domainExpr returns [DomainExpr value] : DOMAIN LPAREN var= coverageVariable COMMA axis= axisName COMMA crs= crsName RPAREN ;
    public final wcpsParser.domainExpr_return domainExpr() throws RecognitionException {
        wcpsParser.domainExpr_return retval = new wcpsParser.domainExpr_return();
        retval.start = input.LT(1);
        int domainExpr_StartIndex = input.index();
        Object root_0 = null;

        Token DOMAIN41=null;
        Token LPAREN42=null;
        Token COMMA43=null;
        Token COMMA44=null;
        Token RPAREN45=null;
        wcpsParser.coverageVariable_return var = null;

        wcpsParser.axisName_return axis = null;

        wcpsParser.crsName_return crs = null;


        Object DOMAIN41_tree=null;
        Object LPAREN42_tree=null;
        Object COMMA43_tree=null;
        Object COMMA44_tree=null;
        Object RPAREN45_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 19) ) { return retval; }
            // wcps.g:128:2: ( DOMAIN LPAREN var= coverageVariable COMMA axis= axisName COMMA crs= crsName RPAREN )
            // wcps.g:128:4: DOMAIN LPAREN var= coverageVariable COMMA axis= axisName COMMA crs= crsName RPAREN
            {
            root_0 = (Object)adaptor.nil();

            DOMAIN41=(Token)match(input,DOMAIN,FOLLOW_DOMAIN_in_domainExpr1104); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            DOMAIN41_tree = (Object)adaptor.create(DOMAIN41);
            adaptor.addChild(root_0, DOMAIN41_tree);
            }
            LPAREN42=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_domainExpr1106); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN42_tree = (Object)adaptor.create(LPAREN42);
            adaptor.addChild(root_0, LPAREN42_tree);
            }
            pushFollow(FOLLOW_coverageVariable_in_domainExpr1110);
            var=coverageVariable();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, var.getTree());
            COMMA43=(Token)match(input,COMMA,FOLLOW_COMMA_in_domainExpr1112); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA43_tree = (Object)adaptor.create(COMMA43);
            adaptor.addChild(root_0, COMMA43_tree);
            }
            pushFollow(FOLLOW_axisName_in_domainExpr1116);
            axis=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, axis.getTree());
            COMMA44=(Token)match(input,COMMA,FOLLOW_COMMA_in_domainExpr1118); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA44_tree = (Object)adaptor.create(COMMA44);
            adaptor.addChild(root_0, COMMA44_tree);
            }
            pushFollow(FOLLOW_crsName_in_domainExpr1122);
            crs=crsName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
            RPAREN45=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_domainExpr1124); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN45_tree = (Object)adaptor.create(RPAREN45);
            adaptor.addChild(root_0, RPAREN45_tree);
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
    // wcps.g:130:1: condenseExpr returns [CondenseExpr value] : (e1= reduceExpr | e2= generalCondenseExpr );
    public final wcpsParser.condenseExpr_return condenseExpr() throws RecognitionException {
        wcpsParser.condenseExpr_return retval = new wcpsParser.condenseExpr_return();
        retval.start = input.LT(1);
        int condenseExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.reduceExpr_return e1 = null;

        wcpsParser.generalCondenseExpr_return e2 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 20) ) { return retval; }
            // wcps.g:131:2: (e1= reduceExpr | e2= generalCondenseExpr )
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
                    // wcps.g:131:4: e1= reduceExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_reduceExpr_in_condenseExpr1141);
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
                    // wcps.g:132:4: e2= generalCondenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_generalCondenseExpr_in_condenseExpr1150);
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
    // wcps.g:134:1: reduceExpr returns [ReduceExpr value] : op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.reduceExpr_return reduceExpr() throws RecognitionException {
        wcpsParser.reduceExpr_return retval = new wcpsParser.reduceExpr_return();
        retval.start = input.LT(1);
        int reduceExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN46=null;
        Token RPAREN47=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN46_tree=null;
        Object RPAREN47_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 21) ) { return retval; }
            // wcps.g:135:2: (op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN )
            // wcps.g:135:4: op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN
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

            LPAREN46=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_reduceExpr1183); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN46_tree = (Object)adaptor.create(LPAREN46);
            adaptor.addChild(root_0, LPAREN46_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_reduceExpr1187);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN47=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_reduceExpr1189); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN47_tree = (Object)adaptor.create(RPAREN47);
            adaptor.addChild(root_0, RPAREN47_tree);
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
    // wcps.g:137:1: generalCondenseExpr returns [GeneralCondenseExpr value] : CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING ce= coverageExpr ;
    public final wcpsParser.generalCondenseExpr_return generalCondenseExpr() throws RecognitionException {
        wcpsParser.generalCondenseExpr_return retval = new wcpsParser.generalCondenseExpr_return();
        retval.start = input.LT(1);
        int generalCondenseExpr_StartIndex = input.index();
        Object root_0 = null;

        Token CONDENSE48=null;
        Token OVER49=null;
        Token WHERE50=null;
        Token USING51=null;
        wcpsParser.condenseOpType_return op = null;

        wcpsParser.axisIteratorList_return ail = null;

        wcpsParser.booleanScalarExpr_return cond = null;

        wcpsParser.coverageExpr_return ce = null;


        Object CONDENSE48_tree=null;
        Object OVER49_tree=null;
        Object WHERE50_tree=null;
        Object USING51_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 22) ) { return retval; }
            // wcps.g:138:2: ( CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING ce= coverageExpr )
            // wcps.g:138:4: CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING ce= coverageExpr
            {
            root_0 = (Object)adaptor.nil();

            CONDENSE48=(Token)match(input,CONDENSE,FOLLOW_CONDENSE_in_generalCondenseExpr1204); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            CONDENSE48_tree = (Object)adaptor.create(CONDENSE48);
            adaptor.addChild(root_0, CONDENSE48_tree);
            }
            pushFollow(FOLLOW_condenseOpType_in_generalCondenseExpr1208);
            op=condenseOpType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, op.getTree());
            OVER49=(Token)match(input,OVER,FOLLOW_OVER_in_generalCondenseExpr1210); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            OVER49_tree = (Object)adaptor.create(OVER49);
            adaptor.addChild(root_0, OVER49_tree);
            }
            pushFollow(FOLLOW_axisIteratorList_in_generalCondenseExpr1214);
            ail=axisIteratorList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ail.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new GeneralCondenseExpr((op!=null?op.value:null), (ail!=null?ail.value:null)); 
            }
            // wcps.g:139:3: ( WHERE cond= booleanScalarExpr )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0==WHERE) ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // wcps.g:139:4: WHERE cond= booleanScalarExpr
                    {
                    WHERE50=(Token)match(input,WHERE,FOLLOW_WHERE_in_generalCondenseExpr1221); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    WHERE50_tree = (Object)adaptor.create(WHERE50);
                    adaptor.addChild(root_0, WHERE50_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarExpr_in_generalCondenseExpr1225);
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

            USING51=(Token)match(input,USING,FOLLOW_USING_in_generalCondenseExpr1233); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            USING51_tree = (Object)adaptor.create(USING51);
            adaptor.addChild(root_0, USING51_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_generalCondenseExpr1237);
            ce=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ce.getTree());
            if ( state.backtracking==0 ) {
               retval.value.setUsing((ce!=null?ce.value:null)); 
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
    // wcps.g:142:1: axisIteratorList returns [AxisIteratorList value] : vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )* ;
    public final wcpsParser.axisIteratorList_return axisIteratorList() throws RecognitionException {
        wcpsParser.axisIteratorList_return retval = new wcpsParser.axisIteratorList_return();
        retval.start = input.LT(1);
        int axisIteratorList_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN52=null;
        Token RPAREN53=null;
        Token COMMA54=null;
        Token LPAREN55=null;
        Token RPAREN56=null;
        wcpsParser.variableName_return vn = null;

        wcpsParser.axisName_return an = null;

        wcpsParser.intervalExpr_return ie = null;

        wcpsParser.variableName_return vn2 = null;

        wcpsParser.axisName_return an2 = null;

        wcpsParser.intervalExpr_return ie2 = null;


        Object LPAREN52_tree=null;
        Object RPAREN53_tree=null;
        Object COMMA54_tree=null;
        Object LPAREN55_tree=null;
        Object RPAREN56_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 23) ) { return retval; }
            // wcps.g:143:2: (vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )* )
            // wcps.g:143:4: vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_variableName_in_axisIteratorList1254);
            vn=variableName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, vn.getTree());
            pushFollow(FOLLOW_axisName_in_axisIteratorList1258);
            an=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, an.getTree());
            LPAREN52=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_axisIteratorList1260); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN52_tree = (Object)adaptor.create(LPAREN52);
            adaptor.addChild(root_0, LPAREN52_tree);
            }
            pushFollow(FOLLOW_intervalExpr_in_axisIteratorList1264);
            ie=intervalExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ie.getTree());
            RPAREN53=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_axisIteratorList1266); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN53_tree = (Object)adaptor.create(RPAREN53);
            adaptor.addChild(root_0, RPAREN53_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new AxisIteratorList(new AxisIterator((vn!=null?vn.value:null), (an!=null?an.value:null), (ie!=null?ie.value:null))); 
            }
            // wcps.g:145:2: ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )*
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( (LA19_0==COMMA) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // wcps.g:145:3: COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN
            	    {
            	    COMMA54=(Token)match(input,COMMA,FOLLOW_COMMA_in_axisIteratorList1274); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA54_tree = (Object)adaptor.create(COMMA54);
            	    adaptor.addChild(root_0, COMMA54_tree);
            	    }
            	    pushFollow(FOLLOW_variableName_in_axisIteratorList1278);
            	    vn2=variableName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, vn2.getTree());
            	    pushFollow(FOLLOW_axisName_in_axisIteratorList1282);
            	    an2=axisName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, an2.getTree());
            	    LPAREN55=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_axisIteratorList1284); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    LPAREN55_tree = (Object)adaptor.create(LPAREN55);
            	    adaptor.addChild(root_0, LPAREN55_tree);
            	    }
            	    pushFollow(FOLLOW_intervalExpr_in_axisIteratorList1288);
            	    ie2=intervalExpr();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, ie2.getTree());
            	    RPAREN56=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_axisIteratorList1290); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    RPAREN56_tree = (Object)adaptor.create(RPAREN56);
            	    adaptor.addChild(root_0, RPAREN56_tree);
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
    // wcps.g:148:1: intervalExpr returns [IntervalExpr value] : (lo= indexExpr COLON hi= indexExpr | IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN );
    public final wcpsParser.intervalExpr_return intervalExpr() throws RecognitionException {
        wcpsParser.intervalExpr_return retval = new wcpsParser.intervalExpr_return();
        retval.start = input.LT(1);
        int intervalExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COLON57=null;
        Token IMAGECRSDOMAIN58=null;
        Token LPAREN59=null;
        Token COMMA60=null;
        Token RPAREN61=null;
        wcpsParser.indexExpr_return lo = null;

        wcpsParser.indexExpr_return hi = null;

        wcpsParser.coverageName_return e1 = null;

        wcpsParser.axisName_return e2 = null;


        Object COLON57_tree=null;
        Object IMAGECRSDOMAIN58_tree=null;
        Object LPAREN59_tree=null;
        Object COMMA60_tree=null;
        Object RPAREN61_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 24) ) { return retval; }
            // wcps.g:149:5: (lo= indexExpr COLON hi= indexExpr | IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN )
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
                    // wcps.g:149:7: lo= indexExpr COLON hi= indexExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_indexExpr_in_intervalExpr1314);
                    lo=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, lo.getTree());
                    COLON57=(Token)match(input,COLON,FOLLOW_COLON_in_intervalExpr1316); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON57_tree = (Object)adaptor.create(COLON57);
                    adaptor.addChild(root_0, COLON57_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_intervalExpr1320);
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
                    // wcps.g:151:7: IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    IMAGECRSDOMAIN58=(Token)match(input,IMAGECRSDOMAIN,FOLLOW_IMAGECRSDOMAIN_in_intervalExpr1335); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    IMAGECRSDOMAIN58_tree = (Object)adaptor.create(IMAGECRSDOMAIN58);
                    adaptor.addChild(root_0, IMAGECRSDOMAIN58_tree);
                    }
                    LPAREN59=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_intervalExpr1337); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN59_tree = (Object)adaptor.create(LPAREN59);
                    adaptor.addChild(root_0, LPAREN59_tree);
                    }
                    pushFollow(FOLLOW_coverageName_in_intervalExpr1341);
                    e1=coverageName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA60=(Token)match(input,COMMA,FOLLOW_COMMA_in_intervalExpr1343); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA60_tree = (Object)adaptor.create(COMMA60);
                    adaptor.addChild(root_0, COMMA60_tree);
                    }
                    pushFollow(FOLLOW_axisName_in_intervalExpr1347);
                    e2=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN61=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_intervalExpr1349); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN61_tree = (Object)adaptor.create(RPAREN61);
                    adaptor.addChild(root_0, RPAREN61_tree);
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
    // wcps.g:154:1: coverageConstantExpr returns [CoverageConstantExpr value] : COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT ;
    public final wcpsParser.coverageConstantExpr_return coverageConstantExpr() throws RecognitionException {
        wcpsParser.coverageConstantExpr_return retval = new wcpsParser.coverageConstantExpr_return();
        retval.start = input.LT(1);
        int coverageConstantExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COVERAGE62=null;
        Token OVER63=null;
        Token VALUE64=null;
        Token LIST65=null;
        Token LT66=null;
        Token GT67=null;
        wcpsParser.coverageName_return aname = null;

        wcpsParser.axisIteratorList_return iter = null;

        wcpsParser.constantList_return values = null;


        Object COVERAGE62_tree=null;
        Object OVER63_tree=null;
        Object VALUE64_tree=null;
        Object LIST65_tree=null;
        Object LT66_tree=null;
        Object GT67_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 25) ) { return retval; }
            // wcps.g:155:5: ( COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT )
            // wcps.g:155:7: COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT
            {
            root_0 = (Object)adaptor.nil();

            COVERAGE62=(Token)match(input,COVERAGE,FOLLOW_COVERAGE_in_coverageConstantExpr1375); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COVERAGE62_tree = (Object)adaptor.create(COVERAGE62);
            adaptor.addChild(root_0, COVERAGE62_tree);
            }
            pushFollow(FOLLOW_coverageName_in_coverageConstantExpr1379);
            aname=coverageName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            OVER63=(Token)match(input,OVER,FOLLOW_OVER_in_coverageConstantExpr1381); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            OVER63_tree = (Object)adaptor.create(OVER63);
            adaptor.addChild(root_0, OVER63_tree);
            }
            pushFollow(FOLLOW_axisIteratorList_in_coverageConstantExpr1385);
            iter=axisIteratorList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, iter.getTree());
            VALUE64=(Token)match(input,VALUE,FOLLOW_VALUE_in_coverageConstantExpr1387); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            VALUE64_tree = (Object)adaptor.create(VALUE64);
            adaptor.addChild(root_0, VALUE64_tree);
            }
            LIST65=(Token)match(input,LIST,FOLLOW_LIST_in_coverageConstantExpr1389); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LIST65_tree = (Object)adaptor.create(LIST65);
            adaptor.addChild(root_0, LIST65_tree);
            }
            LT66=(Token)match(input,LT,FOLLOW_LT_in_coverageConstantExpr1391); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LT66_tree = (Object)adaptor.create(LT66);
            adaptor.addChild(root_0, LT66_tree);
            }
            pushFollow(FOLLOW_constantList_in_coverageConstantExpr1395);
            values=constantList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, values.getTree());
            GT67=(Token)match(input,GT,FOLLOW_GT_in_coverageConstantExpr1397); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            GT67_tree = (Object)adaptor.create(GT67);
            adaptor.addChild(root_0, GT67_tree);
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
    // wcps.g:158:1: constantList returns [ConstantList value] : c= constant ( SEMICOLON c= constant )* ;
    public final wcpsParser.constantList_return constantList() throws RecognitionException {
        wcpsParser.constantList_return retval = new wcpsParser.constantList_return();
        retval.start = input.LT(1);
        int constantList_StartIndex = input.index();
        Object root_0 = null;

        Token SEMICOLON68=null;
        wcpsParser.constant_return c = null;


        Object SEMICOLON68_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 26) ) { return retval; }
            // wcps.g:159:5: (c= constant ( SEMICOLON c= constant )* )
            // wcps.g:159:7: c= constant ( SEMICOLON c= constant )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_constant_in_constantList1428);
            c=constant();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, c.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new ConstantList((c!=null?c.value:null)); 
            }
            // wcps.g:159:59: ( SEMICOLON c= constant )*
            loop21:
            do {
                int alt21=2;
                int LA21_0 = input.LA(1);

                if ( (LA21_0==SEMICOLON) ) {
                    alt21=1;
                }


                switch (alt21) {
            	case 1 :
            	    // wcps.g:159:60: SEMICOLON c= constant
            	    {
            	    SEMICOLON68=(Token)match(input,SEMICOLON,FOLLOW_SEMICOLON_in_constantList1433); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    SEMICOLON68_tree = (Object)adaptor.create(SEMICOLON68);
            	    adaptor.addChild(root_0, SEMICOLON68_tree);
            	    }
            	    pushFollow(FOLLOW_constant_in_constantList1437);
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
    // wcps.g:161:1: coverageConstructorExpr returns [CoverageConstructorExpr value] : COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr ;
    public final wcpsParser.coverageConstructorExpr_return coverageConstructorExpr() throws RecognitionException {
        wcpsParser.coverageConstructorExpr_return retval = new wcpsParser.coverageConstructorExpr_return();
        retval.start = input.LT(1);
        int coverageConstructorExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COVERAGE69=null;
        Token OVER70=null;
        Token VALUES71=null;
        wcpsParser.coverageName_return coverage = null;

        wcpsParser.axisIteratorList_return ail = null;

        wcpsParser.scalarExpr_return se = null;


        Object COVERAGE69_tree=null;
        Object OVER70_tree=null;
        Object VALUES71_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 27) ) { return retval; }
            // wcps.g:162:2: ( COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr )
            // wcps.g:162:4: COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr
            {
            root_0 = (Object)adaptor.nil();

            COVERAGE69=(Token)match(input,COVERAGE,FOLLOW_COVERAGE_in_coverageConstructorExpr1457); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COVERAGE69_tree = (Object)adaptor.create(COVERAGE69);
            adaptor.addChild(root_0, COVERAGE69_tree);
            }
            pushFollow(FOLLOW_coverageName_in_coverageConstructorExpr1461);
            coverage=coverageName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, coverage.getTree());
            OVER70=(Token)match(input,OVER,FOLLOW_OVER_in_coverageConstructorExpr1463); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            OVER70_tree = (Object)adaptor.create(OVER70);
            adaptor.addChild(root_0, OVER70_tree);
            }
            pushFollow(FOLLOW_axisIteratorList_in_coverageConstructorExpr1467);
            ail=axisIteratorList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, ail.getTree());
            VALUES71=(Token)match(input,VALUES,FOLLOW_VALUES_in_coverageConstructorExpr1469); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            VALUES71_tree = (Object)adaptor.create(VALUES71);
            adaptor.addChild(root_0, VALUES71_tree);
            }
            pushFollow(FOLLOW_scalarExpr_in_coverageConstructorExpr1473);
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
    // wcps.g:165:1: setMetaDataExpr returns [SetMetaDataExpr value] : (op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN | op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN | op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN | op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN | op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN );
    public final wcpsParser.setMetaDataExpr_return setMetaDataExpr() throws RecognitionException {
        wcpsParser.setMetaDataExpr_return retval = new wcpsParser.setMetaDataExpr_return();
        retval.start = input.LT(1);
        int setMetaDataExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN72=null;
        Token COMMA73=null;
        Token RPAREN74=null;
        Token LPAREN75=null;
        Token COMMA76=null;
        Token RPAREN77=null;
        Token LPAREN78=null;
        Token COMMA79=null;
        Token RPAREN80=null;
        Token LPAREN81=null;
        Token COMMA82=null;
        Token COMMA83=null;
        Token RPAREN84=null;
        Token LPAREN85=null;
        Token COMMA86=null;
        Token COMMA87=null;
        Token RPAREN88=null;
        wcpsParser.stringConstant_return s = null;

        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.crsList_return crs = null;

        wcpsParser.rangeExprList_return rel = null;

        wcpsParser.fieldName_return fn = null;

        wcpsParser.interpolationMethod_return im = null;

        wcpsParser.interpolationMethodList_return iml = null;


        Object op_tree=null;
        Object LPAREN72_tree=null;
        Object COMMA73_tree=null;
        Object RPAREN74_tree=null;
        Object LPAREN75_tree=null;
        Object COMMA76_tree=null;
        Object RPAREN77_tree=null;
        Object LPAREN78_tree=null;
        Object COMMA79_tree=null;
        Object RPAREN80_tree=null;
        Object LPAREN81_tree=null;
        Object COMMA82_tree=null;
        Object COMMA83_tree=null;
        Object RPAREN84_tree=null;
        Object LPAREN85_tree=null;
        Object COMMA86_tree=null;
        Object COMMA87_tree=null;
        Object RPAREN88_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 28) ) { return retval; }
            // wcps.g:166:5: (op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN | op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN | op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN | op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN | op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN )
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
                    // wcps.g:166:7: op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETIDENTIFIER,FOLLOW_SETIDENTIFIER_in_setMetaDataExpr1495); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN72=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1497); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN72_tree = (Object)adaptor.create(LPAREN72);
                    adaptor.addChild(root_0, LPAREN72_tree);
                    }
                    pushFollow(FOLLOW_stringConstant_in_setMetaDataExpr1501);
                    s=stringConstant();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, s.getTree());
                    COMMA73=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1503); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA73_tree = (Object)adaptor.create(COMMA73);
                    adaptor.addChild(root_0, COMMA73_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1507);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN74=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1509); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN74_tree = (Object)adaptor.create(RPAREN74);
                    adaptor.addChild(root_0, RPAREN74_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (s!=null?s.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // wcps.g:168:7: op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETCRSSET,FOLLOW_SETCRSSET_in_setMetaDataExpr1522); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN75=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1524); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN75_tree = (Object)adaptor.create(LPAREN75);
                    adaptor.addChild(root_0, LPAREN75_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1528);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA76=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1530); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA76_tree = (Object)adaptor.create(COMMA76);
                    adaptor.addChild(root_0, COMMA76_tree);
                    }
                    pushFollow(FOLLOW_crsList_in_setMetaDataExpr1534);
                    crs=crsList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    RPAREN77=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1536); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN77_tree = (Object)adaptor.create(RPAREN77);
                    adaptor.addChild(root_0, RPAREN77_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (crs!=null?crs.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // wcps.g:170:7: op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETNULLSET,FOLLOW_SETNULLSET_in_setMetaDataExpr1553); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN78=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1555); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN78_tree = (Object)adaptor.create(LPAREN78);
                    adaptor.addChild(root_0, LPAREN78_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1559);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA79=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1561); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA79_tree = (Object)adaptor.create(COMMA79);
                    adaptor.addChild(root_0, COMMA79_tree);
                    }
                    pushFollow(FOLLOW_rangeExprList_in_setMetaDataExpr1565);
                    rel=rangeExprList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, rel.getTree());
                    RPAREN80=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1567); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN80_tree = (Object)adaptor.create(RPAREN80);
                    adaptor.addChild(root_0, RPAREN80_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (rel!=null?rel.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // wcps.g:172:7: op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETINTERPOLATIONDEFAULT,FOLLOW_SETINTERPOLATIONDEFAULT_in_setMetaDataExpr1584); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN81=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1586); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN81_tree = (Object)adaptor.create(LPAREN81);
                    adaptor.addChild(root_0, LPAREN81_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1590);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA82=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1592); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA82_tree = (Object)adaptor.create(COMMA82);
                    adaptor.addChild(root_0, COMMA82_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_setMetaDataExpr1596);
                    fn=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, fn.getTree());
                    COMMA83=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1598); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA83_tree = (Object)adaptor.create(COMMA83);
                    adaptor.addChild(root_0, COMMA83_tree);
                    }
                    pushFollow(FOLLOW_interpolationMethod_in_setMetaDataExpr1602);
                    im=interpolationMethod();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, im.getTree());
                    RPAREN84=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1604); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN84_tree = (Object)adaptor.create(RPAREN84);
                    adaptor.addChild(root_0, RPAREN84_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SetMetaDataExpr((op!=null?op.getText():null), (e1!=null?e1.value:null), (im!=null?im.value:null), (fn!=null?fn.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // wcps.g:174:7: op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SETINTERPOLATIONSET,FOLLOW_SETINTERPOLATIONSET_in_setMetaDataExpr1624); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN85=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_setMetaDataExpr1626); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN85_tree = (Object)adaptor.create(LPAREN85);
                    adaptor.addChild(root_0, LPAREN85_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_setMetaDataExpr1630);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA86=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1632); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA86_tree = (Object)adaptor.create(COMMA86);
                    adaptor.addChild(root_0, COMMA86_tree);
                    }
                    pushFollow(FOLLOW_fieldName_in_setMetaDataExpr1636);
                    fn=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, fn.getTree());
                    COMMA87=(Token)match(input,COMMA,FOLLOW_COMMA_in_setMetaDataExpr1638); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA87_tree = (Object)adaptor.create(COMMA87);
                    adaptor.addChild(root_0, COMMA87_tree);
                    }
                    pushFollow(FOLLOW_interpolationMethodList_in_setMetaDataExpr1642);
                    iml=interpolationMethodList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, iml.getTree());
                    RPAREN88=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_setMetaDataExpr1644); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN88_tree = (Object)adaptor.create(RPAREN88);
                    adaptor.addChild(root_0, RPAREN88_tree);
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
    // wcps.g:177:1: crsList returns [CrsList value] : LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE ;
    public final wcpsParser.crsList_return crsList() throws RecognitionException {
        wcpsParser.crsList_return retval = new wcpsParser.crsList_return();
        retval.start = input.LT(1);
        int crsList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE89=null;
        Token COMMA90=null;
        Token RBRACE91=null;
        wcpsParser.crsName_return crs = null;


        Object LBRACE89_tree=null;
        Object COMMA90_tree=null;
        Object RBRACE91_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 29) ) { return retval; }
            // wcps.g:178:5: ( LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE )
            // wcps.g:178:7: LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE89=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_crsList1673); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE89_tree = (Object)adaptor.create(LBRACE89);
            adaptor.addChild(root_0, LBRACE89_tree);
            }
            if ( state.backtracking==0 ) {
              retval.value = new CrsList();
            }
            // wcps.g:178:40: (crs= crsName ( COMMA crs= crsName )* )?
            int alt24=2;
            int LA24_0 = input.LA(1);

            if ( (LA24_0==STRING) ) {
                alt24=1;
            }
            switch (alt24) {
                case 1 :
                    // wcps.g:178:41: crs= crsName ( COMMA crs= crsName )*
                    {
                    pushFollow(FOLLOW_crsName_in_crsList1680);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((crs!=null?crs.value:null)); 
                    }
                    // wcps.g:178:81: ( COMMA crs= crsName )*
                    loop23:
                    do {
                        int alt23=2;
                        int LA23_0 = input.LA(1);

                        if ( (LA23_0==COMMA) ) {
                            alt23=1;
                        }


                        switch (alt23) {
                    	case 1 :
                    	    // wcps.g:178:82: COMMA crs= crsName
                    	    {
                    	    COMMA90=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsList1685); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA90_tree = (Object)adaptor.create(COMMA90);
                    	    adaptor.addChild(root_0, COMMA90_tree);
                    	    }
                    	    pushFollow(FOLLOW_crsName_in_crsList1689);
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

            RBRACE91=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_crsList1698); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE91_tree = (Object)adaptor.create(RBRACE91);
            adaptor.addChild(root_0, RBRACE91_tree);
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
    // wcps.g:180:1: rangeExprList returns [RangeExprList value] : LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE ;
    public final wcpsParser.rangeExprList_return rangeExprList() throws RecognitionException {
        wcpsParser.rangeExprList_return retval = new wcpsParser.rangeExprList_return();
        retval.start = input.LT(1);
        int rangeExprList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE92=null;
        Token COMMA93=null;
        Token RBRACE94=null;
        wcpsParser.rangeExpr_return re1 = null;

        wcpsParser.rangeExpr_return re2 = null;


        Object LBRACE92_tree=null;
        Object COMMA93_tree=null;
        Object RBRACE94_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 30) ) { return retval; }
            // wcps.g:181:5: ( LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE )
            // wcps.g:181:7: LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE92=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_rangeExprList1717); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE92_tree = (Object)adaptor.create(LBRACE92);
            adaptor.addChild(root_0, LBRACE92_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new RangeExprList(); 
            }
            // wcps.g:181:48: (re1= rangeExpr ( COMMA re2= rangeExpr )* )?
            int alt26=2;
            int LA26_0 = input.LA(1);

            if ( (LA26_0==STRUCT) ) {
                alt26=1;
            }
            switch (alt26) {
                case 1 :
                    // wcps.g:181:49: re1= rangeExpr ( COMMA re2= rangeExpr )*
                    {
                    pushFollow(FOLLOW_rangeExpr_in_rangeExprList1724);
                    re1=rangeExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, re1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((re1!=null?re1.value:null)); 
                    }
                    // wcps.g:181:91: ( COMMA re2= rangeExpr )*
                    loop25:
                    do {
                        int alt25=2;
                        int LA25_0 = input.LA(1);

                        if ( (LA25_0==COMMA) ) {
                            alt25=1;
                        }


                        switch (alt25) {
                    	case 1 :
                    	    // wcps.g:181:92: COMMA re2= rangeExpr
                    	    {
                    	    COMMA93=(Token)match(input,COMMA,FOLLOW_COMMA_in_rangeExprList1729); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA93_tree = (Object)adaptor.create(COMMA93);
                    	    adaptor.addChild(root_0, COMMA93_tree);
                    	    }
                    	    pushFollow(FOLLOW_rangeExpr_in_rangeExprList1733);
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

            RBRACE94=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_rangeExprList1742); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE94_tree = (Object)adaptor.create(RBRACE94);
            adaptor.addChild(root_0, RBRACE94_tree);
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
    // wcps.g:183:1: interpolationMethodList returns [InterpolationMethodList value] : LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE ;
    public final wcpsParser.interpolationMethodList_return interpolationMethodList() throws RecognitionException {
        wcpsParser.interpolationMethodList_return retval = new wcpsParser.interpolationMethodList_return();
        retval.start = input.LT(1);
        int interpolationMethodList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE95=null;
        Token COMMA96=null;
        Token RBRACE97=null;
        wcpsParser.interpolationMethod_return e = null;


        Object LBRACE95_tree=null;
        Object COMMA96_tree=null;
        Object RBRACE97_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 31) ) { return retval; }
            // wcps.g:184:2: ( LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE )
            // wcps.g:184:4: LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE95=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_interpolationMethodList1758); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE95_tree = (Object)adaptor.create(LBRACE95);
            adaptor.addChild(root_0, LBRACE95_tree);
            }
            if ( state.backtracking==0 ) {
              retval.value = new InterpolationMethodList();
            }
            // wcps.g:184:53: (e= interpolationMethod ( COMMA e= interpolationMethod )* )?
            int alt28=2;
            int LA28_0 = input.LA(1);

            if ( (LA28_0==LPAREN) ) {
                alt28=1;
            }
            switch (alt28) {
                case 1 :
                    // wcps.g:184:54: e= interpolationMethod ( COMMA e= interpolationMethod )*
                    {
                    pushFollow(FOLLOW_interpolationMethod_in_interpolationMethodList1765);
                    e=interpolationMethod();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((e!=null?e.value:null)); 
                    }
                    // wcps.g:184:102: ( COMMA e= interpolationMethod )*
                    loop27:
                    do {
                        int alt27=2;
                        int LA27_0 = input.LA(1);

                        if ( (LA27_0==COMMA) ) {
                            alt27=1;
                        }


                        switch (alt27) {
                    	case 1 :
                    	    // wcps.g:184:103: COMMA e= interpolationMethod
                    	    {
                    	    COMMA96=(Token)match(input,COMMA,FOLLOW_COMMA_in_interpolationMethodList1770); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA96_tree = (Object)adaptor.create(COMMA96);
                    	    adaptor.addChild(root_0, COMMA96_tree);
                    	    }
                    	    pushFollow(FOLLOW_interpolationMethod_in_interpolationMethodList1774);
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

            RBRACE97=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_interpolationMethodList1782); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE97_tree = (Object)adaptor.create(RBRACE97);
            adaptor.addChild(root_0, RBRACE97_tree);
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
    // wcps.g:186:1: rangeExpr returns [RangeExpr value] : STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COMMA field= fieldName COLON expr= scalarExpr )* )? RBRACE ;
    public final wcpsParser.rangeExpr_return rangeExpr() throws RecognitionException {
        wcpsParser.rangeExpr_return retval = new wcpsParser.rangeExpr_return();
        retval.start = input.LT(1);
        int rangeExpr_StartIndex = input.index();
        Object root_0 = null;

        Token STRUCT98=null;
        Token LBRACE99=null;
        Token COLON100=null;
        Token COMMA101=null;
        Token COLON102=null;
        Token RBRACE103=null;
        wcpsParser.fieldName_return field = null;

        wcpsParser.scalarExpr_return expr = null;


        Object STRUCT98_tree=null;
        Object LBRACE99_tree=null;
        Object COLON100_tree=null;
        Object COMMA101_tree=null;
        Object COLON102_tree=null;
        Object RBRACE103_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 32) ) { return retval; }
            // wcps.g:187:2: ( STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COMMA field= fieldName COLON expr= scalarExpr )* )? RBRACE )
            // wcps.g:187:4: STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COMMA field= fieldName COLON expr= scalarExpr )* )? RBRACE
            {
            root_0 = (Object)adaptor.nil();

            STRUCT98=(Token)match(input,STRUCT,FOLLOW_STRUCT_in_rangeExpr1795); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            STRUCT98_tree = (Object)adaptor.create(STRUCT98);
            adaptor.addChild(root_0, STRUCT98_tree);
            }
            LBRACE99=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_rangeExpr1797); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE99_tree = (Object)adaptor.create(LBRACE99);
            adaptor.addChild(root_0, LBRACE99_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value =new RangeExpr(); 
            }
            // wcps.g:188:2: (field= fieldName COLON expr= scalarExpr ( COMMA field= fieldName COLON expr= scalarExpr )* )?
            int alt30=2;
            int LA30_0 = input.LA(1);

            if ( (LA30_0==INTEGERCONSTANT||LA30_0==STRING||LA30_0==NAME) ) {
                alt30=1;
            }
            switch (alt30) {
                case 1 :
                    // wcps.g:188:3: field= fieldName COLON expr= scalarExpr ( COMMA field= fieldName COLON expr= scalarExpr )*
                    {
                    pushFollow(FOLLOW_fieldName_in_rangeExpr1805);
                    field=fieldName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
                    COLON100=(Token)match(input,COLON,FOLLOW_COLON_in_rangeExpr1807); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON100_tree = (Object)adaptor.create(COLON100);
                    adaptor.addChild(root_0, COLON100_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_rangeExpr1811);
                    expr=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, expr.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((field!=null?field.value:null), (expr!=null?expr.value:null)); 
                    }
                    // wcps.g:189:3: ( COMMA field= fieldName COLON expr= scalarExpr )*
                    loop29:
                    do {
                        int alt29=2;
                        int LA29_0 = input.LA(1);

                        if ( (LA29_0==COMMA) ) {
                            alt29=1;
                        }


                        switch (alt29) {
                    	case 1 :
                    	    // wcps.g:189:4: COMMA field= fieldName COLON expr= scalarExpr
                    	    {
                    	    COMMA101=(Token)match(input,COMMA,FOLLOW_COMMA_in_rangeExpr1818); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COMMA101_tree = (Object)adaptor.create(COMMA101);
                    	    adaptor.addChild(root_0, COMMA101_tree);
                    	    }
                    	    pushFollow(FOLLOW_fieldName_in_rangeExpr1822);
                    	    field=fieldName();

                    	    state._fsp--;
                    	    if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
                    	    COLON102=(Token)match(input,COLON,FOLLOW_COLON_in_rangeExpr1824); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COLON102_tree = (Object)adaptor.create(COLON102);
                    	    adaptor.addChild(root_0, COLON102_tree);
                    	    }
                    	    pushFollow(FOLLOW_scalarExpr_in_rangeExpr1828);
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

            RBRACE103=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_rangeExpr1839); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE103_tree = (Object)adaptor.create(RBRACE103);
            adaptor.addChild(root_0, RBRACE103_tree);
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
    // wcps.g:192:1: rangeConstructorExpr returns [RangeConstructorExpr value] : ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE ;
    public final wcpsParser.rangeConstructorExpr_return rangeConstructorExpr() throws RecognitionException {
        wcpsParser.rangeConstructorExpr_return retval = new wcpsParser.rangeConstructorExpr_return();
        retval.start = input.LT(1);
        int rangeConstructorExpr_StartIndex = input.index();
        Object root_0 = null;

        Token STRUCT104=null;
        Token LBRACE105=null;
        Token COLON106=null;
        Token SEMICOLON107=null;
        Token COLON108=null;
        Token RBRACE109=null;
        wcpsParser.fieldName_return field = null;

        wcpsParser.coverageExpr_return expr = null;


        Object STRUCT104_tree=null;
        Object LBRACE105_tree=null;
        Object COLON106_tree=null;
        Object SEMICOLON107_tree=null;
        Object COLON108_tree=null;
        Object RBRACE109_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 33) ) { return retval; }
            // wcps.g:193:5: ( ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE )
            // wcps.g:193:7: ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            // wcps.g:193:7: ( STRUCT )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==STRUCT) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // wcps.g:193:8: STRUCT
                    {
                    STRUCT104=(Token)match(input,STRUCT,FOLLOW_STRUCT_in_rangeConstructorExpr1856); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    STRUCT104_tree = (Object)adaptor.create(STRUCT104);
                    adaptor.addChild(root_0, STRUCT104_tree);
                    }

                    }
                    break;

            }

            LBRACE105=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_rangeConstructorExpr1860); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE105_tree = (Object)adaptor.create(LBRACE105);
            adaptor.addChild(root_0, LBRACE105_tree);
            }
            pushFollow(FOLLOW_fieldName_in_rangeConstructorExpr1864);
            field=fieldName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
            COLON106=(Token)match(input,COLON,FOLLOW_COLON_in_rangeConstructorExpr1866); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COLON106_tree = (Object)adaptor.create(COLON106);
            adaptor.addChild(root_0, COLON106_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_rangeConstructorExpr1870);
            expr=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, expr.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new RangeConstructorExpr((field!=null?field.value:null), (expr!=null?expr.value:null)); 
            }
            // wcps.g:194:9: ( SEMICOLON field= fieldName COLON expr= coverageExpr )*
            loop32:
            do {
                int alt32=2;
                int LA32_0 = input.LA(1);

                if ( (LA32_0==SEMICOLON) ) {
                    alt32=1;
                }


                switch (alt32) {
            	case 1 :
            	    // wcps.g:194:10: SEMICOLON field= fieldName COLON expr= coverageExpr
            	    {
            	    SEMICOLON107=(Token)match(input,SEMICOLON,FOLLOW_SEMICOLON_in_rangeConstructorExpr1883); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    SEMICOLON107_tree = (Object)adaptor.create(SEMICOLON107);
            	    adaptor.addChild(root_0, SEMICOLON107_tree);
            	    }
            	    pushFollow(FOLLOW_fieldName_in_rangeConstructorExpr1887);
            	    field=fieldName();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, field.getTree());
            	    COLON108=(Token)match(input,COLON,FOLLOW_COLON_in_rangeConstructorExpr1889); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COLON108_tree = (Object)adaptor.create(COLON108);
            	    adaptor.addChild(root_0, COLON108_tree);
            	    }
            	    pushFollow(FOLLOW_coverageExpr_in_rangeConstructorExpr1893);
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

            RBRACE109=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_rangeConstructorExpr1899); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE109_tree = (Object)adaptor.create(RBRACE109);
            adaptor.addChild(root_0, RBRACE109_tree);
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
    // wcps.g:196:1: crsTransformExpr returns [CrsTransformExpr value] : CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionCrsList COMMA fil= fieldInterpolationList RPAREN ;
    public final wcpsParser.crsTransformExpr_return crsTransformExpr() throws RecognitionException {
        wcpsParser.crsTransformExpr_return retval = new wcpsParser.crsTransformExpr_return();
        retval.start = input.LT(1);
        int crsTransformExpr_StartIndex = input.index();
        Object root_0 = null;

        Token CRSTRANSFORM110=null;
        Token LPAREN111=null;
        Token COMMA112=null;
        Token COMMA113=null;
        Token RPAREN114=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionCrsList_return dcl = null;

        wcpsParser.fieldInterpolationList_return fil = null;


        Object CRSTRANSFORM110_tree=null;
        Object LPAREN111_tree=null;
        Object COMMA112_tree=null;
        Object COMMA113_tree=null;
        Object RPAREN114_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 34) ) { return retval; }
            // wcps.g:197:9: ( CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionCrsList COMMA fil= fieldInterpolationList RPAREN )
            // wcps.g:197:11: CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionCrsList COMMA fil= fieldInterpolationList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            CRSTRANSFORM110=(Token)match(input,CRSTRANSFORM,FOLLOW_CRSTRANSFORM_in_crsTransformExpr1922); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            CRSTRANSFORM110_tree = (Object)adaptor.create(CRSTRANSFORM110);
            adaptor.addChild(root_0, CRSTRANSFORM110_tree);
            }
            LPAREN111=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_crsTransformExpr1924); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN111_tree = (Object)adaptor.create(LPAREN111);
            adaptor.addChild(root_0, LPAREN111_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_crsTransformExpr1928);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA112=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsTransformExpr1930); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA112_tree = (Object)adaptor.create(COMMA112);
            adaptor.addChild(root_0, COMMA112_tree);
            }
            pushFollow(FOLLOW_dimensionCrsList_in_crsTransformExpr1934);
            dcl=dimensionCrsList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dcl.getTree());
            COMMA113=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsTransformExpr1936); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA113_tree = (Object)adaptor.create(COMMA113);
            adaptor.addChild(root_0, COMMA113_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationList_in_crsTransformExpr1940);
            fil=fieldInterpolationList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, fil.getTree());
            RPAREN114=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_crsTransformExpr1942); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN114_tree = (Object)adaptor.create(RPAREN114);
            adaptor.addChild(root_0, RPAREN114_tree);
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

    public static class dimensionCrsList_return extends ParserRuleReturnScope {
        public DimensionCrsList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionCrsList"
    // wcps.g:200:1: dimensionCrsList returns [DimensionCrsList value] : LBRACE elem= dimensionCrsElement ( COMMA elem= dimensionCrsElement )* RBRACE ;
    public final wcpsParser.dimensionCrsList_return dimensionCrsList() throws RecognitionException {
        wcpsParser.dimensionCrsList_return retval = new wcpsParser.dimensionCrsList_return();
        retval.start = input.LT(1);
        int dimensionCrsList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE115=null;
        Token COMMA116=null;
        Token RBRACE117=null;
        wcpsParser.dimensionCrsElement_return elem = null;


        Object LBRACE115_tree=null;
        Object COMMA116_tree=null;
        Object RBRACE117_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 35) ) { return retval; }
            // wcps.g:201:5: ( LBRACE elem= dimensionCrsElement ( COMMA elem= dimensionCrsElement )* RBRACE )
            // wcps.g:201:7: LBRACE elem= dimensionCrsElement ( COMMA elem= dimensionCrsElement )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE115=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_dimensionCrsList1983); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE115_tree = (Object)adaptor.create(LBRACE115);
            adaptor.addChild(root_0, LBRACE115_tree);
            }
            pushFollow(FOLLOW_dimensionCrsElement_in_dimensionCrsList1987);
            elem=dimensionCrsElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionCrsList((elem!=null?elem.value:null)); 
            }
            // wcps.g:202:9: ( COMMA elem= dimensionCrsElement )*
            loop33:
            do {
                int alt33=2;
                int LA33_0 = input.LA(1);

                if ( (LA33_0==COMMA) ) {
                    alt33=1;
                }


                switch (alt33) {
            	case 1 :
            	    // wcps.g:202:10: COMMA elem= dimensionCrsElement
            	    {
            	    COMMA116=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionCrsList2000); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA116_tree = (Object)adaptor.create(COMMA116);
            	    adaptor.addChild(root_0, COMMA116_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionCrsElement_in_dimensionCrsList2004);
            	    elem=dimensionCrsElement();

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

            RBRACE117=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_dimensionCrsList2011); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE117_tree = (Object)adaptor.create(RBRACE117);
            adaptor.addChild(root_0, RBRACE117_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 35, dimensionCrsList_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionCrsList"

    public static class dimensionCrsElement_return extends ParserRuleReturnScope {
        public DimensionCrsElement value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "dimensionCrsElement"
    // wcps.g:204:1: dimensionCrsElement returns [DimensionCrsElement value] : aname= axisName COLON crs= crsName ;
    public final wcpsParser.dimensionCrsElement_return dimensionCrsElement() throws RecognitionException {
        wcpsParser.dimensionCrsElement_return retval = new wcpsParser.dimensionCrsElement_return();
        retval.start = input.LT(1);
        int dimensionCrsElement_StartIndex = input.index();
        Object root_0 = null;

        Token COLON118=null;
        wcpsParser.axisName_return aname = null;

        wcpsParser.crsName_return crs = null;


        Object COLON118_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 36) ) { return retval; }
            // wcps.g:205:5: (aname= axisName COLON crs= crsName )
            // wcps.g:205:7: aname= axisName COLON crs= crsName
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_axisName_in_dimensionCrsElement2032);
            aname=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionCrsElement((aname!=null?aname.value:null)); 
            }
            COLON118=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionCrsElement2036); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COLON118_tree = (Object)adaptor.create(COLON118);
            adaptor.addChild(root_0, COLON118_tree);
            }
            pushFollow(FOLLOW_crsName_in_dimensionCrsElement2040);
            crs=crsName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
            if ( state.backtracking==0 ) {
              retval.value.setCrs((crs!=null?crs.value:null)); 
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
            if ( state.backtracking>0 ) { memoize(input, 36, dimensionCrsElement_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "dimensionCrsElement"

    public static class fieldInterpolationList_return extends ParserRuleReturnScope {
        public FieldInterpolationList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "fieldInterpolationList"
    // wcps.g:207:1: fieldInterpolationList returns [FieldInterpolationList value] : LBRACE (elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement ) )* RBRACE ;
    public final wcpsParser.fieldInterpolationList_return fieldInterpolationList() throws RecognitionException {
        wcpsParser.fieldInterpolationList_return retval = new wcpsParser.fieldInterpolationList_return();
        retval.start = input.LT(1);
        int fieldInterpolationList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE119=null;
        Token COMMA120=null;
        Token RBRACE121=null;
        wcpsParser.fieldInterpolationElement_return elem = null;


        Object LBRACE119_tree=null;
        Object COMMA120_tree=null;
        Object RBRACE121_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 37) ) { return retval; }
            // wcps.g:208:2: ( LBRACE (elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement ) )* RBRACE )
            // wcps.g:208:4: LBRACE (elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement ) )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE119=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_fieldInterpolationList2058); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE119_tree = (Object)adaptor.create(LBRACE119);
            adaptor.addChild(root_0, LBRACE119_tree);
            }
            // wcps.g:208:11: (elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement ) )*
            loop34:
            do {
                int alt34=2;
                int LA34_0 = input.LA(1);

                if ( (LA34_0==INTEGERCONSTANT||LA34_0==STRING||LA34_0==NAME) ) {
                    alt34=1;
                }


                switch (alt34) {
            	case 1 :
            	    // wcps.g:208:12: elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )
            	    {
            	    pushFollow(FOLLOW_fieldInterpolationElement_in_fieldInterpolationList2063);
            	    elem=fieldInterpolationElement();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value = new FieldInterpolationList((elem!=null?elem.value:null)); 
            	    }
            	    // wcps.g:209:3: ( COMMA elem= fieldInterpolationElement )
            	    // wcps.g:209:4: COMMA elem= fieldInterpolationElement
            	    {
            	    COMMA120=(Token)match(input,COMMA,FOLLOW_COMMA_in_fieldInterpolationList2070); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA120_tree = (Object)adaptor.create(COMMA120);
            	    adaptor.addChild(root_0, COMMA120_tree);
            	    }
            	    pushFollow(FOLLOW_fieldInterpolationElement_in_fieldInterpolationList2074);
            	    elem=fieldInterpolationElement();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            	    if ( state.backtracking==0 ) {
            	       retval.value.add((elem!=null?elem.value:null)); 
            	    }

            	    }


            	    }
            	    break;

            	default :
            	    break loop34;
                }
            } while (true);

            RBRACE121=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_fieldInterpolationList2082); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE121_tree = (Object)adaptor.create(RBRACE121);
            adaptor.addChild(root_0, RBRACE121_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 37, fieldInterpolationList_StartIndex); }
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
    // wcps.g:211:1: fieldInterpolationElement returns [FieldInterpolationElement value] : aname= fieldName method= interpolationMethod ;
    public final wcpsParser.fieldInterpolationElement_return fieldInterpolationElement() throws RecognitionException {
        wcpsParser.fieldInterpolationElement_return retval = new wcpsParser.fieldInterpolationElement_return();
        retval.start = input.LT(1);
        int fieldInterpolationElement_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.fieldName_return aname = null;

        wcpsParser.interpolationMethod_return method = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 38) ) { return retval; }
            // wcps.g:212:2: (aname= fieldName method= interpolationMethod )
            // wcps.g:212:4: aname= fieldName method= interpolationMethod
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_fieldName_in_fieldInterpolationElement2097);
            aname=fieldName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            pushFollow(FOLLOW_interpolationMethod_in_fieldInterpolationElement2101);
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
            if ( state.backtracking>0 ) { memoize(input, 38, fieldInterpolationElement_StartIndex); }
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
    // wcps.g:214:1: unaryInducedExpr returns [CoverageExpr value] : (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 39) ) { return retval; }
            // wcps.g:215:5: (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr )
            int alt35=7;
            alt35 = dfa35.predict(input);
            switch (alt35) {
                case 1 :
                    // wcps.g:215:7: e6= fieldExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_fieldExpr_in_unaryInducedExpr2121);
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
                    // wcps.g:216:4: e1= unaryArithmeticExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_unaryArithmeticExpr_in_unaryInducedExpr2130);
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
                    // wcps.g:217:7: e2= exponentialExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_exponentialExpr_in_unaryInducedExpr2142);
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
                    // wcps.g:218:7: e3= trigonometricExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_trigonometricExpr_in_unaryInducedExpr2154);
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
                    // wcps.g:219:7: e4= booleanExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanExpr_in_unaryInducedExpr2166);
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
                    // wcps.g:220:7: e5= castExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_castExpr_in_unaryInducedExpr2178);
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
                    // wcps.g:221:7: e7= rangeConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_rangeConstructorExpr_in_unaryInducedExpr2190);
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
            if ( state.backtracking>0 ) { memoize(input, 39, unaryInducedExpr_StartIndex); }
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
    // wcps.g:223:1: unaryArithmeticExpr returns [CoverageExpr value] : (op= ( MINUS | PLUS ) e1= coverageAtom | op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN );
    public final wcpsParser.unaryArithmeticExpr_return unaryArithmeticExpr() throws RecognitionException {
        wcpsParser.unaryArithmeticExpr_return retval = new wcpsParser.unaryArithmeticExpr_return();
        retval.start = input.LT(1);
        int unaryArithmeticExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN122=null;
        Token RPAREN123=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object op_tree=null;
        Object LPAREN122_tree=null;
        Object RPAREN123_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 40) ) { return retval; }
            // wcps.g:224:5: (op= ( MINUS | PLUS ) e1= coverageAtom | op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN )
            int alt36=2;
            int LA36_0 = input.LA(1);

            if ( ((LA36_0>=PLUS && LA36_0<=MINUS)) ) {
                alt36=1;
            }
            else if ( ((LA36_0>=SQRT && LA36_0<=IM)) ) {
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
                    // wcps.g:224:7: op= ( MINUS | PLUS ) e1= coverageAtom
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

                    pushFollow(FOLLOW_coverageAtom_in_unaryArithmeticExpr2221);
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
                    // wcps.g:225:7: op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN
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

                    LPAREN122=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_unaryArithmeticExpr2243); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN122_tree = (Object)adaptor.create(LPAREN122);
                    adaptor.addChild(root_0, LPAREN122_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_unaryArithmeticExpr2247);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN123=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_unaryArithmeticExpr2249); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN123_tree = (Object)adaptor.create(RPAREN123);
                    adaptor.addChild(root_0, RPAREN123_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 40, unaryArithmeticExpr_StartIndex); }
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
    // wcps.g:227:1: exponentialExpr returns [ExponentialExpr value] : op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.exponentialExpr_return exponentialExpr() throws RecognitionException {
        wcpsParser.exponentialExpr_return retval = new wcpsParser.exponentialExpr_return();
        retval.start = input.LT(1);
        int exponentialExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN124=null;
        Token RPAREN125=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN124_tree=null;
        Object RPAREN125_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 41) ) { return retval; }
            // wcps.g:228:5: (op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN )
            // wcps.g:228:7: op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN
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

            LPAREN124=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_exponentialExpr2280); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN124_tree = (Object)adaptor.create(LPAREN124);
            adaptor.addChild(root_0, LPAREN124_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_exponentialExpr2284);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN125=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_exponentialExpr2286); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN125_tree = (Object)adaptor.create(RPAREN125);
            adaptor.addChild(root_0, RPAREN125_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 41, exponentialExpr_StartIndex); }
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
    // wcps.g:230:1: trigonometricExpr returns [TrigonometricExpr value] : op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.trigonometricExpr_return trigonometricExpr() throws RecognitionException {
        wcpsParser.trigonometricExpr_return retval = new wcpsParser.trigonometricExpr_return();
        retval.start = input.LT(1);
        int trigonometricExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN126=null;
        Token RPAREN127=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN126_tree=null;
        Object RPAREN127_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 42) ) { return retval; }
            // wcps.g:231:5: (op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN )
            // wcps.g:231:7: op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN
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

            LPAREN126=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_trigonometricExpr2329); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN126_tree = (Object)adaptor.create(LPAREN126);
            adaptor.addChild(root_0, LPAREN126_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_trigonometricExpr2333);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN127=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_trigonometricExpr2335); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN127_tree = (Object)adaptor.create(RPAREN127);
            adaptor.addChild(root_0, RPAREN127_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 42, trigonometricExpr_StartIndex); }
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
    // wcps.g:233:1: booleanExpr returns [BooleanExpr value] : (op= NOT e1= coverageExpr | op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN );
    public final wcpsParser.booleanExpr_return booleanExpr() throws RecognitionException {
        wcpsParser.booleanExpr_return retval = new wcpsParser.booleanExpr_return();
        retval.start = input.LT(1);
        int booleanExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN128=null;
        Token COMMA129=null;
        Token RPAREN130=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.indexExpr_return e2 = null;


        Object op_tree=null;
        Object LPAREN128_tree=null;
        Object COMMA129_tree=null;
        Object RPAREN130_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 43) ) { return retval; }
            // wcps.g:234:5: (op= NOT e1= coverageExpr | op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN )
            int alt37=2;
            int LA37_0 = input.LA(1);

            if ( (LA37_0==NOT) ) {
                alt37=1;
            }
            else if ( (LA37_0==BIT) ) {
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
                    // wcps.g:234:7: op= NOT e1= coverageExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NOT,FOLLOW_NOT_in_booleanExpr2358); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_booleanExpr2362);
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
                    // wcps.g:235:7: op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,BIT,FOLLOW_BIT_in_booleanExpr2374); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN128=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_booleanExpr2376); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN128_tree = (Object)adaptor.create(LPAREN128);
                    adaptor.addChild(root_0, LPAREN128_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_booleanExpr2380);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA129=(Token)match(input,COMMA,FOLLOW_COMMA_in_booleanExpr2382); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA129_tree = (Object)adaptor.create(COMMA129);
                    adaptor.addChild(root_0, COMMA129_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_booleanExpr2386);
                    e2=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN130=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_booleanExpr2388); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN130_tree = (Object)adaptor.create(RPAREN130);
                    adaptor.addChild(root_0, RPAREN130_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 43, booleanExpr_StartIndex); }
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
    // wcps.g:237:1: indexExpr returns [IndexExpr value] : e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )* ;
    public final wcpsParser.indexExpr_return indexExpr() throws RecognitionException {
        wcpsParser.indexExpr_return retval = new wcpsParser.indexExpr_return();
        retval.start = input.LT(1);
        int indexExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.indexTerm_return e1 = null;

        wcpsParser.indexTerm_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 44) ) { return retval; }
            // wcps.g:238:5: (e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )* )
            // wcps.g:238:7: e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_indexTerm_in_indexExpr2411);
            e1=indexTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:239:3: (op= ( PLUS | MINUS ) e2= indexTerm )*
            loop38:
            do {
                int alt38=2;
                int LA38_0 = input.LA(1);

                if ( ((LA38_0>=PLUS && LA38_0<=MINUS)) ) {
                    alt38=1;
                }


                switch (alt38) {
            	case 1 :
            	    // wcps.g:239:4: op= ( PLUS | MINUS ) e2= indexTerm
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

            	    pushFollow(FOLLOW_indexTerm_in_indexExpr2429);
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
            if ( state.backtracking>0 ) { memoize(input, 44, indexExpr_StartIndex); }
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
    // wcps.g:241:1: indexTerm returns [IndexExpr value] : e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 45) ) { return retval; }
            // wcps.g:242:5: (e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )* )
            // wcps.g:242:7: e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_indexFactor_in_indexTerm2454);
            e1=indexFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:243:6: ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )*
            loop39:
            do {
                int alt39=2;
                int LA39_0 = input.LA(1);

                if ( ((LA39_0>=MULT && LA39_0<=DIVIDE)) ) {
                    alt39=1;
                }


                switch (alt39) {
            	case 1 :
            	    // wcps.g:243:7: (op= ( MULT | DIVIDE ) e2= indexFactor )
            	    {
            	    // wcps.g:243:7: (op= ( MULT | DIVIDE ) e2= indexFactor )
            	    // wcps.g:243:8: op= ( MULT | DIVIDE ) e2= indexFactor
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

            	    pushFollow(FOLLOW_indexFactor_in_indexTerm2475);
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
            if ( state.backtracking>0 ) { memoize(input, 45, indexTerm_StartIndex); }
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
    // wcps.g:245:1: indexFactor returns [IndexExpr value] : (e= INTEGERCONSTANT | op= ROUND LPAREN e1= numericScalarExpr RPAREN | ( LPAREN e2= indexExpr RPAREN ) );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 46) ) { return retval; }
            // wcps.g:246:5: (e= INTEGERCONSTANT | op= ROUND LPAREN e1= numericScalarExpr RPAREN | ( LPAREN e2= indexExpr RPAREN ) )
            int alt40=3;
            switch ( input.LA(1) ) {
            case INTEGERCONSTANT:
                {
                alt40=1;
                }
                break;
            case ROUND:
                {
                alt40=2;
                }
                break;
            case LPAREN:
                {
                alt40=3;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 40, 0, input);

                throw nvae;
            }

            switch (alt40) {
                case 1 :
                    // wcps.g:246:7: e= INTEGERCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,INTEGERCONSTANT,FOLLOW_INTEGERCONSTANT_in_indexFactor2502); if (state.failed) return retval;
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
                    // wcps.g:247:7: op= ROUND LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ROUND,FOLLOW_ROUND_in_indexFactor2515); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN131=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_indexFactor2517); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN131_tree = (Object)adaptor.create(LPAREN131);
                    adaptor.addChild(root_0, LPAREN131_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_indexFactor2521);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN132=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_indexFactor2523); if (state.failed) return retval;
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
                    // wcps.g:248:7: ( LPAREN e2= indexExpr RPAREN )
                    {
                    root_0 = (Object)adaptor.nil();

                    // wcps.g:248:7: ( LPAREN e2= indexExpr RPAREN )
                    // wcps.g:248:8: LPAREN e2= indexExpr RPAREN
                    {
                    LPAREN133=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_indexFactor2535); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN133_tree = (Object)adaptor.create(LPAREN133);
                    adaptor.addChild(root_0, LPAREN133_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_indexFactor2539);
                    e2=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN134=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_indexFactor2541); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 46, indexFactor_StartIndex); }
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
    // wcps.g:250:1: stringScalarExpr returns [StringScalarExpr value] : (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | e= STRING );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 47) ) { return retval; }
            // wcps.g:252:5: (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | e= STRING )
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
                    // wcps.g:252:7: op= IDENTIFIER LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IDENTIFIER,FOLLOW_IDENTIFIER_in_stringScalarExpr2568); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN135=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_stringScalarExpr2570); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN135_tree = (Object)adaptor.create(LPAREN135);
                    adaptor.addChild(root_0, LPAREN135_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_stringScalarExpr2574);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN136=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_stringScalarExpr2576); if (state.failed) return retval;
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
                    // wcps.g:253:7: e= STRING
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,STRING,FOLLOW_STRING_in_stringScalarExpr2589); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 47, stringScalarExpr_StartIndex); }
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
    // wcps.g:255:1: scaleExpr returns [ScaleExpr value] : SCALE LPAREN e1= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE ( COMMA fil= fieldInterpolationList )? RPAREN ;
    public final wcpsParser.scaleExpr_return scaleExpr() throws RecognitionException {
        wcpsParser.scaleExpr_return retval = new wcpsParser.scaleExpr_return();
        retval.start = input.LT(1);
        int scaleExpr_StartIndex = input.index();
        Object root_0 = null;

        Token SCALE137=null;
        Token LPAREN138=null;
        Token COMMA139=null;
        Token LBRACE140=null;
        Token RBRACE141=null;
        Token COMMA142=null;
        Token RPAREN143=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;

        wcpsParser.fieldInterpolationList_return fil = null;


        Object SCALE137_tree=null;
        Object LPAREN138_tree=null;
        Object COMMA139_tree=null;
        Object LBRACE140_tree=null;
        Object RBRACE141_tree=null;
        Object COMMA142_tree=null;
        Object RPAREN143_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 48) ) { return retval; }
            // wcps.g:256:2: ( SCALE LPAREN e1= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE ( COMMA fil= fieldInterpolationList )? RPAREN )
            // wcps.g:256:4: SCALE LPAREN e1= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE ( COMMA fil= fieldInterpolationList )? RPAREN
            {
            root_0 = (Object)adaptor.nil();

            SCALE137=(Token)match(input,SCALE,FOLLOW_SCALE_in_scaleExpr2607); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            SCALE137_tree = (Object)adaptor.create(SCALE137);
            adaptor.addChild(root_0, SCALE137_tree);
            }
            LPAREN138=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_scaleExpr2609); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN138_tree = (Object)adaptor.create(LPAREN138);
            adaptor.addChild(root_0, LPAREN138_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_scaleExpr2613);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA139=(Token)match(input,COMMA,FOLLOW_COMMA_in_scaleExpr2615); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA139_tree = (Object)adaptor.create(COMMA139);
            adaptor.addChild(root_0, COMMA139_tree);
            }
            LBRACE140=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_scaleExpr2617); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE140_tree = (Object)adaptor.create(LBRACE140);
            adaptor.addChild(root_0, LBRACE140_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_scaleExpr2621);
            dil=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
            RBRACE141=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_scaleExpr2623); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RBRACE141_tree = (Object)adaptor.create(RBRACE141);
            adaptor.addChild(root_0, RBRACE141_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new ScaleExpr((e1!=null?e1.value:null), (dil!=null?dil.value:null)); 
            }
            // wcps.g:257:13: ( COMMA fil= fieldInterpolationList )?
            int alt42=2;
            int LA42_0 = input.LA(1);

            if ( (LA42_0==COMMA) ) {
                alt42=1;
            }
            switch (alt42) {
                case 1 :
                    // wcps.g:257:14: COMMA fil= fieldInterpolationList
                    {
                    COMMA142=(Token)match(input,COMMA,FOLLOW_COMMA_in_scaleExpr2640); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA142_tree = (Object)adaptor.create(COMMA142);
                    adaptor.addChild(root_0, COMMA142_tree);
                    }
                    pushFollow(FOLLOW_fieldInterpolationList_in_scaleExpr2644);
                    fil=fieldInterpolationList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, fil.getTree());
                    if ( state.backtracking==0 ) {
                      retval.value.addInterpolationList((fil!=null?fil.value:null)); 
                    }

                    }
                    break;

            }

            RPAREN143=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_scaleExpr2669); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN143_tree = (Object)adaptor.create(RPAREN143);
            adaptor.addChild(root_0, RPAREN143_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 48, scaleExpr_StartIndex); }
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
    // wcps.g:260:1: subsetExpr returns [SubsetExpr value] : (e1= trimExpr | e2= sliceExpr | e3= extendExpr );
    public final wcpsParser.subsetExpr_return subsetExpr() throws RecognitionException {
        wcpsParser.subsetExpr_return retval = new wcpsParser.subsetExpr_return();
        retval.start = input.LT(1);
        int subsetExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.trimExpr_return e1 = null;

        wcpsParser.sliceExpr_return e2 = null;

        wcpsParser.extendExpr_return e3 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 49) ) { return retval; }
            // wcps.g:261:2: (e1= trimExpr | e2= sliceExpr | e3= extendExpr )
            int alt43=3;
            alt43 = dfa43.predict(input);
            switch (alt43) {
                case 1 :
                    // wcps.g:261:4: e1= trimExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_trimExpr_in_subsetExpr2686);
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
                    // wcps.g:262:4: e2= sliceExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_sliceExpr_in_subsetExpr2695);
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
                    // wcps.g:263:4: e3= extendExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_extendExpr_in_subsetExpr2704);
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
            if ( state.backtracking>0 ) { memoize(input, 49, subsetExpr_StartIndex); }
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
    // wcps.g:265:1: trimExpr returns [TrimExpr value] : (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN );
    public final wcpsParser.trimExpr_return trimExpr() throws RecognitionException {
        wcpsParser.trimExpr_return retval = new wcpsParser.trimExpr_return();
        retval.start = input.LT(1);
        int trimExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACKET144=null;
        Token RBRACKET145=null;
        Token TRIM146=null;
        Token LPAREN147=null;
        Token COMMA148=null;
        Token LBRACE149=null;
        Token RBRACE150=null;
        Token RPAREN151=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LBRACKET144_tree=null;
        Object RBRACKET145_tree=null;
        Object TRIM146_tree=null;
        Object LPAREN147_tree=null;
        Object COMMA148_tree=null;
        Object LBRACE149_tree=null;
        Object RBRACE150_tree=null;
        Object RPAREN151_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 50) ) { return retval; }
            // wcps.g:266:2: (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN )
            int alt44=2;
            alt44 = dfa44.predict(input);
            switch (alt44) {
                case 1 :
                    // wcps.g:266:4: e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_trimExpr2721);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    LBRACKET144=(Token)match(input,LBRACKET,FOLLOW_LBRACKET_in_trimExpr2723); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACKET144_tree = (Object)adaptor.create(LBRACKET144);
                    adaptor.addChild(root_0, LBRACKET144_tree);
                    }
                    pushFollow(FOLLOW_dimensionIntervalList_in_trimExpr2727);
                    dil=dimensionIntervalList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
                    RBRACKET145=(Token)match(input,RBRACKET,FOLLOW_RBRACKET_in_trimExpr2729); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACKET145_tree = (Object)adaptor.create(RBRACKET145);
                    adaptor.addChild(root_0, RBRACKET145_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new TrimExpr((e1!=null?e1.value:null), (dil!=null?dil.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // wcps.g:267:6: TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    TRIM146=(Token)match(input,TRIM,FOLLOW_TRIM_in_trimExpr2738); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    TRIM146_tree = (Object)adaptor.create(TRIM146);
                    adaptor.addChild(root_0, TRIM146_tree);
                    }
                    LPAREN147=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_trimExpr2740); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN147_tree = (Object)adaptor.create(LPAREN147);
                    adaptor.addChild(root_0, LPAREN147_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_trimExpr2744);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    COMMA148=(Token)match(input,COMMA,FOLLOW_COMMA_in_trimExpr2746); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA148_tree = (Object)adaptor.create(COMMA148);
                    adaptor.addChild(root_0, COMMA148_tree);
                    }
                    LBRACE149=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_trimExpr2748); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACE149_tree = (Object)adaptor.create(LBRACE149);
                    adaptor.addChild(root_0, LBRACE149_tree);
                    }
                    pushFollow(FOLLOW_dimensionIntervalList_in_trimExpr2752);
                    dil=dimensionIntervalList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
                    RBRACE150=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_trimExpr2754); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACE150_tree = (Object)adaptor.create(RBRACE150);
                    adaptor.addChild(root_0, RBRACE150_tree);
                    }
                    RPAREN151=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_trimExpr2756); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN151_tree = (Object)adaptor.create(RPAREN151);
                    adaptor.addChild(root_0, RPAREN151_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 50, trimExpr_StartIndex); }
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
    // wcps.g:269:1: sliceExpr returns [SliceExpr value] : (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN );
    public final wcpsParser.sliceExpr_return sliceExpr() throws RecognitionException {
        wcpsParser.sliceExpr_return retval = new wcpsParser.sliceExpr_return();
        retval.start = input.LT(1);
        int sliceExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACKET152=null;
        Token RBRACKET153=null;
        Token SLICE154=null;
        Token LPAREN155=null;
        Token COMMA156=null;
        Token LBRACE157=null;
        Token RBRACE158=null;
        Token RPAREN159=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.dimensionPointList_return dpl = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LBRACKET152_tree=null;
        Object RBRACKET153_tree=null;
        Object SLICE154_tree=null;
        Object LPAREN155_tree=null;
        Object COMMA156_tree=null;
        Object LBRACE157_tree=null;
        Object RBRACE158_tree=null;
        Object RPAREN159_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 51) ) { return retval; }
            // wcps.g:270:2: (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN )
            int alt45=2;
            alt45 = dfa45.predict(input);
            switch (alt45) {
                case 1 :
                    // wcps.g:270:4: e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_sliceExpr2773);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    LBRACKET152=(Token)match(input,LBRACKET,FOLLOW_LBRACKET_in_sliceExpr2775); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACKET152_tree = (Object)adaptor.create(LBRACKET152);
                    adaptor.addChild(root_0, LBRACKET152_tree);
                    }
                    pushFollow(FOLLOW_dimensionPointList_in_sliceExpr2779);
                    dpl=dimensionPointList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpl.getTree());
                    RBRACKET153=(Token)match(input,RBRACKET,FOLLOW_RBRACKET_in_sliceExpr2781); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACKET153_tree = (Object)adaptor.create(RBRACKET153);
                    adaptor.addChild(root_0, RBRACKET153_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SliceExpr((e1!=null?e1.value:null), (dpl!=null?dpl.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // wcps.g:271:4: SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    SLICE154=(Token)match(input,SLICE,FOLLOW_SLICE_in_sliceExpr2788); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    SLICE154_tree = (Object)adaptor.create(SLICE154);
                    adaptor.addChild(root_0, SLICE154_tree);
                    }
                    LPAREN155=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_sliceExpr2790); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN155_tree = (Object)adaptor.create(LPAREN155);
                    adaptor.addChild(root_0, LPAREN155_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_sliceExpr2794);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    COMMA156=(Token)match(input,COMMA,FOLLOW_COMMA_in_sliceExpr2796); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA156_tree = (Object)adaptor.create(COMMA156);
                    adaptor.addChild(root_0, COMMA156_tree);
                    }
                    LBRACE157=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_sliceExpr2798); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACE157_tree = (Object)adaptor.create(LBRACE157);
                    adaptor.addChild(root_0, LBRACE157_tree);
                    }
                    pushFollow(FOLLOW_dimensionPointList_in_sliceExpr2802);
                    dpl=dimensionPointList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpl.getTree());
                    RBRACE158=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_sliceExpr2804); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACE158_tree = (Object)adaptor.create(RBRACE158);
                    adaptor.addChild(root_0, RBRACE158_tree);
                    }
                    RPAREN159=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_sliceExpr2806); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN159_tree = (Object)adaptor.create(RPAREN159);
                    adaptor.addChild(root_0, RPAREN159_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 51, sliceExpr_StartIndex); }
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
    // wcps.g:273:1: extendExpr returns [ExtendExpr value] : EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN ;
    public final wcpsParser.extendExpr_return extendExpr() throws RecognitionException {
        wcpsParser.extendExpr_return retval = new wcpsParser.extendExpr_return();
        retval.start = input.LT(1);
        int extendExpr_StartIndex = input.index();
        Object root_0 = null;

        Token EXTEND160=null;
        Token LPAREN161=null;
        Token COMMA162=null;
        Token RPAREN163=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;


        Object EXTEND160_tree=null;
        Object LPAREN161_tree=null;
        Object COMMA162_tree=null;
        Object RPAREN163_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 52) ) { return retval; }
            // wcps.g:274:2: ( EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN )
            // wcps.g:274:4: EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            EXTEND160=(Token)match(input,EXTEND,FOLLOW_EXTEND_in_extendExpr2821); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            EXTEND160_tree = (Object)adaptor.create(EXTEND160);
            adaptor.addChild(root_0, EXTEND160_tree);
            }
            LPAREN161=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_extendExpr2823); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN161_tree = (Object)adaptor.create(LPAREN161);
            adaptor.addChild(root_0, LPAREN161_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_extendExpr2827);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA162=(Token)match(input,COMMA,FOLLOW_COMMA_in_extendExpr2829); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA162_tree = (Object)adaptor.create(COMMA162);
            adaptor.addChild(root_0, COMMA162_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_extendExpr2833);
            dil=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
            RPAREN163=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_extendExpr2835); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN163_tree = (Object)adaptor.create(RPAREN163);
            adaptor.addChild(root_0, RPAREN163_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 52, extendExpr_StartIndex); }
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
    // wcps.g:276:1: castExpr returns [CastExpr value] : LPAREN e1= rangeType RPAREN e2= coverageExpr ;
    public final wcpsParser.castExpr_return castExpr() throws RecognitionException {
        wcpsParser.castExpr_return retval = new wcpsParser.castExpr_return();
        retval.start = input.LT(1);
        int castExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN164=null;
        Token RPAREN165=null;
        wcpsParser.rangeType_return e1 = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LPAREN164_tree=null;
        Object RPAREN165_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 53) ) { return retval; }
            // wcps.g:277:5: ( LPAREN e1= rangeType RPAREN e2= coverageExpr )
            // wcps.g:277:7: LPAREN e1= rangeType RPAREN e2= coverageExpr
            {
            root_0 = (Object)adaptor.nil();

            LPAREN164=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_castExpr2853); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN164_tree = (Object)adaptor.create(LPAREN164);
            adaptor.addChild(root_0, LPAREN164_tree);
            }
            pushFollow(FOLLOW_rangeType_in_castExpr2857);
            e1=rangeType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN165=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_castExpr2859); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN165_tree = (Object)adaptor.create(RPAREN165);
            adaptor.addChild(root_0, RPAREN165_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_castExpr2863);
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
            if ( state.backtracking>0 ) { memoize(input, 53, castExpr_StartIndex); }
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
    // wcps.g:279:1: rangeType returns [String value] : (type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 ) | UNSIGNED type= ( CHAR | SHORT | LONG ) );
    public final wcpsParser.rangeType_return rangeType() throws RecognitionException {
        wcpsParser.rangeType_return retval = new wcpsParser.rangeType_return();
        retval.start = input.LT(1);
        int rangeType_StartIndex = input.index();
        Object root_0 = null;

        Token type=null;
        Token UNSIGNED166=null;

        Object type_tree=null;
        Object UNSIGNED166_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 54) ) { return retval; }
            // wcps.g:280:5: (type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 ) | UNSIGNED type= ( CHAR | SHORT | LONG ) )
            int alt46=2;
            int LA46_0 = input.LA(1);

            if ( ((LA46_0>=BOOLEAN && LA46_0<=COMPLEX2)) ) {
                alt46=1;
            }
            else if ( (LA46_0==UNSIGNED) ) {
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
                    // wcps.g:280:7: type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 )
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
                    // wcps.g:281:7: UNSIGNED type= ( CHAR | SHORT | LONG )
                    {
                    root_0 = (Object)adaptor.nil();

                    UNSIGNED166=(Token)match(input,UNSIGNED,FOLLOW_UNSIGNED_in_rangeType2912); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    UNSIGNED166_tree = (Object)adaptor.create(UNSIGNED166);
                    adaptor.addChild(root_0, UNSIGNED166_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 54, rangeType_StartIndex); }
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
    // wcps.g:283:1: fieldExpr returns [SelectExpr value] : e1= coverageAtom DOT e2= fieldName ;
    public final wcpsParser.fieldExpr_return fieldExpr() throws RecognitionException {
        wcpsParser.fieldExpr_return retval = new wcpsParser.fieldExpr_return();
        retval.start = input.LT(1);
        int fieldExpr_StartIndex = input.index();
        Object root_0 = null;

        Token DOT167=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.fieldName_return e2 = null;


        Object DOT167_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 55) ) { return retval; }
            // wcps.g:284:5: (e1= coverageAtom DOT e2= fieldName )
            // wcps.g:284:7: e1= coverageAtom DOT e2= fieldName
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageAtom_in_fieldExpr2945);
            e1=coverageAtom();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            DOT167=(Token)match(input,DOT,FOLLOW_DOT_in_fieldExpr2947); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            DOT167_tree = (Object)adaptor.create(DOT167);
            adaptor.addChild(root_0, DOT167_tree);
            }
            pushFollow(FOLLOW_fieldName_in_fieldExpr2951);
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
            if ( state.backtracking>0 ) { memoize(input, 55, fieldExpr_StartIndex); }
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
    // wcps.g:288:1: booleanScalarExpr returns [BooleanScalarExpr value] : e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )* ;
    public final wcpsParser.booleanScalarExpr_return booleanScalarExpr() throws RecognitionException {
        wcpsParser.booleanScalarExpr_return retval = new wcpsParser.booleanScalarExpr_return();
        retval.start = input.LT(1);
        int booleanScalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.booleanScalarTerm_return e1 = null;

        wcpsParser.booleanScalarTerm_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 56) ) { return retval; }
            // wcps.g:289:5: (e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )* )
            // wcps.g:289:7: e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_booleanScalarTerm_in_booleanScalarExpr2976);
            e1=booleanScalarTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:290:7: (op= ( OR | XOR ) e2= booleanScalarTerm )*
            loop47:
            do {
                int alt47=2;
                alt47 = dfa47.predict(input);
                switch (alt47) {
            	case 1 :
            	    // wcps.g:290:8: op= ( OR | XOR ) e2= booleanScalarTerm
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

            	    pushFollow(FOLLOW_booleanScalarTerm_in_booleanScalarExpr2997);
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
            if ( state.backtracking>0 ) { memoize(input, 56, booleanScalarExpr_StartIndex); }
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
    // wcps.g:292:1: booleanScalarTerm returns [BooleanScalarExpr value] : e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 57) ) { return retval; }
            // wcps.g:293:2: (e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )* )
            // wcps.g:293:4: e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_booleanScalarNegation_in_booleanScalarTerm3019);
            e1=booleanScalarNegation();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:294:4: (op= AND e2= booleanScalarNegation )*
            loop48:
            do {
                int alt48=2;
                alt48 = dfa48.predict(input);
                switch (alt48) {
            	case 1 :
            	    // wcps.g:294:5: op= AND e2= booleanScalarNegation
            	    {
            	    op=(Token)match(input,AND,FOLLOW_AND_in_booleanScalarTerm3029); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    op_tree = (Object)adaptor.create(op);
            	    adaptor.addChild(root_0, op_tree);
            	    }
            	    pushFollow(FOLLOW_booleanScalarNegation_in_booleanScalarTerm3033);
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
            if ( state.backtracking>0 ) { memoize(input, 57, booleanScalarTerm_StartIndex); }
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
    // wcps.g:296:1: booleanScalarNegation returns [BooleanScalarExpr value] : (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom );
    public final wcpsParser.booleanScalarNegation_return booleanScalarNegation() throws RecognitionException {
        wcpsParser.booleanScalarNegation_return retval = new wcpsParser.booleanScalarNegation_return();
        retval.start = input.LT(1);
        int booleanScalarNegation_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.booleanScalarAtom_return e1 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 58) ) { return retval; }
            // wcps.g:297:2: (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom )
            int alt49=2;
            alt49 = dfa49.predict(input);
            switch (alt49) {
                case 1 :
                    // wcps.g:297:4: e1= booleanScalarAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanScalarAtom_in_booleanScalarNegation3054);
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
                    // wcps.g:298:4: op= NOT e1= booleanScalarAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NOT,FOLLOW_NOT_in_booleanScalarNegation3063); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarAtom_in_booleanScalarNegation3067);
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
            if ( state.backtracking>0 ) { memoize(input, 58, booleanScalarNegation_StartIndex); }
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
    // wcps.g:300:1: booleanScalarAtom returns [BooleanScalarExpr value] : ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 59) ) { return retval; }
            // wcps.g:301:2: ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT )
            int alt50=4;
            alt50 = dfa50.predict(input);
            switch (alt50) {
                case 1 :
                    // wcps.g:301:4: LPAREN e1= booleanScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN168=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_booleanScalarAtom3082); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN168_tree = (Object)adaptor.create(LPAREN168);
                    adaptor.addChild(root_0, LPAREN168_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarExpr_in_booleanScalarAtom3086);
                    e1=booleanScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN169=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_booleanScalarAtom3088); if (state.failed) return retval;
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
                    // wcps.g:302:4: s1= stringScalarExpr cop= compOp s2= stringScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_stringScalarExpr_in_booleanScalarAtom3097);
                    s1=stringScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, s1.getTree());
                    pushFollow(FOLLOW_compOp_in_booleanScalarAtom3101);
                    cop=compOp();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cop.getTree());
                    pushFollow(FOLLOW_stringScalarExpr_in_booleanScalarAtom3105);
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
                    // wcps.g:303:4: n1= numericScalarExpr cop= compOp n2= numericScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_numericScalarExpr_in_booleanScalarAtom3115);
                    n1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, n1.getTree());
                    pushFollow(FOLLOW_compOp_in_booleanScalarAtom3119);
                    cop=compOp();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cop.getTree());
                    pushFollow(FOLLOW_numericScalarExpr_in_booleanScalarAtom3123);
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
                    // wcps.g:304:4: e= BOOLEANCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,BOOLEANCONSTANT,FOLLOW_BOOLEANCONSTANT_in_booleanScalarAtom3133); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 59, booleanScalarAtom_StartIndex); }
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
    // wcps.g:306:1: numericScalarExpr returns [NumericScalarExpr value] : e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )* ;
    public final wcpsParser.numericScalarExpr_return numericScalarExpr() throws RecognitionException {
        wcpsParser.numericScalarExpr_return retval = new wcpsParser.numericScalarExpr_return();
        retval.start = input.LT(1);
        int numericScalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.numericScalarTerm_return e1 = null;

        wcpsParser.numericScalarTerm_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 60) ) { return retval; }
            // wcps.g:307:2: (e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )* )
            // wcps.g:307:4: e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_numericScalarTerm_in_numericScalarExpr3150);
            e1=numericScalarTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:308:4: (op= ( PLUS | MINUS ) e2= numericScalarTerm )*
            loop51:
            do {
                int alt51=2;
                alt51 = dfa51.predict(input);
                switch (alt51) {
            	case 1 :
            	    // wcps.g:308:5: op= ( PLUS | MINUS ) e2= numericScalarTerm
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

            	    pushFollow(FOLLOW_numericScalarTerm_in_numericScalarExpr3168);
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
            	    break loop51;
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
            if ( state.backtracking>0 ) { memoize(input, 60, numericScalarExpr_StartIndex); }
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
    // wcps.g:310:1: numericScalarTerm returns [NumericScalarExpr value] : e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )* ;
    public final wcpsParser.numericScalarTerm_return numericScalarTerm() throws RecognitionException {
        wcpsParser.numericScalarTerm_return retval = new wcpsParser.numericScalarTerm_return();
        retval.start = input.LT(1);
        int numericScalarTerm_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        wcpsParser.numericScalarFactor_return e1 = null;

        wcpsParser.numericScalarFactor_return e2 = null;


        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 61) ) { return retval; }
            // wcps.g:311:2: (e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )* )
            // wcps.g:311:4: e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_numericScalarFactor_in_numericScalarTerm3187);
            e1=numericScalarFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // wcps.g:312:3: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*
            loop52:
            do {
                int alt52=2;
                alt52 = dfa52.predict(input);
                switch (alt52) {
            	case 1 :
            	    // wcps.g:312:4: op= ( MULT | DIVIDE ) e2= numericScalarFactor
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

            	    pushFollow(FOLLOW_numericScalarFactor_in_numericScalarTerm3204);
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
            if ( state.backtracking>0 ) { memoize(input, 61, numericScalarTerm_StartIndex); }
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
    // wcps.g:314:1: numericScalarFactor returns [NumericScalarExpr value] : ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ABS LPAREN e12= numericScalarExpr RPAREN | op= SQRT LPAREN e11= numericScalarExpr RPAREN | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr | e4= variableName );
    public final wcpsParser.numericScalarFactor_return numericScalarFactor() throws RecognitionException {
        wcpsParser.numericScalarFactor_return retval = new wcpsParser.numericScalarFactor_return();
        retval.start = input.LT(1);
        int numericScalarFactor_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token e=null;
        Token LPAREN170=null;
        Token RPAREN171=null;
        Token LPAREN172=null;
        Token RPAREN173=null;
        Token LPAREN174=null;
        Token RPAREN175=null;
        Token LPAREN176=null;
        Token RPAREN177=null;
        wcpsParser.numericScalarExpr_return e1 = null;

        wcpsParser.numericScalarFactor_return e10 = null;

        wcpsParser.numericScalarExpr_return e12 = null;

        wcpsParser.numericScalarExpr_return e11 = null;

        wcpsParser.complexConstant_return e2 = null;

        wcpsParser.condenseExpr_return e3 = null;

        wcpsParser.variableName_return e4 = null;


        Object op_tree=null;
        Object e_tree=null;
        Object LPAREN170_tree=null;
        Object RPAREN171_tree=null;
        Object LPAREN172_tree=null;
        Object RPAREN173_tree=null;
        Object LPAREN174_tree=null;
        Object RPAREN175_tree=null;
        Object LPAREN176_tree=null;
        Object RPAREN177_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 62) ) { return retval; }
            // wcps.g:315:5: ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ABS LPAREN e12= numericScalarExpr RPAREN | op= SQRT LPAREN e11= numericScalarExpr RPAREN | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr | e4= variableName )
            int alt53=10;
            alt53 = dfa53.predict(input);
            switch (alt53) {
                case 1 :
                    // wcps.g:315:7: LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN170=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3224); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN170_tree = (Object)adaptor.create(LPAREN170);
                    adaptor.addChild(root_0, LPAREN170_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3228);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN171=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3230); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN171_tree = (Object)adaptor.create(RPAREN171);
                    adaptor.addChild(root_0, RPAREN171_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 2 :
                    // wcps.g:316:7: op= MINUS e10= numericScalarFactor
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,MINUS,FOLLOW_MINUS_in_numericScalarFactor3242); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_numericScalarFactor_in_numericScalarFactor3246);
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
                    // wcps.g:317:7: op= ABS LPAREN e12= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ABS,FOLLOW_ABS_in_numericScalarFactor3258); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN172=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3260); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN172_tree = (Object)adaptor.create(LPAREN172);
                    adaptor.addChild(root_0, LPAREN172_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3264);
                    e12=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e12.getTree());
                    RPAREN173=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3266); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN173_tree = (Object)adaptor.create(RPAREN173);
                    adaptor.addChild(root_0, RPAREN173_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e12!=null?e12.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // wcps.g:318:7: op= SQRT LPAREN e11= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SQRT,FOLLOW_SQRT_in_numericScalarFactor3278); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN174=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3280); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN174_tree = (Object)adaptor.create(LPAREN174);
                    adaptor.addChild(root_0, LPAREN174_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3284);
                    e11=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e11.getTree());
                    RPAREN175=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3286); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN175_tree = (Object)adaptor.create(RPAREN175);
                    adaptor.addChild(root_0, RPAREN175_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e11!=null?e11.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // wcps.g:319:7: op= ROUND LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ROUND,FOLLOW_ROUND_in_numericScalarFactor3298); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN176=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3300); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN176_tree = (Object)adaptor.create(LPAREN176);
                    adaptor.addChild(root_0, LPAREN176_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3304);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN177=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3306); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN177_tree = (Object)adaptor.create(RPAREN177);
                    adaptor.addChild(root_0, RPAREN177_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 6 :
                    // wcps.g:320:7: e= INTEGERCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,INTEGERCONSTANT,FOLLOW_INTEGERCONSTANT_in_numericScalarFactor3318); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;
                case 7 :
                    // wcps.g:321:7: e= FLOATCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_numericScalarFactor3330); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    e_tree = (Object)adaptor.create(e);
                    adaptor.addChild(root_0, e_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e!=null?e.getText():null)); 
                    }

                    }
                    break;
                case 8 :
                    // wcps.g:322:7: e2= complexConstant
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_complexConstant_in_numericScalarFactor3342);
                    e2=complexConstant();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e2!=null?e2.value:null)); 
                    }

                    }
                    break;
                case 9 :
                    // wcps.g:323:7: e3= condenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_condenseExpr_in_numericScalarFactor3354);
                    e3=condenseExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((e3!=null?e3.value:null)); 
                    }

                    }
                    break;
                case 10 :
                    // wcps.g:324:7: e4= variableName
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_variableName_in_numericScalarFactor3366);
                    e4=variableName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr("var", (e4!=null?e4.value:null)); 
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
            if ( state.backtracking>0 ) { memoize(input, 62, numericScalarFactor_StartIndex); }
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
    // wcps.g:326:1: compOp returns [String value] : ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 63) ) { return retval; }
            // wcps.g:327:2: ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE )
            int alt54=6;
            switch ( input.LA(1) ) {
            case EQUALS:
                {
                alt54=1;
                }
                break;
            case NOTEQUALS:
                {
                alt54=2;
                }
                break;
            case LT:
                {
                alt54=3;
                }
                break;
            case GT:
                {
                alt54=4;
                }
                break;
            case LTE:
                {
                alt54=5;
                }
                break;
            case GTE:
                {
                alt54=6;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 54, 0, input);

                throw nvae;
            }

            switch (alt54) {
                case 1 :
                    // wcps.g:327:4: EQUALS
                    {
                    root_0 = (Object)adaptor.nil();

                    EQUALS178=(Token)match(input,EQUALS,FOLLOW_EQUALS_in_compOp3384); if (state.failed) return retval;
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
                    // wcps.g:328:4: NOTEQUALS
                    {
                    root_0 = (Object)adaptor.nil();

                    NOTEQUALS179=(Token)match(input,NOTEQUALS,FOLLOW_NOTEQUALS_in_compOp3391); if (state.failed) return retval;
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
                    // wcps.g:329:4: LT
                    {
                    root_0 = (Object)adaptor.nil();

                    LT180=(Token)match(input,LT,FOLLOW_LT_in_compOp3398); if (state.failed) return retval;
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
                    // wcps.g:330:4: GT
                    {
                    root_0 = (Object)adaptor.nil();

                    GT181=(Token)match(input,GT,FOLLOW_GT_in_compOp3405); if (state.failed) return retval;
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
                    // wcps.g:331:4: LTE
                    {
                    root_0 = (Object)adaptor.nil();

                    LTE182=(Token)match(input,LTE,FOLLOW_LTE_in_compOp3412); if (state.failed) return retval;
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
                    // wcps.g:332:4: GTE
                    {
                    root_0 = (Object)adaptor.nil();

                    GTE183=(Token)match(input,GTE,FOLLOW_GTE_in_compOp3419); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 63, compOp_StartIndex); }
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
    // wcps.g:334:1: dimensionIntervalList returns [DimensionIntervalList value] : elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )* ;
    public final wcpsParser.dimensionIntervalList_return dimensionIntervalList() throws RecognitionException {
        wcpsParser.dimensionIntervalList_return retval = new wcpsParser.dimensionIntervalList_return();
        retval.start = input.LT(1);
        int dimensionIntervalList_StartIndex = input.index();
        Object root_0 = null;

        Token COMMA184=null;
        wcpsParser.dimensionIntervalElement_return elem = null;


        Object COMMA184_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 64) ) { return retval; }
            // wcps.g:335:5: (elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )* )
            // wcps.g:335:7: elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3439);
            elem=dimensionIntervalElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionIntervalList((elem!=null?elem.value:null)); 
            }
            // wcps.g:336:9: ( COMMA elem= dimensionIntervalElement )*
            loop55:
            do {
                int alt55=2;
                int LA55_0 = input.LA(1);

                if ( (LA55_0==COMMA) ) {
                    alt55=1;
                }


                switch (alt55) {
            	case 1 :
            	    // wcps.g:336:10: COMMA elem= dimensionIntervalElement
            	    {
            	    COMMA184=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionIntervalList3452); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA184_tree = (Object)adaptor.create(COMMA184);
            	    adaptor.addChild(root_0, COMMA184_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3456);
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
            	    break loop55;
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
            if ( state.backtracking>0 ) { memoize(input, 64, dimensionIntervalList_StartIndex); }
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
    // wcps.g:338:1: dimensionIntervalElement returns [DimensionIntervalElement value] : aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 65) ) { return retval; }
            // wcps.g:339:5: (aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN )
            // wcps.g:339:7: aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_axisName_in_dimensionIntervalElement3481);
            aname=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionIntervalElement((aname!=null?aname.value:null)); 
            }
            // wcps.g:339:79: ( COLON crs= crsName )?
            int alt56=2;
            int LA56_0 = input.LA(1);

            if ( (LA56_0==COLON) ) {
                alt56=1;
            }
            switch (alt56) {
                case 1 :
                    // wcps.g:339:80: COLON crs= crsName
                    {
                    COLON185=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalElement3486); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON185_tree = (Object)adaptor.create(COLON185);
                    adaptor.addChild(root_0, COLON185_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionIntervalElement3490);
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

            LPAREN186=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionIntervalElement3501); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN186_tree = (Object)adaptor.create(LPAREN186);
            adaptor.addChild(root_0, LPAREN186_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalExpr_in_dimensionIntervalElement3505);
            die=dimensionIntervalExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, die.getTree());
            RPAREN187=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionIntervalElement3507); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 65, dimensionIntervalElement_StartIndex); }
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
    // wcps.g:342:1: dimensionIntervalExpr returns [DimensionIntervalExpr value] : (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 66) ) { return retval; }
            // wcps.g:343:5: (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN )
            int alt57=2;
            alt57 = dfa57.predict(input);
            switch (alt57) {
                case 1 :
                    // wcps.g:343:7: e1= scalarExpr COLON e2= scalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scalarExpr_in_dimensionIntervalExpr3530);
                    e1=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COLON188=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3532); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON188_tree = (Object)adaptor.create(COLON188);
                    adaptor.addChild(root_0, COLON188_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_dimensionIntervalExpr3536);
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
                    // wcps.g:344:7: DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    DOMAIN189=(Token)match(input,DOMAIN,FOLLOW_DOMAIN_in_dimensionIntervalExpr3546); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    DOMAIN189_tree = (Object)adaptor.create(DOMAIN189);
                    adaptor.addChild(root_0, DOMAIN189_tree);
                    }
                    LPAREN190=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionIntervalExpr3548); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN190_tree = (Object)adaptor.create(LPAREN190);
                    adaptor.addChild(root_0, LPAREN190_tree);
                    }
                    pushFollow(FOLLOW_coverageName_in_dimensionIntervalExpr3552);
                    e3=coverageName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    COLON191=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3554); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON191_tree = (Object)adaptor.create(COLON191);
                    adaptor.addChild(root_0, COLON191_tree);
                    }
                    pushFollow(FOLLOW_axisName_in_dimensionIntervalExpr3558);
                    e4=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    COLON192=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3560); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON192_tree = (Object)adaptor.create(COLON192);
                    adaptor.addChild(root_0, COLON192_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionIntervalExpr3564);
                    e5=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    RPAREN193=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionIntervalExpr3566); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 66, dimensionIntervalExpr_StartIndex); }
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
    // wcps.g:346:1: dimensionPointList returns [DimensionPointList value] : elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 67) ) { return retval; }
            // wcps.g:347:5: (elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )* )
            // wcps.g:347:7: elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_dimensionPointElement_in_dimensionPointList3589);
            elem1=dimensionPointElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionPointList((elem1!=null?elem1.value:null)); 
            }
            // wcps.g:348:6: ( COMMA elem2= dimensionPointElement )*
            loop58:
            do {
                int alt58=2;
                int LA58_0 = input.LA(1);

                if ( (LA58_0==COMMA) ) {
                    alt58=1;
                }


                switch (alt58) {
            	case 1 :
            	    // wcps.g:348:7: COMMA elem2= dimensionPointElement
            	    {
            	    COMMA194=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionPointList3599); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA194_tree = (Object)adaptor.create(COMMA194);
            	    adaptor.addChild(root_0, COMMA194_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionPointElement_in_dimensionPointList3603);
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
            	    break loop58;
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
            if ( state.backtracking>0 ) { memoize(input, 67, dimensionPointList_StartIndex); }
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
    // wcps.g:350:1: dimensionPointElement returns [DimensionPointElement value] : (aname= axisName LPAREN dpe= dimensionPoint RPAREN | aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN );
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 68) ) { return retval; }
            // wcps.g:351:5: (aname= axisName LPAREN dpe= dimensionPoint RPAREN | aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN )
            int alt59=2;
            int LA59_0 = input.LA(1);

            if ( (LA59_0==INTEGERCONSTANT||LA59_0==STRING||LA59_0==NAME) ) {
                int LA59_1 = input.LA(2);

                if ( (LA59_1==COLON) ) {
                    alt59=2;
                }
                else if ( (LA59_1==LPAREN) ) {
                    alt59=1;
                }
                else {
                    if (state.backtracking>0) {state.failed=true; return retval;}
                    NoViableAltException nvae =
                        new NoViableAltException("", 59, 1, input);

                    throw nvae;
                }
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 59, 0, input);

                throw nvae;
            }
            switch (alt59) {
                case 1 :
                    // wcps.g:351:7: aname= axisName LPAREN dpe= dimensionPoint RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_axisName_in_dimensionPointElement3628);
                    aname=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
                    LPAREN195=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionPointElement3630); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN195_tree = (Object)adaptor.create(LPAREN195);
                    adaptor.addChild(root_0, LPAREN195_tree);
                    }
                    pushFollow(FOLLOW_dimensionPoint_in_dimensionPointElement3634);
                    dpe=dimensionPoint();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpe.getTree());
                    RPAREN196=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionPointElement3636); if (state.failed) return retval;
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
                    // wcps.g:352:7: aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_axisName_in_dimensionPointElement3648);
                    aname=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
                    COLON197=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionPointElement3650); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON197_tree = (Object)adaptor.create(COLON197);
                    adaptor.addChild(root_0, COLON197_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionPointElement3654);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    LPAREN198=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionPointElement3656); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN198_tree = (Object)adaptor.create(LPAREN198);
                    adaptor.addChild(root_0, LPAREN198_tree);
                    }
                    pushFollow(FOLLOW_dimensionPoint_in_dimensionPointElement3660);
                    dpe=dimensionPoint();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpe.getTree());
                    RPAREN199=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionPointElement3662); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 68, dimensionPointElement_StartIndex); }
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
    // wcps.g:354:1: dimensionPoint returns [ScalarExpr value] : e1= scalarExpr ;
    public final wcpsParser.dimensionPoint_return dimensionPoint() throws RecognitionException {
        wcpsParser.dimensionPoint_return retval = new wcpsParser.dimensionPoint_return();
        retval.start = input.LT(1);
        int dimensionPoint_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.scalarExpr_return e1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 69) ) { return retval; }
            // wcps.g:355:5: (e1= scalarExpr )
            // wcps.g:355:7: e1= scalarExpr
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_scalarExpr_in_dimensionPoint3685);
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
            if ( state.backtracking>0 ) { memoize(input, 69, dimensionPoint_StartIndex); }
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
    // wcps.g:357:1: interpolationMethod returns [InterpolationMethod value] : LPAREN type= interpolationType COLON res= nullResistence RPAREN ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 70) ) { return retval; }
            // wcps.g:358:2: ( LPAREN type= interpolationType COLON res= nullResistence RPAREN )
            // wcps.g:358:4: LPAREN type= interpolationType COLON res= nullResistence RPAREN
            {
            root_0 = (Object)adaptor.nil();

            LPAREN200=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_interpolationMethod3703); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN200_tree = (Object)adaptor.create(LPAREN200);
            adaptor.addChild(root_0, LPAREN200_tree);
            }
            pushFollow(FOLLOW_interpolationType_in_interpolationMethod3707);
            type=interpolationType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, type.getTree());
            COLON201=(Token)match(input,COLON,FOLLOW_COLON_in_interpolationMethod3709); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COLON201_tree = (Object)adaptor.create(COLON201);
            adaptor.addChild(root_0, COLON201_tree);
            }
            pushFollow(FOLLOW_nullResistence_in_interpolationMethod3713);
            res=nullResistence();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, res.getTree());
            RPAREN202=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_interpolationMethod3715); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 70, interpolationMethod_StartIndex); }
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
    // wcps.g:360:1: interpolationType returns [String value] : type= ( NEAREST | LINEAR | QUADRATIC | CUBIC ) ;
    public final wcpsParser.interpolationType_return interpolationType() throws RecognitionException {
        wcpsParser.interpolationType_return retval = new wcpsParser.interpolationType_return();
        retval.start = input.LT(1);
        int interpolationType_StartIndex = input.index();
        Object root_0 = null;

        Token type=null;

        Object type_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 71) ) { return retval; }
            // wcps.g:361:2: (type= ( NEAREST | LINEAR | QUADRATIC | CUBIC ) )
            // wcps.g:361:4: type= ( NEAREST | LINEAR | QUADRATIC | CUBIC )
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
            if ( state.backtracking>0 ) { memoize(input, 71, interpolationType_StartIndex); }
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
    // wcps.g:363:1: nullResistence returns [String value] : resistance= ( FULL | NONE | HALF | OTHER ) ;
    public final wcpsParser.nullResistence_return nullResistence() throws RecognitionException {
        wcpsParser.nullResistence_return retval = new wcpsParser.nullResistence_return();
        retval.start = input.LT(1);
        int nullResistence_StartIndex = input.index();
        Object root_0 = null;

        Token resistance=null;

        Object resistance_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 72) ) { return retval; }
            // wcps.g:364:2: (resistance= ( FULL | NONE | HALF | OTHER ) )
            // wcps.g:364:4: resistance= ( FULL | NONE | HALF | OTHER )
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
            if ( state.backtracking>0 ) { memoize(input, 72, nullResistence_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "nullResistence"

    public static class condenseOpType_return extends ParserRuleReturnScope {
        public CondenseOperation value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "condenseOpType"
    // wcps.g:366:1: condenseOpType returns [CondenseOperation value] : op= ( PLUS | MULT | MAX | MIN | AND | OR ) ;
    public final wcpsParser.condenseOpType_return condenseOpType() throws RecognitionException {
        wcpsParser.condenseOpType_return retval = new wcpsParser.condenseOpType_return();
        retval.start = input.LT(1);
        int condenseOpType_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;

        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 73) ) { return retval; }
            // wcps.g:367:2: (op= ( PLUS | MULT | MAX | MIN | AND | OR ) )
            // wcps.g:367:4: op= ( PLUS | MULT | MAX | MIN | AND | OR )
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
               retval.value = new CondenseOperation((op!=null?op.getText():null)); 
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
            if ( state.backtracking>0 ) { memoize(input, 73, condenseOpType_StartIndex); }
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
    // wcps.g:369:1: fieldName returns [String value] : name ;
    public final wcpsParser.fieldName_return fieldName() throws RecognitionException {
        wcpsParser.fieldName_return retval = new wcpsParser.fieldName_return();
        retval.start = input.LT(1);
        int fieldName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return name203 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 74) ) { return retval; }
            // wcps.g:370:2: ( name )
            // wcps.g:370:4: name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_fieldName3809);
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
            if ( state.backtracking>0 ) { memoize(input, 74, fieldName_StartIndex); }
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
    // wcps.g:372:1: constant returns [String value] : (e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT ) | e1= complexConstant );
    public final wcpsParser.constant_return constant() throws RecognitionException {
        wcpsParser.constant_return retval = new wcpsParser.constant_return();
        retval.start = input.LT(1);
        int constant_StartIndex = input.index();
        Object root_0 = null;

        Token e=null;
        wcpsParser.complexConstant_return e1 = null;


        Object e_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 75) ) { return retval; }
            // wcps.g:373:2: (e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT ) | e1= complexConstant )
            int alt60=2;
            int LA60_0 = input.LA(1);

            if ( (LA60_0==INTEGERCONSTANT||LA60_0==STRING||(LA60_0>=BOOLEANCONSTANT && LA60_0<=FLOATCONSTANT)) ) {
                alt60=1;
            }
            else if ( (LA60_0==LPAREN) ) {
                alt60=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 60, 0, input);

                throw nvae;
            }
            switch (alt60) {
                case 1 :
                    // wcps.g:373:4: e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT )
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
                    // wcps.g:374:4: e1= complexConstant
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_complexConstant_in_constant3843);
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
            if ( state.backtracking>0 ) { memoize(input, 75, constant_StartIndex); }
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
    // wcps.g:376:1: complexConstant returns [String value] : LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 76) ) { return retval; }
            // wcps.g:377:2: ( LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN )
            // wcps.g:377:4: LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN
            {
            root_0 = (Object)adaptor.nil();

            LPAREN204=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_complexConstant3858); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN204_tree = (Object)adaptor.create(LPAREN204);
            adaptor.addChild(root_0, LPAREN204_tree);
            }
            re1=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_complexConstant3862); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            re1_tree = (Object)adaptor.create(re1);
            adaptor.addChild(root_0, re1_tree);
            }
            COMMA205=(Token)match(input,COMMA,FOLLOW_COMMA_in_complexConstant3864); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA205_tree = (Object)adaptor.create(COMMA205);
            adaptor.addChild(root_0, COMMA205_tree);
            }
            im1=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_complexConstant3868); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            im1_tree = (Object)adaptor.create(im1);
            adaptor.addChild(root_0, im1_tree);
            }
            RPAREN206=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_complexConstant3870); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 76, complexConstant_StartIndex); }
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
    // wcps.g:379:1: stringConstant returns [String value] : s= STRING ;
    public final wcpsParser.stringConstant_return stringConstant() throws RecognitionException {
        wcpsParser.stringConstant_return retval = new wcpsParser.stringConstant_return();
        retval.start = input.LT(1);
        int stringConstant_StartIndex = input.index();
        Object root_0 = null;

        Token s=null;

        Object s_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 77) ) { return retval; }
            // wcps.g:380:2: (s= STRING )
            // wcps.g:380:4: s= STRING
            {
            root_0 = (Object)adaptor.nil();

            s=(Token)match(input,STRING,FOLLOW_STRING_in_stringConstant3887); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 77, stringConstant_StartIndex); }
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
    // wcps.g:382:1: name returns [String value] : var= ( NAME | STRING | INTEGERCONSTANT ) ;
    public final wcpsParser.name_return name() throws RecognitionException {
        wcpsParser.name_return retval = new wcpsParser.name_return();
        retval.start = input.LT(1);
        int name_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 78) ) { return retval; }
            // wcps.g:383:2: (var= ( NAME | STRING | INTEGERCONSTANT ) )
            // wcps.g:383:4: var= ( NAME | STRING | INTEGERCONSTANT )
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
            if ( state.backtracking>0 ) { memoize(input, 78, name_StartIndex); }
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
    // wcps.g:385:1: crsName returns [String value] : s= stringConstant ;
    public final wcpsParser.crsName_return crsName() throws RecognitionException {
        wcpsParser.crsName_return retval = new wcpsParser.crsName_return();
        retval.start = input.LT(1);
        int crsName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.stringConstant_return s = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 79) ) { return retval; }
            // wcps.g:386:2: (s= stringConstant )
            // wcps.g:386:4: s= stringConstant
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_stringConstant_in_crsName3931);
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
            if ( state.backtracking>0 ) { memoize(input, 79, crsName_StartIndex); }
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
    // wcps.g:388:1: axisName returns [String value] : type1= name ;
    public final wcpsParser.axisName_return axisName() throws RecognitionException {
        wcpsParser.axisName_return retval = new wcpsParser.axisName_return();
        retval.start = input.LT(1);
        int axisName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return type1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 80) ) { return retval; }
            // wcps.g:389:2: (type1= name )
            // wcps.g:389:4: type1= name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_axisName3948);
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
            if ( state.backtracking>0 ) { memoize(input, 80, axisName_StartIndex); }
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
    // wcps.g:391:1: variableName returns [String value] : var= VARIABLE_DOLLAR ;
    public final wcpsParser.variableName_return variableName() throws RecognitionException {
        wcpsParser.variableName_return retval = new wcpsParser.variableName_return();
        retval.start = input.LT(1);
        int variableName_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 81) ) { return retval; }
            // wcps.g:392:2: (var= VARIABLE_DOLLAR )
            // wcps.g:392:4: var= VARIABLE_DOLLAR
            {
            root_0 = (Object)adaptor.nil();

            var=(Token)match(input,VARIABLE_DOLLAR,FOLLOW_VARIABLE_DOLLAR_in_variableName3965); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            var_tree = (Object)adaptor.create(var);
            adaptor.addChild(root_0, var_tree);
            }
            if ( state.backtracking==0 ) {
               retval.value = new String((var!=null?var.getText():null)); 
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
            if ( state.backtracking>0 ) { memoize(input, 81, variableName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "variableName"

    public static class coverageVariable_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageVariable"
    // wcps.g:394:1: coverageVariable returns [String value] : var= NAME ;
    public final wcpsParser.coverageVariable_return coverageVariable() throws RecognitionException {
        wcpsParser.coverageVariable_return retval = new wcpsParser.coverageVariable_return();
        retval.start = input.LT(1);
        int coverageVariable_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 82) ) { return retval; }
            // wcps.g:395:2: (var= NAME )
            // wcps.g:395:4: var= NAME
            {
            root_0 = (Object)adaptor.nil();

            var=(Token)match(input,NAME,FOLLOW_NAME_in_coverageVariable3982); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            var_tree = (Object)adaptor.create(var);
            adaptor.addChild(root_0, var_tree);
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
            if ( state.backtracking>0 ) { memoize(input, 82, coverageVariable_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageVariable"

    public static class coverageName_return extends ParserRuleReturnScope {
        public String value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "coverageName"
    // wcps.g:397:1: coverageName returns [String value] : name ;
    public final wcpsParser.coverageName_return coverageName() throws RecognitionException {
        wcpsParser.coverageName_return retval = new wcpsParser.coverageName_return();
        retval.start = input.LT(1);
        int coverageName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return name207 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 83) ) { return retval; }
            // wcps.g:398:2: ( name )
            // wcps.g:398:4: name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_coverageName3997);
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
            if ( state.backtracking>0 ) { memoize(input, 83, coverageName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageName"

    // $ANTLR start synpred8_wcps
    public final void synpred8_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageLogicTerm_return e2 = null;


        // wcps.g:71:10: (op= ( OR | XOR ) e2= coverageLogicTerm )
        // wcps.g:71:10: op= ( OR | XOR ) e2= coverageLogicTerm
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


        // wcps.g:75:10: (op= AND e2= coverageLogicFactor )
        // wcps.g:75:10: op= AND e2= coverageLogicFactor
        {
        op=(Token)match(input,AND,FOLLOW_AND_in_synpred9_wcps416); if (state.failed) return ;
        pushFollow(FOLLOW_coverageLogicFactor_in_synpred9_wcps420);
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


        // wcps.g:79:10: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )
        // wcps.g:79:10: op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr
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

        pushFollow(FOLLOW_coverageArithmeticExpr_in_synpred15_wcps479);
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


        // wcps.g:83:10: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )
        // wcps.g:83:10: op= ( PLUS | MINUS ) e2= coverageArithmeticTerm
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

        pushFollow(FOLLOW_coverageArithmeticTerm_in_synpred17_wcps531);
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


        // wcps.g:87:10: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )
        // wcps.g:87:10: op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor
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

        pushFollow(FOLLOW_coverageArithmeticFactor_in_synpred19_wcps582);
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


        // wcps.g:91:10: (op= OVERLAY e2= coverageValue )
        // wcps.g:91:10: op= OVERLAY e2= coverageValue
        {
        op=(Token)match(input,OVERLAY,FOLLOW_OVERLAY_in_synpred20_wcps625); if (state.failed) return ;
        pushFollow(FOLLOW_coverageValue_in_synpred20_wcps629);
        e2=coverageValue();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred20_wcps

    // $ANTLR start synpred21_wcps
    public final void synpred21_wcps_fragment() throws RecognitionException {   
        wcpsParser.subsetExpr_return e5 = null;


        // wcps.g:94:7: (e5= subsetExpr )
        // wcps.g:94:7: e5= subsetExpr
        {
        pushFollow(FOLLOW_subsetExpr_in_synpred21_wcps655);
        e5=subsetExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred21_wcps

    // $ANTLR start synpred22_wcps
    public final void synpred22_wcps_fragment() throws RecognitionException {   
        wcpsParser.unaryInducedExpr_return e2 = null;


        // wcps.g:95:7: (e2= unaryInducedExpr )
        // wcps.g:95:7: e2= unaryInducedExpr
        {
        pushFollow(FOLLOW_unaryInducedExpr_in_synpred22_wcps668);
        e2=unaryInducedExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred22_wcps

    // $ANTLR start synpred25_wcps
    public final void synpred25_wcps_fragment() throws RecognitionException {   
        wcpsParser.scalarExpr_return e2 = null;


        // wcps.g:101:7: (e2= scalarExpr )
        // wcps.g:101:7: e2= scalarExpr
        {
        pushFollow(FOLLOW_scalarExpr_in_synpred25_wcps727);
        e2=scalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred25_wcps

    // $ANTLR start synpred27_wcps
    public final void synpred27_wcps_fragment() throws RecognitionException {   
        wcpsParser.coverageExpr_return e7 = null;


        // wcps.g:103:7: ( LPAREN e7= coverageExpr RPAREN )
        // wcps.g:103:7: LPAREN e7= coverageExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred27_wcps749); if (state.failed) return ;
        pushFollow(FOLLOW_coverageExpr_in_synpred27_wcps753);
        e7=coverageExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred27_wcps755); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred27_wcps

    // $ANTLR start synpred28_wcps
    public final void synpred28_wcps_fragment() throws RecognitionException {   
        wcpsParser.coverageConstantExpr_return e3 = null;


        // wcps.g:104:7: (e3= coverageConstantExpr )
        // wcps.g:104:7: e3= coverageConstantExpr
        {
        pushFollow(FOLLOW_coverageConstantExpr_in_synpred28_wcps768);
        e3=coverageConstantExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred28_wcps

    // $ANTLR start synpred29_wcps
    public final void synpred29_wcps_fragment() throws RecognitionException {   
        wcpsParser.coverageConstructorExpr_return e4 = null;


        // wcps.g:105:7: (e4= coverageConstructorExpr )
        // wcps.g:105:7: e4= coverageConstructorExpr
        {
        pushFollow(FOLLOW_coverageConstructorExpr_in_synpred29_wcps780);
        e4=coverageConstructorExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred29_wcps

    // $ANTLR start synpred31_wcps
    public final void synpred31_wcps_fragment() throws RecognitionException {   
        wcpsParser.metaDataExpr_return e1 = null;


        // wcps.g:110:7: (e1= metaDataExpr )
        // wcps.g:110:7: e1= metaDataExpr
        {
        pushFollow(FOLLOW_metaDataExpr_in_synpred31_wcps830);
        e1=metaDataExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred31_wcps

    // $ANTLR start synpred32_wcps
    public final void synpred32_wcps_fragment() throws RecognitionException {   
        wcpsParser.condenseExpr_return e2 = null;


        // wcps.g:111:7: (e2= condenseExpr )
        // wcps.g:111:7: e2= condenseExpr
        {
        pushFollow(FOLLOW_condenseExpr_in_synpred32_wcps843);
        e2=condenseExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred32_wcps

    // $ANTLR start synpred33_wcps
    public final void synpred33_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanScalarExpr_return e3 = null;


        // wcps.g:112:7: (e3= booleanScalarExpr )
        // wcps.g:112:7: e3= booleanScalarExpr
        {
        pushFollow(FOLLOW_booleanScalarExpr_in_synpred33_wcps856);
        e3=booleanScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred33_wcps

    // $ANTLR start synpred34_wcps
    public final void synpred34_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return e4 = null;


        // wcps.g:113:7: (e4= numericScalarExpr )
        // wcps.g:113:7: e4= numericScalarExpr
        {
        pushFollow(FOLLOW_numericScalarExpr_in_synpred34_wcps870);
        e4=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred34_wcps

    // $ANTLR start synpred35_wcps
    public final void synpred35_wcps_fragment() throws RecognitionException {   
        wcpsParser.stringScalarExpr_return e5 = null;


        // wcps.g:114:7: (e5= stringScalarExpr )
        // wcps.g:114:7: e5= stringScalarExpr
        {
        pushFollow(FOLLOW_stringScalarExpr_in_synpred35_wcps883);
        e5=stringScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred35_wcps

    // $ANTLR start synpred71_wcps
    public final void synpred71_wcps_fragment() throws RecognitionException {   
        wcpsParser.fieldExpr_return e6 = null;


        // wcps.g:215:7: (e6= fieldExpr )
        // wcps.g:215:7: e6= fieldExpr
        {
        pushFollow(FOLLOW_fieldExpr_in_synpred71_wcps2121);
        e6=fieldExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred71_wcps

    // $ANTLR start synpred72_wcps
    public final void synpred72_wcps_fragment() throws RecognitionException {   
        wcpsParser.unaryArithmeticExpr_return e1 = null;


        // wcps.g:216:4: (e1= unaryArithmeticExpr )
        // wcps.g:216:4: e1= unaryArithmeticExpr
        {
        pushFollow(FOLLOW_unaryArithmeticExpr_in_synpred72_wcps2130);
        e1=unaryArithmeticExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred72_wcps

    // $ANTLR start synpred75_wcps
    public final void synpred75_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanExpr_return e4 = null;


        // wcps.g:219:7: (e4= booleanExpr )
        // wcps.g:219:7: e4= booleanExpr
        {
        pushFollow(FOLLOW_booleanExpr_in_synpred75_wcps2166);
        e4=booleanExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred75_wcps

    // $ANTLR start synpred101_wcps
    public final void synpred101_wcps_fragment() throws RecognitionException {   
        wcpsParser.trimExpr_return e1 = null;


        // wcps.g:261:4: (e1= trimExpr )
        // wcps.g:261:4: e1= trimExpr
        {
        pushFollow(FOLLOW_trimExpr_in_synpred101_wcps2686);
        e1=trimExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred101_wcps

    // $ANTLR start synpred102_wcps
    public final void synpred102_wcps_fragment() throws RecognitionException {   
        wcpsParser.sliceExpr_return e2 = null;


        // wcps.g:262:4: (e2= sliceExpr )
        // wcps.g:262:4: e2= sliceExpr
        {
        pushFollow(FOLLOW_sliceExpr_in_synpred102_wcps2695);
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


        // wcps.g:290:8: (op= ( OR | XOR ) e2= booleanScalarTerm )
        // wcps.g:290:8: op= ( OR | XOR ) e2= booleanScalarTerm
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

        pushFollow(FOLLOW_booleanScalarTerm_in_synpred116_wcps2997);
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


        // wcps.g:294:5: (op= AND e2= booleanScalarNegation )
        // wcps.g:294:5: op= AND e2= booleanScalarNegation
        {
        op=(Token)match(input,AND,FOLLOW_AND_in_synpred117_wcps3029); if (state.failed) return ;
        pushFollow(FOLLOW_booleanScalarNegation_in_synpred117_wcps3033);
        e2=booleanScalarNegation();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred117_wcps

    // $ANTLR start synpred119_wcps
    public final void synpred119_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanScalarExpr_return e1 = null;


        // wcps.g:301:4: ( LPAREN e1= booleanScalarExpr RPAREN )
        // wcps.g:301:4: LPAREN e1= booleanScalarExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred119_wcps3082); if (state.failed) return ;
        pushFollow(FOLLOW_booleanScalarExpr_in_synpred119_wcps3086);
        e1=booleanScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred119_wcps3088); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred119_wcps

    // $ANTLR start synpred121_wcps
    public final void synpred121_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return n1 = null;

        wcpsParser.compOp_return cop = null;

        wcpsParser.numericScalarExpr_return n2 = null;


        // wcps.g:303:4: (n1= numericScalarExpr cop= compOp n2= numericScalarExpr )
        // wcps.g:303:4: n1= numericScalarExpr cop= compOp n2= numericScalarExpr
        {
        pushFollow(FOLLOW_numericScalarExpr_in_synpred121_wcps3115);
        n1=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        pushFollow(FOLLOW_compOp_in_synpred121_wcps3119);
        cop=compOp();

        state._fsp--;
        if (state.failed) return ;
        pushFollow(FOLLOW_numericScalarExpr_in_synpred121_wcps3123);
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


        // wcps.g:308:5: (op= ( PLUS | MINUS ) e2= numericScalarTerm )
        // wcps.g:308:5: op= ( PLUS | MINUS ) e2= numericScalarTerm
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

        pushFollow(FOLLOW_numericScalarTerm_in_synpred123_wcps3168);
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


        // wcps.g:312:4: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )
        // wcps.g:312:4: op= ( MULT | DIVIDE ) e2= numericScalarFactor
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

        pushFollow(FOLLOW_numericScalarFactor_in_synpred125_wcps3204);
        e2=numericScalarFactor();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred125_wcps

    // $ANTLR start synpred126_wcps
    public final void synpred126_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return e1 = null;


        // wcps.g:315:7: ( LPAREN e1= numericScalarExpr RPAREN )
        // wcps.g:315:7: LPAREN e1= numericScalarExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred126_wcps3224); if (state.failed) return ;
        pushFollow(FOLLOW_numericScalarExpr_in_synpred126_wcps3228);
        e1=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred126_wcps3230); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred126_wcps

    // $ANTLR start synpred133_wcps
    public final void synpred133_wcps_fragment() throws RecognitionException {   
        wcpsParser.complexConstant_return e2 = null;


        // wcps.g:322:7: (e2= complexConstant )
        // wcps.g:322:7: e2= complexConstant
        {
        pushFollow(FOLLOW_complexConstant_in_synpred133_wcps3342);
        e2=complexConstant();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred133_wcps

    // $ANTLR start synpred142_wcps
    public final void synpred142_wcps_fragment() throws RecognitionException {   
        wcpsParser.scalarExpr_return e1 = null;

        wcpsParser.scalarExpr_return e2 = null;


        // wcps.g:343:7: (e1= scalarExpr COLON e2= scalarExpr )
        // wcps.g:343:7: e1= scalarExpr COLON e2= scalarExpr
        {
        pushFollow(FOLLOW_scalarExpr_in_synpred142_wcps3530);
        e1=scalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,COLON,FOLLOW_COLON_in_synpred142_wcps3532); if (state.failed) return ;
        pushFollow(FOLLOW_scalarExpr_in_synpred142_wcps3536);
        e2=scalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred142_wcps

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
    public final boolean synpred75_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred75_wcps_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred133_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred133_wcps_fragment(); // can never throw exception
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
    public final boolean synpred142_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred142_wcps_fragment(); // can never throw exception
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
    public final boolean synpred72_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred72_wcps_fragment(); // can never throw exception
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
    protected DFA35 dfa35 = new DFA35(this);
    protected DFA43 dfa43 = new DFA43(this);
    protected DFA44 dfa44 = new DFA44(this);
    protected DFA45 dfa45 = new DFA45(this);
    protected DFA47 dfa47 = new DFA47(this);
    protected DFA48 dfa48 = new DFA48(this);
    protected DFA49 dfa49 = new DFA49(this);
    protected DFA50 dfa50 = new DFA50(this);
    protected DFA51 dfa51 = new DFA51(this);
    protected DFA52 dfa52 = new DFA52(this);
    protected DFA53 dfa53 = new DFA53(this);
    protected DFA57 dfa57 = new DFA57(this);
    static final String DFA4_eotS =
        "\30\uffff";
    static final String DFA4_eofS =
        "\30\uffff";
    static final String DFA4_minS =
        "\1\6\27\uffff";
    static final String DFA4_maxS =
        "\1\154\27\uffff";
    static final String DFA4_acceptS =
        "\1\uffff\1\1\1\2\1\3\24\uffff";
    static final String DFA4_specialS =
        "\30\uffff}>";
    static final String[] DFA4_transitionS = {
            "\1\3\4\uffff\1\1\1\2\12\uffff\1\3\3\uffff\20\3\21\uffff\2\3"+
            "\16\uffff\1\3\1\uffff\3\3\20\uffff\2\3\11\uffff\1\3",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "57:1: processingExpr returns [ProcessingExpr value] : (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr );";
        }
    }
    static final String DFA6_eotS =
        "\77\uffff";
    static final String DFA6_eofS =
        "\1\1\76\uffff";
    static final String DFA6_minS =
        "\1\7\10\uffff\1\0\65\uffff";
    static final String DFA6_maxS =
        "\1\140\10\uffff\1\0\65\uffff";
    static final String DFA6_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA6_specialS =
        "\11\uffff\1\0\65\uffff}>";
    static final String[] DFA6_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\2\11\14\1\21\uffff\2\1\3\uffff\1\1"+
            "\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
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
            return "()* loopback of 71:9: (op= ( OR | XOR ) e2= coverageLogicTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA6_9 = input.LA(1);

                         
                        int index6_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred8_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index6_9);
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
        "\77\uffff";
    static final String DFA7_eofS =
        "\1\1\76\uffff";
    static final String DFA7_minS =
        "\1\7\10\uffff\1\0\65\uffff";
    static final String DFA7_maxS =
        "\1\140\10\uffff\1\0\65\uffff";
    static final String DFA7_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA7_specialS =
        "\11\uffff\1\0\65\uffff}>";
    static final String[] DFA7_transitionS = {
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
            return "()* loopback of 75:9: (op= AND e2= coverageLogicFactor )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA7_9 = input.LA(1);

                         
                        int index7_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred9_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index7_9);
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
        "\u0107\uffff";
    static final String DFA8_eofS =
        "\1\2\u0106\uffff";
    static final String DFA8_minS =
        "\1\7\1\0\14\uffff\5\0\u00f4\uffff";
    static final String DFA8_maxS =
        "\1\140\1\0\14\uffff\5\0\u00f4\uffff";
    static final String DFA8_acceptS =
        "\2\uffff\1\2\73\uffff\1\1\u00c8\uffff";
    static final String DFA8_specialS =
        "\1\uffff\1\0\14\uffff\1\1\1\2\1\3\1\4\1\5\u00f4\uffff}>";
    static final String[] DFA8_transitionS = {
            "\2\2\1\uffff\1\2\2\uffff\3\2\1\1\1\16\1\17\1\20\1\21\1\22\5"+
            "\2\21\uffff\2\2\3\uffff\1\2\7\uffff\1\2\30\uffff\1\2\15\uffff"+
            "\1\2",
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
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "79:9: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?";
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
                        if ( (synpred15_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_1);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA8_14 = input.LA(1);

                         
                        int index8_14 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred15_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_14);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA8_15 = input.LA(1);

                         
                        int index8_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred15_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_15);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA8_16 = input.LA(1);

                         
                        int index8_16 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred15_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_16);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA8_17 = input.LA(1);

                         
                        int index8_17 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred15_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_17);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA8_18 = input.LA(1);

                         
                        int index8_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred15_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 2;}

                         
                        input.seek(index8_18);
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
        "\77\uffff";
    static final String DFA9_eofS =
        "\1\1\76\uffff";
    static final String DFA9_minS =
        "\1\7\10\uffff\1\0\65\uffff";
    static final String DFA9_maxS =
        "\1\140\10\uffff\1\0\65\uffff";
    static final String DFA9_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA9_specialS =
        "\11\uffff\1\0\65\uffff}>";
    static final String[] DFA9_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\11\1\2\11\3\1\21\uffff\2\1\3\uffff"+
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
            return "()* loopback of 83:9: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA9_9 = input.LA(1);

                         
                        int index9_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred17_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index9_9);
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
        "\77\uffff";
    static final String DFA10_eofS =
        "\1\1\76\uffff";
    static final String DFA10_minS =
        "\1\7\10\uffff\1\0\65\uffff";
    static final String DFA10_maxS =
        "\1\140\10\uffff\1\0\65\uffff";
    static final String DFA10_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA10_specialS =
        "\11\uffff\1\0\65\uffff}>";
    static final String[] DFA10_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\13\1\2\11\1\1\21\uffff\2\1\3\uffff"+
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
            return "()* loopback of 87:9: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA10_9 = input.LA(1);

                         
                        int index10_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred19_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index10_9);
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
        "\77\uffff";
    static final String DFA11_eofS =
        "\1\1\76\uffff";
    static final String DFA11_minS =
        "\1\7\10\uffff\1\0\65\uffff";
    static final String DFA11_maxS =
        "\1\140\10\uffff\1\0\65\uffff";
    static final String DFA11_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA11_specialS =
        "\11\uffff\1\0\65\uffff}>";
    static final String[] DFA11_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\15\1\1\11\21\uffff\2\1\3\uffff\1\1"+
            "\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
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
            return "()* loopback of 91:9: (op= OVERLAY e2= coverageValue )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA11_9 = input.LA(1);

                         
                        int index11_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred20_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index11_9);
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
        "\u012d\uffff";
    static final String DFA12_eofS =
        "\u012d\uffff";
    static final String DFA12_minS =
        "\12\6\1\15\1\6\1\0\4\6\4\0\1\6\1\0\1\116\5\6\1\70\1\116\12\uffff"+
        "\62\0\30\uffff\12\0\24\uffff\3\0\124\uffff\15\0\60\uffff\10\0";
    static final String DFA12_maxS =
        "\1\154\11\6\1\51\1\154\1\0\1\154\3\6\4\0\1\154\1\0\1\153\5\6\1\70"+
        "\1\153\12\uffff\62\0\30\uffff\12\0\24\uffff\3\0\124\uffff\15\0\60"+
        "\uffff\10\0";
    static final String DFA12_acceptS =
        "\37\uffff\1\1\2\uffff\1\2\4\uffff\1\3\1\4\111\uffff\1\5\u00ba\uffff";
    static final String DFA12_specialS =
        "\14\uffff\1\0\4\uffff\1\1\1\2\1\3\1\4\1\uffff\1\5\22\uffff\1\6\1"+
        "\7\1\10\1\11\1\12\1\13\1\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1"+
        "\24\1\25\1\26\1\27\1\30\1\31\1\32\1\33\1\34\1\35\1\36\1\37\1\40"+
        "\1\41\1\42\1\43\1\44\1\45\1\46\1\47\1\50\1\51\1\52\1\53\1\54\1\55"+
        "\1\56\1\57\1\60\1\61\1\62\1\63\1\64\1\65\1\66\1\67\30\uffff\1\70"+
        "\1\71\1\72\1\73\1\74\1\75\1\76\1\77\1\100\1\101\24\uffff\1\102\1"+
        "\103\1\104\124\uffff\1\105\1\106\1\107\1\110\1\111\1\112\1\113\1"+
        "\114\1\115\1\116\1\117\1\120\1\121\60\uffff\1\122\1\123\1\124\1"+
        "\125\1\126\1\127\1\130\1\131}>";
    static final String[] DFA12_transitionS = {
            "\1\13\17\uffff\1\42\1\15\3\uffff\1\1\1\2\1\3\1\4\1\6\1\7\1\10"+
            "\1\5\7\11\1\12\3\uffff\1\27\4\uffff\1\30\1\31\1\32\1\33\1\34"+
            "\1\36\1\uffff\1\35\1\50\1\17\1\16\16\42\1\25\1\42\1\21\1\20"+
            "\1\14\1\47\2\uffff\3\37\12\uffff\1\24\1\22\10\uffff\1\26\1\23",
            "\1\51",
            "\1\52",
            "\1\53",
            "\1\54",
            "\1\55",
            "\1\56",
            "\1\57",
            "\1\60",
            "\1\61",
            "\1\62\1\uffff\1\62\6\uffff\1\62\1\uffff\1\62\17\uffff\2\62",
            "\1\64\17\uffff\1\124\1\67\3\uffff\1\65\1\101\1\102\1\103\1"+
            "\105\1\106\1\107\1\104\7\74\1\75\3\uffff\1\111\4\uffff\1\112"+
            "\1\113\1\114\1\115\1\116\1\120\1\uffff\1\117\1\132\1\71\1\70"+
            "\2\125\3\126\11\127\1\100\1\130\1\73\1\72\1\66\1\131\2\uffff"+
            "\1\121\1\122\1\123\11\42\1\uffff\1\77\1\63\10\uffff\1\110\1"+
            "\76",
            "\1\uffff",
            "\1\163\20\uffff\1\164\3\uffff\10\42\7\172\1\173\3\uffff\1\42"+
            "\4\uffff\6\42\1\uffff\1\42\1\uffff\1\166\1\165\16\uffff\1\42"+
            "\1\uffff\1\170\1\167\1\42\20\uffff\1\42\1\171\10\uffff\1\42"+
            "\1\174",
            "\1\u0091",
            "\1\u0092",
            "\1\u0093",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\u00e8\17\uffff\1\42\1\u00eb\3\uffff\1\u00e9\7\42\7\u00f1"+
            "\1\u00f2\3\uffff\1\42\4\uffff\6\42\1\uffff\2\42\1\u00ed\1\u00ec"+
            "\20\42\1\u00ef\1\u00ee\1\u00ea\1\42\2\uffff\3\42\12\uffff\1"+
            "\u00f4\1\u00f0\10\uffff\1\42\1\u00f3",
            "\1\uffff",
            "\1\u0125\1\uffff\1\u0125\32\uffff\1\u0125",
            "\1\u0126",
            "\1\u0127",
            "\1\u0128",
            "\1\u0129",
            "\1\u012a",
            "\1\u012b",
            "\1\u012c\1\uffff\1\u012c\32\uffff\1\u012c",
            "",
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
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "93:1: coverageValue returns [CoverageExpr value] : (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA12_12 = input.LA(1);

                         
                        int index12_12 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_12);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA12_17 = input.LA(1);

                         
                        int index12_17 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_17);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA12_18 = input.LA(1);

                         
                        int index12_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_18);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA12_19 = input.LA(1);

                         
                        int index12_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_19);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA12_20 = input.LA(1);

                         
                        int index12_20 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_20);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA12_22 = input.LA(1);

                         
                        int index12_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_22);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA12_41 = input.LA(1);

                         
                        int index12_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_41);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA12_42 = input.LA(1);

                         
                        int index12_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_42);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA12_43 = input.LA(1);

                         
                        int index12_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_43);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA12_44 = input.LA(1);

                         
                        int index12_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_44);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA12_45 = input.LA(1);

                         
                        int index12_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_45);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA12_46 = input.LA(1);

                         
                        int index12_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_46);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA12_47 = input.LA(1);

                         
                        int index12_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_47);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA12_48 = input.LA(1);

                         
                        int index12_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_48);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA12_49 = input.LA(1);

                         
                        int index12_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_49);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA12_50 = input.LA(1);

                         
                        int index12_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_50);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA12_51 = input.LA(1);

                         
                        int index12_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_51);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA12_52 = input.LA(1);

                         
                        int index12_52 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_52);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA12_53 = input.LA(1);

                         
                        int index12_53 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_53);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA12_54 = input.LA(1);

                         
                        int index12_54 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_54);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA12_55 = input.LA(1);

                         
                        int index12_55 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_55);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA12_56 = input.LA(1);

                         
                        int index12_56 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_56);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA12_57 = input.LA(1);

                         
                        int index12_57 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_57);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA12_58 = input.LA(1);

                         
                        int index12_58 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_58);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA12_59 = input.LA(1);

                         
                        int index12_59 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_59);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA12_60 = input.LA(1);

                         
                        int index12_60 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_60);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA12_61 = input.LA(1);

                         
                        int index12_61 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_61);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA12_62 = input.LA(1);

                         
                        int index12_62 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_62);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA12_63 = input.LA(1);

                         
                        int index12_63 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_63);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA12_64 = input.LA(1);

                         
                        int index12_64 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_64);
                        if ( s>=0 ) return s;
                        break;
                    case 30 : 
                        int LA12_65 = input.LA(1);

                         
                        int index12_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_65);
                        if ( s>=0 ) return s;
                        break;
                    case 31 : 
                        int LA12_66 = input.LA(1);

                         
                        int index12_66 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_66);
                        if ( s>=0 ) return s;
                        break;
                    case 32 : 
                        int LA12_67 = input.LA(1);

                         
                        int index12_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_67);
                        if ( s>=0 ) return s;
                        break;
                    case 33 : 
                        int LA12_68 = input.LA(1);

                         
                        int index12_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_68);
                        if ( s>=0 ) return s;
                        break;
                    case 34 : 
                        int LA12_69 = input.LA(1);

                         
                        int index12_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_69);
                        if ( s>=0 ) return s;
                        break;
                    case 35 : 
                        int LA12_70 = input.LA(1);

                         
                        int index12_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_70);
                        if ( s>=0 ) return s;
                        break;
                    case 36 : 
                        int LA12_71 = input.LA(1);

                         
                        int index12_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_71);
                        if ( s>=0 ) return s;
                        break;
                    case 37 : 
                        int LA12_72 = input.LA(1);

                         
                        int index12_72 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_72);
                        if ( s>=0 ) return s;
                        break;
                    case 38 : 
                        int LA12_73 = input.LA(1);

                         
                        int index12_73 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_73);
                        if ( s>=0 ) return s;
                        break;
                    case 39 : 
                        int LA12_74 = input.LA(1);

                         
                        int index12_74 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_74);
                        if ( s>=0 ) return s;
                        break;
                    case 40 : 
                        int LA12_75 = input.LA(1);

                         
                        int index12_75 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_75);
                        if ( s>=0 ) return s;
                        break;
                    case 41 : 
                        int LA12_76 = input.LA(1);

                         
                        int index12_76 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_76);
                        if ( s>=0 ) return s;
                        break;
                    case 42 : 
                        int LA12_77 = input.LA(1);

                         
                        int index12_77 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_77);
                        if ( s>=0 ) return s;
                        break;
                    case 43 : 
                        int LA12_78 = input.LA(1);

                         
                        int index12_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_78);
                        if ( s>=0 ) return s;
                        break;
                    case 44 : 
                        int LA12_79 = input.LA(1);

                         
                        int index12_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_79);
                        if ( s>=0 ) return s;
                        break;
                    case 45 : 
                        int LA12_80 = input.LA(1);

                         
                        int index12_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_80);
                        if ( s>=0 ) return s;
                        break;
                    case 46 : 
                        int LA12_81 = input.LA(1);

                         
                        int index12_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_81);
                        if ( s>=0 ) return s;
                        break;
                    case 47 : 
                        int LA12_82 = input.LA(1);

                         
                        int index12_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_82);
                        if ( s>=0 ) return s;
                        break;
                    case 48 : 
                        int LA12_83 = input.LA(1);

                         
                        int index12_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_83);
                        if ( s>=0 ) return s;
                        break;
                    case 49 : 
                        int LA12_84 = input.LA(1);

                         
                        int index12_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_84);
                        if ( s>=0 ) return s;
                        break;
                    case 50 : 
                        int LA12_85 = input.LA(1);

                         
                        int index12_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_85);
                        if ( s>=0 ) return s;
                        break;
                    case 51 : 
                        int LA12_86 = input.LA(1);

                         
                        int index12_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_86);
                        if ( s>=0 ) return s;
                        break;
                    case 52 : 
                        int LA12_87 = input.LA(1);

                         
                        int index12_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_87);
                        if ( s>=0 ) return s;
                        break;
                    case 53 : 
                        int LA12_88 = input.LA(1);

                         
                        int index12_88 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_88);
                        if ( s>=0 ) return s;
                        break;
                    case 54 : 
                        int LA12_89 = input.LA(1);

                         
                        int index12_89 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_89);
                        if ( s>=0 ) return s;
                        break;
                    case 55 : 
                        int LA12_90 = input.LA(1);

                         
                        int index12_90 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_90);
                        if ( s>=0 ) return s;
                        break;
                    case 56 : 
                        int LA12_115 = input.LA(1);

                         
                        int index12_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_115);
                        if ( s>=0 ) return s;
                        break;
                    case 57 : 
                        int LA12_116 = input.LA(1);

                         
                        int index12_116 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_116);
                        if ( s>=0 ) return s;
                        break;
                    case 58 : 
                        int LA12_117 = input.LA(1);

                         
                        int index12_117 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_117);
                        if ( s>=0 ) return s;
                        break;
                    case 59 : 
                        int LA12_118 = input.LA(1);

                         
                        int index12_118 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_118);
                        if ( s>=0 ) return s;
                        break;
                    case 60 : 
                        int LA12_119 = input.LA(1);

                         
                        int index12_119 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_119);
                        if ( s>=0 ) return s;
                        break;
                    case 61 : 
                        int LA12_120 = input.LA(1);

                         
                        int index12_120 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_120);
                        if ( s>=0 ) return s;
                        break;
                    case 62 : 
                        int LA12_121 = input.LA(1);

                         
                        int index12_121 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_121);
                        if ( s>=0 ) return s;
                        break;
                    case 63 : 
                        int LA12_122 = input.LA(1);

                         
                        int index12_122 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_122);
                        if ( s>=0 ) return s;
                        break;
                    case 64 : 
                        int LA12_123 = input.LA(1);

                         
                        int index12_123 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_123);
                        if ( s>=0 ) return s;
                        break;
                    case 65 : 
                        int LA12_124 = input.LA(1);

                         
                        int index12_124 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_124);
                        if ( s>=0 ) return s;
                        break;
                    case 66 : 
                        int LA12_145 = input.LA(1);

                         
                        int index12_145 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_145);
                        if ( s>=0 ) return s;
                        break;
                    case 67 : 
                        int LA12_146 = input.LA(1);

                         
                        int index12_146 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_146);
                        if ( s>=0 ) return s;
                        break;
                    case 68 : 
                        int LA12_147 = input.LA(1);

                         
                        int index12_147 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_147);
                        if ( s>=0 ) return s;
                        break;
                    case 69 : 
                        int LA12_232 = input.LA(1);

                         
                        int index12_232 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_232);
                        if ( s>=0 ) return s;
                        break;
                    case 70 : 
                        int LA12_233 = input.LA(1);

                         
                        int index12_233 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_233);
                        if ( s>=0 ) return s;
                        break;
                    case 71 : 
                        int LA12_234 = input.LA(1);

                         
                        int index12_234 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_234);
                        if ( s>=0 ) return s;
                        break;
                    case 72 : 
                        int LA12_235 = input.LA(1);

                         
                        int index12_235 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_235);
                        if ( s>=0 ) return s;
                        break;
                    case 73 : 
                        int LA12_236 = input.LA(1);

                         
                        int index12_236 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_236);
                        if ( s>=0 ) return s;
                        break;
                    case 74 : 
                        int LA12_237 = input.LA(1);

                         
                        int index12_237 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_237);
                        if ( s>=0 ) return s;
                        break;
                    case 75 : 
                        int LA12_238 = input.LA(1);

                         
                        int index12_238 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_238);
                        if ( s>=0 ) return s;
                        break;
                    case 76 : 
                        int LA12_239 = input.LA(1);

                         
                        int index12_239 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_239);
                        if ( s>=0 ) return s;
                        break;
                    case 77 : 
                        int LA12_240 = input.LA(1);

                         
                        int index12_240 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_240);
                        if ( s>=0 ) return s;
                        break;
                    case 78 : 
                        int LA12_241 = input.LA(1);

                         
                        int index12_241 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_241);
                        if ( s>=0 ) return s;
                        break;
                    case 79 : 
                        int LA12_242 = input.LA(1);

                         
                        int index12_242 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_242);
                        if ( s>=0 ) return s;
                        break;
                    case 80 : 
                        int LA12_243 = input.LA(1);

                         
                        int index12_243 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_243);
                        if ( s>=0 ) return s;
                        break;
                    case 81 : 
                        int LA12_244 = input.LA(1);

                         
                        int index12_244 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_244);
                        if ( s>=0 ) return s;
                        break;
                    case 82 : 
                        int LA12_293 = input.LA(1);

                         
                        int index12_293 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_293);
                        if ( s>=0 ) return s;
                        break;
                    case 83 : 
                        int LA12_294 = input.LA(1);

                         
                        int index12_294 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_294);
                        if ( s>=0 ) return s;
                        break;
                    case 84 : 
                        int LA12_295 = input.LA(1);

                         
                        int index12_295 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_295);
                        if ( s>=0 ) return s;
                        break;
                    case 85 : 
                        int LA12_296 = input.LA(1);

                         
                        int index12_296 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_296);
                        if ( s>=0 ) return s;
                        break;
                    case 86 : 
                        int LA12_297 = input.LA(1);

                         
                        int index12_297 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_297);
                        if ( s>=0 ) return s;
                        break;
                    case 87 : 
                        int LA12_298 = input.LA(1);

                         
                        int index12_298 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_298);
                        if ( s>=0 ) return s;
                        break;
                    case 88 : 
                        int LA12_299 = input.LA(1);

                         
                        int index12_299 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_299);
                        if ( s>=0 ) return s;
                        break;
                    case 89 : 
                        int LA12_300 = input.LA(1);

                         
                        int index12_300 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 114;}

                         
                        input.seek(index12_300);
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
        "\112\uffff";
    static final String DFA13_eofS =
        "\112\uffff";
    static final String DFA13_minS =
        "\1\6\12\uffff\1\6\13\uffff\1\116\7\uffff\25\0\23\uffff\1\0\2\uffff";
    static final String DFA13_maxS =
        "\1\154\12\uffff\1\154\13\uffff\1\153\7\uffff\25\0\23\uffff\1\0\2"+
        "\uffff";
    static final String DFA13_acceptS =
        "\1\uffff\1\1\24\uffff\1\2\1\uffff\1\6\4\uffff\1\7\26\uffff\1\3\23"+
        "\uffff\1\4\1\5";
    static final String DFA13_specialS =
        "\37\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\1"+
        "\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\23\uffff\1\25\2\uffff}>";
    static final String[] DFA13_transitionS = {
            "\1\13\20\uffff\1\1\3\uffff\20\1\3\uffff\1\27\4\uffff\5\30\1"+
            "\35\1\uffff\1\35\1\uffff\2\1\16\uffff\1\1\1\uffff\3\1\20\uffff"+
            "\2\1\10\uffff\1\26\1\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\40\17\uffff\1\64\1\43\3\uffff\1\41\1\55\1\56\1\57\1\61\1"+
            "\62\1\63\1\60\7\50\1\51\3\uffff\1\64\4\uffff\6\64\1\uffff\2"+
            "\64\1\45\1\44\16\64\1\54\1\64\1\47\1\46\1\42\1\64\2\uffff\3"+
            "\64\12\uffff\1\53\1\37\10\uffff\1\64\1\52",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\107\1\uffff\1\107\32\uffff\1\107",
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
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "100:1: coverageAtom returns [CoverageExpr value] : (e2= scalarExpr | e1= coverageVariable | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA13_31 = input.LA(1);

                         
                        int index13_31 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_31);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA13_32 = input.LA(1);

                         
                        int index13_32 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_32);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA13_33 = input.LA(1);

                         
                        int index13_33 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_33);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA13_34 = input.LA(1);

                         
                        int index13_34 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_34);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA13_35 = input.LA(1);

                         
                        int index13_35 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_35);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA13_36 = input.LA(1);

                         
                        int index13_36 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_36);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA13_37 = input.LA(1);

                         
                        int index13_37 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_37);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA13_38 = input.LA(1);

                         
                        int index13_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_38);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA13_39 = input.LA(1);

                         
                        int index13_39 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_39);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA13_40 = input.LA(1);

                         
                        int index13_40 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_40);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA13_41 = input.LA(1);

                         
                        int index13_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_41);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA13_42 = input.LA(1);

                         
                        int index13_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_42);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA13_43 = input.LA(1);

                         
                        int index13_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_43);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA13_44 = input.LA(1);

                         
                        int index13_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_44);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA13_45 = input.LA(1);

                         
                        int index13_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_45);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA13_46 = input.LA(1);

                         
                        int index13_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_46);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA13_47 = input.LA(1);

                         
                        int index13_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_47);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA13_48 = input.LA(1);

                         
                        int index13_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_48);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA13_49 = input.LA(1);

                         
                        int index13_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_49);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA13_50 = input.LA(1);

                         
                        int index13_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_50);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA13_51 = input.LA(1);

                         
                        int index13_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred25_wcps()) ) {s = 1;}

                        else if ( (synpred27_wcps()) ) {s = 52;}

                         
                        input.seek(index13_51);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA13_71 = input.LA(1);

                         
                        int index13_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred28_wcps()) ) {s = 72;}

                        else if ( (synpred29_wcps()) ) {s = 73;}

                         
                        input.seek(index13_71);
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
        "\u0091\uffff";
    static final String DFA14_eofS =
        "\14\uffff\1\64\u0084\uffff";
    static final String DFA14_minS =
        "\2\6\7\uffff\1\6\1\15\1\6\1\7\4\6\3\0\2\uffff\21\0\7\uffff\6\0\17"+
        "\uffff\15\0\101\uffff";
    static final String DFA14_maxS =
        "\1\154\1\6\7\uffff\1\6\1\51\1\154\1\140\1\154\3\6\3\0\2\uffff\21"+
        "\0\7\uffff\6\0\17\uffff\15\0\101\uffff";
    static final String DFA14_acceptS =
        "\2\uffff\1\1\21\uffff\1\3\22\uffff\1\6\14\uffff\1\5\60\uffff\1\4"+
        "\52\uffff\1\2";
    static final String DFA14_specialS =
        "\21\uffff\1\0\1\1\1\2\2\uffff\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12"+
        "\1\13\1\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\7\uffff\1\24\1\25"+
        "\1\26\1\27\1\30\1\31\17\uffff\1\32\1\33\1\34\1\35\1\36\1\37\1\40"+
        "\1\41\1\42\1\43\1\44\1\45\1\46\101\uffff}>";
    static final String[] DFA14_transitionS = {
            "\1\13\20\uffff\1\15\3\uffff\1\1\7\2\7\11\1\12\21\uffff\1\17"+
            "\1\16\16\uffff\1\24\1\uffff\1\21\1\20\1\14\20\uffff\1\24\1\22"+
            "\11\uffff\1\23",
            "\1\26",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\27",
            "\1\30\1\uffff\1\30\6\uffff\1\30\1\uffff\1\30\17\uffff\2\30",
            "\1\32\20\uffff\1\35\3\uffff\1\33\7\47\7\42\1\43\21\uffff\1"+
            "\37\1\36\16\uffff\1\46\1\uffff\1\41\1\40\1\34\20\uffff\1\45"+
            "\1\31\11\uffff\1\44",
            "\2\64\1\uffff\1\64\2\uffff\3\64\1\56\1\57\1\60\1\61\1\62\1"+
            "\63\5\64\21\uffff\2\64\3\uffff\1\64\7\uffff\1\64\30\uffff\1"+
            "\64\15\uffff\1\64",
            "\1\103\20\uffff\1\104\13\uffff\7\112\1\113\21\uffff\1\106\1"+
            "\105\20\uffff\1\110\1\107\22\uffff\1\111\11\uffff\1\114",
            "\1\115",
            "\1\116",
            "\1\117",
            "\1\uffff",
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
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "109:1: scalarExpr returns [ScalarExpr value] : (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA14_17 = input.LA(1);

                         
                        int index14_17 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_17);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA14_18 = input.LA(1);

                         
                        int index14_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_18);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA14_19 = input.LA(1);

                         
                        int index14_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_19);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA14_22 = input.LA(1);

                         
                        int index14_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred31_wcps()) ) {s = 2;}

                        else if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_22);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA14_23 = input.LA(1);

                         
                        int index14_23 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred32_wcps()) ) {s = 144;}

                        else if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_23);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA14_24 = input.LA(1);

                         
                        int index14_24 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred32_wcps()) ) {s = 144;}

                        else if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_24);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA14_25 = input.LA(1);

                         
                        int index14_25 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_25);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA14_26 = input.LA(1);

                         
                        int index14_26 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_26);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA14_27 = input.LA(1);

                         
                        int index14_27 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_27);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA14_28 = input.LA(1);

                         
                        int index14_28 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_28);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA14_29 = input.LA(1);

                         
                        int index14_29 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_29);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA14_30 = input.LA(1);

                         
                        int index14_30 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_30);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA14_31 = input.LA(1);

                         
                        int index14_31 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_31);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA14_32 = input.LA(1);

                         
                        int index14_32 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_32);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA14_33 = input.LA(1);

                         
                        int index14_33 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_33);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA14_34 = input.LA(1);

                         
                        int index14_34 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_34);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA14_35 = input.LA(1);

                         
                        int index14_35 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_35);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA14_36 = input.LA(1);

                         
                        int index14_36 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_36);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA14_37 = input.LA(1);

                         
                        int index14_37 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_37);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA14_38 = input.LA(1);

                         
                        int index14_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (true) ) {s = 39;}

                         
                        input.seek(index14_38);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA14_46 = input.LA(1);

                         
                        int index14_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_46);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA14_47 = input.LA(1);

                         
                        int index14_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_47);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA14_48 = input.LA(1);

                         
                        int index14_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_48);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA14_49 = input.LA(1);

                         
                        int index14_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_49);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA14_50 = input.LA(1);

                         
                        int index14_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_50);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA14_51 = input.LA(1);

                         
                        int index14_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred35_wcps()) ) {s = 52;}

                         
                        input.seek(index14_51);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA14_67 = input.LA(1);

                         
                        int index14_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_67);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA14_68 = input.LA(1);

                         
                        int index14_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_68);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA14_69 = input.LA(1);

                         
                        int index14_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_69);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA14_70 = input.LA(1);

                         
                        int index14_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_70);
                        if ( s>=0 ) return s;
                        break;
                    case 30 : 
                        int LA14_71 = input.LA(1);

                         
                        int index14_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_71);
                        if ( s>=0 ) return s;
                        break;
                    case 31 : 
                        int LA14_72 = input.LA(1);

                         
                        int index14_72 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_72);
                        if ( s>=0 ) return s;
                        break;
                    case 32 : 
                        int LA14_73 = input.LA(1);

                         
                        int index14_73 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_73);
                        if ( s>=0 ) return s;
                        break;
                    case 33 : 
                        int LA14_74 = input.LA(1);

                         
                        int index14_74 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_74);
                        if ( s>=0 ) return s;
                        break;
                    case 34 : 
                        int LA14_75 = input.LA(1);

                         
                        int index14_75 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_75);
                        if ( s>=0 ) return s;
                        break;
                    case 35 : 
                        int LA14_76 = input.LA(1);

                         
                        int index14_76 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_76);
                        if ( s>=0 ) return s;
                        break;
                    case 36 : 
                        int LA14_77 = input.LA(1);

                         
                        int index14_77 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_77);
                        if ( s>=0 ) return s;
                        break;
                    case 37 : 
                        int LA14_78 = input.LA(1);

                         
                        int index14_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_78);
                        if ( s>=0 ) return s;
                        break;
                    case 38 : 
                        int LA14_79 = input.LA(1);

                         
                        int index14_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred33_wcps()) ) {s = 20;}

                        else if ( (synpred34_wcps()) ) {s = 101;}

                         
                        input.seek(index14_79);
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
    static final String DFA35_eotS =
        "\u0099\uffff";
    static final String DFA35_eofS =
        "\u0099\uffff";
    static final String DFA35_minS =
        "\1\6\12\uffff\1\6\1\uffff\3\6\5\uffff\1\6\7\uffff\1\70\1\116\57"+
        "\uffff\12\0\24\uffff\17\0\33\uffff\2\0\1\uffff";
    static final String DFA35_maxS =
        "\1\154\12\uffff\1\154\1\uffff\1\154\2\6\5\uffff\1\154\7\uffff\1"+
        "\70\1\153\57\uffff\12\0\24\uffff\17\0\33\uffff\2\0\1\uffff";
    static final String DFA35_acceptS =
        "\1\uffff\1\1\35\uffff\1\2\1\uffff\1\3\1\4\1\5\12\uffff\1\6\151\uffff"+
        "\1\7";
    static final String DFA35_specialS =
        "\116\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\24\uffff\1"+
        "\12\1\13\1\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\1\25\1\26"+
        "\1\27\1\30\33\uffff\1\31\1\32\1\uffff}>";
    static final String[] DFA35_transitionS = {
            "\1\13\17\uffff\1\37\1\15\3\uffff\20\1\3\uffff\1\1\4\uffff\5"+
            "\1\1\36\1\uffff\1\35\1\uffff\1\17\1\16\2\37\3\41\11\42\1\25"+
            "\1\43\3\1\20\uffff\2\1\10\uffff\2\1",
            "",
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
            "\30\1\2\uffff\3\1\11\56\1\uffff\2\1\10\uffff\2\1",
            "",
            "\1\116\20\uffff\1\117\3\uffff\10\37\7\125\1\126\3\uffff\1\37"+
            "\4\uffff\6\37\1\uffff\1\37\1\uffff\1\121\1\120\16\uffff\1\37"+
            "\1\uffff\1\123\1\122\1\37\20\uffff\1\37\1\124\10\uffff\1\37"+
            "\1\127",
            "\1\154",
            "\1\155",
            "",
            "",
            "",
            "",
            "",
            "\1\156\17\uffff\1\43\1\161\3\uffff\1\157\7\43\7\167\1\170\3"+
            "\uffff\1\43\4\uffff\6\43\1\uffff\2\43\1\163\1\162\20\43\1\165"+
            "\1\164\1\160\1\43\2\uffff\3\43\12\uffff\1\172\1\166\10\uffff"+
            "\1\43\1\171",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0096",
            "\1\u0097\1\uffff\1\u0097\32\uffff\1\u0097",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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

    static final short[] DFA35_eot = DFA.unpackEncodedString(DFA35_eotS);
    static final short[] DFA35_eof = DFA.unpackEncodedString(DFA35_eofS);
    static final char[] DFA35_min = DFA.unpackEncodedStringToUnsignedChars(DFA35_minS);
    static final char[] DFA35_max = DFA.unpackEncodedStringToUnsignedChars(DFA35_maxS);
    static final short[] DFA35_accept = DFA.unpackEncodedString(DFA35_acceptS);
    static final short[] DFA35_special = DFA.unpackEncodedString(DFA35_specialS);
    static final short[][] DFA35_transition;

    static {
        int numStates = DFA35_transitionS.length;
        DFA35_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA35_transition[i] = DFA.unpackEncodedString(DFA35_transitionS[i]);
        }
    }

    class DFA35 extends DFA {

        public DFA35(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 35;
            this.eot = DFA35_eot;
            this.eof = DFA35_eof;
            this.min = DFA35_min;
            this.max = DFA35_max;
            this.accept = DFA35_accept;
            this.special = DFA35_special;
            this.transition = DFA35_transition;
        }
        public String getDescription() {
            return "214:1: unaryInducedExpr returns [CoverageExpr value] : (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA35_78 = input.LA(1);

                         
                        int index35_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_78);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA35_79 = input.LA(1);

                         
                        int index35_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_79);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA35_80 = input.LA(1);

                         
                        int index35_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_80);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA35_81 = input.LA(1);

                         
                        int index35_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_81);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA35_82 = input.LA(1);

                         
                        int index35_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_82);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA35_83 = input.LA(1);

                         
                        int index35_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_83);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA35_84 = input.LA(1);

                         
                        int index35_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_84);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA35_85 = input.LA(1);

                         
                        int index35_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_85);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA35_86 = input.LA(1);

                         
                        int index35_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_86);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA35_87 = input.LA(1);

                         
                        int index35_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_87);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA35_108 = input.LA(1);

                         
                        int index35_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_108);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA35_109 = input.LA(1);

                         
                        int index35_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred72_wcps()) ) {s = 31;}

                         
                        input.seek(index35_109);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA35_110 = input.LA(1);

                         
                        int index35_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_110);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA35_111 = input.LA(1);

                         
                        int index35_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_111);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA35_112 = input.LA(1);

                         
                        int index35_112 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_112);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA35_113 = input.LA(1);

                         
                        int index35_113 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_113);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA35_114 = input.LA(1);

                         
                        int index35_114 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_114);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA35_115 = input.LA(1);

                         
                        int index35_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_115);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA35_116 = input.LA(1);

                         
                        int index35_116 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_116);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA35_117 = input.LA(1);

                         
                        int index35_117 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_117);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA35_118 = input.LA(1);

                         
                        int index35_118 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_118);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA35_119 = input.LA(1);

                         
                        int index35_119 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_119);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA35_120 = input.LA(1);

                         
                        int index35_120 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_120);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA35_121 = input.LA(1);

                         
                        int index35_121 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_121);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA35_122 = input.LA(1);

                         
                        int index35_122 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (synpred75_wcps()) ) {s = 35;}

                         
                        input.seek(index35_122);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA35_150 = input.LA(1);

                         
                        int index35_150 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 152;}

                         
                        input.seek(index35_150);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA35_151 = input.LA(1);

                         
                        int index35_151 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred71_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 152;}

                         
                        input.seek(index35_151);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 35, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA43_eotS =
        "\u009c\uffff";
    static final String DFA43_eofS =
        "\u009c\uffff";
    static final String DFA43_minS =
        "\12\6\1\15\1\6\1\20\4\6\3\20\1\15\1\6\1\122\1\116\5\6\1\70\1\116"+
        "\3\uffff\172\0";
    static final String DFA43_maxS =
        "\1\154\11\6\1\51\1\154\1\122\1\154\3\6\4\122\1\154\1\122\1\153\5"+
        "\6\1\70\1\153\3\uffff\172\0";
    static final String DFA43_acceptS =
        "\37\uffff\1\1\1\2\1\3\172\uffff";
    static final String DFA43_specialS =
        "\42\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\1"+
        "\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\1\25\1\26\1\27\1\30"+
        "\1\31\1\32\1\33\1\34\1\35\1\36\1\37\1\40\1\41\1\42\1\43\1\44\1\45"+
        "\1\46\1\47\1\50\1\51\1\52\1\53\1\54\1\55\1\56\1\57\1\60\1\61\1\62"+
        "\1\63\1\64\1\65\1\66\1\67\1\70\1\71\1\72\1\73\1\74\1\75\1\76\1\77"+
        "\1\100\1\101\1\102\1\103\1\104\1\105\1\106\1\107\1\110\1\111\1\112"+
        "\1\113\1\114\1\115\1\116\1\117\1\120\1\121\1\122\1\123\1\124\1\125"+
        "\1\126\1\127\1\130\1\131\1\132\1\133\1\134\1\135\1\136\1\137\1\140"+
        "\1\141\1\142\1\143\1\144\1\145\1\146\1\147\1\150\1\151\1\152\1\153"+
        "\1\154\1\155\1\156\1\157\1\160\1\161\1\162\1\163\1\164\1\165\1\166"+
        "\1\167\1\170\1\171}>";
    static final String[] DFA43_transitionS = {
            "\1\13\20\uffff\1\15\3\uffff\1\1\1\2\1\3\1\4\1\6\1\7\1\10\1\5"+
            "\7\11\1\12\3\uffff\1\27\4\uffff\1\30\1\31\1\32\1\33\1\34\1\36"+
            "\1\uffff\1\35\1\uffff\1\17\1\16\16\uffff\1\25\1\uffff\1\21\1"+
            "\20\1\14\3\uffff\1\37\1\40\1\41\12\uffff\1\24\1\22\10\uffff"+
            "\1\26\1\23",
            "\1\42",
            "\1\43",
            "\1\44",
            "\1\45",
            "\1\46",
            "\1\47",
            "\1\50",
            "\1\51",
            "\1\52",
            "\1\53\1\uffff\1\53\6\uffff\1\53\1\uffff\1\53\17\uffff\2\53",
            "\1\67\17\uffff\1\115\1\71\3\uffff\1\55\1\56\1\57\1\60\1\62"+
            "\1\63\1\64\1\61\7\65\1\66\3\uffff\1\102\4\uffff\1\103\1\104"+
            "\1\105\1\106\1\107\1\111\1\uffff\1\110\1\123\1\73\1\72\2\116"+
            "\3\117\11\120\1\100\1\121\1\75\1\74\1\70\1\122\2\uffff\1\112"+
            "\1\113\1\114\12\uffff\1\77\1\54\10\uffff\1\101\1\76",
            "\1\124\1\125\1\126\1\127\1\130\1\131\74\uffff\1\132",
            "\1\133\20\uffff\1\134\13\uffff\7\142\1\143\21\uffff\1\136\1"+
            "\135\20\uffff\1\140\1\137\22\uffff\1\141\11\uffff\1\144",
            "\1\145",
            "\1\146",
            "\1\147",
            "\1\152\1\153\1\154\1\155\1\156\1\157\2\151\2\150\70\uffff\1"+
            "\160",
            "\1\164\1\165\1\166\1\167\1\170\1\171\2\162\2\161\70\uffff\1"+
            "\163",
            "\1\175\1\176\1\177\1\u0080\1\u0081\1\u0082\2\173\2\172\70\uffff"+
            "\1\174",
            "\2\u0084\1\u0083\102\uffff\1\u0085",
            "\1\u0086\20\uffff\1\u0089\3\uffff\1\u0087\7\uffff\7\u008f\1"+
            "\u0090\21\uffff\1\u008b\1\u008a\20\uffff\1\u008d\1\u008c\1\u0088"+
            "\20\uffff\1\u0092\1\u008e\11\uffff\1\u0091",
            "\1\u0093",
            "\1\u0094\1\uffff\1\u0094\32\uffff\1\u0094",
            "\1\u0095",
            "\1\u0096",
            "\1\u0097",
            "\1\u0098",
            "\1\u0099",
            "\1\u009a",
            "\1\u009b\1\uffff\1\u009b\32\uffff\1\u009b",
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
            return "260:1: subsetExpr returns [SubsetExpr value] : (e1= trimExpr | e2= sliceExpr | e3= extendExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA43_34 = input.LA(1);

                         
                        int index43_34 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_34);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA43_35 = input.LA(1);

                         
                        int index43_35 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_35);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA43_36 = input.LA(1);

                         
                        int index43_36 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_36);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA43_37 = input.LA(1);

                         
                        int index43_37 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_37);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA43_38 = input.LA(1);

                         
                        int index43_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_38);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA43_39 = input.LA(1);

                         
                        int index43_39 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_39);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA43_40 = input.LA(1);

                         
                        int index43_40 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_40);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA43_41 = input.LA(1);

                         
                        int index43_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_41);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA43_42 = input.LA(1);

                         
                        int index43_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_42);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA43_43 = input.LA(1);

                         
                        int index43_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_43);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA43_44 = input.LA(1);

                         
                        int index43_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_44);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA43_45 = input.LA(1);

                         
                        int index43_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_45);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA43_46 = input.LA(1);

                         
                        int index43_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_46);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA43_47 = input.LA(1);

                         
                        int index43_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_47);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA43_48 = input.LA(1);

                         
                        int index43_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_48);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA43_49 = input.LA(1);

                         
                        int index43_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_49);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA43_50 = input.LA(1);

                         
                        int index43_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_50);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA43_51 = input.LA(1);

                         
                        int index43_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_51);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA43_52 = input.LA(1);

                         
                        int index43_52 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_52);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA43_53 = input.LA(1);

                         
                        int index43_53 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_53);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA43_54 = input.LA(1);

                         
                        int index43_54 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_54);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA43_55 = input.LA(1);

                         
                        int index43_55 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_55);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA43_56 = input.LA(1);

                         
                        int index43_56 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_56);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA43_57 = input.LA(1);

                         
                        int index43_57 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_57);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA43_58 = input.LA(1);

                         
                        int index43_58 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_58);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA43_59 = input.LA(1);

                         
                        int index43_59 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_59);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA43_60 = input.LA(1);

                         
                        int index43_60 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_60);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA43_61 = input.LA(1);

                         
                        int index43_61 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_61);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA43_62 = input.LA(1);

                         
                        int index43_62 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_62);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA43_63 = input.LA(1);

                         
                        int index43_63 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_63);
                        if ( s>=0 ) return s;
                        break;
                    case 30 : 
                        int LA43_64 = input.LA(1);

                         
                        int index43_64 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_64);
                        if ( s>=0 ) return s;
                        break;
                    case 31 : 
                        int LA43_65 = input.LA(1);

                         
                        int index43_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_65);
                        if ( s>=0 ) return s;
                        break;
                    case 32 : 
                        int LA43_66 = input.LA(1);

                         
                        int index43_66 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_66);
                        if ( s>=0 ) return s;
                        break;
                    case 33 : 
                        int LA43_67 = input.LA(1);

                         
                        int index43_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_67);
                        if ( s>=0 ) return s;
                        break;
                    case 34 : 
                        int LA43_68 = input.LA(1);

                         
                        int index43_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_68);
                        if ( s>=0 ) return s;
                        break;
                    case 35 : 
                        int LA43_69 = input.LA(1);

                         
                        int index43_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_69);
                        if ( s>=0 ) return s;
                        break;
                    case 36 : 
                        int LA43_70 = input.LA(1);

                         
                        int index43_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_70);
                        if ( s>=0 ) return s;
                        break;
                    case 37 : 
                        int LA43_71 = input.LA(1);

                         
                        int index43_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_71);
                        if ( s>=0 ) return s;
                        break;
                    case 38 : 
                        int LA43_72 = input.LA(1);

                         
                        int index43_72 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_72);
                        if ( s>=0 ) return s;
                        break;
                    case 39 : 
                        int LA43_73 = input.LA(1);

                         
                        int index43_73 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_73);
                        if ( s>=0 ) return s;
                        break;
                    case 40 : 
                        int LA43_74 = input.LA(1);

                         
                        int index43_74 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_74);
                        if ( s>=0 ) return s;
                        break;
                    case 41 : 
                        int LA43_75 = input.LA(1);

                         
                        int index43_75 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_75);
                        if ( s>=0 ) return s;
                        break;
                    case 42 : 
                        int LA43_76 = input.LA(1);

                         
                        int index43_76 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_76);
                        if ( s>=0 ) return s;
                        break;
                    case 43 : 
                        int LA43_77 = input.LA(1);

                         
                        int index43_77 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_77);
                        if ( s>=0 ) return s;
                        break;
                    case 44 : 
                        int LA43_78 = input.LA(1);

                         
                        int index43_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_78);
                        if ( s>=0 ) return s;
                        break;
                    case 45 : 
                        int LA43_79 = input.LA(1);

                         
                        int index43_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_79);
                        if ( s>=0 ) return s;
                        break;
                    case 46 : 
                        int LA43_80 = input.LA(1);

                         
                        int index43_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_80);
                        if ( s>=0 ) return s;
                        break;
                    case 47 : 
                        int LA43_81 = input.LA(1);

                         
                        int index43_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_81);
                        if ( s>=0 ) return s;
                        break;
                    case 48 : 
                        int LA43_82 = input.LA(1);

                         
                        int index43_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_82);
                        if ( s>=0 ) return s;
                        break;
                    case 49 : 
                        int LA43_83 = input.LA(1);

                         
                        int index43_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_83);
                        if ( s>=0 ) return s;
                        break;
                    case 50 : 
                        int LA43_84 = input.LA(1);

                         
                        int index43_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_84);
                        if ( s>=0 ) return s;
                        break;
                    case 51 : 
                        int LA43_85 = input.LA(1);

                         
                        int index43_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_85);
                        if ( s>=0 ) return s;
                        break;
                    case 52 : 
                        int LA43_86 = input.LA(1);

                         
                        int index43_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_86);
                        if ( s>=0 ) return s;
                        break;
                    case 53 : 
                        int LA43_87 = input.LA(1);

                         
                        int index43_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_87);
                        if ( s>=0 ) return s;
                        break;
                    case 54 : 
                        int LA43_88 = input.LA(1);

                         
                        int index43_88 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_88);
                        if ( s>=0 ) return s;
                        break;
                    case 55 : 
                        int LA43_89 = input.LA(1);

                         
                        int index43_89 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_89);
                        if ( s>=0 ) return s;
                        break;
                    case 56 : 
                        int LA43_90 = input.LA(1);

                         
                        int index43_90 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_90);
                        if ( s>=0 ) return s;
                        break;
                    case 57 : 
                        int LA43_91 = input.LA(1);

                         
                        int index43_91 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_91);
                        if ( s>=0 ) return s;
                        break;
                    case 58 : 
                        int LA43_92 = input.LA(1);

                         
                        int index43_92 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_92);
                        if ( s>=0 ) return s;
                        break;
                    case 59 : 
                        int LA43_93 = input.LA(1);

                         
                        int index43_93 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_93);
                        if ( s>=0 ) return s;
                        break;
                    case 60 : 
                        int LA43_94 = input.LA(1);

                         
                        int index43_94 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_94);
                        if ( s>=0 ) return s;
                        break;
                    case 61 : 
                        int LA43_95 = input.LA(1);

                         
                        int index43_95 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_95);
                        if ( s>=0 ) return s;
                        break;
                    case 62 : 
                        int LA43_96 = input.LA(1);

                         
                        int index43_96 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_96);
                        if ( s>=0 ) return s;
                        break;
                    case 63 : 
                        int LA43_97 = input.LA(1);

                         
                        int index43_97 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_97);
                        if ( s>=0 ) return s;
                        break;
                    case 64 : 
                        int LA43_98 = input.LA(1);

                         
                        int index43_98 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_98);
                        if ( s>=0 ) return s;
                        break;
                    case 65 : 
                        int LA43_99 = input.LA(1);

                         
                        int index43_99 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_99);
                        if ( s>=0 ) return s;
                        break;
                    case 66 : 
                        int LA43_100 = input.LA(1);

                         
                        int index43_100 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_100);
                        if ( s>=0 ) return s;
                        break;
                    case 67 : 
                        int LA43_101 = input.LA(1);

                         
                        int index43_101 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_101);
                        if ( s>=0 ) return s;
                        break;
                    case 68 : 
                        int LA43_102 = input.LA(1);

                         
                        int index43_102 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_102);
                        if ( s>=0 ) return s;
                        break;
                    case 69 : 
                        int LA43_103 = input.LA(1);

                         
                        int index43_103 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_103);
                        if ( s>=0 ) return s;
                        break;
                    case 70 : 
                        int LA43_104 = input.LA(1);

                         
                        int index43_104 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_104);
                        if ( s>=0 ) return s;
                        break;
                    case 71 : 
                        int LA43_105 = input.LA(1);

                         
                        int index43_105 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_105);
                        if ( s>=0 ) return s;
                        break;
                    case 72 : 
                        int LA43_106 = input.LA(1);

                         
                        int index43_106 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_106);
                        if ( s>=0 ) return s;
                        break;
                    case 73 : 
                        int LA43_107 = input.LA(1);

                         
                        int index43_107 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_107);
                        if ( s>=0 ) return s;
                        break;
                    case 74 : 
                        int LA43_108 = input.LA(1);

                         
                        int index43_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_108);
                        if ( s>=0 ) return s;
                        break;
                    case 75 : 
                        int LA43_109 = input.LA(1);

                         
                        int index43_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_109);
                        if ( s>=0 ) return s;
                        break;
                    case 76 : 
                        int LA43_110 = input.LA(1);

                         
                        int index43_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_110);
                        if ( s>=0 ) return s;
                        break;
                    case 77 : 
                        int LA43_111 = input.LA(1);

                         
                        int index43_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_111);
                        if ( s>=0 ) return s;
                        break;
                    case 78 : 
                        int LA43_112 = input.LA(1);

                         
                        int index43_112 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_112);
                        if ( s>=0 ) return s;
                        break;
                    case 79 : 
                        int LA43_113 = input.LA(1);

                         
                        int index43_113 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_113);
                        if ( s>=0 ) return s;
                        break;
                    case 80 : 
                        int LA43_114 = input.LA(1);

                         
                        int index43_114 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_114);
                        if ( s>=0 ) return s;
                        break;
                    case 81 : 
                        int LA43_115 = input.LA(1);

                         
                        int index43_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_115);
                        if ( s>=0 ) return s;
                        break;
                    case 82 : 
                        int LA43_116 = input.LA(1);

                         
                        int index43_116 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_116);
                        if ( s>=0 ) return s;
                        break;
                    case 83 : 
                        int LA43_117 = input.LA(1);

                         
                        int index43_117 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_117);
                        if ( s>=0 ) return s;
                        break;
                    case 84 : 
                        int LA43_118 = input.LA(1);

                         
                        int index43_118 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_118);
                        if ( s>=0 ) return s;
                        break;
                    case 85 : 
                        int LA43_119 = input.LA(1);

                         
                        int index43_119 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_119);
                        if ( s>=0 ) return s;
                        break;
                    case 86 : 
                        int LA43_120 = input.LA(1);

                         
                        int index43_120 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_120);
                        if ( s>=0 ) return s;
                        break;
                    case 87 : 
                        int LA43_121 = input.LA(1);

                         
                        int index43_121 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_121);
                        if ( s>=0 ) return s;
                        break;
                    case 88 : 
                        int LA43_122 = input.LA(1);

                         
                        int index43_122 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_122);
                        if ( s>=0 ) return s;
                        break;
                    case 89 : 
                        int LA43_123 = input.LA(1);

                         
                        int index43_123 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_123);
                        if ( s>=0 ) return s;
                        break;
                    case 90 : 
                        int LA43_124 = input.LA(1);

                         
                        int index43_124 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_124);
                        if ( s>=0 ) return s;
                        break;
                    case 91 : 
                        int LA43_125 = input.LA(1);

                         
                        int index43_125 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_125);
                        if ( s>=0 ) return s;
                        break;
                    case 92 : 
                        int LA43_126 = input.LA(1);

                         
                        int index43_126 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_126);
                        if ( s>=0 ) return s;
                        break;
                    case 93 : 
                        int LA43_127 = input.LA(1);

                         
                        int index43_127 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_127);
                        if ( s>=0 ) return s;
                        break;
                    case 94 : 
                        int LA43_128 = input.LA(1);

                         
                        int index43_128 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_128);
                        if ( s>=0 ) return s;
                        break;
                    case 95 : 
                        int LA43_129 = input.LA(1);

                         
                        int index43_129 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_129);
                        if ( s>=0 ) return s;
                        break;
                    case 96 : 
                        int LA43_130 = input.LA(1);

                         
                        int index43_130 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_130);
                        if ( s>=0 ) return s;
                        break;
                    case 97 : 
                        int LA43_131 = input.LA(1);

                         
                        int index43_131 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_131);
                        if ( s>=0 ) return s;
                        break;
                    case 98 : 
                        int LA43_132 = input.LA(1);

                         
                        int index43_132 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_132);
                        if ( s>=0 ) return s;
                        break;
                    case 99 : 
                        int LA43_133 = input.LA(1);

                         
                        int index43_133 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_133);
                        if ( s>=0 ) return s;
                        break;
                    case 100 : 
                        int LA43_134 = input.LA(1);

                         
                        int index43_134 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_134);
                        if ( s>=0 ) return s;
                        break;
                    case 101 : 
                        int LA43_135 = input.LA(1);

                         
                        int index43_135 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_135);
                        if ( s>=0 ) return s;
                        break;
                    case 102 : 
                        int LA43_136 = input.LA(1);

                         
                        int index43_136 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_136);
                        if ( s>=0 ) return s;
                        break;
                    case 103 : 
                        int LA43_137 = input.LA(1);

                         
                        int index43_137 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_137);
                        if ( s>=0 ) return s;
                        break;
                    case 104 : 
                        int LA43_138 = input.LA(1);

                         
                        int index43_138 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_138);
                        if ( s>=0 ) return s;
                        break;
                    case 105 : 
                        int LA43_139 = input.LA(1);

                         
                        int index43_139 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_139);
                        if ( s>=0 ) return s;
                        break;
                    case 106 : 
                        int LA43_140 = input.LA(1);

                         
                        int index43_140 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_140);
                        if ( s>=0 ) return s;
                        break;
                    case 107 : 
                        int LA43_141 = input.LA(1);

                         
                        int index43_141 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_141);
                        if ( s>=0 ) return s;
                        break;
                    case 108 : 
                        int LA43_142 = input.LA(1);

                         
                        int index43_142 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_142);
                        if ( s>=0 ) return s;
                        break;
                    case 109 : 
                        int LA43_143 = input.LA(1);

                         
                        int index43_143 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_143);
                        if ( s>=0 ) return s;
                        break;
                    case 110 : 
                        int LA43_144 = input.LA(1);

                         
                        int index43_144 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_144);
                        if ( s>=0 ) return s;
                        break;
                    case 111 : 
                        int LA43_145 = input.LA(1);

                         
                        int index43_145 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_145);
                        if ( s>=0 ) return s;
                        break;
                    case 112 : 
                        int LA43_146 = input.LA(1);

                         
                        int index43_146 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_146);
                        if ( s>=0 ) return s;
                        break;
                    case 113 : 
                        int LA43_147 = input.LA(1);

                         
                        int index43_147 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_147);
                        if ( s>=0 ) return s;
                        break;
                    case 114 : 
                        int LA43_148 = input.LA(1);

                         
                        int index43_148 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_148);
                        if ( s>=0 ) return s;
                        break;
                    case 115 : 
                        int LA43_149 = input.LA(1);

                         
                        int index43_149 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_149);
                        if ( s>=0 ) return s;
                        break;
                    case 116 : 
                        int LA43_150 = input.LA(1);

                         
                        int index43_150 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_150);
                        if ( s>=0 ) return s;
                        break;
                    case 117 : 
                        int LA43_151 = input.LA(1);

                         
                        int index43_151 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_151);
                        if ( s>=0 ) return s;
                        break;
                    case 118 : 
                        int LA43_152 = input.LA(1);

                         
                        int index43_152 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_152);
                        if ( s>=0 ) return s;
                        break;
                    case 119 : 
                        int LA43_153 = input.LA(1);

                         
                        int index43_153 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_153);
                        if ( s>=0 ) return s;
                        break;
                    case 120 : 
                        int LA43_154 = input.LA(1);

                         
                        int index43_154 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_154);
                        if ( s>=0 ) return s;
                        break;
                    case 121 : 
                        int LA43_155 = input.LA(1);

                         
                        int index43_155 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred101_wcps()) ) {s = 31;}

                        else if ( (synpred102_wcps()) ) {s = 32;}

                         
                        input.seek(index43_155);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 43, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA44_eotS =
        "\40\uffff";
    static final String DFA44_eofS =
        "\40\uffff";
    static final String DFA44_minS =
        "\1\6\37\uffff";
    static final String DFA44_maxS =
        "\1\154\37\uffff";
    static final String DFA44_acceptS =
        "\1\uffff\1\1\35\uffff\1\2";
    static final String DFA44_specialS =
        "\40\uffff}>";
    static final String[] DFA44_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\20\1\3\uffff\1\1\4\uffff\6\1\1\uffff"+
            "\1\1\1\uffff\2\1\16\uffff\1\1\1\uffff\3\1\3\uffff\1\37\14\uffff"+
            "\2\1\10\uffff\2\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "265:1: trimExpr returns [TrimExpr value] : (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN );";
        }
    }
    static final String DFA45_eotS =
        "\40\uffff";
    static final String DFA45_eofS =
        "\40\uffff";
    static final String DFA45_minS =
        "\1\6\37\uffff";
    static final String DFA45_maxS =
        "\1\154\37\uffff";
    static final String DFA45_acceptS =
        "\1\uffff\1\1\35\uffff\1\2";
    static final String DFA45_specialS =
        "\40\uffff}>";
    static final String[] DFA45_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\20\1\3\uffff\1\1\4\uffff\6\1\1\uffff"+
            "\1\1\1\uffff\2\1\16\uffff\1\1\1\uffff\3\1\4\uffff\1\37\13\uffff"+
            "\2\1\10\uffff\2\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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

    static final short[] DFA45_eot = DFA.unpackEncodedString(DFA45_eotS);
    static final short[] DFA45_eof = DFA.unpackEncodedString(DFA45_eofS);
    static final char[] DFA45_min = DFA.unpackEncodedStringToUnsignedChars(DFA45_minS);
    static final char[] DFA45_max = DFA.unpackEncodedStringToUnsignedChars(DFA45_maxS);
    static final short[] DFA45_accept = DFA.unpackEncodedString(DFA45_acceptS);
    static final short[] DFA45_special = DFA.unpackEncodedString(DFA45_specialS);
    static final short[][] DFA45_transition;

    static {
        int numStates = DFA45_transitionS.length;
        DFA45_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA45_transition[i] = DFA.unpackEncodedString(DFA45_transitionS[i]);
        }
    }

    class DFA45 extends DFA {

        public DFA45(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 45;
            this.eot = DFA45_eot;
            this.eof = DFA45_eof;
            this.min = DFA45_min;
            this.max = DFA45_max;
            this.accept = DFA45_accept;
            this.special = DFA45_special;
            this.transition = DFA45_transition;
        }
        public String getDescription() {
            return "269:1: sliceExpr returns [SliceExpr value] : (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN );";
        }
    }
    static final String DFA47_eotS =
        "\77\uffff";
    static final String DFA47_eofS =
        "\1\1\76\uffff";
    static final String DFA47_minS =
        "\1\7\7\uffff\1\0\66\uffff";
    static final String DFA47_maxS =
        "\1\140\7\uffff\1\0\66\uffff";
    static final String DFA47_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA47_specialS =
        "\10\uffff\1\0\66\uffff}>";
    static final String[] DFA47_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\2\10\14\1\21\uffff\2\1\3\uffff\1\1"+
            "\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
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
            return "()* loopback of 290:7: (op= ( OR | XOR ) e2= booleanScalarTerm )*";
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
        "\1\7\7\uffff\1\0\66\uffff";
    static final String DFA48_maxS =
        "\1\140\7\uffff\1\0\66\uffff";
    static final String DFA48_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA48_specialS =
        "\10\uffff\1\0\66\uffff}>";
    static final String[] DFA48_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\2\1\1\10\13\1\21\uffff\2\1\3\uffff"+
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
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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
            return "()* loopback of 294:4: (op= AND e2= booleanScalarNegation )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA48_8 = input.LA(1);

                         
                        int index48_8 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index48_8);
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
        "\17\uffff";
    static final String DFA49_eofS =
        "\17\uffff";
    static final String DFA49_minS =
        "\1\6\16\uffff";
    static final String DFA49_maxS =
        "\1\154\16\uffff";
    static final String DFA49_acceptS =
        "\1\uffff\1\1\14\uffff\1\2";
    static final String DFA49_specialS =
        "\17\uffff}>";
    static final String[] DFA49_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\1\1\7\uffff\10\1\21\uffff\2\1\16\uffff"+
            "\1\16\1\uffff\3\1\20\uffff\2\1\11\uffff\1\1",
            "",
            "",
            "",
            "",
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
            return "296:1: booleanScalarNegation returns [BooleanScalarExpr value] : (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom );";
        }
    }
    static final String DFA50_eotS =
        "\34\uffff";
    static final String DFA50_eofS =
        "\34\uffff";
    static final String DFA50_minS =
        "\2\6\14\uffff\2\0\2\uffff\10\0\2\uffff";
    static final String DFA50_maxS =
        "\2\154\14\uffff\2\0\2\uffff\10\0\2\uffff";
    static final String DFA50_acceptS =
        "\2\uffff\1\2\1\uffff\1\3\10\uffff\1\4\2\uffff\1\1\13\uffff";
    static final String DFA50_specialS =
        "\16\uffff\1\0\1\1\2\uffff\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\2\uffff}>";
    static final String[] DFA50_transitionS = {
            "\1\1\20\uffff\1\4\3\uffff\1\2\7\uffff\10\4\21\uffff\2\4\20\uffff"+
            "\2\4\1\2\20\uffff\1\15\1\4\11\uffff\1\4",
            "\1\17\20\uffff\1\22\3\uffff\1\20\7\uffff\7\27\1\30\21\uffff"+
            "\1\24\1\23\16\uffff\1\20\1\uffff\1\26\1\25\1\20\20\uffff\1\20"+
            "\1\16\11\uffff\1\31",
            "",
            "",
            "",
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
            return "300:1: booleanScalarAtom returns [BooleanScalarExpr value] : ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA50_14 = input.LA(1);

                         
                        int index50_14 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_14);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA50_15 = input.LA(1);

                         
                        int index50_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_15);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA50_18 = input.LA(1);

                         
                        int index50_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_18);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA50_19 = input.LA(1);

                         
                        int index50_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_19);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA50_20 = input.LA(1);

                         
                        int index50_20 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_20);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA50_21 = input.LA(1);

                         
                        int index50_21 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_21);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA50_22 = input.LA(1);

                         
                        int index50_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_22);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA50_23 = input.LA(1);

                         
                        int index50_23 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_23);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA50_24 = input.LA(1);

                         
                        int index50_24 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_24);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA50_25 = input.LA(1);

                         
                        int index50_25 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred119_wcps()) ) {s = 16;}

                        else if ( (synpred121_wcps()) ) {s = 4;}

                         
                        input.seek(index50_25);
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
    static final String DFA51_eotS =
        "\77\uffff";
    static final String DFA51_eofS =
        "\1\1\76\uffff";
    static final String DFA51_minS =
        "\1\7\3\uffff\1\0\72\uffff";
    static final String DFA51_maxS =
        "\1\140\3\uffff\1\0\72\uffff";
    static final String DFA51_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA51_specialS =
        "\4\uffff\1\0\72\uffff}>";
    static final String[] DFA51_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\11\1\2\4\3\1\21\uffff\2\1\3\uffff\1"+
            "\1\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
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
            ""
    };

    static final short[] DFA51_eot = DFA.unpackEncodedString(DFA51_eotS);
    static final short[] DFA51_eof = DFA.unpackEncodedString(DFA51_eofS);
    static final char[] DFA51_min = DFA.unpackEncodedStringToUnsignedChars(DFA51_minS);
    static final char[] DFA51_max = DFA.unpackEncodedStringToUnsignedChars(DFA51_maxS);
    static final short[] DFA51_accept = DFA.unpackEncodedString(DFA51_acceptS);
    static final short[] DFA51_special = DFA.unpackEncodedString(DFA51_specialS);
    static final short[][] DFA51_transition;

    static {
        int numStates = DFA51_transitionS.length;
        DFA51_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA51_transition[i] = DFA.unpackEncodedString(DFA51_transitionS[i]);
        }
    }

    class DFA51 extends DFA {

        public DFA51(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 51;
            this.eot = DFA51_eot;
            this.eof = DFA51_eof;
            this.min = DFA51_min;
            this.max = DFA51_max;
            this.accept = DFA51_accept;
            this.special = DFA51_special;
            this.transition = DFA51_transition;
        }
        public String getDescription() {
            return "()* loopback of 308:4: (op= ( PLUS | MINUS ) e2= numericScalarTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA51_4 = input.LA(1);

                         
                        int index51_4 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred123_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index51_4);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 51, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA52_eotS =
        "\77\uffff";
    static final String DFA52_eofS =
        "\1\1\76\uffff";
    static final String DFA52_minS =
        "\1\7\3\uffff\1\0\72\uffff";
    static final String DFA52_maxS =
        "\1\140\3\uffff\1\0\72\uffff";
    static final String DFA52_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA52_specialS =
        "\4\uffff\1\0\72\uffff}>";
    static final String[] DFA52_transitionS = {
            "\2\1\1\uffff\1\1\2\uffff\13\1\2\4\1\1\21\uffff\2\1\3\uffff\1"+
            "\1\7\uffff\1\1\30\uffff\1\1\15\uffff\1\1",
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
            return "()* loopback of 312:3: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*";
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
                        if ( (synpred125_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index52_4);
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
    static final String DFA53_eotS =
        "\26\uffff";
    static final String DFA53_eofS =
        "\26\uffff";
    static final String DFA53_minS =
        "\2\6\11\uffff\1\0\12\uffff";
    static final String DFA53_maxS =
        "\2\154\11\uffff\1\0\12\uffff";
    static final String DFA53_acceptS =
        "\2\uffff\1\2\1\3\1\4\1\5\1\6\1\7\1\11\1\uffff\1\12\1\uffff\1\1\10"+
        "\uffff\1\10";
    static final String DFA53_specialS =
        "\13\uffff\1\0\12\uffff}>";
    static final String[] DFA53_transitionS = {
            "\1\1\20\uffff\1\2\13\uffff\10\10\21\uffff\1\4\1\3\20\uffff\1"+
            "\6\1\5\22\uffff\1\7\11\uffff\1\12",
            "\1\14\20\uffff\1\14\13\uffff\10\14\21\uffff\2\14\20\uffff\2"+
            "\14\22\uffff\1\13\11\uffff\1\14",
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
            ""
    };

    static final short[] DFA53_eot = DFA.unpackEncodedString(DFA53_eotS);
    static final short[] DFA53_eof = DFA.unpackEncodedString(DFA53_eofS);
    static final char[] DFA53_min = DFA.unpackEncodedStringToUnsignedChars(DFA53_minS);
    static final char[] DFA53_max = DFA.unpackEncodedStringToUnsignedChars(DFA53_maxS);
    static final short[] DFA53_accept = DFA.unpackEncodedString(DFA53_acceptS);
    static final short[] DFA53_special = DFA.unpackEncodedString(DFA53_specialS);
    static final short[][] DFA53_transition;

    static {
        int numStates = DFA53_transitionS.length;
        DFA53_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA53_transition[i] = DFA.unpackEncodedString(DFA53_transitionS[i]);
        }
    }

    class DFA53 extends DFA {

        public DFA53(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 53;
            this.eot = DFA53_eot;
            this.eof = DFA53_eof;
            this.min = DFA53_min;
            this.max = DFA53_max;
            this.accept = DFA53_accept;
            this.special = DFA53_special;
            this.transition = DFA53_transition;
        }
        public String getDescription() {
            return "314:1: numericScalarFactor returns [NumericScalarExpr value] : ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ABS LPAREN e12= numericScalarExpr RPAREN | op= SQRT LPAREN e11= numericScalarExpr RPAREN | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr | e4= variableName );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA53_11 = input.LA(1);

                         
                        int index53_11 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred126_wcps()) ) {s = 12;}

                        else if ( (synpred133_wcps()) ) {s = 21;}

                         
                        input.seek(index53_11);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 53, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA57_eotS =
        "\30\uffff";
    static final String DFA57_eofS =
        "\30\uffff";
    static final String DFA57_minS =
        "\1\6\4\uffff\1\6\20\uffff\1\0\1\uffff";
    static final String DFA57_maxS =
        "\1\154\4\uffff\1\6\20\uffff\1\0\1\uffff";
    static final String DFA57_acceptS =
        "\1\uffff\1\1\25\uffff\1\2";
    static final String DFA57_specialS =
        "\26\uffff\1\0\1\uffff}>";
    static final String[] DFA57_transitionS = {
            "\1\1\20\uffff\1\1\3\uffff\7\1\1\5\10\1\21\uffff\2\1\16\uffff"+
            "\1\1\1\uffff\3\1\20\uffff\2\1\11\uffff\1\1",
            "",
            "",
            "",
            "",
            "\1\26",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
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

    static final short[] DFA57_eot = DFA.unpackEncodedString(DFA57_eotS);
    static final short[] DFA57_eof = DFA.unpackEncodedString(DFA57_eofS);
    static final char[] DFA57_min = DFA.unpackEncodedStringToUnsignedChars(DFA57_minS);
    static final char[] DFA57_max = DFA.unpackEncodedStringToUnsignedChars(DFA57_maxS);
    static final short[] DFA57_accept = DFA.unpackEncodedString(DFA57_acceptS);
    static final short[] DFA57_special = DFA.unpackEncodedString(DFA57_specialS);
    static final short[][] DFA57_transition;

    static {
        int numStates = DFA57_transitionS.length;
        DFA57_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA57_transition[i] = DFA.unpackEncodedString(DFA57_transitionS[i]);
        }
    }

    class DFA57 extends DFA {

        public DFA57(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 57;
            this.eot = DFA57_eot;
            this.eof = DFA57_eof;
            this.min = DFA57_min;
            this.max = DFA57_max;
            this.accept = DFA57_accept;
            this.special = DFA57_special;
            this.transition = DFA57_transition;
        }
        public String getDescription() {
            return "342:1: dimensionIntervalExpr returns [DimensionIntervalExpr value] : (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA57_22 = input.LA(1);

                         
                        int index57_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred142_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 23;}

                         
                        input.seek(index57_22);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 57, _s, input);
            error(nvae);
            throw nvae;
        }
    }
 

    public static final BitSet FOLLOW_forClause_in_wcpsRequest63 = new BitSet(new long[]{0x0000000000000600L});
    public static final BitSet FOLLOW_whereClause_in_wcpsRequest72 = new BitSet(new long[]{0x0000000000000600L});
    public static final BitSet FOLLOW_returnClause_in_wcpsRequest83 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FOR_in_forClause98 = new BitSet(new long[]{0x0000000000000000L,0x0000080000000000L});
    public static final BitSet FOLLOW_coverageVariable_in_forClause102 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_IN_in_forClause104 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_forClause106 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageList_in_forClause110 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_forClause112 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_forClause122 = new BitSet(new long[]{0x0000000000000000L,0x0000080000000000L});
    public static final BitSet FOLLOW_coverageVariable_in_forClause126 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_IN_in_forClause128 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_forClause130 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageList_in_forClause134 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_forClause136 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_WHERE_in_whereClause157 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_whereClause161 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RETURN_in_returnClause176 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
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
    public static final BitSet FOLLOW_AND_in_coverageLogicTerm416 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicFactor_in_coverageLogicTerm420 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor447 = new BitSet(new long[]{0x00000000003F0002L});
    public static final BitSet FOLLOW_set_in_coverageLogicFactor463 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticExpr_in_coverageLogicFactor479 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr507 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_set_in_coverageArithmeticExpr523 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticTerm_in_coverageArithmeticExpr531 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm559 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_coverageArithmeticTerm574 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticFactor_in_coverageArithmeticTerm582 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_coverageValue_in_coverageArithmeticFactor610 = new BitSet(new long[]{0x0000000004000002L});
    public static final BitSet FOLLOW_OVERLAY_in_coverageArithmeticFactor625 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageValue_in_coverageArithmeticFactor629 = new BitSet(new long[]{0x0000000004000002L});
    public static final BitSet FOLLOW_subsetExpr_in_coverageValue655 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryInducedExpr_in_coverageValue668 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scaleExpr_in_coverageValue680 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_crsTransformExpr_in_coverageValue692 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_coverageValue704 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_coverageAtom727 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageVariable_in_coverageAtom739 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_coverageAtom749 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_coverageAtom753 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_coverageAtom755 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstantExpr_in_coverageAtom768 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstructorExpr_in_coverageAtom780 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_setMetaDataExpr_in_coverageAtom793 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rangeConstructorExpr_in_coverageAtom806 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_metaDataExpr_in_scalarExpr830 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_scalarExpr843 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_scalarExpr856 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_scalarExpr870 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_scalarExpr883 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_scalarExpr894 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_scalarExpr898 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_scalarExpr900 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IDENTIFIER_in_metaDataExpr924 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr926 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr930 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr932 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IMAGECRS_in_metaDataExpr944 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr946 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr950 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr952 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IMAGECRSDOMAIN_in_metaDataExpr964 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr966 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr970 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_COMMA_in_metaDataExpr973 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_metaDataExpr977 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr981 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CRSSET_in_metaDataExpr993 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr995 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr999 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1001 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_domainExpr_in_metaDataExpr1013 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NULLSET_in_metaDataExpr1025 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr1027 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1031 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1033 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTERPOLATIONDEFAULT_in_metaDataExpr1045 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr1047 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1051 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_metaDataExpr1053 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_metaDataExpr1057 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1059 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTERPOLATIONSET_in_metaDataExpr1071 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_metaDataExpr1073 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_metaDataExpr1077 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_metaDataExpr1079 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_metaDataExpr1083 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_metaDataExpr1085 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DOMAIN_in_domainExpr1104 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_domainExpr1106 = new BitSet(new long[]{0x0000000000000000L,0x0000080000000000L});
    public static final BitSet FOLLOW_coverageVariable_in_domainExpr1110 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_domainExpr1112 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_domainExpr1116 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_domainExpr1118 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_domainExpr1122 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_domainExpr1124 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reduceExpr_in_condenseExpr1141 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_generalCondenseExpr_in_condenseExpr1150 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_reduceExpr1167 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_reduceExpr1183 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_reduceExpr1187 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_reduceExpr1189 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CONDENSE_in_generalCondenseExpr1204 = new BitSet(new long[]{0x000003000140A000L});
    public static final BitSet FOLLOW_condenseOpType_in_generalCondenseExpr1208 = new BitSet(new long[]{0x0000080000000000L});
    public static final BitSet FOLLOW_OVER_in_generalCondenseExpr1210 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_axisIteratorList_in_generalCondenseExpr1214 = new BitSet(new long[]{0x0000100000000200L});
    public static final BitSet FOLLOW_WHERE_in_generalCondenseExpr1221 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_generalCondenseExpr1225 = new BitSet(new long[]{0x0000100000000000L});
    public static final BitSet FOLLOW_USING_in_generalCondenseExpr1233 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_generalCondenseExpr1237 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variableName_in_axisIteratorList1254 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_axisIteratorList1258 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_axisIteratorList1260 = new BitSet(new long[]{0x0000000020000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_intervalExpr_in_axisIteratorList1264 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_axisIteratorList1266 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_axisIteratorList1274 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_variableName_in_axisIteratorList1278 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_axisIteratorList1282 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_axisIteratorList1284 = new BitSet(new long[]{0x0000000020000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_intervalExpr_in_axisIteratorList1288 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_axisIteratorList1290 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_indexExpr_in_intervalExpr1314 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_intervalExpr1316 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_intervalExpr1320 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IMAGECRSDOMAIN_in_intervalExpr1335 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_intervalExpr1337 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_intervalExpr1341 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_intervalExpr1343 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_intervalExpr1347 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_intervalExpr1349 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_COVERAGE_in_coverageConstantExpr1375 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_coverageConstantExpr1379 = new BitSet(new long[]{0x0000080000000000L});
    public static final BitSet FOLLOW_OVER_in_coverageConstantExpr1381 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_axisIteratorList_in_coverageConstantExpr1385 = new BitSet(new long[]{0x0000800000000000L});
    public static final BitSet FOLLOW_VALUE_in_coverageConstantExpr1387 = new BitSet(new long[]{0x0001000000000000L});
    public static final BitSet FOLLOW_LIST_in_coverageConstantExpr1389 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_LT_in_coverageConstantExpr1391 = new BitSet(new long[]{0x0000000000000040L,0x0000000600014000L});
    public static final BitSet FOLLOW_constantList_in_coverageConstantExpr1395 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_GT_in_coverageConstantExpr1397 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_constant_in_constantList1428 = new BitSet(new long[]{0x0002000000000002L});
    public static final BitSet FOLLOW_SEMICOLON_in_constantList1433 = new BitSet(new long[]{0x0000000000000040L,0x0000000600014000L});
    public static final BitSet FOLLOW_constant_in_constantList1437 = new BitSet(new long[]{0x0002000000000002L});
    public static final BitSet FOLLOW_COVERAGE_in_coverageConstructorExpr1457 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_coverageConstructorExpr1461 = new BitSet(new long[]{0x0000080000000000L});
    public static final BitSet FOLLOW_OVER_in_coverageConstructorExpr1463 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_axisIteratorList_in_coverageConstructorExpr1467 = new BitSet(new long[]{0x0004000000000000L});
    public static final BitSet FOLLOW_VALUES_in_coverageConstructorExpr1469 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_coverageConstructorExpr1473 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETIDENTIFIER_in_setMetaDataExpr1495 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1497 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringConstant_in_setMetaDataExpr1501 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1503 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1507 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1509 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETCRSSET_in_setMetaDataExpr1522 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1524 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1528 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1530 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_crsList_in_setMetaDataExpr1534 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1536 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETNULLSET_in_setMetaDataExpr1553 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1555 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1559 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1561 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_rangeExprList_in_setMetaDataExpr1565 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1567 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETINTERPOLATIONDEFAULT_in_setMetaDataExpr1584 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1586 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1590 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1592 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_setMetaDataExpr1596 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1598 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_setMetaDataExpr1602 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1604 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETINTERPOLATIONSET_in_setMetaDataExpr1624 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_setMetaDataExpr1626 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_setMetaDataExpr1630 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1632 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_setMetaDataExpr1636 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_setMetaDataExpr1638 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_interpolationMethodList_in_setMetaDataExpr1642 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_setMetaDataExpr1644 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_crsList1673 = new BitSet(new long[]{0x0200000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_crsList1680 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsList1685 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_crsList1689 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_crsList1698 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_rangeExprList1717 = new BitSet(new long[]{0x0600000000000000L});
    public static final BitSet FOLLOW_rangeExpr_in_rangeExprList1724 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_rangeExprList1729 = new BitSet(new long[]{0x0400000000000000L});
    public static final BitSet FOLLOW_rangeExpr_in_rangeExprList1733 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_rangeExprList1742 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_interpolationMethodList1758 = new BitSet(new long[]{0x0200000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_interpolationMethodList1765 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_interpolationMethodList1770 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_interpolationMethodList1774 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_interpolationMethodList1782 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRUCT_in_rangeExpr1795 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_rangeExpr1797 = new BitSet(new long[]{0x0200000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeExpr1805 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1807 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_rangeExpr1811 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_rangeExpr1818 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeExpr1822 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1824 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_rangeExpr1828 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_rangeExpr1839 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRUCT_in_rangeConstructorExpr1856 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_rangeConstructorExpr1860 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeConstructorExpr1864 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeConstructorExpr1866 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_rangeConstructorExpr1870 = new BitSet(new long[]{0x0202000000000000L});
    public static final BitSet FOLLOW_SEMICOLON_in_rangeConstructorExpr1883 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeConstructorExpr1887 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeConstructorExpr1889 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_rangeConstructorExpr1893 = new BitSet(new long[]{0x0202000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_rangeConstructorExpr1899 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CRSTRANSFORM_in_crsTransformExpr1922 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_crsTransformExpr1924 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_crsTransformExpr1928 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsTransformExpr1930 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_dimensionCrsList_in_crsTransformExpr1934 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsTransformExpr1936 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_fieldInterpolationList_in_crsTransformExpr1940 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_crsTransformExpr1942 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_dimensionCrsList1983 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionCrsElement_in_dimensionCrsList1987 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_dimensionCrsList2000 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionCrsElement_in_dimensionCrsList2004 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_dimensionCrsList2011 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_axisName_in_dimensionCrsElement2032 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionCrsElement2036 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionCrsElement2040 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_fieldInterpolationList2058 = new BitSet(new long[]{0x0200000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldInterpolationElement_in_fieldInterpolationList2063 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_fieldInterpolationList2070 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldInterpolationElement_in_fieldInterpolationList2074 = new BitSet(new long[]{0x0200000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_RBRACE_in_fieldInterpolationList2082 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldName_in_fieldInterpolationElement2097 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_fieldInterpolationElement2101 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldExpr_in_unaryInducedExpr2121 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryArithmeticExpr_in_unaryInducedExpr2130 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_exponentialExpr_in_unaryInducedExpr2142 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trigonometricExpr_in_unaryInducedExpr2154 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanExpr_in_unaryInducedExpr2166 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_castExpr_in_unaryInducedExpr2178 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rangeConstructorExpr_in_unaryInducedExpr2190 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_unaryArithmeticExpr2213 = new BitSet(new long[]{0x35F847FFF8801840L,0x000018060001D000L});
    public static final BitSet FOLLOW_coverageAtom_in_unaryArithmeticExpr2221 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_unaryArithmeticExpr2233 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_unaryArithmeticExpr2243 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_unaryArithmeticExpr2247 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_unaryArithmeticExpr2249 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_exponentialExpr2272 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_exponentialExpr2280 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_exponentialExpr2284 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_exponentialExpr2286 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_trigonometricExpr2309 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_trigonometricExpr2329 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_trigonometricExpr2333 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_trigonometricExpr2335 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_booleanExpr2358 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_booleanExpr2362 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BIT_in_booleanExpr2374 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_booleanExpr2376 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_booleanExpr2380 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_booleanExpr2382 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_booleanExpr2386 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_booleanExpr2388 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_indexTerm_in_indexExpr2411 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_set_in_indexExpr2421 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexTerm_in_indexExpr2429 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_indexFactor_in_indexTerm2454 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_indexTerm2467 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexFactor_in_indexTerm2475 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_INTEGERCONSTANT_in_indexFactor2502 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ROUND_in_indexFactor2515 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_indexFactor2517 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_indexFactor2521 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_indexFactor2523 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_indexFactor2535 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_indexFactor2539 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_indexFactor2541 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IDENTIFIER_in_stringScalarExpr2568 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_stringScalarExpr2570 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_stringScalarExpr2574 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_stringScalarExpr2576 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_stringScalarExpr2589 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SCALE_in_scaleExpr2607 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_scaleExpr2609 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_scaleExpr2613 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_scaleExpr2615 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_scaleExpr2617 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_scaleExpr2621 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_scaleExpr2623 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_COMMA_in_scaleExpr2640 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_fieldInterpolationList_in_scaleExpr2644 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_scaleExpr2669 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trimExpr_in_subsetExpr2686 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_sliceExpr_in_subsetExpr2695 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_extendExpr_in_subsetExpr2704 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_trimExpr2721 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_LBRACKET_in_trimExpr2723 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_trimExpr2727 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_RBRACKET_in_trimExpr2729 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_TRIM_in_trimExpr2738 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_trimExpr2740 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_trimExpr2744 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_trimExpr2746 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_trimExpr2748 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_trimExpr2752 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_trimExpr2754 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_trimExpr2756 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_sliceExpr2773 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_LBRACKET_in_sliceExpr2775 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointList_in_sliceExpr2779 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_RBRACKET_in_sliceExpr2781 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SLICE_in_sliceExpr2788 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_sliceExpr2790 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_sliceExpr2794 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_sliceExpr2796 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_sliceExpr2798 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointList_in_sliceExpr2802 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_sliceExpr2804 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_sliceExpr2806 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EXTEND_in_extendExpr2821 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_extendExpr2823 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_extendExpr2827 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_extendExpr2829 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_extendExpr2833 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_extendExpr2835 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_castExpr2853 = new BitSet(new long[]{0x0000000000000000L,0x00000000FF800000L});
    public static final BitSet FOLLOW_rangeType_in_castExpr2857 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_castExpr2859 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_castExpr2863 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_rangeType2886 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_UNSIGNED_in_rangeType2912 = new BitSet(new long[]{0x0000000000000000L,0x0000000007000000L});
    public static final BitSet FOLLOW_set_in_rangeType2916 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_fieldExpr2945 = new BitSet(new long[]{0x0000000000000000L,0x0000000100000000L});
    public static final BitSet FOLLOW_DOT_in_fieldExpr2947 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_fieldExpr2951 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_booleanScalarExpr2976 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_set_in_booleanScalarExpr2989 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_booleanScalarExpr2997 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_booleanScalarTerm3019 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_AND_in_booleanScalarTerm3029 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_booleanScalarTerm3033 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_booleanScalarAtom_in_booleanScalarNegation3054 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_booleanScalarNegation3063 = new BitSet(new long[]{0x300007F808800040L,0x000010060001C000L});
    public static final BitSet FOLLOW_booleanScalarAtom_in_booleanScalarNegation3067 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_booleanScalarAtom3082 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_booleanScalarAtom3086 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_booleanScalarAtom3088 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_booleanScalarAtom3097 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_booleanScalarAtom3101 = new BitSet(new long[]{0x0000000008000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringScalarExpr_in_booleanScalarAtom3105 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_booleanScalarAtom3115 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_booleanScalarAtom3119 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_booleanScalarAtom3123 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BOOLEANCONSTANT_in_booleanScalarAtom3133 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarTerm_in_numericScalarExpr3150 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_set_in_numericScalarExpr3160 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarTerm_in_numericScalarExpr3168 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarTerm3187 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_numericScalarTerm3196 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarTerm3204 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3224 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3228 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3230 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MINUS_in_numericScalarFactor3242 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarFactor3246 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ABS_in_numericScalarFactor3258 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3260 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3264 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3266 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SQRT_in_numericScalarFactor3278 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3280 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3284 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3286 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ROUND_in_numericScalarFactor3298 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3300 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3304 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3306 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTEGERCONSTANT_in_numericScalarFactor3318 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_numericScalarFactor3330 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_numericScalarFactor3342 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_numericScalarFactor3354 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variableName_in_numericScalarFactor3366 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EQUALS_in_compOp3384 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOTEQUALS_in_compOp3391 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LT_in_compOp3398 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GT_in_compOp3405 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LTE_in_compOp3412 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GTE_in_compOp3419 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3439 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_dimensionIntervalList3452 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3456 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_axisName_in_dimensionIntervalElement3481 = new BitSet(new long[]{0x0000200000000040L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalElement3486 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionIntervalElement3490 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionIntervalElement3501 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_dimensionIntervalExpr_in_dimensionIntervalElement3505 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionIntervalElement3507 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionIntervalExpr3530 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3532 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionIntervalExpr3536 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DOMAIN_in_dimensionIntervalExpr3546 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionIntervalExpr3548 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_dimensionIntervalExpr3552 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3554 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_dimensionIntervalExpr3558 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3560 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionIntervalExpr3564 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionIntervalExpr3566 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_dimensionPointElement_in_dimensionPointList3589 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_dimensionPointList3599 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointElement_in_dimensionPointList3603 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_axisName_in_dimensionPointElement3628 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionPointElement3630 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_dimensionPoint_in_dimensionPointElement3634 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionPointElement3636 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_axisName_in_dimensionPointElement3648 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionPointElement3650 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionPointElement3654 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionPointElement3656 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_dimensionPoint_in_dimensionPointElement3660 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionPointElement3662 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionPoint3685 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_interpolationMethod3703 = new BitSet(new long[]{0x0000000000000000L,0x0000007800000000L});
    public static final BitSet FOLLOW_interpolationType_in_interpolationMethod3707 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_interpolationMethod3709 = new BitSet(new long[]{0x0000000000000000L,0x0000078000000000L});
    public static final BitSet FOLLOW_nullResistence_in_interpolationMethod3713 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_interpolationMethod3715 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_interpolationType3732 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_nullResistence3757 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_condenseOpType3782 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_fieldName3809 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_constant3826 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_constant3843 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_complexConstant3858 = new BitSet(new long[]{0x0000000000000000L,0x0000000400000000L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_complexConstant3862 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_complexConstant3864 = new BitSet(new long[]{0x0000000000000000L,0x0000000400000000L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_complexConstant3868 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_complexConstant3870 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_stringConstant3887 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_name3904 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringConstant_in_crsName3931 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_axisName3948 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VARIABLE_DOLLAR_in_variableName3965 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NAME_in_coverageVariable3982 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_coverageName3997 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred8_wcps366 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicTerm_in_synpred8_wcps374 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AND_in_synpred9_wcps416 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageLogicFactor_in_synpred9_wcps420 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred15_wcps463 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticExpr_in_synpred15_wcps479 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred17_wcps523 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticTerm_in_synpred17_wcps531 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred19_wcps574 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageArithmeticFactor_in_synpred19_wcps582 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_OVERLAY_in_synpred20_wcps625 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageValue_in_synpred20_wcps629 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_subsetExpr_in_synpred21_wcps655 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryInducedExpr_in_synpred22_wcps668 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred25_wcps727 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred27_wcps749 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_synpred27_wcps753 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred27_wcps755 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstantExpr_in_synpred28_wcps768 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageConstructorExpr_in_synpred29_wcps780 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_metaDataExpr_in_synpred31_wcps830 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_synpred32_wcps843 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_synpred33_wcps856 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred34_wcps870 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_synpred35_wcps883 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldExpr_in_synpred71_wcps2121 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryArithmeticExpr_in_synpred72_wcps2130 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanExpr_in_synpred75_wcps2166 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trimExpr_in_synpred101_wcps2686 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_sliceExpr_in_synpred102_wcps2695 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred116_wcps2989 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_synpred116_wcps2997 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AND_in_synpred117_wcps3029 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_synpred117_wcps3033 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred119_wcps3082 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_synpred119_wcps3086 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred119_wcps3088 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred121_wcps3115 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_synpred121_wcps3119 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred121_wcps3123 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred123_wcps3160 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarTerm_in_synpred123_wcps3168 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred125_wcps3196 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_synpred125_wcps3204 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred126_wcps3224 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred126_wcps3228 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred126_wcps3230 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_synpred133_wcps3342 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred142_wcps3530 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_synpred142_wcps3532 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred142_wcps3536 = new BitSet(new long[]{0x0000000000000002L});

}