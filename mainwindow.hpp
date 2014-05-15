#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "widget/sound_options.hpp"

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

  private:

    QSystemTrayIcon* m_tray_icon;
    QMediaPlayer*    m_media_player;
    SoundOptions*    m_sound_options {nullptr};

  public slots:

    void timeout();
    void showAbout();
    void showSoundOptions();
};

#endif // MAINWINDOW_HPP
