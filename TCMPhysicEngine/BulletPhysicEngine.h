#pragma once
#include "PhysicEngine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Physics
		{
			class BulletPhysicEngine : PhysicEngine
			{
			public:
				BulletPhysicEngine();
				~BulletPhysicEngine();

			protected:
				virtual void CoreInit() override;
				virtual void CoreShutdown() override;
				virtual bool CoreRegisterRigidbody( Rigidbody* rigidbody ) override;
				virtual bool CoreUnregisterRigidbody( Rigidbody* rigidbody ) override;
				virtual void CoreSetGravity( const Math::Vec3& gravity ) override;
				virtual void CoreFixedUpdate( const float fixedDeltaTime ) override;
				virtual RaycastHit CoreRaycast( const Ray& ray ) const override;
			};

		}
	}
}
