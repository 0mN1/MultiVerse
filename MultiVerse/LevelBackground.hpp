#ifndef __LEVELBACKGROUND_HPP__
#define __LEVELBACKGROUND_HPP__

#include "List.hpp"

using namespace LIST;

class BackgroundObject
{
    protected:
    float Distance;
    Image bImage;
    Sprite bSprite;
    Vector2f Origin;
    Vector2f Position;
    Vector2f Speed;
    Vector2f MinCorner;
    Vector2f MaxCorner;

    public:
    BackgroundObject()
    {

    }

    BackgroundObject(string fileName, float distance, Vector2f position, Vector2f speed = Vector2f(0.f, 0.f))
    {
        bSprite = LoadSprite(bImage, fileName);
        Distance = distance;
        Origin = position;
        MinCorner = Vector2f(-1000.f, -1000.f);
        MaxCorner = Vector2f(1000.f, 1000.f);
        Speed = speed;

        bSprite.SetCenter(bSprite.GetSize().x/2, bSprite.GetSize().y/2);
        bSprite.SetPosition(Origin + Position);
    }

    void Move(Vector2f vector)
    {
       Position.x += vector.x;
       Position.y += vector.y;

       Position.x -= vector.x / Distance;
       Position.y -= vector.y / Distance;
    }

    void Update(Application & App)
    {
        /*Position.x += Speed.x / Distance * App.GetFrameTime();
        Position.y += Speed.y / Distance * App.GetFrameTime();

        if(App().ConvertCoords(Position.x, 0).x < MinCorner.x)
        Position.x -= MinCorner.x - MaxCorner.x;
        //if(App().ConvertCoords(0, Position.y).y < MinCorner.y)
        //Position.y = MaxCorner.y;

        if(App().ConvertCoords(Position.x, 0).x > MaxCorner.x)
        Position.x -= MaxCorner.x - MinCorner.x;
        //if(App().ConvertCoords(0, Position.y).y > MaxCorner.y)
        //Position.y = MinCorner.y;*/

        bSprite.SetPosition(Origin + Position);

        App().Draw(bSprite);
    }

    friend class LevelBackground;
};

class LevelBackground
{
    protected:
    List< BackgroundObject > Objects;

    public:
    LevelBackground()
    {

    }

    void AddObject(string fileName, float distance, Vector2f position = Vector2f(0.f, 0.f), Vector2f speed = Vector2f(0.f, 0.f))
    {
        Objects.AddNode(new BackgroundObject(fileName, distance, position, speed));
        SortList();
    }

    void Move(Vector2f vector)
    {
        for(int i = 0; i < Objects.GetSize(); i++)
        {
            Objects[i]->Move(vector);
        }
    }

    void Move(float x, float y)
    {
        Vector2f vector(x, y);
        for(int i = 0; i < Objects.GetSize(); i++)
        {
            Objects[i]->Move(vector);
        }
    }

    void Draw(Application & App)
    {
        for(int i = 0; i < Objects.GetSize(); i++)
        {
            Objects[i]->Update(App);
        }
    }

    void SortList()
    {
        if(Objects.GetSize() > 1)
        {
            for(int j = 1; j < Objects.GetSize(); j++)
            {
                int i = j;

                while(i > 0)
                {
                    if(Objects[i - 1] && Objects[i - 1]->Distance > Objects[i]->Distance)
                    Objects.SwitchNodes(i - 1, i);
                    else
                    break;

                    --i;
                }
            }
        }
    }
};

#endif
