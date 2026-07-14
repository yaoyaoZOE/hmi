#ifndef HMI_STATUS_WIDGET_H
#define HMI_STATUS_WIDGET_H

#include <gtkmm.h>

/// A panel showing subsystem status indicators with colored labels.
class StatusWidget : public Gtk::Box
{
public:
    StatusWidget();

    enum class Level { OK, WARNING, ALARM };

    void set_status(const Glib::ustring& name, Level level);

private:
    struct Row
    {
        Gtk::Label name_label;
        Gtk::Label value_label;
    };

    void build_ui();
    void apply_row_style(Row& row, Level level);

    Gtk::ListBox m_listbox;
    std::map<Glib::ustring, Row> m_rows;
};

#endif // HMI_STATUS_WIDGET_H
