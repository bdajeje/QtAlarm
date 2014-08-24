#ifndef STOP_WINDOW_HPP
#define STOP_WINDOW_HPP

#include <QDialog>

class StopWindow final : public QDialog
{
  Q_OBJECT

  public:

    StopWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

  signals:

    void stopped();
};

#endif // STOP_WINDOW_HPP
