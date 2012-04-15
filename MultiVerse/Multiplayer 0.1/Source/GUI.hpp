#ifndef __GUI_HPP__
#define __GUI_HPP__

#include "SFMLfunctions.hpp"

namespace GUI
{
    KeyButton AKey(Key::A),BKey(Key::B),CKey(Key::C),DKey(Key::D),EKey(Key::E),
    FKey(Key::F),GKey(Key::G),HKey(Key::H),IKey(Key::I),JKey(Key::J),KKey(Key::K),LKey(Key::L),
    MKey(Key::M),NKey(Key::N),OKey(Key::O),PKey(Key::P),QKey(Key::Q),RKey(Key::R),SKey(Key::S),
    TKey(Key::T),UKey(Key::U),VKey(Key::V),WKey(Key::W),XKey(Key::X),YKey(Key::Y),ZKey(Key::Z),
    Num1Key(Key::Num1),Num2Key(Key::Num2),Num3Key(Key::Num3),Num4Key(Key::Num4),Num5Key(Key::Num5),
    Num6Key(Key::Num6),Num7Key(Key::Num7),Num8Key(Key::Num8),Num9Key(Key::Num9),Num0Key(Key::Num0),
    Numpad1Key(Key::Numpad1),Numpad2Key(Key::Numpad2),Numpad3Key(Key::Numpad3),Numpad4Key(Key::Numpad4),Numpad5Key(Key::Numpad5),
    Numpad6Key(Key::Numpad6),Numpad7Key(Key::Numpad7),Numpad8Key(Key::Numpad8),Numpad9Key(Key::Numpad9),Numpad0Key(Key::Numpad0),
    SpaceKey(Key::Space),LShiftKey(Key::LShift),RShiftKey(Key::RShift),LCtrlKey(Key::LControl),TabKey(Key::Tab),
    PeriodKey(Key::Period),CommaKey(Key::Comma),AddKey(Key::Equal),BackKey(Key::Back),EnterKey(Key::Return),
    DeleteKey(Key::Delete),UpKey(Key::Up),DownKey(Key::Down),LeftKey(Key::Left),RightKey(Key::Right);

    void UpdateKeys(RenderWindow & RW)
    {
        AKey.Update(RW);
        BKey.Update(RW);
        CKey.Update(RW);
        DKey.Update(RW);
        EKey.Update(RW);
        FKey.Update(RW);
        GKey.Update(RW);
        HKey.Update(RW);
        IKey.Update(RW);
        JKey.Update(RW);
        KKey.Update(RW);
        LKey.Update(RW);
        MKey.Update(RW);
        NKey.Update(RW);
        OKey.Update(RW);
        PKey.Update(RW);
        QKey.Update(RW);
        RKey.Update(RW);
        SKey.Update(RW);
        TKey.Update(RW);
        UKey.Update(RW);
        VKey.Update(RW);
        WKey.Update(RW);
        XKey.Update(RW);
        YKey.Update(RW);
        ZKey.Update(RW);

        Num1Key.Update(RW);
        Num2Key.Update(RW);
        Num3Key.Update(RW);
        Num4Key.Update(RW);
        Num5Key.Update(RW);
        Num6Key.Update(RW);
        Num7Key.Update(RW);
        Num8Key.Update(RW);
        Num9Key.Update(RW);
        Num0Key.Update(RW);

        Numpad1Key.Update(RW);
        Numpad2Key.Update(RW);
        Numpad3Key.Update(RW);
        Numpad4Key.Update(RW);
        Numpad5Key.Update(RW);
        Numpad6Key.Update(RW);
        Numpad7Key.Update(RW);
        Numpad8Key.Update(RW);
        Numpad9Key.Update(RW);
        Numpad0Key.Update(RW);

        SpaceKey.Update(RW);
        LShiftKey.Update(RW);
        RShiftKey.Update(RW);
        LCtrlKey.Update(RW);
        TabKey.Update(RW);
        PeriodKey.Update(RW);
        CommaKey.Update(RW);
        AddKey.Update(RW);
        BackKey.Update(RW);
        EnterKey.Update(RW);
        DeleteKey.Update(RW);

        UpKey.Update(RW);
        DownKey.Update(RW);
        LeftKey.Update(RW);
        RightKey.Update(RW);
    }

    class TextBox
    {
        protected:
        bool Active;
        Shape Rect;
        string S1;
        String Str;
        Vector2f Position;
        Vector2f Origin;
        Vector2f ViewPosition;
        Vector2f Size;

        public:
        TextBox(float xPos = 0, float yPos = 0,float xSize = 200,float ySize = 30)
        {
            Origin = Vector2f(xPos, yPos);
            Position = ViewPosition + Origin;
            Size = Vector2f(xSize, ySize);
            Rect = Shape::Rectangle(xPos, yPos, xPos + xSize, yPos + ySize, Color(255, 255, 255), 2, Color(128, 128, 128));
            Active = false;

            Str = String("", Font::GetDefaultFont(), ySize);
            Str.SetPosition(Position.x, Position.y - ySize / 10);
            Str.SetColor(Color(150, 150, 150));
        }

        bool     &  GetActive   ()  {return Active;   }
        Shape    &  GetRect     ()  {return Rect;     }
        string   &  GetString   ()  {return S1;       }
        String   &  GetStr      ()  {return Str;      }
        Vector2f &  GetPosition ()  {return Position; }
        Vector2f &  GetSize     ()  {return Size;     }

        void SetPosition(Vector2f pos) {Origin = pos;}

        void SetS1(string s)
        {
            S1 = s;
            Str.SetText(S1);
        }

        bool & SetActive(bool New)
        {
            if(Active != New && New)
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 255));
                Str.SetColor(Color(0, 0, 0));
                Active = true;
            }
            else
            if(Active != New && !New)
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 128));

                Active = false;

                if(S1.empty())
                {
                    Str.SetColor(Color(150,150,150));
                }
                else
                Str.SetColor(Color(75, 75, 75));
            }

            return Active;
        }

        bool Clicked()
        {
            if(Position.x < Cursor.Position.x && Cursor.Position.x < Position.x + Size.x
            && Position.y < Cursor.Position.y && Cursor.Position.y < Position.y + Size.y)
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 255));

                Str.SetColor(Color(0, 0, 0));
                Active = true;

                return true;
            }
            else
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 128));

                Active = false;

                if(S1.empty())
                {
                    Str.SetColor(Color(150, 150, 150));
                }
                else
                Str.SetColor(Color(75, 75, 75));

                return false;
            }
        }

        bool Write()
        {
            if(Active)
            {
                if(LShiftKey.Hold || RShiftKey.Hold)
                {
                    if(AKey.Pressed)
                    S1 += "A";
                    if(BKey.Pressed)
                    S1 += "B";
                    if(CKey.Pressed)
                    S1 += "C";
                    if(DKey.Pressed)
                    S1 += "D";
                    if(EKey.Pressed)
                    S1 += "E";
                    if(FKey.Pressed)
                    S1 += "F";
                    if(GKey.Pressed)
                    S1 += "G";
                    if(HKey.Pressed)
                    S1 += "H";
                    if(IKey.Pressed)
                    S1 += "I";
                    if(JKey.Pressed)
                    S1 += "J";
                    if(KKey.Pressed)
                    S1 += "K";
                    if(LKey.Pressed)
                    S1 += "L";
                    if(MKey.Pressed)
                    S1 += "M";
                    if(NKey.Pressed)
                    S1 += "N";
                    if(OKey.Pressed)
                    S1 += "O";
                    if(PKey.Pressed)
                    S1 += "P";
                    if(QKey.Pressed)
                    S1 += "Q";
                    if(RKey.Pressed)
                    S1 += "R";
                    if(SKey.Pressed)
                    S1 += "S";
                    if(TKey.Pressed)
                    S1 += "T";
                    if(UKey.Pressed)
                    S1 += "U";
                    if(VKey.Pressed)
                    S1 += "V";
                    if(WKey.Pressed)
                    S1 += "W";
                    if(XKey.Pressed)
                    S1 += "X";
                    if(YKey.Pressed)
                    S1 += "Y";
                    if(ZKey.Pressed)
                    S1 += "Z";

                    if(Num1Key.Pressed)
                    S1 += "!";
                    if(Num2Key.Pressed)
                    S1 += "\"";
                    if(Num3Key.Pressed)
                    S1 +="#";
                    if(Num4Key.Pressed)
                    S1 += "¤";
                    if(Num5Key.Pressed)
                    S1 += "%";
                    if(Num6Key.Pressed)
                    S1 += "&";
                    if(Num7Key.Pressed)
                    S1 +="/";
                    if(Num8Key.Pressed)
                    S1 += "(";
                    if(Num9Key.Pressed)
                    S1 += ")";
                    if(Num0Key.Pressed)
                    S1 += "=";
                    if(SpaceKey.Pressed)
                    S1 +=" ";
                    if(PeriodKey.Pressed)
                    S1 += ":";
                    if(CommaKey.Pressed)
                    S1 += ";";
                    if(AddKey.Pressed)
                    S1 += "?";

                    if(BackKey.Pressed&&!S1.empty())S1.erase();
                }
                else
                {
                    if(AKey.Pressed)
                    S1 += "a";
                    if(BKey.Pressed)
                    S1 += "b";
                    if(CKey.Pressed)
                    S1 += "c";
                    if(DKey.Pressed)
                    S1 += "d";
                    if(EKey.Pressed)
                    S1 += "e";
                    if(FKey.Pressed)
                    S1 += "f";
                    if(GKey.Pressed)
                    S1 += "g";
                    if(HKey.Pressed)
                    S1 += "h";
                    if(IKey.Pressed)
                    S1 += "i";
                    if(JKey.Pressed)
                    S1 += "j";
                    if(KKey.Pressed)
                    S1 += "k";
                    if(LKey.Pressed)
                    S1 += "l";
                    if(MKey.Pressed)
                    S1 += "m";
                    if(NKey.Pressed)
                    S1 += "n";
                    if(OKey.Pressed)
                    S1 += "o";
                    if(PKey.Pressed)
                    S1 += "p";
                    if(QKey.Pressed)
                    S1 += "q";
                    if(RKey.Pressed)
                    S1 += "r";
                    if(SKey.Pressed)
                    S1 += "s";
                    if(TKey.Pressed)
                    S1 += "t";
                    if(UKey.Pressed)
                    S1 += "u";
                    if(VKey.Pressed)
                    S1 += "v";
                    if(WKey.Pressed)
                    S1 += "w";
                    if(XKey.Pressed)
                    S1 += "x";
                    if(YKey.Pressed)
                    S1 += "y";
                    if(ZKey.Pressed)
                    S1 += "z";

                    if(Num1Key.Pressed)
                    S1 += "1";
                    if(Num2Key.Pressed)
                    S1 += "2";
                    if(Num3Key.Pressed)
                    S1 += "3";
                    if(Num4Key.Pressed)
                    S1 += "4";
                    if(Num5Key.Pressed)
                    S1 += "5";
                    if(Num6Key.Pressed)
                    S1 += "6";
                    if(Num7Key.Pressed)
                    S1 += "7";
                    if(Num8Key.Pressed)
                    S1 += "8";
                    if(Num9Key.Pressed)
                    S1 += "9";
                    if(Num0Key.Pressed)
                    S1 += "0";

                    if(SpaceKey.Pressed)
                    S1 += " ";
                    if(PeriodKey.Pressed)
                    S1 += ".";
                    if(CommaKey.Pressed)
                    S1 += ",";
                    if(AddKey.Pressed)
                    S1 += "+";

                    if(BackKey.Pressed&&!S1.empty())S1.erase(--S1.end());
                }

                Str.SetText(S1);

                if(EnterKey.Pressed)
                {
                    for(int i = 0; i < 4; i++)
                    Rect.SetPointOutlineColor(i, Color(128, 128, 128));

                    Active = false;

                    if(S1.empty())
                    {
                        Str.SetColor(Color(150, 150, 150));
                    }
                    else
                    {
                        Str.SetColor(Color(75, 75, 75));
                        return false;
                    }

                    return true;
                }
                return true;
            }
            return true;
        }

        void Draw(Application & App)
        {
            ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                    App().GetDefaultView().GetRect().Top);

            Position = ViewPosition + Origin;

            Rect.SetPosition(ViewPosition);
            Str.SetPosition(Position.x, Position.y - Size.y / 10);

            App().Draw(Rect);
            App().Draw(Str);
        }

        void Draw(Application & App, Vector2f VP)
        {
            ViewPosition = VP;

            Position = ViewPosition + Origin;

            Rect.SetPosition(ViewPosition);
            Str.SetPosition(Position.x, Position.y - Size.y / 10);

            App().Draw(Rect);
            App().Draw(Str);
        }

        friend RenderWindow & operator<<(RenderWindow&, TextBox&);
        friend Application & operator<<(Application&, TextBox&);
    };

    RenderWindow & operator<<(RenderWindow & RW, TextBox & TB)
    {
        RW.Draw(TB.Rect);
        RW.Draw(TB.Str);

        return RW;
    }

    Application & operator<<(Application & App, TextBox & TB)
    {
        App().Draw(TB.Rect);
        App().Draw(TB.Str);

        return App;
    }

    class FloatBox
    {
        protected:
        bool Active;
        Shape Rect;
        string S1;
        String Str;
        Vector2f Position;
        Vector2f Origin;
        Vector2f ViewPosition;
        Vector2f Size;

        public:
        FloatBox(float xPos = 0, float yPos = 0, float xSize = 200, float ySize = 30)
        {
            Origin = Vector2f(xPos, yPos);
            Position = ViewPosition + Origin;
            Size = Vector2f(xSize, ySize);

            Rect = Shape::Rectangle(xPos, yPos, xPos + xSize, yPos + ySize,
            Color(255, 255, 255), 2, Color(128, 128, 128));

            Active = false;

            Str = String("", Font::GetDefaultFont(), ySize);
            Str.SetPosition(xPos, yPos - ySize / 10);
            Str.SetColor(Color(150, 150, 150));
        }

        bool & GetActive(){return Active;}
        Shape & GetRect(){return Rect;}
        float GetValue(){return atof(S1.c_str());}
        String & GetStr(){return Str;}
        Vector2f & GetPosition(){return Position;}
        Vector2f & GetSize(){return Size;}

        bool & SetActive(bool New)
        {
            if(Active != New && New)
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 255));
                Str.SetColor(Color(0, 0, 0));
                Active = true;
            }
            else
            if(Active != New && !New)
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 128));

                Active = false;

                if(S1.empty())
                {
                    Str.SetColor(Color(150, 150, 150));
                }
                else
                Str.SetColor(Color(75, 75, 75));
            }

            return Active;
        }

        bool Clicked()
        {
            if(Position.x < Cursor.Position.x && Cursor.Position.x < Position.x + Size.x
            && Position.y < Cursor.Position.y && Cursor.Position.y < Position.y + Size.y)
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 255));

                Str.SetColor(Color(0, 0, 0));
                Active = true;

                return true;
            }
            else
            {
                for(int i = 0; i < 4; i++)
                Rect.SetPointOutlineColor(i, Color(128, 128, 128));

                Active = false;

                if(S1.empty())
                {
                    Str.SetColor(Color(150,150,150));
                }
                else
                Str.SetColor(Color(75, 75, 75));

                return false;
            }
        }

        bool Write()
        {
            if(Active)
            {
                if(LShiftKey.Hold || RShiftKey.Hold)
                {
                    if(BackKey.Pressed && !S1.empty())
                    S1.erase();
                }
                else
                {
                    if(Num1Key.Pressed)  S1 += "1";
                    if(Num2Key.Pressed)  S1 += "2";
                    if(Num3Key.Pressed)  S1 += "3";
                    if(Num4Key.Pressed)  S1 += "4";
                    if(Num5Key.Pressed)  S1 += "5";
                    if(Num6Key.Pressed)  S1 += "6";
                    if(Num7Key.Pressed)  S1 += "7";
                    if(Num8Key.Pressed)  S1 += "8";
                    if(Num9Key.Pressed)  S1 += "9";
                    if(Num0Key.Pressed)  S1 += "0";
                    if(PeriodKey.Pressed)S1 += ".";
                    if(BackKey.Pressed && !S1.empty())
                    S1.erase(--S1.end());
                }

                Str.SetText(S1);

                if(EnterKey.Pressed)
                {
                    for(int i = 0; i < 4; i++)
                    Rect.SetPointOutlineColor(i, Color(128, 128, 128));

                    Active = false;

                    if(S1.empty())
                    {
                        Str.SetColor(Color(150,150,150));
                    }
                    else
                    {
                        Str.SetColor(Color(75,75,75));
                        return false;
                    }

                    return true;
                }
                return true;
            }
            return true;
        }

        void Draw(Application & App)
        {
            ViewPosition = Vector2f(App().GetDefaultView().GetRect().Left,
                                    App().GetDefaultView().GetRect().Top);

            Position = ViewPosition + Origin;

            Rect.SetPosition(ViewPosition);
            Str.SetPosition(Position.x, Position.y - Size.y / 10);

            App().Draw(Rect);
            App().Draw(Str);
        }

        friend RenderWindow & operator<<(RenderWindow&, FloatBox&);
    };

    RenderWindow & operator<<(RenderWindow & RW,FloatBox & TB)
    {
        RW.Draw(TB.Rect);
        RW.Draw(TB.Str);

        return RW;
    }

    class IntBox
    {
        protected:
        bool Active;
        Shape Rect;
        string S1;
        String Str;
        Vector2f Position;
        Vector2f Size;

        public:
        IntBox(float xPos=0,float yPos=0,float xSize=200,float ySize=30)
        {Position=Vector2f(xPos,yPos);Size=Vector2f(xSize,ySize);
        Rect=Shape::Rectangle(xPos,yPos,xPos+xSize,yPos+ySize,
        Color(255,255,255),2,Color(128,128,128));Active=false;
        Str = String("",Font::GetDefaultFont(),ySize);
        Str.SetPosition(xPos,yPos-ySize/10);Str.SetColor(Color(150,150,150));}

        bool & GetActive(){return Active;}
        Shape & GetRect(){return Rect;}
        int GetValue(){return atoi(S1.c_str());}
        String & GetStr(){return Str;}
        Vector2f & GetPosition(){return Position;}
        Vector2f & GetSize(){return Size;}

        void SetValue(string Value)
        {S1 = Value;
        Str.SetText(S1);}

        bool & SetActive(bool New)
        {
            if(Active!=New && New)
            {for(int i=0;i<4;i++)
            Rect.SetPointOutlineColor(i,Color(128,128,255));
            Str.SetColor(Color(0,0,0));
            Active = true;}
            else if(Active!=New && !New)
            {for(int i=0;i<4;i++)
            Rect.SetPointOutlineColor (i,Color(128,128,128));
            Active = false;
            if(S1.empty())Str.SetText(""),
            Str.SetColor(Color(150,150,150));
            else Str.SetColor(Color(75,75,75));}

            return Active;
        }

        bool Clicked()
        {if(Position.x<Cursor.Position.x && Cursor.Position.x<Position.x+Size.x
         && Position.y<Cursor.Position.y && Cursor.Position.y<Position.y+Size.y)
            {
                for(int i=0;i<4;i++)
                Rect.SetPointOutlineColor(i,Color(128,128,255));
                Str.SetColor(Color(0,0,0));
                Active = true;
                return true;
            }
        else
            {
                for(int i=0;i<4;i++)
                Rect.SetPointOutlineColor (i,Color(128,128,128));
                Active = false;
                if(S1.empty())Str.SetText(""),
                Str.SetColor(Color(150,150,150));
                else Str.SetColor(Color(75,75,75));
                return false;
            }
        }

        bool Write()
        {

            if(Active)
            {
                if(LShiftKey.Hold || RShiftKey.Hold)
                {if(BackKey.Pressed&&!S1.empty())S1.erase();}
                else
                {if(Num1Key.Pressed)S1+="1";if(Num2Key.Pressed)S1+="2";
                if(Num3Key.Pressed)S1+="3";if(Num4Key.Pressed)S1+="4";
                if(Num5Key.Pressed)S1+="5";if(Num6Key.Pressed)S1+="6";
                if(Num7Key.Pressed)S1+="7";if(Num8Key.Pressed)S1+="8";
                if(Num9Key.Pressed)S1+="9";if(Num0Key.Pressed)S1+="0";
                if(BackKey.Pressed&&!S1.empty())S1.erase(--S1.end());}

                Str.SetText(S1);

                if(EnterKey.Pressed)
                {
                    for(int i=0;i<4;i++)
                    Rect.SetPointOutlineColor (i,Color(128,128,128));
                    Active = false;
                    if(S1.empty())Str.SetText(""),
                    Str.SetColor(Color(150,150,150));
                    else
                    {Str.SetColor(Color(75,75,75));
                    return false;}
                    return true;
                }
                return true;
            }
            return true;
        }

        friend RenderWindow & operator<<(RenderWindow&,IntBox&);
    };

    RenderWindow & operator<<(RenderWindow & RW,IntBox & TB)
    {
        RW.Draw(TB.Rect);
        RW.Draw(TB.Str);

        return RW;
    }
}

#endif
