#include <gtkmm.h>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create("com.example.hmi_app");

    // CSS styling for industrial HMI look-and-feel
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(R"(
        /* ---------- global ---------- */
        * {
            font-family: "Segoe UI", "Noto Sans", sans-serif;
        }

        window {
            background-color: #1a1d23;
            color: #e0e0e0;
        }

        /* ---------- header bar ---------- */
        headerbar {
            background: #252830;
            border-bottom: 2px solid #3a3f4b;
            padding: 4px 12px;
            min-height: 44px;
        }
        headerbar label {
            font-size: 16px;
            font-weight: bold;
            color: #f0f0f0;
        }

        /* ---------- status panel ---------- */
        .status-panel {
            background: #21242b;
            border: 1px solid #3a3f4b;
            border-radius: 8px;
            padding: 12px;
        }
        .status-panel label.title {
            font-size: 13px;
            font-weight: bold;
            color: #78909c;
            margin-bottom: 8px;
        }
        .status-ok {
            color: #4caf50;
            font-weight: bold;
        }
        .status-warn {
            color: #ff9800;
            font-weight: bold;
        }
        .status-alarm {
            color: #f44336;
            font-weight: bold;
        }

        /* ---------- control buttons ---------- */
        .control-panel {
            background: #21242b;
            border: 1px solid #3a3f4b;
            border-radius: 8px;
            padding: 12px;
        }
        button.start {
            background: #2e7d32;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 0;
            font-weight: bold;
            font-size: 14px;
        }
        button.start:hover {
            background: #388e3c;
        }
        button.stop {
            background: #c62828;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 0;
            font-weight: bold;
            font-size: 14px;
        }
        button.stop:hover {
            background: #d32f2f;
        }
        button.reset {
            background: #37474f;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 0;
            font-weight: bold;
            font-size: 14px;
        }
        button.reset:hover {
            background: #455a64;
        }
        button.emergency {
            background: #b71c1c;
            color: white;
            border: 3px solid #ff1744;
            border-radius: 6px;
            padding: 12px 0;
            font-weight: bold;
            font-size: 16px;
        }
        button.emergency:hover {
            background: #d50000;
        }
        button:disabled {
            opacity: 0.4;
        }

        /* ---------- process-value labels ---------- */
        .pv-label {
            font-size: 28px;
            font-weight: bold;
            color: #64b5f6;
        }
        .pv-unit {
            font-size: 14px;
            color: #90a4ae;
        }
        .section-title {
            font-size: 13px;
            font-weight: bold;
            color: #78909c;
            margin-bottom: 4px;
        }

        /* ---------- log ---------- */
        .log-view {
            background: #181a1f;
            border: 1px solid #3a3f4b;
            border-radius: 6px;
            padding: 8px;
        }
        .log-view textview {
            background: #181a1f;
            color: #b0bec5;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 12px;
        }

        /* ---------- gauge area ---------- */
        .gauge-panel {
            background: #21242b;
            border: 1px solid #3a3f4b;
            border-radius: 8px;
            padding: 12px;
        }
    )");

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    return app->make_window_and_run<MainWindow>(argc, argv);
}
