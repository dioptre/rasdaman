package syntaxParser;

class InterpolationMethodList extends List {
    public InterpolationMethodList(FieldInterpolationMethod elem) { super((IParseTreeNode)elem); }
    public InterpolationMethodList(FieldInterpolationMethod elem, InterpolationMethodList next) { super((IParseTreeNode)elem, (List)next); }
}
