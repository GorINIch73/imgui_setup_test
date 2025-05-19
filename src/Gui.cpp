#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#include "IconsFontAwesome6.h"
#include "ImGuiFileDialog.h"
#include "PdfExporter.h"

void Gui::Init(GLFWwindow *window) {
    this->window = window; // Сохраняем переданное окно

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // грузим русский шрифт
    ImFont *font =
        io.Fonts->AddFontFromFileTTF("NotoSans-Regular.ttf", 24.0f, nullptr,
                                     io.Fonts->GetGlyphRangesCyrillic());

    //    ImFont* mainFont =
    //    io.Fonts->AddFontFromFileTTF("Roboto-Regular.ttf", 16.0f);

    // Добавляем Font Awesome
    ImFontConfig config;
    config.MergeMode = true;
    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 14.0f, &config,
                                 icons_ranges);

    // Настройка иконок в ImGuiFileDialog
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png",
                                              ImVec4(0.0f, 1.0f, 1.0f, 0.9f),
                                              ICON_FA_FILE_IMAGE);
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".db",
                                              ImVec4(0.0f, 1.0f, 1.0f, 0.9f),
                                              ICON_FA_BOOK);
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".txt",
                                              ImVec4(0.0f, 1.0f, 1.0f, 0.9f),
                                              ICON_FA_FILE_LINES);
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "",
                                              ImVec4(0.0f, 1.0f, 1.0f, 0.9f),
                                              ICON_FA_FOLDER); // Папки

    // загружаем данные из конфига
    settings.Load("config.json");
    //	std::cout << "old file: " << settings.lastDbPath;
    recentFiles.push_back(settings.lastDbPath);
}

void Gui::Render() {

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    // ограничиваем FPS
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    double elapsed =
        std::chrono::duration<double>(current_time - last_time).count();

    if (elapsed < 1.0 / 60.0) { // 60 FPS для ImGui
        return;
    }

    last_time = current_time;

    // рисуем
    ImGui::NewFrame();

    ShowMainMenu();
    if (db.IsOpen())
        ShowDatabaseEditor();
    if (showSettings)
        ShowSettings();
    if (showAbout)
        ShowAbout();
    if (showFileDialog)
        ShowFileDialog();

    static double fps = 0.0;
    static auto last_fps_time = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    fps = 0.9 * fps +
          0.1 / std::chrono::duration<double>(now - last_fps_time).count();
    last_fps_time = now;
    ImGui::Text("FPS: %.1f", fps);

    //	ImGui::ShowMetricsWindow();  // Показывает нагрузку (количество окон,
    // вершины и т. д.)

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call
    //  glfwMakeContextCurrent(window) directly)
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    // glClear(GL_COLOR_BUFFER_BIT);
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window); // Используем сохранённое окно
}

void Gui::ShowMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Файл")) {
            if (ImGui::MenuItem("Открыть базу")) {
                showFileDialog = true;
                // Логика открытия файла (используем ImGuiFileDialog)
            }
            if (ImGui::MenuItem("Закрыть базу")) {
                db.Close();
            }
            if (ImGui::BeginMenu("Последние файлы")) {
                for (const auto &file : recentFiles) {
                    if (ImGui::MenuItem(file.c_str())) {
                        db.Open(file);
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Выход")) {
                if (db.IsOpen())
                    db.Close();
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Редактировать")) {
            if (ImGui::MenuItem("Добавить запись")) { /* ... */
            }
            if (ImGui::MenuItem("Удалить запись")) { /* ... */
            }
            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Отчет")) {
            GeneratePdfReport();
        }

        if (ImGui::MenuItem("Настройки")) {
            showSettings = true;
        }

        if (ImGui::MenuItem("Об авторах")) {
            showAbout = true;
        }

        ImGui::EndMainMenuBar();
    }
}

void Gui::ShowDatabaseEditor() {
    ImGui::Begin("Редактор базы данных");

    // Вывод таблицы
    auto data = db.FetchAll("SELECT * FROM users");
    if (ImGui::BeginTable("DatabaseTable", 3)) {
        for (const auto &row : data) {
            ImGui::TableNextRow();
            for (const auto &[key, value] : row) {
                ImGui::TableNextColumn();
                ImGui::Text("%s: %s", key.c_str(), value.c_str());
            }
        }
        ImGui::EndTable();
    }

    // Форма добавления записи
    static char inputBuf[256] = "";
    ImGui::InputText("Новая запись", inputBuf, IM_ARRAYSIZE(inputBuf));
    if (ImGui::Button("Добавить")) {
        std::string query =
            "INSERT INTO users (name) VALUES ('" + std::string(inputBuf) + "')";
        db.Execute(query);
    }

    ImGui::End();
}

void Gui::ShowFileDialog() {
    if (!showFileDialog)
        return;

    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog(
        "ChooseFileDlg", // Уникальный ID
        "Выберите файл", // Заголовок
        //            ".*,.db,.pdf",     // Фильтры (через запятую)
        ".db", // Фильтр (через запятую)
        config // Стартовая папка
    );

    // Обработка выбора файла
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlg")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();

            // std::cout << "Выбран файл имя: " << filePathName << std::endl;
            // std::cout << "Выбран файл: " << filePath << std::endl;

            // Пример: загрузка базы данных
            db.Open(filePathName);

            // Добавляем в список последних файлов и сохраняем
            recentFiles.push_back(filePathName);
            settings.lastDbPath = filePathName;
            settings.Save("config.json");
        }

        // Закрываем диалог
        ImGuiFileDialog::Instance()->Close();
        showFileDialog = false;
    }
}

void Gui::ShowSettings() {
    if (!showSettings)
        return;

    ImGui::Begin("Настройки", &showSettings);
    ImGui::Combo("Тема", &settings.theme, "Светлая\0Темная\0");

    if (ImGui::Button("Сохранить")) {
        settings.Save("config.json");
    }
    ImGui::End();
}

void Gui::ShowAbout() {
    if (!showAbout)
        return;

    ImGui::Begin("Об авторах", &showAbout);

    ImGui::End();
}

void Gui::GeneratePdfReport() {
    auto data = db.FetchAll("SELECT * FROM users");
    PdfExporter exporter;
    exporter.ExportToPdf(data, "report.pdf");
}

void Gui::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}
