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
#include "ArtificialIntelligence.hpp"
#include "LevelScript.hpp"

#define UPDATETIME 0.02f

namespace FONT
{
    sf::Font Calibri;
    sf::Font CalibriB;

    void LoadFonts()
    {
        FONT::Calibri.LoadFromFile("Data/Font/calibri.ttf");
        FONT::CalibriB.LoadFromFile("Data/Font/calibrib.ttf");
    }
}

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
    String flicka;
    Vector2f Position;
    Vector2f ViewPosition;
    bool Selected;
    bool isFlicka;
    Shape Box;

    public:
    bool Pressed;

    CharacterButton()
    {}

    CharacterButton(Vector2f position, string imageName = "")
    {
        Position = position;
        if(imageName == "")
        {
            flicka.SetText("Flicka");
            flicka.SetCenter(GetCenter(flicka));
            flicka.SetColor(Color(0, 0, 0));
            flicka.SetPosition(ViewPosition + Position);

            isFlicka = true;
        }
        else
        {
            sprite = LoadSprite(image, imageName);
            SetCenterInCenter(sprite);
            sprite.SetPosition(ViewPosition + Position);

            isFlicka = false;
        }

        Box = Shape::Rectangle(-75, -75, 75, 75, Color(255, 255, 255), 2, Color(128, 128, 128));

        Pressed = false;
    }

    bool Update(Application & App)
    {
        ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                App().GetDefaultView().GetRect().Top);

        Box.SetPosition(ViewPosition + Position);

        App().Draw(Box);

        if(isFlicka)
        {
            flicka.SetPosition(ViewPosition + Position);
            App().Draw(flicka);
        }
        else
        {
            sprite.SetPosition(ViewPosition + Position);
            App().Draw(sprite);
        }

        return Pressed = (Cursor.Left.Released && AABBCollsion(Cursor.Position, ViewPosition + Position + Vector2f(-75, -75), ViewPosition + Position + Vector2f(75, 75)));
    }

    bool Update(Application & App, Vector2f VP)
    {
        ViewPosition = VP;

        Box.SetPosition(ViewPosition + Position);

        App().Draw(Box);

        if(isFlicka)
        {
            flicka.SetPosition(ViewPosition + Position);
            App().Draw(flicka);
        }
        else
        {
            sprite.SetPosition(ViewPosition + Position);
            App().Draw(sprite);
        }

        return Pressed = (Cursor.Left.Released && AABBCollsion(Cursor.Position, ViewPosition + Position + Vector2f(-75, -75), ViewPosition + Position + Vector2f(75, 75)));
    }
};

/*class NetworkWindow
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

    NetworkWindow(Image & WindowImage, Image NBImages[3])
    {
        WindowSprite.SetImage(WindowImage);

        Position.x = 1280/2 - WindowSprite.GetSize().x/2;
        Position.y = 720/2 - WindowSprite.GetSize().y/2;

        NameBox = GUI::TextBox(25, 60, 200, 24);
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

    IPAddressWindow(Image & WindowImage, Image NBImages[3])
    {
        WindowSprite.SetImage(WindowImage);

        Position.x = 1280/2 - WindowSprite.GetSize().x/2;
        Position.y = 720/2 - WindowSprite.GetSize().y/2;

        NameBox = GUI::TextBox(25, 60, 200, 24);
        NameBox.SetActive(true);
        ConnectButton = NewButton(NBImages, "Connect", Vector2f(170, 125));
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
};*/

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

        Buttons.AddNode(new CharacterButton(Vector2f(100, 170), "Data/Image/Guy01r.png"));
        Buttons.AddNode(new CharacterButton(Vector2f(250, 170), "Data/Image/Guy02r.png"));
        Buttons.AddNode(new CharacterButton(Vector2f(400, 170)));
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

    string Name;
    CharacterWindow characterWindow;
    Application App;
    PlayerClass Player;
    int CharacterImage;

    bool GameOn;

    Level * Lvl;

    GameHandler() : App((GetScreenSize().x<1280 || GetScreenSize().y<720)?800:1280, (GetScreenSize().x<1280 || GetScreenSize().y<720)?600:720, 32, "Multiverse")
    {

//        cout << "GameHandler Constructor is ON\n";

        App().SetFramerateLimit(120);

        GothicFont.LoadFromFile("Data/Font/CenturyGothic.ttf");

        FONT::LoadFonts();

        NBImages[0].LoadFromFile("Data/Image/EditorInterface/NewButtonIdle.png");
        NBImages[1].LoadFromFile("Data/Image/EditorInterface/NewButtonHoover.png");
        NBImages[2].LoadFromFile("Data/Image/EditorInterface/NewButtonPressed.png");

        WindowImages[0].LoadFromFile("Data/Image/EditorInterface/NewWindow01.png");
        WindowImages[1].LoadFromFile("Data/Image/EditorInterface/NewWindow02.png");
        WindowImages[2].LoadFromFile("Data/Image/EditorInterface/NewWindow03.png");

        characterWindow = CharacterWindow(WindowImages[2]);

        GameOn = true;

//        cout << "Loading IntroLevel1 Level...";

        Lvl = new Level(false, &Player.GetMask(), &Player.GetPosition());
        Lvl->Load("IntroLevel1", App); // TestLevel01

        CharacterImage = 2;

//        cout << "Complete\n";

        if(Lvl->SpawnPoints.GetSize() > 0)
        {
            Player.SetPosition(Lvl->SpawnPoints[0]->Position);
        }
    }

    ~GameHandler()
    {
        delete Lvl;
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

            sf::Sleep(1.f/200.f);
        }
    }

/*    void NetworkOptionsLoop()
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

                        if(ClientHandler.ConnectionAddress.IsValid())
                        ClientHandler.ConnectToServer();
                    }
                }
            }

            if(ServerHandler.isConnected || ClientHandler.isConnected)
                return ;

            App.Display();
        }
    }*/

    void GameLoop()
    {
        Player.SetSprite(CharacterImage);
        //Lvl->DepthSprites.Sort();

        Image VignetteImage;
        Sprite Vignette = LoadSprite(VignetteImage, "Data/Image/Vignette3.png"); // Invertera bilden så att man kan välja färg via SetColor och scriptet
        SetCenterInCenter(Vignette);

        Image EbSymbolImage;
        Sprite EbSymbol = LoadSprite(EbSymbolImage, "Data/Image/EditorInterface/EdgeBoxSymbol.png");
        SetCenterInCenter(EbSymbol);

        float TimeWarp = 1.f;

        bool ShowData = false;
        bool ShowVignette = true;
        String Fps("FPS: ");
        Fps.SetPosition(5.f, 0.f);
        Fps.SetFont(GothicFont);
        Fps.SetSize(32);

        while(App().IsOpened())
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

                    case Key::F3:
                    LoadLevel("IntroLevel1", 0, 1);
                    break;

                    case Key::F5:
                    ShowData = !ShowData;
                    break;

                    case Key::F6:
                    ShowVignette = !ShowVignette;
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
                    if(GUI::LCtrlKey())
                    {
                        if(Event.MouseWheel.Delta > 0.f)
                        {
                            /// Up
                            TimeWarp *= 1.2f;
                            App.SetTimeWarp(TimeWarp);
                        }
                        else
                        if(Event.MouseWheel.Delta < 0.f)
                        {
                            /// Down
                            TimeWarp *= 1/1.2f;
                            App.SetTimeWarp(TimeWarp);
                        }
                    }
                    else
                    {
                        if(Event.MouseWheel.Delta > 0.f)
                        {
                            /// Up
                            App().GetDefaultView().Zoom(1.2f);
//                            Lvl->ZoomView(Vector2f(1.2f, 1.2f));
                        }
                        else
                        if(Event.MouseWheel.Delta < 0.f)
                        {
                            /// Down
                            App().GetDefaultView().Zoom(1/1.2f);
//                            Lvl->ZoomView(Vector2f(1/1.2f, 1/1.2f));
                        }
                    }
                }
            }

            /// Update Window and Keyboard
            App.Update(Lvl->BgColor);

            GUI::UpdateKeys(App());

            Lvl->UpdateSounds();
            Lvl->UpdateMusic();

            Player.Update(App, Lvl);

            Lvl->UpdatePlatforms(App);
            Lvl->UpdateArtifacts(App);

            Lvl->RunEveryFrameScript(App);

            Lvl->DrawBack(App);

            Lvl->DrawIGAI(App);

            Player.Draw(App);

            Lvl->DrawFront(App);

            if(ShowData)
            {
                Lvl->DrawActionBoxes(App, FONT::Calibri);
                Lvl->DrawEdgeBoxes(App, EbSymbol);
                Fps.SetPosition(UpperLeft(App().GetDefaultView()) + Vector2f(5, 0));
                Fps.SetText("FPS: " + ToString(1/App.GetFrameTime()) + "\nZoom: " + ToString(App.GetZoom().x));

                App().Draw(Fps);
            }

            if(ShowVignette)
            {
                Vignette.SetPosition(App().GetDefaultView().GetCenter());
                Vignette.SetScale(1/App.GetZoom().x, 1/App.GetZoom().y);
                App().Draw(Vignette);
            }

            App.Display();

            if(Lvl->VignetteChanged)
            {
                Vignette.SetColor(Lvl->VignetteColor);
                Lvl->VignetteChanged = false;
            }

            if(Lvl->GotoLevel)
            LoadLevel(Lvl->NextLevel, Lvl->NextSpawnPoint, Lvl->NextDirection);

//            sf::Sleep(1.f/200.f);
//
//            sf::Sleep(0.2);
        }
    }

    void LoadLevel(string Name, int spawnPoint = 0, bool direction = 0)
    {
//        cout << "\nLoading " + Name + " Level...";
        delete Lvl;

        Lvl = new Level(false, &Player.GetMask(), &Player.GetPosition());
        Lvl->Load(Name, App);

//        cout << "Complete\n";

        Player.SetDirection(direction);

        if(Lvl->SpawnPoints.GetSize() > 0)
        {
            if(spawnPoint < Lvl->SpawnPoints.GetSize())
            Player.SetPosition(Lvl->SpawnPoints[spawnPoint]->Position);
            else
            {
                Player.SetPosition(Lvl->SpawnPoints[0]->Position);
//                cerr << "\nSpawnPoint " << spawnPoint << " in level " << Name << " doesn't exist!\n";
            }

        }
    }
};

#endif
