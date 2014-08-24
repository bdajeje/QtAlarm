#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "widget/sound_options.hpp"
#include "widget/stop_window.hpp"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:

    void createMenu();
    QMenu* createTrayMenu();
    QAction* createQuitAction();
    QString fileToPlay();
    QString addApplicationPath(QString path);

  private:

    QSystemTrayIcon* m_tray_icon;
    QMediaPlayer*    m_media_player;
    SoundOptions*    m_sound_options {nullptr};
    StopWindow*      m_stop_window {nullptr};

  public slots:

    void timeout();
    void showAbout();
    void showSoundOptions();
    void toggleWindowVisibility(QSystemTrayIcon::ActivationReason);
    void stopSound();
};

#endif // MAINWINDOW_HPP
