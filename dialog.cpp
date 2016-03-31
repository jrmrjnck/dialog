/*
 * This is free and unencumbered software released into the public domain.
 */

#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <QCommandLineParser>

static
void addOptions( QCommandLineParser& argparser )
{
   const QList<QStringList> options = {
      {"message", "text", "Message text.", "Hello, world."},
      {"window-title", "text", "Window title.", "Timer"},
      {"period", "sec", "Pop-up period. (0 = disable)", "0"},
      {"timeout", "sec", "Time to show window.", QString::number(30)}
      };

   for(auto& option : options)
   {
      argparser.addOption(
         QCommandLineOption(
            {QString(option[0][0]), option[0]},
            QString("%1 (Default: %2)").arg(option[2]).arg(option[3]),
            option[1],
            option[3]
            )
         );
   }
}

class DialogBox : public QMessageBox
{
private:
   int _period;
   int _timeout;

public:
   DialogBox(
      const QString& windowTitle,
      const QString& message,
      int period,
      int timeout
      )
    : _period(period),
      _timeout(timeout)
   {
      setIcon( QMessageBox::Information );
      setWindowTitle( windowTitle );
      setText( message );
      setWindowFlags( Qt::WindowStaysOnTopHint );

      startTimeout();
   }

   void startTimeout()
   {
      show();
      QTimer::singleShot( 1000 * _timeout, this, &DialogBox::endTimeout );
   }

   void endTimeout()
   {
      if( _period > 0)
      {
         hide();
         QTimer::singleShot( 1000 * (_period - _timeout), this, &DialogBox::startTimeout );
      }
      else
      {
         close();
      }
   }

   void done( int r )
   {
      if( _period > 0 )
      {
         hide();
      }
      else
      {
         QMessageBox::done( r );
      }
   }
};

int main( int argc, char* argv[] )
{
   QApplication app( argc, argv );
   QApplication::setApplicationName( "dialog" );

   QCommandLineParser argparse;
   argparse.setApplicationDescription( "A simple timed dialog box." );
   argparse.addHelpOption();
   addOptions( argparse );
   argparse.process( app );

   auto timeout = argparse.value("timeout").toInt();
   auto period = argparse.value("period").toInt();

   DialogBox dialogBox(
      argparse.value("window-title"),
      argparse.value("message"),
      period,
      timeout
      );

   return app.exec();
}
