#include "Gui.h"  
#include <GLFW/glfw3.h>  
#include <iostream>


int main() { 


    if (!glfwInit()) {
        std::cerr << "Ошибка: не удалось инициализировать GLFW" << std::endl;
        return -1;
    }


    GLFWwindow* window = glfwCreateWindow(1280, 720, "SQLite Editor", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
//    gladLoadGL();

    Gui gui;
    gui.Init(window);  // Передаём окно в Gui
    while (!glfwWindowShouldClose(window)) {


        glfwPollEvents();

        gui.Render();
    }

    gui.Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}  
