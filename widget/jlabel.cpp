#include "jlabel.hpp"

JLabel::JLabel(QWidget *parent)
  : QLabel("", parent)
{}

JLabel::JLabel(const QString& label, QWidget *parent)
  : QLabel(label, parent)
{
  connect( this, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
}

void JLabel::slotClicked()
{

}

void JLabel::mousePressEvent(QMouseEvent*)
{
  emit clicked();
}
