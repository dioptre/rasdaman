package syntaxParser;

class AxisIntervalList extends List {
    public AxisIntervalList(AxisIntervalElement elem) { super((IParseTreeNode)elem); }
    public AxisIntervalList(AxisIntervalElement elem, AxisIntervalList next) { super((IParseTreeNode)elem, (List)next); }
}