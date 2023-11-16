//SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <string>
//Headers
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "SDL2SoundEffects.h"



using namespace std;

RenderWindow window("Contador/Metas", 1280, 720);
//RenderWindow gamesWindow("Games Window", 1000, 500);

const int FPS = 60;
int refreshRate = 1000 / FPS;
int frame = 0;

struct contadores {
	string tipoCont;
	string titulo;
	string cantTemp;
	string cantTotal;
};

void leerContadores(contadores contador[], int &cantContadores){
	fstream txt;
	stringstream traductor;
	
	string tempString;
	
	txt.open("contadores.txt");
	getline(txt,tempString);
	
	traductor.clear();
	
	traductor<<tempString;
	traductor>>cantContadores;
	
	//cout<<cantContadores<<endl;
	traductor.clear();
	
	for (int i = 0; i < cantContadores; i++) {
		getline(txt, contador[i].tipoCont);
		getline(txt, contador[i].titulo);
		getline(txt, contador[i].cantTemp);
		getline(txt, contador[i].cantTotal);
		
		
	}
	txt.close();



	
	
}
void crearContador(contadores contador[], int cantContadores){
	
	leerContadores(contador,cantContadores);
	
	struct contadores nuevoContador;
	
	
	system("cls");
	cout<<"Ingrese el tipo de contador. 1= Meta 2=Contador"<<endl;
	cin>>nuevoContador.tipoCont;
	
	system("cls");
	cout<<"Ingrese el titulo"<<endl;
	cin>>nuevoContador.titulo;

	nuevoContador.cantTemp="0";
	
	system("cls");
	if(nuevoContador.tipoCont=="1"){
		cout<<"Ingrese la cantidad total a contar"<<endl;
	}
	else{
		cout<<"Ingrese la cantidad inicial"<<endl;
	}
	cin>>nuevoContador.cantTotal;
		
	
	ofstream writetxt;
	writetxt.open("contadores.txt");
	cantContadores+=1;
	writetxt<< cantContadores<<endl;
	for(int i = 0; i < cantContadores-1; i++){
		writetxt<< contador[i].tipoCont<<endl;
		writetxt<< contador[i].titulo<<endl;
		writetxt<< contador[i].cantTemp<<endl;
		writetxt<< contador[i].cantTotal<<endl;
	}
	
	writetxt<< nuevoContador.tipoCont<<endl;
	writetxt<< nuevoContador.titulo<<endl;
	writetxt<< nuevoContador.cantTemp<<endl;
	writetxt<< nuevoContador.cantTotal<<endl;
	
	
	writetxt.close();
	
	leerContadores(contador,cantContadores);
	
}
void mostrarContadores(contadores contador[3], int cantContadores){
	
	leerContadores(contador,cantContadores);
	system("cls");
	for (int i = 0; i < cantContadores; i++) {
		cout<<"Contador "<<i+1<<":"<<endl;
		cout<<"Titulo: "<<contador[i].titulo<<endl;	
		cout<<"Contando: "<<contador[i].cantTemp<<endl;	
		cout<<"Total: "<<contador[i].cantTotal<<endl;	
		cout<<endl<<endl<<endl;
		
		
	}
	system("pause");
}

void actualizarContadores(contadores contador[], int& cantContadores, int tipoCont[], string titleMessage[], string subtitleMessage[], string subsubtitleMessage[],
	int contadorTemp[], int contadorTotal[], float porcentaje[], int progresbarwidth[]) {
	stringstream tr;

	tr.clear();
	
	for (int i = 0; i < cantContadores; i++) {
		tr << contador[i].tipoCont;
		tr >> tipoCont[i];
		tr.clear();
		titleMessage[i] = contador[i].titulo;
		tr << contador[i].cantTemp;
		tr >> contadorTemp[i];
		tr.clear();
		tr << contador[i].cantTotal;
		tr >> contadorTotal[i];
		tr.clear();
		porcentaje[i] = (contadorTemp[i] * 100) / contadorTotal[i];
		progresbarwidth[i] = (porcentaje[i] * 300) / 100;
	}


	string tempString2;
	string queCuenta = " wins";

	stringstream stringput;


	for (int i = 0; i < cantContadores; i++) {
		if (tipoCont[i] == 1) {
			stringput.clear();

			stringput << contadorTemp[i] << "/" << contadorTotal[i];
			stringput >> tempString2;

			tempString2 += queCuenta;
			subtitleMessage[i] = tempString2;
		}

		if (tipoCont[i] == 2) {
			stringput.clear();

			stringput << contadorTotal[i];
			stringput >> tempString2;

			tempString2 += queCuenta;
			subtitleMessage[i] = tempString2;
		}






	}

	for (int i = 0; i < cantContadores; i++) {

		stringput.clear();

		stringput << "(" << porcentaje[i] << "%)";
		stringput >> tempString2;

		subsubtitleMessage[i] = tempString2;
	}



}

void menu(){
	system("cls");
	cout<<"Acciones a realizar"<<endl;
	cout<<"1- Actualizar contadores"<<endl;
	cout<<"2- Agregar contador"<<endl;
	cout<<"3- Borrar contadores"<<endl;
	cout<<"4- Mostrar contadores"<<endl;
}

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		cout << "IMG_init has failed. Error: " << SDL_GetError() << endl;

	if (TTF_Init() == -1) {
		cout << "TTF_init has failed. Error: " << SDL_GetError() << endl;
	}
	int mouseX, mouseY;
	int clicks = 0;
	int clickMenu = 0;

	//Sounds
	SDL2SoundEffects se;
	//se.addSoundEffect("res/sound/memory/pink.mp3");//0


	//Mix_Music* bgm = Mix_LoadMUS("res/sound/music.mp3");
	//se.addSoundEffect("res/sound/strike.mp3"); //0

	//SDL_Texture* brickTexture = window.loadTexture("res/gfx/brick.png");
	SDL_Texture* rectTexture = window.loadTexture("res/gfx/rect.png");
	SDL_Texture* pbTexture = window.loadTexture("res/gfx/progressbar.png");
	SDL_Texture* fpbTexture = window.loadTexture("res/gfx/fullprogressbar.png");



	//Entity LogicE(0, 50, 1280, 720, brickTexture);

	SDL_Event event;
	SDL_Event mouseEvent{ SDL_MOUSEMOTION };

	bool gameRunning = true;

	string subsubtitleMessage[3];
	string subtitleMessage[3];
	string titleMessage[3];
	titleMessage[0] = "test";
	titleMessage[1] = "testo mas";
	titleMessage[2] = "Titulo mas largo";



	int cantContadores = 0;
	string tempString;

	fstream txt;
	stringstream tr;

	struct contadores contador[3];
	
	int input;

	int contadorTotal[3];
	int contadorTemp[3];

	float porcentaje[3];
	int progresbarwidth[3];
	int tipoCont[3];

		
	

	while (gameRunning)
	{

		leerContadores(contador, cantContadores);
		actualizarContadores(contador, cantContadores, tipoCont, titleMessage, subtitleMessage, subsubtitleMessage,
			contadorTemp, contadorTotal, porcentaje, progresbarwidth);

		
		window.RenderWindow::clear();
		// Get our controls and events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				gameRunning = false;
			if (event.type == SDL_MOUSEMOTION) {

				mouseX = event.motion.x;
				mouseY = event.motion.y;
				//cout << mouseX << "," << mouseY << endl;

			}
			if (event.type == SDL_MOUSEBUTTONUP) {
			
			}
		}



		
	
		window.backgroundColor(100, 100, 100, 255);
		
		
	
	
		
		for (int i = 0; i < cantContadores; i++) {
			Entity rect(0, 0 + (240 * i), 568, 240, rectTexture);
			window.render(rect, 1);

			window.textAlignTo(titleMessage[i].c_str(),rect,255,255,255,255,35,2,0,-50,35+(240*i),"res/fonts/EncodeBold.ttf");
			window.textAlignTo(subtitleMessage[i].c_str(),rect,230, 230, 230,255,40,2,0,-50,30+55+(240*i),"res/fonts/Encode.ttf");
			
			if (tipoCont[i] == 1) {
				window.textAlignTo(subsubtitleMessage[i].c_str(), rect, 230, 230, 230, 255, 30, 2, 0, -50, 30 + 105 + (240 * i), "res/fonts/Encode.ttf");
				Entity progressBar(190, 170 + 240 * i, 300, 15, pbTexture);
				//window.render(progressBar, 1);
				window.renderAlignTo(progressBar, rect, 1, 2, 0, -50, 180 + 240 * i);
				Entity fullprogressBar(190, 170 + 240 * i, progresbarwidth[i], 15, fpbTexture);
				window.renderAlignTo(fullprogressBar, rect, 1, 2, 0, -50 - (300 - progresbarwidth[i]), 180 + 240 * i);

			}
			
			//window.drawText("test", 130 + (540), 550 + (58 * i), 234, 167, 2, 255, 30);
		}
		
		

		window.display();
		
		
		Sleep(refreshRate);
		if (frame == FPS) {
			frame = 0;
		}
		else frame++;


		
	}

	window.cleanUp();
	SDL_Quit();
	TTF_Quit();

	return 0;
}




		