//
//  ASTree.cpp
//  Abstract Syntax Tree
//
//  Created by Jonathan Maletic
//  Copyright 2023 Kent State University. All rights reserved.
//  Spring 2023
//  srcML 1.0
//
//  Modified by: Andrew Cash
//
//

#include "ASTree.hpp"


// Copy constructor for srcML
//
srcML::srcML(const srcML& actual) {
    header = actual.header;
    if (actual.tree)
        tree   = new AST(*(actual.tree));
    else
        tree = 0;
}

// Constant time swap for srcML
//
void srcML::swap(srcML& b) {
    std::string t_header = header;
    header = b.header;
    b.header = t_header;
    
    AST *temp = tree;
    tree = b.tree;
    b.tree = temp;
}

// Assignment for srcML
//
srcML& srcML::operator=(srcML rhs) {
    swap(rhs);
    return *this;
}

// Reads in and constructs a srcML object.
//
std::istream& operator>>(std::istream& in, srcML& src){
    char ch;
    if (!in.eof()) in >> ch;
    src.header = readUntil(in, '>');
    if (!in.eof()) in >> ch;
    if (src.tree) delete src.tree;
    src.tree = new AST(category, readUntil(in, '>'));
    src.tree->read(in);
    return in;
}

// Prints out a srcML object
//
std::ostream& operator<<(std::ostream& out, const srcML& src){
    if (src.tree) src.tree->print(out);
    return out;
}

//  Adds in the includes and profile object declarations
//
void srcML::mainHeader(const std::vector<std::string>& profileName,
                       const std::vector<std::string>& fileName    ) {
    tree->mainHeader(profileName, fileName);
}

//  Adds in the includes and declares profile object declarations
//
void srcML::fileHeader(const std::string& profileName) {
    tree->fileHeader(profileName);
}

// Adds in the report to the main
//
void srcML::mainReport(const std::vector<std::string>& profileName) {
        tree->mainReport(profileName);
}

///  Inserts a profile.count() into each function body.
//
void srcML::functionCount(const std::string& profileName) {
    tree->functionCount(profileName);
}

// Inserts a profile.count() for each statement.
//
void srcML::lineCount(const std::string& profileName) {
    tree->lineCount(profileName);
}

    

//
//AST::
//


// Constructs a category, token, or whitespace node for the tree.
//
AST::AST(nodes t, const std::string& s) {
    nodeType = t;
    switch (nodeType) {
        case category:
            tag = s;
            break;
        case token:
            text = unEscape(s);
            break;
        case whitespace:
            text = s;
            break;
    }
}


// Destructor for AST
//
AST::~AST() {
    //TODO: IMPLEMENT
    //Recursively traverse tree and delete from bottom up
    if (!child.empty()){ //not empty;
        for (std::list<AST*>::iterator it = child.begin(); it != child.end(); ++it){
            delete *it;
        }
    }
}


// Copy Constructor for AST
//
AST::AST(const AST& actual) {
    //TODO: IMPLEMENT
    //Recursively traverse actual and
    //make a copy of each node putting it
    //into this.
    //first: copy all data
    nodeType = actual.nodeType;
    tag = actual.tag;
    closeTag = actual.closeTag;
    text = actual.text;
    //now: check for subtrees. if the AST has subtrees, you need to create them with the recursive copy constructor
    if (!actual.child.empty()){ //not empty; subtrees need to be created
        for (std::list<AST*>::const_iterator it = actual.child.begin(); it != actual.child.end(); ++it) { //goes through all subtrees
            AST* temp = new AST(*(*it)); //creates a pointer to an AST. Uses recursive copy constructor.
            child.push_back(temp); //pushes a pointer to an AST into the actual list.
        }
    }
}


// Constant time swap for AST
//
void AST::swap(AST& rhs) {
    //TODO: IMPLEMENT
    //Swap all the top level childern (pointers to AST)
    nodes nodetemp = nodeType;
    nodeType = rhs.nodeType;
    rhs.nodeType = nodetemp;

    std::string strTemp = tag;
    tag = rhs.tag;
    rhs.tag = strTemp;

    strTemp = closeTag;
    closeTag = rhs.closeTag;
    rhs.closeTag = strTemp;

    strTemp = text;
    text = rhs.text;
    rhs.text = strTemp;

    child.swap(rhs.child);
}

/// Assignment for AST
//
AST& AST::operator=(AST rhs) {
    swap(rhs);
    return *this;
}


// REQUIRES: this->nodetype == category && tagName is valid srcML tag
// ENSURES: RETVAL == this->child[i] where this->child[i]->tag == tagName
//
// IMPORTANT for milestone 2 and 3
//
AST* AST::getChild(std::string tagName) {
    std::list<AST*>::iterator ptr = child.begin();
    while (((*ptr)->tag != tagName) && (ptr != child.end())) {
         ++ptr;
    }
    return *ptr;
}


// REQUIRES: this->tag == "name"
// Returns the full name of a <name> node
//  There are two types of names in srcML.  A simple name (e.g., foo) and a
//   name with a scope (e.g., std::bar).  This returns the correct
//   one from <name> AST node.
//
// IMPORTANT for milestone 3
//
std::string AST::getName() const {
    std::string result;
    if (child.front()->tag != "name") {
        result = child.front()->text;   //A simple name (e.g., main)
    } else {                            //A complex name (e.g., stack::push).
        result = child.front()->child.front()->text;
        result += "::";
        result += child.back()->child.front()->text;
    }
    return result;
}


//  Adds above the main, in the main file:
//  1. #include "profile.hpp"
//  2. All needed profile object declarations
//      example: profile foo_cpp("foo.cpp");
//
void AST::mainHeader(const std::vector<std::string>& profileName,
                     const std::vector<std::string>& fileName     ) {

    //TODO: IMPLEMENT
    //Skip down a couple lines or find main and put it before it
    //Add a node with #include "profile.hpp"
    //For each file profile name, add a node with a profile
    //   declaration "profile foo_cpp("foo.cpp");"

    AST* temp = new AST(token, "#include \"profile.hpp\"");
    AST* temp2 = new AST(whitespace, "\n");
    std::list<AST*>::const_iterator it = child.begin();
    for (int j = 0; j < 1; ++j){
        ++it;
    }
    child.insert(it, temp2);
    child.insert(it, temp);

    AST* temp5 = new AST(whitespace, "\n");
    child.insert(it, temp5);
    std::string tempStr;
    for (size_t i = 0; i  < fileName.size(); ++i){
        tempStr = "profile " + profileName[i] + "(\"" + fileName[i] + "\");";
        AST* temp3 = new AST(token, tempStr);
        child.insert(it, temp3);
        AST* temp4 = new AST(whitespace, "\n");
        child.insert(it, temp4);
    }
}


//  Adds in each file (except main file):
//  1. #include "profile.hpp"
//  2. All needed external profile object declarations
//      example: extern profile thisfile_cpp;
//
void AST::fileHeader(const std::string& profileName) {

    //TODO: IMPLEMENT
    //Skip down a couple lines or find first function and put it before it
    //Add #include "profile.hpp"
    //Add in the external declaration for that file "extern profile foo_cpp;"
    AST* temp = new AST(token, "#include \"profile.hpp\"");
    AST* temp2 = new AST(whitespace, "\n");
    std::list<AST*>::const_iterator it = child.begin();
    for (int i = 0; i < 1; ++i){
        ++it;
    }
    child.insert(it, temp2);
    child.insert(it, temp);

    AST* temp3 = new AST(token, ("extern profile " + profileName + ";"));
    AST* temp4 = new AST(whitespace, "\n");
    child.insert(it, temp4);
    child.insert(it, temp3);
    AST* temp5 = new AST(whitespace, "\n");
    child.insert(it, temp5);
}


// Adds in the report to the main file
// Assumes only one return in the main
//
void AST::mainReport(const std::vector<std::string>& profileName) {
    
    //TODO: IMPLEMENT
    //Find the function with name of "main" in the file
    // -Get the "block" node of this function
    // -Set an iterator to the child.end() of "block"
    // -Decrement it once (to skip the "}")
    // -Search backwards for a "return" tag
    // -Insert the report statements before the return

    //insert?
    //std::cerr << "Searching for main function ::: \n";
    auto findMain = child.begin();
    bool isMainFound = false;
    while (findMain != child.end() && !isMainFound){
        if ((*findMain)->tag == "function"){
            std::string FunctionName = (*findMain)->getChild("name")->getName();
            //std::cerr << "Found Function. Name: " << FunctionName << "\n";
            if (FunctionName == "main"){
                isMainFound = true;
            } else ++findMain;
        } else ++findMain;
    } //findMain will be a pointer towards the <AST*> that is the function "main"


    AST* blockThing = (*findMain)->getChild("block");
    AST* blockContentThing = blockThing->getChild("block_content");

    //insert?
    auto it = blockContentThing->child.end(); //child.insert(it, new AST(token, nameThing->getName());
    --it;

    //std::cerr << "Searching for return tag::: \n";

    while (it != blockContentThing->child.begin() && (*it)->tag != "return"){
        --it;
        //std::cerr << "Tag: " << (*it)->tag << "\n";
    }



    for (size_t i = 0; i < profileName.size(); ++i){
        child.insert(it, new AST(whitespace, "\n"));
        child.insert(it, new AST(token, "    std::cout << " + profileName[i] + " << std::endl;"));
    }
    child.insert(it, new AST(whitespace, "\n    "));

}


// Adds a node to the function block to count the times each function
//  is executed
//  Assumes no nested functions
//
void AST::functionCount(const std::string& profileName) {
    
    //TODO: IMPLEMENT
    // for each child in the "unit"
    //     if child is a function, constructor, destructor
    //        Find the function name (use AST::getName())
    //        Find <block>, then find <block_content>
    //        Insert the count as first child in <block_content>
    //        Example: main1_cpp.count(__LINE__, "main");
    for (std::list<AST*>::const_iterator it = child.begin(); it != child.end(); ++it){
        if ((*it)->tag == "function" || (*it)->tag == "constructor" || (*it)->tag == "destructor"){
            //std::cerr << "Finding func name: \n";
            std::string FunctionName = (*it)->getChild("name")->getName();
            //std::cerr << "func name: " << FunctionName << "\n";
            //std::cerr << "getting blockthing: ";
            AST* blockThing = (*it)->getChild("block");
            //std::cerr << "got block thing. getting blockcontent thing";
            AST* blockContentThing = blockThing->getChild("block_content");
            //std::cerr << "Attempting to insert things";
            blockContentThing->child.insert(blockContentThing->child.begin(), new AST(whitespace, "\n"));
            blockContentThing->child.insert(blockContentThing->child.begin(), new AST(token, " " + profileName + ".count(__LINE__, \"" + FunctionName + "\");"));
        }
    }

}


// Adds in a node to count the number of times each statement is executed
//   Do not count breaks, returns, throw, declarations, etc.
//   Assumes all construts (for, while, if) have an explicit block { }
//
void AST::lineCount(const std::string& profileName) {
    
    //TODO: IMPLEMENT
    // Recursively check for expr_stmt within all category nodes <block>
    // Very similar to AST::print
    // For each child:
    //   If it is a category node
    //       If it is a expr_stmt insert a count after it
    //          Example: foo_cpp.count(__LINE__);
    //       Else call lineCount if not a stop tag  See isStopTag()
    //   Else (token or whitespace) do nothing

    for (std::list<AST*>::const_iterator i = child.begin(); i != child.end(); ++i){
        if ((*i)->nodeType == category){
            if ((*i)->tag == "expr_stmt"){
                child.insert(++i, new AST(token, " " + profileName + ".count(__LINE__);"));
            }
            else if ((*i)->tag == "condition"){
                AST* withinConditional = (*i)->getChild("expr");
                withinConditional->child.insert((withinConditional->child.begin()), new AST(token, profileName + ".conditionCount(__LINE__)" + " && ")); ///////////////////////EDIT HERE RECENT, child.begin
            }
            else{
                if (!isStopTag((*i)->tag)){
                    (*i)->lineCount(profileName);
                }
            }
        }
    }
}

//Explanation for honors challenge:
//From discrete mathmatics, any boolean x && true is simply equal to x.
//You could not simply put the line count in the loop, as the amount of loops is not equal to the amount of times the conditional statement is run.
//Instead, I created a boolean function conditionalCount in profile.hpp that does that same thing as void count, except this returns true.
//By inserting  profileName + ".conditionCount(__Line__)" + " && " after every conditional statement, I effectively call the count function within the conditional statement without changing it. 

// Returns TRUE if the tag (syntactic category) is not to be profiled
//
// IMPORTANT for milestone 3
//
bool isStopTag(std::string tag) {
    //if (tag == "condition"             ) return true; //Remove for challenge
    if (tag == "type"                  ) return true;
    if (tag == "name"                  ) return true;
    if (tag == "return"                ) return true;
    if (tag == "break"                 ) return true;
    if (tag == "continue"              ) return true;
    if (tag == "parameter_list"        ) return true;
    if (tag == "decl_stmt"             ) return true;
    if (tag == "argument_list"         ) return true;
    if (tag == "init"                  ) return true;
    if (tag == "cpp:include"           ) return true;
    if (tag == "macro"                 ) return true;
    if (tag == "comment type=\"block\"") return true;
    if (tag == "comment type=\"line\"" ) return true;
    return false;
}


// Print an AST
// Preorder traversal that prints out leaf nodes only (tokens & whitesapce)
//
std::ostream& AST::print(std::ostream& out) const {
    for (std::list<AST*>::const_iterator i = child.begin(); i != child.end(); ++i) {
        if ((*i)->nodeType != category)
            out << (*i)->text;   //Token or whitespace node, print it
        else
            (*i)->print(out);    //Category node, recursive call
    }
    return out;
}


// Read in and construct AST
// REQUIRES: '>' was previous charater read 
//           && this == new AST(category, "TagName")
//
std::istream& AST::read(std::istream& in) {
    AST *subtree;
    std::string temp, Lws, Rws;
    char ch;
    if (!in.eof()) in.get(ch);
    while (!in.eof()) {
        if (ch == '<') {                      //Found a tag
            temp = readUntil(in, '>');
            if (temp[0] == '/') {
                closeTag = temp;
                break;                        //Found close tag, stop recursion
            }
            subtree = new AST(category, temp);               //New subtree
            subtree->read(in);                               //Read it in
            in.get(ch);
            child.push_back(subtree);                        //Add it to child
        } else {                                             //Found a token
            temp = std::string(1, ch) + readUntil(in, '<');  //Read it in.
            std::vector<std::string> tokenList = tokenize(temp);
            for (std::vector<std::string>::const_iterator i=tokenList.begin();
                 i != tokenList.end();
                 ++i) {
                if (isspace((*i)[0])) {
                    subtree = new AST(whitespace, *i);
                } else {
                    subtree = new AST(token, *i);
                }
                child.push_back(subtree);
            }
            ch = '<';
        }
    }
    return in;
}



// Utilities for AST::read()

// Reads until a key is encountered.  Does not include ch.
// REQUIRES: in.open()
// ENSURES: RetVal[i] != key for all i.
std::string readUntil(std::istream& in, char key) {
    std::string result;
    char ch;
    in.get(ch);
    while (!in.eof() && (ch != key)) {
        result += ch;
        in.get(ch);
    }
    return result;
}

// Converts escaped XML charaters back to charater form
// REQUIRES: s == "&lt;"
// ENSURES:  RetVal == "<"
std::string unEscape(std::string s) {
    std::size_t pos = 0;
    while ((pos = s.find("&gt;"))  != s.npos) { s.replace(pos, 4, ">"); }
    while ((pos = s.find("&lt;"))  != s.npos) { s.replace(pos, 4, "<"); }
    while ((pos = s.find("&amp;")) != s.npos) { s.replace(pos, 5, "&"); }
    return s;
}

// Given: s == "   a + c  "
// RetVal == {"   ", "a", " ", "+", "c", " "}
std::vector<std::string> tokenize(const std::string& s) {
    std::vector<std::string> result;
    std::string temp = "";
    unsigned i = 0;
    while (i < s.length()) {
        while (isspace(s[i]) && (i < s.length())) {
            temp.push_back(s[i]);
            ++i;
        }
        if (temp != "") {
            result.push_back(temp);
            temp = "";
        }
        while (!isspace(s[i]) && (i < s.length())) {
            temp.push_back(s[i]);
            ++i;
        }
        if (temp != "") {
            result.push_back(temp);
            temp = "";
        }
    }
    return result;
}
    

