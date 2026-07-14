#include "ControlPanel.h"

ControlPanel::ControlPanel()
    : Gtk::Box(Gtk::Orientation::VERTICAL, 8)
{
    set_css_classes({"control-panel"});

    auto* title = Gtk::make_managed<Gtk::Label>("Controls");
    title->set_css_classes({"title"});
    title->set_halign(Gtk::Align::START);
    append(*title);

    // --- Start ---
    m_btn_start.set_label("▶  START");
    m_btn_start.set_css_classes({"start"});
    m_btn_start.signal_clicked().connect([this]() { m_sig_start.emit(); });
    append(m_btn_start);

    // --- Stop ---
    m_btn_stop.set_label("■  STOP");
    m_btn_stop.set_css_classes({"stop"});
    m_btn_stop.signal_clicked().connect([this]() { m_sig_stop.emit(); });
    m_btn_stop.set_sensitive(false);
    append(m_btn_stop);

    // --- Reset ---
    m_btn_reset.set_label("↻  RESET");
    m_btn_reset.set_css_classes({"reset"});
    m_btn_reset.signal_clicked().connect([this]() { m_sig_reset.emit(); });
    append(m_btn_reset);

    // --- spacer ---
    auto* spacer = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::HORIZONTAL);
    spacer->set_margin_top(4);
    append(*spacer);

    // --- Emergency ---
    m_btn_emergency.set_label("⚠  EMERGENCY STOP");
    m_btn_emergency.set_css_classes({"emergency"});
    m_btn_emergency.signal_clicked().connect([this]() { m_sig_emergency.emit(); });
    append(m_btn_emergency);
}

ControlPanel::SignalVoid& ControlPanel::signal_start()     { return m_sig_start; }
ControlPanel::SignalVoid& ControlPanel::signal_stop()      { return m_sig_stop; }
ControlPanel::SignalVoid& ControlPanel::signal_reset()     { return m_sig_reset; }
ControlPanel::SignalVoid& ControlPanel::signal_emergency() { return m_sig_emergency; }

void ControlPanel::set_buttons_enabled(bool start, bool stop, bool reset, bool emergency)
{
    m_btn_start.set_sensitive(start);
    m_btn_stop.set_sensitive(stop);
    m_btn_reset.set_sensitive(reset);
    m_btn_emergency.set_sensitive(emergency);
}
