/*Modificar el código de ejemplo para que la estructura de datos que se pasa como
param a la función que ejecutan los threads sea ahora un objeto de una clase que
contenga un bool, dos int, un string y un puntero a char. La clase al menos debe
tener constructor con estos params y sobrecargar “<<” para hacer conversión a
string rellenando adecuadamente el siguiente fragmento de código:
ostream& operator << (ostream& os, const MyData& md) {
    return (os << std::endl);
}*/
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include<string>
#include<sstream>
using namespace std;

#define MAX_THREADS 3
#define BUF_SIZE 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void ErrorHandler(LPCTSTR lpszFunction);

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef class MyData {
public:
    bool b;
    int tid; //thread number(position) in our array
    int val;
    string s;
    char* pchar;
    MyData() {}
    /*~MyData() { // this is already defined 'automagically', and deletes in reverse order all the params
        delete s;
        delete[] pchar;
    }*/
    MyData(bool bb, int tidd, int vall, string ss, char* pcharr) {
        b = bb;
        tid = tidd;
        val = vall;
        s = ss;
        pchar = pcharr;
    }
} *PMYDATA;

ostream& operator << (ostream& os, const MyData& md) {
    return (os << "b: " << md.b << "\n tid: " << md.tid << "\n val: " << md.val << std::endl);
}

int main() { //_tmain
    PMYDATA pDataArray[MAX_THREADS]; //pointers to the data passed as params for each thread so that they can run the function with the params
    DWORD   dwThreadIdArray[MAX_THREADS]; //ids of threads as a long
    HANDLE  hThreadArray[MAX_THREADS]; //handlers of threads

    // Create MAX_THREADS worker threads.

    for (int i = 0; i < MAX_THREADS; i++) {

        // Generate unique data for each thread to work with.
        char adios[6] = { 'a', 'd', 'i', 'o','s', '\0' };
        pDataArray[i] = new MyData(false, i, i + 100, "hola", adios); //this hides the memory allocation in the heap, and also generates data
        //allocation of data and generation of data could be in a different function if complex

        // Create the thread to begin execution on its own.
        hThreadArray[i] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MyThreadFunction,       // thread function name
            (void*)pDataArray[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 

        // Check the return value for success.
        // If CreateThread fails, terminate execution. 
        // This will automatically clean up threads and memory. 

        if (hThreadArray[i] == NULL) {
            ErrorHandler(TEXT("CreateThread"));
            ExitProcess(3);
        }
    } // End of main thread creation loop.

    // Wait until all threads have terminated.
    //how many to wait for, array of handlers, wait for all of them, how long to wait
    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    // Close all thread handles and free memory allocations.
    //this could be in a different function
    for (int i = 0; i < MAX_THREADS; i++) {
        CloseHandle(hThreadArray[i]);
        if (pDataArray[i] != NULL) {
            //HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            delete pDataArray[i];
            //pDataArray[i]->~MyData(); // NEVER(almost) invoke directly the dtor
                //pDataArray[i] = NULL;    // Ensure address is not reused.
        }
    }

    return 0;
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    HANDLE hStdout;
    PMYDATA pDataArray;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;

    // Make sure there is a console to receive output results. 

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
        return 1;

    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because 
    // it was checked for NULL before the thread was created.

    pDataArray = (PMYDATA)lpParam;

    //make appropiate operations with the params
    pDataArray->val *= 2;

    // Print the parameter values using thread-safe functions to console.
    //In concurrent programs it is important to be able to identify who says what, because the order of execution is random
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Thread[%d]: %d\n"), pDataArray->tid, pDataArray->val);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

    return 0;
}



void ErrorHandler(LPCTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}