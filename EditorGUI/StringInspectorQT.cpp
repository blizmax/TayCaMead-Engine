#include "StringInspectorQT.h"
#include <QLineEdit>
#include "TCMMemory/TCMSerialization.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			StringInspectorQT::StringInspectorQT( SerializedInterface* var ) :
			                                                                 VarInspectorQT( var )
			{
				QLineEdit* lineEdit = new QLineEdit;
				connect( lineEdit, SIGNAL( editingFinished()), this, SLOT(OnTextChanged()) );
				lineEdit->setPlaceholderText( tr( "String" ) );

				SetReadOnly( lineEdit, var->GetEditorType() == SerializedInterface::READONLY );

				m_widget = lineEdit;
			}

			void StringInspectorQT::UpdateFromValue()
			{
				qobject_cast<QLineEdit*>( m_widget )->setText( QString::fromStdString( VarHelper<std::string>::Get( m_var ) ) );
			}

			void StringInspectorQT::OnTextChanged() const
			{
				QLineEdit* line = qobject_cast<QLineEdit*>( m_widget );
				VarHelper<std::string>::Set( m_var, line->text().toStdString() );
			}
		}
	}
}
