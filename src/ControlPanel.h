#ifndef HMI_CONTROL_PANEL_H
#define HMI_CONTROL_PANEL_H

#include <gtkmm.h>

/// A panel of industrial control buttons: Start, Stop, Reset, Emergency.
class ControlPanel : public Gtk::Box
{
public:
    ControlPanel();

    // signal accessors — return the proxied signal so MainWindow can connect
    using SignalVoid = sigc::signal<void()>;

    SignalVoid& signal_start();
    SignalVoid& signal_stop();
    SignalVoid& signal_reset();
    SignalVoid& signal_emergency();

    void set_buttons_enabled(bool start, bool stop, bool reset, bool emergency);

private:
    Gtk::Button m_btn_start;
    Gtk::Button m_btn_stop;
    Gtk::Button m_btn_reset;
    Gtk::Button m_btn_emergency;

    SignalVoid m_sig_start;
    SignalVoid m_sig_stop;
    SignalVoid m_sig_reset;
    SignalVoid m_sig_emergency;
};

#endif // HMI_CONTROL_PANEL_H
