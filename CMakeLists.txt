cmake_minimum_required(VERSION 3.28)
project(imgui-sfml-test LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

set(SFML_VERSION 3.0.0)
set(IMGUI_VERSION 1.91.1)
set(IMGUI_SFML_VERSION 3.0)

# It's nice to get stripped-down release zips instead of cloning
# a repo and checking out a tag
FetchContent_Declare(
  SFML
  URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)

FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip
)

# Or by tag...
FetchContent_Declare(
  imgui-sfml
  URL https://github.com/SFML/imgui-sfml/archive/v${IMGUI_SFML_VERSION}.zip
)

## finally, let's build

# SFML
FetchContent_MakeAvailable(sfml)

# Dear ImGui
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" OFF)

FetchContent_MakeAvailable(imgui-sfml)

add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(main PUBLIC ImGui-SFML::ImGui-SFML)
