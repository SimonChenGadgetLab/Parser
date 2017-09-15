//Use only the following three libraries:
#include "parserClasses.h"
#include <iostream>
#include <fstream>

using namespace std;

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main()
{
	ifstream sourceFile;
	TokenList tokens;

  //Lists for types of tokens
  TokenList operatorTokens;
  TokenList identifierTokens;
  TokenList literalTokens;
  TokenList commentBodyTokens;
  TokenList otherTokens;

	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("hi.vhd");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

//	while(!sourceFile.eof()) {
//		string line;
//		getline(sourceFile, line);
//
//		tokenizer.setString(&line);
//		while(!tokenizer.isComplete())
//        {
//			tokens.append(tokenizer.getNextToken());
//		}
//		tokens.append("\n");
//	}
		while(!sourceFile.eof()) {
		string lineA;

		getline(sourceFile, lineA);

		tokenizer.setString(&lineA);
		while(!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
		//Re-insert newline that was removed by the getline function
		tokens.append("\n");
	}

//	Token *t = tokens.getFirst();
//	while(t) {
//		cout << t->getStringRep() << " ";
//		t = t->getNext();
//	}
//
//	cout<<"give me my lines !!!!!!!!!!!!!!!!!!!"<<endl;
//	Token *k = tokens.getFirst();
//
//		while(k!=nullptr)
//    {
//
//        tokens.findAndSetTokenDetails(k);
//        k = k->getNext();
//
//    }
//
//	k = tokens.getFirst();
//	while(k)
//    {
//		cout<<k->getStringRep() << " "<<endl;
//		if(k->getTokenDetails() != nullptr)
//            cout<<k->getTokenDetails()->width<<" This is width"<<endl;
//		k = k->getNext();
//	}
////cout<<"number of mismatch "<<numOfWidthMismatch(tokens)<<endl;
//
//	 t = tokens.getFirst();
//	while(t) {
//		cout << t->getStringRep() << " ";
//        if(t->getTokenDetails() != nullptr)
//        cout<<"["<<t->getTokenDetails()->width<<"]"<<"["<<t->getTokenDetails()->type<<"]" << " ";
//		t = t->getNext();
//	}
//	cout<<endl;
////		 t = tokens.getFirst();
////	while(t) {
////		cout << t->getStringRep();
////        //if(t->getTokenDetails() != nullptr)
////        //cout<<"["<<t->getTokenDetails()->width<<"]"<<"["<<t->getTokenDetails()->type<<"]" << " ";
////		t = t->getNext();
////	}
////cout<<"number of mismatch "<<numOfTypeMismatch(tokens,0)<<endl;
//std::pair <int, int> object = widthNtypeMismatch(tokens,1);
//cout<<"Number of Width Mismatch "<<object.first<<endl<<"Number of Type Mismatch "<<object.second<<endl;
////
////TokenList xx=transferToLowerCase(tokens);
////k = xx.getFirst();
////	while(k)
////    {
////		cout<<k->getStringRep() << " "<<endl;
////		if(k->getTokenDetails() != nullptr)
////            cout<<k->getTokenDetails()->width<<" This is width"<<endl;
////		k = k->getNext();
////	}
//
//cout<<"number of missing then "<<" "<<numOfmissThen(tokens,1)<<endl;
//

//	Token *k = tokens.getFirst();
//
//		while(k!=nullptr)
//    {
//
//        tokens.findAndSetTokenDetails(k);
//        k = k->getNext();
//
//    }
//
//
//Token *t = tokens.getFirst();
//	 t = tokens.getFirst();
//	while(t) {
//		cout << t->getStringRep() << " ";
//        if(t->getTokenDetails() != nullptr)
//        cout<<"["<<t->getTokenDetails()->width<<"]"<<"["<<t->getTokenDetails()->type<<"]" << " ";
//		t = t->getNext();
//	}
//	cout<<endl;
//
// TokenList* loda = transferToLowerCase(tokens);
// Token *q = loda ->getFirst();
// 	while(q)
//    {
//		cout<<q->getStringRep() << " ";
//        if(q->getTokenDetails() != nullptr)
//        cout<<"["<<q->getTokenDetails()->width<<"]"<<"["<<q->getTokenDetails()->type<<"]" << " ";
//		q = q->getNext();
//	}
//
//TokenList *loda2 = findAllConditionalExpressions(tokens);
//Token *q;
//q = loda2 ->getFirst();
//
// 	while(q)
//    {
//		cout<<q->getStringRep() << " ";
//        //if(q->getTokenDetails() != nullptr)
//        //cout<<"["<<q->getTokenDetails()->width<<"]"<<"["<<q->getTokenDetails()->type<<"]" << " ";
//		q = q->getNext();
//	}
//cout<<endl<<endl;

//  TokenList *superList = findAllConditionalExpressions3(tokens);
//
//    Token* check = superList->getFirst();
//
//    while(check!=nullptr)
//    {
//        cout<<check->getStringRep()<<" ";
//        check = check ->getNext();
//    }
//compile(tokens);

//cout<<"number of missing if "<<numOfmissEndif(tokens, 1)<<endl;
//cout<<"number of missing then "<<numOfmissThen(tokens, 1)<<endl;

//std::pair <int, int> object = widthNtypeMismatch(tokens,1);
//cout<<"Number of Width Mismatch "<<object.first<<endl<<"Number of Type Mismatch "<<object.second<<endl;

//	k = loda->getFirst();
//	while(k)
//    {
//		cout<<k->getStringRep() << " "<<endl;
//		//if(k->getTokenDetails() != nullptr)
//           // cout<<k->getTokenDetails()->width<<" This is width"<<endl;
//		k = k->getNext();
//	}
//cout<<"number of missing endIf "<<" "<<numOfmissEndif(tokens,0)<<endl;

//cout<<" I hope this shit is right, mehhhhhhhhhhhhhhhhhhhhhhhh";
//	Token *h = tokens.getFirst();
//	while(h)
//    {
//		cout<<h->getStringRep() << " "<<endl;
//		h = h->getNext();
//	}
//	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/
//	Token *t = tokens.getFirst();
//	while(t)
//    {
//		cout<<t->getStringRep() << " "<<endl;
//		t = t->getNext();
//	}
//	cout<<"lalalal"<<endl;
//	Token*x = tokens.getFirst();
//	Token*temp = x;
//	while(x!=nullptr)
//    {
//
//        tokens.findAndSetTokenDetails(x);
//        temp = x->getNext();
//        x = temp;
//
//    }
//    Token*y = tokens.getFirst();
//    while(y!=nullptr)
//    {
//
//        cout<<y->getTokenType()<<endl;
//        temp = y->getNext();
//        y = temp;
//
//    }
//
//
//
  compile(tokens);



//   For your testing purposes only
//
//   Ensure that tokens have all type information set
//
//   Create operator,identifier,literal, etc. tokenLists from the master list of tokens


	return 0;
}
