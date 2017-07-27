#pragma once

#include <QDialog>

namespace TCM
{
	namespace Engine
	{
		class Project;
	}
}

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class ProjectEditorQT : public QDialog
			{
				Q_OBJECT

			public:
				ProjectEditorQT( Engine::Project* project, QWidget* parent = nullptr );
			};
		}
	}
}
