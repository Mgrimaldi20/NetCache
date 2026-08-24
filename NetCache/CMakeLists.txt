# CMakeList.txt : CMake project for NetCache, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.25)

# Add source to this project's executable
add_executable(NetCache)

project(NetCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set_property(TARGET NetCache PROPERTY CXX_STANDARD 23)

# Set the NetCache version
target_compile_definitions(NetCache PRIVATE NETCACHE_VERSION=1)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(NetCache PRIVATE NETCACHE_DEBUG)
endif()

if(WIN32)
	target_compile_definitions(NetCache PRIVATE UNICODE _UNICODE)
	target_compile_definitions(NetCache PRIVATE _WIN32_WINNT=0x0601)
endif()

if(MSVC)
	target_compile_definitions(NetCache PRIVATE NETCACHE_MSVC)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_definitions(NetCache PRIVATE NETCACHE_CLANG)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_definitions(NetCache PRIVATE NETCACHE_GCC)
endif()

if (WIN32)
	target_compile_definitions(NetCache PRIVATE NETCACHE_WIN32)
elseif (APPLE)
	target_compile_definitions(NetCache PRIVATE NETCACHE_APPLE)
elseif (UNIX)
	target_compile_definitions(NetCache PRIVATE NETCACHE_LINUX)
endif()

# Specify the common OS independent source files for the NetCache project
target_sources(NetCache PRIVATE
	"src/Main.cpp"
	"src/framework/Asio.h"
	"src/framework/Asio.cpp"
	"src/framework/Server.h"
	"src/framework/Server.cpp"
	"src/framework/Client.h"
	"src/framework/Session.h"
	"src/framework/Session.cpp"
	"src/framework/Channel.h"
	"src/framework/Channel.cpp"
	"src/framework/ChannelManager.h"
	"src/framework/ChannelManager.cpp"
	"src/framework/CmdDispatcher.h"
	"src/framework/CmdDispatcher.cpp"
	"src/framework/log/Log.h"
	"src/framework/log/Log.cpp"
	"src/framework/log/driver/Driver.h"
	"src/framework/log/driver/Driver.cpp"
	"src/framework/log/entry/Entry.h"
	"src/framework/log/entry/EntryBuilder.h"
	"src/framework/log/entry/EntryBuilderImpl.h"
	"src/framework/log/entry/EntryBuilderImpl.cpp"
	"src/framework/log/sink/Sink.h"
	"src/framework/log/sink/text/TextSink.h"
	"src/framework/log/sink/text/file/FileSink.h"
	"src/framework/log/sink/text/file/FileSink.cpp"
	"src/framework/log/sink/text/console/ConsoleSink.h"
	"src/framework/log/sink/text/console/ConsoleSink.cpp"
	"src/framework/log/formatter/text/TextFormatter.h"
	"src/framework/log/formatter/text/basic/BasicTextFormatter.h"
	"src/framework/log/formatter/text/basic/BasicTextFormatter.cpp"
	"src/framework/log/policy/Policy.h"
	"src/framework/log/policy/level/LevelPolicy.h"
	"src/framework/log/policy/level/LevelPolicy.cpp"
	"src/framework/log/policy/trace/StacktracePolicy.h"
	"src/framework/log/policy/trace/StacktracePolicy.cpp"
	"src/framework/log/policy/trace/SourceLocationPolicy.h"
	"src/framework/log/policy/trace/SourceLocationPolicy.cpp"
 "src/protocol/Parser.h")

message(STATUS "The full source include path is ${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(NetCache PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Add the ASIO library as an interface target
add_library(asio INTERFACE)

target_include_directories(asio SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/asio/include")

target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

if(WIN32)
	target_compile_definitions(asio INTERFACE ASIO_HAS_IOCP)
elseif(UNIX)
	target_compile_definitions(asio INTERFACE ASIO_HAS_IO_URING)
endif()

target_link_libraries(NetCache PRIVATE asio)

set_target_properties(NetCache PROPERTIES COMPILE_WARNING_AS_ERROR ON)

# Set up all the compiler options here
# MSVC compiler options
if(MSVC)
	target_compile_options(NetCache PRIVATE
		"/external:W0"
		"/external:anglebrackets"
		"/analyze:external-"
	)

	# Common options for all build types
	target_compile_options(NetCache PRIVATE
		"/W4"
		"/WX"
		"/permissive-"
		"/analyze"
		"/fp:fast"
		"/FAs"
		"/MP"
		"/Zc:__cplusplus"
	)

	# Debug build compiler options
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(NetCache PRIVATE
			"/Zi"
			"/fsanitize=address"
			"/Od"
			"/MDd"
			"/JMC"
		)

		# Ensure PDB file is generated (Win32)
		target_link_options(NetCache PRIVATE "/DEBUG")

	# Release build compiler options
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_options(NetCache PRIVATE
			"/O2"
			"/MD"
			"/GL"
			"/LTCG"
			"/Gw"
			"/Z7"
		)
	endif()

# CLANG and GCC compiler options
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# Common options for all build types
	target_compile_options(NetCache PRIVATE
		"-Wall"
		"-Werror"
		"-Wpedantic"
		"-ffast-math"
		"-fanalyzer"
		"-fvisibility=hidden"
		"-g"
	)

	# Debug build compiler options
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_options(NetCache PRIVATE
			"-fsanitize=address"
			"-fsanitize=leak"
			"-fsanitize=undefined"
			"--fsanitize=thread"
			"-O0"
		)

		target_link_options(NetCache PRIVATE
			"-fsanitize=address"
			"-fsanitize=leak"
			"-fsanitize=undefined"
			"--fsanitize=thread"
		)

	# Release build compiler options
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_options(NetCache PRIVATE
			"-O3"
			"-flto"
		)
	endif()
endif()

# TODO: Add tests and install targets if needed.
