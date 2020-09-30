/**
 * created by Tsuf Cohen and Safi Azmi 22.01.2019
 */
#ifndef EX3_MTMMATTRIAG_H
#define EX3_MTMMATTRIAG_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMatSq.h"

using std::size_t;
using MtmMath::MtmMat;

namespace MtmMath {

    template <typename T>
    class MtmMatTriag : public MtmMatSq<T> {
        bool upperTriag;
    public:
/*
 * Triangular Matrix constructor, m is the number of rows and columns in the matrix,
 * val is the initial value for the matrix elements and isUpper_ is whether it is upper
 * Rectangular matrix (true means it is)
 */
        explicit MtmMatTriag(
                size_t m,
                const T& val = T(),
                bool isUpper_t = true) try :
                MtmMatSq<T>(m, val), upperTriag(isUpper_t) {
            if(upperTriag) {
                for(size_t i(0) ; i<m ; ++i) {
                    for(size_t j(0) ; j<i ; ++j) {
                        (*this)[i][j] = T();
                    }
                    this->matrix[i].lock_start = i;
                }
            }else {
                for(size_t i(0) ; i<m ; ++i) {
                    for(size_t j(m-1) ; i<j ; --j) {
                        this->matrix[i][j] = T();
                    }
                    this->matrix[i].lock_end = i;
                }
            }
        }catch(MtmExceptions::OutOfMemory& e) {
            throw;
        }catch(MtmExceptions::IllegalInitialization& e) {
            throw;
        }catch(MtmExceptions::MtmExceptions& e) {
            throw;
        }

        /*
        * default copy constructor
        */
        MtmMatTriag(const MtmMatTriag& matrix) = default;
        /*
         * default destructor
         */
        ~MtmMatTriag() = default;
        /*
         * default operator =
         */
        MtmMatTriag& operator=(const MtmMatTriag& matrix) = default;

        /**
         * lock triag matrix
         */
        void lockTriag() {
            if(upperTriag) {
                for(size_t i(0) ; i<this->rowSize() ; ++i) {
                    this->matrix[i].lock_start = i;
                }
            }else {
                for(size_t i(0) ; i<this->rowSize() ; ++i) {
                    this->matrix[i].lock_end = i;
                }
            }
        }

        explicit MtmMatTriag(const MtmMat<T>& matrix) try :
                MtmMatTriag(matrix.rowSize()) {
            if(matrix.rowSize()!=matrix.colSize()) {
                throw MtmExceptions::IllegalInitialization();
            }
            this->unlockTriag();
            bool lowerTriag(true);
            for(size_t i(0) ; i<matrix.rowSize() ; ++i) {
                for(size_t j(0) ; j<matrix.colSize() ; ++j) {
                    (*this)[i][j] = matrix[i][j];
                    if(i==j) {
                        continue;
                    }
                    if(i<j && (matrix[i][j]!=T())) {
                        lowerTriag = false;
                    }
                    if(i>j && (matrix[i][j]!=T())) {
                        upperTriag = false;
                    }
                }
            }
            if(!upperTriag && !lowerTriag) {
                throw MtmExceptions::IllegalInitialization();
            }
            lockTriag();
        }catch(MtmExceptions::OutOfMemory& e) {
            throw;
        }catch(MtmExceptions::IllegalInitialization& e) {
            throw;
        }catch(MtmExceptions::MtmExceptions& e) {
            throw;
        }

        void resize(
                Dimensions dim,
                const T& val = T()) override {               //DONE
            if((dim.getCol())!=(dim.getRow())) {
                throw MtmExceptions::ChangeMatFail(this->dimOfMat, dim);
            }
            if(dim.getRow()==0 || dim.getCol()==0) {
                throw MtmExceptions::ChangeMatFail(this->getDimensions(), dim);
            }
            try {
                MtmMatTriag<T> newMat(dim.getCol(), val, upperTriag);
                if(upperTriag) {
                    for(size_t i(0) ;
                            (i<dim.getRow()) && (i<this->rowSize()) ;
                            ++i) {
                        for(size_t j(i) ;
                                (j<dim.getCol()) && (j<this->colSize()) ;
                                ++j) {
                            newMat[i][j] = this->matrix[i][j];
                        }
                    }
                }else {
                    for(size_t i(0) ;
                            (i<dim.getRow()) && (i<this->rowSize()) ;
                            ++i) {
                        for(size_t j(0) ; j<=i ; ++j) {
                            newMat[i][j] = this->matrix[i][j];
                        }
                    }
                }
                *this = newMat;
            }catch(MtmExceptions::MtmExceptions& e) {
                throw;
            }
        }

        void
        transpose() override {                                            //DONE
            upperTriag = !upperTriag;
            this->unlockTriag();
            MtmMat<T>::transpose();
            lockTriag();
        }
    };
}

#endif //EX3_MTMMATTRIAG_H
