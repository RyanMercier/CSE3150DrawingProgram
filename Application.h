#ifndef APPLICATION_H
#define APPLICATION_H

#include "ECGraphicViewImp.h"
#include "GraphicController.h"
#include "GraphicDocument.h"
#include "Observers.h"

class Application
{
public:
	Application() { Initialize(); }
	~Application() {}


	void Initialize()
	{
		// initialize timer observer
		TimerObserver t_observer(&view, &controller, &doc);
		view.Attach(&t_observer);

		// initialize mouse observers
		LeftMouseObserver lm_observer(&view, &controller);
		view.Attach(&lm_observer);

		// initialize key observers
		KeyObserver k_observer(&view, &controller);
		view.Attach(&k_observer);

		view.Show();
	}
	
private:
	const int widthWin = 800, heightWin = 600;
	ECGraphicViewImp view = ECGraphicViewImp(widthWin, heightWin);		// view
	GraphicDocument doc = GraphicDocument(&view);						// model
	GraphicController controller = GraphicController(&doc);				// controller
};

#endif APPLICATION_H