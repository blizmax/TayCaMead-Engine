#include "FloatInspectorQT.h"
#include <qvalidator.h>
#include "FloatLineEdit.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			FloatInspectorQT::FloatInspectorQT( SerializedInterface* var ) :
			                                                               VarInspectorQT( var )
			{
				FloatLineEdit* line = new FloatLineEdit;
				connect( line, SIGNAL( OnValueChanged(const float &)), this, SLOT( OnValueChanged(const float &)) );
				SetReadOnly( line, var->GetEditorType() == SerializedInterface::READONLY );
				m_widget = line;
			}

			void FloatInspectorQT::UpdateFromValue()
			{
				qobject_cast<FloatLineEdit*>( m_widget )->SetValue( VarHelper<float>::Get( m_var ) );
			}

			void FloatInspectorQT::OnValueChanged( const float& value )
			{
				VarHelper<float>::Set( m_var, value );
			}
		}
	}
}
