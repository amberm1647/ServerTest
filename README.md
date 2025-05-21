# ServerTest

This is old; please refer to https://github.com/amberm1647/unreal_grpc_helloworld instead

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

To create an example project, follow this guide from "Generation of source files using proto file" through "Creating a project using CMake": https://sanoj.in/2020/05/07/working-with-grpc-in-windows.html

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

Add grpc example files generated in previous step -- protobuf files (.proto, .pb.h, .pb.cc, .grpc.pb.h, .grpc.pb.cc) to Source/MyProject/protos/ and client and server (.cc) files to Source/MyProject/src/.

Add the following to MyProject.Build.cs inside the block starting with `public MyProject(ReadOnlyTargetRules Target) : base(Target)` to link third party libraries:

```
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",});

PrivateDependencyModuleNames.AddRange(new string[] {  });

PublicIncludePaths.Add("C:\\src\\vcpkg\\installed\\x64-windows\\include");

string LibrariesPath = "C:\\src\\vcpkg\\installed\\x64-windows\\lib";
DirectoryInfo d = new DirectoryInfo(LibrariesPath);
FileInfo[] Files = d.GetFiles("*.lib");
foreach (FileInfo file in Files)
{
    Console.WriteLine("adding " + file.Name);
    PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, file.Name));
}

PublicDefinitions.Add("PROTOBUF_ENABLE_DEBUG_LOGGING_MAY_LEAK_PII=0");
PublicDefinitions.Add("PROTOBUF_BUILTIN_ATOMIC=0");
PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
```

At this point, building the solution will probably generate a lot of errors.

Some errors are just warnings that are treated by Unreal Build Tool as errors and can be disabled. In helloworld.grpc.pb.cc and helloworld.pb.cc, add the following before includes:

```
#pragma warning(disable: 4668)
#pragma warning(disable: 4800)
```

Abseil errors are caused by a conflict between the function `verify()` defined in the library and UE's macro definition of `verify()`. In C:\src\vcpkg\installed\x64-windows\include\absl\container\internal\btree.h, make the following changes to cancel the UE macro definition in context:

Around line 1579, after modification:
```
#ifdef verify
#undef verify
#endif
  // Verifies the structure of the btree.
  void verify() const;
#ifndef verify
#define verify(expr)			UE_CHECK_IMPL(expr)  // copy from line 221 of /Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h
#endif
```

Around line 2610, after modification:
```
#ifdef verify
#undef verify
#endif
template <typename P>
void btree<P>::verify() const {
  assert(root() != nullptr);
  assert(leftmost() != nullptr);
  assert(rightmost() != nullptr);
  assert(empty() || size() == internal_verify(root(), nullptr, nullptr));
  assert(leftmost() == (++const_iterator(root(), -1)).node_);
  assert(rightmost() == (--const_iterator(root(), root()->finish())).node_);
  assert(leftmost()->is_leaf());
  assert(rightmost()->is_leaf());
}
#ifndef verify
#define verify(expr)			UE_CHECK_IMPL(expr)  // copy from line 221 of /Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h
#endif
```

In file C:\src\vcpkg\installed\x64-windows\include\absl\container\internal\btree_container.h, around line 225, after modification: 

```
#ifdef verify
#undef verify
#endif
  void verify() const { tree_.verify(); }
#ifndef verify
#define verify(expr)			UE_CHECK_IMPL(expr)  // copy from line 221 of /Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h
#endif
```

(Taken from https://forums.unrealengine.com/t/abseil-cpp-absl-in-thirdparty-redefinition-error/1794996/2)
