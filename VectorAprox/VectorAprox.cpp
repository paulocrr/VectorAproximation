// VectorAprox.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <time.h>
#include "VAFile.h"

using namespace std;
int main()
{
	time_t start, end;
	time(&start);
	VAFile v(5, 23);
	time(&end);
	double time_taken = double(end - start);
	cout << "Creation Time is : " << fixed
		<< time_taken;
	cout << " sec " << endl;
	string query = "0.0119	0.1330	0.0761	0.0140	0.1215	0.0103	0.0506	0.0029	0.3555	0.0545	0.0421	0.0510	0.0279	0.0214	0.2524	0.0571	0.3887	0.1843	0.0434	0.1729	0.1590	0.1786	0.1460";
	vector<float> data;
	string temp;
	for (int i = 0; i < query.size(); ++i) {
		if (query[i] == '	') {
			data.push_back(stod(temp));
			temp = "";
			continue;
		}
		temp += query[i];
	}
	if (temp != "") {
		data.push_back(stod(temp));
		temp = "";
	}
	time(&start);
	cout << "Query: " << query << endl;
	vector<pair<float, VANode*>> resSingleQuery = v.knnSearch(data, 5);
	time(&end);
	time_taken = double(end - start);
	cout << "Single Query Time is : " << fixed
		<< time_taken;
	cout << " sec " << endl;

	point *q = new point(0.01, 0.03);
	time(&start);
	vector<
		pair<
			pair<point*, vector<float>>,
			vector<pair<float, VANode*>
			>
		>
	> res = v.knnRegionSearch(q,1, 10);
	time(&end);
	time_taken = double(end - start);
	cout << "GeoPosition Query Time is : " << fixed
		<< time_taken;
	cout << " sec " << endl;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
