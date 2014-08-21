#include "mainwindow.hpp"

#include <iostream>

#include <utils/properties.hpp>

#include <QApplication>
#include <QMessageBox>
#include <QObject>
//#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);

  if( !utils::Properties::isValid() )
  {
    QMessageBox::warning(nullptr, QObject::tr("Property error"),
                                  QObject::tr("The application can't load property file: ") + utils::Properties::instance().filepath());
    return EXIT_FAILURE;
  }

  // \todo Translations
//  QTranslator translator;
//  translator.load("resources/translations/" + QLocale::system().name());
//  application.installTranslator(&translator);

  MainWindow window;
  window.show();

  return application.exec();
}
