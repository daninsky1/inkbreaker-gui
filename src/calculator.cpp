#include "calculator.h"

#include "align.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <format>
#include <sstream>
#include <vector>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <tinyexpr.h>

#include "button.h"
#include "grid.h"
#include "padding.h"
#include "render_strategy.h"
#include "text.h"

namespace {

constexpr BLRgba32 Background{32, 34, 37, 255};
constexpr BLRgba32 Panel{42, 44, 48, 255};
constexpr BLRgba32 DigitButton{58, 61, 66, 255};
constexpr BLRgba32 DigitButtonHover{68, 72, 78, 255};
constexpr BLRgba32 DigitButtonPressed{76, 81, 88, 255};
constexpr BLRgba32 OperatorButton{69, 74, 84, 255};
constexpr BLRgba32 OperatorButtonHover{83, 89, 101, 255};
constexpr BLRgba32 OperatorButtonPressed{92, 99, 113, 255};
constexpr BLRgba32 AccentButton{53, 132, 228, 255};
constexpr BLRgba32 AccentButtonHover{68, 145, 238, 255};
constexpr BLRgba32 AccentButtonPressed{28, 113, 216, 255};
constexpr BLRgba32 UtilityButton{49, 51, 56, 255};
constexpr BLRgba32 UtilityButtonHover{61, 64, 70, 255};
constexpr BLRgba32 UtilityButtonPressed{69, 73, 80, 255};
constexpr BLRgba32 TextPrimary{246, 245, 244, 255};
constexpr const char* RegularFont = "resources/fonts/roboto/static/Roboto-Regular.ttf";
constexpr const char* BoldFont = "resources/fonts/roboto/static/Roboto-Bold.ttf";

bool isOperatorAction(const std::string& action)
{
    return action == "+" || action == "-" || action == "*" || action == "/";
}

bool isNumberAction(const std::string& action)
{
    return action.size() == 1 && ((action[0] >= '0' && action[0] <= '9') || action[0] == '.');
}

bool endsWithOperator(const std::string& value)
{
    return !value.empty() && isOperatorAction(std::string(1, value.back()));
}

} // namespace

CalculatorWidget::CalculatorWidget()
{
    buildLayout();
}

Size CalculatorWidget::layout(const BoxConstraints& boxConstraints)
{
    _size = {boxConstraints.maxWidth, boxConstraints.maxHeight};
    if (_child != nullptr) {
        _childPosition = {0, 0};
        _child->layout(boxConstraints);
    }
    return normalize(boxConstraints);
}

void CalculatorWidget::render(BLContext& context, Position offset)
{
    getRenderStrategy().drawCalculator(context, offset, _size);

    if (_child != nullptr) {
        _child->render(context, offset.add(_childPosition));
    }
}

ui::Event& CalculatorWidget::eventHandler(ui::Event& event)
{
    if (event.sdlEvent.type == SDL_EVENT_KEY_DOWN) {
        handleKeyDown(event.sdlEvent.key.key, event.sdlEvent.key.mod);
        event.handled = true;
        return event;
    }

    return SingleChildWidget::eventHandler(event);
}

void CalculatorWidget::buildLayout()
{
    const std::vector<std::vector<std::pair<std::string, std::string>>> rowsSpec = {
        {{"C", "clear"}, {"(", "("}, {")", ")"}, {"/", "/"}},
        {{"7", "7"}, {"8", "8"}, {"9", "9"}, {"*", "*"}},
        {{"4", "4"}, {"5", "5"}, {"6", "6"}, {"-", "-"}},
        {{"1", "1"}, {"2", "2"}, {"3", "3"}, {"+", "+"}},
        {{"0", "0"}, {".", "."}, {"DEL", "backspace"}, {"=", "equals"}}
    };

    auto* grid = new ui::Grid(4, 6);
    grid->setColor(Background);
    grid->setPadding({24, 16, 24, 16});
    grid->setGap(8);

    std::vector<ui::Widget*> children;
    std::vector<ui::Grid::Cell> cells;

    _display = new ui::Text(displayText());
    _display->setFontSize(42);
    _display->setColor(TextPrimary);
    _display->setFontFilepath(RegularFont);
    _display->setHorizontalAlignment(ui::Text::TextAlignment::Right);
    auto* displayPadding = new ui::Padding();
    displayPadding->setColor(Panel);
    displayPadding->setPadding({0, 12, 0, 12});
    displayPadding->setChild(*_display);
    auto* displayAlign = new ui::Align();
    displayAlign->setAlignment(ui::Alignment::CenterRight);
    displayAlign->setColor(Panel);
    displayAlign->setChild(*displayPadding);
    children.push_back(displayAlign);
    cells.push_back({.column = 0, .row = 0, .columnSpan = 4, .rowSpan = 1});

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 4; ++col) {
            const auto& [label, action] = rowsSpec[row][col];
            auto* text = new ui::Text(label);
            text->setFontSize(label == "DEL" ? 20 : 24);
            text->setColor(TextPrimary);
            text->setFontFilepath(BoldFont);
            text->setHorizontalAlignment(ui::Text::TextAlignment::Center);

            auto* button = new ui::Button(*text);
            button->setPadding({0, 0, 0, 0});
            button->setOnPressed([this, action] {
                activate(action);
            });

            if (action == "equals") {
                button->setColor(AccentButton);
                button->setHoverColor(AccentButtonHover);
                button->setPressedColor(AccentButtonPressed);
            } else if (isOperatorAction(action) || action == "(" || action == ")") {
                button->setColor(OperatorButton);
                button->setHoverColor(OperatorButtonHover);
                button->setPressedColor(OperatorButtonPressed);
            } else if (action == "clear" || action == "backspace") {
                button->setColor(UtilityButton);
                button->setHoverColor(UtilityButtonHover);
                button->setPressedColor(UtilityButtonPressed);
            } else {
                button->setColor(DigitButton);
                button->setHoverColor(DigitButtonHover);
                button->setPressedColor(DigitButtonPressed);
            }

            children.push_back(button);
            cells.push_back({.column = col, .row = row + 1, .columnSpan = 1, .rowSpan = 1});
        }
    }

    grid->setChildren(children);
    grid->setCells(std::move(cells));
    setChild(*grid);
}

void CalculatorWidget::syncDisplay()
{
    if (_display != nullptr) {
        _display->setValue(displayText());
        _display->setFontFilepath(_showingResult ? BoldFont : RegularFont);
    }
}

void CalculatorWidget::activate(const std::string& action)
{
    if (action == "clear") {
        clear();
    } else if (action == "backspace") {
        backspace();
    } else if (action == "equals") {
        evaluate();
    } else {
        appendToken(action);
    }
}

void CalculatorWidget::appendToken(const std::string& token)
{
    if (_showingResult && (isNumberAction(token) || token == "(")) {
        _expression.clear();
        _status = "0";
    }
    _showingResult = false;

    if (isOperatorAction(token)) {
        if (_expression.empty()) {
            if (token != "-") {
                return;
            }
        } else if (endsWithOperator(_expression)) {
            _expression.back() = token[0];
            _status = _expression;
            syncDisplay();
            return;
        }
    }

    if (token == "." && !_expression.empty()) {
        size_t pos = _expression.size();
        while (pos > 0 && (std::isdigit(static_cast<unsigned char>(_expression[pos - 1])) || _expression[pos - 1] == '.')) {
            if (_expression[pos - 1] == '.') {
                return;
            }
            --pos;
        }
    }

    _expression += token;
    _status = _expression;
    syncDisplay();
}

void CalculatorWidget::evaluate()
{
    if (_expression.empty() || endsWithOperator(_expression)) {
        return;
    }

    int error = 0;
    const double result = te_interp(_expression.c_str(), &error);
    if (error != 0 || !std::isfinite(result)) {
        _status = "Error";
        _showingResult = true;
        syncDisplay();
        return;
    }

    _status = formatResult(result);
    _expression = _status;
    _showingResult = true;
    syncDisplay();
}

void CalculatorWidget::backspace()
{
    if (_showingResult) {
        clear();
        return;
    }
    if (!_expression.empty()) {
        _expression.pop_back();
    }
    _status = _expression.empty() ? "0" : _expression;
    syncDisplay();
}

void CalculatorWidget::clear()
{
    _expression.clear();
    _status = "0";
    _showingResult = false;
    syncDisplay();
}

void CalculatorWidget::handleKeyDown(SDL_Keycode key, SDL_Keymod mod)
{
    const bool shiftPressed = (mod & SDL_KMOD_SHIFT) != 0;

    if (shiftPressed) {
        switch (key) {
        case SDLK_8:
            appendToken("*");
            return;
        case SDLK_EQUALS:
            appendToken("+");
            return;
        case SDLK_9:
            appendToken("(");
            return;
        case SDLK_0:
            appendToken(")");
            return;
        default:
            break;
        }
    }

    if (key >= SDLK_0 && key <= SDLK_9) {
        appendToken(std::string(1, static_cast<char>('0' + key - SDLK_0)));
        return;
    }
    if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
        appendToken(std::string(1, static_cast<char>('0' + key - SDLK_KP_0)));
        return;
    }

    switch (key) {
    case SDLK_PLUS:
    case SDLK_KP_PLUS:
        appendToken("+");
        break;
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        appendToken("-");
        break;
    case SDLK_ASTERISK:
    case SDLK_KP_MULTIPLY:
        appendToken("*");
        break;
    case SDLK_SLASH:
    case SDLK_KP_DIVIDE:
        appendToken("/");
        break;
    case SDLK_PERIOD:
    case SDLK_KP_PERIOD:
        appendToken(".");
        break;
    case SDLK_LEFTPAREN:
        appendToken("(");
        break;
    case SDLK_RIGHTPAREN:
        appendToken(")");
        break;
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
    case SDLK_EQUALS:
        evaluate();
        break;
    case SDLK_BACKSPACE:
        backspace();
        break;
    case SDLK_ESCAPE:
    case SDLK_C:
        clear();
        break;
    default:
        break;
    }
}

std::string CalculatorWidget::displayText() const
{
    return _status.empty() ? "0" : _status;
}

std::string CalculatorWidget::formatResult(double value)
{
    if (std::abs(value) < 1e-12) {
        value = 0.0;
    }

    std::ostringstream stream;
    stream.precision(12);
    stream << std::fixed << value;
    std::string result = stream.str();

    while (result.size() > 1 && result.back() == '0') {
        result.pop_back();
    }
    if (!result.empty() && result.back() == '.') {
        result.pop_back();
    }
    return result;
}
