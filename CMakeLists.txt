cmake_minimum_required(VERSION 4.0)
project(imgui_setup)

# Настройка компилятора для Windows
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()


# Ищем необходимые библиотеки  
find_package(OpenGL REQUIRED)  
find_package(glfw3 REQUIRED)
find_package(SQLite3 REQUIRED)


include(ExternalProject)

ExternalProject_Add(
    libharu
    GIT_REPOSITORY https://github.com/libharu/libharu.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libharu-install
)


# Указываем пути к собранной библиотеке
set(LIBHARU_INCLUDE_DIR ${CMAKE_BINARY_DIR}/libharu-install/include)
if (WIN32)
set(LIBHARU_LIBRARY ${CMAKE_BINARY_DIR}/libharu-install/lib/libhpdf.dll.a)
else()
set(LIBHARU_LIBRARY ${CMAKE_BINARY_DIR}/libharu-install/lib/libhpdf.so)
endif()



ExternalProject_Add(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
#    GIT_TAG v3.11.2
    CMAKE_ARGS -DJSON_BuildTests=OFF -DJSON_Install=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/json_install
    INSTALL_DIR ${CMAKE_BINARY_DIR}/json_install
)

# Указываем пути к установленной библиотеке
set(JSON_INCLUDE_DIR "${CMAKE_BINARY_DIR}/json_install/include")


# Зависимость от сборки nlohmann_json
#add_dependencies(imgui_setup nlohmann_json)



# субмодуль диалога открытия файла
#add_subdirectory(thirdparty/ImGuiFileDialog)




# Подключаем IMGUI, GLFW, Glad  
#add_subdirectory(thirdparty/glfw)  
#add_subdirectory(thirdparty/imgui)  
#add_subdirectory(thirdparty/glad)  

#add_subdirectory(thirdparty/libharu)
#add_subdirectory(thirdparty/json)



# для IMGUI
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/../imgui)  # Путь к ImGui
#message("Значение IMGUI_DIR: ${IMGUI_DIR}")
# для IMGUI
file(GLOB IMGUI_SOURCES 
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)


# для ImGuiFileDialog
set(ImGuiFileDialog_DIR ${CMAKE_SOURCE_DIR}/thirdparty/ImGuiFileDialog)  # Путь
file(GLOB ImGuiFileDialog_SOURCES 
    "${ImGuiFileDialog_DIR}/*.cpp"
)


# Основной исполняемый файл
add_executable(imgui_setup

    src/main.cpp  
    src/Database.cpp  
    src/Gui.cpp  
    src/Settings.cpp  
    src/PdfExporter.cpp  

    ${IMGUI_SOURCES}
	${ImGuiFileDialog_SOURCES}

)

# Подключаем зависимости
target_include_directories(imgui_setup  PRIVATE
    ${CMAKE_SOURCE_DIR}/stc  #наши заголовки
    ${IMGUI_DIR}/
    ${IMGUI_DIR}/backends
    ${OPENGL_INCLUDE_DIR}    
    ${GLFW3_INCLUDE_DIR}
    ${SQLite3_INCLUDE_DIR} 
	${LIBHARU_INCLUDE_DIR}
	${JSON_INCLUDE_DIR}

	${ImGuiFileDialog_DIR}

)





target_link_libraries(imgui_setup PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
    ${SQLite3_LIBRARY} 
	${LIBHARU_LIBRARY}
#	ImGuiFileDialog
)



# Для Windows: копирование DLL GLFW
if(WIN32)
    find_file(GLFW_DLL
        NAMES glfw3.dll
        PATHS ${GLFW_ROOT}/lib-mingw-w64
        REQUIRED
    )
    add_custom_command(TARGET imgui_setup POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${GLFW_DLL}"
        $<TARGET_FILE_DIR:imgui_setup>
    )
endif()
