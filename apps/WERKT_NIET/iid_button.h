#pragma once
#include "id_buttonlistener.h"

namespace crt
{
	class ID_ButtonListener;

	class IID_Button
	{
	public:
		virtual void addButtonListener(ID_ButtonListener* pButtonListener) = 0;
		virtual bool isPressed() = 0;
		virtual void setButtonID(int n) = 0;
		virtual int getButtonID() = 0;
	};
};