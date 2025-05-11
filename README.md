# imgui_settings_pdf_sqlite

##IMGUI CMake sqlite тестовое приложение
##открывает базу users

Откройте MSYS2 MinGW x64 и выполните:
pacman -Syu                          # Обновление системы
pacman -S --needed base-devel        # Базовые инструменты разработки
pacman -S mingw-w64-x86_64-toolchain # Компилятор MinGW-w64
pacman -S mingw-w64-x86_64-cmake     # CMake для MinGW
pacman -S mingw-w64-x86_64-glfw      # GLFW (оконная система для ImGui)



// git clone --depth=1 https://github.com/nlohmann/json.git thirdparty/json
// git clone --depth=1 https://github.com/libharu/libharu.git thirdparty/libharu

git clone https://github.com/aiekick/ImGuiFileDialog.git thirdparty/ImGuiFileDialog

IMGUI должет быть в ../imgui

фонты
fontawesome-webfont.ttf
NotoSans-Regular.ttf

для фонтов https://fontdrop.info/


Сборка проекта:
mkdir build && cd build
cmake -G "MinGW Makefiles" ..  //для win
cmake --build . 


некоторые dll нужно положить руками

библиотеку d:\msys64\home\Gor\C_dev\imgui\ds_setup\build\libharu-install\bin\libhpdf.dll нужно с билд руками кинуть подумаю позже

для релиза
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..


для автономной работы надо докинуть из каталога msys64\mingw64\bin\ библиотеки:
libwinpthread-1.dll
libgcc_s_seh-1.dll
libstdc++-6.dll


