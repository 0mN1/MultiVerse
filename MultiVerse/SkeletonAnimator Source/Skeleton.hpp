#ifndef __SKELETON_HPP__
#define __SKELETON_HPP__

#include "SFMLfunctions.hpp"
#include "List.hpp"

using namespace LIST;

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	float min, max, delta;
	min = Min3(r, g, b);
	max = Max3(r, g, b);
	*v = max;				// v
	delta = max - min;

	if(max != 0)
	{
		*s = delta / max;		// s
	}
	else
	{
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if(r == max)
	{
		*h = (g - b) / delta;		// between yellow & magenta
	}
	else
	if(g == max)
	{
		*h = 2 + (b - r) / delta;	// between cyan & yellow
	}
	else
	{
		*h = 4 + (r - g) / delta;	// between magenta & cyan
	}

	*h *= 60;				// degrees
	if(*h < 0)
	{
		*h += 360;
	}
}

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;

	if(s == 0)
	{
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = (int)floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch(i)
	{
		case 0:
			*r = v;
			*g = t;
			*b = p;
        break;

		case 1:
			*r = q;
			*g = v;
			*b = p;
        break;

		case 2:
			*r = p;
			*g = v;
			*b = t;
        break;

		case 3:
			*r = p;
			*g = q;
			*b = v;
        break;

		case 4:
			*r = t;
			*g = p;
			*b = v;
        break;

		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
        break;
	}
}

class IDVector
{
    public:
    string ID;
    Color ColorID;
    Vector2f Vector;

    IDVector(string id = "", Color colorId = Color(255, 255, 255))
    {
        ID = id;
        ColorID = colorId;
    }
};

class VectorList : public List< IDVector >
{
    protected:

    public:
    VectorList()
    {
        Size = 0;
        FirstNode = NULL;
    }

    VectorList(const VectorList & Copy)
    {
        Size = Copy.Size;
        FirstNode = NULL;

        if(Size > 0)
        {
            FirstNode = new LIST::Node< IDVector >;
            FirstNode->SetItem(*Copy.FirstNode->GetItem());

            LIST::Node< IDVector > * Current = FirstNode, * NewNode, * CopyCurrent = Copy.FirstNode;

            for(int i = 1; i < Copy.Size; i++)
            {
                CopyCurrent = CopyCurrent->Next;

                NewNode = new LIST::Node< IDVector >;
                NewNode->SetItem(*CopyCurrent->GetItem());

                Current->Next = NewNode;

                Current = NewNode;
            }
        }
    }

    ~VectorList()
    {
        if(FirstNode)
        delete FirstNode;
    }

    inline IDVector * Find(string Name)
    {
        LIST::Node< IDVector > * Current = FirstNode;

        for(int i = 0; i < Size; i++)
        {
            if(i != 0)
            Current = Current->Next;

            if(Current->GetItem()->ID == Name)
            return Current->GetItem();
        }

        return NULL;
    }

    inline IDVector * Find(Color color)
    {
        LIST::Node< IDVector > * Current = FirstNode;

        for(int i = 0; i < Size; i++)
        {
            if(i != 0)
            Current = Current->Next;

            if(Current->GetItem()->ColorID == color)
            return Current->GetItem();
        }

        return NULL;
    }
};

class Limb
{
    protected:
    string Name;
    Image image;
    Vector2f ImageCenter;

    public:
    IDVector * SkeletonCenter, * SkeletonPoint;
    Vector2f Position;
    float Rotation;
    float Tilt;
    float Dist;
    Sprite sprite;

    Limb(string name = "", string img = "", Vector2f imgCenter = Vector2f(0, 0), IDVector * SC = NULL, IDVector * SP = NULL)
    {
        if(name != "")
        {
            Name = name;
            sprite = LoadSprite(image, img);
            ImageCenter = imgCenter;
            SkeletonCenter = SC;
            SkeletonPoint = SP;
            Position = SkeletonCenter->Vector;
            Rotation = Angle(SkeletonCenter->Vector, SkeletonPoint->Vector) * -DPR;
            Dist = Distance(SkeletonCenter->Vector, SkeletonPoint->Vector);
            Tilt = 0.f;
        }
    }

    void CalculateDistance()
    {
        Dist = Distance(SkeletonCenter->Vector, SkeletonPoint->Vector);
    }

    void Draw(Application & App, Vector2f pos, bool Mirror = false, bool DrawBones = false)
    {
        Position = SkeletonCenter->Vector + pos;
        Rotation = Angle(SkeletonCenter->Vector, SkeletonPoint->Vector) * -DPR;

        sprite.SetPosition(Position);
        sprite.SetRotation(Rotation + (Mirror?-Tilt:Tilt));
        sprite.SetCenter(Vector2f(ImageCenter.x, Mirror?sprite.GetSize().y-ImageCenter.y:ImageCenter.y));
        sprite.FlipY(Mirror);
        App().Draw(sprite);

        if(DrawBones)
        App().Draw(Shape::Line(SkeletonCenter->Vector, SkeletonPoint->Vector, 2, Color(0, 0, 0)));
    }
};

enum {RARM, LARM, BODY, RLEG, LLEG, ALL};

class Bones
{
    public:
    VectorList vectors;

    bool rArmGravity;
    bool lArmGravity;
    bool rLegGravity;
    bool lLegGravity;

    float Time;

    Bones()
    {
        vectors.AddNode(IDVector("CenterRHand", Color(50, 0, 0)));
        vectors.AddNode(IDVector("CenterRForearm", Color(100, 0, 0)));
        vectors.AddNode(IDVector("CenterRUpperarm", Color(150, 0, 0)));

        vectors.AddNode(IDVector("PointRFoot", Color(0, 250, 0)));
        vectors.AddNode(IDVector("CenterRFoot", Color(0, 200, 0)));
        vectors.AddNode(IDVector("CenterRKnee", Color(0, 150, 0)));
        vectors.AddNode(IDVector("CenterRHip", Color(0, 100, 0)));

        vectors.AddNode(IDVector("CenterLHand", Color(0, 0, 100)));
        vectors.AddNode(IDVector("CenterLForearm", Color(0, 0, 150)));
        vectors.AddNode(IDVector("CenterLUpperarm", Color(0, 0, 200)));

        vectors.AddNode(IDVector("PointLFoot", Color(150, 50, 0)));
        vectors.AddNode(IDVector("CenterLFoot", Color(100, 50, 0)));
        vectors.AddNode(IDVector("CenterLKnee", Color(50, 50, 0)));
        vectors.AddNode(IDVector("CenterLHip", Color(0, 0, 250)));

        vectors.AddNode(IDVector("PointHead", Color(250, 0, 0)));
        vectors.AddNode(IDVector("CenterHead", Color(200, 0, 0)));
        vectors.AddNode(IDVector("CenterBody", Color(0, 50, 0)));

        rArmGravity = false;
        lArmGravity = false;
        rLegGravity = false;
        lLegGravity = false;

        Time = 1.f;
    }

    void CenterBody()
    {
        for(int i = 0; i < vectors.GetSize(); i++)
        vectors[i]->Vector -= vectors.Find("CenterBody")->Vector;
    }
};

class Skeleton
{
    protected:
    Image image;
    Sprite sprite;
    Vector2f PrevPosition;

    public:
    Bones bones;
    List< Bones > Frames;
    bool Animation;
    float TimeCounter;
    int Index;
    List< Limb > rArm;
    List< Limb > lArm;
    List< Limb > Body;
    List< Limb > rLeg;
    List< Limb > lLeg;
    Vector2f Position;
    Vector2f Scale;
    bool Play;
    bool Mirror;
    float PlaybackSpeed;
    float HeadShakeAmount;
    bool ShakingHead;
    float HeadTiltAmount;
    bool TiltingHead;

    Vector2f rArmPosition, lArmPosition,
             rLegPosition, lLegPosition;

    Skeleton()
    {
        image.Create(10, 10, Color(255, 255, 255, 255));
        sprite = Sprite(image);
        SetCenterInCenter(sprite);

        Animation = false;
        TimeCounter = 0.f;
        Index = 0;
        Play = true;
        Mirror = false;
        PlaybackSpeed = 1.f;

        ShakingHead = false;
        TiltingHead = false;

        Scale = Vector2f(1.f, 1.f);

         //LoadAnimation("Default.anim");
        LoadPosFromImage("Data/Limbs/BonesColor.png");
        CalculateDistances();
        CreateLimbs();
    }

    void TiltHead(float amount)
    {
        TiltingHead = true;
        HeadTiltAmount = amount;
    }

    void ShakeHead(float amount)
    {
        ShakingHead = true;
        HeadShakeAmount = amount;
    }

    VectorList & GetvList()
    {
        return bones.vectors;
    }

    Sprite & GetJointSprite(int i, bool withColorID)
    {
        sprite.SetPosition(bones.vectors[i]->Vector);
        sprite.SetColor(withColorID?bones.vectors[i]->ColorID:Color(255, 255, 255));
        return sprite;
    }

    Sprite & GetJointSprite(int bg, int i = 0)
    {
        if(bg == RARM)
        {
            sprite.SetPosition(rArm[i]->SkeletonPoint->Vector);
        }
        else if(bg == LARM)
        {
            sprite.SetPosition(lArm[i]->SkeletonPoint->Vector);
        }
        else if(bg == BODY)
        {
            sprite.SetPosition(Body[i]->SkeletonPoint->Vector);
        }
        else if(bg == RLEG)
        {
            sprite.SetPosition(rLeg[i]->SkeletonPoint->Vector);
        }
        else if(bg == LLEG)
        {
            sprite.SetPosition(lLeg[i]->SkeletonPoint->Vector);
        }
        else if(bg == ALL)
        {
            sprite.SetPosition(Body[0]->SkeletonCenter->Vector);
        }

        return sprite;
    }

    void LoadPosFromImage(string FileName)
    {
        Image i;
        i.LoadFromFile(FileName);

        for(int y = 0; y < (int)i.GetHeight(); y++)
        for(int x = 0; x < (int)i.GetWidth(); x++)
        {
            if(i.GetPixel(x, y) != Color(255, 255, 255))
            {
                for(int j = 0; j < bones.vectors.GetSize(); j++)
                {
                    if(bones.vectors[j]->ColorID == i.GetPixel(x, y))
                    bones.vectors[j]->Vector = Vector2f(x, y);
                }
            }
        }

        for(int i = 0; i < bones.vectors.GetSize(); i++)
        {
            bones.vectors[i]->Vector -= bones.vectors.Find("CenterBody")->Vector;
        }
    }

    void Restart()
    {
        Index = 0;
        TimeCounter = 0.f;
    }

    void CreateLimbs()
    {
        lArm.AddNode(new Limb("LUpperarm", "Data/Limbs/UpperarmLeft00.png", Vector2f(18, 25), bones.vectors.Find("CenterLUpperarm"), bones.vectors.Find("CenterLForearm")));
        lArm.AddNode(new Limb("LForearm",  "Data/Limbs/ForearmLeft00.png", Vector2f(35, 46), bones.vectors.Find("CenterLForearm"),  bones.vectors.Find("CenterLHand")));

        rLeg.AddNode(new Limb("RHip",  "Data/Limbs/ThighRight00.png", Vector2f(36, 50), bones.vectors.Find("CenterRHip"),  bones.vectors.Find("CenterRKnee")));
        rLeg.AddNode(new Limb("RKnee", "Data/Limbs/KneeRight.png", Vector2f(4, 5), bones.vectors.Find("CenterRKnee"), bones.vectors.Find("CenterRFoot")));
        rLeg.AddNode(new Limb("RFoot", "Data/Limbs/FootRight.png", Vector2f(7, 5),  bones.vectors.Find("CenterRFoot"), bones.vectors.Find("PointRFoot")));

        lLeg.AddNode(new Limb("LHip",  "Data/Limbs/ThighLeft00.png", Vector2f(36, 50), bones.vectors.Find("CenterLHip"),  bones.vectors.Find("CenterLKnee")));
        lLeg.AddNode(new Limb("LKnee", "Data/Limbs/KneeLeft.png", Vector2f(4, 5), bones.vectors.Find("CenterLKnee"), bones.vectors.Find("CenterLFoot")));
        lLeg.AddNode(new Limb("LFoot", "Data/Limbs/FootLeft.png", Vector2f(7, 5),  bones.vectors.Find("CenterLFoot"), bones.vectors.Find("PointLFoot")));

        Body.AddNode(new Limb("Body", "Data/Limbs/Body.png", Vector2f(22, 51), bones.vectors.Find("CenterBody"), bones.vectors.Find("CenterHead")));
        Body.AddNode(new Limb("HeadBack", "Data/Limbs/HeadBack.png", Vector2f(33, 29), bones.vectors.Find("CenterHead"), bones.vectors.Find("PointHead")));
        Body.AddNode(new Limb("HeadFront", "Data/Limbs/HeadFront2.png", Vector2f(33, 29), bones.vectors.Find("CenterHead"), bones.vectors.Find("PointHead")));
        //Boy Body.AddNode(new Limb("Head", "Data/Limbs/Head.png", Vector2f(39, 54), bones.vectors.Find("CenterHead"), bones.vectors.Find("PointHead")));

        rArm.AddNode(new Limb("RUpperarm", "Data/Limbs/UpperarmRight00.png", Vector2f(18, 25), bones.vectors.Find("CenterRUpperarm"), bones.vectors.Find("CenterRForearm")));
        rArm.AddNode(new Limb("RForearm",  "Data/Limbs/ForearmRight00.png", Vector2f(35, 46), bones.vectors.Find("CenterRForearm"),  bones.vectors.Find("CenterRHand")));

//        lArm.AddNode(new Limb("LUpperarm", "Data/Limbs/UpperarmLeft.png", Vector2f(8, 9), bones.vectors.Find("CenterLUpperarm"), bones.vectors.Find("CenterLForearm")));
//        lArm.AddNode(new Limb("LForearm",  "Data/Limbs/ForearmLeft.png", Vector2f(8, 10), bones.vectors.Find("CenterLForearm"),  bones.vectors.Find("CenterLHand")));
//
//        rLeg.AddNode(new Limb("RHip",  "Data/Limbs/ThighRight.png", Vector2f(10, 16), bones.vectors.Find("CenterRHip"),  bones.vectors.Find("CenterRKnee")));
//        rLeg.AddNode(new Limb("RKnee", "Data/Limbs/KneeRight.png", Vector2f(8, 10), bones.vectors.Find("CenterRKnee"), bones.vectors.Find("CenterRFoot")));
//        rLeg.AddNode(new Limb("RFoot", "Data/Limbs/FootRight.png", Vector2f(5, 4),  bones.vectors.Find("CenterRFoot"), bones.vectors.Find("PointRFoot")));
//
//        lLeg.AddNode(new Limb("LHip",  "Data/Limbs/ThighLeft.png", Vector2f(10, 16), bones.vectors.Find("CenterLHip"),  bones.vectors.Find("CenterLKnee")));
//        lLeg.AddNode(new Limb("LKnee", "Data/Limbs/KneeLeft.png", Vector2f(8, 10), bones.vectors.Find("CenterLKnee"), bones.vectors.Find("CenterLFoot")));
//        lLeg.AddNode(new Limb("LFoot", "Data/Limbs/FootLeft.png", Vector2f(5, 4),  bones.vectors.Find("CenterLFoot"), bones.vectors.Find("PointLFoot")));
//
//        Body.AddNode(new Limb("Body", "Data/Limbs/Body.png", Vector2f(47, 25), bones.vectors.Find("CenterBody"), bones.vectors.Find("CenterHead")));
//        Body.AddNode(new Limb("Head", "Data/Limbs/Head.png", Vector2f(9, 28), bones.vectors.Find("CenterHead"), bones.vectors.Find("PointHead")));
//
//        rArm.AddNode(new Limb("RUpperarm", "Data/Limbs/UpperarmRight.png", Vector2f(8, 11), bones.vectors.Find("CenterRUpperarm"), bones.vectors.Find("CenterRForearm")));
//        rArm.AddNode(new Limb("RForearm",  "Data/Limbs/ForearmRight.png", Vector2f(8, 13), bones.vectors.Find("CenterRForearm"),  bones.vectors.Find("CenterRHand")));

        if(!Frames.GetSize())
        AddFrame();
    }

    void AddFrame()
    {
        Frames.AddNode(new Bones);

        for(int i = 0; i < bones.vectors.GetSize(); i++)
        Frames[Frames.GetSize()-1]->vectors[i]->Vector = bones.vectors[i]->Vector;
    }

    void CalculateDistances()
    {
        for(int i = 0; i < lArm.GetSize(); i++)
        lArm[i]->CalculateDistance();

        for(int i = 0; i < rLeg.GetSize(); i++)
        rLeg[i]->CalculateDistance();

        for(int i = 0; i < lLeg.GetSize(); i++)
        lLeg[i]->CalculateDistance();

        for(int i = 0; i < Body.GetSize(); i++)
        Body[i]->CalculateDistance();

        for(int i = 0; i < rArm.GetSize(); i++)
        rArm[i]->CalculateDistance();

        rArmPosition = bones.vectors.Find("CenterRUpperarm")->Vector - bones.vectors.Find("CenterBody")->Vector;
        lArmPosition = bones.vectors.Find("CenterLUpperarm")->Vector - bones.vectors.Find("CenterBody")->Vector;
        rLegPosition = bones.vectors.Find("CenterRHip")->Vector - bones.vectors.Find("CenterBody")->Vector;
        lLegPosition = bones.vectors.Find("CenterLHip")->Vector - bones.vectors.Find("CenterBody")->Vector;

//        rArmPosition = RotatePoint(rArmPosition, Vector2f(0, 0), -Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR-90);
//        lArmPosition = RotatePoint(lArmPosition, Vector2f(0, 0), -Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR-90);
//        rLegPosition = RotatePoint(rLegPosition, Vector2f(0, 0), -Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR-90);
//        lLegPosition = RotatePoint(lLegPosition, Vector2f(0, 0), -Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR-90);
    }

    void CenterCurrent(int Current)
    {
        Frames[Current]->CenterBody();

        for(int i = 0; i < bones.vectors.GetSize(); i++)
        bones.vectors[i]->Vector = Frames[Current]->vectors[i]->Vector;
    }

    void CenterAll(int Current)
    {
        for(int i = 0; i < Frames.GetSize(); i++)
        Frames[i]->CenterBody();

        for(int i = 0; i < bones.vectors.GetSize(); i++)
        bones.vectors[i]->Vector = Frames[Current]->vectors[i]->Vector;
    }

    void RemoveFrame(int i)
    {
        Frames.RemoveNode(i);
    }

    bool Draw(Application & App, bool mirror = false, int Priority = 1, bool DrawBones = false, bool DrawAll = true)
    {
        bool Restart = false;
        Mirror = mirror;
        if(Play && Frames.GetSize() > 0)
        {
            for(int i = 0; i < bones.vectors.GetSize(); i++)
            {
                if(Index == Frames.GetSize()-1 && Priority == 2)
                bones.vectors[i]->Vector = Frames[Index]->vectors[i]->Vector;
                else
                bones.vectors[i]->Vector = (Frames[Index]->vectors[i]->Vector + (Frames[((Index+1 < Frames.GetSize()) ? Index+1 : 0)]->vectors[i]->Vector - Frames[Index]->vectors[i]->Vector) * (TimeCounter / Frames[Index]->Time));

                if(Mirror)
                bones.vectors[i]->Vector.x = -bones.vectors[i]->Vector.x;
            }

            TimeCounter += PlaybackSpeed * App.GetFrameTime();

            if(TimeCounter >= Frames[Index]->Time)
            {
                TimeCounter -= Frames[Index]->Time;

                Index++;

                if(Index >= Frames.GetSize())
                {
                    Index -= Frames.GetSize();
                    Restart = true;
                }
            }
        }

        /// rArm
        {
            for(int i = rArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(rArm[i]->SkeletonPoint->Vector, rArm[i+1]->SkeletonPoint->Vector);
                rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(rArm[i]->SkeletonPoint->Vector, rArm[i+1]->SkeletonPoint->Vector), Dist-rArm[i+1]->Dist);
            }

            float Dist = Distance(rArm[0]->SkeletonPoint->Vector, rArm[0]->SkeletonCenter->Vector);
            rArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(rArm[0]->SkeletonPoint->Vector, rArm[0]->SkeletonCenter->Vector), Dist-rArm[0]->Dist);

            for(int i = 1; i < rArm.GetSize(); i++)
            {
                float Dist = Distance(rArm[i]->SkeletonPoint->Vector, rArm[i-1]->SkeletonPoint->Vector);
                rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(rArm[i]->SkeletonPoint->Vector, rArm[i-1]->SkeletonPoint->Vector), Dist-rArm[i]->Dist);
            }

            for(int i = rArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(rArm[i]->SkeletonPoint->Vector, rArm[i+1]->SkeletonPoint->Vector);
                rArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(rArm[i]->SkeletonPoint->Vector, rArm[i+1]->SkeletonPoint->Vector), Dist-rArm[i+1]->Dist);
            }
        }

        /// lArm
        {
            for(int i = lArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(lArm[i]->SkeletonPoint->Vector, lArm[i+1]->SkeletonPoint->Vector);
                lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(lArm[i]->SkeletonPoint->Vector, lArm[i+1]->SkeletonPoint->Vector), Dist-lArm[i+1]->Dist);
            }

            float Dist = Distance(lArm[0]->SkeletonPoint->Vector, lArm[0]->SkeletonCenter->Vector);
            lArm[0]->SkeletonPoint->Vector += MoveInAngle(Angle(lArm[0]->SkeletonPoint->Vector, lArm[0]->SkeletonCenter->Vector), Dist-lArm[0]->Dist);

            for(int i = 1; i < lArm.GetSize(); i++)
            {
                float Dist = Distance(lArm[i]->SkeletonPoint->Vector, lArm[i-1]->SkeletonPoint->Vector);
                lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(lArm[i]->SkeletonPoint->Vector, lArm[i-1]->SkeletonPoint->Vector), Dist-lArm[i]->Dist);
            }

            for(int i = lArm.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(lArm[i]->SkeletonPoint->Vector, lArm[i+1]->SkeletonPoint->Vector);
                lArm[i]->SkeletonPoint->Vector += MoveInAngle(Angle(lArm[i]->SkeletonPoint->Vector, lArm[i+1]->SkeletonPoint->Vector), Dist-lArm[i+1]->Dist);
            }
        }

        /// rLeg
        {
            for(int i = rLeg.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(rLeg[i]->SkeletonPoint->Vector, rLeg[i+1]->SkeletonPoint->Vector);
                rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(rLeg[i]->SkeletonPoint->Vector, rLeg[i+1]->SkeletonPoint->Vector), Dist-rLeg[i+1]->Dist);
            }

            float Dist = Distance(rLeg[0]->SkeletonPoint->Vector, rLeg[0]->SkeletonCenter->Vector);
            rLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(rLeg[0]->SkeletonPoint->Vector, rLeg[0]->SkeletonCenter->Vector), Dist-rLeg[0]->Dist);

            for(int i = 1; i < rLeg.GetSize(); i++)
            {
                float Dist = Distance(rLeg[i]->SkeletonPoint->Vector, rLeg[i-1]->SkeletonPoint->Vector);
                rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(rLeg[i]->SkeletonPoint->Vector, rLeg[i-1]->SkeletonPoint->Vector), Dist-rLeg[i]->Dist);
            }

            for(int i = rLeg.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(rLeg[i]->SkeletonPoint->Vector, rLeg[i+1]->SkeletonPoint->Vector);
                rLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(rLeg[i]->SkeletonPoint->Vector, rLeg[i+1]->SkeletonPoint->Vector), Dist-rLeg[i+1]->Dist);
            }
        }

        /// lLeg
        {
            for(int i = lLeg.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(lLeg[i]->SkeletonPoint->Vector, lLeg[i+1]->SkeletonPoint->Vector);
                lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(lLeg[i]->SkeletonPoint->Vector, lLeg[i+1]->SkeletonPoint->Vector), Dist-lLeg[i+1]->Dist);
            }

            float Dist = Distance(lLeg[0]->SkeletonPoint->Vector, lLeg[0]->SkeletonCenter->Vector);
            lLeg[0]->SkeletonPoint->Vector += MoveInAngle(Angle(lLeg[0]->SkeletonPoint->Vector, lLeg[0]->SkeletonCenter->Vector), Dist-lLeg[0]->Dist);

            for(int i = 1; i < lLeg.GetSize(); i++)
            {
                float Dist = Distance(lLeg[i]->SkeletonPoint->Vector, lLeg[i-1]->SkeletonPoint->Vector);
                lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(lLeg[i]->SkeletonPoint->Vector, lLeg[i-1]->SkeletonPoint->Vector), Dist-lLeg[i]->Dist);
            }

            for(int i = lLeg.GetSize()-2; i >= 0; i--)
            {
                float Dist = Distance(lLeg[i]->SkeletonPoint->Vector, lLeg[i+1]->SkeletonPoint->Vector);
                lLeg[i]->SkeletonPoint->Vector += MoveInAngle(Angle(lLeg[i]->SkeletonPoint->Vector, lLeg[i+1]->SkeletonPoint->Vector), Dist-lLeg[i+1]->Dist);
            }
        }

        /// Body
        {
            for(int i = Body.GetSize()-3; i >= 0; i--)
            {
                float Dist = Distance(Body[i]->SkeletonPoint->Vector, Body[i+1]->SkeletonPoint->Vector);
                Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Body[i]->SkeletonPoint->Vector, Body[i+1]->SkeletonPoint->Vector), Dist-Body[i+1]->Dist);
            }

            float Dist = Distance(Body[0]->SkeletonPoint->Vector, Body[0]->SkeletonCenter->Vector);
            Body[0]->SkeletonPoint->Vector += MoveInAngle(Angle(Body[0]->SkeletonPoint->Vector, Body[0]->SkeletonCenter->Vector), Dist-Body[0]->Dist);

            for(int i = 1; i < Body.GetSize()-1; i++)
            {
                float Dist = Distance(Body[i]->SkeletonPoint->Vector, Body[i-1]->SkeletonPoint->Vector);
                Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Body[i]->SkeletonPoint->Vector, Body[i-1]->SkeletonPoint->Vector), Dist-Body[i]->Dist);
            }

            for(int i = Body.GetSize()-3; i >= 0; i--)
            {
                float Dist = Distance(Body[i]->SkeletonPoint->Vector, Body[i+1]->SkeletonPoint->Vector);
                Body[i]->SkeletonPoint->Vector += MoveInAngle(Angle(Body[i]->SkeletonPoint->Vector, Body[i+1]->SkeletonPoint->Vector), Dist-Body[i+1]->Dist);
            }
        }

        bones.vectors.Find("CenterRUpperarm")->Vector = bones.vectors.Find("CenterBody")->Vector + RotatePoint(Mirror?Vector2f(-rArmPosition.x, rArmPosition.y):rArmPosition, Vector2f(0, 0), Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR+90);
        bones.vectors.Find("CenterLUpperarm")->Vector = bones.vectors.Find("CenterBody")->Vector + RotatePoint(Mirror?Vector2f(-lArmPosition.x, lArmPosition.y):lArmPosition, Vector2f(0, 0), Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR+90);
        bones.vectors.Find("CenterRHip")->Vector = bones.vectors.Find("CenterBody")->Vector + RotatePoint(Mirror?Vector2f(-rLegPosition.x, rLegPosition.y):rLegPosition, Vector2f(0, 0), Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR+90);
        bones.vectors.Find("CenterLHip")->Vector = bones.vectors.Find("CenterBody")->Vector + RotatePoint(Mirror?Vector2f(-lLegPosition.x, lLegPosition.y):lLegPosition, Vector2f(0, 0), Angle(bones.vectors.Find("CenterBody")->Vector, bones.vectors.Find("CenterHead")->Vector)*DPR+90);

        if(ShakingHead)
        {
            Body[1]->sprite.SetScale(1.f, HeadShakeAmount);
            Body[2]->sprite.SetScale(1.f, HeadShakeAmount);
        }
        else
        {
            Body[1]->sprite.SetScale(1.f, 1.f);
            Body[2]->sprite.SetScale(1.f, 1.f);
        }

        if(TiltingHead)
        {
            Body[1]->Tilt = HeadTiltAmount;
            Body[2]->Tilt = HeadTiltAmount;
        }
        else
        {
            Body[1]->Tilt = 0.f;
            Body[2]->Tilt = 0.f;
        }

        if(GUI::Numpad5Key() || DrawAll)
        Body[1]->Draw(App, Position, Mirror, DrawBones);

        if(GUI::Numpad4Key() || DrawAll)
        for(int i = 0; i < rArm.GetSize(); i++)
        {
//            if(!bones.rArmGravity)
//            rArm[i]->SkeletonPoint->Vector += Position - PrevPosition;

            rArm[i]->Draw(App, Position, Mirror, DrawBones);
        }

        if(GUI::Numpad1Key() || DrawAll)
        for(int i = rLeg.GetSize()-1; i >= 0; i--)
        {
//            if(!bones.rLegGravity)
//            rLeg[i]->SkeletonPoint->Vector += Position - PrevPosition;

            rLeg[i]->Draw(App, Position, Mirror, DrawBones);
        }

        if(GUI::Numpad3Key() || DrawAll)
        for(int i = lLeg.GetSize()-1; i >= 0; i--)
        {
//            if(!bones.lLegGravity)
//            lLeg[i]->SkeletonPoint->Vector += Position - PrevPosition;

            lLeg[i]->Draw(App, Position, Mirror, DrawBones);
        }

        if(GUI::Numpad5Key() || DrawAll)
        Body[0]->Draw(App, Position, Mirror, DrawBones);

        if(GUI::Numpad6Key() || DrawAll)
        for(int i = 0; i < lArm.GetSize(); i++)
        {
//            if(!bones.lArmGravity)
//            lArm[i]->SkeletonPoint->Vector += Position - PrevPosition;

            lArm[i]->Draw(App, Position, Mirror, DrawBones);
        }

        if(GUI::Numpad5Key() || DrawAll)
        Body[2]->Draw(App, Position, Mirror, DrawBones);

        PrevPosition = Position;
        ShakingHead = false;
        TiltingHead = false;

        return Restart;
    }

    void SaveFrame(string fn)
    {
        ofstream fout;
        fout.open(fn.c_str());

        for(int i = 0; i < bones.vectors.GetSize(); i++)
        {
            fout << bones.vectors[i]->ID << " " << bones.vectors[i]->Vector.x << " " << bones.vectors[i]->Vector.y << ((i+1 == int((i+1)/5)*5)?"\n":" ");
        }

        fout.close();
    }

    void LoadFrame(string fn)
    {
        ifstream fin;
        fin.open(fn.c_str());

        if(fin.is_open())
        while(!fin.eof())
        {
            string name, xpos, ypos;
            fin >> name;

            if(bones.vectors.Find(name) != NULL)
            {
                fin >> xpos >> ypos;

                bones.vectors.Find(name)->Vector = Vector2f(StringToFloat(xpos), StringToFloat(ypos));
            }
        }

        fin.close();
    }

    void SaveAnimation(string fn)
    {
        ofstream fout;
        fout.open(fn.c_str());

        fout << "NewAnimation\n\n";

        for(int j = 0; j < Frames.GetSize(); j++)
        {
            fout << "    NewFrame\n\n        ";
            for(int i = 0; i < Frames[j]->vectors.GetSize(); i++)
            {
                fout << Frames[j]->vectors[i]->ID << " " << Frames[j]->vectors[i]->Vector.x << " " << Frames[j]->vectors[i]->Vector.y << ((i+1 == int((i+1)/5)*5)?"\n        ":" ");
            }

            fout << "Duration " << Frames[j]->Time << "\n\n    EndFrame\n\n";
        }

        fout << "EndAnimation";

        fout.close();
    }

    void LoadAnimation(string fn)
    {
        ifstream fin;
        fin.open(fn.c_str());

        if(fin.is_open())
        {
            string temp;

            fin >> temp;

            if(temp == "NewAnimation")
            {
                Frames.Clear();

                fin >> temp;

                while(temp != "EndAnimation")
                {
                    if(temp == "NewFrame")
                    {
                        AddFrame();
                        fin >> temp;

                        while(temp != "EndFrame")
                        {
                            if(temp == "Duration")
                            {
                                fin >> temp;
                                Frames[Frames.GetSize() - 1]->Time = StringToFloat(temp);
                            }
                            else
                            {
                                string name, xpos, ypos;
                                name = temp;

                                if(Frames[Frames.GetSize() - 1]->vectors.Find(name) != NULL)
                                {
                                    fin >> xpos >> ypos;

                                    Frames[Frames.GetSize() - 1]->vectors.Find(name)->Vector = Vector2f(StringToFloat(xpos), StringToFloat(ypos));
                                }
                            }

                            fin >> temp;
                        }
                    }

                    fin >> temp;
                }

                for(int i = 0; i < bones.vectors.GetSize(); i++)
                bones.vectors[i]->Vector = Frames[0]->vectors[i]->Vector;
            }
        }

        fin.close();
    }
};


#endif
