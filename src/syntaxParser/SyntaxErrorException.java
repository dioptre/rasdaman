package syntaxParser;

public class SyntaxErrorException extends Exception {
    private int line;
    private int column;

    public SyntaxErrorException(int line, int column) {
        super("Syntax Error");
        this.line = line;
        this.column = column;
    }
    
    public SyntaxErrorException(int line, int column, String message) {
        super(message);
        this.line = line;
        this.column = column;
    }
    
    public int getLine() {
        return line;
    }

    public int getColumn() {
        return column;
    }
}
