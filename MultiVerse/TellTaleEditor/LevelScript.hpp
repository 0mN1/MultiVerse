#ifndef __LEVELSCRIPT_HPP__
#define __LEVELSCRIPT_HPP__

#include "List.hpp"
#include "Animation.hpp"

using namespace LIST;

class DepthSprite
{
    public:
    Sprite sprite;
    AnimSprite * aSprite;
    Image * image;
    AnimImage * aImage;
    float depth;
    Vector2f * position;
    float rotation;
    Vector2f scale;
    bool isAnimation;

    DepthSprite()
    {
        isAnimation = false;
    }

    void Move(Vector2f v)
    {
        position->x += v.x;
        position->y += v.y;

        position->x -= v.x / depth;
        position->y -= v.y / depth;

        if(isAnimation)
        {
            aSprite->SetPosition(*position);
        }
        else
        {
            sprite.SetPosition(*position);
        }
    }

    void Draw(Application & App)
    {
        if(isAnimation)
        {
            aSprite->Draw(App);
        }
        else
        {
            App().Draw(sprite);
        }
    }

    ~DepthSprite()
    {
        if(isAnimation && aSprite != NULL)
        {
            delete aSprite;
            aSprite = NULL;
        }
    }
};

class Artifact
{
    public:
    Vector2f Position;
    DepthSprite sprite;
    string ImageName;
};

class Solid
{
    public:
    Vector2f Position;
    DepthSprite sprite;
    string ImageName;
};

template < class T >
class ScriptLoadObject
{
    public:
    string Name;
    string FileName;
    T Content;
};

void LoadImage(ScriptLoadObject< Image > * SLO)
{
    SLO->Content.LoadFromFile(SLO->FileName);
}

void LoadAnimation(ScriptLoadObject< AnimImage > * SLO)
{
    SLO->Content.LoadFromImage(SLO->FileName);
}

class ImageList : public List< ScriptLoadObject< Image > >
{
    protected:

    public:
    ImageList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    ImageList(const ImageList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptLoadObject< Image > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptLoadObject< Image > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptLoadObject< Image > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~ImageList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptLoadObject< Image > * Find(string Name)
    {
        LIST::Node< ScriptLoadObject< Image > > * Current = FirstNode;

        for(int i = 0; i < Size; i++)
        {
            if(i != 0)
            Current = Current->Next;

            if(Current->GetItem()->Name == Name)
            return Current->GetItem();
        }

        return NULL;
    }
};

class AnimationList : public List< ScriptLoadObject< AnimImage > >
{
    protected:

    public:
    AnimationList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    AnimationList(const AnimationList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptLoadObject< AnimImage > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptLoadObject< AnimImage > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptLoadObject< AnimImage > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~AnimationList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptLoadObject< AnimImage > * Find(string Name)
    {
        LIST::Node< ScriptLoadObject< AnimImage > > * Current = FirstNode;

        for(int i = 0; i < Size; i++)
        {
            if(i != 0)
            Current = Current->Next;

            if(Current->GetItem()->Name == Name)
            return Current->GetItem();
        }

        return NULL;
    }
};

class SpriteList : public List< DepthSprite* >
{
    protected:

    public:
    SpriteList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    SpriteList(const SpriteList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< DepthSprite* >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< DepthSprite* > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< DepthSprite* >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~SpriteList()
    {
        Clear();
    }

    inline DepthSprite* * Find(Vector2f * Position)
    {
        LIST::Node< DepthSprite* > * Current = FirstNode;

        for(int i = 0; i < Size; i++)
        {
            if(i != 0)
            Current = Current->Next;

            if((*Current->GetItem())->position == Position)
            return Current->GetItem();
        }

        return NULL;
    }

    void Sort()
    {
        if(Size > 1)
        {
            for(int j = 1; j < Size; j++)
            {
                int i = j;

                while(i > 0)
                {
                    if(GetElement(i - 1) && (*GetElement(i - 1))->depth < (*GetElement(i))->depth)
                    SwitchNodes(i - 1, i);
                    else
                    break;

                    --i;
                }
            }
        }
    }
};

class MusicTrack
{
    public:
    string Name;
    string FileName;
    Music Track;
    int LoopTimes;

    void Load()
    {
        Track.OpenFromFile(FileName);
    }
};

class MusicList : public List< MusicTrack >
{
    public:
    MusicList()
    {
        Size = 0;
        FirstNode = NULL;
    }

//    MusicList(const MusicList & Copy)
//    {
//        Size = Copy.Size;
//        FirstNode = NULL;
//
//        if(Size > 0)
//        {
//            FirstNode = new LIST::Node< MusicTrack >;
//            FirstNode->SetItem(*Copy.FirstNode->GetItem());
//
//            LIST::Node< MusicTrack > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;
//
//            for(int i = 1; i < Copy.Size; i++)
//            {
//                CopyCurrent = CopyCurrent->Next;
//
//                NewNode = new LIST::Node< MusicTrack >;
//                NewNode->SetItem(*CopyCurrent->GetItem());
//
//                Current->Next = NewNode;
//
//                Current = NewNode;
//            }
//        }
//    }

    ~MusicList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline MusicTrack * Find(string Name)
    {
        LIST::Node< MusicTrack > * Current = FirstNode;

        for(int i = 0; i < Size; i++)
        {
            if(i != 0)
            Current = Current->Next;

            if(Current->GetItem()->Name == Name)
            return Current->GetItem();
        }

        return NULL;
    }
};

class MusicRequest
{
    public:
    MusicTrack * Track;
    int Priority;
    int LoopTimes;
    int TimesLooped;

    MusicRequest()
    {
        TimesLooped = 0;
    }
};

class PlayList
{
    public:
    List< MusicRequest > TrackList;
    List< MusicRequest > DefaultList;
    bool ListType;

    void Play()
    {

    }

    void Update()
    {
        //if()
    }
};

template < class T >
class ScriptObject
{
    public:
    string Name;
    T Object;
};

class EdgeBox
{
    public:
    Vector2f Position;
    bool hasWall;
    bool fromRight;

    EdgeBox()
    {
        haveWall = true;
        fromRight = false;
    }
};

class Level
{
    public:

    /// ObjectLists and ObjectListFunctions

    ImageList Images;
    AnimationList Animations;
    List< ScriptObject< bool > > Booleans;
    List< ScriptObject< float > > Floats;
    List< ScriptObject< Artifact > > Artifacts;
    List< ScriptObject< Solid > > Solids;
    SpriteList DepthSprites;
    List< EdgeBox > EdgeBoxes;
    Color BgColor;
    float Gravity;

    bool drawEdgeBoxes;

    Level()
    {
        drawEdgeBoxes = false;
    }

    void ClearObjectLists()
    {
        Images.Clear();
        Animations.Clear();
        Artifacts.Clear();
        Solids.Clear();
        DepthSprites.Clear();
    }

    void MoveView(Vector2f v)
    {
//        cout << DepthSprites.GetSize() << endl;
//        for(int i = 0; i < DepthSprites.GetSize(); i++)
//        {
//            (*DepthSprites[i])->Move(v);
//        }

        for(int i = 0; i < Artifacts.GetSize(); i++)
        {
            Artifacts[i]->Object.sprite.Move(v);
//            cout << Artifacts[i]->Object.sprite.depth;
        }

//        cout << endl;
    }

    void MoveView(float x, float y)
    {
        Vector2f v(x, y);

//        cout << DepthSprites.GetSize() << endl;
//        for(int i = 0; i < DepthSprites.GetSize(); i++)
//        {
//            (*DepthSprites[i])->Move(v);
//        }

        for(int i = 0; i < Artifacts.GetSize(); i++)
        {
            Artifacts[i]->Object.sprite.Move(v);
//            cout << Artifacts[i]->Object.sprite.depth;
        }

//        cout << endl;
    }

    void DrawEdgeBoxes()
    {

    }

    void Draw(Application & App)
    {
        for(int i = 0; i < DepthSprites.GetSize(); i++)
        {
            (*DepthSprites[i])->Draw(App);
        }
    }

    /// CodeLists

    List< string > CodeLines;
    List< string > LevelLines;
    List< List< string > > ScriptLines;
    List< string > UndefinedLines;

    /// Add-Functions

    void AddBoolean(string Name, bool boolean)
    {
        Booleans.AddNode(ScriptObject< bool >());
        Booleans[Booleans.GetSize() - 1]->Name = Name;
        Booleans[Booleans.GetSize() - 1]->Object = boolean;
    }

    void AddFloat(string Name, float value)
    {
        Floats.AddNode(ScriptObject< float >());
        Floats[Floats.GetSize() - 1]->Name = Name;
        Floats[Floats.GetSize() - 1]->Object = value;
    }

    void AddImage(string Name, string FileName)
    {
        Images.AddNode(ScriptLoadObject< Image >());
        Images[Images.GetSize() - 1]->Name = Name;
        Images[Images.GetSize() - 1]->FileName = FileName;

        LoadImage(Images[Images.GetSize() - 1]);
    }

    void AddAnimation(string Name, string FileName)
    {
        Animations.AddNode(new ScriptLoadObject< AnimImage >());
        Animations[Animations.GetSize() - 1]->Name = Name;
        Animations[Animations.GetSize() - 1]->FileName = FileName;

        LoadAnimation(Animations[Animations.GetSize() - 1]);
    }

    void AddSolid(string Name, string ImageName, Vector2f position)
    {
        Solids.AddNode(ScriptObject< Solid >());
        Solids[Solids.GetSize() - 1]->Name = Name;
        Solids[Solids.GetSize() - 1]->Object.ImageName = ImageName;
        Solids[Solids.GetSize() - 1]->Object.Position = position;

        Solids[Solids.GetSize() - 1]->Object.sprite.sprite.SetImage(Images.Find(ImageName)->Content);
        SetCenterInCenter(Solids[Solids.GetSize() - 1]->Object.sprite.sprite);
        Solids[Solids.GetSize() - 1]->Object.sprite.sprite.SetPosition(position);
        Solids[Solids.GetSize() - 1]->Object.sprite.position = &Solids[Solids.GetSize() - 1]->Object.Position;
        Solids[Solids.GetSize() - 1]->Object.sprite.depth = 1.f;

        DepthSprites.AddNode(&Solids[Solids.GetSize() - 1]->Object.sprite);
        DepthSprites.Sort();
    }

    void AddArtifact(string Name, string isAnimation, string ImageName, Vector2f position, float depth)
    {
        Artifacts.AddNode(ScriptObject< Artifact >());
        Artifacts[Artifacts.GetSize() - 1]->Name = Name;
        Artifacts[Artifacts.GetSize() - 1]->Object.ImageName = ImageName;
        Artifacts[Artifacts.GetSize() - 1]->Object.Position = position;

        if(isAnimation == "Animation")
        {
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.aSprite = new AnimSprite;
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.aSprite->SetSource(&Animations.Find(ImageName)->Content);
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.aSprite->SetPosition(position);
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.isAnimation = true;
        }
        else
        {
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.sprite.SetImage(Images.Find(ImageName)->Content);
            SetCenterInCenter(Artifacts[Artifacts.GetSize() - 1]->Object.sprite.sprite);
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.sprite.SetPosition(position);
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.isAnimation = false;
        }

        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.position = &Artifacts[Artifacts.GetSize() - 1]->Object.Position;
        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.depth = depth;

        DepthSprites.AddNode(&Artifacts[Artifacts.GetSize() - 1]->Object.sprite);
        DepthSprites.Sort();
    }

    void AddEdgeBox(Vector2f Position, bool haveWall, bool fromRight)
    {
        EdgeBoxes.AddNode(EdgeBox());
        EdgeBoxes[EdgeBoxes.GetSize() - 1]->Position = Position;
        EdgeBoxes[EdgeBoxes.GetSize() - 1]->haveWall = haveWall;
        EdgeBoxes[EdgeBoxes.GetSize() - 1]->fromRight = fromRight;
    }

    void AddMusic(string Name, string FileName);

    /// Load and Save-Functions

    void Load(string Name)
    {
        enum {UNDEFINED, LEVELOBJECTS, SCRIPT};

        ifstream fin;
        fin.open(string(Name + ".GWlvl").c_str());

        string input;
        short LoadMode = UNDEFINED;
        bool ChangingMode = false;
        ClearObjectLists();

        while(!fin.eof() && fin.good())
        {
            getline(fin, input);
            ChangingMode = true;

            if(input == "@LevelObjects")
            {
                LoadMode = LEVELOBJECTS;
            }
            else if(input == "@Script")
            {
                LoadMode = SCRIPT;
            }
            else
            {
                ChangingMode = false;
            }

            if(!ChangingMode)
            {
                if(LoadMode == UNDEFINED)
                {
                    if(input != "")
                    UndefinedLines.AddNode(input);
                }
                else if(LoadMode == LEVELOBJECTS)
                {
                    if(input != "")
                    LevelLines.AddNode(input);
                }
                else if(LoadMode == SCRIPT)
                {
                    ScriptLines.AddNode(new List< string >());

                    string Word;

                    for(int i = 0; i < (int)input.size(); i++)
                    {
                        if(input[i] == ' ')
                        {
                            ScriptLines[ScriptLines.GetSize()-1]->AddNode(Word);
                            Word.erase();
                        }
                        else
                        {
                            Word += input[i];
                        }
                    }

                    ScriptLines[ScriptLines.GetSize()-1]->AddNode(Word);
                }
            }
        }

        fin.close();

        CodeToLevel();
    }

    void LevelToCode()
    {
        LevelLines.Clear();

        LevelLines.AddNode("@LevelObjects");

        for(int i = 0; i < Images.GetSize(); i++)
        {
            LevelLines.AddNode("addImage " + Images[i]->Name + " " + Images[i]->FileName);
        }

        for(int i = 0; i < Animations.GetSize(); i++)
        {
            LevelLines.AddNode("addAnimation " + Animations[i]->Name + " " + Animations[i]->FileName);
        }

        for(int i = 0; i < Solids.GetSize(); i++)
        {
            LevelLines.AddNode("addSolid " + Solids[i]->Name + " Image<-" + Solids[i]->Object.ImageName +
                               " " + ToString(Solids[i]->Object.Position.x) + " " + ToString(Solids[i]->Object.Position.y));
        }

        for(int i = 0; i < Artifacts.GetSize(); i++)
        {
            LevelLines.AddNode("addArtifact " + Artifacts[i]->Name + (Artifacts[i]->Object.sprite.isAnimation?" Animation<-":" Image<-") + Artifacts[i]->Object.ImageName +
                               " " + ToString(Artifacts[i]->Object.Position.x) + " " + ToString(Artifacts[i]->Object.Position.y));
        }
    }

    void CodeToLevel()
    {
        for(int i = 0; i < LevelLines.GetSize(); i++)
        {
            LevelDecoder(*LevelLines[i]);
        }
    }

    void LevelDecoder(string Line)
    {
        List< string > Words;
        Words.AddNode(string());
        for(int i = 0; i < (int)Line.size(); i++)
        {
            if(Line[i] == ' ')
            {
                Words.AddNode(string());
            }
            else if(Line[i] == '<' && Line[i+1] == '-')
            {
                i++;
                Words.AddNode(string());
            }
            else if(Line[i] == '#')
            {
                break;
            }
            else
            {
                *Words[Words.GetSize()-1] += Line[i];
            }
        }

        if(*Words[0] == "addImage")
        {
            AddImage(*Words[1], *Words[2]);
        }
        else if(*Words[0] == "addSolid")
        {
            AddSolid(*Words[1], *Words[3], Vector2f(StringToFloat(*Words[4]), StringToFloat(*Words[5])));
        }
        else if(*Words[0] == "addArtifact")
        {
            AddArtifact(*Words[1], *Words[2], *Words[3], Vector2f(StringToFloat(*Words[4]), StringToFloat(*Words[5])), StringToFloat(*Words[6]));
        }
        else if(*Words[0] == "setBgColor")
        {
            BgColor = Color(StringToInt(*Words[1]), StringToInt(*Words[2]), StringToInt(*Words[3]));
        }
        else if(*Words[0] == "setGravity")
        {
            Gravity = StringToFloat(*Words[1]) * 100;
        }
    }

    void Save(string Name)
    {
        LevelToCode();

        ofstream fout;
        fout.open(string(Name + ".GWlvl").c_str());

        for(int i = 0; i < LevelLines.GetSize(); i++)
        {
            fout << *LevelLines[i] << endl;
        }

        fout << "\n@Script\n";

        fout.close();
    }
};

#endif
