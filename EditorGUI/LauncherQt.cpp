#include "LauncherQt.h"
#include "MainWindowQT.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QTranslator>
#include <QLibraryInfo>

int TCM::Editor::QT::LauncherQt::CoreLaunchEditor( int& argc, char** argv )
{
	QApplication a( argc, argv );
	QApplication::setStyle( QStyleFactory::create( "fusion" ) );
	QPalette palette;
	palette.setColor( QPalette::Window, QColor( 53, 53, 53 ) );
	palette.setColor( QPalette::WindowText, Qt::white );
	palette.setColor( QPalette::Base, QColor( 15, 15, 15 ) );
	palette.setColor( QPalette::AlternateBase, QColor( 53, 53, 53 ) );
	palette.setColor( QPalette::ToolTipBase, Qt::white );
	palette.setColor( QPalette::ToolTipText, Qt::white );
	palette.setColor( QPalette::Text, Qt::white );
	palette.setColor( QPalette::Button, QColor( 53, 53, 53 ) );
	palette.setColor( QPalette::ButtonText, Qt::white );
	palette.setColor( QPalette::BrightText, Qt::red );

	palette.setColor( QPalette::Highlight, QColor( 42, 130, 218 ) );
	palette.setColor( QPalette::HighlightedText, Qt::black );

	palette.setColor( QPalette::Disabled, QPalette::Text, Qt::darkGray );
	palette.setColor( QPalette::Disabled, QPalette::ButtonText, Qt::darkGray );

	qApp->setPalette( palette );

	QCoreApplication::setOrganizationName( "TayCaMead" );
	QCoreApplication::setApplicationName( "TayCaMead Editor" );

	QTranslator qtTranslator;
	qtTranslator.load( "qt_" + QLocale::system().name(),
	                   QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
	a.installTranslator( &qtTranslator );

	QTranslator myappTranslator;
	myappTranslator.load( "editorgui_" + QLocale::system().name() );
	a.installTranslator( &myappTranslator );

	QPixmap pixmap( ":/icon/splash" );
	QSplashScreen splash( pixmap );
	splash.show();
	a.processEvents();

	MainWindowQT w;
	w.show();
	splash.finish( &w );
	return a.exec();
}
