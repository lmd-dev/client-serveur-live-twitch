#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>

int main()
{
	WSADATA data;
	WSAStartup(2, &data);

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serverData;
	//TCP/IP
	serverData.sin_family = AF_INET;
	//Port du server à contacter
	serverData.sin_port = htons(3000);
	//Adresse du serveur à contacter
	inet_pton(AF_INET, "127.0.0.1", &serverData.sin_addr.s_addr);

	//Connexion au serveur
	if (connect(socketClient, (SOCKADDR*)&serverData, sizeof(serverData)) != SOCKET_ERROR)
	{
		std::cout << "Connexion reussie !" << std::endl;

		std::string message = "Charles, Oscours !";

		//Envoie le message au serveur
		send(socketClient, message.c_str(), message.length(), 0);

		//Création d'un buffer de 5000 octets
		int bufferSize = 5000;
		char* buffer = new char[bufferSize];
		memset(buffer, 0, bufferSize);

		//REcoie les données en provenance du serveur
		recv(socketClient, buffer, bufferSize, 0);

		std::cout << buffer << std::endl;

		//Suppression du buffer
		delete[] buffer;
	}

	//Fermeture de la bibliothèque WinSock2
	WSACleanup();

	return 0;
}