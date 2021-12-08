cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(TOP-WASM-DEMO CXX C)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    # common compile options
    add_compile_options(
            /Zc:auto,forScope,inline,wchar_t
            /Zc:externConstexpr   # https://docs.microsoft.com/en-us/cpp/build/reference/zc-externconstexpr
            /Zc:implicitNoexcept  # https://docs.microsoft.com/en-us/cpp/build/reference/zc-implicitnoexcept-implicit-exception-specifiers
            /Zc:noexceptTypes     # https://docs.microsoft.com/en-us/cpp/build/reference/zc-noexcepttypes
            /Zc:referenceBinding  # https://docs.microsoft.com/en-us/cpp/build/reference/zc-referencebinding-enforce-reference-binding-rules
            /Zc:rvalueCast        # https://docs.microsoft.com/en-us/cpp/build/reference/zc-rvaluecast-enforce-type-conversion-rules
            /Zc:sizedDealloc      # https://docs.microsoft.com/en-us/cpp/build/reference/zc-sizeddealloc-enable-global-sized-dealloc-functions
            /Zc:strictStrings     # https://docs.microsoft.com/en-us/cpp/build/reference/zc-strictstrings-disable-string-literal-type-conversion
            /Zc:threadSafeInit    # https://docs.microsoft.com/en-us/cpp/build/reference/zc-threadsafeinit-thread-safe-local-static-initialization
            /Zc:throwingNew       # https://docs.microsoft.com/en-us/cpp/build/reference/zc-throwingnew-assume-operator-new-throws
            /Zc:trigraphs-        # https://docs.microsoft.com/en-us/cpp/build/reference/zc-trigraphs-trigraphs-substitution
            /Zc:__cplusplus       # https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus
            /permissive-          # https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/
                                  # https://blogs.msdn.microsoft.com/vcblog/2017/09/11/two-phase-name-lookup-support-comes-to-msvc/
                                  # https://docs.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance
            /volatile:iso         # https://docs.microsoft.com/en-us/cpp/build/reference/volatile-volatile-keyword-interpretation
            /GS
            /Zi
            /Gm-                  # Disable Minimal Rebuild
            /EHsc
            /fp:precise
            /errorReport:prompt
            /GF                   # Eliminate Duplicate Strings
            /Gd                   # Specifies the __cdecl calling convention for all functions except C++ member functions and functions that are marked __stdcall, __fastcall, or __vectorcall.
            /diagnostics:caret    # https://docs.microsoft.com/en-us/cpp/build/reference/diagnostics-compiler-diagnostic-options
            /doc
            /wd4634               # XML document comment target: cannot be applied: reason
            /wd4635
            /utf-8                # https://docs.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8
            /WL                   # https://docs.microsoft.com/en-us/cpp/build/reference/wl-enable-one-line-diagnostics
    )
endif()

include_directories(C:/Users/user/Documents/github/vcpkg/installed/x64-windows/include)
include_directories(D:/Documents/github/vcpkg/installed/x64-windows/include)
include_directories(./)

add_subdirectory(xbasic)
add_subdirectory(xcodec)
add_subdirectory(xcommon)
add_subdirectory(xconfig)
add_subdirectory(xdata)
add_subdirectory(xstate_accessor)
add_subdirectory(xstate_accessor_c)
add_subdirectory(xwasm_rs)
