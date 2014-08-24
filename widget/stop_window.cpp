#include "stop_window.hpp"

#include <QPushButton>
#include <QVBoxLayout>

StopWindow::StopWindow(QWidget *parent, Qt::WindowFlags flags)
  : QDialog(parent, flags)
{
  setWindowTitle(tr("It's time !"));
  setWindowIcon(QIcon("resources/images/sounds.jpg"));

  auto main_layout = new QVBoxLayout;
  auto stop_button = new QPushButton("STOP");

  main_layout->addWidget( stop_button );
  setLayout(main_layout);

  connect( stop_button, SIGNAL(pressed()), this, SIGNAL(stopped()) );
}
