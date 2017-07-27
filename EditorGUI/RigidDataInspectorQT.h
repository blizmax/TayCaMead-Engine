#pragma once

#include <QObject>
#include "VarInspectorQT.h"
#include <qcheckbox.h>

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class FloatLineEdit;

			class RigidDataInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				RigidDataInspectorQT( SerializedInterface* var );

				void UpdateFromValue() override;

				bool UseAllColumns() override;

			protected slots:

				void OnMassChanged( const float& value );
				void OnFrictionChanged( const float& value );
				void OnRestitutionChanged( const float& value );
				void OnUseGravyChanged( bool state );
				void OnIsKinematicChanged( bool state );
			private:
				FloatLineEdit* massField;
				FloatLineEdit* friction;
				FloatLineEdit* restitution;
				QCheckBox* useGravity;
				QCheckBox* isKinematic;
			};
		}
	}
}
