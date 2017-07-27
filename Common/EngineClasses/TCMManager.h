#pragma once

#include "TCMTicking.h"

namespace TCM
{
	namespace Engine
	{
		/**
		 * \brief Base class for every Manager
		 */
		class ENGINECLASSES_DLL_EXPORT TCMManager : public TCMTicking
		{
			TCM_CLASS(TCMManager, TCMTicking);

			TCMManager();
			virtual ~TCMManager();

			/**
			 * \brief TCM function to manage updates
			 * \param deltaTime _IN_ Deltatime since last call
			 * \return True if can update, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			bool TCMUpdate( const float deltaTime ) override final;

		protected:
			/**
			 * \brief Called when destroying the instance
			 */
			void OnDestroy() override;
		};
	}
}
