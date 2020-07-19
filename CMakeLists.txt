# CMakeList.txt : CMake project for MagicGUI, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("MagicGUI")
set(CMAKE_CXX_STANDARD 20)
# Add source to this project's executable.
add_library (MagicGUI SHARED "Src/fasttrigo.cpp" "Src/WinBase.cpp" "Src/Window.cpp" "Src/Texture.cpp" "Src/Widget.cpp" "Src/Application.cpp" "Src/Widgets/Image.cpp" "Src/Shader.cpp" "Src/Windows.cpp"  "Src/Widgets/Canvas.cpp" "Src/Gridsubsystem.cpp" "Src/StaticAllocator.cpp" "Src/Widgets/Button.cpp" "Src/Themes/ThemeManager.cpp" "Src/HString.cpp" "Src/HMath.cpp" "Src/HIO.cpp" "Src/Widgets/ScrollPanel.cpp" "Src/Widgets/ClippedCanvas.cpp"  "Src/Widgets/SlideButton.cpp" "Src/Widgets/TabPanel.cpp" "Src/RenderBuffer.cpp" "Src/Widgets/WindowCanvas.cpp" "Src/Font.cpp" "Src/TextRenderer.cpp" "Src/Widgets/Label.cpp")
target_include_directories(MagicGUI PUBLIC "glew-2.1.0/include")
target_include_directories(MagicGUI PUBLIC "./Inc/Widgets")
target_include_directories(MagicGUI PUBLIC "./Inc/Themes")
target_include_directories(MagicGUI PUBLIC "./Inc/google")
target_include_directories(MagicGUI PUBLIC "./glfw-3.3.2/include")
target_include_directories(MagicGUI PUBLIC "./freetype-2.10.2/include")
target_include_directories(MagicGUI PUBLIC "./glfw-3.3.2/deps")
target_link_directories(MagicGUI PUBLIC "out/build/Mingw64-Release-2/glfw-3.3.2/src")
target_link_directories(MagicGUI PUBLIC "out/build/Mingw64-Release-2/lib")
target_link_directories(MagicGUI PUBLIC "freetype-2.10.2/objs/.libs")
target_link_directories(MagicGUI PUBLIC "C:/msys64/mingw64/lib")


target_link_libraries(MagicGUI libglfw3.a libgdi32.a libGLEW32.a libglu32.a libwinmm.a libfreetype.a libz.a)
add_subdirectory("glfw-3.3.2")
add_subdirectory("glew-2.1.0/build/cmake")
#add_subdirectory("freetype-2.10.2")
#target_include_directories(MagicGUI PUBLIC "C:/msys64/mingw64/include")
#target_include_directories(MagicGUI PUBLIC "C:/msys64/mingw64/include/c++/9.3.0")
target_include_directories(MagicGUI PUBLIC ".")
target_include_directories(MagicGUI PUBLIC "Inc")
target_include_directories(MagicGUI PUBLIC "")
add_definitions(-DBUILD)
# TODO: Add tests and install targets if needed.
add_custom_command(TARGET MagicGUI POST_BUILD COMMAND "robocopy . C:/Bin libMagicGUI.dll & Ver > nul ") 