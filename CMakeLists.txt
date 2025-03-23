cmake_minimum_required(VERSION 3.10)
project(Claude-OpenGL VERSION 0.1.0 LANGUAGES C CXX)

# C++標準設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# プラットフォーム判定のためのビルド情報出力
message(STATUS "Build platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# オプション
option(BUILD_TESTS "ビルドするテストスイート" OFF)
option(USE_BUNDLED_LIBS "バンドルされたライブラリを使用する" ON)

# プロジェクト共通のインクルードディレクトリ
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad-3.3/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)

# GLADライブラリの設定
set(GLAD_C ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad-3.3/src/gl.c)
set_source_files_properties(${GLAD_C} PROPERTIES LANGUAGE C)

add_library(glad STATIC ${GLAD_C})
target_include_directories(glad PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad-3.3/include
)

# GLFWの設定 - プラットフォームごとに適切なライブラリを選択
set(GLFW_LIBS "")
set(EXTRA_LIBS "")

if(USE_BUNDLED_LIBS)
    # バンドルされたライブラリを使用する場合
    if(APPLE)
        # macOS用の設定
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
            # M1/M2/M3 Mac用
            set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-arm64/libglfw3.a)
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            # Intel Mac用
            set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-x86_64/libglfw3.a)
        else()
            # アーキテクチャが不明な場合はユニバーサルバイナリを使用
            set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-universal/libglfw3.a)
        endif()
        
        # GLFWの静的ライブラリ
        add_library(glfw STATIC IMPORTED)
        set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB_PATH})
        set(GLFW_LIBS glfw)
        
        # Mac用の追加フレームワーク
        find_library(COCOA_LIBRARY Cocoa)
        find_library(OPENGL_LIBRARY OpenGL)
        find_library(IOKIT_LIBRARY IOKit)
        find_library(COREVIDEO_LIBRARY CoreVideo)
        mark_as_advanced(COCOA_LIBRARY OPENGL_LIBRARY IOKIT_LIBRARY COREVIDEO_LIBRARY)
        set(EXTRA_LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
        
    elseif(WIN32)
        # Windows用の設定
        # 32bit vs 64bit の判定
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64bit Windows
            set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-win64/glfw3.lib)
        else()
            # 32bit Windows
            set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-win32/glfw3.lib)
        endif()
        
        add_library(glfw STATIC IMPORTED)
        set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB_PATH})
        set(GLFW_LIBS glfw)
        
        # Windows用の追加ライブラリ
        set(EXTRA_LIBS opengl32.lib)
        
    else()
        # Linux用の設定
        set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-linux/libglfw3.a)
        
        if(EXISTS ${GLFW_LIB_PATH})
            add_library(glfw STATIC IMPORTED)
            set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GLFW_LIB_PATH})
            set(GLFW_LIBS glfw)
            
            # Linux用の追加ライブラリ
            set(EXTRA_LIBS GL X11 pthread Xrandr Xi dl)
        else()
            message(WARNING "Bundled GLFW library not found for Linux, trying system package...")
            set(USE_BUNDLED_LIBS OFF)
        endif()
    endif()
endif()

# バンドルされたライブラリが使用できない場合、システムのパッケージを使用
if(NOT USE_BUNDLED_LIBS)
    # GLFWパッケージの検索
    find_package(glfw3 QUIET)
    
    if(glfw3_FOUND)
        message(STATUS "Found system GLFW3 package")
        set(GLFW_LIBS glfw)
    else()
        message(FATAL_ERROR "GLFW library not found. Please install GLFW3 or provide the bundled libraries.")
    endif()
    
    # OpenGLの検索
    find_package(OpenGL REQUIRED)
    list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})
    
    if(UNIX AND NOT APPLE)
        # Linux用の追加ライブラリ
        list(APPEND EXTRA_LIBS X11 pthread Xrandr Xi dl)
    endif()
endif()

# ソースファイル収集
file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# コンパイラ固有のフラグ設定
if(MSVC)
    # Visual Studioの場合
    add_compile_options(/W4 /MP)
    # エンコーディングをUTF-8に設定
    add_compile_options(/utf-8)
    # 警告をエラーとして扱わないためのオプションを設定
    add_compile_options(/WX-)
else()
    # GCC/Clangの場合
    add_compile_options(-Wall -Wextra)
endif()

# 実行ファイル定義
add_executable(${PROJECT_NAME} ${SOURCES})

# プラットフォーム固有の定義
if(WIN32)
    # Windowsの場合
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_DLL _CRT_SECURE_NO_WARNINGS)
endif()

# インクルードディレクトリ
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# GLFWのインクルードディレクトリを追加（システムパッケージを使用する場合）
if(NOT USE_BUNDLED_LIBS AND glfw3_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${GLFW_INCLUDE_DIRS}
    )
endif()

# ライブラリリンク
target_link_libraries(${PROJECT_NAME} 
    glad
    ${GLFW_LIBS}
    ${EXTRA_LIBS}
)

# アセットをビルドディレクトリにコピー
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Windows向けのDLLコピー (必要な場合)
if(WIN32 AND USE_BUNDLED_LIBS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/bin-win$<$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>:64>$<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:32>/glfw3.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

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