#include "examplewindow.h"

ExampleWindow::ExampleWindow()
{
    set_title("Inheritance Based Mouse Tracker");
    set_default_size(800, 600);

    m_box.set_orientation(Gtk::Orientation::VERTICAL);
    set_child(m_box);
    m_info_label.set_text("Ready...");
    m_box.append(m_info_label);

    // Inicializace logiky
    // Předáme 'this' jako widget, který se má sledovat
    m_mouse_logic = std::make_unique<MyMouseLogic>(this, this);
}
