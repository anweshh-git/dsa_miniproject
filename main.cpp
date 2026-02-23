#include <QApplication>
#include <QStyleFactory>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Dark theme
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette dark;
    dark.setColor(QPalette::Window,          QColor(18, 28, 42));
    dark.setColor(QPalette::WindowText,      QColor(200, 220, 255));
    dark.setColor(QPalette::Base,            QColor(13, 26, 42));
    dark.setColor(QPalette::AlternateBase,   QColor(22, 36, 56));
    dark.setColor(QPalette::ToolTipBase,     Qt::white);
    dark.setColor(QPalette::ToolTipText,     Qt::black);
    dark.setColor(QPalette::Text,            QColor(200, 220, 255));
    dark.setColor(QPalette::Button,          QColor(30, 50, 80));
    dark.setColor(QPalette::ButtonText,      QColor(200, 220, 255));
    dark.setColor(QPalette::BrightText,      Qt::red);
    dark.setColor(QPalette::Highlight,       QColor(42, 100, 180));
    dark.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(dark);

    MainWindow window;
    window.show();

    return app.exec();
}
