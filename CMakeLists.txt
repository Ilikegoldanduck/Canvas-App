# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)

project ("Canvas App" LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
  SDL3 
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
  GIT_TAG release-3.2.0
)

FetchContent_Declare(
  SDL3_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG release-3.2.2
) 

message(STATUS "CHECKPOINT 1: Before SDL3")

FetchContent_MakeAvailable(SDL3)

message(STATUS "CHECKPOINT 2: SDL3 finished")

FetchContent_MakeAvailable(SDL3_ttf)

if(TARGET SDL3_ttf::SDL3_ttf)
    message(STATUS "SDL3_ttf found!")
else()
    message(WARNING "SDL3_ttf target missing!")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Include sub-projects.
add_subdirectory ("App Thing")
