//Checar lo de la hojas de dat


//Puedes poner pesta�as
//Puedes poner los datos de operaciones 

#include <Windows.h>
#include <CommCtrl.h>
#include "Resource.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include <ctime>
using namespace std;
/*
Se requiere crear un archivo: usuarios.txt

Con el siguiente orden:

Nombre completo del usuario
Usuario
Contrase�a

NOTA: EL LECTOR DE TEXTO TOMA EN CUENTA LOS ESPACIOS, POR LO QUE LOS DATOS DEBEN SER CONTINUOS PARA QUE LOS LEA APROPIADAMENTE
NOTA: PUEDE SEGUIR DANDO DE ALTA M�S USUARIOS USANDO EL MISMO ORDEN
	UNO TRAS OTRO
*/
char  QfotoDir[MAX_PATH]; 
struct usuario {
	char nombre[100];
	char usu[100];
	char contra[100];
	char fotoDir[MAX_PATH]; 
	usuario* sig;
}usuP;
usuario* inicio_usu = NULL;
usuario* auxU = NULL;

struct operacion {
	char nombreP[100];
	char costoP[100];
	char descriP[999];
	operacion* sig;
	operacion* ant;
}opP;
operacion* inicio_oper = NULL;
operacion* final_oper = NULL;
operacion* auxP = NULL;

struct veterinario {
	char claveV[10];
	char nombreV[100];
	char apeP[50], apeM[50];
	int turnoV;
	char diasV[7][10];
	int diaV[7];
	char numeroV[10];
	char especialV[20];
	veterinario* sig;
	veterinario* ant;
}vetP;
veterinario* inicio_v = NULL;
veterinario* final_v = NULL;
veterinario* auxV = NULL;

struct cliente {
	char nomC[100];
	char apeP[50], apeM[50];
	char celular[10];
	char direccion[100];
	char nombredeM[100];
	char tipodeEspecie[100];
	char raza[100];
	char problemasdeS[100];
	char fechadeN[100];
	cliente* sig;
	cliente* ant;
}client;
cliente* inicio_cl = NULL;
cliente* final_cl = NULL;
cliente* auxCl = NULL;
cliente* inicio_clA = NULL;

struct citas {
	char nomCl[100];
	char celCl[10];
	char oper[100];
	char nomV[100];
	SYSTEMTIME fecha;
	char hora[8];
	char clave[6];
	char estado[100];
	citas* sig;
	citas* ant;
}citaP;
citas* inicio_c = NULL;
citas* final_c = NULL;
citas* auxC = NULL;

//Usuarios
void agregarUsu(usuario*&, char[], char[], char[]);
void eliminarUsu(usuario*&, char[]);
void guardarUsuarios();
void leerUsuarios();
bool buscarUsu(usuario*&, char[], char[]);
void leerArcUsu();

//Operaciones
void agregarOper(operacion*&, operacion*&, char[], char[], char[]);
void eliminarOper(operacion*&, char[]);
void limpiarOper(HWND);
void guardarOpers();
void leerOpers(operacion*&, operacion*&);

//Veterinarios
void agregarVet(veterinario*&, veterinario*&, char[], char[], char[], char[], int, char[][10], int[7], char[], char[]);
void eliminarVet(veterinario*&, char[]);
void limpiarVet(HWND);
void ponerDias(HWND);
void verVets(HWND, veterinario*);
bool empalme(veterinario*, char[], int);
void guardarVets();
void leerVets(veterinario*&, veterinario*&);
void RepVets();

//Citas
void agregarCita(citas*&, citas*&, char[], char[], char[], char[], SYSTEMTIME, char[], char[], char[]); 
void eliminarCita(citas*&, citas*&, char[]);
void limpiarCita(HWND);
void guardarCitas();
void leerCitas(citas*&, citas*&);
cliente* buscarNum(char[]);
citas* buscarCita(char[]);

//Clientes 
void agregarCliente(cliente*&, cliente*&, char[], char[], char[], char[], char[], char[], char[], char[], char[], char[]);
void eliminarCliente(cliente*&, cliente*&, char[], char[], char[]);
void limpiarCliente(HWND);
void verCliente(HWND, cliente*);
void guardarClientes();
void leerClientes(cliente*&, cliente*&);
bool repNombredeM(char[]);
void crearArbolCl(cliente*&, cliente*);
void heapSortCl(cliente*&, cliente*);
void RepClientes(cliente*);
void swapCl(cliente*, cliente*);

LRESULT CALLBACK Procf(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MenuPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Operaciones(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Veterinarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Clientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Citas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Consultas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	/*Declaracion de variables */
	void leerOpers(operacion*&, operacion*&);
	void leerVets(veterinario*&, veterinario*&);
	void leerClientes(cliente*&, cliente*&);
	void leerCitas(citas*&, citas*&); 
	HWND hwnd;
	MSG mensaje;

	hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Procf);
	ShowWindow(hwnd, SW_SHOWDEFAULT);

	//BUCLE DE MENSAJES
	while (TRUE == GetMessage(&mensaje, 0, 0, 0))
	{
		TranslateMessage(&mensaje);
		DispatchMessage(&mensaje);
	}
	guardarOpers();
	guardarVets();
	guardarClientes();
	guardarCitas();
}

//Ventana de acceso
LRESULT CALLBACK Procf(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH pincel;
	switch (msg)
	{
	case WM_INITDIALOG: {
		pincel = CreateSolidBrush(RGB(0, 255, 205));
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), EM_LIMITTEXT, 100, NULL);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT2), EM_LIMITTEXT, 100, NULL);
		leerArcUsu();
		leerOpers(inicio_oper, final_oper);
		leerVets(inicio_v, final_v);
		leerClientes(inicio_cl, final_cl);
		leerCitas(inicio_c, inicio_c);  
		break;
	}
	case WM_CTLCOLORDLG:
		return (LRESULT)pincel; 
	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDOK) {
			char aux_usu[100]; char aux_contra[100]; char fotoDir[100];
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, 100, (LPARAM)&aux_usu);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, 100, (LPARAM)&aux_contra);
			if (aux_usu[0] != NULL && aux_contra[0] != NULL) {
				if (buscarUsu(inicio_usu, aux_usu, aux_contra) == true) {
					EndDialog(hwnd, LOWORD(wParam));
					DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)MenuPrincipal);
				}
				else {
					MessageBox(NULL, "Usuario o Contrase�a erroneos", "Resultado", MB_OK);
				}
			}
			else {
				MessageBox(NULL, "Usuario o Contrase�a erroneos", "Resultado", MB_OK);
			}
		}
		if (elemento == ID_IMAGEN_AGREGAR) {
			OPENFILENAME fotoVet;
			ZeroMemory(&fotoVet, sizeof(fotoVet));

			strcpy_s(QfotoDir, "");

			fotoVet.hwndOwner = hwnd;
			fotoVet.lpstrFile = QfotoDir;
			fotoVet.lStructSize = sizeof(OPENFILENAME);
			fotoVet.nMaxFile = MAX_PATH;
			fotoVet.lpstrDefExt = "txt";
			fotoVet.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			fotoVet.lpstrFilter = "*.bmp \0 *.*\0";
			if (GetOpenFileName(&fotoVet)) {

				HWND hPControl = GetDlgItem(hwnd, IDC_VET);
				HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen);
			}
		}
		if (elemento == ID_IMAGEN_ELIMINAR) {
			SendMessage(GetDlgItem(hwnd, IDC_VET), STM_SETIMAGE, IMAGE_BITMAP, NULL);
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(hwnd);
		break;
	}
	default:
		break;
	}
	return false;
}

//Menu Principal
LRESULT CALLBACK MenuPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static HBRUSH pincel;
	switch (msg)
	{
	case WM_INITDIALOG: {
		pincel = CreateSolidBrush(RGB(5, 40, 255)); 
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 100, (LPARAM)&usuP.nombre); 
		HWND hPControl = GetDlgItem(hwnd, IDC_VET4); 
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE); 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen); 

		break;
	}
	case WM_CTLCOLORDLG:
		return (LRESULT)pincel;

	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDC_BUTTON1)
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, (DLGPROC)Operaciones);
		if (elemento == IDC_BUTTON2)
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, (DLGPROC)Veterinarios);
		if (elemento == IDC_BUTTON3)
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, (DLGPROC)Citas);
		if (elemento == IDC_BUTTON4)
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, (DLGPROC)Clientes);
		if (elemento == IDC_BUTTON5)
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, (DLGPROC)Consultas);
		if (elemento == IDC_BUTTON6)
			PostQuitMessage(117);
		break;
	}
	case WM_CLOSE: { DestroyWindow(hwnd);
	}
	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;

	default:
		break;
	}
	return false;
}
//Operaciones 
LRESULT CALLBACK Operaciones(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 100, (LPARAM)&usuP.nombre);
		HWND hPControl = GetDlgItem(hwnd, IDC_VET4); 
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE); 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen); 
		SendMessage(GetDlgItem(hwnd, IDC_EDIT2), EM_LIMITTEXT, 100, NULL);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT3), EM_LIMITTEXT, 100, NULL);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT4), EM_LIMITTEXT, 999, NULL);
		break;
	}
	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDOK) 
		{
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, 100, (LPARAM)&opP.nombreP);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_GETTEXT, 100, (LPARAM)&opP.costoP);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_GETTEXT, 999, (LPARAM)&opP.descriP);
			if (opP.nombreP[0] != NULL && opP.costoP[0] != NULL && opP.descriP[0] != NULL) {
				agregarOper(inicio_oper, final_oper, opP.nombreP, opP.costoP, opP.descriP);
				MessageBox(NULL, "Registro hecho correctamente", "REGISTRO", MB_OK);
				limpiarOper(hwnd);
			}
			else
				MessageBox(NULL, "Faltan datos por su registro", "REGISTRO", MB_OK);
		}
		if (elemento == IDC_Ver) {
			auxP = inicio_oper;
			if (auxP == NULL)
				MessageBox(NULL, "No existen operaciones", "OPERS", MB_OK);
			else {
				limpiarOper(hwnd);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 100, (LPARAM)&auxP->nombreP);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 100, (LPARAM)&auxP->costoP);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 999, (LPARAM)&auxP->descriP);
			}
		}
		if (elemento == IDC_BUTTON4) {
			if (auxP != NULL) {
				if (auxP->sig != NULL) {
					auxP = auxP->sig;
					limpiarOper(hwnd);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 100, (LPARAM)&auxP->nombreP);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 100, (LPARAM)&auxP->costoP);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 999, (LPARAM)&auxP->descriP);
				}
			}
			else
				MessageBox(NULL, "No existen m�s operaciones", "OPERS", MB_OK);
		}
		if (elemento == IDC_BUTTON5) {
			if (auxP != NULL) {
				if (auxP->ant != NULL) {
					auxP = auxP->ant;
					limpiarOper(hwnd);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 100, (LPARAM)&auxP->nombreP);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 100, (LPARAM)&auxP->costoP);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 999, (LPARAM)&auxP->descriP);
				}
			}
			else
				MessageBox(NULL, "No existen m�s operaciones", "OPERS", MB_OK);
		}
		if (elemento == IDC_BUTTON3)
			limpiarOper(hwnd);
		if (elemento == IDC_BUTTON6) {
			if (auxP->nombreP != NULL && auxP->costoP != NULL && auxP->descriP != NULL) {
				eliminarOper(inicio_oper, auxP->nombreP);
				limpiarOper(hwnd);
			}
			else
				MessageBox(NULL, "Datos inconclusos", "OPERS", MB_OK);
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

//Veterinarios - Genera los reportes con un quicksort para ordenar de forma rapida y eficiente.
LRESULT CALLBACK Veterinarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(hwnd, IDC_NOMBRE), WM_SETTEXT, 100, (LPARAM)&usuP.nombre);
		HWND hPControl = GetDlgItem(hwnd, IDC_VET); 
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 75, 75, LR_LOADFROMFILE); 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen); 
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), EM_LIMITTEXT, 10, NULL);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT2), EM_LIMITTEXT, 100, NULL);
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Matutino");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Vespertino");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Nocturno");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Lunes");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Martes");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Mi�rcoles");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Jueves");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Viernes");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"S�bado");
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Domingo");
		SendMessage(GetDlgItem(hwnd, IDC_EDIT3), EM_LIMITTEXT, 10, NULL);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT5), EM_LIMITTEXT, 20, NULL);
		//Fin de Funciones
		break;
	}
	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDOK) {
			{
				SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, 10, (LPARAM)&vetP.claveV);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, 100, (LPARAM)&vetP.nombreV);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_GETTEXT, 50, (LPARAM)&vetP.apeP);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_GETTEXT, 50, (LPARAM)&vetP.apeM);
			}
			vetP.turnoV = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, NULL, NULL);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETSELITEMS, 7, (LPARAM)&vetP.diaV);
			for (int i = 0; i < 7; i++)
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETTEXT, vetP.diaV[i], (LPARAM)&vetP.diasV[i]);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_GETTEXT, 10, (LPARAM)&vetP.numeroV);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_GETTEXT, 20, (LPARAM)&vetP.especialV);
			if (vetP.claveV[0] != NULL && vetP.nombreV[0] != NULL && vetP.apeP[0] != NULL && vetP.apeM[0] != NULL && vetP.diaV != NULL && vetP.numeroV[10] != NULL) {
				if (empalme(inicio_v, vetP.especialV, vetP.turnoV) == false) {
					agregarVet(inicio_v, final_v, vetP.claveV, vetP.nombreV, vetP.apeP, vetP.apeM, vetP.turnoV, vetP.diasV, vetP.diaV, vetP.numeroV, vetP.especialV);
					limpiarVet(hwnd);
				}
				else
					MessageBox(NULL, "Empalme de datos", "Especialidad & Turno", MB_OK);
			}
			else
				MessageBox(NULL, "Datos inconclusos", "VETS", MB_OK);
		}
		if (elemento == IDC_OBSERVAR) {
			auxV = inicio_v;
			if (auxV == NULL)
				MessageBox(NULL, "No existen veterinarios", "VETS", MB_OK);
			else {
				limpiarVet(hwnd);
				verVets(hwnd, auxV);
				ponerDias(hwnd);
				//Fin de Funciones 
			}
		}
		if (elemento == IDC_SIG) {
			if (auxV != NULL) {
				if (auxV->sig != NULL) {
					auxV = auxV->sig;
					limpiarVet(hwnd);
					verVets(hwnd, auxV);
					ponerDias(hwnd);
				}
				else
					MessageBox(NULL, "No existen m�s veterinarios", "VETS", MB_OK);
			}
			else
				MessageBox(NULL, "No existen m�s veterinarios", "VETS", MB_OK);
		}
		if (elemento == IDC_ANT) {
			if (auxV != NULL) {
				if (auxV->ant != NULL) {
					auxV = auxV->ant;
					limpiarVet(hwnd);
					verVets(hwnd, auxV);
					ponerDias(hwnd);
				}
				else
					MessageBox(NULL, "No existen m�s veterinarios", "VETS", MB_OK);
			}
			else
				MessageBox(NULL, "No existen m�s veterinarios", "VETS", MB_OK);
		}
		if (elemento == IDC_CLEAN)
			limpiarVet(hwnd);
		if (elemento == IDC_DELETE) {
			if (auxV->claveV[0] != NULL && auxV->nombreV[0] != NULL && auxV->numeroV[0] != NULL) {
				eliminarVet(inicio_v, auxV->claveV);
				limpiarVet(hwnd);
			}
			else
				MessageBox(NULL, "Datos inconclusos", "VETS", MB_OK);
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

//Clientes  
LRESULT CALLBACK Clientes(HWND handler1, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(handler1, IDC_NOMBRE1), WM_SETTEXT, 100, (LPARAM)&usuP.nombre);
		HWND hPControl = GetDlgItem(handler1, IDC_VET4); 
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE); 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen);  
		SendMessage(GetDlgItem(handler1, IDC_EDIT1), EM_LIMITTEXT, 100, NULL);
		SendMessage(GetDlgItem(handler1, IDC_EDIT2), EM_LIMITTEXT, 50, NULL);
		SendMessage(GetDlgItem(handler1, IDC_EDIT3), EM_LIMITTEXT, 50, NULL);
		SendMessage(GetDlgItem(handler1, IDC_EDIT5), EM_LIMITTEXT, 10, NULL);
		SendMessage(GetDlgItem(handler1, IDC_EDIT6), EM_LIMITTEXT, 100, NULL);
		SendMessage(GetDlgItem(handler1, IDC_EDIT7), EM_LIMITTEXT, 20, NULL);
		break;
	}
	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDOK) {
			SendMessage(GetDlgItem(handler1, IDC_EDIT1), WM_GETTEXT, 100, (LPARAM)&client.nomC);
			SendMessage(GetDlgItem(handler1, IDC_EDIT2), WM_GETTEXT, 50, (LPARAM)&client.apeP);
			SendMessage(GetDlgItem(handler1, IDC_EDIT3), WM_GETTEXT, 50, (LPARAM)&client.apeM);
			SendMessage(GetDlgItem(handler1, IDC_EDIT5), WM_GETTEXT, 10, (LPARAM)&client.celular);
			SendMessage(GetDlgItem(handler1, IDC_EDIT6), WM_GETTEXT, 100, (LPARAM)&client.direccion);
			SendMessage(GetDlgItem(handler1, IDC_EDIT7), WM_GETTEXT, 20, (LPARAM)&client.nombredeM);
			SendMessage(GetDlgItem(handler1, IDC_EDIT8), WM_GETTEXT, 100, (LPARAM)&client.tipodeEspecie);
			SendMessage(GetDlgItem(handler1, IDC_EDIT9), WM_GETTEXT, 100, (LPARAM)&client.raza);
			SendMessage(GetDlgItem(handler1, IDC_EDIT10), WM_GETTEXT, 25, (LPARAM)&client.fechadeN);
			SendMessage(GetDlgItem(handler1, IDC_EDIT11), WM_GETTEXT, 50, (LPARAM)&client.problemasdeS);

			bool blanco = true;
			if (client.nomC[0] != NULL && client.apeP[0] != NULL && client.apeM[0] != NULL && client.celular[8] != NULL && client.direccion[0] != NULL
				&& blanco == true && client.tipodeEspecie[0] != NULL && client.raza[0] != NULL && client.fechadeN[0] != NULL && client.problemasdeS[0] != NULL) {
				if (!repNombredeM(client.nombredeM)) {
					agregarCliente(inicio_cl, final_cl, client.nomC, client.apeP, client.apeM, client.celular, client.direccion,
						client.nombredeM, client.tipodeEspecie, client.raza, client.fechadeN, client.problemasdeS);
					MessageBox(NULL, "Registro hecho exitosamente", "Clientes", MB_OK);
					limpiarCliente(handler1);
				}
				else {
					MessageBox(NULL, "Nombre de Mascota repetido", "Clientes", MB_OK);
				}
			}
			else {
				MessageBox(NULL, "Datos incompletos", "Clientes", MB_OK);
			}
		}
		if (elemento == IDCANCEL) {
			SendMessage(GetDlgItem(handler1, IDC_EDIT1), WM_GETTEXT, 100, (LPARAM)&client.nomC);
			SendMessage(GetDlgItem(handler1, IDC_EDIT2), WM_GETTEXT, 50, (LPARAM)&client.apeP);
			SendMessage(GetDlgItem(handler1, IDC_EDIT3), WM_GETTEXT, 50, (LPARAM)&client.apeM);
			eliminarCliente(inicio_cl, final_cl, client.nomC, client.apeP, client.apeM);
			limpiarCliente(handler1);
		}
		if (elemento == IDC_CLEAN) {
			limpiarCliente(handler1);
		}
		if (elemento == IDC_OBSERVAR) {
			auxCl = inicio_cl;
			if (auxCl == NULL)
				MessageBox(NULL, "No existen veterinarios", "VETS", MB_OK);
			else {
				limpiarCliente(handler1);
				verCliente(handler1, auxCl);
			}
		}
		if (elemento == IDC_SIG) {
			if (auxCl != NULL) {
				if (auxCl->sig != NULL) {
					auxCl = auxCl->sig;
					limpiarCliente(handler1);
					verCliente(handler1, auxCl);;
				}
				else
					MessageBox(NULL, "No existen m�s clientes", "CLIENT", MB_OK);
			}
			else
				MessageBox(NULL, "No existen m�s clientes", "CLIENT", MB_OK);
		}
		if (elemento == IDC_ANT) {
			if (auxCl != NULL) {
				if (auxCl->ant != NULL) {
					auxCl = auxCl->ant;
					limpiarCliente(handler1);
					verCliente(handler1, auxCl);;
				}
				else
					MessageBox(NULL, "No existen m�s clientes", "CLIENT", MB_OK);
			}
			else
				MessageBox(NULL, "No existen m�s clientes", "CLIENT", MB_OK);
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handler1, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

//Registro de Citas
LRESULT CALLBACK Citas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(hwnd, IDC_NOMBRE), WM_SETTEXT, 100, (LPARAM)&usuP.nombre);
		HWND hPControl = GetDlgItem(hwnd, IDC_VET4); 
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE); 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen); 
		auxCl = inicio_cl;
		while (auxCl != NULL) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, 100, (LPARAM)&auxCl->nomC);
			auxCl = auxCl->sig;
		}
		auxP = inicio_oper;
		while (auxP != NULL) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_ADDSTRING, 100, (LPARAM)&auxP->nombreP);
			auxP = auxP->sig;
		}
		auxV = inicio_v;
		while (auxV != NULL) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_ADDSTRING, 100, (LPARAM)&auxV->nombreV); 
			auxV = auxV->sig;
		}
		srand(time(NULL));
		break;
	}
	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDOK) {
			//C�digo random de la cita
			int r = rand() % 9999 + 1000;
			char cod[6];
			_itoa(r, cod, 10);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 6, (LPARAM)&cod); 

			int index1 = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, NULL, NULL);
			int index2 = SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_GETCURSEL, NULL, NULL);
			if (index1 != CB_ERR || index2 != CB_ERR) {
				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETLBTEXT, index1, (LPARAM)&citaP.nomCl);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_GETLBTEXT, index2, (LPARAM)&citaP.nomV);
				// HAY QUE BUSCAR COINCIDIR EL CLIENTE Y PONER EL N�MERO
				auxCl = buscarNum(citaP.nomCl);
				if (auxCl != NULL) {
					SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 10, (LPARAM)&auxCl->celular); 
				} 
			}
		}
		if (elemento == IDREGIS) {
			int index1 = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, NULL, NULL);
			int index2 = SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_GETCURSEL, NULL, NULL);
			int oper = SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_GETCURSEL, NULL, NULL);
			if (index1 != CB_ERR || index2 != CB_ERR) {
				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETLBTEXT, index1, (LPARAM)&citaP.nomCl);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_GETTEXT, 10, (LPARAM)&citaP.celCl);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_GETLBTEXT, oper, (LPARAM)&citaP.oper);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_GETLBTEXT, index2, (LPARAM)&citaP.nomV);
				SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), DTM_GETSYSTEMTIME, NULL, (LPARAM)&citaP.fecha);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, 10, (LPARAM)&citaP.hora);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, 6, (LPARAM)&citaP.clave);
			}
			if (citaP.nomCl[0] != NULL && citaP.celCl[8] != NULL && citaP.oper[0] != NULL && citaP.nomV[0] != NULL && citaP.fecha.wDay != 0 && citaP.fecha.wDay != 6 && citaP.hora[0] != NULL && citaP.clave[0] != NULL) {
				agregarCita(inicio_c, final_c, citaP.nomCl, citaP.celCl, citaP.oper, citaP.nomV, citaP.fecha, citaP.hora, citaP.clave, citaP.estado);
				limpiarCita(hwnd);
				MessageBox(NULL, citaP.clave, "Recuerde su c�digo", MB_OK);
			}
			else {
				MessageBox(NULL, "Datso inconclusos", "CITAS", MB_OK);
			}
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

//Consultas
LRESULT CALLBACK Consultas(HWND handler2, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(handler2, IDC_NOMBRE), WM_SETTEXT, 100, (LPARAM)&usuP.nombre);
		HWND hPControl = GetDlgItem(handler2, IDC_VET2);    
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, QfotoDir, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);  
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen); 
		break;
	}
	case WM_COMMAND: {
		int elemento = LOWORD(wParam);
		if (elemento == IDOK) {
			SendMessage(GetDlgItem(handler2, IDC_EDIT1), WM_GETTEXT, 8, (LPARAM)&citaP.clave);
			auxC = buscarCita(citaP.clave);

			if (auxC != NULL) {
				SendMessage(GetDlgItem(handler2, IDC_EDIT2), WM_SETTEXT, 100, (LPARAM)&auxC->nomCl);
				SendMessage(GetDlgItem(handler2, IDC_EDIT4), WM_SETTEXT, 10, (LPARAM)&auxC->celCl);
				SendMessage(GetDlgItem(handler2, IDC_EDIT5), WM_SETTEXT, 100, (LPARAM)&auxC->oper);
				SendMessage(GetDlgItem(handler2, IDC_EDIT6), WM_SETTEXT, 100, (LPARAM)&auxC->nomV);
				 
				char dia[10]; _itoa(auxC->fecha.wDay, dia, 10);
				char mes[10]; _itoa(auxC->fecha.wMonth, mes, 10);
				char a�o[10]; _itoa(auxC->fecha.wYear, a�o, 10);

				SendMessage(GetDlgItem(handler2, IDC_DIA), WM_SETTEXT, 100, (LPARAM)&dia);
				SendMessage(GetDlgItem(handler2, IDC_MEH), WM_SETTEXT, 100, (LPARAM)&mes);
				SendMessage(GetDlgItem(handler2, IDC_ANO), WM_SETTEXT, 100, (LPARAM)&a�o);
				SendMessage(GetDlgItem(handler2, IDC_EDIT8), WM_SETTEXT, 100, (LPARAM)&auxC->hora);
				SendMessage(GetDlgItem(handler2, IDC_EDIT9), WM_SETTEXT, NULL, (LPARAM)&auxC->estado);
			}
		}
		if (elemento == IDCANCEL) {
			char razon[100];
			SendMessage(GetDlgItem(handler2, IDC_EDIT1), WM_GETTEXT, 100, (LPARAM)&razon);
			if (auxC != NULL || razon[0] != NULL) {
				SendMessage(GetDlgItem(handler2, IDC_EDIT9), WM_SETTEXT, NULL, (LPARAM)"Cancelada");
				strcpy(auxC->estado, "Cancelada");
				eliminarCita(inicio_c, final_c, auxC->clave);
			}
			else {
				MessageBox(handler2, "Puede que te haga falta una raz�n", "POSIBLE FALTA DE DATOS", MB_OK);
			}
		}
		if (elemento == IDCONFIRM) {
			if (auxC != NULL) {
				SendMessage(GetDlgItem(handler2, IDC_EDIT9), WM_SETTEXT, NULL, (LPARAM)"Confirmada");
				strcpy(auxC->estado, "Confirmada");
			}
			else {
				MessageBox(handler2, "Falta actualizar c�digo", "CONSULTA//C�DIGO", MB_OK);
			}
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handler2, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

// Funciones de Usuario
void agregarUsu(usuario*& inicio, char nombre[], char usu[], char contra[]) {
	usuario* nuevo_nodo = new usuario();
	strcpy(nuevo_nodo->nombre, nombre);
	strcpy(nuevo_nodo->usu, usu);
	strcpy(nuevo_nodo->contra, contra);

	usuario* aux1 = inicio;
	usuario* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicio == aux1)
		inicio = nuevo_nodo;
	else
		aux2->sig = nuevo_nodo;

	nuevo_nodo->sig = aux1;
}

void eliminarUsu(usuario*& inicio, char nombre[]) {
	if (inicio != NULL) {
		usuario* aux_borrar;
		usuario* ant = NULL;
		aux_borrar = inicio;

		while (aux_borrar != NULL) {
			ant = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No se encontro el dato", "Resultado", MB_OK);
		else if (ant == NULL) {
			inicio = inicio->sig;
			delete aux_borrar;
		}
		else {
			ant->sig = aux_borrar->sig;
			delete aux_borrar;
		}
	}
}

void guardarUsuarios() {
	ofstream archivo;
	archivo.open("usuarios.txt", ios::out);
	if (archivo.is_open()) {
		usuario* aux = inicio_usu;
		if (aux == NULL) {
			MessageBox(NULL, "No se encontro el dato", "Resultado", MB_OK);
		}
		else {
			while (aux != NULL) {
				archivo.write(reinterpret_cast<char*>(aux), sizeof(usuario));
				aux = aux->sig;
			}
		}
	}
	archivo.close();
	return;
}

void leerUsuarios() {
	ifstream archivo;
	archivo.open("usuarios.txt", ios::in);
	inicio_usu = NULL;
	if (archivo.is_open()) {
		int arh = sizeof(usuario) / sizeof(archivo);
		usuario* aux = inicio_usu;
		for (int i = 0; i < arh; i++) {
			archivo.seekg(i * sizeof(usuario));
			archivo.read(reinterpret_cast<char*>(aux), sizeof(usuario));
			agregarUsu(inicio_usu, aux->nombre, aux->usu, aux->contra);
			aux = aux->sig;
		}
		archivo.close();
	}
	else
		archivo.close();
	return;
}

bool buscarUsu(usuario*& inicio, char usu[], char contra[]) {
	usuario* busqueda = inicio;
	while (inicio != NULL && busqueda != NULL) {
		if (strcmp(busqueda->usu, usu) == 0 && strcmp(busqueda->contra, contra) == 0) {
			strcpy(usuP.nombre, busqueda->nombre);
			strcpy(usu, busqueda->usu);
			strcpy(contra, busqueda->contra);
			return true;
		}
		else {
			busqueda = busqueda->sig;
		}
	}
	return false;
}

void leerArcUsu() {
	ifstream archivo;
	archivo.open("usuarios.txt", ios::in);
	if (archivo.is_open()) {
		while (!archivo.eof()) {
			archivo.getline(usuP.nombre, 100);
			archivo.getline(usuP.usu, 100);
			archivo.getline(usuP.contra, 100);
			agregarUsu(inicio_usu, usuP.nombre, usuP.usu, usuP.contra);
		}
	}
	archivo.close();
}

//Guardado como Operaciones.dat
void agregarOper(operacion*& inicioOp, operacion*& finalOp, char nombreP[], char costoP[], char descriP[]) {
	operacion* nuevo_nodo = new operacion();
	strcpy(nuevo_nodo->nombreP, nombreP);
	strcpy(nuevo_nodo->costoP, costoP);
	strcpy(nuevo_nodo->descriP, descriP);

	operacion* aux1 = inicioOp;
	operacion* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicioOp == aux1) {
		inicioOp = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	finalOp = nuevo_nodo;
}

void eliminarOper(operacion*& inicioOp, char nombreP[]) {
	if (inicioOp != NULL) {
		operacion* aux_borrar;
		operacion* ante = NULL;
		aux_borrar = inicioOp;

		while (aux_borrar != NULL && strcmp(aux_borrar->nombreP, nombreP)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No existe esa operaci�n", "OPERS", MB_OK);
		else if (ante == NULL) {
			inicioOp = inicioOp->sig;
			if (aux_borrar->sig != NULL)
				aux_borrar->sig->ant = NULL;
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}
void limpiarOper(HWND hwnd) {
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, NULL, (LPARAM)L"");
}

void guardarOpers() {
	ofstream opers;
	opers.open("operaciones.txt", ios::binary | ios::out | ios::trunc); //Abriendo el archivo binario 
	if (opers.is_open()) {
		auxP = inicio_oper;
		while (auxP != NULL) {
			opers.write(reinterpret_cast<char*>(auxP), sizeof(operacion));
			auxP = auxP->sig;
		}
	}
	opers.close(); //Cerrar archivo 
	return;
}

void leerOpers(operacion*& inicio, operacion*& final) {
	ifstream opers;
	opers.open("operaciones.txt", ios::binary | ios::in);
	if (opers.is_open())
	{
		while (!opers.eof()) {
			operacion* nuevo_nodo = new operacion();
			opers.read(reinterpret_cast<char*>(nuevo_nodo), sizeof(operacion));
			if (nuevo_nodo->nombreP[0] != NULL) {
				operacion* aux1 = inicio;
				operacion* aux2 = NULL;

				while (aux1 != NULL) {
					aux2 = aux1;
					aux1 = aux1->sig;
				}
				if (inicio == aux1) {
					inicio = nuevo_nodo;
				}
				else {
					aux2->sig = nuevo_nodo;
					nuevo_nodo->ant = aux2;
				}
				nuevo_nodo->sig = aux1;
				nuevo_nodo->ant = aux2;
				final = nuevo_nodo;
			}
		}
	}
	opers.close();
	return;
}

//Veterinarios 
void agregarVet(veterinario*& inicioVet, veterinario*& finalVet, char claveV[], char nombreV[], char apeP[], char apeM[], int turnoV, char diasV[][10], int diaV[7], char numeroV[], char especialV[]) {
	veterinario* nuevo_nodo = new veterinario;
	strcpy(nuevo_nodo->claveV, claveV);
	strcpy(nuevo_nodo->nombreV, nombreV);
	strcpy(nuevo_nodo->apeP, apeP);
	strcpy(nuevo_nodo->apeM, apeM);
	nuevo_nodo->turnoV = turnoV;
	for (int i = 0; i < 7; i++) {
		strcpy(nuevo_nodo->diasV[i], diasV[i]);
		nuevo_nodo->diaV[i] = diaV[i];
	}
	strcpy(nuevo_nodo->numeroV, numeroV);
	strcpy(nuevo_nodo->especialV, especialV);

	veterinario* aux1 = inicioVet;
	veterinario* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicioVet == aux1) {
		inicioVet = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	finalVet = nuevo_nodo;
}
void eliminarVet(veterinario*& inicioVet, char claveV[]) {
	if (inicioVet != NULL) {
		veterinario* aux_borrar;
		veterinario* ante = NULL;
		aux_borrar = inicioVet;

		while (aux_borrar != NULL && strcmp(aux_borrar->claveV, claveV)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "Veterinario no encontrado", "VETS", MB_OK);
		else if (ante == NULL) {
			inicioVet = inicioVet->sig;
			if (aux_borrar->sig != NULL)
				aux_borrar->sig->ant = NULL;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}
void limpiarVet(HWND handler) {
	SendMessage(GetDlgItem(handler, IDC_EDIT1), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT6), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT7), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_RESETCONTENT, NULL, NULL);
	SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Matutino");
	SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Vespertino");
	SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Nocturno");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_RESETCONTENT, NULL, NULL);
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Lunes");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Martes");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Mi�rcoles");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Jueves");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Viernes");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"S�bado");
	SendMessage(GetDlgItem(handler, IDC_LIST1), LB_ADDSTRING, NULL, (LPARAM)"Domingo");
}

void ponerDias(HWND hwnd) {
	int dif = 0, space = 0, h = 0;
	for (int j = 0; j < 7; j++) {
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGE, TRUE, auxV->diaV[j]);
		dif = auxV->diaV[j] - j;
		if (auxV->diaV[0] != 0) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGE, FALSE, auxV->diaV[0] - 1);
		}
		if (auxV->diaV[j] != j && auxV->diaV[j] != 0) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGE, FALSE, j);
			space++; h = j;
			if (dif != 0) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGE, FALSE, j + (dif - 1));
			}
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGEEX, 0, auxV->diaV[j - 1]);
			if (space == 2) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGE, FALSE, h);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SELITEMRANGEEX, 0, auxV->diaV[h - 1]);
			}
		}

	}
}
void verVets(HWND hwnd, veterinario* vet) {
	SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 10, (LPARAM)&vet->claveV);
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 100, (LPARAM)&vet->nombreV);
	SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 50, (LPARAM)&vet->apeP);
	SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, 50, (LPARAM)&vet->apeM);
	SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 10, (LPARAM)&vet->numeroV);
	SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 20, (LPARAM)&vet->especialV);
	SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_SETCURSEL, vet->turnoV, NULL);
}
bool empalme(veterinario* inicioVet, char especialV[], int turnoV) {
	veterinario* aux = inicioVet;
	if (inicioVet != NULL) {
		while (aux != NULL) {
			if (strcmp(inicioVet->especialV, especialV) == 0 && inicioVet->turnoV == turnoV) {
				return true;
			}
			aux = aux->sig;
		}
	}
	return false;
}

//Guardado como veterinarios.txt;
void guardarVets() {
	ofstream vets;
	vets.open("veterinarios.txt", ios::binary | ios::out | ios::trunc);
	if (vets.is_open()) {
		auxV = inicio_v;
		while (auxV != NULL) {
			vets.write(reinterpret_cast<char*>(auxV), sizeof(veterinario));
			auxV = auxV->sig;
		}
	}
	vets.close();
	return;
}

void leerVets(veterinario*& inicio, veterinario*& final) {
	ifstream vets;
	vets.open("veterinarios.txt", ios::binary | ios::in);
	if (vets.is_open()) {
		while (!vets.eof()) {
			veterinario* nuevo_nodo = new veterinario();
			vets.read(reinterpret_cast<char*>(nuevo_nodo), sizeof(veterinario));
			if (nuevo_nodo->nombreV[0] != NULL) {
				veterinario* aux1 = inicio;
				veterinario* aux2 = NULL;

				while (aux1 != NULL) {
					aux2 = aux1;
					aux1 = aux1->sig;
				}
				if (inicio == aux1) {
					inicio = nuevo_nodo;
				}
				else {
					aux2->sig = nuevo_nodo;
					nuevo_nodo->ant = aux2;
				}
				nuevo_nodo->sig = aux1;
				nuevo_nodo->ant = aux2;
				final = nuevo_nodo;
			}
		}
	}
	vets.close();
	return;
}

//Clientes
void agregarCliente(cliente*& inicio, cliente*& final, char nombre[], char apeP[], char apeM[], char cel[], char dir[], char nombreM[], char tipodeEspecie[], char raza[], char fechadeN[], char problemasdeS[]) {
	cliente* nuevo_nodo = new cliente();
	strcpy(nuevo_nodo->nomC, nombre);
	strcpy(nuevo_nodo->apeP, apeP);
	strcpy(nuevo_nodo->apeM, apeM);
	strcpy(nuevo_nodo->celular, cel);
	strcpy(nuevo_nodo->direccion, dir);
	strcpy(nuevo_nodo->nombredeM, nombreM);
	strcpy(nuevo_nodo->tipodeEspecie, tipodeEspecie);
	strcpy(nuevo_nodo->raza, raza);
	strcpy(nuevo_nodo->fechadeN, fechadeN);
	strcpy(nuevo_nodo->problemasdeS, problemasdeS);

	cliente* aux1 = inicio;
	cliente* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicio == aux1) {
		inicio = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	final = nuevo_nodo;
}
void eliminarCliente(cliente*& inicio, cliente*& final, char nombre[], char apeP[], char apeM[]) {
	if (inicio != NULL) {
		cliente* aux_borrar;
		cliente* ante = NULL;
		aux_borrar = inicio;

		while (aux_borrar != NULL && strcmp(aux_borrar->nomC, nombre) && strcmp(aux_borrar->apeP, apeP) && strcmp(aux_borrar->apeM, apeM)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No existe ese cliente", "CLIENTES", MB_OK);
		else if (ante == NULL) {
			inicio = inicio->sig;
			if (aux_borrar->sig != NULL) {
				aux_borrar->sig->ant = NULL;
				final = ante;
			}
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			final = ante;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}
void limpiarCliente(HWND handler) {
	SendMessage(GetDlgItem(handler, IDC_EDIT1), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT6), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT7), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT8), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT9), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT10), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT11), WM_SETTEXT, NULL, (LPARAM)L"");
}
void verCliente(HWND handler, cliente* aux) {
	SendMessage(GetDlgItem(handler, IDC_EDIT1), WM_SETTEXT, 100, (LPARAM)&aux->nomC);
	SendMessage(GetDlgItem(handler, IDC_EDIT2), WM_SETTEXT, 50, (LPARAM)&aux->apeP);
	SendMessage(GetDlgItem(handler, IDC_EDIT3), WM_SETTEXT, 50, (LPARAM)&aux->apeM);
	SendMessage(GetDlgItem(handler, IDC_EDIT5), WM_SETTEXT, 10, (LPARAM)&aux->celular);
	SendMessage(GetDlgItem(handler, IDC_EDIT6), WM_SETTEXT, 100, (LPARAM)&aux->direccion);
	SendMessage(GetDlgItem(handler, IDC_EDIT7), WM_SETTEXT, 20, (LPARAM)&aux->nombredeM);
	SendMessage(GetDlgItem(handler, IDC_EDIT8), WM_SETTEXT, 100, (LPARAM)&aux->tipodeEspecie);
	SendMessage(GetDlgItem(handler, IDC_EDIT9), WM_SETTEXT, 100, (LPARAM)&aux->raza);
	SendMessage(GetDlgItem(handler, IDC_EDIT10), WM_SETTEXT, 25, (LPARAM)&aux->fechadeN);
	SendMessage(GetDlgItem(handler, IDC_EDIT11), WM_SETTEXT, 50, (LPARAM)&aux->problemasdeS);
}

void guardarClientes() {
	ofstream client;
	client.open("clientes.txt", ios::binary | ios::out | ios::trunc);
	if (client.is_open()) {
		auxCl = inicio_cl;
		while (auxCl != NULL) {
			client.write(reinterpret_cast<char*>(auxCl), sizeof(cliente));
			auxCl = auxCl->sig;
		}
	}
	client.close();
	return;
}
void leerClientes(cliente*& inicio, cliente*& final) {
	ifstream client;
	client.open("clientes.txt", ios::binary | ios::in);
	if (client.is_open()) {
		while (!client.eof()) {
			cliente* nuevo_nodo = new cliente();
			client.read(reinterpret_cast<char*>(nuevo_nodo), sizeof(cliente));
			if (nuevo_nodo->nomC[0] != NULL) {
				cliente* aux1 = inicio;
				cliente* aux2 = NULL;

				while (aux1 != NULL) {
					aux2 = aux1;
					aux1 = aux1->sig;
				}
				if (inicio == aux1) {
					inicio = nuevo_nodo;
				}
				else {
					aux2->sig = nuevo_nodo;
					nuevo_nodo->ant = aux2;
				}
				nuevo_nodo->sig = aux1;
				nuevo_nodo->ant = aux2;
				final = nuevo_nodo;
			}
		}
	}
	client.close();
	return;
}
bool repNombredeM(char nombredeM[]) {
	cliente* aux = inicio_cl;
	while (aux != NULL) {
		if (strcmp(aux->nombredeM, nombredeM) == 0) {
			return true;
		}
		aux = aux->sig;
	}
	return false;
}

void RepClientes(cliente* inicio_clA) {
	ofstream client;
	client.open("ClientReport.txt", ios::out | ios::app);
	if (client.is_open()) {
		if (inicio_clA != NULL) {
			RepClientes(inicio_clA->ant);
			client.write("\n", 2);
			client.write(inicio_clA->apeP, sizeof(inicio_clA->apeP));
			client.write(inicio_clA->apeM, sizeof(inicio_clA->apeM));
			client.write("\n", 2);
			client.write(inicio_clA->nomC, sizeof(inicio_clA->nomC));
			client.write("\n", 2);
			RepClientes(inicio_clA->sig);
		}
	}
	client.close();
	return;
}
void swapCl(cliente* a, cliente* b) {
	cliente aux;
	// aux to a
	strcpy(aux.nomC, a->nomC);
	strcpy(aux.apeP, a->apeP);
	strcpy(aux.apeM, a->apeM);
	strcpy(aux.celular, a->celular);
	strcpy(aux.direccion, a->direccion);
	strcpy(aux.nombredeM, a->nombredeM);
	strcpy(aux.tipodeEspecie, a->tipodeEspecie);
	strcpy(aux.raza, a->raza);
	strcpy(aux.fechadeN, a->fechadeN);
	strcpy(aux.problemasdeS, a->problemasdeS);
	// a to b
	strcpy(a->nomC, b->nomC);
	strcpy(a->apeP, b->apeP);
	strcpy(a->apeM, b->apeM);
	strcpy(a->celular, b->celular);
	strcpy(a->direccion, b->direccion);
	strcpy(a->nombredeM, b->nombredeM);
	strcpy(a->tipodeEspecie, b->tipodeEspecie);
	strcpy(a->raza, b->raza);
	strcpy(a->fechadeN, b->fechadeN);
	strcpy(a->problemasdeS, b->problemasdeS);
	// b to aux
	strcpy(b->nomC, aux.nomC);
	strcpy(b->apeP, aux.apeP);
	strcpy(b->apeM, aux.apeM);
	strcpy(b->celular, aux.celular);
	strcpy(b->direccion, aux.direccion);
	strcpy(b->nombredeM, aux.nombredeM);
	strcpy(b->tipodeEspecie, aux.tipodeEspecie);
	strcpy(b->raza, aux.raza);
	strcpy(b->fechadeN, aux.fechadeN);
	strcpy(b->problemasdeS, aux.problemasdeS);
}

//Citas
void agregarCita(citas*& inicio, citas*& final, char nomCl[], char celCl[], char oper[], char nomV[], SYSTEMTIME fecha, char hora[], char clave[], char estado[]) {
	citas* nuevo_nodo = new citas;
	strcpy(nuevo_nodo->nomCl, nomCl);
	strcpy(nuevo_nodo->celCl, celCl);
	strcpy(nuevo_nodo->oper, oper);
	strcpy(nuevo_nodo->nomV, nomV);
	nuevo_nodo->fecha = fecha;
	strcpy(nuevo_nodo->hora, hora);
	strcpy(nuevo_nodo->clave, clave);
	strcpy(nuevo_nodo->estado, "Pendiente");


	citas* aux1 = inicio;
	citas* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicio == aux1) {
		inicio = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	final = nuevo_nodo;
}
void eliminarCita(citas*& inicio, citas*& final, char clave[]) {
	if (inicio != NULL) {
		citas* aux_borrar;
		citas* ante = NULL;
		aux_borrar = inicio;

		while (aux_borrar != NULL && strcmp(aux_borrar->clave, clave)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "Cita no encontrada", "CITAS", MB_OK);
		else if (ante == NULL) {
			inicio = inicio->sig;
			if (aux_borrar->sig != NULL) {
				aux_borrar->sig->ant = NULL;
				final = ante;
			}
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			final = ante;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}
void limpiarCita(HWND handler) {
	SendMessage(GetDlgItem(handler, IDC_COMBO1), CB_RESETCONTENT, NULL, NULL);
	SendMessage(GetDlgItem(handler, IDC_EDIT4), WM_SETTEXT, 10, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_COMBO2), CB_RESETCONTENT, 0, NULL);
	SendMessage(GetDlgItem(handler, IDC_COMBO3), CB_RESETCONTENT, 0, NULL);
	SendMessage(GetDlgItem(handler, IDC_VET), STM_SETIMAGE, IMAGE_BITMAP, NULL);
	SendMessage(GetDlgItem(handler, IDC_DATETIMEPICKER1), NM_KILLFOCUS, NULL, NULL);
	SendMessage(GetDlgItem(handler, IDC_EDIT1), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(handler, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
}
void guardarCitas() {
	ofstream dates;
	dates.open("citas.txt", ios::binary | ios::out | ios::trunc); 
	if (dates.is_open()) {
		auxC = inicio_c;
		while (auxC != NULL) {
			dates.write(reinterpret_cast<char*>(auxC), sizeof(citas));
			auxC = auxC->sig;
		}
	}
	dates.close();
	return;
}
void leerCitas(citas*& inicio, citas*& final) {
	ifstream dates;
	dates.open("citas.txt", ios::binary | ios::in); 
	if (dates.is_open()) {
		while (!dates.eof()) {
			citas* nuevo_nodo = new citas();
			dates.read(reinterpret_cast<char*>(nuevo_nodo), sizeof(citas));  
			if (nuevo_nodo->nomCl[0] != NULL) { 
				citas* aux1 = inicio; 
				citas* aux2 = NULL; 
				while (aux1 != NULL) {
					aux2 = aux1;
					aux1 = aux1->sig;
				}
				if (inicio == aux1) {
					inicio = nuevo_nodo;
				}
				else {
					aux2->sig = nuevo_nodo;
					nuevo_nodo->ant = aux2;
				}
				nuevo_nodo->sig = aux1;
				nuevo_nodo->ant = aux2;
				final = nuevo_nodo;
			}
		}
	}
	dates.close();  
	return;
}

cliente* buscarNum(char nomCl[]) {
	cliente* aux = inicio_cl;
	while (aux != NULL) {
		if (strcmp(aux->nomC, nomCl) == 0) {
			return aux;
		}
		aux = aux->sig;
	}
	return aux;
}
citas* buscarCita(char codigo[]) {
	citas* aux = inicio_c;
	while (aux != NULL) {
		if (strcmp(aux->clave, codigo) == 0) {
			return aux;
		}
		aux = aux->sig;
	}
	return aux;
}

