#include "Renderer.h"
#include <QtGui>

Renderer::Renderer(Simulation *sim)
{
	simulation = sim;
	colorMode = GENOME;
}
void Renderer::update(){
	updatePicture();
	QTimer::singleShot(UPDATE_INTERVAL, this, SLOT(update()));	
}

QColor Renderer::getColor(struct Cell *cell, int mode){
	int r = 0; //temporary red value
	int g = 0; //temporary green value
	int b = 0; //temporary blue value
	switch(mode){
		case GENERATION:
			r = qRed(cell->generation);
			g = qGreen(cell->generation);
			b = qBlue(cell->generation);
			break;
		case GENOME:
			if(cell->generation >= LIVING_CELL){
				int hash = 0;
				for(uint i = 0; i < cell->genome_size;i++ ){
					if(cell->genome[i] != cell->genome_size-1){
						hash += cell->genome[i];
					}else{
						break;
					}
				}
				r = hash % cell->genome_size + 40;
				g = (hash + 64) % cell->genome_size + 50;
				b = (hash +21) % cell->genome_size + 20;
			}
			break;
		case LINEAGE:
			if(cell->generation >= LIVING_CELL){
				r = qRed(cell->lineage);
				g = qGreen(cell->lineage);
				b = qBlue(cell->lineage);
			}
			break;
		case LOGO:
			if(cell->generation > 2){
				r = cell->genome[0] * 10;
				g = cell->genome[0] * 10;
				b = cell->genome[0] * 10;
			}
			break;
		case ENERGY:
			r = qRed(cell->energy * 900);
			g = qGreen(cell->energy * 180);
			b = qBlue(cell->energy );
			break;
	}
	
	return QColor(r % 255,g % 255,b % 255);
}

void Renderer::updatePicture(){	
	int sizeX = simulation->x();
	int sizeY = simulation->y();
	
	QImage temp(sizeX,sizeY,QImage::Format_RGB32);
	simulation->pause();
	for(int x = 0; x < sizeX; x++){
		for(int y = 0; y < sizeY; y++){
			struct Cell *cell = simulation->cell(x,y,0);
			
			temp.setPixel(x,y,getColor(cell,colorMode).rgb());
		}
	}
	int counter = simulation->counter();
	
	simulation->resume();
	setPixmap(QPixmap::fromImage (temp));
	qDebug() << "cells executed: " << counter;
}

void Renderer::mousePressEvent ( QMouseEvent * event ){
	if(event->button() == Qt::RightButton){
		colorMode = (colorMode + 1) % RENDERMODES;
		updatePicture();
	}else if(event->button() == Qt::LeftButton){
		simulation->pause();
		struct Cell *cell = simulation->cell(event->x(),event->y(),0);
		qDebug() << "try"<<event->x() << event->y();
		if(cell->generation >= LIVING_CELL){
			qDebug() << "got"<<event->x() << event->y();
			struct Cell tempCell = *cell;
			emit cellSelected(tempCell);
		}	
		
		simulation->resume();
	}
}

Renderer::~Renderer()
{
}
