#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "ECGraphicViewImp.h"
#include "GraphicController.h"
#include "GraphicDocument.h"
#include "Observers.h"

class Application
{
public:
	Application(std::string p_filename) { Initialize(p_filename); }
	~Application() {}


	void Initialize(std::string p_filename)
	{
		// setup save file system
        filename = p_filename;
        doc.LoadFile(filename);
		
		// initialize timer observer
		AppObserver a_observer(&view, &controller, &doc, filename);
		view.Attach(&a_observer);

		// initialize mouse observers
		LeftMouseObserver lm_observer(&view, &controller);
		view.Attach(&lm_observer);

		// initialize key observers
		KeyObserver k_observer(&view, &controller);
		view.Attach(&k_observer);

		view.Show();
	}
	
private:
    std::string filename;
	const int widthWin = 800, heightWin = 600;
	ECGraphicViewImp view = ECGraphicViewImp(widthWin, heightWin);		// view
	GraphicDocument doc = GraphicDocument(&view);						// model
	GraphicController controller = GraphicController(&doc);				// controller
};

#endif APPLICATION_H