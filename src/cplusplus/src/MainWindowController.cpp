#include "MainWindowController.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <vector>


MainWindowController::MainWindowController() {
    app = Gtk::Application::create("org.gtkmm.calculator");
    builder = Gtk::Builder::create_from_resource("/com/angel/calculator/calculator.glade");
    builder->get_widget("frmMain", window);
    builder->get_widget("entry_display", entry_display);
    connect_buttons();
}

void MainWindowController::run() {
    window->show_all();
    app->run(*window);
}

void MainWindowController::connect_buttons() {
    const std::vector<std::string> digits = {
        "btn_0", "btn_1", "btn_2", "btn_3", "btn_4",
        "btn_5", "btn_6", "btn_7", "btn_8", "btn_9"
    };

    for (const auto& id : digits) {
        Gtk::Button* btn;
        builder->get_widget(id, btn);
        if (btn) {
            btn->signal_clicked().connect([this, btn]() {
                on_digit_pressed(btn->get_label());
            });
        }
    }

    const std::vector<std::pair<std::string, std::string>> operators = {
        {"btn_add", "+"}, {"btn_subtract", "-"},
        {"btn_multiply", "*"}, {"btn_divide", "/"}
    };

    for (const auto& [id, symbol] : operators) {
        Gtk::Button* btn;
        builder->get_widget(id, btn);
        if (btn) {
            btn->signal_clicked().connect([this, symbol]() {
                on_operator_pressed(symbol);
            });
        }
    }

    Gtk::Button* btn_clear;
    builder->get_widget("btn_clear", btn_clear);
    if (btn_clear) {
        btn_clear->signal_clicked().connect(sigc::mem_fun(*this, &MainWindowController::on_clear_pressed));
    }

    Gtk::Button* btn_equals;
    builder->get_widget("btn_equals", btn_equals);
    if (btn_equals) {
        btn_equals->signal_clicked().connect(sigc::mem_fun(*this, &MainWindowController::on_equals_pressed));
    }
}

void MainWindowController::on_digit_pressed(const std::string& digit) {
    expression += digit;
    entry_display->set_text(expression);
}

void MainWindowController::on_operator_pressed(const std::string& op) {
    if (!expression.empty() && std::string("+-*/").find(expression.back()) == std::string::npos) {
        expression += op;
        entry_display->set_text(expression);
    }
}

void MainWindowController::on_clear_pressed() {
    expression.clear();
    entry_display->set_text("");
}

void MainWindowController::on_equals_pressed() {
    try {
        double result = evaluate_expression(expression);
        entry_display->set_text(std::to_string(result));
        expression = std::to_string(result);
    } catch (const std::exception& e) {
        entry_display->set_text("Error");
        expression.clear();
    }
}

// Simple expression evaluator (supports +, -, *, /)
double MainWindowController::evaluate_expression(const std::string& expr) {
    double num;
    char op = '+';
    double result = 0.0;
    std::istringstream iss(expr);

    while (iss >> num) {
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': result /= num; break;
        }
        iss >> op;
    }

    return result;
}