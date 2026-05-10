#pragma once

#include <string>

#include "text.h"
#include "widget.h"

class CalculatorWidget final : public ui::SingleChildWidget {
public:
    CalculatorWidget();

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(BLContext& context, Position offset) override;
    ui::Event& eventHandler(ui::Event& event) override;

private:
    void buildLayout();
    void syncDisplay();
    void activate(const std::string& action);
    void appendToken(const std::string& token);
    void evaluate();
    void backspace();
    void clear();
    void handleKeyDown(SDL_Keycode key, SDL_Keymod mod);

    [[nodiscard]] std::string displayText() const;
    [[nodiscard]] static std::string formatResult(double value);

    ui::Text* _display = nullptr;
    std::string _expression;
    std::string _status = "0";
    bool _showingResult = false;
};
