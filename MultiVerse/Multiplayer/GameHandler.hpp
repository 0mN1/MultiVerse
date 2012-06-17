#ifndef __GAMEHANDLER_HPP__
#define __GAMEHANDLER_HPP__

#include <iostream>
#include <windows.h>
#include "SFMLfunctions.hpp"
#include "List.hpp"
#include "GUI.hpp"
#include "Animation.hpp"
#include "Buttons.hpp"
#include "Player.hpp"
#include "LevelObjects.hpp"
#include "LevelBackground.hpp"
#include "Multiplayer.hpp"

#define UPDATETIME 0.02f

bool AABBCollsion(Vector2f p, Vector2f c1, Vector2f c2)
{
    return (c1.x < p.x && c2.x > p.x && c1.y < p.y && c2.y > p.y);
}

Vector2f GetScreenSize()
{
    Vector2f size;

    size.x = GetSystemMetrics(SM_CXSCREEN);
    size.y = GetSystemMetrics(SM_CYSCREEN);

    return size;
}

class NewButton
{
    protected:
    Sprite Sprites[3];
    Vector2f Position;
    Vector2f ViewPosition;
    String Text;
    short index;
    float Size;

    public:
    bool Pressed;

    NewButton()
    {}

    NewButton(Image Images[3], string text, Vector2f position, float size = 16)
    {
        Position = position;
        for(int i = 0; i < 3; i++)
        {
            Sprites[i].SetImage(Images[i]);
            Sprites[i].SetCenter(Sprites[i].GetSize().x/2, Sprites[i].GetSize().y/2);
            Sprites[i].SetPosition(ViewPosition + Position);
        }

        Size = size;

        Text.SetText(text);
        Text.SetSize(Size);
        Text.SetCenter((Text.GetRect().Right - Text.GetRect().Left) / 2, (Text.GetRect().Bottom - Text.GetRect().Top) / 2);
        Text.SetPosition(ViewPosition + Position);

        index = 0;
        Pressed = false;
    }

    void SetText(string s)
    {
        Text.SetText(s);
        Text.SetCenter((Text.GetRect().Right - Text.GetRect().Left) / 2, (Text.GetRect().Bottom - Text.GetRect().Top) / 2);
    }

    bool Update(Application & App)
    {
        if(PPCollision(Cursor.Position, Sprites[0]))
        {
            if(Cursor.Left())
            index = 2;
            else
            index = 1;
        }
        else
        index = 0;

        ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                App().GetDefaultView().GetRect().Top);

        Sprites[index].SetPosition(ViewPosition + Position);
        Text.SetPosition(ViewPosition + Position);

        App().Draw(Sprites[index]);
        App().Draw(Text);

        if(index == 1 && Cursor.Left.Released)
        return Pressed = true;
        else
        return Pressed = false;
    }

    bool Update(Application & App, Vector2f VP)
    {
        if(PPCollision(Cursor.Position, Sprites[0]))
        {
            if(Cursor.Left())
            index = 2;
            else
            index = 1;
        }
        else
        index = 0;

        ViewPosition = VP;

        Sprites[index].SetPosition(ViewPosition + Position);
        Text.SetPosition(ViewPosition + Position);

        App().Draw(Sprites[index]);
        App().Draw(Text);

        if(index == 1 && Cursor.Left.Released)
        return Pressed = true;
        else
        return Pressed = false;
    }
};

class CharacterButton
{
    protected:
    Image image;
    Sprite sprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool Selected;
    Shape Box;

    public:
    bool Pressed;

    CharacterButton()
    {}

    CharacterButton(Vector2f position, string imageName)
    {
        Position = position;
        sprite = LoadSprite(image, imageName);
        SetCenterInCenter(sprite);
        sprite.SetPosition(ViewPosition + Position);

        Box = Shape::Rectangle(-75, -75, 75, 75, Color(255, 255, 255), 2, Color(128, 128, 128));

        Pressed = false;
    }

    bool Update(Application & App)
    {
        ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                App().GetDefaultView().GetRect().Top);

        Box.SetPosition(ViewPosition + Position);
        sprite.SetPosition(ViewPosition + Position);

        App().Draw(Box);
        App().Draw(sprite);

        return Pressed = (Cursor.Left.Released && AABBCollsion(Cursor.Position, ViewPosition + Position + Vector2f(-75, -75), ViewPosition + Position + Vector2f(75, 75)));
    }

    bool Update(Application & App, Vector2f VP)
    {
        ViewPosition = VP;

        Box.SetPosition(ViewPosition + Position);
        sprite.SetPosition(ViewPosition + Position);

        App().Draw(Box);
        App().Draw(sprite);

        return Pressed = (Cursor.Left.Released && AABBCollsion(Cursor.Position, ViewPosition + Position + Vector2f(-75, -75), ViewPosition + Position + Vector2f(75, 75)));
    }
};

class NetworkWindow
{
    protected:
    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool PickedUp;
    Vector2f MouseOffset;
    GUI::TextBox NameBox;

    public:
    NewButton ServerButton;
    NewButton ClientButton;

    NetworkWindow()
    {}

    NetworkWindow(Image & WindowImage, Image NBImages[3], string lastName)
    {
        WindowSprite.SetImage(WindowImage);

        Position.x = 1280/2 - WindowSprite.GetSize().x/2;
        Position.y = 720/2 - WindowSprite.GetSize().y/2;

        cout << lastName;

        NameBox = GUI::TextBox(25, 60, 200, 24);
        NameBox.SetS1(lastName);
        NameBox.SetActive(true);
        ServerButton = NewButton(NBImages, "Server", Vector2f(70, 125));
        ClientButton = NewButton(NBImages, "Client", Vector2f(180, 125));
    }

    string GetName()
    {
        return NameBox.GetString();
    }

    bool Update(Application & App)
    {
        ViewPosition.x = App().GetDefaultView().GetRect().Left;
        ViewPosition.y = App().GetDefaultView().GetRect().Top;

        if(Cursor.Left.Pressed && Cursor.Position.y < Position.y + ViewPosition.y + 20 && PPCollision(Cursor.Position, WindowSprite))
        {
            PickedUp = true;
            MouseOffset = Position - Cursor.Position;
        }

        if(PickedUp)
        {
            if(!Cursor.Left())
            PickedUp = false;
            else
            Position = Cursor.Position + MouseOffset;
        }

        WindowSprite.SetPosition(ViewPosition + Position);
        App().Draw(WindowSprite);

        NameBox.Draw(App, ViewPosition + Position);

        if(Cursor.Left.Pressed)
        NameBox.Clicked();

        NameBox.Write();

        if((ServerButton.Update(App, ViewPosition + Position)
        ||  ClientButton.Update(App, ViewPosition + Position))
        &&  NameBox.GetString() != "")
        {
            return true;
        }

        return false;
    }

    Vector2f GetPosition()
    {
        return Position;
    }

    void SetPosition(Vector2f Pos)
    {
        Position = Pos;
    }
};

class IPAddressWindow
{
    protected:
    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool PickedUp;
    Vector2f MouseOffset;
    GUI::TextBox NameBox;

    public:
    NewButton ConnectButton;

    IPAddressWindow()
    {}

    IPAddressWindow(Image & WindowImage, Image NBImages[3], string lastServer)
    {
        WindowSprite.SetImage(WindowImage);

        Position.x = 1280/2 - WindowSprite.GetSize().x/2;
        Position.y = 720/2 - WindowSprite.GetSize().y/2;

        NameBox = GUI::TextBox(25, 60, 200, 24);
        NameBox.SetActive(true);
        ConnectButton = NewButton(NBImages, "Connect", Vector2f(170, 125));

        NameBox.SetS1(lastServer);
    }

    string GetName()
    {
        return NameBox.GetString();
    }

    bool Update(Application & App)
    {
        ViewPosition.x = App().GetDefaultView().GetRect().Left;
        ViewPosition.y = App().GetDefaultView().GetRect().Top;

        if(Cursor.Left.Pressed && Cursor.Position.y < Position.y + ViewPosition.y + 20 && PPCollision(Cursor.Position, WindowSprite))
        {
            PickedUp = true;
            MouseOffset = Position - Cursor.Position;
        }

        if(PickedUp)
        {
            if(!Cursor.Left())
            PickedUp = false;
            else
            Position = Cursor.Position + MouseOffset;
        }

        WindowSprite.SetPosition(ViewPosition + Position);
        App().Draw(WindowSprite);

        NameBox.Draw(App, ViewPosition + Position);

        if(Cursor.Left.Pressed)
        NameBox.Clicked();

        NameBox.Write();

        if((ConnectButton.Update(App, ViewPosition + Position))
        &&  NameBox.GetString() != "")
        {
            return true;
        }

        return false;
    }

    Vector2f GetPosition()
    {
        return Position;
    }

    void SetPosition(Vector2f Pos)
    {
        Position = Pos;
    }
};

class CharacterWindow
{
    protected:
    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool PickedUp;
    Vector2f MouseOffset;

    public:
    List< CharacterButton > Buttons;
    int Selected;

    CharacterWindow()
    {}

    CharacterWindow(Image & WindowImage)
    {
        WindowSprite.SetImage(WindowImage);

        Position.x = 1280/2 - WindowSprite.GetSize().x/2;
        Position.y = 720/2 - WindowSprite.GetSize().y/2;

        Buttons.AddNode(new CharacterButton(Vector2f(150, 170), "Data/Image/Guy01r.png"));
        Buttons.AddNode(new CharacterButton(Vector2f(350, 170), "Data/Image/Guy02r.png"));
    }

    bool Update(Application & App)
    {
        ViewPosition.x = App().GetDefaultView().GetRect().Left;
        ViewPosition.y = App().GetDefaultView().GetRect().Top;

        if(Cursor.Left.Pressed && Cursor.Position.y < Position.y + ViewPosition.y + 20 && PPCollision(Cursor.Position, WindowSprite))
        {
            PickedUp = true;
            MouseOffset = Position - Cursor.Position;
        }

        if(PickedUp)
        {
            if(!Cursor.Left())
            PickedUp = false;
            else
            Position = Cursor.Position + MouseOffset;
        }

        WindowSprite.SetPosition(ViewPosition + Position);
        App().Draw(WindowSprite);

        for(int i = 0; i < Buttons.GetSize(); i++)
        {
            if(Buttons[i]->Update(App, ViewPosition + Position))
            {
                Selected = i;
                return true;
            }
        }

        return false;
    }

    Vector2f GetPosition()
    {
        return Position;
    }

    void SetPosition(Vector2f Pos)
    {
        Position = Pos;
    }
};

class GameHandler
{
    public:
    Font GothicFont;
    Image NBImages[3];
    Image WindowImages[3];

    bool isServer;
    string Name;
    NetworkWindow networkWindow;
    IPAddressWindow IPWindow;
    CharacterWindow characterWindow;
    Application App;
    PlayerClass Player;
    PlayerClass Friend;
    int CharacterImage;

    bool GameOn;

    float NetworkTimer;

    ServerNetworkHandler ServerHandler;
    ClientNetworkHandler ClientHandler;

    List< Obstacle > Ob;
    LevelBackground lBg;

    bool WriteChat;

    /// Settings
    Color IdColor;
    string Server;

    GameHandler() : App((GetScreenSize().x<1280 || GetScreenSize().y<720)?800:1280, (GetScreenSize().x<1280 || GetScreenSize().y<720)?600:720, 32, "Multiverse Multiplayer")
    {
        LoadSettings();

        GothicFont.LoadFromFile("Data/Font/CenturyGothic.ttf");

        NBImages[0].LoadFromFile("Data/Image/EditorInterface/NewButtonIdle.png");
        NBImages[1].LoadFromFile("Data/Image/EditorInterface/NewButtonHoover.png");
        NBImages[2].LoadFromFile("Data/Image/EditorInterface/NewButtonPressed.png");

        WindowImages[0].LoadFromFile("Data/Image/EditorInterface/NewWindow01.png");
        WindowImages[1].LoadFromFile("Data/Image/EditorInterface/NewWindow02.png");
        WindowImages[2].LoadFromFile("Data/Image/EditorInterface/NewWindow03.png");

        networkWindow = NetworkWindow(WindowImages[0], NBImages, Name);
        IPWindow = IPAddressWindow(WindowImages[0], NBImages, Server);
        characterWindow = CharacterWindow(WindowImages[2]);

        Ob.AddNode(new Obstacle("Data/Image/Obstacles/Platform01Bumpy.png", Vector2f(-1100.f, 1250.f)));
        Ob.AddNode(new Obstacle("Data/Image/Obstacles/ComplexLevel.png", Vector2f(1200.f, 2500.f)));

        lBg.AddObject("Data/Image/Sun.png", 200.f, Vector2f(650.f, 150.f));
        lBg.AddObject("Data/Image/Clouds01.png", 20.f, Vector2f(650.f, 200.f), Vector2f(-1000.f, 0.f));
        lBg.AddObject("Data/Image/Clouds02.png", 12.f, Vector2f(200.f, 300.f), Vector2f(-1000.f, 0.f));

        NetworkTimer = UPDATETIME;

        GameOn = true;
        WriteChat = false;
    }

    void LoadSettings()
    {
        ifstream fin;
        fin.open("MultiplayerSettings");

        if(fin.is_open())
        {
            int r,g,b;
            getline(fin, Name);
            fin >> r >> g >> b >> Server;
            IdColor = Color(r, g, b);
        }

        fin.close();
        fin.clear();
    }

    void SaveSettings()
    {
        ofstream fout;
        fout.open("MultiplayerSettings");

        fout << Name << endl
             << (int)IdColor.r << " " << (int)IdColor.g << " "
             << (int)IdColor.b << endl
             << Server << endl;

        fout.close();
    }

    void CharacterSelectionLoop()
    {
        while (App().IsOpened())
        {
            sf::Event Event;
            while (App().GetEvent(Event))
            {
                if(Event.Type == sf::Event::Closed)
                {
                    GameOn = false;
                    App().Close();
                }

                if(Event.Type == sf::Event::KeyPressed)
                switch(Event.Key.Code)
                {
                    case Key::Escape:
                    GameOn = false;
                    App().Close();
                    break;

                    default:
                    break;
                }

                if(Event.Type == sf::Event::Resized)
                {
                    App().GetDefaultView().SetHalfSize(App().GetWidth()/2, App().GetHeight()/2);
                }

                if(Event.Type == sf::Event::MouseWheelMoved)
                {
                    if(Event.MouseWheel.Delta > 0.f)
                    {
                        /// Up
                        App().GetDefaultView().Zoom(1.2f);
                    }
                    else
                    if(Event.MouseWheel.Delta < 0.f)
                    {
                        /// Down
                        App().GetDefaultView().Zoom(1/1.2f);
                    }
                }
            }

            /// Update Window and Keyboard
            App.Update(Color(0, 0, 0));

            GUI::UpdateKeys(App());

            if(characterWindow.Update(App))
            {
                CharacterImage = characterWindow.Selected;
                return ;
            }

            App.Display();
        }
    }

    void NetworkOptionsLoop()
    {
        bool Step1Finnished = false;

        String Text;
        Text.SetText("Waiting for client to connect...");
        Text.SetCenter(GetCenter(Text));
        Text.SetPosition(1280/2, 720/2);

        Thread ListenThread(&Listen, &ServerHandler);

        while (App().IsOpened())
        {
            sf::Event Event;
            while (App().GetEvent(Event))
            {
                if(Event.Type == sf::Event::Closed)
                {
                    GameOn = false;
                    App().Close();
                    ListenThread.Terminate();
                }

                if(Event.Type == sf::Event::KeyPressed)
                switch(Event.Key.Code)
                {
                    case Key::Escape:
                    GameOn = false;
                    App().Close();
                    ListenThread.Terminate();
                    break;

                    default:
                    break;
                }

                if(Event.Type == sf::Event::Resized)
                {
                    App().GetDefaultView().SetHalfSize(App().GetWidth()/2, App().GetHeight()/2);
                }

                if(Event.Type == sf::Event::MouseWheelMoved)
                {
                    if(Event.MouseWheel.Delta > 0.f)
                    {
                        /// Up
                        App().GetDefaultView().Zoom(1.2f);
                    }
                    else
                    if(Event.MouseWheel.Delta < 0.f)
                    {
                        /// Down
                        App().GetDefaultView().Zoom(1/1.2f);
                    }
                }
            }

            /// Update Window and Keyboard
            App.Update(Color(0, 0, 0));

            GUI::UpdateKeys(App());

            if(!Step1Finnished && networkWindow.Update(App))
            {
                Name = networkWindow.GetName();
                if(networkWindow.ServerButton.Pressed)
                {
                    isServer = true;
                    ListenThread.Launch();
                    ServerHandler.myHostName = Name;
                    ServerHandler.myCharacterImage = CharacterImage;
                }
                else
                {
                    isServer = false;
                    ClientHandler.myHostName = Name;
                    ClientHandler.myCharacterImage = CharacterImage;
                }

                Step1Finnished = true;
            }

            if(Step1Finnished)
            {
                if(isServer)
                {
                    App().Draw(Text);
                }
                else
                {
                    if(IPWindow.Update(App))
                    {
                        ClientHandler.ConnectionAddress = IPWindow.GetName();
                        Server = IPWindow.GetName();

                        if(ClientHandler.ConnectionAddress.IsValid())
                        ClientHandler.ConnectToServer();
                    }
                }
            }

            if(ServerHandler.isConnected || ClientHandler.isConnected)
                return ;

            App.Display();
        }
    }

    void ServerLoop()
    {
        while(!ServerHandler.isReady)
        {
            sf::Sleep(0.1);
        }

        Player.SetSprite(ServerHandler.myCharacterImage);
        //Player.SetName(ServerHandler.myHostName);

        Friend.SetSprite(ServerHandler.CharacterImage);
        Friend.SetName(ServerHandler.HostName);

        bool ShowData = true;
        String Fps("FPS: ");
        Fps.SetPosition(5.f, 0.f);
        Fps.SetFont(GothicFont);
        Fps.SetSize(32);

        Image BubbleImage;
        BubbleImage.LoadFromFile("Data/Image/ChatUI/ChatBubble01.png");
        ChatBox chat(1280/2 - 60, 300, BubbleImage);

        List< ChatBubble > ChatBubbles;

        while (App().IsOpened())
        {
            sf::Event Event;
            while (App().GetEvent(Event))
            {
                if(Event.Type == sf::Event::Closed)
                {
                    GameOn = false;
                    App().Close();
                }

                if(Event.Type == sf::Event::KeyPressed)
                switch(Event.Key.Code)
                {
                    case Key::Escape:
                    GameOn = false;
                    App().Close();
                    break;

                    case Key::F5:
                    ShowData = !ShowData;
                    break;

                    case Key::Tab:
                    WriteChat = !WriteChat;
                    chat.SetActive(true);
                    break;

                    default:
                    break;
                }

                if(Event.Type == sf::Event::Resized)
                {
                    App().GetDefaultView().SetHalfSize(App().GetWidth()/2, App().GetHeight()/2);
                }

                if(Event.Type == sf::Event::MouseWheelMoved)
                {
                    if(Event.MouseWheel.Delta > 0.f)
                    {
                        /// Up
                        App().GetDefaultView().Zoom(1.2f);
                    }
                    else
                    if(Event.MouseWheel.Delta < 0.f)
                    {
                        /// Down
                        App().GetDefaultView().Zoom(1/1.2f);
                    }
                }
            }

            /// Update Window and Keyboard
            App.Update(Color(75, 75, 75));

            GUI::UpdateKeys(App());

            NetworkTimer -= App.GetFrameTime();

            if(NetworkTimer <= 0.f)
            {
                ServerHandler.myData.UpdateData(Player);
                ServerHandler.SendCharacterUpdate();
                ServerHandler.Data.UpdateCharacter(Friend);
                if(ServerHandler.Data.newMsg)
                {
                    ChatBubbles.AddNode(new ChatBubble(ServerHandler.Data, BubbleImage));
                    ServerHandler.Data.newMsg = false;
                }
                NetworkTimer = UPDATETIME;
            }

            lBg.Draw(App);

            for(int i = 0; i < Ob.GetSize(); i++)
            Ob[i]->Draw(App);

            Friend.Predict(App, Ob);
            Friend.Draw(App);

            if(!WriteChat)
            Player.Update(App, lBg, Ob);
            Player.Draw(App);

            for(int i = 0; i < ChatBubbles.GetSize(); i++)
            {
                ChatBubbles[i]->Draw(App);
            }

            if(WriteChat)
            {
                if(!chat.Write())
                {
                    ChatBubbles.AddNode(new ChatBubble(chat.GetString(), chat.GetPosition().x+60.5f,
                                                       chat.GetPosition().y+16, BubbleImage, IdColor));

                    ServerHandler.myData.SendMessage(ChatBubbles[ChatBubbles.GetSize()-1]->GetString(),
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetPosition(),
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetColor().r,
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetColor().g,
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetColor().b);
                    chat.SetS1("");
                    WriteChat = false;
                }

                chat.Draw(App);
            }

            if(ShowData)
            {
                Fps.SetText("FPS: " + ToString(1/App.GetFrameTime()));

                App().Draw(Fps);
            }

            App.Display();
        }
    }

    void ClientLoop()
    {
        while(!ClientHandler.isReady)
        {
            sf::Sleep(0.1);
        }

        Player.SetSprite(ClientHandler.myCharacterImage);
        //Player.SetName(ClientHandler.myHostName);

        Friend.SetSprite(ClientHandler.CharacterImage);
        Friend.SetName(ClientHandler.HostName);

        bool ShowData = true;
        String Fps("FPS: ");
        Fps.SetPosition(5.f, 0.f);
        Fps.SetFont(GothicFont);
        Fps.SetSize(32);

        Image BubbleImage;
        BubbleImage.LoadFromFile("Data/Image/ChatUI/ChatBubble01.png");
        ChatBox chat(1280/2 - 60, 300, BubbleImage);

        List< ChatBubble > ChatBubbles;

        while (App().IsOpened())
        {
            sf::Event Event;
            while (App().GetEvent(Event))
            {
                if(Event.Type == sf::Event::Closed)
                {
                    GameOn = false;
                    App().Close();
                }

                if(Event.Type == sf::Event::KeyPressed)
                switch(Event.Key.Code)
                {
                    case Key::Escape:
                    GameOn = false;
                    App().Close();
                    break;

                    case Key::F5:
                    ShowData = !ShowData;
                    break;

                    case Key::Tab:
                    WriteChat = !WriteChat;
                    chat.SetActive(true);
                    break;

                    default:
                    break;
                }

                if(Event.Type == sf::Event::Resized)
                {
                    App().GetDefaultView().SetHalfSize(App().GetWidth()/2, App().GetHeight()/2);
                }

                if(Event.Type == sf::Event::MouseWheelMoved)
                {
                    if(Event.MouseWheel.Delta > 0.f)
                    {
                        /// Up
                        App().GetDefaultView().Zoom(1.2f);
                    }
                    else
                    if(Event.MouseWheel.Delta < 0.f)
                    {
                        /// Down
                        App().GetDefaultView().Zoom(1/1.2f);
                    }
                }
            }

            /// Update Window and Keyboard
            App.Update(Color(75, 75, 75));

            GUI::UpdateKeys(App());

            NetworkTimer -= App.GetFrameTime();

            if(NetworkTimer <= 0.f)
            {
                ClientHandler.myData.UpdateData(Player);
                ClientHandler.SendCharacterUpdate();
                ClientHandler.Data.UpdateCharacter(Friend);
                if(ClientHandler.Data.newMsg)
                {
                    ChatBubbles.AddNode(new ChatBubble(ClientHandler.Data, BubbleImage));
                    ClientHandler.Data.newMsg = false;
                }
                NetworkTimer = UPDATETIME;
            }

            lBg.Draw(App);

            for(int i = 0; i < Ob.GetSize(); i++)
            Ob[i]->Draw(App);

            Friend.Predict(App, Ob);
            Friend.Draw(App);

            if(!WriteChat)
            Player.Update(App, lBg, Ob);
            Player.Draw(App);

            for(int i = 0; i < ChatBubbles.GetSize(); i++)
            {
                ChatBubbles[i]->Draw(App);
            }

            if(WriteChat)
            {
                if(!chat.Write())
                {
                    ChatBubbles.AddNode(new ChatBubble(chat.GetString(), chat.GetPosition().x+60.5f,
                                                       chat.GetPosition().y+16, BubbleImage, IdColor));

                    ClientHandler.myData.SendMessage(ChatBubbles[ChatBubbles.GetSize()-1]->GetString(),
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetPosition(),
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetColor().r,
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetColor().g,
                                                     ChatBubbles[ChatBubbles.GetSize()-1]->GetColor().b);
                    chat.SetS1("");
                    WriteChat = false;
                }

                chat.Draw(App);
            }

            if(ShowData)
            {
                Fps.SetText("FPS: " + ToString(1/App.GetFrameTime()));

                App().Draw(Fps);
            }

            App.Display();
        }
    }
};

#endif
