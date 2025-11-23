#include "examplewindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    // Vytvoříme aplikaci
    auto app = Gtk::Application::create("org.gtkmm.example.mouseevents");

    // Zobrazíme okno a spustíme smyčku událostí
    return app->make_window_and_run<ExampleWindow>(argc, argv);
}
