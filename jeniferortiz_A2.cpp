#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;
int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sql", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Conectado a la base de datos exitosamente." << endl;
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        wcout << L"+----+---------------------------+------------+------------+-----------------------------+----------------------+----------+ " << endl;
        wcout << L"| No.| Nombre completo           | f. nac.    | RFC        | Centro                      | Descripcion          |Directivos| " << endl;
        wcout << L"+----+---------------------------+------------+------------+-----------------------------+----------------------+----------+ " << endl;

        //Select a la base de datos
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLWCHAR name[50];
            SQLWCHAR last_name[50];
            SQLWCHAR slast_name[50];
            SQLWCHAR RFC[20];
            SQLWCHAR centro_trabajo[50];
            SQLWCHAR puesto[30];
            SQLWCHAR descripcion_puesto[50];
            SQLWCHAR directivo[10];



            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_WCHAR, name, sizeof(name), NULL);
                SQLGetData(hStmt, 3, SQL_C_WCHAR, last_name, sizeof(last_name), NULL);
                SQLGetData(hStmt, 4, SQL_C_WCHAR, slast_name, sizeof(slast_name), NULL);
                SQLGetData(hStmt, 5, SQL_C_WCHAR, RFC, sizeof(RFC), NULL);
                SQLGetData(hStmt, 6, SQL_C_WCHAR, centro_trabajo, sizeof(centro_trabajo), NULL);
                SQLGetData(hStmt, 7, SQL_C_WCHAR, puesto, sizeof(puesto), NULL);
                SQLGetData(hStmt, 8, SQL_C_WCHAR, descripcion_puesto, sizeof(descripcion_puesto), NULL),
                SQLGetData(hStmt, 9, SQL_C_WCHAR, directivo, sizeof(directivo), NULL);


                //concatenar apellido paterno + apellido materno
                wstring last_names = wstring(last_name) + L" " + wstring(slast_name);
                wstring full_name = wstring(name) + L" " + wstring(last_names);
                wstring dir = wstring(directivo);
                wstring respuesta;

                if (dir == L"1") {
                    respuesta = L"Si";

                }
                else
                {
                    respuesta = L"No";
                }


                //Imprimir datos de la fila con alineacion
                wcout << L"| " << setw(3) << left << num_empleado
                    << L"| " << setw(26) << left << full_name
                    << L"| " << setw(11) << left << RFC
                    << L"| " << setw(11) << left << centro_trabajo
                    << L"| " << setw(28) << left << puesto
                    << L"| " << setw(21) << left << descripcion_puesto
                    << L"| " << setw(8) << left << respuesta << L" |" << endl;



            }
            wcout << L"+----+---------------------------+------------+------------+-----------------------------+---------------------------------+" << endl;
        }
        // Liberar el manejador de conexión
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }
    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    
    return 0;
}