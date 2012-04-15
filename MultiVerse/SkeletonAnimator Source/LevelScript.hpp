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

class Level
{
    public:
    ImageList Images;
    List< ScriptLoadObject< AnimImage > > Animations;
    List< ScriptObject< bool > > Booleans;
    List< ScriptObject< float > > Floats;
    List< ScriptObject< Artifact > > Artifacts;
    List< ScriptObject< Solid > > Solids;
    List< string > LevelCode;
    List< List< string > > ScriptCode;

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
    }

    void AddArtifact(string Name, string ImageName, Vector2f position)
    {
        Artifacts.AddNode(ScriptObject< Artifact >());
        Artifacts[Artifacts.GetSize() - 1]->Name = Name;
        Artifacts[Artifacts.GetSize() - 1]->Object.ImageName = ImageName;
        Artifacts[Artifacts.GetSize() - 1]->Object.Position = position;

        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.sprite.SetImage(Images.Find(ImageName)->Content);
        SetCenterInCenter(Artifacts[Artifacts.GetSize() - 1]->Object.sprite.sprite);
        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.sprite.SetPosition(position);
        Artifacts[Artifacts.GetSize() - 1]->Object.sprite.position = &Artifacts[Artifacts.GetSize() - 1]->Object.Position;
    }

    void AddMusic(string Name, string FileName);

    void LevelToCode()
    {

    }

    void Save(string Name)
    {
        ofstream fout;
        fout.open(string(Name + ".GWlvl").c_str());

        fout << "# Level Objects and Variables\n";

        for(int i = 0; i < Images.GetSize(); i++)
        {
            fout << "addImage " << Images[i]->Name << " " << Images[i]->FileName << endl;
        }

        for(int i = 0; i < Solids.GetSize(); i++)
        {
            fout << "addSolid " << Solids[i]->Name << " Image<-" << Solids[i]->Object.ImageName << " "
                 << Solids[i]->Object.Position.x << " " << Solids[i]->Object.Position.y << endl;
        }

        for(int i = 0; i < Artifacts.GetSize(); i++)
        {
            fout << "addArtifact " << Artifacts[i]->Name << " Image<-" << Artifacts[i]->Object.ImageName << " "
                 << Artifacts[i]->Object.Position.x << " " << Artifacts[i]->Object.Position.y << endl;
        }

        fout << "\n# Put Script Here #";
        fout << "\n#  |   |   |   |  #";
        fout << "\n#  |   |   |   |  #";
        fout << "\n#  V   V   V   V  #" << endl;

        fout.close();
    }
};

#endif
