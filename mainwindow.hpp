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

  private:

    void createMenu();
    QMenu* createTrayMenu();
    QAction* createQuitAction();
    QString fileToPlay();
    QString addApplicationPath(QString path);
    void raiseWindow();

  private:

    QSystemTrayIcon* m_tray_icon;
    QMediaPlayer*    m_media_player;
    SoundOptions*    m_sound_options;
    bool             m_stopped {true};

  public slots:

    void timeout();
    void showAbout();
    void showSoundOptions();
    void toggleWindowVisibility(QSystemTrayIcon::ActivationReason);
    void increaseVolume();
    void stopSound();
    void alarmStatusChanged(QMediaPlayer::State status);
};

#endif // MAINWINDOW_HPP
