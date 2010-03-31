// $ANTLR 3.2 Sep 23, 2009 12:02:23 src/petascope/wcps/grammar/wcps.g 2010-03-31 22:54:40
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
            this.state.ruleMemo = new HashMap[240+1];
             
             
        }
        
    protected TreeAdaptor adaptor = new CommonTreeAdaptor();

    public void setTreeAdaptor(TreeAdaptor adaptor) {
        this.adaptor = adaptor;
    }
    public TreeAdaptor getTreeAdaptor() {
        return adaptor;
    }

    public String[] getTokenNames() { return wcpsParser.tokenNames; }
    public String getGrammarFileName() { return "src/petascope/wcps/grammar/wcps.g"; }


    public static class wcpsRequest_return extends ParserRuleReturnScope {
        public WCPSRequest value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "wcpsRequest"
    // src/petascope/wcps/grammar/wcps.g:32:1: wcpsRequest returns [WCPSRequest value] : e1= forClause (e2= whereClause )? e3= returnClause ;
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
            // src/petascope/wcps/grammar/wcps.g:33:2: (e1= forClause (e2= whereClause )? e3= returnClause )
            // src/petascope/wcps/grammar/wcps.g:33:4: e1= forClause (e2= whereClause )? e3= returnClause
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
            // src/petascope/wcps/grammar/wcps.g:34:3: (e2= whereClause )?
            int alt1=2;
            int LA1_0 = input.LA(1);

            if ( (LA1_0==WHERE) ) {
                alt1=1;
            }
            switch (alt1) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:34:4: e2= whereClause
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
    // src/petascope/wcps/grammar/wcps.g:37:1: forClause returns [ForClauseElements value] : FOR v= coverageVariable IN LPAREN list= coverageList RPAREN ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )* ;
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
            // src/petascope/wcps/grammar/wcps.g:38:2: ( FOR v= coverageVariable IN LPAREN list= coverageList RPAREN ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )* )
            // src/petascope/wcps/grammar/wcps.g:38:4: FOR v= coverageVariable IN LPAREN list= coverageList RPAREN ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )*
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
            // src/petascope/wcps/grammar/wcps.g:40:4: ( COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==COMMA) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:40:5: COMMA v= coverageVariable IN LPAREN list= coverageList RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:43:1: whereClause returns [WhereClause value] : WHERE e1= booleanScalarExpr ;
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
            // src/petascope/wcps/grammar/wcps.g:44:2: ( WHERE e1= booleanScalarExpr )
            // src/petascope/wcps/grammar/wcps.g:44:4: WHERE e1= booleanScalarExpr
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
    // src/petascope/wcps/grammar/wcps.g:46:1: returnClause returns [ReturnClause value] : RETURN e1= processingExpr ;
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
            // src/petascope/wcps/grammar/wcps.g:47:2: ( RETURN e1= processingExpr )
            // src/petascope/wcps/grammar/wcps.g:47:4: RETURN e1= processingExpr
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
    // src/petascope/wcps/grammar/wcps.g:49:1: coverageList returns [CoverageList value] : cname= coverageName ( COMMA next= coverageName )* ;
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
            // src/petascope/wcps/grammar/wcps.g:50:2: (cname= coverageName ( COMMA next= coverageName )* )
            // src/petascope/wcps/grammar/wcps.g:50:4: cname= coverageName ( COMMA next= coverageName )*
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
            // src/petascope/wcps/grammar/wcps.g:51:3: ( COMMA next= coverageName )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==COMMA) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:51:4: COMMA next= coverageName
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
    // src/petascope/wcps/grammar/wcps.g:53:1: processingExpr returns [ProcessingExpr value] : (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr );
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
            // src/petascope/wcps/grammar/wcps.g:54:5: (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr )
            int alt4=3;
            alt4 = dfa4.predict(input);
            switch (alt4) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:54:7: e1= encodedCoverageExpr
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
                    // src/petascope/wcps/grammar/wcps.g:55:7: e2= storeExpr
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
                    // src/petascope/wcps/grammar/wcps.g:56:7: e3= scalarExpr
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
    // src/petascope/wcps/grammar/wcps.g:58:1: encodedCoverageExpr returns [EncodedCoverageExpr value] : ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN ;
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
            // src/petascope/wcps/grammar/wcps.g:59:2: ( ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN )
            // src/petascope/wcps/grammar/wcps.g:59:4: ENCODE LPAREN cov= coverageExpr COMMA format= stringConstant ( COMMA params= stringConstant )? RPAREN
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
            // src/petascope/wcps/grammar/wcps.g:60:3: ( COMMA params= stringConstant )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==COMMA) ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:60:4: COMMA params= stringConstant
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
    // src/petascope/wcps/grammar/wcps.g:62:1: storeExpr returns [StoreExpr value] : STORE LPAREN e1= encodedCoverageExpr RPAREN ;
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
            // src/petascope/wcps/grammar/wcps.g:63:5: ( STORE LPAREN e1= encodedCoverageExpr RPAREN )
            // src/petascope/wcps/grammar/wcps.g:63:7: STORE LPAREN e1= encodedCoverageExpr RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:65:1: coverageExpr returns [CoverageExpr value] : e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )* ;
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
            // src/petascope/wcps/grammar/wcps.g:66:5: (e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )* )
            // src/petascope/wcps/grammar/wcps.g:66:7: e1= coverageLogicTerm (op= ( OR | XOR ) e2= coverageLogicTerm )*
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
            // src/petascope/wcps/grammar/wcps.g:67:9: (op= ( OR | XOR ) e2= coverageLogicTerm )*
            loop6:
            do {
                int alt6=2;
                alt6 = dfa6.predict(input);
                switch (alt6) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:67:10: op= ( OR | XOR ) e2= coverageLogicTerm
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
    // src/petascope/wcps/grammar/wcps.g:69:1: coverageLogicTerm returns [CoverageExpr value] : e1= coverageLogicFactor (op= AND e2= coverageLogicFactor )* ;
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
            // src/petascope/wcps/grammar/wcps.g:70:5: (e1= coverageLogicFactor (op= AND e2= coverageLogicFactor )* )
            // src/petascope/wcps/grammar/wcps.g:70:7: e1= coverageLogicFactor (op= AND e2= coverageLogicFactor )*
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
            // src/petascope/wcps/grammar/wcps.g:71:9: (op= AND e2= coverageLogicFactor )*
            loop7:
            do {
                int alt7=2;
                alt7 = dfa7.predict(input);
                switch (alt7) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:71:10: op= AND e2= coverageLogicFactor
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
    // src/petascope/wcps/grammar/wcps.g:73:1: coverageLogicFactor returns [CoverageExpr value] : e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )? ;
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
            // src/petascope/wcps/grammar/wcps.g:74:5: (e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )? )
            // src/petascope/wcps/grammar/wcps.g:74:7: e1= coverageArithmeticExpr (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?
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
            // src/petascope/wcps/grammar/wcps.g:75:9: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?
            int alt8=2;
            alt8 = dfa8.predict(input);
            switch (alt8) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:75:10: op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr
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
    // src/petascope/wcps/grammar/wcps.g:77:1: coverageArithmeticExpr returns [CoverageExpr value] : e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )* ;
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
            // src/petascope/wcps/grammar/wcps.g:78:5: (e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )* )
            // src/petascope/wcps/grammar/wcps.g:78:7: e1= coverageArithmeticTerm (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*
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
            // src/petascope/wcps/grammar/wcps.g:79:9: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*
            loop9:
            do {
                int alt9=2;
                alt9 = dfa9.predict(input);
                switch (alt9) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:79:10: op= ( PLUS | MINUS ) e2= coverageArithmeticTerm
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
    // src/petascope/wcps/grammar/wcps.g:81:1: coverageArithmeticTerm returns [CoverageExpr value] : e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )* ;
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
            // src/petascope/wcps/grammar/wcps.g:82:5: (e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )* )
            // src/petascope/wcps/grammar/wcps.g:82:9: e1= coverageArithmeticFactor (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*
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
            // src/petascope/wcps/grammar/wcps.g:83:9: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*
            loop10:
            do {
                int alt10=2;
                alt10 = dfa10.predict(input);
                switch (alt10) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:83:10: op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor
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
    // src/petascope/wcps/grammar/wcps.g:85:1: coverageArithmeticFactor returns [CoverageExpr value] : e1= coverageValue (op= OVERLAY e2= coverageValue )* ;
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
            // src/petascope/wcps/grammar/wcps.g:86:5: (e1= coverageValue (op= OVERLAY e2= coverageValue )* )
            // src/petascope/wcps/grammar/wcps.g:86:7: e1= coverageValue (op= OVERLAY e2= coverageValue )*
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
            // src/petascope/wcps/grammar/wcps.g:87:9: (op= OVERLAY e2= coverageValue )*
            loop11:
            do {
                int alt11=2;
                alt11 = dfa11.predict(input);
                switch (alt11) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:87:10: op= OVERLAY e2= coverageValue
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
    // src/petascope/wcps/grammar/wcps.g:89:1: coverageValue returns [CoverageExpr value] : (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom );
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
            // src/petascope/wcps/grammar/wcps.g:90:5: (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom )
            int alt12=5;
            alt12 = dfa12.predict(input);
            switch (alt12) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:90:7: e5= subsetExpr
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
                    // src/petascope/wcps/grammar/wcps.g:91:7: e2= unaryInducedExpr
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
                    // src/petascope/wcps/grammar/wcps.g:92:7: e4= scaleExpr
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
                    // src/petascope/wcps/grammar/wcps.g:93:7: e3= crsTransformExpr
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
                    // src/petascope/wcps/grammar/wcps.g:94:7: e1= coverageAtom
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
    // src/petascope/wcps/grammar/wcps.g:96:1: coverageAtom returns [CoverageExpr value] : (e2= scalarExpr | e1= coverageVariable | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr );
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
            // src/petascope/wcps/grammar/wcps.g:97:5: (e2= scalarExpr | e1= coverageVariable | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr )
            int alt13=7;
            alt13 = dfa13.predict(input);
            switch (alt13) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:97:7: e2= scalarExpr
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
                    // src/petascope/wcps/grammar/wcps.g:98:7: e1= coverageVariable
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
                    // src/petascope/wcps/grammar/wcps.g:99:7: LPAREN e7= coverageExpr RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:100:7: e3= coverageConstantExpr
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
                    // src/petascope/wcps/grammar/wcps.g:101:7: e4= coverageConstructorExpr
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
                    // src/petascope/wcps/grammar/wcps.g:102:7: e5= setMetaDataExpr
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
                    // src/petascope/wcps/grammar/wcps.g:103:7: e6= rangeConstructorExpr
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
    // src/petascope/wcps/grammar/wcps.g:105:1: scalarExpr returns [ScalarExpr value] : (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN );
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
            // src/petascope/wcps/grammar/wcps.g:106:5: (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN )
            int alt14=6;
            alt14 = dfa14.predict(input);
            switch (alt14) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:106:7: e1= metaDataExpr
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
                    // src/petascope/wcps/grammar/wcps.g:107:7: e2= condenseExpr
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
                    // src/petascope/wcps/grammar/wcps.g:108:7: e3= booleanScalarExpr
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
                    // src/petascope/wcps/grammar/wcps.g:109:7: e4= numericScalarExpr
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
                    // src/petascope/wcps/grammar/wcps.g:110:7: e5= stringScalarExpr
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
                    // src/petascope/wcps/grammar/wcps.g:111:7: LPAREN e6= scalarExpr RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:113:1: metaDataExpr returns [MetaDataExpr value] : (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | op= IMAGECRS LPAREN e1= coverageExpr RPAREN | op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN | op= CRSSET LPAREN e1= coverageExpr RPAREN | de= domainExpr | op= NULLSET LPAREN e1= coverageExpr RPAREN | op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN | op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN );
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
            // src/petascope/wcps/grammar/wcps.g:114:5: (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | op= IMAGECRS LPAREN e1= coverageExpr RPAREN | op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN | op= CRSSET LPAREN e1= coverageExpr RPAREN | de= domainExpr | op= NULLSET LPAREN e1= coverageExpr RPAREN | op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN | op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN )
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
                    // src/petascope/wcps/grammar/wcps.g:114:7: op= IDENTIFIER LPAREN e1= coverageExpr RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:115:7: op= IMAGECRS LPAREN e1= coverageExpr RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:116:7: op= IMAGECRSDOMAIN LPAREN e1= coverageExpr ( COMMA e2= axisName )? RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:116:48: ( COMMA e2= axisName )?
                    int alt15=2;
                    int LA15_0 = input.LA(1);

                    if ( (LA15_0==COMMA) ) {
                        alt15=1;
                    }
                    switch (alt15) {
                        case 1 :
                            // src/petascope/wcps/grammar/wcps.g:116:49: COMMA e2= axisName
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
                    // src/petascope/wcps/grammar/wcps.g:117:7: op= CRSSET LPAREN e1= coverageExpr RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:118:7: de= domainExpr
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
                    // src/petascope/wcps/grammar/wcps.g:119:7: op= NULLSET LPAREN e1= coverageExpr RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:120:7: op= INTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:121:7: op= INTERPOLATIONSET LPAREN e1= coverageExpr COMMA f1= fieldName RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:123:1: domainExpr returns [DomainExpr value] : DOMAIN LPAREN var= coverageVariable COMMA axis= axisName COMMA crs= crsName RPAREN ;
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
            // src/petascope/wcps/grammar/wcps.g:124:2: ( DOMAIN LPAREN var= coverageVariable COMMA axis= axisName COMMA crs= crsName RPAREN )
            // src/petascope/wcps/grammar/wcps.g:124:4: DOMAIN LPAREN var= coverageVariable COMMA axis= axisName COMMA crs= crsName RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:126:1: condenseExpr returns [CondenseExpr value] : (e1= reduceExpr | e2= generalCondenseExpr );
    public final wcpsParser.condenseExpr_return condenseExpr() throws RecognitionException {
        wcpsParser.condenseExpr_return retval = new wcpsParser.condenseExpr_return();
        retval.start = input.LT(1);
        int condenseExpr_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.reduceExpr_return e1 = null;

        wcpsParser.generalCondenseExpr_return e2 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 20) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:127:2: (e1= reduceExpr | e2= generalCondenseExpr )
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
                    // src/petascope/wcps/grammar/wcps.g:127:4: e1= reduceExpr
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
                    // src/petascope/wcps/grammar/wcps.g:128:4: e2= generalCondenseExpr
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
    // src/petascope/wcps/grammar/wcps.g:130:1: reduceExpr returns [ReduceExpr value] : op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN ;
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
            // src/petascope/wcps/grammar/wcps.g:131:2: (op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN )
            // src/petascope/wcps/grammar/wcps.g:131:4: op= ( ALL | SOME | COUNT | ADD | AVG | MIN | MAX ) LPAREN e1= coverageExpr RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:133:1: generalCondenseExpr returns [GeneralCondenseExpr value] : CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING ce= coverageExpr ;
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
            // src/petascope/wcps/grammar/wcps.g:134:2: ( CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING ce= coverageExpr )
            // src/petascope/wcps/grammar/wcps.g:134:4: CONDENSE op= condenseOpType OVER ail= axisIteratorList ( WHERE cond= booleanScalarExpr )? USING ce= coverageExpr
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
            // src/petascope/wcps/grammar/wcps.g:135:3: ( WHERE cond= booleanScalarExpr )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0==WHERE) ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:135:4: WHERE cond= booleanScalarExpr
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
    // src/petascope/wcps/grammar/wcps.g:138:1: axisIteratorList returns [AxisIteratorList value] : vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )* ;
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
            // src/petascope/wcps/grammar/wcps.g:139:2: (vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )* )
            // src/petascope/wcps/grammar/wcps.g:139:4: vn= variableName an= axisName LPAREN ie= intervalExpr RPAREN ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )*
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
            // src/petascope/wcps/grammar/wcps.g:141:2: ( COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN )*
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( (LA19_0==COMMA) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:141:3: COMMA vn2= variableName an2= axisName LPAREN ie2= intervalExpr RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:144:1: intervalExpr returns [IntervalExpr value] : (lo= indexExpr COLON hi= indexExpr | IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN );
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
            // src/petascope/wcps/grammar/wcps.g:145:5: (lo= indexExpr COLON hi= indexExpr | IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN )
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
                    // src/petascope/wcps/grammar/wcps.g:145:7: lo= indexExpr COLON hi= indexExpr
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
                    // src/petascope/wcps/grammar/wcps.g:147:7: IMAGECRSDOMAIN LPAREN e1= coverageName COMMA e2= axisName RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:150:1: coverageConstantExpr returns [CoverageConstantExpr value] : COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT ;
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
            // src/petascope/wcps/grammar/wcps.g:151:5: ( COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT )
            // src/petascope/wcps/grammar/wcps.g:151:7: COVERAGE aname= coverageName OVER iter= axisIteratorList VALUE LIST LT values= constantList GT
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
    // src/petascope/wcps/grammar/wcps.g:154:1: constantList returns [ConstantList value] : c= constant ( SEMICOLON c= constant )* ;
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
            // src/petascope/wcps/grammar/wcps.g:155:5: (c= constant ( SEMICOLON c= constant )* )
            // src/petascope/wcps/grammar/wcps.g:155:7: c= constant ( SEMICOLON c= constant )*
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
            // src/petascope/wcps/grammar/wcps.g:155:59: ( SEMICOLON c= constant )*
            loop21:
            do {
                int alt21=2;
                int LA21_0 = input.LA(1);

                if ( (LA21_0==SEMICOLON) ) {
                    alt21=1;
                }


                switch (alt21) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:155:60: SEMICOLON c= constant
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
    // src/petascope/wcps/grammar/wcps.g:157:1: coverageConstructorExpr returns [CoverageConstructorExpr value] : COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr ;
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
            // src/petascope/wcps/grammar/wcps.g:158:2: ( COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr )
            // src/petascope/wcps/grammar/wcps.g:158:4: COVERAGE coverage= coverageName OVER ail= axisIteratorList VALUES se= scalarExpr
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
    // src/petascope/wcps/grammar/wcps.g:161:1: setMetaDataExpr returns [SetMetaDataExpr value] : (op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN | op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN | op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN | op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN | op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN );
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
            // src/petascope/wcps/grammar/wcps.g:162:5: (op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN | op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN | op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN | op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN | op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN )
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
                    // src/petascope/wcps/grammar/wcps.g:162:7: op= SETIDENTIFIER LPAREN s= stringConstant COMMA e1= coverageExpr RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:164:7: op= SETCRSSET LPAREN e1= coverageExpr COMMA crs= crsList RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:166:7: op= SETNULLSET LPAREN e1= coverageExpr COMMA rel= rangeExprList RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:168:7: op= SETINTERPOLATIONDEFAULT LPAREN e1= coverageExpr COMMA fn= fieldName COMMA im= interpolationMethod RPAREN
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
                    // src/petascope/wcps/grammar/wcps.g:170:7: op= SETINTERPOLATIONSET LPAREN e1= coverageExpr COMMA fn= fieldName COMMA iml= interpolationMethodList RPAREN
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
    // src/petascope/wcps/grammar/wcps.g:173:1: crsList returns [CrsList value] : LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE ;
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
            // src/petascope/wcps/grammar/wcps.g:174:5: ( LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE )
            // src/petascope/wcps/grammar/wcps.g:174:7: LBRACE (crs= crsName ( COMMA crs= crsName )* )? RBRACE
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
            // src/petascope/wcps/grammar/wcps.g:174:40: (crs= crsName ( COMMA crs= crsName )* )?
            int alt24=2;
            int LA24_0 = input.LA(1);

            if ( (LA24_0==STRING) ) {
                alt24=1;
            }
            switch (alt24) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:174:41: crs= crsName ( COMMA crs= crsName )*
                    {
                    pushFollow(FOLLOW_crsName_in_crsList1680);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((crs!=null?crs.value:null)); 
                    }
                    // src/petascope/wcps/grammar/wcps.g:174:81: ( COMMA crs= crsName )*
                    loop23:
                    do {
                        int alt23=2;
                        int LA23_0 = input.LA(1);

                        if ( (LA23_0==COMMA) ) {
                            alt23=1;
                        }


                        switch (alt23) {
                    	case 1 :
                    	    // src/petascope/wcps/grammar/wcps.g:174:82: COMMA crs= crsName
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
    // src/petascope/wcps/grammar/wcps.g:176:1: rangeExprList returns [RangeExprList value] : LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE ;
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
            // src/petascope/wcps/grammar/wcps.g:177:5: ( LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE )
            // src/petascope/wcps/grammar/wcps.g:177:7: LBRACE (re1= rangeExpr ( COMMA re2= rangeExpr )* )? RBRACE
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
            // src/petascope/wcps/grammar/wcps.g:177:48: (re1= rangeExpr ( COMMA re2= rangeExpr )* )?
            int alt26=2;
            int LA26_0 = input.LA(1);

            if ( (LA26_0==STRUCT) ) {
                alt26=1;
            }
            switch (alt26) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:177:49: re1= rangeExpr ( COMMA re2= rangeExpr )*
                    {
                    pushFollow(FOLLOW_rangeExpr_in_rangeExprList1724);
                    re1=rangeExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, re1.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((re1!=null?re1.value:null)); 
                    }
                    // src/petascope/wcps/grammar/wcps.g:177:91: ( COMMA re2= rangeExpr )*
                    loop25:
                    do {
                        int alt25=2;
                        int LA25_0 = input.LA(1);

                        if ( (LA25_0==COMMA) ) {
                            alt25=1;
                        }


                        switch (alt25) {
                    	case 1 :
                    	    // src/petascope/wcps/grammar/wcps.g:177:92: COMMA re2= rangeExpr
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
    // src/petascope/wcps/grammar/wcps.g:179:1: interpolationMethodList returns [InterpolationMethodList value] : LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE ;
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
            // src/petascope/wcps/grammar/wcps.g:180:2: ( LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE )
            // src/petascope/wcps/grammar/wcps.g:180:4: LBRACE (e= interpolationMethod ( COMMA e= interpolationMethod )* )? RBRACE
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
            // src/petascope/wcps/grammar/wcps.g:180:53: (e= interpolationMethod ( COMMA e= interpolationMethod )* )?
            int alt28=2;
            int LA28_0 = input.LA(1);

            if ( (LA28_0==LPAREN) ) {
                alt28=1;
            }
            switch (alt28) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:180:54: e= interpolationMethod ( COMMA e= interpolationMethod )*
                    {
                    pushFollow(FOLLOW_interpolationMethod_in_interpolationMethodList1765);
                    e=interpolationMethod();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e.getTree());
                    if ( state.backtracking==0 ) {
                       retval.value.add((e!=null?e.value:null)); 
                    }
                    // src/petascope/wcps/grammar/wcps.g:180:102: ( COMMA e= interpolationMethod )*
                    loop27:
                    do {
                        int alt27=2;
                        int LA27_0 = input.LA(1);

                        if ( (LA27_0==COMMA) ) {
                            alt27=1;
                        }


                        switch (alt27) {
                    	case 1 :
                    	    // src/petascope/wcps/grammar/wcps.g:180:103: COMMA e= interpolationMethod
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
    // src/petascope/wcps/grammar/wcps.g:182:1: rangeExpr returns [RangeExpr value] : STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )? RBRACE ;
    public final wcpsParser.rangeExpr_return rangeExpr() throws RecognitionException {
        wcpsParser.rangeExpr_return retval = new wcpsParser.rangeExpr_return();
        retval.start = input.LT(1);
        int rangeExpr_StartIndex = input.index();
        Object root_0 = null;

        Token STRUCT98=null;
        Token LBRACE99=null;
        Token COLON100=null;
        Token COLON101=null;
        Token COLON102=null;
        Token RBRACE103=null;
        wcpsParser.fieldName_return field = null;

        wcpsParser.scalarExpr_return expr = null;


        Object STRUCT98_tree=null;
        Object LBRACE99_tree=null;
        Object COLON100_tree=null;
        Object COLON101_tree=null;
        Object COLON102_tree=null;
        Object RBRACE103_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 32) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:183:2: ( STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )? RBRACE )
            // src/petascope/wcps/grammar/wcps.g:183:4: STRUCT LBRACE (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )? RBRACE
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
            // src/petascope/wcps/grammar/wcps.g:184:2: (field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )* )?
            int alt30=2;
            int LA30_0 = input.LA(1);

            if ( (LA30_0==INTEGERCONSTANT||LA30_0==STRING||LA30_0==NAME) ) {
                alt30=1;
            }
            switch (alt30) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:184:3: field= fieldName COLON expr= scalarExpr ( COLON field= fieldName COLON expr= scalarExpr )*
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
                    // src/petascope/wcps/grammar/wcps.g:185:3: ( COLON field= fieldName COLON expr= scalarExpr )*
                    loop29:
                    do {
                        int alt29=2;
                        int LA29_0 = input.LA(1);

                        if ( (LA29_0==COLON) ) {
                            alt29=1;
                        }


                        switch (alt29) {
                    	case 1 :
                    	    // src/petascope/wcps/grammar/wcps.g:185:4: COLON field= fieldName COLON expr= scalarExpr
                    	    {
                    	    COLON101=(Token)match(input,COLON,FOLLOW_COLON_in_rangeExpr1818); if (state.failed) return retval;
                    	    if ( state.backtracking==0 ) {
                    	    COLON101_tree = (Object)adaptor.create(COLON101);
                    	    adaptor.addChild(root_0, COLON101_tree);
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
    // src/petascope/wcps/grammar/wcps.g:188:1: rangeConstructorExpr returns [RangeConstructorExpr value] : ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE ;
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
            // src/petascope/wcps/grammar/wcps.g:189:5: ( ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE )
            // src/petascope/wcps/grammar/wcps.g:189:7: ( STRUCT )? LBRACE field= fieldName COLON expr= coverageExpr ( SEMICOLON field= fieldName COLON expr= coverageExpr )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            // src/petascope/wcps/grammar/wcps.g:189:7: ( STRUCT )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==STRUCT) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:189:8: STRUCT
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
            // src/petascope/wcps/grammar/wcps.g:190:9: ( SEMICOLON field= fieldName COLON expr= coverageExpr )*
            loop32:
            do {
                int alt32=2;
                int LA32_0 = input.LA(1);

                if ( (LA32_0==SEMICOLON) ) {
                    alt32=1;
                }


                switch (alt32) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:190:10: SEMICOLON field= fieldName COLON expr= coverageExpr
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
    // src/petascope/wcps/grammar/wcps.g:192:1: crsTransformExpr returns [CrsTransformExpr value] : CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN ;
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

        wcpsParser.dimensionIntervalList_return dcl = null;

        wcpsParser.fieldInterpolationList_return fil = null;


        Object CRSTRANSFORM110_tree=null;
        Object LPAREN111_tree=null;
        Object COMMA112_tree=null;
        Object COMMA113_tree=null;
        Object RPAREN114_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 34) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:193:2: ( CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN )
            // src/petascope/wcps/grammar/wcps.g:193:4: CRSTRANSFORM LPAREN e1= coverageExpr COMMA dcl= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            CRSTRANSFORM110=(Token)match(input,CRSTRANSFORM,FOLLOW_CRSTRANSFORM_in_crsTransformExpr1915); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            CRSTRANSFORM110_tree = (Object)adaptor.create(CRSTRANSFORM110);
            adaptor.addChild(root_0, CRSTRANSFORM110_tree);
            }
            LPAREN111=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_crsTransformExpr1917); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN111_tree = (Object)adaptor.create(LPAREN111);
            adaptor.addChild(root_0, LPAREN111_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_crsTransformExpr1921);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA112=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsTransformExpr1923); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA112_tree = (Object)adaptor.create(COMMA112);
            adaptor.addChild(root_0, COMMA112_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_crsTransformExpr1927);
            dcl=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dcl.getTree());
            COMMA113=(Token)match(input,COMMA,FOLLOW_COMMA_in_crsTransformExpr1929); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA113_tree = (Object)adaptor.create(COMMA113);
            adaptor.addChild(root_0, COMMA113_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationList_in_crsTransformExpr1933);
            fil=fieldInterpolationList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, fil.getTree());
            RPAREN114=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_crsTransformExpr1935); if (state.failed) return retval;
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

    public static class fieldInterpolationList_return extends ParserRuleReturnScope {
        public FieldInterpolationList value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "fieldInterpolationList"
    // src/petascope/wcps/grammar/wcps.g:196:1: fieldInterpolationList returns [FieldInterpolationList value] : LBRACE elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )* RBRACE ;
    public final wcpsParser.fieldInterpolationList_return fieldInterpolationList() throws RecognitionException {
        wcpsParser.fieldInterpolationList_return retval = new wcpsParser.fieldInterpolationList_return();
        retval.start = input.LT(1);
        int fieldInterpolationList_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACE115=null;
        Token COMMA116=null;
        Token RBRACE117=null;
        wcpsParser.fieldInterpolationElement_return elem = null;


        Object LBRACE115_tree=null;
        Object COMMA116_tree=null;
        Object RBRACE117_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 35) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:197:2: ( LBRACE elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )* RBRACE )
            // src/petascope/wcps/grammar/wcps.g:197:4: LBRACE elem= fieldInterpolationElement ( COMMA elem= fieldInterpolationElement )* RBRACE
            {
            root_0 = (Object)adaptor.nil();

            LBRACE115=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_fieldInterpolationList1952); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LBRACE115_tree = (Object)adaptor.create(LBRACE115);
            adaptor.addChild(root_0, LBRACE115_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1956);
            elem=fieldInterpolationElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new FieldInterpolationList((elem!=null?elem.value:null)); 
            }
            // src/petascope/wcps/grammar/wcps.g:198:3: ( COMMA elem= fieldInterpolationElement )*
            loop33:
            do {
                int alt33=2;
                int LA33_0 = input.LA(1);

                if ( (LA33_0==COMMA) ) {
                    alt33=1;
                }


                switch (alt33) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:198:4: COMMA elem= fieldInterpolationElement
            	    {
            	    COMMA116=(Token)match(input,COMMA,FOLLOW_COMMA_in_fieldInterpolationList1963); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA116_tree = (Object)adaptor.create(COMMA116);
            	    adaptor.addChild(root_0, COMMA116_tree);
            	    }
            	    pushFollow(FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1967);
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

            RBRACE117=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_fieldInterpolationList1974); if (state.failed) return retval;
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
    // src/petascope/wcps/grammar/wcps.g:200:1: fieldInterpolationElement returns [FieldInterpolationElement value] : aname= fieldName method= interpolationMethod ;
    public final wcpsParser.fieldInterpolationElement_return fieldInterpolationElement() throws RecognitionException {
        wcpsParser.fieldInterpolationElement_return retval = new wcpsParser.fieldInterpolationElement_return();
        retval.start = input.LT(1);
        int fieldInterpolationElement_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.fieldName_return aname = null;

        wcpsParser.interpolationMethod_return method = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 36) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:201:2: (aname= fieldName method= interpolationMethod )
            // src/petascope/wcps/grammar/wcps.g:201:4: aname= fieldName method= interpolationMethod
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_fieldName_in_fieldInterpolationElement1989);
            aname=fieldName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            pushFollow(FOLLOW_interpolationMethod_in_fieldInterpolationElement1993);
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
    // src/petascope/wcps/grammar/wcps.g:203:1: unaryInducedExpr returns [CoverageExpr value] : (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr );
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
            // src/petascope/wcps/grammar/wcps.g:204:5: (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr )
            int alt34=7;
            alt34 = dfa34.predict(input);
            switch (alt34) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:204:7: e6= fieldExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_fieldExpr_in_unaryInducedExpr2013);
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
                    // src/petascope/wcps/grammar/wcps.g:205:4: e1= unaryArithmeticExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_unaryArithmeticExpr_in_unaryInducedExpr2022);
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
                    // src/petascope/wcps/grammar/wcps.g:206:7: e2= exponentialExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_exponentialExpr_in_unaryInducedExpr2034);
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
                    // src/petascope/wcps/grammar/wcps.g:207:7: e3= trigonometricExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_trigonometricExpr_in_unaryInducedExpr2046);
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
                    // src/petascope/wcps/grammar/wcps.g:208:7: e4= booleanExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanExpr_in_unaryInducedExpr2058);
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
                    // src/petascope/wcps/grammar/wcps.g:209:7: e5= castExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_castExpr_in_unaryInducedExpr2070);
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
                    // src/petascope/wcps/grammar/wcps.g:210:7: e7= rangeConstructorExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_rangeConstructorExpr_in_unaryInducedExpr2082);
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
    // src/petascope/wcps/grammar/wcps.g:212:1: unaryArithmeticExpr returns [CoverageExpr value] : (op= ( MINUS | PLUS ) e1= coverageAtom | op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN );
    public final wcpsParser.unaryArithmeticExpr_return unaryArithmeticExpr() throws RecognitionException {
        wcpsParser.unaryArithmeticExpr_return retval = new wcpsParser.unaryArithmeticExpr_return();
        retval.start = input.LT(1);
        int unaryArithmeticExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN118=null;
        Token RPAREN119=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object op_tree=null;
        Object LPAREN118_tree=null;
        Object RPAREN119_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 38) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:213:5: (op= ( MINUS | PLUS ) e1= coverageAtom | op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN )
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
                    // src/petascope/wcps/grammar/wcps.g:213:7: op= ( MINUS | PLUS ) e1= coverageAtom
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

                    pushFollow(FOLLOW_coverageAtom_in_unaryArithmeticExpr2113);
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
                    // src/petascope/wcps/grammar/wcps.g:214:7: op= ( SQRT | ABS | RE | IM ) LPAREN e2= coverageExpr RPAREN
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

                    LPAREN118=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_unaryArithmeticExpr2135); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN118_tree = (Object)adaptor.create(LPAREN118);
                    adaptor.addChild(root_0, LPAREN118_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_unaryArithmeticExpr2139);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN119=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_unaryArithmeticExpr2141); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN119_tree = (Object)adaptor.create(RPAREN119);
                    adaptor.addChild(root_0, RPAREN119_tree);
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
    // src/petascope/wcps/grammar/wcps.g:216:1: exponentialExpr returns [ExponentialExpr value] : op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.exponentialExpr_return exponentialExpr() throws RecognitionException {
        wcpsParser.exponentialExpr_return retval = new wcpsParser.exponentialExpr_return();
        retval.start = input.LT(1);
        int exponentialExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN120=null;
        Token RPAREN121=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN120_tree=null;
        Object RPAREN121_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 39) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:217:5: (op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN )
            // src/petascope/wcps/grammar/wcps.g:217:7: op= ( EXP | LOG | LN ) LPAREN e1= coverageExpr RPAREN
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

            LPAREN120=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_exponentialExpr2172); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN120_tree = (Object)adaptor.create(LPAREN120);
            adaptor.addChild(root_0, LPAREN120_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_exponentialExpr2176);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN121=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_exponentialExpr2178); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN121_tree = (Object)adaptor.create(RPAREN121);
            adaptor.addChild(root_0, RPAREN121_tree);
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
    // src/petascope/wcps/grammar/wcps.g:219:1: trigonometricExpr returns [TrigonometricExpr value] : op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN ;
    public final wcpsParser.trigonometricExpr_return trigonometricExpr() throws RecognitionException {
        wcpsParser.trigonometricExpr_return retval = new wcpsParser.trigonometricExpr_return();
        retval.start = input.LT(1);
        int trigonometricExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN122=null;
        Token RPAREN123=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object LPAREN122_tree=null;
        Object RPAREN123_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 40) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:220:5: (op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN )
            // src/petascope/wcps/grammar/wcps.g:220:7: op= ( SIN | COS | TAN | SINH | COSH | TANH | ARCSIN | ARCCOS | ARCTAN ) LPAREN e1= coverageExpr RPAREN
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

            LPAREN122=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_trigonometricExpr2221); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN122_tree = (Object)adaptor.create(LPAREN122);
            adaptor.addChild(root_0, LPAREN122_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_trigonometricExpr2225);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN123=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_trigonometricExpr2227); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN123_tree = (Object)adaptor.create(RPAREN123);
            adaptor.addChild(root_0, RPAREN123_tree);
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
    // src/petascope/wcps/grammar/wcps.g:222:1: booleanExpr returns [BooleanExpr value] : (op= NOT e1= coverageExpr | op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN );
    public final wcpsParser.booleanExpr_return booleanExpr() throws RecognitionException {
        wcpsParser.booleanExpr_return retval = new wcpsParser.booleanExpr_return();
        retval.start = input.LT(1);
        int booleanExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token LPAREN124=null;
        Token COMMA125=null;
        Token RPAREN126=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.indexExpr_return e2 = null;


        Object op_tree=null;
        Object LPAREN124_tree=null;
        Object COMMA125_tree=null;
        Object RPAREN126_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 41) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:223:5: (op= NOT e1= coverageExpr | op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN )
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
                    // src/petascope/wcps/grammar/wcps.g:223:7: op= NOT e1= coverageExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NOT,FOLLOW_NOT_in_booleanExpr2250); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_booleanExpr2254);
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
                    // src/petascope/wcps/grammar/wcps.g:224:7: op= BIT LPAREN e1= coverageExpr COMMA e2= indexExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,BIT,FOLLOW_BIT_in_booleanExpr2266); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN124=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_booleanExpr2268); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN124_tree = (Object)adaptor.create(LPAREN124);
                    adaptor.addChild(root_0, LPAREN124_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_booleanExpr2272);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COMMA125=(Token)match(input,COMMA,FOLLOW_COMMA_in_booleanExpr2274); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA125_tree = (Object)adaptor.create(COMMA125);
                    adaptor.addChild(root_0, COMMA125_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_booleanExpr2278);
                    e2=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN126=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_booleanExpr2280); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN126_tree = (Object)adaptor.create(RPAREN126);
                    adaptor.addChild(root_0, RPAREN126_tree);
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
    // src/petascope/wcps/grammar/wcps.g:226:1: indexExpr returns [IndexExpr value] : e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 42) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:227:5: (e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )* )
            // src/petascope/wcps/grammar/wcps.g:227:7: e1= indexTerm (op= ( PLUS | MINUS ) e2= indexTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_indexTerm_in_indexExpr2303);
            e1=indexTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/petascope/wcps/grammar/wcps.g:228:3: (op= ( PLUS | MINUS ) e2= indexTerm )*
            loop37:
            do {
                int alt37=2;
                int LA37_0 = input.LA(1);

                if ( ((LA37_0>=PLUS && LA37_0<=MINUS)) ) {
                    alt37=1;
                }


                switch (alt37) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:228:4: op= ( PLUS | MINUS ) e2= indexTerm
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

            	    pushFollow(FOLLOW_indexTerm_in_indexExpr2321);
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
            	    break loop37;
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
    // src/petascope/wcps/grammar/wcps.g:230:1: indexTerm returns [IndexExpr value] : e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )* ;
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
            // src/petascope/wcps/grammar/wcps.g:231:5: (e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )* )
            // src/petascope/wcps/grammar/wcps.g:231:7: e1= indexFactor ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_indexFactor_in_indexTerm2346);
            e1=indexFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/petascope/wcps/grammar/wcps.g:232:6: ( (op= ( MULT | DIVIDE ) e2= indexFactor ) )*
            loop38:
            do {
                int alt38=2;
                int LA38_0 = input.LA(1);

                if ( ((LA38_0>=MULT && LA38_0<=DIVIDE)) ) {
                    alt38=1;
                }


                switch (alt38) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:232:7: (op= ( MULT | DIVIDE ) e2= indexFactor )
            	    {
            	    // src/petascope/wcps/grammar/wcps.g:232:7: (op= ( MULT | DIVIDE ) e2= indexFactor )
            	    // src/petascope/wcps/grammar/wcps.g:232:8: op= ( MULT | DIVIDE ) e2= indexFactor
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

            	    pushFollow(FOLLOW_indexFactor_in_indexTerm2367);
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
    // src/petascope/wcps/grammar/wcps.g:234:1: indexFactor returns [IndexExpr value] : (e= INTEGERCONSTANT | op= ROUND LPAREN e1= numericScalarExpr RPAREN | ( LPAREN e2= indexExpr RPAREN ) );
    public final wcpsParser.indexFactor_return indexFactor() throws RecognitionException {
        wcpsParser.indexFactor_return retval = new wcpsParser.indexFactor_return();
        retval.start = input.LT(1);
        int indexFactor_StartIndex = input.index();
        Object root_0 = null;

        Token e=null;
        Token op=null;
        Token LPAREN127=null;
        Token RPAREN128=null;
        Token LPAREN129=null;
        Token RPAREN130=null;
        wcpsParser.numericScalarExpr_return e1 = null;

        wcpsParser.indexExpr_return e2 = null;


        Object e_tree=null;
        Object op_tree=null;
        Object LPAREN127_tree=null;
        Object RPAREN128_tree=null;
        Object LPAREN129_tree=null;
        Object RPAREN130_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 44) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:235:5: (e= INTEGERCONSTANT | op= ROUND LPAREN e1= numericScalarExpr RPAREN | ( LPAREN e2= indexExpr RPAREN ) )
            int alt39=3;
            switch ( input.LA(1) ) {
            case INTEGERCONSTANT:
                {
                alt39=1;
                }
                break;
            case ROUND:
                {
                alt39=2;
                }
                break;
            case LPAREN:
                {
                alt39=3;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 39, 0, input);

                throw nvae;
            }

            switch (alt39) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:235:7: e= INTEGERCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,INTEGERCONSTANT,FOLLOW_INTEGERCONSTANT_in_indexFactor2394); if (state.failed) return retval;
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
                    // src/petascope/wcps/grammar/wcps.g:236:7: op= ROUND LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ROUND,FOLLOW_ROUND_in_indexFactor2407); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN127=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_indexFactor2409); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN127_tree = (Object)adaptor.create(LPAREN127);
                    adaptor.addChild(root_0, LPAREN127_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_indexFactor2413);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN128=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_indexFactor2415); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN128_tree = (Object)adaptor.create(RPAREN128);
                    adaptor.addChild(root_0, RPAREN128_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new IndexExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 3 :
                    // src/petascope/wcps/grammar/wcps.g:237:7: ( LPAREN e2= indexExpr RPAREN )
                    {
                    root_0 = (Object)adaptor.nil();

                    // src/petascope/wcps/grammar/wcps.g:237:7: ( LPAREN e2= indexExpr RPAREN )
                    // src/petascope/wcps/grammar/wcps.g:237:8: LPAREN e2= indexExpr RPAREN
                    {
                    LPAREN129=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_indexFactor2427); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN129_tree = (Object)adaptor.create(LPAREN129);
                    adaptor.addChild(root_0, LPAREN129_tree);
                    }
                    pushFollow(FOLLOW_indexExpr_in_indexFactor2431);
                    e2=indexExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    RPAREN130=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_indexFactor2433); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN130_tree = (Object)adaptor.create(RPAREN130);
                    adaptor.addChild(root_0, RPAREN130_tree);
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
    // src/petascope/wcps/grammar/wcps.g:239:1: stringScalarExpr returns [StringScalarExpr value] : (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | e= STRING );
    public final wcpsParser.stringScalarExpr_return stringScalarExpr() throws RecognitionException {
        wcpsParser.stringScalarExpr_return retval = new wcpsParser.stringScalarExpr_return();
        retval.start = input.LT(1);
        int stringScalarExpr_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token e=null;
        Token LPAREN131=null;
        Token RPAREN132=null;
        wcpsParser.coverageExpr_return e1 = null;


        Object op_tree=null;
        Object e_tree=null;
        Object LPAREN131_tree=null;
        Object RPAREN132_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 45) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:241:5: (op= IDENTIFIER LPAREN e1= coverageExpr RPAREN | e= STRING )
            int alt40=2;
            int LA40_0 = input.LA(1);

            if ( (LA40_0==IDENTIFIER) ) {
                alt40=1;
            }
            else if ( (LA40_0==STRING) ) {
                alt40=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 40, 0, input);

                throw nvae;
            }
            switch (alt40) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:241:7: op= IDENTIFIER LPAREN e1= coverageExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,IDENTIFIER,FOLLOW_IDENTIFIER_in_stringScalarExpr2460); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN131=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_stringScalarExpr2462); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN131_tree = (Object)adaptor.create(LPAREN131);
                    adaptor.addChild(root_0, LPAREN131_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_stringScalarExpr2466);
                    e1=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN132=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_stringScalarExpr2468); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN132_tree = (Object)adaptor.create(RPAREN132);
                    adaptor.addChild(root_0, RPAREN132_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new StringScalarExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:242:7: e= STRING
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,STRING,FOLLOW_STRING_in_stringScalarExpr2481); if (state.failed) return retval;
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
    // src/petascope/wcps/grammar/wcps.g:244:1: scaleExpr returns [ScaleExpr value] : SCALE LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN ;
    public final wcpsParser.scaleExpr_return scaleExpr() throws RecognitionException {
        wcpsParser.scaleExpr_return retval = new wcpsParser.scaleExpr_return();
        retval.start = input.LT(1);
        int scaleExpr_StartIndex = input.index();
        Object root_0 = null;

        Token SCALE133=null;
        Token LPAREN134=null;
        Token COMMA135=null;
        Token COMMA136=null;
        Token RPAREN137=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;

        wcpsParser.fieldInterpolationList_return fil = null;


        Object SCALE133_tree=null;
        Object LPAREN134_tree=null;
        Object COMMA135_tree=null;
        Object COMMA136_tree=null;
        Object RPAREN137_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 46) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:245:2: ( SCALE LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN )
            // src/petascope/wcps/grammar/wcps.g:245:4: SCALE LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList COMMA fil= fieldInterpolationList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            SCALE133=(Token)match(input,SCALE,FOLLOW_SCALE_in_scaleExpr2499); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            SCALE133_tree = (Object)adaptor.create(SCALE133);
            adaptor.addChild(root_0, SCALE133_tree);
            }
            LPAREN134=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_scaleExpr2501); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN134_tree = (Object)adaptor.create(LPAREN134);
            adaptor.addChild(root_0, LPAREN134_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_scaleExpr2505);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA135=(Token)match(input,COMMA,FOLLOW_COMMA_in_scaleExpr2507); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA135_tree = (Object)adaptor.create(COMMA135);
            adaptor.addChild(root_0, COMMA135_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_scaleExpr2511);
            dil=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
            COMMA136=(Token)match(input,COMMA,FOLLOW_COMMA_in_scaleExpr2513); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA136_tree = (Object)adaptor.create(COMMA136);
            adaptor.addChild(root_0, COMMA136_tree);
            }
            pushFollow(FOLLOW_fieldInterpolationList_in_scaleExpr2517);
            fil=fieldInterpolationList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, fil.getTree());
            RPAREN137=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_scaleExpr2519); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN137_tree = (Object)adaptor.create(RPAREN137);
            adaptor.addChild(root_0, RPAREN137_tree);
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
    // src/petascope/wcps/grammar/wcps.g:248:1: subsetExpr returns [SubsetExpr value] : (e1= trimExpr | e2= sliceExpr | e3= extendExpr );
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
            // src/petascope/wcps/grammar/wcps.g:249:2: (e1= trimExpr | e2= sliceExpr | e3= extendExpr )
            int alt41=3;
            alt41 = dfa41.predict(input);
            switch (alt41) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:249:4: e1= trimExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_trimExpr_in_subsetExpr2538);
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
                    // src/petascope/wcps/grammar/wcps.g:250:4: e2= sliceExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_sliceExpr_in_subsetExpr2547);
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
                    // src/petascope/wcps/grammar/wcps.g:251:4: e3= extendExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_extendExpr_in_subsetExpr2556);
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
    // src/petascope/wcps/grammar/wcps.g:253:1: trimExpr returns [TrimExpr value] : (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN );
    public final wcpsParser.trimExpr_return trimExpr() throws RecognitionException {
        wcpsParser.trimExpr_return retval = new wcpsParser.trimExpr_return();
        retval.start = input.LT(1);
        int trimExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACKET138=null;
        Token RBRACKET139=null;
        Token TRIM140=null;
        Token LPAREN141=null;
        Token COMMA142=null;
        Token LBRACE143=null;
        Token RBRACE144=null;
        Token RPAREN145=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LBRACKET138_tree=null;
        Object RBRACKET139_tree=null;
        Object TRIM140_tree=null;
        Object LPAREN141_tree=null;
        Object COMMA142_tree=null;
        Object LBRACE143_tree=null;
        Object RBRACE144_tree=null;
        Object RPAREN145_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 48) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:254:2: (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN )
            int alt42=2;
            alt42 = dfa42.predict(input);
            switch (alt42) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:254:4: e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_trimExpr2573);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    LBRACKET138=(Token)match(input,LBRACKET,FOLLOW_LBRACKET_in_trimExpr2575); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACKET138_tree = (Object)adaptor.create(LBRACKET138);
                    adaptor.addChild(root_0, LBRACKET138_tree);
                    }
                    pushFollow(FOLLOW_dimensionIntervalList_in_trimExpr2579);
                    dil=dimensionIntervalList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
                    RBRACKET139=(Token)match(input,RBRACKET,FOLLOW_RBRACKET_in_trimExpr2581); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACKET139_tree = (Object)adaptor.create(RBRACKET139);
                    adaptor.addChild(root_0, RBRACKET139_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new TrimExpr((e1!=null?e1.value:null), (dil!=null?dil.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:255:6: TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    TRIM140=(Token)match(input,TRIM,FOLLOW_TRIM_in_trimExpr2590); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    TRIM140_tree = (Object)adaptor.create(TRIM140);
                    adaptor.addChild(root_0, TRIM140_tree);
                    }
                    LPAREN141=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_trimExpr2592); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN141_tree = (Object)adaptor.create(LPAREN141);
                    adaptor.addChild(root_0, LPAREN141_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_trimExpr2596);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    COMMA142=(Token)match(input,COMMA,FOLLOW_COMMA_in_trimExpr2598); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA142_tree = (Object)adaptor.create(COMMA142);
                    adaptor.addChild(root_0, COMMA142_tree);
                    }
                    LBRACE143=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_trimExpr2600); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACE143_tree = (Object)adaptor.create(LBRACE143);
                    adaptor.addChild(root_0, LBRACE143_tree);
                    }
                    pushFollow(FOLLOW_dimensionIntervalList_in_trimExpr2604);
                    dil=dimensionIntervalList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
                    RBRACE144=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_trimExpr2606); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACE144_tree = (Object)adaptor.create(RBRACE144);
                    adaptor.addChild(root_0, RBRACE144_tree);
                    }
                    RPAREN145=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_trimExpr2608); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN145_tree = (Object)adaptor.create(RPAREN145);
                    adaptor.addChild(root_0, RPAREN145_tree);
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
    // src/petascope/wcps/grammar/wcps.g:257:1: sliceExpr returns [SliceExpr value] : (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN );
    public final wcpsParser.sliceExpr_return sliceExpr() throws RecognitionException {
        wcpsParser.sliceExpr_return retval = new wcpsParser.sliceExpr_return();
        retval.start = input.LT(1);
        int sliceExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LBRACKET146=null;
        Token RBRACKET147=null;
        Token SLICE148=null;
        Token LPAREN149=null;
        Token COMMA150=null;
        Token LBRACE151=null;
        Token RBRACE152=null;
        Token RPAREN153=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.dimensionPointList_return dpl = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LBRACKET146_tree=null;
        Object RBRACKET147_tree=null;
        Object SLICE148_tree=null;
        Object LPAREN149_tree=null;
        Object COMMA150_tree=null;
        Object LBRACE151_tree=null;
        Object RBRACE152_tree=null;
        Object RPAREN153_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 49) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:258:2: (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN )
            int alt43=2;
            alt43 = dfa43.predict(input);
            switch (alt43) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:258:4: e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_coverageAtom_in_sliceExpr2625);
                    e1=coverageAtom();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    LBRACKET146=(Token)match(input,LBRACKET,FOLLOW_LBRACKET_in_sliceExpr2627); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACKET146_tree = (Object)adaptor.create(LBRACKET146);
                    adaptor.addChild(root_0, LBRACKET146_tree);
                    }
                    pushFollow(FOLLOW_dimensionPointList_in_sliceExpr2631);
                    dpl=dimensionPointList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpl.getTree());
                    RBRACKET147=(Token)match(input,RBRACKET,FOLLOW_RBRACKET_in_sliceExpr2633); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACKET147_tree = (Object)adaptor.create(RBRACKET147);
                    adaptor.addChild(root_0, RBRACKET147_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new SliceExpr((e1!=null?e1.value:null), (dpl!=null?dpl.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:259:4: SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    SLICE148=(Token)match(input,SLICE,FOLLOW_SLICE_in_sliceExpr2640); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    SLICE148_tree = (Object)adaptor.create(SLICE148);
                    adaptor.addChild(root_0, SLICE148_tree);
                    }
                    LPAREN149=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_sliceExpr2642); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN149_tree = (Object)adaptor.create(LPAREN149);
                    adaptor.addChild(root_0, LPAREN149_tree);
                    }
                    pushFollow(FOLLOW_coverageExpr_in_sliceExpr2646);
                    e2=coverageExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e2.getTree());
                    COMMA150=(Token)match(input,COMMA,FOLLOW_COMMA_in_sliceExpr2648); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COMMA150_tree = (Object)adaptor.create(COMMA150);
                    adaptor.addChild(root_0, COMMA150_tree);
                    }
                    LBRACE151=(Token)match(input,LBRACE,FOLLOW_LBRACE_in_sliceExpr2650); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LBRACE151_tree = (Object)adaptor.create(LBRACE151);
                    adaptor.addChild(root_0, LBRACE151_tree);
                    }
                    pushFollow(FOLLOW_dimensionPointList_in_sliceExpr2654);
                    dpl=dimensionPointList();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpl.getTree());
                    RBRACE152=(Token)match(input,RBRACE,FOLLOW_RBRACE_in_sliceExpr2656); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RBRACE152_tree = (Object)adaptor.create(RBRACE152);
                    adaptor.addChild(root_0, RBRACE152_tree);
                    }
                    RPAREN153=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_sliceExpr2658); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN153_tree = (Object)adaptor.create(RPAREN153);
                    adaptor.addChild(root_0, RPAREN153_tree);
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
    // src/petascope/wcps/grammar/wcps.g:261:1: extendExpr returns [ExtendExpr value] : EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN ;
    public final wcpsParser.extendExpr_return extendExpr() throws RecognitionException {
        wcpsParser.extendExpr_return retval = new wcpsParser.extendExpr_return();
        retval.start = input.LT(1);
        int extendExpr_StartIndex = input.index();
        Object root_0 = null;

        Token EXTEND154=null;
        Token LPAREN155=null;
        Token COMMA156=null;
        Token RPAREN157=null;
        wcpsParser.coverageExpr_return e1 = null;

        wcpsParser.dimensionIntervalList_return dil = null;


        Object EXTEND154_tree=null;
        Object LPAREN155_tree=null;
        Object COMMA156_tree=null;
        Object RPAREN157_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 50) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:262:2: ( EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN )
            // src/petascope/wcps/grammar/wcps.g:262:4: EXTEND LPAREN e1= coverageExpr COMMA dil= dimensionIntervalList RPAREN
            {
            root_0 = (Object)adaptor.nil();

            EXTEND154=(Token)match(input,EXTEND,FOLLOW_EXTEND_in_extendExpr2673); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            EXTEND154_tree = (Object)adaptor.create(EXTEND154);
            adaptor.addChild(root_0, EXTEND154_tree);
            }
            LPAREN155=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_extendExpr2675); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN155_tree = (Object)adaptor.create(LPAREN155);
            adaptor.addChild(root_0, LPAREN155_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_extendExpr2679);
            e1=coverageExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            COMMA156=(Token)match(input,COMMA,FOLLOW_COMMA_in_extendExpr2681); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA156_tree = (Object)adaptor.create(COMMA156);
            adaptor.addChild(root_0, COMMA156_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalList_in_extendExpr2685);
            dil=dimensionIntervalList();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, dil.getTree());
            RPAREN157=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_extendExpr2687); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN157_tree = (Object)adaptor.create(RPAREN157);
            adaptor.addChild(root_0, RPAREN157_tree);
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
    // src/petascope/wcps/grammar/wcps.g:264:1: castExpr returns [CastExpr value] : LPAREN e1= rangeType RPAREN e2= coverageExpr ;
    public final wcpsParser.castExpr_return castExpr() throws RecognitionException {
        wcpsParser.castExpr_return retval = new wcpsParser.castExpr_return();
        retval.start = input.LT(1);
        int castExpr_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN158=null;
        Token RPAREN159=null;
        wcpsParser.rangeType_return e1 = null;

        wcpsParser.coverageExpr_return e2 = null;


        Object LPAREN158_tree=null;
        Object RPAREN159_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 51) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:265:5: ( LPAREN e1= rangeType RPAREN e2= coverageExpr )
            // src/petascope/wcps/grammar/wcps.g:265:7: LPAREN e1= rangeType RPAREN e2= coverageExpr
            {
            root_0 = (Object)adaptor.nil();

            LPAREN158=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_castExpr2705); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN158_tree = (Object)adaptor.create(LPAREN158);
            adaptor.addChild(root_0, LPAREN158_tree);
            }
            pushFollow(FOLLOW_rangeType_in_castExpr2709);
            e1=rangeType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            RPAREN159=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_castExpr2711); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN159_tree = (Object)adaptor.create(RPAREN159);
            adaptor.addChild(root_0, RPAREN159_tree);
            }
            pushFollow(FOLLOW_coverageExpr_in_castExpr2715);
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
    // src/petascope/wcps/grammar/wcps.g:267:1: rangeType returns [String value] : (type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 ) | UNSIGNED type= ( CHAR | SHORT | LONG ) );
    public final wcpsParser.rangeType_return rangeType() throws RecognitionException {
        wcpsParser.rangeType_return retval = new wcpsParser.rangeType_return();
        retval.start = input.LT(1);
        int rangeType_StartIndex = input.index();
        Object root_0 = null;

        Token type=null;
        Token UNSIGNED160=null;

        Object type_tree=null;
        Object UNSIGNED160_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 52) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:268:5: (type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 ) | UNSIGNED type= ( CHAR | SHORT | LONG ) )
            int alt44=2;
            int LA44_0 = input.LA(1);

            if ( ((LA44_0>=BOOLEAN && LA44_0<=COMPLEX2)) ) {
                alt44=1;
            }
            else if ( (LA44_0==UNSIGNED) ) {
                alt44=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 44, 0, input);

                throw nvae;
            }
            switch (alt44) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:268:7: type= ( BOOLEAN | CHAR | SHORT | LONG | FLOAT | DOUBLE | COMPLEX | COMPLEX2 )
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
                    // src/petascope/wcps/grammar/wcps.g:269:7: UNSIGNED type= ( CHAR | SHORT | LONG )
                    {
                    root_0 = (Object)adaptor.nil();

                    UNSIGNED160=(Token)match(input,UNSIGNED,FOLLOW_UNSIGNED_in_rangeType2764); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    UNSIGNED160_tree = (Object)adaptor.create(UNSIGNED160);
                    adaptor.addChild(root_0, UNSIGNED160_tree);
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
    // src/petascope/wcps/grammar/wcps.g:271:1: fieldExpr returns [SelectExpr value] : e1= coverageAtom DOT e2= fieldName ;
    public final wcpsParser.fieldExpr_return fieldExpr() throws RecognitionException {
        wcpsParser.fieldExpr_return retval = new wcpsParser.fieldExpr_return();
        retval.start = input.LT(1);
        int fieldExpr_StartIndex = input.index();
        Object root_0 = null;

        Token DOT161=null;
        wcpsParser.coverageAtom_return e1 = null;

        wcpsParser.fieldName_return e2 = null;


        Object DOT161_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 53) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:272:5: (e1= coverageAtom DOT e2= fieldName )
            // src/petascope/wcps/grammar/wcps.g:272:7: e1= coverageAtom DOT e2= fieldName
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_coverageAtom_in_fieldExpr2797);
            e1=coverageAtom();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            DOT161=(Token)match(input,DOT,FOLLOW_DOT_in_fieldExpr2799); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            DOT161_tree = (Object)adaptor.create(DOT161);
            adaptor.addChild(root_0, DOT161_tree);
            }
            pushFollow(FOLLOW_fieldName_in_fieldExpr2803);
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
    // src/petascope/wcps/grammar/wcps.g:276:1: booleanScalarExpr returns [BooleanScalarExpr value] : e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 54) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:277:5: (e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )* )
            // src/petascope/wcps/grammar/wcps.g:277:7: e1= booleanScalarTerm (op= ( OR | XOR ) e2= booleanScalarTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_booleanScalarTerm_in_booleanScalarExpr2828);
            e1=booleanScalarTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/petascope/wcps/grammar/wcps.g:278:7: (op= ( OR | XOR ) e2= booleanScalarTerm )*
            loop45:
            do {
                int alt45=2;
                alt45 = dfa45.predict(input);
                switch (alt45) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:278:8: op= ( OR | XOR ) e2= booleanScalarTerm
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

            	    pushFollow(FOLLOW_booleanScalarTerm_in_booleanScalarExpr2849);
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
            	    break loop45;
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
    // src/petascope/wcps/grammar/wcps.g:280:1: booleanScalarTerm returns [BooleanScalarExpr value] : e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )* ;
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
            // src/petascope/wcps/grammar/wcps.g:281:2: (e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )* )
            // src/petascope/wcps/grammar/wcps.g:281:4: e1= booleanScalarNegation (op= AND e2= booleanScalarNegation )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_booleanScalarNegation_in_booleanScalarTerm2871);
            e1=booleanScalarNegation();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/petascope/wcps/grammar/wcps.g:282:4: (op= AND e2= booleanScalarNegation )*
            loop46:
            do {
                int alt46=2;
                alt46 = dfa46.predict(input);
                switch (alt46) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:282:5: op= AND e2= booleanScalarNegation
            	    {
            	    op=(Token)match(input,AND,FOLLOW_AND_in_booleanScalarTerm2881); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    op_tree = (Object)adaptor.create(op);
            	    adaptor.addChild(root_0, op_tree);
            	    }
            	    pushFollow(FOLLOW_booleanScalarNegation_in_booleanScalarTerm2885);
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
            	    break loop46;
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
    // src/petascope/wcps/grammar/wcps.g:284:1: booleanScalarNegation returns [BooleanScalarExpr value] : (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom );
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
            // src/petascope/wcps/grammar/wcps.g:285:2: (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom )
            int alt47=2;
            alt47 = dfa47.predict(input);
            switch (alt47) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:285:4: e1= booleanScalarAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_booleanScalarAtom_in_booleanScalarNegation2906);
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
                    // src/petascope/wcps/grammar/wcps.g:286:4: op= NOT e1= booleanScalarAtom
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,NOT,FOLLOW_NOT_in_booleanScalarNegation2915); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarAtom_in_booleanScalarNegation2919);
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
    // src/petascope/wcps/grammar/wcps.g:288:1: booleanScalarAtom returns [BooleanScalarExpr value] : ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT );
    public final wcpsParser.booleanScalarAtom_return booleanScalarAtom() throws RecognitionException {
        wcpsParser.booleanScalarAtom_return retval = new wcpsParser.booleanScalarAtom_return();
        retval.start = input.LT(1);
        int booleanScalarAtom_StartIndex = input.index();
        Object root_0 = null;

        Token e=null;
        Token LPAREN162=null;
        Token RPAREN163=null;
        wcpsParser.booleanScalarExpr_return e1 = null;

        wcpsParser.stringScalarExpr_return s1 = null;

        wcpsParser.compOp_return cop = null;

        wcpsParser.stringScalarExpr_return s2 = null;

        wcpsParser.numericScalarExpr_return n1 = null;

        wcpsParser.numericScalarExpr_return n2 = null;


        Object e_tree=null;
        Object LPAREN162_tree=null;
        Object RPAREN163_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 57) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:289:2: ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT )
            int alt48=4;
            alt48 = dfa48.predict(input);
            switch (alt48) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:289:4: LPAREN e1= booleanScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN162=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_booleanScalarAtom2934); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN162_tree = (Object)adaptor.create(LPAREN162);
                    adaptor.addChild(root_0, LPAREN162_tree);
                    }
                    pushFollow(FOLLOW_booleanScalarExpr_in_booleanScalarAtom2938);
                    e1=booleanScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN163=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_booleanScalarAtom2940); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN163_tree = (Object)adaptor.create(RPAREN163);
                    adaptor.addChild(root_0, RPAREN163_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:290:4: s1= stringScalarExpr cop= compOp s2= stringScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_stringScalarExpr_in_booleanScalarAtom2949);
                    s1=stringScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, s1.getTree());
                    pushFollow(FOLLOW_compOp_in_booleanScalarAtom2953);
                    cop=compOp();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cop.getTree());
                    pushFollow(FOLLOW_stringScalarExpr_in_booleanScalarAtom2957);
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
                    // src/petascope/wcps/grammar/wcps.g:291:4: n1= numericScalarExpr cop= compOp n2= numericScalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_numericScalarExpr_in_booleanScalarAtom2967);
                    n1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, n1.getTree());
                    pushFollow(FOLLOW_compOp_in_booleanScalarAtom2971);
                    cop=compOp();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, cop.getTree());
                    pushFollow(FOLLOW_numericScalarExpr_in_booleanScalarAtom2975);
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
                    // src/petascope/wcps/grammar/wcps.g:292:4: e= BOOLEANCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,BOOLEANCONSTANT,FOLLOW_BOOLEANCONSTANT_in_booleanScalarAtom2985); if (state.failed) return retval;
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
    // src/petascope/wcps/grammar/wcps.g:294:1: numericScalarExpr returns [NumericScalarExpr value] : e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 58) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:295:2: (e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )* )
            // src/petascope/wcps/grammar/wcps.g:295:4: e1= numericScalarTerm (op= ( PLUS | MINUS ) e2= numericScalarTerm )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_numericScalarTerm_in_numericScalarExpr3002);
            e1=numericScalarTerm();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
              retval.value = (e1!=null?e1.value:null); 
            }
            // src/petascope/wcps/grammar/wcps.g:296:4: (op= ( PLUS | MINUS ) e2= numericScalarTerm )*
            loop49:
            do {
                int alt49=2;
                alt49 = dfa49.predict(input);
                switch (alt49) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:296:5: op= ( PLUS | MINUS ) e2= numericScalarTerm
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

            	    pushFollow(FOLLOW_numericScalarTerm_in_numericScalarExpr3020);
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
            	    break loop49;
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
    // src/petascope/wcps/grammar/wcps.g:298:1: numericScalarTerm returns [NumericScalarExpr value] : e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )* ;
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
            if ( state.backtracking>0 && alreadyParsedRule(input, 59) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:299:2: (e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )* )
            // src/petascope/wcps/grammar/wcps.g:299:4: e1= numericScalarFactor (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_numericScalarFactor_in_numericScalarTerm3039);
            e1=numericScalarFactor();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (e1!=null?e1.value:null); 
            }
            // src/petascope/wcps/grammar/wcps.g:300:3: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*
            loop50:
            do {
                int alt50=2;
                alt50 = dfa50.predict(input);
                switch (alt50) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:300:4: op= ( MULT | DIVIDE ) e2= numericScalarFactor
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

            	    pushFollow(FOLLOW_numericScalarFactor_in_numericScalarTerm3056);
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
            	    break loop50;
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
    // src/petascope/wcps/grammar/wcps.g:302:1: numericScalarFactor returns [NumericScalarExpr value] : ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ABS LPAREN e12= numericScalarExpr RPAREN | op= SQRT LPAREN e11= numericScalarExpr RPAREN | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr | e4= variableName );
    public final wcpsParser.numericScalarFactor_return numericScalarFactor() throws RecognitionException {
        wcpsParser.numericScalarFactor_return retval = new wcpsParser.numericScalarFactor_return();
        retval.start = input.LT(1);
        int numericScalarFactor_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;
        Token e=null;
        Token LPAREN164=null;
        Token RPAREN165=null;
        Token LPAREN166=null;
        Token RPAREN167=null;
        Token LPAREN168=null;
        Token RPAREN169=null;
        Token LPAREN170=null;
        Token RPAREN171=null;
        wcpsParser.numericScalarExpr_return e1 = null;

        wcpsParser.numericScalarFactor_return e10 = null;

        wcpsParser.numericScalarExpr_return e12 = null;

        wcpsParser.numericScalarExpr_return e11 = null;

        wcpsParser.complexConstant_return e2 = null;

        wcpsParser.condenseExpr_return e3 = null;

        wcpsParser.variableName_return e4 = null;


        Object op_tree=null;
        Object e_tree=null;
        Object LPAREN164_tree=null;
        Object RPAREN165_tree=null;
        Object LPAREN166_tree=null;
        Object RPAREN167_tree=null;
        Object LPAREN168_tree=null;
        Object RPAREN169_tree=null;
        Object LPAREN170_tree=null;
        Object RPAREN171_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 60) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:303:5: ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ABS LPAREN e12= numericScalarExpr RPAREN | op= SQRT LPAREN e11= numericScalarExpr RPAREN | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr | e4= variableName )
            int alt51=10;
            alt51 = dfa51.predict(input);
            switch (alt51) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:303:7: LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    LPAREN164=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3076); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN164_tree = (Object)adaptor.create(LPAREN164);
                    adaptor.addChild(root_0, LPAREN164_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3080);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN165=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3082); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN165_tree = (Object)adaptor.create(RPAREN165);
                    adaptor.addChild(root_0, RPAREN165_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = (e1!=null?e1.value:null); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:304:7: op= MINUS e10= numericScalarFactor
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,MINUS,FOLLOW_MINUS_in_numericScalarFactor3094); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    pushFollow(FOLLOW_numericScalarFactor_in_numericScalarFactor3098);
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
                    // src/petascope/wcps/grammar/wcps.g:305:7: op= ABS LPAREN e12= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ABS,FOLLOW_ABS_in_numericScalarFactor3110); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN166=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3112); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN166_tree = (Object)adaptor.create(LPAREN166);
                    adaptor.addChild(root_0, LPAREN166_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3116);
                    e12=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e12.getTree());
                    RPAREN167=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3118); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN167_tree = (Object)adaptor.create(RPAREN167);
                    adaptor.addChild(root_0, RPAREN167_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e12!=null?e12.value:null)); 
                    }

                    }
                    break;
                case 4 :
                    // src/petascope/wcps/grammar/wcps.g:306:7: op= SQRT LPAREN e11= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,SQRT,FOLLOW_SQRT_in_numericScalarFactor3130); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN168=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3132); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN168_tree = (Object)adaptor.create(LPAREN168);
                    adaptor.addChild(root_0, LPAREN168_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3136);
                    e11=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e11.getTree());
                    RPAREN169=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3138); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN169_tree = (Object)adaptor.create(RPAREN169);
                    adaptor.addChild(root_0, RPAREN169_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e11!=null?e11.value:null)); 
                    }

                    }
                    break;
                case 5 :
                    // src/petascope/wcps/grammar/wcps.g:307:7: op= ROUND LPAREN e1= numericScalarExpr RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    op=(Token)match(input,ROUND,FOLLOW_ROUND_in_numericScalarFactor3150); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    op_tree = (Object)adaptor.create(op);
                    adaptor.addChild(root_0, op_tree);
                    }
                    LPAREN170=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_numericScalarFactor3152); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN170_tree = (Object)adaptor.create(LPAREN170);
                    adaptor.addChild(root_0, LPAREN170_tree);
                    }
                    pushFollow(FOLLOW_numericScalarExpr_in_numericScalarFactor3156);
                    e1=numericScalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    RPAREN171=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_numericScalarFactor3158); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN171_tree = (Object)adaptor.create(RPAREN171);
                    adaptor.addChild(root_0, RPAREN171_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new NumericScalarExpr((op!=null?op.getText():null), (e1!=null?e1.value:null)); 
                    }

                    }
                    break;
                case 6 :
                    // src/petascope/wcps/grammar/wcps.g:308:7: e= INTEGERCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,INTEGERCONSTANT,FOLLOW_INTEGERCONSTANT_in_numericScalarFactor3170); if (state.failed) return retval;
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
                    // src/petascope/wcps/grammar/wcps.g:309:7: e= FLOATCONSTANT
                    {
                    root_0 = (Object)adaptor.nil();

                    e=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_numericScalarFactor3182); if (state.failed) return retval;
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
                    // src/petascope/wcps/grammar/wcps.g:310:7: e2= complexConstant
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_complexConstant_in_numericScalarFactor3194);
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
                    // src/petascope/wcps/grammar/wcps.g:311:7: e3= condenseExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_condenseExpr_in_numericScalarFactor3206);
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
                    // src/petascope/wcps/grammar/wcps.g:312:7: e4= variableName
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_variableName_in_numericScalarFactor3218);
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
    // src/petascope/wcps/grammar/wcps.g:314:1: compOp returns [String value] : ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE );
    public final wcpsParser.compOp_return compOp() throws RecognitionException {
        wcpsParser.compOp_return retval = new wcpsParser.compOp_return();
        retval.start = input.LT(1);
        int compOp_StartIndex = input.index();
        Object root_0 = null;

        Token EQUALS172=null;
        Token NOTEQUALS173=null;
        Token LT174=null;
        Token GT175=null;
        Token LTE176=null;
        Token GTE177=null;

        Object EQUALS172_tree=null;
        Object NOTEQUALS173_tree=null;
        Object LT174_tree=null;
        Object GT175_tree=null;
        Object LTE176_tree=null;
        Object GTE177_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 61) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:315:2: ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE )
            int alt52=6;
            switch ( input.LA(1) ) {
            case EQUALS:
                {
                alt52=1;
                }
                break;
            case NOTEQUALS:
                {
                alt52=2;
                }
                break;
            case LT:
                {
                alt52=3;
                }
                break;
            case GT:
                {
                alt52=4;
                }
                break;
            case LTE:
                {
                alt52=5;
                }
                break;
            case GTE:
                {
                alt52=6;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 52, 0, input);

                throw nvae;
            }

            switch (alt52) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:315:4: EQUALS
                    {
                    root_0 = (Object)adaptor.nil();

                    EQUALS172=(Token)match(input,EQUALS,FOLLOW_EQUALS_in_compOp3236); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    EQUALS172_tree = (Object)adaptor.create(EQUALS172);
                    adaptor.addChild(root_0, EQUALS172_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("equals"); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:316:4: NOTEQUALS
                    {
                    root_0 = (Object)adaptor.nil();

                    NOTEQUALS173=(Token)match(input,NOTEQUALS,FOLLOW_NOTEQUALS_in_compOp3243); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    NOTEQUALS173_tree = (Object)adaptor.create(NOTEQUALS173);
                    adaptor.addChild(root_0, NOTEQUALS173_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("notEqual"); 
                    }

                    }
                    break;
                case 3 :
                    // src/petascope/wcps/grammar/wcps.g:317:4: LT
                    {
                    root_0 = (Object)adaptor.nil();

                    LT174=(Token)match(input,LT,FOLLOW_LT_in_compOp3250); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LT174_tree = (Object)adaptor.create(LT174);
                    adaptor.addChild(root_0, LT174_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("lessThan"); 
                    }

                    }
                    break;
                case 4 :
                    // src/petascope/wcps/grammar/wcps.g:318:4: GT
                    {
                    root_0 = (Object)adaptor.nil();

                    GT175=(Token)match(input,GT,FOLLOW_GT_in_compOp3257); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    GT175_tree = (Object)adaptor.create(GT175);
                    adaptor.addChild(root_0, GT175_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("greaterThan"); 
                    }

                    }
                    break;
                case 5 :
                    // src/petascope/wcps/grammar/wcps.g:319:4: LTE
                    {
                    root_0 = (Object)adaptor.nil();

                    LTE176=(Token)match(input,LTE,FOLLOW_LTE_in_compOp3264); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LTE176_tree = (Object)adaptor.create(LTE176);
                    adaptor.addChild(root_0, LTE176_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new String("lessOrEqual"); 
                    }

                    }
                    break;
                case 6 :
                    // src/petascope/wcps/grammar/wcps.g:320:4: GTE
                    {
                    root_0 = (Object)adaptor.nil();

                    GTE177=(Token)match(input,GTE,FOLLOW_GTE_in_compOp3271); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    GTE177_tree = (Object)adaptor.create(GTE177);
                    adaptor.addChild(root_0, GTE177_tree);
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
    // src/petascope/wcps/grammar/wcps.g:322:1: dimensionIntervalList returns [DimensionIntervalList value] : elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )* ;
    public final wcpsParser.dimensionIntervalList_return dimensionIntervalList() throws RecognitionException {
        wcpsParser.dimensionIntervalList_return retval = new wcpsParser.dimensionIntervalList_return();
        retval.start = input.LT(1);
        int dimensionIntervalList_StartIndex = input.index();
        Object root_0 = null;

        Token COMMA178=null;
        wcpsParser.dimensionIntervalElement_return elem = null;


        Object COMMA178_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 62) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:323:5: (elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )* )
            // src/petascope/wcps/grammar/wcps.g:323:7: elem= dimensionIntervalElement ( COMMA elem= dimensionIntervalElement )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3291);
            elem=dimensionIntervalElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionIntervalList((elem!=null?elem.value:null)); 
            }
            // src/petascope/wcps/grammar/wcps.g:324:9: ( COMMA elem= dimensionIntervalElement )*
            loop53:
            do {
                int alt53=2;
                int LA53_0 = input.LA(1);

                if ( (LA53_0==COMMA) ) {
                    int LA53_1 = input.LA(2);

                    if ( (LA53_1==INTEGERCONSTANT||LA53_1==STRING||LA53_1==NAME) ) {
                        alt53=1;
                    }


                }


                switch (alt53) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:324:10: COMMA elem= dimensionIntervalElement
            	    {
            	    COMMA178=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionIntervalList3304); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA178_tree = (Object)adaptor.create(COMMA178);
            	    adaptor.addChild(root_0, COMMA178_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3308);
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
            	    break loop53;
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
    // src/petascope/wcps/grammar/wcps.g:326:1: dimensionIntervalElement returns [DimensionIntervalElement value] : aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN ;
    public final wcpsParser.dimensionIntervalElement_return dimensionIntervalElement() throws RecognitionException {
        wcpsParser.dimensionIntervalElement_return retval = new wcpsParser.dimensionIntervalElement_return();
        retval.start = input.LT(1);
        int dimensionIntervalElement_StartIndex = input.index();
        Object root_0 = null;

        Token COLON179=null;
        Token LPAREN180=null;
        Token RPAREN181=null;
        wcpsParser.axisName_return aname = null;

        wcpsParser.crsName_return crs = null;

        wcpsParser.dimensionIntervalExpr_return die = null;


        Object COLON179_tree=null;
        Object LPAREN180_tree=null;
        Object RPAREN181_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 63) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:327:5: (aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN )
            // src/petascope/wcps/grammar/wcps.g:327:7: aname= axisName ( COLON crs= crsName )? LPAREN die= dimensionIntervalExpr RPAREN
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_axisName_in_dimensionIntervalElement3333);
            aname=axisName();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionIntervalElement((aname!=null?aname.value:null)); 
            }
            // src/petascope/wcps/grammar/wcps.g:327:79: ( COLON crs= crsName )?
            int alt54=2;
            int LA54_0 = input.LA(1);

            if ( (LA54_0==COLON) ) {
                alt54=1;
            }
            switch (alt54) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:327:80: COLON crs= crsName
                    {
                    COLON179=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalElement3338); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON179_tree = (Object)adaptor.create(COLON179);
                    adaptor.addChild(root_0, COLON179_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionIntervalElement3342);
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

            LPAREN180=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionIntervalElement3353); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN180_tree = (Object)adaptor.create(LPAREN180);
            adaptor.addChild(root_0, LPAREN180_tree);
            }
            pushFollow(FOLLOW_dimensionIntervalExpr_in_dimensionIntervalElement3357);
            die=dimensionIntervalExpr();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, die.getTree());
            RPAREN181=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionIntervalElement3359); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN181_tree = (Object)adaptor.create(RPAREN181);
            adaptor.addChild(root_0, RPAREN181_tree);
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
    // src/petascope/wcps/grammar/wcps.g:330:1: dimensionIntervalExpr returns [DimensionIntervalExpr value] : (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN );
    public final wcpsParser.dimensionIntervalExpr_return dimensionIntervalExpr() throws RecognitionException {
        wcpsParser.dimensionIntervalExpr_return retval = new wcpsParser.dimensionIntervalExpr_return();
        retval.start = input.LT(1);
        int dimensionIntervalExpr_StartIndex = input.index();
        Object root_0 = null;

        Token COLON182=null;
        Token DOMAIN183=null;
        Token LPAREN184=null;
        Token COLON185=null;
        Token COLON186=null;
        Token RPAREN187=null;
        wcpsParser.scalarExpr_return e1 = null;

        wcpsParser.scalarExpr_return e2 = null;

        wcpsParser.coverageName_return e3 = null;

        wcpsParser.axisName_return e4 = null;

        wcpsParser.crsName_return e5 = null;


        Object COLON182_tree=null;
        Object DOMAIN183_tree=null;
        Object LPAREN184_tree=null;
        Object COLON185_tree=null;
        Object COLON186_tree=null;
        Object RPAREN187_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 64) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:331:5: (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN )
            int alt55=2;
            alt55 = dfa55.predict(input);
            switch (alt55) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:331:7: e1= scalarExpr COLON e2= scalarExpr
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_scalarExpr_in_dimensionIntervalExpr3382);
                    e1=scalarExpr();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e1.getTree());
                    COLON182=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3384); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON182_tree = (Object)adaptor.create(COLON182);
                    adaptor.addChild(root_0, COLON182_tree);
                    }
                    pushFollow(FOLLOW_scalarExpr_in_dimensionIntervalExpr3388);
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
                    // src/petascope/wcps/grammar/wcps.g:332:7: DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    DOMAIN183=(Token)match(input,DOMAIN,FOLLOW_DOMAIN_in_dimensionIntervalExpr3398); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    DOMAIN183_tree = (Object)adaptor.create(DOMAIN183);
                    adaptor.addChild(root_0, DOMAIN183_tree);
                    }
                    LPAREN184=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionIntervalExpr3400); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN184_tree = (Object)adaptor.create(LPAREN184);
                    adaptor.addChild(root_0, LPAREN184_tree);
                    }
                    pushFollow(FOLLOW_coverageName_in_dimensionIntervalExpr3404);
                    e3=coverageName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e3.getTree());
                    COLON185=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3406); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON185_tree = (Object)adaptor.create(COLON185);
                    adaptor.addChild(root_0, COLON185_tree);
                    }
                    pushFollow(FOLLOW_axisName_in_dimensionIntervalExpr3410);
                    e4=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e4.getTree());
                    COLON186=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionIntervalExpr3412); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON186_tree = (Object)adaptor.create(COLON186);
                    adaptor.addChild(root_0, COLON186_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionIntervalExpr3416);
                    e5=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, e5.getTree());
                    RPAREN187=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionIntervalExpr3418); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN187_tree = (Object)adaptor.create(RPAREN187);
                    adaptor.addChild(root_0, RPAREN187_tree);
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
    // src/petascope/wcps/grammar/wcps.g:334:1: dimensionPointList returns [DimensionPointList value] : elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )* ;
    public final wcpsParser.dimensionPointList_return dimensionPointList() throws RecognitionException {
        wcpsParser.dimensionPointList_return retval = new wcpsParser.dimensionPointList_return();
        retval.start = input.LT(1);
        int dimensionPointList_StartIndex = input.index();
        Object root_0 = null;

        Token COMMA188=null;
        wcpsParser.dimensionPointElement_return elem1 = null;

        wcpsParser.dimensionPointElement_return elem2 = null;


        Object COMMA188_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 65) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:335:5: (elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )* )
            // src/petascope/wcps/grammar/wcps.g:335:7: elem1= dimensionPointElement ( COMMA elem2= dimensionPointElement )*
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_dimensionPointElement_in_dimensionPointList3441);
            elem1=dimensionPointElement();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, elem1.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new DimensionPointList((elem1!=null?elem1.value:null)); 
            }
            // src/petascope/wcps/grammar/wcps.g:336:6: ( COMMA elem2= dimensionPointElement )*
            loop56:
            do {
                int alt56=2;
                int LA56_0 = input.LA(1);

                if ( (LA56_0==COMMA) ) {
                    alt56=1;
                }


                switch (alt56) {
            	case 1 :
            	    // src/petascope/wcps/grammar/wcps.g:336:7: COMMA elem2= dimensionPointElement
            	    {
            	    COMMA188=(Token)match(input,COMMA,FOLLOW_COMMA_in_dimensionPointList3451); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	    COMMA188_tree = (Object)adaptor.create(COMMA188);
            	    adaptor.addChild(root_0, COMMA188_tree);
            	    }
            	    pushFollow(FOLLOW_dimensionPointElement_in_dimensionPointList3455);
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
            	    break loop56;
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
    // src/petascope/wcps/grammar/wcps.g:338:1: dimensionPointElement returns [DimensionPointElement value] : (aname= axisName LPAREN dpe= dimensionPoint RPAREN | aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN );
    public final wcpsParser.dimensionPointElement_return dimensionPointElement() throws RecognitionException {
        wcpsParser.dimensionPointElement_return retval = new wcpsParser.dimensionPointElement_return();
        retval.start = input.LT(1);
        int dimensionPointElement_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN189=null;
        Token RPAREN190=null;
        Token COLON191=null;
        Token LPAREN192=null;
        Token RPAREN193=null;
        wcpsParser.axisName_return aname = null;

        wcpsParser.dimensionPoint_return dpe = null;

        wcpsParser.crsName_return crs = null;


        Object LPAREN189_tree=null;
        Object RPAREN190_tree=null;
        Object COLON191_tree=null;
        Object LPAREN192_tree=null;
        Object RPAREN193_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 66) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:339:5: (aname= axisName LPAREN dpe= dimensionPoint RPAREN | aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN )
            int alt57=2;
            int LA57_0 = input.LA(1);

            if ( (LA57_0==INTEGERCONSTANT||LA57_0==STRING||LA57_0==NAME) ) {
                int LA57_1 = input.LA(2);

                if ( (LA57_1==COLON) ) {
                    alt57=2;
                }
                else if ( (LA57_1==LPAREN) ) {
                    alt57=1;
                }
                else {
                    if (state.backtracking>0) {state.failed=true; return retval;}
                    NoViableAltException nvae =
                        new NoViableAltException("", 57, 1, input);

                    throw nvae;
                }
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 57, 0, input);

                throw nvae;
            }
            switch (alt57) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:339:7: aname= axisName LPAREN dpe= dimensionPoint RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_axisName_in_dimensionPointElement3480);
                    aname=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
                    LPAREN189=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionPointElement3482); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN189_tree = (Object)adaptor.create(LPAREN189);
                    adaptor.addChild(root_0, LPAREN189_tree);
                    }
                    pushFollow(FOLLOW_dimensionPoint_in_dimensionPointElement3486);
                    dpe=dimensionPoint();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpe.getTree());
                    RPAREN190=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionPointElement3488); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN190_tree = (Object)adaptor.create(RPAREN190);
                    adaptor.addChild(root_0, RPAREN190_tree);
                    }
                    if ( state.backtracking==0 ) {
                       retval.value = new DimensionPointElement((aname!=null?aname.value:null), (dpe!=null?dpe.value:null)); 
                    }

                    }
                    break;
                case 2 :
                    // src/petascope/wcps/grammar/wcps.g:340:7: aname= axisName COLON crs= crsName LPAREN dpe= dimensionPoint RPAREN
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_axisName_in_dimensionPointElement3500);
                    aname=axisName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, aname.getTree());
                    COLON191=(Token)match(input,COLON,FOLLOW_COLON_in_dimensionPointElement3502); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    COLON191_tree = (Object)adaptor.create(COLON191);
                    adaptor.addChild(root_0, COLON191_tree);
                    }
                    pushFollow(FOLLOW_crsName_in_dimensionPointElement3506);
                    crs=crsName();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, crs.getTree());
                    LPAREN192=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_dimensionPointElement3508); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    LPAREN192_tree = (Object)adaptor.create(LPAREN192);
                    adaptor.addChild(root_0, LPAREN192_tree);
                    }
                    pushFollow(FOLLOW_dimensionPoint_in_dimensionPointElement3512);
                    dpe=dimensionPoint();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) adaptor.addChild(root_0, dpe.getTree());
                    RPAREN193=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_dimensionPointElement3514); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                    RPAREN193_tree = (Object)adaptor.create(RPAREN193);
                    adaptor.addChild(root_0, RPAREN193_tree);
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
    // src/petascope/wcps/grammar/wcps.g:342:1: dimensionPoint returns [ScalarExpr value] : e1= scalarExpr ;
    public final wcpsParser.dimensionPoint_return dimensionPoint() throws RecognitionException {
        wcpsParser.dimensionPoint_return retval = new wcpsParser.dimensionPoint_return();
        retval.start = input.LT(1);
        int dimensionPoint_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.scalarExpr_return e1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 67) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:343:5: (e1= scalarExpr )
            // src/petascope/wcps/grammar/wcps.g:343:7: e1= scalarExpr
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_scalarExpr_in_dimensionPoint3537);
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
    // src/petascope/wcps/grammar/wcps.g:345:1: interpolationMethod returns [InterpolationMethod value] : LPAREN type= interpolationType COLON res= nullResistence RPAREN ;
    public final wcpsParser.interpolationMethod_return interpolationMethod() throws RecognitionException {
        wcpsParser.interpolationMethod_return retval = new wcpsParser.interpolationMethod_return();
        retval.start = input.LT(1);
        int interpolationMethod_StartIndex = input.index();
        Object root_0 = null;

        Token LPAREN194=null;
        Token COLON195=null;
        Token RPAREN196=null;
        wcpsParser.interpolationType_return type = null;

        wcpsParser.nullResistence_return res = null;


        Object LPAREN194_tree=null;
        Object COLON195_tree=null;
        Object RPAREN196_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 68) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:346:2: ( LPAREN type= interpolationType COLON res= nullResistence RPAREN )
            // src/petascope/wcps/grammar/wcps.g:346:4: LPAREN type= interpolationType COLON res= nullResistence RPAREN
            {
            root_0 = (Object)adaptor.nil();

            LPAREN194=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_interpolationMethod3555); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN194_tree = (Object)adaptor.create(LPAREN194);
            adaptor.addChild(root_0, LPAREN194_tree);
            }
            pushFollow(FOLLOW_interpolationType_in_interpolationMethod3559);
            type=interpolationType();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, type.getTree());
            COLON195=(Token)match(input,COLON,FOLLOW_COLON_in_interpolationMethod3561); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COLON195_tree = (Object)adaptor.create(COLON195);
            adaptor.addChild(root_0, COLON195_tree);
            }
            pushFollow(FOLLOW_nullResistence_in_interpolationMethod3565);
            res=nullResistence();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, res.getTree());
            RPAREN196=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_interpolationMethod3567); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN196_tree = (Object)adaptor.create(RPAREN196);
            adaptor.addChild(root_0, RPAREN196_tree);
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
    // src/petascope/wcps/grammar/wcps.g:348:1: interpolationType returns [String value] : type= ( NEAREST | LINEAR | QUADRATIC | CUBIC ) ;
    public final wcpsParser.interpolationType_return interpolationType() throws RecognitionException {
        wcpsParser.interpolationType_return retval = new wcpsParser.interpolationType_return();
        retval.start = input.LT(1);
        int interpolationType_StartIndex = input.index();
        Object root_0 = null;

        Token type=null;

        Object type_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 69) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:349:2: (type= ( NEAREST | LINEAR | QUADRATIC | CUBIC ) )
            // src/petascope/wcps/grammar/wcps.g:349:4: type= ( NEAREST | LINEAR | QUADRATIC | CUBIC )
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
    // src/petascope/wcps/grammar/wcps.g:351:1: nullResistence returns [String value] : resistance= ( FULL | NONE | HALF | OTHER ) ;
    public final wcpsParser.nullResistence_return nullResistence() throws RecognitionException {
        wcpsParser.nullResistence_return retval = new wcpsParser.nullResistence_return();
        retval.start = input.LT(1);
        int nullResistence_StartIndex = input.index();
        Object root_0 = null;

        Token resistance=null;

        Object resistance_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 70) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:352:2: (resistance= ( FULL | NONE | HALF | OTHER ) )
            // src/petascope/wcps/grammar/wcps.g:352:4: resistance= ( FULL | NONE | HALF | OTHER )
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
        public CondenseOperation value;
        Object tree;
        public Object getTree() { return tree; }
    };

    // $ANTLR start "condenseOpType"
    // src/petascope/wcps/grammar/wcps.g:354:1: condenseOpType returns [CondenseOperation value] : op= ( PLUS | MULT | MAX | MIN | AND | OR ) ;
    public final wcpsParser.condenseOpType_return condenseOpType() throws RecognitionException {
        wcpsParser.condenseOpType_return retval = new wcpsParser.condenseOpType_return();
        retval.start = input.LT(1);
        int condenseOpType_StartIndex = input.index();
        Object root_0 = null;

        Token op=null;

        Object op_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 71) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:355:2: (op= ( PLUS | MULT | MAX | MIN | AND | OR ) )
            // src/petascope/wcps/grammar/wcps.g:355:4: op= ( PLUS | MULT | MAX | MIN | AND | OR )
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
    // src/petascope/wcps/grammar/wcps.g:357:1: fieldName returns [String value] : name ;
    public final wcpsParser.fieldName_return fieldName() throws RecognitionException {
        wcpsParser.fieldName_return retval = new wcpsParser.fieldName_return();
        retval.start = input.LT(1);
        int fieldName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return name197 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 72) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:358:2: ( name )
            // src/petascope/wcps/grammar/wcps.g:358:4: name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_fieldName3661);
            name197=name();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, name197.getTree());
            if ( state.backtracking==0 ) {
               retval.value = new String((name197!=null?name197.value:null));
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
    // src/petascope/wcps/grammar/wcps.g:360:1: constant returns [String value] : (e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT ) | e1= complexConstant );
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
            // src/petascope/wcps/grammar/wcps.g:361:2: (e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT ) | e1= complexConstant )
            int alt58=2;
            int LA58_0 = input.LA(1);

            if ( (LA58_0==INTEGERCONSTANT||LA58_0==STRING||(LA58_0>=BOOLEANCONSTANT && LA58_0<=FLOATCONSTANT)) ) {
                alt58=1;
            }
            else if ( (LA58_0==LPAREN) ) {
                alt58=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 58, 0, input);

                throw nvae;
            }
            switch (alt58) {
                case 1 :
                    // src/petascope/wcps/grammar/wcps.g:361:4: e= ( STRING | BOOLEANCONSTANT | INTEGERCONSTANT | FLOATCONSTANT )
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
                    // src/petascope/wcps/grammar/wcps.g:362:4: e1= complexConstant
                    {
                    root_0 = (Object)adaptor.nil();

                    pushFollow(FOLLOW_complexConstant_in_constant3695);
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
    // src/petascope/wcps/grammar/wcps.g:364:1: complexConstant returns [String value] : LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN ;
    public final wcpsParser.complexConstant_return complexConstant() throws RecognitionException {
        wcpsParser.complexConstant_return retval = new wcpsParser.complexConstant_return();
        retval.start = input.LT(1);
        int complexConstant_StartIndex = input.index();
        Object root_0 = null;

        Token re1=null;
        Token im1=null;
        Token LPAREN198=null;
        Token COMMA199=null;
        Token RPAREN200=null;

        Object re1_tree=null;
        Object im1_tree=null;
        Object LPAREN198_tree=null;
        Object COMMA199_tree=null;
        Object RPAREN200_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 74) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:365:2: ( LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN )
            // src/petascope/wcps/grammar/wcps.g:365:4: LPAREN re1= FLOATCONSTANT COMMA im1= FLOATCONSTANT RPAREN
            {
            root_0 = (Object)adaptor.nil();

            LPAREN198=(Token)match(input,LPAREN,FOLLOW_LPAREN_in_complexConstant3710); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            LPAREN198_tree = (Object)adaptor.create(LPAREN198);
            adaptor.addChild(root_0, LPAREN198_tree);
            }
            re1=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_complexConstant3714); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            re1_tree = (Object)adaptor.create(re1);
            adaptor.addChild(root_0, re1_tree);
            }
            COMMA199=(Token)match(input,COMMA,FOLLOW_COMMA_in_complexConstant3716); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            COMMA199_tree = (Object)adaptor.create(COMMA199);
            adaptor.addChild(root_0, COMMA199_tree);
            }
            im1=(Token)match(input,FLOATCONSTANT,FOLLOW_FLOATCONSTANT_in_complexConstant3720); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            im1_tree = (Object)adaptor.create(im1);
            adaptor.addChild(root_0, im1_tree);
            }
            RPAREN200=(Token)match(input,RPAREN,FOLLOW_RPAREN_in_complexConstant3722); if (state.failed) return retval;
            if ( state.backtracking==0 ) {
            RPAREN200_tree = (Object)adaptor.create(RPAREN200);
            adaptor.addChild(root_0, RPAREN200_tree);
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
    // src/petascope/wcps/grammar/wcps.g:367:1: stringConstant returns [String value] : s= STRING ;
    public final wcpsParser.stringConstant_return stringConstant() throws RecognitionException {
        wcpsParser.stringConstant_return retval = new wcpsParser.stringConstant_return();
        retval.start = input.LT(1);
        int stringConstant_StartIndex = input.index();
        Object root_0 = null;

        Token s=null;

        Object s_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 75) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:368:2: (s= STRING )
            // src/petascope/wcps/grammar/wcps.g:368:4: s= STRING
            {
            root_0 = (Object)adaptor.nil();

            s=(Token)match(input,STRING,FOLLOW_STRING_in_stringConstant3739); if (state.failed) return retval;
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
    // src/petascope/wcps/grammar/wcps.g:370:1: name returns [String value] : var= ( NAME | STRING | INTEGERCONSTANT ) ;
    public final wcpsParser.name_return name() throws RecognitionException {
        wcpsParser.name_return retval = new wcpsParser.name_return();
        retval.start = input.LT(1);
        int name_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 76) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:371:2: (var= ( NAME | STRING | INTEGERCONSTANT ) )
            // src/petascope/wcps/grammar/wcps.g:371:4: var= ( NAME | STRING | INTEGERCONSTANT )
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
    // src/petascope/wcps/grammar/wcps.g:373:1: crsName returns [String value] : s= stringConstant ;
    public final wcpsParser.crsName_return crsName() throws RecognitionException {
        wcpsParser.crsName_return retval = new wcpsParser.crsName_return();
        retval.start = input.LT(1);
        int crsName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.stringConstant_return s = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 77) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:374:2: (s= stringConstant )
            // src/petascope/wcps/grammar/wcps.g:374:4: s= stringConstant
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_stringConstant_in_crsName3783);
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
    // src/petascope/wcps/grammar/wcps.g:376:1: axisName returns [String value] : type1= name ;
    public final wcpsParser.axisName_return axisName() throws RecognitionException {
        wcpsParser.axisName_return retval = new wcpsParser.axisName_return();
        retval.start = input.LT(1);
        int axisName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return type1 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 78) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:377:2: (type1= name )
            // src/petascope/wcps/grammar/wcps.g:377:4: type1= name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_axisName3800);
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
    // src/petascope/wcps/grammar/wcps.g:379:1: variableName returns [String value] : var= VARIABLE_DOLLAR ;
    public final wcpsParser.variableName_return variableName() throws RecognitionException {
        wcpsParser.variableName_return retval = new wcpsParser.variableName_return();
        retval.start = input.LT(1);
        int variableName_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 79) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:380:2: (var= VARIABLE_DOLLAR )
            // src/petascope/wcps/grammar/wcps.g:380:4: var= VARIABLE_DOLLAR
            {
            root_0 = (Object)adaptor.nil();

            var=(Token)match(input,VARIABLE_DOLLAR,FOLLOW_VARIABLE_DOLLAR_in_variableName3817); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 79, variableName_StartIndex); }
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
    // src/petascope/wcps/grammar/wcps.g:382:1: coverageVariable returns [String value] : var= NAME ;
    public final wcpsParser.coverageVariable_return coverageVariable() throws RecognitionException {
        wcpsParser.coverageVariable_return retval = new wcpsParser.coverageVariable_return();
        retval.start = input.LT(1);
        int coverageVariable_StartIndex = input.index();
        Object root_0 = null;

        Token var=null;

        Object var_tree=null;

        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 80) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:383:2: (var= NAME )
            // src/petascope/wcps/grammar/wcps.g:383:4: var= NAME
            {
            root_0 = (Object)adaptor.nil();

            var=(Token)match(input,NAME,FOLLOW_NAME_in_coverageVariable3834); if (state.failed) return retval;
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
            if ( state.backtracking>0 ) { memoize(input, 80, coverageVariable_StartIndex); }
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
    // src/petascope/wcps/grammar/wcps.g:385:1: coverageName returns [String value] : name ;
    public final wcpsParser.coverageName_return coverageName() throws RecognitionException {
        wcpsParser.coverageName_return retval = new wcpsParser.coverageName_return();
        retval.start = input.LT(1);
        int coverageName_StartIndex = input.index();
        Object root_0 = null;

        wcpsParser.name_return name201 = null;



        try {
            if ( state.backtracking>0 && alreadyParsedRule(input, 81) ) { return retval; }
            // src/petascope/wcps/grammar/wcps.g:386:2: ( name )
            // src/petascope/wcps/grammar/wcps.g:386:4: name
            {
            root_0 = (Object)adaptor.nil();

            pushFollow(FOLLOW_name_in_coverageName3849);
            name201=name();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) adaptor.addChild(root_0, name201.getTree());
            if ( state.backtracking==0 ) {
               retval.value = (name201!=null?name201.value:null); 
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
            if ( state.backtracking>0 ) { memoize(input, 81, coverageName_StartIndex); }
        }
        return retval;
    }
    // $ANTLR end "coverageName"

    // $ANTLR start synpred8_wcps
    public final void synpred8_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.coverageLogicTerm_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:67:10: (op= ( OR | XOR ) e2= coverageLogicTerm )
        // src/petascope/wcps/grammar/wcps.g:67:10: op= ( OR | XOR ) e2= coverageLogicTerm
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


        // src/petascope/wcps/grammar/wcps.g:71:10: (op= AND e2= coverageLogicFactor )
        // src/petascope/wcps/grammar/wcps.g:71:10: op= AND e2= coverageLogicFactor
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


        // src/petascope/wcps/grammar/wcps.g:75:10: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )
        // src/petascope/wcps/grammar/wcps.g:75:10: op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr
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


        // src/petascope/wcps/grammar/wcps.g:79:10: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )
        // src/petascope/wcps/grammar/wcps.g:79:10: op= ( PLUS | MINUS ) e2= coverageArithmeticTerm
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


        // src/petascope/wcps/grammar/wcps.g:83:10: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )
        // src/petascope/wcps/grammar/wcps.g:83:10: op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor
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


        // src/petascope/wcps/grammar/wcps.g:87:10: (op= OVERLAY e2= coverageValue )
        // src/petascope/wcps/grammar/wcps.g:87:10: op= OVERLAY e2= coverageValue
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


        // src/petascope/wcps/grammar/wcps.g:90:7: (e5= subsetExpr )
        // src/petascope/wcps/grammar/wcps.g:90:7: e5= subsetExpr
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


        // src/petascope/wcps/grammar/wcps.g:91:7: (e2= unaryInducedExpr )
        // src/petascope/wcps/grammar/wcps.g:91:7: e2= unaryInducedExpr
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


        // src/petascope/wcps/grammar/wcps.g:97:7: (e2= scalarExpr )
        // src/petascope/wcps/grammar/wcps.g:97:7: e2= scalarExpr
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


        // src/petascope/wcps/grammar/wcps.g:99:7: ( LPAREN e7= coverageExpr RPAREN )
        // src/petascope/wcps/grammar/wcps.g:99:7: LPAREN e7= coverageExpr RPAREN
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


        // src/petascope/wcps/grammar/wcps.g:100:7: (e3= coverageConstantExpr )
        // src/petascope/wcps/grammar/wcps.g:100:7: e3= coverageConstantExpr
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


        // src/petascope/wcps/grammar/wcps.g:101:7: (e4= coverageConstructorExpr )
        // src/petascope/wcps/grammar/wcps.g:101:7: e4= coverageConstructorExpr
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


        // src/petascope/wcps/grammar/wcps.g:106:7: (e1= metaDataExpr )
        // src/petascope/wcps/grammar/wcps.g:106:7: e1= metaDataExpr
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


        // src/petascope/wcps/grammar/wcps.g:107:7: (e2= condenseExpr )
        // src/petascope/wcps/grammar/wcps.g:107:7: e2= condenseExpr
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


        // src/petascope/wcps/grammar/wcps.g:108:7: (e3= booleanScalarExpr )
        // src/petascope/wcps/grammar/wcps.g:108:7: e3= booleanScalarExpr
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


        // src/petascope/wcps/grammar/wcps.g:109:7: (e4= numericScalarExpr )
        // src/petascope/wcps/grammar/wcps.g:109:7: e4= numericScalarExpr
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


        // src/petascope/wcps/grammar/wcps.g:110:7: (e5= stringScalarExpr )
        // src/petascope/wcps/grammar/wcps.g:110:7: e5= stringScalarExpr
        {
        pushFollow(FOLLOW_stringScalarExpr_in_synpred35_wcps883);
        e5=stringScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred35_wcps

    // $ANTLR start synpred70_wcps
    public final void synpred70_wcps_fragment() throws RecognitionException {   
        wcpsParser.fieldExpr_return e6 = null;


        // src/petascope/wcps/grammar/wcps.g:204:7: (e6= fieldExpr )
        // src/petascope/wcps/grammar/wcps.g:204:7: e6= fieldExpr
        {
        pushFollow(FOLLOW_fieldExpr_in_synpred70_wcps2013);
        e6=fieldExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred70_wcps

    // $ANTLR start synpred71_wcps
    public final void synpred71_wcps_fragment() throws RecognitionException {   
        wcpsParser.unaryArithmeticExpr_return e1 = null;


        // src/petascope/wcps/grammar/wcps.g:205:4: (e1= unaryArithmeticExpr )
        // src/petascope/wcps/grammar/wcps.g:205:4: e1= unaryArithmeticExpr
        {
        pushFollow(FOLLOW_unaryArithmeticExpr_in_synpred71_wcps2022);
        e1=unaryArithmeticExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred71_wcps

    // $ANTLR start synpred74_wcps
    public final void synpred74_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanExpr_return e4 = null;


        // src/petascope/wcps/grammar/wcps.g:208:7: (e4= booleanExpr )
        // src/petascope/wcps/grammar/wcps.g:208:7: e4= booleanExpr
        {
        pushFollow(FOLLOW_booleanExpr_in_synpred74_wcps2058);
        e4=booleanExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred74_wcps

    // $ANTLR start synpred99_wcps
    public final void synpred99_wcps_fragment() throws RecognitionException {   
        wcpsParser.trimExpr_return e1 = null;


        // src/petascope/wcps/grammar/wcps.g:249:4: (e1= trimExpr )
        // src/petascope/wcps/grammar/wcps.g:249:4: e1= trimExpr
        {
        pushFollow(FOLLOW_trimExpr_in_synpred99_wcps2538);
        e1=trimExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred99_wcps

    // $ANTLR start synpred100_wcps
    public final void synpred100_wcps_fragment() throws RecognitionException {   
        wcpsParser.sliceExpr_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:250:4: (e2= sliceExpr )
        // src/petascope/wcps/grammar/wcps.g:250:4: e2= sliceExpr
        {
        pushFollow(FOLLOW_sliceExpr_in_synpred100_wcps2547);
        e2=sliceExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred100_wcps

    // $ANTLR start synpred114_wcps
    public final void synpred114_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.booleanScalarTerm_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:278:8: (op= ( OR | XOR ) e2= booleanScalarTerm )
        // src/petascope/wcps/grammar/wcps.g:278:8: op= ( OR | XOR ) e2= booleanScalarTerm
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

        pushFollow(FOLLOW_booleanScalarTerm_in_synpred114_wcps2849);
        e2=booleanScalarTerm();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred114_wcps

    // $ANTLR start synpred115_wcps
    public final void synpred115_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.booleanScalarNegation_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:282:5: (op= AND e2= booleanScalarNegation )
        // src/petascope/wcps/grammar/wcps.g:282:5: op= AND e2= booleanScalarNegation
        {
        op=(Token)match(input,AND,FOLLOW_AND_in_synpred115_wcps2881); if (state.failed) return ;
        pushFollow(FOLLOW_booleanScalarNegation_in_synpred115_wcps2885);
        e2=booleanScalarNegation();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred115_wcps

    // $ANTLR start synpred117_wcps
    public final void synpred117_wcps_fragment() throws RecognitionException {   
        wcpsParser.booleanScalarExpr_return e1 = null;


        // src/petascope/wcps/grammar/wcps.g:289:4: ( LPAREN e1= booleanScalarExpr RPAREN )
        // src/petascope/wcps/grammar/wcps.g:289:4: LPAREN e1= booleanScalarExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred117_wcps2934); if (state.failed) return ;
        pushFollow(FOLLOW_booleanScalarExpr_in_synpred117_wcps2938);
        e1=booleanScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred117_wcps2940); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred117_wcps

    // $ANTLR start synpred119_wcps
    public final void synpred119_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return n1 = null;

        wcpsParser.compOp_return cop = null;

        wcpsParser.numericScalarExpr_return n2 = null;


        // src/petascope/wcps/grammar/wcps.g:291:4: (n1= numericScalarExpr cop= compOp n2= numericScalarExpr )
        // src/petascope/wcps/grammar/wcps.g:291:4: n1= numericScalarExpr cop= compOp n2= numericScalarExpr
        {
        pushFollow(FOLLOW_numericScalarExpr_in_synpred119_wcps2967);
        n1=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        pushFollow(FOLLOW_compOp_in_synpred119_wcps2971);
        cop=compOp();

        state._fsp--;
        if (state.failed) return ;
        pushFollow(FOLLOW_numericScalarExpr_in_synpred119_wcps2975);
        n2=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred119_wcps

    // $ANTLR start synpred121_wcps
    public final void synpred121_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.numericScalarTerm_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:296:5: (op= ( PLUS | MINUS ) e2= numericScalarTerm )
        // src/petascope/wcps/grammar/wcps.g:296:5: op= ( PLUS | MINUS ) e2= numericScalarTerm
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

        pushFollow(FOLLOW_numericScalarTerm_in_synpred121_wcps3020);
        e2=numericScalarTerm();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred121_wcps

    // $ANTLR start synpred123_wcps
    public final void synpred123_wcps_fragment() throws RecognitionException {   
        Token op=null;
        wcpsParser.numericScalarFactor_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:300:4: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )
        // src/petascope/wcps/grammar/wcps.g:300:4: op= ( MULT | DIVIDE ) e2= numericScalarFactor
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

        pushFollow(FOLLOW_numericScalarFactor_in_synpred123_wcps3056);
        e2=numericScalarFactor();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred123_wcps

    // $ANTLR start synpred124_wcps
    public final void synpred124_wcps_fragment() throws RecognitionException {   
        wcpsParser.numericScalarExpr_return e1 = null;


        // src/petascope/wcps/grammar/wcps.g:303:7: ( LPAREN e1= numericScalarExpr RPAREN )
        // src/petascope/wcps/grammar/wcps.g:303:7: LPAREN e1= numericScalarExpr RPAREN
        {
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred124_wcps3076); if (state.failed) return ;
        pushFollow(FOLLOW_numericScalarExpr_in_synpred124_wcps3080);
        e1=numericScalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred124_wcps3082); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred124_wcps

    // $ANTLR start synpred131_wcps
    public final void synpred131_wcps_fragment() throws RecognitionException {   
        wcpsParser.complexConstant_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:310:7: (e2= complexConstant )
        // src/petascope/wcps/grammar/wcps.g:310:7: e2= complexConstant
        {
        pushFollow(FOLLOW_complexConstant_in_synpred131_wcps3194);
        e2=complexConstant();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred131_wcps

    // $ANTLR start synpred140_wcps
    public final void synpred140_wcps_fragment() throws RecognitionException {   
        wcpsParser.scalarExpr_return e1 = null;

        wcpsParser.scalarExpr_return e2 = null;


        // src/petascope/wcps/grammar/wcps.g:331:7: (e1= scalarExpr COLON e2= scalarExpr )
        // src/petascope/wcps/grammar/wcps.g:331:7: e1= scalarExpr COLON e2= scalarExpr
        {
        pushFollow(FOLLOW_scalarExpr_in_synpred140_wcps3382);
        e1=scalarExpr();

        state._fsp--;
        if (state.failed) return ;
        match(input,COLON,FOLLOW_COLON_in_synpred140_wcps3384); if (state.failed) return ;
        pushFollow(FOLLOW_scalarExpr_in_synpred140_wcps3388);
        e2=scalarExpr();

        state._fsp--;
        if (state.failed) return ;

        }
    }
    // $ANTLR end synpred140_wcps

    // Delegated rules

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
    public final boolean synpred115_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred115_wcps_fragment(); // can never throw exception
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
    public final boolean synpred100_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred100_wcps_fragment(); // can never throw exception
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
    public final boolean synpred140_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred140_wcps_fragment(); // can never throw exception
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
    public final boolean synpred124_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred124_wcps_fragment(); // can never throw exception
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
    public final boolean synpred114_wcps() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred114_wcps_fragment(); // can never throw exception
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
    protected DFA41 dfa41 = new DFA41(this);
    protected DFA42 dfa42 = new DFA42(this);
    protected DFA43 dfa43 = new DFA43(this);
    protected DFA45 dfa45 = new DFA45(this);
    protected DFA46 dfa46 = new DFA46(this);
    protected DFA47 dfa47 = new DFA47(this);
    protected DFA48 dfa48 = new DFA48(this);
    protected DFA49 dfa49 = new DFA49(this);
    protected DFA50 dfa50 = new DFA50(this);
    protected DFA51 dfa51 = new DFA51(this);
    protected DFA55 dfa55 = new DFA55(this);
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
            return "53:1: processingExpr returns [ProcessingExpr value] : (e1= encodedCoverageExpr | e2= storeExpr | e3= scalarExpr );";
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
            return "()* loopback of 67:9: (op= ( OR | XOR ) e2= coverageLogicTerm )*";
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
            return "()* loopback of 71:9: (op= AND e2= coverageLogicFactor )*";
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
            return "75:9: (op= ( EQUALS | NOTEQUALS | LT | GT | LTE | GTE ) e2= coverageArithmeticExpr )?";
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
            return "()* loopback of 79:9: (op= ( PLUS | MINUS ) e2= coverageArithmeticTerm )*";
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
            return "()* loopback of 83:9: (op= ( MULT | DIVIDE ) e2= coverageArithmeticFactor )*";
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
            return "()* loopback of 87:9: (op= OVERLAY e2= coverageValue )*";
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
        "\52\uffff";
    static final String DFA12_eofS =
        "\52\uffff";
    static final String DFA12_minS =
        "\1\6\36\0\13\uffff";
    static final String DFA12_maxS =
        "\1\154\36\0\13\uffff";
    static final String DFA12_acceptS =
        "\37\uffff\1\1\2\uffff\1\2\4\uffff\1\3\1\4\1\5";
    static final String DFA12_specialS =
        "\1\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\1\14"+
        "\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\1\25\1\26\1\27\1\30\1\31"+
        "\1\32\1\33\1\34\1\35\13\uffff}>";
    static final String[] DFA12_transitionS = {
            "\1\13\17\uffff\1\42\1\15\3\uffff\1\1\1\2\1\3\1\4\1\6\1\7\1\10"+
            "\1\5\7\11\1\12\3\uffff\1\27\4\uffff\1\30\1\31\1\32\1\33\1\34"+
            "\1\36\1\uffff\1\35\1\50\1\17\1\16\16\42\1\25\1\42\1\21\1\20"+
            "\1\14\1\47\2\uffff\3\37\12\uffff\1\24\1\22\10\uffff\1\26\1\23",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
            "\1\uffff",
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
            ""
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
            return "89:1: coverageValue returns [CoverageExpr value] : (e5= subsetExpr | e2= unaryInducedExpr | e4= scaleExpr | e3= crsTransformExpr | e1= coverageAtom );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA12_1 = input.LA(1);

                         
                        int index12_1 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_1);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA12_2 = input.LA(1);

                         
                        int index12_2 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_2);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA12_3 = input.LA(1);

                         
                        int index12_3 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_3);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA12_4 = input.LA(1);

                         
                        int index12_4 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_4);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA12_5 = input.LA(1);

                         
                        int index12_5 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_5);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA12_6 = input.LA(1);

                         
                        int index12_6 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_6);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA12_7 = input.LA(1);

                         
                        int index12_7 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_7);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA12_8 = input.LA(1);

                         
                        int index12_8 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_8);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA12_9 = input.LA(1);

                         
                        int index12_9 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_9);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA12_10 = input.LA(1);

                         
                        int index12_10 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_10);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA12_11 = input.LA(1);

                         
                        int index12_11 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_11);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA12_12 = input.LA(1);

                         
                        int index12_12 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_12);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA12_13 = input.LA(1);

                         
                        int index12_13 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_13);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA12_14 = input.LA(1);

                         
                        int index12_14 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_14);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA12_15 = input.LA(1);

                         
                        int index12_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_15);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA12_16 = input.LA(1);

                         
                        int index12_16 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_16);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA12_17 = input.LA(1);

                         
                        int index12_17 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_17);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA12_18 = input.LA(1);

                         
                        int index12_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_18);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA12_19 = input.LA(1);

                         
                        int index12_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_19);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA12_20 = input.LA(1);

                         
                        int index12_20 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_20);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA12_21 = input.LA(1);

                         
                        int index12_21 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_21);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA12_22 = input.LA(1);

                         
                        int index12_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_22);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA12_23 = input.LA(1);

                         
                        int index12_23 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_23);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA12_24 = input.LA(1);

                         
                        int index12_24 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_24);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA12_25 = input.LA(1);

                         
                        int index12_25 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_25);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA12_26 = input.LA(1);

                         
                        int index12_26 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_26);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA12_27 = input.LA(1);

                         
                        int index12_27 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_27);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA12_28 = input.LA(1);

                         
                        int index12_28 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_28);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA12_29 = input.LA(1);

                         
                        int index12_29 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_29);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA12_30 = input.LA(1);

                         
                        int index12_30 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred21_wcps()) ) {s = 31;}

                        else if ( (synpred22_wcps()) ) {s = 34;}

                        else if ( (true) ) {s = 41;}

                         
                        input.seek(index12_30);
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
            return "96:1: coverageAtom returns [CoverageExpr value] : (e2= scalarExpr | e1= coverageVariable | LPAREN e7= coverageExpr RPAREN | e3= coverageConstantExpr | e4= coverageConstructorExpr | e5= setMetaDataExpr | e6= rangeConstructorExpr );";
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
            "\1\32\20\uffff\1\33\3\uffff\1\43\7\47\7\40\1\41\21\uffff\1"+
            "\35\1\34\16\uffff\1\46\1\uffff\1\37\1\36\1\44\20\uffff\1\45"+
            "\1\31\11\uffff\1\42",
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
            return "105:1: scalarExpr returns [ScalarExpr value] : (e1= metaDataExpr | e2= condenseExpr | e3= booleanScalarExpr | e4= numericScalarExpr | e5= stringScalarExpr | LPAREN e6= scalarExpr RPAREN );";
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

                        else if ( (synpred34_wcps()) ) {s = 101;}

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

                        else if ( (synpred34_wcps()) ) {s = 101;}

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
    static final String DFA34_eotS =
        "\u0099\uffff";
    static final String DFA34_eofS =
        "\u0099\uffff";
    static final String DFA34_minS =
        "\1\6\12\uffff\1\6\1\uffff\3\6\5\uffff\1\6\7\uffff\1\70\1\116\57"+
        "\uffff\12\0\24\uffff\17\0\33\uffff\2\0\1\uffff";
    static final String DFA34_maxS =
        "\1\154\12\uffff\1\154\1\uffff\1\154\2\6\5\uffff\1\154\7\uffff\1"+
        "\70\1\153\57\uffff\12\0\24\uffff\17\0\33\uffff\2\0\1\uffff";
    static final String DFA34_acceptS =
        "\1\uffff\1\1\35\uffff\1\2\1\uffff\1\3\1\4\1\5\16\uffff\1\6\145\uffff"+
        "\1\7";
    static final String DFA34_specialS =
        "\116\uffff\1\0\1\1\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\24\uffff\1"+
        "\12\1\13\1\14\1\15\1\16\1\17\1\20\1\21\1\22\1\23\1\24\1\25\1\26"+
        "\1\27\1\30\33\uffff\1\31\1\32\1\uffff}>";
    static final String[] DFA34_transitionS = {
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
            "\30\1\2\uffff\3\1\11\62\1\uffff\2\1\10\uffff\2\1",
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
            return "203:1: unaryInducedExpr returns [CoverageExpr value] : (e6= fieldExpr | e1= unaryArithmeticExpr | e2= exponentialExpr | e3= trigonometricExpr | e4= booleanExpr | e5= castExpr | e7= rangeConstructorExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA34_78 = input.LA(1);

                         
                        int index34_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_78);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA34_79 = input.LA(1);

                         
                        int index34_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_79);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA34_80 = input.LA(1);

                         
                        int index34_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_80);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA34_81 = input.LA(1);

                         
                        int index34_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_81);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA34_82 = input.LA(1);

                         
                        int index34_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_82);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA34_83 = input.LA(1);

                         
                        int index34_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_83);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA34_84 = input.LA(1);

                         
                        int index34_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_84);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA34_85 = input.LA(1);

                         
                        int index34_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_85);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA34_86 = input.LA(1);

                         
                        int index34_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_86);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA34_87 = input.LA(1);

                         
                        int index34_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_87);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA34_108 = input.LA(1);

                         
                        int index34_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_108);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA34_109 = input.LA(1);

                         
                        int index34_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred71_wcps()) ) {s = 31;}

                         
                        input.seek(index34_109);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA34_110 = input.LA(1);

                         
                        int index34_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_110);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA34_111 = input.LA(1);

                         
                        int index34_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_111);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA34_112 = input.LA(1);

                         
                        int index34_112 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_112);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA34_113 = input.LA(1);

                         
                        int index34_113 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_113);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA34_114 = input.LA(1);

                         
                        int index34_114 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_114);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA34_115 = input.LA(1);

                         
                        int index34_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_115);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA34_116 = input.LA(1);

                         
                        int index34_116 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_116);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA34_117 = input.LA(1);

                         
                        int index34_117 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_117);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA34_118 = input.LA(1);

                         
                        int index34_118 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_118);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA34_119 = input.LA(1);

                         
                        int index34_119 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_119);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA34_120 = input.LA(1);

                         
                        int index34_120 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_120);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA34_121 = input.LA(1);

                         
                        int index34_121 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_121);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA34_122 = input.LA(1);

                         
                        int index34_122 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (synpred74_wcps()) ) {s = 35;}

                         
                        input.seek(index34_122);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA34_150 = input.LA(1);

                         
                        int index34_150 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 152;}

                         
                        input.seek(index34_150);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA34_151 = input.LA(1);

                         
                        int index34_151 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred70_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 152;}

                         
                        input.seek(index34_151);
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
    static final String DFA41_eotS =
        "\u009c\uffff";
    static final String DFA41_eofS =
        "\u009c\uffff";
    static final String DFA41_minS =
        "\12\6\1\15\1\6\1\20\4\6\3\20\1\15\1\6\1\122\1\116\5\6\1\70\1\116"+
        "\3\uffff\172\0";
    static final String DFA41_maxS =
        "\1\154\11\6\1\51\1\154\1\122\1\154\3\6\4\122\1\154\1\122\1\153\5"+
        "\6\1\70\1\153\3\uffff\172\0";
    static final String DFA41_acceptS =
        "\37\uffff\1\1\1\2\1\3\172\uffff";
    static final String DFA41_specialS =
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
    static final String[] DFA41_transitionS = {
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
            "\1\55\17\uffff\1\115\1\56\3\uffff\1\66\1\72\1\73\1\74\1\76"+
            "\1\77\1\100\1\75\7\63\1\64\3\uffff\1\102\4\uffff\1\103\1\104"+
            "\1\105\1\106\1\107\1\111\1\uffff\1\110\1\123\1\60\1\57\2\116"+
            "\3\117\11\120\1\71\1\121\1\62\1\61\1\67\1\122\2\uffff\1\112"+
            "\1\113\1\114\12\uffff\1\70\1\54\10\uffff\1\101\1\65",
            "\1\124\1\125\1\126\1\127\1\130\1\131\74\uffff\1\132",
            "\1\133\20\uffff\1\134\13\uffff\7\142\1\143\21\uffff\1\136\1"+
            "\135\20\uffff\1\140\1\137\22\uffff\1\141\11\uffff\1\144",
            "\1\145",
            "\1\146",
            "\1\147",
            "\1\153\1\154\1\155\1\156\1\157\1\160\2\151\2\150\70\uffff\1"+
            "\152",
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

    static final short[] DFA41_eot = DFA.unpackEncodedString(DFA41_eotS);
    static final short[] DFA41_eof = DFA.unpackEncodedString(DFA41_eofS);
    static final char[] DFA41_min = DFA.unpackEncodedStringToUnsignedChars(DFA41_minS);
    static final char[] DFA41_max = DFA.unpackEncodedStringToUnsignedChars(DFA41_maxS);
    static final short[] DFA41_accept = DFA.unpackEncodedString(DFA41_acceptS);
    static final short[] DFA41_special = DFA.unpackEncodedString(DFA41_specialS);
    static final short[][] DFA41_transition;

    static {
        int numStates = DFA41_transitionS.length;
        DFA41_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA41_transition[i] = DFA.unpackEncodedString(DFA41_transitionS[i]);
        }
    }

    class DFA41 extends DFA {

        public DFA41(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 41;
            this.eot = DFA41_eot;
            this.eof = DFA41_eof;
            this.min = DFA41_min;
            this.max = DFA41_max;
            this.accept = DFA41_accept;
            this.special = DFA41_special;
            this.transition = DFA41_transition;
        }
        public String getDescription() {
            return "248:1: subsetExpr returns [SubsetExpr value] : (e1= trimExpr | e2= sliceExpr | e3= extendExpr );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA41_34 = input.LA(1);

                         
                        int index41_34 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_34);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA41_35 = input.LA(1);

                         
                        int index41_35 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_35);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA41_36 = input.LA(1);

                         
                        int index41_36 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_36);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA41_37 = input.LA(1);

                         
                        int index41_37 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_37);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA41_38 = input.LA(1);

                         
                        int index41_38 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_38);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA41_39 = input.LA(1);

                         
                        int index41_39 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_39);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA41_40 = input.LA(1);

                         
                        int index41_40 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_40);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA41_41 = input.LA(1);

                         
                        int index41_41 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_41);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA41_42 = input.LA(1);

                         
                        int index41_42 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_42);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA41_43 = input.LA(1);

                         
                        int index41_43 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_43);
                        if ( s>=0 ) return s;
                        break;
                    case 10 : 
                        int LA41_44 = input.LA(1);

                         
                        int index41_44 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_44);
                        if ( s>=0 ) return s;
                        break;
                    case 11 : 
                        int LA41_45 = input.LA(1);

                         
                        int index41_45 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_45);
                        if ( s>=0 ) return s;
                        break;
                    case 12 : 
                        int LA41_46 = input.LA(1);

                         
                        int index41_46 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_46);
                        if ( s>=0 ) return s;
                        break;
                    case 13 : 
                        int LA41_47 = input.LA(1);

                         
                        int index41_47 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_47);
                        if ( s>=0 ) return s;
                        break;
                    case 14 : 
                        int LA41_48 = input.LA(1);

                         
                        int index41_48 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_48);
                        if ( s>=0 ) return s;
                        break;
                    case 15 : 
                        int LA41_49 = input.LA(1);

                         
                        int index41_49 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_49);
                        if ( s>=0 ) return s;
                        break;
                    case 16 : 
                        int LA41_50 = input.LA(1);

                         
                        int index41_50 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_50);
                        if ( s>=0 ) return s;
                        break;
                    case 17 : 
                        int LA41_51 = input.LA(1);

                         
                        int index41_51 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_51);
                        if ( s>=0 ) return s;
                        break;
                    case 18 : 
                        int LA41_52 = input.LA(1);

                         
                        int index41_52 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_52);
                        if ( s>=0 ) return s;
                        break;
                    case 19 : 
                        int LA41_53 = input.LA(1);

                         
                        int index41_53 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_53);
                        if ( s>=0 ) return s;
                        break;
                    case 20 : 
                        int LA41_54 = input.LA(1);

                         
                        int index41_54 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_54);
                        if ( s>=0 ) return s;
                        break;
                    case 21 : 
                        int LA41_55 = input.LA(1);

                         
                        int index41_55 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_55);
                        if ( s>=0 ) return s;
                        break;
                    case 22 : 
                        int LA41_56 = input.LA(1);

                         
                        int index41_56 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_56);
                        if ( s>=0 ) return s;
                        break;
                    case 23 : 
                        int LA41_57 = input.LA(1);

                         
                        int index41_57 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_57);
                        if ( s>=0 ) return s;
                        break;
                    case 24 : 
                        int LA41_58 = input.LA(1);

                         
                        int index41_58 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_58);
                        if ( s>=0 ) return s;
                        break;
                    case 25 : 
                        int LA41_59 = input.LA(1);

                         
                        int index41_59 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_59);
                        if ( s>=0 ) return s;
                        break;
                    case 26 : 
                        int LA41_60 = input.LA(1);

                         
                        int index41_60 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_60);
                        if ( s>=0 ) return s;
                        break;
                    case 27 : 
                        int LA41_61 = input.LA(1);

                         
                        int index41_61 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_61);
                        if ( s>=0 ) return s;
                        break;
                    case 28 : 
                        int LA41_62 = input.LA(1);

                         
                        int index41_62 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_62);
                        if ( s>=0 ) return s;
                        break;
                    case 29 : 
                        int LA41_63 = input.LA(1);

                         
                        int index41_63 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_63);
                        if ( s>=0 ) return s;
                        break;
                    case 30 : 
                        int LA41_64 = input.LA(1);

                         
                        int index41_64 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_64);
                        if ( s>=0 ) return s;
                        break;
                    case 31 : 
                        int LA41_65 = input.LA(1);

                         
                        int index41_65 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_65);
                        if ( s>=0 ) return s;
                        break;
                    case 32 : 
                        int LA41_66 = input.LA(1);

                         
                        int index41_66 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_66);
                        if ( s>=0 ) return s;
                        break;
                    case 33 : 
                        int LA41_67 = input.LA(1);

                         
                        int index41_67 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_67);
                        if ( s>=0 ) return s;
                        break;
                    case 34 : 
                        int LA41_68 = input.LA(1);

                         
                        int index41_68 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_68);
                        if ( s>=0 ) return s;
                        break;
                    case 35 : 
                        int LA41_69 = input.LA(1);

                         
                        int index41_69 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_69);
                        if ( s>=0 ) return s;
                        break;
                    case 36 : 
                        int LA41_70 = input.LA(1);

                         
                        int index41_70 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_70);
                        if ( s>=0 ) return s;
                        break;
                    case 37 : 
                        int LA41_71 = input.LA(1);

                         
                        int index41_71 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_71);
                        if ( s>=0 ) return s;
                        break;
                    case 38 : 
                        int LA41_72 = input.LA(1);

                         
                        int index41_72 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_72);
                        if ( s>=0 ) return s;
                        break;
                    case 39 : 
                        int LA41_73 = input.LA(1);

                         
                        int index41_73 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_73);
                        if ( s>=0 ) return s;
                        break;
                    case 40 : 
                        int LA41_74 = input.LA(1);

                         
                        int index41_74 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_74);
                        if ( s>=0 ) return s;
                        break;
                    case 41 : 
                        int LA41_75 = input.LA(1);

                         
                        int index41_75 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_75);
                        if ( s>=0 ) return s;
                        break;
                    case 42 : 
                        int LA41_76 = input.LA(1);

                         
                        int index41_76 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_76);
                        if ( s>=0 ) return s;
                        break;
                    case 43 : 
                        int LA41_77 = input.LA(1);

                         
                        int index41_77 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_77);
                        if ( s>=0 ) return s;
                        break;
                    case 44 : 
                        int LA41_78 = input.LA(1);

                         
                        int index41_78 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_78);
                        if ( s>=0 ) return s;
                        break;
                    case 45 : 
                        int LA41_79 = input.LA(1);

                         
                        int index41_79 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_79);
                        if ( s>=0 ) return s;
                        break;
                    case 46 : 
                        int LA41_80 = input.LA(1);

                         
                        int index41_80 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_80);
                        if ( s>=0 ) return s;
                        break;
                    case 47 : 
                        int LA41_81 = input.LA(1);

                         
                        int index41_81 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_81);
                        if ( s>=0 ) return s;
                        break;
                    case 48 : 
                        int LA41_82 = input.LA(1);

                         
                        int index41_82 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_82);
                        if ( s>=0 ) return s;
                        break;
                    case 49 : 
                        int LA41_83 = input.LA(1);

                         
                        int index41_83 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_83);
                        if ( s>=0 ) return s;
                        break;
                    case 50 : 
                        int LA41_84 = input.LA(1);

                         
                        int index41_84 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_84);
                        if ( s>=0 ) return s;
                        break;
                    case 51 : 
                        int LA41_85 = input.LA(1);

                         
                        int index41_85 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_85);
                        if ( s>=0 ) return s;
                        break;
                    case 52 : 
                        int LA41_86 = input.LA(1);

                         
                        int index41_86 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_86);
                        if ( s>=0 ) return s;
                        break;
                    case 53 : 
                        int LA41_87 = input.LA(1);

                         
                        int index41_87 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_87);
                        if ( s>=0 ) return s;
                        break;
                    case 54 : 
                        int LA41_88 = input.LA(1);

                         
                        int index41_88 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_88);
                        if ( s>=0 ) return s;
                        break;
                    case 55 : 
                        int LA41_89 = input.LA(1);

                         
                        int index41_89 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_89);
                        if ( s>=0 ) return s;
                        break;
                    case 56 : 
                        int LA41_90 = input.LA(1);

                         
                        int index41_90 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_90);
                        if ( s>=0 ) return s;
                        break;
                    case 57 : 
                        int LA41_91 = input.LA(1);

                         
                        int index41_91 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_91);
                        if ( s>=0 ) return s;
                        break;
                    case 58 : 
                        int LA41_92 = input.LA(1);

                         
                        int index41_92 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_92);
                        if ( s>=0 ) return s;
                        break;
                    case 59 : 
                        int LA41_93 = input.LA(1);

                         
                        int index41_93 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_93);
                        if ( s>=0 ) return s;
                        break;
                    case 60 : 
                        int LA41_94 = input.LA(1);

                         
                        int index41_94 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_94);
                        if ( s>=0 ) return s;
                        break;
                    case 61 : 
                        int LA41_95 = input.LA(1);

                         
                        int index41_95 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_95);
                        if ( s>=0 ) return s;
                        break;
                    case 62 : 
                        int LA41_96 = input.LA(1);

                         
                        int index41_96 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_96);
                        if ( s>=0 ) return s;
                        break;
                    case 63 : 
                        int LA41_97 = input.LA(1);

                         
                        int index41_97 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_97);
                        if ( s>=0 ) return s;
                        break;
                    case 64 : 
                        int LA41_98 = input.LA(1);

                         
                        int index41_98 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_98);
                        if ( s>=0 ) return s;
                        break;
                    case 65 : 
                        int LA41_99 = input.LA(1);

                         
                        int index41_99 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_99);
                        if ( s>=0 ) return s;
                        break;
                    case 66 : 
                        int LA41_100 = input.LA(1);

                         
                        int index41_100 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_100);
                        if ( s>=0 ) return s;
                        break;
                    case 67 : 
                        int LA41_101 = input.LA(1);

                         
                        int index41_101 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_101);
                        if ( s>=0 ) return s;
                        break;
                    case 68 : 
                        int LA41_102 = input.LA(1);

                         
                        int index41_102 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_102);
                        if ( s>=0 ) return s;
                        break;
                    case 69 : 
                        int LA41_103 = input.LA(1);

                         
                        int index41_103 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_103);
                        if ( s>=0 ) return s;
                        break;
                    case 70 : 
                        int LA41_104 = input.LA(1);

                         
                        int index41_104 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_104);
                        if ( s>=0 ) return s;
                        break;
                    case 71 : 
                        int LA41_105 = input.LA(1);

                         
                        int index41_105 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_105);
                        if ( s>=0 ) return s;
                        break;
                    case 72 : 
                        int LA41_106 = input.LA(1);

                         
                        int index41_106 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_106);
                        if ( s>=0 ) return s;
                        break;
                    case 73 : 
                        int LA41_107 = input.LA(1);

                         
                        int index41_107 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_107);
                        if ( s>=0 ) return s;
                        break;
                    case 74 : 
                        int LA41_108 = input.LA(1);

                         
                        int index41_108 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_108);
                        if ( s>=0 ) return s;
                        break;
                    case 75 : 
                        int LA41_109 = input.LA(1);

                         
                        int index41_109 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_109);
                        if ( s>=0 ) return s;
                        break;
                    case 76 : 
                        int LA41_110 = input.LA(1);

                         
                        int index41_110 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_110);
                        if ( s>=0 ) return s;
                        break;
                    case 77 : 
                        int LA41_111 = input.LA(1);

                         
                        int index41_111 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_111);
                        if ( s>=0 ) return s;
                        break;
                    case 78 : 
                        int LA41_112 = input.LA(1);

                         
                        int index41_112 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_112);
                        if ( s>=0 ) return s;
                        break;
                    case 79 : 
                        int LA41_113 = input.LA(1);

                         
                        int index41_113 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_113);
                        if ( s>=0 ) return s;
                        break;
                    case 80 : 
                        int LA41_114 = input.LA(1);

                         
                        int index41_114 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_114);
                        if ( s>=0 ) return s;
                        break;
                    case 81 : 
                        int LA41_115 = input.LA(1);

                         
                        int index41_115 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_115);
                        if ( s>=0 ) return s;
                        break;
                    case 82 : 
                        int LA41_116 = input.LA(1);

                         
                        int index41_116 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_116);
                        if ( s>=0 ) return s;
                        break;
                    case 83 : 
                        int LA41_117 = input.LA(1);

                         
                        int index41_117 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_117);
                        if ( s>=0 ) return s;
                        break;
                    case 84 : 
                        int LA41_118 = input.LA(1);

                         
                        int index41_118 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_118);
                        if ( s>=0 ) return s;
                        break;
                    case 85 : 
                        int LA41_119 = input.LA(1);

                         
                        int index41_119 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_119);
                        if ( s>=0 ) return s;
                        break;
                    case 86 : 
                        int LA41_120 = input.LA(1);

                         
                        int index41_120 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_120);
                        if ( s>=0 ) return s;
                        break;
                    case 87 : 
                        int LA41_121 = input.LA(1);

                         
                        int index41_121 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_121);
                        if ( s>=0 ) return s;
                        break;
                    case 88 : 
                        int LA41_122 = input.LA(1);

                         
                        int index41_122 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_122);
                        if ( s>=0 ) return s;
                        break;
                    case 89 : 
                        int LA41_123 = input.LA(1);

                         
                        int index41_123 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_123);
                        if ( s>=0 ) return s;
                        break;
                    case 90 : 
                        int LA41_124 = input.LA(1);

                         
                        int index41_124 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_124);
                        if ( s>=0 ) return s;
                        break;
                    case 91 : 
                        int LA41_125 = input.LA(1);

                         
                        int index41_125 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_125);
                        if ( s>=0 ) return s;
                        break;
                    case 92 : 
                        int LA41_126 = input.LA(1);

                         
                        int index41_126 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_126);
                        if ( s>=0 ) return s;
                        break;
                    case 93 : 
                        int LA41_127 = input.LA(1);

                         
                        int index41_127 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_127);
                        if ( s>=0 ) return s;
                        break;
                    case 94 : 
                        int LA41_128 = input.LA(1);

                         
                        int index41_128 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_128);
                        if ( s>=0 ) return s;
                        break;
                    case 95 : 
                        int LA41_129 = input.LA(1);

                         
                        int index41_129 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_129);
                        if ( s>=0 ) return s;
                        break;
                    case 96 : 
                        int LA41_130 = input.LA(1);

                         
                        int index41_130 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_130);
                        if ( s>=0 ) return s;
                        break;
                    case 97 : 
                        int LA41_131 = input.LA(1);

                         
                        int index41_131 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_131);
                        if ( s>=0 ) return s;
                        break;
                    case 98 : 
                        int LA41_132 = input.LA(1);

                         
                        int index41_132 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_132);
                        if ( s>=0 ) return s;
                        break;
                    case 99 : 
                        int LA41_133 = input.LA(1);

                         
                        int index41_133 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_133);
                        if ( s>=0 ) return s;
                        break;
                    case 100 : 
                        int LA41_134 = input.LA(1);

                         
                        int index41_134 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_134);
                        if ( s>=0 ) return s;
                        break;
                    case 101 : 
                        int LA41_135 = input.LA(1);

                         
                        int index41_135 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_135);
                        if ( s>=0 ) return s;
                        break;
                    case 102 : 
                        int LA41_136 = input.LA(1);

                         
                        int index41_136 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_136);
                        if ( s>=0 ) return s;
                        break;
                    case 103 : 
                        int LA41_137 = input.LA(1);

                         
                        int index41_137 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_137);
                        if ( s>=0 ) return s;
                        break;
                    case 104 : 
                        int LA41_138 = input.LA(1);

                         
                        int index41_138 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_138);
                        if ( s>=0 ) return s;
                        break;
                    case 105 : 
                        int LA41_139 = input.LA(1);

                         
                        int index41_139 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_139);
                        if ( s>=0 ) return s;
                        break;
                    case 106 : 
                        int LA41_140 = input.LA(1);

                         
                        int index41_140 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_140);
                        if ( s>=0 ) return s;
                        break;
                    case 107 : 
                        int LA41_141 = input.LA(1);

                         
                        int index41_141 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_141);
                        if ( s>=0 ) return s;
                        break;
                    case 108 : 
                        int LA41_142 = input.LA(1);

                         
                        int index41_142 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_142);
                        if ( s>=0 ) return s;
                        break;
                    case 109 : 
                        int LA41_143 = input.LA(1);

                         
                        int index41_143 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_143);
                        if ( s>=0 ) return s;
                        break;
                    case 110 : 
                        int LA41_144 = input.LA(1);

                         
                        int index41_144 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_144);
                        if ( s>=0 ) return s;
                        break;
                    case 111 : 
                        int LA41_145 = input.LA(1);

                         
                        int index41_145 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_145);
                        if ( s>=0 ) return s;
                        break;
                    case 112 : 
                        int LA41_146 = input.LA(1);

                         
                        int index41_146 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_146);
                        if ( s>=0 ) return s;
                        break;
                    case 113 : 
                        int LA41_147 = input.LA(1);

                         
                        int index41_147 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_147);
                        if ( s>=0 ) return s;
                        break;
                    case 114 : 
                        int LA41_148 = input.LA(1);

                         
                        int index41_148 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_148);
                        if ( s>=0 ) return s;
                        break;
                    case 115 : 
                        int LA41_149 = input.LA(1);

                         
                        int index41_149 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_149);
                        if ( s>=0 ) return s;
                        break;
                    case 116 : 
                        int LA41_150 = input.LA(1);

                         
                        int index41_150 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_150);
                        if ( s>=0 ) return s;
                        break;
                    case 117 : 
                        int LA41_151 = input.LA(1);

                         
                        int index41_151 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_151);
                        if ( s>=0 ) return s;
                        break;
                    case 118 : 
                        int LA41_152 = input.LA(1);

                         
                        int index41_152 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_152);
                        if ( s>=0 ) return s;
                        break;
                    case 119 : 
                        int LA41_153 = input.LA(1);

                         
                        int index41_153 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_153);
                        if ( s>=0 ) return s;
                        break;
                    case 120 : 
                        int LA41_154 = input.LA(1);

                         
                        int index41_154 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_154);
                        if ( s>=0 ) return s;
                        break;
                    case 121 : 
                        int LA41_155 = input.LA(1);

                         
                        int index41_155 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred99_wcps()) ) {s = 31;}

                        else if ( (synpred100_wcps()) ) {s = 32;}

                         
                        input.seek(index41_155);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 41, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA42_eotS =
        "\40\uffff";
    static final String DFA42_eofS =
        "\40\uffff";
    static final String DFA42_minS =
        "\1\6\37\uffff";
    static final String DFA42_maxS =
        "\1\154\37\uffff";
    static final String DFA42_acceptS =
        "\1\uffff\1\1\35\uffff\1\2";
    static final String DFA42_specialS =
        "\40\uffff}>";
    static final String[] DFA42_transitionS = {
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
            return "253:1: trimExpr returns [TrimExpr value] : (e1= coverageAtom LBRACKET dil= dimensionIntervalList RBRACKET | TRIM LPAREN e2= coverageExpr COMMA LBRACE dil= dimensionIntervalList RBRACE RPAREN );";
        }
    }
    static final String DFA43_eotS =
        "\40\uffff";
    static final String DFA43_eofS =
        "\40\uffff";
    static final String DFA43_minS =
        "\1\6\37\uffff";
    static final String DFA43_maxS =
        "\1\154\37\uffff";
    static final String DFA43_acceptS =
        "\1\uffff\1\1\35\uffff\1\2";
    static final String DFA43_specialS =
        "\40\uffff}>";
    static final String[] DFA43_transitionS = {
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
            return "257:1: sliceExpr returns [SliceExpr value] : (e1= coverageAtom LBRACKET dpl= dimensionPointList RBRACKET | SLICE LPAREN e2= coverageExpr COMMA LBRACE dpl= dimensionPointList RBRACE RPAREN );";
        }
    }
    static final String DFA45_eotS =
        "\77\uffff";
    static final String DFA45_eofS =
        "\1\1\76\uffff";
    static final String DFA45_minS =
        "\1\7\7\uffff\1\0\66\uffff";
    static final String DFA45_maxS =
        "\1\140\7\uffff\1\0\66\uffff";
    static final String DFA45_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA45_specialS =
        "\10\uffff\1\0\66\uffff}>";
    static final String[] DFA45_transitionS = {
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
            return "()* loopback of 278:7: (op= ( OR | XOR ) e2= booleanScalarTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA45_8 = input.LA(1);

                         
                        int index45_8 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred114_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index45_8);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 45, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA46_eotS =
        "\77\uffff";
    static final String DFA46_eofS =
        "\1\1\76\uffff";
    static final String DFA46_minS =
        "\1\7\7\uffff\1\0\66\uffff";
    static final String DFA46_maxS =
        "\1\140\7\uffff\1\0\66\uffff";
    static final String DFA46_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA46_specialS =
        "\10\uffff\1\0\66\uffff}>";
    static final String[] DFA46_transitionS = {
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

    static final short[] DFA46_eot = DFA.unpackEncodedString(DFA46_eotS);
    static final short[] DFA46_eof = DFA.unpackEncodedString(DFA46_eofS);
    static final char[] DFA46_min = DFA.unpackEncodedStringToUnsignedChars(DFA46_minS);
    static final char[] DFA46_max = DFA.unpackEncodedStringToUnsignedChars(DFA46_maxS);
    static final short[] DFA46_accept = DFA.unpackEncodedString(DFA46_acceptS);
    static final short[] DFA46_special = DFA.unpackEncodedString(DFA46_specialS);
    static final short[][] DFA46_transition;

    static {
        int numStates = DFA46_transitionS.length;
        DFA46_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA46_transition[i] = DFA.unpackEncodedString(DFA46_transitionS[i]);
        }
    }

    class DFA46 extends DFA {

        public DFA46(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 46;
            this.eot = DFA46_eot;
            this.eof = DFA46_eof;
            this.min = DFA46_min;
            this.max = DFA46_max;
            this.accept = DFA46_accept;
            this.special = DFA46_special;
            this.transition = DFA46_transition;
        }
        public String getDescription() {
            return "()* loopback of 282:4: (op= AND e2= booleanScalarNegation )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA46_8 = input.LA(1);

                         
                        int index46_8 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred115_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index46_8);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 46, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA47_eotS =
        "\17\uffff";
    static final String DFA47_eofS =
        "\17\uffff";
    static final String DFA47_minS =
        "\1\6\16\uffff";
    static final String DFA47_maxS =
        "\1\154\16\uffff";
    static final String DFA47_acceptS =
        "\1\uffff\1\1\14\uffff\1\2";
    static final String DFA47_specialS =
        "\17\uffff}>";
    static final String[] DFA47_transitionS = {
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
            return "284:1: booleanScalarNegation returns [BooleanScalarExpr value] : (e1= booleanScalarAtom | op= NOT e1= booleanScalarAtom );";
        }
    }
    static final String DFA48_eotS =
        "\34\uffff";
    static final String DFA48_eofS =
        "\34\uffff";
    static final String DFA48_minS =
        "\2\6\14\uffff\2\0\2\uffff\10\0\2\uffff";
    static final String DFA48_maxS =
        "\2\154\14\uffff\2\0\2\uffff\10\0\2\uffff";
    static final String DFA48_acceptS =
        "\2\uffff\1\2\1\uffff\1\3\10\uffff\1\4\2\uffff\1\1\13\uffff";
    static final String DFA48_specialS =
        "\16\uffff\1\0\1\1\2\uffff\1\2\1\3\1\4\1\5\1\6\1\7\1\10\1\11\2\uffff}>";
    static final String[] DFA48_transitionS = {
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
            return "288:1: booleanScalarAtom returns [BooleanScalarExpr value] : ( LPAREN e1= booleanScalarExpr RPAREN | s1= stringScalarExpr cop= compOp s2= stringScalarExpr | n1= numericScalarExpr cop= compOp n2= numericScalarExpr | e= BOOLEANCONSTANT );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA48_14 = input.LA(1);

                         
                        int index48_14 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_14);
                        if ( s>=0 ) return s;
                        break;
                    case 1 : 
                        int LA48_15 = input.LA(1);

                         
                        int index48_15 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_15);
                        if ( s>=0 ) return s;
                        break;
                    case 2 : 
                        int LA48_18 = input.LA(1);

                         
                        int index48_18 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_18);
                        if ( s>=0 ) return s;
                        break;
                    case 3 : 
                        int LA48_19 = input.LA(1);

                         
                        int index48_19 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_19);
                        if ( s>=0 ) return s;
                        break;
                    case 4 : 
                        int LA48_20 = input.LA(1);

                         
                        int index48_20 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_20);
                        if ( s>=0 ) return s;
                        break;
                    case 5 : 
                        int LA48_21 = input.LA(1);

                         
                        int index48_21 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_21);
                        if ( s>=0 ) return s;
                        break;
                    case 6 : 
                        int LA48_22 = input.LA(1);

                         
                        int index48_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_22);
                        if ( s>=0 ) return s;
                        break;
                    case 7 : 
                        int LA48_23 = input.LA(1);

                         
                        int index48_23 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_23);
                        if ( s>=0 ) return s;
                        break;
                    case 8 : 
                        int LA48_24 = input.LA(1);

                         
                        int index48_24 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_24);
                        if ( s>=0 ) return s;
                        break;
                    case 9 : 
                        int LA48_25 = input.LA(1);

                         
                        int index48_25 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred117_wcps()) ) {s = 16;}

                        else if ( (synpred119_wcps()) ) {s = 4;}

                         
                        input.seek(index48_25);
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
        "\77\uffff";
    static final String DFA49_eofS =
        "\1\1\76\uffff";
    static final String DFA49_minS =
        "\1\7\3\uffff\1\0\72\uffff";
    static final String DFA49_maxS =
        "\1\140\3\uffff\1\0\72\uffff";
    static final String DFA49_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA49_specialS =
        "\4\uffff\1\0\72\uffff}>";
    static final String[] DFA49_transitionS = {
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
            return "()* loopback of 296:4: (op= ( PLUS | MINUS ) e2= numericScalarTerm )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA49_4 = input.LA(1);

                         
                        int index49_4 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred121_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index49_4);
                        if ( s>=0 ) return s;
                        break;
            }
            if (state.backtracking>0) {state.failed=true; return -1;}
            NoViableAltException nvae =
                new NoViableAltException(getDescription(), 49, _s, input);
            error(nvae);
            throw nvae;
        }
    }
    static final String DFA50_eotS =
        "\77\uffff";
    static final String DFA50_eofS =
        "\1\1\76\uffff";
    static final String DFA50_minS =
        "\1\7\3\uffff\1\0\72\uffff";
    static final String DFA50_maxS =
        "\1\140\3\uffff\1\0\72\uffff";
    static final String DFA50_acceptS =
        "\1\uffff\1\2\74\uffff\1\1";
    static final String DFA50_specialS =
        "\4\uffff\1\0\72\uffff}>";
    static final String[] DFA50_transitionS = {
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
            return "()* loopback of 300:3: (op= ( MULT | DIVIDE ) e2= numericScalarFactor )*";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA50_4 = input.LA(1);

                         
                        int index50_4 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred123_wcps()) ) {s = 62;}

                        else if ( (true) ) {s = 1;}

                         
                        input.seek(index50_4);
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
        "\26\uffff";
    static final String DFA51_eofS =
        "\26\uffff";
    static final String DFA51_minS =
        "\2\6\11\uffff\1\0\12\uffff";
    static final String DFA51_maxS =
        "\2\154\11\uffff\1\0\12\uffff";
    static final String DFA51_acceptS =
        "\2\uffff\1\2\1\3\1\4\1\5\1\6\1\7\1\11\1\uffff\1\12\1\uffff\1\1\10"+
        "\uffff\1\10";
    static final String DFA51_specialS =
        "\13\uffff\1\0\12\uffff}>";
    static final String[] DFA51_transitionS = {
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
            return "302:1: numericScalarFactor returns [NumericScalarExpr value] : ( LPAREN e1= numericScalarExpr RPAREN | op= MINUS e10= numericScalarFactor | op= ABS LPAREN e12= numericScalarExpr RPAREN | op= SQRT LPAREN e11= numericScalarExpr RPAREN | op= ROUND LPAREN e1= numericScalarExpr RPAREN | e= INTEGERCONSTANT | e= FLOATCONSTANT | e2= complexConstant | e3= condenseExpr | e4= variableName );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA51_11 = input.LA(1);

                         
                        int index51_11 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred124_wcps()) ) {s = 12;}

                        else if ( (synpred131_wcps()) ) {s = 21;}

                         
                        input.seek(index51_11);
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
    static final String DFA55_eotS =
        "\30\uffff";
    static final String DFA55_eofS =
        "\30\uffff";
    static final String DFA55_minS =
        "\1\6\4\uffff\1\6\20\uffff\1\0\1\uffff";
    static final String DFA55_maxS =
        "\1\154\4\uffff\1\6\20\uffff\1\0\1\uffff";
    static final String DFA55_acceptS =
        "\1\uffff\1\1\25\uffff\1\2";
    static final String DFA55_specialS =
        "\26\uffff\1\0\1\uffff}>";
    static final String[] DFA55_transitionS = {
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
            return "330:1: dimensionIntervalExpr returns [DimensionIntervalExpr value] : (e1= scalarExpr COLON e2= scalarExpr | DOMAIN LPAREN e3= coverageName COLON e4= axisName COLON e5= crsName RPAREN );";
        }
        public int specialStateTransition(int s, IntStream _input) throws NoViableAltException {
            TokenStream input = (TokenStream)_input;
        	int _s = s;
            switch ( s ) {
                    case 0 : 
                        int LA55_22 = input.LA(1);

                         
                        int index55_22 = input.index();
                        input.rewind();
                        s = -1;
                        if ( (synpred140_wcps()) ) {s = 1;}

                        else if ( (true) ) {s = 23;}

                         
                        input.seek(index55_22);
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
    public static final BitSet FOLLOW_scalarExpr_in_rangeExpr1811 = new BitSet(new long[]{0x0200200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1818 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_rangeExpr1822 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_rangeExpr1824 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_rangeExpr1828 = new BitSet(new long[]{0x0200200000000000L});
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
    public static final BitSet FOLLOW_CRSTRANSFORM_in_crsTransformExpr1915 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_crsTransformExpr1917 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_crsTransformExpr1921 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsTransformExpr1923 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_crsTransformExpr1927 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_crsTransformExpr1929 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_fieldInterpolationList_in_crsTransformExpr1933 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_crsTransformExpr1935 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACE_in_fieldInterpolationList1952 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1956 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_COMMA_in_fieldInterpolationList1963 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldInterpolationElement_in_fieldInterpolationList1967 = new BitSet(new long[]{0x0200000000000100L});
    public static final BitSet FOLLOW_RBRACE_in_fieldInterpolationList1974 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldName_in_fieldInterpolationElement1989 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_interpolationMethod_in_fieldInterpolationElement1993 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_fieldExpr_in_unaryInducedExpr2013 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryArithmeticExpr_in_unaryInducedExpr2022 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_exponentialExpr_in_unaryInducedExpr2034 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trigonometricExpr_in_unaryInducedExpr2046 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanExpr_in_unaryInducedExpr2058 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_castExpr_in_unaryInducedExpr2070 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rangeConstructorExpr_in_unaryInducedExpr2082 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_unaryArithmeticExpr2105 = new BitSet(new long[]{0x35F847FFF8801840L,0x000018060001D000L});
    public static final BitSet FOLLOW_coverageAtom_in_unaryArithmeticExpr2113 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_unaryArithmeticExpr2125 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_unaryArithmeticExpr2135 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_unaryArithmeticExpr2139 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_unaryArithmeticExpr2141 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_exponentialExpr2164 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_exponentialExpr2172 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_exponentialExpr2176 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_exponentialExpr2178 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_trigonometricExpr2201 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_trigonometricExpr2221 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_trigonometricExpr2225 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_trigonometricExpr2227 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_booleanExpr2250 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_booleanExpr2254 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BIT_in_booleanExpr2266 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_booleanExpr2268 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_booleanExpr2272 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_booleanExpr2274 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_booleanExpr2278 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_booleanExpr2280 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_indexTerm_in_indexExpr2303 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_set_in_indexExpr2313 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexTerm_in_indexExpr2321 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_indexFactor_in_indexTerm2346 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_indexTerm2359 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexFactor_in_indexTerm2367 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_INTEGERCONSTANT_in_indexFactor2394 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ROUND_in_indexFactor2407 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_indexFactor2409 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_indexFactor2413 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_indexFactor2415 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_indexFactor2427 = new BitSet(new long[]{0x0000000000000040L,0x000000000000C000L});
    public static final BitSet FOLLOW_indexExpr_in_indexFactor2431 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_indexFactor2433 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IDENTIFIER_in_stringScalarExpr2460 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_stringScalarExpr2462 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_stringScalarExpr2466 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_stringScalarExpr2468 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_stringScalarExpr2481 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SCALE_in_scaleExpr2499 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_scaleExpr2501 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_scaleExpr2505 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_scaleExpr2507 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_scaleExpr2511 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_scaleExpr2513 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_fieldInterpolationList_in_scaleExpr2517 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_scaleExpr2519 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trimExpr_in_subsetExpr2538 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_sliceExpr_in_subsetExpr2547 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_extendExpr_in_subsetExpr2556 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_trimExpr2573 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_LBRACKET_in_trimExpr2575 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_trimExpr2579 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_RBRACKET_in_trimExpr2581 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_TRIM_in_trimExpr2590 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_trimExpr2592 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_trimExpr2596 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_trimExpr2598 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_trimExpr2600 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_trimExpr2604 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_trimExpr2606 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_trimExpr2608 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_sliceExpr2625 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_LBRACKET_in_sliceExpr2627 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointList_in_sliceExpr2631 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_RBRACKET_in_sliceExpr2633 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SLICE_in_sliceExpr2640 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_sliceExpr2642 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_sliceExpr2646 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_sliceExpr2648 = new BitSet(new long[]{0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACE_in_sliceExpr2650 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointList_in_sliceExpr2654 = new BitSet(new long[]{0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACE_in_sliceExpr2656 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_sliceExpr2658 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EXTEND_in_extendExpr2673 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_extendExpr2675 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_extendExpr2679 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_extendExpr2681 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalList_in_extendExpr2685 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_extendExpr2687 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_castExpr2705 = new BitSet(new long[]{0x0000000000000000L,0x00000000FF800000L});
    public static final BitSet FOLLOW_rangeType_in_castExpr2709 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_castExpr2711 = new BitSet(new long[]{0xFDF847FFF8C01840L,0x000018060073FFFFL});
    public static final BitSet FOLLOW_coverageExpr_in_castExpr2715 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_rangeType2738 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_UNSIGNED_in_rangeType2764 = new BitSet(new long[]{0x0000000000000000L,0x0000000007000000L});
    public static final BitSet FOLLOW_set_in_rangeType2768 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_coverageAtom_in_fieldExpr2797 = new BitSet(new long[]{0x0000000000000000L,0x0000000100000000L});
    public static final BitSet FOLLOW_DOT_in_fieldExpr2799 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_fieldName_in_fieldExpr2803 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_booleanScalarExpr2828 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_set_in_booleanScalarExpr2841 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_booleanScalarExpr2849 = new BitSet(new long[]{0x0000000000006002L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_booleanScalarTerm2871 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_AND_in_booleanScalarTerm2881 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_booleanScalarTerm2885 = new BitSet(new long[]{0x0000000000008002L});
    public static final BitSet FOLLOW_booleanScalarAtom_in_booleanScalarNegation2906 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_booleanScalarNegation2915 = new BitSet(new long[]{0x300007F808800040L,0x000010060001C000L});
    public static final BitSet FOLLOW_booleanScalarAtom_in_booleanScalarNegation2919 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_booleanScalarAtom2934 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_booleanScalarAtom2938 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_booleanScalarAtom2940 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringScalarExpr_in_booleanScalarAtom2949 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_booleanScalarAtom2953 = new BitSet(new long[]{0x0000000008000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_stringScalarExpr_in_booleanScalarAtom2957 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_booleanScalarAtom2967 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_booleanScalarAtom2971 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_booleanScalarAtom2975 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BOOLEANCONSTANT_in_booleanScalarAtom2985 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarTerm_in_numericScalarExpr3002 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_set_in_numericScalarExpr3012 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarTerm_in_numericScalarExpr3020 = new BitSet(new long[]{0x0000000000C00002L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarTerm3039 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_set_in_numericScalarTerm3048 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarTerm3056 = new BitSet(new long[]{0x0000000003000002L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3076 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3080 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3082 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MINUS_in_numericScalarFactor3094 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_numericScalarFactor3098 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ABS_in_numericScalarFactor3110 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3112 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3116 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3118 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SQRT_in_numericScalarFactor3130 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3132 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3136 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3138 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ROUND_in_numericScalarFactor3150 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_numericScalarFactor3152 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_numericScalarFactor3156 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_numericScalarFactor3158 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INTEGERCONSTANT_in_numericScalarFactor3170 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_numericScalarFactor3182 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_numericScalarFactor3194 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_condenseExpr_in_numericScalarFactor3206 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variableName_in_numericScalarFactor3218 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EQUALS_in_compOp3236 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOTEQUALS_in_compOp3243 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LT_in_compOp3250 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GT_in_compOp3257 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LTE_in_compOp3264 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GTE_in_compOp3271 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3291 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_dimensionIntervalList3304 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionIntervalElement_in_dimensionIntervalList3308 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_axisName_in_dimensionIntervalElement3333 = new BitSet(new long[]{0x0000200000000040L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalElement3338 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionIntervalElement3342 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionIntervalElement3353 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_dimensionIntervalExpr_in_dimensionIntervalElement3357 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionIntervalElement3359 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionIntervalExpr3382 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3384 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionIntervalExpr3388 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DOMAIN_in_dimensionIntervalExpr3398 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionIntervalExpr3400 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_coverageName_in_dimensionIntervalExpr3404 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3406 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_axisName_in_dimensionIntervalExpr3410 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionIntervalExpr3412 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionIntervalExpr3416 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionIntervalExpr3418 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_dimensionPointElement_in_dimensionPointList3441 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_COMMA_in_dimensionPointList3451 = new BitSet(new long[]{0x0000000000000000L,0x0000080000014000L});
    public static final BitSet FOLLOW_dimensionPointElement_in_dimensionPointList3455 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_axisName_in_dimensionPointElement3480 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionPointElement3482 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_dimensionPoint_in_dimensionPointElement3486 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionPointElement3488 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_axisName_in_dimensionPointElement3500 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_dimensionPointElement3502 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_crsName_in_dimensionPointElement3506 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_LPAREN_in_dimensionPointElement3508 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_dimensionPoint_in_dimensionPointElement3512 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_dimensionPointElement3514 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_dimensionPoint3537 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_interpolationMethod3555 = new BitSet(new long[]{0x0000000000000000L,0x0000007800000000L});
    public static final BitSet FOLLOW_interpolationType_in_interpolationMethod3559 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_interpolationMethod3561 = new BitSet(new long[]{0x0000000000000000L,0x0000078000000000L});
    public static final BitSet FOLLOW_nullResistence_in_interpolationMethod3565 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_interpolationMethod3567 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_interpolationType3584 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_nullResistence3609 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_condenseOpType3634 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_fieldName3661 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_constant3678 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_constant3695 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_complexConstant3710 = new BitSet(new long[]{0x0000000000000000L,0x0000000400000000L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_complexConstant3714 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_COMMA_in_complexConstant3716 = new BitSet(new long[]{0x0000000000000000L,0x0000000400000000L});
    public static final BitSet FOLLOW_FLOATCONSTANT_in_complexConstant3720 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_complexConstant3722 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_stringConstant3739 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_name3756 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_stringConstant_in_crsName3783 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_axisName3800 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VARIABLE_DOLLAR_in_variableName3817 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NAME_in_coverageVariable3834 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_coverageName3849 = new BitSet(new long[]{0x0000000000000002L});
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
    public static final BitSet FOLLOW_fieldExpr_in_synpred70_wcps2013 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_unaryArithmeticExpr_in_synpred71_wcps2022 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_booleanExpr_in_synpred74_wcps2058 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_trimExpr_in_synpred99_wcps2538 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_sliceExpr_in_synpred100_wcps2547 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred114_wcps2841 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarTerm_in_synpred114_wcps2849 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AND_in_synpred115_wcps2881 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarNegation_in_synpred115_wcps2885 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred117_wcps2934 = new BitSet(new long[]{0x300007F808800040L,0x000010060001D000L});
    public static final BitSet FOLLOW_booleanScalarExpr_in_synpred117_wcps2938 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred117_wcps2940 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred119_wcps2967 = new BitSet(new long[]{0x00000000003F0000L});
    public static final BitSet FOLLOW_compOp_in_synpred119_wcps2971 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred119_wcps2975 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred121_wcps3012 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarTerm_in_synpred121_wcps3020 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_synpred123_wcps3048 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarFactor_in_synpred123_wcps3056 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_synpred124_wcps3076 = new BitSet(new long[]{0x300007F800800040L,0x000010040000C000L});
    public static final BitSet FOLLOW_numericScalarExpr_in_synpred124_wcps3080 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_RPAREN_in_synpred124_wcps3082 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_complexConstant_in_synpred131_wcps3194 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred140_wcps3382 = new BitSet(new long[]{0x0000200000000000L});
    public static final BitSet FOLLOW_COLON_in_synpred140_wcps3384 = new BitSet(new long[]{0x300007FFF8801840L,0x000010060001D000L});
    public static final BitSet FOLLOW_scalarExpr_in_synpred140_wcps3388 = new BitSet(new long[]{0x0000000000000002L});

}