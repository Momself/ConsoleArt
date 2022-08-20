// Tencent is pleased to support the open source community by making RapidJSON available.
// 
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.

#ifndef RAPIDJSON_DOCUMENT_H_
#define RAPIDJSON_DOCUMENT_H_

/*! \file document.h */

#include "reader.h"
#include "internal/meta.h"
#include "internal/strfunc.h"
#include "memorystream.h"
#include "encodedstream.h"
#include <new>      // placement new
#include <limits>

RAPIDJSON_DIAG_PUSH
#ifdef __clang__
RAPIDJSON_DIAG_OFF(padded)
RAPIDJSON_DIAG_OFF(switch-enum)
RAPIDJSON_DIAG_OFF(c++98-compat)
#elif defined(_MSC_VER)
RAPIDJSON_DIAG_OFF(4127) // conditional expression is constant
RAPIDJSON_DIAG_OFF(4244) // conversion from kXxxFlags to 'uint16_t', possible loss of data
#endif

#ifdef __GNUC__
RAPIDJSON_DIAG_OFF(effc++)
#if __GNUC__ >= 6
RAPIDJSON_DIAG_OFF(terminate) // ignore throwing RAPIDJSON_ASSERT in RAPIDJSON_NOEXCEPT functions
#endif
#endif // __GNUC__

#ifndef RAPIDJSON_NOMEMBERITERATORCLASS
#include <iterator> // std::random_access_iterator_tag
#endif

#if RAPIDJSON_HAS_CXX11_RVALUE_REFS
#include <utility> // std::move
#endif

RAPIDJSON_NAMESPACE_BEGIN

// Forward declaration.
template <typename Encoding, typename Allocator>
class GenericValue;

template <typename Encoding, typename Allocator, typename StackAllocator>
class GenericDocument;

//! Name-value pair in a JSON object value.
/*!
    This class was internal to GenericValue. It used to be a inner struct.
    But a compiler (IBM XL C/C++ for AIX) have reported to have problem with that so it moved as a namespace scope struct.
    https://code.google.com/p/rapidjson/issues/detail?id=64
*/
template <typename Encoding, typename Allocator> 
struct GenericMember { 
    GenericValue<Encoding, Allocator> name;     //!< name of member (must be a string)
    GenericValue<Encoding, Allocator> value;    //!< value of member.
};

///////////////////////////////////////////////////////////////////////////////
// GenericMemberIterator

#ifndef RAPIDJSON_NOMEMBERITERATORCLASS

//! (Constant) member iterator for a JSON object value
/*!
    \tparam Const Is this a constant iterator?
    \tparam Encoding    Encoding of the value. (Even non-string values need to have the same encoding in a document)
    \tparam Allocator   Allocator type for allocating memory of object, array and string.

    This class implements a Random Access Iterator for GenericMember elements
    of a GenericValue, see ISO/IEC 14882:2003(E) C++ standard, 24.1 [lib.iterator.requirements].

    \note This iterator implementation is mainly intended to avoid implicit
        conversions from iterator values to \c NULL,
        e.g. from GenericValue::FindMember.

    \note Define \c RAPIDJSON_NOMEMBERITERATORCLASS to fall back to a
        pointer-based implementation, if your platform doesn't provide
        the C++ <iterator> header.

    \see GenericMember, GenericValue::MemberIterator, GenericValue::ConstMemberIterator
 */
template <bool Const, typename Encoding, typename Allocator>
class GenericMemberIterator {

    friend class GenericValue<Encoding,Allocator>;
    template <bool, typename, typename> friend class GenericMemberIterator;

    typedef GenericMember<Encoding,Allocator> PlainType;
    typedef typename internal::MaybeAddConst<Const,PlainType>::Type ValueType;

public:
    //! Iterator type itself
    typedef GenericMemberIterator Iterator;
    //! Constant iterator type
    typedef GenericMemberIterator<true,Encoding,Allocator>  ConstIterator;
    //! Non-constant iterator type
    typedef GenericMemberIterator<false,Encoding,Allocator> NonConstIterator;

    /** \name std::iterator_traits support */
    //@{
    typedef ValueType      value_type;
    typedef ValueType *    pointer;
    typedef ValueType &    reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
    //@}

    //! Pointer to (const) GenericMember
    typedef pointer         Pointer;
    //! Reference to (const) GenericMember
    typedef reference       Reference;
    //! Signed integer type (e.g. \c ptrdiff_t)
    typedef difference_type DifferenceType;

    //! Default constructor (singular value)
    /*! Creates an iterator pointing to no element.
        \note All operations, except for comparisons, are undefined on such values.
     */
    GenericMemberIterator() : ptr_() {}

    //! Iterator conversions to more const
    /*!
        \param it (Non-const) iterator to copy from

        Allows the creation of an iterator from another GenericMemberIterator
        that is "less const".  Especially, creating a non-constant iterator
        from a constant iterator are disabled:
        \li const -> non-const (not ok)
        \li const -> const (ok)
        \li non-const -> const (ok)
        \li non-const -> non-const (ok)

        \note If the \c Const template parameter is already \c false, this
            constructor effectively defines a regular copy-constructor.
            Otherwise, the copy constructor is implicitly defined.
    */
    GenericMemberIterator(const NonConstIterator & it) : ptr_(it.ptr_) {}
    Iterator& operator=(const NonConstIterator & it) { ptr_ = it.ptr_; return *this; }

    //! @name stepping
    //@{
    Iterator& operator++(){ ++ptr_; return *this; }
    Iterator& operator--(){ --ptr_; return *this; }
    Iterator  operator++(int){ Iterator old(*this); ++ptr_; return old; }
    Iterator  operator--(int){ Iterator old(*this); --ptr_; return old; }
    //@}

    //! @name increment/decrement
    //@{
    Iterator operator+(DifferenceType n) const { return Iterator(ptr_+n); }
    Iterator operator-(DifferenceType n) const { return Iterator(ptr_-n); }

    Iterator& operator+=(DifferenceType n) { ptr_+=n; return *this; }
    Iterator& operator-=(DifferenceType n) { ptr_-=n; return *this; }
    //@}

    //! @name relations
    //@{
    bool operator==(ConstIterator that) const { return ptr_ == that.ptr_; }
    bool operator!=(ConstIterator that) const { return ptr_ != that.ptr_; }
    bool operator<=(ConstIterator that) const { return ptr_ <= that.ptr_; }
    bool operator>=(ConstIterator that) const { return ptr_ >= that.ptr_; }
    bool operator< (ConstIterator that) const { return ptr_ < that.ptr_; }
    bool operator> (ConstIterator that) const { return ptr_ > that.ptr_; }
    //@}

    //! @name dereference
    //@{
    Reference operator*() const { return *ptr_; }
    Pointer   operator->() const { return ptr_; }
    Reference operator[](DifferenceType n) const { return ptr_[n]; }
    //@}

    //! Distance
    DifferenceType operator-(ConstIterator that) const { return ptr_-that.ptr_; }

private:
    //! Internal constructor from plain pointer
    explicit GenericMemberIterator(Pointer p) : ptr_(p) {}

    Pointer ptr_; //!< raw pointer
};

#else // RAPIDJSON_NOMEMBERITERATORCLASS

// class-based member iterator implementation disabled, use plain pointers

template <bool Const, typename Encoding, typename Allocator>
struct GenericMemberIterator;

//! non-const GenericMemberIterator
template <typename Encoding, typename Allocator>
struct GenericMemberIterator<false,Encoding,Allocator> {
    //! use plain pointer as iterator type
    typedef GenericMember<Encoding,Allocator>* Iterator;
};
//! const GenericMemberIterator
template <typename Encoding, typename Allocator>
struct GenericMemberIterator<true,Encoding,Allocator> {
    //! use plain const pointer as iterator type
    typedef const GenericMember<Encoding,Allocator>* Iterator;
};

#endif // RAPIDJSON_NOMEMBERITERATORCLASS

///////////////////////////////////////////////////////////////////////////////
// GenericStringRef

//! Reference to a constant string (not taking a copy)
/*!
    \tparam CharType character type of the string

    This helper class is used to automatically infer constant string
    references for string literals, especially from \c const \b (!)
    character arrays.

    The main use is for creating JSON string values without copying the
    source string via an \ref Allocator.  This requires that the referenced
    string pointers have a sufficient lifetime, which exceeds the lifetime
    of the associated GenericValue.

    \b Example
    \code
    Value v("foo");   // ok, no need to copy & calculate length
    const char foo[] = "foo";
    v.SetString(foo); // ok

    const char* bar = foo;
    // Value x(bar); // not ok, can't rely on bar's lifetime
    Value x(StringRef(bar)); // lifetime explicitly guaranteed by user
    Value y(StringRef(bar, 3));  // ok, explicitly pass length
    \endcode

    \see StringRef, GenericValue::SetString
*/
template<typename CharType>
struct GenericStringRef {
    typedef CharType Ch; //!< character type of the string

    //! Create string reference from \c const character array
#ifndef __clang__ // -Wdocumentation
    /*!
        This constructor implicitly creates a constant string reference from
        a \c const character array.  It has better performance than
        \ref StringRef(const CharType*) by inferring the string \ref length
        from the array length, and also supports strings containing null
        characters.

        \tparam N length of the string, automatically inferred

        \param str Constant character array, lifetime assumed to be longer
            than the use of the string in e.g. a GenericValue

        \post \ref s == str

        \note Constant complexity.
        \note There is a hidden, private overload to disallow references to
            non-const character arrays to be created via this constructor.
            By this, e.g. function-scope arrays used to be filled via
            \c snprintf are excluded from consideration.
            In such cases, the referenced string should be \b copied to the
            GenericValue instead.
     */
#endif
    template<SizeType N>
    GenericStringRef(const CharType (&str)[N]) RAPIDJSON_NOEXCEPT
        : s(str), length(N-1) {}

    //! Explicitly create string reference from \c const character pointer
#ifndef __clang__ // -Wdocumentation
    /*!
        This constructor can be used to \b explicitly  create a reference to
        a constant string pointer.

        \see StringRef(const CharType*)

        \param str Constant character pointer, lifetime assumed to be longer
            than the use of the string in e.g. a GenericValue

        \post \ref s == str

        \note There is a hidden, private overload to disallow references to
            non-const character arrays to be created via this constructor.
            By this, e.g. function-scope arrays used to be filled via
            \c snprintf are excluded from consideration.
            In such cases, the referenced string should be \b copied to the
            GenericValue instead.
     */
#endif
    explicit GenericStringRef(const CharType* str)
        : s(str), length(NotNullStrLen(str)) {}

    //! Create constant string reference from pointer and length
#ifndef __clang__ // -Wdocumentation
    /*! \param str constant string, lifetime assumed to be longer than the use of the string in e.g. a GenericValue
        \param len length of the string, excluding the trailing NULL terminator

        \post \ref s == str && \ref length == len
        \note Constant complexity.
     */
#endif
    GenericStringRef(const CharType* str, SizeType len)
        : s(RAPIDJSON_LIKELY(str) ? str : emptyString), length(len) { RAPIDJSON_ASSERT(str != 0 || len == 0u); }

    GenericStringRef(const GenericStringRef& rhs) : s(rhs.s), length(rhs.length) {}

    //! implicit conversion to plain CharType pointer
    operator const Ch *() const { return s; }

    const Ch* const s; //!< plain CharType pointer
    const SizeType length; //!< length of the string (excluding the trailing NULL terminator)

private:
    SizeType NotNullStrLen(const CharType* str) {
        RAPIDJSON_ASSERT(str != 0);
        return internal::StrLen(str);
    }

    /// Empty string - used when passing in a NULL pointer
    static const Ch emptyString[];

    //! Disallow construction from non-const array
    template<SizeType N>
    GenericStringRef(CharType (&str)[N]) /* = delete */;
    //! Copy assignment operator not permitted - immutable type
    GenericStringRef& operator=(const GenericStringRef& rhs) /* = delete */;
};

template<typename CharType>
const CharType GenericStringRef<CharType>::emptyString[] = { CharType() };

//! Mark a character pointer as constant string
/*! Mark a plain character pointer as a "string literal".  This function
    can be used to avoid copying a character string to be referenced as a
    value in a JSON GenericValue object, if the string's lifetime is known
    to be valid long enough.
    \tparam CharType Character type of the string
    \param str Constant string, lifetime assumed to be longer than the use of the string in e.g. a GenericValue
    \return GenericStringRef string reference object
    \relatesalso GenericStringRef

    \see GenericValue::GenericValue(StringRefType), GenericValue::operator=(StringRefType), GenericValue::SetString(StringRefType), GenericValue::PushBack(StringRefType, Allocator&), GenericValue::AddMember
*/
template<typename CharType>
inline GenericStringRef<CharType> StringRef(const CharType* str) {
    return GenericStringRef<CharType>(str);
}

//! Mark a character pointer as constant string
/*! Mark a plain character pointer as a "string literal".  This function
    can be used to avoid copying a character string to be referenced as a
    value in a JSON GenericValue object, if the string's lifetime is known
    to be valid long enough.

    This version has better performance with supplied length, and also
    supports string containing null characters.

    \tparam CharType character type of the string
    \param str Constant string, lifetime assumed to be longer than the use of the string in e.g. a GenericValue
    \param length The length of source string.
    \return GenericStringRef string reference object
    \relatesalso GenericStringRef
*/
template<typename CharType>
inline GenericStringRef<CharType> StringRef(const CharType* str, size_t length) {
    return GenericStringRef<CharType>(str, SizeType(length));
}

#if RAPIDJSON_HAS_STDSTRING
//! Mark a string object as constant string
/*! Mark a string object (e.g. \c std::string) as a "string literal".
    This function can be used to avoid copying a string to be referenced as a
    value in a JSON GenericValue object, if the string's lifetime is known
    to be valid long enough.

    \tparam CharType character type of the string
    \param str Constant string, lifetime assumed to be longer than the use of the string in e.g. a GenericValue
    \return GenericStringRef string reference object
    \relatesalso GenericStringRef
    \note Requires the definition of the preprocessor symbol \ref RAPIDJSON_HAS_STDSTRING.
*/
template<typename CharType>
inline GenericStringRef<CharType> StringRef(const std::basic_string<CharType>& str) {
    return GenericStringRef<CharType>(str.data(), SizeType(str.size()));
}
#endif

///////////////////////////////////////////////////////////////////////////////
// GenericValue type traits
namespace internal {

template <typename T, typename Encoding = void, typename Allocator = void>
struct IsGenericValueImpl : FalseType {};

// select candidates according to nested encoding and allocator types
template <typename T> struct IsGenericValueImpl<T, typename Void<typename T::EncodingType>::Type, typename Void<typename T::AllocatorType>::Type>
    : IsBaseOf<GenericValue<typename T::EncodingType, typename T::AllocatorType>, T>::Type {};

// helper to match arbitrary GenericValue instantiations, including derived classes
template <typename T> struct IsGenericValue : IsGenericValueImpl<T>::Type {};

} // namespace internal

///////////////////////////////////////////////////////////////////////////////
// TypeHelper

namespace internal {

template <typename ValueType, typename T>
struct TypeHelper {};

template<typename ValueType> 
struct TypeHelper<ValueType, bool> {
    static bool Is(const ValueType& v) { return v.IsBool(); }
    static bool Get(const ValueType& v) { return v.GetBool(); }
    static ValueType& Set(ValueType& v, bool data) { return v.SetBool(data); }
    static ValueType& Set(ValueType& v, bool data, typename ValueType::AllocatorType&) { return v.SetBool(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, int> {
    static bool Is(const ValueType& v) { return v.IsInt(); }
    static int Get(const ValueType& v) { return v.GetInt(); }
    static ValueType& Set(ValueType& v, int data) { return v.SetInt(data); }
    static ValueType& Set(ValueType& v, int data, typename ValueType::AllocatorType&) { return v.SetInt(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, unsigned> {
    static bool Is(const ValueType& v) { return v.IsUint(); }
    static unsigned Get(const ValueType& v) { return v.GetUint(); }
    static ValueType& Set(ValueType& v, unsigned data) { return v.SetUint(data); }
    static ValueType& Set(ValueType& v, unsigned data, typename ValueType::AllocatorType&) { return v.SetUint(data); }
};

#ifdef _MSC_VER
RAPIDJSON_STATIC_ASSERT(sizeof(long) == sizeof(int));
template<typename ValueType>
struct TypeHelper<ValueType, long> {
    static bool Is(const ValueType& v) { return v.IsInt(); }
    static long Get(const ValueType& v) { return v.GetInt(); }
    static ValueType& Set(ValueType& v, long data) { return v.SetInt(data); }
    static ValueType& Set(ValueType& v, long data, typename ValueType::AllocatorType&) { return v.SetInt(data); }
};

RAPIDJSON_STATIC_ASSERT(sizeof(unsigned long) == sizeof(unsigned));
template<typename ValueType>
struct TypeHelper<ValueType, unsigned long> {
    static bool Is(const ValueType& v) { return v.IsUint(); }
    static unsigned long Get(const ValueType& v) { return v.GetUint(); }
    static ValueType& Set(ValueType& v, unsigned long data) { return v.SetUint(data); }
    static ValueType& Set(ValueType& v, unsigned long data, typename ValueType::AllocatorType&) { return v.SetUint(data); }
};
#endif

template<typename ValueType> 
struct TypeHelper<ValueType, int64_t> {
    static bool Is(const ValueType& v) { return v.IsInt64(); }
    static int64_t Get(const ValueType& v) { return v.GetInt64(); }
    static ValueType& Set(ValueType& v, int64_t data) { return v.SetInt64(data); }
    static ValueType& Set(ValueType& v, int64_t data, typename ValueType::AllocatorType&) { return v.SetInt64(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, uint64_t> {
    static bool Is(const ValueType& v) { return v.IsUint64(); }
    static uint64_t Get(const ValueType& v) { return v.GetUint64(); }
    static ValueType& Set(ValueType& v, uint64_t data) { return v.SetUint64(data); }
    static ValueType& Set(ValueType& v, uint64_t data, typename ValueType::AllocatorType&) { return v.SetUint64(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, double> {
    static bool Is(const ValueType& v) { return v.IsDouble(); }
    static double Get(const ValueType& v) { return v.GetDouble(); }
    static ValueType& Set(ValueType& v, double data) { return v.SetDouble(data); }
    static ValueType& Set(ValueType& v, double data, typename ValueType::AllocatorType&) { return v.SetDouble(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, float> {
    static bool Is(const ValueType& v) { return v.IsFloat(); }
    static float Get(const ValueType& v) { return v.GetFloat(); }
    static ValueType& Set(ValueType& v, float data) { return v.SetFloat(data); }
    static ValueType& Set(ValueType& v, float data, typename ValueType::AllocatorType&) { return v.SetFloat(data); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, const typename ValueType::Ch*> {
    typedef const typename ValueType::Ch* StringType;
    static bool Is(const ValueType& v) { return v.IsString(); }
    static StringType Get(const ValueType& v) { return v.GetString(); }
    static ValueType& Set(ValueType& v, const StringType data) { return v.SetString(typename ValueType::StringRefType(data)); }
    static ValueType& Set(ValueType& v, const StringType data, typename ValueType::AllocatorType& a) { return v.SetString(data, a); }
};

#if RAPIDJSON_HAS_STDSTRING
template<typename ValueType> 
struct TypeHelper<ValueType, std::basic_string<typename ValueType::Ch> > {
    typedef std::basic_string<typename ValueType::Ch> StringType;
    static bool Is(const ValueType& v) { return v.IsString(); }
    static StringType Get(const ValueType& v) { return StringType(v.GetString(), v.GetStringLength()); }
    static ValueType& Set(ValueType& v, const StringType& data, typename ValueType::AllocatorType& a) { return v.SetString(data, a); }
};
#endif

template<typename ValueType> 
struct TypeHelper<ValueType, typename ValueType::Array> {
    typedef typename ValueType::Array ArrayType;
    static bool Is(const ValueType& v) { return v.IsArray(); }
    static ArrayType Get(ValueType& v) { return v.GetArray(); }
    static ValueType& Set(ValueType& v, ArrayType data) { return v = data; }
    static ValueType& Set(ValueType& v, ArrayType data, typename ValueType::AllocatorType&) { return v = data; }
};

template<typename ValueType> 
struct TypeHelper<ValueType, typename ValueType::ConstArray> {
    typedef typename ValueType::ConstArray ArrayType;
    static bool Is(const ValueType& v) { return v.IsArray(); }
    static ArrayType Get(const ValueType& v) { return v.GetArray(); }
};

template<typename ValueType> 
struct TypeHelper<ValueType, typename ValueType::Object> {
    typedef typename ValueType::Object ObjectType;
    static bool Is(const ValueType& v) { return v.IsObject(); }
    static ObjectType Get(ValueType& v) { return v.GetObject(); }
    static ValueType& Set(ValueType& v, ObjectType data) { return v = data; }
    static ValueType& Set(ValueType& v, ObjectType data, typename ValueType::AllocatorType&) { return v = data; }
};

template<typename ValueType> 
struct TypeHelper<ValueType, typename ValueType::ConstObject> {
    typedef typename ValueType::ConstObject ObjectType;
    static bool Is(const ValueType& v) { return v.IsObject(); }
    static ObjectType Get(const ValueType& v) { return v.GetObject(); }
};

} // namespace internal

// Forward declarations
template <bool, typename> class GenericArray;
template <bool, typename> class GenericObject;

///////////////////////////////////////////////////////////////////////////////
// GenericValue

//! Represents a JSON value. Use Value for UTF8 encoding and default allocator.
/*!
    A JSON value can be one of 7 types. This class is a variant type supporting
    these types.

    Use the Value if UTF8 and default allocator

    \tparam Encoding    Encoding of the value. (Even non-string values need to have the same encoding in a document)
    \tparam Allocator   Allocator type for allocating memory of object, array and string.
*/
template <typename Encoding, typename Allocator = MemoryPoolAllocator<> > 
class GenericValue {
public:
    //! Name-value pair in an object.
    typedef GenericMember<Encoding, Allocator> Member;
    typedef Encoding EncodingType;                  //!< Encoding type from template parameter.
    typedef Allocator AllocatorType;                //!< Allocator type from template parameter.
    typedef typename Encoding::Ch Ch;               //!< Character type derived from Encoding.
    typedef GenericStringRef<Ch> StringRefType;     //!< Reference to a constant string
    typedef typename GenericMemberIterator<false,Encoding,Allocator>::Iterator MemberIterator;  //!< Member iterator for iterating in object.
    typedef typename GenericMemberIterator<true,Encoding,Allocator>::Iterator ConstMemberIterator;  //!< Constant member iterator for iterating in object.
    typedef GenericValue* ValueIterator;            //!< Value iterator for iterating in array.
    typedef const GenericValue* ConstValueIterator; //!< Constant value iterator for iterating in array.
    typedef GenericValue<Encoding, Allocator> ValueType;    //!< Value type of itself.
    typedef GenericArray<false, ValueType> Array;
    typedef GenericArray<true, ValueType> ConstArray;
    typedef GenericObject<false, ValueType> Object;
    typedef GenericObject<true, ValueType> ConstObject;

    //!@name Constructors and destructor.
    //@{

    //! Default constructor creates a null value.
    GenericValue() RAPIDJSON_NOEXCEPT : data_() { data_.f.flags = kNullFlag; }

#if RAPIDJSON_HAS_CXX11_RVALUE_REFS
    //! Move constructor in C++11
    GenericValue(GenericValue&& rhs) RAPIDJSON_NOEXCEPT : data_(rhs.data_) {
        rhs.data_.f.flags = kNullFlag; // give up contents
    }
#endif

private:
    //! Copy constructor is not permitted.
    GenericValue(const GenericValue& rhs);

#if RAPIDJSON_HAS_CXX11_RVALUE_REFS
    //! Moving from a GenericDocument is not permitted.
    template <typename StackAllocator>
    GenericValue(GenericDocument<Encoding,Allocator,StackAllocator>&& rhs);

    //! Move assignment from a GenericDocument is not permitted.
    template <typename StackAllocator>
    GenericValue& operator=(GenericDocument<Encoding,Allocator,StackAllocator>&& rhs);
#endif

public:

    //! Constructor with JSON value type.
    /*! This creates a Value of specified type with default content.
        \param type Type of the value.
        \note Default content for number is zero.
    */
    explicit GenericValue(Type type) RAPIDJSON_NOEXCEPT : data_() {
        static const uint16_t defaultFlags[7] = {
            kNullFlag, kFalseFlag, kTrueFlag, kObjectFlag, kArrayFlag, kShortStringFlag,
            kNumberAnyFlag
        };
        RAPIDJSON_ASSERT(type >= kNullType && type <= kNumberType);
        data_.f.flags = defaultFlags[type];

        // Use ShortString to store empty string.
        if (type == kStringType)
            data_.ss.SetLength(0);
    }

    //! Explicit copy constructor (with allocator)
    /*! Creates a copy of a Value by using the given Allocator
        \tparam SourceAllocator allocator of \c rhs
        \param rhs Value to copy from (read-only)
        \param allocator Allocator for allocating copied elements and buffers. Commonly use GenericDocument::GetAllocator().
        \param copyConstStrings Force copying of constant strings (e.g. referencing an in-situ buffer)
        \see CopyFrom()
    */
    template <typename SourceAllocator>
    GenericValue(const GenericValue<Encoding,SourceAllocator>& rhs, Allocator& allocator, bool copyConstStrings = false) {
        switch (rhs.GetType()) {
        case kObjectType: {
                SizeType count = rhs.data_.o.size;
                Member* lm = reinterpret_cast<Member*>(allocator.Malloc(count * sizeof(Member)));
                const typename GenericValue<Encoding,SourceAllocator>::Member* rm = rhs.GetMembersPointer();
                for (SizeType i = 0; i < count; i++) {
                    new (&lm[i].name) GenericValue(rm[i].name, allocator, copyConstStrings);
                    new (&lm[i].value) GenericValue(rm[i].value, allocator, copyConstStrings);
                }
                data_.f.flags = kObjectFlag;
                data_.o.size = data_.o.capacity = count;
                SetMembersPointer(lm);
            }
            break;
        case kArrayType: {
                SizeType count = rhs.data_.a.size;
                GenericValue* le = reinterpret_cast<GenericValue*>(allocator.Malloc(count * sizeof(GenericValue)));
                const GenericValue<Encoding,SourceAllocator>* re = rhs.GetElementsPointer();
                for (SizeType i = 0; i < count; i++)
                    new (&le[i]) GenericValue(re[i], allocator, copyConstStrings);
                data_.f.flags = kArrayFlag;
                data_.a.size = data_.a.capacity = count;
                SetElementsPointer(le);
            }
            break;
        case kStringType:
            if (rhs.data_.f.flags == kConstStringFlag && !copyConstStrings) {
                data_.f.flags = rhs.data_.f.flags;
                data_  = *reinterpret_cast<const Data*>(&rhs.data_);
            }
            else
                SetStringRaw(StringRef(rhs.GetString(), rhs.GetStringLength()), allocator);
            break;
        default:
            data_.f.flags = rhs.data_.f.flags;
            data_  = *reinterpret_cast<const Data*>(&rhs.data_);
            break;
        }
    }

    //! Constructor for boolean value.
    /*! \param b Boolean value
        \note This constructor is limited to \em real boolean values and rejects
            implicitly converted types like arbitrary pointers.  Use an explicit cast
            to \c bool, if you want to construct a boolean JSON value in such cases.
     */
#ifndef RAPIDJSON_DOXYGEN_RUNNING // hide SFINAE from Doxygen
    template <typename T>
    explicit GenericValue(T b, RAPIDJSON_ENABLEIF((internal::IsSame<bool, T>))) RAPIDJSON_NOEXCEPT  // See #472
#else
    explicit GenericValue(bool b) RAPIDJSON_NOEXCEPT
#endif
        : data_() {
            // safe-guard against failing SFINAE
            RAPIDJSON_STATIC_ASSERT((internal::IsSame<bool,T>::Value));
            data_.f.flags = b ? kTrueFlag : kFalseFlag;
    }

    //! Constructor for int value.
    explicit GenericValue(int i) RAPIDJSON_NOEXCEPT : data_() {
        data_.n.i64 = i;
        data_.f.flags = (i >= 0) ? (kNumberIntFlag | kUintFlag | kUint64Flag) : kNumberIntFlag;
    }

    //! Constructor for unsigned value.
    explicit GenericValue(unsigned u) RAPIDJSON_NOEXCEPT : data_() {
        data_.n.u64 = u; 
        data_.f.flags = (u & 0x80000000) ? kNumberUintFlag : (kNumberUintFlag | kIntFlag | kInt64Flag);
    }

    //! Constructor for int64_t value.
    explicit GenericValue(int64_t i64) RAPIDJSON_NOEXCEPT : data_() {
        data_.n.i64 = i64;
        data_.f.flags = kNumberInt64Flag;
        if (i64 >= 0) {
            data_.f.flags |= kNumberUint64Flag;
            if (!(static_cast<uint64_t>(i64) & RAPIDJSON_UINT64_C2(0xFFFFFFFF, 0x00000000)))
                data_.f.flags |= kUintFlag;
            if (!(static_cast<uint64_t>(i64) & RAPIDJSON_UINT64_C2(0xFFFFFFFF, 0x80000000)))
                data_.f.flags |= kIntFlag;
        }
        else if (i64 >= static_cast<int64_t>(RAPIDJSON_UINT64_C2(0xFFFFFFFF, 0x80000000)))
            data_.f.flags |= kIntFlag;
    }

    //! Constructor for uint64_t value.
    explicit GenericValue(uint64_t u64) RAPIDJSON_NOEXCEPT : data_() {
        data_.n.u64 = u64;
        data_.f.flags = kNumberUint64Flag;
        if (!(u64 & RAPIDJSON_UINT64_C2(0x80000000, 0x00000000)))
            data_.f.flags |= kInt64Flag;
        if (!(u64 & RAPIDJSON_UINT64_C2(0xFFFFFFFF, 0x00000000)))
            data_.f.flags |= kUintFlag;
        if (!(u64 & RAPIDJSON_UINT64_C2(0xFFFFFFFF, 0x80000000)))
            data_.f.flags |= kIntFlag;
    }

    //! Constructor for double value.
    explicit GenericValue(double d) RAPIDJSON_NOEXCEPT : data_() { data_.n.d = d; data_.f.flags = kNumberDoubleFlag; }

    //! Constructor for float value.
    explicit GenericValue(float f) RAPIDJSON_NOEXCEPT : data_() { data_.n.d = static_cast<double>(f); data_.f.flags = kNumberDoubleFlag; }

    //! Constructor for constant string (i.e. do not make a copy of string)
    GenericValue(const Ch* s, SizeType length) RAPIDJSON_NOEXCEPT : data_() { SetStringRaw(StringRef(s, length)); }

    //! Constructor for constant string (i.e. do not make a copy of string)
    explicit GenericValue(StringRefType s) RAPIDJSON_NOEXCEPT : data_() { SetStringRaw(s); }

    //! Constructor for copy-string (i.e. do make a copy of string)
    GenericValue(const Ch* s, SizeType length, Allocator& allocator) : data_() { SetStringRaw(StringRef(s, length), allocator); }

    //! Constructor for copy-string (i.e. do make a copy of string)
    GenericValue(const Ch*s, Allocator& allocator) : data_() { SetStringRaw(StringRef(s), allocator); }

#if RAPIDJSON_HAS_STDSTRING
    //! Constructor for copy-string from a string object (i.e. do make a copy of string)
    /*! \note Requires the definition of the preprocessor symbol \ref RAPIDJSON_HAS_STDSTRING.
     */
    GenericValue(const std::basic_string<Ch>& s, Allocator& allocator) : data_() { SetStringRaw(StringRef(s), allocator); }
#endif

    //! Constructor for Array.
    /*!
        \param a An array obtained by \c GetArray().
        \note \c Array is always pass-by-value.
        \note the source array is moved into this value and the sourec array becomes empty.
    */
    GenericValue(Array a) RAPIDJSON_NOEXCEPT : data_(a.value_.data_) {
        a.value_.data_ = Data();
        a.value_.data_.f.flags = kArrayFlag;
    }

    //! Constructor for Object.
    /*!
        \param o An object obtained by \c GetObject().
        \note \c Object is always pass-by-value.
        \note the source object is moved into this value and the sourec object becomes empty.
    */
    GenericValue(Object o) RAPIDJSON_NOEXCEPT : data_(o.value_.data_) {
        o.value_.data_ = Data();
        o.value_.data_.f.flags = kObjectFlag;
    }

    //! Destructor.
    /*! Need to destruct elements of array, members of object, or copy-string.
    */
    ~GenericValue() {
        if (Allocator::kNeedFree) { // Shortcut by Allocator's trait
            switch(data_.f.flags) {
            case kArrayFlag:
                {
                    GenericValue* e = GetElementsPointer();
                    for (GenericValue* v = e; v != e + data_.a.size; ++v)
                        v->~GenericValue();
                    Allocator::Free(e);
                }
                break;

            case kObjectFlag:
                for (MemberIterator m = MemberBegin(); m != MemberEnd(); ++m)
                    m->~Member();
                Allocator::Free(GetMembersPointer());
                break;

            case kCopyStringFlag:
                Allocator::Free(const_cast<Ch*>(GetStringPointer()));
                break;

            default:
                break;  // Do nothing for other types.
            }
        }
    }

    //@}

    //!@name Assignment operators
    //@{

    //! Assignment with move semantics.
    /*! \param rhs Source of the assignment. It will become a null value after assignment.
    */
    GenericValue& operator=(GenericValue& rhs) RAPIDJSON_NOEXCEPT {
        RAPIDJSON_ASSERT(this != &rhs);
        this->~GenericValue();
        RawAssign(rhs);
        return *this;
    }

#if RAPIDJSON_HAS_CXX11_RVALUE_REFS
    //! Move assignment in C++11
    GenericValue& operator=(GenericValue&& rhs) RAPIDJSON_NOEXCEPT {
        return *this = rhs.Move();
    }
#endif

    //! Assignment of constant string reference (no copy)
    /*! \param str Constant string reference to be assigned
        \note This overload is needed to avoid clashes with the generic primitive type assignment overload below.
        \see GenericStringRef, operator=(T)
    */
    GenericValue& operator=(StringRefType str) RAPIDJSON_NOEXCEPT {
        GenericValue s(str);
        return *this = s;
    }

    //! Assignment with primitive types.
    /*! \tparam T Either \ref Type, \c int, \c unsigned, \c int64_t, \c uint64_t
        \param value The value to be assigned.

        \note The source type \c T explicitly disallows all pointer types,
            especially (\c const) \ref Ch*.  This helps avoiding implicitly
            referencing character strings with insufficient lifetime, use
            \ref SetString(const Ch*, Allocator&) (for copying) or
            \ref StringRef() (to explicitly mark the pointer as constant) instead.
            All other pointer types would implicitly convert to \c bool,
            use \ref SetBool() instead.
    */
    template <typename T>
    RAPIDJSON_DISABLEIF_RETURN((internal::IsPointer<T>), (GenericValue&))
    operator=(T value) {
        GenericValue v(value);
        return *this = v;
    }

    //! Deep-copy assignment from Value
    /*! Assigns a \b copy of the Value to the current Value object
        \tparam SourceAllocator Allocator type of \c rhs
        \param rhs Value to copy from (read-only)
        \param allocator Allocator to use for copying
        \param copyConstStrings Force copying of constant strings (e.g. referencing an in-situ buffer)
     */
    template <typename SourceAllocator>
    GenericValue& CopyFrom(const GenericValue<Encoding, SourceAllocator>& rhs, Allocator& allocator, bool copyConstStrings = false) {
        RAPIDJSON_ASSERT(static_cast<void*>(this) != static_cast<void const*>(&rhs));
        this->~GenericValue();
        new (this) GenericValue(rhs, allocator, copyConstStrings);
        return *this;
    }

    //! Exchange the contents of this value with those of other.
    /*!
        \param other Another value.
        \note Constant complexity.
    */
    GenericValue& Swap(GenericValue& other) RAPIDJSON_NOEXCEPT {
        GenericValue temp;
        temp.RawAssign(*this);
        RawAssign(other);
        other.RawAssign(temp);
        return *this;
    }

    //! free-standing swap function helper
    /*!
        Helper function to enable support for common swap implementation pattern based on \c std::swap:
        \code
        void swap(MyClass& a, MyClass& b) {
            using std::swap;
            swap(a.value, b.value);
            // ...
        }
        \endcode
        \see Swap()
     */
    friend inline void swap(GenericValue& a, GenericValue& b) RAPIDJSON_NOEXCEPT { a.Swap(b); }

    //! Prepare Value for move semantics
    /*! \return *this */
    GenericValue& Move() RAPIDJSON_NOEXCEPT { return *this; }
    //@}

    //!@name Equal-to and not-equal-to operators
    //@{
    //! Equal-to operator
    /*!
        \note If an object contains duplicated named member, comparing equality with any object is always \c false.
        \note Linear time complexity (number of all values in the subtree and total lengths of all strings).
    */
    template <typename SourceAllocator>
    bool operator==(const GenericValue<Encoding, SourceAllocator>& rhs) const {
        typedef GenericValue<Encoding, SourceAllocator> RhsType;
        if (GetType() != rhs.GetType())
            return false;

        switch (GetType()) {
        case kObjectType: // Warning: O(n^2) inner-loop
            if (data_.o.size != rhs.data_.o.size)
                return false;           
            for (ConstMemberIterator lhsMemberItr = MemberBegin(); lhsMemberItr != MemberEnd(); ++lhsMemberItr) {
                typename RhsType::ConstMemberIterator rhsMemberItr = rhs.FindMember(lhsMemberItr->name);
                if (rhsMemberItr == rhs.MemberEnd() || lhsMemberItr->value != rhsMemberItr->value)
                    return false;
            }
            return true;
            
        case kArrayType:
            if (data_.a.size != rhs.data_.a.size)
                return false;
            for (SizeType i = 0; i < data_.a.size; i++)
                if ((*this)[i] != rhs[i])
                    return false;
            return true;

        case kStringType:
            return StringEqual(rhs);

        case kNumberType:
            if (IsDouble() || rhs.IsDouble()) {
                double a = GetDouble();     // May convert from integer to double.
                double b = rhs.GetDouble(); // Ditto
                return a >= b && a <= b;    // Prevent -Wfloat-equal
            }
            else
                return data_.n.u64 == rhs.data_.n.u64;

        default:
            return true;
        }
    }

    //! Equal-to operator with const C-string pointer
    bool operator==(const Ch* rhs) const { return *this == GenericValue(StringRef(rhs)); }

#if RAPIDJSON_HAS_STDSTRING
    //! Equal-to operator with string object
    /*! \note Requires the definition of the preprocessor symbol \ref RAPIDJSON_HAS_STDSTRING.
     */
    bool operator==(const std::basic_string<Ch>& rhs) const { return *this == GenericValue(StringRef(rhs)); }
#endif

    //! Equal-to operator with primitive types
    /*! \tparam T Either \ref Type, \c int, \c unsigned, \c int64_t, \c uint64_t, \c double, \c true, \c false
    */
    template <typename T> RAPIDJSON_DISABLEIF_RETURN((internal::OrExpr<internal::IsPointer<T>,internal::IsGenericValue<T> >), (bool)) operator==(const T& rhs) const { return *this == GenericValue(rhs); }

    //! Not-equal-to operator
    /*! \return !(*this == rhs)
     */
    template <typename SourceAllocator>
    bool operator!=(const GenericValue<Encoding, SourceAllocator>& rhs) const { return !(*this == rhs); }

    //! Not-equal-to operator with const C-string pointer
    bool operator!=(const Ch* rhs) const { return !(*this == rhs); }

    //! Not-equal-to operator with arbitrary types
    /*! \return !(*this == rhs)
     */
    template <typename T> RAPIDJSON_DISABLEIF_RETURN((internal::IsGenericValue<T>), (bool)) operator!=(const T& rhs) const { return !(*this == rhs); }

    //! Equal-to operator with arbitrary types (symmetric version)
    /*! \return (rhs == lhs)
     */
    template <typename T> friend RAPIDJSON_DISABLEIF_RETURN((internal::IsGenericValue<T>), (bool)) operator==(const T& lhs, const GenericValue& rhs) { return rhs == lhs; }

    //! Not-Equal-to operator with arbitrary types (symmetric version)
    /*! \return !(rhs == lhs)
     */
    template <typename T> friend RAPIDJSON_DISABLEIF_RETURN((internal::IsGenericValue<T>), (bool)) operator!=(const T& lhs, const GenericValue& rhs) { return !(rhs == lhs); }
    //@}

    //!@name Type
    //@{

    Type GetType()  const { return static_cast<Type>(data_.f.flags & kTypeMask); }
    bool IsNull()   const { return data_.f.flags == kNullFlag; }
    bool IsFalse()  const { return data_.f.flags == kFalseFlag; }
    bool IsTrue()   const { return data_.f.flags == kTrueFlag; }
    bool IsBool()   const { return (data_.f.flags & kBoolFlag) != 0; }
    bool IsObject() const { return data_.f.flags == kObjectFlag; }
    bool IsArray()  const { return data_.f.flags == kArrayFlag; }
    bool IsNumber() const { return (data_.f.flags & kNumberFlag) != 0; }
    bool IsInt()    const { return (data_.f.flags & kIntFlag) != 0; }
    bool IsUint()   const { return (data_.f.flags & kUintFlag) != 0; }
    bool IsInt64()  const { return (data_.f.flags & kInt64Flag) != 0; }
    bool IsUint64() const { return (data_.f.flags & kUint64Flag) != 0; }
    bool IsDouble() const { return (data_.f.flags & kDoubleFlag) != 0; }
    bool IsString() const { return (data_.f.flags & kStringFlag) != 0; }

    // Checks whether a number can be losslessly converted to a double.
    bool IsLosslessDouble() const {
        if (!IsNumber()) return false;
        if (IsUint64()) {
            uint64_t u = GetUint64();
            volatile double d = static_cast<double>(u);
            return (d >= 0.0)
                && (d < static_cast<double>((std::numeric_limits<uint64_t>::max)()))
                && (u == static_cast<uint64_t>(d));
        }
        if (IsInt64()) {
            int64_t i = GetInt64();
            volatile double d = static_cast<double>(i);
            return (d >= static_cast<double>((std::numeric_limits<int64_t>::min)()))
                && (d < static_cast<double>((std::numeric_limits<int64_t>::max)()))
                && (i == static_cast<int64_t>(d));
        }
        return true; // double, int, uint are always lossless
    }

    // Checks whether a number is a float (possible lossy).
    bool IsFloat() const  {
        if ((data_.f.flags & kDoubleFlag) == 0)
            return false;
        double d = GetDouble();
        return d >= -3.4028234e38 && d <= 3.4028234e38;
    }
    // Checks whether a number can be losslessly converted to a float.
    bool IsLosslessFloat() const {
        if (!IsNumber()) return false;
        double a = GetDouble();
        if (a < static_cast<double>(-(std::numeric_limits<float>::max)())
                || a > static_cast<double>((std::numeric_limits<float>::max)()))
            return false;
        double b = static_cast<double>(static_cast<float>(a));
        return a >= b && a <= b;    // Prevent -Wfloat-equal
    }

    //@}

    //!@name Null
    //@{

    GenericValue& SetNull() { this->~GenericValue(); new (this) GenericValue(); return *this; }

    //@}

    //!@name Bool
    //@{

    bool GetBool() const { RAPIDJSON_ASSERT(IsBool()); return data_.f.flags == kTrueFlag; }
    //!< Set boolean value
    /*! \post IsBool() == true */
    GenericValue& SetBool(bool b) { this->~GenericValue(); new (this) GenericValue(b); return *this; }

    //@}

    //!@name Object
    //@{

    //! Set this value as an empty object.
    /*! \post IsObject() == true */
    GenericValue& SetObject() { this->~GenericValue(); new (this) GenericValue(kObjectType); return *this; }

    //! Get the number of members in the object.
    SizeType MemberCount() const { RAPIDJSON_ASSERT(IsObject()); return data_.o.size; }

    //! Get the capacity of object.
    SizeType MemberCapacity() const { RAPIDJSON_ASSERT(IsObject()); return data_.o.capacity; }

    //! Check whether the object is empty.
    bool ObjectEmpty() const { RAPIDJSON_ASSERT(IsObject()); return data_.o.size == 0; }

    //! Get a value from an object associated with the name.
    /*! \pre IsObject() == true
        \tparam T Either \c Ch or \c const \c Ch (template used for disambiguation with \ref operator[](SizeType))
        \note In version 0.1x, if the member is not found, this function returns a null value. This makes issue 7.
        Since 0.2, if the name is not correct, it will assert.
        If user is unsure whether a member exists, user should use HasMember() first.
        A better approach is to use FindMember(