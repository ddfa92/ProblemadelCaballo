#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//Prototipos Interfaz gráfica
LRESULT CALLBACK Procedimientos(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);
void loadImages();
//Variables Interfaz gráfica
HWND hTablero[64];
HBITMAP hImagenCuadradoBlanco;
HBITMAP hImagenCuadradoGris;
int yajugo=0;

//Prototipos algoritmo
int movimiento(int); //interpreta los movimientos del caballo en en el tablero como movimientos en un vector
void mover(); // Hace que el caballo se mueva por el tablero
void inicializarTablero(); //Inicializa el tablero
//Variables algoritmo
int posInicial=4; //Indica la posición inicial del caballo; DEBE ESTAR DEFINIDO
int posActual; // Indica la posición actual del caballo
int tablero[64]; //Vector que guardará el tablero
int movs[]={3,4,7,5,3,4,7,6,7,8,3,1,7,8,3,5,8,2,4,5,6,4,2,3,2,8,6,1,2,8,6,6,1,6,5,4,5,2,1,3,5,2,1,8,1,6,4,1,7,5,4,3,5,7,6,7,1,3,8,7,1,3,4};


///////////////////////////////////////////////////////////////////////////////////


int WINAPI WinMain(HINSTANCE hInst,
                   HINSTANCE hPrevInst,
                   LPSTR pCmdLine,
                   int nCmdShow)
{

    const wchar_t NOMBRE_DE_CLASE[] = L"miClaseDeVentana";

    WNDCLASSW wc= { }; //Es una estructura, la estamos inicializando

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW ; //define el color de fondo de la ventana
    wc.hCursor = LoadCursor(NULL,IDC_ARROW); //carga el cursor
    wc.hInstance = hInst;

    wc.lpszClassName = NOMBRE_DE_CLASE; //Nombre de la clase de esta ventana
    wc.lpfnWndProc = Procedimientos; //Nombre de la función que nos manejará los mensajes

    if (!RegisterClassW(&wc))
        return -1;
    //              TIPO DE VENTANA, TITULO, ESTILO, POSICION X, POSICION Y, TAMAÑO X, TAMAÑO Y,
    CreateWindowW(NOMBRE_DE_CLASE, L"El titulo", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50,50, 660, 680, NULL, NULL, NULL, NULL);

    MSG msg = { }; //inicializa un mensaje

    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK Procedimientos(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch (msg)
    {
        case WM_CREATE:
            loadImages();
            AddControls(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_CLOSE:
            if (MessageBox(hWnd, L"Esta seguro de que quiere cerrar la aplicacion?", L"Problema del caballo", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hWnd);
            }
            break;



        case WM_KEYDOWN:
            switch(wp){
            case 0x26:
                if (yajugo==0){
                    mover();
                    yajugo=1;
                }
                else{
                    DestroyWindow(hWnd);
                }
                break;
                }
            break;

        default:
            return DefWindowProc(hWnd, msg, wp, lp);
    }//end switch
}


//Funcion para los controles Estáticos y Editables, ventanas hijas
void AddControls(HWND hWnd)
{
    int i,j;
    for (int k=0;k<64;k++){
        i= k%8;
        j= (k - i)/8;
        hTablero[k] =CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP, 80*i,80*j,80,80,hWnd,NULL,NULL,NULL);
        SendMessage(hTablero[k],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) hImagenCuadradoBlanco);
    }
}   //WS : Window Style, SS: Static Style, ES: Edit Style


void loadImages()
{
    hImagenCuadradoBlanco = (HBITMAP)LoadImageW(NULL, L"cuadrado.bmp",IMAGE_BITMAP,80,80,LR_LOADFROMFILE);
    hImagenCuadradoGris = (HBITMAP)LoadImageW(NULL, L"cuadrado2.bmp",IMAGE_BITMAP,80,80,LR_LOADFROMFILE);
}

//----------------------------Funciones algoritmo
int movimiento(int x){
    int y=0;
    switch (x){
    case 1:
        y=-15;
        break;
    case 2:
        y=-6;
        break;
    case 3:
        y=10;
        break;
    case 4:
        y=17;
        break;
    case 5:
        y=15;
        break;
    case 6:
        y=6;
        break;
    case 7:
        y=-10;
        break;
    case 8:
        y=-17;
        break;
    default:
        break;
    }
	return y;
}


//Llenamos el tablero de 0, lo que indica que el caballo no ha estado en ninguna posición del tablero.
void inicializarTablero(){
    for (int i=0;i<64;i++){
        tablero[i]=0;
        SendMessage(hTablero[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) hImagenCuadradoBlanco);
    }
}

void mover(){
    inicializarTablero();
    posActual=posInicial; //La posición actual comienza con la posición inicial
    tablero[posActual]++; //marcamos que el caballo pasó por la posición inicial
    SendMessage(hTablero[posActual],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) hImagenCuadradoGris);
    Sleep(500);
    for (int i=0;i<63;i++){
        posActual=posActual + movimiento(movs[i]); //actualizamos la posición actual
        tablero[posActual]++; //marcamos que el caballo pasó por la posición actual
        SendMessage(hTablero[posActual],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) hImagenCuadradoGris);
        Sleep(500);
    }
}

