#include "RigidDataInspectorQT.h"

#include <QGroupBox>
#include <QFormLayout>
#include <QValidator>
#include <QLineEdit>
#include "FloatLineEdit.h"
#include <QCheckBox>
#include "VarHelper.h"
#include "EngineClasses/TCMComponentRigidBody.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			RigidDataInspectorQT::RigidDataInspectorQT( SerializedInterface* var ) :
			                                                                       VarInspectorQT( var )
			{
				m_widget = new QGroupBox( tr( "RigidBody Data" ) );
				QFormLayout* formLayout = new QFormLayout;

				massField = new FloatLineEdit;
				connect( massField, SIGNAL( OnValueChanged ( const float& ) ), this, SLOT( OnMassChanged( const float& ) ) );

				friction = new FloatLineEdit;
				connect( friction, SIGNAL( OnValueChanged( const float& ) ), this, SLOT( OnFrictionChanged( const float& ) ) );

				restitution = new FloatLineEdit;
				connect( restitution, SIGNAL( OnValueChanged( const float& ) ), this, SLOT( OnRestitutionChanged( const float& ) ) );

				useGravity = new QCheckBox;
				connect( useGravity, SIGNAL( clicked( bool ) ), this, SLOT( OnUseGravyChanged( bool ) ) );

				isKinematic = new QCheckBox;
				connect( isKinematic, SIGNAL( clicked( bool ) ), this, SLOT( OnIsKinematicChanged( bool ) ) );

				formLayout->addRow( tr( "Mass" ), massField );
				formLayout->addRow( tr( "Friction" ), friction );
				formLayout->addRow( tr( "Restitution" ), restitution );
				formLayout->addRow( tr( "Use Gravity" ), useGravity );
				formLayout->addRow( tr( "Is Kinematic" ), isKinematic );

				m_widget->setLayout( formLayout );
			}

			void RigidDataInspectorQT::UpdateFromValue()
			{
				Engine::Physics::RigidBodyData rigidBodyData = VarHelper<Engine::Physics::RigidBodyData>::Get( m_var );

				massField->SetValue( rigidBodyData.m_mass );
				friction->SetValue( rigidBodyData.m_friction );
				restitution->SetValue( rigidBodyData.m_restitution );

				useGravity->setChecked( rigidBodyData.m_useGravity );
				isKinematic->setChecked( rigidBodyData.m_isKinematic );
			}

			bool RigidDataInspectorQT::UseAllColumns()
			{
				return true;
			}

			void RigidDataInspectorQT::OnMassChanged( const float& value )
			{
				auto result = VarHelper<Engine::Physics::RigidBodyData>::Get( m_var );
				result.m_mass = value;
				VarHelper<Engine::Physics::RigidBodyData>::Set( m_var, result );
			}

			void RigidDataInspectorQT::OnFrictionChanged( const float& value )
			{
				auto result = VarHelper<Engine::Physics::RigidBodyData>::Get( m_var );
				result.m_friction = value;
				VarHelper<Engine::Physics::RigidBodyData>::Set( m_var, result );
			}

			void RigidDataInspectorQT::OnRestitutionChanged( const float& value )
			{
				auto result = VarHelper<Engine::Physics::RigidBodyData>::Get( m_var );
				result.m_restitution = value;
				VarHelper<Engine::Physics::RigidBodyData>::Set( m_var, result );
			}

			void RigidDataInspectorQT::OnUseGravyChanged( bool state )
			{
				auto result = VarHelper<Engine::Physics::RigidBodyData>::Get( m_var );
				result.m_useGravity = state;
				VarHelper<Engine::Physics::RigidBodyData>::Set( m_var, result );
			}

			void RigidDataInspectorQT::OnIsKinematicChanged( bool state )
			{
				auto result = VarHelper<Engine::Physics::RigidBodyData>::Get( m_var );
				result.m_isKinematic = state;
				VarHelper<Engine::Physics::RigidBodyData>::Set( m_var, result );
			}
		}
	}
}
