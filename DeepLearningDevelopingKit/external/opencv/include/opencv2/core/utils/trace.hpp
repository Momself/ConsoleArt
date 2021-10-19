
// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_TRACE_HPP
#define OPENCV_TRACE_HPP

#include <opencv2/core/cvdef.h>

//! @addtogroup core_logging
// This section describes OpenCV tracing utilities.
//
//! @{

namespace cv {
namespace utils {
namespace trace {

//! Macro to trace function
#define CV_TRACE_FUNCTION()

#define CV_TRACE_FUNCTION_SKIP_NESTED()

//! Trace code scope.
//! @note Dynamic names are not supported in this macro (on stack or heap). Use string literals here only, like "initialize".
#define CV_TRACE_REGION(name_as_static_string_literal)
//! mark completed of the current opened region and create new one
//! @note Dynamic names are not supported in this macro (on stack or heap). Use string literals here only, like "step1".
#define CV_TRACE_REGION_NEXT(name_as_static_string_literal)

//! Macro to trace argument value
#define CV_TRACE_ARG(arg_id)

//! Macro to trace argument value (expanded version)
#define CV_TRACE_ARG_VALUE(arg_id, arg_name, value)

//! @cond IGNORED
#define CV_TRACE_NS cv::utils::trace

namespace details {

#ifndef __OPENCV_TRACE
# if defined __OPENCV_BUILD && !defined __OPENCV_TESTS && !defined __OPENCV_APPS
#   define __OPENCV_TRACE 1
# else
#   define __OPENCV_TRACE 0
# endif
#endif

#ifndef CV_TRACE_FILENAME
# define CV_TRACE_FILENAME __FILE__
#endif

#ifndef CV__TRACE_FUNCTION
# if defined _MSC_VER
#   define CV__TRACE_FUNCTION __FUNCSIG__
# elif defined __GNUC__
#   define CV__TRACE_FUNCTION __PRETTY_FUNCTION__
# else
#   define CV__TRACE_FUNCTION "<unknown>"
# endif
#endif

//! Thread-local instance (usually allocated on stack)
class CV_EXPORTS Region
{
public:
    struct LocationExtraData;
    struct LocationStaticStorage
    {
        LocationExtraData** ppExtra;   //< implementation specific data
        const char* name;              //< region name (function name or other custom name)
        const char* filename;          //< source code filename
        int line;                      //< source code line
        int flags;                     //< flags (implementation code path: Plain, IPP, OpenCL)
    };

    Region(const LocationStaticStorage& location);
    inline ~Region()
    {
        if (implFlags != 0)
            destroy();
        CV_DbgAssert(implFlags == 0);
        CV_DbgAssert(pImpl == NULL);
    }

    class Impl;
    Impl* pImpl; // NULL if current region is not active
    int implFlags; // see RegionFlag, 0 if region is ignored

    bool isActive() const { return pImpl != NULL; }

    void destroy();
private:
    Region(const Region&); // disabled
    Region& operator= (const Region&); // disabled
};

//! Specify region flags
enum RegionLocationFlag {
    REGION_FLAG_FUNCTION = (1 << 0),             //< region is function (=1) / nested named region (=0)