#include <iostream>
#include <sstream>
#include <string.h>
using namespace std;
#include <iomanip>
#include <sqlite3.h>
#include <string>

//Estructura con todas las variables de la telemetria
typedef struct{
    int Matricula;
	char Mision[50];
	int Altura;
	int Angulo;
	int Distancia;
	int Tiempo;
}Telemetria;

//Definicion de la funciones utilizadas
void get_s(Telemetria *);	//
void f_despegar(Telemetria *,Telemetria []);
void f_girar(Telemetria *,Telemetria []);
void f_ir(Telemetria *,Telemetria []);
void f_corrigegiro(Telemetria *,Telemetria []);
void f_volver(Telemetria *,Telemetria []);
void f_aterrizar(Telemetria *,Telemetria []);
void PrintStruct(Telemetria [],Telemetria);
void print_r(char **Res, int , int );




//Funcion para calcular la subida del dron
void f_despegar(Telemetria *Tele,Telemetria Datos[]){
	int c,tmax;
	c=Tele->Tiempo;
	tmax=Tele->Tiempo+Tele->Altura/2;
	for(c=1;c<=tmax;c++){
		Datos[c].Altura=Datos[c-1].Altura+2;
		Datos[c].Tiempo=c;
		Datos[c].Angulo=Datos[c-1].Angulo;
		Datos[c].Distancia=Datos[c-1].Distancia;
	}
	Tele->Tiempo=c;
}
//Funcion para calcular la orientacion del dron
void f_girar(Telemetria *Tele,Telemetria Datos[]){
	int c,tmax;
	tmax=Tele->Tiempo+Tele->Angulo/10;
	c=Tele->Tiempo;
	for(c;c<tmax;c++){
		Datos[c].Angulo=Datos[c-1].Angulo+10;
		Datos[c].Tiempo=c;
		Datos[c].Altura=Datos[c-1].Altura;
		Datos[c].Distancia=Datos[c-1].Distancia;
	}
	Tele->Tiempo=c;
}
//Funcion para calcular la distancia de ida del dron
void f_ir(Telemetria *Tele,Telemetria Datos[]){
	int c,tmax;
	c=Tele->Tiempo;
	tmax=Tele->Tiempo+Tele->Distancia/10;
	for(c;c<tmax;c++){
		Datos[c].Distancia=Datos[c-1].Distancia+10;
		Datos[c].Tiempo=c;
		Datos[c].Altura=Datos[c-1].Altura;
		Datos[c].Angulo=Datos[c-1].Angulo;
	}
	Tele->Tiempo=c;
}
//Funcion para calcular la orientacion corregida del dron
void f_corrigegiro(Telemetria *Tele,Telemetria Datos[]){
	int c,tmax;
	tmax=Tele->Tiempo+Tele->Angulo/10;
	c=Tele->Tiempo;
	for(c;c<tmax;c++){
		Datos[c].Angulo=Datos[c-1].Angulo-10;
		Datos[c].Tiempo=c;
		Datos[c].Altura=Datos[c-1].Altura;
		Datos[c].Distancia=Datos[c-1].Distancia;
	}
	Tele->Tiempo=c;
}
//Funcion para calcular la distancia de Vuelta del dron
void f_volver(Telemetria *Tele,Telemetria Resul[]){
	int c,tmax;
	c=Tele->Tiempo;
	tmax=Tele->Tiempo+Tele->Distancia/20;
	for(c;c<=tmax;c++){
		Resul[c].Distancia=Resul[c-1].Distancia-20;
		Resul[c].Tiempo=c;
		Resul[c].Altura=Resul[c-1].Altura;
		Resul[c].Angulo=Resul[c-1].Angulo;
		if(Resul[c].Distancia<0){
			Resul[c].Distancia=0;
		}
	}

	Tele->Tiempo=c;
}

//Funcion para calcular la bajada del dron
void f_aterrizar(Telemetria *Tele,Telemetria Datos[]){
	int c,tmax;
	c=Tele->Tiempo;
	tmax=Tele->Tiempo+(Tele->Altura/2);
	for(c;c<tmax;c++){
		Datos[c].Altura=Datos[c-1].Altura-2;
		Datos[c].Tiempo=c;
		Datos[c].Angulo=Datos[c-1].Angulo;
		Datos[c].Distancia=Datos[c-1].Distancia;
	}
	Tele->Tiempo=c;
}
//Funcion para introducir los datos
void get_s(Telemetria *Tele){

	cout<<"Introducir la matricula: ";
	cin>>Tele->Matricula;
	cout<<endl<<"Introducir el nombre de la mison: ";
	cin>> Tele->Mision ;
	cout<<endl<<"Introducir la altura: ";
	cin>>Tele->Altura;
	cout<<endl<<"Introducir el angulo: ";
	cin>>Tele->Angulo;
	cout<<endl<<"Introducir la distancia: ";
	cin>>Tele->Distancia;

	Tele->Tiempo=0;
}
void PrintStruct(Telemetria Datos[],Telemetria Tele){
	int c,tmax;
	tmax=Tele.Tiempo;
	cout<<"Matricula, Mision, Altura, Angulo, Distancia, Tiempo"<<endl;
	for(c=0;c<tmax;c++){
		cout<<Datos[c].Matricula<<"	"<<Datos[c].Mision<<"	"<<Datos[c].Altura<<"	"<<Datos[c].Angulo<<"	"<<Datos[c].Distancia<<"	 "<<Datos[c].Tiempo<<endl;
	}
}
//Funcion para dibujar la table obtenida de SQLite
void print_r(char **Res, int NF, int NC){
 for(int c1=0;c1<=NF;c1++) {
    for(int c2=0;c2<NC;c2++)
        cout<<setw(12)<<Res[c1*NC+c2];
        cout<<endl;
 }
}


int main() {

	sqlite3 *db;
	char *Errores = 0;
	char **Resultados;
	int rc, N_reg, N_col;
	const char *sql ;
	char Sql[100];

    Telemetria Tele1;	//Estructura de los datos insertados
	Telemetria Datos[200] ;	//Vector con los resultados

	Datos[0].Altura=0;
	Datos[0].Angulo=0;
	Datos[0].Distancia=0;
	Datos[0].Tiempo=0;

		//Llamada a las funciones en orden de funcionamiento del dron
	get_s(&Tele1);
	f_despegar(&Tele1,Datos);
	f_girar(&Tele1,Datos);
	f_ir(&Tele1,Datos);
	f_corrigegiro(&Tele1,Datos);
	f_volver(&Tele1,Datos);
	f_aterrizar(&Tele1,Datos);

	//Abrimos la base de datos Dron
	rc = sqlite3_open("TelemetriaDron.db", &db);
	if(rc == SQLITE_OK)
	cout<<endl<<"Base de datos abierta correctamente."<<endl;
	else{
	cout<<endl<<"Error al abrir base de datos: ";
	cout<<endl<<sqlite3_errmsg(db);
	return(0);
	}
	//Borramos la tabla del anterior uso del programa
	strcpy(Sql, "DROP TABLE IF EXISTS Telemetria0");
	rc = sqlite3_get_table(db, Sql, &Resultados, &N_reg, &N_col, &Errores);
	if(rc == SQLITE_OK)
	cout<<endl<<"Sentencia ejecuta correctamente."<<endl;
	else{
	cout<<Errores;
	return(0);
	}

	//Creamos la tabla telem 2 con todas las variables
	sql = "CREATE TABLE IF NOT EXISTS Telemetria0("\
	"Matricula INT ," \
	"Mision TEXT ," \
	"Altura INT ," \
	"Angulo INT ," \
	"Distancia INT ," \
	"Tiempo INT );";
	rc = sqlite3_exec(db, sql, NULL, 0, &Errores);
	if(rc==SQLITE_OK)
	cout<<"La tabla se ha creado correctamente."<<endl;
	else {
	cout<<"Error en la orden de SQL: "<<Errores<<endl;
	return(0);
	}

	for(int c=0;c<Tele1.Tiempo;c++){
		Datos[c].Matricula=Tele1.Matricula;
		strcpy(Datos[c].Mision,Tele1.Mision);
	}
	//Insertamos todos los datos en la tabla creada en un bucle ya que estan todos dentro de un vector
	for(int c=0;c<Tele1.Tiempo;c++){
		sprintf(Sql,"INSERT INTO Telemetria0 (Matricula,Mision,Altura,Angulo,Distancia,Tiempo)" "VALUES ( '%s', %d, %d, %d, %d, %d); ",Datos[c].Mision,Datos[c].Matricula,Datos[c].Altura,Datos[c].Angulo,Datos[c].Distancia,Datos[c].Tiempo);
		rc = sqlite3_exec(db, Sql, NULL, 0, &Errores);
		}
	if(rc == SQLITE_OK)
	cout<<"Entradas insertadas correctamente."<<endl;
	else{
	cout<<"Error en la orden SQL: "<<Errores<<endl;
	return(0);
	}

	//Sentencia para seleccionar la tabla creada y asi despues poder imprimirla
	strcpy(Sql,"SELECT * from Telemetria0");
	rc=sqlite3_get_table(db, Sql, &Resultados, &N_reg, &N_col, &Errores);
	if(rc==SQLITE_OK)
	cout<<"La sentencia se ha ejecutado correctamente."<<endl;
	else{
	cout<< Errores;
	return(0);
	}
	system("CLS");
	print_r(Resultados, N_reg, N_col);
	sqlite3_free_table(Resultados);

	//Cerramos la base de datos
	sqlite3_close(db);
	return 0;
}
