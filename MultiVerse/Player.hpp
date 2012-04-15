#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "LevelObjects.hpp"
#include "LevelBackground.hpp"
#include "LevelScript.hpp"
#include "Skeleton.hpp"
#include "AnimationSystem.hpp"

Vector2f PPCollision2(Sprite Sprite1,Sprite Sprite2,float AlphaLimit = 50.f)
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

    if(xmax <= xmin || ymax <= ymin) { return Vector2f(-5, -5); }

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

                if(color1.a >= AlphaLimit && color2.a >= AlphaLimit)
                {
                    return Vector2f(x1, y1);
                }
            }
        }
    }

    return Vector2f(-5, -5);
}

class PlayerClass
{
    protected:
    Image pImage[2];
    Sprite pSprite[2];
    AnimationSystem AS;
    bool useSkeleton;

    bool CombatMode;

    Image MaskImage[3];
    Sprite Mask[3];
    Vector2f Position, PrevPosition;
    Vector2f Momentum;
    bool Direction;
    bool Crouching;
    bool canJump;
    String NameSign;
    bool ShowNameSign;
    bool GravityOn;
    bool drawMask;
    Vector2f MouseOrigin;
    bool EdgeGrab;
    int GrabbedEdgeBox;
    Vector2f EdgeBoxPosition;
    bool EdgeBoxFromRight;
    bool EdgeBoxHaveWall;
    bool JumpingFromEdgeBox;
    string File;

    Vector2f movingPlatformDelta;

    public:
    PlayerClass() : AS("FirstBody", CHARTYPE01)
    {
        Position = Vector2f(0.f, 0.f);
        Momentum = Vector2f(0.f, 0.f);
        Direction = 1;
        Crouching = false;
        canJump = true;
        ShowNameSign = false;
        GravityOn = true;
        useSkeleton = false;
        drawMask = false;
        EdgeGrab = false;
        JumpingFromEdgeBox = false;
        File = "FirstBody";

        CombatMode = false;
    }

    void SetName(string name) /// Used in Multiplayer
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
                Mask[0] = LoadSprite(MaskImage[0], "Data/Image/Mask.png");
            }
            else
            {
                pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy02l.png");
                pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy02r.png");
                Mask[0] = LoadSprite(MaskImage[0], "Data/Image/Mask.png");
            }
        }
        else if(characterImage == 0)
        {
            pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy01l.png");
            pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy01r.png");
            Mask[0] = LoadSprite(MaskImage[0], "Data/Image/Mask.png");
        }
        else if(characterImage == 1)
        {
            pSprite[0] = LoadSprite(pImage[0], "Data/Image/Guy02l.png");
            pSprite[1] = LoadSprite(pImage[1], "Data/Image/Guy02r.png");
            Mask[0] = LoadSprite(MaskImage[0], "Data/Image/Mask.png");
        }
        else if(characterImage == 2) /// Fix the limbs
        {
            useSkeleton = true;
            AS.AddEntity("WalkCycle", "CharType01/WalkCycle.anm", true, File);
            AS.AddEntity("SprintCycle", "CharType01/SprintCycle.anm", true, File);
            AS.AddEntity("JumpUp", "CharType01/JumpUp.anm", true, File);
            AS.AddEntity("JumpDown", "CharType01/JumpDown.anm", true, File);
            AS.AddEntity("JumpLand", "CharType01/JumpLand.anm", true, File);
            AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
            AS.AddEntity("EdgeIdle", "CharType01/EdgeIdle.anm", true, File);
            AS.AddEntity("EdgeClose", "CharType01/EdgeClose.anm", true, File);
            AS.AddEntity("EdgePush", "CharType01/EdgePush.anm", true, File);
            AS.AddEntity("EdgeIdle2", "CharType01/EdgeIdle2.anm", true, File);
            AS.AddEntity("EdgeClose2", "CharType01/EdgeClose2.anm", true, File);
            AS.AddEntity("EdgePush2", "CharType01/EdgePush2.anm", true, File);

            Mask[0] = LoadSprite(MaskImage[0], "Data/Image/Mask2.png");
            Mask[1] = LoadSprite(MaskImage[1], "Data/Image/Mask2.png");
            Mask[2] = LoadSprite(MaskImage[2], "Data/Image/Mask3.png");

            Mask[0].SetCenter(Mask[0].GetSize().x/2, Mask[0].GetSize().y-74);
            Mask[1].SetCenter(Mask[1].GetSize().x/2, Mask[1].GetSize().y-74);
            Mask[2].SetCenter(Mask[2].GetSize().x/2, Mask[2].GetSize().y-77);
        }

        for(int i = 0; i < 2; i++)
        SetCenterInCenter(pSprite[i]);
    }

    bool GetDirection()
    {
        return Direction;
    }

    void SetDirection(bool dir)
    {
        Direction = dir;
    }

    Sprite & GetMask()
    {
        return Mask[0];
    }

    Vector2f & GetPosition()
    {
        return Position;
    }

    void SetPosition(Vector2f p)
    {
        Position = p;
    }

    void ControlBody(Level * Lvl)
    {
        if(EdgeGrab)
        {
            if((((GUI::AKey.Pressed || GUI::LeftKey.Pressed) || ((GUI::AKey() || GUI::LeftKey())
            && (GUI::DKey.Released || GUI::RightKey.Released))) && EdgeBoxFromRight)
            || (((GUI::DKey.Pressed || GUI::RightKey.Pressed) || ((GUI::DKey() || GUI::RightKey())
            && (GUI::AKey.Released || GUI::LeftKey.Released))) && !EdgeBoxFromRight))
            {
                if(EdgeBoxHaveWall)
                AS.AddRequest("EdgeClose", 1);
                else
                AS.AddRequest("EdgeClose2", 1);
            }
            else
            if(((GUI::AKey() || GUI::LeftKey()) && !EdgeBoxFromRight)
            || ((GUI::DKey() || GUI::RightKey()) && EdgeBoxFromRight))
            {
                if(EdgeBoxHaveWall)
                AS.AddRequest("EdgePush", 1);
                else
                AS.AddRequest("EdgePush2", 1);
            }

            if(!GUI::AKey() && !GUI::LeftKey()
            && !GUI::DKey() && !GUI::RightKey())
            {
                if(EdgeBoxHaveWall)
                AS.AddRequest("EdgeIdle", 1);
                else
                AS.AddRequest("EdgeIdle2", 1);
            }

            if(GUI::SKey.Pressed || GUI::DownKey.Pressed)
            {
                Momentum.y = 0;

                EdgeGrab = false;
                AS.AddRequest("JumpDown", 1);
            }

            if(GUI::WKey.Pressed || GUI::UpKey.Pressed)
            {
                Momentum.y = -700;

                JumpingFromEdgeBox = true;

                if((GUI::AKey.Pressed || GUI::LeftKey.Pressed) && !EdgeBoxFromRight)
                Momentum.x = -500;

                if((GUI::DKey.Pressed || GUI::RightKey.Pressed) && EdgeBoxFromRight)
                Momentum.x = 500;

                EdgeGrab = false;
                AS.AddRequest("JumpUp", 1);
            }
        }
        else
        {
            if((GUI::AKey() || GUI::LeftKey()) && AS.Requests[0]->Id != "JumpLand")
            {
                if(GUI::LShiftKey())
                {
                    Momentum.x = -400;
                    if(canJump)
                    AS.AddRequest("SprintCycle", 1);
                }
                else
                {
                    Momentum.x = -150;
                    if(canJump)
                    AS.AddRequest("WalkCycle", 1);
                }

                Direction = 0;
            }

            if((GUI::DKey() || GUI::RightKey()) && AS.Requests[0]->Id != "JumpLand")
            {
                if(GUI::LShiftKey())
                {
                    Momentum.x = 400;
                    if(canJump || Momentum.y == 0.f)
                    AS.AddRequest("SprintCycle", 1);
                }
                else
                {
                    Momentum.x = 150;
                    if(canJump || Momentum.y == 0.f)
                    AS.AddRequest("WalkCycle", 1);
                }

                Direction = 1;
            }

            if(!GUI::DKey() && !GUI::RightKey() && !GUI::AKey() && !GUI::LeftKey())
            {
                if(Momentum.x || ((GUI::DKey.Released || GUI::RightKey.Released
                || GUI::AKey.Released || GUI::LeftKey.Released) && canJump))
                {
                    Momentum.x = 0;
                    if(AS.Requests[0]->Id != "JumpLand" && AS.Requests[0]->Id != "JumpUp" && AS.Requests[0]->Id != "JumpDown")
                    AS.AddRequest("Idle", 1);
                }
            }

            if(GUI::MKey.Pressed)
            {
                drawMask = !drawMask;
            }

            if(GUI::SKey.Pressed || GUI::DownKey.Pressed)
            {
                Crouching = true;
                Momentum.y = 100.f;
                AS.AddRequest("JumpUp", 1);
            }

            if(!GUI::SKey() && !GUI::DownKey() && Crouching)
            {
                Crouching = false;
            }

            if(GUI::SpaceKey.Pressed)
            {
                Position = Cursor.Position;
            }

            if(JumpingFromEdgeBox && (GUI::WKey.Released || GUI::UpKey.Released))
            JumpingFromEdgeBox = false;

            if(GUI::WKey.Pressed || GUI::UpKey.Pressed)
            {
                if(canJump)// || !Momentum.y)
                {
                    canJump = false;
        //            bool yMomentum = Momentum.y;
                    Momentum.y = -700;

        //            if(yMomentum)
        //            Position.y -= 2.f;

                    AS.AddRequest("JumpUp", 1);
                }
            }

            if(!JumpingFromEdgeBox && !canJump && (GUI::WKey() || GUI::UpKey()))
            {
                if(GUI::AKey() || GUI::LeftKey() || GUI::DKey() || GUI::RightKey())
                for(int i = 0; i < Lvl->EdgeBoxes.GetSize(); i++)
                {
                    if(Distance(Position+Vector2f(0, -75), Lvl->EdgeBoxes[i]->Position) < 35)
                    {
                        if(((GUI::AKey() || GUI::LeftKey()) && Lvl->EdgeBoxes[i]->fromRight)
                        || ((GUI::DKey() || GUI::RightKey()) && !Lvl->EdgeBoxes[i]->fromRight))
                        {
                            EdgeGrab = true;
                            GrabbedEdgeBox = i;
                            EdgeBoxPosition = Lvl->EdgeBoxes[i]->Position;
                            EdgeBoxFromRight = Lvl->EdgeBoxes[i]->fromRight;
                            EdgeBoxHaveWall = Lvl->EdgeBoxes[i]->hasWall;
                            if(EdgeBoxHaveWall)
                            AS.AddRequest("EdgeIdle", 1);
                            else
                            AS.AddRequest("EdgeIdle2", 1);
                            Momentum = Vector2f(0, 0);
                            Position.y = EdgeBoxPosition.y + 75;
                        }
                    }
                }
            }

            if(!canJump && Momentum.y > 0)
            {
                AS.AddRequest("JumpDown", 1);
            }
        }
    }

    void ControlHeadAndArms()
    {
        if(GUI::TabKey.Pressed)
        CombatMode = !CombatMode;

        if(CombatMode)
        {
            AS.CombatMode = true;
            AS.CalculateMouseRelativity(Cursor.Position);
            AS.MoveArms();
        }
        else
        {
            AS.CombatMode = false;
            /// Tilt Head
            if(Cursor.Left.Pressed)
            {
                MouseOrigin.y = Cursor.RelativePosition.y;
                AS.TiltingHead = true;
            }

            if(Cursor.Left())
            {
                float Delta = Cursor.RelativePosition.y - MouseOrigin.y;

                Delta /= 5.f;

                if(Delta > 30.f)
                {
                    MouseOrigin.y += Delta - 30.f;
                    Delta = 30.f;
                }
                else if(Delta < -20.f)
                {
                    MouseOrigin.y += Delta + 20.f;
                    Delta = -20.f;
                }

                AS.HeadTiltAmount = Delta;
            }

            if(Cursor.Left.Released)
            {
                AS.TiltingHead = false;
            }

            /// Shake Head

            if(Cursor.Right.Pressed)
            {
                MouseOrigin.x = Cursor.Position.x;
                AS.ShakingHead = true;
            }

            if(Cursor.Right())
            {
                float Delta = Cursor.Position.x - MouseOrigin.x;
                Delta = (Delta)/400.f;

                if(Delta < -1)
                Delta = -1.f;
                else if(Delta > 1)
                Delta = 1.f;

                AS.HeadShakeAmount = 1-Delta;
                /// Mattias får rita att huvudet vrider på sig istället...
            }

            if(Cursor.Right.Released)
            {
                AS.ShakingHead = false;
            }
        }
    }

    void PlatformInteraction(Application & App, Level * Lvl)
    {
        if(!EdgeGrab)
        {
            movingPlatformDelta = Vector2f(0, 0);

            Position.x += Momentum.x * App.GetFrameTime();

            Mask[Crouching].SetPosition(Position);
            Mask[2].SetPosition(Position);

            int indent = -5;

            Vector2f CollisionPosition;

            for(int i = 0; i < Lvl->Solids.GetSize(); i++)
            {
                ScriptObject< Solid > * solid = Lvl->Solids[i];

                if(solid->Object.Collision)
                {
                    if(/*Momentum.x && */SimpleCollision(Mask[2], solid->Object.sprite.sprite))
                    {
                        bool collision = false;
                        int spot = 0;
                        List< int > Spots;

                        for(int j = ((int)Mask[0].GetSize().x)/2 + indent - 1; j >= 0; j--)
                        {
                            if(j != 0 && PPCollision(Vector2f(Position.x - j, Position.y + 75), solid->Object.sprite.sprite))
                            {
                                collision = true;
                                spot = -j;
                                Spots.AddNode(-j);
                            }

                            if(PPCollision(Vector2f(Position.x + j, Position.y + 75), solid->Object.sprite.sprite))
                            {
                                collision = true;
                                spot = j;
                                Spots.AddNode(j);
                            }
                        }

                        if(collision)
                        {
        //                    cout << spot << " ";

                            movingPlatformDelta.x += solid->Object.Momentum.x * App.GetFrameTime();
                            movingPlatformDelta.y += solid->Object.Momentum.y * App.GetFrameTime();

                            bool flag = false;
                            int height = 0;

                            // Collision spot
                            for(int j = 0; j <= 7; j += 1)
                            {
                                bool emptyLayer = true;
                                for(int n = 0; n < Spots.GetSize(); n++)
                                {
                                    if(PPCollision(Vector2f(Position.x + *Spots[n], Position.y + (75 - j)), solid->Object.sprite.sprite))
                                    {
                                        emptyLayer = false;
                                        break;
                                    }
                                }

                                if(emptyLayer)
                                {
                                    flag = true;
                                    //if(Momentum.x != 0) // TemporaryFix
                                    height = j-1;
                                    break;
                                }

                                if(j == 7)
                                {
//                                    Position.x = PrevPosition.x; isolate
        //                            cout << "0" << endl;
                                }
                            }

                            /*// Left
                            for(int j = 7; j >= 0; j -= 1)
                            {
                                if(PPCollision(Vector2f(Position.x - 13, Position.y + (75 - j)), solid->Object.sprite.sprite))
                                {
                                    if(j > height)
                                    {
                                        flag = true;
                                        height = j;
                                        break;
                                    }
                                }
                            }

                            // Right
                            for(int j = 7; j >= 0; j -= 1)
                            {
                                if(PPCollision(Vector2f(Position.x + 13, Position.y + (75 - j)), solid->Object.sprite.sprite))
                                {
                                    if(j > height)
                                    {
                                        flag = true;
                                        height = j;
                                        break;
                                    }
                                }
                            }*/

                            if(flag && Momentum.y >= -1.f)
                            {
        //                        cout << height << endl;

                                if(drawMask)
                                {
                                    for(int n = 0; n < Spots.GetSize(); n++)
                                    App().Draw(Shape::Line(Position.x + *Spots[n], Position.y + 76,
                                    Position.x + *Spots[n], Position.y + 75 - height, 2, Color(255, 0, 0)));
                                }

                                Position.y -= height;//-1;// - (Momentum.x ? 0 : 1.f);

                                if((GUI::WKey.Pressed || GUI::UpKey.Pressed
                                || GUI::SpaceKey.Pressed))
                                ;
                                else
                                {
                                    if(!canJump)
                                    {
                                        canJump = true;

                                        if(Momentum.y > 510.f)
                                        {
                                            AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
                                            AS.AddRequest("JumpLand", 1);
                                            AS.AddRequest("Idle", 2);
                                            Momentum.x *= 0.1;

        //                                    cout << Momentum.y << " ";
                                        }
                                        else if(Momentum.y > 400.f)
                                        {
                                            AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 5.f;
                                            AS.AddRequest("JumpLand", 1);
                                            AS.AddRequest("Idle", 2);
                                            Momentum.x *= 0.5;

        //                                    cout << Momentum.y << " ";
                                        }
                                        else
                                        {
                                            AS.AddRequest("Idle", 1);
                                        }
                                    }
                                    else
                                    {
                                        if(Momentum.y > 750.f)
                                        {
                                            AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
                                            AS.AddRequest("JumpLand", 1);
                                            AS.AddRequest("Idle", 2);
                                            Momentum.x *= 0.1;
                                        }
                                    }

                                    Momentum.y = 0.f;
                                }

                                if(PPCollision(Mask[Crouching], solid->Object.sprite.sprite))
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
                        if((CollisionPosition = PPCollision2(Mask[Crouching], solid->Object.sprite.sprite)) != Vector2f(-5, -5))
                        {
                            if((CollisionPosition.x > Mask[Crouching].GetSize().x/2 && Momentum.x > 0)
                            || (CollisionPosition.x < Mask[Crouching].GetSize().x/2 && Momentum.x < 0))
                            {
                                Position.x = PrevPosition.x;
                                Momentum.x = 0.f;
                            }
                        }

        //                if(Momentum.y > 0)
        //                {
        //                    if((CollisionPosition = PPCollision2(Mask[Crouching], solid->Object.sprite.sprite)) != Vector2f(-5, -5)
        //                    &&  CollisionPosition.y > Mask[Crouching].GetSize().y - 10)
        //                    {
        //                        Position.y -= Mask[Crouching].GetSize().y - CollisionPosition.y;
        //
        //                        if(CollisionPosition.x > Mask[Crouching].GetSize().x/2)
        //                        {
        //                            Position.x -= Mask[Crouching].GetSize().x - CollisionPosition.x + 1;
        //                        }
        //                        else if(CollisionPosition.x < Mask[Crouching].GetSize().x/2)
        //                        {
        //                            Position.x += CollisionPosition.x + 1;
        //                        }
        //
        //                    }
        //                }
                    }
                }
            }

            movingPlatformDelta.y *= 1.001;
            Position += movingPlatformDelta;

            Position.y += Momentum.y * App.GetFrameTime();

            Mask[2].SetPosition(Position);
            Mask[Crouching].SetPosition(Position);

            for(int i = 0; i < Lvl->Solids.GetSize(); i++)
            {
                ScriptObject< Solid > * solid = Lvl->Solids[i];

                if(solid->Object.Collision)
                {
                    if(SimpleCollision(Mask[Crouching], solid->Object.sprite.sprite)
                    && (CollisionPosition = PPCollision2(Mask[Crouching], solid->Object.sprite.sprite)) != Vector2f(-5, -5))
                    {
                        if(Momentum.y > 0)
                        {
                            /*if(CollisionPosition.x > Mask[Crouching].GetSize().x/2)
                            {
                                Position.x -= Mask[Crouching].GetSize().x - CollisionPosition.x + 1;
                            }
                            else if(CollisionPosition.x < Mask[Crouching].GetSize().x/2)
                            {
                                Position.x += CollisionPosition.x + 1;
                            }*/

                            if(!canJump)
                            {
                                canJump = true;

                                if(Momentum.y > 510.f)
                                {
                                    AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
                                    AS.AddRequest("JumpLand", 1);
                                    AS.AddRequest("Idle", 2);
                                    Momentum.x *= 0.1;

        //                                    cout << Momentum.y << " ";
                                }
                                else if(Momentum.y > 400.f)
                                {
                                    AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 5.f;
                                    AS.AddRequest("JumpLand", 1);
                                    AS.AddRequest("Idle", 2);
                                    Momentum.x *= 0.5;

        //                                    cout << Momentum.y << " ";
                                }
                                else
                                {
                                    AS.AddRequest("Idle", 1);
                                }
                            }
                            else
                            {
                                if(Momentum.y > 750.f)
                                {
                                    AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
                                    AS.AddRequest("JumpLand", 1);
                                    AS.AddRequest("Idle", 2);
                                    Momentum.x *= 0.1;
                                }
                            }

                            Position.y = PrevPosition.y;
                            Momentum.y = 0.f;
                        }

                        /*if(Momentum.y > 0)
                        {
                            if(!canJump)
                            {
                                canJump = true;

                                if(Momentum.y > 510.f)
                                {
                                    AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
                                    AS.AddRequest("JumpLand", 1);
                                    AS.AddRequest("Idle", 2);
                                    Momentum.x *= 0.1;
                                }
                                else if(Momentum.y > 300.f)
                                {
                                    AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 5.f;
                                    AS.AddRequest("JumpLand", 1);
                                    AS.AddRequest("Idle", 2);
                                    Momentum.x *= 0.5;
                                }
                            }
                            cout << Momentum.y << " ";
                        }

                        if(Momentum.y > 750.f)
                        {
                            AS.Entities.Find("JumpLand")->skeleton.PlaybackSpeed = 2.f;
                            AS.AddRequest("JumpLand", 1);
                            AS.AddRequest("Idle", 2);
                            Momentum.x *= 0.1;

                            cout << Momentum.y << " ";
                        }*/

                        if(Momentum.y < 0)
                        //&& PPCollision(Vector2f(Position.x, Position.y + 57), Ob.GetSprite()))
                        {
                            Position.y = PrevPosition.y;
                            Momentum.y = 0.f;
                        }
                    }
                }
            }
        }
    }

    void Update(Application & App, Level * Lvl)
    {
        /// Collect User Input Trigger Actions and Animations
        ControlBody(Lvl);

        /// Nod and Shake Head
        ControlHeadAndArms();

        /// Turn Gravity On/Off
        if(GUI::GKey.Pressed)
        {
            GravityOn = !GravityOn;
        }

        /// Add Gravity to Momentum
        if(GravityOn)
        Momentum.y += Lvl->Gravity * App.GetFrameTime();

        /// Calculate Movement and Platform Interaction
        PlatformInteraction(App, Lvl);

        /// Save the Players Current Position
        PrevPosition = Position;

        /// Center Player In View
        MoveView(App().GetDefaultView(), Lvl);
    }

    void Predict(Application & App) /// Used in Multiplayer
    {
        PrevPosition = Position;

        Position.x += Momentum.x * App.GetFrameTime();
        Position.y += Momentum.y * App.GetFrameTime();
    }

    void Predict(Application & App, Level * Lvl) /// Used in Multiplayer
    {
        PrevPosition = Position;

        Position.x += Momentum.x * App.GetFrameTime();

        Mask[Crouching].SetPosition(Position);

        for(int i = 0; i < Lvl->Solids.GetSize(); i++)
        {
            if(Momentum.x && SimpleCollision(Mask[Crouching], Lvl->Solids[i]->Object.sprite.sprite))
            {
                bool collision = false;
                int spot = 0;

                for(int j = 0; j < Mask[Crouching].GetSize().x/2 + 1; j++)
                {
                    if(PPCollision(Vector2f(Position.x - j, Position.y + 125), Lvl->Solids[i]->Object.sprite.sprite))
                    {
                        collision = true;
                        spot = -j;
                        break;
                    }

                    if(PPCollision(Vector2f(Position.x + j, Position.y + 125), Lvl->Solids[i]->Object.sprite.sprite))
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
                        if(!PPCollision(Vector2f(Position.x + spot, Position.y + (125 - j)), Lvl->Solids[i]->Object.sprite.sprite))
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

                        if(PPCollision(Mask[Crouching], Lvl->Solids[i]->Object.sprite.sprite))
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
                if(PPCollision(Mask[Crouching], Lvl->Solids[i]->Object.sprite.sprite))
                {
                    Position.x = PrevPosition.x;
                    Momentum.x = 0.f;
                }
            }
        }

        if(GravityOn)
        Momentum.y += 982.f * App.GetFrameTime();

        Position.y += Momentum.y * App.GetFrameTime();

        Mask[Crouching].SetPosition(Position);

        for(int i = 0; i < Lvl->Solids.GetSize(); i++)
        {
            if(SimpleCollision(Mask[Crouching], Lvl->Solids[i]->Object.sprite.sprite)
            && PPCollision(Mask[Crouching], Lvl->Solids[i]->Object.sprite.sprite))
            {
                if(Momentum.y > 0)
                canJump = true;

                if(Momentum.y)
                //&& PPCollision(Vector2f(Position.x, Position.y + 57), Ob.GetSprite()))
                Position.y = PrevPosition.y;
                Momentum.y = 0.f;
            }
        }
    }

    void Draw(Application & App)
    {
        if(useSkeleton)
        {
            if(drawMask)
            {
                App().Draw(Mask[2]);
                App().Draw(Mask[Crouching]);
            }

            if(drawMask)
            {
                App().Draw(Shape::Line(Position.x - (Mask[Crouching].GetSize().x/2 + -5), Position.y + 75,
                Position.x + (Mask[Crouching].GetSize().x/2 + -5), Position.y + 75, 1, Color(255, 255, 255)));
            }
            AS.Position = Position;
            AS.Draw(App, Direction);
        }
        else
        {
            //if(Position == PrevPosition)
            pSprite[Direction].SetPosition(Position);
            //else
            //pSprite[Direction].SetPosition(PrevPosition);

            App().Draw(pSprite[Direction]);
        }

        if(ShowNameSign)
        {
            NameSign.SetPosition(Position + Vector2f(0, -75));
            App().Draw(NameSign);
        }
    }

    void MoveView(View & view, Level * Lvl, Vector2f CamPos = Vector2f(0.f, 0.f))
    {
        /*if(Position.x > view.GetRect().Right - view.GetHalfSize().x * 0.5)
        {
            lBg.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 0.5), 0);
            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 0.5), 0);
        }
        else
        if(Position.x < view.GetRect().Right - view.GetHalfSize().x * 1.5)
        {
            lBg.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 1.5), 0);
            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x * 1.5), 0);
        }

        if(Position.y > view.GetRect().Bottom - view.GetHalfSize().y * 0.5)
        {
            lBg.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 0.5));
            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 0.5));
        }
        else
        if(Position.y < view.GetRect().Bottom - view.GetHalfSize().y * 1.5)
        {
            lBg.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 1.5));
            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y * 1.5));
        }*/

        if(Position.x > view.GetRect().Right - view.GetHalfSize().x + CamPos.x)
        {
            Lvl->MoveView(Position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
        }
        else
        if(Position.x < view.GetRect().Right - view.GetHalfSize().x + CamPos.x)
        {
            Lvl->MoveView(Position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
            view.Move(Position.x - (view.GetRect().Right - view.GetHalfSize().x + CamPos.x), 0);
        }

        if(Position.y > view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y)
        {
            Lvl->MoveView(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
        }
        else
        if(Position.y < view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y)
        {
            Lvl->MoveView(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
            view.Move(0, Position.y - (view.GetRect().Bottom - view.GetHalfSize().y + CamPos.y));
        }
    }

    friend class CharacterData;
};

#endif
