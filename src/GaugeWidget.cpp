#include "GaugeWidget.h"
#include <cmath>

namespace {
    constexpr double PI = 3.14159265358979323846;
}

GaugeWidget::GaugeWidget()
{
    set_content_width(200);
    set_content_height(200);
    set_draw_func(sigc::mem_fun(*this, &GaugeWidget::on_draw));
}

void GaugeWidget::set_value(double value)
{
    m_value = std::clamp(value, 0.0, 100.0);
    queue_draw();
}

void GaugeWidget::set_label(const Glib::ustring& text)
{
    m_label = text;
    queue_draw();
}

void GaugeWidget::on_draw(const Cairo::RefPtr<Cairo::Context>& cr,
                          int width, int height)
{
    const double w = static_cast<double>(width);
    const double h = static_cast<double>(height);

    const double cx  = w / 2.0;
    const double cy  = h * 0.55;
    const double r   = std::min(w, h) * 0.38;

    const double angle_start = 0.75 * PI;  // 135°
    const double angle_end   = 2.25 * PI;  // 405° (225° from start)
    const double angle_range = angle_end - angle_start;

    // ---- background track ----
    cr->set_source_rgba(0.18, 0.20, 0.24, 1.0);  // #2e3239
    cr->set_line_width(18.0);
    cr->set_line_cap(Cairo::Context::LineCap::ROUND);
    cr->arc(cx, cy, r, angle_start, angle_end);
    cr->stroke();

    // ---- value arc (color shifts green→yellow→red) ----
    const double frac   = m_value / 100.0;
    const double sweep  = angle_range * frac;

    // colour interpolation
    double red, green, blue;
    if (frac < 0.5)
    {
        // green → yellow
        double t = frac / 0.5;
        red   = t;
        green = 1.0;
        blue  = 0.0;
    }
    else
    {
        // yellow → red
        double t = (frac - 0.5) / 0.5;
        red   = 1.0;
        green = 1.0 - t;
        blue  = 0.0;
    }

    cr->set_source_rgb(red, green, blue);
    cr->set_line_width(18.0);
    cr->set_line_cap(Cairo::Context::LineCap::ROUND);
    cr->arc(cx, cy, r, angle_start, angle_start + sweep);
    cr->stroke();

    // ---- center value text ----
    cr->set_source_rgb(0.88, 0.88, 0.88);
    cr->select_font_face("Segoe UI", Cairo::ToyFontFace::Slant::NORMAL,
                         Cairo::ToyFontFace::Weight::BOLD);
    cr->set_font_size(32.0);

    char buf[16];
    std::snprintf(buf, sizeof(buf), "%.0f", m_value);
    Cairo::TextExtents te;
    cr->get_text_extents(buf, te);
    cr->move_to(cx - te.width / 2.0, cy + te.height / 2.0);
    cr->show_text(buf);

    // ---- label below value ----
    cr->set_source_rgb(0.49, 0.55, 0.59);
    cr->set_font_size(13.0);
    cr->get_text_extents(m_label.c_str(), te);
    cr->move_to(cx - te.width / 2.0, cy + 30.0);
    cr->show_text(m_label.c_str());

    // ---- scale tick marks ----
    cr->set_source_rgba(0.55, 0.55, 0.55, 0.5);
    cr->set_line_width(2.0);
    const double tick_r_inner = r * 0.82;
    const double tick_r_outer = r * 0.92;

    for (int i = 0; i <= 10; ++i)
    {
        double a = angle_start + angle_range * (i / 10.0);
        cr->move_to(cx + tick_r_inner * std::cos(a),
                    cy + tick_r_inner * std::sin(a));
        cr->line_to(cx + tick_r_outer * std::cos(a),
                    cy + tick_r_outer * std::sin(a));
    }
    cr->stroke();
}
