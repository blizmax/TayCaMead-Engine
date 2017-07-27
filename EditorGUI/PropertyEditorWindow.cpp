#include "PropertyEditorWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>

PropertyEditorWindow::PropertyEditorWindow( QString name, QString path, QWidget* parent )
	: QDialog( parent )
{
	m_Path = path;
	m_Name = name;

	QVBoxLayout* mainLayout = new QVBoxLayout( this );

	m_ListLayout = new QVBoxLayout( this );

	QHBoxLayout* btnLayout = new QHBoxLayout( this );

	QPushButton* addBtn = new QPushButton( tr( "Add" ), this );
	connect( addBtn, SIGNAL(clicked()), this, SLOT(AddProperty()) );

	QPushButton* closeBtn = new QPushButton( tr( "Close" ), this );
	connect( closeBtn, SIGNAL(clicked()), this, SLOT(close()) );

	btnLayout->addWidget( addBtn );
	btnLayout->addWidget( closeBtn );

	mainLayout->addItem( m_ListLayout );
	mainLayout->addItem( btnLayout );

	setLayout( mainLayout );

	setWindowTitle( name + " Manager" );

	LoadProperties();

	if ( m_FieldList.count() == 0 )
		AddProperty();
}

PropertyEditorWindow::~PropertyEditorWindow()
{
}

void PropertyEditorWindow::AddProperty( const QString str )
{
	if ( !m_ListLayout )
		return;

	QHBoxLayout* tagLayout = new QHBoxLayout();
	tagLayout->setSpacing( 5 );

	int index = m_FieldList.count();

	QLineEdit* field = new QLineEdit( this );
	field->setPlaceholderText( m_Name );
	field->setText( str );
	field->show();

	QLabel* label = new QLabel( m_Name + " " + QString::number( index ), this );
	tagLayout->addWidget( label );
	tagLayout->addWidget( field );

	label->show();

	QSignalMapper mapper;

	QPushButton* removeBtn = new QPushButton( "-", this );
	connect( removeBtn, SIGNAL(clicked()), &mapper, SLOT(map()) );
	mapper.setMapping( removeBtn, tagLayout );
	connect( &mapper, SIGNAL(mapped(QObject*)), this, SLOT(RemoveProperty(QObject*)) );
	removeBtn->show();

	tagLayout->addWidget( removeBtn );

	m_ListLayout->addItem( tagLayout );

	m_FieldList.push_back( field );
}

void PropertyEditorWindow::RemoveProperty( QObject* container )
{
	if ( !m_ListLayout )
		return;

	if ( !container )
		return;

	m_ListLayout->removeItem( reinterpret_cast<QLayoutItem*>(container) );
}

void PropertyEditorWindow::closeEvent( QCloseEvent* event )
{
	SaveProperties();
	QDialog::closeEvent( event );
}

void PropertyEditorWindow::LoadProperties()
{
	/*QStringList list = SaveManager::FileToStringArray(m_Path);

	for (int i = 0; i < list.count(); i++)
			AddProperty(list[i]);*/
}

void PropertyEditorWindow::SaveProperties()
{
	QStringList list;

	for ( int i = 0; i < m_FieldList.count(); i++ )
		if ( m_FieldList[i] && m_FieldList[i]->text().count() > 0 )
			list << m_FieldList[i]->text();

	//SaveManager::StringArrayToFile(list, m_Path);
}
