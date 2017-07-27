#pragma once
#include "DLLDefine.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			/**
			 * \brief Class which contains the window specific for each platform
			 */
			class NativeWindow abstract
			{
			public:
				virtual ~NativeWindow() = default;

				/**
				 * \brief Init the native window
				 */
				static TCMNATIVEWINDOW_DLL void Init();
				/**
				 * \brief Get the current window
				 * \return Data of the current window, specific for each plateform
				 */
				static TCMNATIVEWINDOW_DLL void* GetWindow();

				/**
				 * \brief Check if window is focused
				 */
				static TCMNATIVEWINDOW_DLL bool IsActive();

			protected:

				static NativeWindow& GetInstance();
				virtual void CoreInit() = 0;

				virtual bool CoreIsActive() = 0;

				void* m_whd = nullptr;
			};
		}
	}
}
