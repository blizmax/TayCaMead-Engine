#pragma once
#include "Collider.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			class BulletBoxCollider : public BoxCollider
			{
			public:
				virtual void Reset() override;
			};

			class BulletSphereCollider : public SphereCollider
			{
			public:
				virtual void Reset() override;
			};

			class BulletPlaneCollider : public PlaneCollider
			{
			public:
				virtual void Reset() override;
			};

			class BulletCylinderCollider : public CylinderCollider
			{
			public:
				virtual void Reset() override;
			};

			class BulletCapsuleCollider : public CapsuleCollider
			{
			public:
				virtual void Reset() override;
			};
		}
	}
}
