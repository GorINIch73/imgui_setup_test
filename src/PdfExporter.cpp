#include "PdfExporter.h"
#include <hpdf.h>

bool PdfExporter::ExportToPdf(const std::vector<std::map<std::string, std::string>>& data, const std::string& filename) {
    HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
    if (!pdf) return false;

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 50, HPDF_Page_GetHeight(page) - 50);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", nullptr), 12);

    // Заголовок
    HPDF_Page_ShowText(page, "Отчет из базы данных");
    HPDF_Page_EndText(page);

    // Вывод данных
    float y = HPDF_Page_GetHeight(page) - 80;
    for (const auto& row : data) {
        std::string line;
        for (const auto& [key, value] : row) {
            line += key + ": " + value + " | ";
        }
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, 50, y);
        HPDF_Page_ShowText(page, line.c_str());
        HPDF_Page_EndText(page);
        y -= 20;
    }

    HPDF_SaveToFile(pdf, filename.c_str());
    HPDF_Free(pdf);
    return true;
}