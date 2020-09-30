#include <iostream>

#include "MtmVec.h"
#include "MtmMat.h"
#include "MtmMatSq.h"
#include "MtmMatTriag.h"
#include "Complex.h"


using namespace MtmMath;
using std::cout;
using std::endl;

void myAssert(bool flag) {
    if (not(flag)) {
        cout << "fail" <<endl;
    }
}



void constructorsAndAssigment() {
    MtmVec<Complex> v1(4,Complex(3,3));
    MtmMat<int> m1(Dimensions(3,3),0);
    m1[0][2]=3;
    MtmMatTriag<int> m3(m1);
    m1[0][0]=1;
    m1[1][1]=4;
    MtmMatSq<int> m2(m1);
    myAssert(m2[0][0]==1 and m1[1][1]==4 and m1[0][2]==3 );
}



int main() {
    constructorsAndAssigment(); //15 points
}

