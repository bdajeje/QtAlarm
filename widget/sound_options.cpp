#include "sound_options.hpp"

#include <utils/properties.hpp>

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

SoundOptions::SoundOptions(QWidget *parent)
  : QWidget(parent)
{
  auto main_layout = new QVBoxLayout;

  // Retrieve volume property
  const auto& volume = utils::Properties::get(utils::Property::AlarmVolume);

  m_widget_sound_label = new JLabel;
  auto widget_slider   = new QSlider(Qt::Orientation::Horizontal);
  widget_slider->setTickInterval(1);
  widget_slider->setMaximum(100);
  widget_slider->setValue(volume.toInt());

  auto widget_select_file = new QPushButton(tr("Select alarm sound"));
  m_widget_file_label     = new QLabel(utils::Properties::get(utils::Property::AlarmFile));

  main_layout->addWidget(m_widget_sound_label);
  main_layout->addWidget(widget_slider);
  main_layout->addWidget(widget_select_file);
  main_layout->addWidget(m_widget_file_label);
  setLayout(main_layout);

  updateVolumeUI(volume);

  connect(m_widget_sound_label, SIGNAL(clicked()), widget_slider, SLOT(setFocus()));
  connect(widget_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVolume(int)));
  connect(widget_select_file, SIGNAL(pressed()), this, SLOT(selectFile()));
}

void SoundOptions::selectFile()
{
  const QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.ogg *.mp3)"));
  m_widget_file_label->setText(filepath);
  utils::Properties::save( utils::Property::AlarmFile, filepath );
}

void SoundOptions::updateVolume(int value)
{
  const auto volume = QString::number(value);
  utils::Properties::save( utils::Property::AlarmVolume, volume );
  updateVolumeUI(volume);
}

void SoundOptions::updateVolumeUI(const QString& volume)
{
  m_widget_sound_label->setText(tr("Sound volume: ") + volume + "%");
}
