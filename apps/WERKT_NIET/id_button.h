#pragma once
#include "crt_CleanRTOS.h"
#include "crt_IHandlerListener.h"
#include "crt_IHandler.h"
#include "crt_ILogger.h"

#include "id_buttonlistener.h"
#include "iid_button.h"

// crt_Button met id ipv name

namespace crt
{
    template< unsigned int MAXBUTTONLISTENERCOUNT>
    class ID_Button : public IHandlerListener, public IID_Button{
	private:
		int buttonID;
		uint8_t pinButton;
		bool bPositiveLogic;	// If true, 1 means pressed, else 0 means pressed.
		bool bPrevPressed;

		ID_ButtonListener* arButtonListeners[MAXBUTTONLISTENERCOUNT] = {};
		uint16_t nofButtonListeners;

	public:
		ID_Button(int buttonID, uint8_t pinButton, bool bPositiveLogic, IHandler& buttonHandler) :
			buttonID(buttonID), pinButton(pinButton), bPositiveLogic(bPositiveLogic), bPrevPressed(false),
            nofButtonListeners(0)
			// iets met de str zoals in crt_Button?
		{
			for (int i = 0;i < MAXBUTTONLISTENERCOUNT;i++)
			{
				arButtonListeners[i] = nullptr;
			}
			buttonHandler.addHandlerListener(this);

			gpio_pad_select_gpio(pinButton);
			gpio_set_direction((gpio_num_t)pinButton, GPIO_MODE_INPUT);
		}

		/*override keyword not supported in current compiler*/
		void addButtonListener(ID_ButtonListener* pButtonListener)
		{
			if (!isAlreadyPresent(pButtonListener))
			{
				assert(nofButtonListeners < MAXBUTTONLISTENERCOUNT);
				arButtonListeners[nofButtonListeners++] = pButtonListener;
			}
		}

		bool isPressed()
		{
			return bPrevPressed;
		}

		void setButtonID(int n)
		{
            buttonID = n;
		}

		int getButtonID()
		{
			return buttonID;
		}

		// It is assumed that this update function is called by a ButtonHandler once in about 70ms: slow enough for debounce, fast enough for response.
		/*override keyword not supported in current compiler*/
		void update()
		{
			bool buttonRead = (gpio_get_level((gpio_num_t)pinButton) != 0);

			bool bPressed = (buttonRead == bPositiveLogic); // TODO: don't use digitalRead but use ESP specific code instead (to allow build with ESP_IDF without Arduino IDE).
			if (bPressed && !bPrevPressed)
			{
				for (int i = 0; i < nofButtonListeners; i++)
				{
					arButtonListeners[i]->buttonPressed(buttonID);
				}
			}
			else if (!bPressed && bPrevPressed)
			{
				for (int i = 0; i < nofButtonListeners; i++)
				{
					arButtonListeners[i]->buttonReleased();
				}
			}
			bPrevPressed = bPressed;
		}

	private:
		bool isAlreadyPresent(ID_ButtonListener* pButtonListener)
		{
			for (int i = 0; i < nofButtonListeners; i++)
			{
				//ESP_LOGI("adasdffasfsadf", "%s", i);
				if (arButtonListeners[i] == pButtonListener)
				{
					return true;
				}
			}
			return false;
		}
	};
};
