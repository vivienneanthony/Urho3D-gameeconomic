#
# Copyright (c) 2008-2014 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# Set project name
project (Urho3D-GameEconomic)

# Find Urho3D library
find_package (Urho3D REQUIRED)

# Only add MySQL to the server
find_package (MySql REQUIRED)

include_directories (${URHO3D_INCLUDE_DIRS})

# Add common files based on type of client
if (URHO3D_GAMEECONOMICSERVER OR GAMEECONOMICSERVERCLIENT)
	# Include common to all samples
	set (COMMON_SAMPLE_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicAppHeadless.h" "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicAppHeadless.inl")
endif()

if (URHO3D_GAMEECONOMICGAMECLIENT)
	# Include common to all samples
	set (COMMON_SAMPLE_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicAppHead.h" "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicAppHead.inl")
endif()

# Define dependency libs
set (INCLUDE_DIRS_ONLY ${CMAKE_CURRENT_SOURCE_DIR})

# All Required Server Files
file (GLOB SERVER_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicComponents/*.cpp")
file (GLOB SERVER_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicComponents/*.h")


list(REMOVE_ITEM SERVER_CPP_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicComponents/GameStateHandlerComponent.cpp")
list(REMOVE_ITEM SERVER_H_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicComponents/GameStateHandlerComponent.h")

# All Required Game Client files
# All Required Server Files
file (GLOB ADDITIONAL_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicComponents/*.cpp")
file (GLOB ADDITIONAL_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GameEconomicComponents/*.h")


# Add Code
if (URHO3D_GAMEECONOMICSERVER)
	add_subdirectory (GameEconomicServer)
endif ()

if (URHO3D_GAMEECONOMICSERVERCLIENT)
	add_subdirectory (GameEconomicServerClient)
endif ()

if (URHO3D_GAMEECONOMICGAMECLIENT)
	add_subdirectory (GameEconomicGameClient)
endif ()



