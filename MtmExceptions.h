/**
 * created by Tsuf Cohen and Safi Azmi 22.01.2019
 */
#ifndef EX3_MTMEXCEPTIONS_H
#define EX3_MTMEXCEPTIONS_H

#include <exception>
#include <string>
#include <iostream>
#include "Auxilaries.h"

using std::string;
using MtmMath::Dimensions;

namespace MtmMath {
    namespace MtmExceptions {
        class MtmExceptions : public std::exception {
        public:
            virtual const char* what() const throw() override {
                return "MtmError: Error unknown\n";
            }

            virtual ~MtmExceptions() throw() {}
        };

/*
 * Exception for illegal initialization of an object, needs to output
 * "MtmError: Illegal initialization values" in what() class function
 */
        class IllegalInitialization : public MtmExceptions {
        public:
            const char* what() const throw() override {
                return "MtmError: Illegal initialization values\n";
            }
        };

/*
 * Exception for Memory allocation failure for an object, needs to output
 * "MtmError: Out of memory" in what() class function
 */
        class OutOfMemory : public MtmExceptions {
        public:
            const char* what() const throw() override {
                return "MtmError: Out of memory\n";
            }
        };

/*
 * Exception for dimension mismatch during a mathematical function, needs to output
 * "MtmError: Dimension mismatch: (<mat 1 row>,<mat 1 col>) (<mat 2 row>,<mat 2 col>)"
 * in what() class function
 */
        class DimensionMismatch : public MtmExceptions {
            string mtm_error_message;
        public:
            DimensionMismatch(Dimensions dim1, Dimensions dim2) :
                    mtm_error_message(
                            "MtmError: Dimension mismatch: "+
                                    dim1.to_string()+
                                    dim2.to_string()+
                                    "\n") {}

            const char* what() const throw() override {
                return mtm_error_message.data();
            }
        };

/*
 * Exception for error for changing matrix/vector shape in reshape and resize, needs to output
 * "MtmError: Change matrix shape failed from: (<mat row>,<mat col>) (<new mat row>,<new mat col>)"
 * in what() class function
 */
        class ChangeMatFail : public MtmExceptions {
            string mtm_error_message;
        public:
            ChangeMatFail(Dimensions dim1, Dimensions dim2) : mtm_error_message(
                    "MtmError: Change matrix shape failed from: "+
                            dim1.to_string()+
                            dim2.to_string()+
                            "\n") {}

            const char* what() const throw() override {
                return mtm_error_message.data();
            }
        };

/*
 * Exception for accessing an illegal element in matrix or vector, needs to output
 * "MtmError: Attempt access to illegal element" in what() class function
 */
        class AccessIllegalElement : public MtmExceptions {
        public:
            const char* what() const throw() override {
                return "MtmError: Attempt access to illegal element\n";
            }
        };
    }
}


#endif //EX3_MTMEXCEPTIONS_H
