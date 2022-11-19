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

#ifndef RAPIDJSON_POINTER_H_
#define RAPIDJSON_POINTER_H_

#include "document.h"
#include "internal/itoa.h"

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(switch-enum)
#elif defined(_MSC_VER)
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(4512) // assignment operator could not be generated
#endif

RAPIDJSON_NAMESPACE_BEGIN

static const SizeType kPointerInvalidIndex = ~SizeType(0);  //!< Represents an invalid index in GenericPointer::Token

//! Error code of parsing.
/*! \ingroup RAPIDJSON_ERRORS
    \see GenericPointer::GenericPointer, GenericPointer::GetParseErrorCode
*/
enum PointerParseErrorCode {
    kPointerParseErrorNone = 0,                     //!< The parse is successful

    kPointerParseErrorTokenMustBeginWithSolidus,    //!< A token must begin with a '/'
    kPointerParseErrorInvalidEscape,                //!< Invalid escape
    kPointerParseErrorInvalidPercentEncoding,       //!< Invalid percent encoding in URI fragment
    kPointerParseErrorCharacterMustPercentEncode    //!< A character must percent encoded in URI fragment
};

///////////////////////////////////////////////////////////////////////////////
// GenericPointer

//! Represents a JSON Pointer. Use Pointer for UTF8 encoding and default allocator.
/*!
    This class implements RFC 6901 "JavaScript Object Notation (JSON) Pointer" 
    (https://tools.ietf.org/html/rfc6901).

    A JSON pointer is for identifying a specific value in a JSON document
    (GenericDocument). It can simplify coding of DOM tree manipulation, because it
    can access multiple-level depth of DOM tree with single API call.

    After it parses a string representation (e.g. "/foo/0" or URI fragment 
    representation (e.g. "#/foo/0") into its internal representation (tokens),
    it can be used to resolve a specific value in multiple documents, or sub-tree 
    of documents.

    Contrary to GenericValue, Pointer can be copy constructed and copy assigned.
    Apart from assignment, a Pointer cannot be modified after construction.

    Although Pointer is very convenient, please aware that constructing Pointer
    involves parsing and dynamic memory allocation. A special constructor with user-
    supplied tokens eliminates these.

    GenericPointer depends on GenericDocument and GenericValue.
    
    \tparam ValueType The value type of the DOM tree. E.g. GenericValue<UTF8<> >
    \tparam Allocator The allocator type for allocating memory for internal representation.
    
    \note GenericPointer uses same encoding of ValueType.
    However, Allocator of GenericPointer is independent of Allocator of Value.
*/
template <typename ValueType, typename Allocator = CrtAllocator>
class GenericPointer {
public:
    typedef typename ValueType::EncodingType EncodingType;  //!< Encoding type from Value
    typedef typename ValueType::Ch Ch;                      //!< Character type from Value

    //! A token is the basic units of internal representation.
    /*!
        A JSON pointer string representation "/foo/123" is parsed to two tokens: 
        "foo" and 123. 123 will be represented in both numeric form and string form.
        They are resolved according to the actual value type (object or array).

        For token that are not numbers, or the numeric value is out of bound
        (greater than limits of SizeType), they are only treated as string form
        (i.e. the token's index will be equal to kPointerInvalidIndex).

        This struct is public so that user can create a Pointer without parsing and 
        allocation, using a special constructor.
    */
    struct Token {
        const Ch* name;             //!< Name of the token. It has null character at the end but it can contain null character.
        SizeType length;            //!< Length of the name.
        SizeType index;             //!< A valid array index, if it is not equal to kPointerInvalidIndex.
    };

    //!@name Constructors and destructor.
    //@{

    //! Default constructor.
    GenericPointer(Allocator* allocator = 0) : allocator_(allocator), ownAllocator_(), nameBuffer_(), tokens_(), tokenCount_(), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {}

    //! Constructor that parses a string or URI fragment representation.
    /*!
        \param source A null-terminated, string or URI fragment representation of JSON pointer.
        \param allocator User supplied allocator for this pointer. If no allocator is provided, it creates a self-owned one.
    */
    explicit GenericPointer(const Ch* source, Allocator* allocator = 0) : allocator_(allocator), ownAllocator_(), nameBuffer_(), tokens_(), tokenCount_(), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {
        Parse(source, internal::StrLen(source));
    }

#if RAPIDJSON_HAS_STDSTRING
    //! Constructor that parses a string or URI fragment representation.
    /*!
        \param source A string or URI fragment representation of JSON pointer.
        \param allocator User supplied allocator for this pointer. If no allocator is provided, it creates a self-owned one.
        \note Requires the definition of the preprocessor symbol \ref RAPIDJSON_HAS_STDSTRING.
    */
    explicit GenericPointer(const std::basic_string<Ch>& source, Allocator* allocator = 0) : allocator_(allocator), ownAllocator_(), nameBuffer_(), tokens_(), tokenCount_(), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {
        Parse(source.c_str(), source.size());
    }
#endif

    //! Constructor that parses a string or URI fragment representation, with length of the source string.
    /*!
        \param source A string or URI fragment representation of JSON pointer.
        \param length Length of source.
        \param allocator User supplied allocator for this pointer. If no allocator is provided, it creates a self-owned one.
        \note Slightly faster than the overload without length.
    */
    GenericPointer(const Ch* source, size_t length, Allocator* allocator = 0) : allocator_(allocator), ownAllocator_(), nameBuffer_(), tokens_(), tokenCount_(), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {
        Parse(source, length);
    }

    //! Constructor with user-supplied tokens.
    /*!
        This constructor let user supplies const array of tokens.
        This prevents the parsing process and eliminates allocation.
        This is preferred for memory constrained environments.

        \param tokens An constant array of tokens representing the JSON pointer.
        \param tokenCount Number of tokens.

        \b Example
        \code
        #define NAME(s) { s, sizeof(s) / sizeof(s[0]) - 1, kPointerInvalidIndex }
        #define INDEX(i) { #i, sizeof(#i) - 1, i }

        static const Pointer::Token kTokens[] = { NAME("foo"), INDEX(123) };
        static const Pointer p(kTokens, sizeof(kTokens) / sizeof(kTokens[0]));
        // Equivalent to static const Pointer p("/foo/123");

        #undef NAME
        #undef INDEX
        \endcode
    */
    GenericPointer(const Token* tokens, size_t tokenCount) : allocator_(), ownAllocator_(), nameBuffer_(), tokens_(const_cast<Token*>(tokens)), tokenCount_(tokenCount), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {}

    //! Copy constructor.
    GenericPointer(const GenericPointer& rhs) : allocator_(rhs.allocator_), ownAllocator_(), nameBuffer_(), tokens_(), tokenCount_(), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {
        *this = rhs;
    }

    //! Copy constructor.
    GenericPointer(const GenericPointer& rhs, Allocator* allocator) : allocator_(allocator), ownAllocator_(), nameBuffer_(), tokens_(), tokenCount_(), parseErrorOffset_(), parseErrorCode_(kPointerParseErrorNone) {
        *this = rhs;
    }

    //! Destructor.
    ~GenericPointer() {
        if (nameBuffer_)    // If user-supplied tokens constructor is used, nameBuffer_ is nullptr and tokens_ are not deallocated.
            Allocator::Free(tokens_);
        RAPIDJSON_DELETE(ownAllocator_);
    }

    //! Assignment operator.
    GenericPointer& operator=(const GenericPointer& rhs) {
        if (this != &rhs) {
            // Do not delete ownAllcator
            if (nameBuffer_)
                Allocator::Free(tokens_);

            tokenCount_ = rhs.tokenCount_;
            parseErrorOffset_ = rhs.parseErrorOffset_;
            parseErrorCode_ = rhs.parseErrorCode_;

            if (rhs.nameBuffer_)
       