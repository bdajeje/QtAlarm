#ifndef SOUND_OPTIONS_HPP
#define SOUND_OPTIONS_HPP

#include <widget/jlabel.hpp>

#include <QDialog>
#include <QLabel>
#include <QSlider>

class SoundOptions final : public QDialog
{
  Q_OBJECT

  public:

    explicit SoundOptions(QWidget *parent = 0, Qt::WindowFlags flags = 0);

  protected:

    void keyReleaseEvent(QKeyEvent* event) override;

  private:

    void updateSelectFileUI(const QString& filepath);
    void setSelectedPath(const QString& path);

  private:

    JLabel*   m_widget_sound_label;
    JLabel*   m_widget_fading_label;
    QLabel*   m_widget_file_label;
    QSlider*  m_widget_slider;
    QSlider*  m_widget_volume_fade;

  public slots:

    void selectFile();
    void selectDirectory();
    void updateVolume(int value);
    void updateVolumeFading(int value);
    void saveVolume();
    void saveVolumeFading();
};

#endif // SOUND_OPTIONS_HPP
