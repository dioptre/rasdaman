package syntaxParser;

class FieldInterpolationList extends List {
    public FieldInterpolationList(FieldInterpolationElement elem) { super((IParseTreeNode)elem); }
    public FieldInterpolationList(FieldInterpolationElement elem, FieldInterpolationList next) { super((IParseTreeNode)elem, (List)next); }
}



