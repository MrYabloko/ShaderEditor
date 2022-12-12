cmake_minimum_required(VERSION 3.23)
project(ShaderEditor)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE IMGUI_FILES external/imgui/*.cpp)

add_executable(ShaderEditor main.cpp
        ${IMGUI_FILES} Project.cpp Project.h project_windows/InProjectWindow.h project_windows/InspectorWindow.cpp project_windows/InspectorWindow.h project_windows/PreviewWindow.cpp project_windows/PreviewWindow.h project_windows/CodeEditorWindow.cpp project_windows/CodeEditorWindow.h utils/RenderTexture.cpp utils/RenderTexture.h utils/Camera.cpp utils/Camera.h utils/Mesh.cpp utils/Mesh.h utils/Vertex.h utils/Shader.cpp utils/Shader.h utils/primitives.h utils/primitives.cpp utils/global_vars.h utils/global_vars.cpp utils/EditableShader.h utils/EditableShader.cpp "code_analyzer/CodeAnalyzer.cpp" "code_analyzer/CodeAnalyzer.h" "code_analyzer/StringHandler.cpp" "code_analyzer/StringHandler.h")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfwSources)

target_link_libraries(ShaderEditor glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(ShaderEditor OpenGL::GL)

add_subdirectory(external/glad)
target_link_libraries(ShaderEditor glad)

add_subdirectory(external/glm)
target_link_libraries(ShaderEditor glm)

target_include_directories(ShaderEditor PRIVATE external/imgui)