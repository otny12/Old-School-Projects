// Tony Lau
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
// 
// main.cpp is used for testing
#include <iostream>
#include "mymatrix.h"

using namespace std;

bool sizeTest(){
  mymatrix<int> tester;
  cout << "default matrix" << endl;
  tester._output();
  if(tester.size()==16)
    return true;
  else
    return false;

}
bool parameterizedSmallest(){
  cout <<"this is parameterized 2,2" << endl;
  mymatrix<int> tester(2,2);
  tester._output();

  if(tester.size()==4)
    return true;
  else
    return false;
}

bool growCosTest(){
  mymatrix<int> tester;
  tester.growcols(0,6);
  tester._output();

  if(tester.size()==18)
    return true;
  else
    return false;
}

bool growTest(){
  mymatrix<int> tester;
  tester.grow(5,6);
  tester._output();

  if(tester.size()==30)
    return true;
  else
    return false;
}

bool accessorTest(){
  mymatrix<int> tester1;
  tester1.at(1,1) = 40;
  if(tester1.at(1,1)!=40)
    return false;

  tester1(3,3) = 60;
  if(tester1(3,3)!=60)
    return false;

  tester1._output();
  return true;
}
void copyConstructorTest(){
  mymatrix<int> tester1;
  for(int i = 0; i< 4 ;i++){
    for(int j =0; j <4; j++){
      tester1(i,j)=i*j;
    }
  }
  tester1._output();
  cout<<"matrix below should be times 2 the one above"<< endl;
  mymatrix<int>tester2 = tester1*2;
  tester2._output();
}

void matrixMultiplication(){
  mymatrix<int>M1(2,2);
  mymatrix<int>M2(2,2);

  M1(0,0)=2;
  M1(0,1)=3;
  M1(1,0)=4;
  M1(1,1)=5;
  M1._output();

  M2(0,0)=2;
  M2(0,1)=3;
  M2(1,0)=4;
  M2(1,1)=5;
    M2._output();

    mymatrix<int>test (M1*M2);
    test._output();
}

void copyToFunctionTest(mymatrix<int> M){
    M(1,1)=100000;
    cout << "inside func" << endl;
    M._output();
    cout<< "outside func"<< endl;
}

int main(){

    if(sizeTest())
        cout << "default size works!"<< endl;
    else
        cout << "default size doesn't work :(((("<< endl;

    if(parameterizedSmallest())
        cout << "param construtictor works" << endl;
    else
        cout << "param constructor fails" << endl;

    if(growCosTest())
        cout << "columns grew correctly" << endl;
    else
        cout << "columns did not grow correctly" << endl;

    if(growTest())
        cout << "matrix grew correctly" << endl;
    else
        cout << "matrix did not grow correctly" << endl;

    if(accessorTest())
        cout << "accessors work"<< endl;
    else
        cout << "accessors do not work" << endl;

    copyConstructorTest();
    matrixMultiplication();
    mymatrix<int> M;
    copyToFunctionTest(M);
    M._output();
    
    mymatrix<string>S;
    S(0,0)="hi";
    S(0,1)="hello";
    S(1,0)="how are you?";
    S(1,1)="what's this?";
    S._output();
    
    mymatrix<int> M1(2,3);
    mymatrix<int> M2(3,2);
    mymatrix<int> M3=M1*M2;
    M3._output();
}
