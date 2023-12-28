#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

enum TokenType {
    CIN_TOKEN, COUT_TOKEN, CIN_OP_TOKEN, COUT_OP_TOKEN, LPAREN_TOKEN, RPAREN_TOKEN, PLUS_TOKEN, MULT_TOKEN, IDENTIFIER, INT_TOKEN, REAL_TOKEN,
    ERROR_TOKEN, END_SOURCE_TOKEN
};

class TokenParser {
private:
    ifstream file;
    TokenType currentToken;

    TokenType checkReserved(string str) {
        if (str == "cin") return CIN_TOKEN;
        else if (str == "cout") return COUT_TOKEN;
        else return IDENTIFIER;
    }

    TokenType getToken() {
        char ch;
        string str;
        int state = 0;

        while (state >= 0 && state <= 11 && !file.eof()) {
            switch (state) {
                case 0:
                    file.get(ch);
                    if (file.eof()) return END_SOURCE_TOKEN;
                    else if (isspace(ch)) { str = ch; state = 0; }
                    else if (isdigit(ch)) { str = ch; state = 1; }
                    else if (ch == '.') { str = ch; state = 2; }
                    else if (isalpha(ch)) { str = ch; state = 4; }
                    else if (ch == '<') state = 5;
                    else if (ch == '>') state = 6;
                    else if (ch == '+') state = 7;
                    else if (ch == '*') state = 8;
                    else if (ch == '(') state = 9;
                    else if (ch == ')') state = 10;
                    else state = 11;
                    break;

                case 1:
                    file.get(ch);
                    if (isdigit(ch)) { str += ch; state = 1; }
                    else if (ch == '.') state = 3;
                    else { file.putback(ch); return INT_TOKEN; }
                    break;

                case 2:
                    file.get(ch);
                    if (isdigit(ch)) { str += ch; state = 2; }
                    else {
                        file.putback(ch);
                        return REAL_TOKEN;
                    }
                    break;

                case 3:
                    file.get(ch);
                    if (isdigit(ch)) { str += ch; state = 3; }
                    else {
                        file.putback(ch);
                        return REAL_TOKEN;
                    }
                    break;

                case 4:
                    file.get(ch);
                    if (isalnum(ch)) { str += ch; state = 4; }
                    else {
                        file.putback(ch);
                        return checkReserved(str);
                    }
                    break;

                case 5:
                    file.get(ch);
                    if (ch == '<') return COUT_OP_TOKEN;
                    else return ERROR_TOKEN;
                    break;

                case 6:
                    file.get(ch);
                    if (ch == '>') return CIN_OP_TOKEN;
                    else return ERROR_TOKEN;
                    break;

                case 7:
                    return PLUS_TOKEN;
                    break;

                case 8:
                    return MULT_TOKEN;
                    break;

                case 9:
                    return LPAREN_TOKEN;
                    break;

                case 10:
                    return RPAREN_TOKEN;
                    break;

                case 11:
                    return ERROR_TOKEN;
                    break;
            }
        }
        return END_SOURCE_TOKEN;
    }

    string getTokenName(TokenType t) {
        switch (t) {
            case CIN_TOKEN: return "cin token"; break;
            case COUT_TOKEN: return "cout token"; break;
            case CIN_OP_TOKEN: return ">> token"; break;
            case COUT_OP_TOKEN: return "<< token"; break;
            case IDENTIFIER: return "identifier"; break;
            case INT_TOKEN: return "int token"; break;
            case REAL_TOKEN: return "real token"; break;
            case PLUS_TOKEN: return "+ token"; break;
            case MULT_TOKEN: return "* token"; break;
            case LPAREN_TOKEN: return "( token"; break;
            case RPAREN_TOKEN: return ") token"; break;
            case ERROR_TOKEN: return "error token"; break;
            case END_SOURCE_TOKEN: return "End of file"; break;
        }
        return "error token";
    }

public:
    TokenParser(string filename) {
        file.open(filename.c_str());
    }

    ~TokenParser() {
        file.close();
    }

    void match(TokenType terminal) {
        if (currentToken == terminal)
            cout << getTokenName(currentToken) << " is matched" << endl;
        else
            syntaxError(currentToken);
        currentToken = getToken();
    }

    void syntaxError(TokenType terminal) {
        cout << getTokenName(terminal) << " has an syntax error \n";
    }

    void sampleParser() {
        currentToken = getToken();
        parseProgram();
        match(END_SOURCE_TOKEN);
    }

    void parseProgram() {
        if (currentToken == CIN_TOKEN) {
            match(CIN_TOKEN);
            match(CIN_OP_TOKEN);
            parseExpressionSeq();
        }
        else if (currentToken == COUT_TOKEN) {
            match(COUT_TOKEN);
            match(COUT_OP_TOKEN);
            parseExpressionSeq();
        }
        else
            syntaxError(currentToken);
    }

    void parseExpressionSeq() {
        parseExpression();
        while (currentToken == IDENTIFIER || currentToken == INT_TOKEN || currentToken == REAL_TOKEN || currentToken == LPAREN_TOKEN || currentToken == RPAREN_TOKEN) {
            parseExpression();
        }
    }

    void parseExpression() {
        switch (currentToken) {
            case IDENTIFIER: match(IDENTIFIER); break;
            case INT_TOKEN: match(INT_TOKEN); break;
            case REAL_TOKEN: match(REAL_TOKEN); break;
            case LPAREN_TOKEN:
                match(LPAREN_TOKEN);
                if (currentToken == PLUS_TOKEN) {
                    match(PLUS_TOKEN);
                    parseExpressionSeq();
                    match(RPAREN_TOKEN);
                }
                else if (currentToken == MULT_TOKEN) {
                    match(MULT_TOKEN);
                    parseExpressionSeq();
                    match(RPAREN_TOKEN);
                }
                else {
                    syntaxError(currentToken);
                }
                break;
        }
    }
};

int main() {
    string filename;
    cout << "Enter File Name:"; cin >> filename;
    TokenParser tokenParser(filename);
    tokenParser.sampleParser();
    return 0;
}



