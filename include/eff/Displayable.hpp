#pragma once

namespace eff {

class BaseGame;

class Displayable {
public:
    explicit Displayable(BaseGame& game) : m_game(game) {}
    virtual ~Displayable() = default;

    virtual void onShow() {}
    virtual void onHide() {}
    
    virtual void update(float dt) { (void)dt; }
    virtual void render() = 0;
    
    [[nodiscard]] virtual bool drawsBelow() const { return false; }
    [[nodiscard]] virtual bool updatesBelow() const { return false; }

    BaseGame& game() const { return m_game; }
private:
    BaseGame& m_game;
};
}
