[QWlanManagerApp][repo-home] is a _Qt application_ allowing to control interface operations, by providing an UI to the library [QWlanManager][qwlanmanager-repo].

> [!TIP]
> Latest development/pull requests will be committed into `main` branch.  
> Each stable release have their dedicated branch:
> - `1.0.x`: branch `dev/1.0`
> - `1.1.x`: branch `dev/1.1`
> - etc...

**Table of contents :**
- [1. Application details](#1-application-details)
  - [1.1. Overview](#11-overview)
  - [1.2. Features](#12-features)
- [2. Requirements](#2-requirements)
  - [2.1. C++ Standards](#21-c-standards)
  - [2.2. Dependencies](#22-dependencies)
- [3. How to build](#3-how-to-build)
  - [3.1. CMake Usage](#31-cmake-usage)
  - [3.2. CMake options](#32-cmake-options)
- [4. License](#4-license)

# 1. Application details
## 1.1. Overview

![doc-ui-app]

## 1.2. Features

This application implements all features available via [QWlanManager library][qwlanmanager-repo], see the associated documentation for more informations.

# 2. Requirements
## 2.1. C++ Standards

This application requires at least **C++ 17** standard

## 2.2. Dependencies

Below, list of required dependencies:

| Dependencies | VCPKG package | Comments |
|:-:|:-:|:-:|
| [Qt][qt-official] | / | Compatible with **Qt6.x**<br>Compatible with **Qt5.15.x** |
| [QWlanManager][qwlanmanager-repo] | / | Included as a git submodule |
| [QLogger][qlogger-repo] | / | Included as a git submodule |
| [ToolBoxQt][toolboxqt-repo] | / | Included as a git submodule |

> [!NOTE]
> Dependency manager [VCPKG][vcpkg-tutorial] is not mandatory, this is only a note to be able to list needed packages

# 3. How to build
## 3.1. CMake Usage

This application use **CMake** as build-system, so simply run the `CMakeLists.txt` file to build the application.

## 3.2. CMake options

This application provide some **CMake** build options:
- _Currently no custom CMake options_

# 4. License

This application is licensed under [MIT license][repo-license].

<!-- Anchor of this page -->

<!-- Links of this repository -->
[repo-home]: https://github.com/lecherch/QWlanManagerApp
[repo-license]: LICENSE.md

[doc-ui-app]: docs/app-ui.png

<!-- External links -->
[qlogger-repo]: https://github.com/legerch/QLogger
[qt-official]: https://www.qt.io/
[qwlanmanager-repo]: https://github.com/legerch/QWlanManager
[toolboxqt-repo]: https://github.com/legerch/ToolBoxQt
[vcpkg-tutorial]: https://github.com/legerch/develop-memo/tree/master/Toolchains/Build%20systems/VCPKG