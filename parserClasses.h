﻿#ifndef PARSERCLASSES_H_
#define PARSERCLASSES_H_

//Use only the string library DO NOT add any other libraries
#include <string>

using namespace std;

enum tokenType {T_Operator, T_Identifier, T_Literal, T_CommentBody, T_Other};

struct tokenDetails
{
  string type; //boolean, std_logic, std_logic_vector, integer etc.
  int width; //bit width for vector types
};

//Declare your variables for storing delimiters here:
//for literals: check whether the previous token is ' or " or X"10010"(do we need to consider this as a vector?)

//Declare your variables for storing delimiters here:
const string caseOne = "+&;.,|#()-";
const string caseTwoA = "<>=*/:";
const string caseTwoB1 = "<=";
const string caseTwoB2 = "=>";
const string caseTwoB3 = "**";
const string caseTwoB4 = ":=";
const string caseTwoB5 = ">=";
const string caseTwoB6 = "/=";
const string caseThree = "--";
const string caseThreea = "-";
const string caseFive = "\"";
const string caseFour = "qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM";
const string caseBitVector = "xboXBO";
const string caseSingleQuote = "'";
const string letters = "qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM";
const string integers="012345679";
const string Other[7]={",",";",":","(",")","<>"};
const string Operator[28]={"**","abs","not","*","/","mod","rem","+","-","&","sll","srl","sla","sra","rol","ror","=","/=","<","<=",">",">=","and","or","nand","nor","xor","xnor"};
const string keywords[96]={"abs","not","mod","rem","sll","srl","sla","sra","rol","ror","access","and","or","nand","nor","xor","xnor","after","alias","all","architecture","array","assert","attribute","begin","block","body","buffer","bus","case","component","configuration","constant","disconnect","downto","else","elsif","end","entity","exit","file","for","function","generate","generic","group","guarded","if","impure","in","inertial","inout","is","label","library","linkage","literal","loop","map","new","next","null","of","on","open","others","out","package","port","postponed","procedure","process","pure","range","record","register","reject","report","return","select","severity","signal","shared","subtype","then","to","transport","type","unaffected","units","until","use","variable","wait","while","with"};
//CommentBody, just check whether the previous token is --



//Token class for a doubly-linked list of string tokens


class Token {
private:
	Token *next; //Next pointer for doubly linked list
	Token *prev; //Previous pointer for doubly linked list
	string stringRep; //Token value

  bool _isKeyword; //true if token is a reserved keyword
  tokenType type; //enum that holds the type of the token
  tokenDetails *details; //pointer to tokenDetails struct, owned by this token, only valid if type is T_Literal or  is a T_Identifier and is a variable/signal.  Lazy allocation, only allocated when needed (see setTokenDetails function declaration).

	//Allow TokenList class to access Token member variables marked private
  //https://en.wikipedia.org/wiki/Friend_class
	friend class TokenList;

public:
	//Default Constructor, pointers initialized to NULL, and other variable initialization
  //tokenDetails should NOT be allocated here
	Token();

	//Constructor with string initialization
	Token(const string &stringRep);

  //Copy constructor
  Token(const Token &token);

  //Destructor, free any memory owned by this object
  ~Token();

  //Assignment operator
  void operator =(const Token& token);

	//Returns the Token's *next member
	Token* getNext ( ) const {  return next; }

	//Sets the Token's *next member
	void setNext (Token* next ) { this->next = next; }

	//Returns the Token's *prev member
	Token* getPrev ( ) const { return prev; }

	//Sets the Token's *prev member
	void setPrev (Token* prev ){ this->prev = prev; }

	//Returns a reference to the Token's stringRep member variable
	const string& getStringRep ( ) const { return stringRep; }

	//Sets the token's stringRep variable
	void setStringRep (const string& stringRep ) { this->stringRep = stringRep; }

  //Returns true if token is a keyword
  bool isKeyword () const { return _isKeyword; }

  //Sets isKeyword to true
  void setKeyword() { _isKeyword = true; }

  //Returns the token type
  tokenType getTokenType() const { return type; }

  //Set's the token type
  void setTokenType(tokenType type) { this->type = type; }

  //Returns true if token matches this type
  bool isOperator() const { return (type == T_Operator); }
  //Returns true if token matches this type
  bool isIdentifier() const { return (type == T_Identifier); }
  //Returns true if token matches this type
  bool isLiteral() const { return (type == T_Literal); }
  //Returns true if token matches this type
  bool isComment() const { return (type == T_CommentBody); }
  //Returns true if token matches this type
  bool isOther() const { return (type == T_Other); }

  //Returns a pointer to tokenDetails
  tokenDetails* getTokenDetails() const { return details; }

  //Set's the tokenDetails given a string type and optional vector width
  //Allocates tokenDetails if it doesn't already exist
  void setTokenDetails(const string &type, int width = 0);

};

//A doubly-linked list class consisting of Token elements
class TokenList {
private:
	Token *head; //Points to the head of the token list (doubly linked)
	Token *tail; //Points to the tail of the function list (doubly linked)

public:
	//Default Constructor, Empty list with pointers initialized to NULL
	TokenList() : head(nullptr), tail(nullptr) { }

	//Returns a pointer to the head of the list
	Token* getFirst() const { return head; }

	//Returns a pointer to the tail of the list
	Token* getLast() const { return tail; }

	//Creates a new token for the string input, str
	//Appends this new token to the TokenList
	//On return from the function, it will be the last token in the list
	void append(const string &str); //example comment

	//Appends the token to the TokenList if not null
	//On return from the function, it will be the last token in the list
	void append(Token *token);

  //Removes the token from the linked list if it is not null
	//Deletes the token
	//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
	void deleteToken(Token *token);

  //find token details and type and update token.  May require examining properties of neighbouring tokens
  void findAndSetTokenDetails(Token *token);

};

//A class for tokenizing a string of VHDL  code into tokens
class Tokenizer {
private:
	/*State tracking variables for processing a single string*/
	bool complete; //True if finished processing the current string

	size_t offset; //Current position in string
	size_t tokenLength; //Current token length
	string *str; //A pointer to the current string being processed

    bool tracker1, tracker2, comment, comment_tracker, quotation_tracker1, quotation_tracker2;
	//Include any helper functions here
	//e.g. trimming whitespace, comment processing

	//Computes a new tokenLength for the next token
	//Modifies: size_t tokenLength, and bool complete
	//(Optionally): may modify offset
	//Does NOT modify any other member variable of Tokenizer
	void prepareNextToken();

public:
	//Default Constructor- YOU need to add the member variable initializers.
	Tokenizer();

	//Sets the current string to be tokenized
	//Resets all Tokenizer state variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	void setString(string *str);

	//Returns true if all possible tokens have been extracted from the current string (string *str)
	bool isComplete() const { return complete; }

	//Returns the next token. Hint: consider the substr function
	//Updates the tokenizer state
	//Updates offset, resets tokenLength, updates processingABC member variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	string getNextToken();
};


//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList);

//Removes all tokens of the given tokenType
//Returns the number of tokens removed
int removeTokensOfType(TokenList &tokenList, tokenType type);


//Creates a new TokenList, and returns a pointer to this list
//Searches for all conditional expressions in tokenList and appends them to the new list
//Format is as follows:
//Each token that is part of a condtional expression is appended sequentially
//At the end of a conditional expression a newline character is appened
   //Example: if (a = true) then
   //Your list should include "(", "a", "=", "true", ")" and "\n"
//tokenList is NOT modified
TokenList* findAllConditionalExpressions(const TokenList &tokenList);
string strWithLowerCase(Token* token);
int numOfTokens(const TokenList &tokenList);
int numOfConditionalExp(const TokenList &tokenList);
int numOfmissEndif(const TokenList &tokenList, int mode);
int numOfmissThen(const TokenList &tokenList, int mode);
//int numOfTypeMismatch(TokenList &tokenList,int mode);
//int numOfWidthMismatch(TokenList &tokenList);
void compile (TokenList &tokenList);
TokenList* findAllConditionalExpressions2(const TokenList &tokenList);
TokenList* findAllConditionalExpressions3(const TokenList &tokenList);
std::pair <int, int> widthNtypeMismatch(TokenList &tokenList, int mode);
TokenList* transferToLowerCase(const TokenList &tokenList);

#endif /* PARSERCLASSES_H_ */
