#include "sound_options.hpp"

#include <utils/properties.hpp>
#include <utils/strings.hpp>

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
  m_widget_slider = new QSlider(Qt::Orientation::Horizontal);
  m_widget_slider->setTickInterval(1);
  m_widget_slider->setMaximum(100);
  m_widget_slider->setValue(volume.toInt());

  auto widget_select_file = new QPushButton(tr("Select alarm sound"));
  m_widget_file_label     = new QLabel();

  main_layout->addWidget(m_widget_sound_label);
  main_layout->addWidget(m_widget_slider);
  main_layout->addWidget(widget_select_file);
  main_layout->addWidget(m_widget_file_label);
  setLayout(main_layout);

  updateVolume(volume.toInt());
  updateSelectFileUI(utils::Properties::get(utils::Property::AlarmFile));

  connect(m_widget_sound_label, SIGNAL(clicked()), m_widget_slider, SLOT(setFocus()));
  connect(m_widget_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVolume(int)));
  connect(m_widget_slider, SIGNAL(sliderReleased()), this, SLOT(saveVolume()));
  connect(widget_select_file, SIGNAL(pressed()), this, SLOT(selectFile()));
}

void SoundOptions::selectFile()
{
  const QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.ogg *.mp3)"));
  updateSelectFileUI(filepath);
  utils::Properties::save( utils::Property::AlarmFile, filepath );
}

void SoundOptions::updateSelectFileUI(const QString& filepath)
{
  m_widget_file_label->setText( utils::strings::fitIn(filepath.toStdString(), 40, utils::strings::Fit::Middle).c_str() );
}

void SoundOptions::updateVolume(int value)
{
  m_widget_sound_label->setText(tr("Sound volume: ") + QString::number(value) + "%");
}

void SoundOptions::saveVolume()
{
  utils::Properties::save( utils::Property::AlarmVolume, QString::number(m_widget_slider->value()) );
}
