//build: g++ -std=c++17 -g main.cpp examplewindow.cpp -o app $(pkg-config --cflags --libs gtkmm-4.0)
#include "examplewindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{

    auto app = Gtk::Application::create("org.gtkmm.example.mouseevents");


    return app->make_window_and_run<ExampleWindow>(argc, argv);
}
