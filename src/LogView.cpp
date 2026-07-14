#include "LogView.h"
#include <ctime>

LogView::LogView()
    : Gtk::Box(Gtk::Orientation::VERTICAL, 0)
{
    set_css_classes({"log-view"});

    m_buffer = Gtk::TextBuffer::create();
    m_textview.set_buffer(m_buffer);
    m_textview.set_editable(false);
    m_textview.set_monospace(true);
    m_textview.set_wrap_mode(Gtk::WrapMode::WORD_CHAR);

    m_scroll.set_child(m_textview);
    m_scroll.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::ALWAYS);
    m_scroll.set_min_content_height(120);
    m_scroll.set_vexpand(true);

    Gtk::Box::append(m_scroll);
}

void LogView::append(const Glib::ustring& message)
{
    auto now  = std::time(nullptr);
    auto* tm  = std::localtime(&now);

    char ts[32];
    std::strftime(ts, sizeof(ts), "[%H:%M:%S] ", tm);

    m_buffer->insert(m_buffer->end(), ts);
    m_buffer->insert(m_buffer->end(), message);
    m_buffer->insert(m_buffer->end(), "\n");

    // auto-scroll
    auto adj = m_scroll.get_vadjustment();
    adj->set_value(adj->get_upper());
}

void LogView::clear()
{
    m_buffer->set_text("");
}
