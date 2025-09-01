[AppName][repo-home] is a _Qt application_ allowing to \<brief description>.

> [!TIP]
> Latest development/pull requests will be committed into `main` branch.  
> Each stable release have their dedicated branch:
> - `1.0.x`: branch `dev/1.0`
> - `1.1.x`: branch `dev/1.1`
> - etc...

**Table of contents :**
- [1. Application details](#1-application-details)
  - [1.1. Features](#11-features)
  - [1.2. Supported platforms](#12-supported-platforms)
    - [1.2.1. Status](#121-status)
    - [1.2.2. Specific behaviours](#122-specific-behaviours)
- [2. Requirements](#2-requirements)
  - [2.1. C++ Standards](#21-c-standards)
  - [2.2. Dependencies](#22-dependencies)
- [3. How to build](#3-how-to-build)
  - [3.1. CMake Usage](#31-cmake-usage)
  - [3.2. CMake options](#32-cmake-options)
- [4. How to use](#4-how-to-use)
  - [4.1. Usage](#41-usage)
  - [4.2. Application version](#42-application-version)
    - [4.2.1. Compilation time](#421-compilation-time)
- [5. License](#5-license)

# 1. Application details
## 1.1. Features

- _Provide application details here_
- _Provide informations about available and unavailable features (what can be do, what can't)_

## 1.2. Supported platforms
### 1.2.1. Status

- _Useful if cross-platform to track current implementation status or the supported/planned/rejected backend_

### 1.2.2. Specific behaviours

- _Because sometimes you can't escape some specific platforms/versions quirks_

# 2. Requirements
## 2.1. C++ Standards

This application requires at least **C++ 17** standard

## 2.2. Dependencies

_A "dependencies" section example_

Below, list of required dependencies:

| Dependencies | VCPKG package | Comments |
|:-:|:-:|:-:|
| [Qt][qt-official] | / | Compatible with **Qt6.x**<b>Compatible with **Qt5.15.x** |

> [!NOTE]
> Dependency manager [VCPKG][vcpkg-tutorial] is not mandatory, this is only a note to be able to list needed packages

# 3. How to build
## 3.1. CMake Usage

This application use **CMake** as build-system, so simply run the `CMakeLists.txt` file to build the application.

## 3.2. CMake options

This application provide some **CMake** build options:
- _add here your custom CMake applicaion options, for each, add: the name, the default value, when to use and what consequences_

# 4. How to use
## 4.1. Usage

_An "usage" section example_

## 4.2. Application version
### 4.2.1. Compilation time

In order to easily check at compilation time application version (to manage compatibility between multiple versions for example), macro `APPNAME_VERSION_ENCODE` (defined inside _appname_global.h_ file) can be used:
```cpp
#if APPNAME_VERSION >= APPNAME_VERSION_ENCODE(2,0,0)
    // Do stuff for version 2.0.0 or higher
#else
    // Do stuff for earlier versions
#endif
```

# 5. License

This application is licensed under [MIT license][repo-license-url].

<!-- Anchor of this page -->
[anchor-platforms]: #12-supported-platforms
[anchor-cmake-opts]: #32-cmake-options

<!-- Links of this repository -->
[repo-home]: https://github.com/username/projectname
[repo-doc-web]: https://username.github.io/projectname/
[repo-license]: LICENSE
[repo-license-url]: https://username.github.io/projectname/LICENSE

<!-- External links -->
[doxygen-official]: https://www.doxygen.nl/index.html

[gtest-repo]: https://github.com/google/googletest

[qt-official]: https://www.qt.io/

[vcpkg-tutorial]: https://github.com/legerch/develop-memo/tree/master/Toolchains/Build%20systems/VCPKG

[semver-home]: https://semver.org
[pimpl-doc-cpp]: https://en.cppreference.com/w/cpp/language/pimpl
[pimpl-doc-qt]: https://wiki.qt.io/D-Pointer