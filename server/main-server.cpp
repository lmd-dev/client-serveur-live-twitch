#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <sstream>
#include <thread>

/**
 * Traite la requete d'un client - THREAD
 */
void processRequest(SOCKET socketClient)
{
	if (socketClient != INVALID_SOCKET)
	{
		std::cout << "Nouvelle connexion" << std::endl;

		//Cr�ation d'un buffer de 5000 octets initialis� � 0
		int bufferSize = 5000;
		char* buffer = new char[bufferSize];
		memset(buffer, 0, bufferSize);

		//R�ception des donn�es envoy�es par le client
		recv(socketClient, buffer, bufferSize, 0);
		std::cout << buffer << std::endl;

		//Cr�ation de la r�ponse au format HTTP 1.1
		std::string html = "<!DOCTYPE html><html><head><title>Les Moulins du Dev</title></head><body>Hello World !</body></html>";
		std::stringstream response;
		response << "HTTP/1.1 200 OK\r\n";
		response << "Content-Type: text/html\r\n";
		response << "Content-Length: " << html.length() << "\r\n";
		response << "\r\n";
		response << html;

		//Envoie de la r�ponse au client
		send(socketClient, response.str().c_str(), response.str().length(), 0);

		//Suppression du buffer
		delete[] buffer;

		//Fermeture de la socket
		closesocket(socketClient);
	}
}

int main()
{
	//Initialisation de la biblioth�que WinSock2
	WSADATA data;
	WSAStartup(2, &data);

	//Cr�ation de la socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);

	//Donn�es associ� � la socket pour la configurer en socket server
	SOCKADDR_IN serverData;
	serverData.sin_addr.s_addr = htonl(ADDR_ANY);
	serverData.sin_family = AF_INET;					// TCP IP
	serverData.sin_port = htons(3000);					// Port du serveur

	//Attache les donn�es pr�c�dentes � la socket
	bind(socketServer, (SOCKADDR*)&serverData, sizeof(serverData));

	//D�marre l'�coute du serveur sur le port
	listen(socketServer, 0);

	while (1)
	{
		SOCKADDR_IN clientData;
		int clientDataSize = sizeof(clientData);

		//Attend la connexion d'un client
		SOCKET socketClient = accept(socketServer, (SOCKADDR*)&clientData, &clientDataSize);

		new std::thread(&processRequest, socketClient);		
	}

	//Ferme la biblioth�que WinSock2
	WSACleanup();

	return 0;
}