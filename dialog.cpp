#include <QMessageBox>
#include <QApplication>
#include <QTimer>

int main( int argc, char* argv[] )
{
  QApplication app( argc, argv );

  auto args = app.arguments();
  auto text = args.value( 1, "No text provided" );
  auto title = args.value( 2, "Timer" );
  auto time = args.value( 3, "30" ).toInt();

  QMessageBox messageBox( QMessageBox::Information,
                          title,
                          text,
                          QMessageBox::NoButton,
                          nullptr,
                          Qt::WindowStaysOnTopHint
                          );
  messageBox.show();

  QTimer::singleShot( 1000 * time, &app, QApplication::quit );

  return app.exec();
}
