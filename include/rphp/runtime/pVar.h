/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * ***** END LICENSE BLOCK ***** */

#ifndef RPHP_PVAR_H_
#define RPHP_PVAR_H_

#include <iostream>
#include "rphp/runtime/pTypes.h"

// each pVar holds an int32 which stores both refcount (lower 31 bits)
// and the "is php reference" flag (high bit)
#define PVAR_RCNT_BITS 0x7fffffff
#define PVAR_RFLAG_BIT 0x80000000

namespace rphp {

/**
 @brief pVar is the basic container for a runtime variable

 It is implemented with a boost::variant, which is a type-safe, discriminated union.
 pVar can hold all of the basic runtime types

 - pNull
 - pBool
 - pInt
 - pFloat
 - pBString
 - pUString
 - pHash
 - pObject
 - pResource
 - pVarP (shared ptr to a pVar)

 Space for each type is stored on the stack
 
*/
class pVar {

    /**
        variant data
     */
    pVarDataType pVarData_;
    /**
        reference count (lower 31 bits) and reference flag (high bit)
     */
    boost::int32_t refData_;

public:

    /* constructors */

    /// default constructor holds a pNull
    pVar(void): pVarData_(pNull), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: default construct (pNull)" << std::endl;
#endif
    }

    /// generic constructor will accept any type that is valid for the pVar variant (see pVarDataType)
    template <typename T>
    pVar(const T &v): pVarData_(v), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: generic construct to: " << pVarData_.which() << std::endl;
#endif
    }

    // some specializations to avoid ambiguity

    /// construction from char* defaults to a binary strings
    pVar(const char* str): pVarData_(pBString(str)), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: binary string construct" << std::endl;
#endif
    }

    /// construction from int creates pInt
    pVar(int i): pVarData_(pInt(i)), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: int construct: " << i << std::endl;
#endif
    }

    /// convenience function for creating a new empty hash
    void newEmptyHash(void);

    /* default copy constructor */
#ifdef RPHP_PVAR_DEBUG
    pVar(pVar const& v) {
        std::cout << "pVar [" << this << "]: copy construct from type: " << v.getType() << std::endl;
        pVarData_ = v.pVarData_;
        refData_ = v.refData_;
    }
#endif

    /* default destructor */
#ifdef RPHP_PVAR_DEBUG
    ~pVar(void) {
        std::cout << "pVar [" << this << "]: destruct" << std::endl;
    }
#endif    

    /// generic assignment works for any type pVarDataType supports
    template <typename T>
    void operator=(T val) { pVarData_ = val; }

    // some specializations to avoid ambiguity
    /// default assignment from char* to binary strings
    void operator=(const char* str) { pVarData_ = pBString(str); }
    /// default assignment from int to pInt
    void operator=(int i) { pVarData_ = pInt(i); }

    /* reference counting counting */
    /// reference counting is handled automatically upon construction, destruction and
    /// assignment so this shouldn't normally be called in userland
    /// only useful when type is NOT pVarP
    /// in other words, we only count reference when we are the boxed pVar

    /// return current reference count.
    inline boost::int32_t getRefCount(void) {
        // note, refcount only makes sense if variant type is NOT pVarP
        return refData_ & PVAR_RCNT_BITS;
    }

    /// increment the reference count
    inline boost::int32_t incRefCount(void) {
        // TODO: overflow?
        refData_ += 1;
    }
    /// decrement the reference count
    inline boost::int32_t decRefCount(void) {
        refData_ -= 1;
    }

    /* reference variable flag */
    // note that all reference variables are pVarP, but not all pVarP are
    // reference variables. this happens because a pVar can live on the
    // heap but not have multiple runtime symbols pointing to it

    /// return true if this pVar is a "reference" variable
    inline bool isRef(void) {
        return refData_ & PVAR_RFLAG_BIT;
    }
    /// flag this pVar as a reference variable
    /// this only make sense when the variant type is pVarP
    inline void makeRef(void) {
        if (pVarData_.which() == pVarPtrType_)
            refData_ |= PVAR_RFLAG_BIT;
    }
    /// unflag this pVar as a reference variable
    inline void unmakeRef(void) {
        refData_ ^= PVAR_RFLAG_BIT;
    }

    /* custom visitors */
    /// apply a boost static visitor to the variant
    template <typename T>
    typename T::result_type applyVisitor() {
        return boost::apply_visitor( T(), pVarData_ );
    }

    /* type checks */
    /// return the current type represented by this pVar
    const pVarType getType() const;
    /// return true if pVar is currently a pNull. no type conversion.
    bool isNull() const {
        return ((pVarData_.which() == pVarTriStateType_) && pNull(boost::get<pTriState>(pVarData_)));
    }
    /// return true if pVar is currently a pBool. no type conversion.
    bool isBool() const {
        return ((pVarData_.which() == pVarTriStateType_) && !pNull(boost::get<pTriState>(pVarData_)));
    }
    /// return true if pVar is currently a pInt. no type conversion.
    bool isInt() const {
        return (pVarData_.which() == pVarIntType_);
    }
    /// return true if pVar is currently a pFloat. no type conversion.
    bool isFloat() const {
        return (pVarData_.which() == pVarFloatType_);
    }
    /// return true if pVar is currently a pBString. no type conversion.
    bool isBString() const {
        return (pVarData_.which() == pVarBStringType_);
    }
    /// return true if pVar is currently a pUString. no type conversion.
    bool isUString() const {
        return (pVarData_.which() == pVarUStringType_);
    }
    /// return true if pVar is currently a pHash. no type conversion.
    bool isHash() const {
        return (pVarData_.which() == pVarHashType_);
    }
    /// return true if pVar is currently a pObject. no type conversion.
    bool isObject() const {
        return (pVarData_.which() == pVarObjectType_);
    }
    /// return true if pVar is currently a pResource. no type conversion.
    bool isResource() const {
        return (pVarData_.which() == pVarResourceType_);
    }
    /// return true if pVar is currently a boxed pVar. no type conversion.
    bool isBoxed() const {
        return (pVarData_.which() == pVarPtrType_);
    }

    /// stream interface
    friend std::ostream& operator << (std::ostream& os, const rphp::pVar& v);

    /** @brief evaluate as boolean

        evaluates according to the language semantics for bool conversion.
        does not mutate.
        
     */
    bool evalAsBool() const;
    
    /* in place type conversion */
    /// free current value and represent a pNull instead
    void convertToNull();
    /// convert current value to a boolean. returns converted value.
    pTriState& convertToBool();
    /// convert current value to an integer. return converted value.
    pInt& convertToInt();
    // float
    /** @brief convert to binary or unicode string
    
       it may convert to a binary or unicode string, depending on runtime settings
       or it's current value, so you must check which it converted to after the conversion
     */
    void convertToString();
    /// force convertion to a binary string. if it was a unicode string, the conversion is lossy
    pBString& convertToBString();
    // ustring
    // hash
    // object
    // resource

    /* return type casted copy */
    // null
    // bool
    /// cast to pInt, does not mutate
    pInt copyAsInt() const;
    // float
    /// cast to pBString, does not mutate
    pBString copyAsBString() const;
    // ustring
    // hash
    // object
    // resource

    /* these do no conversions, and throw exception if the wrong type is accessed */
    /// bool accessor. throws exception if pVar is wrong type
    pTriState& getBool() {
        return boost::get<pTriState&>(pVarData_);
    }

    /// const bool accessor. throws exception if pVar is wrong type
    const pTriState& getBool() const {
        return boost::get<const pTriState&>(pVarData_);
    }
    
    /// pInt accessor. throws exception if pVar is wrong type
    pInt& getInt() {
        return boost::get<pInt&>(pVarData_);
    }

    /// const pInt accessor. throws exception if pVar is wrong type
    const pInt& getInt() const {
        return boost::get<const pInt&>(pVarData_);
    }

    /// pFloat accessor. throws exception if pVar is wrong type
    pFloat& getFloat() {
        return boost::get<pFloat&>(pVarData_);
    }

    /// const pFloat accessor. throws exception if pVar is wrong type
    const pFloat& getFloat() const {
        return boost::get<const pFloat&>(pVarData_);
    }

    /// pBString accessor. throws exception if pVar is wrong type
    pBString& getBString() {
        return boost::get<pBString&>(pVarData_);
    }

    /// const pBString accessor. throws exception if pVar is wrong type
    const pBString& getBString() const {
        return boost::get<const pBString&>(pVarData_);
    }

    /// pUString accessor. throws exception if pVar is wrong type
    pUString& getUString() {
        return boost::get<pUString&>(pVarData_);
    }

    /// const pBString accessor. throws exception if pVar is wrong type
    const pUString& getUString() const {
        return boost::get<const pUString&>(pVarData_);
    }

    /// pHash accessor. throws exception if pVar is wrong type
    pHashP& getHash() {
        return boost::get<pHashP&>(pVarData_);
    }

    /// const pHash accessor. throws exception if pVar is wrong type
    const pHashP& getHash() const {
        return boost::get<const pHashP&>(pVarData_);
    }

    /// read only hash access
    const pHashP& getConstHash() const {
        return boost::get<const pHashP&>(pVarData_);
    }

    /// pObject accessor. throws exception if pVar is wrong type
    pObjectP& getObject() {
        return boost::get<pObjectP&>(pVarData_);
    }
    
    /// const pObject accessor. throws exception if pVar is wrong type
    const pObjectP& getObject() const {
        return boost::get<const pObjectP&>(pVarData_);
    }

    /// pResource accessor. throws exception if pVar is wrong type
    pResourceP& getResource() {
        return boost::get<pResourceP&>(pVarData_);
    }
    
    /// const pResource accessor. throws exception if pVar is wrong type
    const pResourceP& getResource() const {
        return boost::get<const pResourceP&>(pVarData_);
    }

    /// boxed pVar accessor. throws exception if pVar is wrong type
    pVarP& getPtr() {
        return boost::get<pVarP&>(pVarData_);
    }
    
    /// boxed pVar accessor (const). throws exception if pVar is wrong type
    const pVarP& getPtr() const {
        return boost::get<const pVarP&>(pVarData_);
    }

};

// used by boost::instrusive_ptr to handle ref counting
void intrusive_ptr_add_ref(pVar* v);
void intrusive_ptr_release(pVar* v);

} /* namespace rphp */

#endif /* RPHP_PVAR_H_ */