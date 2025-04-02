// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include "pch.h"
#include "framework.h"
#include "UDPLib.h"


// TODO: Ejemplo de una función de biblioteca
std::ostream& operator << (std::ostream& os, const DataPacket& dp) {
    return (os << "DataPacket{client_id: "<< dp.client_id << " sequence: " << dp.sequence << " msg : " << dp.msg << " }");
}

//will print msg with WSAGetLastError, then closesocket
void treatError(const std::string msg, SOCKET s) {
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
}

//will print msg with WSAGetLastError, then closesocket, WSACleanup and ExitProcess(error)
void treatErrorExit(const std::string msg, SOCKET s, int error) {
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
    WSACleanup();
    ExitProcess(error);
}

