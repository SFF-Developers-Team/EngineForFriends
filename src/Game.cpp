#include <eff/BaseGame.hpp>
#include <stdexcept>
#include <utility>
#include <Color.hpp>

namespace eff {

void BaseGame::setDisplay(std::unique_ptr<Displayable> display) {
    m_pending.push_back({CommandType::Set, std::move(display)});
}

void BaseGame::pushDisplay(std::unique_ptr<Displayable> display) {
    if (!display) throw std::invalid_argument("Cannot push a null display");
    
    m_pending.push_back({CommandType::Push, std::move(display)});
}

void BaseGame::popDisplay() { 
    m_pending.push_back({CommandType::Pop, nullptr}); 
}

void BaseGame::applyCommands() {
    std::vector<Command> commands;
    commands.swap(m_pending);

    for (auto& command : commands) {
        switch (command.type) {
            case CommandType::Set: {
                if (!m_displays.empty()) m_displays.back()->onHide();

                m_displays.clear();
                
                if (command.display) {
                    m_displays.push_back(std::move(command.display));
                    m_displays.back()->onShow();
                }

                break;
            }

            case CommandType::Push: {
                if (!m_displays.empty()) m_displays.back()->onHide();

                m_displays.push_back(std::move(command.display));
                m_displays.back()->onShow();
                
                break;
            }

            case CommandType::Pop: {
                if (!m_displays.empty()) {
                    m_displays.back()->onHide();
                    m_displays.pop_back();
                    
                    if (!m_displays.empty()) m_displays.back()->onShow();
                }

                break;
            }
        }
    }
}

void BaseGame::updateDisplays(float dt) {
    if (m_displays.empty()) return;

    std::size_t first = m_displays.size() - 1;
    
    while (first > 0 && m_displays[first]->updatesBelow()) --first;
    for (std::size_t i = first; i < m_displays.size(); ++i) m_displays[i]->update(dt);
}

void BaseGame::renderDisplays() {
    if (m_displays.empty()) return;

    std::size_t first = m_displays.size() - 1;

    while (first > 0 && m_displays[first]->drawsBelow()) --first;
    for (std::size_t i = first; i < m_displays.size(); ++i) m_displays[i]->render();
}

int BaseGame::run(const WindowConfig& config) {
    if (m_running) throw std::logic_error("Game is already running");
    
    m_running = true;

    m_window.Init(config.width, config.height, config.title, config.flags);

    if (config.targetFps > 0) m_window.SetTargetFPS(config.targetFps);
    
    try {
        onInit();

        bool wasFocused = m_window.IsFocused();

        while (!m_window.ShouldClose()) {
            const bool focused = m_window.IsFocused();

            if (focused != wasFocused) {
                if (focused) onAppForeground();
                else onAppBackground();

                wasFocused = focused;
            }

            applyCommands();
            updateDisplays(m_window.GetFrameTime());
            applyCommands();
            
            m_window.BeginDrawing();
            m_window.ClearBackground(raylib::Color::Black());

            renderDisplays();
            
            m_window.EndDrawing();
        }

        m_displays.clear();
        m_pending.clear();
    } catch (...) {
        m_displays.clear();
        m_pending.clear();
        m_window.Close();
        
        m_running = false;
        throw;
    }

    m_window.Close();
    m_running = false;
    
    return 0;
}

}