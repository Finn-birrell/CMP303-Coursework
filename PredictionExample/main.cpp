#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <vector>
#include <iostream>


int main()
{
	std::vector<sf::TcpSocket*> clientVector;

	sf::TcpListener	TcpListner;
	TcpListner.listen(7777);

	sf::SocketSelector SocketSelector;
	SocketSelector.add(TcpListner);

	sf::Packet packet;
	sf::Packet sendPackets;
	bool boolean = true;
	int ID;

	while (boolean)
	{
		if (SocketSelector.wait())
		{
			if (SocketSelector.isReady(TcpListner))
			{
				if (clientVector.size() < 2)
				{
					sf::TcpSocket* socket = new sf::TcpSocket;
					if (TcpListner.accept(*socket) == sf::Socket::Done)
					{
						if (socket->receive(packet) == sf::Socket::Done)
						{
						}
						clientVector.push_back(socket);
						SocketSelector.add(*socket);
						packet.clear();
						ID = clientVector.size();
						packet << ID;
						socket->send(packet);

					}
				}
			}
			else
			{
				for (int i = 0; i < clientVector.size(); ++i)
				{
					if (SocketSelector.isReady(*clientVector[i]))
					{
						sf::Socket::Status Status = clientVector[i]->receive(packet);
						switch (Status)
						{
						case sf::Socket::Done:
							for (int k = 0; k < clientVector.size(); k++)
							{
								if (i != k)
								{
									clientVector[k]->send(packet);
								}
							}
							break;
						case sf::Socket::Disconnected:
							SocketSelector.remove(*clientVector[i]);
							clientVector.erase(find(clientVector.begin(), clientVector.end(), clientVector[i]));
							break;

						default:
							break;
						}
					}
				}
			}
		}
	}



	return 0;
}