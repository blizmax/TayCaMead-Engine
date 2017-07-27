#pragma once
#include <QWidget>
#include "Inspector.h"

QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QComboBox )
QT_FORWARD_DECLARE_CLASS( QToolBox )

namespace TCM
{
	namespace Editor
	{
		class ComponentInsertion;

		namespace QT
		{
			class InspectorQT : public QWidget, public Inspector
			{
				Q_OBJECT

			public:
				InspectorQT( QWidget* parent = nullptr );
				~InspectorQT();

				ComponentInspector* CreateComponentInspector( Engine::TCMComponent* component ) override;

			private:

				QToolBox* m_inspectorToolbox;
				ComponentInsertion* m_creationMenu;

			protected:
				void HideUI() override;
				void ShowUI() override;

				void EnableUI() override;
				void DisableUI() override;

				void UIRemoveComponent( int index ) override;
				void UIRemoveAllComponents() override;
			};
		}
	}
}
