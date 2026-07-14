#ifndef HMI_LOG_VIEW_H
#define HMI_LOG_VIEW_H

#include <gtkmm.h>

/// Scrolled text view for timestamped event / alarm messages.
class LogView : public Gtk::Box
{
public:
    LogView();

    /// Append a timestamped message (auto-scrolls to bottom).
    void append(const Glib::ustring& message);

    /// Clear all entries.
    void clear();

private:
    Gtk::ScrolledWindow m_scroll;
    Gtk::TextView       m_textview;
    Glib::RefPtr<Gtk::TextBuffer> m_buffer;
};

#endif // HMI_LOG_VIEW_H
