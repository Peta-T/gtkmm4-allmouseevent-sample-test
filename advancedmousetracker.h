#pragma once

#include <gtkmm.h>
#include <map>
#include <string>

/**
 * @brief Knihovní třída pro pokročilou správu myši.
 * Použití: Vytvořte třídu, která dědí z AdvancedMouseTracker a přepište (override)
 * metody on_MBx_..., které chcete používat.
 */
class AdvancedMouseTracker : public sigc::trackable
{
public:
    /**
     * @param target_widget Widget, na který se mají navázat události myši (např. "this" z okna, nebo DrawingArea).
     */
    explicit AdvancedMouseTracker(Gtk::Widget* target_widget);
    virtual ~AdvancedMouseTracker();

protected:
    // ========================================================================
    // VIRTUÁLNÍ ROZHRANÍ (API) - Přepište tyto metody ve svém kódu
    // ========================================================================

    // Single Click
    virtual void on_MB1_cli() {}
    virtual void on_MB2_cli() {}
    virtual void on_MB3_cli() {}
    virtual void on_MB9_cli() {}
    virtual void on_MB8_cli() {}

    // Multi Click
    virtual void on_MB1_double_cli() {}
    virtual void on_MB1_triple_cli() {}
    virtual void on_MB1_quad_cli() {}
    virtual void on_MB2_double_cli() {}
    virtual void on_MB3_double_cli() {}
    virtual void on_MB9_double_cli() {}
    virtual void on_MB8_double_cli() {}

    // Release (uvolnění tlačítka bez dragu)
    virtual void on_MB1_rel() {}
    virtual void on_MB2_rel() {}
    virtual void on_MB3_rel() {}
    virtual void on_MB9_rel() {}
    virtual void on_MB8_rel() {}

    // Drag Start/Update/End (MB1)
    virtual void on_MB1_drag_start() {}
    virtual void on_MB1_drag_update(double dx, double dy) {}
    virtual void on_MB1_drag_end() {}

    // Drag Start/Update/End (MB2)
    virtual void on_MB2_drag_start() {}
    virtual void on_MB2_drag_update(double dx, double dy) {}
    virtual void on_MB2_drag_end() {}

    // Drag Start/Update/End (MB3)
    virtual void on_MB3_drag_start() {}
    virtual void on_MB3_drag_update(double dx, double dy) {}
    virtual void on_MB3_drag_end() {}

    // Drag Start/Update/End (MB9)
    virtual void on_MB9_drag_start() {}
    virtual void on_MB9_drag_update(double dx, double dy) {}
    virtual void on_MB9_drag_end() {}

    // Drag Start/Update/End (MB8)
    virtual void on_MB8_drag_start() {}
    virtual void on_MB8_drag_update(double dx, double dy) {}
    virtual void on_MB8_drag_end() {}

    // Combo Drag (MB2 + MB1)
    virtual void on_MB21_drag_start() {}
    virtual void on_MB21_drag_update(double dx, double dy) {}
    virtual void on_MB21_drag_end() {}

    // Combo Drag (MB2 + MB3)
    virtual void on_MB23_drag_start() {}
    virtual void on_MB23_drag_update(double dx, double dy) {}
    virtual void on_MB23_drag_end() {}

    // Scroll (volitelné)
    virtual void on_scroll(double dx, double dy) {}

private:
    // --- Interní logika a stavy (uživatele nezajímá) ---
    struct DragCandidate {
        bool is_pressed = false;
        bool valid_for_drag = true;
        double start_x = 0.0;
        double start_y = 0.0;
    };

    Gtk::Widget* m_target;
    const double DRAG_THRESHOLD = 5.0;

    // GTK Controllers
    Glib::RefPtr<Gtk::EventControllerMotion> m_controller_motion;
    Glib::RefPtr<Gtk::EventControllerLegacy> m_controller_legacy;
    Glib::RefPtr<Gtk::EventControllerScroll> m_controller_scroll;
    std::vector<Glib::RefPtr<Gtk::GestureClick>> m_gestures;

    // Stavy
    std::map<guint, DragCandidate> m_drag_candidates;
    guint m_active_drag_id = 0; // 0 = žádný drag

    // Interní handlery (zde se děje magie)
    void setup_gesture(guint btn_id);
    void on_mouse_motion_internal(double x, double y);
    void on_mouse_enter_internal(double x, double y);
    void on_mouse_leave_internal();
    bool on_scroll_internal(double dx, double dy);

    void on_mb_pressed_internal(int n_press, double x, double y, guint button_id);
    void on_mb_released_internal(int n_press, double x, double y, guint button_id);
    void on_mb_stopped_internal(guint button_id);

    // Legacy fix
    bool on_event_legacy(const Glib::RefPtr<const Gdk::Event>& event);

    void force_drag_end(std::string reason);
};