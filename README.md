# ServerTest

## Install gRPC using vcpkg

Run in Powershell:

```
mkdir C:\src\
cd C:\src\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
git checkout -b 2025.02.14 2025.02.14
.\bootstrap-vcpkg.bat
.\vcpkg.exe --triplet x64-windows install protobuf grpc
.\vcpkg.exe integrate install
```

If behind proxy:

```
$ENV:HTTP_PROXY='http://proxy-dmz.intel.com:912'
$ENV:HTTPS_PROXY='http://proxy-dmz.intel.com:912'
```

## Install Visual Studio 2022

Install the following workloads:

- .NET desktop development
- Desktop development with C++
- Universal Windows Platform development
- Game development with C++

Make sure you have MSVC v143 or above and Windows 11 SDK 10.0.22621.0 or above.

## Building an example (outside of Unreal)

Install CMake for Windows: https://cmake.org/download

Follow this guide from "Generation of source files using proto file" through "Creating a project using CMake": https://sanoj.in/2020/05/07/working-with-grpc-in-windows.html

Filenames and paths in the provided CMakeLists.txt need to be updated to match the example files. If the provided protoc and cmake commands don't work, use these (run from `build` directory):

```
C:\src\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe  --cpp_out=. --proto_path=. helloworld.proto

C:\src\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe --proto_path=. --grpc_out=. --plugin=protoc-gen-grpc="C:\src\vcpkg\packages\grpc_x64-windows\tools\grpc\grpc_cpp_plugin.exe" helloworld.proto

cmake "-DCMAKE_TOOLCHAIN_FILE:PATH=C:\src\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_PREFIX_PATH=C:/src/vcpkg/packages"  ..
```

Open the .sln file in Visual Studio and build the server and client. Run the executables from the command line to see it working.

## Unreal Integration (WIP)

Unreal needs an older version of MSVC to build. When opening the project in VS, there will be a pop up in the upper right above the Solution Explorer prompting you to install additional components; click on the link and install them. The components should be:

- .NET 6.0 Runtime (Out of support)
- MSVC v143 - VS 2022 C++ x64/x86 build tools (v14.38-17.8)

Please note that this MSVC version will cause gRPC builds outside of Unreal to fail with linker errors.

Referencing this, don't know how much is necessary/applicable: https://kvachev.com/blog/posts/grpc-with-unreal-engine/

Definitely at least copy gRPC libraries into the project:

```
cp vcpkg/installed/x64-windows/lib/* MyProject/ThirdParty/lib
cp vcpkg/installed/x64-windows/include/* MyProject/ThirdParty/include
```
