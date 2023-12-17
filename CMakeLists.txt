cmake_minimum_required(VERSION 3.13)
project(libsquishtestvss3tc)

if (NOT WIN32 OR NOT MSVC)
	message(FATAL_ERROR "ouch...")
endif()

if ("${CMAKE_SIZEOF_VOIDP}" STREQUAL "8")
	message(FATAL_ERROR "bad news...")
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_subdirectory(libsquish)

add_executable(test WIN32
	dd.cpp
	dd.h
	s3tc.h
	resource.h
	ddsamp.rc
	ddicon.ico
	ddsamp.cpp
	bmp.h
	bmp.cpp
	stdafx.h
	test.h
	test1.cpp
	test2.cpp
	test3.cpp
)

target_link_libraries(test PRIVATE s3tc squish ddraw dxguid)
target_link_options(test PRIVATE "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
target_link_options(test PRIVATE "$<$<C_COMPILER_ID:MSVC>:-NODEFAULTLIB:LIBC.LIB>")
target_link_directories(test PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(test PRIVATE libsquish ${CMAKE_CURRENT_LIST_DIR})
target_precompile_headers(test PRIVATE stdafx.h)
