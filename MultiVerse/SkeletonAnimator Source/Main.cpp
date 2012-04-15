#include "../SFMLfunctions.hpp"
#include "../GUI.hpp"
#include "List.hpp"
#include "../Skeleton.hpp"

using LIST::List;

class Action
{
    public:
    string Command;
    Bones * Before;
    Bones After;
};

class History
{
    public:
    List< Action > Actions;
};

class AddButton
{
    protected:
    Sprite Sprites[3];
    Vector2f Position;
    Vector2f ViewPosition;
    String Text;
    short index;
    float Size;

    public:
    AddButton()
    {}

    AddButton(Image Images[3], string text, Vector2f position, float size = 16)
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
        return true;
        else
        return false;
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
        return true;
        else
        return false;
    }
};

class CheckBox
{
    protected:
    Sprite Sprites[4];
    Vector2f Position;
    Vector2f ViewPosition;
    short index;
    bool Checked;
    String Text;

    public:
    CheckBox()
    {}

    CheckBox(Image Images[4], Vector2f position, string text, int size = 12)
    {
        Position = position;
        for(int i = 0; i < 4; i++)
        {
            Sprites[i].SetImage(Images[i]);
            Sprites[i].SetCenter(Sprites[i].GetSize().x/2, Sprites[i].GetSize().y/2);
            Sprites[i].SetPosition(ViewPosition + Position);
        }

        Text.SetText(text);
        Text.SetSize(size);
        Text.SetCenter((Text.GetRect().Right - Text.GetRect().Left) / 2, 0);
        Text.SetPosition(ViewPosition + Position + Vector2f(0, -30));

        index = 0;
        Checked = false;
    }

    bool GetValue()
    {
        return Checked;
    }

    void SetValue(bool val)
    {
        Checked = val;
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
        App().Draw(Sprites[index]);

        if(Checked)
        {
            Sprites[3].SetPosition(ViewPosition + Position);
            App().Draw(Sprites[3]);
        }

        if(index == 1 && Cursor.Left.Released)
        {
            Checked = !Checked;

            return true;
        }
        else
        return false;
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

        Text.SetPosition(ViewPosition + Position + Vector2f(0, -30));
        App().Draw(Text);

        Sprites[index].SetPosition(ViewPosition + Position);
        App().Draw(Sprites[index]);

        if(Checked)
        {
            Sprites[3].SetPosition(ViewPosition + Position);
            App().Draw(Sprites[3]);
        }

        if(index == 1 && Cursor.Left.Released)
        {
            Checked = !Checked;

            return true;
        }
        else
        return false;
    }
};

namespace GUI
{
    class FrameTimeBox
    {
        protected:
        bool Active;
        bool Replace;
        string S1;
        String Str;
        Vector2f Position;
        Vector2f Origin;
        Vector2f ViewPosition;
        Vector2f Size;

        public:
        FrameTimeBox(float xPos = 0, float yPos = 0, float xSize = 200, float ySize = 30)
        {
            Origin = Vector2f(xPos, yPos);
            Position = ViewPosition + Origin;
            Size = Vector2f(xSize, ySize);

            Active = false;
            Replace = true;

            Str = String("", Font::GetDefaultFont(), ySize);
            Str.SetPosition(xPos, yPos - ySize / 10);
            Str.SetColor(Color(255, 255, 255));
        }

        bool & GetActive(){return Active;}
        float GetValue(){return atof(S1.c_str());}
        String & GetStr(){return Str;}
        Vector2f & GetPosition(){return Position;}
        Vector2f & GetSize(){return Size;}

        void SetPosition(Vector2f pos) {Origin = pos;}
        void SetText(string s) {S1 = s; Str.SetText(S1 + " ms");}

        bool & SetActive(bool New)
        {
            if(Active != New && New)
            {
                Active = true;
            }
            else
            if(Active != New && !New)
            {
                Active = false;
                Replace = true;

                if(S1.empty())
                {
                    S1 = "0";
                    Str.SetText(S1 + " ms");
                }
            }

            return Active;
        }

        bool Clicked()
        {
            if(Str.GetRect().Left < Cursor.Position.x && Cursor.Position.x < Str.GetRect().Right
            && Str.GetRect().Top < Cursor.Position.y && Cursor.Position.y < Str.GetRect().Bottom)
            {
                Active = true;

                return true;
            }
            else
            if(Active && !LShiftKey())
            {
                Active = false;
                Replace = true;

                if(S1.empty())
                {
                    S1 = "0";
                    Str.SetText(S1 + " ms");
                }

                return false;
            }

            return true;
        }

        bool Write()
        {
            if(Active)
            {
                if(LShiftKey.Hold || RShiftKey.Hold)
                {
                    if(BackKey.Pressed && !S1.empty())
                    S1.erase();
                }
                else
                {
                    if(Num1Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "1";
                    }
                    if(Num2Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "2";
                    }
                    if(Num3Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "3";
                    }
                    if(Num4Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "4";
                    }
                    if(Num5Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "5";
                    }
                    if(Num6Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "6";
                    }
                    if(Num7Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "7";
                    }
                    if(Num8Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "8";
                    }
                    if(Num9Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "9";
                    }
                    if(Num0Key.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += "0";
                    }
                    if(PeriodKey.Pressed)
                    {
                        if(Replace)
                        {
                            S1.erase();
                            Replace = false;
                        }
                        S1 += ".";
                    }
                    if(BackKey.Pressed && !S1.empty())
                    {
                        if(Replace)
                        {
                            Replace = false;
                        }
                        S1.erase(--S1.end());
                    }
                }

                Str.SetText(S1 + " ms");

                if(EnterKey.Pressed)
                {
                    Active = false;
                    Replace = true;

                    if(S1.empty())
                    {
                        S1 = "0";
                        Str.SetText(S1 + " ms");
                    }

                    return false;
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

            Str.SetPosition(Position);

            App().Draw(Str);
        }
    };
}

enum SAVELOADMODE {SAVE, LOAD};

class SaveLoadWindow
{
    protected:
    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    AddButton SaveLoadButton;
    AddButton CancelButton;
    bool PickedUp;
    Vector2f MouseOffset;
    GUI::TextBox FileNameBox;

    public:
    bool SaveLoadButtonPressed, CancelButtonPressed;
    bool Active;
    SAVELOADMODE SaveLoadMode;

    SaveLoadWindow(Image & WindowImage, Image ABImages[3])
    {
        WindowSprite.SetImage(WindowImage);

        Position.x = 700 - WindowSprite.GetSize().x/2;
        Position.y = 450 - WindowSprite.GetSize().y/2;

        FileNameBox = GUI::TextBox(25, 60, 200, 24);
        SaveLoadButton = AddButton(ABImages, "Save", Vector2f(70, 125));
        CancelButton = AddButton(ABImages, "Cancel", Vector2f(180, 125));
        Active = false;
        SaveLoadButtonPressed = false;
        CancelButtonPressed = false;
    }

    string GetFileName()
    {
        return FileNameBox.GetString();
    }

    void Activate(SAVELOADMODE mode)
    {
        SaveLoadMode = mode;

        FileNameBox.SetActive(true);

        if(!Active)
        Active = true;

        if(SaveLoadMode == SAVE)
        {
            SaveLoadButton.SetText("Save");
        }
        else if(SaveLoadMode == LOAD)
        {
            SaveLoadButton.SetText("Load");
        }
    }

    void Update(Application & App)
    {
        if(Active)
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

            FileNameBox.Draw(App, ViewPosition + Position);

            if(Cursor.Left.Pressed)
            FileNameBox.Clicked(App);

            FileNameBox.Write();

            SaveLoadButtonPressed = SaveLoadButton.Update(App, ViewPosition + Position);

            if((CancelButtonPressed = CancelButton.Update(App, ViewPosition + Position)))
            {
                Active = false;
            }
        }
        else if(SaveLoadButtonPressed || CancelButtonPressed)
        {
            SaveLoadButtonPressed = false;
            CancelButtonPressed = false;
        }
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

class MainButtonsWindow
{
    protected:
    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    AddButton AddFrameButton;
    AddButton PlayButton, StopButton;
    AddButton LoadButton, SaveButton;
    AddButton CenterCurrentButton, CenterAllButton;

    bool PickedUp;
    Vector2f MouseOffset;

    public:
    bool AddFrameButtonPressed;
    bool PlayButtonPressed, StopButtonPressed;
    bool LoadButtonPressed, SaveButtonPressed;
    bool CenterCurrentButtonPressed, CenterAllButtonPressed;
    bool DrawMaskCBPressed, DrawFloorCBPressed, DrawEdgeCBPressed;
    CheckBox DrawMaskCB, DrawFloorCB, DrawEdgeCB;

    MainButtonsWindow(Image & WindowImage, Image ABImages[3], Image CBImages[4])
    {
        WindowSprite.SetImage(WindowImage);

        AddFrameButton = AddButton(ABImages, "Add Frame", Vector2f(70, 50));

        PlayButton = AddButton(ABImages, "Play", Vector2f(70, 90));
        StopButton = AddButton(ABImages, "Stop", Vector2f(70, 90));

        LoadButton = AddButton(ABImages, "Load", Vector2f(180, 50));
        SaveButton = AddButton(ABImages, "Save", Vector2f(180, 90));

        CenterCurrentButton = AddButton(ABImages, "Center Current", Vector2f(70, 130));
        CenterAllButton = AddButton(ABImages, "Center All", Vector2f(180, 130));

        DrawFloorCB = CheckBox(CBImages, Vector2f(60, 210), "DrawFloor");
        DrawFloorCB.SetValue(true);
        DrawMaskCB = CheckBox(CBImages, Vector2f(125, 210), "DrawMask");
        DrawEdgeCB = CheckBox(CBImages, Vector2f(190, 210), "DrawEdge");

        AddFrameButtonPressed =
        PlayButtonPressed = StopButtonPressed =
        LoadButtonPressed = SaveButtonPressed =
        CenterCurrentButtonPressed =
        CenterAllButtonPressed = DrawMaskCBPressed =
        DrawFloorCBPressed = DrawEdgeCBPressed = false;
    }

    void Update(Application & App, bool SkeletonPlay)
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

        AddFrameButtonPressed = AddFrameButton.Update(App, ViewPosition + Position);
        LoadButtonPressed = LoadButton.Update(App, ViewPosition + Position);
        SaveButtonPressed = SaveButton.Update(App, ViewPosition + Position);
        CenterCurrentButtonPressed = CenterCurrentButton.Update(App, ViewPosition + Position);
        CenterAllButtonPressed = CenterAllButton.Update(App, ViewPosition + Position);
        DrawMaskCBPressed = DrawMaskCB.Update(App, ViewPosition + Position);
        DrawFloorCBPressed = DrawFloorCB.Update(App, ViewPosition + Position);
        DrawEdgeCBPressed = DrawEdgeCB.Update(App, ViewPosition + Position);

        if(SkeletonPlay)
        {
            StopButtonPressed = StopButton.Update(App, ViewPosition + Position);
            PlayButtonPressed = false;
        }
        else
        {
            PlayButtonPressed = PlayButton.Update(App, ViewPosition + Position);
            StopButtonPressed = false;
        }
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

class FrameListItem
{
    protected:
    Sprite Sprites[7];
    Vector2f * ListPosition;
    Vector2f * ViewPosition;

    String FrameIDString;//, DurationString;

    GUI::FrameTimeBox TimeBox;

    public:
    Vector2f Position;
    int FrameID;
    float Duration;
    bool UpdateDuration;

    FrameListItem()
    {}

    FrameListItem(int FID, float Dur, Image Images[7], Vector2f Pos, Vector2f * LP, Vector2f * VP)
    {
        Sprites[0].SetImage(Images[0]);
        Sprites[1].SetImage(Images[1]);
        Sprites[2].SetImage(Images[2]);
        Sprites[3].SetImage(Images[3]);
        Sprites[4].SetImage(Images[4]);
        Sprites[5].SetImage(Images[5]);
        Sprites[6].SetImage(Images[6]);

        SetCenterInCenter(Sprites[0]);
        SetCenterInCenter(Sprites[1]);
        SetCenterInCenter(Sprites[2]);
        SetCenterInCenter(Sprites[3]);
        SetCenterInCenter(Sprites[4]);
        SetCenterInCenter(Sprites[5]);
        SetCenterInCenter(Sprites[6]);

        Position = Pos;
        ListPosition = LP;
        ViewPosition = VP;

        FrameID = FID;
        Duration = Dur;

        FrameIDString.SetSize(16);
        //DurationString.SetSize(16);
        FrameIDString.SetText(ToString(FrameID));
        //DurationString.SetText(ToString(Duration*1000) + " ms");

        TimeBox = GUI::FrameTimeBox(0, 0, 60, 16);
        TimeBox.SetText(ToString(Duration*1000));

        UpdateDuration = false;
    }

    void UpdateText()
    {
        FrameIDString.SetText(ToString(FrameID));
        TimeBox.SetText(ToString(Duration*1000));
    }

    bool Update(Application & App, bool isSelected, bool isCurrent)
    {
        bool ReturnValue;
        UpdateDuration = false;

        if(isSelected || TimeBox.GetActive())
        {
            if(TimeBox.GetActive())
            {
                if(!isCurrent)
                {
                    Sprites[3].SetPosition(*ViewPosition + *ListPosition + Position);
                    App().Draw(Sprites[3]);
                }
                else
                {
                    Sprites[6].SetPosition(*ViewPosition + *ListPosition + Position);
                    App().Draw(Sprites[6]);
                }
            }
            else
            {
                if(!isCurrent)
                {
                    Sprites[2].SetPosition(*ViewPosition + *ListPosition + Position);
                    App().Draw(Sprites[2]);
                }
                else
                {
                    Sprites[5].SetPosition(*ViewPosition + *ListPosition + Position);
                    App().Draw(Sprites[5]);
                }
            }

            ReturnValue = false;
        }
        else
        {
            if(PPCollision(Cursor.Position, Sprites[0]))
            {
                Sprites[1].SetPosition(*ViewPosition + *ListPosition + Position);
                App().Draw(Sprites[1]);

                ReturnValue = Cursor.Left.Pressed;
            }
            else
            {
                if(!isCurrent)
                {
                    Sprites[0].SetPosition(*ViewPosition + *ListPosition + Position);
                    App().Draw(Sprites[0]);
                }
                else
                {
                    Sprites[4].SetPosition(*ViewPosition + *ListPosition + Position);
                    App().Draw(Sprites[4]);
                }

                ReturnValue = false;
            }
        }

        if((Cursor.Left.Pressed && !TimeBox.Clicked()) || !TimeBox.Write() && !GUI::LShiftKey())
        {
            UpdateDuration = true;
            Duration = TimeBox.GetValue()/1000.f;
        }

        FrameIDString.SetPosition(*ViewPosition + *ListPosition + Vector2f(35-100+2, Position.y-2) - GetCenter(FrameIDString));
        //DurationString.SetPosition(*ViewPosition + *ListPosition + Vector2f(135-100+2, Position.y-2) - GetCenter(DurationString));
        TimeBox.SetPosition(*ListPosition + Vector2f(135-100+2, Position.y-2) - GetCenter(TimeBox.GetStr()));

        App().Draw(FrameIDString);
        TimeBox.Draw(App);
        //App().Draw(DurationString);

        return ReturnValue;
    }
};

class FrameList
{
    protected:
    Image Images[7];
    Image HeadImage;
    Sprite HeadSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool PickedUp;
    Vector2f MouseOffset;

    public:
    List< FrameListItem > Items;

    bool Selected;
    int isSelected;
    bool NewSelected;
    bool UpdateDuration;
    int UpdateDurationFrame;
    int ItemY;

    FrameList()
    {
        Images[0].LoadFromFile("Data/Image/EditorInterface/FrameBar.png");
        Images[1].LoadFromFile("Data/Image/EditorInterface/FrameBarHoover.png");
        Images[2].LoadFromFile("Data/Image/EditorInterface/FrameBarSelected.png");
        Images[3].LoadFromFile("Data/Image/EditorInterface/FrameBarWrite.png");
        Images[4].LoadFromFile("Data/Image/EditorInterface/FrameBarCurrent.png");
        Images[5].LoadFromFile("Data/Image/EditorInterface/FrameBarSelectedCurrent.png");
        Images[6].LoadFromFile("Data/Image/EditorInterface/FrameBarWriteCurrent.png");
        HeadImage.LoadFromFile("Data/Image/EditorInterface/FrameBarHead.png");

        HeadSprite.SetImage(HeadImage);
        SetCenterInCenter(HeadSprite);
        ItemY = 20;

        Position = Vector2f(200, 90);

        Selected = true;
        isSelected = 0;
        NewSelected = false;
        UpdateDuration = false;
        UpdateDurationFrame = 0;

        PickedUp = false;

        Items.AddNode(new FrameListItem(Items.GetSize()+1, 1.f, Images, Vector2f(0, ItemY), &Position, &ViewPosition));
        ItemY += 20;
    }

    void Update(Application & App, bool isPlaying, int current = 0)
    {
        NewSelected = false;
        UpdateDuration = false;

        ViewPosition.x = App().GetDefaultView().GetRect().Left;
        ViewPosition.y = App().GetDefaultView().GetRect().Top;

        if(Cursor.Left.Pressed && PPCollision(Cursor.Position, HeadSprite))
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

        HeadSprite.SetPosition(ViewPosition + Position);
        App().Draw(HeadSprite);

        for(int i = 0; i < Items.GetSize(); i++)
        {
            if(Items[i]->Update(App, (i == isSelected && Selected), (isPlaying && current == i)))
            {
                Selected = true;
                isSelected = i;
                NewSelected = true;
            }

            if(Items[i]->UpdateDuration)
            {
                UpdateDuration = true;
                UpdateDurationFrame = i;
            }
        }
    }

    void AddItem()
    {
        Items.AddNode(new FrameListItem(Items.GetSize()+1, 1.f, Images, Vector2f(0, ItemY), &Position, &ViewPosition));
        ItemY += 20;
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

class Settings
{
    public:
    Vector2f Window01Position;
    Vector2f Window02Position;

    Settings()
    {}

    void Save()
    {
        ofstream fout;
        fout.open("Settings.dat");

        fout << Window01Position.x << " " << Window01Position.y << endl;
        fout << Window02Position.x << " " << Window02Position.y << endl;

        fout.close();
    }

    void Load()
    {
        ifstream fin;
        fin.open("Settings.dat");

        fin >> Window01Position.x >> Window01Position.y;
        fin >> Window02Position.x >> Window02Position.y;

        fin.close();
    }
};

void SelectBody(Application & App, int & BodyType, string & LimbDirectory)
{
    String ctt;
    ctt.SetText("BodyTypes Available:\n0 = CharType01\n1 = CorpPeople\n2 = OldPeople01");
    ctt.SetPosition(680, 295);
    ctt.SetSize(16);
    String btt;
    btt.SetText("BodyType");
    btt.SetPosition(500, 265);
    btt.SetSize(24);
    String ldt;
    ldt.SetText("LimbDirectory");
    ldt.SetPosition(500, 345);
    ldt.SetSize(24);

    GUI::FloatBox btb(500, 300, 150, 30);
    GUI::TextBox ldb(500, 380, 150, 15);

    bool IsPlaying = true;
    while(App().IsOpened())
    {
        Event e;
        while(App().GetEvent(e))
        {
            if(e.Type == Event::Closed)
            {
                App().Close();
                IsPlaying = false;
            }

            /*if(e.Type == Event::Resized)
            {
                App().GetDefaultView().SetHalfSize(App().GetWidth()/2 * Zoom.x, App().GetHeight()/2 * Zoom.y);
                ViewScale = Vector2f((float)App().GetWidth() / (float)App.GetOriginalSize().x, (float)App().GetHeight() / (float)App.GetOriginalSize().y);
                bg.SetScale(ViewScale.x * Zoom.x, ViewScale.y * Zoom.y);
                bg.SetPosition(App().GetDefaultView().GetRect().Left, App().GetDefaultView().GetRect().Top);
            }*/

            if(e.Type == sf::Event::MouseWheelMoved)
            {
                if(e.MouseWheel.Delta > 0.f)
                {
                    App().GetDefaultView().Zoom(1.5f);
                }
                else
                if(e.MouseWheel.Delta < 0.f)
                {
                    App().GetDefaultView().Zoom(1.f/1.5f);
                }
            }

            if(e.Type == Event::KeyPressed)
            switch(e.Key.Code)
            {
                case Key::Escape:

                    App().Close();
                    IsPlaying = false;

                break;

                default:
                break;
            }
        }

        if(IsPlaying)
        {
            App.Update(Color(25, 20, 30));
            GUI::UpdateKeys(App());

            if(Cursor.Left.Pressed)
            {
                btb.Clicked(App);
                ldb.Clicked(App);
            }

            btb.Write();
            ldb.Write();

            App().Draw(ctt);
            App().Draw(btt);
            App().Draw(ldt);
            btb.Draw(App);
            ldb.Draw(App);

            if(GUI::EnterKey.Pressed)
            {
                BodyType = (int)btb.GetValue();
                LimbDirectory = ldb.GetString();

                return ;
            }

            App.Display();
        }
    }

}

int main()
{
    Application App(1280, 720, 32, "Skeleton Editor");
    App().SetFramerateLimit(120);
    Settings ProgramSettings;
    ProgramSettings.Load();

    String FPStext;
    FPStext.SetPosition(-1280/2+5, -720/2);
    FPStext.SetSize(24);

    int BodyType = CORPPEOPLE;
    int LimbType = 2;

    string LimbDirectory;

    if(BodyType == CHARTYPE01)
    {
        if(LimbType == 1)
        LimbDirectory = "KingOfLimbs";
        else if(LimbType == 2)
        LimbDirectory = "FirstBody";
    }
    else if(BodyType == CORPPEOPLE)
    {
        LimbDirectory = "CorporatePeople";
    }

    SelectBody(App, BodyType, LimbDirectory);

    Skeleton skeleton(BodyType);
    skeleton.CreateLimbs(LimbDirectory);
    skeleton.Play = false;
//    {
//        skeleton.LoadAnimation("Default.anim");
//    }
//    skeleton.CreateLimbs();

    int BodyGroup = 0;
    int Selected = -1;
    bool PickUp = false;
    Vector2f PickUpOffset;
    bool AngleMode = false;

    bool DrawBones = true;
    bool showColorID = false;
    bool GravityOn = true;
    bool DrawAllLimbs = true;
    bool DrawFloor = true;
    bool DrawMask = false;

    Vector2f CursorPrevPosition;

    Shape GroundLine = Shape::Line(-100, 76, 100, 76, 2, Color::White);
    Image MaskImage;
    Sprite Mask = LoadSprite(MaskImage, "Data/Image/Mask2.png");
    Mask.SetCenter(Mask.GetSize().x/2, Mask.GetSize().y-75);

    Image ABImages[3];
    ABImages[0].LoadFromFile("Data/Image/EditorInterface/NewButtonIdle.png");
    ABImages[1].LoadFromFile("Data/Image/EditorInterface/NewButtonHoover.png");
    ABImages[2].LoadFromFile("Data/Image/EditorInterface/NewButtonPressed.png");

    Image CBImages[4];
    CBImages[0].LoadFromFile("Data/Image/EditorInterface/CBIdle.png");
    CBImages[1].LoadFromFile("Data/Image/EditorInterface/CBHoover.png");
    CBImages[2].LoadFromFile("Data/Image/EditorInterface/CBPressed.png");
    CBImages[3].LoadFromFile("Data/Image/EditorInterface/CBCross.png");

    Image WindowImage[2];
    WindowImage[0].LoadFromFile("Data/Image/EditorInterface/FinalWindow01.png");
    WindowImage[1].LoadFromFile("Data/Image/EditorInterface/FinalWindow02.png");

    FrameList FL;
    FL.SetPosition(ProgramSettings.Window01Position);

    MainButtonsWindow MainWindow(WindowImage[1], ABImages, CBImages);
    MainWindow.SetPosition(ProgramSettings.Window02Position);

    SaveLoadWindow SLWindow(WindowImage[0], ABImages);

    App().GetDefaultView().SetCenter(0, 0);

    bool IsPlaying = true;
    while(App().IsOpened())
    {
        Event e;
        while(App().GetEvent(e))
        {
            if(e.Type == Event::Closed)
            {
                App().Close();
                IsPlaying = false;
            }

            /*if(e.Type == Event::Resized)
            {
                App().GetDefaultView().SetHalfSize(App().GetWidth()/2 * Zoom.x, App().GetHeight()/2 * Zoom.y);
                ViewScale = Vector2f((float)App().GetWidth() / (float)App.GetOriginalSize().x, (float)App().GetHeight() / (float)App.GetOriginalSize().y);
                bg.SetScale(ViewScale.x * Zoom.x, ViewScale.y * Zoom.y);
                bg.SetPosition(App().GetDefaultView().GetRect().Left, App().GetDefaultView().GetRect().Top);
            }*/

            if(e.Type == sf::Event::MouseWheelMoved)
            {
                if(e.MouseWheel.Delta > 0.f)
                {
                    App().GetDefaultView().Zoom(1.5f);
                }
                else
                if(e.MouseWheel.Delta < 0.f)
                {
                    App().GetDefaultView().Zoom(1.f/1.5f);
                }
            }

            if(e.Type == Event::KeyPressed)
            switch(e.Key.Code)
            {
                case Key::Escape:

                    App().Close();
                    IsPlaying = false;

                break;

                case Key::Space:

                    DrawBones = !DrawBones;

                break;

                case Key::Numpad0:

                    DrawAllLimbs = !DrawAllLimbs;

                break;

                case Key::F1:

                    showColorID = !showColorID;

                break;

                case Key::F2:

                    GravityOn = !GravityOn;

                break;

                case Key::F5:

                    AngleMode = !AngleMode;

                break;

                /*case Key::F7:

                    skeleton.AddFrame();

                break;

                case Key::F8:

                    skeleton.Play = !skeleton.Play;

                break;

                case Key::F11:

                    skeleton.LoadFrame("DefaultFrame");

                break;

                case Key::F12:

                    skeleton.SaveFrame("Skeleton.txt");

                break;*/

                default:
                break;
            }
        }

        if(IsPlaying)
        {
            App.Update(Color(75, 75, 75));
            GUI::UpdateKeys(App());

            if(DrawFloor)
            App().Draw(GroundLine);

            if(DrawMask)
            App().Draw(Mask);

            skeleton.Draw(App, false, 0, DrawBones, DrawAllLimbs);

            if(DrawBones)
            for(int i = 0; i < skeleton.GetvList().GetSize(); i++)
            {
                App().Draw(skeleton.GetJointSprite(i, showColorID));
            }

            /// rArm
            {
                if(GravityOn && skeleton.bones.rArmGravity)
                for(int i = 0; i < skeleton.rArm.GetSize(); i++)
                {
                    skeleton.rArm[i]->SkeletonPoint->Vector.y += 500.f * App.GetFrameTime();
                }

                for(int i = skeleton.rArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                    skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                }

                float Dist = Distance(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector);
                skeleton.rArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector), Dist-skeleton.rArm[0]->Dist);

                for(int i = 1; i < skeleton.rArm.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector);
                    skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i]->Dist);
                }

                for(int i = skeleton.rArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                    skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                }
            }

            /// lArm
            {
                if(GravityOn && skeleton.bones.lArmGravity)
                for(int i = 0; i < skeleton.lArm.GetSize(); i++)
                {
                    skeleton.lArm[i]->SkeletonPoint->Vector.y += 400.f * App.GetFrameTime();
                }

                for(int i = skeleton.lArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                    skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                }

                float Dist = Distance(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector);
                skeleton.lArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector), Dist-skeleton.lArm[0]->Dist);

                for(int i = 1; i < skeleton.lArm.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector);
                    skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i]->Dist);
                }

                for(int i = skeleton.lArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                    skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                }
            }

            /// rLeg
            {
                if(GravityOn && skeleton.bones.rLegGravity)
                for(int i = 0; i < skeleton.rLeg.GetSize(); i++)
                {
                    skeleton.rLeg[i]->SkeletonPoint->Vector.y += 400.f * App.GetFrameTime();
                }

                for(int i = skeleton.rLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                }

                float Dist = Distance(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector);
                skeleton.rLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector), Dist-skeleton.rLeg[0]->Dist);

                for(int i = 1; i < skeleton.rLeg.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector);
                    skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i]->Dist);
                }

                for(int i = skeleton.rLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                }
            }

            /// lLeg
            {
                if(GravityOn && skeleton.bones.lLegGravity)
                for(int i = 0; i < skeleton.lLeg.GetSize(); i++)
                {
                    skeleton.lLeg[i]->SkeletonPoint->Vector.y += 400.f * App.GetFrameTime();
                }

                for(int i = skeleton.lLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                }

                float Dist = Distance(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector);
                skeleton.lLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector), Dist-skeleton.lLeg[0]->Dist);

                for(int i = 1; i < skeleton.lLeg.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector);
                    skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i]->Dist);
                }

                for(int i = skeleton.lLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                }
            }

            /// Body
            {
                if(BodyType == CHARTYPE01)
                {
                    for(int i = skeleton.Body.GetSize()-3; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }

                    float Dist = Distance(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector);
                    skeleton.Body[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector), Dist-skeleton.Body[0]->Dist);

                    for(int i = 1; i < skeleton.Body.GetSize()-1; i++)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector), Dist-skeleton.Body[i]->Dist);
                    }

                    for(int i = skeleton.Body.GetSize()-3; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }
                }
                else if(BodyType == CORPPEOPLE)
                {
                    for(int i = skeleton.Body.GetSize()-2; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }

                    float Dist = Distance(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector);
                    skeleton.Body[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector), Dist-skeleton.Body[0]->Dist);

                    for(int i = 1; i < skeleton.Body.GetSize(); i++)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector), Dist-skeleton.Body[i]->Dist);
                    }

                    for(int i = skeleton.Body.GetSize()-2; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }
                }
            }

            if(Cursor.Left.Pressed || Cursor.Right.Pressed)
            {
                for(int i = 0; i < skeleton.rArm.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(RARM, i)))
                    {
                        PickUp = true;
                        BodyGroup = RARM;

                        PickUpOffset = Cursor.Position - skeleton.rArm[i]->SkeletonPoint->Vector;

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.lArm.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(LARM, i)))
                    {
                        PickUp = true;
                        BodyGroup = LARM;

                        PickUpOffset = Cursor.Position - skeleton.lArm[i]->SkeletonPoint->Vector;

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.Body.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(BODY, i)))
                    {
                        PickUp = true;
                        BodyGroup = BODY;

                        PickUpOffset = Cursor.Position - skeleton.Body[i]->SkeletonPoint->Vector;

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.rLeg.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(RLEG, i)))
                    {
                        PickUp = true;
                        BodyGroup = RLEG;

                        PickUpOffset = Cursor.Position - skeleton.rLeg[i]->SkeletonPoint->Vector;

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.lLeg.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(LLEG, i)))
                    {
                        PickUp = true;
                        BodyGroup = LLEG;

                        PickUpOffset = Cursor.Position - skeleton.lLeg[i]->SkeletonPoint->Vector;

                        Selected = i;
                    }
                }

                if(PPCollision(Cursor.Position, skeleton.GetJointSprite(ALL)))
                {
                    PickUp = true;
                    BodyGroup = ALL;

                    PickUpOffset = Cursor.Position - skeleton.Body[0]->SkeletonCenter->Vector;
                }
            }

            if(PickUp)
            {
                if(BodyGroup == RARM)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.rArm[Selected]->SkeletonPoint->Vector, skeleton.rArm[Selected]->SkeletonCenter->Vector,
                            skeleton.rArm[Selected]->Rotation - Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.rArm[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.rArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.rArm.GetSize(); i++)
                            {
                                skeleton.rArm[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.rArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.rArmGravity = !skeleton.bones.rArmGravity;

                            Vector2f Pos1 = skeleton.rArm[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.rArm[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rArm[Selected]->SkeletonPoint->Vector;

                            skeleton.rArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector);
                            skeleton.rArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector), Dist-skeleton.rArm[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.rArm.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rArm[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.rArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.rArm.GetSize(); i++)
                        {
                            skeleton.rArm[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.rArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.rArm[Selected]->Dist = Distance(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == LARM)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.lArm[Selected]->SkeletonPoint->Vector, skeleton.lArm[Selected]->SkeletonCenter->Vector,
                            skeleton.lArm[Selected]->Rotation - Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.lArm[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.lArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.lArm.GetSize(); i++)
                            {
                                skeleton.lArm[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.lArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.lArmGravity = !skeleton.bones.lArmGravity;

                            Vector2f Pos1 = skeleton.lArm[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.lArm[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lArm[Selected]->SkeletonPoint->Vector;

                            skeleton.lArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector);
                            skeleton.lArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector), Dist-skeleton.lArm[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.lArm.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lArm[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.lArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.lArm.GetSize(); i++)
                        {
                            skeleton.lArm[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.lArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.lArm[Selected]->Dist = Distance(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == BODY)
                {
//                    if(AngleMode)
                    {
                        Vector2f PositionDelta = RotatePoint(skeleton.Body[Selected]->SkeletonPoint->Vector, skeleton.Body[Selected]->SkeletonCenter->Vector,
                        skeleton.Body[Selected]->Rotation - Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.Body[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.Body[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.Body.GetSize(); i++)
                        {
                            skeleton.Body[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.Body[i]->SkeletonPoint->Vector = RotatePoint(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }
                    }
//                    else
//                    {
//                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.Body[Selected]->SkeletonPoint->Vector;
//                        float RotationDelta = (Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector + PositionDelta) -
//                                               Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector)) * DPR;
//                        skeleton.Body[Selected]->SkeletonPoint->Vector += PositionDelta;
//
//                        for(int i = Selected+1; i < skeleton.Body.GetSize(); i++)
//                        {
//                            skeleton.Body[i]->SkeletonPoint->Vector += PositionDelta;
//                            skeleton.Body[i]->SkeletonPoint->Vector = RotatePoint(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[Selected+1]->SkeletonCenter->Vector, RotationDelta);
//                        }
//                    }
                }
                else if(BodyGroup == RLEG)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.rLeg[Selected]->SkeletonPoint->Vector, skeleton.rLeg[Selected]->SkeletonCenter->Vector,
                            skeleton.rLeg[Selected]->Rotation - Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.rLeg[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.rLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.rLeg.GetSize(); i++)
                            {
                                skeleton.rLeg[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.rLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.rLegGravity = !skeleton.bones.rLegGravity;

                            Vector2f Pos1 = skeleton.rLeg[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.rLeg[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rLeg[Selected]->SkeletonPoint->Vector;

                            skeleton.rLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector);
                            skeleton.rLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector), Dist-skeleton.rLeg[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.rLeg.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rLeg[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.rLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.rLeg.GetSize(); i++)
                        {
                            skeleton.rLeg[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.rLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.rLeg[Selected]->Dist = Distance(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == LLEG)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.lLeg[Selected]->SkeletonPoint->Vector, skeleton.lLeg[Selected]->SkeletonCenter->Vector,
                            skeleton.lLeg[Selected]->Rotation - Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.lLeg[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.lLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.lLeg.GetSize(); i++)
                            {
                                skeleton.lLeg[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.lLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.lLegGravity = !skeleton.bones.lLegGravity;

                            Vector2f Pos1 = skeleton.lLeg[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.lLeg[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lLeg[Selected]->SkeletonPoint->Vector;

                            skeleton.lLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector);
                            skeleton.lLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector), Dist-skeleton.lLeg[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.lLeg.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lLeg[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.lLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.lLeg.GetSize(); i++)
                        {
                            skeleton.lLeg[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.lLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.lLeg[Selected]->Dist = Distance(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == ALL)
                {
                    if(Cursor.Left())
                    skeleton.Body[0]->SkeletonCenter->Vector = Cursor.Position - PickUpOffset;
                    else if(Cursor.Right())
                    {
                        for(int i = 0; i < skeleton.GetvList().GetSize(); i++)
                        {
                            skeleton.GetvList()[i]->Vector += Cursor.Position - CursorPrevPosition;
                        }
                    }
                }

                if(Cursor.Left.Released || (Cursor.Right.Released && !Cursor.Left()))
                PickUp = false;
            }

            MainWindow.Update(App, skeleton.Play);

            if(MainWindow.AddFrameButtonPressed)
            {
                skeleton.AddFrame();
                FL.AddItem();
                FL.Items[FL.Items.GetSize() - 1]->Duration = FL.Items[FL.isSelected]->Duration;

                FL.isSelected = FL.Items.GetSize() - 1;
                skeleton.Frames[FL.isSelected]->Time = FL.Items[FL.isSelected]->Duration;
                FL.Items[FL.isSelected]->UpdateText();

                //for(int i = 0; i < skeleton.bones.vectors.GetSize(); i++)
                //skeleton.bones.vectors[i]->Vector = skeleton.Frames[FL.isSelected]->vectors[i]->Vector;
            }

            if(FL.UpdateDuration)
            {
                for(int i = 0; i < skeleton.Frames.GetSize(); i++)
                skeleton.Frames[i]->Time = FL.Items[i]->Duration;
            }

            if(GUI::DeleteKey.Pressed && FL.Items.GetSize() > 1)
            {
                int DeleteFrame = FL.isSelected;

                FL.Items.RemoveNode(DeleteFrame);
                skeleton.RemoveFrame(DeleteFrame);

                if(DeleteFrame >= FL.Items.GetSize())
                FL.isSelected -= 1;

                for(int i = DeleteFrame; i < FL.Items.GetSize(); i++)
                {
                    FL.Items[i]->FrameID -= 1;
                    FL.Items[i]->Position.y -= 20;
                    FL.Items[i]->UpdateText();
                }

                FL.ItemY -= 20;

                for(int i = 0; i < skeleton.bones.vectors.GetSize(); i++)
                skeleton.bones.vectors[i]->Vector = skeleton.Frames[FL.isSelected]->vectors[i]->Vector;
            }

            if(skeleton.Play)
            {
                if(MainWindow.StopButtonPressed)
                {
                    skeleton.Play = !skeleton.Play;

                    skeleton.Index = 0;
                    skeleton.TimeCounter = 0.f;

                    for(int i = 0; i < skeleton.bones.vectors.GetSize(); i++)
                    skeleton.bones.vectors[i]->Vector = skeleton.Frames[FL.isSelected]->vectors[i]->Vector;
                }
            }
            else
            {
                if(FL.NewSelected)
                {
                    for(int i = 0; i < skeleton.bones.vectors.GetSize(); i++)
                    skeleton.bones.vectors[i]->Vector = skeleton.Frames[FL.isSelected]->vectors[i]->Vector;
                }

                for(int i = 0; i < skeleton.bones.vectors.GetSize(); i++)
                skeleton.Frames[FL.isSelected]->vectors[i]->Vector = skeleton.bones.vectors[i]->Vector;

                if(MainWindow.PlayButtonPressed)
                skeleton.Play = !skeleton.Play;
            }

            if(MainWindow.SaveButtonPressed)
            {
                SLWindow.Activate(SAVE);
            }

            if(MainWindow.LoadButtonPressed)
            {
                SLWindow.Activate(LOAD);
            }

            if(MainWindow.CenterCurrentButtonPressed)
            {
                skeleton.CenterCurrent(FL.isSelected);
            }

            if(MainWindow.CenterAllButtonPressed)
            {
                skeleton.CenterAll(FL.isSelected);
            }

            if(MainWindow.DrawFloorCBPressed)
            {
                DrawFloor = MainWindow.DrawFloorCB.GetValue();
            }

            if(MainWindow.DrawMaskCBPressed)
            {
                DrawMask = MainWindow.DrawMaskCB.GetValue();
            }

            SLWindow.Update(App);

            if(SLWindow.Active && SLWindow.GetFileName() != "" && (SLWindow.SaveLoadButtonPressed || GUI::EnterKey.Pressed))
            {
                if(SLWindow.SaveLoadMode == LOAD)
                {
                    FL.isSelected = 0;

                    if(BodyType == CHARTYPE01)
                    skeleton.LoadAnimation("Data/Animation/Skeleton/CharType01/" + SLWindow.GetFileName() + ".anm");
                    else if(BodyType == CORPPEOPLE)
                    skeleton.LoadAnimation("Data/Animation/Skeleton/CorpPeople/" + SLWindow.GetFileName() + ".anm");

                    FL.Items.Clear();
                    FL.ItemY = 20;

                    for(int i = 0; i < skeleton.Frames.GetSize(); i++)
                    {
                        FL.AddItem();
                        FL.Items[i]->Duration = skeleton.Frames[i]->Time;
                        FL.Items[i]->UpdateText();
                    }

//                    if(Selected >= FL.Items.GetSize())
//                    Selected = FL.Items.GetSize()-1;
                }
                else if(SLWindow.SaveLoadMode == SAVE)
                {
                    if(BodyType == CHARTYPE01)
                    skeleton.SaveAnimation("Data/Animation/Skeleton/CharType01/" + SLWindow.GetFileName() + ".anm");
                    else if(BodyType == CORPPEOPLE)
                    skeleton.SaveAnimation("Data/Animation/Skeleton/CorpPeople/" + SLWindow.GetFileName() + ".anm");
                }

                SLWindow.Active = false;
            }

            FL.Update(App, skeleton.Play, skeleton.Index);

            FPStext.SetText("FPS: " + ToString(int(1/App.GetFrameTime())));
            App().Draw(FPStext);

            App.Display();

            CursorPrevPosition = Cursor.Position;
        }
    }

    ProgramSettings.Window01Position = FL.GetPosition();
    ProgramSettings.Window02Position = MainWindow.GetPosition();
    ProgramSettings.Save();

    return 0;
}
