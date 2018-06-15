The Xli source code is released under The MIT License.

The source code is located in the `include` and `src` directories.

> Copyright (C) 2010-2014 Outracks Technologies AS
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
> associated documentation files (the "Software"), to deal in the Software without restriction, 
> including without limitation the rights to use, copy, modify, merge, publish, distribute, 
> sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or 
> substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT 
> NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
> NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
> DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT 
> OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


Third-party dependencies
------------------------

The table below shows the libraries Xli uses internally and how they are linked on the different target platforms.

License details are found in the source code located in the `3rdparty` directory.

Library                 | Module     | Android | iOS     | OS X    | *nix    | Win32  |
------------------------|:----------:|:-------:|:-------:|:-------:|:-------:|:-------:
android_native_app_glue | Main       | static  | -       | -       | -       | -      |
ConvertUTF              | Core       | static  | static  | static  | static  | static |
curl                    | HttpClient | -       | -       | dynamic | dynamic | static |
freetype                | Font       | static  | static  | static  | dynamic | static |
glew                    | GL         | -       | -       | static  | dynamic | static |
jpeg                    | Image      | static  | -       | static  | dynamic | static |
JSON_parser             | Json       | static  | static  | static  | static  | static |
png                     | Image      | static  | -       | static  | dynamic | static |
SDL2                    | Platform   | -       | static  | static  | dynamic | -      |
unzip                   | Zip        | static  | static  | static  | static  | static |
zlib                    | Zip        | dynamic | static  | dynamic | dynamic | static |
