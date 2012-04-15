#ifndef __SFMLFUNCTIONS_HPP__
#define __SFMLFUNCTIONS_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "std_string-functions.hpp"

#define Max(a, b) (((a) > (b)) ? (a): (b))
#define Min(a, b) (((a) < (b)) ? (a): (b))

#define Max3(a, b, c) ((((a) > (b)) ? (a): (b)) > c ?(((a) > (b)) ? (a): (b)): c)
#define Min3(a, b, c) ((((a) < (b)) ? (a): (b)) < c ?(((a) > (b)) ? (a): (b)): c)

#define PI 3.1415926535898
#define DPR 57.29577951

using namespace std;
using namespace sf;

class Meter
{
    protected:
    float * Number;
    float MaxNumber;
    Shape Shape1;
    Shape Shape2;
    Vector2f Position;
    Vector2f Size;
    Color Color1;
    Color Color2;
    Color Color3;

    public:
    Meter(float * Pointer = NULL, Vector2f Pos = Vector2f(0, 0), Vector2f Sz = Vector2f(200, 20),
    Color Col1 = Color(0, 255, 0, 150), Color Col2 = Color(0, 200, 0, 150), bool BG = false, Color BGCol = Color(0, 0, 0))
    {
        Number = Pointer;
        MaxNumber = *Number;
        Position = Pos;
        Size = Sz;

        Color1 = Col1;
        Color2 = Col2;
        Color3 = BGCol;

        Shape1 = Shape::Rectangle(Position, Position + Size, Col1);
        if(BG)
        {
            Shape2 = Shape::Rectangle(Vector2f(Position.x, Position.y),
            Vector2f(Position.x + Size.x, Position.y + Size.y), BGCol,2, Col2);
        }
        else
        {
            Shape2 = Shape::Rectangle(Vector2f(Position.x, Position.y),
            Vector2f(Position.x + Size.x, Position.y + Size.y), BGCol, 2, Col2);
            Shape2.EnableFill(false);
        }

    }

    void SetScale(float xScale, float yScale)
    {
        Shape1.SetScale(xScale, yScale);
        Shape2.SetScale(xScale, yScale);
    }

    void Draw(RenderWindow & App)
    {
        Shape1.SetPosition(App.ConvertCoords((int)Position.x, (int)Position.y));
        Shape2.SetPosition(App.ConvertCoords((int)Position.x, (int)Position.y));
        App.Draw(Shape2);
        App.Draw(Shape1);
        Shape1 = Shape::Rectangle(Position, Vector2f(Position.x + Size.x * (*Number / MaxNumber), Position.y + Size.y), Color1);
    }
};

void SetCenterInCenter(Sprite & s)
{
    s.SetCenter(s.GetSize().x/2.f, s.GetSize().y/2.f);
}

Vector2f GetCenter(String & s)
{
    return Vector2f((s.GetRect().Right - s.GetRect().Left)/2.f, (s.GetRect().Bottom - s.GetRect().Top)/2.f);
}

void VertFlip(Image & Pic)
{
    Color Temp;

    for(unsigned int y = 1; y < Pic.GetHeight()/2; y++)
    for(unsigned int x = 1; x < Pic.GetWidth(); x++)
    {
        Temp = Pic.GetPixel(x, y);
        Pic.SetPixel(x, y, Pic.GetPixel(x, Pic.GetHeight() - y));
        Pic.SetPixel(x, Pic.GetHeight() - y, Temp);
    }
}

Vector2f UpperLeft(Sprite & sprite)
{
    return sprite.GetPosition() - sprite.GetCenter();
}

Vector2f LowerRight(Sprite & sprite)
{
    return sprite.GetPosition() + sprite.GetSize() - sprite.GetCenter();
}

Vector2f UpperLeft(View & view)
{
    return Vector2f(view.GetRect().Left, view.GetRect().Top);
}

Vector2f LowerRight(View & view)
{
    return Vector2f(view.GetRect().Right, view.GetRect().Bottom);
}

int Randomi(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

float Randomf(float min, float max)
{
    return (rand() % int((max - min + 1.f) * 1000000.f)) / 1000000.f + min;
}

bool ownCollision(Sprite s1, Sprite s2)
{
    if(s1.GetPosition().x < s2.GetPosition().x + s2.GetSize().x
    && s1.GetPosition().x + s1.GetSize().x > s2.GetPosition().x
    && s1.GetPosition().y < s2.GetPosition().y + s2.GetSize().y
    && s1.GetPosition().y + s1.GetSize().y > s2.GetPosition().y)
    return true;
    else
    return false;
}

bool SimpleCollision(Sprite s1, Sprite s2)
{
    if(s1.GetPosition().x - s1.GetCenter().x < s2.GetPosition().x + s2.GetSize().x  - s2.GetCenter().x
    && s1.GetPosition().x + s1.GetSize().x - s1.GetCenter().x > s2.GetPosition().x - s2.GetCenter().x
    && s1.GetPosition().y - s1.GetCenter().y < s2.GetPosition().y + s2.GetSize().y - s2.GetCenter().y
    && s1.GetPosition().y + s1.GetSize().y - s1.GetCenter().y > s2.GetPosition().y - s2.GetCenter().y)
    return true;
    else
    return false;
}

bool ownCollision(Sprite& s1, Sprite& s2, int n)
{
    if(s1.GetPosition().x < s2.GetPosition().x+s2.GetSize().x
    && s1.GetPosition().x+s1.GetSize().x > s2.GetPosition().x
    && s1.GetPosition().y < s2.GetPosition().y+s2.GetSize().y
    && s1.GetPosition().y+s1.GetSize().y+n > s2.GetPosition().y)
    return true;
    else
    return false;
}

Vector2f Rotate(float Angle, Vector2f Point)
{
    float SinVal = sin(Angle/DPR);
    float CosVal = cos(Angle/DPR);

    Vector2f Rotated;
    Rotated.x = Point.x*CosVal - Point.y*SinVal;
    Rotated.y = Point.y*CosVal + Point.x*SinVal;

    return Rotated;
}
Vector2f RotatePoint(Vector2f Point,Vector2f Origin,float Angle)
{
    Vector2f RotatedPoint = Rotate(Angle,Point-Origin);
    RotatedPoint += Origin;

    return RotatedPoint;
}

bool PPCollisionWR(Sprite Sprite1,Sprite Sprite2,float AlphaLimit = 50.f)
{
    float xmin1 = Sprite1.GetPosition().x - Sprite1.GetCenter().x;
    float ymin1 = Sprite1.GetPosition().y - Sprite1.GetCenter().y;
    float xmin2 = Sprite2.GetPosition().x - Sprite2.GetCenter().x;
    float ymin2 = Sprite2.GetPosition().y - Sprite2.GetCenter().y;

    float xmax1 = xmin1 + Sprite1.GetSize().x;
    float ymax1 = ymin1 + Sprite1.GetSize().y;
    float xmax2 = xmin2 + Sprite2.GetSize().x;
    float ymax2 = ymin2 + Sprite2.GetSize().y;

    float xmin = max(xmin1, xmin2);
    float ymin = max(ymin1, ymin2);
    float xmax = min(xmax1, xmax2);
    float ymax = min(ymax1, ymax2);

    //if(xmax <= xmin || ymax <= ymin) { return false; }

    for(int y = int(ymin);y < ymax;++y)
    {
        for(int x = int(xmin);x < xmax;++x)
        {
            float x1 = x - xmin1, y1 = y - ymin1;
            float x2 = x - xmin2, y2 = y - ymin2;

            Vector2f Point1 =
            RotatePoint(Vector2f(x1,y1),
            Sprite1.GetCenter(),
            -Sprite1.GetRotation());

            Vector2f Point2 =
            RotatePoint(Vector2f(x2,y2),
            Sprite2.GetCenter(),
            -Sprite2.GetRotation());

            if(Point1.x > 0 && Point1.y > 0 && Point2.x > 0 && Point2.y > 0
            && Point1.x < Sprite1.GetSize().x && Point1.y < Sprite1.GetSize().y
            && Point2.x < Sprite2.GetSize().x && Point2.y < Sprite2.GetSize().y)
            {
                Color color1 = Sprite1.GetPixel(int(Point1.x),int(Point1.y));
                Color color2 = Sprite2.GetPixel(int(Point2.x),int(Point2.y));

                if (color1.a >= AlphaLimit && color2.a >= AlphaLimit)
                {
                        return true;
                }
            }
        }
    }

    return false;
}

bool PPCollision(Vector2f point, Sprite & s, float AlphaLimit = 20.f)
{
    point -= s.GetPosition() - s.GetCenter();

    if(point.x >= 0 && point.x < s.GetSize().x
    && point.y >= 0 && point.y < s.GetSize().y
    && s.GetPixel((int)point.x, (int)point.y).a > AlphaLimit)
    return true;
    else
    return false;
}

bool PPCollision(Sprite Sprite1,Sprite Sprite2,float AlphaLimit = 50.f)
{
    float xmin1 = Sprite1.GetPosition().x - Sprite1.GetCenter().x;
    float ymin1 = Sprite1.GetPosition().y - Sprite1.GetCenter().y;
    float xmin2 = Sprite2.GetPosition().x - Sprite2.GetCenter().x;
    float ymin2 = Sprite2.GetPosition().y - Sprite2.GetCenter().y;

    float xmax1 = xmin1 + Sprite1.GetSize().x;
    float ymax1 = ymin1 + Sprite1.GetSize().y;
    float xmax2 = xmin2 + Sprite2.GetSize().x;
    float ymax2 = ymin2 + Sprite2.GetSize().y;

    float xmin = max(xmin1, xmin2);
    float ymin = max(ymin1, ymin2);
    float xmax = min(xmax1, xmax2);
    float ymax = min(ymax1, ymax2);

    if(xmax <= xmin || ymax <= ymin) { return false; }

    for(int y = int(ymin); y < ymax; y += 5)
    {
        for(int x = int(xmin); x < xmax; x += 5)
        {
            float x1 = x - xmin1, y1 = y - ymin1;
            float x2 = x - xmin2, y2 = y - ymin2;

            if(x1 > 0 && y1 > 0 && x2 > 0 && y2 > 0
            && x1 < Sprite1.GetSize().x && y1 < Sprite1.GetSize().y
            && x2 < Sprite2.GetSize().x && y2 < Sprite2.GetSize().y)
            {
                Color color1 = Sprite1.GetPixel(int(x1),int(y1));
                Color color2 = Sprite2.GetPixel(int(x2),int(y2));

                if (color1.a >= AlphaLimit && color2.a >= AlphaLimit)
                {
                        return true;
                }
            }
        }
    }

    return false;
}

/*Sound LoadSound(SoundBuffer & Buffer,string name)
{
    if (!Buffer.LoadFromFile(name))
        return Sound();

    return Sound(Buffer);
}*/

Sprite LoadSprite(Image & Buffer,string name)
{
    if (!Buffer.LoadFromFile(name))
        return Sprite();

    return Sprite(Buffer);
}

Vector2i Res;

class KeyButton
{
    protected:
    Key::Code KeyCode;
    bool Last;
    bool Current;

    public:
    bool Pressed;
    bool Released;
    bool Hold;
    bool Untouched;
    bool ShortHold;
    float Timer;

    KeyButton(Key::Code tkc = Key::Code(0))
    {
        KeyCode = tkc;
        Current = Last = Pressed
        = Released = false;
    }

    void Update(RenderWindow & Target)
    {
        Last = Current;
        Current = Target.GetInput().IsKeyDown(KeyCode);
        Pressed = (Current && !Last);
        Released = (!Current && Last);
        Hold = (Current && Last);
        Untouched = (!Current && !Last);

        if(Pressed)
        {
            Timer = 0.f;
            ShortHold = true;
        }
        else
        if(Hold && Timer < 0.15f)
        {
            Timer += Target.GetFrameTime();
        }
        else
        if(ShortHold == true)
        {
            ShortHold = false;
        }
    }

    bool operator()()
    {
        return (Pressed || Hold);
    }
};

class MouseButton
{
    protected:
    Mouse::Button Mousebutton;
    bool Last;
    bool Current;

    public:
    bool Pressed;
    bool Released;
    bool Hold;
    bool Untouched;

    MouseButton(Mouse::Button tmb)
    {
        Mousebutton=tmb;
        Current=Last=
        Pressed=Released=false;
    }

    void Update(RenderWindow & Target)
    {
        Last = Current;
        Current = Target.GetInput().IsMouseButtonDown(Mousebutton);
        Pressed = (Current && !Last);
        Released = (!Current && Last);
        Hold = (Current && Last);
        Untouched = (!Current && !Last);
    }

    bool operator()()
    {
        return (Pressed || Hold);
    }
};

class c_Cursor
{
    protected:
    Image CursorImage;
    Sprite CursorSprite;
    bool OwnCursor;
    bool DefaultVisible;
    Shape RectangleShape;
    Color Col1;
    Color Col2;
    Vector2f Pos1;
    bool Keep;
    bool ShowCursor;

    public:
    MouseButton Left;
    MouseButton Right;
    Vector2f Position;
    Vector2f PrevPosition;
    Vector2f RelativePosition;
    Vector2f PrevRelativePosition;

    bool ScrollUp;
    bool ScrollDown;

    c_Cursor(Color tCol = Color(0,255,0))
    : Left(Mouse::Left),Right(Mouse::Right)
    {
        Keep = false;
        Col1 = tCol;
        Col1.a = 200;
        Col2 = tCol;
        Col2.a = 50;

        OwnCursor = false;
        DefaultVisible = true;
        ShowCursor = true;
    }

    void SetScale(float Scale)
    {
        if(OwnCursor)
        CursorSprite.SetScale(Scale, Scale);
    }

    void SetCursorImage(string FileName, Vector2f Center = Vector2f(-500, -500))
    {
        OwnCursor = true;
        CursorSprite = LoadSprite(CursorImage, FileName);
        if(Center == Vector2f(-500, -500))
        CursorSprite.SetCenter(CursorSprite.GetSize().x/2, CursorSprite.GetSize().y/2);
        else
        CursorSprite.SetCenter(Center);
    }

    void SetKeep(bool tKeep)
    {
        Keep = tKeep;
    }

    void SetRectangleColor(Color tCol)
    {
        Col1 = tCol;
        Col1.a = 200;
        Col2 = tCol;
        Col2.a = 50;
    }

    void SetRectangleColor(Color tCol1,Color tCol2)
    {
        Col1 = tCol1;
        Col1.a = 200;
        Col2 = tCol2;
        Col2.a = 50;
    }

    void Rectangle(RenderWindow & App)
    {
        if(Left.Pressed)
        Pos1 = Position;

        if(Left.Hold)
        RectangleShape = Shape::Rectangle(Pos1,Position,Col2,1,Col1);

        if(Right.Hold)
        RectangleShape = Shape::Rectangle(Pos1,Position,Col2,1,Col1);

        if(Left.Hold || Keep)
        App.Draw(RectangleShape);
    }

    void Draw(RenderWindow & App)
    {
        if(OwnCursor && ShowCursor)
        {
            CursorSprite.SetPosition(Position);
            App.Draw(CursorSprite);
        }
    }

    friend class Application;
}Cursor;

class Application
{
    protected:
    RenderWindow App;
    KeyButton PauseKey;
    int Screens;
    float TimeWarp;
    float FrameTimeMin;
    float FrameTime;
    Listener listener;
    bool Music;
    bool Sound;
    Vector2i OriginalSize;

    public:
    Application(){}

    Application(int x, int y, int d, string s)
    :App(VideoMode(x,y,d),s)
    {
        Res.x = x;
        Res.y = y;

        srand(time(0));

        PauseKey = KeyButton(Key::Pause);

        ifstream fin;
        fin.open("Screenshots/Screens.txt");

        if(fin.is_open())
        fin >> Screens;
        else
        Screens = 0;

        fin.close();

        Music = true;
        Sound = true;

        OriginalSize = Vector2i(x, y);

        TimeWarp = 1.f;
        FrameTimeMin = 20.f;
    }

    void SetTimeWarp(float warpTime)
    {
        TimeWarp = warpTime;
    }

    void CenterCursor()
    {
        App.SetCursorPosition(App.GetWidth()/2, App.GetHeight()/2);
    }

    void ShowMouseCursor(bool show)
    {
        if(!Cursor.OwnCursor)
        App.ShowMouseCursor(show);

        Cursor.ShowCursor = show;
    }

    RenderWindow & operator()()
    {return App;}

    bool & GetMusic()
    {return Music;}

    bool & GetSound()
    {return Sound;}

    void SetVolume(float Volume)
    {
        listener.SetGlobalVolume(Volume);
    }

    float GetFrameTime()
    {
        if(1/FrameTime < FrameTimeMin)
        return 1/FrameTimeMin * TimeWarp;
        else
        return FrameTime * TimeWarp;
    }

    float GetActualFrameTime()
    {
        return FrameTime;
    }

    void Update(Color c = Color(0, 0, 0))
    {
        Cursor.PrevRelativePosition = Cursor.RelativePosition;
        Cursor.PrevPosition = Cursor.Position;
        Cursor.RelativePosition.x = App.GetInput().GetMouseX();
        Cursor.RelativePosition.y = App.GetInput().GetMouseY();
        Cursor.Position.x = App.ConvertCoords(App.GetInput().GetMouseX(),App.GetInput().GetMouseY()).x;
        Cursor.Position.y = App.ConvertCoords(App.GetInput().GetMouseX(), App.GetInput().GetMouseY()).y;
        App.Clear(c);
        FrameTime = App.GetFrameTime();
        Cursor.Left.Update(App);
        Cursor.Right.Update(App);
        PauseKey.Update(App);

        if(Cursor.OwnCursor && Cursor.DefaultVisible)
        {
            App.ShowMouseCursor(false);
            Cursor.DefaultVisible = false;
        }
    }

    void Display()
    {
        Cursor.Draw(App);
        App.Display();
        Screenshot(PauseKey.Pressed);
    }

    void Screenshot(bool Pressed)
    {
        if(Pressed)
        {
            Screens++;
            Image Screen = App.Capture();
            Screen.SaveToFile("Screenshots/Screenshot"+ToString(Screens)+".png");
            ofstream fout;
            fout.open("Screenshots/Screens.txt");
            fout << Screens;
            fout.close();
        }
    }

    Vector2i GetOriginalSize()
    {
        return OriginalSize;
    }
};

class Timer
{
    protected:
    float Time;
    float Alarm;
    Application * App;

    public:
    Timer(Application * _App = NULL, float _Alarm = 10.f)
    {
        App = _App;
        Time = 0.f;
        Alarm = _Alarm;
    }

    bool Tick()
    {
        if(App != NULL)
        {
            Time += App->GetFrameTime();

            if(Time >= Alarm)
            {
                Time -= Alarm;
                return true;
            }
        }

        return false;
    }

    float GetAlarm()
    {
        return Alarm;
    }

    void SetAlarm(float New)
    {
        Alarm = New;
    }

    void SetSource(Application * _App)
    {
        App = _App;
    }

    float GetTime()
    {
        return Time;
    }

    void SetTime(float New)
    {
        Time = New;
    }

    void Reset()
    {
        Time = 0.f;
    }
};

bool ownClick(Sprite& s1, bool Click)
{
    if(Click && s1.GetPosition().x < Cursor.Position.x
    && s1.GetPosition().x+s1.GetSize().x > Cursor.Position.x
    && s1.GetPosition().y < Cursor.Position.y
    && s1.GetPosition().y+s1.GetSize().y > Cursor.Position.y)
    return true;
    else
    return false;
}

class Background
{
    protected:
    Image BGImage;
    Sprite BGSprite;

    public:
    Background()
    {

    }

    Background(string FileName)
    {
        BGSprite = LoadSprite(BGImage, FileName);
    }

    void Draw(Application & App)
    {
        App().Draw(BGSprite);
    }

    void SetScale(Vector2f scale)
    {
        BGSprite.SetScale(scale);
    }

    void SetScale(float x, float y)
    {
        BGSprite.SetScale(x, y);
    }

    void SetPosition(Vector2f UpperLeftCorner)
    {
        BGSprite.SetPosition(UpperLeftCorner);
    }

    void SetPosition(float Left, float Upper)
    {
        BGSprite.SetPosition(Left, Upper);
    }

    Sprite & GetSprite()
    {
        return BGSprite;
    }
};

enum MouseMode{UPPER_LEFT,MIDDLE};
class GridCursor
{
      protected:
                Sprite pic;
                MouseMode mode;
                int xgrid;
                int ygrid;
      public:
             GridCursor(){}

             GridCursor(Sprite tpic,int tgrid,MouseMode tmode)
             {pic=tpic;xgrid = ygrid = tgrid;mode = tmode;}

             GridCursor(Sprite tpic,int txgrid,int tygrid,MouseMode tmode)
             {pic=tpic;xgrid = txgrid;ygrid = tygrid;mode = tmode;}

             void SetXGrid(int x)
             {xgrid = x;}

             void SetYGrid(int y)
             {ygrid = y;}

             void SetGrid(int xy)
             {xgrid = ygrid = xy;}

             void SetGrid(Vector2i xy)
             {xgrid = xy.x;
             ygrid = xy.y;}

             Vector2f Draw(RenderWindow& Target)
             {
                  for(float i1 = 0;i1<=10000/xgrid;i1++)
                  for(float i2 = 0;i2<=10000/ygrid;i2++)
                  if(Cursor.Position.x >= (i1*xgrid)-(xgrid/2) && Cursor.Position.x <= (i1*xgrid)+(xgrid/2)
                  && Cursor.Position.y >= (i2*ygrid)-(ygrid/2) && Cursor.Position.y <= (i2*ygrid)+(xgrid/2))
                  {if(mode == MIDDLE)
                  {pic.SetX(i1*xgrid-pic.GetSize().x/2);
                  pic.SetY(i2*ygrid-pic.GetSize().y/2);
                  Target.Draw(pic);
                  return Vector2f(int(i1*xgrid),int(i2*ygrid));}
                  if(mode == UPPER_LEFT){
                  pic.SetX(i1*xgrid);pic.SetY(i2*ygrid);
                  Target.Draw(pic);
                  return Vector2f(int((i1*xgrid)+(xgrid/2)),int((i2*ygrid)+(xgrid/2)));}}

                  Target.Draw(pic);
                  return Vector2f(-5,-5);
             }
};

inline float Distance(float x1,float y1,float x2,float y2)
{
  return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

inline float Distance(Vector2f Pos1,Vector2f Pos2)
{
  return sqrt((Pos2.x-Pos1.x)*(Pos2.x-Pos1.x)+(Pos2.y-Pos1.y)*(Pos2.y-Pos1.y));
}

inline int Distance(Vector2i Pos1,Vector2i Pos2)
{
  return int(sqrt((Pos2.x-Pos1.x)*(Pos2.x-Pos1.x)+(Pos2.y-Pos1.y)*(Pos2.y-Pos1.y)) + 0.5);
}

float Angle(int x1,int y1,int x2,int y2)
{return atan2(y2-y1,x2-x1);}

float Angle(Vector2f Pos1,Vector2f Pos2)
{return atan2(Pos2.y-Pos1.y,Pos2.x-Pos1.x);}

float Angle(Vector2i Pos1,Vector2i Pos2)
{return atan2(Pos2.y-Pos1.y,Pos2.x-Pos1.x);}

Vector2f MoveInAngle(float angle,float speed)
{
    float x = speed * cos(angle);
    float y = speed * sin(angle);
    return Vector2f(x,y);
}
/*
bool IsEmpty(Vector2i Pos,int Grid);

bool IsEmpty(int x,int y);

bool InLineOfSight(Vector2f fPos1,Vector2f fPos2,int Grid)
{
    Vector2i iPos1;
    iPos1.x = int((fPos1.x) / Grid);
    iPos1.y = int((fPos1.y) / Grid);
    Vector2i iPos2;
    iPos2.x = int((fPos2.x) / Grid);
    iPos2.y = int((fPos2.y) / Grid);

    int Dist = Distance(iPos1,iPos2);
    Vector2f Pos;
    Pos.x = iPos1.x;
    Pos.y = iPos1.y;
    bool InSight = true;
    for(int i=0;i<Dist;i++)
    {
        Pos += MoveInAngle(Angle(iPos1,iPos2),1);
        if(!IsEmpty(int(Pos.x+0.5f),int(Pos.y+0.5f)))
        {
            InSight = false;
            break;
        }
    }

    return InSight;
}
*/
#endif
