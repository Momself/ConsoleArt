/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCV_CORE_PERSISTENCE_HPP
#define OPENCV_CORE_PERSISTENCE_HPP

#ifndef CV_DOXYGEN
/// Define to support persistence legacy formats
#define CV__LEGACY_PERSISTENCE
#endif

#ifndef __cplusplus
#  error persistence.hpp header must be compiled as C++
#endif

//! @addtogroup core_c
//! @{

/** @brief "black box" representation of the file storage associated with a file on disk.

Several functions that are described below take CvFileStorage\* as inputs and allow the user to
save or to load hierarchical collections that consist of scalar values, standard CXCore objects
(such as matrices, sequences, graphs), and user-defined objects.

OpenCV can read and write data in XML (<http://www.w3c.org/XML>), YAML (<http://www.yaml.org>) or
JSON (<http://www.json.org/>) formats. Below is an example of 3x3 floating-point identity matrix A,
stored in XML and YAML files
using CXCore functions:
XML:
@code{.xml}
    <?xml version="1.0">
    <opencv_storage>
    <A type_id="opencv-matrix">
      <rows>3</rows>
      <cols>3</cols>
      <dt>f</dt>
      <data>1. 0. 0. 0. 1. 0. 0. 0. 1.</data>
    </A>
    </opencv_storage>
@endcode
YAML:
@code{.yaml}
    %YAML:1.0
    A: !!opencv-matrix
      rows: 3
      cols: 3
      dt: f
      data: [ 1., 0., 0., 0., 1., 0., 0., 0., 1.]
@endcode
As it can be seen from the examples, XML uses nested tags to represent hierarchy, while YAML uses
indentation for that purpose (similar to the Python programming language).

The same functions can read and write data in both formats; the particular format is determined by
the extension of the opened file, ".xml" for XML files, ".yml" or ".yaml" for YAML and ".json" for
JSON.
 */
typedef struct CvFileStorage CvFileStorage;
typedef struct CvFileNode CvFileNode;
typedef struct CvMat CvMat;
typedef struct CvMatND CvMatND;

//! @} core_c

#include "opencv2/core/types.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

/** @addtogroup core_xml

XML/YAML/JSON file storages.     {#xml_storage}
=======================
Writing to a file storage.
--------------------------
You can store and then restore various OpenCV data structures to/from XML (<http://www.w3c.org/XML>),
YAML (<http://www.yaml.org>) or JSON (<http://www.json.org/>) formats. Also, it is possible to store
and load arbitrarily complex data structures, which include OpenCV data structures, as well as
primitive data types (integer and floating-point numbers and text strings) as their elements.

Use the following procedure to write something to XML, YAML or JSON:
-# Create new FileStorage and open it for writing. It can be done with a single call to
FileStorage::FileStorage constructor that takes a filename, or you can use the default constructor
and then call FileStorage::open. Format of the file (XML, YAML or JSON) is determined from the filename
extension (".xml", ".yml"/".yaml" and ".json", respectively)
-# Write all the data you want using the streaming operator `<<`, just like in the case of STL
streams.
-# Close the file using FileStorage::release. FileStorage destructor also closes the file.

Here is an example:
@code
    #include "opencv2/opencv.hpp"
    #include <time.h>

    using namespace cv;

    int main(int, char** argv)
    {
        FileStorage fs("test.yml", FileStorage::WRITE);

        fs << "frameCount" << 5;
        time_t rawtime; time(&rawtime);
        fs << "calibrationDate" << asctime(localtime(&rawtime));
        Mat cameraMatrix = (Mat_<double>(3,3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
        Mat distCoeffs = (Mat_<double>(5,1) << 0.1, 0.01, -0.001, 0, 0);
        fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;
        fs << "features" << "[";
        for( int i = 0; i < 3; i++ )
        {
            int x = rand() % 640;
            int y = rand() % 480;
            uchar lbp = rand() % 256;

            fs << "{:" << "x" << x << "y" << y << "lbp" << "[:";
            for( int j = 0; j < 8; j++ )
                fs << ((lbp >> j) & 1);
            fs << "]" << "}";
        }
        fs << "]";
        fs.release();
        return 0;
    }
@endcode
The sample above stores to YML an integer, a text string (calibration date), 2 matrices, and a custom
structure "feature", which includes feature coordinates and LBP (local binary pattern) value. Here
is output of the sample:
@code{.yaml}
%YAML:1.0
frameCount: 5
calibrationDate: "Fri Jun 17 14:09:29 2011\n"
cameraMatrix: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 1000., 0., 320., 0., 1000., 240., 0., 0., 1. ]
distCoeffs: !!opencv-matrix
   rows: 5
   cols: 1
   dt: d
   data: [ 1.0000000000000001e-01, 1.0000000000000000e-02,
       -1.0000000000000000e-03, 0., 0. ]
features:
   - { x:167, y:49, lbp:[ 1, 0, 0, 1, 1, 0, 1, 1 ] }
   - { x:298, y:130, lbp:[ 0, 0, 0, 1, 0, 0, 1, 1 ] }
   - { x:344, y:158, lbp:[ 1, 1, 0, 0, 0, 0, 1, 0 ] }
@endcode

As an exercise, you can replace ".yml" with ".xml" or ".json" in the sample above and see, how the
corresponding XML file will look like.

Several things can be noted by looking at the sample code and the output:

-   The produced YAML (and XML/JSON) consists of heterogeneous collections that can be nested. There are
    2 types of collections: named collections (mappings) and unnamed collections (sequences). In mappings
    each element has a name and is accessed by name. This is similar to structures and std::map in
    C/C++ and dictionaries in Python. In sequences elements do not have names, they are accessed by
    indices. This is similar to arrays and std::vector in C/C++ and lists, tuples in Python.
    "Heterogeneous" means that elements of each single collection can have different types.

    Top-level collection in YAML/XML/JSON is a mapping. Each matrix is stored as a mapping, and the matrix
    elements are stored as a sequence. Then, there is a sequence of features, where each feature is
    represented a mapping, and lbp value in a nested sequence.

-   When you write to a mapping (a structure), you write element name followed by its value. When you
    write to a sequence, you simply write the elements one by one. OpenCV data structures (such as
    cv::Mat) are written in absolutely the same way as simple C data structures - using `<<`
    operator.

-   To write a mapping, you first write the special string `{` to the storage, then write the
    elements as pairs (`fs << <element_name> << <element_value>`) and then write the closing
    `}`.

-   To write a sequence, you first write the special string `[`, then write the elements, then
    write the closing `]`.

-   In YAML/JSON (but not XML), mappings and sequences can be written in a compact Python-like inline
    form. In the sample above matrix elements, as well as each feature, including its lbp value, is
    stored in such inline form. To store a mapping/sequence in a compact form, put `:` after the
    opening character, e.g. use `{:` instead of `{` and `[:` instead of `[`. When the
    data is written to XML, those extra `:` are ignored.

Reading data from a file storage.
---------------------------------
To read the previously written XML, YAML or JSON file, do the following:
-#  Open the file storage using FileStorage::FileStorage constructor or FileStorage::open method.
    In the current implementation the whole file is parsed and the whole representation of file
    storage is built in memory as a hierarchy of file nodes (see FileNode)

-#  Read the data you are interested in. Use FileStorage::operator [], FileNode::operator []
    and/or FileNodeIterator.

-#  Close the storage using FileStorage::release.

Here is how to read the file created by the code sample above:
@code
    FileStorage fs2("test.yml", FileStorage::READ);

    // first method: use (type) operator on FileNode.
    int frameCount = (int)fs2["frameCount"];

    String date;
    // second method: use FileNode::operator >>
    fs2["calibrationDate"] >> date;

    Mat cameraMatrix2, distCoeffs2;
    fs2["cameraMatrix"] >> cameraMatrix2;
    fs2["distCoeffs"] >> distCoeffs2;

    cout << "frameCount: " << frameCount << endl
         << "calibration date: " << date << endl
         << "camera matrix: " << cameraMatrix2 << endl
         << "distortion coeffs: " << distCoeffs2 << endl;

    FileNode features = fs2["features"];
    FileNodeIterator it = features.begin(), it_end = features.end();
    int idx = 0;
    std::vector<uchar> lbpval;

    // iterate through a sequence using FileNodeIterator
    for( ; it != it_end; ++it, idx++ )
    {
        cout << "feature #" << idx << ": ";
        cout << "x=" << (int)(*it)["x"] << ", y=" << (int)(*it)["y"] << ", lbp: (";
        // you can also easily read numerical arrays using FileNode >> std::vector operator.
        (*it)["lbp"] >> lbpval;
        for( int i = 0; i < (int)lbpval.size(); i++ )
            cout << " " << (int)lbpval[i];
        cout << ")" << endl;
    }
    fs2.release();
@endcode

Format specification    {#format_spec}
--------------------
`([count]{u|c|w|s|i|f|d})`... where the characters correspond to fundamental C++ types:
-   `u` 8-bit unsigned number
-   `c` 8-bit signed number
-   `w` 16-bit unsigned number
-   `s` 16-bit signed number
-   `i` 32-bit signed number
-   `f` single precision floating-point number
-   `d` double precision floating-point number
-   `r` pointer, 32 lower bits of which are written as a signed integer. The type can be used to
    store structures with links between the elements.

`count` is the optional counter of values of a given type. For example, `2if` means that each array
element is a structure of 2 integers, followed by a single-precision floating-point number. The
equivalent notations of the above specification are `iif`, `2i1f` and so forth. Other examples: `u`
means that the array consists of bytes, and `2d` means the array consists of pairs of doubles.

@see @ref filestorage.cpp
*/

//! @{

/** @example filestorage.cpp
A complete example using the FileStorage interface
*/

////////////////////////// XML & YAML I/O //////////////////////////

class CV_EXPORTS FileNode;
class CV_EXPORTS FileNodeIterator;

/** @brief XML/YAML/JSON file storage class that encapsulates all the information necessary for writing or
reading data to/from a file.
 */
class CV_EXPORTS_W FileStorage
{
public:
    //! file storage mode
    enum Mode
    {
        READ        = 0, //!< value, open the file for reading
        WRITE       = 1, //!< value, open the file for writing
        APPEND      = 2, //!< value, open the file for appending
        MEMORY      = 4, //!< flag, read data from source or write data to the internal buffer (which is
                         //!< returned by FileStorage::release)
        FORMAT_MASK = (7<<3), //!< mask for format flags
        FORMAT_AUTO = 0,      //!< flag, auto format
        FORMAT_XML  = (1<<3), //!< flag, XML format
        FORMAT_YAML = (2<<3), //!< flag, YAML format
        FORMAT_JSON = (3<<3), //!< flag, JSON format

        BASE64      = 64,     //!< flag, write rawdata in Base64 by default. (consider using WRITE_BASE64)
        WRITE_BASE64 = BASE64 | WRITE, //!< flag, enable both WRITE and BASE64
    };
    enum
    {
        UNDEFINED      = 0,
        VALUE_EXPECTED = 1,
        NAME_EXPECTED  = 2,
        INSIDE_MAP     = 4
    };

    /** @brief The constructors.

    The full constructor opens the file. Alternatively you can use the default constructor and then
    call FileStorage::open.
     */
    CV_WRAP FileStorage();

    /** @overload
    @copydoc open()
    */
    CV_WRAP FileStorage(const String& filename, int flags, const String& encoding=String());

    /** @overload */
    FileStorage(CvFileStorage* fs, bool owning=true);

    //! the destructor. calls release()
    virtual ~FileStorage();

    /** @brief Opens a file.

    See description of parameters in FileStorage::FileStorage. The method calls FileStorage::release
    before opening the file.
    @param filename Name of the file to open or the text string to read the data from.
       Extension of the file (.xml, .yml/.yaml or .json) determines its format (XML, YAML or JSON
        respectively). Also you can append .gz to work with compressed files, for example myHugeMatrix.xml.gz. If both
        FileStorage::WRITE and FileStorage::MEMORY flags are specified, source is used just to specify
        the output file format (e.g. mydata.xml, .yml etc.). A file name can also contain parameters.
        You can use this format, "*?base64" (e.g. "file.json?base64" (case sensitive)), as an alternative to
        FileStorage::BASE64 flag.
    @param flags Mode of operation. One of FileStorage::Mode
    @param encoding Encoding of the file. Note that UTF-16 XML encoding is not supported currently and
    you should use 8-bit encoding instead of it.
     */
    CV_WRAP virtual bool open(const String& filename, int flags, const String& encoding=String());

    /** @brief Checks whether the file is opened.

    @returns true if the object is associated with the current file and false otherwise. It is a
    good practice to call this method after you tried to open a file.
     */
    CV_WRAP virtual bool isOpened() const;

    /** @brief Closes the file and releases all the memory buffers.

    Call this method after all I/O operations with the storage are finished.
     */
    CV_WRAP virtual void release();

    /** @brief Closes the file and releases all the memory buffers.

    Call this method after all I/O operations with the storage are finished. If the storage was
    opened for writing data and FileStorage::WRITE was specified
     */
    CV_WRAP virtual String releaseAndGetString();

    /** @brief Returns the first element of the top-level mapping.
    @returns The first element of the top-level mapping.
     */
    CV_WRAP FileNode getFirstTopLevelNode() const;

    /** @brief Returns the top-level mapping
    @param streamidx Zero-based index of the stream. In most cases there is only one stream in the file.
    However, YAML supports multiple streams and so there can be several.
    @returns The top-level mapping.
     */
    CV_WRAP FileNode root(int streamidx=0) const;

    /** @brief Returns the specified element of the top-level mapping.
    @param nodename Name of the file node.
    @returns Node with the given name.
     */
    FileNode operator[](const String& nodename) const;

    /** @overload */
    CV_WRAP_AS(getNode) FileNode operator[](const char* nodename) const;

    /** @brief Returns the obsolete C FileStorage structure.
    @returns Pointer to the underlying C FileStorage structure
     */
    CvFileStorage* operator *() { return fs.get(); }

    /** @overload */
    const CvFileStorage* operator *() const { return fs.get(); }

    /** @brief Writes multiple numbers.

    Writes one or more numbers of the specified format to the currently written structure. Usually it is
    more convenient to use operator `<<` instead of this method.
    @param fmt Specification of each array element, see @ref format_spec "format specification"
    @param vec Pointer to the written array.
    @param len Number of the uchar elements to write.
     */
    void writeRaw( const String& fmt, const uchar* vec, size_t len );

    /** @brief Writes the registered C structure (CvMat, CvMatND, CvSeq).
    @param name Name of the written object.
    @param obj Pointer to the object.
    @see ocvWrite for details.
     */
    void writeObj( const String& name, const void* obj );

    /**
     * @brief Simplified writing API to use with bindings.
     * @param name Name of the written object
     * @param val Value of the written object
     */
    CV_WRAP void write(const String& name, double val);
    /// @overload
    CV_WRAP void write(const String& name, const String& val);
    /// @overload
    CV_WRAP void write(const String& name, InputArray val);

    /** @brief Writes a comment.

    The function writes a comment into file storage. The comments are skipped when the storage is read.
    @param comment The written comment, single-line or multi-line
    @param append If true, the function tries to put the comment at the end of current line.
    Else if the comment is multi-line, or if it does not fit at the end of the current
    line, the comment starts a new line.
     */
    CV_WRAP void writeComment(const String& comment, bool append = false);

    /** @brief Returns the normalized object name for the specified name of a file.
    @param filename Name of a file
    @returns The normalized object name.
     */
    static String getDefaultObjectName(const String& filename);

    /** @brief Returns the current format.
     * @returns The current format, see FileStorage::Mode
     */
    CV_WRAP int getFormat() const;

    Ptr<CvFileStorage> fs; //!< the underlying C FileStorage structure
    String elname; //!< the currently written element
    std::vector<char> structs; //!< the stack of written structures
    int state; //!< the writer state
};

template<> CV_EXPORTS void DefaultDeleter<CvFileStorage>::operator ()(CvFileStorage* obj) const;

/** @brief File Storage Node class.

The node is used to store each and every element of the file storage opened for reading. When
XML/YAML file is read, it is first parsed and stored in the memory as a hierarchical collection of
nodes. Each node can be a "leaf" that is contain a single number or a string, or be a collection of
other nodes. There can be named collections (mappings) where each element has a name and it is
accessed by a name, and ordered collections (sequences) where elements do not have names but rather
accessed by index. Type of the file node can be determined using FileNode::type method.

Note that file nodes are only used for navigating file storages opened for reading. When a file
storage is opened for writing, no data is stored in memory after it is written.
 */
class CV_EXPORTS_W_SIMPLE FileNode
{
public:
    //! type of the file storage node
    enum Type
    {
        NONE      = 0, //!< empty node
        INT       = 1, //!< an integer
        REAL      = 2, //!< floating-point number
        FLOAT     = REAL, //!< synonym or REAL
        STR       = 3, //!< text string in UTF-8 encoding
        STRING    = STR, //!< synonym for STR
        REF       = 4, //!< integer of size size_t. Typically used for storing complex dynamic structures where some elements reference the others
        SEQ       = 5, //!< sequence
        MAP       = 6, //!< mapping
        TYPE_MASK = 7,
        FLOW      = 8,  //!< compact representation of a sequence or mapping. Used only by YAML writer
        USER      = 16, //!< a registered object (e.g. a matrix)
        EMPTY     = 32, //!< empty structure (sequence or mapping)
        NAMED     = 64  //!< the node has a name (i.e. it is element of a mapping)
    };
    /** @brief The constructors.

    These constructors are used to create a default file node, construct it from obsolete structures or
    from the another file node.
     */
    CV_WRAP FileNode();

    /** @overload
    @param fs Pointer to the obsolete file storage structure.
    @param node File node to be used as initialization for the created file node.
    */
    FileNode(const CvFileStorage* fs, const CvFileNode* node);

    /** @overload
    @param node File node to be used as initialization for the created file node.
    */
    FileNode(const FileNode& node);

    /** @brief Returns element of a mapping node or a sequence node.
    @param nodename Name of an element in the mapping node.
    @returns Returns the element with the given identifier.
     */
    FileNode operator[](const String& nodename) const;

    /** @overload
    @param nodename Name of an element in the mapping node.
    */
    CV_WRAP_AS(getNode) FileNode operator[](const char* nodename) const;

    /** @overload
    @param i Index of an element in the sequence node.
    */
    CV_WRAP_AS(at) FileNode operator[](int i) const;

    /** @brief Returns type of the node.
    @returns Type of the node. See FileNode::Type
     */
    CV_WRAP int type() const;

    //! returns true if the node is empty
    CV_WRAP bool empty() const;
    //! returns true if the node is a "none" object
    CV_WRAP bool isNone() const;
    //! returns true if the node is a sequence
    CV_WRAP bool isSeq() const;
    //! returns true if the node is a mapping
    CV_WRAP bool isMap() const;
    //! returns true if the node is an integer
    CV_WRAP bool isInt() const;
    //! returns true if the node is a floating-point number
    CV_WRAP bool isReal() const;
    //! returns true if the node is a text string
    CV_WRAP bool isString() const;
    //! returns true if the node has a name
    CV_WRAP bool isNamed() const;
    //! returns the node name or an empty string if the node is nameless
    CV_WRAP String name() const;
    //! returns the number of elements in the node, if it is a sequence or mapping, or 1 otherwise.
    CV_WRAP size_t size() const;
    //! returns the node content as an integer. If the node stores floating-point number, it is rounded.
    operator int() const;
    //! returns the node content as float
    operator float() const;
    //! returns the node content as double
    operator double() const;
    //! returns the node content as text string
    operator String() const;
    operator std::string() const;

    //! returns pointer to the underlying file node
    CvFileNode* operator *();
    //! returns pointer to the underlying file node
    const CvFileNode* operator* () const;

    //! returns iterator pointing to the first node element
    FileNodeIterator begin() const;
    //! returns iterator pointing to the element following the last node element
    FileNodeIterator end() const;

    /** @brief Reads node elements to the buffer with the specified format.

   