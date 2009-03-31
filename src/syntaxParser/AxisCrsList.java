package syntaxParser;

class AxisCrsList extends List {
    public AxisCrsList(AxisCrsElement elem) { super((IParseTreeNode)elem); }
    public AxisCrsList(AxisCrsElement elem, AxisCrsList next) { super((IParseTreeNode)elem, (List)next); }
}

