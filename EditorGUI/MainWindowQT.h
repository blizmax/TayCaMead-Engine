#pragma once
#include "MainWindow.h"

#include <QtWidgets/QMainWindow>
#include <QTimer>

#define PROJECT_EXTENSION_QT QString("tcmproj")
#define SCENE_EXTENSION_QT QString("tcmscene")
#define SCENE_OBJ_EXTENSION_QT QString("tcmscobj")

#define LAST_SCENE_KEY "LastScene"
#define LAST_PROJECT_KEY "LastProject"

QT_FORWARD_DECLARE_CLASS( QAction )
QT_FORWARD_DECLARE_CLASS( QStatusBar )
QT_FORWARD_DECLARE_CLASS( QFile )
QT_FORWARD_DECLARE_CLASS( QTreeWidget )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class MainWindowQT : public QMainWindow, public MainWindow
			{
				Q_OBJECT

			public:
				MainWindowQT( QWidget* parent = Q_NULLPTR );

				void SetupStatusBar();
				void SetupMenuBar();
				void SaveLayout();
				void LoadLayout();
				void SetupToolbar();

				QAction* CreateProjectExplorerDock();
				QAction* CreateLoggerDock();
				QAction* CreateInspectorDock();
				QAction* CreateSceneTreeDock();
				QAction* CreateDebugWidgetTree();

				void ShowMessage( const QString& str ) const;

			protected :
				void closeEvent( QCloseEvent* event ) override;
				void resizeEvent( QResizeEvent* event ) override;

				void SetEnableUndo( const bool value ) override;
				void SetEnableRedo( const bool value ) override;

			private slots:
				void SaveAllClicked();

				void NewProjectClicked();
				void SaveProjectClicked();
				void SaveProjectAsClicked();
				void LoadProjectClicked();

				void NewSceneClicked();
				void SaveSceneClicked();
				void SaveSceneAsClicked();
				void LoadSceneClicked();

				void BuildClicked();
				void BuildSettingsClicked();

				void SettingsClicked();

				void UndoClicked();
				void RedoClicked();

				void CopyClicked();
				void CutClicked();
				void PasteClicked();

				void FileExplorerClicked();
				void InspectorClicked();
				void SceneTreeClicked();
				void ProjectSettingsClicked();
				void SkyboxEditorClicked();

				void DocClicked();
				void AboutClicked();

				void MoveClicked();
				void TranslateClicked();
				void RotateClicked();
				void ScaleClicked();

				void PauseClicked();
				void PlayClicked();
				void NextFrameClicked();

				void QRefreshCore();

				void showEvent( QShowEvent* event ) override;

				void FocusClicked();
				void UnfocusClicked();
				void OvniClicked();

			private:

				void CoreOnPlayEvent() override;
				void OnPauseEvent() override;
				void CoreOnStopEvent() override;
				void OnNextFrameEvent() override;

				QTimer timer;
				QAction* pauseAction = nullptr;
				QAction* playAction = nullptr;
				QAction* nextAction = nullptr;

				QAction* undoMenu = nullptr;
				QAction* undoToolbar = nullptr;
				QAction* redoMenu = nullptr;
				QAction* redoToolbar = nullptr;

				QAction* ovniAction = nullptr;
			};
		}
	}
}
