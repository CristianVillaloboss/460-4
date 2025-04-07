#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <stack>
#include <vector>
#include "Graph.h"
#include <sstream>
#include "SymbolTable.h"
#include <stack>
using namespace std;


int removeComments(int fileName) {
    std::ifstream file("TEST CASES/programming_assignment_3-test_file_" + to_string(fileName) + ".c" );
    //std::ifstream file2("TEST CASES/project2Test1.c");
    std::ofstream outfile("output.txt");

    if (!file) {
        std::cerr << "Error opening file!\n";
        return 1;
    }

    string line;
    char word;
    bool inBlockComment = false;  //    `/* ... */` comments
    bool inLineComment = false;   //    `//` comments
    char r1, i1, n1, t1, f1;
    int lineNumber =  1;
    stack<pair<string,int> > balancedStack;

    int openCount = 0;
    int closeCount = 0;

    while (file.get(word)) {
        if (!inBlockComment && !inLineComment) {
            if (word == '/') {  // Possible start of a comment
                char nextChar = file.peek();
                if (nextChar == '*') {  // Found "/*"

                    openCount++; // push open tag onto stack along with line number
                    pair<string,int> mom;
                    mom.first = "open";
                    mom.second = lineNumber;
                    balancedStack.push(mom);

                    inBlockComment = true;
                    outfile << "  ";  // replace '/' and '*' with spaces
                    file.get(word);
                    continue;
                } else if (nextChar == '/') {  // found "//"
                    inLineComment = true;
                    outfile << "  ";
                    file.get(word);
                    continue;
                }
            }
            if(word == 'p') { //make sure we print a printf.
                file.get(r1); //gets the next 5 chars. will put back if they are not printf.
                file.get(i1);
                file.get(n1);
                file.get(t1);
                file.get(f1);

                if( r1 == 'r' && i1 == 'i' && n1 == 'n' && t1 == 't' && f1 == 'f') {
                    getline(file, line, ';');
                    outfile << "printf" + line + ';';
                    file.get(word);
                }
                else {
                    file.putback(f1);
                    file.putback(t1);
                    file.putback(n1);
                    file.putback(i1);
                    file.putback(r1);
                }

            }
            if(word == '\n')
                lineNumber++;

            outfile << word;  // prints non-comment characters

        }
        else if (inBlockComment) {  // we see block comments /* ... */
            if (word == '\n') {
                outfile << '\n';
                lineNumber++;
            } else {
                outfile << ' ';  // replace comments with space
            }

            if (word == '*') {
                if (file.peek() == '/') {  // Found "*/"

                    closeCount++; //pushes closed tags onto the stack
                    pair<string,int> mom;
                    mom.first = "close";
                    mom.second = lineNumber;
                    balancedStack.push(mom);

                    file.get(word);  // consumes '/'
                    outfile << ' ';  // replaces '/' with space
                    inBlockComment = false;
                }
            }
        }
        if (word == '*' && file.peek() == '/') {
            closeCount++; //pushes closed tags onto the stack
            pair<string,int> mom;
            mom.first = "close";
            mom.second = lineNumber;
            balancedStack.push(mom);
        }
        else if (inLineComment) {  // we see single-line comments
            if (word == '\n') {  // end of `//` comment
                lineNumber++;
                inLineComment = false;
                outfile<< '\n';
            } else {
                outfile << ' ';  // replace comment content with spaces
            }
        }
    }
    if(openCount == closeCount) { //this means there are no errors coming from unclosed tags.
        file.close();
        return 0;
    }
    outfile.close();

    //checking where the error occurs
    if(balancedStack.top().first == "open") { //unclosed tag towards end of file
        std::ofstream outfile("output.txt", std::ios::trunc); // (clear file)
        outfile<< "ERROR: Program contains C-style, unterminated comment on line " << balancedStack.top().second << std::endl;
        //cout << "check output.txt" << endl;
        return 1;
    }
    for(int i = 0; i < balancedStack.size() - 1; ++i) {
        int tempLineNumber = balancedStack.top().second;
        if (balancedStack.top().first == "close") {
            balancedStack.pop();
            if(balancedStack.top().first == "open") {
                balancedStack.pop();
            }
            else {
                std::ofstream outfile("output.txt", std::ios::trunc); // (clear file)
                outfile << "ERROR: Program contains C-style, unterminated comment on line " << tempLineNumber << endl;
                //cout << "check output.txt" << endl;
                return 1;
            }
        }
    }
    file.close();
    return 0;
}

int tokenize (int fileName) {
    bool isNewline = false;
    std::ofstream outfile1("output1.txt");
    std::ofstream outfile2("symbolTableInputFile.txt");
    int fileToRemove;
    fileToRemove = fileName;
    cout << "\n";

    int removeCmments = removeComments(fileToRemove);

    if(removeCmments == 1){
        cout << "error in removing comments. check output.txt\n";
        return 1;
    }
    //cout << "comments removed\n\n";
    char word;
    std::ifstream file("output.txt");

    vector <string> STRING;
    //..Type.. //name
    std::tuple<std::string, std::string, int> token;

    //pair <string,string> token;
    vector<tuple<string,string,int>> tokenVector;


    string getString;
    int lineNumber = 1;
    while (file.get(word)) {
        if (word == '\n') {
            lineNumber++;
        }
        if(word == '&') {
            if(file.peek() != '&') {
                token = std::make_tuple("ADDRESS", "&", lineNumber);
                tokenVector.push_back(token);
            }
            if(file.peek() == '&') {
                file.get(word);
                token = std::make_tuple("BOOLEAN_AND", "&&", lineNumber);
                tokenVector.push_back(token);
            }

        }
        if(word == '%') {
            token = std::make_tuple("MODULO", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '[') {
            token = std::make_tuple("L_BRACKET", word, lineNumber);
            tokenVector.push_back(token);

            if(file.peek() == '-') {
                cout << "Syntax error on line "<<lineNumber
                << ": array declaration size must be a positive integer.\n";
                exit(0);
            }
        }
        if(word == ']') {
            token = std::make_tuple("R_BRACKET", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '(') {
            token = std::make_tuple("L_PAREN", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == ')') {
            token = std::make_tuple("R_PAREN", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '{') {
            token = std::make_tuple("L_BRACE", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '}') {
            token = std::make_tuple("R_BRACE", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '=') {
            if(file.peek()!= '=') {
                token = std::make_tuple("ASSIGNMENT_OPERATOR", word, lineNumber);
                tokenVector.push_back(token);
            }
            if(file.peek()== '=') {
                file.get(word);
                token = std::make_tuple("BOOLEAN_EQUAL", "==", lineNumber);
                tokenVector.push_back(token);
            }
        }
        if(word == ',') {
            token = std::make_tuple("COMMA", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == ';') {
            token = std::make_tuple("SEMICOLON", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '-') {
            if(!isdigit(file.peek())) {
                token = std::make_tuple("MINUS", word, lineNumber);
                tokenVector.push_back(token);
            }
            string tempNum = "-";
            if(isdigit(file.peek())) {
                file.get(word);
                while(isdigit(word)) {
                    tempNum += word;
                    file.get(word);
                }
                token = std::make_tuple("INTEGER", tempNum, lineNumber);
                tokenVector.push_back(token);
                file.putback(word); //should put back whatever we took that doesnt belond to an int
            }

        }
        if(word =='!') {
            token = std::make_tuple("NOT", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '+') {
            token = std::make_tuple("PLUS", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '*') {
            token = std::make_tuple("ASTERISK", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '/') {
            token = std::make_tuple("DIVIDE", word, lineNumber);
            tokenVector.push_back(token);
        }
        if(word == '<') {
            if(file.peek()!= '=') {
                token = std::make_tuple("LT", word, lineNumber);
                tokenVector.push_back(token);
            }
            if(file.peek()== '=') {
                file.get(word);
                token = std::make_tuple("LT_EQUAL", "<=", lineNumber);
                tokenVector.push_back(token);
            }
        }
        if(word == '>') {
            if(file.peek()!= '=') {
                token = std::make_tuple("GT", word, lineNumber);
                tokenVector.push_back(token);
            }
            if(file.peek()== '=') {
                file.get(word);
                token = std::make_tuple("GT_EQUAL", ">=", lineNumber);
                tokenVector.push_back(token);
            }
        }


        if(isdigit(word)) {
            if(isalpha(file.peek())) {
                char tmp = file.peek();
                string error;
                error = error + word + tmp;
                cout << "Syntax error on line " << lineNumber << ": "
                     <<"invalid integer\n";
                outfile1 << "Syntax error on line " << lineNumber << ": "
                         <<"invalid integer\n";

                exit(2);
            }
            string tempString;
            while(isdigit(word)) {
                tempString += word;
                file.get(word);
            }
            token = std::make_tuple("INTEGER", tempString, lineNumber);
            tokenVector.push_back(token);
            file.putback(word);

        }
        if(word == '"') {
            token = std::make_tuple("DOUBLE_QUOTE", word, lineNumber);
            tokenVector.push_back(token);
            char temp;
            temp = file.peek();
            if (temp != '"' ) {
                getline(file, getString, '\"');
                for(int i = 0; i < getString.size(); ++i) {
                    char x = getString.at(i);
                   // char yy = getString.at(i+1);
                    if(getString.at(i) == '\\') {
                       //cout << "we are a terminator";
                       char o = getString.at(getString.size() -1);
                        if(getString.at(getString.size() -1) == '\\') {
                            cout << "Syntax error on line "<< lineNumber << ": unterminated string quote.\n";
                            exit(0);
                        }
                        //if(getString.at(i+1)!= 'n' || getString.at(i+1)!= 'x') {
                           // cout << "syntax errorr: unterminated string quote.";
                         //   exit(0);
                       // }
                    }
                }
                token = std::make_tuple("STRING", getString, lineNumber);
                tokenVector.push_back(token);
                getString = "";
                token = std::make_tuple("DOUBLE_QUOTE", word, lineNumber);
                tokenVector.push_back(token);
            }
        }
        if(word == '\'') {
            token = std::make_tuple("SINGLE_QUOTE", word, lineNumber);
            tokenVector.push_back(token);
            string getString;

            getline(file, getString, '\'');
            token = std::make_tuple("CHAR_STRING", getString, lineNumber);
            tokenVector.push_back(token);
            getString = "";

            token = std::make_tuple("SINGLE_QUOTE", word, lineNumber);
            tokenVector.push_back(token);
        }

        if (isalpha(word) ) {
            getString = word;
            while (file.get(word)) {
                if (word == ' ' || word == '(' || word == ')' || word == ','
                    || word == ';' || word == '['|| word == ']'|| word == '{' || word == '\n') {
                    break;
                }
                getString += word;
            }
            token = std::make_tuple("IDENTIFIER", getString, lineNumber);
            tokenVector.push_back(token);
            file.putback(word);
            getString = "";

        }
        //now we are at either at " or a ( or {

    }
    std::ifstream file3("project2Test1.c");

    //cout << "\nToken list:\n\n";
    //outfile1 << "\nToken list:\n\n";
    for (int i = 0; i < tokenVector.size(); ++i) { //vector of pairs
        outfile1 << std::get<0>(tokenVector.at(i)) << "\n"; // first element
        outfile1 << std::get<1>(tokenVector.at(i)) << "\n"; // second element
        outfile2 << std::get<1>(tokenVector.at(i)) << "\n"; // second element
        //cout << "Token type: "<<tokenVector.at(i).first << "\n";
        //cout << "Token:      " <<tokenVector.at(i).second << "\n\n";
    }
    //cout << "assignment 2 output is now stored in output1.txt";
    //cout << "\n";
    //outfile1 << "\n";
    //checks for errors like char char
    for(int i =0; i < tokenVector.size(); ++i) {
        if (std::get<1>(tokenVector[i]) == "char") {
            if (std::get<1>(tokenVector[i+1]) == "char") {
                cout << "Syntax error on line "<< get<2>(tokenVector.at(i))<<": reserved word \"char\" cannot be used for the name of a\n"
                                                    "variable.\n";
                exit(0);
            }
            if(std::get<1>(tokenVector[i+1]) == "void") {
                cout << "Syntax error on line "<< get<2>(tokenVector.at(i)) <<": reserved word \"void\" cannot be used for the name of a\n"
                        "variable.\n";

                exit(0);
            }
            if(std::get<1>(tokenVector[i+1]) == "int") {
                cout << "Syntax error on line "<< get<2>(tokenVector.at(i)) <<": reserved word \"int\" cannot be used for the name of a variable.\n";
                        exit(0);
            }
        }
        if (std::get<1>(tokenVector[i]) == "function") {
            if (std::get<1>(tokenVector[i+1]) == "int") {
                if (std::get<1>(tokenVector[i+2]) == "printf") {
                    cout << "Syntax error on line "<< get<2>(tokenVector.at(i)) << ": reserved word \"printf\" cannot be used for the name of a function.\n";
                    exit(0);
                   }
                }

            }
        }

    return 0;
}
//TODO  7,
int cst () { //1, 2, 3, 4, 5, 6, 8, 9, 10

    int z;
    int y;
    cout << "enter the number (1 - 10) of the file you want to use as input\n";
    cin >> z;
    y = tokenize(z);
    ifstream file("output1.txt");

    string type;
    string name;
    getline(file,type) && getline(file,name);
    TreeNode* rootBlock = new TreeNode(name,type);

    Graph LCRS(rootBlock);
    bool addChildAgain = false;
    while(getline(file,type) && getline(file,name)) {
        TreeNode* nodee = new TreeNode(name, type);
        if(nodee->getName() == ";") {
            LCRS.addSibling(nodee);
            getline(file,type) && getline(file,name);
            TreeNode* node3 = new TreeNode(name, type);
            LCRS.addChild(node3);
        }
        else if(nodee->getName() == "}" || nodee->getName() == "{") {
            LCRS.addChild(nodee);
            getline(file,type) && getline(file,name);
            TreeNode* node2 = new TreeNode(name, type);
            LCRS.addChild(node2);
        }
        else{
            LCRS.addSibling(nodee);
        }
    } //8,6, 6, 6, 12, 12
    LCRS.printTree();
        return 0;
//zuri
}
bool isDataType (string word) { //procedure, function, char, int, bool
    vector<string> reserveWords {"procedure", "function", "main","char", "int", "bool"};
    bool flag = false;

    for (int i = 0; i < reserveWords.size(); ++i) {
        if(word==reserveWords.at(i)) {
            flag = true;
        }
    }
    return flag;
}
void fourErrorChecker(string name, int scope, SymbolTable list) {
    if(list.exists(name, scope)){ //already declared at
        if(scope!= 0) {
            cout << "ERROR ON LINE... : variable " << name << " is already defined locally";
            exit(5);
        }
        if(scope== 0) {
            cout << "ERROR ON LINE... : variable " << name << " is already defined globally";
            exit(5);
        }
    }
}
int main () { //symbol table.
    stack<char> pile; // will hold { or } only
    int scopeTracker = 0;
    int scopeTrackerHelper = 0;
    int x;
    x = cst();
    std::string word;
    std::ifstream infile("symbolTableInputFile.txt");
    std::ofstream outfile1("newOutput.txt");
    SymbolTable list;
    string IDENTIFIER_NAME;
    string IDENTIFIER_TYPE;
    string DATATYPE;
    bool DATATYPE_IS_ARRAY;
    int DATATYPE_ARRAY_SIZE = 0;
    int SCOPE = 0;
    string parameterList;
    bool declarationInProgress = false;
    bool decList = false;

    bool nameFlag = false, typeFlag = false, dataTypeFlag = false, dtIsArrFlag = false, dtArrSizeFlag = false, scopeFlag = false;
    bool inParanthesis = false;
    int lineNumber = 0;
    bool inFunctionOrProcess = false;
    string functionName;
    string tempDataType;
    int disableParam = 0;
    while(getline(infile, word, '\n')) {
        if(word == "{") {
            pile.push('{');
        }
        if(word == "}") {
            if (!pile.empty()) {
                pile.pop();
                //inFunctionOrProcess = false;
            }
             if(pile.empty()){
                SCOPE = 0;
                //cout << "im empty!\n";
            }
        }
        lineNumber++;

        if(isDataType(word) && !typeFlag) { //char
            IDENTIFIER_TYPE = word;
            typeFlag = true;
            if(word == "function") {
                inFunctionOrProcess = true;
                IDENTIFIER_TYPE = "function";
                scopeTracker += 1;
                SCOPE = scopeTracker;
                getline(infile, word, '\n');

                DATATYPE = word;
                getline(infile, word, '\n');
                functionName = word; //this is the name of the function which will be stored for param use
                IDENTIFIER_NAME = word;
                dataTypeFlag = true;
                nameFlag = true;
                //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                    if(SCOPE!= 0) {
                        cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                        exit(5);
                    }
                    if(SCOPE== 0) {
                        cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                        exit(5);
                    }
                }

                list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                nameFlag = false, typeFlag = false, dataTypeFlag = false, dtIsArrFlag = false, dtArrSizeFlag = false, scopeFlag = false;
            }
            if (word == "procedure") {
                inFunctionOrProcess = true;
                IDENTIFIER_TYPE = "procedure";
                scopeTracker += 1;
                SCOPE = scopeTracker;
                getline(infile, word, '\n');
                functionName = word; //this is the name of the function which will be stored for param use
                IDENTIFIER_NAME = word;
                nameFlag = true;
                //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                    if(SCOPE!= 0) {
                        cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                        exit(5);
                    }
                    if(SCOPE== 0) {
                        cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                        exit(5);
                    }
                }

                list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                nameFlag = false, typeFlag = false, dataTypeFlag = false, dtIsArrFlag = false, dtArrSizeFlag = false, scopeFlag = false;
            }
            if(word == "int" || word == "char" || word == "bool") { //PARAMETER
                if(inFunctionOrProcess) { //parameter detection is active, CHAR
                    DATATYPE = word;
                    inFunctionOrProcess = false;
                    getline(infile, word, '\n'); //this will be the name
                    IDENTIFIER_NAME = word; //ensity
                    if(getline(infile, word, '\n')) { // ) could be a [ or  , or ; or a )
                            if(word == ",") { //declaration is in progress.
                                //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                                if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                                    if(SCOPE!= 0) {
                                        cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                                        exit(5);
                                    }
                                    if(SCOPE== 0) {
                                        cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                                        exit(5);
                                    }
                                }

                                list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                               bool xx = true;
                               decList = true;
                                while(getline(infile, word, '\n')){//char
                                    if(xx) {
                                        DATATYPE = word;
                                        getline(infile, word, '\n');
                                        xx = false;
                                    }
                                    if(word == ")") {
                                        break;
                                    }
                                    if(word == ",") {
                                        getline(infile, word, '\n');
                                        DATATYPE = word;
                                        getline(infile, word, '\n');
                                        parameterList = "";
                                    }
                                    parameterList = "";
                                    IDENTIFIER_TYPE = "none";
                                    IDENTIFIER_NAME = word;          //adds to list in a declaration in parameters.
                                    //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                                    if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                                        if(SCOPE!= 0) {
                                            cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                                            exit(5);
                                        }
                                        if(SCOPE== 0) {
                                            cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                                            exit(5);
                                        }
                                    }

                                    list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                                }
                            }
                        if(word == "[") { //if word == [
                            DATATYPE_IS_ARRAY = true;
                            getline(infile, word, '\n'); //gets 4096
                            if(word != "]") {
                                DATATYPE_ARRAY_SIZE = stoi(word);
                            }
                            else { //empty square brackets seen...
                                DATATYPE_ARRAY_SIZE = 0;
                            }
                        }
                        if(!DATATYPE_IS_ARRAY && word != ")") {
                            IDENTIFIER_NAME = word; //only do this if it wasnt an array
                        }
                        if(disableParam = 0) {
                            parameterList = functionName;
                        }
                        if(!decList) { //just a regular single parameter.... not a list
                            //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                            if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                                if(SCOPE!= 0) {
                                    cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                                    exit(5);
                                }
                                if(SCOPE== 0) {
                                    cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                                    exit(5);
                                }
                            }

                            list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY,
                                        DATATYPE_ARRAY_SIZE, SCOPE, parameterList);
                            parameterList = "";
                        }
                    }
                }
                //parameter detection is not active.
                else { //char here example...
                    DATATYPE = word; //should remain static!
                    IDENTIFIER_TYPE = "datatype";
                    getline(infile, word, '\n'); //this will be the name
                    IDENTIFIER_NAME = word;
                    string tempWord = word; //will be name
                    getline(infile, word, '\n'); // could be a [ or  , or ;
                    if(word == "[") { //if word == [
                        DATATYPE_IS_ARRAY = true;
                        getline(infile, word, '\n');
                        if(word != "]") {
                            DATATYPE_ARRAY_SIZE = stoi(word);
                        }
                        else { //empty square brackets seen...
                            DATATYPE_ARRAY_SIZE = 0;
                        }
                    }
                    if(word == ",") { //declaration is in progress.
                        IDENTIFIER_NAME = tempWord; //we get the name of the declaration.
                        //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                        if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                            if(SCOPE!= 0) {
                                cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                                exit(5);
                            }
                            if(SCOPE== 0) {
                                cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                                exit(5);
                            }
                        }

                        list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                        while(getline(infile, word, '\n')){
                            if(word == ";") {
                                break;
                            }
                            if(word == ",") {
                                getline(infile, word, '\n');
                            } // char hexnum [9]; , (char hex_digit) param for hexdigit2int
                            IDENTIFIER_NAME = word;
                            //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                            if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                                if(SCOPE!= 0) {
                                    cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                                    exit(5);
                                }
                                if(SCOPE== 0) {
                                    cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                                    exit(5);
                                }
                            }

                            list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                        }
                    }
                    else { //its just a single declaration;
                        //fourErrorChecker(IDENTIFIER_NAME, SCOPE, list);

                        if(list.exists(IDENTIFIER_NAME, SCOPE)){ //already declared at
                            if(SCOPE!= 0) {
                                cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined locally";
                                exit(5);
                            }
                            if(SCOPE== 0) {
                                cout << "ERROR ON LINE... : variable " << IDENTIFIER_NAME << " is already defined globally";
                                exit(5);
                            }
                        }

                        list.insert(IDENTIFIER_NAME, IDENTIFIER_TYPE, DATATYPE, DATATYPE_IS_ARRAY, DATATYPE_ARRAY_SIZE,SCOPE, parameterList);
                    }
                    nameFlag = false, typeFlag = false, dataTypeFlag = false, dtIsArrFlag = false, dtArrSizeFlag = false, scopeFlag = false;
                }

            }
        }

        if(word == ")" || word == "{" || word == ";" || word == "}" || word == "(") {
            if(word == "("){
                //if()
                inParanthesis = true;
                parameterList = IDENTIFIER_NAME;
            }
            if(word == ")"){
                inFunctionOrProcess = false; //no longer in parameters
                inParanthesis = false;
                parameterList = "";
            }
            typeFlag = false;
            //reset the variables
            IDENTIFIER_NAME = IDENTIFIER_TYPE = DATATYPE = "";
            DATATYPE_IS_ARRAY = false;
            DATATYPE_ARRAY_SIZE = 0;
            declarationInProgress = false;
        }
    }
    int size = list.getSize();
    string name1;
    string name2;
    int scope1 = 0;
    int scope2 = 0;
    bool isExist = false;
    //cout << name;
    /*
    for(int i = 1; i <= size; ++i) { //iterating over a list.
        name1 = list.getName(i);
        scope1 = list.getScope(i);
        isExist = list.exists(name1, scope2); //will return the scope of the node
        if(isExist) {
            if(scope2 == 0) {
                cout << "error, " << name1 << " already exists globally\n";
            }
            if(scope2 !=0) {
                cout << "error, " << name1 << " already exists locally\n";
            }
            exit(8);
        }
    }
     */

    list.print();
    return 0;
}