#ifndef __LEVELOBJECTS_HPP__
#define __LEVELOBJECTS_HPP__

class Obstacle
{
    protected:
    Image oImage;
    Sprite oSprite;
    Vector2f Position;

    public:
    Obstacle()
    {}

    Obstacle(string fileName, Vector2f pos)
    {
        oSprite = LoadSprite(oImage, fileName);
        oSprite.SetCenter(oSprite.GetSize().x/2.f, oSprite.GetSize().y/2.f);
        Position = pos;
    }

    Vector2f GetSize()
    {
        return oSprite.GetSize();
    }

    Sprite & GetSprite()
    {
        return oSprite;
    }

    Vector2f GetPosition()
    {
        return oSprite.GetPosition();
    }

    void Draw(Application & App)
    {
        oSprite.SetPosition(Position);
        App().Draw(oSprite);
    }
};

#endif
