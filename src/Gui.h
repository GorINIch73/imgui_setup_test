#pragma once
#include "Database.h"
#include "Settings.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#
class Gui {
    public:
        void Init(GLFWwindow *window); // Принимаем окно
        void Render();
        void Shutdown();

    private:
        GLFWwindow *window = nullptr; // Храним указатель на окно
        Database db;
        Settings settings;
        std::vector<std::string> recentFiles;
        bool showSettings = false;
        bool showAbout = false;
        bool showFileDialog = false;

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        void ShowMainMenu();
        void ShowDatabaseEditor();
        void ShowSettings();
        void ShowAbout();
        void ShowFileDialog();
        void GeneratePdfReport();
};

