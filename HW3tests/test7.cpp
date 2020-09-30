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

void resizeReshapeTest() {
    MtmMat<int> m1(Dimensions(4,4),5);
    m1.reshape(Dimensions(16,1));
    myAssert(m1[15][0]==5);
    MtmMat<int> m2(Dimensions(2,2),5);
    m2.resize(Dimensions(3,3),1);
    myAssert(m2[2][2]==1 and m2[2][0]==1 and m2[0][0]==5);
}

int main() {

    resizeReshapeTest(); //10 points
}

