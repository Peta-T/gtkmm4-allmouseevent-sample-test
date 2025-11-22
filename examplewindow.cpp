#include "examplewindow.h"
#include <map>
#include <iostream>
#include <cmath>
#include <string>
#include <gdk/gdk.h> // Necessary for C API functions (gdk_event_get_event_type, etc.)

namespace
{
// Map of names for console output
std::map<unsigned int, Glib::ustring> mouse_buttons_names
{
    {GDK_BUTTON_PRIMARY, "Left (Primary)"},
    {GDK_BUTTON_MIDDLE, "Middle (Middle)"},
    {GDK_BUTTON_SECONDARY, "Right (Secondary)"},
    {8, "Button 8 (Extra)"},
    {9, "Button 9 (Extra)"}
};

// Helper function to get a readable button name
std::string get_btn_name(guint id) {
    if (mouse_buttons_names.count(id)) return mouse_buttons_names[id];
    return "ID " + std::to_string(id);
}

// Helper function to get the system button mask
Gdk::ModifierType get_mask_for_btn(guint btn_id) {
    switch (btn_id) {
        case GDK_BUTTON_PRIMARY: return Gdk::ModifierType::BUTTON1_MASK;
        case GDK_BUTTON_MIDDLE: return Gdk::ModifierType::BUTTON2_MASK;
        case GDK_BUTTON_SECONDARY: return Gdk::ModifierType::BUTTON3_MASK;
        // Buttons 8, 9 have no standard mask, return 0
        default: return static_cast<Gdk::ModifierType>(0);
    }
}
}

ExampleWindow::ExampleWindow()
{
    set_title("Full Fix: MB8/MB9 Drag & Release");
    set_default_size(800, 600);
    m_box.set_margin(10);
    m_box.set_orientation(Gtk::Orientation::VERTICAL);
    set_child(m_box);

    m_info_label.set_text("Support: MB1, MB2, MB3, MB8, MB9 + Combos");
    m_box.append(m_info_label);

    // --- Key Controller ---
    auto key_controller = Gtk::EventControllerKey::create();
    key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &ExampleWindow::on_key_pressed), false);
    add_controller(key_controller);

    // --- Motion Controller ---
    m_controller_motion = Gtk::EventControllerMotion::create();
    m_controller_motion->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);

    m_controller_motion->signal_motion().connect(sigc::mem_fun(*this, &ExampleWindow::on_mouse_motion));
    m_controller_motion->signal_enter().connect(sigc::mem_fun(*this, &ExampleWindow::on_mouse_enter));
    m_controller_motion->signal_leave().connect(sigc::mem_fun(*this, &ExampleWindow::on_mouse_leave));
    add_controller(m_controller_motion);

    // --- Scroll Controller ---
    auto scroll_controller = Gtk::EventControllerScroll::create();
    scroll_controller->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
    scroll_controller->signal_scroll().connect(sigc::mem_fun(*this, &ExampleWindow::on_mouse_scroll), true);
    add_controller(scroll_controller);

    // --- Gestures (Click Controllers) ---
    auto setup_gesture = [&](Glib::RefPtr<Gtk::GestureClick>& gesture, guint btn_id) {
        gesture = Gtk::GestureClick::create();
        gesture->set_button(btn_id);
        gesture->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);

        gesture->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &ExampleWindow::on_mb_pressed), gesture.get(), btn_id));
        gesture->signal_released().connect(sigc::bind(sigc::mem_fun(*this, &ExampleWindow::on_mb_released), btn_id));
        gesture->signal_stopped().connect(sigc::bind(sigc::mem_fun(*this, &ExampleWindow::on_mb_stopped), btn_id));

        add_controller(gesture);
        m_drag_candidates[btn_id] = DragCandidate{};
    };

    setup_gesture(m_gesture_left, GDK_BUTTON_PRIMARY);
    setup_gesture(m_gesture_middle, GDK_BUTTON_MIDDLE);
    setup_gesture(m_gesture_right, GDK_BUTTON_SECONDARY);
    setup_gesture(m_gesture_btn9, 9);
    setup_gesture(m_gesture_btn8, 8);

    // --- LEGACY CONTROLLER (Fix for sticky MB8/MB9) ---
    m_controller_legacy = Gtk::EventControllerLegacy::create();
    m_controller_legacy->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);

    // Added second parameter 'false' for correct compilation
    m_controller_legacy->signal_event().connect(
        sigc::mem_fun(*this, &ExampleWindow::on_event), false);

    add_controller(m_controller_legacy);
}

// ================================================================================
// 1. ROUTER: BUTTON PRESS
//    Now calls the correct method based on n_press (click count).
// ================================================================================
void ExampleWindow::on_mb_pressed(int n_press, double x, double y, Gtk::GestureClick* gesture, guint button_id)
{
    // For clicks > 1, the drag/release logic is ignored, GTKmm handles the "click" event.
    // However, for safety, we keep the logic for detecting the first press.
    if (n_press == 1)
    {
        // Use .at() because buttons are initialized in the constructor
        if (m_drag_candidates.at(button_id).is_pressed) return;

        m_drag_candidates.at(button_id).is_pressed = true;
        m_drag_candidates.at(button_id).valid_for_drag = true;
        m_drag_candidates.at(button_id).start_x = x;
        m_drag_candidates.at(button_id).start_y = y;
    }


    // Call specific method for press/click
    switch (button_id) {
        case GDK_BUTTON_PRIMARY:
            if (n_press == 1) on_MB1_cli();
            else if (n_press == 2) on_MB1_double_cli();
            else if (n_press == 3) on_MB1_triple_cli();
            else if (n_press == 4) on_MB1_quad_cli();
            break;

        case GDK_BUTTON_MIDDLE:
            if (n_press == 1) on_MB2_cli();
            else if (n_press == 2) on_MB2_double_cli();
            break;

        case GDK_BUTTON_SECONDARY:
            if (n_press == 1) on_MB3_cli();
            else if (n_press == 2) on_MB3_double_cli();
            break;

        case 9:
            if (n_press == 1) on_MB9_cli();
            else if (n_press == 2) on_MB9_double_cli();
            break;

        case 8:
            if (n_press == 1) on_MB8_cli();
            else if (n_press == 2) on_MB8_double_cli();
            break;
    }
}

// ================================================================================
// 2. ROUTER: MOUSE MOTION (Safety Check + Logic)
// ================================================================================
void ExampleWindow::on_mouse_motion(double x, double y)
{
    // A) Safety Check (only for buttons with mask)
    Gdk::ModifierType state = m_controller_motion->get_current_event_state();
    for (auto& [btn_id, candidate] : m_drag_candidates) {
        if (candidate.is_pressed) {
            Gdk::ModifierType mask = get_mask_for_btn(btn_id);
            if ((int)mask != 0 && (state & mask) != mask) {
                candidate.is_pressed = false;
                if (m_active_drag_id != 0) force_drag_end("Safety Check Fail");
                return;
            }
        }
    }

    // B) Drag Logic
    if (m_active_drag_id != 0)
    {
        // --- UPDATE ---
        auto& c = m_drag_candidates.at(m_active_drag_id);
        double dx = x - c.start_x;
        double dy = y - c.start_y;

        if (m_active_drag_id == GDK_BUTTON_PRIMARY)         on_MB1_drag_update(dx, dy);
        else if (m_active_drag_id == GDK_BUTTON_SECONDARY) on_MB3_drag_update(dx, dy);
        else if (m_active_drag_id == 9)                    on_MB9_drag_update(dx, dy);
        else if (m_active_drag_id == 8)                    on_MB8_drag_update(dx, dy);
        else if (m_active_drag_id == GDK_BUTTON_MIDDLE) {
             bool left_down = m_drag_candidates.at(GDK_BUTTON_PRIMARY).is_pressed;
             bool right_down = m_drag_candidates.at(GDK_BUTTON_SECONDARY).is_pressed;
             if (right_down)      on_MB23_drag_update(dx, dy);
             else if (left_down)  on_MB21_drag_update(dx, dy);
             else                 on_MB2_drag_update(dx, dy);
        }
    }
    else
    {
        // --- START ---
        for (auto& [btn_id, candidate] : m_drag_candidates) {
            if (candidate.is_pressed && candidate.valid_for_drag) {
                double dist = std::sqrt(std::pow(x - candidate.start_x, 2) + std::pow(y - candidate.start_y, 2));

                if (dist > DRAG_THRESHOLD) {
                    m_active_drag_id = btn_id;

                    if (btn_id == GDK_BUTTON_PRIMARY)          on_MB1_drag_start();
                    else if (btn_id == GDK_BUTTON_SECONDARY) on_MB3_drag_start();
                    else if (btn_id == 9)                      on_MB9_drag_start();
                    else if (btn_id == 8)                      on_MB8_drag_start();
                    else if (btn_id == GDK_BUTTON_MIDDLE) {
                         bool left_down = m_drag_candidates.at(GDK_BUTTON_PRIMARY).is_pressed;
                         bool right_down = m_drag_candidates.at(GDK_BUTTON_SECONDARY).is_pressed;
                         if (right_down)      on_MB23_drag_start();
                         else if (left_down)  on_MB21_drag_start();
                         else                 on_MB2_drag_start();
                    }
                    break;
                }
            }
        }
    }
}

// ================================================================================
// 3. ROUTER: BUTTON RELEASE
// ================================================================================
void ExampleWindow::on_mb_released(int n_press, double x, double y, guint button_id)
{
    // If n_press > 1, it is a multi-click and the release has already been handled by GTKmm.
    // Drag/Release logic only runs on the first press (n_press == 1).
    if (n_press > 1) return;

    bool was_dragging_owner = (m_active_drag_id == button_id);

    switch (button_id) {
        case GDK_BUTTON_PRIMARY:   on_MB1_rel(); break;
        case GDK_BUTTON_MIDDLE:    on_MB2_rel(); break;
        case GDK_BUTTON_SECONDARY: on_MB3_rel(); break;
        case 9:                    on_MB9_rel(); break;
        case 8:                    on_MB8_rel(); break;
    }

    if (was_dragging_owner) {
        if (button_id == GDK_BUTTON_PRIMARY)         on_MB1_drag_end();
        else if (button_id == GDK_BUTTON_SECONDARY) on_MB3_drag_end();
        else if (button_id == 9)                     on_MB9_drag_end();
        else if (button_id == 8)                     on_MB8_drag_end();
        else if (button_id == GDK_BUTTON_MIDDLE)     on_MB2_drag_end();

        m_active_drag_id = 0;
    }
    else if (m_active_drag_id != 0) {
        force_drag_end("Secondary combo button released");
    }

    // Use .at() because buttons are initialized in the constructor
    m_drag_candidates.at(button_id).is_pressed = false;
    m_drag_candidates.at(button_id).valid_for_drag = true;
}

// ================================================================================
// 4. ROUTER: STOPPED (Safety net against GTK behavior)
// ================================================================================
void ExampleWindow::on_mb_stopped(guint button_id)
{
    // For MB8/MB9: If we are dragging, ignore stop (because it only means "moved too far for click").
    // Use .at() because buttons are initialized in the constructor
    if ((button_id == 9 || button_id == 8) && m_drag_candidates.at(button_id).is_pressed)
    {
        if (m_active_drag_id == button_id) return;

        // If we are not dragging, it might be a lost release, BUT with the Legacy controller
        // we handle that better, so we prefer not to do anything destructive here.
        return;
    }
}

void ExampleWindow::force_drag_end(std::string reason)
{
    std::cout << "[SYSTEM] FORCE END: " << reason << std::endl;
    if (m_active_drag_id != 0) {
        if (m_active_drag_id == 1) on_MB1_drag_end();
        else if (m_active_drag_id == 2) on_MB2_drag_end();
        else if (m_active_drag_id == 3) on_MB3_drag_end();
        else if (m_active_drag_id == 9) on_MB9_drag_end();
        else if (m_active_drag_id == 8) on_MB8_drag_end();
        m_active_drag_id = 0;
    }
    for (auto& [id, c] : m_drag_candidates) {
        if (c.is_pressed) c.valid_for_drag = false;
    }
}

// ================================================================================
// 5. NEW: RAW EVENT HANDLER (Fix for sticky buttons)
// ================================================================================
bool ExampleWindow::on_event(const Glib::RefPtr<const Gdk::Event>& event)
{
    // Get the raw pointer for the C API. event->gobj() returns const GdkEvent*,
    // const_cast is necessary because GDK C API (even for getters) expects GdkEvent*.
    GdkEvent* c_event = const_cast<GdkEvent*>(event->gobj());

    // Check the event type via the C API. Use gdk_event_get_event_type() and the GDK macro.
    if (gdk_event_get_event_type(c_event) == GDK_BUTTON_RELEASE)
    {
        guint btn_id = gdk_button_event_get_button(c_event);

        // Check for existence in the map and access member variables/functions.
        if ((btn_id == 8 || btn_id == 9) && this->m_drag_candidates.count(btn_id) && this->m_drag_candidates.at(btn_id).is_pressed)
        {
            std::cout << "[SYSTEM] Raw Release Event caught for MB" << btn_id << std::endl;
            // Manually trigger release logic
            this->on_mb_released(1, 0, 0, btn_id);
            return false;
        }
    }
    return false;
}

// --- OTHER HANDLERS ---

bool ExampleWindow::on_mouse_scroll(double dx, double dy) {
    std::cout << "Scroll: " << dx << ", " << dy << std::endl;
    return true;
}

bool ExampleWindow::on_key_pressed(guint keyval, guint, Gdk::ModifierType) {
    if (keyval == GDK_KEY_Escape) { set_visible(false); return true; }
    return false;
}

void ExampleWindow::on_mouse_enter(double x, double y) {
    std::cout << "[SYSTEM] Mouse ENTERED widget at (" << x << ", " << y << ")" << std::endl;
}

void ExampleWindow::on_mouse_leave() {
    std::cout << "[SYSTEM] Mouse LEFT widget" << std::endl;
    if (m_active_drag_id != 0) force_drag_end("Mouse left window");
}

// ================================================================================
// IMPLEMENTATION OF SPECIFIC METHODS (APP LOGIC)
// ================================================================================

// --- Single Click ---
void ExampleWindow::on_MB1_cli() { std::cout << "MB1: Click (Press)" << std::endl; }
void ExampleWindow::on_MB2_cli() { std::cout << "MB2: Click (Press)" << std::endl; }
void ExampleWindow::on_MB3_cli() { std::cout << "MB3: Click (Press)" << std::endl; }
void ExampleWindow::on_MB9_cli() { std::cout << "MB9: Click (Press)" << std::endl; }
void ExampleWindow::on_MB8_cli() { std::cout << "MB8: Click (Press)" << std::endl; }

// --- Multi Click ---
void ExampleWindow::on_MB1_double_cli() { std::cout << "MB1: DOUBLE Click!" << std::endl; }
void ExampleWindow::on_MB1_triple_cli() { std::cout << "MB1: TRIPLE Click!" << std::endl; }
void ExampleWindow::on_MB1_quad_cli() { std::cout << "MB1: QUAD Click! (4x)" << std::endl; }
void ExampleWindow::on_MB2_double_cli() { std::cout << "MB2: DOUBLE Click!" << std::endl; }
void ExampleWindow::on_MB3_double_cli() { std::cout << "MB3: DOUBLE Click!" << std::endl; }
void ExampleWindow::on_MB9_double_cli() { std::cout << "MB9: DOUBLE Click!" << std::endl; }
void ExampleWindow::on_MB8_double_cli() { std::cout << "MB8: DOUBLE Click!" << std::endl; }

// --- Release ---
void ExampleWindow::on_MB1_rel() { std::cout << "MB1: Release" << std::endl; }
void ExampleWindow::on_MB2_rel() { std::cout << "MB2: Release" << std::endl; }
void ExampleWindow::on_MB3_rel() { std::cout << "MB3: Release" << std::endl; }
void ExampleWindow::on_MB9_rel() { std::cout << "MB9: Release" << std::endl; }
void ExampleWindow::on_MB8_rel() { std::cout << "MB8: Release" << std::endl; }

// --- Drag Handlers ---
void ExampleWindow::on_MB1_drag_start() { std::cout << "MB1: Drag START" << std::endl; }
void ExampleWindow::on_MB1_drag_update(double dx, double dy) { std::cout << "MB1: Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB1_drag_end() { std::cout << "MB1: Drag END" << std::endl; }

void ExampleWindow::on_MB2_drag_start() { std::cout << "MB2: Drag START" << std::endl; }
void ExampleWindow::on_MB2_drag_update(double dx, double dy) { std::cout << "MB2: Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB2_drag_end() { std::cout << "MB2: Drag END" << std::endl; }

void ExampleWindow::on_MB3_drag_start() { std::cout << "MB3: Drag START" << std::endl; }
void ExampleWindow::on_MB3_drag_update(double dx, double dy) { std::cout << "MB3: Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB3_drag_end() { std::cout << "MB3: Drag END" << std::endl; }

void ExampleWindow::on_MB9_drag_start() { std::cout << "MB9: Drag START" << std::endl; }
void ExampleWindow::on_MB9_drag_update(double dx, double dy) { std::cout << "MB9: Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB9_drag_end() { std::cout << "MB9: Drag END" << std::endl; }

void ExampleWindow::on_MB8_drag_start() { std::cout << "MB8: Drag START" << std::endl; }
void ExampleWindow::on_MB8_drag_update(double dx, double dy) { std::cout << "MB8: Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB8_drag_end() { std::cout << "MB8: Drag END" << std::endl; }

void ExampleWindow::on_MB21_drag_start() { std::cout << "MB2+1: Combo Drag START" << std::endl; }
void ExampleWindow::on_MB21_drag_update(double dx, double dy) { std::cout << "MB2+1: Combo Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB21_drag_end() { std::cout << "MB2+1: Combo Drag END" << std::endl; }

void ExampleWindow::on_MB23_drag_start() { std::cout << "MB2+3: Combo Drag START" << std::endl; }
void ExampleWindow::on_MB23_drag_update(double dx, double dy) { std::cout << "MB2+3: Combo Drag UPDATE " << dx << "," << dy << std::endl; }
void ExampleWindow::on_MB23_drag_end() { std::cout << "MB2+3: Combo Drag END" << std::endl; }

ExampleWindow::~ExampleWindow() {}