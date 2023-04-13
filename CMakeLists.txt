cmake_minimum_required(VERSION 3.7.1)

# This is your project name
set(TARGET_NAME openvr_test)

# This is the name that the binary will be called (driver_<my_driver_name>)
set(DRIVER_NAME "driver_${TARGET_NAME}")

project(${TARGET_NAME})

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=2)

set(OPENVR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/openvr)
set(BOOST_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/boost_1_81_0)

include_directories(${BOOST_LIB_DIR})
link_directories(${BOOST_LIB_DIR}/stage/lib)

# If not set, determine the platform architecture
if (NOT PLATFORM)
    if (CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(PLATFORM 64)
    else ()
        set(PLATFORM 32)
    endif ()
endif ()
message(STATUS "Compilation set for ${PLATFORM}bits architectures.")

# OpenVR compatibility checking
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DLINUX -DPOSIX)
    set(ARCH_TARGET linux64)

    if (${PLATFORM} MATCHES 32)
        message(WARNING "OpenVR x86 binaries not provided on GNU/Linux.")
    endif ()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MACOSX_RPATH 0)
    add_definitions(-DOSX -DPOSIX)
    set(ARCH_TARGET osx32)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-D_WIN32)
    set(ARCH_TARGET win${PLATFORM})

    # Binaries path for thirdparties are not generics so we try to guess their suffixes.
    set(WINDOWS_PATH_SUFFIXES win${PLATFORM} Win${PLATFORM} x${PLATFORM})
endif ()

find_library(OPENVR_LIBRARIES
        NAMES
        openvr_api
        PATHS
        ${OPENVR_LIB_DIR}/bin
        ${OPENVR_LIB_DIR}/lib
        PATH_SUFFIXES
        osx${PLATFORM}
        linux${PLATFORM}
        win${PLATFORM}
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH
        )

set(OPENVR_INCLUDE_DIR ${OPENVR_LIB_DIR}/headers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/bin/${ARCH_TARGET}>)

add_library(${DRIVER_NAME} SHARED
        src/hmd_driver_factory.cpp
        src/device_provider.h
        src/device_provider.cpp
        src/controller_device.h
        src/controller_device.cpp
	src/serial_read.h
	src/serial_read.cpp
	src/hand_simulation.h
	src/hand_simulation.cpp
	src/vrmath.h
        )

set(BOOST_SYSTEM_LIB ${BOOST_LIB_DIR}/stage_x64/lib/boost_system-vc143-mt-g-x64-1_81.lib)
set(BOOST_THREAD_LIB ${BOOST_LIB_DIR}/stage_x64/lib/boost_thread-vc143-mt-g-x64-1_81.lib)
target_link_libraries(${DRIVER_NAME} PRIVATE ${OPENVR_LIBRARIES} ${BOOST_SYSTEM_LIB} ${BOOST_THREAD_LIB})
target_include_directories(${DRIVER_NAME} PRIVATE ${OPENVR_INCLUDE_DIR})

# Copy driver assets to output folder
add_custom_command(
        TARGET ${DRIVER_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
)

#add_executable(test_serial_read src/serial_read.cpp src/serial_read.h)
#target_link_libraries(test_serial_read ${BOOST_SYSTEM_LIB} ${BOOST_THREAD_LIB})