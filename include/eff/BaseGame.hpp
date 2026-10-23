#pragma once
#include <memory>
#include <vector>
#include <eff/Displayable.hpp>
#include <Window.hpp>

namespace eff {
struct WindowConfig {
    int width = 1280;
    int height = 720;
    const char* title = "EFF Game";
    int targetFps = 60;
    unsigned int flags = 0;
};

class BaseGame {
public:
    virtual ~BaseGame() = default;

    BaseGame(const BaseGame&) = delete;
    BaseGame& operator=(const BaseGame&) = delete;
    
    int run(const WindowConfig& config = {});

    void setDisplay(std::unique_ptr<Displayable> display);
    
    void pushDisplay(std::unique_ptr<Displayable> display);
    void popDisplay();

    raylib::Window& window() { return m_window; }

protected:
    BaseGame() = default;

    virtual void onInit() {}
    virtual void onAppBackground() {}
    virtual void onAppForeground() {}

private:
    raylib::Window m_window;

    enum class CommandType { Set, Push, Pop };

    struct Command {
        CommandType type;
        std::unique_ptr<Displayable> display;
    };
    
    std::vector<std::unique_ptr<Displayable>> m_displays;
    std::vector<Command> m_pending;
    
    bool m_running = false;
    
    void applyCommands();
    void updateDisplays(float dt);
    void renderDisplays();
};
}
