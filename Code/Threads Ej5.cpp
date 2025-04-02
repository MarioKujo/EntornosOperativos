/*Modificar el código de ejemplo para que ahora el master thread
cree N threads (nivel 1) que a su vez creen M threads (nivel 2).
Harán falta dos funciones para los threads diferentes, una para los
de nivel 1 y otra para los de nivel 2. La función de nivel 2 debe
imprimir por pantalla: “Thread[m] son of [n], son of master, x”
donde m es el index del thread de nivel 2, n el index de su padre y
x es 2^(n+1)^(m+1) donde n y m son los índices correspondientes a 
thread y subthread.
Consejo: master que tenga id = 0, hijos 0..n y nietos 0..m
Con n=3 y m=5, sale el número mayor 2^(2+1)^(4+1) = 32768*/
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include<string>
#include<sstream>
#include<cmath>
using namespace std;

#define MAX_THREADS 3
#define MAX_SUBTHREADS 5 // this MUST be >= than the param in MyData, otw will fail
#define BUF_SIZE 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
DWORD WINAPI MyThreadFunctionSub(LPVOID lpParam);
void ErrorHandler(LPCTSTR lpszFunction);

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef class MyData {
public:
    int parent;
    int tid; //thread number(position) in our array
    int max_subthreads;
    int data;
    MyData() {}
    /*~MyData() { // this is already defined 'automagically', and deletes in reverse order all the params
        delete s;
        delete[] pchar;
    }*/
    MyData(int parentt, int tidd, int max_subthreadss, int dataa) {
        parent = parentt;
        tid = tidd;
        max_subthreads = max_subthreadss;
        data = dataa;
    }
} *PMYDATA;

ostream& operator << (ostream& os, const MyData& md) {
    return (os << "parent: " << md.parent << "\n tid: " << md.tid << "\n max_subthreads : " << md.max_subthreads << "\n data: " << md.data << std::endl);
}

int main() { //_tmain
    PMYDATA pDataArray[MAX_THREADS]; //pointers to the data passed as params for each thread so that they can run the function with the params
    DWORD   dwThreadIdArray[MAX_THREADS]; //ids of threads as a long
    HANDLE  hThreadArray[MAX_THREADS]; //handlers of threads

    // Create MAX_THREADS worker threads.

    for (int i = 0; i < MAX_THREADS; i++) {

        // Generate unique data for each thread to work with.
        pDataArray[i] = new MyData(0, i, MAX_SUBTHREADS, i + 100); //this hides the memory allocation in the heap, and also generates data
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
    }

    return 0;
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    HANDLE hStdout;
    PMYDATA pData;

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

    pData = (PMYDATA)lpParam;

    //make appropiate operations with the params
    pData->data = pow(2, pData->parent + 1);

    // Print the parameter values using thread-safe functions to console.
    //In concurrent programs it is important to be able to identify who says what, because the order of execution is random
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Thread[%d], son of master: %d\n"), pData->tid, pData->data);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

    const int max_subs = pData->max_subthreads;
    PMYDATA pDataArraySub[MAX_SUBTHREADS]; //pointers to the data passed as params for each thread so that they can run the function with the params
    DWORD   dwThreadIdArraySub[MAX_SUBTHREADS]; //ids of threads as a long
    HANDLE  hThreadArraySub[MAX_SUBTHREADS]; //handlers of threads

    // Create MAX_THREADS worker threads.

    for (int i = 0; i < max_subs; i++) {//only to the real param

        // Generate unique data for each thread to work with.
        pDataArraySub[i] = new MyData(pData->tid, i, max_subs, pData->data); //this hides the memory allocation in the heap, and also generates data
        //allocation of data and generation of data could be in a different function if complex

        // Create the thread to begin execution on its own.
        hThreadArraySub[i] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MyThreadFunctionSub,       // thread function name
            (void*)pDataArraySub[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArraySub[i]);   // returns the thread identifier 

        // Check the return value for success.
        // If CreateThread fails, terminate execution. 
        // This will automatically clean up threads and memory. 

        if (hThreadArraySub[i] == NULL) {
            ErrorHandler(TEXT("CreateThread"));
            ExitProcess(3);
        }
    } // End of main thread creation loop.

    // Wait until all threads have terminated.
    //how many to wait for, array of handlers, wait for all of them, how long to wait
    WaitForMultipleObjects(max_subs, hThreadArraySub, TRUE, INFINITE);

    // Close all thread handles and free memory allocations.
    //this could be in a different function
    for (int i = 0; i < MAX_THREADS; i++) {
        CloseHandle(hThreadArraySub[i]);
    }

    //free the mem of my own param
    if (pData != NULL) {
        delete pData;
    }

    return 0;
}

DWORD WINAPI MyThreadFunctionSub(LPVOID lpParam) {
    HANDLE hStdout;
    PMYDATA pData;

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

    pData = (PMYDATA)lpParam;

    //make appropiate operations with the params
    //pData->data = pow(pData->data, pData->tid+1);
    pData->data = pow(pow(2, pData->parent + 1), pData->tid + 1);

    // Print the parameter values using thread-safe functions to console.
    //In concurrent programs it is important to be able to identify who says what, because the order of execution is random
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Thread[%d] son of [%d], son of master, %d\n"), pData->tid, pData->parent, pData->data);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

    //free the mem of my own param
    if (pData != NULL) {
        delete pData;
    }

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


