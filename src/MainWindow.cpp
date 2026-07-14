#include "MainWindow.h"

#include <sstream>

MainWindow::MainWindow()
{
    set_title("HMI — Machine Control Panel");
    set_default_size(900, 600);

    build_ui();
    connect_signals();

    m_log.append("HMI application started — system ready.");

    // start simulation timer (10 Hz)
    m_timer_conn = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &MainWindow::on_simulation_tick),
        100);
}

// ---------------------------------------------------------------------------
// UI layout
// ---------------------------------------------------------------------------
void MainWindow::build_ui()
{
    // --- header bar ---
    m_header.set_title_widget(*Gtk::make_managed<Gtk::Label>("HMI Machine Control"));
    m_header.set_show_title_buttons(true);
    set_titlebar(m_header);

    // --- sidebar (left: 220 px) ---
    m_sidebar.set_size_request(220, -1);
    m_sidebar.set_margin(8);
    m_sidebar.set_valign(Gtk::Align::FILL);

    // status widget inside a Frame-like container
    m_status.set_vexpand(false);
    m_sidebar.append(m_status);

    // controls
    m_controls.set_vexpand(false);
    m_sidebar.append(m_controls);

    // --- content area (right) ---
    m_content_area.set_margin(8);
    m_content_area.set_margin_start(0);

    // process value display
    {
        auto* title = Gtk::make_managed<Gtk::Label>("Process Value");
        title->set_css_classes({"section-title"});
        title->set_halign(Gtk::Align::START);
        m_content_area.append(*title);

        m_pv_label.set_text("0.0");
        m_pv_label.set_css_classes({"pv-label"});

        m_pv_unit_label.set_text("RPM");
        m_pv_unit_label.set_css_classes({"pv-unit"});
        m_pv_unit_label.set_valign(Gtk::Align::END);
        m_pv_unit_label.set_margin_bottom(2);

        m_pv_box.append(m_pv_label);
        m_pv_box.append(m_pv_unit_label);
        m_content_area.append(m_pv_box);
    }

    // machine state display
    {
        auto* title = Gtk::make_managed<Gtk::Label>("Machine State");
        title->set_css_classes({"section-title"});
        title->set_halign(Gtk::Align::START);
        m_content_area.append(*title);

        m_state_label.set_text("IDLE");
        m_state_label.set_css_classes({"pv-label"});

        m_state_box.append(m_state_label);
        m_content_area.append(m_state_box);
    }

    // gauge
    {
        m_gauge.set_label("RPM");
        m_gauge.set_vexpand(true);
        m_gauge.set_hexpand(true);

        auto* frame = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 0);
        frame->set_css_classes({"gauge-panel"});
        frame->append(m_gauge);
        m_content_area.append(*frame);
    }

    // log area
    {
        auto* title = Gtk::make_managed<Gtk::Label>("Event Log");
        title->set_css_classes({"section-title"});
        title->set_halign(Gtk::Align::START);
        m_content_area.append(*title);

        m_log.set_vexpand(true);
        m_content_area.append(m_log);
    }

    // --- assemble main layout ---
    m_main_box.append(m_sidebar);
    m_main_box.append(m_content_area);
    set_child(m_main_box);
}

// ---------------------------------------------------------------------------
// signal wiring
// ---------------------------------------------------------------------------
void MainWindow::connect_signals()
{
    m_controls.signal_start().connect(
        sigc::mem_fun(*this, &MainWindow::start_machine));

    m_controls.signal_stop().connect(
        sigc::mem_fun(*this, &MainWindow::stop_machine));

    m_controls.signal_reset().connect(
        sigc::mem_fun(*this, &MainWindow::reset_machine));

    m_controls.signal_emergency().connect(
        sigc::mem_fun(*this, &MainWindow::emergency_stop));
}

// ---------------------------------------------------------------------------
// machine logic
// ---------------------------------------------------------------------------
void MainWindow::start_machine()
{
    if (m_state == MachineState::RUNNING)
        return;

    m_state = MachineState::RUNNING;
    m_state_label.set_text("RUNNING");
    m_state_label.set_css_classes({"pv-label", "status-ok"});

    m_controls.set_buttons_enabled(false, true, false, true);

    std::ostringstream oss;
    oss << "Machine STARTED at t=" << m_tick_count << "s";
    m_log.append(oss.str());
}

void MainWindow::stop_machine()
{
    if (m_state != MachineState::RUNNING)
        return;

    m_state = MachineState::STOPPED;
    m_state_label.set_text("STOPPED");
    m_state_label.set_css_classes({"pv-label", "status-warn"});

    m_controls.set_buttons_enabled(false, false, true, true);

    std::ostringstream oss;
    oss << "Machine STOPPED at t=" << m_tick_count << "s, PV=" << m_process_value;
    m_log.append(oss.str());
}

void MainWindow::reset_machine()
{
    m_state          = MachineState::IDLE;
    m_process_value  = 0.0;
    m_direction      = true;
    m_tick_count    = 0;

    m_state_label.set_text("IDLE");
    m_state_label.set_css_classes({"pv-label", "pv-label"});

    m_controls.set_buttons_enabled(true, false, true, true);

    m_pv_label.set_text("0.0");
    m_gauge.set_value(0.0);

    // restore all statuses to OK
    m_status.set_status("Power Supply",    StatusWidget::Level::OK);
    m_status.set_status("Motor Drive",     StatusWidget::Level::OK);
    m_status.set_status("Cooling",         StatusWidget::Level::OK);
    m_status.set_status("Communication",   StatusWidget::Level::OK);
    m_status.set_status("Safety Relay",    StatusWidget::Level::OK);

    m_log.append("Machine RESET — all subsystems nominal.");
}

void MainWindow::emergency_stop()
{
    m_state = MachineState::EMERGENCY;
    m_state_label.set_text("EMERGENCY STOP");
    m_state_label.set_css_classes({"pv-label", "status-alarm"});

    m_controls.set_buttons_enabled(false, false, false, false);

    m_status.set_status("Safety Relay", StatusWidget::Level::ALARM);

    m_log.append("!!! EMERGENCY STOP ACTIVATED !!!");
}

// ---------------------------------------------------------------------------
// simulation loop (called every 100 ms)
// ---------------------------------------------------------------------------
bool MainWindow::on_simulation_tick()
{
    if (m_state != MachineState::RUNNING)
        return true; // keep timer alive

    ++m_tick_count;

    // oscillate process value between 0 and 100
    const double step = 0.5;
    if (m_direction)
    {
        m_process_value += step;
        if (m_process_value >= 100.0)
        {
            m_process_value = 100.0;
            m_direction = false;
        }
    }
    else
    {
        m_process_value -= step;
        if (m_process_value <= 0.0)
        {
            m_process_value = 0.0;
            m_direction = true;
        }
    }

    // update UI
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%.1f", m_process_value);
    m_pv_label.set_text(buf);
    m_gauge.set_value(m_process_value);

    // --- simulate subsystem status changes based on PV ---
    // Cooling — check highest thresholds first
    if (m_process_value > 90.0)
        m_status.set_status("Cooling", StatusWidget::Level::ALARM);
    else if (m_process_value > 70.0)
        m_status.set_status("Cooling", StatusWidget::Level::WARNING);
    else
        m_status.set_status("Cooling", StatusWidget::Level::OK);

    // Motor drive stress
    if (m_process_value > 85.0)
        m_status.set_status("Motor Drive", StatusWidget::Level::WARNING);
    else
        m_status.set_status("Motor Drive", StatusWidget::Level::OK);

    // periodic log messages
    if (m_tick_count % 50 == 0)
    {
        std::ostringstream oss;
        oss << "Runtime: " << m_tick_count / 10 << "s  PV=" << m_process_value;
        m_log.append(oss.str());
    }

    return true; // keep timer alive
}
