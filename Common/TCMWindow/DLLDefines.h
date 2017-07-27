#pragma once
# if defined(TCMWINDOW_LIB)
#  define TCMWINDOW_EXPORT __declspec(dllexport)
#	pragma comment (lib, "TCMRenderer/TCMRenderer.lib")
# else
#  define TCMWINDOW_EXPORT __declspec(dllimport)
# endif
