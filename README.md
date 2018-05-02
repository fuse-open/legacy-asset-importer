Uno.Native is a .NET library for importing and conditioning of real-time assets such as 3D
textures and fonts. It is implemented on top of native libraries that are wrapped in a
cross-compiled C interface which is called from C# using dynamic P/Invoke.

We use this library for implementing `import` backends in the Uno compiler, and for loading raw
assets in Uno runtime code for .NET.

Prerequisites
-------------

* CMake
* Mono/.NET
* Platform specific libraries (see below)

**Installing using APT (Linux)**
```
sudo apt-get install -y libfreetype6-dev
sudo apt-get install -y libjpeg-dev
sudo apt-get install -y libpng12-dev
```

Building
--------

Run `build.bat (or .sh)`. Remember to init submodules in git first.

Debugging in Visual Studio
--------------------------

When the native code is built using `build.bat`, you can open `Uno.Native.sln` in Visual
Studio, enable Native Code Debugging in your startup project, and step down in to the C/C++ layer
through the C# wrapper.
