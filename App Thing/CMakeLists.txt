# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CMakeProject1 "App Thing.cpp" "App Thing.h" "functions.h" "Canvas.cpp" "Gui.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(CMakeProject1 PRIVATE SDL3::SDL3)

# TODO: Add tests and install targets if needed.
