#pragma once

#include "TCMComponent.h"

namespace TCM
{
	namespace Engine
	{
		// * Base class for every user custom component
		class ENGINECLASSES_DLL_EXPORT TCMComponentBehaviour abstract : public TCMComponent
		{
			TCM_CLASS(TCMComponent);

		public:
			TCMComponentBehaviour();
			virtual ~TCMComponentBehaviour();

			void TCMEditorUpdate( const float deltaTime ) override final;
			void TCMPoolCreate() override final;
			void TCMPoolRelease() override final;
			void Init() override final;
			bool TCMUpdate( const float deltaTime ) override final;
			bool TCMFixedUpdate( const float fixedDeltaTime ) override final;

		protected:
			/**
			 * \brief Called at the creattion of the component
			 */
			virtual void Awake();
			/**
			 * \brief Called at the initialisation of the component
			 */
			virtual void Start();
			/**
			 * \brief Called each frame
			 * \param deltaTime _IN_ Time of a frame
			 */
			void Update( const float deltaTime ) override;
			/**
			 * \brief Called each physic loop
			 * \param fixedDeltaTime _IN_ Time of a physic loop
			 */
			void FixedUpdate( const float fixedDeltaTime ) override;
		};
	}
}
