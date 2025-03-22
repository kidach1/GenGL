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

# GLFWの設定 - 静的ライブラリを使用
if(APPLE)
    # Mac用の静的ライブラリパスを設定
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        # M1/M2 Mac
        set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-arm64/libglfw3.a)
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        # Intel Mac
        set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-x86_64/libglfw3.a)
    else()
        # アーキテクチャが不明な場合はユニバーサルバイナリを使用
        set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-universal/libglfw3.a)
    endif()
    
    # GLFWの静的ライブラリ
    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB_PATH})
    
    # Mac用の追加フレームワーク
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OPENGL_LIBRARY IOKIT_LIBRARY COREVIDEO_LIBRARY)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(WIN32)
    # Windows用の設定
    find_package(glfw3 QUIET)
    if(NOT glfw3_FOUND)
        message(FATAL_ERROR "Windows用のGLFWが見つかりません")
    endif()
    set(EXTRA_LIBS "")
else()
    # Linux用の設定
    find_package(glfw3 QUIET)
    if(NOT glfw3_FOUND)
        message(FATAL_ERROR "Linux用のGLFWが見つかりません")
    endif()
    set(EXTRA_LIBS "")
endif()

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
    ${EXTRA_LIBS}  # Macの場合はCocoaやOpenGLなどのフレームワーク
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
