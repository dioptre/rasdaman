package syntaxParser;

class AxisPointList extends List {
    public AxisPointList(AxisPointElement elem) { super((IParseTreeNode)elem); }
    public AxisPointList(AxisPointElement elem, AxisPointList next) { super((IParseTreeNode)elem, (List)next); }
}


