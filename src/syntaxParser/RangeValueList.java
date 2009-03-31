package syntaxParser;

class RangeValueList extends List {
    public RangeValueList(RangeValue elem) { super((IParseTreeNode)elem); }
    public RangeValueList(RangeValue elem, RangeValueList next) { super((IParseTreeNode)elem, (List)next); }
}




