cmake_minimum_required(VERSION 3.10)
project(Claude-OpenGL VERSION 0.1.0 LANGUAGES C CXX)

# C++標準設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# オプション
option(BUILD_TESTS "ビルドするテストスイート" OFF)

# インクルードディレクトリ
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad-3.3/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)

# GLADライブラリ追加
set(GLAD_C ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad-3.3/src/gl.c)
set_source_files_properties(${GLAD_C} PROPERTIES LANGUAGE C)

add_library(glad STATIC ${GLAD_C})
target_include_directories(glad PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad-3.3/include
)

# GLFWのソースからのビルドを強制的に使用
# セキュリティポリシーの問題を避けるため
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)

# ソースファイル収集
file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# 実行ファイル定義
add_executable(${PROJECT_NAME} ${SOURCES})

# ライブラリリンク
target_link_libraries(${PROJECT_NAME} 
    glad
    glfw
    ${CMAKE_DL_LIBS}  # Linux向けGLAD用
)

# インクルードディレクトリ
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# アセットをビルドディレクトリにコピー
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# テスト
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# インストールルール
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION bin
)
