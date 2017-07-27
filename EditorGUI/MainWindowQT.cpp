#include "MainWindowQT.h"

#include <QApplication>
#include <QToolBar>
#include <QStatusBar>
#include <QGridLayout>

#include <QToolBox>
#include <QTextStream>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QActionGroup>
#include <QAction>
#include <QSettings>

#include <QDesktopServices>

#include "TCMCore/Core.h"
#include "FileExplorerQT.h"
#include "InspectorQT.h"
#include "SceneTreeQT.h"
#include "LoggerViewQT.h"
#include "WidgetTree.h"
#include "EditorCommandManager.h"
#include "ProjectEditorQT.h"
#include "RendererWindowQT.h"
#include "TCMNativeWindow/NativeWindow.h"
#include "TCMInputManager/InputManager.h"
#include "EngineClasses/TCMScene.h"
#include "SkyboxEditorQT.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			MainWindowQT::MainWindowQT( QWidget* parent )
				: QMainWindow( parent )
			{
				if ( qApp->palette().color( QPalette::WindowText ) == Qt::white )
					m_color = "white";
				else
					m_color = "black";

				setWindowTitle( "TayCaMead" );
				setWindowIcon( QIcon::fromTheme( ":/icon/launcher" ) );
				//container->setFocusPolicy( Qt::StrongFocus );
				QWindow* win = QWindow::fromWinId( WId( Engine::Graphics::NativeWindow::GetWindow() ) );
				win->setKeyboardGrabEnabled( true );
				win->setMouseGrabEnabled( true );

				QWidget* container = QWidget::createWindowContainer( win );
				container->setFocusPolicy( Qt::StrongFocus );

				container->setAttribute( Qt::WA_PaintOnScreen );
				container->setAttribute( Qt::WA_OpaquePaintEvent );
				container->setAttribute( Qt::WA_NoSystemBackground );

				setCentralWidget( container );

				setDockOptions( dockOptions() | AllowNestedDocks );

				SetupStatusBar();
				SetupMenuBar();
				SetupToolbar();

				QSettings settings;
				LoadProject( settings.value( LAST_PROJECT_KEY ).toString().toStdString() );
				LoadScene( settings.value( LAST_SCENE_KEY ).toString().toStdString() );
			}

#pragma region Initialization

			void MainWindowQT::SetupStatusBar()
			{
				statusBar()->setObjectName( "StatusBar" );
			}

			void MainWindowQT::SetupMenuBar()
			{
				QMenu* menu = menuBar()->addMenu( tr( "&File" ) );
				QAction* action;

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::ADD ) ) ),
				                          tr( "New Project" ), this, &MainWindowQT::NewProjectClicked );
				action->setStatusTip( tr( "Create a new project" ) );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::SAVE ) ) ),
				                          tr( "Save Project" ), this, &MainWindowQT::SaveProjectClicked );
				action->setStatusTip( tr( "Save the current project" ) );

				//itemsDependingOnScene.push_back( action );

				action = menu->addAction( tr( "Save Project As" ), this, &MainWindowQT::SaveProjectAsClicked );
				action->setStatusTip( tr( "Save the current project" ) );

				//itemsDependingOnScene.push_back( action );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::OPEN ) ) ),
				                          tr( "Load Project" ), this, &MainWindowQT::LoadProjectClicked );
				action->setStatusTip( tr( "Load a project" ) );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::ADD ) ) ),
				                          tr( "New Scene" ), this, &MainWindowQT::NewSceneClicked );
				action->setShortcuts( QKeySequence::New );
				action->setStatusTip( tr( "Create a new scene" ) );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::SAVE ) ) ),
				                          tr( "Save Scene" ), this, &MainWindowQT::SaveSceneClicked );
				action->setShortcuts( QKeySequence::Save );
				action->setStatusTip( tr( "Save the current scene" ) );

				//itemsDependingOnScene.push_back( action );

				action = menu->addAction( tr( "Save Scene As" ), this, &MainWindowQT::SaveSceneAsClicked );
				action->setShortcuts( QKeySequence::SaveAs );
				action->setStatusTip( tr( "Save the current scene" ) );

				//itemsDependingOnScene.push_back( action );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::OPEN ) ) ),
				                          tr( "Load Scene" ), this, &MainWindowQT::LoadSceneClicked );
				//action->setShortcuts( QKeySequence::Open );
				action->setStatusTip( tr( "Open a scene" ) );

				menu->addSeparator();
				menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::BUILD ) ) ),
				                 tr( "Build" ), this, &MainWindowQT::BuildClicked );

				//itemsDependingOnScene.push_back( action );

				menu->addAction( tr( "Build Settings" ), this, &MainWindowQT::BuildSettingsClicked );
				menu->addSeparator();

				//itemsDependingOnScene.push_back( action );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::SETTINGS ) ) ),
				                          tr( "Settings" ), this, &MainWindowQT::SettingsClicked );
				action->setShortcuts( QKeySequence::Preferences );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::CANCEL ) ) ),
				                          tr( "Quit" ), this, &QWidget::close );
				action->setShortcuts( QKeySequence::Quit );
				action->setStatusTip( tr( "Quit the editor" ) );

				menu = menuBar()->addMenu( tr( "&Edit" ) );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::UNDO ) ) ),
				                          tr( "Undo" ), this, &MainWindowQT::UndoClicked );
				action->setShortcuts( QKeySequence::Undo );
				action->setEnabled( false );
				undoMenu = action;

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::REDO ) ) ),
				                          tr( "Redo" ), this, &MainWindowQT::RedoClicked );
				action->setShortcuts( QKeySequence::Redo );
				action->setEnabled( false );
				redoMenu = action;

				menu->addSeparator();

				/*action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::COPY ) ) ),
				                          tr( "Copy" ), this, &MainWindowQT::CopyClicked );
				action->setShortcuts( QKeySequence::Copy );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::CUT ) ) ),
				                          tr( "Cut" ), this, &MainWindowQT::CutClicked );
				action->setShortcuts( QKeySequence::Cut );

				action = menu->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::PASTE ) ) ),
				                          tr( "Paste" ), this, &MainWindowQT::PasteClicked );
				action->setShortcuts( QKeySequence::Paste );*/

				menu->addSeparator();

				menu->addAction( tr( "Project Settings" ), this, &MainWindowQT::ProjectSettingsClicked );
				menu->addAction( tr( "Skybox Editor" ), this, &MainWindowQT::SkyboxEditorClicked );

				menu = menuBar()->addMenu( tr( "&Window" ) );

				menu->addAction( CreateProjectExplorerDock() );
				menu->addAction( CreateLoggerDock() );
				menu->addAction( CreateInspectorDock() );
				menu->addAction( CreateSceneTreeDock() );
#if _DEBUG
				menu->addAction( CreateDebugWidgetTree() );
#endif
				menu->addSeparator();

				menu = menuBar()->addMenu( tr( "&Help" ) );

				menu->addAction( tr( "Documentation" ), this, &MainWindowQT::DocClicked );
				menu->addAction( tr( "About" ), this, &MainWindowQT::AboutClicked );
			}

			void MainWindowQT::SetupToolbar()
			{
				QAction* action;
				QToolBar* fileToolBar = new QToolBar( tr( "File" ) );
				fileToolBar->setObjectName( "File Toolbar" );

				fileToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::SAVE ) ) ),
				                        tr( "Save All" ), this, &MainWindowQT::SaveAllClicked );

				//itemsDependingOnScene.push_back( action );

				/*fileToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::OPEN ) ) ),
				                        tr( "Load Scene" ), this, &MainWindowQT::LoadProjectClicked );

				fileToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::SETTINGS ) ) ),
				                        tr( "Settings" ), this, &MainWindowQT::SettingsClicked );*/

				addToolBar( fileToolBar );

				QToolBar* actionsToolBar = new QToolBar( tr( "Actions" ) );
				actionsToolBar->setObjectName( "ActionToolbar" );

				action = actionsToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::UNDO ) ) ),
				                                    tr( "Undo" ), this, &MainWindowQT::UndoClicked );
				action->setEnabled( false );
				undoToolbar = action;

				action = actionsToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::REDO ) ) ),
				                                    tr( "Redo" ), this, &MainWindowQT::RedoClicked );
				action->setEnabled( false );
				redoToolbar = action;
				addToolBar( actionsToolBar );

				QToolBar* transformToolBar = new QToolBar( tr( "Transform" ) );
				transformToolBar->setObjectName( "TransformToolbar" );

				QActionGroup* transformGroup = new QActionGroup( transformToolBar );
				transformGroup->setObjectName( "TransformGroup" );

				QAction* moveAction = transformToolBar->addAction( tr( "Move" ), this, &MainWindowQT::MoveClicked );
				moveAction->setCheckable( true );
				moveAction->setChecked( true );
				QAction* translateAction = transformToolBar->addAction( tr( "Translate" ), this, &MainWindowQT::TranslateClicked );
				translateAction->setCheckable( true );
				QAction* rotateAction = transformToolBar->addAction( tr( "Rotate" ), this, &MainWindowQT::RotateClicked );
				rotateAction->setCheckable( true );
				QAction* scaleAction = transformToolBar->addAction( tr( "Scale" ), this, &MainWindowQT::ScaleClicked );
				scaleAction->setCheckable( true );

				transformGroup->addAction( moveAction );
				transformGroup->addAction( translateAction );
				transformGroup->addAction( rotateAction );
				transformGroup->addAction( scaleAction );

				addToolBar( transformToolBar );

				QToolBar* gameToolBar = new QToolBar( tr( "Game" ) );
				gameToolBar->setObjectName( "GameToolbar" );

				pauseAction = gameToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::PAUSE ) ) ),
				                                      tr( "Pause" ), this, &MainWindowQT::PauseClicked );
				pauseAction->setCheckable( true );

				playAction = gameToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::PLAY ) ) ),
				                                     tr( "Play" ), this, &MainWindowQT::PlayClicked );
				playAction->setCheckable( true );

				nextAction = gameToolBar->addAction( QIcon::fromTheme( QString::fromStdString( GetIconPath( IconType::NEXT ) ) ),
				                                     tr( "Next Frame" ), this, &MainWindowQT::NextFrameClicked );
				addToolBar( gameToolBar );

				QToolBar* debugToolBar = new QToolBar( tr( "Debug" ) );
				debugToolBar->setObjectName( "DebugToolbar" );

				debugToolBar->addAction( tr( "Focus" ), this, &MainWindowQT::FocusClicked );
				debugToolBar->addAction( tr( "Unfocus" ), this, &MainWindowQT::UnfocusClicked );

				action = debugToolBar->addAction( tr( "Ovni Mode" ), this, &MainWindowQT::OvniClicked );
				action->setCheckable( true );
				addToolBar( debugToolBar );
			}

			QAction* MainWindowQT::CreateProjectExplorerDock()
			{
				QDockWidget* dock = new QDockWidget( tr( "Project Explorer" ) );
				dock->setObjectName( "ProjectExplorerDock" );

				FileExplorerQT* feWidget = new FileExplorerQT();
				feWidget->Register();

				feWidget->setObjectName( "ProjectExplorerWidget" );
				//feWidget->SetDirectory("C:/Users/");

				dock->setWidget( feWidget );
				dock->setAllowedAreas( Qt::BottomDockWidgetArea );
				addDockWidget( Qt::BottomDockWidgetArea, dock );

				return dock->toggleViewAction();
			}

			QAction* MainWindowQT::CreateLoggerDock()
			{
				QDockWidget* dock = new QDockWidget( tr( "Logger" ) );
				dock->setObjectName( "LoggerDock" );

				LoggerViewQT* logWidget = new LoggerViewQT();
				logWidget->Register();

				logWidget->setObjectName( "LoggerWidget" );
				//feWidget->SetDirectory("C:/Users/");

				dock->setWidget( logWidget );
				dock->setAllowedAreas( Qt::BottomDockWidgetArea );
				addDockWidget( Qt::BottomDockWidgetArea, dock );

				return dock->toggleViewAction();
			}

			QAction* MainWindowQT::CreateInspectorDock()
			{
				QDockWidget* dock = new QDockWidget( tr( "Inspector" ) );
				dock->setObjectName( "InspectorDock" );

				InspectorQT* inspectorWidget = new InspectorQT();
				inspectorWidget->setObjectName( "InspectorWidget" );
				inspectorWidget->Register();

				dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
				dock->setWidget( inspectorWidget );
				addDockWidget( Qt::RightDockWidgetArea, dock );

				return dock->toggleViewAction();
			}

			QAction* MainWindowQT::CreateSceneTreeDock()
			{
				QDockWidget* dock = new QDockWidget( tr( "SceneTree" ) );
				dock->setObjectName( "SceneDock" );

				SceneTreeQT* sceneTreeWidget = new SceneTreeQT();
				sceneTreeWidget->Register();
				sceneTreeWidget->setObjectName( "SceneWidget" );

				dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
				dock->setWidget( sceneTreeWidget );
				addDockWidget( Qt::LeftDockWidgetArea, dock );

				return dock->toggleViewAction();
			}

			QAction* MainWindowQT::CreateDebugWidgetTree()
			{
				QDockWidget* dock = new QDockWidget( tr( "QT Tree" ) );
				dock->setObjectName( "QT Tree" );

				dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
				dock->setWidget( new WidgetTree( this ) );
				addDockWidget( Qt::LeftDockWidgetArea, dock );

				return dock->toggleViewAction();
			}

#pragma endregion

			void MainWindowQT::ShowMessage( const QString& str ) const
			{
				statusBar()->showMessage( str );
			}

			void MainWindowQT::closeEvent( QCloseEvent* event )
			{
				if ( EditorCommandManager::CanSave() )
				{
					QMessageBox msgBox;
					msgBox.setText( tr( "The scene has been modified." ) );
					msgBox.setInformativeText( tr( "Do you want to save your changes?" ) );
					msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
					msgBox.setDefaultButton( QMessageBox::Save );

					int ret = msgBox.exec();
					switch ( ret )
					{
						case QMessageBox::Save:
							SaveAllClicked();
							QMainWindow::closeEvent( event );
							break;
						case QMessageBox::Discard:
							QMainWindow::closeEvent( event );
							// Don't Save was clicked
							break;
						case QMessageBox::Cancel:
							// Cancel was clicked
							break;
						default:
							// should never be reached
							break;
					}
				}
			}

			void MainWindowQT::resizeEvent( QResizeEvent* event )
			{
				QMainWindow::resizeEvent( event );
				TCM::Engine::Core::ResizeRenderer( centralWidget()->width(), centralWidget()->height() );
			}

			void MainWindowQT::SetEnableUndo( const bool value )
			{
				if ( undoMenu )
					undoMenu->setEnabled( value );
				if ( undoToolbar )
					undoToolbar->setEnabled( value );
			}

			void MainWindowQT::SetEnableRedo( const bool value )
			{
				if ( redoMenu )
					redoMenu->setEnabled( value );
				if ( redoToolbar )
					redoToolbar->setEnabled( value );
			}

			void MainWindowQT::SaveAllClicked()
			{
				SaveSceneClicked();
				SaveProjectClicked();
			}

#pragma region Layout Management
			void MainWindowQT::SaveLayout()
			{
				QString fileName
						= QFileDialog::getSaveFileName( this, tr( "Save layout" ) );
				if ( fileName.isEmpty() )
					return;
				QFile file( fileName );
				if ( !file.open( QFile::WriteOnly ) )
				{
					QString msg = tr( "Failed to open %1\n%2" )
							.arg( QDir::toNativeSeparators( fileName ), file.errorString() );
					QMessageBox::warning( this, tr( "Error" ), msg );
					return;
				}

				QByteArray geo_data = saveGeometry();
				QByteArray layout_data = saveState();

				bool ok = file.putChar( static_cast<uchar>(geo_data.size()) );
				if ( ok )
					ok = file.write( geo_data ) == geo_data.size();
				if ( ok )
					ok = file.write( layout_data ) == layout_data.size();

				if ( !ok )
				{
					QString msg = tr( "Error writing to %1\n%2" )
							.arg( QDir::toNativeSeparators( fileName ), file.errorString() );
					QMessageBox::warning( this, tr( "Error" ), msg );
					return;
				}
			}

			void MainWindowQT::LoadLayout()
			{
				QString fileName
						= QFileDialog::getOpenFileName( this, tr( "Load layout" ) );
				if ( fileName.isEmpty() )
					return;
				QFile file( fileName );
				if ( !file.open( QFile::ReadOnly ) )
				{
					QString msg = tr( "Failed to open %1\n%2" )
							.arg( QDir::toNativeSeparators( fileName ), file.errorString() );
					QMessageBox::warning( this, tr( "Error" ), msg );
					return;
				}

				uchar geo_size;
				QByteArray geo_data;
				QByteArray layout_data;

				bool ok = file.getChar( reinterpret_cast<char*>(&geo_size) );
				if ( ok )
				{
					geo_data = file.read( geo_size );
					ok = geo_data.size() == geo_size;
				}
				if ( ok )
				{
					layout_data = file.readAll();
					ok = layout_data.size() > 0;
				}

				if ( ok )
					ok = restoreGeometry( geo_data );
				if ( ok )
					ok = restoreState( layout_data );

				if ( !ok )
				{
					QString msg = tr( "Error reading %1" ).arg( QDir::toNativeSeparators( fileName ) );
					QMessageBox::warning( this, tr( "Error" ), msg );
					return;
				}
			}

#pragma endregion

#pragma region Menu Callback

			void MainWindowQT::NewProjectClicked()
			{
				ShowMessage( "New Project Clicked" );

				CoreNewProject();

				QSettings settings;
			}

			void MainWindowQT::SaveProjectClicked()
			{
				ShowMessage( "Save Project Clicked" );
				if ( !CoreSaveProject() )
					SaveProjectAsClicked();
				else
				{
					QSettings settings;
					settings.setValue( LAST_PROJECT_KEY, QString::fromStdString( GetProjectFilepath() ) );
				}
			}

			void MainWindowQT::SaveProjectAsClicked()
			{
				ShowMessage( "Save Project Clicked" );

				QString fileName
						= QFileDialog::getSaveFileName( this, tr( "Save project" ), QString(), tr( "TCM Project" ) + " (*.tcmproj)" );
				if ( fileName.isEmpty() )
					return;
				if ( !CoreSaveProject( fileName.toStdString() ) )
				{
					QMessageBox msgBox;
					msgBox.setText( tr( "Cannot save the project..." ) );
					msgBox.setIcon( QMessageBox::Critical );
					msgBox.exec();
				}
				else
				{
					QSettings settings;
					settings.setValue( LAST_PROJECT_KEY, QString::fromStdString( GetProjectFilepath() ) );
				}
			}

			void MainWindowQT::LoadProjectClicked()
			{
				ShowMessage( "Load Project Clicked" );
				QString fileName
						= QFileDialog::getOpenFileName( this, tr( "Load project" ), QString(), tr( "TCM Project" ) + " (*.tcmproj)" );
				if ( fileName.isEmpty() )
					return;
				if ( CoreLoadProject( fileName.toStdString() ) )
				{
					QMessageBox msgBox;
					msgBox.setText( tr( "Cannot load the project..." ) );
					msgBox.setIcon( QMessageBox::Critical );
					msgBox.exec();
				}
				else
				{
					QSettings settings;
					settings.setValue( LAST_PROJECT_KEY, QString::fromStdString( GetProjectFilepath() ) );
				}
			}

			void MainWindowQT::NewSceneClicked()
			{
				ShowMessage( "New Scene Clicked" );
				CoreNewScene();
			}

			void MainWindowQT::SaveSceneClicked()
			{
				ShowMessage( "Save Scene Clicked" );
				if ( !CoreSaveScene() )
					SaveSceneAsClicked();
				else
				{
					QSettings settings;
					settings.setValue( LAST_SCENE_KEY, QString::fromStdString( GetSceneFilepath() ) );
					EditorCommandManager::Save();
				}
			}

			void MainWindowQT::SaveSceneAsClicked()
			{
				ShowMessage( "Save Scene Clicked" );
				QString fileName
						= QFileDialog::getSaveFileName( this, tr( "Save scene" ), QString(), tr( "TCM Scene" ) + " (*.tcmscene)" );
				if ( fileName.isEmpty() )
					return;
				if ( !CoreSaveScene( fileName.toStdString() ) )
				{
					QMessageBox msgBox;
					msgBox.setText( tr( "Cannot save the scene..." ) );
					msgBox.setIcon( QMessageBox::Critical );
					msgBox.exec();
				}
				else
				{
					QSettings settings;
					settings.setValue( LAST_SCENE_KEY, QString::fromStdString( GetSceneFilepath() ) );
				}
			}

			void MainWindowQT::LoadSceneClicked()
			{
				ShowMessage( "Load Scene Clicked" );
				QString fileName
						= QFileDialog::getOpenFileName( this, tr( "Load scene" ), QString(), tr( "TCM Project" ) + " (*.tcmscene)" );
				if ( fileName.isEmpty() )
					return;
				if ( !CoreLoadScene( fileName.toStdString() ) )
				{
					QMessageBox msgBox;
					msgBox.setText( tr( "Cannot load the scene..." ) );
					msgBox.setIcon( QMessageBox::Critical );
					msgBox.exec();
				}
				else
				{
					QSettings settings;
					settings.setValue( LAST_SCENE_KEY, QString::fromStdString( GetSceneFilepath() ) );
				}
			}

			void MainWindowQT::BuildClicked()
			{
				ShowMessage( "Build Clicked" );
			}

			void MainWindowQT::BuildSettingsClicked()
			{
				ShowMessage( "Build Settings Clicked" );
			}

			void MainWindowQT::SettingsClicked()
			{
				ShowMessage( "Build Settings Clicked" );
			}

			void MainWindowQT::UndoClicked()
			{
				ShowMessage( "Undo Clicked" );
				EditorCommandManager::Undo();
			}

			void MainWindowQT::RedoClicked()
			{
				ShowMessage( "Redo Clicked" );
				EditorCommandManager::Redo();
			}

			void MainWindowQT::CopyClicked()
			{
				ShowMessage( "Copy Clicked" );
			}

			void MainWindowQT::CutClicked()
			{
				ShowMessage( "Cut Clicked" );
			}

			void MainWindowQT::PasteClicked()
			{
				ShowMessage( "Paste Clicked" );
			}

			void MainWindowQT::FileExplorerClicked()
			{
				ShowMessage( "FileExplorer Clicked" );
			}

			void MainWindowQT::InspectorClicked()
			{
				ShowMessage( "Inspector Clicked" );
			}

			void MainWindowQT::SceneTreeClicked()
			{
				ShowMessage( "Scene Tree Clicked" );
			}

			void MainWindowQT::ProjectSettingsClicked()
			{
				ProjectEditorQT* window = new ProjectEditorQT( GetProject(), this );
				window->setAttribute( Qt::WA_DeleteOnClose );
				window->exec();
				Engine::Core::RefreshProjectSettings();
			}

			void MainWindowQT::SkyboxEditorClicked()
			{
				SkyboxEditorQT* window = new SkyboxEditorQT( this );
				window->setAttribute( Qt::WA_DeleteOnClose );
				window->exec();
			}

			void MainWindowQT::DocClicked()
			{
				std::string docPath = GetDocPath();
				QDesktopServices::openUrl( QUrl( QString::fromStdString( "file:///" + docPath ) ) );
			}

			void MainWindowQT::AboutClicked()
			{
				QMessageBox::about( this, tr( "About" ) + "TayCaMead Engine",
				                    tr( "Made by Bastien Leduc, Guillaume Newton and Nicolas Fontes, students in Game Programming 2nd year, inside Isart DIGITAL, Paris, FRANCE, 2017." ) );
			}

#pragma endregion

#pragma region Toolbar Callback

			void MainWindowQT::MoveClicked()
			{
				ShowMessage( "Move Clicked" );
			}

			void MainWindowQT::TranslateClicked()
			{
				ShowMessage( "Translate Clicked" );
			}

			void MainWindowQT::RotateClicked()
			{
				ShowMessage( "Rotate Clicked" );
			}

			void MainWindowQT::ScaleClicked()
			{
				ShowMessage( "Scale Clicked" );
			}

			void MainWindowQT::PauseClicked()
			{
				ShowMessage( "Pause Clicked" );

				if ( !pauseAction->isChecked() )
					GetPlayEvent().Invoke();
				else
					GetPauseEvent().Invoke();
			}

			void MainWindowQT::PlayClicked()
			{
				ShowMessage( "Play Clicked" );

				if ( !playAction->isChecked() )
					GetStopEvent().Invoke();
				else
					GetPlayEvent().Invoke();
			}

			void MainWindowQT::NextFrameClicked()
			{
				ShowMessage( "Next Frame Clicked" );
				GetNextFrameEvent().Invoke();
			}

			void MainWindowQT::CoreOnPlayEvent()
			{
				pauseAction->setChecked( false );
				playAction->setChecked( true );
				nextAction->setChecked( false );
			}

			void MainWindowQT::OnPauseEvent()
			{
				pauseAction->setChecked( true );
				playAction->setChecked( true );
				nextAction->setChecked( false );
			}

			void MainWindowQT::CoreOnStopEvent()
			{
				pauseAction->setChecked( false );
				playAction->setChecked( false );
				nextAction->setChecked( false );
			}

			void MainWindowQT::OnNextFrameEvent()
			{
				pauseAction->setChecked( true );
				playAction->setChecked( true );
				nextAction->setChecked( false );
			}

			void MainWindowQT::QRefreshCore()
			{
				RefreshCore();
				/*if ( centralWidget()->hasFocus() )
						int i = 0;*/
			}

			void MainWindowQT::showEvent( QShowEvent* event )
			{
				connect( &timer, SIGNAL( timeout() ), this, SLOT( QRefreshCore() ) );
				timer.start();
			}

			void MainWindowQT::FocusClicked()
			{
				Engine::UserIO::InputManager::SetFocus( true );
			}

			void MainWindowQT::UnfocusClicked()
			{
				Engine::UserIO::InputManager::SetFocus( false );
			}

			void MainWindowQT::OvniClicked()
			{
				bool value = qobject_cast<QAction*>( sender() )->isChecked();
				GetScene()->SwitchOvniMode( value );
			}

#pragma endregion
		}
	}
}
