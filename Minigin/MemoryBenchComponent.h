#pragma once
#include "Component.h"
#include <chrono>
#include <map>
#include <algorithm>

#include "GameObject3D.h"
#include "imgui_plot.h"

// This whole class is a total mess from hell
// I was on a tight schedule due to my illness and this is best I can do in very limited time

// Global function to generate unique IDs
inline uint32_t GetUniqueID() {
    static uint32_t uniqueID = 0;
    return uniqueID++;
}

template <typename C, class = std::enable_if_t<
    std::is_same_v<C, uint32_t> || 
    std::is_same_v<C, GameObject3D> || 
    std::is_same_v<C, GameObject3DAlt>>>
class MemoryBenchComponent : public Component
{
public:
    void FixedUpdate(const float) override {}
    void Update(const float) override {}
    void LateUpdate(const float) override {}
    void Render() const override
    {
        ImGui::PushID(m_ID);

        ImGui::PushItemWidth(50);
        ImGui::TextUnformatted(m_Title.c_str());
        ImGui::InputInt("", &m_SampleCount, 0, 0, ImGuiInputTextFlags_CharsDecimal);
        ImGui::SameLine();
        ImGui::TextUnformatted("# samples");
        
        ImGui::SameLine();
        if (ImGui::Button("-"))
        {
            m_SampleCount = std::max(10, m_SampleCount - 10);
        }
        
        ImGui::SameLine();
        if (ImGui::Button("+"))
        {
            m_SampleCount = std::min(150, m_SampleCount + 10);
        }
        
        if (ImGui::Button("Trash the cache")) 
        {
            ApplyBenchMark();
            m_RenderOtherBench = true;
        }

        ImGui::Plot("Graph", m_Config);

        ImGui::NewLine();

        if (m_OtherBench && m_RenderOtherBench)
        {
            ImGui::PlotConfig compConfig;
        }

        ImGui::PopID();
    }

    ~MemoryBenchComponent() override
    {
        delete m_pBuffer;
    }

    MemoryBenchComponent(const MemoryBenchComponent&) = delete;
    MemoryBenchComponent(MemoryBenchComponent&&) noexcept = delete;
    MemoryBenchComponent& operator=(const MemoryBenchComponent&) = delete;
    MemoryBenchComponent& operator=(MemoryBenchComponent&&) noexcept = delete;

protected:
    explicit MemoryBenchComponent(dae::GameObject& owner, uint32_t sampleCount = 50,
        uint32_t bufferSize = 67108864, uint32_t maxStepSize = 1024)
        : Component(owner),
          m_ID(GetUniqueID()),
          m_RenderPlot(true),
          m_SampleCount(sampleCount),
          m_BufferSize(bufferSize),
          m_MaxStepSize(maxStepSize)
    {
        m_OtherBench = owner.GetComponent<MemoryBenchComponent>();

        m_pBuffer = new C[m_BufferSize];

        FillBuffer();

        m_Title = "Benchmark memory for";

        if constexpr (std::is_same_v<C, uint32_t>)
        {
            m_Config.values.color = ImColor(255, 165, 0);
            m_Title += " uint32_t";
        }
        else if constexpr (std::is_same_v<C, GameObject3D>)
        {
            m_Config.values.color = ImColor(0, 255, 0);
            m_Title += " GameObject3D";
        }
        else  // GameObject3DAlt
        {
            m_Title += " GameObject3DAlt";
            m_Config.values.color = ImColor(0, 255, 0);
        }

        m_Config.v_lines.show = true;
        m_Config.frame_size = ImVec2(200, 100);
        m_Config.grid_x.show = true;
        m_Config.grid_y.show = true;
        m_Config.tooltip.show = true;
    }

    template <typename T, typename ... Args>
    friend T* dae::GameObject::AddComponent(Args&&... args);

    C* m_pBuffer;
private:
    void FillBuffer()
    {
        if constexpr (std::is_same_v<C, uint32_t>)
        {
            for (uint32_t i = 0; i < m_BufferSize; i++) m_pBuffer[i] = i;
        }
        else
        {
            for (uint32_t i = 0; i < m_BufferSize; i++) m_pBuffer[i].ID = i;
        }
    }

    void RunBenchMark(uint32_t currentStepSize) const
    {
        if constexpr (std::is_same_v<C, uint32_t>)
        {
            for (uint32_t i = 0; i < m_BufferSize; i += currentStepSize) m_pBuffer[i]++;
        }
        else
        {
            for (uint32_t i = 0; i < m_BufferSize; i += currentStepSize) m_pBuffer[i].ID++;
        }
    }

    void ApplyBenchMark() const
    {
        std::map<uint32_t, std::vector<long long>> stepTimes;

        for (int sample = 0; sample < m_SampleCount; sample++)
        {
            for (uint32_t stepSize = 1; stepSize <= m_MaxStepSize; stepSize *= 2)
            {
                const auto start{ std::chrono::high_resolution_clock::now() };
                RunBenchMark(stepSize);
                const auto end{ std::chrono::high_resolution_clock::now() };
                const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                stepTimes[stepSize].push_back(elapsedTime);
            }
        }

        std::vector<float> stepSizes;
        std::vector<std::vector<float>> times;

        for (auto& step : stepTimes)
        {
            long long sum = 0;

            std::sort(step.second.begin(), step.second.end());
            step.second.erase(step.second.begin());
            step.second.pop_back();

            for (auto time : step.second)
            {
                sum += time;
            }

            float avgTime = static_cast<float>(sum) / static_cast<float>(step.second.size());

            stepSizes.push_back(static_cast<float>(step.first));
            times.push_back({ avgTime });
        }

        static std::vector<float> xs(stepSizes.begin(), stepSizes.end());
        static std::vector<float> ys;
        for (const auto& time : times)
        {
            ys.insert(ys.end(), time.begin(), time.end());
        }

        m_Config.values.xs = xs.data();
        m_Config.values.ys = ys.data();
        m_Config.values.count = static_cast<int>(stepSizes.size());
        m_Config.values.ys_count = 1;
        m_Config.scale.min = *std::min_element(ys.begin(), ys.end());
        m_Config.scale.max = *std::max_element(ys.begin(), ys.end());
        m_Config.grid_x.size = static_cast<float>(m_MaxStepSize) / 10.0f;
        m_Config.grid_y.size = (m_Config.scale.max - m_Config.scale.min) / 10.0f;
        m_Config.frame_size = ImVec2(200, 100);
    }

    mutable bool m_RenderOtherBench = false;
    MemoryBenchComponent* m_OtherBench;
    std::string m_Title;
    int m_ID;
    mutable bool m_RenderPlot = true;
    mutable int m_SampleCount;
    const uint32_t m_BufferSize;
    const uint32_t m_MaxStepSize;
    mutable ImGui::PlotConfig m_Config;
};
