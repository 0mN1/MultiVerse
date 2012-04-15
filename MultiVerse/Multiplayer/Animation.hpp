#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include "SFMLFunctions.hpp"

class AnimImage
{
    protected:
    int Size;
    float FPS;
    Image * Frames;

    public:
    AnimImage()
    {
        Size = 1;
        FPS = 30;
        Frames = new Image[Size];
    }

    ~AnimImage()
    {
        delete [] Frames;
    }

    AnimImage * Load(string Location, bool fromBlender = false)
    {
        string AnimFile = Location + ((Location[Location.size()-1] != '/') ? (Location += '/') : (Location = Location))  + "AnimationInfo.txt";
        ifstream fin;
        fin.open(AnimFile.c_str());

        if(fin.is_open())
        {
            fin >> Size >> FPS;

            delete [] Frames;
            Frames = new Image[Size];

            for(int i = 0; i < Size; i++)
            {
                string FrameName;
                if(fromBlender)
                FrameName = Location + "Frames/" + string((i<9)?"000":((i<99)?"00":"0")) + ToString(i+1) + ".png";
                else
                FrameName = Location + "Frames/" + ToString(i+1) + ".png";

                Frames[i].LoadFromFile(FrameName);
            }

            fin.close();
            return this;
        }

        fin.close();
        return NULL;
    }

    void CreateFromImage(Image & image, Vector2f size, int n, float fps)
    {
        Size = n;
        FPS = fps;

        delete [] Frames;
        Frames = new Image[Size];

        for(int i = 0; i < Size; i++)
        {
            Frames[i].Create((unsigned int)size.x, (unsigned int)size.y);

            for(int y = 0; y < size.y; y++)
            for(int x = 0; x < size.x; x++)
            {
                Frames[i].SetPixel(x, y, image.GetPixel((unsigned int)(size.x * i + x), (unsigned int)y));
            }
        }
    }

    AnimImage * LoadFromImage(string Location)
    {
        string AnimFile = Location + ((Location[Location.size()-1] != '/') ? (Location += '/') : (Location = Location))  + "AnimationInfo.txt";
        ifstream fin;
        fin.open(AnimFile.c_str());

        if(fin.is_open())
        {
            int n;
            float fps;
            Vector2f size;
            Image image;

            fin >> n >> fps >> size.x >> size.y;

            image.LoadFromFile(string(Location + "AS.png").c_str());

            CreateFromImage(image, size, n, fps);

            fin.close();
            return this;
        }

        fin.close();
        return NULL;
    }

    friend class AnimSprite;
};

class AnimSprite
{
    protected:
    AnimImage * Source;
    Sprite * Frames;
    float Timer;
    bool Playing;
    bool Looping;

    Vector2f Position;
    float Rotation;
    Vector2f Scale;
    Vector2f Center;
    float Speed;

    public:
    AnimSprite()
    {
        Frames = new Sprite[1];
        Timer = 0.f;
        Playing = true;
        Looping = true;

        Rotation = 0.f;
        Scale = Vector2f(1.f, 1.f);
        Speed = 1.f;
    }

    AnimSprite(AnimImage * _Source)
    {
        Source = _Source;
        Frames = new Sprite[Source->Size];
        Timer = 0.f;
        Playing = true;
        Looping = true;

        Rotation = 0.f;
        Scale = Vector2f(1.f, 1.f);
        Speed = 1.f;

        for(int i = 0; i < Source->Size; i++)
        {
            Frames[i] = Source->Frames[i];
        }

        Center = Vector2f(Frames[0].GetSize().x/2, Frames[0].GetSize().y/2);
    }

    void SetSource(AnimImage * _Source)
    {
        Source = _Source;

        delete [] Frames;
        Frames = new Sprite[Source->Size];

        for(int i = 0; i < Source->Size; i++)
        {
            Frames[i] = Source->Frames[i];
            Frames[i].SetCenter(Frames[i].GetSize().x/2, Frames[i].GetSize().y/2);
        }
    }

    ~AnimSprite()
    {
        Source = NULL;

        delete [] Frames;
        Frames = NULL;
    }

    inline void SetPosition     (Vector2f _Position)        {Position = _Position;}
    inline void SetPosition     (float _XPos, float _YPos)  {Position.x = _XPos; Position.y = _YPos;}
    inline void SetRotation     (float _Rotation)           {Rotation = _Rotation;}
    inline void SetScale        (float _Scale)              {Scale = Vector2f(_Scale, _Scale);}
    inline void SetScale        (Vector2f _Scale)           {Scale = _Scale;}
    inline void SetCenter       (Vector2f _Center)          {Center = _Center;}
    inline void SetCenter       (float _XPos, float _YPos)  {Center.x = _XPos; Center.y = _YPos;}
    inline void SetSpeed        (float _Speed)              {Speed = _Speed;}
    inline void SetLooping      (bool _Looping)             {Looping = _Looping;}
    inline void SetFrame        (float _Offset)             {Timer = _Offset;}

    inline Vector2f    GetPosition()        {return Position;}
    inline Vector2f    GetCenter()          {return Center;}
    inline Sprite&     GetCurrentFrame()    {return Frames[(int)Timer];}
    inline float       GetRotation()        {return Rotation;}
    inline Vector2f    GetScale()        {return Scale;}
    inline float       GetSpeed()           {return Speed;}
    inline bool        GetLooping()         {return Looping;}
    inline bool        GetPlaying()         {return Playing;}

    inline void Play()     {Playing = true;}
    inline void Pause()    {Playing = false;}

    void Draw(Application & App)
    {
        if(Playing)
        {
            Timer += Speed * Source->FPS * App.GetFrameTime();

            if(Looping)
            {
                while(Timer >= Source->Size)
                Timer -= Source->Size;
            }
            else
            {
                if(Timer >= Source->Size)
                Timer = Source->Size-1;

                Playing = false;
            }

            if(Timer < 0.f)
            Timer = 0.f;
        }

        Frames[(int)Timer].SetPosition(Position);
        Frames[(int)Timer].SetRotation(Rotation);
        Frames[(int)Timer].SetScale(Scale);
        Frames[(int)Timer].SetCenter(Center);

        App().Draw(Frames[(int)Timer]);
    }
};

#endif
