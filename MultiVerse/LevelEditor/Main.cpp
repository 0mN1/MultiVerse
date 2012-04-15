#include "../SFMLfunctions.hpp"
#include "../GUI.hpp"
#include "../LevelScript.hpp"
#include "List.hpp"

using LIST::List;

enum {EDITOR, ADDIMAGE, ADDANIMATION, ADDSOLID, ADDARTIFACT, ADDMUSICTRACK, ADDEDGEBOX, ADDACTIONBOX, ADDSPAWNPOINT, ADDNPC, ADDAI};
enum {ADDING, IMAGEPROPERTIES, SOLIDPROPERTIES, ARTIFACTPROPERTIES};
enum {WORLD, IMAGE, ANIMATION, SOLID, ARTIFACT, MUSICTRACK, EDGEBOX, ACTIONBOX, SPAWNPOINT, NPC, AI};

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

bool Collision(Sprite & s1, Sprite & s2)
{
    return (s1.GetPosition().x - s1.GetCenter().x < s2.GetPosition().x + s2.GetSize().x  - s2.GetCenter().x
        &&  s1.GetPosition().x + s1.GetSize().x - s1.GetCenter().x > s2.GetPosition().x - s2.GetCenter().x
        &&  s1.GetPosition().y - s1.GetCenter().y < s2.GetPosition().y + s2.GetSize().y - s2.GetCenter().y
        &&  s1.GetPosition().y + s1.GetSize().y - s1.GetCenter().y > s2.GetPosition().y - s2.GetCenter().y);
}

bool Surrounded(Sprite & s1, Sprite & s2)
{
    return (s1.GetPosition().x - s1.GetCenter().x > s2.GetPosition().x - s2.GetCenter().x
        &&  s1.GetPosition().x - s1.GetCenter().x + s1.GetSize().x < s2.GetPosition().x - s2.GetCenter().x + s2.GetSize().x
        &&  s1.GetPosition().y - s1.GetCenter().y > s2.GetPosition().y - s2.GetCenter().y
        &&  s1.GetPosition().y - s1.GetCenter().y + s1.GetSize().y < s2.GetPosition().y - s2.GetCenter().y + s2.GetSize().y);
}

class SettingsHandler
{
    public:
    string EditorSettingsFileName;
    string LvlFileName;

    SettingsHandler()
    {
        EditorSettingsFileName = "EditorSettings.txt";

        Load();
    }

    ~SettingsHandler()
    {
        Save();
    }

    void Load()
    {
        ifstream fin;
        fin.open(EditorSettingsFileName.c_str());

        fin >> LvlFileName;

        fin.close();
    }

    void Save()
    {
        ofstream fout;
        fout.open(EditorSettingsFileName.c_str());

        fout << LvlFileName << endl;

        fout.close();
    }
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
    bool Pressed;

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
        if(!GUI::SpaceKey() && PPCollision(Cursor.Position, Sprites[index]))
        {
            if(Cursor.Left())
            index = 2;
            else
            index = 1;
        }
        else
        index = 0;

        ViewPosition = VP;

        Sprites[index].SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
        Text.SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

        Sprites[index].SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
        Text.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

        App().Draw(Sprites[index]);
        App().Draw(Text);

        if(index == 1 && Cursor.Left.Released && !GUI::SpaceKey())
        return Pressed = true;
        else
        return Pressed = false;
    }
};

class GUIString
{
    public:
    String Text;
    Vector2f Position;
    Vector2f ViewPosition;

    GUIString()
    {}

    GUIString(string text, Vector2f position, int size = 20)
    {
        Position = position;
        Text.SetText(text);
        Text.SetPosition(Position);
        Text.SetSize(size);
    }

    void SetSize(int size)
    {
        Text.SetSize(size);
    }

    void Draw(Application & App, Vector2f viewPosition)
    {
        ViewPosition = viewPosition;

        Text.SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
        Text.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

        App().Draw(Text);
    }
};

class ImagePreview
{
    protected:
    Sprite Preview;
    int Size;
    Vector2f Position;
    Vector2f ViewPosition;
    float resizer;

    public:
    ImagePreview(int size = 0, Vector2f position = Vector2f(0, 0))
    {
        Position = position;
        Size = size;
    }

    void SetImage(Image & Source)
    {
        Preview.SetScale(1, 1);

        Preview = Sprite(Source);

        SetCenterInCenter(Preview);

        if(Preview.GetSize().x >= Preview.GetSize().y)
        resizer = Size / Preview.GetSize().x;
        else
        resizer = Size / Preview.GetSize().y;

        Preview.SetScale(resizer, resizer);
    }

    void SetImage(const Image * Source)
    {
        Preview.SetScale(1, 1);

        Preview = Sprite(*Source);

        SetCenterInCenter(Preview);

        if(Preview.GetSize().x >= Preview.GetSize().y)
        resizer = Size / Preview.GetSize().x;
        else
        resizer = Size / Preview.GetSize().y;

        Preview.SetScale(resizer, resizer);
    }

    void Draw(Application & App, Vector2f viewPosition)
    {
        ViewPosition = viewPosition;

        Preview.SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x + ((Size/2)/App.GetZoom().x), Position.y / App.GetZoom().y + ((Size/2)/App.GetZoom().x)));
        Preview.SetScale(Vector2f(1 / App.GetZoom().x * resizer, 1 / App.GetZoom().y * resizer));

        Shape box = Shape::Rectangle(Position, Position + Vector2f(Size, Size), Color(0, 0, 0, 50), 1, Color(255, 255, 255, 50));

        box.SetPosition(ViewPosition);
        box.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

        App().Draw(box);
        App().Draw(Preview);
    }
};

class WindowBase
{
    protected:
    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool PickedUp;
    Vector2f MouseOffset;
    String NameText;

    public:
    void UpdateWindow(Application & App)
    {
        ViewPosition.x = App().GetDefaultView().GetRect().Left;
        ViewPosition.y = App().GetDefaultView().GetRect().Top;

        if(Cursor.Left.Pressed && !GUI::SpaceKey() && Cursor.Position.y < ViewPosition.y + (Position.y + 20)/App.GetZoom().y && PPCollision(Cursor.Position, WindowSprite))
        {
            PickedUp = true;
            MouseOffset = Vector2f(Position.x - Cursor.Position.x*App.GetZoom().x,
                                   Position.y - Cursor.Position.y*App.GetZoom().y);
        }

        if(PickedUp)
        {
            if(!Cursor.Left())
            PickedUp = false;
            else
            Position = Vector2f(Cursor.Position.x*App.GetZoom().x + MouseOffset.x,
                                    Cursor.Position.y*App.GetZoom().y + MouseOffset.y);
        }

        WindowSprite.SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
        NameText.SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

        WindowSprite.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
        NameText.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

        App().Draw(WindowSprite);
        App().Draw(NameText);
    }

    inline Vector2f GetPosition()
    {
        return Position;
    }

    inline void SetPosition(Vector2f Pos)
    {
        Position = Pos;
    }

    bool CursorCollision()
    {
        return PPCollision(Cursor.Position, WindowSprite);
    }
};

class FileBrowserItem
{
    protected:
    Sprite Icon;
    string Name;
    String FileName;
    bool isDirectory;
    Vector2f Position;
    Vector2f TextPosition;
    Vector2f OriginalSize;

    public:
    FileBrowserItem()
    {
        isDirectory = false;
    }

    FileBrowserItem(Image & icon, string name, Vector2f Pos, bool isDir = false, bool isKnown = true)
    {
        Icon.SetImage(icon);
        SetCenterInCenter(Icon);
        OriginalSize = Icon.GetSize();
        Name = name;
        Position = Pos;
        isDirectory = isDir;

        TextPosition = Vector2f(0, 23);

        string extLess;

        if(!isDirectory && isKnown)
        {
            int extStart = 0;

            for(int i = Name.size(); i > 0; i--)
            {
                if(Name[i] == '.')
                {
                    extStart = i;
                    break;
                }
            }

            for(int i = 0; i < extStart; i++)
            {
                extLess += Name[i];
            }
        }
        else
        {
            extLess = Name;
        }

        FileName.SetText(extLess);
        FileName.SetSize(12);
        FileName.SetCenter(GetCenter(FileName));
        FileName.SetColor(Color(0, 0, 0));
    }

    bool IsDirectory()
    {
        return isDirectory;
    }

    string GetName()
    {
        return Name;
    }

    bool Update(Application & App, Vector2f Offset, Sprite & WindowSprite, float ScrollAmount)
    {
        FileName.SetPosition(Offset.x + (Position.x + TextPosition.x) / App.GetZoom().x,
                             Offset.y + (Position.y + TextPosition.y + ScrollAmount) / App.GetZoom().y);

        Icon.SetSubRect(IntRect(0, 0, (int)OriginalSize.x, (int)OriginalSize.y));
        Icon.SetPosition(Offset.x + Position.x / App.GetZoom().x, Offset.y + (Position.y + ScrollAmount) / App.GetZoom().y);

        if(Collision(Icon, WindowSprite))
        {
            if(LowerRight(WindowSprite).y < LowerRight(Icon).y)
            {
                Icon.SetSubRect(IntRect(0, 0, (int)OriginalSize.x, (int)(Icon.GetSize().y + LowerRight(WindowSprite).y - LowerRight(Icon).y)));
            }
            else if(UpperLeft(WindowSprite).y+20> UpperLeft(Icon).y)
            {
                Icon.SetSubRect(IntRect(0, (int)((OriginalSize.y - Icon.GetSize().y) + UpperLeft(WindowSprite).y+20 -
                UpperLeft(Icon).y), (int)OriginalSize.x, (int)Icon.GetSize().y));

                Icon.SetPosition(Icon.GetPosition().x,
                                 Icon.GetPosition().y + (OriginalSize.y - Icon.GetSize().y) / App.GetZoom().y);
            }

            Icon.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
            App().Draw(Icon);

            if(FileName.GetRect().Top > UpperLeft(WindowSprite).y+15
            && FileName.GetRect().Bottom < LowerRight(WindowSprite).y)
            {
                FileName.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
                App().Draw(FileName);
            }
        }

        if(Cursor.Left.Pressed && PPCollision(Cursor.Position, Icon))
            return true;

        return false;
    }
};

class FileBrowser
{
    protected:

    Sprite WindowSprite;
    Vector2f Position;
    Vector2f ViewPosition;
    bool PickedUp;
    Vector2f MouseOffset;
    String NameText;

    Image emptFoldImage;
    Image fullFoldImage;
    Image unknownImage;
    Image exeImage;
    Image dllImage;
    Image datImage;
    Image zipImage;
    Image txtImage;
    Image lvlImage;
    Image anmImage;
    Image pngImage;
    Image jpgImage;
    Image wavImage;
    Image mp3Image;
    Image oggImage;

    string SelectedFileName;

    string Path;
    List< string > Content;
    List< FileBrowserItem > Items;

    bool SelectDirectory;
    bool Active;

    float ScrollAmount;
    float ScrollChange;

    public:
    FileBrowser(Image & WindowImage)
    {
        WindowSprite.SetImage(WindowImage);

        emptFoldImage.LoadFromFile("Data/Image/EditorInterface/Icons/emptFold.png");
        fullFoldImage.LoadFromFile("Data/Image/EditorInterface/Icons/fullFold.png");
        unknownImage.LoadFromFile("Data/Image/EditorInterface/Icons/unknown.png");

        exeImage.LoadFromFile("Data/Image/EditorInterface/Icons/exe.png");
        dllImage.LoadFromFile("Data/Image/EditorInterface/Icons/dll.png");
        datImage.LoadFromFile("Data/Image/EditorInterface/Icons/dat.png");
        zipImage.LoadFromFile("Data/Image/EditorInterface/Icons/zip.png");
        txtImage.LoadFromFile("Data/Image/EditorInterface/Icons/txt.png");
        lvlImage.LoadFromFile("Data/Image/EditorInterface/Icons/lvl.png");
        anmImage.LoadFromFile("Data/Image/EditorInterface/Icons/anm.png");
        pngImage.LoadFromFile("Data/Image/EditorInterface/Icons/png.png");
        jpgImage.LoadFromFile("Data/Image/EditorInterface/Icons/jpg.png");
        wavImage.LoadFromFile("Data/Image/EditorInterface/Icons/wav.png");
        mp3Image.LoadFromFile("Data/Image/EditorInterface/Icons/mp3.png");
        oggImage.LoadFromFile("Data/Image/EditorInterface/Icons/ogg.png");

        Position = Vector2f(1400/2-250, 900/2-160);
        PickedUp = false;

        Path = "Data/";
        OpenDirectory(Path);

        SelectDirectory = false;
        Active = false;

        ScrollAmount = 0.f;
        ScrollChange = 0.f;

        NameText.SetText("FileBrowser: " + Path);
    }

    string GetPath()
    {
        return SelectedFileName;
    }

    bool GetActive()
    {
        return Active;
    }

    void Activate(bool DirSelection = false)
    {
        Path = "Data/";
        OpenDirectory(Path);

        //Position = Vector2f(700, 450);

        Active = true;
        SelectDirectory = DirSelection;
    }

    void OpenDirectory(string dir)
    {
        if(Items.GetSize() > 0)
        Items.Clear();

        if(Content.GetSize() > 0)
        Content.Clear();

        Path = dir;
        ExploreDirectory(Path, Content);

        Vector2f ItemPosition = Vector2f(65, 50);

        for(int i = 0; i < Content.GetSize(); i++)
        {
            string CurrentFile = *Content[i];

            if(CurrentFile != ".")
            {
                if(IsDirectory(Path + CurrentFile))
                {
                    List< string > explore;
                    ExploreDirectory(Path + CurrentFile, explore);

                    if(explore.GetSize() > 2)
                    Items.AddNode(FileBrowserItem(fullFoldImage, CurrentFile, ItemPosition, true));
                    else
                    Items.AddNode(FileBrowserItem(emptFoldImage, CurrentFile, ItemPosition, true));
                }
                else
                {
                    string ext;
                    bool haveExt = false;

                    for(int i = CurrentFile.size()-1; i > (int)CurrentFile.size() - 7; i--)
                    {
                        if(CurrentFile[i] == '.')
                        {
                            haveExt = true;
                            break;
                        }

                        ext += CurrentFile[i];
                    }

                    if(haveExt)
                    {
                        string reverseExt;

                        for(int i = ext.size()-1; i >= 0; i--)
                        {
                            reverseExt += ext[i];
                        }

                        CaseShift(reverseExt, TO_LOWER);

                        if(reverseExt == "txt")
                        Items.AddNode(FileBrowserItem(txtImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "exe")
                        Items.AddNode(FileBrowserItem(exeImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "dll")
                        Items.AddNode(FileBrowserItem(dllImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "dat")
                        Items.AddNode(FileBrowserItem(datImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "zip")
                        Items.AddNode(FileBrowserItem(zipImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "lvl")
                        Items.AddNode(FileBrowserItem(lvlImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "anm")
                        Items.AddNode(FileBrowserItem(anmImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "png")
                        Items.AddNode(FileBrowserItem(pngImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "jpg")
                        Items.AddNode(FileBrowserItem(jpgImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "wav")
                        Items.AddNode(FileBrowserItem(wavImage, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "mp3")
                        Items.AddNode(FileBrowserItem(mp3Image, CurrentFile, ItemPosition, false));
                        else if(reverseExt == "ogg")
                        Items.AddNode(FileBrowserItem(oggImage, CurrentFile, ItemPosition, false));
                        else
                        Items.AddNode(FileBrowserItem(unknownImage, CurrentFile, ItemPosition, false, false));
                    }
                    else
                    Items.AddNode(FileBrowserItem(unknownImage, CurrentFile, ItemPosition, false, false));
                }

                ItemPosition.x += 120;

                if(ItemPosition.x > 500 - 20)
                {
                    ItemPosition.x = 65;
                    ItemPosition.y += 65;
                }
            }
        }
    }

    bool CursorCollision()
    {
        return PPCollision(Cursor.Position, WindowSprite);
    }

    bool Update(Application & App)
    {
        if(Active)
        {
            ViewPosition.x = App().GetDefaultView().GetRect().Left;
            ViewPosition.y = App().GetDefaultView().GetRect().Top;

            if(Cursor.Left.Pressed && Cursor.Position.y < ViewPosition.y + (Position.y + 20)/App.GetZoom().y && PPCollision(Cursor.Position, WindowSprite))
            {
                PickedUp = true;
                MouseOffset = Vector2f(Position.x - Cursor.Position.x*App.GetZoom().x,
                                       Position.y - Cursor.Position.y*App.GetZoom().y);
            }

            if(PickedUp)
            {
                if(!Cursor.Left())
                PickedUp = false;
                else
                Position = Vector2f(Cursor.Position.x*App.GetZoom().x + MouseOffset.x,
                                    Cursor.Position.y*App.GetZoom().y + MouseOffset.y);
            }

            if((Cursor.ScrollUp || Cursor.ScrollDown) && PPCollision(Cursor.Position, WindowSprite))
            {
                if(Cursor.ScrollUp)
                {
                    ScrollAmount += 65/4.f;

                    if(ScrollAmount > 0)
                    ScrollAmount = 0.f;
                }
                else if(Cursor.ScrollDown)
                {
                    ScrollAmount -= 65/4.f;

//                    if(ScrollAmount > 0)
//                    ScrollAmount = 0.f;
                    if(Items.GetSize()/4.f > 4.f && Items.GetSize()/4.f <= 5.f)
                    {
                        if(ScrollAmount < int(Items.GetSize()/4.f - 5.f) * -65.f + -65.f/2.f)
                        ScrollAmount = int(Items.GetSize()/4.f - 5.f) * -65.f + -65.f/2.f;
                    }
                    else if(Items.GetSize()/4.f > 5.f)
                    {
                        if(ScrollAmount < int(Items.GetSize()/4.f - 4.f) * -65.f + -65.f/2.f)
                        ScrollAmount = int(Items.GetSize()/4.f - 4.f) * -65.f + -65.f/2.f;
                    }
                    else
                    {
                        ScrollAmount = 0.f;
                    }
                }
            }

            if(Cursor.Right.Pressed && PPCollision(Cursor.Position, WindowSprite))
            {
                MouseOffset = Cursor.Position;
            }

            if(Cursor.Right.Hold && PPCollision(Cursor.Position, WindowSprite))
            {
                ScrollChange = Cursor.Position.y - MouseOffset.y;

                if(ScrollAmount + ScrollChange > 0)
                {
                    ScrollChange = 0.f;
                    ScrollAmount = 0.f;

//                    if(ScrollAmount + ScrollChange > 0)
//                    {
//                        ScrollAmount = 0.f;
//                    }
                }
            }

            if(Cursor.Right.Released && PPCollision(Cursor.Position, WindowSprite))
            {
                ScrollAmount += ScrollChange;

                if(ScrollAmount > 0)
                ScrollAmount = 0.f;

                ScrollChange = 0.f;
            }

//            if(ScrollAmount > 0)
//            ScrollAmount = 0.f;

            WindowSprite.SetPosition(ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
            WindowSprite.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
            NameText.SetText("FileBrowser: " + Path);
            NameText.SetSize(15);
            NameText.SetColor(Color::White);
            NameText.SetPosition(ViewPosition.x + (Position.x + 10) / App.GetZoom().x, ViewPosition.y + Position.y / App.GetZoom().y);
            NameText.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

            App().Draw(WindowSprite);
            App().Draw(NameText);

            for(int i = 0; i < Items.GetSize(); i++)
            {
                if(Items[i]->Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y), WindowSprite, ScrollAmount + ScrollChange))
                {
                    if(Items[i]->IsDirectory())
                    {
                        OpenDirectory(Path + Items[i]->GetName() + "/");
                        ScrollAmount = 0.f;
                        break;
                    }
                    else
                    {
                        SelectedFileName = Path + Items[i]->GetName();
                        Active = false;
                        ScrollAmount = 0.f;
                        return true;
                    }
                }
            }

            return false;
        }

        return false;
    }
};

class AddButtonsWindow : public WindowBase
{
    protected:

    public:
    List< AddButton > Buttons;

    AddButtonsWindow(Image & WindowImage, Image ABImages[3])
    {
        WindowSprite.SetImage(WindowImage);

        Buttons.AddNode(AddButton(ABImages, "Add Image",        Vector2f( 70,  50)));
        Buttons.AddNode(AddButton(ABImages, "Add Animation",    Vector2f(180,  50), 14));
        Buttons.AddNode(AddButton(ABImages, "Add Solid",        Vector2f( 70,  90)));
        Buttons.AddNode(AddButton(ABImages, "Add Artifact",     Vector2f(180,  90)));
        Buttons.AddNode(AddButton(ABImages, "Add MusicTrack",   Vector2f( 70, 130), 13));
        Buttons.AddNode(AddButton(ABImages, "Add EdgeBox",      Vector2f(180, 130), 15));
        Buttons.AddNode(AddButton(ABImages, "Add ActionBox",    Vector2f( 70, 170), 14));
        Buttons.AddNode(AddButton(ABImages, "Add SpawnPoint",   Vector2f(180, 170)));
        Buttons.AddNode(AddButton(ABImages, "Add NPC",          Vector2f( 70, 210)));
        Buttons.AddNode(AddButton(ABImages, "Add AI",           Vector2f(180, 210)));

        NameText.SetText("   AddWindow");
        NameText.SetSize(15);
    }

    void Update(Application & App, int Mode)
    {
        switch(Mode)
        {
            case EDITOR:
                NameText.SetText("   AddWindow");
            break;

            case ADDIMAGE:
                NameText.SetText("   AddWindow: AddImage");
            break;

            case ADDANIMATION:
                NameText.SetText("   AddWindow: AddAnimation");
            break;

            case ADDSOLID:
                NameText.SetText("   AddWindow: AddSolid");
            break;

            case ADDARTIFACT:
                NameText.SetText("   AddWindow: AddArtifact");
            break;

            case ADDMUSICTRACK:
                NameText.SetText("   AddWindow: AddMusicTrack");
            break;

            case ADDEDGEBOX:
                NameText.SetText("   AddWindow: AddEdgeBox");
            break;

            case ADDACTIONBOX:
                NameText.SetText("   AddWindow: AddActionBox");
            break;

            case ADDSPAWNPOINT:
                NameText.SetText("   AddWindow: AddSpawnPoint");
            break;

            case ADDNPC:
                NameText.SetText("   AddWindow: AddNPC");
            break;

            case ADDAI:
                NameText.SetText("   AddWindow: AddAI");
            break;

            default:
                NameText.SetText("   AddWindow");
            break;
        }

        UpdateWindow(App);

        for(int i = 0; i < Buttons.GetSize(); i++)
        {
            Buttons[i]->Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
        }
    }
};

class ObjectSlot
{
    protected:
    string Name;
    String ObName;
    Vector2f Position;
    Sprite SlotSprite;
    int ID;

    public:

    ObjectSlot()
    {}

    ObjectSlot(string name, int id, Vector2f pos, Image & slotImage)
    {
        Name = name;
        ID = id;
        ObName.SetText(Name);
        Position = pos;
        SlotSprite.SetImage(slotImage);

        ObName.SetFont(FONT::CalibriB);
        ObName.SetSize(16);
        ObName.SetColor(Color(50, 50, 50));
    }

    void Draw(Application & App)
    {
        SlotSprite.SetPosition(UpperLeft(App().GetDefaultView()) + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
        SlotSprite.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

        ObName.SetPosition(UpperLeft(App().GetDefaultView()) + Vector2f((Position.x + 100) / App.GetZoom().x, (Position.y + -1) / App.GetZoom().y));
        ObName.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
        ObName.SetCenter(GetCenter(ObName).x * App.GetZoom().x, 0);

        App().Draw(SlotSprite);
        App().Draw(ObName);
    }

    bool CursorCollision()
    {
        return PPCollision(Cursor.Position, SlotSprite);
    }

    friend class ObjectBarSegment;
};

class ObjectBarSegment
{
    protected:
    Image HeadImage[3];
    Sprite HeadSprite[3];
    int PrevAmount;
    String Amount;
    Image * SlotImage;
    List< ObjectSlot > Slots;
    Vector2f Position;
    int ID;
    short State;
    string CallResponse;
    bool HaveResponse;

    enum {PASSIVE, SELECTED, SELECTION};

    public:

    ObjectBarSegment()
    {}

    ObjectBarSegment(int id, Image & si)
    {
        SlotImage = &si;
        State = PASSIVE;
        PrevAmount = 0;
        HaveResponse = false;

        ID = id;

        switch(ID)
        {
            case IMAGE:
                HeadSprite[0] = LoadSprite(HeadImage[0], "Data/Image/EditorInterface/PullUpMenus/ImagesHeadIdle.png");
                HeadSprite[1] = LoadSprite(HeadImage[1], "Data/Image/EditorInterface/PullUpMenus/ImagesHeadSelected.png");
                HeadSprite[2] = LoadSprite(HeadImage[2], "Data/Image/EditorInterface/PullUpMenus/ImagesHeadSelection.png");
            break;

            case ANIMATION:
                HeadSprite[0] = LoadSprite(HeadImage[0], "Data/Image/EditorInterface/PullUpMenus/AnimationsHeadIdle.png");
                HeadSprite[1] = LoadSprite(HeadImage[1], "Data/Image/EditorInterface/PullUpMenus/AnimationsHeadSelected.png");
                HeadSprite[2] = LoadSprite(HeadImage[2], "Data/Image/EditorInterface/PullUpMenus/AnimationsHeadSelection.png");
            break;

            case SOLID:
                HeadSprite[0] = LoadSprite(HeadImage[0], "Data/Image/EditorInterface/PullUpMenus/SolidsHeadIdle.png");
                HeadSprite[1] = LoadSprite(HeadImage[1], "Data/Image/EditorInterface/PullUpMenus/SolidsHeadSelected.png");
                HeadSprite[2] = LoadSprite(HeadImage[2], "Data/Image/EditorInterface/PullUpMenus/SolidsHeadSelection.png");
            break;

            case ARTIFACT:
                HeadSprite[0] = LoadSprite(HeadImage[0], "Data/Image/EditorInterface/PullUpMenus/ArtifactsHeadIdle.png");
                HeadSprite[1] = LoadSprite(HeadImage[1], "Data/Image/EditorInterface/PullUpMenus/ArtifactsHeadSelected.png");
                HeadSprite[2] = LoadSprite(HeadImage[2], "Data/Image/EditorInterface/PullUpMenus/ArtifactsHeadSelection.png");
            break;

            default:
            break;
        }

        Position = Vector2f((ID-1)*200, 880);
        Amount.SetFont(FONT::CalibriB);
        Amount.SetSize(16);
        Amount.SetColor(Color(50, 50, 50));
    }

    void MakeCall()
    {
        State = SELECTION;
    }

    short UpdateCall()
    {
        if(HaveResponse)
        return 1;
        else if(State == SELECTION)
        return 0;
        else if(State == PASSIVE)
        return -1;

        return -5;
    }

    string GetResponse()
    {
        string tmpResponse = CallResponse;
        State = PASSIVE;
        CallResponse = "";
        HaveResponse = false;

        return tmpResponse;
    }

    void Draw(Application & App, Level * Lvl)
    {
        switch(ID)
        {
            case IMAGE:
            Amount.SetText(ToString(Lvl->Images.GetSize()));

            if(PrevAmount != Lvl->Images.GetSize())
            UpdateList(Lvl);
            break;

            case ANIMATION:
            Amount.SetText(ToString(Lvl->Animations.GetSize()));

            if(PrevAmount != Lvl->Animations.GetSize())
            UpdateList(Lvl);
            break;

            case SOLID:
            Amount.SetText(ToString(Lvl->Solids.GetSize()));

            if(PrevAmount != Lvl->Solids.GetSize())
            UpdateList(Lvl);
            break;

            case ARTIFACT:
            Amount.SetText(ToString(Lvl->Artifacts.GetSize()));

            if(PrevAmount != Lvl->Artifacts.GetSize())
            UpdateList(Lvl);
            break;
        }

        HeadSprite[State].SetPosition(UpperLeft(App().GetDefaultView()) + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
        HeadSprite[State].SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

        Amount.SetPosition(UpperLeft(App().GetDefaultView()) + Vector2f((Position.x + 165) / App.GetZoom().x, (Position.y + -1) / App.GetZoom().y));
        Amount.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));
        Amount.SetCenter(GetCenter(Amount).x * App.GetZoom().x, 0);

        App().Draw(HeadSprite[State]);
        App().Draw(Amount);

        if(State)
        for(int i = 0; i < Slots.GetSize(); i++)
        Slots[i]->Draw(App);
    }

    int Update()
    {
        int nSel = -5;

        switch(State)
        {
            case PASSIVE:
                if(Cursor.Left.Pressed && !GUI::SpaceKey() && CursorCollision())
                State = SELECTED;
            break;

            case SELECTED:
            {
                bool contact = CursorCollision();

                for(int i = 0; i < Slots.GetSize(); i++)
                {
                    if(Slots[i]->CursorCollision())
                    {
                        if(Cursor.Left.Pressed)
                        nSel = i;

                        contact = true;
                        break;
                    }
                }

                if(!contact)
                State = PASSIVE;
            }
            break;

            case SELECTION:
            {
                if(Cursor.Left.Pressed)
                {
                    bool contact = false;

                    for(int i = 0; i < Slots.GetSize(); i++)
                    {
                        if(Slots[i]->CursorCollision())
                        {

                            CallResponse = Slots[i]->Name;
                            HaveResponse = true;

                            contact = true;
                            break;
                        }
                    }

                    if(!contact)
                    State = PASSIVE;
                }
            }
            break;
        }

        return nSel;
    }

    void UpdateList(Level * Lvl)
    {
        Slots.Clear();

        switch(ID)
        {
            case IMAGE:
            for(int i = 0; i < Lvl->Images.GetSize(); i++)
            Slots.AddNode(new ObjectSlot(Lvl->Images[i]->Name, i, Vector2f((ID-1)*200, 860 - i*20), *SlotImage));
            break;

            case ANIMATION:
            for(int i = 0; i < Lvl->Animations.GetSize(); i++)
            Slots.AddNode(new ObjectSlot(Lvl->Animations[i]->Name, i, Vector2f((ID-1)*200, 860 - i*20), *SlotImage));
            break;

            case SOLID:
            for(int i = 0; i < Lvl->Solids.GetSize(); i++)
            Slots.AddNode(new ObjectSlot(Lvl->Solids[i]->Name, i, Vector2f((ID-1)*200, 860 - i*20), *SlotImage));
            break;

            case ARTIFACT:
            for(int i = 0; i < Lvl->Artifacts.GetSize(); i++)
            Slots.AddNode(new ObjectSlot(Lvl->Artifacts[i]->Name, i, Vector2f((ID-1)*200, 860 - i*20), *SlotImage));
            break;
        }
    }

    bool CursorCollision()
    {
        return PPCollision(Cursor.Position, HeadSprite[State]);
    }

    bool SlotCursorCollision()
    {
        bool contact = false;

        for(int i = 0; i < Slots.GetSize(); i++)
        {
            if(Slots[i]->CursorCollision())
            {
                contact = true;
                break;
            }
        }

        return contact;
    }
};

class ObjectBar
{
    protected:
    List< ObjectBarSegment > Segments;
    Image SlotImage;

    public:
    int nSelected;
    int modeSelected;

    ObjectBar()
    {
        Segments.AddNode(new ObjectBarSegment(IMAGE, SlotImage));
        Segments.AddNode(new ObjectBarSegment(ANIMATION, SlotImage));
        Segments.AddNode(new ObjectBarSegment(SOLID, SlotImage));
        Segments.AddNode(new ObjectBarSegment(ARTIFACT, SlotImage));

        SlotImage.LoadFromFile("Data/Image/EditorInterface/PullUpMenus/ObjectSlotIdle.png");
    }

    bool CursorCollision()
    {
        bool flag = false;

        for(int i = 0; i < Segments.GetSize(); i++)
        {
            if(Segments[i]->CursorCollision())
            flag = true;
        }

        return flag;
    }

    bool SlotCursorCollision()
    {
        bool flag = false;

        for(int i = 0; i < Segments.GetSize(); i++)
        {
            if(Segments[i]->SlotCursorCollision())
            flag = true;
        }

        return flag;
    }

    bool Update(Application & App, Level * Lvl)
    {
        bool flag = false;
        int n;

        for(int i = 0; i < Segments.GetSize(); i++)
        {
            n = Segments[i]->Update();
            if(n != -5)
            {
                nSelected = n;
                modeSelected = i+1;
                flag = true;
            }

            Segments[i]->Draw(App, Lvl);
        }

        return flag;
    }

    ObjectBarSegment & GetImageSegment()
    {
        return *Segments[0];
    }

    ObjectBarSegment & GetSolidSegment()
    {
        return *Segments[2];
    }

    ObjectBarSegment & GetArtifactSegment()
    {
        return *Segments[3];
    }
};

class AddImageWindow : public WindowBase
{
    protected:

    public:
    List< GUIString > Text;
    List< GUI::TextBox > TextBoxes;
    AddButton FBButton, OkButton, CancelButton;
    bool Active;

    AddImageWindow(Image & WindowImage, Image ABImages[3], Image FBBImages[3])
    {
        Active = false;
        WindowSprite.SetImage(WindowImage);

        NameText.SetText("   AddImage");
        NameText.SetSize(15);

        Position = Vector2f(1400/2-250, 900/2-175);

        Text.AddNode(GUIString("Name", Vector2f(20, 50), 16)); // ObjectNameString : gsid = 0
        Text.AddNode(GUIString("FileName", Vector2f(260, 50), 16)); // ImageNameString : gsid = 1
        TextBoxes.AddNode(GUI::TextBox(20, 70, 220, 16)); // ObjectNameTextBox : tbid = 0
        TextBoxes.AddNode(GUI::TextBox(260, 70, 220, 16)); // ImageNameTextBox : tbid = 1
        TextBoxes[0]->SetActive(true); // Activate ObjectNameTextBox

        TextBoxes[0]->SetString("i");

        FBButton = AddButton(FBBImages, "", Vector2f(340, 59));
        OkButton = AddButton(ABImages, "Ok", Vector2f(310, 220));
        CancelButton = AddButton(ABImages, "Cancel", Vector2f(430, 220));
    }

    bool GetActive()
    {
        return Active;
    }

    void Open()
    {
        Active = true;
    }

    bool Update(Application & App, Level * Lvl, FileBrowser & FB)
    {
        UpdateWindow(App);

        /// Take code from main and insert here.

        if(Cursor.Left.Pressed)
        {
            TextBoxes[0]->Clicked(App); // Press ObjectNameTextBox
            TextBoxes[1]->Clicked(App); // Press ImageNameTextBox
        }

        TextBoxes[0]->Write(); // Write ObjectNameTextBox
        TextBoxes[1]->Write(); // Write ImageNameTextBox

        TextBoxes[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ObjectNameTextBox
        TextBoxes[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ImageNameTextBox
        Text[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ObjectNameString
        Text[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ImageNameString

        if(FBButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            FB.Activate(false);
        }

        if(OkButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            Lvl->AddImage(TextBoxes[0]->GetString(), TextBoxes[1]->GetString());

            TextBoxes[0]->SetString("i");
            TextBoxes[1]->SetString("");

            Close();
            return true;
        }

        if(CancelButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            Close();
            return true;
        }

        if(FB.Update(App))
        {
            TextBoxes[1]->SetString(FB.GetPath());
        }

        return false;
    }

    void Close()
    {
        TextBoxes[0]->SetActive(true); // Activate ObjectNameTextBox
        Active = false;
    }
};

class AddSolidWindow : public WindowBase
{
    protected:

    public:
    List< GUIString > Text;
    List< GUI::TextBox > TextBoxes;
    List< GUI::FloatBox > FloatBoxes;
    AddButton OBButton, OkButton, CancelButton;
    bool Active;
    bool GrabbingImage;

    AddSolidWindow(Image & WindowImage, Image ABImages[3], Image OBBImages[3])
    {
        Active = false;
        GrabbingImage = false;
        WindowSprite.SetImage(WindowImage);

        NameText.SetText("   AddSolid");
        NameText.SetSize(15);

        Position = Vector2f(1400/2-250, 900/2-175);

        Text.AddNode(GUIString("Name", Vector2f(20, 20), 16)); // ObjectNameString : gsid = 0
        Text.AddNode(GUIString("Image", Vector2f(260, 20), 16)); // ImageNameString : gsid = 1
        Text.AddNode(GUIString("xPosition", Vector2f(20, 60), 16)); // xPositionString : gsid = 2
        Text.AddNode(GUIString("yPosition", Vector2f(260, 60), 16)); // yPositionString : gsid = 3
        TextBoxes.AddNode(GUI::TextBox(20, 40, 220, 16)); // ObjectNameTextBox : tbid = 0
        TextBoxes.AddNode(GUI::TextBox(260, 40, 220, 16)); // ImageNameTextBox : tbid = 1
        FloatBoxes.AddNode(GUI::FloatBox(20, 80, 220, 16)); // xPositionFloatBox : fbid = 0
        FloatBoxes.AddNode(GUI::FloatBox(260, 80, 220, 16)); // yPositionFloatBox : fbid = 1
        TextBoxes[0]->SetActive(true); // Activate ObjectNameTextBox

        TextBoxes[1]->SetString("i");

        FloatBoxes[0]->SetValue(0);
        FloatBoxes[1]->SetValue(0);

        OBButton = AddButton(OBBImages, "", Vector2f(315, 29));
        OkButton = AddButton(ABImages, "Ok", Vector2f(310, 220));
        CancelButton = AddButton(ABImages, "Cancel", Vector2f(430, 220));
    }

    bool GetActive()
    {
        return Active;
    }

    void Open()
    {
        Active = true;
    }

    bool Update(Application & App, Level * Lvl, ObjectBar & OB)
    {
        UpdateWindow(App);

        /// Take code from main and insert here.

        if(Cursor.Left.Pressed)
        {
            TextBoxes[0]->Clicked(App); // Press ObjectNameTextBox
            TextBoxes[1]->Clicked(App); // Press ImageNameTextBox
            FloatBoxes[0]->Clicked(App); // Press xPositionFloatBox
            FloatBoxes[1]->Clicked(App); // Press yPositionFloatBox
        }

        TextBoxes[0]->Write(); // Write ObjectNameTextBox
        TextBoxes[1]->Write(); // Write ImageNameTextBox
        FloatBoxes[0]->Write(); // Write xPositionFloatBox
        FloatBoxes[1]->Write(); // Write yPositionFloatBox

        TextBoxes[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ObjectNameTextBox
        TextBoxes[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ImageNameTextBox
        FloatBoxes[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw xPositionFloatBox
        FloatBoxes[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw yPositionFloatBox
        Text[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ObjectNameString
        Text[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ImageNameString
        Text[2]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw xPositionString
        Text[3]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw yPositionString

        if(OBButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            OB.GetImageSegment().MakeCall();
            GrabbingImage = true;
        }

        if(GrabbingImage)
        {
            switch(OB.GetImageSegment().UpdateCall())
            {
                case 0:
                break;

                case 1:
                    TextBoxes[1]->SetString(OB.GetImageSegment().GetResponse());
                    GrabbingImage = false;
                break;

                case -1:
                    GrabbingImage = false;
                break;

                default:
                break;
            }
        }

        if(OkButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            Lvl->AddSolid(TextBoxes[0]->GetString(), TextBoxes[1]->GetString(), Vector2f(FloatBoxes[0]->GetValue(), FloatBoxes[1]->GetValue()));

            TextBoxes[0]->SetString("");
            TextBoxes[1]->SetString("i");
            FloatBoxes[0]->SetValue(0);
            FloatBoxes[1]->SetValue(0);

            Close();
            return true;
        }

        if(CancelButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            Close();
            return true;
        }

        return false;
    }

    void Close()
    {
        TextBoxes[0]->SetActive(true); // Activate ObjectNameTextBox
        Active = false;
    }
};

class AddArtifactWindow : public WindowBase
{
    protected:

    public:
    List< GUIString > Text;
    List< GUI::TextBox > TextBoxes;
    List< GUI::FloatBox > FloatBoxes;
    AddButton OBButton, OkButton, CancelButton;
    bool Active;
    bool GrabbingImage;

    AddArtifactWindow(Image & WindowImage, Image ABImages[3], Image OBBImages[3])
    {
        Active = false;
        GrabbingImage = false;
        WindowSprite.SetImage(WindowImage);

        NameText.SetText("   AddArtifact");
        NameText.SetSize(15);

        Position = Vector2f(1400/2-250, 900/2-175);

        Text.AddNode(GUIString("Name", Vector2f(20, 20), 16)); // ObjectNameString : gsid = 0
        Text.AddNode(GUIString("Image", Vector2f(260, 20), 16)); // ImageNameString : gsid = 1
        Text.AddNode(GUIString("xPosition", Vector2f(20, 60), 16)); // xPositionString : gsid = 2
        Text.AddNode(GUIString("yPosition", Vector2f(260, 60), 16)); // yPositionString : gsid = 3
        Text.AddNode(GUIString("Rotation", Vector2f(20, 100), 16)); // RotationString : gsid = 4
        Text.AddNode(GUIString("Depth", Vector2f(260, 100), 16)); // DepthString : gsid = 5
        TextBoxes.AddNode(GUI::TextBox(20, 40, 220, 16)); // ObjectNameTextBox : tbid = 0
        TextBoxes.AddNode(GUI::TextBox(260, 40, 220, 16)); // ImageNameTextBox : tbid = 1
        FloatBoxes.AddNode(GUI::FloatBox(20, 80, 220, 16)); // xPositionFloatBox : fbid = 0
        FloatBoxes.AddNode(GUI::FloatBox(260, 80, 220, 16)); // yPositionFloatBox : fbid = 1
        FloatBoxes.AddNode(GUI::FloatBox(20, 120, 220, 16)); // RotationFloatBox : fbid = 2
        FloatBoxes.AddNode(GUI::FloatBox(260, 120, 220, 16)); // DepthFloatBox : fbid = 3
        TextBoxes[0]->SetActive(true); // Activate ObjectNameTextBox

        FloatBoxes[0]->SetValue(0);
        FloatBoxes[1]->SetValue(0);
        FloatBoxes[2]->SetValue(0);
        FloatBoxes[3]->SetValue(1);

        OBButton = AddButton(OBBImages, "", Vector2f(315, 29));
        OkButton = AddButton(ABImages, "Ok", Vector2f(310, 220));
        CancelButton = AddButton(ABImages, "Cancel", Vector2f(430, 220));
    }

    bool GetActive()
    {
        return Active;
    }

    void Open()
    {
        Active = true;
    }

    bool Update(Application & App, Level * Lvl, ObjectBar & OB)
    {
        UpdateWindow(App);

        /// Take code from main and insert here.

        if(Cursor.Left.Pressed)
        {
            TextBoxes[0]->Clicked(App); // Press ObjectNameTextBox
            TextBoxes[1]->Clicked(App); // Press ImageNameTextBox
            FloatBoxes[0]->Clicked(App); // Press xPositionFloatBox
            FloatBoxes[1]->Clicked(App); // Press yPositionFloatBox
            FloatBoxes[2]->Clicked(App); // Press RotationFloatBox
            FloatBoxes[3]->Clicked(App); // Press DepthFloatBox
        }

        TextBoxes[0]->Write(); // Write ObjectNameTextBox
        TextBoxes[1]->Write(); // Write ImageNameTextBox
        FloatBoxes[0]->Write(); // Write xPositionFloatBox
        FloatBoxes[1]->Write(); // Write yPositionFloatBox
        FloatBoxes[2]->Write(); // Write RotationFloatBox
        FloatBoxes[3]->Write(); // Write DepthFloatBox

        TextBoxes[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ObjectNameTextBox
        TextBoxes[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ImageNameTextBox
        FloatBoxes[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw xPositionFloatBox
        FloatBoxes[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw yPositionFloatBox
        FloatBoxes[2]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw RotationFloatBox
        FloatBoxes[3]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw DepthFloatBox
        Text[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ObjectNameString
        Text[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw ImageNameString
        Text[2]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw xPositionString
        Text[3]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw yPositionString
        Text[4]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw RotationString
        Text[5]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)); // Draw DepthString

        if(OBButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            OB.GetImageSegment().MakeCall();
            GrabbingImage = true;
        }

        if(GrabbingImage)
        {
            switch(OB.GetImageSegment().UpdateCall())
            {
                case 0:
                break;

                case 1:
                    TextBoxes[1]->SetString(OB.GetImageSegment().GetResponse());
                    GrabbingImage = false;
                break;

                case -1:
                    GrabbingImage = false;
                break;

                default:
                break;
            }
        }

        if(OkButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            Lvl->AddArtifact(TextBoxes[0]->GetString(), "Image", TextBoxes[1]->GetString(), Vector2f(FloatBoxes[0]->GetValue(), FloatBoxes[1]->GetValue()), FloatBoxes[2]->GetValue(), FloatBoxes[3]->GetValue());

            TextBoxes[0]->SetString("");
            TextBoxes[1]->SetString("i");
            FloatBoxes[0]->SetValue(0);
            FloatBoxes[1]->SetValue(0);
            FloatBoxes[2]->SetValue(0);
            FloatBoxes[3]->SetValue(1);

            Close();
            return true;
        }

        if(CancelButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
        {
            Close();
            return true;
        }

        return false;
    }

    void Close()
    {
        TextBoxes[0]->SetActive(true); // Activate ObjectNameTextBox
        Active = false;
    }
};

class PropertiesWindow : public WindowBase
{
    protected:
    int * Mode;
    int prevMode;
    int prevSel;
    string Selected;

    List< AddButton > WorldButtons;
    List< GUI::TextBox > WorldTBox;

    AddButton FBButton, OBButton;

    List< GUIString > ImageText;
    List< GUI::TextBox > ImageBox;

    List< GUIString > SolidText;
    List< GUI::TextBox > SolidTBox;
    List< GUI::FloatBox > SolidFBox;
    bool sGrabbingImage;

    List< GUIString > ArtifactText;
    List< GUI::TextBox > ArtifactTBox;
    List< GUI::FloatBox > ArtifactFBox;
    bool aGrabbingImage;
    float RotOrigin;
    float artRotOrigin;

    ImagePreview Preview;

    public:

    PropertiesWindow(int & mode, Image & WindowImage, Image ABImages[3], Image FBBImages[3], Image OBBImages[3], SettingsHandler & SH)
    {
        WindowSprite.SetImage(WindowImage);

        /// World
        WorldButtons.AddNode(AddButton(ABImages, "Save", Vector2f(298, 200)));
        WorldButtons.AddNode(AddButton(ABImages, "Load", Vector2f(422, 200)));
        WorldButtons.AddNode(AddButton(ABImages, "LoadImageDir", Vector2f(58, 65), 14));
        WorldTBox.AddNode(GUI::TextBox(250, 165, 220, 16));
        WorldTBox[0]->SetString(SH.LvlFileName);
        WorldTBox.AddNode(GUI::TextBox(10, 30, 220, 16));
        WorldTBox[1]->SetString("Data/Image/");

        NameText.SetText("   PropertiesWindow: World");
        NameText.SetSize(15);

        Preview = ImagePreview(200, Vector2f(20, 33));

        Mode = &mode;
        Selected = "Settings";

        FBButton = AddButton(FBBImages, "", Vector2f(330, 74));
        OBButton = AddButton(OBBImages, "", Vector2f(305, 74));


        /// Image
        ImageText.AddNode(GUIString("Name",     Vector2f(250, 25), 16));
        ImageText.AddNode(GUIString("FileName", Vector2f(250, 65), 16));

        ImageBox.AddNode(GUI::TextBox(250, 45, 220, 16));
        ImageBox.AddNode(GUI::TextBox(250, 85, 220, 16));

        /// Solid
        SolidText.AddNode(GUIString("Name",     Vector2f(250, 25), 16));
        SolidText.AddNode(GUIString("Image",    Vector2f(250, 65), 16));
        SolidText.AddNode(GUIString("xPosition",Vector2f(250, 105), 16));
        SolidText.AddNode(GUIString("yPosition",Vector2f(370, 105), 16));

        SolidTBox.AddNode(GUI::TextBox(250, 45, 220, 16));
        SolidTBox.AddNode(GUI::TextBox(250, 85, 220, 16));

        SolidFBox.AddNode(GUI::FloatBox(250, 125, 100, 16));
        SolidFBox.AddNode(GUI::FloatBox(370, 125, 100, 16));

        sGrabbingImage = false;

        /// Artifact
        ArtifactText.AddNode(GUIString("Name",     Vector2f(250, 25), 16));
        ArtifactText.AddNode(GUIString("Image",    Vector2f(250, 65), 16));
        ArtifactText.AddNode(GUIString("xPosition",Vector2f(250, 105), 16));
        ArtifactText.AddNode(GUIString("yPosition",Vector2f(370, 105), 16));
        ArtifactText.AddNode(GUIString("Rotation", Vector2f(250, 145), 16));
        ArtifactText.AddNode(GUIString("Depth",    Vector2f(370, 145), 16));

        ArtifactTBox.AddNode(GUI::TextBox(250, 45, 220, 16));
        ArtifactTBox.AddNode(GUI::TextBox(250, 85, 220, 16));

        ArtifactFBox.AddNode(GUI::FloatBox(250, 125, 100, 16));
        ArtifactFBox.AddNode(GUI::FloatBox(370, 125, 100, 16));
        ArtifactFBox.AddNode(GUI::FloatBox(250, 165, 100, 16));
        ArtifactFBox.AddNode(GUI::FloatBox(370, 165, 100, 16));

        aGrabbingImage = false;
        RotOrigin = 0.f;
        artRotOrigin = 0.f;
    }

    void Update(int nSel, Application & App, Level * Lvl, FileBrowser & FB, ObjectBar & OB)
    {
        UpdateWindow(App);

        switch(*Mode)
        {
            case WORLD:
            {
                if(prevMode != *Mode || prevSel != nSel)
                {
                    NameText.SetText("   PropertiesWindow: World - Settings");
                }

                if(Cursor.Left.Pressed)
                {
                    WorldTBox[0]->Clicked(App);
                    WorldTBox[1]->Clicked(App);
                }

                if(WorldButtons[0]->Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
                {
                    Lvl->Save(WorldTBox[0]->GetString(), App);
                }

                if(WorldButtons[1]->Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
                {
                    Lvl->ClearObjectLists();
                    Lvl->Load(WorldTBox[0]->GetString(), App);
                }

                if(WorldButtons[2]->Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
                {
                    Lvl->LoadImageDirectory(WorldTBox[1]->GetString());
                }

                WorldTBox[0]->Write();
                WorldTBox[1]->Write();

                WorldTBox[0]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
                WorldTBox[1]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
            }
            break;

            case IMAGE: // Finish This Tomorrow
            {
                if(prevMode != *Mode || prevSel != nSel)
                {
                    Selected = Lvl->Images[nSel]->Name;
                    NameText.SetText("   PropertiesWindow: Image - \"" + Selected + "\"");
                    Preview.SetImage(Lvl->Images[nSel]->Content);
                    ImageBox[0]->SetString(Lvl->Images[nSel]->Name);
                    ImageBox[1]->SetString(Lvl->Images[nSel]->FileName);
                }

                if(Cursor.Left.Pressed)
                {
                    ImageBox[0]->Clicked(App);
                    ImageBox[1]->Clicked(App);
                }

                if(FBButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
                {
                    FB.Activate(false);
                }

                if(ImageBox[0]->Write()) // Change Name
                {
                    Lvl->Images[nSel]->Name = ImageBox[0]->GetString();
                    Selected = Lvl->Images[nSel]->Name;
                    NameText.SetText("   PropertiesWindow: Image - \"" + Selected + "\"");
                    OB.GetImageSegment().UpdateList(Lvl);
                }
                if(ImageBox[1]->Write()) // Change File
                {
                    Lvl->Images[nSel]->FileName = ImageBox[1]->GetString();
                    LoadImage(Lvl->Images[nSel]);
                    Preview.SetImage(Lvl->Images[nSel]->Content);
                    Lvl->UpdateSprites();
                }

                Preview.Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < ImageText.GetSize(); i++)
                ImageText[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < ImageBox.GetSize(); i++)
                ImageBox[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                if(FB.Update(App))
                {
                    ImageBox[1]->SetString(FB.GetPath());
                    Lvl->Images[nSel]->FileName = ImageBox[1]->GetString();
                    LoadImage(Lvl->Images[nSel]);
                    Preview.SetImage(Lvl->Images[nSel]->Content);
                    Lvl->UpdateSprites();
                }
            }
            break;

            case SOLID:
            {
                if(prevMode != *Mode || prevSel != nSel)
                {
                    Selected = Lvl->Solids[nSel]->Name;
                    NameText.SetText("   PropertiesWindow: Solid - \"" + Selected + "\"");
                    Preview.SetImage(Lvl->Solids[nSel]->Object.sprite.sprite.GetImage());
                    SolidTBox[0]->SetString(Lvl->Solids[nSel]->Name);
                    SolidTBox[1]->SetString(Lvl->Solids[nSel]->Object.ImageName);
                    SolidFBox[0]->SetValue(Lvl->Solids[nSel]->Object.Position.x);
                    SolidFBox[1]->SetValue(Lvl->Solids[nSel]->Object.Position.y);
                }

                if(!SolidFBox[0]->GetActive())
                SolidFBox[0]->SetValue(Lvl->Solids[nSel]->Object.Position.x);
                if(!SolidFBox[1]->GetActive())
                SolidFBox[1]->SetValue(Lvl->Solids[nSel]->Object.Position.y);

                if(Cursor.Left.Pressed)
                {
                    SolidTBox[0]->Clicked(App);
                    SolidTBox[1]->Clicked(App);
                    SolidFBox[0]->Clicked(App);
                    SolidFBox[1]->Clicked(App);
                }

                if(OBButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
                {
                    OB.GetImageSegment().MakeCall();
                    sGrabbingImage = true;
                }

                if(sGrabbingImage)
                {
                    switch(OB.GetImageSegment().UpdateCall())
                    {
                        case 0:
                        break;

                        case 1:
                            SolidTBox[1]->SetString(OB.GetImageSegment().GetResponse());
                            sGrabbingImage = false;
                            // Change Image
                        break;

                        case -1:
                            sGrabbingImage = false;
                        break;

                        default:
                        break;
                    }
                }

                if(SolidTBox[0]->Write()) // Change Name
                {
                    Lvl->Solids[nSel]->Name = SolidTBox[0]->GetString();
                    Selected = Lvl->Solids[nSel]->Name;
                    NameText.SetText("   PropertiesWindow: Solid - \"" + Selected + "\"");
                    OB.GetSolidSegment().UpdateList(Lvl);
                }
                if(SolidTBox[1]->Write())
                {
                    // Change Image
                }
                if(SolidFBox[0]->Write()) // Change xPosition
                {
                    Lvl->Solids[nSel]->Object.Position.x = SolidFBox[0]->GetValue();
                }
                if(SolidFBox[1]->Write()) // Change yPosition
                {
                    Lvl->Solids[nSel]->Object.Position.y = SolidFBox[1]->GetValue();
                }

                // incrementalMove
                if(GUI::UpKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.y += -100;
                }
                else if(GUI::UpKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.y += -10;
                }
                else if(GUI::UpKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.y += -1;
                }

                if(GUI::DownKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.y += 100;
                }
                else if(GUI::DownKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.y += 10;
                }
                else if(GUI::DownKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.y += 1;
                }

                if(GUI::LeftKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.x += -100;
                }
                else if(GUI::LeftKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.x += -10;
                }
                else if(GUI::LeftKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.x += -1;
                }

                if(GUI::RightKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.x += 100;
                }
                else if(GUI::RightKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.x += 10;
                }
                else if(GUI::RightKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Solids[nSel]->Object.Position.x += 1;
                }

                Preview.Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < SolidText.GetSize(); i++)
                SolidText[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < SolidTBox.GetSize(); i++)
                SolidTBox[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < SolidFBox.GetSize(); i++)
                SolidFBox[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
            }
            break;

            case ARTIFACT:
            {
                if(prevMode != *Mode || prevSel != nSel)
                {
                    Selected = Lvl->Artifacts[nSel]->Name;
                    NameText.SetText("   PropertiesWindow: Artifact - \"" + Selected + "\"");
                    Preview.SetImage(Lvl->Artifacts[nSel]->Object.sprite.sprite.GetImage());
                    ArtifactTBox[0]->SetString(Lvl->Artifacts[nSel]->Name);
                    ArtifactTBox[1]->SetString(Lvl->Artifacts[nSel]->Object.ImageName);
                    ArtifactFBox[0]->SetValue(Lvl->Artifacts[nSel]->Object.Position.x);
                    ArtifactFBox[1]->SetValue(Lvl->Artifacts[nSel]->Object.Position.y);
                    ArtifactFBox[2]->SetValue(Lvl->Artifacts[nSel]->Object.sprite.rotation);
                    ArtifactFBox[3]->SetValue(Lvl->Artifacts[nSel]->Object.sprite.depth);
                }

                if(!ArtifactFBox[0]->GetActive())
                ArtifactFBox[0]->SetValue(Lvl->Artifacts[nSel]->Object.Position.x);
                if(!ArtifactFBox[1]->GetActive())
                ArtifactFBox[1]->SetValue(Lvl->Artifacts[nSel]->Object.Position.y);
                if(!ArtifactFBox[2]->GetActive())
                ArtifactFBox[2]->SetValue(Lvl->Artifacts[nSel]->Object.sprite.rotation);

                if(Cursor.Left.Pressed)
                {
                    ArtifactTBox[0]->Clicked(App);
                    ArtifactTBox[1]->Clicked(App);
                    ArtifactFBox[0]->Clicked(App);
                    ArtifactFBox[1]->Clicked(App);
                    ArtifactFBox[2]->Clicked(App);
                    ArtifactFBox[3]->Clicked(App);
                }

                if(OBButton.Update(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y)))
                {
                    OB.GetImageSegment().MakeCall();
                    aGrabbingImage = true;
                }

                if(aGrabbingImage)
                {
                    switch(OB.GetImageSegment().UpdateCall())
                    {
                        case 0:
                        break;

                        case 1:
                            ArtifactTBox[1]->SetString(OB.GetImageSegment().GetResponse());
                            aGrabbingImage = false;
                            // Change Image
                        break;

                        case -1:
                            aGrabbingImage = false;
                        break;

                        default:
                        break;
                    }
                }

                if(ArtifactTBox[0]->Write())
                {
                    Lvl->Artifacts[nSel]->Name = ArtifactTBox[0]->GetString();
                    Selected = Lvl->Artifacts[nSel]->Name;
                    NameText.SetText("   PropertiesWindow: Artifact - \"" + Selected + "\"");
                    OB.GetArtifactSegment().UpdateList(Lvl);
                }
                if(ArtifactTBox[1]->Write())
                {
                    // Change Image
                }
                if(ArtifactFBox[0]->Write()) // Change xPosition
                {
                    Lvl->Artifacts[nSel]->Object.Position.x = ArtifactFBox[0]->GetValue();
                }
                if(ArtifactFBox[1]->Write()) // Change yPosition
                {
                    Lvl->Artifacts[nSel]->Object.Position.y = ArtifactFBox[1]->GetValue();
                }
                if(ArtifactFBox[2]->Write()) // Change Rotation
                {
                    Lvl->Artifacts[nSel]->Object.sprite.rotation = ArtifactFBox[2]->GetValue();
                }
                if(ArtifactFBox[3]->Write()) // Change Depth
                {
                    Lvl->Artifacts[nSel]->Object.sprite.depth = ArtifactFBox[3]->GetValue();
                    Lvl->UpdateDepth();
                }

                // incrementalMove
                if(GUI::UpKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.y += -100;
                }
                else if(GUI::UpKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.y += -10;
                }
                else if(GUI::UpKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.y += -1;
                }

                if(GUI::DownKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.y += 100;
                }
                else if(GUI::DownKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.y += 10;
                }
                else if(GUI::DownKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.y += 1;
                }

                if(GUI::LeftKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.x += -100;
                }
                else if(GUI::LeftKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.x += -10;
                }
                else if(GUI::LeftKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.x += -1;
                }

                if(GUI::RightKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && (GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.x += 100;
                }
                else if(GUI::RightKey.Pressed && (GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.x += 10;
                }
                else if(GUI::RightKey.Pressed && !(GUI::LShiftKey() || GUI::RShiftKey()) && !(GUI::LCtrlKey() || GUI::RCtrlKey()))
                {
                    Lvl->Artifacts[nSel]->Object.Position.x += 1;
                }

                if(GUI::RKey.Pressed)
                {
                    RotOrigin = Angle(Lvl->Artifacts[nSel]->Object.Position, Cursor.Position)*DPR;
                    artRotOrigin = Lvl->Artifacts[nSel]->Object.sprite.rotation;
                }

                if(GUI::RKey())
                {
                    float newRot = artRotOrigin - Angle(Lvl->Artifacts[nSel]->Object.Position, Cursor.Position)*DPR + RotOrigin;
                    while(newRot < 0)
                    newRot += 360;
                    while(newRot >= 360)
                    newRot -= 360;
                    Lvl->Artifacts[nSel]->Object.sprite.rotation = newRot;
                    App().Draw(Shape::Line(Lvl->Artifacts[nSel]->Object.Position, Cursor.Position, 3, Color(200, 255, 220)));
                }

                Preview.Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < ArtifactText.GetSize(); i++)
                ArtifactText[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < ArtifactTBox.GetSize(); i++)
                ArtifactTBox[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));

                for(int i = 0; i < ArtifactFBox.GetSize(); i++)
                ArtifactFBox[i]->Draw(App, ViewPosition + Vector2f(Position.x / App.GetZoom().x, Position.y / App.GetZoom().y));
            }
            break;

            default:
            {
                NameText.SetText("   PropertiesWindow: Unknown - \"" + Selected + "\"");
            }
            break;
        }

        prevMode = *Mode;
        prevSel = nSel;
    }

    string GetLvlFileName()
    {
        return WorldTBox[0]->GetString();
    }
};

int main()
{
    Application App(1400, 900, 32, "Level Editor");
    App().SetFramerateLimit(120);

    SettingsHandler SH;

    FONT::LoadFonts();

    Level * Lvl = new Level(true, NULL, NULL);

    int Mode = EDITOR;
    int EditorMode = ADDING;
    int PropertiesMode = WORLD;
    int Selected = -5;
    bool PickUp = false;
    Vector2f PickUpOffset;

    bool EdgeBoxCreation = false;
    Vector2f tmpEbPos;
    Image EbSymbolImage;
    Sprite EbSymbol = LoadSprite(EbSymbolImage, "Data/Image/EditorInterface/EdgeBoxSymbol.png");
    SetCenterInCenter(EbSymbol);
    Image SpSymbolImage;
    Sprite SpSymbol = LoadSprite(SpSymbolImage, "Data/Image/EditorInterface/SpawnPointSymbol.png");
    SetCenterInCenter(SpSymbol);
    bool ActionBoxCreation = false;
    Vector2f tmpAbPos;

    Vector2f MovePositions[2];

    Image AIobjImages[1];
    Sprite AIobjSymbols[1];
    AIobjSymbols[0] = LoadSprite(AIobjImages[0], "Data/Image/AIobj/CorpPeople.png");
    SetCenterInCenter(AIobjSymbols[0]);

    Image ABImages[3]; // NormalButton
    ABImages[0].LoadFromFile("Data/Image/EditorInterface/NewButtonIdle.png");
    ABImages[1].LoadFromFile("Data/Image/EditorInterface/NewButtonHoover.png");
    ABImages[2].LoadFromFile("Data/Image/EditorInterface/NewButtonPressed.png");

    Image FBBImages[3]; // FilebrowserButton
    FBBImages[0].LoadFromFile("Data/Image/EditorInterface/FBButtonIdle.png");
    FBBImages[1].LoadFromFile("Data/Image/EditorInterface/FBButtonHoover.png");
    FBBImages[2].LoadFromFile("Data/Image/EditorInterface/FBButtonPressed.png");

    Image OBBImages[3]; // ObjectbarButton
    OBBImages[0].LoadFromFile("Data/Image/EditorInterface/OBButtonIdle.png");
    OBBImages[1].LoadFromFile("Data/Image/EditorInterface/OBButtonHoover.png");
    OBBImages[2].LoadFromFile("Data/Image/EditorInterface/OBButtonPressed.png");

    Image WindowImages[5];
    WindowImages[0].LoadFromFile("Data/Image/EditorInterface/FinalWindow01.png"); // 250x170
    WindowImages[1].LoadFromFile("Data/Image/EditorInterface/FinalWindow02.png"); // 250x250
    WindowImages[2].LoadFromFile("Data/Image/EditorInterface/FinalWindow03.png"); // 500x320
    WindowImages[3].LoadFromFile("Data/Image/EditorInterface/FinalWindow04.png"); // 500x170
    WindowImages[4].LoadFromFile("Data/Image/EditorInterface/FinalWindow05.png"); // 500x250

    /// Windows
    AddButtonsWindow ABWindow(WindowImages[1], ABImages);
    ABWindow.SetPosition(Vector2f(1400 - 250, 900 - 499));

    // AddWindows
    AddImageWindow      aImageWindow    (WindowImages[4], ABImages, FBBImages);
    AddSolidWindow      aSolidWindow    (WindowImages[4], ABImages, OBBImages);
    AddArtifactWindow   aArtifactWindow (WindowImages[4], ABImages, OBBImages);

    PropertiesWindow PWindow(PropertiesMode, WindowImages[4], ABImages, FBBImages, OBBImages, SH);
    PWindow.SetPosition(Vector2f(1400 - 500, 900 - 250));

    FileBrowser FB(WindowImages[2]);
    ObjectBar OB;

    string CreateMessage;

    /// Animation
    GUIString AnimationNameText("Name", Vector2f(400, 740));
    GUIString AnimationValueText("FileName", Vector2f(750, 740));
    GUI::TextBox AnimationNameBox (400, 775, 200, 30);
    GUI::TextBox AnimationValueBox(750, 775, 200, 30);
    AnimationNameBox.SetActive(true);

    float Zoom = 1.f;
    Vector2f ViewPosition;

    bool IsPlaying = true;
    while(App().IsOpened())
    {
        Cursor.ScrollUp = false;
        Cursor.ScrollDown = false;

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
                    if(!FB.GetActive() || !FB.CursorCollision())
                    {
                        App().GetDefaultView().Zoom(1.2f);
                        Zoom *= 1.2f;
                    }

                    Cursor.ScrollUp = true;
                    Cursor.ScrollDown = false;
                }
                else if(e.MouseWheel.Delta < 0.f)
                {
                    if(!FB.GetActive() || !FB.CursorCollision())
                    {
                        App().GetDefaultView().Zoom(1.f/1.2f);
                        Zoom *= 1/1.2f;
                    }

                    Cursor.ScrollDown = true;
                    Cursor.ScrollUp = false;
                }
            }

            if(e.Type == Event::KeyPressed)
            switch(e.Key.Code)
            {
                case Key::Escape:

                    App().Close();
                    IsPlaying = false;

                break;

                /*case Key::S:

                    Lvl->Save("newlvl");

                break;

                case Key::L:

                    Lvl->Load("newlvl");

                break;*/

                default:
                break;
            }
        }

        if(IsPlaying)
        {
            //PrevCursorPosition = Cursor.Position;

            App.Update(Lvl->BgColor);
            GUI::UpdateKeys(App());

            if(GUI::SpaceKey() && Cursor.Left.Pressed)/*
            && !ABWindow.CursorCollision() && !PWindow.CursorCollision() && !OB.CursorCollision()
            && (!FB.GetActive() || !FB.CursorCollision()) && (!aImageWindow.GetActive() || !aImageWindow.CursorCollision())
            && (!aSolidWindow.GetActive() || !aSolidWindow.CursorCollision()) && (!aArtifactWindow.GetActive() || !aArtifactWindow.CursorCollision()))*/
            {
                MovePositions[0] = Cursor.RelativePosition;
                MovePositions[1] = App().GetDefaultView().GetCenter();
            }

            if(GUI::SpaceKey() && Cursor.Left.Hold)
            {
                Vector2f Delta = App().GetDefaultView().GetCenter();

                App().GetDefaultView().SetCenter(MovePositions[1]+
                Vector2f((MovePositions[0].x-Cursor.RelativePosition.x)/Zoom,
                (MovePositions[0].y-Cursor.RelativePosition.y)/Zoom));

                Delta = App().GetDefaultView().GetCenter() - Delta;

                if(Delta != Vector2f(0, 0))
                {
                    //cout << Delta.x << " " << Delta.y << " ";

                    Lvl->MoveView(Delta);
                }
            }

            Lvl->Draw(App);

            Lvl->DrawAIobjs(App, AIobjSymbols, FONT::Calibri);

            if(EditorMode == SOLIDPROPERTIES)
            {
                Sprite & sprite = Lvl->Solids[Selected]->Object.sprite.sprite;
                Shape rect = Shape::Rectangle(Vector2f(), sprite.GetSize(), Color(0, 0, 0, 0), 2, Color(255, 255, 255, 200));
                rect.SetCenter(sprite.GetCenter());
                rect.SetPosition(sprite.GetPosition());
                rect.SetRotation(sprite.GetRotation());
                App().Draw(rect);
            }
            else
            if(EditorMode == ARTIFACTPROPERTIES)
            {
                Sprite & sprite = Lvl->Artifacts[Selected]->Object.sprite.sprite;
                Shape rect = Shape::Rectangle(Vector2f(), sprite.GetSize(), Color(0, 0, 0, 0), 2, Color(255, 255, 255, 200));
                rect.SetCenter(sprite.GetCenter());
                rect.SetPosition(sprite.GetPosition());
                rect.SetRotation(sprite.GetRotation());
                App().Draw(rect);
            }

            if(!GUI::SpaceKey() && Cursor.Left.Pressed && Mode != ADDEDGEBOX && Mode != ADDACTIONBOX && Mode != ADDSPAWNPOINT
            && !ABWindow.CursorCollision() && !PWindow.CursorCollision() && !OB.CursorCollision() && !OB.SlotCursorCollision()
            && (!FB.GetActive() || !FB.CursorCollision()) && (!aImageWindow.GetActive() || !aImageWindow.CursorCollision())
            && (!aSolidWindow.GetActive() || !aSolidWindow.CursorCollision()) && (!aArtifactWindow.GetActive() || !aArtifactWindow.CursorCollision()))
            {
                bool Select = false;
                DepthSprite * spritePoint = NULL;

                for(int i = Lvl->DepthSprites.GetSize()-1; i >= 0; i--)
                {
                    if(PPCollisionWR(Cursor.Position, (*Lvl->DepthSprites[i])->sprite))
                    {
                        spritePoint = (*Lvl->DepthSprites[i]);
                        break;
                    }
                }

                if(spritePoint != NULL)
                for(int i = 0; i < Lvl->Solids.GetSize(); i++)
                {
                    if(spritePoint == &Lvl->Solids[i]->Object.sprite)
                    {
                        PickUp = true;
                        Select = true;

                        PickUpOffset = Cursor.Position - Lvl->Solids[i]->Object.Position;

                        if(Selected != i || EditorMode != SOLIDPROPERTIES)
                        {
                            Selected = i;
                            EditorMode = SOLIDPROPERTIES;
                            PropertiesMode = SOLID;

                            if(Mode != EDITOR)
                            Mode = EDITOR;
                        }
                    }
                }

                if(spritePoint != NULL && !Select)
                for(int i = 0; i < Lvl->Artifacts.GetSize(); i++)
                {
                    if(spritePoint == &Lvl->Artifacts[i]->Object.sprite)
                    {
                        PickUp = true;
                        Select = true;

                        PickUpOffset = Cursor.Position - Lvl->Artifacts[i]->Object.Position;

                        if(Selected != i || EditorMode != ARTIFACTPROPERTIES)
                        {
                            Selected = i;
                            EditorMode = ARTIFACTPROPERTIES;
                            PropertiesMode = ARTIFACT;

                            if(Mode != EDITOR)
                            Mode = EDITOR;
                        }
                    }
                }

                if(!Select)
                {
                    if(Mode == ADDIMAGE)
                    aImageWindow.Close();
                    else if(Mode == ADDSOLID)
                    aSolidWindow.Close();
                    else if(Mode == ADDARTIFACT)
                    aArtifactWindow.Close();

                    Mode = EDITOR;
                    EditorMode = ADDING;
                    PropertiesMode = WORLD;
                    Selected = -5;
                }
            }

            if(PickUp)
            {
                if(EditorMode == SOLIDPROPERTIES)
                {
                    Lvl->Solids[Selected]->Object.Position = Cursor.Position - PickUpOffset;
                    Lvl->Solids[Selected]->Object.sprite.sprite.SetPosition(Cursor.Position - PickUpOffset);
                }
                else
                if(EditorMode == ARTIFACTPROPERTIES)
                {
                    Lvl->Artifacts[Selected]->Object.Position = Cursor.Position - PickUpOffset;
                    Lvl->Artifacts[Selected]->Object.sprite.sprite.SetPosition(Cursor.Position - PickUpOffset);
                }

                if(Cursor.Left.Released)
                PickUp = false;
            }

            ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left, App().GetDefaultView().GetRect().Top);

            switch(Mode)
            {
                case EDITOR:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                default:
                                break;
                            }
                        }
                    }
                }
                break;

                case ADDIMAGE:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            aImageWindow.Close();
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                default:
                                break;
                            }
                            break;
                        }
                    }

                    if(aImageWindow.Update(App, Lvl, FB))
                    Mode = EDITOR;
                }
                break;

                case ADDANIMATION:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            AnimationNameBox.SetActive(true);
                            break;
                        }
                    }

                    if(Cursor.Left.Pressed)
                    {
                        AnimationNameBox.Clicked(App);
                        AnimationValueBox.Clicked(App);
                    }

                    AnimationNameBox.Write();
                    AnimationValueBox.Write();

//                    if(OkButton.Update(App))
//                    {
//                        Lvl->AddAnimation(AnimationNameBox.GetString(), "Data/Animation/" +  AnimationValueBox.GetString());
//
//                        Mode = EDITOR;
//                        AnimationNameBox.SetActive(true);
//                    }
//
//                    if(CancelButton.Update(App))
//                    {
//                        Mode = EDITOR;
//                        AnimationNameBox.SetActive(true);
//                    }

                    AnimationNameBox.Draw(App);
                    AnimationValueBox.Draw(App);
                    AnimationNameText.Draw(App, ViewPosition);
                    AnimationValueText.Draw(App, ViewPosition);
                }
                break;

                case ADDSOLID:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            aSolidWindow.Close();
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                default:
                                break;
                            }
                            break;
                        }
                    }

                    if(aSolidWindow.Update(App, Lvl, OB))
                    Mode = EDITOR;
                }
                break;

                case ADDARTIFACT:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            aArtifactWindow.Close();
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                default:
                                break;
                            }
                            break;
                        }
                    }

                    if(aArtifactWindow.Update(App, Lvl, OB))
                    Mode = EDITOR;
                }
                break;

                case ADDEDGEBOX:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                case ADDEDGEBOX:
                                    Mode = EDITOR;
                                break;

                                default:
                                break;
                            }
                            break;
                        }
                    }

                    if(!GUI::SpaceKey() && Cursor.Left.Pressed
                    && !ABWindow.CursorCollision() && !PWindow.CursorCollision()
                    && !OB.CursorCollision() && !OB.SlotCursorCollision())
                    {
                        EdgeBoxCreation = true;
                        tmpEbPos = Cursor.Position;
                    }

                    if(EdgeBoxCreation && Cursor.Left() && Cursor.Right.Pressed)
                    {
                        EdgeBoxCreation = false;
                    }

                    if(EdgeBoxCreation && !Cursor.Left())
                    {
                        Lvl->AddEdgeBox(tmpEbPos, true, (Cursor.Position.x >= tmpEbPos.x));
                        EdgeBoxCreation = false;
                    }

                    if(EdgeBoxCreation)
                    {
                        EbSymbol.SetPosition(tmpEbPos);
                        EbSymbol.FlipX(Cursor.Position.x >= tmpEbPos.x);
                        EbSymbol.SetColor(Color(255, 255, 255, 155));
                        App().Draw(EbSymbol);
                        EbSymbol.SetColor(Color(255, 255, 255, 255));
                    }

                    Lvl->DrawEdgeBoxes(App, EbSymbol);
                }
                break;

                case ADDACTIONBOX:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                case ADDACTIONBOX:
                                    Mode = EDITOR;
                                break;

                                default:
                                break;
                            }
                            break;
                        }
                    }

                    if(!GUI::SpaceKey() && Cursor.Left.Pressed
                    && !ABWindow.CursorCollision() && !PWindow.CursorCollision()
                    && !OB.CursorCollision() && !OB.SlotCursorCollision())
                    {
                        ActionBoxCreation = true;
                        tmpAbPos = Cursor.Position;
                    }

                    if(ActionBoxCreation && Cursor.Left() && Cursor.Right.Pressed)
                    {
                        ActionBoxCreation = false;
                    }

                    if(ActionBoxCreation && !Cursor.Left())
                    {
                        Lvl->AddActionBox(ToString(Lvl->ActionBoxes.GetSize()+1), tmpAbPos, Cursor.Position);
                        ActionBoxCreation = false;
                    }

                    if(ActionBoxCreation)
                    {
                        App().Draw(Shape::Rectangle(tmpAbPos, Cursor.Position, Color(255, 0, 0, 75), 2, Color(255, 0, 0, 150)));
                    }

                    Lvl->DrawActionBoxes(App, FONT::CalibriB);
                }
                break;

                case ADDSPAWNPOINT:
                {
                    for(int i = 0; i < ABWindow.Buttons.GetSize(); i++)
                    {
                        if(ABWindow.Buttons[i]->Pressed)
                        {
                            Mode = i+1;
                            switch(Mode)
                            {
                                case ADDIMAGE:
                                    aImageWindow.Open();
                                break;

                                case ADDSOLID:
                                    aSolidWindow.Open();
                                break;

                                case ADDARTIFACT:
                                    aArtifactWindow.Open();
                                break;

                                case ADDSPAWNPOINT:
                                    Mode = EDITOR;
                                break;

                                default:
                                break;
                            }
                            break;
                        }
                    }

                    if(!GUI::SpaceKey() && Cursor.Left.Released && !Cursor.Right()
                    && !ABWindow.CursorCollision() && !PWindow.CursorCollision()
                    && !OB.CursorCollision() && !OB.SlotCursorCollision())
                    {
                        Lvl->AddSpawnPoint(Cursor.Position);
                    }

                    if(!GUI::SpaceKey() && Cursor.Left()
                    && !ABWindow.CursorCollision() && !PWindow.CursorCollision()
                    && !OB.CursorCollision() && !OB.SlotCursorCollision())
                    {
                        SpSymbol.SetPosition(Cursor.Position);
                        SpSymbol.SetColor(Color(255, 255, 255, 155));
                        App().Draw(SpSymbol);
                        SpSymbol.SetColor(Color(255, 255, 255, 255));
                    }

                    Lvl->DrawSpawnPoints(App, SpSymbol);
                }
                break;

                default:
                {
                    cout << "Error in mode-loop!\n";
                    Mode = EDITOR;
                }
                break;
            }

            /// Update Windows
            PWindow.Update(Selected, App, Lvl, FB, OB);
            ABWindow.Update(App, Mode);
            if(OB.Update(App, Lvl))
            {
                Selected = OB.nSelected;
                PropertiesMode = OB.modeSelected;
                if(PropertiesMode == ARTIFACT)
                EditorMode = ARTIFACTPROPERTIES;
                else if(PropertiesMode == SOLID)
                EditorMode = SOLIDPROPERTIES;
                else
                EditorMode = ADDING;
            }
            //FB.Update(App); Handle In Separate Windows

            App.Display();

            sf::Sleep(1.f/200.f);
        }
    }

    SH.LvlFileName = PWindow.GetLvlFileName();

    delete Lvl;

    return 0;
}

