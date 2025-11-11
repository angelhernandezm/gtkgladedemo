#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <string>
#include <glibmm.h>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>

class MainWindowController {
public:
    MainWindowController();
    void run();

private:
    // GTK components
    Glib::RefPtr<Gtk::Application> app;
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::ApplicationWindow* window;
    Gtk::Entry* entry_display;

    // Button handlers
    void on_digit_pressed(const std::string& digit);
    void on_operator_pressed(const std::string& op);
    void on_clear_pressed();
    void on_equals_pressed();

    // Internal state
    std::string expression;

    // Helper methods
    void connect_buttons();
    double evaluate_expression(const std::string& expr);
};

#endif // MAINWINDOWCONTROLLER_H
