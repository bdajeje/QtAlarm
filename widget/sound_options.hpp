#ifndef SOUND_OPTIONS_HPP
#define SOUND_OPTIONS_HPP

#include <widget/jlabel.hpp>

#include <QLabel>
#include <QSlider>
#include <QWidget>

class SoundOptions final : public QWidget
{
  Q_OBJECT

  public:

    explicit SoundOptions(QWidget *parent = 0);

  private:

    void updateSelectFileUI(const QString& filepath);

  private:

    JLabel*  m_widget_sound_label;
    QLabel*  m_widget_file_label;
    QSlider* m_widget_slider;

  signals:

  public slots:

    void selectFile();
    void updateVolume(int value);
    void saveVolume();
};

#endif // SOUND_OPTIONS_HPP
