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

void exceptionsTest() {
    try {
        MtmMat<float> m(Dimensions(4,3),0);
        MtmMatSq<float> m2(m);
        myAssert(false);
    }
    catch (MtmExceptions::IllegalInitialization& e){
        cout<< e.what() <<endl;
    }
/*
    try {
        MtmMat<double> m1(Dimensions(10,10),5);
        m1.resize(Dimensions(100000000,100000000));
        myAssert(false);
    }
    catch (MtmExceptions::OutOfMemory& e){
        cout<< "Mem" <<endl;
    }
    catch (MtmExceptions::ChangeMatFail& e){
        cout<< "Mem" <<endl;
    }
*/
    try {
        MtmVec<int> v1(3,5);
        MtmVec<int> v2(3,5);
        MtmMat<int> m3=v1*v2;
        myAssert(false);
    }
    catch (MtmExceptions::DimensionMismatch& e){
        cout<< e.what() <<endl;
    }
    try {
        MtmMatSq<int> m1(3,1);
        m1.reshape(Dimensions(3,3));
        myAssert(false);
    }
    catch (MtmExceptions::ChangeMatFail& e) {
        cout << e.what() << endl;
    }
}


int main() {
    exceptionsTest(); //15 points
}

