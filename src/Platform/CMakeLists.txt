#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

set(MagnumPlatform_HEADERS
    AbstractContextHandler.h
    ExtensionWrangler.h)

# Extension wrangler
add_library(MagnumPlatformExtensionWrangler OBJECT ExtensionWrangler.cpp)

install(FILES ${MagnumPlatform_HEADERS} DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)

# GLUT application
if(WITH_GLUTAPPLICATION)
    find_package(GLUT)
    if(GLUT_FOUND)
        add_library(MagnumGlutApplication STATIC
            GlutApplication.cpp
            $<TARGET_OBJECTS:MagnumPlatformExtensionWrangler>)
        install(FILES GlutApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
        install(TARGETS MagnumGlutApplication DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
    else()
        message(FATAL_ERROR "GLUT library, required by GlutApplication, was not found. Set WITH_GLUTAPPLICATION to OFF to skip building it.")
    endif()
endif()

# SDL2 application
if(WITH_SDL2APPLICATION)
    find_package(SDL2)
    if(SDL2_FOUND)
        include_directories(${SDL2_INCLUDE_DIR})
        add_library(MagnumSdl2Application STATIC
            Sdl2Application.cpp
            $<TARGET_OBJECTS:MagnumPlatformExtensionWrangler>)
        install(FILES Sdl2Application.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
        install(TARGETS MagnumSdl2Application DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
    else()
        message(FATAL_ERROR "SDL2 library, required by Sdl2Application, was not found. Set WITH_SDL2APPLICATION to OFF to skip building it.")
    endif()
endif()

# NaCl application
if(WITH_NACLAPPLICATION)
    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL NaCl)
        message(FATAL_ERROR "NaClApplication is available only when targeting Google Chrome Native Client. Set WITH_NACLAPPLICATION to OFF to skip building it.")
    endif()

    add_library(MagnumNaClApplication STATIC
        NaClApplication.cpp)
    install(FILES NaClApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
    install(TARGETS MagnumNaClApplication DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
endif()

# Windowless NaCl application
if(WITH_WINDOWLESSNACLAPPLICATION)
    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL NaCl)
        message(FATAL_ERROR "WindowlessNaClApplication is available only when targeting Google Chrome Native Client. Set WITH_WINDOWLESSNACLAPPLICATION to OFF to skip building it.")
    endif()

    add_library(MagnumWindowlessNaClApplication STATIC
        WindowlessNaClApplication.cpp)
    install(FILES WindowlessNaClApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
    install(TARGETS MagnumWindowlessNaClApplication DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
endif()

# GLX application
if(WITH_GLXAPPLICATION)
    set(NEED_ABSTRACTXAPPLICATION 1)
    set(NEED_GLXCONTEXT 1)
    add_library(MagnumGlxApplication STATIC
        $<TARGET_OBJECTS:MagnumAbstractXApplication>
        $<TARGET_OBJECTS:MagnumGlxContextHandler>
        $<TARGET_OBJECTS:MagnumPlatformExtensionWrangler>)
    install(FILES GlxApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
    install(TARGETS MagnumGlxApplication DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
endif()

# X/EGL application
if(WITH_XEGLAPPLICATION)
    set(NEED_ABSTRACTXAPPLICATION 1)
    set(NEED_EGLCONTEXT 1)
    add_library(MagnumXEglApplication STATIC
        $<TARGET_OBJECTS:MagnumAbstractXApplication>
        $<TARGET_OBJECTS:MagnumEglContextHandler>
        $<TARGET_OBJECTS:MagnumPlatformExtensionWrangler>)
    install(FILES XEglApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
    install(TARGETS MagnumXEglApplication DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
endif()

if(WITH_WINDOWLESSGLXAPPLICATION OR NEED_ABSTRACTXAPPLICATION)
    find_package(X11)
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 library, required by some contexts, was not found. Set WITH_*X*APPLICATION to OFF to skip building them.")
    endif()
endif()

# Windowless GLX application
if(WITH_WINDOWLESSGLXAPPLICATION)
    add_library(MagnumWindowlessGlxApplication STATIC
        WindowlessGlxApplication.cpp
        $<TARGET_OBJECTS:MagnumPlatformExtensionWrangler>)
    # X11 macros are a mess, disable warnings for C-style casts
    set_target_properties(MagnumWindowlessGlxApplication PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast")
    install(FILES WindowlessGlxApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
    install(TARGETS MagnumWindowlessGlxApplication DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
endif()

# Abstract X application
if(NEED_ABSTRACTXAPPLICATION)
    add_library(MagnumAbstractXApplication OBJECT AbstractXApplication.cpp)
    # X11 macros are a mess, disable warnings for C-style casts
    set_target_properties(MagnumAbstractXApplication PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast")
    install(FILES AbstractXApplication.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
endif()

# GLX context
if(NEED_GLXCONTEXT)
    add_library(MagnumGlxContextHandler OBJECT GlxContextHandler.cpp)
    # X11 macros are a mess, disable warnings for C-style casts
    set_target_properties(MagnumGlxContextHandler PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast")
    install(FILES GlxContextHandler.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
endif()

# EGL context
if(NEED_EGLCONTEXT)
    find_package(EGL)
    if(NOT EGL_FOUND)
        message(FATAL_ERROR "EGL library, required by some window contexts, was not found. Set WITH_*EGL*APPLICATION to OFF to skip building them.")
    endif()
    add_library(MagnumEglContextHandler OBJECT EglContextHandler.cpp)
    # X11 macros are a mess, disable warnings for C-style casts
    set_target_properties(MagnumEglContextHandler PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast")
    install(FILES EglContextHandler.h DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Platform)
endif()

# Magnum Info
if(WITH_MAGNUMINFO)
    if(NOT UNIX AND NOT CORRADE_TARGET_NACL)
        message(FATAL_ERROR "magnum-info is not available on this platform. Set WITH_MAGNUMINFO to OFF to suppress this warning.")
    endif()

    add_executable(magnum-info magnum-info.cpp)
    if(UNIX AND NOT CORRADE_TARGET_NACL)
        target_link_libraries(magnum-info Magnum MagnumWindowlessGlxApplication ${X11_LIBRARIES})
    elseif(CORRADE_TARGET_NACL)
        target_link_libraries(magnum-info Magnum MagnumWindowlessNaClApplication ppapi_cpp ppapi)
    endif()

    install(TARGETS magnum-info DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})
    if(CORRADE_TARGET_NACL)
        install(FILES magnum-info-nacl.html DESTINATION ${MAGNUM_DATA_INSTALL_DIR} RENAME magnum-info.html)
        install(FILES magnum-info-nacl.nmf DESTINATION ${MAGNUM_DATA_INSTALL_DIR} RENAME magnum-info.nmf)
    endif()
endif()
