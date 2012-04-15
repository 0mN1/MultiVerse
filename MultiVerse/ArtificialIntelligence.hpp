#ifndef __ARITIFICIALINTELLIGENCE_HPP__
#define __ARITIFICIALINTELLIGENCE_HPP__

#include "AnimationSystem.hpp"

enum {AICORPPEOPLE, AIUNSUEABLE};

class ArtificialIntelligence
{
    public:
    string Name;
    Image MaskImage;
    Sprite Mask;
    Vector2f Position, PrevPosition;
    Vector2f Momentum;
    bool Direction;
};

class CorpPeople : public ArtificialIntelligence
{
    public:
    bool Unsueable;
    AnimationSystem AS;
    string File;

    CorpPeople(bool Unsueable = false) : AS(Unsueable?"Unsueable":"CorpPeople", CORPPEOPLE)
    {
        Direction = 1;

        Mask = LoadSprite(MaskImage, "Data/Image/CorpPeopleMask.png");

        Mask.SetCenter(Mask.GetSize().x/2, Mask.GetSize().y-150);

        if(Unsueable)
        {
            File = "Unsueable";
        }
        else
        {
            File = "CorpPeople";
        }
    }

    void Update()
    {

    }

    void Draw(Application & App)
    {
        AS.Position = Position;
        AS.Draw(App, Direction);
    }

    ~CorpPeople()
    {}
};

#endif
