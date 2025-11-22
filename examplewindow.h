#pragma once

#include <gtkmm.h>
#include <map>

// Constant to determine when a click should become a drag (motion)
const double DRAG_THRESHOLD = 5.0;

/**
 * @brief Structure for tracking the pressed state and potential drag of a button.
 */
struct DragCandidate
{
    bool is_pressed = false;
    bool valid_for_drag = true;
    double start_x = 0.0;
    double start_y = 0.0;
};

/**
 * @brief Main application window that tracks mouse events.
 */
class ExampleWindow : public Gtk::Window
{
public:
    ExampleWindow();
    ~ExampleWindow() override;

protected:
    // UI Elements
    Gtk::Box m_box;
    Gtk::Label m_info_label;

    // Controllers
    Glib::RefPtr<Gtk::EventControllerMotion> m_controller_motion;
    Glib::RefPtr<Gtk::EventControllerLegacy> m_controller_legacy;

    // Gestures
    Glib::RefPtr<Gtk::GestureClick> m_gesture_left;
    Glib::RefPtr<Gtk::GestureClick> m_gesture_middle;
    Glib::RefPtr<Gtk::GestureClick> m_gesture_right;
    Glib::RefPtr<Gtk::GestureClick> m_gesture_btn9;
    Glib::RefPtr<Gtk::GestureClick> m_gesture_btn8;

    // Tracking button state (for MB1, 2, 3, 8, 9)
    std::map<guint, DragCandidate> m_drag_candidates;
    guint m_active_drag_id = 0; // 0 = no active drag

    // --- Signal Handlers (Implemented in examplewindow.cpp) ---

    // Motion and Leave
    void on_mouse_motion(double x, double y);
    void on_mouse_enter(double x, double y);
    void on_mouse_leave();
    bool on_mouse_scroll(double dx, double dy);

    // Key
    bool on_key_pressed(guint keyval, guint, Gdk::ModifierType);

    // Click/Release
    void on_mb_pressed(int n_press, double x, double y, Gtk::GestureClick* gesture, guint button_id);
    // Declaration of on_mb_released for manual calling from on_event
    void on_mb_released(int n_press, double x, double y, guint button_id);
    void on_mb_stopped(guint button_id);

    // Legacy Event Handler (Fix for sticky MB8/MB9 release)
    bool on_event(const Glib::RefPtr<const Gdk::Event>& event);

    // Force Drag End
    void force_drag_end(std::string reason);

    // --- App Logic Placeholders ---
    // Press/Click (Single)
    void on_MB1_cli();
    void on_MB2_cli();
    void on_MB3_cli();
    void on_MB9_cli();
    void on_MB8_cli();

    // New methods for multi-click
    void on_MB1_double_cli();
    void on_MB1_triple_cli();
    void on_MB1_quad_cli();
    void on_MB2_double_cli();
    void on_MB3_double_cli();
    void on_MB9_double_cli();
    void on_MB8_double_cli();

    // Release
    void on_MB1_rel();
    void on_MB2_rel();
    void on_MB3_rel();
    void on_MB9_rel();
    void on_MB8_rel();

    // Drag Start/Update/End
    void on_MB1_drag_start();
    void on_MB1_drag_update(double dx, double dy);
    void on_MB1_drag_end();

    void on_MB2_drag_start();
    void on_MB2_drag_update(double dx, double dy);
    void on_MB2_drag_end();

    void on_MB3_drag_start();
    void on_MB3_drag_update(double dx, double dy);
    void on_MB3_drag_end();

    void on_MB9_drag_start();
    void on_MB9_drag_update(double dx, double dy);
    void on_MB9_drag_end();

    void on_MB8_drag_start();
    void on_MB8_drag_update(double dx, double dy);
    void on_MB8_drag_end();

    void on_MB21_drag_start();
    void on_MB21_drag_update(double dx, double dy);
    void on_MB21_drag_end();

    void on_MB23_drag_start();
    void on_MB23_drag_update(double dx, double dy);
    void on_MB23_drag_end();
};