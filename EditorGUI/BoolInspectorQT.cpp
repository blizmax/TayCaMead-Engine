#include "BoolInspectorQT.h"
#include <QCheckBox>
#include "TCMMemory/TCMSerialization.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			BoolInspectorQT::BoolInspectorQT( SerializedInterface* var ) :
			                                                             VarInspectorQT( var )
			{
				m_widget = new QCheckBox;

				SetReadOnly( m_widget, var->GetEditorType() == SerializedInterface::READONLY );

				QObject::connect( m_widget, SIGNAL( clicked( bool )), this, SLOT(OnStateChanged( bool )) );
			}

			void BoolInspectorQT::UpdateFromValue()
			{
				qobject_cast<QCheckBox*>( m_widget )->setChecked( VarHelper<bool>::Get( m_var ) );
			}

			void BoolInspectorQT::OnStateChanged( bool state ) const
			{
				VarHelper<bool>::Set( m_var, state );
			}
		}
	}
}
