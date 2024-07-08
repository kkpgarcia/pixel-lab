//
// Created by Kyle on 7/8/2024.
//

#include "imgui.h"
#include "ProfilerUI.h"

#include <PixelEngine/Analysis/Profiler.h>

void ProfilerUI::OnGUI() {
    if (!m_Enabled) return;
    ImGui::Begin("Profiler", &m_Enabled);

    auto results = Profiler::GetResults();
    std::map<std::string, std::tuple<int, float, double>> averages;

    for (const auto& result : results)
    {
        float total = 0.0f;
        for (const auto& entry : result.second)
        {
            total += entry.Duration;
        }

        float memoryTotal = 0.0f;
        for (const auto& entry : result.second)
        {
            memoryTotal += entry.MemoryUsage;
        }

        float average = total / result.second.size();
        float averageMemory = memoryTotal / result.second.size() / (1024.0f);
        averages[result.first] = { result.second.size(), average, averageMemory };
    }

    if (ImGui::BeginTable("Stats", 4))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Samples", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Memory", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        for (auto& result : averages)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(result.first.c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%d", std::get<0>(result.second));

            ImGui::TableNextColumn();
            ImGui::Text("%f ms", std::get<1>(result.second));

            ImGui::TableNextColumn();
            ImGui::Text("%f kb", std::get<2>(result.second));
        }

        ImGui::EndTable();
    }

    ImGui::End();
}
