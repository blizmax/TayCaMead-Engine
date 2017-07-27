#include "IntInspectorQT.h"
#include <qvalidator.h>
#include "IntLineEdit.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			IntInspectorQT::IntInspectorQT( SerializedInterface* var ) :
			                                                           VarInspectorQT( var )
			{
				IntLineEdit* line = new IntLineEdit;
				connect( line, SIGNAL( OnValueChanged(const int )), this, SLOT( OnValueChanged(const int)) );

				SetReadOnly( line, var->GetEditorType() == SerializedInterface::READONLY );

				m_widget = line;
			}

			void IntInspectorQT::UpdateFromValue()
			{
				qobject_cast<IntLineEdit*>( m_widget )->SetValue( VarHelper<int>::Get( m_var ) );
			}

			void IntInspectorQT::OnValueChanged( const int value )
			{
				VarHelper<int>::Set( m_var, value );
			}
		}
	}
}
