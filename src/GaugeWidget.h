#ifndef HMI_GAUGE_WIDGET_H
#define HMI_GAUGE_WIDGET_H

#include <gtkmm.h>

/// A custom circular gauge drawn with Cairo.
/// Displays a value 0..100 with a colored arc and central text.
class GaugeWidget : public Gtk::DrawingArea
{
public:
    GaugeWidget();

    /// Set value 0..100
    void set_value(double value);

    /// Set the label shown below the arc
    void set_label(const Glib::ustring& text);

private:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr,
                 int width, int height);

    double        m_value{0.0};
    Glib::ustring m_label{"RPM"};
};

#endif // HMI_GAUGE_WIDGET_H
