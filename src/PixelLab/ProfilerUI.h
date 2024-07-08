//
// Created by Kyle on 7/8/2024.
//

#ifndef PROFILERUI_H
#define PROFILERUI_H

class ProfilerUI {

public:
    ProfilerUI() : m_Enabled(false) {}
    virtual ~ProfilerUI() = default;

    void Show() { m_Enabled = true; }
    void Hide() { m_Enabled = false; }
    void OnGUI();

    [[nodiscard]] bool IsEnabled() const { return m_Enabled; }

protected:
    bool m_Enabled;

};



#endif //PROFILERUI_H
