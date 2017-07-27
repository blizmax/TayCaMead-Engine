#include "ProjectEditorQT.h"
#include "BaseInspectorQT.h"
#include "EngineClasses/Project.h"

#include <QFormLayout>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			ProjectEditorQT::ProjectEditorQT( Engine::Project* project, QWidget* parent )
				: QDialog( parent )
			{
				if ( !project )
					close();
				BaseInspectorQT* baseInspector = new BaseInspectorQT( project );
				setLayout( baseInspector->GetLayout() );
			}
		}
	}
}
