#include "StatusWidget.h"

StatusWidget::StatusWidget()
    : Gtk::Box(Gtk::Orientation::VERTICAL, 4)
{
    build_ui();
}

void StatusWidget::build_ui()
{
    set_css_classes({"status-panel"});

    auto* title = Gtk::make_managed<Gtk::Label>("System Status");
    title->set_css_classes({"title"});
    title->set_halign(Gtk::Align::START);
    append(*title);

    m_listbox.set_selection_mode(Gtk::SelectionMode::NONE);
    m_listbox.add_css_class("rich-list");

    // subsystem rows
    const std::vector<Glib::ustring> subsystems = {
        "Power Supply",
        "Motor Drive",
        "Cooling",
        "Communication",
        "Safety Relay"
    };

    for (const auto& name : subsystems)
    {
        auto& row = m_rows[name];
        row.name_label.set_text(name);
        row.name_label.set_halign(Gtk::Align::START);
        row.name_label.set_hexpand(true);
        row.value_label.set_text("OK");
        row.value_label.set_halign(Gtk::Align::END);

        auto* hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 8);
        hbox->set_margin(4);
        hbox->append(row.name_label);
        hbox->append(row.value_label);

        m_listbox.append(*hbox);
    }

    // default all green
    for (const auto& name : subsystems)
        set_status(name, Level::OK);

    append(m_listbox);
}

void StatusWidget::set_status(const Glib::ustring& name, Level level)
{
    auto it = m_rows.find(name);
    if (it == m_rows.end())
        return;

    const char* text = "";
    switch (level)
    {
        case Level::OK:      text = "OK";       break;
        case Level::WARNING: text = "WARNING";  break;
        case Level::ALARM:   text = "ALARM";    break;
    }

    it->second.value_label.set_text(text);
    apply_row_style(it->second, level);
}

void StatusWidget::apply_row_style(Row& row, Level level)
{
    const char* css = "";
    switch (level)
    {
        case Level::OK:      css = "status-ok";    break;
        case Level::WARNING: css = "status-warn";  break;
        case Level::ALARM:   css = "status-alarm"; break;
    }

    row.value_label.set_css_classes({css});
}
