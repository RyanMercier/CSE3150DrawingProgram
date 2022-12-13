#ifndef OBSERVERS_H
#define OBSERVERS_H

#include "ECObserver.h"
#include "ECGraphicViewImp.h"
#include "GraphicDocument.h"
#include "GraphicController.h"
#include <iostream>

class TimerObserver : public ECObserver
{
public:
	TimerObserver(ECGraphicViewImp* p_view, GraphicController* p_controller, GraphicDocument *p_doc)
	{
		view = p_view;
		controller = p_controller;
		doc = p_doc;
		std::cout << "Timer Created" << std::endl;
	}

	virtual void Update()
	{
		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_TIMER)
		{
			doc->Draw();
			view->SetRedraw(true);
		}
	}

private:
	ECGraphicViewImp* view = nullptr;
	GraphicController* controller = nullptr;
	GraphicDocument* doc = nullptr;
};

class LeftMouseObserver : public ECObserver
{
public:
	LeftMouseObserver(ECGraphicViewImp* p_view, GraphicController* p_controller)
	{
		view = p_view;
		controller = p_controller;
		std::cout << "Mouse Observer Created" << std::endl;
	}

	virtual void Update()
	{
		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_MOUSE_BUTTON_DOWN)
		{
			pressed = true;
			view->GetCursorPosition(startingX, startingY);
			currentX = startingX;
			currentY = startingY;

			if (controller->GetMode() == false && controller->GetSelecting() == true) // if we are in edit mode and ctrl key is held down
			{
				std::cout << "Select" << std::endl;
				controller->Select(startingX, startingY);
			}

			if (controller->GetMode() == false && controller->GetSelecting() == false) // if we are in edit mode and ctrl key is not held down
			{
				for (int i = 0; i < controller->GetSelected().size(); i++)
				{
					controller->GetSelected()[i]->visible = false;
				}
			}
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_TIMER)
		{
			view->GetCursorPosition(currentX, currentY);
			
			if (pressed && controller->GetMode() == true) // mouse button is down and we are in insert mode
			{
				if (controller->GetFillMode() == false) // not filled
				{
					if (controller->GetShapeMode() == true) // rectangle mode
					{
						// draw rectangle gizmo
						view->DrawRectangle(startingX, startingY, currentX, currentY);
						view->SetRedraw(true);
					}

					if (controller->GetShapeMode() == false) // ellipse mode
					{
						// draw ellipse gizmo
						view->DrawRectangle(startingX, startingY, currentX, currentY);
						view->DrawEllipse((startingX + currentX) / 2, (startingY + currentY) / 2, abs(currentX - startingX) / 2, abs(currentY - startingY) / 2);
						view->SetRedraw(true);
					}
				}

				else // filled
				{
					if (controller->GetShapeMode() == true) // rectangle mode
					{
						// draw rectangle gizmo
						view->DrawFilledRectangle(startingX, startingY, currentX, currentY);
						view->SetRedraw(true);
					}

					if (controller->GetShapeMode() == false) // ellipse mode
					{
						// draw ellipse gizmo
						view->DrawRectangle(startingX, startingY, currentX, currentY);
						view->DrawFilledEllipse((startingX + currentX) / 2, (startingY + currentY) / 2, abs(currentX - startingX) / 2, abs(currentY - startingY) / 2);
						view->SetRedraw(true);
					}
				}
			}

			if (pressed && controller->GetMode() == false && controller->GetSelecting() == false) // mouse button is down and we are in edit mode
			{
				for (int i = 0; i < controller->GetSelected().size(); i++)
				{
					if (controller->GetSelected()[i]->filled == false)
					{
						if (controller->GetSelected()[i]->type == primitive::rectangle)
						{

							view->DrawRectangle(controller->GetSelected()[i]->x1 + currentX - startingX, controller->GetSelected()[i]->y1 + currentY - startingY, controller->GetSelected()[i]->x2 + currentX - startingX, controller->GetSelected()[i]->y2 + currentY - startingY, 3, ECGV_BLUE);
							view->SetRedraw(true);
						}

						if (controller->GetSelected()[i]->type == primitive::ellipse)
						{

							view->DrawEllipse(controller->GetSelected()[i]->x1 + currentX - startingX, controller->GetSelected()[i]->y1 + currentY - startingY, controller->GetSelected()[i]->radiusX, controller->GetSelected()[i]->radiusY, 3, ECGV_BLUE);
							view->SetRedraw(true);
						}
					}

					if (controller->GetSelected()[i]->filled == true)
					{
						if (controller->GetSelected()[i]->type == primitive::rectangle)
						{

							view->DrawFilledRectangle(controller->GetSelected()[i]->x1 + currentX - startingX, controller->GetSelected()[i]->y1 + currentY - startingY, controller->GetSelected()[i]->x2 + currentX - startingX, controller->GetSelected()[i]->y2 + currentY - startingY, ECGV_BLUE);
							view->SetRedraw(true);
						}

						if (controller->GetSelected()[i]->type == primitive::ellipse)
						{
							view->DrawFilledEllipse(controller->GetSelected()[i]->x1 + currentX - startingX, controller->GetSelected()[i]->y1 + currentY - startingY, controller->GetSelected()[i]->radiusX, controller->GetSelected()[i]->radiusY, ECGV_BLUE);
							view->SetRedraw(true);
						}
					}
				}
			}
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_MOUSE_BUTTON_UP)
		{
			pressed = false;

			for (int i = 0; i < controller->GetSelected().size(); i++)
			{
				controller->GetSelected()[i]->visible = true;
			}

			if (controller->GetMode() == true) // insert mode
			{
				if (controller->GetShapeMode() == true) // ellipse mode
				{
					controller->InsertRec(startingX, startingY, currentX, currentY);
				}
				
				if (controller->GetShapeMode() == false) // ellipse mode
				{
					controller->InsertEllipse((startingX + currentX) / 2, (startingY + currentY) / 2, abs(currentX - startingX) / 2, abs(currentY - startingY) / 2);
				}
			}

			if (controller->GetMode() == false && controller->GetSelecting() == false) // edit mode
			{
				std::cout << "Move" << std::endl;
				controller->Move(startingX - currentX, startingY - currentY);
			}
		}
	}

private:
	ECGraphicViewImp* view = nullptr;
	GraphicController* controller = nullptr;

	bool pressed = false;
	int startingX;
	int startingY;
	int currentX;
	int currentY;
};

class KeyObserver : public ECObserver
{
public:
	KeyObserver(ECGraphicViewImp* p_view, GraphicController* p_controller)
	{
		view = p_view;
		controller = p_controller;
		std::cout << "Key Observer Created" << std::endl;
	}

	virtual void Update()
	{
		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_SPACE)
		{
			if (controller->SwitchMode() == true)
			{
				std::cout << "Insert Mode" << std::endl;
			}

			else
			{
				std::cout << "Edit Mode" << std::endl;
			}
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_D)
		{
			controller->Delete();
			std::cout << "Delete" << std::endl;
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_Z)
		{
			controller->Undo();
			std::cout << "Undo" << std::endl;
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_Y)
		{
			controller->Redo();
			std::cout << "Redo" << std::endl;
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_G)
		{
			if (controller->GetMode() == true) // insert mode
			{
				if (controller->SwitchShape() == true)
				{
					std::cout << "Rectangle Mode" << std::endl;
				}

				else
				{
					std::cout << "Ellipse Mode" << std::endl;
				}
			}

			else // edit mode
			{
				// handle group
			}
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_F)
		{
			if (controller->FillShape() == true)
			{
				std::cout << "Fill Mode" << std::endl;
			}

			else
			{
				std::cout << "Wireframe Mode" << std::endl;
			}
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_DOWN_CTRL)
		{
			controller->ToggleSelecting();
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_CTRL)
		{
			controller->ToggleSelecting();
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_UP)
		{
			controller->Move(0, 10);
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_DOWN)
		{
			controller->Move(0, -10);
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_LEFT)
		{
			controller->Move(10, 0);
		}

		if (view->GetCurrEvent() == ECGVEventType::ECGV_EV_KEY_UP_RIGHT)
		{
			controller->Move(-10, 0);
		}
	}

private:
	ECGraphicViewImp* view = nullptr;
	GraphicController* controller = nullptr;
};

#endif // !OBSERVERS_H
