/**
 * created by Tsuf Cohen and Safi Azmi 22.01.2019
 */
#ifndef EX3_MTMMATREC_H
#define EX3_MTMMATREC_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMat.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMatSq : public MtmMat<T> {
    public:
/*
 * Rectangular Matrix constructor, m is the number of rows and columns in the matrix
 * and val is the initial value for the matrix elements
 */
        explicit MtmMatSq(size_t m, const T& val = T()) try :
                MtmMat<T>(m, m, val) {

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
        MtmMatSq(const MtmMatSq<T>& matrix) = default;

        explicit MtmMatSq(const MtmMat <T>& matrix) try :
                MtmMatSq(matrix.rowSize()) {
            if(matrix.rowSize()!=matrix.colSize()) {
                throw MtmExceptions::IllegalInitialization();
            }else {
                MtmMat<T> tmp(matrix);
                tmp.unlockTriag();
                for(size_t i(0) ; i<tmp.rowSize() ; ++i) {
                    for(size_t j(0) ; j<tmp.rowSize() ; ++j) {
                        (*this)[i][j] = tmp[i][j];
                    }
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
         * default destructor
         */
        ~MtmMatSq() = default;
        /*
         * default operator =
         */
        MtmMatSq& operator=(const MtmMatSq& matrix) = default;

        virtual void resize(
                Dimensions dim,
                const T& val = T()) override {               //DONE
            if((dim.getCol())!=(dim.getRow())) {
                throw MtmExceptions::ChangeMatFail(this->dimOfMat, dim);
            }else {
                MtmMat<T>::resize(dim, val);
            }
        }

        void reshape(Dimensions newDim) override {
            throw MtmExceptions::ChangeMatFail(this->dimOfMat, newDim);
        }

    };

}

#endif //EX3_MTMMATREC_H
