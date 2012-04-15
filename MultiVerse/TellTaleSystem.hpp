#ifndef __TELLTALESYSTEM_HPP__
#define __TELLTALESYSTEM_HPP__

#include "Skeleton.hpp"

class Level;

class TellTalePosition
{
    public:
    Vector2f Position;
    int TimePosition;

    TellTalePosition()
    {
        TimePosition = 0;
    }

    TellTalePosition(Vector2f Pos, int timePos)
    {
        Position = Pos;
        TimePosition = timePos;
    }
};

class TellTaleDirection
{
    public:
    bool Direction;
    float TimePosition;

    TellTaleDirection()
    {
        Direction = false;
        TimePosition = 0.f;
    }

    TellTaleDirection(bool Dir, float timePos)
    {
        Direction = Dir;
        TimePosition = timePos;
    }
};

class TTPositionList : public List< TellTalePosition >
{
    protected:

    public:
    TTPositionList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    TTPositionList(const TTPositionList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< TellTalePosition >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< TellTalePosition > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< TellTalePosition >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~TTPositionList()
    {
        Clear();
    }

//    inline TellTalePosition * Find(Vector2f * Position)
//    {
//        LIST::Node< TellTalePosition > * Current = FirstNode;
//
//        for(int i = 0; i < Size; i++)
//        {
//            if(i != 0)
//            Current = Current->Next;
//
//            if((*Current->GetItem())->position == Position)
//            return Current->GetItem();
//        }
//
//        return NULL;
//    }

    void Sort()
    {
        if(Size > 1)
        {
            for(int j = 1; j < Size; j++)
            {
                int i = j;

                while(i > 0)
                {
                    if(GetElement(i - 1) && GetElement(i - 1)->TimePosition > GetElement(i)->TimePosition)
                    SwitchNodes(i - 1, i);
                    else
                    break;

                    --i;
                }
            }
        }
    }
};

class TTDirectionList : public List< TellTaleDirection >
{
    protected:

    public:
    TTDirectionList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    TTDirectionList(const TTDirectionList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< TellTaleDirection >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< TellTaleDirection > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< TellTaleDirection >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~TTDirectionList()
    {
        Clear();
    }

//    inline TellTaleDirection * Find(Vector2f * Position)
//    {
//        LIST::Node< TellTaleDirection > * Current = FirstNode;
//
//        for(int i = 0; i < Size; i++)
//        {
//            if(i != 0)
//            Current = Current->Next;
//
//            if((*Current->GetItem())->position == Position)
//            return Current->GetItem();
//        }
//
//        return NULL;
//    }

    void Sort()
    {
        if(Size > 1)
        {
            for(int j = 1; j < Size; j++)
            {
                int i = j;

                while(i > 0)
                {
                    if(GetElement(i - 1) && GetElement(i - 1)->TimePosition > GetElement(i)->TimePosition)
                    SwitchNodes(i - 1, i);
                    else
                    break;

                    --i;
                }
            }
        }
    }
};

class TellTaleAudio
{
    public:
    SoundBuffer SB;
    Sound sound;
    float TimePosition;
};

class TellTaleSubtitle
{
    public:
    string Text;
    float TimePosition;
    float Duration;

    TellTaleSubtitle()
    {}

    TellTaleSubtitle(string text, float time, float dur)
    {
        Text = text;
        TimePosition = time;
        Duration = dur;
    }
};

class TTSubtitleList : public List< TellTaleSubtitle >
{
    protected:

    public:
    TTSubtitleList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    TTSubtitleList(const TTSubtitleList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< TellTaleSubtitle >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< TellTaleSubtitle > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< TellTaleSubtitle >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~TTSubtitleList()
    {
        Clear();
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
                    if(GetElement(i - 1) && GetElement(i - 1)->TimePosition > GetElement(i)->TimePosition)
                    SwitchNodes(i - 1, i);
                    else
                    break;

                    --i;
                }
            }
        }
    }
};

class TellTaleCharacter
{
    public:
    string ID;
    String Identification;
    int BodyType;
    string EditorBody;
    Skeleton skeleton;
    Vector2f Position;
    bool Direction;
    TTPositionList Positions;
    TTDirectionList Directions;
    bool PickUp;
    int BodyGroup;
    int Selected;
    Vector2f PickUpOffset;
    bool AngleMode;

    TellTaleCharacter()
    {}

    TellTaleCharacter(string id, int bodyType, string editorBody, Vector2f Pos = Vector2f(0, 0))
    : skeleton(bodyType)
    {
        BodyType = bodyType;
        EditorBody = editorBody;
        skeleton.CreateLimbs(editorBody);
        skeleton.Play = false;

        PickUp = false;
        BodyGroup = 0;
        Selected = -1;
        AngleMode = false;


        ID = id;

        Identification.SetText(ID);
        Identification.SetSize(20);
        Identification.SetCenter(GetCenter(Identification));

        if(Positions.GetSize() == 0)
        Positions.AddNode(TellTalePosition(Pos, 0));

        if(Directions.GetSize() == 0)
        Directions.AddNode(TellTaleDirection(false, 0));

        Direction = false;
    }

    void ClearPositionKeyFrame(float Time)
    {
        int time = int(Time*100.f+0.5f);

        for(int i = 0; i < Positions.GetSize(); i++)
        {
            if(Positions[i]->TimePosition == time && time != 0.f)
            {
                Positions.RemoveNode(i);
                break;
            }
        }
    }

    bool IsPositionKeyFrame(float Time)
    {
        int time = int(Time*100.f+0.5f);
        // If I do a cout here it works, if I don't it doesn't work. (Fixed by using int's)
        for(int i = 0; i < Positions.GetSize(); i++)
        {
            if(Positions[i]->TimePosition == time)
            {
                return true;
            }
        }
        return false;
    }

    void SetPosition(Vector2f Pos, float Time)
    {
        Position = Pos;
        int time = int(Time*100.f+0.5f);
//        cout << "X: " << Pos.x << " Y: " << Pos.y << " " << Time << " " << time;
        for(int i = 0; i < Positions.GetSize(); i++)
        {
            if(Positions[i]->TimePosition == time)
            {
                Positions[i]->Position = Pos;
                break;
            }
            else if(i == Positions.GetSize()-1 || Positions[i+1]->TimePosition > time)
            {
                Positions.AddNode(new TellTalePosition(Pos, time));
                Positions.Sort();
//                cout << " " << Positions[i+1]->TimePosition;
                break;
            }
        }

//        cout << "\n";
    }

    void ClearDirectionKeyFrame(float Time)
    {
        int time = int(Time*100.f+0.5f);

        for(int i = 0; i < Directions.GetSize(); i++)
        {
            if(Directions[i]->TimePosition == time && time != 0)
            {
                Directions.RemoveNode(i);
                break;
            }
        }
    }

    bool IsDirectionKeyFrame(float Time)
    {
        int time = int(Time*100.f+0.5f);

        for(int i = 0; i < Directions.GetSize(); i++)
        {
            if(Directions[i]->TimePosition == time)
            {
                return true;
            }
            else if(i == Directions.GetSize()-1 || Directions[i+1]->TimePosition > time)
            {
                return false;
            }
        }

        return false;
    }

    void SetDirection(bool Dir, float Time)
    {
        Direction = Dir;
        int time = int(Time*100.f+0.5f);
        //cout << "Dir: " << Dir << " " << Time << " " << time << "\n";
        for(int i = 0; i < Directions.GetSize(); i++)
        {
            if(Directions[i]->TimePosition == time)
            {
                Directions[i]->Direction = Dir;
                break;
            }
            else if(i == Directions.GetSize()-1 || Directions[i+1]->TimePosition > time)
            {
                Directions.AddNode(new TellTaleDirection(Dir, time));
                Directions.Sort();
                break;
            }
        }
    }

    void Update(float Time)
    {
        float time = Time*100.f;

        int i = 0;

        if(time >= Positions[Positions.GetSize()-1]->TimePosition)
        {
            Position = Positions[Positions.GetSize()-1]->Position;
        }
        else
        {
            while(i < Positions.GetSize()-1 && Positions[i]->TimePosition <= time)
            {
                if(time == Positions[i]->TimePosition)
                {
                    Position = Positions[i]->Position;

                    break;
                }
                else if(time < Positions[i+1]->TimePosition)
                {
                    float percent = (time-Positions[i]->TimePosition) / (Positions[i+1]->TimePosition-Positions[i]->TimePosition);
                    Position.x = Positions[i]->Position.x + (Positions[i+1]->Position.x-Positions[i]->Position.x) * percent;
                    Position.y = Positions[i]->Position.y + (Positions[i+1]->Position.y-Positions[i]->Position.y) * percent;

                    break;
                }
                else
                i++;
            }
        }

        i = 0;

        if(time >= Directions[Directions.GetSize()-1]->TimePosition)
        {
            Direction = Directions[Directions.GetSize()-1]->Direction;
        }
        else
        {
            while(i < Directions.GetSize()-1 && Directions[i]->TimePosition <= time)
            {
                if(time == Directions[i]->TimePosition || time < Directions[i+1]->TimePosition)
                {
                    Direction = Directions[i]->Direction;

                    break;
                }
                else
                i++;
            }
        }

        skeleton.Position = Position;
    }

    void Draw(Application & App, float Time, bool Loop = false, bool Selected = true, Vector2f CursorPrevPosition = Vector2f(0, 0))
    {
        skeleton.Position = Position;
        skeleton.Draw(App,/* Time,*/ Direction,/* Loop,*/ 0, Selected);

        if(Selected)
        {
            for(int i = 0; i < skeleton.GetvList().GetSize(); i++)
            {
                App().Draw(skeleton.GetJointSprite(i, false));
            }

            /// rArm
            {
                for(int i = skeleton.rArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                    skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                }

                float Dist = Distance(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector);
                skeleton.rArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector), Dist-skeleton.rArm[0]->Dist);

                for(int i = 1; i < skeleton.rArm.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector);
                    skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i]->Dist);
                }

                for(int i = skeleton.rArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                    skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                }
            }

            /// lArm
            {
                for(int i = skeleton.lArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                    skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                }

                float Dist = Distance(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector);
                skeleton.lArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector), Dist-skeleton.lArm[0]->Dist);

                for(int i = 1; i < skeleton.lArm.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector);
                    skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i]->Dist);
                }

                for(int i = skeleton.lArm.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                    skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                }
            }

            /// rLeg
            {
                for(int i = skeleton.rLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                }

                float Dist = Distance(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector);
                skeleton.rLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector), Dist-skeleton.rLeg[0]->Dist);

                for(int i = 1; i < skeleton.rLeg.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector);
                    skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i]->Dist);
                }

                for(int i = skeleton.rLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                }
            }

            /// lLeg
            {
                for(int i = skeleton.lLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                }

                float Dist = Distance(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector);
                skeleton.lLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector), Dist-skeleton.lLeg[0]->Dist);

                for(int i = 1; i < skeleton.lLeg.GetSize(); i++)
                {
                    float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector);
                    skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i]->Dist);
                }

                for(int i = skeleton.lLeg.GetSize()-2; i >= 0; i--)
                {
                    float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                    skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                }
            }

            /// Body
            {
                if(BodyType == CHARTYPE01)
                {
                    for(int i = skeleton.Body.GetSize()-3; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }

                    float Dist = Distance(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector);
                    skeleton.Body[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector), Dist-skeleton.Body[0]->Dist);

                    for(int i = 1; i < skeleton.Body.GetSize()-1; i++)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector), Dist-skeleton.Body[i]->Dist);
                    }

                    for(int i = skeleton.Body.GetSize()-3; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }
                }
                else if(BodyType == CORPPEOPLE)
                {
                    for(int i = skeleton.Body.GetSize()-2; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }

                    float Dist = Distance(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector);
                    skeleton.Body[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[0]->SkeletonPoint->Vector, skeleton.Body[0]->SkeletonCenter->Vector), Dist-skeleton.Body[0]->Dist);

                    for(int i = 1; i < skeleton.Body.GetSize(); i++)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i-1]->SkeletonPoint->Vector), Dist-skeleton.Body[i]->Dist);
                    }

                    for(int i = skeleton.Body.GetSize()-2; i >= 0; i--)
                    {
                        float Dist = Distance(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector);
                        skeleton.Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[i+1]->SkeletonPoint->Vector), Dist-skeleton.Body[i+1]->Dist);
                    }
                }
            }

            if(Cursor.Left.Pressed || Cursor.Right.Pressed)
            {
                for(int i = 0; i < skeleton.rArm.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(RARM, i)))
                    {
                        cout << "n" << i << " ";
                        PickUp = true;
                        BodyGroup = RARM;

                        PickUpOffset = Cursor.Position - (skeleton.Position+skeleton.rArm[i]->SkeletonPoint->Vector);

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.lArm.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(LARM, i)))
                    {
                        PickUp = true;
                        BodyGroup = LARM;

                        PickUpOffset = Cursor.Position - (skeleton.Position+skeleton.lArm[i]->SkeletonPoint->Vector);

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.Body.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(BODY, i)))
                    {
                        PickUp = true;
                        BodyGroup = BODY;

                        PickUpOffset = Cursor.Position - (skeleton.Position+skeleton.Body[i]->SkeletonPoint->Vector);

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.rLeg.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(RLEG, i)))
                    {
                        PickUp = true;
                        BodyGroup = RLEG;

                        PickUpOffset = Cursor.Position - (skeleton.Position+skeleton.rLeg[i]->SkeletonPoint->Vector);

                        Selected = i;
                    }
                }

                for(int i = 0; i < skeleton.lLeg.GetSize(); i++)
                {
                    if(PPCollision(Cursor.Position, skeleton.GetJointSprite(LLEG, i)))
                    {
                        PickUp = true;
                        BodyGroup = LLEG;

                        PickUpOffset = Cursor.Position - (skeleton.Position+skeleton.lLeg[i]->SkeletonPoint->Vector);

                        Selected = i;
                    }
                }

                if(PPCollision(Cursor.Position, skeleton.GetJointSprite(ALL)))
                {
                    PickUp = true;
                    BodyGroup = ALL;

                    PickUpOffset = Cursor.Position - (skeleton.Position+skeleton.Body[0]->SkeletonCenter->Vector);
                }
            }

            if(PickUp)
            {
                if(BodyGroup == RARM)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.rArm[Selected]->SkeletonPoint->Vector, skeleton.rArm[Selected]->SkeletonCenter->Vector,
                            skeleton.rArm[Selected]->Rotation - Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.rArm[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.rArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.rArm.GetSize(); i++)
                            {
                                skeleton.rArm[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.rArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.rArmGravity = !skeleton.bones.rArmGravity;

                            Vector2f Pos1 = skeleton.Position+skeleton.rArm[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.Position+skeleton.rArm[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - (skeleton.Position+skeleton.rArm[Selected]->SkeletonPoint->Vector);

                            skeleton.rArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            cout << "m" << Selected << " ";

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector);
                            skeleton.rArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[0]->SkeletonPoint->Vector, skeleton.rArm[0]->SkeletonCenter->Vector), Dist-skeleton.rArm[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.rArm.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector);
                                skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.rArm[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rArm[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.rArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.rArm.GetSize(); i++)
                        {
                            skeleton.rArm[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.rArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rArm[i]->SkeletonPoint->Vector, skeleton.rArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.rArm[Selected]->Dist = Distance(skeleton.rArm[Selected]->SkeletonCenter->Vector, skeleton.rArm[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == LARM)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.lArm[Selected]->SkeletonPoint->Vector, skeleton.lArm[Selected]->SkeletonCenter->Vector,
                            skeleton.lArm[Selected]->Rotation - Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.lArm[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.lArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.lArm.GetSize(); i++)
                            {
                                skeleton.lArm[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.lArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.lArmGravity = !skeleton.bones.lArmGravity;

                            Vector2f Pos1 = skeleton.lArm[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.lArm[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lArm[Selected]->SkeletonPoint->Vector;

                            skeleton.lArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector);
                            skeleton.lArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[0]->SkeletonPoint->Vector, skeleton.lArm[0]->SkeletonCenter->Vector), Dist-skeleton.lArm[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.lArm.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector);
                                skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-skeleton.lArm[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lArm[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.lArm[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.lArm.GetSize(); i++)
                        {
                            skeleton.lArm[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.lArm[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lArm[i]->SkeletonPoint->Vector, skeleton.lArm[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.lArm[Selected]->Dist = Distance(skeleton.lArm[Selected]->SkeletonCenter->Vector, skeleton.lArm[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == BODY)
                {
//                    if(AngleMode)
                    {
                        Vector2f PositionDelta = RotatePoint(skeleton.Body[Selected]->SkeletonPoint->Vector, skeleton.Body[Selected]->SkeletonCenter->Vector,
                        skeleton.Body[Selected]->Rotation - Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.Body[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.Body[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.Body.GetSize(); i++)
                        {
                            skeleton.Body[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.Body[i]->SkeletonPoint->Vector = RotatePoint(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }
                    }
//                    else
//                    {
//                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.Body[Selected]->SkeletonPoint->Vector;
//                        float RotationDelta = (Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector + PositionDelta) -
//                                               Angle(skeleton.Body[Selected]->SkeletonCenter->Vector, skeleton.Body[Selected]->SkeletonPoint->Vector)) * DPR;
//                        skeleton.Body[Selected]->SkeletonPoint->Vector += PositionDelta;
//
//                        for(int i = Selected+1; i < skeleton.Body.GetSize(); i++)
//                        {
//                            skeleton.Body[i]->SkeletonPoint->Vector += PositionDelta;
//                            skeleton.Body[i]->SkeletonPoint->Vector = RotatePoint(skeleton.Body[i]->SkeletonPoint->Vector, skeleton.Body[Selected+1]->SkeletonCenter->Vector, RotationDelta);
//                        }
//                    }
                }
                else if(BodyGroup == RLEG)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.rLeg[Selected]->SkeletonPoint->Vector, skeleton.rLeg[Selected]->SkeletonCenter->Vector,
                            skeleton.rLeg[Selected]->Rotation - Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.rLeg[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.rLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.rLeg.GetSize(); i++)
                            {
                                skeleton.rLeg[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.rLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.rLegGravity = !skeleton.bones.rLegGravity;

                            Vector2f Pos1 = skeleton.rLeg[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.rLeg[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rLeg[Selected]->SkeletonPoint->Vector;

                            skeleton.rLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector);
                            skeleton.rLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[0]->SkeletonPoint->Vector, skeleton.rLeg[0]->SkeletonCenter->Vector), Dist-skeleton.rLeg[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.rLeg.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.rLeg[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.rLeg[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.rLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.rLeg.GetSize(); i++)
                        {
                            skeleton.rLeg[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.rLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.rLeg[i]->SkeletonPoint->Vector, skeleton.rLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.rLeg[Selected]->Dist = Distance(skeleton.rLeg[Selected]->SkeletonCenter->Vector, skeleton.rLeg[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == LLEG)
                {
                    if(Cursor.Left())
                    {
                        if(AngleMode)
                        {
                            Vector2f PositionDelta = RotatePoint(skeleton.lLeg[Selected]->SkeletonPoint->Vector, skeleton.lLeg[Selected]->SkeletonCenter->Vector,
                            skeleton.lLeg[Selected]->Rotation - Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, Cursor.Position)*-DPR) - skeleton.lLeg[Selected]->SkeletonPoint->Vector;
                            float RotationDelta = (Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                                   Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                            skeleton.lLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected+1; i < skeleton.lLeg.GetSize(); i++)
                            {
                                skeleton.lLeg[i]->SkeletonPoint->Vector += PositionDelta;
                                skeleton.lLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                            }
                        }
                        else
                        {
                            if(Cursor.Right.Pressed)
                            skeleton.bones.lLegGravity = !skeleton.bones.lLegGravity;

                            Vector2f Pos1 = skeleton.lLeg[Selected]->SkeletonPoint->Vector;
                            Vector2f Pos2 = skeleton.lLeg[Selected]->SkeletonCenter->Vector;
                            Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lLeg[Selected]->SkeletonPoint->Vector;

                            skeleton.lLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                            }

                            float Dist = Distance(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector);
                            skeleton.lLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[0]->SkeletonPoint->Vector, skeleton.lLeg[0]->SkeletonCenter->Vector), Dist-skeleton.lLeg[0]->Dist);

                            for(int i = 1; i <= Selected; i++)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i]->Dist);
                            }

                            for(int i = Selected-1; i >= 0; i--)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i+1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i+1]->Dist);
                            }

                            for(int i = Selected+1; i < skeleton.lLeg.GetSize(); i++)
                            {
                                float Dist = Distance(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector);
                                skeleton.lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[i-1]->SkeletonPoint->Vector), Dist-skeleton.lLeg[i]->Dist);
                            }
                        }
                    }
                    else if(Cursor.Right())
                    {
                        Vector2f PositionDelta = (Cursor.Position - PickUpOffset) - skeleton.lLeg[Selected]->SkeletonPoint->Vector;
                        float RotationDelta = (Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector + PositionDelta) -
                                               Angle(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector)) * DPR;
                        skeleton.lLeg[Selected]->SkeletonPoint->Vector += PositionDelta;

                        for(int i = Selected+1; i < skeleton.lLeg.GetSize(); i++)
                        {
                            skeleton.lLeg[i]->SkeletonPoint->Vector += PositionDelta;
                            skeleton.lLeg[i]->SkeletonPoint->Vector = RotatePoint(skeleton.lLeg[i]->SkeletonPoint->Vector, skeleton.lLeg[Selected+1]->SkeletonCenter->Vector, RotationDelta);
                        }

                        skeleton.lLeg[Selected]->Dist = Distance(skeleton.lLeg[Selected]->SkeletonCenter->Vector, skeleton.lLeg[Selected]->SkeletonPoint->Vector);
                    }
                }
                else if(BodyGroup == ALL)
                {
                    if(Cursor.Left())
                    skeleton.Body[0]->SkeletonCenter->Vector = Cursor.Position - PickUpOffset;
                    else if(Cursor.Right())
                    {
                        for(int i = 0; i < skeleton.GetvList().GetSize(); i++)
                        {
                            skeleton.GetvList()[i]->Vector += Cursor.Position - CursorPrevPosition;
                        }
                    }
                }

                if(Cursor.Left.Released || (Cursor.Right.Released && !Cursor.Left()))
                PickUp = false;
            }

        }
    }

    void DrawIdentification(Application & App)
    {
        Identification.SetPosition(Position);
        App().Draw(Identification);
    }
};

class TellTaleView
{
    public:
    Vector2f Position;
    float Scale;
    float TimePosition;

    TellTaleView()
    {}

    TellTaleView(Vector2f Pos, float Scal, float timePos)
    {
        Position = Pos;
        Scale = Scal;
        TimePosition = timePos;
    }
};

class TellTaleScene
{
    public:
    string LevelFile;
    string Name;
    char Type; /// C = Cutscene, S = Sequence, D = Dialog
    bool FocusCamOnPlayer;
    bool LockPlayerInIdle;
    TellTaleCharacter * PlayerCharacter;
    List< TellTaleCharacter > Characters;
    List< TellTaleView > Camera;
    TTSubtitleList Subtitles;
    String Subtitle;
    Shape SubRect;
    float Time;
    float Duration;
    bool InGame;
    bool Playing;
    float EditorTime;

    TellTaleScene(bool inGame = false)
    {
        InGame = inGame;

        Playing = false;
        EditorTime = 0.f;

        Time = 0.f;
        Duration = 30.f;
        Type = 'D';
        FocusCamOnPlayer = true;
        LockPlayerInIdle = true;
        PlayerCharacter = NULL;

        if(Camera.GetSize() > 0)
        Camera.AddNode(new TellTaleView(Vector2f(0, 0), 1.f, 0.f));
    }

    ~TellTaleScene()
    {
        if(PlayerCharacter != NULL)
        delete PlayerCharacter;
    }

    void Play()
    {
        Playing = true;
        EditorTime = Time;
        Time = 0.f;
    }

    void Stop()
    {
        Playing = false;

        if(!InGame)
        Time = EditorTime;
    }

    void AddCharacter(string id, int charType, string editorBody, Vector2f p = Vector2f(0, 0))
    {
        Characters.AddNode(new TellTaleCharacter(id, charType, editorBody, p));
    }

    void AddSubtitle(string text, float dur)
    {
        Subtitles.AddNode(new TellTaleSubtitle(text, Time, dur == 0.f?Duration-Time:dur));
    }

    void AddSubtitle(string text, float timePos, float dur)
    {
        Subtitles.AddNode(new TellTaleSubtitle(text, timePos, dur == 0.f?Duration-Time:dur));
    }

    void SetType(string t, Vector2f p = Vector2f(0, 0))
    {
        Type = t[0];
        if(Type == 'S')
        {
            if(PlayerCharacter == NULL)
            PlayerCharacter = new TellTaleCharacter("PLAYER", CHARTYPE01, "KingOfLimbs", p);
            PlayerCharacter->skeleton.LoadAnimation("Data/Animation/Skeleton/CharType01/IdleCycle.anm");
        }
    }

    void Update(Application & App, Level * Lvl);

    void UpdateCharacterPositions()
    {
        for(int i = 0; i < Characters.GetSize(); i++)
        {
            Characters[i]->Update(Time);
        }

        if(Type == 'S')
        {
            PlayerCharacter->Update(Time);
        }
    }

    void DrawCharacters(Application & App, bool ShowName = false, bool PlaySel = false, bool CharSel = false, int nCharSel = 0, Vector2f lastMouse = Vector2f(0, 0))
    {
        for(int i = 0; i < Characters.GetSize(); i++)
        {
            Characters[i]->Draw(App, Time, false, (CharSel && nCharSel == i), lastMouse);
            if(ShowName)
            Characters[i]->DrawIdentification(App);
        }

        if(Type == 'S')
        {
            PlayerCharacter->Draw(App, Time, LockPlayerInIdle, PlaySel, lastMouse);
            if(ShowName)
            PlayerCharacter->DrawIdentification(App);
        }
    }

    void DrawSubtitles(Application & App, Vector2f ViewPosition = Vector2f(0, 0))
    {
        if(Subtitles.GetSize() > 0)
        {
            int i = 0;

            while(i < Subtitles.GetSize() && Subtitles[i]->TimePosition <= Time)
            {
                if(!(i < Subtitles.GetSize()-1 && Time >= Subtitles[i+1]->TimePosition)
                && Time >= Subtitles[i]->TimePosition
                && Time < Subtitles[i]->TimePosition+Subtitles[i]->Duration)
                {
                    Subtitle.SetText(Subtitles[i]->Text);
                    Subtitle.SetScale(Vector2f(1.f, 1.f));
                    Subtitle.SetCenter(GetCenter(Subtitle));

                    Subtitle.SetPosition(ViewPosition + Vector2f(640 / App.GetZoom().x, 660 / App.GetZoom().y));
                    Subtitle.SetScale(Vector2f(1.f / App.GetZoom().x, 1.f / App.GetZoom().y));

                    SubRect = Shape::Rectangle(Subtitle.GetRect().Left, Subtitle.GetRect().Top,
                                               Subtitle.GetRect().Right, Subtitle.GetRect().Bottom,
                                               Color(0, 0, 0, 200));
                    App().Draw(SubRect);
                    App().Draw(Subtitle);

                    break;
                }
                else
                i++;
            }
        }
    }

    void Save(string fileName)
    {
        Name = fileName;

        ofstream fout;

        fout.open(string("Data/Scene/"+fileName+".scn").c_str());

        fout << "Name " << Name << endl;
        fout << "Lvl " << LevelFile << endl;
        fout << "Type " << Type << endl;
        fout << "Dur " << Duration << endl;
        fout << "Fcop " << FocusCamOnPlayer << endl;

        for(int i = 0; i < Subtitles.GetSize(); i++)
        {
            fout << "Subt " << Subtitles[i]->Text << " | " << Subtitles[i]->TimePosition << " " << Subtitles[i]->Duration << endl;
        }

        if(Type == 'S')
        {
            fout << "Lpii " << LockPlayerInIdle << endl;
            fout << "PChar " << PlayerCharacter->ID << " " << PlayerCharacter->BodyType << " " << PlayerCharacter->EditorBody << endl;

            for(int j = 0; j < PlayerCharacter->Positions.GetSize(); j++)
            fout << "\tPos " << PlayerCharacter->Positions[j]->Position.x << " " << PlayerCharacter->Positions[j]->Position.y
                 << " " << PlayerCharacter->Positions[j]->TimePosition << endl;

            for(int j = 0; j < PlayerCharacter->Directions.GetSize(); j++)
            fout << "\tDir " << PlayerCharacter->Directions[j]->Direction << " " << PlayerCharacter->Directions[j]->TimePosition << endl;

            fout << "endChar\n";
        }

        for(int i = 0; i < Characters.GetSize(); i++)
        {
            TellTaleCharacter * Char = Characters[i];
            fout << "Char " << Char->ID << " " << Char->BodyType << " " << Char->EditorBody << endl;

            for(int j = 0; j < Char->Positions.GetSize(); j++)
            fout << "\tPos " << Char->Positions[j]->Position.x << " " << Char->Positions[j]->Position.y
                 << " " << Char->Positions[j]->TimePosition << endl;

            for(int j = 0; j < Char->Directions.GetSize(); j++)
            fout << "\tDir " << Char->Directions[j]->Direction << " " << Char->Directions[j]->TimePosition << endl;

            fout << "endChar\n";
        }

        fout.close();

        if(Type == 'S')
        PlayerCharacter->skeleton.SaveAnimation("Data/Scene/"+fileName+PlayerCharacter->ID+".anm");

        for(int i = 0; i < Characters.GetSize(); i++)
        {
            Characters[i]->skeleton.SaveAnimation("Data/Scene/"+fileName+Characters[i]->ID+".anm");
        }
    }

    void Load(string fileName)
    {
        ifstream fin;
        fin.open(string("Data/Scene/"+fileName+".scn").c_str());

        string input;
        List< List< string > > Lines;

        bool CreatingChar = false;
        bool CreatingPlayer = false;

        while(!fin.eof() && fin.good())
        {
            //getline(fin, input);

            if(!(fin >> input))
            break;

            if(input == "Name")
            {
                fin >> input;
                Name = input;
            }
            else if(input == "Lvl")
            {
                fin >> input;
                LevelFile = input;
            }
            else if(input == "Type")
            {
                fin >> input;
                SetType(input);
            }
            else if(input == "Dur")
            {
                fin >> input;
                Duration = StringToFloat(input);
            }
            else if(input == "Fcop")
            {
                fin >> input;
                FocusCamOnPlayer = StringToInt(input);
            }
            else if(input == "Lpii")
            {
                fin >> input;
                LockPlayerInIdle = StringToInt(input);
            }
            else if(input == "Subt")
            {
                string subt;
                while(1)
                {
                    fin >> input;

                    if(input == "|")
                    {
                        subt.erase(--subt.end());
                        break;
                    }

                    subt += input + " ";
                }

                fin >> input;
                float time = StringToFloat(input);
                fin >> input;
                AddSubtitle(subt, time, StringToFloat(input));
            }
            else if(input == "Char")
            {
                fin >> input;
                string charName = input;
                fin >> input;
                int charType = StringToInt(input);
                fin >> input;
                AddCharacter(charName, charType, input);
                CreatingChar = true;
            }
            else if(CreatingChar)
            {
                if(input == "Pos")
                {
                    fin >> input;
                    float x = StringToFloat(input);
                    fin >> input;
                    float y = StringToFloat(input);
                    fin >> input;
                    Characters[Characters.GetSize()-1]->SetPosition(Vector2f(x, y), StringToFloat(input));
                }
                else if(input == "Dir")
                {
                    fin >> input;
                    float dir = StringToInt(input);
                    fin >> input;
                    Characters[Characters.GetSize()-1]->SetDirection(dir, StringToFloat(input));
                }
                else if(input == "endChar")
                {
                    CreatingChar = false;
                }
            }
            else if(input == "PChar")
            {
                fin >> input;
                fin >> input;
                fin >> input;
                CreatingPlayer = true;
            }
            else if(CreatingPlayer)
            {
                if(input == "Pos")
                {
                    fin >> input;
                    float x = StringToFloat(input);
                    fin >> input;
                    float y = StringToFloat(input);
                    fin >> input;
                    PlayerCharacter->SetPosition(Vector2f(x, y), StringToFloat(input));
                }
                else if(input == "Dir")
                {
                    fin >> input;
                    float dir = StringToInt(input);
                    fin >> input;
                    PlayerCharacter->SetDirection(dir, StringToFloat(input));
                }
                else if(input == "endChar")
                {
                    CreatingPlayer = false;
                }
            }
            else if(input == "" || input == "\n" || input == "\t");
            else
            {
                cout << "Error while loading scene: \"" << fileName
                     << "\". Command \"" << input << "\" does not exist.\n";
            }
        }

        fin.close();

        if(Type == 'S')
        PlayerCharacter->skeleton.LoadAnimation("Data/Scene/"+fileName+PlayerCharacter->ID+".anm");

        for(int i = 0; i < Characters.GetSize(); i++)
        {
            Characters[i]->skeleton.LoadAnimation("Data/Scene/"+fileName+Characters[i]->ID+".anm");
        }
    }
};

#endif
