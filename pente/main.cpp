#include"game.h"

//kompilowanie przy użyciu C++11 (visual studio 2013) powoduje 10 razy wolniejszą analizę planszy przez AI niż kompilacja przy użyciu C++17 (visual studio 2017).
//dlatego zalecane jest używanie visual studio 2017, lub unikanie mistrzowskiego poziomu trudności AI w przypadku kompilacji na visual studio 2013.

//Obecna wersja programu nie skompiluje się na visual studio 2013, ponieważ w graphicalMode.h wartości listy są przypisyane w dekleracji klasy, co jest dostępne 
//od c++17. W przypadku chęci uruchomienia na visual studio 2013, trzeba przenieść przypisywanie wartości do konstruktora i podmieć plik 
//w dependencies/GLFW/lib-vc2022/glfw3.lib na plik z katalogu dependencies/GLFW/LIB FOR VISUAL STUDIO 2013/glfw3.lib

int main() {
	startGame();
}