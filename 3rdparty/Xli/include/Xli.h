/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_H__
#define __XLI_H__

/**
    \defgroup XliCoreLibs Core libraries
    \brief Core functionality

    \defgroup XliMediaLibs Media libraries
    \brief Interfaces for various media formats

    \defgroup XliPlatformLibs Platform libraries
    \brief Interfaces to platform specific functionality

    \defgroup Xli
    \brief The core library
    \ingroup XliCoreLibs

    \defgroup XliBuffer Buffer
    \brief Buffer abstraction
    \ingroup Xli

    \defgroup XliContainers Containers
    \brief Container templates
    \ingroup Xli

    \defgroup XliConsole Console
    \brief Standard input, output and error abstraction
    \ingroup Xli

    \defgroup XliDataTypes Data types
    \brief Vectors, rectangles, integers and constants
    \ingroup Xli

    \defgroup XliExceptions Exceptions
    \brief Exception classes and throw macros
    \ingroup Xli

    \defgroup XliIO I/O
    \brief File, Stream, serialization, virtual file systems
    \ingroup Xli

    \defgroup XliObject Object
    \brief Object base class and pointer templates
    \ingroup Xli

    \defgroup XliText Text
    \brief String, StringBuilder, UTF-8<=>16 conversion
    \ingroup Xli

    \defgroup XliThreading Threading 
    \brief Threading and concurrency
    \ingroup Xli

    \defgroup XliTime Time
    \brief DateTime and time measurement 
    \ingroup Xli

    \defgroup XliTraits Traits
    \brief Default template traits
    \ingroup Xli

    \defgroup XliUtils Utils
    \brief Rand, Sort, Swap
    \ingroup Xli
*/

#include <Xli/Array.h>
#include <Xli/ArrayStream.h>
#include <Xli/Atomic.h>
#include <Xli/BinaryReader.h>
#include <Xli/BinaryWriter.h>
#include <Xli/Buffer.h>
#include <Xli/BufferStream.h>
#include <Xli/ConditionVariable.h>
#include <Xli/Config.h>
#include <Xli/Console.h>
#include <Xli/CoreLib.h>
#include <Xli/DataType.h>
#include <Xli/DateTime.h>
#include <Xli/Disk.h>
#include <Xli/Exception.h>
#include <Xli/File.h>
#include <Xli/FileSystem.h>
#include <Xli/HashMap.h>
#include <Xli/IntTypes.h>
#include <Xli/Managed.h>
#include <Xli/Mutex.h>
#include <Xli/Object.h>
#include <Xli/Map.h>
#include <Xli/Path.h>
#include <Xli/Queue.h>
#include <Xli/Random.h>
#include <Xli/Rectangle.h>
#include <Xli/Sort.h>
#include <Xli/Stream.h>
#include <Xli/String.h>
#include <Xli/StringBuilder.h>
#include <Xli/Text.h>
#include <Xli/TextReader.h>
#include <Xli/TextStream.h>
#include <Xli/TextWriter.h>
#include <Xli/Thread.h>
#include <Xli/Time.h>
#include <Xli/Traits.h>
#include <Xli/Unicode.h>
#include <Xli/Uri.h>
#include <Xli/Utf16String.h>
#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>

#endif
