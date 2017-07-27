#pragma once

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QLayoutItem)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

class PropertyEditorWindow : public QDialog
{
	Q_OBJECT

public:
	PropertyEditorWindow( QString name, QString path, QWidget* parent = nullptr );
	~PropertyEditorWindow();

public slots:
	void AddProperty( const QString str = "" );
	void RemoveProperty( QObject* );

	void closeEvent( QCloseEvent* event ) override;

private :
	void LoadProperties();
	void SaveProperties();

	QVBoxLayout* m_ListLayout = nullptr;
	QVector<QLineEdit *> m_FieldList;

	QString m_Name;
	QString m_Path;
};
