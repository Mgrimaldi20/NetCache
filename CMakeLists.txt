# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required(VERSION 3.25)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	#set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("NetCache")

# Include sub-projects.
add_subdirectory("NetCache")

# Create a main output directory under the build directory where all exe and dll files will be placed
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Binary output directory set: ${CMAKE_BINARY_DIR}")

set_target_properties(NetCache PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(NetCache PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(NetCache PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################################################################################################
############################################## Install Targets #############################################
############################################################################################################
# Install the executable bin directory
if(WIN32)
	install(TARGETS NetCache
		RUNTIME DESTINATION NetCache/bin
	)
endif()

# Install the runtime to the bin directory
if(UNIX)
	install(TARGETS NetBase
		RUNTIME DESTINATION NetCache/bin
	)
endif()
