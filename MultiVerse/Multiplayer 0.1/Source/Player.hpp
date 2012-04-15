#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "LevelObjects.hpp"
#include "LevelBackground.hpp"

class PlayerClass
{
    protected:
    Image pImage[2];
    Sprite pSprite[2];
    Image MaskImage;
    Sprite Mask;
    Vector2f Position, PrevPosition;
    Vector2f Momentum;
    bool Direction;
    bool canJump;
    String NameSign;
    bool ShowNameSign;
    bool GravityOn;

    public:
    PlayerClass()
    {
        Position = Vector2f(400.f, 300.f);
        Momentum = Vector2f(0.f, 0.f);
        Direction = 1;
        canJump = true;
        ShowNameSign = false;
        GravityOn = true;
    }

    void SetName(string name)
    {
        NameSign.SetText(name);
        NameSign.SetCenter(GetCenter(NameSign));
        ShowNameSign = true;
    }

    void SetSprite(int characterImage)
    {
        if(characterImage == -1)
        {
            if(Randomi(0, 2))
            {
                pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy01l.png");
                pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy01r.png");
            }
            else
            {
                pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy02l.png");
                pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy02r.png");
            }
        }
        else if(characterImage == 0)
        {
            pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy01l.png");
            pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy01r.png");
        }
        else if(characterImage == 1)
        {
            pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy02l.png");
            pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy02r.png");
        }

        Mask = LoadSprite(MaskImage, "Data/Image/Mask.png");
        SetCenterInCenter(Mask);

        for(int i = 0; i < 2; i++)
        SetCenterInCenter(pSprite[i]);
    }

    bool GetDirection()
    {
        return Direction;
    }

    void SetDirection(bool d)
    {
        Direction = d;
    }

    Vector2f GetPosition()
    {
        return Position;
    }

    void SetPosition(Vector2f p)
    {
        Position = p;
    }

    void Update(Application & App, LevelBackground & lBg, List< Obstacle > & Ob)
    {
        if(GUI::AKey() || GUI::LeftKey())
        {
            Momentum.x = -200;
            Direction = 0;
        }

        if(GUI::DKey() || GUI::RightKey())
        {
            Momentum.x = 200;
            Direction = 1;
        }

        if(GUI::DKey.Untouched && GUI::RightKey.Untouched
        && GUI::AKey.Untouched && GUI::LeftKey.Untouched)
        {
            Momentum.x = 0;
        }

        if((GUI::WKey.Pressed || GUI::UpKey.Pressed
        || GUI::SpaceKey.Pressed))// && canJump)
        {
//            bool yMomentum = Momentum.y;
            Momentum.y = -500;

//            if(yMomentum)
//            Position.y -= 2.f;

            canJump = false;
        }

        PrevPosition = Position;

        Position.x += Momentum.x * App.GetFrameTime();

        Mask.SetPosition(Position);

        for(int i = 0; i < Ob.GetSize(); i++)
        {
            if(Momentum.x && SimpleCollision(Mask, Ob[i]->GetSprite()))
            {
                bool collision = false;
                int spot = 0;

                for(int j = 0; j < Mask.GetSize().x/2 + 1; j++)
                {
                    if(PPCollision(Vector2f(Position.x - j, Position.y + Mask.GetSize().y/2), Ob[i]->GetSprite()))
                    {
                        collision = true;
                        spot = -j;
                        break;
                    }

                    if(PPCollision(Vector2f(Position.x + j, Position.y + Mask.GetSize().y/2), Ob[i]->GetSprite()))
                    {
                        collision = true;
                        spot = j;
                        break;
                    }
                }

                if(collision)
                {
                    bool flag = false;
                    int height = 0;

                    for(int j = 1; j <= 7; j += 1)
                    {
                        if(!PPCollision(Vector2f(Position.x + spot, Position.y + (Mask.GetSize().y/2 - j)), Ob[i]->GetSprite()))
                        {
                            flag = true;
                            height = j;
                            break;
                        }
                    }

                    if(flag && Momentum.y >= 0.f)
                    {
                        Position.y -= height;

                        if((GUI::WKey.Pressed || GUI::UpKey.Pressed
                        || GUI::SpaceKey.Pressed))
                        ;
                        else
                        Momentum.y = 0.f;

                        if(PPCollision(Mask, Ob[i]->GetSprite()))
                        {
                            Position.x = PrevPosition.x;
                            Momentum.x = 0.f;
                        }
                    }
                    else
                    {
                        Position.x = PrevPosition.x;
                        Momentum.x = 0.f;
                    }
                }
                else
                if(PPCollision(Mask, Ob[i]->GetSprite()))
                {
                    Position.x = PrevPosition.x;
                    Momentum.x = 0.f;
                }
            }
        }

        if(GUI::GKey.Pressed)
        {
            GravityOn = !GravityOn;
        }

        if(GravityOn)
        Momentum.y += 982.f * App.GetFrameTime();

        Position.y += Momentum.y * App.GetFrameTime();

        Mask.SetPosition(Position);

        for(int i = 0; i < Ob.GetSize(); i++)
        {
            if(SimpleCollision(Mask, Ob[i]->GetSprite())
            && PPCollision(Mask, Ob[i]->GetSprite()))
            {
                if(Momentum.y > 0)
                canJump = true;

                if(Momentum.y)
                //&& PPCollision(Vector2f(Position.x, Position.y + 57), Ob.GetSprite()))
                Position.y = PrevPosition.y;
                Momentum.y = 0.f;
            }
        }

        /*if(Momentum.y && SimpleCollision(Mask, Ob.GetSprite()))
        {
            if(PPCollision(Vector2f(Position.x, Position.y + 63), Ob.GetSprite())
            || PPCollision(Vector2f(Position.x + 16, Position.y + 63), Ob.GetSprite())
            || PPCollision(Vector2f(Position.x - 16, Position.y + 63), Ob.GetSprite()))
            {
                if(!PPCollision(Vector2f(Position.x, Position.y + 60), Ob.GetSprite())
                || !PPCollision(Vector2f(Position.x, Position.y + 59), Ob.GetSprite())
                || !PPCollision(Vector2f(Position.x, Position.y + 58), Ob.GetSprite())
                || !PPCollision(Vector2f(Position.x, Position.y + 57), Ob.GetSprite()))
                {
                    //Position.y -= 1.f;
                    Momentum.y = 0.f;
                }
                else
                {
                    Position.y = PrevPosition.x;
                    Momentum.y = 0.f;
                }
            }
            else
            if(PPCollision(Mask, Ob.GetSprite()))
            {
                Position.y = PrevPosition.y;
                Momentum.y = 0.f;
            }
        }*/

        MoveView(App().GetDefaultView(), lBg);
    }

    void Draw(Application & App)
    {
        //if(Position == PrevPosition)
        pSprite[Direction].SetPosition(Position);
        //else
        //pSprite[Direction].SetPosition(PrevPosition);

        App().Draw(pSprite[Direction]);

        if(ShowNameSign)
        {
            NameSign.SetPosition(Position + Vector2f(0, -75));
            App().Draw(NameSign);
        }
    }

    void MoveView(View & view, LevelBackground & lBg)
    {
//        if(Position.x > view.GetRect().Right - view.GetHalfSize().x * 0.5)
//        {
//            lBg.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 0.5), 0);
//            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 0.5), 0);
//        }
//        else
//        if(Position.x < view.GetRect().Right - view.GetHalfSize().x * 1.5)
//        {
//            lBg.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 1.5), 0);
//            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 1.5), 0);
//        }
//
//        if(Position.y > view.GetRect().Bottom - view.GetHalfSize().y * 0.5)
//        {
//            lBg.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 0.5));
//            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 0.5));
//        }
//        else
//        if(Position.y < view.GetRect().Bottom - view.GetHalfSize().y * 1.5)
//        {
//            lBg.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 1.5));
//            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 1.5));
//        }

        if(Position.x > view.GetRect().Right - view.GetHalfSize().x)
        {
            lBg.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x), 0);
            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x), 0);
        }
        else
        if(Position.x < view.GetRect().Right - view.GetHalfSize().x)
        {
            lBg.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x), 0);
            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x), 0);
        }

        if(Position.y > view.GetRect().Bottom - view.GetHalfSize().y)
        {
            lBg.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y));
            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y));
        }
        else
        if(Position.y < view.GetRect().Bottom - view.GetHalfSize().y)
        {
            lBg.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y));
            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y));
        }
    }

    friend class CharacterData;
};

#endif
