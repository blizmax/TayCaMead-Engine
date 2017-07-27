#include "VarInspectorQT.h"
#include <QLineEdit>

QWidget* TCM::Editor::QT::VarInspectorQT::GetQWidget() const
{
	return m_widget;
}

void TCM::Editor::QT::VarInspectorQT::SetReadOnly( QWidget* widget, bool readOnly ) const
{
	widget->setAttribute( Qt::WA_TransparentForMouseEvents, readOnly );
	widget->setFocusPolicy( readOnly ? Qt::NoFocus : Qt::StrongFocus );
}

void TCM::Editor::QT::VarInspectorQT::SetReadOnly( QLineEdit* lineEdit, bool readOnly ) const
{
	lineEdit->setReadOnly( readOnly );
}
