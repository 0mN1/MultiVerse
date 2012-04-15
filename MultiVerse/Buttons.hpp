#ifndef __BUTTON_CLASSES_HPP__
#define __BUTTON_CLASSES_HPP__
//#pragma once
#include "SFMLfunctions.hpp"
#include <string>

class Button
{
    protected:
    Image image[2];
    Sprite sprite[2];
    int index;
    Vector2f Position;

    public:
    Button(Vector2f _pos, string _fileName1, string _fileName2)
    {
        Position = _pos;
        index = 0;

        for(int i = 0; i < 2; i++)
        {
            sprite[i] = LoadSprite(image[i], (!i)?_fileName1:_fileName2);
            sprite[i].SetPosition(Position);
        }
    }

    bool Click(Vector2f mousePos, bool leftclicked)
    {
        if(mousePos.x > Position.x && mousePos.x < Position.x + sprite[index].GetSize().x
        && mousePos.y > Position.y && mousePos.y < Position.y + sprite[index].GetSize().y)
        {
            index = 1;

            if(leftclicked)
            return true;
        }
        else
        {
            index = 0;
        }

        return false;
    }

    void Draw(RenderWindow & Target)
    {
        Target.Draw(sprite[index]);
    }
};

class OnOffButton
{
      protected:
                Sprite sprite[4];
                int index;
                int x;
                int y;
                bool * B;
      public:
             OnOffButton(bool & tB,Image & tOn1,Image & tOn2,Image & tOff1,Image & tOff2,int tx,int ty)
             {
                 sprite[0] = Sprite(tOn1);
                 sprite[1] = Sprite(tOn2);
                 sprite[2] = Sprite(tOff1);
                 sprite[3] = Sprite(tOff2);
                 x = tx;
                 y = ty;
                 B = &tB;

                 if(*B)
                 index = 0;
                 else
                 index = 2;

                 for(int i = 0;i<4;i++)
                 sprite[i].SetX(x-sprite[i].GetSize().x/2),
                 sprite[i].SetY(y-sprite[i].GetSize().y/2);
             }

             bool Click(int x1,int y1,bool leftclicked)
             {
                  if(x1>x-sprite[index].GetSize().x/2 && x1<x+sprite[index].GetSize().x/2
                  && y1>y-sprite[index].GetSize().y/2 && y1<y+sprite[index].GetSize().y/2)
                  {
                      if(*B)
                      index = 1;
                      else
                      index = 3;
                  }
                  else
                  {
                      if(*B)
                      index = 0;
                      else
                      index = 2;
                  }

                  if(leftclicked && x1>x-sprite[index].GetSize().x/2 && x1<x+sprite[index].GetSize().x/2
                  && y1>y-sprite[index].GetSize().y/2 && y1<y+sprite[index].GetSize().y/2)return true;

                  return false;
             }

             bool SwitchB()
             {
                 return *B = !*B;
             }

             void Draw(RenderWindow & Target)
             {
                 Target.Draw(sprite[index]);
             }
};

class ToggleButton
{
      protected:
                Sprite sprite[2];
                int index;
                int x;
                int y;
      public:
             ToggleButton(Sprite tsprite0,Sprite tsprite1,int tx,int ty)
             {sprite[0]=tsprite0;sprite[1]=tsprite1;x=tx;y=ty;index=0;
             sprite[0].SetX(x-sprite[0].GetSize().x/2);
             sprite[0].SetY(y-sprite[0].GetSize().y/2);
             sprite[1].SetX(x-sprite[1].GetSize().x/2);
             sprite[1].SetY(y-sprite[1].GetSize().y/2);}

             bool click(int x1,int y1,bool leftclicked)
             {
                  if(leftclicked && x1>x-sprite[0].GetSize().x/2 && x1<x+sprite[0].GetSize().x/2
                  && y1>y-sprite[0].GetSize().y/2 && y1<y+sprite[0].GetSize().y/2)return true;

                  return false;
             }

             void draw(RenderWindow & Target)
             {Target.Draw(sprite[index]);}

             void setindex(int n)
             {index = n;}
};

class TextButton
{
      protected:
                Sprite sprite;
                String text;
                int x;
                int y;
      public:
             TextButton(Sprite tsprite,string txt,Font font,int tx,int ty)
             {sprite=tsprite;text.SetFont(font);text.SetText(txt);
             text.SetPosition(10.f,0.f);x=tx;y=ty;}

             bool click(int x1,int y1,bool leftclicked)
             {
                  if(leftclicked && x1>x-sprite.GetSize().x/2 && x1<x+sprite.GetSize().x/2
                  && y1>y-sprite.GetSize().y/2 && y1<y+sprite.GetSize().y/2)return true;

                  return false;
             }

             void draw(RenderWindow & Target)
             {Target.Draw(sprite);
             Target.Draw(text);}
};

#endif
