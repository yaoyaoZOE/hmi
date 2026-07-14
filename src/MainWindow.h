#ifndef HMI_MAIN_WINDOW_H
#define HMI_MAIN_WINDOW_H

#include <gtkmm.h>

#include "ControlPanel.h"
#include "GaugeWidget.h"
#include "LogView.h"
#include "StatusWidget.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    ~MainWindow() override = default;

private:
    // --- helpers ---
    void build_ui();
    void connect_signals();
    void start_machine();
    void stop_machine();
    void reset_machine();
    void emergency_stop();
    bool on_simulation_tick();

    // --- state ---
    enum class MachineState { IDLE, RUNNING, STOPPED, EMERGENCY };

    MachineState  m_state{MachineState::IDLE};
    double        m_process_value{0.0};  // 0..100 simulated
    bool          m_direction{true};     // true = rising
    unsigned int  m_tick_count{0};  // 100 ms ticks

    // --- timer ---
    sigc::connection m_timer_conn;

    // --- UI widgets ---
    Gtk::Box        m_main_box{Gtk::Orientation::HORIZONTAL, 8};
    Gtk::Box        m_sidebar{Gtk::Orientation::VERTICAL, 8};
    Gtk::Box        m_content_area{Gtk::Orientation::VERTICAL, 8};

    Gtk::HeaderBar  m_header;

    // sidebar widgets (owned via Gtk::make_managed or direct embedding)
    StatusWidget    m_status;
    ControlPanel    m_controls;

    // content area
    Gtk::Label      m_pv_label;
    Gtk::Label      m_pv_unit_label;
    Gtk::Label      m_state_label;
    GaugeWidget     m_gauge;
    LogView         m_log;

    // process value box
    Gtk::Box        m_pv_box{Gtk::Orientation::HORIZONTAL, 4};
    Gtk::Box        m_state_box{Gtk::Orientation::HORIZONTAL, 4};
};

#endif // HMI_MAIN_WINDOW_H
