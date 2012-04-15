#ifndef __LEVELSCRIPT_HPP__
#define __LEVELSCRIPT_HPP__

#include "SFMLfunctions.hpp"
#include "List.hpp"
#include "Animation.hpp"
#include "ArtificialIntelligence.hpp"
#include "TellTaleSystem.hpp"

#include <dirent.h>

using namespace LIST;

/// Collision Functions
bool BBCollision(Vector2f p1, Vector2f p2a, Vector2f p2b)
{
    return (p1.x < p2b.x && p1.x > p2a.x
         && p1.y < p2b.y && p1.y > p2a.y);
}

bool BBCollision(Vector2f p1a, Vector2f p1b, Vector2f p2a, Vector2f p2b)
{
    return (p1a.x < p2b.x && p1b.x > p2a.x
         && p1a.y < p2b.y && p1b.y > p2a.y);
}

void ExploreDirectory(string path, List< string > & list)
{
    list.Clear();
    DIR * dir = opendir(path.c_str());

    if(!dir)
    {
        cerr << "\nError when trying to explore " << path << " with function: ExploreDirectory\n";

        return ;
    }

    struct dirent * dirEnt;

    while((dirEnt = readdir(dir)))
    {
        list.AddNode(dirEnt->d_name);
    }

    closedir(dir);
}

bool IsDirectory(string s)
{
    DIR * dir = opendir(s.c_str());

    bool isDir = dir;

    closedir(dir);

    return isDir;
}

void WriteDirectoryTree(string DirName, short i = 0)
{
    List< string > DirectoryContent;
    ExploreDirectory(DirName, DirectoryContent);

    for(int j = 0; j < DirectoryContent.GetSize(); j++)
    {
        if(*DirectoryContent[j] != "." && *DirectoryContent[j] != "..")
        {
            for(int n = 0; n < i; n++)
            cout << "  ";
            cout << *DirectoryContent[j];

            if(IsDirectory(DirName + *DirectoryContent[j]))
            {
                cout << "/\n";

                WriteDirectoryTree(DirName + *DirectoryContent[j] + "/", i+1);
            }
            else
            cout << endl;
        }
    }
}

/// LevelObject Classes
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
    bool Visible;
    char OwnerType;

    DepthSprite()
    {
        isAnimation = false;
        Visible = true;
        scale = Vector2f(1, 1);
        rotation = 0.f;
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

    /*void Zoom(Vector2f v)
    {
        scale.x *= v.x * depth;
        scale.y *= v.y * depth;

//        scale.x *= v.x / depth;
//        scale.y *= v.y / depth;

        if(isAnimation)
        {
            aSprite->SetScale(scale);
        }
        else
        {
            sprite.SetScale(scale);
        }
    }*/

    void Draw(Application & App)
    {
        if(isAnimation)
        {
            aSprite->SetPosition(*position);
            aSprite->SetRotation(rotation);
            aSprite->Draw(App);
        }
        else
        {
            sprite.SetPosition(*position);
            sprite.SetRotation(rotation);
            App().Draw(sprite);
        }
    }

    inline Vector2f GetUpperLeft()
    {
        return *position - sprite.GetCenter();
    }

    inline Vector2f GetLowerRight()
    {
        return *position + sprite.GetSize() - sprite.GetCenter();
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
    Vector2f Momentum;
    float RotMomentum;
};

class Solid
{
    public:
    Vector2f Position;
    DepthSprite sprite;
    string ImageName;
    Vector2f Momentum;
    bool Collision;

    Solid()
    {
        Collision = true;
    }
};

class AIobj
{
    public:
    string Name;
    int Type;
    Vector2f Position;
    bool Direction;
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

void LoadSound(ScriptLoadObject< SoundBuffer > * SLO)
{
    SLO->Content.LoadFromFile(SLO->FileName);
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

class SoundBufferList : public List< ScriptLoadObject< SoundBuffer > >
{
    protected:

    public:
    SoundBufferList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    SoundBufferList(const SoundBufferList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptLoadObject< SoundBuffer > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptLoadObject< SoundBuffer > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptLoadObject< SoundBuffer > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~SoundBufferList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptLoadObject< SoundBuffer > * Find(string Name)
    {
        LIST::Node< ScriptLoadObject< SoundBuffer > > * Current = FirstNode;

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

class SoundList : public List< Sound >
{
    protected:

    public:
    SoundList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    SoundList(const SoundList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< Sound >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< Sound > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< Sound >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~SoundList()
    {
        Clear();
    }
};

class MusicTrack
{
    public:
    string Name;
    string FileName;
    Music Track;

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
    int Priority; // 1 = play directly 2 = play after current loop 3 = play when done
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

class SpawnPoint
{
    public:
    Vector2f Position;
};

class EdgeBox
{
    public:
    Vector2f Position;
    bool hasWall;
    bool fromRight;

    EdgeBox()
    {
        hasWall = true;
        fromRight = false;
    }
};

class ActionBox
{
    public:
    string Name;
    Vector2f UpperLeft, LowerRight;

    ActionBox()
    {}
};

class SolidList : public List< ScriptObject< Solid > >
{
    protected:

    public:
    SolidList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    SolidList(const SolidList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptObject< Solid > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptObject< Solid > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptObject< Solid > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~SolidList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptObject< Solid > * Find(string Name)
    {
        LIST::Node< ScriptObject< Solid > > * Current = FirstNode;

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

class ArtifactList : public List< ScriptObject< Artifact > >
{
    protected:

    public:
    ArtifactList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    ArtifactList(const ArtifactList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptObject< Artifact > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptObject< Artifact > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptObject< Artifact > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~ArtifactList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptObject< Artifact > * Find(string Name)
    {
        LIST::Node< ScriptObject< Artifact > > * Current = FirstNode;

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

class ActionBoxList : public List< ScriptObject< ActionBox > >
{
    protected:

    public:
    ActionBoxList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    ActionBoxList(const ActionBoxList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptObject< ActionBox > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptObject< ActionBox > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptObject< ActionBox > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~ActionBoxList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptObject< ActionBox > * Find(string Name)
    {
        LIST::Node< ScriptObject< ActionBox > > * Current = FirstNode;

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

class AIList : public List< AIobj >
{
    protected:

    public:
    AIList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    AIList(const AIList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< AIobj >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< AIobj > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< AIobj >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~AIList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline AIobj * Find(string Name)
    {
        LIST::Node< AIobj > * Current = FirstNode;

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

class CPAIList : public List< CorpPeople >
{
    protected:

    public:
    CPAIList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    CPAIList(const CPAIList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< CorpPeople >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< CorpPeople > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< CorpPeople >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~CPAIList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline CorpPeople * Find(string Name)
    {
        LIST::Node< CorpPeople > * Current = FirstNode;

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

class BooleanList : public List< ScriptObject< bool > >
{
    protected:

    public:
    BooleanList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    BooleanList(const BooleanList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptObject< bool > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptObject< bool > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptObject< bool > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~BooleanList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptObject< bool > * Find(string Name)
    {
        LIST::Node< ScriptObject< bool > > * Current = FirstNode;

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

class IntList : public List< ScriptObject< int > >
{
    protected:

    public:
    IntList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    IntList(const IntList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptObject< int > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptObject< int > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptObject< int > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~IntList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptObject< int > * Find(string Name)
    {
        LIST::Node< ScriptObject< int > > * Current = FirstNode;

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

class FloatList : public List< ScriptObject< float > >
{
    protected:

    public:
    FloatList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    FloatList(const FloatList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptObject< float > >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptObject< float > > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptObject< float > >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~FloatList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptObject< float > * Find(string Name)
    {
        LIST::Node< ScriptObject< float > > * Current = FirstNode;

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

class ScriptFunction
{
    public:
    string Name;
    List< List< string > > Lines;
};

class FunctionList : public List< ScriptFunction >
{
    protected:

    public:
    FunctionList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    FunctionList(const FunctionList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< ScriptFunction >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< ScriptFunction > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< ScriptFunction >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~FunctionList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline ScriptFunction * Find(string Name)
    {
        LIST::Node< ScriptFunction > * Current = FirstNode;

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

class Level
{
    public:

    /// ObjectLists and ObjectListFunctions

    ImageList Images;
    AnimationList Animations;
    SoundBufferList SoundBuffers;
    SoundList Sounds;
    MusicList MusicTracks;
    PlayList Playlist;
    BooleanList Booleans;
    IntList Ints;
    FloatList Floats;
    ArtifactList Artifacts;
    SolidList Solids;
    SpriteList DepthSprites;
    List< SpawnPoint > SpawnPoints;
    List< EdgeBox > EdgeBoxes;
    ActionBoxList ActionBoxes;
    AIList AIs;
    CPAIList CPAIs;
    Color BgColor;
    Color VignetteColor;
    bool VignetteChanged;
    float Gravity;
    Vector2f prevZoom;

    bool DrawSolids;
    bool DrawArtifacts;

    bool Editor;

    bool GotoLevel;
    string NextLevel;
    int NextSpawnPoint;
    bool NextDirection;

    bool drawEdgeBoxes;

    Sprite * PlayerMask;
    Vector2f * PlayerPosition;

    /// CodeLists

    List< string > UndefinedLines;

    List< string > CodeLines;
    List< string > LevelLines;
    List< string > ScriptLines;
    List< List< string > > OnStartLines;
    List< List< string > > EveryFrameLines;
    List< List< string > > OnEndLines;

    FunctionList Functions;

    Level(bool inEditor, Sprite * pMask, Vector2f * pPosition)
    {
        Editor = inEditor;

        PlayerMask = pMask;
        PlayerPosition = pPosition;

        GotoLevel = false;
        NextSpawnPoint = 0;
        NextDirection = 0;

        DrawSolids = true;
        DrawArtifacts = true;

        VignetteChanged = false;
        VignetteColor = Color(255, 255, 255);

        drawEdgeBoxes = false;
        prevZoom = Vector2f(1, 1);
        Gravity = 982;
    }

    void ClearObjectLists()
    {
        Images.Clear();
        Animations.Clear();
        Artifacts.Clear();
        Solids.Clear();
        DepthSprites.Clear();
    }

    void LoadImageDirectory(string Path) /// Fix this function
    {
        if(Path[Path.size()-1] != '/')
        Path += '/';

        List< string > files;
        ExploreDirectory(Path, files);

        if(files.GetSize() > 2)
        {
            for(int i = 0; i < files.GetSize(); i++)
            {
                string CurrentFile = *files[i];

                if(CurrentFile != "." && CurrentFile != "..")
                {
                    if(IsDirectory(Path + CurrentFile))
                    {
                        LoadImageDirectory(Path + CurrentFile + '/');
                    }
                    else
                    {
                        string ext;
                        bool haveExt = false;

                        for(int j = CurrentFile.size()-1; j > (int)CurrentFile.size() - 5; j--)
                        {
                            if(CurrentFile[j] == '.')
                            {
                                haveExt = true;
                                break;
                            }

                            ext += CurrentFile[j];
                        }

                        if(haveExt)
                        {
                            string reverseExt = "";
                            string extLess = "";

                            for(int j = ext.size()-1; j >= 0; j--)
                            {
                                reverseExt += ext[j];
                            }

                            for(int j = 0; j < (int)(CurrentFile.size()-ext.size())-1; j++)
                            {
                                extLess += CurrentFile[j];
                            }

                            CaseShift(reverseExt, TO_LOWER);

                            if(reverseExt == "png" || reverseExt == "jpg" || reverseExt == "jpeg")
                            {
                                AddImage('i' + extLess, Path + CurrentFile);
                            }
                        }
                    }
                }

            }
        }
    }

    /// View, Graphics and DebugVisualization Functions

    void UpdateSprites()
    {
        for(int i = 0; i < DepthSprites.GetSize(); i++)
        {
            Image const * img = (*DepthSprites[i])->sprite.GetImage();
            Vector2f position = (*DepthSprites[i])->sprite.GetPosition();
            Vector2f center = (*DepthSprites[i])->sprite.GetCenter();

            (*DepthSprites[i])->sprite = Sprite(*img);
            (*DepthSprites[i])->sprite.SetCenter(center);
            (*DepthSprites[i])->sprite.SetPosition(position);
        }
    }

    void UpdateDepth()
    {
        DepthSprites.Sort();
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

    void DrawSpawnPoints(Application & App, Sprite & spSprite)
    {
        for(int i = 0; i < SpawnPoints.GetSize(); i++)
        {
            spSprite.SetPosition(SpawnPoints[i]->Position);

            App().Draw(spSprite);
        }
    }

    void DrawEdgeBoxes(Application & App, Sprite & ebSprite)
    {
        for(int i = 0; i < EdgeBoxes.GetSize(); i++)
        {
            ebSprite.SetPosition(EdgeBoxes[i]->Position);
            ebSprite.FlipX(EdgeBoxes[i]->fromRight);

            App().Draw(ebSprite);
        }
    }

    void DrawActionBoxes(Application & App, Font calibri)
    {
        String Name;
        Name.SetFont(calibri);
        Name.SetColor(Color(255, 255, 255, 200));
        Name.SetSize(16);

        for(int i = 0; i < ActionBoxes.GetSize(); i++)
        {
            App().Draw(Shape::Rectangle(ActionBoxes[i]->Object.UpperLeft, ActionBoxes[i]->Object.LowerRight, Color(255, 0, 0, 100), 2, Color(255, 0, 0, 200)));

            Name.SetText(ActionBoxes[i]->Name);
            Name.SetPosition(ActionBoxes[i]->Object.UpperLeft + Vector2f(0, -3));

            App().Draw(Name);
        }
    }

    void DrawIGAI(Application & App)
    {
        for(int i = 0; i < CPAIs.GetSize(); i++)
        {
            CPAIs[i]->Draw(App);
        }
    }

    void DrawAIobjs(Application & App, Sprite AiSprites[], Font calibri)
    {
        String Name;
        Name.SetFont(calibri);
        Name.SetColor(Color(255, 255, 255, 200));
        Name.SetSize(16);

        for(int i = 0; i < AIs.GetSize(); i++)
        {
            AiSprites[AIs[i]->Type].SetPosition(AIs[i]->Position);
            AiSprites[AIs[i]->Type].FlipX(AIs[i]->Direction);
            App().Draw(AiSprites[AIs[i]->Type]);

            Name.SetText(AIs[i]->Name);
            Name.SetCenter(GetCenter(Name));
            Name.SetPosition(AIs[i]->Position);

            App().Draw(Name);
        }
    }

    void DrawBack(Application & App)
    {
        DepthSprite * sprite = NULL;

        for(int i = 0; i < DepthSprites.GetSize(); i++)
        {
            sprite = *DepthSprites[i];

            if(sprite->depth >= 1)
            {
                if(sprite->Visible && ((sprite->OwnerType == 's' && DrawSolids)
                ||  (sprite->OwnerType == 'a' && DrawArtifacts)))
                sprite->Draw(App);
            }
            else
            break;
        }
    }

    void DrawFront(Application & App)
    {
        DepthSprite * sprite = NULL;

        for(int i = 0; i < DepthSprites.GetSize(); i++)
        {
            sprite = *DepthSprites[i];

            if(sprite->depth < 1)
            {
                if(sprite->Visible && ((sprite->OwnerType == 's' && DrawSolids)
                ||  (sprite->OwnerType == 'a' && DrawArtifacts)))
                sprite->Draw(App);
            }
        }
    }

    void Draw(Application & App)
    {
        for(int i = 0; i < DepthSprites.GetSize(); i++)
        {
            (*DepthSprites[i])->Draw(App);
        }
    }

    void UpdateSounds()
    {
        for(int i = 0; i < Sounds.GetSize(); i++)
        {
            if(Sounds[i]->GetStatus() == Sound::Stopped)
            {
                Sounds.RemoveNode(i);
                i--;
            }
        }
    }

    void UpdateMusic()
    {
        if(Playlist.TrackList.GetSize() > 0 && Playlist.TrackList[0]->Track->Track.GetStatus() == Music::Stopped)
        {
            Playlist.TrackList[0]->TimesLooped += 1;
            if((Playlist.TrackList[0]->LoopTimes != 0 && Playlist.TrackList[0]->TimesLooped > Playlist.TrackList[0]->LoopTimes)
            || (Playlist.TrackList.GetSize() > 1 && Playlist.TrackList[1]->Priority == 2))
            Playlist.TrackList.RemoveNode(0);

            if(Playlist.TrackList.GetSize() == 0)
            {
                for(int i = 0; i < Playlist.DefaultList.GetSize(); i++)
                {
                    Playlist.TrackList.AddNode(new MusicRequest());
                    Playlist.TrackList[i]->Track = Playlist.DefaultList[i]->Track;
                    Playlist.TrackList[i]->Priority = Playlist.DefaultList[i]->Priority;
                    Playlist.TrackList[i]->LoopTimes = Playlist.DefaultList[i]->LoopTimes;
                }
            }

            Playlist.TrackList[0]->Track->Track.Play();
        }
    }

    void UpdatePlatforms(Application & App)
    {
        ScriptObject< Solid > * solid = NULL;
        for(int i = 0; i < Solids.GetSize(); i++)
        {
            solid = Solids[i];
            solid->Object.Position.x += solid->Object.Momentum.x * App.GetFrameTime();
            solid->Object.Position.y += solid->Object.Momentum.y * App.GetFrameTime();
        }
    }

    void UpdateArtifacts(Application & App)
    {
        ScriptObject< Artifact > * artifact = NULL;
        for(int i = 0; i < Artifacts.GetSize(); i++)
        {
            artifact = Artifacts[i];
            artifact->Object.Position.x += artifact->Object.Momentum.x * App.GetFrameTime();
            artifact->Object.Position.y += artifact->Object.Momentum.y * App.GetFrameTime();
            artifact->Object.sprite.rotation += artifact->Object.RotMomentum * App.GetFrameTime();
        }
    }

    /// Add-Functions

    void AddBoolean(string Name, bool boolean)
    {
        Booleans.AddNode(ScriptObject< bool >());
        Booleans[Booleans.GetSize() - 1]->Name = Name;
        Booleans[Booleans.GetSize() - 1]->Object = boolean;
    }

    void AddInt(string Name, int value)
    {
        Ints.AddNode(ScriptObject< int >());
        Ints[Ints.GetSize() - 1]->Name = Name;
        Ints[Ints.GetSize() - 1]->Object = value;
    }

    void AddFloat(string Name, float value)
    {
        Floats.AddNode(ScriptObject< float >());
        Floats[Floats.GetSize() - 1]->Name = Name;
        Floats[Floats.GetSize() - 1]->Object = value;
    }

    void AddImage(string Name, string FileName)
    {
        if(Images.Find(Name) == NULL)
        {
            Images.AddNode(ScriptLoadObject< Image >());
            Images[Images.GetSize() - 1]->Name = Name;
            Images[Images.GetSize() - 1]->FileName = FileName;

            LoadImage(Images[Images.GetSize() - 1]);
        }
    }

    void AddAnimation(string Name, string FileName)
    {
        Animations.AddNode(new ScriptLoadObject< AnimImage >());
        Animations[Animations.GetSize() - 1]->Name = Name;
        Animations[Animations.GetSize() - 1]->FileName = FileName;

        LoadAnimation(Animations[Animations.GetSize() - 1]);
    }

    void AddSound(string Name, string FileName)
    {
        SoundBuffers.AddNode(ScriptLoadObject< SoundBuffer >());
        SoundBuffers[SoundBuffers.GetSize() - 1]->Name = Name;
        SoundBuffers[SoundBuffers.GetSize() - 1]->FileName = FileName;

        LoadSound(SoundBuffers[SoundBuffers.GetSize() - 1]);
    }

    void PlaySound(string SoundName)
    {
        Sounds.AddNode(new Sound(SoundBuffers.Find(SoundName)->Content));
        Sounds[Sounds.GetSize() - 1]->Play();
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
        Solids[Solids.GetSize() - 1]->Object.sprite.OwnerType = 's';

        DepthSprites.AddNode(&Solids[Solids.GetSize() - 1]->Object.sprite);
        DepthSprites.Sort();
    }

    void AddArtifact(string Name, string isAnimation, string ImageName, Vector2f position, float rotation = 0.f, float depth = 1.f)
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
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.OwnerType = 'a';
            Artifacts[Artifacts.GetSize() - 1]->Object.sprite.isAnimation = false;
        }

        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.position = &Artifacts[Artifacts.GetSize() - 1]->Object.Position;
        Artifacts[Artifacts.GetSize() - 1]->Object.RotMomentum = 0;
        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.rotation = rotation;
        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.depth = depth;

        DepthSprites.AddNode(&Artifacts[Artifacts.GetSize() - 1]->Object.sprite);
        DepthSprites.Sort();
    }

    void AddSpawnPoint(Vector2f Position)
    {
        SpawnPoints.AddNode(SpawnPoint());
        SpawnPoints[SpawnPoints.GetSize() - 1]->Position = Position;
    }

    void AddEdgeBox(Vector2f Position, bool hasWall, bool fromRight)
    {
        EdgeBoxes.AddNode(EdgeBox());
        EdgeBoxes[EdgeBoxes.GetSize() - 1]->Position = Position;
        EdgeBoxes[EdgeBoxes.GetSize() - 1]->hasWall = hasWall;
        EdgeBoxes[EdgeBoxes.GetSize() - 1]->fromRight = fromRight;
    }

    void AddActionBox(string Name, Vector2f Pos1, Vector2f Pos2)
    {
        ActionBoxes.AddNode(ScriptObject< ActionBox >());
        ActionBoxes[ActionBoxes.GetSize() - 1]->Name = Name;
        ActionBoxes[ActionBoxes.GetSize() - 1]->Object.UpperLeft = Vector2f(Min(Pos1.x, Pos2.x), Min(Pos1.y, Pos2.y));
        ActionBoxes[ActionBoxes.GetSize() - 1]->Object.LowerRight = Vector2f(Max(Pos1.x, Pos2.x), Max(Pos1.y, Pos2.y));
    }

    void AddAI(string Name, int Type, Vector2f Pos, bool Direction)
    {
        AIs.AddNode(AIobj());
        AIs[AIs.GetSize() - 1]->Name = Name;
        AIs[AIs.GetSize() - 1]->Type = Type;
        AIs[AIs.GetSize() - 1]->Position = Pos;
        AIs[AIs.GetSize() - 1]->Direction = Direction;
    }

    void AddIGAI(string Name, int Type, Vector2f Pos, bool Direction)
    {
        if(Type == AICORPPEOPLE || Type == AIUNSUEABLE)
        CPAIs.AddNode(new CorpPeople(Type == AIUNSUEABLE));
        CPAIs[CPAIs.GetSize()-1]->Name = Name;
        CPAIs[CPAIs.GetSize()-1]->Position = Pos;
        CPAIs[CPAIs.GetSize()-1]->Direction = Direction;
    }

    void AddMusicTrack(string Name, string FileName)
    {
        MusicTracks.AddNode(new MusicTrack());
        MusicTracks[MusicTracks.GetSize()-1]->Name = Name;
        MusicTracks[MusicTracks.GetSize()-1]->FileName = FileName;
        MusicTracks[MusicTracks.GetSize()-1]->Load();
    }

    void AddDefMusicRequest(string TrackName, int Prio, int LoopTimes)
    {
        Playlist.DefaultList.AddNode(new MusicRequest());
        Playlist.DefaultList[Playlist.DefaultList.GetSize()-1]->Track = MusicTracks.Find(TrackName);
        Playlist.DefaultList[Playlist.DefaultList.GetSize()-1]->Priority = Prio;
        Playlist.DefaultList[Playlist.DefaultList.GetSize()-1]->LoopTimes = LoopTimes;

        Playlist.TrackList.AddNode(new MusicRequest());
        Playlist.TrackList[Playlist.TrackList.GetSize()-1]->Track = MusicTracks.Find(TrackName);
        Playlist.TrackList[Playlist.TrackList.GetSize()-1]->Priority = Prio;
        Playlist.TrackList[Playlist.TrackList.GetSize()-1]->LoopTimes = LoopTimes;
    }

    /// Load and Save-Functions

    void Load(string Name, Application & App)
    {
        App().GetDefaultView().SetCenter(0, 0);

        enum {UNDEFINED, LEVELOBJECTS, SCRIPT};

        ifstream fin;
        fin.open(string("Data/Level/" + Name + ".lvl").c_str());

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
                    ScriptLines.AddNode(input);
                }
            }
        }

        fin.close();

        CodeToLevel();

        ScriptParser();

//        cout << "\n\nScript:\n\n";
//        for(int i = 0; i < ScriptLines.GetSize(); i++)
//        {
//            cout << *ScriptLines[i] << endl;
//        }

//        cout << "\nBooleans: " << Booleans.GetSize() << "     Ints: " << Ints.GetSize() << "     Floats: " << Floats.GetSize();
//        cout << "\nOSLines: " << OnStartLines.GetSize() << "     EFLines: " << EveryFrameLines.GetSize() << "     OELines: " << OnEndLines.GetSize();
//        cout << "\nFunctions: " << Functions.GetSize() << endl;

        RunOnStartScript(App);

        if(!Editor && Playlist.TrackList.GetSize() > 0)
        Playlist.TrackList[0]->Track->Track.Play();
    }

    void ScriptParser() /// Lägg till funktions-inladdning
    {
        bool FunctionMode = false;

        for(int j = 0; j < ScriptLines.GetSize(); j++)
        {
            string input = *ScriptLines[j];
            string Word;
            List< string > Words;

            for(int i = 0; i < (int)input.size(); i++)
            {
                if(input[i] == ' ' || input[i] == '\t')
                {
                    if(Word.size() > 0)
                    {
                        Words.AddNode(Word);
                        Word.erase();
                    }
                }
                else if(input[i] == '<' && input[i+1] == '-')
                {
                    if(Word.size() > 0)
                    {
                        Words.AddNode(Word);
                        Word.erase();
                        i++;
                    }
                }
                else if(input[i] == '#')
                {
                    break;
                }
                else if(input[i] == '!')
                {
                    if(Word.size() > 0)
                    {
                        Words.AddNode(Word);
                        Word.erase();
                    }

                    Word += input[i];

                    Words.AddNode(Word);
                    Word.erase();
                }
                else
                {
                    Word += input[i];
                }
            }

            if(Word.size() > 0)
            Words.AddNode(Word);

            if(Words.GetSize() > 0)
            {
                if(!FunctionMode)
                {
                    if(*Words[0] == "addBoolean")
                    {
                        AddBoolean(*Words[1], StringToInt(*Words[2]));
                    }
                    else if(*Words[0] == "addInt")
                    {
                        AddInt(*Words[1], StringToInt(*Words[2]));
                    }
                    else if(*Words[0] == "addFloat")
                    {
                        AddFloat(*Words[1], StringToFloat(*Words[2]));
                    }
                    else if(*Words[0] == "onStart")
                    {
                        OnStartLines.AddNode(List< string >());

                        for(int i = 1; i < Words.GetSize(); i++)
                        {
                            OnStartLines[OnStartLines.GetSize()-1]->AddNode(*Words[i]);
                        }
                    }
                    else if(*Words[0] == "everyFrame")
                    {
                        EveryFrameLines.AddNode(List< string >());

                        for(int i = 1; i < Words.GetSize(); i++)
                        {
                            EveryFrameLines[EveryFrameLines.GetSize()-1]->AddNode(*Words[i]);
                        }
                    }
                    else if(*Words[0] == "onEnd")
                    {
                        OnEndLines.AddNode(List< string >());

                        for(int i = 1; i < Words.GetSize(); i++)
                        {
                            OnEndLines[OnEndLines.GetSize()-1]->AddNode(*Words[i]);
                        }
                    }
                    else if(*Words[0] == "addFunction")
                    {
                        FunctionMode = true;
                        Functions.AddNode(ScriptFunction());
                        Functions[Functions.GetSize()-1]->Name = *Words[1];
                    }
                }
                else
                {
                    if(*Words[0] != "endFunction")
                    {
                        Functions[Functions.GetSize()-1]->Lines.AddNode(List< string >());
                        for(int i = 0; i < Words.GetSize(); i++)
                        {
                            Functions[Functions.GetSize()-1]->Lines[Functions[Functions.GetSize()-1]->Lines.GetSize()-1]->AddNode(*Words[i]);
                        }
                    }
                    else
                    {
                        FunctionMode = false;
                    }
                }
            }
        }
    }

    void RunOnStartScript(Application & App)
    {
        for(int i = 0; i < OnStartLines.GetSize(); i++)
        {
            ScriptDecoder(*OnStartLines[i], App);
        }
    }

    void RunEveryFrameScript(Application & App)
    {
        for(int i = 0; i < EveryFrameLines.GetSize(); i++)
        {
            ScriptDecoder(*EveryFrameLines[i], App);
        }
    }

    void RunOnEndScript(Application & App)
    {
        for(int i = 0; i < OnEndLines.GetSize(); i++)
        {
            ScriptDecoder(*OnEndLines[i], App);
        }
    }

    void ScriptDecoder(List< string > & Words, Application & App) /// Add a move view function
    {
        bool result = false, ifstatement = false, andSign = false;

        for(int i = 0; i < Words.GetSize(); i++) /// When adding something, don't forget i += #;
        {
            if(*Words[i] == "&&")
            {
                andSign = true;
                if(!result)
                break;
            }
            else if(*Words[i] == "||")
            {
                andSign = false;
            }
            else if(*Words[i] == ":")
            {
                ifstatement = false;
                if(!result)
                break;
            }
            else if(*Words[i] == "if" || ifstatement)
            {
                bool invert = false;

                if(ifstatement)
                i--;

                if(*Words[i+1] == "!")
                {
                    invert = true;
                    i += 1;
                }

                if(*Words[i+1] == "Boolean")
                {
                    ScriptObject< bool > * boolean = Booleans.Find(*Words[i+2]);

                    if(*Words[i+3] == "==")
                    {
                        if(*Words[i+4] == "Boolean")
                        {
                            ScriptObject< bool > * boolean2 = Booleans.Find(*Words[i+5]);

                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(boolean->Object == boolean2->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(boolean->Object == boolean2->Object);
                                    }
                                }
                                else
                                {
                                    result = !(boolean->Object == boolean2->Object);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (boolean->Object == boolean2->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (boolean->Object == boolean2->Object);
                                    }
                                }
                                else
                                {
                                    result = (boolean->Object == boolean2->Object);
                                }
                            }

                            i += 5;
                        }
                        else if(*Words[i+4] == "true")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(boolean->Object == true);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(boolean->Object == true);
                                    }
                                }
                                else
                                {
                                    result = !(boolean->Object == true);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (boolean->Object == true);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (boolean->Object == true);
                                    }
                                }
                                else
                                {
                                    result = (boolean->Object == true);
                                }
                            }

                            i += 4;
                        }
                        else if(*Words[i+4] == "false")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(boolean->Object == false);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(boolean->Object == false);
                                    }
                                }
                                else
                                {
                                    result = !(boolean->Object == false);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (boolean->Object == false);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (boolean->Object == false);
                                    }
                                }
                                else
                                {
                                    result = (boolean->Object == false);
                                }
                            }

                            i += 4;
                        }
                    }
                    else if(*Words[i+3] == "!=")
                    {
                        if(*Words[i+4] == "Boolean")
                        {
                            ScriptObject< bool > * boolean2 = Booleans.Find(*Words[i+5]);

                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(boolean->Object != boolean2->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(boolean->Object != boolean2->Object);
                                    }
                                }
                                else
                                {
                                    result = !(boolean->Object != boolean2->Object);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (boolean->Object != boolean2->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (boolean->Object != boolean2->Object);
                                    }
                                }
                                else
                                {
                                    result = (boolean->Object != boolean2->Object);
                                }
                            }

                            i += 5;
                        }
                        else if(*Words[i+4] == "true")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(boolean->Object != true);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(boolean->Object != true);
                                    }
                                }
                                else
                                {
                                    result = !(boolean->Object != true);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (boolean->Object != true);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (boolean->Object != true);
                                    }
                                }
                                else
                                {
                                    result = (boolean->Object != true);
                                }
                            }

                            i += 4;
                        }
                        else if(*Words[i+4] == "false")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(boolean->Object != false);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(boolean->Object != false);
                                    }
                                }
                                else
                                {
                                    result = !(boolean->Object != false);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (boolean->Object != false);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (boolean->Object != false);
                                    }
                                }
                                else
                                {
                                    result = (boolean->Object != false);
                                }
                            }

                            i += 4;
                        }
                    }
                    else
                    {
                        if(invert)
                        {
                            if(ifstatement)
                            {
                                if(andSign)
                                {
                                    result = !boolean->Object;
                                }
                                else
                                {
                                    if(!result)
                                    result = !boolean->Object;
                                }
                            }
                            else
                            {
                                result = !boolean->Object;
                            }
                        }
                        else
                        {
                            if(ifstatement)
                            {
                                if(andSign)
                                {
                                    result = boolean->Object;
                                }
                                else
                                {
                                    if(!result)
                                    result = boolean->Object;
                                }
                            }
                            else
                            {
                                result = boolean->Object;
                            }
                        }

                        i += 2;
                    }
                }
                else if(*Words[i+1] == "Int")
                {
                    ScriptObject< int > * Int = Ints.Find(*Words[i+2]);

                    if(*Words[i+3] == "==")
                    {
                        if(*Words[i+4] == "Int")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object == Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object == Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object == Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object == Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object == Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Int->Object == Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object == StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object == StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object == StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object == StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object == StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Int->Object == StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == "!=")
                    {
                        if(*Words[i+4] == "Int")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object != Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object != Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object != Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object != Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object != Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Int->Object != Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object != StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object != StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object != StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object != StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object != StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Int->Object != StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == "<=")
                    {
                        if(*Words[i+4] == "Int")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object <= Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object <= Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object <= Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object <= Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object <= Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Int->Object <= Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object <= StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object <= StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object <= StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object <= StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object <= StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Int->Object <= StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == ">=")
                    {
                        if(*Words[i+4] == "Int")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object >= Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object >= Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object >= Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object >= Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object >= Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Int->Object >= Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object >= StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object >= StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object >= StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object >= StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object >= StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Int->Object >= StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == "<")
                    {
                        if(*Words[i+4] == "Int")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object < Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object < Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object < Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object < Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object < Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Int->Object < Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object < StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object < StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object < StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object < StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object < StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Int->Object < StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == ">")
                    {
                        if(*Words[i+4] == "Int")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object > Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object > Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object > Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object > Ints.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object > Ints.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Int->Object > Ints.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Int->Object > StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Int->Object > StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Int->Object > StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Int->Object > StringToInt(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Int->Object > StringToInt(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Int->Object > StringToInt(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                }
                else if(*Words[i+1] == "Float")
                {
                    ScriptObject< float > * Float = Floats.Find(*Words[i+2]);

                    if(*Words[i+3] == "==")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object == Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object == Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object == Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object == Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object == Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Float->Object == Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object == StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object == StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object == StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object == StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object == StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Float->Object == StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == "!=")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object != Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object != Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object != Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object != Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object != Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Float->Object != Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object != StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object != StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object != StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object != StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object != StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Float->Object != StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == "<=")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object <= Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object <= Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object <= Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object <= Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object <= Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Float->Object <= Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object <= StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object <= StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object <= StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object <= StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object <= StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Float->Object <= StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == ">=")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object >= Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object >= Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object >= Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object >= Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object >= Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Float->Object >= Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object >= StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object >= StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object >= StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object >= StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object >= StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Float->Object >= StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == "<")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object < Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object < Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object < Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object < Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object < Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Float->Object < Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object < StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object < StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object < StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object < StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object < StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Float->Object < StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                    else if(*Words[i+3] == ">")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object > Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object > Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object > Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object > Floats.Find(*Words[i+5])->Object);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object > Floats.Find(*Words[i+5])->Object);
                                    }
                                }
                                else
                                {
                                    result = (Float->Object > Floats.Find(*Words[i+5])->Object);
                                }

                                i += 5;
                            }
                        }
                        else
                        {
                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !(Float->Object > StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !(Float->Object > StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = !(Float->Object > StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = (Float->Object > StringToFloat(*Words[i+4]));
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = (Float->Object > StringToFloat(*Words[i+4]));
                                    }
                                }
                                else
                                {
                                    result = (Float->Object > StringToFloat(*Words[i+4]));
                                }

                                i += 4;
                            }
                        }
                    }
                }
                else if(*Words[i+1] == "Artifact")
                {
                    ScriptObject< Artifact > * artifact = Artifacts.Find(*Words[i+2]);

                    if(*Words[i+3] == "Rotation")
                    {
                        if(*Words[i+4] == "==")
                        {
                            if(*Words[i+5] == "Float")
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation == Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation == Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation == Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation == Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation == Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation == Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                            }
                            else
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation == StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation == StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation == StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation == StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation == StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation == StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                            }
                        }
                        else if(*Words[i+4] == "!=")
                        {
                            if(*Words[i+5] == "Float")
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation != Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation != Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation != Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation != Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation != Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation != Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                            }
                            else
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation != StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation != StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation != StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation != StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation != StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation != StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                            }
                        }
                        else if(*Words[i+4] == "<=")
                        {
                            if(*Words[i+5] == "Float")
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation <= Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation <= Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation <= Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation <= Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation <= Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation <= Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                            }
                            else
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation <= StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation <= StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation <= StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation <= StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation <= StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation <= StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                            }
                        }
                        else if(*Words[i+4] == ">=")
                        {
                            if(*Words[i+5] == "Float")
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation >= Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation >= Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation >= Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation >= Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation >= Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation >= Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                            }
                            else
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation >= StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation >= StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation >= StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation >= StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation >= StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation >= StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                            }
                        }
                        else if(*Words[i+4] == "<")
                        {
                            if(*Words[i+5] == "Float")
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation < Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation < Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation < Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation < Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation < Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation < Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                            }
                            else
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation < StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation < StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation < StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation < StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation < StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation < StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                            }
                        }
                        else if(*Words[i+4] == ">")
                        {
                            if(*Words[i+5] == "Float")
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation > Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation > Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation > Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation > Floats.Find(*Words[i+6])->Object);
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation > Floats.Find(*Words[i+6])->Object);
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation > Floats.Find(*Words[i+6])->Object);
                                    }

                                    i += 6;
                                }
                            }
                            else
                            {
                                if(invert)
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = !(artifact->Object.sprite.rotation > StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = !(artifact->Object.sprite.rotation > StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = !(artifact->Object.sprite.rotation > StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                                else
                                {
                                    if(ifstatement)
                                    {
                                        if(andSign)
                                        {
                                            result = (artifact->Object.sprite.rotation > StringToFloat(*Words[i+5]));
                                        }
                                        else
                                        {
                                            if(!result)
                                            result = (artifact->Object.sprite.rotation > StringToFloat(*Words[i+5]));
                                        }
                                    }
                                    else
                                    {
                                        result = (artifact->Object.sprite.rotation > StringToFloat(*Words[i+5]));
                                    }

                                    i += 5;
                                }
                            }
                        }
                    }
                }
                else if(*Words[i+1] == "ABCollision")
                {
                    if(*Words[i+2] == "Player")
                    {
                        if(*Words[i+3] == "Mask")
                        {
                            ScriptObject< ActionBox > * actionBox = NULL;

                            if(*Words[i+4] == "ActionBox")
                            actionBox = ActionBoxes.Find(*Words[i+5]);

                            if(actionBox == NULL)
                            cerr << "\n!Error: " << *Words[i+5] << " doesnt exist!\n";

                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !BBCollision(*PlayerPosition - PlayerMask->GetCenter(),
                                                             *PlayerPosition + PlayerMask->GetSize() - PlayerMask->GetCenter(),
                                                             actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !BBCollision(*PlayerPosition - PlayerMask->GetCenter(),
                                                             *PlayerPosition + PlayerMask->GetSize() - PlayerMask->GetCenter(),
                                                             actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                }
                                else
                                {
                                    result = !BBCollision(*PlayerPosition - PlayerMask->GetCenter(),
                                                         *PlayerPosition + PlayerMask->GetSize() - PlayerMask->GetCenter(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = BBCollision(*PlayerPosition - PlayerMask->GetCenter(),
                                                             *PlayerPosition + PlayerMask->GetSize() - PlayerMask->GetCenter(),
                                                             actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = BBCollision(*PlayerPosition - PlayerMask->GetCenter(),
                                                             *PlayerPosition + PlayerMask->GetSize() - PlayerMask->GetCenter(),
                                                             actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                }
                                else
                                {
                                    result = BBCollision(*PlayerPosition - PlayerMask->GetCenter(),
                                                         *PlayerPosition + PlayerMask->GetSize() - PlayerMask->GetCenter(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }

                            i += 5;
                        }
                        else if(*Words[i+3] == "Position")
                        {
                            ScriptObject< ActionBox > * actionBox = NULL;

                            if(*Words[i+4] == "ActionBox")
                            actionBox = ActionBoxes.Find(*Words[i+5]);

                            if(actionBox == NULL)
                            cerr << "\n!Error: " << *Words[i+5] << " doesnt exist!\n";

                            if(invert)
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = !BBCollision(*PlayerPosition, actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = !BBCollision(*PlayerPosition, actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                }
                                else
                                {
                                    result = !BBCollision(*PlayerPosition, actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }
                            else
                            {
                                if(ifstatement)
                                {
                                    if(andSign)
                                    {
                                        result = BBCollision(*PlayerPosition, actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                    else
                                    {
                                        if(!result)
                                        result = BBCollision(*PlayerPosition, actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                    }
                                }
                                else
                                {
                                    result = BBCollision(*PlayerPosition, actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }

                            i += 5;
                        }
                    }
                    else if(*Words[i+2] == "Solid")
                    {
                        ScriptObject< ActionBox > * actionBox = NULL;
                        ScriptObject< Solid > * solid = Solids.Find(*Words[i+3]);

                        if(*Words[i+4] == "ActionBox")
                        actionBox = ActionBoxes.Find(*Words[i+5]);

                        if(actionBox == NULL)
                        cerr << "\n!Error: " << *Words[i+5] << " doesnt exist!\n";

                        if(invert)
                        {
                            if(ifstatement)
                            {
                                if(andSign)
                                {
                                    result = !BBCollision(solid->Object.sprite.GetUpperLeft(), solid->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                                else
                                {
                                    if(!result)
                                    result = !BBCollision(solid->Object.sprite.GetUpperLeft(), solid->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }
                            else
                            {
                                result = !BBCollision(solid->Object.sprite.GetUpperLeft(), solid->Object.sprite.GetLowerRight(),
                                                     actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                            }
                        }
                        else
                        {
                            if(ifstatement)
                            {
                                if(andSign)
                                {
                                    result = BBCollision(solid->Object.sprite.GetUpperLeft(), solid->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                                else
                                {
                                    if(!result)
                                    result = BBCollision(solid->Object.sprite.GetUpperLeft(), solid->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }
                            else
                            {
                                result = BBCollision(solid->Object.sprite.GetUpperLeft(), solid->Object.sprite.GetLowerRight(),
                                                     actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                            }
                        }

                        i += 5;
                    }
                    else if(*Words[i+2] == "Artifact")
                    {
                        ScriptObject< ActionBox > * actionBox = NULL;
                        ScriptObject< Artifact > * artifact = Artifacts.Find(*Words[i+3]);

                        if(*Words[i+4] == "ActionBox")
                        actionBox = ActionBoxes.Find(*Words[i+5]);

                        if(actionBox == NULL)
                        cerr << "\n!Error: " << *Words[i+5] << " doesnt exist!\n";

                        if(invert)
                        {
                            if(ifstatement)
                            {
                                if(andSign)
                                {
                                    result = !BBCollision(artifact->Object.sprite.GetUpperLeft(), artifact->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                                else
                                {
                                    if(!result)
                                    result = !BBCollision(artifact->Object.sprite.GetUpperLeft(), artifact->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }
                            else
                            {
                                result = !BBCollision(artifact->Object.sprite.GetUpperLeft(), artifact->Object.sprite.GetLowerRight(),
                                                     actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                            }
                        }
                        else
                        {
                            if(ifstatement)
                            {
                                if(andSign)
                                {
                                    result = BBCollision(artifact->Object.sprite.GetUpperLeft(), artifact->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                                else
                                {
                                    if(!result)
                                    result = BBCollision(artifact->Object.sprite.GetUpperLeft(), artifact->Object.sprite.GetLowerRight(),
                                                         actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                                }
                            }
                            else
                            {
                                result = BBCollision(artifact->Object.sprite.GetUpperLeft(), artifact->Object.sprite.GetLowerRight(),
                                                     actionBox->Object.UpperLeft, actionBox->Object.LowerRight);
                            }
                        }

                        i += 5;
                    }
                }

                ifstatement = true;
            }
            else if(*Words[i] == "Solid")
            {
                ScriptObject< Solid > * solid = Solids.Find(*Words[i+1]);

                if(*Words[i+2] == "Alpha")
                {
                    if(*Words[i+3] == "=")
                    {
                        Color c = solid->Object.sprite.sprite.GetColor();
                        c.a = StringToInt(*Words[i+4]);
                        solid->Object.sprite.sprite.SetColor(c);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "xPosition")
                {
                    if(*Words[i+3] == "=")
                    {
                        solid->Object.Position.x = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        solid->Object.Position.x += StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        solid->Object.Position.x -= StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "yPosition")
                {
                    if(*Words[i+3] == "=")
                    {
                        solid->Object.Position.y = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        solid->Object.Position.y += StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        solid->Object.Position.y -= StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "xMomentum")
                {
                    if(*Words[i+3] == "=")
                    {
                        solid->Object.Momentum.x = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        solid->Object.Momentum.x += StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        solid->Object.Momentum.x -= StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "yMomentum")
                {
                    if(*Words[i+3] == "=")
                    {
                        solid->Object.Momentum.y = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        solid->Object.Momentum.y += StringToInt(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        solid->Object.Momentum.y -= StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "Collision")
                {
                    if(*Words[i+3] == "=")
                    {
                        solid->Object.Collision = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "Visible")
                {
                    if(*Words[i+3] == "=")
                    {
                        solid->Object.sprite.Visible = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
            }
            else if(*Words[i] == "Artifact")
            {
                ScriptObject< Artifact > * artifact = Artifacts.Find(*Words[i+1]);

                if(*Words[i+2] == "Alpha")
                {
                    if(*Words[i+3] == "=")
                    {
                        Color c = artifact->Object.sprite.sprite.GetColor();
                        c.a = StringToInt(*Words[i+4]);
                        artifact->Object.sprite.sprite.SetColor(c);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "xMomentum")
                {
                    if(*Words[i+3] == "=")
                    {
                        artifact->Object.Momentum.x = StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "yMomentum")
                {
                    if(*Words[i+3] == "=")
                    {
                        artifact->Object.Momentum.y = StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "rotMomentum")
                {
                    if(*Words[i+3] == "=")
                    {
                        artifact->Object.RotMomentum = StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "xPosition")
                {
                    if(*Words[i+3] == "=")
                    {
                        artifact->Object.Position.x = StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        artifact->Object.Position.x += StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        artifact->Object.Position.x -= StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "yPosition")
                {
                    if(*Words[i+3] == "=")
                    {
                        artifact->Object.Position.y = StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        artifact->Object.Position.y += StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        artifact->Object.Position.y -= StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "Rotation")
                {
                    if(*Words[i+3] == "=")
                    {
                        if(*Words[i+4] == "Float")
                        {
                            artifact->Object.sprite.rotation = Floats.Find(*Words[i+5])->Object;
                            i += 5;
                        }
                        else
                        {
                            artifact->Object.sprite.rotation = StringToFloat(*Words[i+4]);
                            i += 4;
                        }
                    }
                    else if(*Words[i+3] == "+=")
                    {
                        artifact->Object.sprite.rotation += StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                    else if(*Words[i+3] == "-=")
                    {
                        artifact->Object.sprite.rotation -= StringToFloat(*Words[i+4]);

                        i += 4;
                    }
                }
                else if(*Words[i+2] == "Visible")
                {
                    if(*Words[i+3] == "=")
                    {
                        artifact->Object.sprite.Visible = StringToInt(*Words[i+4]);

                        i += 4;
                    }
                }
            }
            else if(*Words[i] == "Boolean")
            {
                ScriptObject< bool > * boolean = Booleans.Find(*Words[i+1]);

                if(*Words[i+2] == "=")
                {
                    if(*Words[i+3] == "!")
                    {
                        if(*Words[i+4] == "Boolean")
                        {
                            boolean->Object = !Booleans.Find(*Words[i+5])->Object;

                            i += 5;
                        }
                        else if(*Words[i+4] == "true")
                        {
                            boolean->Object = false;

                            i += 4;
                        }
                        else if(*Words[i+4] == "false")
                        {
                            boolean->Object = true;

                            i += 4;
                        }
                    }
                    else if(*Words[i+3] == "Boolean")
                    {
                        boolean->Object = Booleans.Find(*Words[i+4])->Object;

                        i += 4;
                    }
                    else if(*Words[i+3] == "true")
                    {
                        boolean->Object = true;

                        i += 3;
                    }
                    else if(*Words[i+3] == "false")
                    {
                        boolean->Object = false;

                        i += 3;
                    }
                    else
                    {
                        boolean->Object = StringToInt(*Words[i+3]);

                        i += 3;
                    }
                }
            }
            else if(*Words[i] == "Int")
            {
                ScriptObject< int > * Int = Ints.Find(*Words[i+1]);

                if(*Words[i+2] == "=")
                {
                    if(*Words[i+3] == "FrameTime")
                    Int->Object = (int)App.GetFrameTime();
                    else
                    Int->Object = StringToInt(*Words[i+3]);

                    i += 3;
                }
                else if(*Words[i+2] == "+=")
                {
                    if(*Words[i+3] == "FrameTime")
                    Int->Object += (int)App.GetFrameTime();
                    else
                    Int->Object += StringToInt(*Words[i+3]);

                    i += 3;
                }
                else if(*Words[i+2] == "-=")
                {
                    if(*Words[i+3] == "FrameTime")
                    Int->Object -= (int)App.GetFrameTime();
                    else
                    Int->Object -= StringToInt(*Words[i+3]);

                    i += 3;
                }
            }
            else if(*Words[i] == "Float")
            {
                ScriptObject< float > * Float = Floats.Find(*Words[i+1]);

                if(*Words[i+2] == "=")
                {
                    if(*Words[i+3] == "FrameTime")
                    Float->Object = App.GetFrameTime();
                    else
                    Float->Object = StringToFloat(*Words[i+3]);

                    i += 3;
                }
                else if(*Words[i+2] == "+=")
                {
                    if(*Words[i+3] == "FrameTime")
                    Float->Object += App.GetFrameTime();
                    else
                    Float->Object += StringToFloat(*Words[i+3]);

                    i += 3;
                }
                else if(*Words[i+2] == "-=")
                {
                    if(*Words[i+3] == "FrameTime")
                    Float->Object -= App.GetFrameTime();
                    else
                    Float->Object -= StringToFloat(*Words[i+3]);

                    i += 3;
                }
            }
            else if(*Words[i] == "Function")
            {
                ScriptFunction * function = Functions.Find(*Words[i+1]);

                for(int j = 0; j < function->Lines.GetSize(); j++)
                {
                    ScriptDecoder(*function->Lines[j], App);
                }

                i += 1;
            }
            else if(*Words[i] == "GotoLevel")
            {
                GotoLevel = true;
                NextLevel = *Words[i+1];
                NextSpawnPoint = StringToInt(*Words[i+2]);
                NextDirection = StringToInt(*Words[i+3]);

                i+=3;
            }
            else if(*Words[i] == "PlaySound")
            {
                PlaySound(*Words[i+2]);

                i+=2;
            }
            else if(*Words[i] == "Output")
            {
                if(*Words[i+1] == "Boolean")
                {
                    cout << " " << Booleans.Find(*Words[i+2])->Object << " ";

                    i += 2;
                }
                else if(*Words[i+1] == "Float")
                {
                    cout << " " << Floats.Find(*Words[i+2])->Object << " ";

                    i += 2;
                }
                else
                {
                    cout << " " << *Words[i+1] << " ";

                    i += 1;
                }
            }
            else
            {
                cout << "Error: " << *Words[i] << "\t";
            }
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
        else if(*Words[0] == "addSound")
        {
            AddSound(*Words[1], *Words[2]);
        }
        else if(*Words[0] == "addMusicTrack")
        {
            AddMusicTrack(*Words[1], *Words[2]);
        }
        else if(*Words[0] == "addMusicRequest")
        {
            AddDefMusicRequest(*Words[2], StringToInt(*Words[3]), StringToInt(*Words[4]));
        }
        else if(*Words[0] == "addSolid")
        {
            AddSolid(*Words[1], *Words[3], Vector2f(StringToFloat(*Words[4]), StringToFloat(*Words[5])));
        }
        else if(*Words[0] == "addArtifact")
        {
            AddArtifact(*Words[1], *Words[2], *Words[3], Vector2f(StringToFloat(*Words[4]), StringToFloat(*Words[5])), StringToFloat(*Words[6]), StringToFloat(*Words[7]));
        }
        else if(*Words[0] == "addSpawnPoint")
        {
            AddSpawnPoint(Vector2f(StringToFloat(*Words[1]), StringToFloat(*Words[2])));
        }
        else if(*Words[0] == "addEdgeBox")
        {
            AddEdgeBox(Vector2f(StringToFloat(*Words[1]), StringToFloat(*Words[2])), StringToInt(*Words[3]), StringToInt(*Words[4]));
        }
        else if(*Words[0] == "addActionBox")
        {
            AddActionBox(*Words[1], Vector2f(StringToFloat(*Words[2]), StringToFloat(*Words[3])), Vector2f(StringToFloat(*Words[4]), StringToFloat(*Words[5])));
        }
        else if(*Words[0] == "addAI")
        {
            int Type = 0;

            if(*Words[2] == "CorpPeople")
            Type = AICORPPEOPLE;
            else if(*Words[2] == "Unsueable")
            Type = AIUNSUEABLE;

            if(Editor)
            AddAI(*Words[1], Type, Vector2f(StringToFloat(*Words[3]), StringToFloat(*Words[4])), StringToInt(*Words[5]));
            else
            AddIGAI(*Words[1], Type, Vector2f(StringToFloat(*Words[3]), StringToFloat(*Words[4])), StringToInt(*Words[5]));
        }
        else if(*Words[0] == "setBgColor")
        {
            BgColor = Color(StringToInt(*Words[1]), StringToInt(*Words[2]), StringToInt(*Words[3]));
        }
        else if(*Words[0] == "setVignetteColor")
        {
            VignetteColor = Color(StringToInt(*Words[1]), StringToInt(*Words[2]), StringToInt(*Words[3]));
            VignetteChanged = true;
        }
        else if(*Words[0] == "setGravity")
        {
            Gravity = StringToFloat(*Words[1]) * 100;
        }
    }

    void LevelToCode()
    {
        LevelLines.Clear();

        LevelLines.AddNode("@LevelObjects");

        LevelLines.AddNode("setBgColor " + ToString((int)BgColor.r) + " " + ToString((int)BgColor.g) + " " + ToString((int)BgColor.b));
        LevelLines.AddNode("setVignetteColor " + ToString((int)VignetteColor.r) + " " + ToString((int)VignetteColor.g) + " " + ToString((int)VignetteColor.b));
        LevelLines.AddNode("setGravity " + ToString(Gravity/100));

        for(int i = 0; i < Images.GetSize(); i++)
        {
            LevelLines.AddNode("addImage " + Images[i]->Name + " " + Images[i]->FileName);
        }

        for(int i = 0; i < Animations.GetSize(); i++)
        {
            LevelLines.AddNode("addAnimation " + Animations[i]->Name + " " + Animations[i]->FileName);
        }

        for(int i = 0; i < SoundBuffers.GetSize(); i++)
        {
            LevelLines.AddNode("addSound " + SoundBuffers[i]->Name + " " + SoundBuffers[i]->FileName);
        }

        for(int i = 0; i < MusicTracks.GetSize(); i++)
        {
            LevelLines.AddNode("addMusicTrack " + MusicTracks[i]->Name + " " + MusicTracks[i]->FileName);
        }

        for(int i = 0; i < Playlist.DefaultList.GetSize(); i++)
        {
            LevelLines.AddNode("addMusicRequest Music<-" + Playlist.DefaultList[i]->Track->Name
                               + " " + ToString(Playlist.DefaultList[i]->Priority)
                               + " " + ToString(Playlist.DefaultList[i]->LoopTimes));
        }

        for(int i = 0; i < Solids.GetSize(); i++)
        {
            LevelLines.AddNode("addSolid " + Solids[i]->Name + " Image<-" + Solids[i]->Object.ImageName +
                               " " + ToString(Solids[i]->Object.Position.x) + " " + ToString(Solids[i]->Object.Position.y));
        }

        for(int i = 0; i < Artifacts.GetSize(); i++)
        {
            LevelLines.AddNode("addArtifact " + Artifacts[i]->Name + (Artifacts[i]->Object.sprite.isAnimation?" Animation<-":" Image<-") + Artifacts[i]->Object.ImageName +
                               " " + ToString(Artifacts[i]->Object.Position.x) + " " + ToString(Artifacts[i]->Object.Position.y) +
                               " " + ToString(Artifacts[i]->Object.sprite.rotation) + " " + ToString(Artifacts[i]->Object.sprite.depth));
        }

        for(int i = 0; i < SpawnPoints.GetSize(); i++)
        {
            LevelLines.AddNode("addSpawnPoint " + ToString(SpawnPoints[i]->Position.x) + " " + ToString(SpawnPoints[i]->Position.y));
        }

        for(int i = 0; i < EdgeBoxes.GetSize(); i++)
        {
            LevelLines.AddNode("addEdgeBox " + ToString(EdgeBoxes[i]->Position.x) + " " + ToString(EdgeBoxes[i]->Position.y) +
                               " " + ToString(EdgeBoxes[i]->hasWall) + " " + ToString(EdgeBoxes[i]->fromRight));
        }

        for(int i = 0; i < ActionBoxes.GetSize(); i++)
        {
            LevelLines.AddNode("addActionBox " + ActionBoxes[i]->Name + " " +
                               ToString(ActionBoxes[i]->Object.UpperLeft.x) + " " + ToString(ActionBoxes[i]->Object.UpperLeft.y) + " " +
                               ToString(ActionBoxes[i]->Object.LowerRight.x) + " " + ToString(ActionBoxes[i]->Object.LowerRight.y));
        }

        if(Editor)
        for(int i = 0; i < AIs.GetSize(); i++)
        {
            LevelLines.AddNode("addAI " + AIs[i]->Name + (AIs[i]->Type?" Unsueable ":" CorpPeople ") +
                               ToString(AIs[i]->Position.x) + " " + ToString(AIs[i]->Position.y) + " " + ToString(AIs[i]->Direction));
        }
        else
        for(int i = 0; i < CPAIs.GetSize(); i++)
        {
            LevelLines.AddNode("addAI " + CPAIs[i]->Name + (CPAIs[i]->Unsueable?" Unsueable ":" CorpPeople ") +
                               ToString(CPAIs[i]->Position.x) + " " + ToString(CPAIs[i]->Position.y) + " " + ToString(CPAIs[i]->Direction));
        }
    }

    void Save(string Name, Application & App)
    {
        Vector2f viewPos = App().GetDefaultView().GetCenter();
        App().GetDefaultView().SetCenter(0, 0);
        MoveView(-viewPos);

        LevelToCode();

        ofstream fout;
        fout.open(string("Data/Level/" + Name + ".lvl").c_str());

        for(int i = 0; i < LevelLines.GetSize(); i++)
        {
            fout << *LevelLines[i] << endl;
        }

        fout << "\n@Script\n";

        if(ScriptLines.GetSize() > 1 && *ScriptLines[ScriptLines.GetSize()-1] == "")
        ScriptLines.RemoveNode(ScriptLines.GetSize()-1);

        for(int i = 0; i < ScriptLines.GetSize(); i++)
        {
            fout << *ScriptLines[i] << endl;
        }

        fout.close();

        App().GetDefaultView().SetCenter(viewPos);
        MoveView(viewPos);
    }
};

#endif
