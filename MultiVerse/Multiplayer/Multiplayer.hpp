#ifndef __MULTIPLAYER_HPP__
#define __MULTIPLAYER_HPP__

#include "SFMLfunctions.hpp"
#include "Player.hpp"

#define NETWORKPORT 4346
#define NETWORKFREQ 0.1

/// Upnp port forwarding

class CharacterData
{
    public:
    Vector2f Position;
    Vector2f Momentum;
    bool Direction;

    void UpdateData(PlayerClass & p)
    {
        Position = p.Position;
        Direction = p.Direction;
        Momentum = p.Momentum;
    }

    void UpdateCharacter(PlayerClass & p)
    {
        p.Position = Position;
        p.Direction = Direction;
        p.Momentum = Momentum;
    }

//    void BuildPacket(Packet & p)
//    {
//        p << sf::Uint16(Position.x) << sf::Uint16(Position.y) << Direction;
//    }
//
//    void OpenPacket(Packet & p)
//    {
//        sf::Uint16 x, y;
//        bool dir;
//
//        p >> x >> y >> dir;
//
//        Position.x = (float)x;
//        Position.y = (float)y;
//        Direction = dir;
//    }

    void BuildPacket(Packet & p)
    {
        p << (int)Position.x << (int)Position.y << Direction << Momentum.x << Momentum.y;
    }

    void OpenPacket(Packet & p)
    {
        int x, y;
        p >> x >> y >> Direction >> Momentum.x << Momentum.y;

        Position.x = x;
        Position.y = y;
    }
};

class NetworkHandler
{
    public:
    /// Connection
    bool isConnected;
    bool isReady;
    SocketTCP mySocket;
    IPAddress ConnectionAddress;

    /// PreGame
    string LevelFile;

    string myHostName;
    int myCharacterImage;

    string HostName;
    int CharacterImage;

    /// Gameplay
    CharacterData myData;
    CharacterData Data;
};

class ServerNetworkHandler : public NetworkHandler
{
    public:
    SocketTCP ClientSocket;

    ServerNetworkHandler(string name = "Server")
    {
        myHostName = name;
        isConnected = false;
        isReady = false;
    }

//    void Listen(void* UserData)
//    {
//        if(!mySocket.Listen(NETWORKPORT))
//            return;
//        std::cout << "Server is listening to port " << NETWORKPORT << ", waiting for connections... " << std::endl;
//
//        // Wait for a connection
//        if(mySocket.Accept(ClientSocket, &ConnectionAddress) != sf::Socket::Done)
//            return;
//        std::cout << "Client connected : " << ConnectionAddress << std::endl;
//        isConnected = true;
//    }

    void SyncWithClient()
    {
        {
            Packet p;

            string Command = "SyncData";
            p << Command << myHostName << myCharacterImage << LevelFile;

            if(ClientSocket.Send(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Syncing with Client\n";
                return ;
            }
        }

        Sleep(0.1f);

        {
            Packet p;

            if(ClientSocket.Receive(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Syncing with Client\n";
                return ;
            }

            string Command;
            p >> Command;
            if(Command == "SyncData")
            {
                cout << "\nSyncing Data\n";
                p >> HostName >> CharacterImage;
            }
        }

        isReady = true;
    }

    void SendCharacterUpdate()
    {
        {
            Packet p;
            myData.BuildPacket(p);

            if(ClientSocket.Send(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Updating Client\n";
                return ;
            }
        }

        //sf::Sleep(0.02);

        {
            Packet p;

            if(ClientSocket.Receive(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Updating Client\n";
                return ;
            }

            Data.OpenPacket(p);
        }
    }

    ~ServerNetworkHandler()
    {
        mySocket.Close();
        ClientSocket.Close();
    }

    friend void Listen(void* UserData);
};

void Listen(void* UserData)
{
    ServerNetworkHandler * ServerHandler = static_cast<ServerNetworkHandler*>(UserData);

    if(!ServerHandler->mySocket.Listen(NETWORKPORT))
        return;
    std::cout << "Server is listening to port " << NETWORKPORT << ", waiting for connections... " << std::endl;

    // Wait for a connection
    if(ServerHandler->mySocket.Accept(ServerHandler->ClientSocket, &ServerHandler->ConnectionAddress) != sf::Socket::Done)
        return;

    std::cout << "Client connected : " << ServerHandler->ConnectionAddress << std::endl;
    ServerHandler->isConnected = true;

    ServerHandler->SyncWithClient();
}

class ClientNetworkHandler : public NetworkHandler
{
    public:
    ClientNetworkHandler(string name = "Client")
    {
        myHostName = name;
        isConnected = false;
        isReady = false;
    }

    void ConnectToServer()
    {
        if (mySocket.Connect(NETWORKPORT, ConnectionAddress) != sf::Socket::Done)
            return;
        std::cout << "Connected to server " << ConnectionAddress << std::endl;
        isConnected = true;

        SyncWithServer();
    }

    void SyncWithServer()
    {
        {
            Packet p;

            if(mySocket.Receive(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Syncing with Server\n";
                return ;
            }

            string Command;
            p >> Command;
            if(Command == "SyncData")
            {
                cout << "\nSyncing Data\n";
                p >> HostName >> CharacterImage >> LevelFile;
            }
        }

        Sleep(0.1f);

        {
            Packet p;

            string Command = "SyncData";
            p << Command << myHostName << myCharacterImage;

            if(mySocket.Send(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Syncing with Server\n";
                return ;
            }
        }

        isReady = true;
    }


    void SendCharacterUpdate()
    {
        {
            Packet p;

            if(mySocket.Receive(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Updating Server\n";
                return ;
            }

            Data.OpenPacket(p);
        }

        //sf::Sleep(0.02);

        {
            Packet p;
            myData.BuildPacket(p);

            if(mySocket.Send(p) != sf::Socket::Done)
            {
                cerr << "\nProblem Encountered While Updating Server\n";
                return ;
            }
        }
    }

    ~ClientNetworkHandler()
    {
        mySocket.Close();
    }
};

#endif
