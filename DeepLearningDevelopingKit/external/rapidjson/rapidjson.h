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

#ifndef RAPIDJSON_RAPIDJSON_H_
#define RAPIDJSON_RAPIDJSON_H_

/*!\file rapidjson.h
    \brief common definitions and configuration
    
    \see RAPIDJSON_CONFIG
 */

/*! \defgroup RAPIDJSON_CONFIG RapidJSON configuration
    \brief Configuration macros for library features

    Some RapidJSON features are configurable to adapt the library to a wide
    variety of platforms, environments and usage scenarios.  Most of the
    features can be configured in terms of overridden or predefined
    preprocessor macros at compile-time.

    Some additional customization is available in the \ref RAPIDJSON_ERRORS APIs.

    \note These macros should be given on the compiler command-line
          (where applicable)  to avoid inconsistent values when compiling
          different translation units of a single application.
 */

#include <cstdlib>  // malloc(), realloc(), free(), size_t
#include <cstring>  // memset(), memcpy(), memmove(), memcmp()

///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_VERSION_STRING
//
// ALWAYS synchronize the following 3 macros with corresponding variables in /CMakeLists.txt.
//

//!@cond RAPIDJSON_HIDDEN_FROM_DOXYGEN
// token stringification
#define RAPIDJSON_STRINGIFY(x) RAPIDJSON_DO_STRINGIFY(x)
#define RAPIDJSON_DO_STRINGIFY(x) #x

// token concatenation
#define RAPIDJSON_JOIN(X, Y) RAPIDJSON_DO_JOIN(X, Y)
#define RAPIDJSON_DO_JOIN(X, Y) RAPIDJSON_DO_JOIN2(X, Y)
#define RAPIDJSON_DO_JOIN2(X, Y) X##Y
//!@endcond

/*! \def RAPIDJSON_MAJOR_VERSION
    \ingroup RAPIDJSON_CONFIG
    \brief Major version of RapidJSON in integer.
*/
/*! \def RAPIDJSON_MINOR_VERSION
    \ingroup RAPIDJSON_CONFIG
    \brief Minor version of RapidJSON in integer.
*/
/*! \def RAPIDJSON_PATCH_VERSION
    \ingroup RAPIDJSON_CONFIG
    \brief Patch version of RapidJSON in integer.
*/
/*! \def RAPIDJSON_VERSION_STRING
    \ingroup RAPIDJSON_CONFIG
    \brief Version of RapidJSON in "<major>.<minor>.<patch>" string format.
*/
#define RAPIDJSON_MAJOR_VERSION 1
#define RAPIDJSON_MINOR_VERSION 1
#define RAPIDJSON_PATCH_VERSION 0
#define RAPIDJSON_VERSION_STRING \
    RAPIDJSON_STRINGIFY(RAPIDJSON_MAJOR_VERSION.RAPIDJSON_MINOR_VERSION.RAPIDJSON_PATCH_VERSION)

///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_NAMESPACE_(BEGIN|END)
/*! \def RAPIDJSON_NAMESPACE
    \ingroup RAPIDJSON_CONFIG
    \brief   provide custom rapidjson namespace

    In order to avoid symbol clashes and/or "One Definition Rule" errors
    between multiple inclusions of (different versions of) RapidJSON in
    a single binary, users can customize the name of the main RapidJSON
    namespace.

    In case of a single nesting level, defining \c RAPIDJSON_NAMESPACE
    to a custom name (e.g. \c MyRapidJSON) is sufficient.  If multiple
    levels are needed, both \ref RAPIDJSON_NAMESPACE_BEGIN and \ref
    RAPIDJSON_NAMESPACE_END need to be defined as well:

    \code
    // in some .cpp file
    #define RAPIDJSON_NAMESPACE my::rapidjson
    #define RAPIDJSON_NAMESPACE_BEGIN namespace my { namespace rapidjson {
    #define RAPIDJSON_NAMESPACE_END   } }
    #include "rapidjson/..."
    \endcode

    \see rapidjson
 */
/*! \def RAPIDJSON_NAMESPACE_BEGIN
    \ingroup RAPIDJSON_CONFIG
    \brief   provide custom rapidjson namespace (opening expression)
    \see RAPIDJSON_NAMESPACE
*/
/*! \def RAPIDJSON_NAMESPACE_END
    \ingroup RAPIDJSON_CONFIG
    \brief   provide custom rapidjson namespace (closing expression)
    \see RAPIDJSON_NAMESPACE
*/
#ifndef RAPIDJSON_NAMESPACE
#define RAPIDJSON_NAMESPACE rapidjson
#endif
#ifndef RAPIDJSON_NAMESPACE_BEGIN
#define RAPIDJSON_NAMESPACE_BEGIN namespace RAPIDJSON_NAMESPACE {
#endif
#ifndef RAPIDJSON_NAMESPACE_END
#define RAPIDJSON_NAMESPACE_END }
#endif

///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_HAS_STDSTRING

#ifndef RAPIDJSON_HAS_STDSTRING
#ifdef RAPIDJSON_DOXYGEN_RUNNING
#def