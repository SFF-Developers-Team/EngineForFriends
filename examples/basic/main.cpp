#include <eff/BaseGame.hpp>
#include <raylib-cpp.hpp>
#include <memory>

class PauseScreen final : public eff::Displayable {
public:
    explicit PauseScreen(eff::BaseGame& game) : Displayable(game) {}

    bool updatesBelow() const override { return false; }
    bool drawsBelow() const override { return true; }

    void render() override {
        auto& window = game().window();

        raylib::Rectangle(window.GetSize()).Draw(raylib::Color::Black().Alpha(0.5f));
        raylib::Text paused("Paused! Press ESC to unpause", 30);
        paused.SetSpacing(1.f);
        paused.Draw(window.GetSize() / 2.f - paused.MeasureEx() / 2.f);
    }

    void update(float dt) override {
        if (raylib::Keyboard::IsKeyPressed(KEY_ESCAPE)) {
            game().popDisplay();
        }
    }
};

class MainScreen final : public eff::Displayable {
public:
    explicit MainScreen(eff::BaseGame& game) : Displayable(game) {}

    void render() override {
        raylib::DrawText("EFF is running!", 60, 80, 30, raylib::Color::Purple());
        raylib::DrawText("Press ESC to pause", 60, 130, 20, raylib::Color::Gray());
    }

    void update(float dt) override {
        if (raylib::Keyboard::IsKeyPressed(KEY_ESCAPE)) {
            game().pushDisplay(std::make_unique<PauseScreen>(game()));
        }
    }
};


class MyGame final : public eff::BaseGame {
    void onInit() override {
        setDisplay(std::make_unique<MainScreen>(*this));
        window().SetExitKey(0);
    }
};

int main() {
    MyGame game;
    return game.run({.width = 960, .height = 540, .title = "EFF Example"});
}
