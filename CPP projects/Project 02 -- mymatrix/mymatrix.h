/*mymatrix.h*/

//
// Tony Lau
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and
// cols).  Also, rows can be "jagged" --- i.e. rows can have
// different column sizes, and thus the matrix is not necessarily
// rectangular.  All elements are initialized to the default value
// for the given type T.  Example:
//
//   mymatrix<int>  M;  // 4x4 matrix, initialized to 0
//
//   M(0, 0) = 123;
//   M(1, 1) = 456;
//   M(2, 2) = 789;
//   M(3, 3) = -99;
//
//   M.growcols(1, 8);  // increase # of cols in row 1 to 8
//
//   for (int r = 0; r < M.numrows(); ++r)
//   {
//      for (int c = 0; c < M.numcols(r); ++c)
//         cout << M(r, c) << " ";
//      cout << endl;
//   }
//
// Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class mymatrix
{
private:
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // dynamic array of ROWs
  int  NumRows;  // total # of rows (0..NumRows-1)

public:
  //
  // default constructor:
  //
  // Called automatically by C++ to construct a 4x4 matrix.  All
  // elements are initialized to the default value of T.
  //
  mymatrix()
  {
    Rows = new ROW[4];  // an array with 4 ROW structs:
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (int r = 0; r < NumRows; ++r)
    {
      Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
      }
    }
  }

  //
  // parameterized constructor:
  //
  // Called automatically by C++ to construct a matrix with R rows,
  // where each row has C columns. All elements are initialized to
  // the default value of T.
  //
  mymatrix(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::constructor: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::constructor: # of cols");

    Rows = new ROW[R];
    NumRows = R;
    // initialize each row to have C columns:
    for(int r = 0; r <NumRows; ++r)
    {
        Rows[r].Cols = new T[C];
        Rows[r].NumCols = C;
        // initialize the elements to their default value:
        for(int c = 0; c < Rows[r].NumCols; ++c)
        {
            Rows[r].Cols[c] = T{};
        }
    }
  }


  //
  // copy constructor:
  //
  // Called automatically by C++ to construct a matrix that contains a
  // copy of an existing matrix.  Example: this occurs when passing
  // mymatrix as a parameter by value
  //
  //   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
  //   { ... }
  //
  mymatrix(const mymatrix<T>& other)
  {
    this->Rows= new ROW[other.NumRows];
    this->NumRows=other.NumRows;
    
    for(int i = 0; i < NumRows; i++){
      this->Rows[i].Cols =  new T[other.Rows[i].NumCols];
      this->Rows[i].NumCols = other.Rows[i].NumCols;
      for(int j = 0; j < this->Rows[i].NumCols; j++){
        this->Rows[i].Cols[j]=other.Rows[i].Cols[j];
      }
    }
  }


  //
  // numrows
  //
  // Returns the # of rows in the matrix.  The indices for these rows
  // are 0..numrows-1.
  //
  int numrows() const
  {
  return NumRows;
    return -1;
  }


  //
  // numcols
  //
  // Returns the # of columns in row r.  The indices for these columns
  // are 0..numcols-1.  Note that the # of columns can be different
  // row-by-row since "jagged" rows are supported --- matrices are not
  // necessarily rectangular.
  //
  int numcols(int r) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");

    return Rows[r].NumCols;
        return -1;
  }


  //
  // growcols
  //
  // Grows the # of columns in row r to at least C.  If row r contains
  // fewer than C columns, then columns are added; the existing elements
  // are retained and new locations are initialized to the default value
  // for T.  If row r has C or more columns, then all existing columns
  // are retained -- we never reduce the # of columns.
  //
  // Jagged rows are supported, i.e. different rows may have different
  // column capacities -- matrices are not necessarily rectangular.
  //
  void growcols(int r, int C)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");

    if(Rows[r].NumCols < C){
        T* temp = new T[C];
        for(int i = Rows[r].NumCols; i < C; ++i)
        {
            temp[i]= T{};
        }
        for(int i = 0; i < Rows[r].NumCols; ++i)
        {
            temp[i]=Rows[r].Cols[i];
        }
        delete[] Rows[r].Cols;
        Rows[r].Cols = temp;
        Rows[r].NumCols= C;
    }
  }//end of growcols


  //
  // grow
  //
  // Grows the size of the matrix so that it contains at least R rows,
  // and every row contains at least C columns.
  //
  // If the matrix contains fewer than R rows, then rows are added
  // to the matrix; each new row will have C columns initialized to
  // the default value of T.  If R <= numrows(), then all existing
  // rows are retained -- we never reduce the # of rows.
  //
  // If any row contains fewer than C columns, then columns are added
  // to increase the # of columns to C; existing values are retained
  // and additional columns are initialized to the default value of T.
  // If C <= numcols(r) for any row r, then all existing columns are
  // retained -- we never reduce the # of columns.
  //
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");
      
      //Grow the rows first
    if(NumRows < R){
      ROW* temp = new ROW[R];//make new ROW array with newR
      for(int i = 0; i < NumRows; i++){//iterate through each Row in temp
          temp[i].Cols = new T[Rows[i].NumCols];//for each Row increase to C or more
          temp[i].NumCols = Rows[i].NumCols;
          for(int j = 0; j<Rows[i].NumCols; j++){
              temp[i].Cols[j]=Rows[i].Cols[j];//copy data over
          }
      }
      for(int i = NumRows; i<R; i++){
          temp[i].Cols= new T[C];
          temp[i].NumCols=C;
          for(int j = 0; j<C; j++){//fill in new indices with default value of T
              temp[i].Cols[j] = T{};
          }
      }
      delete[] Rows;
      Rows=temp;
      NumRows=R;
    }
    //Grow the Columns second
    for(int i = 0; i < NumRows; i++){
      if(Rows[i].NumCols<C){
        growcols(i,C);
      }
    }
  }


  //
  // size
  //
  // Returns the total # of elements in the matrix.
  //
  int size() const
  {
      int total = 0;
      for (int i =0; i<NumRows; i++){
          total += Rows[i].NumCols;
      }
      return total;

    return -1;
  }


  //
  // at
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M.at(r, c) = ...
  //    cout << M.at(r, c) << endl;
  //
  T& at(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");

    return Rows[r].Cols[c];

   
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M(r, c) = ...
  //    cout << M(r, c) << endl;
  //
  T& operator()(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return Rows[r].Cols[c];
  
  }

  //
  // scalar multiplication
  //
  // Multiplies every element of this matrix by the given scalar value,
  // producing a new matrix that is returned.  "This" matrix is not
  // changed.
  //
  // Example:  M2 = M1 * 2;
  //
  mymatrix<T> operator*(T scalar)
  {
    mymatrix<T> result;
    result.Rows = new ROW[this->NumRows];
    result.NumRows = this ->NumRows;//give results the same number of ROW's as this
    for( int i = 0; i < (this->NumRows); i++){
        result.Rows[i].Cols= new T[this->Rows[i].NumCols];//give each result row the same number of columns
        result.Rows[i].NumCols = this-> Rows[i].NumCols;
    }

    for(int r = 0; r < NumRows; r++){
        for(int c = 0; c <Rows[r].NumCols; c++){
            result.Rows[r].Cols[c]=this->Rows[r].Cols[c]*scalar;
        }
    }


    return result;
  }


  //
  // matrix multiplication
  //
  // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
  // M2 is the "other" matrix.  This produces a new matrix, which is returned.
  // "This" matrix is not changed, and neither is the "other" matrix.
  //
  // Example:  M3 = M1 * M2;
  //
  // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
  // addition, the sizes of M1 and M2 must be compatible in the following sense:
  // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
  // multiplication can be performed, and the resulting matrix is of size RxC.
  //
  mymatrix<T> operator*(const mymatrix<T>& other)
  {
    int checker;
    // both matrices must be rectangular for this to work:
    checker = this->Rows[0].NumCols;
    for(int i = 0; i < this->NumRows; i++){
      if(checker !=this-> Rows[i].NumCols)
        throw runtime_error("mymatrix::*: this not rectangular");
    }
    checker = other.Rows[0].NumCols;
    for(int i = 0; i < other.NumRows; i++){
      if(checker != other.Rows[i].NumCols)
        throw runtime_error("mymatrix::*: other not rectangular");
    }
   
    // if (this matrix is not rectangular)
    //   throw runtime_error("mymatrix::*: this not rectangular");
    // if (other matrix is not rectangular)
    //   throw runtime_error("mymatrix::*: other not rectangular");

    if(this->Rows[0].NumCols != other.NumRows){
      throw runtime_error("mymatrix::*: size mismatch");
    }
   
    // if (this matrix's # of columns != other matrix's # of rows)
    //   throw runtime_error("mymatrix::*: size mismatch");

    mymatrix<T> result(this->NumRows,other.Rows[0].NumCols);
    for(int thisRow = 0; thisRow < this->NumRows; thisRow++){
      for(int otherCol = 0; otherCol < other.Rows[0].NumCols; otherCol++){
        for(int otherRow =0; otherRow < other.NumRows; otherRow++){
          result.Rows[thisRow].Cols[otherCol]+=this->Rows[thisRow].Cols[otherRow]*other.Rows[otherRow].Cols[otherCol];
        }
      }
    }


    return result;
  }

  // Outputs the contents of the matrix; for debugging purposes.
  void _output()
  {
    for (int r = 0; r < this->NumRows; ++r)
    {
      for (int c = 0; c < this->Rows[r].NumCols; ++c)
      {
        cout << this->Rows[r].Cols[c] << " ";
      }
      cout << endl;
    }
  }

};
