#ifndef JLABEL_HPP
#define JLABEL_HPP

#include <QLabel>

class JLabel final : public QLabel
{
  Q_OBJECT

  public:

    explicit JLabel(QWidget *parent = 0);
    explicit JLabel(const QString& label, QWidget *parent = 0);

  protected:

    void mousePressEvent( QMouseEvent * event );

  signals:

    void clicked();

  public slots:

     void slotClicked();
};

#endif // JLABEL_HPP
