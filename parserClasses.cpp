/*
Author: Simon Chen
Last Edit Date: Jan 19, 2016
*/

//Use only the following libraries:

#include "parserClasses.h"

#include <string>

#include <iostream>









//****TokenList class function definitions******


//           function implementations for append have been provided and do not need to be modified




//On return from the function, it will be the last token in the list

//Default Constructor, pointers initialized to NULL, and other variable initialization
  //tokenDetails should NOT be allocated here
Token::Token()
{
	next = nullptr;
	prev = nullptr;
	stringRep = "nothing" ;
	_isKeyword = false;
	type = T_Other;        //?
	details = nullptr;
}

//Constructor with string initialization
Token::Token(const string &stringRep)
{
	next = nullptr;
	prev = nullptr;
	this->stringRep = stringRep ;
	_isKeyword = false ;
	type = T_Other ;
	details = nullptr;
}

//Copy constructor
Token::Token(const Token &token)
 {
	  next = token.next;
	  prev = token.prev;
	  stringRep = token.stringRep;
	  _isKeyword = token._isKeyword;
	  type = token.type;
	  details = token.details;
 }

//Destructor, free any memory owned by this object
Token::~Token()
{
    delete next; //Next pointer for doubly linked list
	delete prev; //Previous pointer for doubly linked list
    //pointer to tokenDetails struct, owned by this token, only valid if type is T_Literal or  is a T_Identifier and is a variable/signal.  Lazy allocation, only allocated when needed (see setTokenDetails function declaration).
    delete details;
    delete []details;
}

  //Assignment operator
void Token::operator =(const Token& token)
{
      next = token.next;
	  prev = token.prev;
	  stringRep = token.stringRep;
	  _isKeyword = token._isKeyword;
	  type = token.type;
	  details = token.details;
}

//Set's the tokenDetails given a string type and optional vector width
  //Allocates tokenDetails if it doesn't already exist
 void Token::setTokenDetails(const string &type, int width)

{
     details = new tokenDetails;
	 details->type = type;
	 details->width = width;

}
//Creates a new token for the string input, str
//Appends this new token to the TokenList
void TokenList::append(const string &str)
{

 Token *token = new Token(str);
 append(token);

}



//Appends the token to the TokenList if not null

//On return from the function, it will be the last token in the list

void TokenList::append(Token *token)
{

 if (!head)
 {

   head = token;

   tail = token;

 }

 else
{

   tail->setNext(token);

   token->setPrev(tail);

   tail = token;

 }

}



void TokenList::findAndSetTokenDetails(Token *token)
{
        if (token==nullptr)
        {
           cout<<"nullptr"<<endl;
        }
        Token *ptr = token;

        //this part is to deal with comment symbol"--" and comment body
        if (ptr->getStringRep() == "--")
		{
			ptr->setTokenType(T_Other);
			ptr->next->setTokenType(T_CommentBody);
			cout<<"type of commentbody BODYBODYBODYBODYBODYBODYBODYBODYBODY"<<ptr->next->getTokenType()<<endl;
			ptr = ptr->next->next;
		}

		//cases other than comment body and comment symbol contain: T_other, T_operator, T_literal and T_identifier
		else
		{
			int i_ope(0);
			//first, set the string contained in each token to be lower case just in case.

			string check=ptr->getStringRep();
			int sizeOfCheck = check.size();
			for(int i = 0; i<sizeOfCheck; i++)
				check[i]=tolower(check[i]);
            cout<<"check string"<<" "<<check<<endl;


			//check whether the TokenType is T_operator
			while(i_ope<28)
			{
				if((check.compare(Operator[i_ope]) == 0))
				{
					ptr->setTokenType(T_Operator);
					// on the premise that TokenType is found to be operator, check whether the operator is one of reserved keywords
					int i_key(0);
					while(i_key<96)
					{
						if((check.compare(keywords[i_key])) == 0)
						{
							ptr->setKeyword();
							cout<<"keywords is here"<<" "<<keywords[i_key]<<endl;
						}
						i_key++;
					}
				}

				i_ope++;
			}


		//check whether TokenType is Literal
			//1)  bit vector： x"0001010"
			if((check.find_first_of(caseBitVector,0))==0 && check.find_first_of("\"",0)==1)
			{//{
                cout<<"lalalalalla"<<endl;
			//	if((check.find_first_of(caseBitVector,0))==0)
				switch(check[0])
				{
                    case 'b':
                        ptr->setTokenDetails("std_logic_vector",((check.size()-3)));
                        ptr->setTokenType(T_Literal);
                        cout<<"b's width is !!!!!"<<ptr->getTokenDetails()->width;
                        break;
                    case 'o':
                        ptr->setTokenDetails("std_logic_vector",(2*((check.size()-3))));
                        ptr->setTokenType(T_Literal);
                        cout<<"o's width is !!!!!"<<ptr->getTokenDetails()->width;

                        break;
                    case 'x':
                        ptr->setTokenDetails("std_logic_vector",(4*((check.size()-3))));
                        ptr->setTokenType(T_Literal);
                        cout<<"x's width is !!!!!"<<ptr->getTokenDetails()->width;
                        break;
                    default:
                        ptr->setTokenType(T_Other);
				}
			}


//			2) form: '1'
			else if((check[0]=='\''&&check[(check.size()-1)]=='\''))
			{
			    cout<<"CAN U SEE ME?????????????????????"<<endl;
				ptr->setTokenType(T_Literal);
				//on the premise that TokenType is T_Literal, check whether the detail is std_logic or std_logic_vector
                ptr->setTokenDetails("std_logic",1);

			}
			//3) form: "10203020"
			else if((check[0]=='\"'&&check[(check.size()-1)]=='\"'))
			{
				ptr->setTokenType(T_Literal);
				ptr->setTokenDetails("std_logic_vector",(check.size()-2));
				cout<<"double quote here lalalala"<<endl;

			}
			//4) form: 5,55,40;
			else if((isdigit(check[0])))
			{
				ptr->setTokenType(T_Literal);
				ptr->setTokenDetails("integer",(check.size()));
                cout<<"integer here lalalala"<<endl;
			}
//
		//this part is to deal with identifiers
			else if((check.find_first_of(letters,0)==0))

			{

                    cout<<"time for literal"<<endl;
                    ptr->setTokenType(T_Identifier);
                    int i_key(0);
                    bool matchIdentifier=false;
                    while(i_key<96 && matchIdentifier==false)
                    {
                        if((check.compare(keywords[i_key])) == 0)//check whether the identifier is also a keyword
                        {
                            ptr->setKeyword();
                            cout<<"keyword of identifier is"<<" "<<keywords[i_key]<<endl;
                            matchIdentifier = true;
                        }
                        else
                            i_key++;
                    }
                    //check whether it is boolean identifier
                    if ((check.compare("false")) == 0 || (check.compare("true")) == 0)
					{
					    ptr->setTokenDetails("boolean",1);
					    cout<<"lalalla true or false!!!!"<<endl;
					    token->setKeyword();
					}


					//  signal a : std_logic;
					//  signal b : std_logic_vector (0 to 1);
                    else if(( check.compare("signal") == 0 ||check.compare("variable") == 0 ))

                           {
                                ptr->next->setTokenType(T_Identifier);
                                string identifierDetail = ptr->next->next->next->stringRep;
                                for(int i = 0; i<identifierDetail.size(); i++)   //set the string in prev pointer to lower case
                                    identifierDetail[i]=tolower(identifierDetail[i]);
                                if (identifierDetail.compare("std_logic") == 0)
                                {
                                     ptr->next->setTokenDetails("std_logic",1);
                                     cout<<"std_logic details is set to"<<" "<<"this identifier!!!!!!!!!!!!!!!!!!!!!!!!!"<<check<<endl;

                                }
                                else if(identifierDetail.compare("std_logic_vector") == 0)
                                {
                                    int n1(0),n2(0);
                                    int N=0; //difference between n1 and n2;
                                    string firstInt= token->next->next->next->next->next->stringRep;       //get the integer 0 .. signal b : std_logic_vector (0 to 1)
                                    string secondInt= token->next->next->next->next->next->next->next->stringRep;  //get the integer 1 .. signal b : std_logic_vector (0 to 1)
//                                    n1 = stoi(firstInt,nullptr,10);
//                                    n2 = stoi(secondInt,nullptr,10);
//                                    if (n1>n2)
//                                        N=n1-n2+1;
//                                    else
//                                    {
//                                        N=n2-n1+1;
//                                        ptr->next->setTokenDetails("std_logic_vector",N);
//                                        cout<<"std_logic details is set to"<<" "<<"this identifier!!!!!!!!!!!!!!!!!!!!!!!!!"<<check<<endl;
//
//                                    }

                                }
                                else if(identifierDetail.compare("integer") == 0)   //check whether the type is inter
                                {
                                    ptr->next->setTokenDetails("integer",1);

                                    cout<<"I see integer IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"<<endl;
                                }
                                else if(identifierDetail.compare("boolean") == 0)
                                {
                                    ptr->next->setTokenDetails("boolean",1);   //check whether type is boolean
                                    cout<<"I see boolean BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"<<endl;
                                }
                                Token* sameIdentifier = ptr;
                                Token* temp1 = ptr;
                                string x = strWithLowerCase(ptr->getNext());
                                string y = "default";
                                while(sameIdentifier!=nullptr)
                                    {
                                        y= strWithLowerCase(sameIdentifier);
                                        if (y==x)
                                        {
                                            sameIdentifier->setTokenType(T_Identifier);
                                            sameIdentifier->setTokenDetails(ptr->next->getTokenDetails()->type,ptr->next->getTokenDetails()->width);
                                            cout<<"sameIdentifier stringRep get assigned details******************"<<sameIdentifier->getTokenDetails()->type<<endl;
                                            cout<<"sameIdentifier stringRep get token type T_identifier RRRRRRRRRRRRRRRRRRR"<<sameIdentifier->getTokenType()<<endl;
                                        }

                                        sameIdentifier = sameIdentifier->next;

                                    }
                           }
        }
    }
}//function ending



void Tokenizer::prepareNextToken()

{

size_t terminate = (*str).size();

if(((*str).find_first_not_of(" \t\r", offset)) == std::string::npos)// check for empty line
complete = true;

if(tracker2 == true)
{
complete = true;
}
else if(comment_tracker == true)
{
     tokenLength = terminate - offset;
     tracker1 = true;
}

/*else if(quotation_tracker1 == true)
{
      tokenLength = (*str).find_first_of(caseFive, offset) - offset;
      quotation_tracker1 = false;
      quotation_tracker2 = true;
}
else if(quotation_tracker2 == true)
{
    tokenLength = 1;
    quotation_tracker2 = false;
}
*/
else{

size_t case1,case2, case3, case4, case4a, case2a, case2A, case2b, case2b1, case2b2, case2b3, case2b4, case2b5, case3a, case5a, first, firstA, space, case6, case7,case2b6;


        case1 = (*str).find_first_of(caseOne, offset);// case 1 single delimiters


        case2a = (*str).find_first_of(caseTwoA, offset); //case 2a
        case2b1 = (*str).find(caseTwoB1, offset);//<=
        case2b2 = (*str).find(caseTwoB2, offset);//=>
        case2b3 = (*str).find(caseTwoB3, offset);//**
        case2b4 = (*str).find(caseTwoB4, offset);//:=
        case2b5 = (*str).find(caseTwoB5, offset);//>=
        case2b6 = (*str).find(caseTwoB6, offset);
        case2A = std::min(case2b1,case2b2);
        case2A= std::min(case2b3,case2A);
        case2A= std::min(case2b4,case2A);
        case2A= std::min(case2b5,case2A);
        case2b= std::min(case2b6,case2A);
        case2 = std::min( case2a, case2b);// decide whether to use single or double character delmiters // case 2 end


        case3a = (*str).find_first_of(caseThreea, offset);// -
        case3 = (*str).find(caseThree, offset);//case 3 copmments

        case4 = (*str).find_first_not_of(" \t\r", offset);//case 4 letters

        case5a = (*str).find_first_of(caseFive, offset);//case 5 quotation marks
        case6 = (*str).find_first_of(caseBitVector, offset);// bit vector
        case7 = (*str).find_first_of(caseSingleQuote, offset);// single quote

        // dicide which case occur first
        firstA = std::min(case1, case2);
        firstA = std::min(firstA, case3);
        first = std::min(firstA, case4);
        first = std::min(case5a, first);
        first = std::min(case6, first);
        first = std::min(case7, first);

        offset = first;




        if(first == case2)
        {
            if(case2a >= case2b)
            {
              tokenLength = 2;
            }
            else
            {
               tokenLength = 1;
            }
        }
        else if(first == case1 && case1!=case3)
        {
           tokenLength = 1;
        }
        else if(first == case3)
        {

           if(case3a != std::string::npos && case3a < case3)
           {
            tokenLength = 1;
           }
            else
            {
            comment = true;
            tokenLength = 2;
            }
        }
        else if(first == case4 && first != case5a && first != case6 && first!= case7)// only enter case4 if it doesn't sastisfy "" or ''
        {

            case4a = (*str).find_first_of(" \t\r\'", offset);
            space = std::min(firstA,case4a);// comparing case 1&2&3 with case 4
            space = std::min(firstA,case4a);
            if(case4a == space && case4a != std::string::npos)// enter 4a if 4a is found
            {
               tokenLength = case4a - case4;
            }
            else if(firstA == space)// enters if 4a is not found
            {
                tokenLength = firstA - case4;
            }
            else
            {
               tokenLength = terminate - offset;// if it's the end of line
            }
        }
        else if(first == case5a)// quotation mark
        {

            tokenLength = (*str).find_first_of(caseFive, case5a + 1) - case5a + 1;
           // quotation_tracker1 = true;

        }

        else if(first == case6)//bit vector
        {
           if((*str).find_first_of(caseFive, case6) == case6 + 1)// if it is a bit vector
           {
             tokenLength = (*str).find_first_of(caseFive, case6 + 2) - case6 + 1;
           }
           else// if it is not a bit vector
           {
             tokenLength = (*str).find_first_not_of(caseFour, case6) - case6;
           }
        }
        else if(first == case7)// if it's ''
        {
        if((*str).find_first_of(caseFour, case7 + 1 ) < (*str).find_first_of(caseSingleQuote, case7 + 1 ) - case7 +1) // differenciating single hyphen from literals
        {
            tokenLength = 1;
        }
        else
        {
            tokenLength = (*str).find_first_of(caseSingleQuote, case7 + 1 ) - case7 +1;
        }

        }

if (offset + 1 == terminate)//if offset is at the end of string

{
       tracker1 = true;// let getNextToken function know it's the end of string
}

}//else ending

}//function ending



//Sets the current string to be tokenized

//Resets all Tokenizer state variables

//Calls Tokenizer::prepareNextToken() as the last statement before returning.
Tokenizer::Tokenizer()
{
	complete = false;
	offset = 0;
	tokenLength = 0;
	str = nullptr;

}
void Tokenizer::setString(string *str)

{
    offset = 0;
    tokenLength = 0;
    complete = false;
    tracker1 = false;
    tracker2 = false;
    comment = false;
    comment_tracker = false;
    //quotation_tracker1 = false;
   // quotation_tracker2 = false;
    this -> str = str;

    prepareNextToken();

}



//Returns the next token. Hint: consider the substr function

//Updates the tokenizer state

//Updates offset, resets tokenLength, updates processingABC member variables

//Calls Tokenizer::prepareNextToken() as the last statement before returning.

string Tokenizer::getNextToken()

{

if(tracker1 == true)
tracker2 = true;// signal prepareNextToken function to set complete to true
if(comment == true)
{
comment_tracker = true;
}
    string hop;
    if(complete == false)
{
    hop = (*str).substr(offset, tokenLength );
}

    offset = (tokenLength + offset);
    tokenLength = 0;
    prepareNextToken();

    return hop;
}







//****Challenge Task Functions******



//Removes the token from the linked list if it is not null

//Deletes the token

//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.

void TokenList::deleteToken(Token *token)

{


    if (token == nullptr)
    {
        cout<<"this linked node is empty"<<endl;

    }
    else if(token == head)
    {
        head->prev=nullptr;
        head=head->next;

    }

    else if(token == tail)
    {
        tail=tail->prev;
        tail->next=nullptr;


    }
    else
    {
        token->prev->next=token->next;
        token->next->prev=token->prev;

    }
    delete token;
}




//Removes all comments from the tokenList including the -- marker

//Returns the number of comments removed

int removeComments(TokenList &tokenList)

{

    Token* temp, *comments, *symbol;
    int counter=0;
    temp = tokenList.getFirst();

while(temp != nullptr)
{

        if( temp-> getStringRep()=="--")
        {
            symbol=temp;
            comments = temp -> getNext();   //cntents of comments follow the "--"symbol
            temp = comments -> getNext();   //let temp points to the the pointer after contents of comments
            tokenList.deleteToken(symbol);   //delete "--"

            tokenList.deleteToken(comments);  //delete contents of comments

            counter++;
        }
        else
        {
                temp = temp -> getNext();    // if temp token does not find "--", search next one
        }


}

     return counter-1;

}



int removeTokensOfType(TokenList &tokenList, tokenType type)
{
    int counter = 0; // count how many times the type of token is deleted
    Token* ptr = tokenList.getFirst();
    Token* temp = tokenList.getFirst();
    Token* should_be_delete = tokenList.getFirst(); //the token pointer which will be deleted
    while(ptr != nullptr)
    {
        if (ptr->getTokenType()==type)
        {
            should_be_delete = ptr;
            cout<<"deleted token type is DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"<<should_be_delete->getTokenType()<<endl;
            tokenList.deleteToken(should_be_delete);
            cout<<"did u delete?????????????????????????????????????"<<endl;
            counter++;
        }
        temp = ptr->getNext();
        ptr = temp;
    }
    return counter;

}


//PreCondition: Intact token list made by tokenizing input
//PostCondition: Returns a linked list that contains all of the valid conditional statement
TokenList* findAllConditionalExpressions(const TokenList &tokenList)// part 2
{

    TokenList* pointer = new TokenList;
    Token* temp = nullptr;
    Token* temp2 = nullptr;

    temp = tokenList.getFirst();

    while(temp != nullptr)
    {
        if(temp->getStringRep() == "if" || temp->getStringRep() == "elsif" || temp->getStringRep() == "when")
        {
            temp2 = temp;

            //find the token at the end of the line
            while(temp2 -> getStringRep()!= "\n")
            {
                temp2 = temp2 ->getNext();
            }

            // if the last token on the line is "then" or "else" then it is a conditional statement
            if(temp2->getPrev()->getStringRep() == "then" || temp2->getPrev()->getStringRep() == "else" )
            {
                temp = temp -> getNext();

                while(temp ->getStringRep()!= "then" && temp ->getStringRep()!= "else")
                {
                    pointer->append(temp);
                    temp = temp -> getNext();
                }

                pointer->append("\n");
            }
        }

        temp = temp -> getNext();
    }

    return pointer;
}

//PreCondition: any token
//PostCondition: Returns the token in lower case
string strWithLowerCase(Token* token)
{
    Token* transfer = token;
    string str = token->getStringRep();
    if (transfer = nullptr)
    {
         cout<<"nothing in this token"<<endl;
         return 0;
    }
    else
    {
        for(int i = 0; i<str.size(); i++)
				str[i]=tolower(str[i]);
    }
    return str;

}

//PreCondition: any token list that has variable "string" in each node
//PostCondition: return the same list but with all the letters transfered to lowercase
TokenList* transferToLowerCase(const TokenList &tokenList)
{
    TokenList* pointer2 = new TokenList;
    //Token *token = tokenList.getFirst();
    Token *temp = tokenList.getFirst();

    string str ="default";

    while (temp!=nullptr)//copy all of the tokens to a new list
    {
        pointer2->append(temp);
        temp = temp->getNext();
    }

    Token *temp2 = pointer2->getFirst();

    while (temp2!= nullptr)// transfer all of the string in tokens to lower case
    {
        temp2->setStringRep(strWithLowerCase(temp2));
        temp2 = temp2->getNext();

    }

    return pointer2;//finished list

}



//PreCondition: Intact token list made by tokenizing input
//PostCondition: Returns the number of tokens
int numOfTokens(const TokenList &tokenList)
{
    int numOfToken = 0;
    Token* temp = tokenList.getFirst();
    //cout<< temp->getStringRep();
    if (temp == nullptr)
    {
         cout<<"this tokenList is empty"<<endl;
         return 0;
    }
    else
    {
        while(temp!=nullptr)
        {
        if (temp->getStringRep()!="\n")
         {
            numOfToken++;
         }
          temp = temp->getNext();
        }
        return numOfToken;
    }
}

//Precondition: linked list from tokenizer
//Postcondition: return number missing endif and display lines where error occured
int numOfmissEndif(const TokenList &tokenList, int mode)
{

// check how many if out there, each endif matches one then
    int numOfIF(0);
    int numOfEndif(0);
    TokenList *newOne = findAllConditionalExpressions3(tokenList);
    Token* check = newOne->getFirst();

//    while(check)
//    {
//        cout<<check->getStringRep();
//        check = check->getNext();
//    }
//check = newOne->getFirst();
    if (check == nullptr)
{
         cout<<"this tokenList is empty"<<endl;
        return 0;
}
    else
{
        while(check!=nullptr)
        {
            if (check->getStringRep()=="if" && check->getPrev()->getStringRep()!="end")   //if we see if, we keep counting the number of If
                 numOfIF++;

            else if(check->getStringRep()=="end" && check->getNext()->getStringRep()=="if")   //if we see end, we check whether the token next to it contains if, and if so, number of end if ++
                numOfEndif++;

            check = check->getNext();
        }

        if ((numOfIF-numOfEndif)!=0 && (mode == 1))
        {
            check = newOne->getFirst();
            while(check)
                {
                    cout<<check->getStringRep() << " ";
                    //if(check->getTokenDetails() != nullptr)
                   //cout<<"["<<check->getTokenDetails()->width<<"]"<<"["<<check->getTokenDetails()->type<<"]" << " ";
                    check = check->getNext();
                }
        }

  }

        return (numOfIF-numOfEndif);   //the number of missing end if is the difference between num of if and number of end if
}

//Precondition: linked list from tokenizer
//Postcondition: return number of conditional expression
int numOfConditionalExp(const TokenList& tokenList)
{
    /*"\n"  to separate each expression
    but this is on the premise that each conditional expression can be correctly separated from the token list, if there is something wrong with the code, we cannot make sure that num of conditional expression will be correct*/
    int numOfConExp = 0;  //number of conditional expression
    Token* ptr = tokenList.getFirst();
     TokenList* newTokenList = findAllConditionalExpressions2(tokenList);
    if (ptr == nullptr)
    {
         cout<<"this tokenList is empty"<<endl;
         return 0;
    }

    else
    {

        ptr = newTokenList->getFirst();
        while (ptr != nullptr)
        {
            if ((ptr->getStringRep()).compare("\n")== 0)
            {
                 numOfConExp++;
                 ptr=ptr->getNext();
            }
            else
                 ptr=ptr->getNext();
        }
    }

    return numOfConExp;

}

//Precondition: linked list from tokenizer
//Postcondition: return number of missing then, and cout lines where error occured
int numOfmissThen(const TokenList &tokenList, int mode)
{
    bool found = false;
    bool nomore = false;
    bool entered = false;
    TokenList *superList = transferToLowerCase(tokenList);

    Token* check = superList->getFirst();
Token* marker;
Token* print;
//    while(check!=nullptr)
//    {
//        cout<<check->getStringRep()<<" ";
//        check = check ->getNext();
//    }
//    check = superList->getFirst();
//cout<<endl<<endl<<endl;

    int n_missThen = 0;
    if (check== nullptr)
{
        cout<<"this tokenList is empty"<<endl;
         return 0;
}
else
{
            while(check!=nullptr)
        {
            entered = false;
            nomore = false;
             if (((check->getStringRep()=="if") ||(check->getStringRep() == "elsif"))&&(check->getPrev()->getStringRep()!="end"))
             {

                 found = false;

                marker = check;
                print = marker;
                check = check ->getNext();
                while((check->getStringRep()!="if") &&(check->getStringRep() != "elsif")&&(check->getStringRep() != ";")&&(check->getStringRep() != "when")&&(check->getStringRep() != "else"))
                {
                    check = check->getNext();
                }
                while(marker!=check)
                {

                    if(marker->getStringRep() == "then")
                    found = true;
                    marker = marker ->getNext();

                }

                if(found== false )
                {
                    entered = true;
                    n_missThen++;
                    if (mode==1)
                   {
                       if(marker->getPrev()->getStringRep() == "end")
                        marker = marker->getPrev()->getPrev();
                    while(print != marker)
                        {
                            cout<<print->getStringRep()<<" ";
                            print = print->getNext();
                        }
                        cout<<" "<<"-- Missing then"<<endl;;
                    }
                }




             }
             if(entered==false)//only move to next one if check is not on keywords(if,elsif)
             check=check->getNext();
        }
}


     return n_missThen;
}

//Precondition: linked list from tokenizer
//Postcondition: return number of type and width missmatch and cout lines where error occured
std::pair <int, int> widthNtypeMismatch(TokenList &tokenList, int mode)
{
    int widthNotMatch = 0;
    int typeNotMatch = 0;
    TokenList *superList = findAllConditionalExpressions2(tokenList);
    Token *ptr = superList->getFirst();
    Token *tempPtr = nullptr;

	bool entered = false;
    if (ptr == nullptr)
    {
         cout<<"this tokenList is empty"<<endl;
       //  exit(1);
    }
    else
    {

        while (ptr != nullptr)
        {
            if((ptr->getStringRep()== "<") || (ptr->getStringRep()== ">") || (ptr->getStringRep()== "=" )|| (ptr->getStringRep()== "/=" )|| (ptr->getStringRep()== ">="))
            {
                entered = false;
                if(ptr -> getPrev() -> getTokenDetails()->type != ptr->getNext()->getTokenDetails()->type)
               {
                   entered = true;
				   typeNotMatch++;

				   if (mode == 1)
				   {



                        tempPtr = ptr->getPrev();

                        while(tempPtr->getStringRep() != "\n")
                        {
                            tempPtr = tempPtr->getPrev();
                        }
                        tempPtr = tempPtr->getNext();

                        while(tempPtr->getStringRep()  != "\n")
                        {
                        cout<<tempPtr->getStringRep()<<" ";
                            tempPtr = tempPtr->getNext();
                        }
                        cout<<"-- Type Mismatch"<<endl;
                        }
               }
               if(ptr -> getPrev() -> getTokenDetails()->width != ptr->getNext()->getTokenDetails()->width)
               {
                   if(entered == false)

                   {
                        widthNotMatch++;
                        tempPtr = ptr->getPrev();
                        if(mode == 1)
                        {


                        while(tempPtr->getStringRep() != "\n")
                        {
                            tempPtr = tempPtr->getPrev();
                        }
                        tempPtr = tempPtr->getNext();

                        while(tempPtr->getStringRep()  != "\n")
                        {
                        cout<<tempPtr->getStringRep()<<" ";
                            tempPtr = tempPtr->getNext();
                        }
                        cout<<"-- Width Mismatch"<<endl;
                        }

                    }

               }
            }
           ptr = ptr ->getNext();
        }

    }

    return std::make_pair(widthNotMatch,typeNotMatch);

}

//Precondition: linked list from tokenizer
//Postcondition: allow user to choose mode and display message accordingly
void compile (TokenList &tokenList)
{
    int answer;
    cout<<"would you like to see verbose mode?  type 1 or 0  (1 for \"yes,verbose mode plz\", 0 for \"no,non-verbosemode plz\")"<<endl;
    cin>>answer;
	while (answer!=1 && answer!= 0)
	{
		cout<<"retype again plz, only 1 or 0 will be processed"<<endl;
		cout<<"type in your option here: 1 or 0"<<" "<<endl;
		cin>>answer;
	}

	if (answer == 1 )  //non-verbose mode
    {
        cout<<"number of missing then is"<<" "<<numOfmissThen(tokenList,answer)<<endl;
        cout<<"number of tokes is"<<" "<<numOfTokens(tokenList)<<endl;

        cout<<"number of missing End If is"<<" "<<numOfmissEndif(tokenList,answer)<<endl;

        cout<<"number of conditional expression is"<<" "<<numOfConditionalExp(tokenList)<<endl;

//        std::pair <int, int> object = widthNtypeMismatch(tokenList,answer);
//        cout<<"Number of Width Mismatch "<<object.first<<endl<<"Number of Type Mismatch "<<object.second<<endl;


    }

    else if (answer == 0)  //verbose mode
    {
        cout<<"number of missing then is"<<" "<<numOfmissThen(tokenList,answer)<<endl;
		cout<<"number of tokens is"<<" "<<numOfTokens(tokenList)<<endl;
        cout<<"number of missing End If is"<<" "<<numOfmissEndif(tokenList,answer)<<endl;

        cout<<"number of conditional expression is"<<" "<<numOfConditionalExp(tokenList)<<endl;

        //std::pair <int, int> object = widthNtypeMismatch(tokenList,answer);
       // cout<<"Number of Width Mismatch "<<object.first<<endl<<"Number of Type Mismatch "<<object.second<<endl;


   //      how to allocate error on which line?  think about it
   //     additional detail, maybe .... missing ; missing bracket etc.....

    }

}

//Precondition: linked list from tokenizer
//Postcondition: return a list of conditional expression including keywords (if/elsif)
TokenList* findAllConditionalExpressions2( const TokenList &tokenList)
{

    TokenList* pointer = new TokenList;
    Token* temp = nullptr;
    Token* temp2 = nullptr;
    TokenList* newone = transferToLowerCase(tokenList);


    temp = newone->getFirst();

    while(temp != nullptr)
    {
        if(temp->getStringRep() == "if" || temp->getStringRep() == "elsif" || temp->getStringRep() == "when")
        {
            temp2 = temp;

            //find the token at the end of the line
            while(temp2 -> getStringRep()!= "\n")
            {
                temp2 = temp2 ->getNext();
            }

            // if the last token on the line is "then" or "else" then it is a conditional statement
            if(temp2->getPrev()->getStringRep() == "then" || temp2->getPrev()->getStringRep() == "else" )
            {
                //temp = temp -> getNext();

                while(temp ->getStringRep()!= "then" && temp ->getStringRep()!= "else")
                {
                    pointer->append(temp);
                    temp = temp -> getNext();
                }
               if(temp ->getStringRep()== "then" || temp ->getStringRep()== "else")
                pointer->append(temp);
                temp = temp -> getNext();

                pointer->append("\n");
            }
        }

        temp = temp -> getNext();
    }

    return pointer;
}


//Precondition: linked list from tokenizer
//Postcondition: return a list of conditional expression including keywords (if/elsif), including end if
TokenList* findAllConditionalExpressions3(const TokenList &tokenList)//3rd version for findind endif, exclusively
{

    TokenList* pointer = new TokenList;
    Token* temp = nullptr;
    TokenList* newone = transferToLowerCase(tokenList);
    temp = newone->getFirst();

    while(temp != nullptr)
    {
        if(temp->getStringRep() == "if" || temp->getStringRep() == "elsif" || temp->getStringRep() == "when"|| temp->getStringRep() == "end")
        {

            while(temp->getStringRep()!= "\n")
            {
                pointer->append(temp);
                temp = temp->getNext();
            }
           // temp = temp->getNext();
            pointer ->append("\n");
        }


        temp = temp -> getNext();
    }


    return pointer;
}



