#ifndef RENDERER_H_
#define RENDERER_H_

#include <QtGui>
#include "Simulation.h"

#define UPDATE_INTERVAL 1000

#define GENERATION 0
#define GENOME 1
#define LINEAGE 2
#define LOGO 3
#define ENERGY 4

#define RENDERMODES 5

class Renderer: public QLabel
{
	Q_OBJECT
public:
	Renderer(Simulation *sim);
	virtual ~Renderer();
	
	void mousePressEvent ( QMouseEvent * event );
public slots:
	void update();
	
private:
	void updatePicture();
	void printCell(int x, int y, int z);
	void printReadableGenome(int x, int y, int z);
	Simulation *simulation;
	int colorMode;
};

#endif /*RENDERER_H_*/
