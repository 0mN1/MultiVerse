#ifndef __ANIMATIONSYSTEM_HPP__
#define __ANIMATIONSYSTEM_HPP__

#include "Skeleton.hpp"

class AnimationRequest
{
    public:
    short Priority;
    string Id;

    AnimationRequest(short priority = 0, string id = "")
    {
        if(priority != 0 && id != "")
        {
            Priority = priority;
            Id = id;
        }
    }
};

class AnimationEntity
{
    public:
    string Name;
    Skeleton skeleton;

    AnimationEntity(string name = "", string filename = "", string File = "KingOfLimbs", int bodyType = CHARTYPE01) : skeleton(bodyType)
    {
        if(name != "" && filename != "")
        {
            Name = name;
            skeleton.CreateLimbs(File);
            skeleton.LoadAnimation(filename);
        }
    }
};

class EntityList : public List< AnimationEntity >
{
    protected:

    public:
    EntityList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    EntityList(const EntityList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< AnimationEntity >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< AnimationEntity > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< AnimationEntity >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~EntityList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline AnimationEntity * Find(string Name)
    {
        LIST::Node< AnimationEntity > * Current = FirstNode;

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

class AnimationSystem
{
    public:
    EntityList Entities;
    List< AnimationRequest > Requests;

    Vector2f Position;
    bool Mirror;

    bool CombatMode;
    Vector2f MouseRelativity;

    bool TiltingHead;
    float HeadTiltAmount;
    bool ShakingHead;
    float HeadShakeAmount;

    int BodyType;

    AnimationSystem(string File, int bodyType)
    {
        BodyType = bodyType;

        if(bodyType == CHARTYPE01)
        {
            AddEntity("Idle", "CharType01/IdleCycle.anm", true, File);
        }
        else if(bodyType == CORPPEOPLE)
        {
            AddEntity("Idle", "CorpPeople/IdleCycle.anm", true, File);
        }
        AddRequest("Idle", 1);
        TiltingHead = false;
        ShakingHead = false;
        CombatMode = false;
        HeadTiltAmount = 0.f;
        HeadShakeAmount = 1.f;
    }

    void AddRequest(string id, short priority)
    {
        if(Requests.GetSize() > 0)
        {
            if(Requests[0]->Id == id)
            return ;

            if(priority == 1)
            {
                Entities.Find(Requests[0]->Id)->skeleton.Restart();
                Requests.Clear();
            }
        }

        Requests.AddNode(AnimationRequest(priority, id));
    }

    void AddEntity(string name, string filename, bool AddPrefix = true, string File = "KingOfLimbs")
    {
        Entities.AddNode(new AnimationEntity(name, (AddPrefix?"Data/Animation/Skeleton/":"") + filename, File, BodyType));
    }

    void CalculateMouseRelativity(Vector2f CursorPos)
    {
        CursorPos -= Position;
        MouseRelativity.x = CursorPos.x - (Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonPoint->Vector.x + Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonPoint->Vector.x) / 2;
        MouseRelativity.y = CursorPos.y - (Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonPoint->Vector.y + Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonPoint->Vector.y) / 2;
    }

    void MoveArms()
    {
        /*Entities.Find(Requests[0]->Id)->skeleton.rArm[Entities.Find(Requests[0]->Id)->skeleton.rArm.GetSize()-1]->SkeletonPoint->Vector.x = MouseRelativity.x;
        Entities.Find(Requests[0]->Id)->skeleton.rArm[Entities.Find(Requests[0]->Id)->skeleton.rArm.GetSize()-1]->SkeletonPoint->Vector.y = MouseRelativity.y;
        Entities.Find(Requests[0]->Id)->skeleton.lArm[Entities.Find(Requests[0]->Id)->skeleton.lArm.GetSize()-1]->SkeletonPoint->Vector.x = MouseRelativity.x;
        Entities.Find(Requests[0]->Id)->skeleton.lArm[Entities.Find(Requests[0]->Id)->skeleton.lArm.GetSize()-1]->SkeletonPoint->Vector.y = MouseRelativity.y;*/

        Entities.Find(Requests[0]->Id)->skeleton.CombatArms = MouseRelativity;

        /*/// rArm
        {
            for(int i = Entities.Find(Requests[0]->Id)->skeleton.rArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.rArm[i+1]->SkeletonPoint->Vector);
                Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector,
                Entities.Find(Requests[0]->Id)->skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.rArm[i+1]->Dist);
            }

            float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonCenter->Vector);
            Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonPoint->Vector,
            Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->SkeletonCenter->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.rArm[0]->Dist);

            for(int i = 1; i < Entities.Find(Requests[0]->Id)->skeleton.rArm.GetSize(); i++)
            {
                float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.rArm[i-1]->SkeletonPoint->Vector);
                Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector,
                Entities.Find(Requests[0]->Id)->skeleton.rArm[i-1]->SkeletonPoint->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->Dist);
            }

            for(int i = Entities.Find(Requests[0]->Id)->skeleton.rArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.rArm[i+1]->SkeletonPoint->Vector);
                Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.rArm[i]->SkeletonPoint->Vector,
                Entities.Find(Requests[0]->Id)->skeleton.rArm[i+1]->SkeletonPoint->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.rArm[i+1]->Dist);
            }
        }

        /// lArm
        {
            for(int i = Entities.Find(Requests[0]->Id)->skeleton.lArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.lArm[i+1]->SkeletonPoint->Vector);
                Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector,
                Entities.Find(Requests[0]->Id)->skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.lArm[i+1]->Dist);
            }

            float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonCenter->Vector);
            Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonPoint->Vector,
            Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->SkeletonCenter->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.lArm[0]->Dist);

            for(int i = 1; i < Entities.Find(Requests[0]->Id)->skeleton.lArm.GetSize(); i++)
            {
                float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.lArm[i-1]->SkeletonPoint->Vector);
                Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector,
                Entities.Find(Requests[0]->Id)->skeleton.lArm[i-1]->SkeletonPoint->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->Dist);
            }

            for(int i = Entities.Find(Requests[0]->Id)->skeleton.lArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector, Entities.Find(Requests[0]->Id)->skeleton.lArm[i+1]->SkeletonPoint->Vector);
                Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Entities.Find(Requests[0]->Id)->skeleton.lArm[i]->SkeletonPoint->Vector,
                Entities.Find(Requests[0]->Id)->skeleton.lArm[i+1]->SkeletonPoint->Vector), Dist-Entities.Find(Requests[0]->Id)->skeleton.lArm[i+1]->Dist);
            }
        }*/
    }

    void Draw(Application & App, bool Direction)
    {
        if(Requests.GetSize() == 0)
        {
            AddRequest("Idle", 1);
        }

        Entities.Find(Requests[0]->Id)->skeleton.CombatMode = CombatMode;

        if(TiltingHead)
        Entities.Find(Requests[0]->Id)->skeleton.TiltHead(HeadTiltAmount);

        if(ShakingHead)
        Entities.Find(Requests[0]->Id)->skeleton.ShakeHead(HeadShakeAmount);

        Entities.Find(Requests[0]->Id)->skeleton.Position = Position;

        if(Requests.GetSize() > 1)
        {
            if(Entities.Find(Requests[0]->Id)->skeleton.Draw(App, Direction, Requests[1]->Priority, false, true))
            {
                //cout << Requests.GetSize() << " ";
                Entities.Find(Requests[0]->Id)->skeleton.Restart();
                Requests.RemoveNode(0);
            }
        }
        else
        Entities.Find(Requests[0]->Id)->skeleton.Draw(App, Direction, 1, false, true);
    }
};

#endif
