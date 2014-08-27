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
    void defaultToolTip();

  private:

    QSystemTrayIcon* m_tray_icon;
    QMediaPlayer*    m_media_player;
    SoundOptions*    m_sound_options;
    QAction*         m_action_stop_alarm;
    bool             m_stopped {true};

  public slots:

    void timeout();
    void showAbout();
    void showSoundOptions();
    void toggleWindowVisibility(QSystemTrayIcon::ActivationReason);
    void increaseVolume();
    void stopSound();
    void alarmStatusChanged(QMediaPlayer::State status);
    void alarmStarted();
    void alarmStopped();
    void countdownUpdated(int remaining_seconds);
    void tabChanged(int index);

  signals:

    void menuStopSound();
};

#endif // MAINWINDOW_HPP
