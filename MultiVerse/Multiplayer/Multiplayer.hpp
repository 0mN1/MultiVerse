#ifndef __MULTIPLAYER_HPP__
#define __MULTIPLAYER_HPP__

#include "SFMLfunctions.hpp"
#include "Player.hpp"

#define NETWORKPORT 4709
#define NETWORKFREQ 0.1

/// Upnp port forwarding

class SynchronizationData
{
    public:
    Vector2f Position;
    Vector2f Momentum;
    bool Direction;
    bool newMsg;
    string Msg;
    Vector2f MsgPos;
    int MsgR, MsgG, MsgB;

    SynchronizationData()
    {
        newMsg = false;
    }

    void SendMessage(string s, Vector2f p, int R, int G, int B)
    {
        newMsg = true;
        Msg = s;
        MsgPos = p;
        MsgR = R;
        MsgG = G;
        MsgB = B;
    }

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

    /*void BuildPacket(Packet & p)
    {
        p << sf::Uint16(Position.x) << sf::Uint16(Position.y) << Direction;
    }

    void OpenPacket(Packet & p)
    {
        sf::Uint16 x, y;
        bool dir;

        p >> x >> y >> dir;

        Position.x = (float)x;
        Position.y = (float)y;
        Direction = dir;
    }*/

    void BuildPacket(Packet & p)
    {
        p << Position.x << Position.y << Direction << Momentum.x << Momentum.y;
        p << newMsg;
        if(newMsg)
        {
            p << Msg << MsgPos.x << MsgPos.y << MsgR << MsgG << MsgB;
            newMsg = false;
            Msg = "";
        }
    }

    void OpenPacket(Packet & p)
    {
        p >> Position.x >> Position.y >> Direction >> Momentum.x >> Momentum.y;

        p >> newMsg;

        if(newMsg)
        {
            cout << "New Msg\n";
            p >> Msg >> MsgPos.x >> MsgPos.y >> MsgR >> MsgG >> MsgB;
        }
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
    SynchronizationData myData;
    SynchronizationData Data;
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

class ChatBox
{
    protected:
    bool Active;
    Sprite Bubble;
    string S1;
    String Str;
    Vector2f Position;
    Vector2f Origin;
    Vector2f ViewPosition;
    Vector2f Size;

    public:
    ChatBox(float xPos = 0, float yPos = 0, Image & bubble)
    {
        Origin = Vector2f(xPos, yPos);
        Position = ViewPosition + Origin;
        Active = true;

        Bubble.SetImage(bubble);
        Bubble.SetColor(Color(200, 200, 200, 200));

        Str = String("", Font::GetDefaultFont(), 22);
        Str.SetPosition(Position.x, Position.y);
        Str.SetColor(Color(20, 20, 20));
    }

    bool     &  GetActive   ()  {return Active;   }
    Sprite   &  GetBubble   ()  {return Bubble;   }
    string   &  GetString   ()  {return S1;       }
    String   &  GetStr      ()  {return Str;      }
    Vector2f &  GetPosition ()  {return Position; }
    Vector2f &  GetSize     ()  {return Size;     }

    void SetPosition(Vector2f pos) {Origin = pos;}

    void SetS1(string s)
    {
        S1 = s;
        Str.SetText(S1);
    }

    bool & SetActive(bool New)
    {
        if(Active != New && New)
        {
            Str.SetColor(Color(20, 20, 20));
            Active = true;
        }
        else
        if(Active != New && !New)
        {
            Active = false;

            if(S1.empty())
            {
                Str.SetColor(Color(150,150,150));
            }
            else
            Str.SetColor(Color(75, 75, 75));
        }

        return Active;
    }

    bool Clicked()
    {
        if(Position.x < Cursor.Position.x && Cursor.Position.x < Position.x + Size.x
        && Position.y < Cursor.Position.y && Cursor.Position.y < Position.y + Size.y)
        {
            Str.SetColor(Color(20, 20, 20));
            Active = true;

            return true;
        }
        else
        {
            Active = false;

            if(S1.empty())
            {
                Str.SetColor(Color(150, 150, 150));
            }
            else
            Str.SetColor(Color(75, 75, 75));

            return false;
        }
    }

    bool Write()
    {
        using namespace GUI;
        if(Active)
        {
            if(LShiftKey.Hold || RShiftKey.Hold)
            {
                if(AKey.Pressed)
                S1 += "A";
                if(BKey.Pressed)
                S1 += "B";
                if(CKey.Pressed)
                S1 += "C";
                if(DKey.Pressed)
                S1 += "D";
                if(EKey.Pressed)
                S1 += "E";
                if(FKey.Pressed)
                S1 += "F";
                if(GKey.Pressed)
                S1 += "G";
                if(HKey.Pressed)
                S1 += "H";
                if(IKey.Pressed)
                S1 += "I";
                if(JKey.Pressed)
                S1 += "J";
                if(KKey.Pressed)
                S1 += "K";
                if(LKey.Pressed)
                S1 += "L";
                if(MKey.Pressed)
                S1 += "M";
                if(NKey.Pressed)
                S1 += "N";
                if(OKey.Pressed)
                S1 += "O";
                if(PKey.Pressed)
                S1 += "P";
                if(QKey.Pressed)
                S1 += "Q";
                if(RKey.Pressed)
                S1 += "R";
                if(SKey.Pressed)
                S1 += "S";
                if(TKey.Pressed)
                S1 += "T";
                if(UKey.Pressed)
                S1 += "U";
                if(VKey.Pressed)
                S1 += "V";
                if(WKey.Pressed)
                S1 += "W";
                if(XKey.Pressed)
                S1 += "X";
                if(YKey.Pressed)
                S1 += "Y";
                if(ZKey.Pressed)
                S1 += "Z";

                if(Num1Key.Pressed)
                S1 += "!";
                if(Num2Key.Pressed)
                S1 += "\"";
                if(Num3Key.Pressed)
                S1 +="#";
                if(Num4Key.Pressed)
                S1 += "¤";
                if(Num5Key.Pressed)
                S1 += "%";
                if(Num6Key.Pressed)
                S1 += "&";
                if(Num7Key.Pressed)
                S1 +="/";
                if(Num8Key.Pressed)
                S1 += "(";
                if(Num9Key.Pressed)
                S1 += ")";
                if(Num0Key.Pressed)
                S1 += "=";
                if(SpaceKey.Pressed)
                S1 +=" ";
                if(PeriodKey.Pressed)
                S1 += ":";
                if(CommaKey.Pressed)
                S1 += ";";
                if(AddKey.Pressed)
                S1 += "?";

                if(BackKey.Pressed&&!S1.empty())S1.erase();
            }
            else
            {
                if(AKey.Pressed)
                S1 += "a";
                if(BKey.Pressed)
                S1 += "b";
                if(CKey.Pressed)
                S1 += "c";
                if(DKey.Pressed)
                S1 += "d";
                if(EKey.Pressed)
                S1 += "e";
                if(FKey.Pressed)
                S1 += "f";
                if(GKey.Pressed)
                S1 += "g";
                if(HKey.Pressed)
                S1 += "h";
                if(IKey.Pressed)
                S1 += "i";
                if(JKey.Pressed)
                S1 += "j";
                if(KKey.Pressed)
                S1 += "k";
                if(LKey.Pressed)
                S1 += "l";
                if(MKey.Pressed)
                S1 += "m";
                if(NKey.Pressed)
                S1 += "n";
                if(OKey.Pressed)
                S1 += "o";
                if(PKey.Pressed)
                S1 += "p";
                if(QKey.Pressed)
                S1 += "q";
                if(RKey.Pressed)
                S1 += "r";
                if(SKey.Pressed)
                S1 += "s";
                if(TKey.Pressed)
                S1 += "t";
                if(UKey.Pressed)
                S1 += "u";
                if(VKey.Pressed)
                S1 += "v";
                if(WKey.Pressed)
                S1 += "w";
                if(XKey.Pressed)
                S1 += "x";
                if(YKey.Pressed)
                S1 += "y";
                if(ZKey.Pressed)
                S1 += "z";

                if(Num1Key.Pressed)
                S1 += "1";
                if(Num2Key.Pressed)
                S1 += "2";
                if(Num3Key.Pressed)
                S1 += "3";
                if(Num4Key.Pressed)
                S1 += "4";
                if(Num5Key.Pressed)
                S1 += "5";
                if(Num6Key.Pressed)
                S1 += "6";
                if(Num7Key.Pressed)
                S1 += "7";
                if(Num8Key.Pressed)
                S1 += "8";
                if(Num9Key.Pressed)
                S1 += "9";
                if(Num0Key.Pressed)
                S1 += "0";

                if(SpaceKey.Pressed)
                S1 += " ";
                if(PeriodKey.Pressed)
                S1 += ".";
                if(CommaKey.Pressed)
                S1 += ",";
                if(AddKey.Pressed)
                S1 += "+";

                if(BackKey.Pressed&&!S1.empty())S1.erase(--S1.end());
            }

            Str.SetText(S1);

            if(EnterKey.Pressed)
            {
                Active = false;

                if(S1.empty())
                {
                    Str.SetColor(Color(150, 150, 150));
                }
                else
                {
                    Str.SetColor(Color(75, 75, 75));
                    return false;
                }

                return true;
            }
            return true;
        }
        return true;
    }

    void Draw(Application & App)
    {
        ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                App().GetDefaultView().GetRect().Top);

        Position = ViewPosition + Origin;

        Bubble.SetPosition(Position);
        App().Draw(Bubble);

        Str.SetColor(Color(255, 255, 255, 72));
        Str.SetPosition(Position.x + 8, Position.y + 1);
        App().Draw(Str);

        Str.SetColor(Color(20, 20, 20, 240));
        Str.SetPosition(Position.x + 8, Position.y);
        App().Draw(Str);
    }

    void Draw(Application & App, Vector2f VP)
    {
        ViewPosition = VP;

        Position = ViewPosition + Origin;

        Str.SetPosition(Position.x, Position.y - Size.y / 10);

        App().Draw(Str);
    }
};

class ChatBubble
{
    protected:
    Sprite Bubble;
    string S1;
    String Str;
    Vector2f Position;
    Vector2f Origin;
    Vector2f ViewPosition;
    Color BubbleColor;

    public:
    ChatBubble()
    {}

    ChatBubble(SynchronizationData & data, Image & bubble)
    {
        Origin = data.MsgPos;
        Position = ViewPosition + Origin;

        Bubble.SetImage(bubble);
        SetCenterInCenter(Bubble);
        BubbleColor = Color(data.MsgR, data.MsgG, data.MsgB);
        Bubble.SetColor(BubbleColor);

        S1 = data.Msg;
        Str = String(S1, Font::GetDefaultFont(), 22);
        Str.SetCenter(GetCenter(Str));
        Str.SetPosition(Position.x, Position.y);
        Str.SetColor(Color(20, 20, 20));
    }

    ChatBubble(string msg, float xPos = 0, float yPos = 0, Image & bubble, Color color = Color(255, 255, 255))
    {
        Origin = Vector2f(xPos, yPos);
        Position = ViewPosition + Origin;

        Bubble.SetImage(bubble);
        SetCenterInCenter(Bubble);
        BubbleColor = color;
        Bubble.SetColor(BubbleColor);

        S1 = msg;
        Str = String(msg, Font::GetDefaultFont(), 22);
        Str.SetCenter(GetCenter(Str));
        Str.SetPosition(Position.x, Position.y);
        Str.SetColor(Color(20, 20, 20));
    }

    Sprite   &  GetBubble   ()  {return Bubble;     }
    string   &  GetString   ()  {return S1;         }
    String   &  GetStr      ()  {return Str;        }
    Vector2f &  GetPosition ()  {return Position;   }
    Color    &  GetColor    ()  {return BubbleColor;}

    void SetPosition(Vector2f pos) {Origin = pos;}

    void SetS1(string s)
    {
        S1 = s;
        Str.SetText(S1);
    }

    void Draw(Application & App, bool followView = false)
    {
        ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                App().GetDefaultView().GetRect().Top);

        Position = followView?ViewPosition:Vector2f(0,0) + Origin;

        Bubble.SetPosition(Position);
        App().Draw(Bubble);

        Str.SetColor(Color(255, 255, 255, 72));
        Str.SetPosition(Position.x-1, Position.y-4 + 1);
        App().Draw(Str);

        Str.SetColor(Color(20, 20, 20, 240));
        Str.SetPosition(Position.x-1, Position.y-4);
        App().Draw(Str);
    }
};

#endif
