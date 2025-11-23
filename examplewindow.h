#pragma once
#include <gtkmm.h>
#include "advancedmousetracker.h"
#include <memory>
#include <iostream>
#include <string>

class ExampleWindow : public Gtk::Window
{
public:
    ExampleWindow();

protected:
    Gtk::Box m_box;
    Gtk::Label m_info_label;

    // Definujeme vlastní třídu pro ovládání, která dědí od naší knihovny
    class MyMouseLogic : public AdvancedMouseTracker {
    public:
        MyMouseLogic(Gtk::Widget* w, ExampleWindow* parent)
            : AdvancedMouseTracker(w), m_parent(parent) {}

    protected:
        ExampleWindow* m_parent;

void on_MB1_cli() override{ std::cout << "MB1: Click (Press)" << std::endl; }
void on_MB2_cli() override { std::cout << "MB2: Click (Press)" << std::endl; }
void on_MB3_cli() override { std::cout << "MB3: Click (Press)" << std::endl; }
void on_MB9_cli() override  { std::cout << "MB9: Click (Press)" << std::endl; }
void on_MB8_cli() override  { std::cout << "MB8: Click (Press)" << std::endl; }

// --- Multi Click ---
void on_MB1_double_cli() override  { std::cout << "MB1: DOUBLE Click!" << std::endl; }
void on_MB1_triple_cli() override  { std::cout << "MB1: TRIPLE Click!" << std::endl; }
void on_MB1_quad_cli() override  { std::cout << "MB1: QUAD Click! (4x)" << std::endl; }
void on_MB2_double_cli() override  { std::cout << "MB2: DOUBLE Click!" << std::endl; }
void on_MB3_double_cli() override  { std::cout << "MB3: DOUBLE Click!" << std::endl; }
void on_MB9_double_cli() override  { std::cout << "MB9: DOUBLE Click!" << std::endl; }
void on_MB8_double_cli() override  { std::cout << "MB8: DOUBLE Click!" << std::endl; }

// --- Release ---
void on_MB1_rel() override  { std::cout << "MB1: Release" << std::endl; }
void on_MB2_rel() override  { std::cout << "MB2: Release" << std::endl; }
void on_MB3_rel() override  { std::cout << "MB3: Release" << std::endl; }
void on_MB9_rel() override  { std::cout << "MB9: Release" << std::endl; }
void on_MB8_rel() override  { std::cout << "MB8: Release" << std::endl; }

// --- Drag Handlers ---
void on_MB1_drag_start() override  { std::cout << "MB1: Drag START" << std::endl; }
void on_MB1_drag_update(double dx, double dy)  override { std::cout << "MB1: Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB1_drag_end() override  { std::cout << "MB1: Drag END" << std::endl; }

void on_MB2_drag_start() override  { std::cout << "MB2: Drag START" << std::endl; }
void on_MB2_drag_update(double dx, double dy) override  { std::cout << "MB2: Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB2_drag_end() override  { std::cout << "MB2: Drag END" << std::endl; }

void on_MB3_drag_start() override  { std::cout << "MB3: Drag START" << std::endl; }
void on_MB3_drag_update(double dx, double dy) override  { std::cout << "MB3: Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB3_drag_end() override  { std::cout << "MB3: Drag END" << std::endl; }

void on_MB9_drag_start() override  { std::cout << "MB9: Drag START" << std::endl; }
void on_MB9_drag_update(double dx, double dy) override  { std::cout << "MB9: Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB9_drag_end() override  { std::cout << "MB9: Drag END" << std::endl; }

void on_MB8_drag_start() override  { std::cout << "MB8: Drag START" << std::endl; }
void on_MB8_drag_update(double dx, double dy) override  { std::cout << "MB8: Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB8_drag_end() override  { std::cout << "MB8: Drag END" << std::endl; }

void on_MB21_drag_start() override  { std::cout << "MB2+1: Combo Drag START" << std::endl; }
void on_MB21_drag_update(double dx, double dy) override  { std::cout << "MB2+1: Combo Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB21_drag_end() override  { std::cout << "MB2+1: Combo Drag END" << std::endl; }

void on_MB23_drag_start() override  { std::cout << "MB2+3: Combo Drag START" << std::endl; }
void on_MB23_drag_update(double dx, double dy) override  { std::cout << "MB2+3: Combo Drag UPDATE " << dx << "," << dy << std::endl; }
void on_MB23_drag_end() override  { std::cout << "MB2+3: Combo Drag END" << std::endl; }

    };

    std::unique_ptr<MyMouseLogic> m_mouse_logic;
};
